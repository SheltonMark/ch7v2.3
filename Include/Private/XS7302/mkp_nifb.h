/*******************************************************************************
 * Copyright (C) 2020-2025 ZheJiang Chipup Technology CO.,LTD.
 *
 * 文件名 : mkp_nifb.h
 *
 * 作  者 : Chipup Software Team
 * 版  本 : V1.0.0
 * 日  期 : 2021年1月23日 Create
 *
 * 描  述 : 
 *
 *       1. 硬件说明
 *          无。
 *
 *       2. 程序结构说明。
 *          无。
 *
 *       3. 使用说明。
 *          无。
 *
 *       4. 局限性说明。
 *          无。
 *
 *       5. 其他说明。
 *          无。
 *
 * 修订历史: 
 * 日    期: 
 * 修订版本:
 * 作    者:
 * 修订备注:
 *******************************************************************************/
#ifndef _MKP_NIFB_H_
#define _MKP_NIFB_H_

#include <linux/ioctl.h>
#include <linux/fb.h>

#include "ni_common.h"


#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#define IOC_MAGIC_NIFB      'F'

typedef enum tagIocNrNifb
{
    IOC_NR_NIFB_GET_ALPHA = 90,
    IOC_NR_NIFB_PUT_ALPHA,

    IOC_NR_NIFB_GET_SCREEN_ORIGIN,
    IOC_NR_NIFB_PUT_SCREEN_ORIGIN,

    IOC_NR_NIFB_WAIT_VSYNC,

    IOC_NR_NIFB_GET_SHOW,
    IOC_NR_NIFB_PUT_SHOW,

    IOC_NR_NIFB_GET_CAPABILITY,

    IOC_NR_NIFB_GET_SCREENSIZE,
    IOC_NR_NIFB_PUT_SCREENSIZE,

    IOC_NR_NIFB_PUT_LAYER_INFO,
    IOC_NR_NIFB_GET_LAYER_INFO,

    IOC_NR_NIFB_REFRESH,
    IOC_NR_NIFB_WAIT_REFRESH_DONE,
    
    IOC_NR_NIFB_GET_CANVAS_BUFFER,

    IOC_NR_NIFB_PUT_MIRROR_MODE,

    IOC_NR_NIFB_BUTT
}IOC_NR_NIFB_E;

/** Alpha info */
typedef struct tagNifbAlpha
{
    NI_BOOL     bAlphaEnable;        /* 是否使能alpha叠加 */
    NI_U8       u8Alpha0;            /* alpha0仅在ARGB1555格式时有效，其他格式使用像素自身alpha位 */
    NI_U8       u8Alpha1;            /* alpha1仅在ARGB1555格式时有效，其他格式使用像素自身alpha位 */
    
    NI_BOOL     bGlobalAlphaEn;      /* 是否使能全局alpha叠加，默认值为NI_FALSE */
    NI_U8       u8GlobalAlpha;       /* 全局alpha, bGlobalAlphaEn使能时有效 */
    NI_U8       u8Reserved;
}NIFB_ALPHA_S;

typedef enum tagNifbColorFmt
{
    NIFB_FMT_RGB565 = 0,
    NIFB_FMT_RGB888,

    NIFB_FMT_ARGB4444,
    NIFB_FMT_ARGB1555,
    NIFB_FMT_ARGB8888,
    NIFB_FMT_ARGB8565,

    NIFB_FMT_RGBA4444,
    NIFB_FMT_RGBA5551,
    NIFB_FMT_RGBA8888,
    NIFB_FMT_RGBA5658,

    NIFB_FMT_BGR565,
    NIFB_FMT_BGR888,
    NIFB_FMT_ABGR4444,
    NIFB_FMT_ABGR1555,
    NIFB_FMT_ABGR8888,
    NIFB_FMT_ABGR8565,

    NIFB_FMT_BUTT
}NIFB_COLOR_FMT_E;

typedef enum tagNifbBufMode
{
    NIFB_BUFMOD_ONE = 0,            /* 单buf模式，用户绘制的内容将直接显示 */
    NIFB_BUFMOD_DOUBLE = 1,         /* 双buf模式 */

    NIFB_BUFMOD_BUTT
}NIFB_BUFMOD_E;


typedef struct tagNifbCapability
{
    NI_BOOL     bGlobalAlpha;            /* 是否支持全局alpha */
    NI_BOOL     bPalette;                /* 是否支持调色板模式   */
    NI_BOOL     abSupportColFmt[NIFB_FMT_BUTT]; /*   支持NIFB_COLOR_FMT_E中的哪些格式 */
    NI_U32      u32MaxWidth;
    NI_U32      u32MaxHeight;
    NI_U32      u32MinWidth;
    NI_U32      u32MinHeight;
}NIFB_CAPABILITY_S;

/* surface info */
typedef struct tagNifbSurface
{
    NI_U64  u64PhyAddr;     /* 起始物理地址 */
    NI_U32  u32Width;       /* 像素宽度 */
    NI_U32  u32Height;      /* 像素高度 */
    NI_U32  u32Stride;      /* 跨距 */
    NIFB_COLOR_FMT_E enFmt;
}NIFB_SURFACE_S;


/* refresh surface info */
typedef struct tagNifbBuffer
{
    NIFB_SURFACE_S stCanvas;
    RECT_S     stUpdateRect;       /* refresh region*/
}NIFB_BUFFER_S;

/*layer info maskbit*/
typedef enum tagNifbLayerInfoMaskBit
{
    NIFB_LAYERMASK_BUFMODE          = 0x1,       /**< BUFMODE bitmask */
    NIFB_LAYERMASK_POS              = 0x2,       /**< the position bitmask */
    NIFB_LAYERMASK_CANVASSIZE       = 0x4,       /**< canvassize bitmask */
    NIFB_LAYERMASK_DISPSIZE         = 0x8,       /**< displaysize bitmask */
    NIFB_LAYERMASK_SCREENSIZE       = 0x10,      /**< screensize bitmask */
    NIFB_LAYERMASK_BUTT
}NIFB_LAYER_INFO_MASKBIT;

typedef struct tagNifbLayerInfo
{
    NIFB_BUFMOD_E       enBufMode;
    NI_S32              s32XPos;           /**<  the x pos of origion point in screen */
    NI_S32              s32YPos;           /**<  the y pos of origion point in screen */
    NI_U32              u32CanvasWidth;    /**<  the width of canvas buffer */
    NI_U32              u32CanvasHeight;   /**<  the height of canvas buffer */
    NI_U32              u32DisplayWidth;   /**<  the width of display buf in fb.for 0 buf ,there is no display buf in fb, so it's effectless*/
    NI_U32              u32DisplayHeight;  /**<  the height of display buf in fb. */
    NI_U32              u32ScreenWidth;    /**<  the width of screen */
    NI_U32              u32ScreenHeight;   /**<  the height of screen */
    NI_U32              u32Mask;           /**<  param modify mask bit, see NIFB_LAYER_INFO_MASKBIT*/
}NIFB_LAYER_INFO_S;

typedef enum
{
    NIFB_MIRROR_NONE = 0x0,
    NIFB_MIRROR_VERTICAL = 0x1,

    NIFB_MIRROR_BUTT
}NIFB_MIRROR_MODE_E;

/* 获取图形层的alpha参数 */
#define FBIOGET_ALPHA_NIFB                _IOR(IOC_MAGIC_NIFB, IOC_NR_NIFB_GET_ALPHA, NIFB_ALPHA_S)
/* 设置图形层的alpha参数 */
#define FBIOPUT_ALPHA_NIFB                _IOW(IOC_MAGIC_NIFB, IOC_NR_NIFB_PUT_ALPHA, NIFB_ALPHA_S)
/* 获取图形层在屏幕上的原点参数 */
#define FBIOGET_SCREEN_ORIGIN_NIFB        _IOR(IOC_MAGIC_NIFB, IOC_NR_NIFB_GET_SCREEN_ORIGIN, POINT_S)
/* 设置图形层在屏幕上的原点参数 */
#define FBIOPUT_SCREEN_ORIGIN_NIFB        _IOW(IOC_MAGIC_NIFB, IOC_NR_NIFB_PUT_SCREEN_ORIGIN, POINT_S)
/* 等待图形层垂直同步信号的到来 */
#define FBIOWAIT_VSYNC_NIFB               _IO(IOC_MAGIC_NIFB, IOC_NR_NIFB_WAIT_VSYNC)
/** 获取图形层当前的显示状态 */
#define FBIOGET_SHOW_NIFB                 _IOR(IOC_MAGIC_NIFB, IOC_NR_NIFB_PUT_SHOW, NI_BOOL)
/** 设置图形层的显示状态 */
#define FBIOPUT_SHOW_NIFB                 _IOW(IOC_MAGIC_NIFB, IOC_NR_NIFB_GET_SHOW, NI_BOOL)
/* 获取图形层能力级 */
#define FBIOGET_CAPABILITY_NIFB           _IOR(IOC_MAGIC_NIFB, IOC_NR_NIFB_GET_CAPABILITY, NIFB_CAPABILITY_S)
/* 设置图形层在屏幕上的显示大小 */
#define FBIOPUT_SCREENSIZE_NIFB           _IOW(IOC_MAGIC_NIFB, IOC_NR_NIFB_PUT_SCREENSIZE, SIZE_S)
/* 获取图形层在屏幕上的显示大小 */
#define FBIOGET_SCREENSIZE_NIFB           _IOR(IOC_MAGIC_NIFB, IOC_NR_NIFB_GET_SCREENSIZE, SIZE_S)
/* 设置图形层镜像模式,暂不支持 */
#define FBIOPUT_MIRROR_MODE_NIFB          _IOW(IOC_MAGIC_NIFB, IOC_NR_NIFB_PUT_MIRROR_MODE, NIFB_MIRROR_MODE_E)

/** To set the layer information */
#define FBIOPUT_LAYER_INFO_NIFB           _IOW(IOC_MAGIC_NIFB, IOC_NR_NIFB_PUT_LAYER_INFO, NIFB_LAYER_INFO_S)
/** To get the layer information */
#define FBIOGET_LAYER_INFO_NIFB           _IOR(IOC_MAGIC_NIFB, IOC_NR_NIFB_GET_LAYER_INFO, NIFB_LAYER_INFO_S)

#define FBIOGET_CANVAS_BUFFER_NIFB        _IOR(IOC_MAGIC_NIFB, IOC_NR_NIFB_GET_CANVAS_BUFFER, NIFB_BUFFER_S)

/** 刷新图形层的内容 */
#define FBIO_REFRESH_NIFB                 _IOW(IOC_MAGIC_NIFB, IOC_NR_NIFB_REFRESH, NIFB_BUFFER_S)
#define FBIO_WAIT_REFRESH_DONE_NIFB       _IO(IOC_MAGIC_NIFB, IOC_NR_NIFB_WAIT_REFRESH_DONE)


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

