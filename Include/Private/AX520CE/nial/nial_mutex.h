/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NIAL_MUTEX_H_
#define _NIAL_MUTEX_H_

typedef struct nial_mutex {
    void *mutex;
} nial_mutex_t;

/**
 * 函数名: nial_mutex_init
 * 描述: 初始化互斥锁
 * 输入: 参数       描述
 *      mutex       互斥锁变量
 * 返回: 返回值     描述
 *      -1          失败
 *      0           成功
 */
int nial_mutex_init(nial_mutex_t *mutex);

/**
 * 函数名: nial_mutex_lock
 * 描述: 对临界区上锁
 * 输入: 参数       描述
 *      mutex       互斥锁变量
 * 返回: 返回值     描述
 *      -1          失败
 *      0           成功
 */
int nial_mutex_lock(nial_mutex_t *mutex);

/**
 * 函数名: nial_mutex_lock_interruptible
 * 描述: 对临界区上锁，可中断
 * 输入: 参数       描述
 *      mutex       互斥锁变量
 * 返回: 返回值     描述
 *      <0          错误码
 *      0           成功
 */
int nial_mutex_lock_interruptible(nial_mutex_t *mutex);

/**
 * 函数名: nial_mutex_trylock
 * 描述: 对临界区尝试上锁
 * 输入: 参数       描述
 *      mutex       互斥锁变量
 * 返回: 返回值     描述
 *       0           成功
 *      others       失败
 */
int nial_mutex_trylock(nial_mutex_t *mutex);

/**
 * 函数名: nial_mutex_unlock
 * 描述: 对临界区下锁
 * 输入: 参数       描述
 *      mutex       互斥锁变量
 * 返回: 返回值     描述
 *       无
 */
void nial_mutex_unlock(nial_mutex_t *mutex);

/**
 * 函数名: nial_mutex_is_locked
 * 描述: 检查互斥锁是否锁定
 * 输入: 参数       描述
 *      mutex       互斥锁变量
 * 返回: 返回值     描述
 *       0           未上锁
 *      others       已上锁
 */
int nial_mutex_is_locked(nial_mutex_t *mutex);

/* notice:must be called when kmod exit, other wise will lead to memory leak; */
/**
 * 函数名: nial_mutex_destroy
 * 描述: 销毁互斥锁
 * 输入: 参数       描述
 *      mutex       互斥锁变量
 * 返回: 返回值     描述
 *       无
 */
void nial_mutex_destroy(nial_mutex_t *mutex);

#endif /* _NIAL_MUTEX_H_ */
