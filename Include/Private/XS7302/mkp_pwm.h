/* 
 * mkp_pwm.h -- pwm 模块对外接口声明
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */


#ifndef _MKP_PWM_H_
#define _MKP_PWM_H_

/* ========================================================================== */
/*                             头文件区                                       */
/*                             头文件区                                       */
/* ========================================================================== */
#include "nial.h"
#include "ni_type.h"
#include "ni_debug.h"
#include "mkp_ioctl.h"


#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */


/* ========================================================================== */
/*                           宏、枚举、类型数据结构定义区                              */
/* ========================================================================== */

/*PWM工作模式*/
typedef enum{
    ONE_SHOT_MODE = 1,
    CONTINUOUS_MODE = 2,
    PWM_WORK_MODE_MAX,
}PWM_WORK_MODE_E;

/*oneshot模式下触发方式*/
typedef enum{
    PWM_EVTRING_MODE_NORMAL  = 0,
    PWM_EVTRING_MODE_RISING,
    PWM_EVTRING_MODE_FALLING,
    PWM_EVTRING_MODE_MAX,
}PWM_EVTRING_MODE_E;

typedef enum tagIocNrPwm
{
    IOC_NR_PWM_CREATE   = 0,
    IOC_NR_PWM_DELETE,
    IOC_NR_PWM_ENABLE,
    IOC_NR_PWM_DISABLE,
    IOC_NR_PWM_SETFREQ,
    IOC_NR_PWM_SETDUTYRATION,
    IOC_NR_PWM_SETINACTIVE,
    IOC_NR_PWM_SETFIRSTPHASE,
    IOC_NR_PWM_SETEVTRIG,
    IOC_NR_PWM_SETRPTVAL,
    IOC_NR_PWM_SOFTSET,
    IOC_NR_PWM_CFGUPDATE,
    IOC_NR_PWM_SETINTENABLE,
} IOC_NR_PWM_E;

/* PWM内部数据结构 */
typedef struct tagPwmInitParams
{
    /*PWM ID号*/
    NI_U32 u32PwmId;

    /* PWM模式 ,1:ONE_SHOT模式 ，2:CONTINUOUS模式*/
    NI_U32 u32Mode;

    /*一个周期内第一个相位的输出电平*/
    NI_U8 u8FirstPhaseLevel;

    /*不被使能时的输出电平*/
    NI_U8 u8InActiveLevel;

    /*中断开关*/
    NI_U8 u8IntEn;

    /*one shot模式下的中断触发模式*/
    NI_U8 u8IntTrigMode;

    /* PWM频率 */
    NI_U32 u32Freq;

    /* PWM占空比 */
    NI_U32 u32DutyRation;

    /*one shot 重复周期数*/
    NI_U32 u32OneShotRptNum;
}PWM_INIT_PARAM_S;


/* ========================================================================== */
/*                          系统调用、接口函数声明区                                        */
/* ========================================================================== */
#define IOCTL_PWM_CREATE          _IOW(IOC_MAGIC_PWM, IOC_NR_PWM_CREATE,        PWM_INIT_PARAM_S)
#define IOCTL_PWM_DELETE          _IOW(IOC_MAGIC_PWM, IOC_NR_PWM_DELETE,        PWM_INIT_PARAM_S)
#define IOCTL_PWM_ENABLE          _IOW(IOC_MAGIC_PWM, IOC_NR_PWM_ENABLE,        PWM_INIT_PARAM_S)
#define IOCTL_PWM_DISABLE         _IOW(IOC_MAGIC_PWM, IOC_NR_PWM_DISABLE,       PWM_INIT_PARAM_S)
#define IOCTL_PWM_SETFREQ         _IOW(IOC_MAGIC_PWM, IOC_NR_PWM_SETFREQ,       PWM_INIT_PARAM_S)
#define IOCTL_PWM_SETDUTYRATION   _IOW(IOC_MAGIC_PWM, IOC_NR_PWM_SETDUTYRATION, PWM_INIT_PARAM_S)
#define IOCTL_PWM_SETINACTIVE     _IOW(IOC_MAGIC_PWM, IOC_NR_PWM_SETINACTIVE,   PWM_INIT_PARAM_S)
#define IOCTL_PWM_SETFIRSTPHASE   _IOW(IOC_MAGIC_PWM, IOC_NR_PWM_SETFIRSTPHASE, PWM_INIT_PARAM_S)
#define IOCTL_PWM_SETRPTVAL       _IOW(IOC_MAGIC_PWM, IOC_NR_PWM_SETRPTVAL,     PWM_INIT_PARAM_S)
#define IOCTL_PWM_SOFTRESET       _IOW(IOC_MAGIC_PWM, IOC_NR_PWM_SOFTSET,       PWM_INIT_PARAM_S)
#define IOCTL_PWM_CFGUPDATE       _IOW(IOC_MAGIC_PWM, IOC_NR_PWM_CFGUPDATE,     PWM_INIT_PARAM_S)
#define IOCTL_PWM_SETINTENABLE    _IOWR(IOC_MAGIC_PWM, IOC_NR_PWM_SETINTENABLE, PWM_INIT_PARAM_S)

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

