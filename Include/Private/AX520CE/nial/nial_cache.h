/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NIAL_CACHE_H_
#define _NIAL_CACHE_H_

/**
 * 函数名: nial_flush_cache_all
 * 描述: 刷新所有cache
 * 输入: 参数       描述
 *      无
 * 返回: 返回值     描述
 *      无
 */
void nial_flush_cache_all(void);

/**
 * 函数名: nial_flush_dcache_area
 * 描述: 将指定地址的cache刷新
 * 输入: 参数       描述
 *      kvirt       内核虚拟空间地址
 *      phys_addr   物理地址
 *      size        大小
 * 返回: 返回值     描述
 *      无
 */
void nial_flush_dcache_area(void *kvirt, unsigned long phys_addr, unsigned long size);

/**
 * 函数名:
 * 描述: 刷新所有数据cache
 * 输入: 参数       描述
 *      无
 * 返回: 返回值     描述
 *      无
 */
int nial_flush_dcache_all(void);

#endif /* _NIAL_CACHE_H_ */
