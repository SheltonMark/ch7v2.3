/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NIAL_WAIT_H_
#define _NIAL_WAIT_H_

typedef int (*nial_wait_cond_func_t)(void *param);

typedef struct nial_wait {
    void *wait;
} nial_wait_t;

void *__nial_alloc_wait_queue(void);
void  __nial_prepare_to_wait_uninterruptible(nial_wait_t *q, void *wait);
void  __nial_prepare_to_wait_interruptible(nial_wait_t *q, void *wait);
void  __nial_finish_wait(nial_wait_t *q, void *wait);
int   __nial_signal_pending(void *task);

#define __nial_wait_event(wq, condition)                        \
    do {                                                        \
        void *__wait = __nial_alloc_wait_queue();               \
        for (;;) {                                              \
            __nial_prepare_to_wait_uninterruptible(wq, __wait); \
            if (condition)                                      \
                break;                                          \
            nial_schedule();                                    \
        }                                                       \
        __nial_finish_wait(wq, __wait);                         \
    } while (0)

#define __nial_wait_event_timeout(wq, condition, ret)           \
    do {                                                        \
        void *__wait = __nial_alloc_wait_queue();               \
        for (;;) {                                              \
            __nial_prepare_to_wait_uninterruptible(wq, __wait); \
            if (condition)                                      \
                break;                                          \
            ret = nial_schedule_timeout(ret);                   \
            if (!ret)                                           \
                break;                                          \
        }                                                       \
        if (!ret && (condition))                                \
            ret = 1;                                            \
        __nial_finish_wait(wq, __wait);                         \
    } while (0)

#define __nial_wait_event_interruptible(wq, condition, ret)   \
    do {                                                      \
        void *__wait = __nial_alloc_wait_queue();             \
        for (;;) {                                            \
            __nial_prepare_to_wait_interruptible(wq, __wait); \
            if (condition)                                    \
                break;                                        \
            if (!__nial_signal_pending(nial_task_self())) {   \
                nial_schedule();                              \
                continue;                                     \
            }                                                 \
            ret = -E_NIAL_RESTARTSYS;                         \
            break;                                            \
        }                                                     \
        __nial_finish_wait(wq, __wait);                       \
    } while (0)

#define __nial_wait_event_interruptible_timeout(wq, condition, ret) \
    do {                                                            \
        void *__wait = __nial_alloc_wait_queue();                   \
        for (;;) {                                                  \
            __nial_prepare_to_wait_interruptible(wq, __wait);       \
            if (condition)                                          \
                break;                                              \
            if (!__nial_signal_pending(nial_task_self())) {         \
                ret = nial_schedule_timeout(ret);                   \
                if (!ret)                                           \
                    break;                                          \
                continue;                                           \
            }                                                       \
            ret = -E_NIAL_RESTARTSYS;                               \
            break;                                                  \
        }                                                           \
        if (!ret && (condition))                                    \
            ret = 1;                                                \
        __nial_finish_wait(wq, __wait);                             \
    } while (0)

/**
 * 宏名: nial_wait_event
 * 描述: 运行等待队列
 * 输入: 参数       描述
 *      wq          等待队列
 *      condition   条件值，可以是整数/布尔返回类型函数
 * 返回: 返回值     描述
 *       无
 */
#define nial_wait_event(wq, condition)    \
    do {                                  \
        if (condition)                    \
            break;                        \
        __nial_wait_event(wq, condition); \
    } while (0)

/**
 * 宏名: nial_wait_event_timeout
 * 描述: 运行等待队列
 * 输入: 参数       描述
 *      wq          等待队列
 *      condition   条件值，可以是整数/布尔返回类型函数
 *      timeout     超时时长
 * 返回: 返回值     描述
 *       0          超时
 *      <0          失败
 *      >0          成功，放入等待队列中
 */
#define nial_wait_event_timeout(wq, condition, timeout)      \
    ({                                                       \
        long __ret = timeout;                                \
        if (!(condition))                                    \
            __nial_wait_event_timeout(wq, condition, __ret); \
        __ret;                                               \
    })

/**
 * 宏名: nial_wait_event_interruptible
 * 描述: 运行等待队列，可中断
 * 输入: 参数       描述
 *      wq          等待队列
 *      condition   条件值，可以是整数/布尔返回类型函数
 * 返回: 返回值     描述
 *       0          成功，放入等待队列中
 *      <0          失败
 */
#define nial_wait_event_interruptible(wq, condition)               \
    ({                                                             \
        int __ret = 0;                                             \
        if (!(condition))                                          \
            __nial_wait_event_interruptible(wq, condition, __ret); \
        __ret;                                                     \
    })

/**
 * 宏名: nial_wait_event_interruptible
 * 描述: 运行等待队列，可中断
 * 输入: 参数       描述
 *      wq          等待队列
 *      condition   条件值，可以是整数/布尔返回类型函数
 *      timeout     超时时长
 * 返回: 返回值     描述
 *       0          超时
 *      <0          失败
 *      >0          成功，放入等待队列中
 */
#define nial_wait_event_interruptible_timeout(wq, condition, timeout)      \
    ({                                                                     \
        long __ret = timeout;                                              \
        if (!(condition))                                                  \
            __nial_wait_event_interruptible_timeout(wq, condition, __ret); \
        __ret;                                                             \
    })

/**
 * 函数名: nial_wait_init
 * 描述: 初始化等待队列
 * 输入: 参数       描述
 *      nial_wait_t    等待队列
 * 返回: 返回值     描述
 *       0          成功
 *      -1          失败
 */
int nial_wait_init(nial_wait_t *wait);

/**
 * 函数名: nial_wake_up
 * 描述: 唤醒等待队列
 * 输入: 参数       描述
 *      nial_wait_t    等待队列
 * 返回: 返回值     描述
 *       无
 */
void nial_wake_up(nial_wait_t *wait);

/**
 * 函数名: nial_wait_destroy
 * 描述: 销毁等待队列
 * 输入: 参数       描述
 *      nial_wait_t    等待队列
 * 返回: 返回值     描述
 *       无
 */
void nial_wait_destroy(nial_wait_t *wait);

/**
 * 函数名: nial_wake_up_interruptible
 * 描述: 唤醒等待队列
 * 输入: 参数       描述
 *      nial_wait_t    等待队列
 * 返回: 返回值     描述
 *       无
 */
void nial_wake_up_interruptible(nial_wait_t *wait);

#endif /* _NIAL_WAIT_H_ */
