/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NIAL_SEMAPHORE_H_
#define _NIAL_SEMAPHORE_H_

typedef struct nial_semaphore {
    void *sem;
} nial_semaphore_t;

/**
 * 函数名: nial_sem_init
 * 描述: 初始化信号量
 * 输入: 参数       描述
 *      sem       信号量
 *      val         初始值
 * 返回: 返回值     描述
 *       0           成功
 *      -1          失败
 */
int nial_sem_init(nial_semaphore_t *sem, int val);

/**
 * 函数名: nial_down
 * 描述: 信号量自减(获取)
 * 输入: 参数       描述
 *      sem       信号量
 * 返回: 返回值     描述
 *       0           成功
 *      -1          失败
 */
int nial_down(nial_semaphore_t *sem);

/**
 * 函数名: nial_down_interruptible
 * 描述: 信号量自减(获取)，可中断
 * 输入: 参数       描述
 *      mutex       信号量
 * 返回: 返回值     描述
 *       0           成功
 *      <0          失败
 */
int nial_down_interruptible(nial_semaphore_t *sem);

/**
 * 函数名: nial_down_trylock
 * 描述: 尝试信号量自减(获取)
 * 输入: 参数       描述
 *      sem       信号量
 * 返回: 返回值     描述
 *       0           成功
 *      <0       失败
 */
int nial_down_trylock(nial_semaphore_t *sem);

/**
 * 函数名: nial_down_trylock
 * 描述: 在指定时间信号量自减(获取)
 * 输入: 参数       描述
 *      sem       信号量
 *      ms          时长
 * 返回: 返回值     描述
 *       0           成功
 *      <0       失败
 */
int nial_down_timeout(nial_semaphore_t *sem, long ms);

/**
 * 函数名: nial_up
 * 描述: 信号量自增(释放)
 * 输入: 参数       描述
 *      sem       信号量
 * 返回: 返回值     描述
 *       无
 */
void nial_up(nial_semaphore_t *sem);

/**
 * 函数名: nial_sem_destroy
 * 描述: 销毁信号量
 * 输入: 参数       描述
 *      sem       信号量
 * 返回: 返回值     描述
 *       无
 */
void nial_sem_destroy(nial_semaphore_t *sem);

/**
 * 函数名: nial_sem_is_wait
 * 描述: 是否在等待信号量
 * 输入: 参数       描述
 *      sem       信号量
 * 返回: 返回值     描述
 *       1         在等待
 *       0         未在等待
 */
int nial_sem_is_wait(nial_semaphore_t *mutex);

#endif /* _NIAL_SEMAPHORE_H_ */
