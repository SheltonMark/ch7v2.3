/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef __NI_DEBUG_H__
#define __NI_DEBUG_H__

#ifndef __KERNEL__
#include <stdarg.h>
#endif

#include "ni_type.h"
#include "ni_common.h"
#include "ni_errno.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/* 打印字体显示的颜色 */
#ifdef __KERNEL__
#define NONE_C ""
#else
#define NONE_C "\033[0m"
#endif

#define RED        "\033[0;32;31m"
#define GREEN      "\033[0;32;32m"
#define BLUE       "\033[0;32;34m"
#define DARY_GRAY  "\033[1;30m"
#define CYAN       "\033[0;36m"
#define PURPLE     "\033[0;35m"
#define BROWN      "\033[0;33m"
#define YELLOW     "\033[1;33m"
#define WHITE      "\033[1;37m"
#define LIGHT_GRAY "\033[0;37m"
#define LIGHT_CYAN "\033[1;36m"

#define _EX__FILE_LINE(fxx, lxx) "[File]:" fxx "\n[Line]:" #lxx "\n[Info]:"
#define EX__FILE_LINE(fxx, lxx)  _EX__FILE_LINE(fxx, lxx)
#define __FILE_LINE__            EX__FILE_LINE(__FILE__, __LINE__)

#define NI_DBG_EMERG  0 /* system is unusable                   */
#define NI_DBG_ALERT  1 /* action must be taken immediately     */
#define NI_DBG_CRIT   2 /* critical conditions                  */
#define NI_DBG_ERR    3 /* error conditions                     */
#define NI_DBG_WARN   4 /* warning conditions                   */
#define NI_DBG_NOTICE 5 /* normal but significant condition     */
#define NI_DBG_INFO   6 /* informational                        */
#define NI_DBG_DEBUG  7 /* debug-level messages                 */

#ifndef __KERNEL__
#define NI_PRINT printf
#else
#define NI_PRINT nial_printk
#endif

#ifdef CONFIG_NI_LOG_TRACE_SUPPORT

#ifndef __KERNEL__
/******************************************************************************
** For User Mode : NI_PRINT, NI_ASSERT, NI_TRACE
******************************************************************************/

/* Using samples:   NI_ASSERT(x>y); */
#define NI_ASSERT(expr)                                   \
    do {                                                  \
        if (!(expr)) {                                    \
            printf(                                       \
                "\nASSERT failed at:\n"                   \
                "  >File name: %s\n"                      \
                "  >Function : %s\n"                      \
                "  >Line No. : %d\n"                      \
                "  >Condition: %s\n",                     \
                __FILE__, __FUNCTION__, __LINE__, #expr); \
        }                                                 \
    } while (0)

/** Using samples:
 * NI_TRACE(NI_DBG_DEBUG, NI_ID_VI, "Test %d, %s\n", 12, "Test");
 */
#define NI_TRACE NI_MDK_SYS_LogWrite

#else
/******************************************************************************
** For Linux Kernel : NI_PRINT, NI_ASSERT, NI_TRACE
******************************************************************************/
extern int    printk(const char *fmt, ...);
extern NI_S32 NI_ChkLogLevel(NI_S32 s32Level, MOD_ID_E enModId);
extern int    NI_LOG(NI_S32 s32Level, MOD_ID_E enModId, const char *fmt, ...);

/* Using samples:   NI_ASSERT(x>y); */
#define NI_ASSERT(expr)                                   \
    do {                                                  \
        if (!(expr)) {                                    \
            nial_panic(                                   \
                "\nASSERT failed at:\n"                   \
                "  >File name: %s\n"                      \
                "  >Function : %s\n"                      \
                "  >Line No. : %d\n"                      \
                "  >Condition: %s\n",                     \
                __FILE__, __FUNCTION__, __LINE__, #expr); \
        }                                                 \
    } while (0)

/** Using samples:
 * NI_TRACE(NI_DBG_DEBUG, NI_ID_VI, "Test %d, %s\n", 12, "Test");
 */
#define NI_TRACE NI_LOG

#endif /* end of __KERNEL__ */

/* 调试打印，输出字体为紫色。*/
#define NI_DBG(fmt, args...) NI_PRINT(PURPLE "DEBUG (%s|%d): " fmt NONE_C, __func__, __LINE__, ##args)

/* 出错打印，输出字体为红色，打印出函数名和行号,表明程序不能继续运行。*/
#define NI_ERROR(fmt, args...) NI_PRINT(RED "ERROR (%s|%d): " fmt NONE_C, __func__, __LINE__, ##args)

/* 警告打印，输出字体为黄色，打印出函数名和行号。表明程序仍可继续运行，但须警示。*/
#define NI_WARN(fmt, args...) NI_PRINT(YELLOW "WARN (%s|%d): " fmt NONE_C, __func__, __LINE__, ##args)

/* 信息通告打印，输出字体为绿色。*/
#define NI_INFO(fmt, args...) NI_PRINT(GREEN "INFO (%s|%d): " fmt NONE_C, __func__, __LINE__, ##args)

#else

#define NI_ASSERT(expr)
#define NI_TRACE(level, enModId, fmt...)
#define NI_DBG(fmt, args...)
#define NI_ERROR(fmt, args...)
#define NI_WARN(fmt, args...)
#define NI_INFO(fmt, args...)

#endif /* end of CONFIG_NI_LOG_TRACE_SUPPORT */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __NI_DEBUG_H__ */
