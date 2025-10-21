/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NIAL_INTERRUPT_H_
#define _NIAL_INTERRUPT_H_

#define NIAL_IRQF_TRIGGER_NONE    0x00000000
#define NIAL_IRQF_TRIGGER_RISING  0x00000001
#define NIAL_IRQF_TRIGGER_FALLING 0x00000002
#define NIAL_IRQF_TRIGGER_HIGH    0x00000004
#define NIAL_IRQF_TRIGGER_LOW     0x00000008
#define NIAL_IRQF_TRIGGER_MASK \
    (NIAL_IRQF_TRIGGER_HIGH | NIAL_IRQF_TRIGGER_LOW | NIAL_IRQF_TRIGGER_RISING | NIAL_IRQF_TRIGGER_FALLING)
#define NIAL_IRQF_TRIGGER_PROBE 0x00000010

enum nial_irqreturn {
    NIAL_IRQ_NONE        = (0 << 0),
    NIAL_IRQ_HANDLED     = (1 << 0),
    NIAL_IRQ_WAKE_THREAD = (1 << 1),
};

typedef enum nial_irqreturn nial_irqreturn_t;

typedef struct nial_tasklet_struct {
    void *tasklet_struct;
} nial_tasklet_struct_t;
typedef enum nial_irqreturn (*nial_irq_handler_t)(int, void *);

/**
 * 函数名: nial_tasklet_init
 * 描述: tasklet初始化
 * 输入: 参数       描述
 *      nial_t         对应的tasklet
 *      func            下半部执行函数
 *      data            func所需参数
 * 返回: 返回值     描述
 *      -1        失败
 *      0       成功
 */
int nial_tasklet_init(nial_tasklet_struct_t *nial_t, void (*func)(unsigned long), unsigned long data);

/**
 * 函数名: nial_tasklet_kill
 * 描述: 移除tasklet
 * 输入: 参数       描述
 *      nial_t         对应的tasklet
 * 返回: 返回值     描述
 *      -1        失败
 *      0           成功
 */
int nial_tasklet_kill(nial_tasklet_struct_t *nial_t);

/**
 * 函数名: nial_tasklet_schedule
 * 描述: 触发执行下半部函数
 * 输入: 参数       描述
 *      nial_t         对应的tasklet
 * 返回: 返回值     描述
 *      -1        失败
 *      0           成功
 */
int nial_tasklet_schedule(nial_tasklet_struct_t *nial_t);

/**
 * 函数名: nial_request_threaded_irq
 * 描述: 带线程注册中断
 * 输入: 参数       描述
 *      irq         中断号
 *      handler     中断处理程序
 *      thread_fn   中断处理线程
 *      flags       中断触发标志
 *      name        处理程序的名字
 *      dev         中断处理参数
 * 返回: 返回值     描述
 *      <0        失败
 *      0           成功
 */
int nial_request_threaded_irq(unsigned int irq, nial_irq_handler_t handler, nial_irq_handler_t thread_fn,
                              unsigned long flags, const char *name, void *dev);

/**
 * 函数名: nial_free_irq
 * 描述: 释放中断
 * 输入: 参数       描述
 *      irq             中断号
 *      dev             中断处理参数
 * 返回: 返回值     描述
 *      无
 */
void nial_free_irq(unsigned int irq, void *dev);

/**
 * 函数名: nial_in_interrupt
 * 描述: 判断是否处于中断上下文中
 * 输入: 参数       描述
 *      无
 * 返回: 返回值     描述
 *      非0          是
 *      0           否
 */
int nial_in_interrupt(void);

/**
 * 函数名: nial_request_irq
 * 描述: 注册中断
 * 输入: 参数       描述
 *      irq         中断号
 *      handler     中断处理程序
 *      flags       中断触发标志
 *      name        中断处理程序名称
 *      dev         中断处理程序参数
 * 返回: 返回值     描述
 *      <0        失败
 *      0           成功
 */
int nial_request_irq(unsigned int irq, nial_irq_handler_t handler, unsigned long flags, const char *name, void *dev);

/**
 * 函数名: nial_irq_set_irq_wake
 * 描述: 设置唤醒中断源
 * 输入: 参数       描述
 *      irq         中断号
 *      on          开关
 * 返回: 返回值     描述
 *      <0          失败
 *      0           成功
 */
int nial_irq_set_irq_wake(unsigned int irq, unsigned int on);

/**
 * 函数名: nial_enable_irq
 * 描述: 使能中断
 * 输入: 参数       描述
 *      irq             中断号
 * 返回: 返回值     描述
 *      无
 */
void nial_enable_irq(unsigned int irq);

/**
 * 函数名: nial_disable_irq
 * 描述: 使能中断
 * 输入: 参数       描述
 *      irq             中断号
 * 返回: 返回值     描述
 *      无
 */
void nial_disable_irq(unsigned int irq);

#endif /* _NIAL_INTERRUPT_H_ */
