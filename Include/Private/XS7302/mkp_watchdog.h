/* 
 * mkp_watchdog.h -- watchdog 模块对外接口声明
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */


#ifndef _MKP_WATCHDOG_H_
#define _MKP_WATCHDOG_H_


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "nial.h"
#include "mkp_ioctl.h"
#include "ni_type.h"
#include "ni_debug.h"



#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */


/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */
typedef enum{
    WDG_RST_NORMAL = 0,
    WDG_RST_INT,
    WDG_RST_BUTT,
}WDG_RST_MODE_E;

typedef struct{
    NI_U32 u32TimeMs;
    WDG_RST_MODE_E enRstMode;
}WDG_PARAM_S;


typedef enum tagIocNrWatchdog
{   
    IOC_NR_WATCHDOG_CREATE  = 0,
    IOC_NR_WATCHDOG_CHANGETIME,
    IOC_NR_WATCHDOG_DELETE,
    IOC_NR_WATCHDOG_FEEDDOG, 
} IOC_NR_WATCHDOG_E;

#define IOCTL_WATCHDOG_CREATE       _IOW(IOC_MAGIC_WATCHDOG, IOC_NR_WATCHDOG_CREATE, WDG_PARAM_S)
#define IOCTL_WATCHDOG_CHANGETIME   _IOW(IOC_MAGIC_WATCHDOG, IOC_NR_WATCHDOG_CHANGETIME, NI_U32)
#define IOCTL_WATCHDOG_DELETE       _IO(IOC_MAGIC_WATCHDOG, IOC_NR_WATCHDOG_DELETE)
#define IOCTL_WATCHDOG_FEEDDOG      _IO(IOC_MAGIC_WATCHDOG, IOC_NR_WATCHDOG_FEEDDOG)

/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */


/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

