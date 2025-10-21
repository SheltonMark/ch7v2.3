/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NIAL_ADDR_H_
#define _NIAL_ADDR_H_

#define NIAL_PAGE_ALIGN(addr)       __NIAL_ALIGN_UP(addr, 4096)
#define __NIAL_ALIGN_UP(addr, size) (((addr) + ((size)-1)) & (~((size)-1)))

/**
 * 函数名: nial_ioremap
 * 描述: 将设备IO内存地址映射为虚拟地址，不可cache
 * 输入: 参数       描述
 *      phys_addr   设备的物理地址
 *      size        映射区域大小，单位为字节
 * 返回: 返回值     描述
 *      virt_addr   映射后的虚拟地址
 */

void *nial_ioremap(unsigned long phys_addr, unsigned long size);

/**
 * 函数名: nial_ioremap_nocache
 * 描述: 将设备IO内存地址映射为虚拟地址，不可cache
 * 输入: 参数       描述
 *      phys_addr   设备的物理地址
 *      size        映射区域大小，单位为字节
 * 返回: 返回值     描述
 *      virt_addr   映射后的虚拟地址
 */
void *nial_ioremap_nocache(unsigned long phys_addr, unsigned long size);

/**
 * 函数名: nial_ioremap_wc
 * 描述: 将设备IO内存地址映射为虚拟地址，不可cache，支持高速回写
 * 输入: 参数       描述
 *      phys_addr   设备的物理地址
 *      size        映射区域大小，单位为字节
 * 返回: 返回值     描述
 *      virt_addr   映射后的虚拟地址
 */
void *nial_ioremap_wc(unsigned long phys_addr, unsigned long size);

/**
 * 函数名: nial_ioremap_cache
 * 描述: 将设备IO内存地址映射为虚拟地址，可cache
 * 输入: 参数       描述
 *      phys_addr   设备的物理地址
 *      size        映射区域大小，单位为字节
 * 返回: 返回值     描述
 *      virt_addr   映射后的虚拟地址
 */
void *nial_ioremap_cache(unsigned long phys_addr, unsigned long size);

/**
 * 函数名: nial_iounmap
 * 描述: 取消使用ioremap创建的映射关系
 * 输入: 参数       描述
 *      addr   虚拟地址
 * 返回: 返回值     描述
 *      无
 */
void nial_iounmap(void *addr);

/**
 * 函数名: nial_copy_from_user
 * 描述: 从用户空间拷贝数据到内核空间
 * 输入: 参数       描述
 *      to      内核空间目标地址
 *      from    用户空间源地址
 *      n       需要拷贝的长度，单位为字节
 * 返回: 返回值     描述
 *      0           成功
 *      非0         拷贝失败的字节数
 */
unsigned long nial_copy_from_user(void *to, const void *from, unsigned long n);

/**
 * 函数名: nial_copy_to_user
 * 描述: 将内核中的数据拷贝至用户空间
 * 输入: 参数       描述
 *      to      用户空间目标地址
 *      from    内核空间源地址
 *      n       需要拷贝的长度，单位为字节
 * 返回: 返回值     描述
 *      0           成功
 *      非0         拷贝失败的字节数
 */
unsigned long nial_copy_to_user(void *to, const void *from, unsigned long n);

/**
 * 函数名: nial_access_ok
 * 描述: 对用空间传入地址检查
 * 输入: 参数       描述
 *      type      0——检查读，1——检查写
 *      addr    用户空间地址
 *      size       需要检查大小
 * 返回: 返回值     描述
 *      0           失败
 *      非0         成功
 */
int nial_access_ok(int type, const void *addr, unsigned long size);

/**
 * 函数名: nial_kmap_cache
 * 描述: 将内存物理地址映射为虚拟地址，可cache
 * 输入: 参数       描述
 *      phys_addr  物理地址
 *      size       映射区域大小，单位为字节
 * 返回: 返回值     描述
 *      virt_addr   映射后的虚拟地址
 */
void *nial_kmap_cache(unsigned long phys_addr, unsigned long size);

/**
 * 函数名: nial_kmap_noncache
 * 描述: 将内存物理地址映射为虚拟地址，不可cache
 * 输入: 参数       描述
 *      phys_addr  物理地址
 *      size       映射区域大小，单位为字节
 * 返回: 返回值     描述
 *      virt_addr   映射后的虚拟地址
 */
void *nial_kmap_noncache(unsigned long phys_addr, unsigned long size);

/**
 * 函数名: nial_kmap_wc
 * 描述: 将内存物理地址映射为虚拟地址，不可cache，可回写
 * 输入: 参数       描述
 *      phys_addr  物理地址
 *      size       映射区域大小，单位为字节
 * 返回: 返回值     描述
 *      virt_addr   映射后的虚拟地址
 */
void *nial_kmap_wc(unsigned long phys_addr, unsigned long size);

/**
 * 函数名: nial_unkmap
 * 描述: 取消内存物理地址映射为虚拟地址的映射关系
 * 输入: 参数       描述
 *      vaddr       映射的虚拟地址
 * 返回: 返回值     描述
 *      0       成功
 *      -1      失败
 */
int nial_unkmap(void *vaddr);

/**
 * 函数名: nial_virt_to_phys
 * 描述: 将虚拟地址转化为物理地址
 * 输入: 参数       描述
 *      virt       虚拟地址
 * 返回: 返回值     描述
 *      phys_addr       物理地址
 */
unsigned long nial_virt_to_phys(void *virt);

/**
 * 函数名: nial_phys_to_virt
 * 描述: 将物理地址转化为虚拟地址
 * 输入: 参数       描述
 *      phys       物理地址
 * 返回: 返回值     描述
 *      virt_addr       虚拟地址
 */
void *nial_phys_to_virt(unsigned long phys);

/**
 * 函数名: nial_readl
 * 描述: 从被映射的设备IO虚拟地址读取4字节(32bit)数据
 * 输入: 参数       描述
 *      addr       被映射的虚拟地址
 * 返回: 返回值     描述
 *      value         读取到的结果
 */
unsigned int nial_readl(volatile void *addr);

/**
 * 函数名: nial_writel
 * 描述: 向被映射的设备IO虚拟地址写数据
 * 输入: 参数       描述
 *      value       需要写入的数据
 *      addr       目标被映射的虚拟地址
 * 返回: 返回值     描述
 *      无
 */
void nial_writel(unsigned int value, volatile void *addr);

/**
 * 函数名: nial_vm_mmap
 * 描述: 将物理内存中的内存页地址映射为虚拟空间内存页地址
 * 输入: 参数       描述
 *      file        文件描述
 *      addr        起始地址
 *      len         长度
 *      port        Page类型
 *      flag        映射标志
 *      offset      偏移
 * 返回: 返回值     描述
 *      <0          错误码
 *      others      虚拟空间内存页地址
 */
unsigned long nial_vm_mmap(void *file, unsigned long addr, unsigned long len, unsigned long prot, unsigned long flag,
                           unsigned long offset, unsigned int cached);

/**
 * 函数名: nial_vm_munmap
 * 描述: 取消物理内存中的内存页地址映射为虚拟空间内存页地址
 * 输入: 参数       描述
 *      start       物理地址
 *      len         长度
 * 返回: 返回值     描述
 *      无
 */
int nial_vm_munmap(unsigned long start, unsigned long len);

#endif /* _NIAL_ADDR_H_ */
