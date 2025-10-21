/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NIAL_BARRIER_H_
#define _NIAL_BARRIER_H_

/**
 * 函数名: nial_mb
 * 描述: 保证前方就近读写操作指令有序
 * 输入: 参数       描述
 *      无
 * 返回: 返回值     描述
 *      无
 */
void nial_mb(void);

/**
 * 函数名: nial_rmb
 * 描述: 保证前方就近读操作指令有序
 * 输入: 参数       描述
 *      无
 * 返回: 返回值     描述
 *      无
 */
void nial_rmb(void);

/**
 * 函数名: nial_wmb
 * 描述: 保证前方就近写操作指令有序
 * 输入: 参数       描述
 *      无
 * 返回: 返回值     描述
 *      无
 */
void nial_wmb(void);

/**
 * 函数名: nial_smp_mb
 * 描述: 多核处理器中，保证就近读写操作指令有序
 * 输入: 参数       描述
 *      无
 * 返回: 返回值     描述
 *      无
 */
void nial_smp_mb(void);

/**
 * 函数名: nial_smp_rmb
 * 描述: 多核处理器中，保证就近读操作指令有序
 * 输入: 参数       描述
 *      无
 * 返回: 返回值     描述
 *      无
 */
void nial_smp_rmb(void);

/**
 * 函数名: nial_smp_wmb
 * 描述: 多核处理器中，保证就近写操作指令有序
 * 输入: 参数       描述
 *      无
 * 返回: 返回值     描述
 *      无
 */
void nial_smp_wmb(void);

/**
 * 函数名: nial_isb
 * 描述: 指令同步
 * 输入: 参数       描述
 *      无
 * 返回: 返回值     描述
 *      无
 */
void nial_isb(void);

/**
 * 函数名: nial_dsb
 * 描述: 数据同步
 * 输入: 参数       描述
 *      无
 * 返回: 返回值     描述
 *      无
 */
void nial_dsb(void);

/**
 * 函数名: nial_dmb
 * 描述: 数据存储屏障
 * 输入: 参数       描述
 *      无
 * 返回: 返回值     描述
 *      无
 */
void nial_dmb(void);

#endif
