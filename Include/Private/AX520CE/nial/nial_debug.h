/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NIAL_DEBUG_H_
#define _NIAL_DEBUG_H_

/**
 * 函数名: nial_printk
 * 描述: 刷新所有cache
 * 输入: 参数       描述
 *      fmt         待打印数据
 *      ...         可变参数
 * 返回: 返回值     描述
 *      val         完成打印的数量
 */
int nial_printk(const char *fmt, ...);

/**
 * 函数名: nial_panic
 * 描述: 对系统发生错误时的信息收集
 * 输入: 参数       描述
 *      fmt         待打印数据
 *      file        发生所在文件
 *      func        发生时所在函数
 *      line        发生时所在行
 *      cond        发生时条件
 * 返回: 返回值     描述
 *      无
 */
void nial_panic(const char *fmt, const char *file, const char *fun, int line, const char *cond);

/**
 * 函数名: nial_vprintk
 * 描述: 刷新所有cache
 * 输入: 参数       描述
 *      fmt         待打印数据
 *      args        参数列表
 * 返回: 返回值     描述
 *      val         完成打印的数量
 */
int nial_vprintk(const char *fmt, va_list args);

#define NIAL_BUG() \
    do {           \
    } while (1)

#define NIAL_ASSERT(expr)               \
    do {                                \
        if (!(expr)) {                  \
            nial_printk(                \
                "\nASSERT failed at:\n" \
                "  >Condition: %s\n",   \
                #expr);                 \
            NIAL_BUG();                 \
        }                               \
    } while (0)

#define NIAL_BUG_ON(expr)                                                               \
    do {                                                                                \
        if (expr) {                                                                     \
            nial_printk("BUG: failure at %s:%d/%s()!\n", __FILE__, __LINE__, __func__); \
            NIAL_BUG();                                                                 \
        }                                                                               \
    } while (0)

#endif /* _NIAL_DEBUG_H_ */
