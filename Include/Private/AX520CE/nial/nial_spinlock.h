/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NIAL_SPINLOCK_H_
#define _NIAL_SPINLOCK_H_

typedef struct nial_spinlock {
    void *lock;
} nial_spinlock_t;

/**
 * 函数名: nial_spin_lock_init
 * 描述: 初始化自旋锁
 * 输入: 参数       描述
 *      lock       自旋锁变量
 * 返回: 返回值     描述
 *       0           成功
 *      -1       失败
 */
int nial_spin_lock_init(nial_spinlock_t *lock);

/**
 * 函数名: nial_spin_lock
 * 描述: 自旋锁上锁
 * 输入: 参数       描述
 *      lock       自旋锁变量
 * 返回: 返回值     描述
 *       无
 */
void nial_spin_lock(nial_spinlock_t *lock);

/**
 * 函数名: nial_spin_trylock
 * 描述: 自旋锁尝试上锁
 * 输入: 参数       描述
 *      lock       自旋锁变量
 * 返回: 返回值     描述
 *       1          成功
 *       0或-1    失败
 */
int nial_spin_trylock(nial_spinlock_t *lock);

/**
 * 函数名: nial_spin_unlock
 * 描述: 自旋锁尝试下锁
 * 输入: 参数       描述
 *      lock       自旋锁变量
 * 返回: 返回值     描述
 *       无
 */
void nial_spin_unlock(nial_spinlock_t *lock);

/**
 * 函数名: nial_spin_lock_irqsave
 * 描述: 禁用本地中断自旋锁上锁
 * 输入: 参数       描述
 *      lock       自旋锁变量
 * 输出: 参数       描述
 *      flags       保存的本地中断状态
 * 返回: 返回值     描述
 *       无
 */
void nial_spin_lock_irqsave(nial_spinlock_t *lock, unsigned long *flags);

/**
 * 函数名: nial_spin_trylock_irqsave
 * 描述: 禁用本地中断自旋锁尝试上锁
 * 输入: 参数       描述
 *      lock       自旋锁变量
 * 输出: 参数       描述
 *      flags       保存的本地中断状态
 * 返回: 返回值     描述
 *       1          成功
 *       0或-1    失败
 */
int nial_spin_trylock_irqsave(nial_spinlock_t *lock, unsigned long *flags);

/**
 * 函数名: nial_spin_unlock_irqrestore
 * 描述: 禁用本地中断自旋锁尝试下锁
 * 输入: 参数       描述
 *      mutex       自旋锁变量
 *      flags       保存的本地中断状态
 * 返回: 返回值     描述
 *       无
 */
void nial_spin_unlock_irqrestore(nial_spinlock_t *lock, unsigned long *flags);

/**
 * 函数名: nial_spin_lock_destroy
 * 描述: 销毁自旋锁变量
 * 输入: 参数       描述
 *      lock       自旋锁变量
 * 返回: 返回值     描述
 *       无
 */
void nial_spin_lock_destroy(nial_spinlock_t *lock);

#endif /* _NIAL_SPINLOCK_H_ */
