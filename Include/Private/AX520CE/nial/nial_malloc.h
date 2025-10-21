/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NIAL_MALLOC_H_
#define _NIAL_MALLOC_H_

#define NIAL_GFP_KERNEL 0
#define NIAL_GFP_ATOMIC 1
#define NIAL_GFP_DMA    3

/**
 * 函数名: nial_vmalloc
 * 描述: 空间申请，物理地址不一定连续
 * 输入: 参数       描述
 *      size       申请空间大小
 * 返回: 返回值     描述
 *      virt_addr   申请的虚拟空间首地址
 */
void *nial_vmalloc(unsigned long size);

/**
 * 函数名: nial_vzalloc
 * 描述: 空间申请并初始化为0，物理地址不一定连续
 * 输入: 参数       描述
 *      size       申请空间大小
 * 返回: 返回值     描述
 *      virt_addr   申请的虚拟空间首地址
 */
void *nial_vzalloc(unsigned long size);

/**
 * 函数名: nial_vfree
 * 描述: 对申请的空间进行释放
 * 输入: 参数       描述
 *      addr       申请空间的虚拟地址
 * 返回: 返回值     描述
 *      无
 */
void nial_vfree(const void *addr);

/**
 * 函数名: nial_kmalloc
 * 描述: 空间申请，物理地址连续
 * 输入: 参数       描述
 *      size       申请空间大小
 *      nial_gfp_flag   分配空间的类型
 * 返回: 返回值     描述
 *      virt_addr   申请的虚拟空间首地址
 */
void *nial_kmalloc(unsigned long size, unsigned int nial_gfp_flag);

/**
 * 函数名: nial_kzalloc
 * 描述: 空间申请并初始化为0，物理地址连续
 * 输入: 参数       描述
 *      size       申请空间大小
 *      nial_gfp_flag   分配空间的类型
 * 返回: 返回值     描述
 *      virt_addr   申请的虚拟空间首地址
 */
void *nial_kzalloc(unsigned long size, unsigned int nial_gfp_flag);

/**
 * 函数名: nial_kfree
 * 描述: 对申请空间进行释放
 * 输入: 参数       描述
 *      addr       申请空间的虚拟地址
 * 返回: 返回值     描述
 *      无
 */
void nial_kfree(const void *addr);

/**
 * 函数名: nial_get_order
 * 描述: 获取page页编号
 * 输入: 参数       描述
 *      size       页大小
 * 返回: 返回值     描述
 *      num         此大小页所占编号
 */
int nial_get_order(unsigned long size);

/**
 * 函数名: nial_alloc_pages
 * 描述: 完成page页的分配
 * 输入: 参数       描述
 *      dev         设备
 *      nial_gfp_flag   分配标志
 *      order           对应页面编号
 * 返回: 返回值     描述
 *      0           失败
 *      >0          成功分配后的地址(非物理)
 *      <0          错误码
 */
unsigned long nial_alloc_pages(void *dev, unsigned int nial_gfp_flag, int order);

/**
 * 函数名: nial_zalloc_pages
 * 描述: 完成page页的分配，并初始化为0
 * 输入: 参数       描述
 *      dev         设备
 *      nial_gfp_flag   分配标志
 *      order           对应页面编号
 * 返回: 返回值     描述
 *      0           失败
 *      >0          成功分配后的地址(非物理)
 *      <0          错误码
 */
unsigned long nial_zalloc_pages(void *dev, unsigned int nial_gfp_flag, int order);

/**
 * 函数名: nial_free_pages
 * 描述: 对申请的page空间进行释放
 * 输入: 参数       描述
 *      dev         设备
 *      nial_gfp_flag   分配page页的地址
 *      order           对应页面编号
 * 返回: 返回值     描述
 *      无
 */
void nial_free_pages(void *dev, unsigned long pa, int order);

/**
 * 函数名: nial_alloc_page
 * 描述: 完成page单页的分配
 * 输入: 参数       描述
 *      dev         设备
 *      nial_gfp_flag   分配page页的地址
 * 返回: 返回值     描述
 *      0           失败
 *      >0          成功分配后的地址(非物理)
 *      <0          错误码
 */
static inline unsigned long nial_alloc_page(void *dev, unsigned int nial_gfp_flag)
{
    return nial_alloc_pages(dev, nial_gfp_flag, 0);
}

/**
 * 函数名: nial_alloc_page
 * 描述: 完成page单页的分配，并初始化为0
 * 输入: 参数       描述
 *      dev         设备
 *      nial_gfp_flag   分配page页的地址
 * 返回: 返回值     描述
 *      0           失败
 *      >0          成功分配后的地址(非物理)
 *      <0          错误码
 */
static inline unsigned long nial_zalloc_page(void *dev, unsigned int nial_gfp_flag)
{
    return nial_zalloc_pages(dev, nial_gfp_flag, 0);
}

/**
 * 函数名: nial_free_pages
 * 描述: 对申请的page空间进行释放
 * 输入: 参数       描述
 *      dev         设备
 *      nial_gfp_flag   分配page页的地址
 * 返回: 返回值     描述
 *      无
 */
static inline void nial_free_page(void *dev, unsigned long pa)
{
    return nial_free_pages(dev, pa, 0);
}

#endif /* _NIAL_MALLOC_H_ */
