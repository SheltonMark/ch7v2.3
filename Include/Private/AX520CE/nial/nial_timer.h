/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NIAL_TIMER_H_
#define _NIAL_TIMER_H_

typedef void (*timer_cb_fn)(void *data);

/**
 * 函数名: nial_timer_init
 * 描述: 初始化定时器
 * 输入: 参数       描述
 *      function     回调函数
 *      data         私有数据
 * 返回: 返回值     描述
 *       无
 */
void *nial_timer_init(timer_cb_fn function, void *data);

/**
 * 函数名: nial_mod_timer
 * 描述: 修改定时时长
 * 输入: 参数       描述
 *      timer       定时器
 *      interval    设定超时时长
 * 返回: 返回值     描述
 *       0          定时器未被激活
 *      1           定时器已被激活
 */
int nial_mod_timer(void *timer, unsigned long interval);

/**
 * 函数名: nial_mod_timer_pending
 * 描述: 重设定时时长，会覆盖之前的值
 * 输入: 参数       描述
 *      timer       定时器
 *      interval    设定超时时长
 * 返回: 返回值     描述
 *       0          定时器未被激活
 *      1           定时器已被激活
 */
int nial_mod_timer_pending(void *timer, unsigned long interval);

/**
 * 函数名: nial_del_timer_sync
 * 描述: 等待处理函数完成，删除定时器，
 * 输入: 参数       描述
 *      timer       定时器
 * 返回: 返回值     描述
 *       0          定时器未被激活
 *      1           定时器已被激活
 */
int nial_del_timer_sync(void *timer);

/**
 * 函数名: nial_del_timer
 * 描述: 直接删除定时器，
 * 输入: 参数       描述
 *      timer       定时器
 * 返回: 返回值     描述
 *       0          定时器未被激活
 *      1           定时器已被激活
 */
int nial_del_timer(void *timer);

/**
 * 函数名: nial_timer_destroy
 * 描述: 销毁定时器
 * 输入: 参数       描述
 *      timer       定时器
 * 返回: 返回值     描述
 *       0          成功
 *      -1          失败
 */
int nial_timer_destroy(void *timer);

/**
 * 函数名: nial_timer_get_private_data
 * 描述: 获取定时器私有数据
 * 输入: 参数       描述
 *      data       定时器
 * 返回: 返回值     描述
 *       0          成功
 *      -1          失败
 */
void *nial_timer_get_private_data(void *data);

#endif /* _NIAL_TIMER_H_ */
