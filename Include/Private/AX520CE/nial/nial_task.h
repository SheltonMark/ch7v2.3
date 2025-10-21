/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NIAL_TASK_H_
#define _NIAL_TASK_H_

typedef int (*threadfn_t)(void *data);

/**
 * 函数名: nial_kthread_create
 * 描述: 创建线程
 * 输入: 参数       描述
 *      thread       线程回调函数
 *      data        回调函数参数
 *      name        线程名称
 *      prio        优先级
 *      stack_size  线程栈大小
 * 返回: 返回值     描述
 *       thread_addr 线程地址
 *      NIAL_NULL   失败
 */
void *nial_kthread_create(threadfn_t thread, void *data, char *name, int prio, unsigned int stack_size);

/**
 * 函数名: nial_kthread_destroy
 * 描述: 初始化自旋锁
 * 输入: 参数       描述
 *      task       线程地址
 *      stop_flag   1——未结束，0——已结束
 * 返回: 返回值     描述
 *       无
 */
void nial_kthread_destroy(void *task, unsigned int stop_flag);

/**
 * 函数名: nial_kthread_bind
 * 描述: 将线程绑定到指定CPU
 * 输入: 参数       描述
 *      task       指定线程地址
 *      cpu         指定cpu的编号
 * 返回: 返回值     描述
 *       0           成功
 *      -1       失败
 */
void nial_kthread_bind(void *task, unsigned int cpu);

/**
 * 函数名: nial_kthread_should_stop
 * 描述: 检查线程是否执行结束退出
 * 输入: 参数       描述
 *      无
 * 返回: 返回值     描述
 *       0           退出
 *      1           不退出
 */
int nial_kthread_should_stop(void);

/**
 * 函数名: nial_task_self
 * 描述: 获取当前执行线程句柄
 * 输入: 参数       描述
 *      无
 * 返回: 返回值     描述
 *       thread_addr 句柄
 */
void *nial_task_self(void);

/**
 * 函数名: nial_get_cpu_idle
 * 描述: 获取当前CPU空闲率
 * 输入: 参数       描述
 *      cpu        CPU ID
 *      idle       CPU空闲占用率，以100为单位
 * 返回: 返回值     描述
 *       无
 */
void nial_get_cpu_idle(unsigned int cpu, unsigned int *idle);

/**
 * 函数名: nial_show_slp_stat
 * 描述: 显示低功耗耗时统计
 * 输入: 参数       描述
 *      无
 * 返回: 返回值     描述
 *       无
 */
void nial_show_slp_stat(void);

#endif /* _NIAL_TASK_H_ */
