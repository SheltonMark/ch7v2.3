/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NIAL_TIME_H_
#define _NIAL_TIME_H_

#define NIAL_HZ 100

typedef struct nial_timeval {
    long tv_sec;
    long tv_usec;
} nial_timeval_t;

typedef struct nial_rtc_time {
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
    int tm_isdst;
} nial_rtc_time_t;

/**
 * 函数名: nial_msleep
 * 描述: 不可中断睡眠
 * 输入: 参数       描述
 *      msecs       睡眠时长
 * 返回: 返回值     描述
 *       jiffies    睡眠时长对应的jiffies
 */
unsigned long nial_msleep(unsigned int msecs);

/**
 * 函数名: nial_msleep_interruptible
 * 描述: 可中断睡眠
 * 输入: 参数       描述
 *      msecs       睡眠时长
 * 返回: 返回值     描述
 *       jiffies    睡眠时长对应的jiffies
 */
unsigned long nial_msleep_interruptible(unsigned int msecs);

/**
 * 函数名: nial_udelay
 * 描述: 进行us级延时
 * 输入: 参数       描述
 *      usecs       延时时长
 * 返回: 返回值     描述
 *       无
 */
void nial_udelay(unsigned int usecs);

/**
 * 函数名: nial_mdelay
 * 描述: 进行ms级延时
 * 输入: 参数       描述
 *      msecs       延时时长
 * 返回: 返回值     描述
 *       无
 */
void nial_mdelay(unsigned int msecs);

/**
 * 函数名: nial_msecs_to_jiffies
 * 描述: 将ms转化未jiffies
 * 输入: 参数       描述
 *      m       待转化的时长值
 * 返回: 返回值     描述
 *       jiffies    转化后的jieffies值
 */
unsigned long nial_msecs_to_jiffies(const unsigned int m);

/**
 * 函数名: nial_get_jiffies
 * 描述: 获取系统当前的jiffies
 * 输入: 参数       描述
 *      无
 * 返回: 返回值     描述
 *       jiffies    系统当前jieffies值
 */
unsigned long nial_get_jiffies(void);

/**
 * 函数名: nial_time_before
 * 描述: 比较两时钟的jiffies值，a时钟是否在b时钟之前
 * 输入: 参数       描述
 *      无
 * 返回: 返回值     描述
 *       非0        是
 *      0          否
 */
int nial_time_before(unsigned long a, unsigned long b);

/**
 * 函数名: nial_time_after
 * 描述: 比较两时钟的jiffies值，a时钟是否在b时钟之后
 * 输入: 参数       描述
 *      无
 * 返回: 返回值     描述
 *       非0        是
 *      0          否
 */
int nial_time_after(unsigned long a, unsigned long b);

/**
 * 函数名: nial_sched_clock
 * 描述: 获取当前系统运行时间
 * 输入: 参数       描述
 *      无
 * 返回: 返回值     描述
 *       TSC        当前时间
 */
unsigned long long nial_sched_clock(void);

/**
 * 函数名: nial_gettimeofday
 * 描述: 获取当前系统nial_timeval_t时间
 * 输入: 参数       描述
 *      无
 * 输出: 参数       描述
 *      tv          本地时间(s-us)
 * 返回: 返回值     描述
 *       无
 */
void nial_gettimeofday(nial_timeval_t *tv);

/**
 * 函数名: nial_rtc_time_to_tm
 * 描述: 将rtc时间转化为公历时间
 * 输入: 参数       描述
 *      time        秒数
 * 输出: 参数       描述
 *      tv          公历时间
 * 返回: 返回值     描述
 *       无
 */
void nial_rtc_time_to_tm(unsigned long time, nial_rtc_time_t *tm);

/**
 * 函数名: nial_rtc_time_to_tm
 * 描述: 将公历时间转化为rtc时间
 * 输入: 参数       描述
 *      tm        公历时间
 * 输出: 参数       描述
 *      time          rtc时间指针
 * 返回: 返回值     描述
 *       无
 */
void nial_rtc_tm_to_time(nial_rtc_time_t *tm, unsigned long *time);

/**
 * 函数名: nial_rtc_valid_tm
 * 描述: rtc时间是否有效
 * 输入: 参数       描述
 *      tm        rtc时间指针
 * 返回: 返回值     描述
 *       0          成功
 *      <0          失败
 */
int nial_rtc_valid_tm(struct nial_rtc_time *tm);

#endif /* _NIAL_TIME_H_ */
