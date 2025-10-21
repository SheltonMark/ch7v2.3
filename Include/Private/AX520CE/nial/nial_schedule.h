/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NIAL_SCHEDULE_H_
#define _NIAL_SCHEDULE_H_

/**
 * 函数名: nial_yield
 * 描述: 放弃CPU资源，立刻禁用中断
 * 输入: 参数       描述
 *      无
 * 返回: 返回值     描述
 *       无
 */
void nial_yield(void);

/**
 * 函数名: nial_cond_resched
 * 描述: 检查进程为可调度状态，则调度其他进程
 * 输入: 参数       描述
 *      无
 * 返回: 返回值     描述
 *       无
 */
void nial_cond_resched(void);

/**
 * 函数名: nial_schedule_timeout
 * 描述: 当前进程进入睡眠，并调度其他进程
 * 输入: 参数       描述
 *      timeout     睡眠时长
 * 返回: 返回值     描述
 *       0          到达时间被唤醒
 *      >0          未到达时间被唤醒
 *      <0          错误码
 */
unsigned long nial_schedule_timeout(unsigned long timeout);

/**
 * 函数名: nial_schedule
 * 描述: 调度其他进程，需要重新执行调度时禁用中断
 * 输入: 参数       描述
 *      无
 * 返回: 返回值     描述
 *       无
 */
void nial_schedule(void);

#endif /* _NIAL_SCHEDULE_H_ */
