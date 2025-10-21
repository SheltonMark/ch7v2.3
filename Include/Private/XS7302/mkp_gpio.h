/* 
 * mkp_gpio.h -- gpio 模块对外接口声明
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */

#ifndef _MKP_GPIO_H_
#define _MKP_GPIO_H_


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



/* GPIO方向标志 */
typedef enum tagGpioDirection
{
    GPIO_DIR_OUT  = 0,
    GPIO_DIR_IN   = 1,
}GPIO_DIRECTION_EN;

typedef enum tagIocNrGpio
{
    IOC_NR_GPIO_SETDIPLEX   = 0,
    IOC_NR_GPIO_SETLEVEL,
    IOC_NR_GPIO_GETLEVEL,
    IOC_NR_GPIO_REGINT,
    IOC_NR_GPIO_UNREGINT,
    IOC_NR_GPIO_SETINTTYPE,
    IOC_NR_GPIO_ENABLEINT,
    IOC_NR_GPIO_DISABLEINT,
    IOC_NR_GPIO_SETDIR,
    IOC_NR_GPIO_SETPOLARITY,
    IOC_NR_GPIO_MASKINT,
} IOC_NR_GPIO_E;

#define IOCTL_GPIO_SETDIPLEX    _IOWR(IOC_MAGIC_GPIO, IOC_NR_GPIO_SETDIPLEX, NI_U32)
#define IOCTL_GPIO_SETLEVEL     _IOWR(IOC_MAGIC_GPIO, IOC_NR_GPIO_SETLEVEL, NI_U32)
#define IOCTL_GPIO_GETLEVEL     _IOWR(IOC_MAGIC_GPIO, IOC_NR_GPIO_GETLEVEL, NI_U32)
#define IOCTL_GPIO_REGINT       _IOWR(IOC_MAGIC_GPIO, IOC_NR_GPIO_REGINT, NI_U32)
#define IOCTL_GPIO_UNREGINT     _IOWR(IOC_MAGIC_GPIO, IOC_NR_GPIO_UNREGINT, NI_U32)
#define IOCTL_GPIO_SETINTTYPE   _IOWR(IOC_MAGIC_GPIO, IOC_NR_GPIO_SETINTTYPE, NI_U32)
#define IOCTL_GPIO_ENABLEINT    _IOWR(IOC_MAGIC_GPIO, IOC_NR_GPIO_ENABLEINT, NI_U32)
#define IOCTL_GPIO_DISABLEINT   _IOWR(IOC_MAGIC_GPIO, IOC_NR_GPIO_DISABLEINT, NI_U32)
#define IOCTL_GPIO_SETDIR       _IOWR(IOC_MAGIC_GPIO, IOC_NR_GPIO_SETDIR, NI_U32)
#define IOCTL_GPIO_SETPOLARITY  _IOWR(IOC_MAGIC_GPIO, IOC_NR_GPIO_SETPOLARITY, NI_U32)
#define IOCTL_GPIO_MASKINT      _IOWR(IOC_MAGIC_GPIO, IOC_NR_GPIO_MASKINT, NI_U32)



typedef NI_S32 (*GPIO_INTCALLBACK_PF)(NI_U32 u32Gpio, void *pvUsrArgs);


/* GPIO中断参数设置 */
typedef struct tagGpioIntAttr
{
    NI_U32              u32Gpio;         /* IO端口id    */
    NI_U32              u32IntType;      /* 中断类型	：0：电平敏感；1：边沿敏感    */
    NI_U32              u32IntPolarity;  /* 中断触发方式	0：低/下降沿；1：高/上升沿               2：双边延触发  */        
    NI_U32              u32IntMask;      /* 中断屏蔽*/
    NI_VOID             *pvUsrArgs;      /* 用户自定义参数 */
    GPIO_INTCALLBACK_PF pfnIntCallBack;  /* GPIO中断回调函数 */
}GPIO_INT_ATTR_S;


/* GPIO中断参数设置 */
typedef struct tagGpioBaseAttr
{
    NI_U32  u32Gpio;
    NI_U32  u32Level;
    NI_U32  u32Dir;          /* 引脚方向，其定义见GPIO_DIRECTION_EN。*/    
    NI_S32  s32DiplexValue;  /* 设置管脚复用值 */
}GPIO_BASE_ATTR_S;


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

