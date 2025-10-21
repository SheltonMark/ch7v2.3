/******************************************************************************

  Copyright (C), 2011-2016, ZheJiang Chipup Technology CO.,LTD.

 ******************************************************************************
  File Name      : api_venc.h
  Version          : Initial Draft
  Author           : Lv Zhuqing
  Created         : 2014.2.28
  Last Modified :
  Description    : all struct and maro definition for debug
  Function List  :
  DHstory        :
  1.Date          : 2014/2/28
    Author        : 24497
    Modification: Create
******************************************************************************/

#ifndef __NI_DEBUG_H__
#define __NI_DEBUG_H__

#ifndef __KERNEL__
#include <stdarg.h>
#endif

#include "ni_common.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/* 打印字体显示的颜色 */
#ifdef __KERNEL__
#ifndef __UYOS__
#include <linux/version.h>

    #if LINUX_VERSION_CODE < KERNEL_VERSION(4,19,132)
        #define NONE_C        "\033[0m"
    #else
        /* to avoid printk log delay when use NI_ERROR in high linux version */
        #define NONE_C        ""
    #endif
#else
	#define NONE_C		  ""
#endif
#else
    #define NONE_C        "\033[0m"
#endif

#define RED           "\033[0;32;31m"
#define GREEN         "\033[0;32;32m"
#define BLUE          "\033[0;32;34m"
#define DARY_GRAY     "\033[1;30m"
#define CYAN          "\033[0;36m"
#define PURPLE        "\033[0;35m"
#define BROWN         "\033[0;33m"
#define YELLOW        "\033[1;33m"
#define WHITE         "\033[1;37m"

#define _EX__FILE_LINE(fxx,lxx) "[File]:"fxx"\n[Line]:"#lxx"\n[Info]:"
#define EX__FILE_LINE(fxx,lxx) _EX__FILE_LINE(fxx,lxx)
#define __FILE_LINE__ EX__FILE_LINE(__FILE__, __LINE__)

#define NI_DBG_EMERG      0   /* system is unusable                   */
#define NI_DBG_ALERT      1   /* action must be taken immediately     */
#define NI_DBG_CRIT       2   /* critical conditions                  */
#define NI_DBG_ERR        3   /* error conditions                     */
#define NI_DBG_WARN       4   /* warning conditions                   */
#define NI_DBG_NOTICE     5   /* normal but significant condition     */
#define NI_DBG_INFO       6   /* informational                        */
#define NI_DBG_DEBUG      7   /* debug-level messages                 */

typedef struct tagLogLevelConf
{
    MOD_ID_E  enModId;
    NI_S32    s32Level;
    NI_CHAR   szModName[16];
    NI_S32    s32Print2serial;
} LOG_LEVEL_CONF_S;

#ifndef __KERNEL__ 
/******************************************************************************
** For User Mode : NI_PRINT, NI_ASSERT, NI_TRACE
******************************************************************************/

#define NI_PRINT printf

#if 1
    /* Using samples:   NI_ASSERT(x>y); */
    #define NI_ASSERT(expr)               \
    do{                                   \
        if (!(expr)) {                    \
            printf("\nASSERT failed at:\n"\
                   "  >File name: %s\n"   \
                   "  >Function : %s\n"   \
                   "  >Line No. : %d\n"   \
                   "  >Condition: %s\n",  \
                    __FILE__,__FUNCTION__, __LINE__, #expr);\
        } \
    }while(0)

    //#define NI_TRACE(level, enModId, fmt...) fprintf(stderr,##fmt)
    #define NI_TRACE NI_MDK_SYS_LogWrite
#else
    #define NI_ASSERT(expr)
    #define NI_TRACE(level, enModId, fmt...)
#endif

#else
/******************************************************************************
** For Linux Kernel : NI_PRINT, NI_ASSERT, NI_TRACE
******************************************************************************/
extern int printk(const char *fmt, ...);
#define NI_PRINT printk

extern NI_S32 NI_ChkLogLevel(NI_S32 s32Level, MOD_ID_E enModId);
//extern asmlinkage int NI_LOG(NI_S32 s32Level, MOD_ID_E enModId,const char *fmt, ...);
extern int NI_LOG(NI_S32 s32Level, MOD_ID_E enModId,const char *fmt, ...);


#if 1
    /* Using samples:   NI_ASSERT(x>y); */
    #define NI_ASSERT(expr)               \
    do{                                   \
        if (!(expr)) {                    \
            panic("\nASSERT failed at:\n" \
                  "  >File name: %s\n"    \
                  "  >Function : %s\n"    \
                  "  >Line No. : %d\n"    \
                  "  >Condition: %s\n",   \
                    __FILE__,__FUNCTION__, __LINE__, #expr);\
        } \
    }while(0)

    /* Using samples: 
    ** NI_TRACE(NI_DBG_DEBUG, NI_ID_VI, "Test %d, %s\n", 12, "Test");
    **/
    #define NI_TRACE NI_LOG
#else
    #define NI_ASSERT(expr)
    #define NI_TRACE(level, enModId, fmt...)
#endif

#endif  /* end of __KERNEL__ */

#ifdef _NI_DEBUG_
#define NI_handleCheck(handle) \
    do \
    { \
        if(NULL == (handle)) \
        { \
            NI_ERROR("NULL handle\n"); \
            return NI_FAILURE; \
        } \
        \
        if(NI_MAGIC_NUM != (handle)->nMgicNum) \
        { \
            NI_ERROR("Invalid Magic Num : %d\n", (handle)->nMgicNum); \
            return NI_FAILURE; \
        } \
    } \
    while(0)
#else

#define NI_handleCheck(handle)

#endif

/*=========================================================================================
 ***************************************调试打印******************************************
==========================================================================================*/

/* 调试打印，输出字体为紫色。*/
#define NI_DBG(fmt, args ...) \
	do \
	{ \
		NI_PRINT(PURPLE "DEBUG (%s|%d): " fmt NONE_C, __func__, __LINE__, ## args); \
	} \
	while(0)

/* 出错打印，输出字体为红色，打印出文件名、函数名和行号,表明程序不能继续运行。*/
#define NI_ERROR(fmt, args ...) \
    do \
    { \
    	NI_PRINT(RED "ERROR (%s|%d): " fmt NONE_C, __func__, __LINE__, ## args); \
    } \
    while(0)

/* 警告打印，输出字体为黄色，打印出文件名、函数名和行号。表明程序仍可继续运行，但须警示。*/
#define NI_WARN(fmt, args ...) \
	do \
	{ \
		NI_PRINT(YELLOW "WARN (%s|%d): " fmt NONE_C, __func__, __LINE__, ## args); \
	} \
	while(0)

/* 信息通告打印，输出字体为绿色。*/
#define NI_INFO(fmt, args ...) \
	do \
	{ \
		NI_PRINT(GREEN "INFO (%s|%d): " fmt NONE_C, __func__, __LINE__, ## args); \
	} \
	while(0)



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __NI_DEBUG_H__ */

