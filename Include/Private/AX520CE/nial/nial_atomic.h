/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NIAL_ATOMIC_H_
#define _NIAL_ATOMIC_H_

#define NIAL_BITS_PER_LONG ((sizeof(unsigned long) * 8))
#define NIAL_BIT_MASK(nr)  (1 << ((nr) % NIAL_BITS_PER_LONG))
#define NIAL_BIT_WORD(nr)  ((nr) / NIAL_BITS_PER_LONG)

typedef struct {
    void *atomic;
} nial_atomic_t;

/**
 * 函数名: nial_atomic_init
 * 描述: 初始化原子量
 * 输入: 参数       描述
 *      atomic      待初始化的原子变量
 * 返回: 返回值     描述
 *      0           成功
 *      -1          失败
 */
int nial_atomic_init(nial_atomic_t *atomic);

/**
 * 函数名: nial_atomic_destroy
 * 描述: 销毁原子变量
 * 输入: 参数       描述
 *      atomic   待摧毁的原子变量
 * 返回: 返回值     描述
 *      无
 */
void nial_atomic_destroy(nial_atomic_t *atomic);

/**
 * 函数名: nial_atomic_read
 * 描述: 读取原子变量的值
 * 输入: 参数       描述
 *      atomic   原子变量
 * 返回: 返回值     描述
 *      val   原子变量的值
 */
int nial_atomic_read(nial_atomic_t *atomic);

/**
 * 函数名: nial_atomic_set
 * 描述: 设置原子变量的初始值
 * 输入: 参数       描述
 *      atomic   原子变量
 *      i        初始值
 * 返回: 返回值     描述
 *      无
 */
void nial_atomic_set(nial_atomic_t *atomic, int i);

/**
 * 函数名: nial_atomic_inc_return
 * 描述: 原子量自增1，并返回原子量的值
 * 输入: 参数       描述
 *      atomic   原子变量
 * 返回: 返回值     描述
 *      val         自增后的值
 */
int nial_atomic_inc_return(nial_atomic_t *atomic);

/**
 * 函数名: nial_atomic_inc_return
 * 描述: 原子量自减1，并返回原子量的值
 * 输入: 参数       描述
 *      atomic   原子变量
 * 返回: 返回值     描述
 *      val         自减后的值
 */
int nial_atomic_dec_return(nial_atomic_t *atomic);

/**
 * 函数名: nial_atomic_inc
 * 描述: 原子量自增1
 * 输入: 参数       描述
 *      atomic   原子变量
 * 返回: 返回值     描述
 *      无
 */
void nial_atomic_inc(nial_atomic_t *atomic);

/**
 * 函数名: nial_atomic_inc
 * 描述: 原子量自减1
 * 输入: 参数       描述
 *      atomic   原子变量
 * 返回: 返回值     描述
 *      无
 */
void nial_atomic_dec(nial_atomic_t *atomic);

/**
 * 函数名: nial_set_bit
 * 描述: 设置内存地址对应值的第nr比特位设置为1
 * 输入: 参数       描述
 *      nr          位置
 *      addr        内存地址
 * 返回: 返回值     描述
 *      无
 */
void nial_set_bit(int nr, volatile unsigned long *addr);

/**
 * 函数名: nial_clear_bit
 * 描述: 清除内存地址对应值的第nr比特位为0
 * 输入: 参数       描述
 *      nr          位置
 *      addr        内存地址
 * 返回: 返回值     描述
 *      无
 */
void nial_clear_bit(int nr, volatile unsigned long *addr);

/**
 * 函数名: nial_change_bit
 * 描述: 反转内存地址对应值的第nr比特位
 * 输入: 参数       描述
 *      nr          位置
 *      addr        内存地址
 * 返回: 返回值     描述
 *      无
 */
void nial_change_bit(int nr, volatile unsigned long *addr);

/**
 * 函数名: nial_test_and_set_bit
 * 描述: 将内存地址对应值的第nr比特位设置为1，并返回第nr位之前的值
 * 输入: 参数       描述
 *      nr          位置
 *      addr        内存地址
 * 返回: 返回值     描述
 *      val         改变之前的值
 */
int nial_test_and_set_bit(int nr, volatile unsigned long *addr);

/**
 * 函数名: nial_test_and_clear_bit
 * 描述: 将内存地址对应值的第nr比特位清除为0，并返回第nr位之前的值
 * 输入: 参数       描述
 *      nr          位置
 *      addr        内存地址
 * 返回: 返回值     描述
 *      val         改变之前的值
 */
int nial_test_and_clear_bit(int nr, volatile unsigned long *addr);

/**
 * 函数名: nial_test_and_change_bit
 * 描述: 将内存地址对应值的第nr比特位进行反转，并返回第nr位之前的值
 * 输入: 参数       描述
 *      nr          位置
 *      addr        内存地址
 * 返回: 返回值     描述
 *      val         改变之前的值
 */
int nial_test_and_change_bit(int nr, volatile unsigned long *addr);

/**
 * 函数名: nial_test_bit
 * 描述: 返回内存地址对应值的第nr比特位的值
 * 输入: 参数       描述
 *      nr          位置
 *      addr        内存地址
 * 返回: 返回值     描述
 *      val         改变之前的值
 */
int nial_test_bit(int nr, const volatile unsigned long *addr);

#endif /* _NIAL_ATOMIC_H_ */
