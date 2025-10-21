/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NI_COMM_OSD_H_
#define _NI_COMM_OSD_H_

#include "ni_errno.h"
#include "ni_type.h"
#include "ni_common.h"
#include "ni_define.h"

/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */

/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */

/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */
typedef enum tagOsdErrCode {
    ERR_INVALID_HANDLE = 100,
    ERR_INVALID_MODID  = 101,
    ERR_NOT_PAINTED    = 102,
    ERR_NOT_GET_BUF    = 103,
    ERROR_TIMEOUT      = 104,
    ERR_OSD_IRQ_FAIL   = 105,

    ERR_CAL_RUTURN_NULL     = 106,
    ERR_CAL_ABN_STAT        = 107,
    ERR_CAL_ABN_CALLBACK    = 108,
    ERR_CAL_TASK_IN_RUNNING = 109,

    ERR_OSD_BUTT

} OSD_ERR_CODE_E;

#define NI_ERR_OSD_INVALID_HANDLE NI_DEF_ERR(NI_ID_OSD, EN_ERR_LEVEL_ERROR, ERR_INVALID_HANDLE)

#define NI_ERR_OSD_INVALID_MODID NI_DEF_ERR(NI_ID_OSD, EN_ERR_LEVEL_ERROR, ERR_INVALID_MODID)

#define NI_ERR_OSD_INVALID_GRPID NI_DEF_ERR(NI_ID_OSD, EN_ERR_LEVEL_ERROR, ERR_INVALID_GRPID)

#define NI_ERR_OSD_INVALID_CHNID NI_DEF_ERR(NI_ID_OSD, EN_ERR_LEVEL_ERROR, ERR_INVALID_CHNID)

#define NI_ERR_OSD_NULL_PTR NI_DEF_ERR(NI_ID_OSD, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)

#define NI_ERR_OSD_ILLEGAL_PARAM NI_DEF_ERR(NI_ID_OSD, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)

#define NI_ERR_OSD_NOMEM NI_DEF_ERR(NI_ID_OSD, EN_ERR_LEVEL_ERROR, ERR_NOMEM)

#define NI_ERR_OSD_PAL_NOT_CONFIG NI_DEF_ERR(NI_ID_OSD, EN_ERR_LEVEL_ERROR, ERR_NOT_CONFIG) /* failure caused by palette not config */

#define NI_ERR_OSD_BUF_NOT_GET NI_DEF_ERR(NI_ID_OSD, EN_ERR_LEVEL_ERROR, ERR_NOT_GET_BUF)

#define NI_ERR_OSD_HAS_EXIST NI_DEF_ERR(NI_ID_OSD, EN_ERR_LEVEL_ERROR, ERR_EXIST)

#define NI_ERR_OSD_NOT_EXIST NI_DEF_ERR(NI_ID_OSD, EN_ERR_LEVEL_ERROR, ERR_UNEXIST)

#define NI_ERR_OSD_NOT_PERM NI_DEF_ERR(NI_ID_OSD, EN_ERR_LEVEL_ERROR, ERR_NOT_PERM) /* failure caused by osd module not init */

#define NI_ERR_OSD_NOT_PAINTED NI_DEF_ERR(NI_ID_OSD, EN_ERR_LEVEL_ERROR, ERR_NOT_PAINTED) /* failure caused by osd not be painted */

#define NI_ERR_OSD_BUF_EMPTY NI_DEF_ERR(NI_ID_OSD, EN_ERR_LEVEL_ERROR, ERR_BUF_EMPTY)

#define NI_ERR_OSD_BUF_FULL NI_DEF_ERR(NI_ID_OSD, EN_ERR_LEVEL_ERROR, ERR_BUF_FULL)

#define NI_ERR_OSD_NOT_SUPPORT NI_DEF_ERR(NI_ID_OSD, EN_ERR_LEVEL_ERROR, ERR_NOT_SUPPORT)

#define NI_ERR_OSD_CAL_RETURN_NULL NI_DEF_ERR(NI_ID_OSD, EN_ERR_LEVEL_ERROR, ERR_CAL_RUTURN_NULL) /* failure caused by cal function ruturn null */

#define NI_ERR_OSD_CAL_ABN_STAT NI_DEF_ERR(NI_ID_OSD, EN_ERR_LEVEL_ERROR, ERR_CAL_ABN_STAT) /* failure caused by cal abnormal status */

#define NI_ERR_OSD_CAL_ABN_CALLBACK NI_DEF_ERR(NI_ID_OSD, EN_ERR_LEVEL_ERROR, ERR_CAL_ABN_CALLBACK) /* failure caused by cal abnormal callback run */

#define NI_ERR_OSD_CAL_TASK_IN_RUNNING NI_DEF_ERR(NI_ID_OSD, EN_ERR_LEVEL_ERROR, ERR_CAL_TASK_IN_RUNNING) /* failure caused by cal work queue in running */

#define NI_ERR_OSD_REQUEST_IRQFAIL NI_DEF_ERR(NI_ID_OSD, EN_ERR_LEVEL_ERROR, ERR_OSD_IRQ_FAIL) /* failure caused by request irq */

#define NI_ERR_OSD_TIMEOUT NI_DEF_ERR(NI_ID_OSD, EN_ERR_LEVEL_ERROR, ERROR_TIMEOUT) /* failure caused by timeout */

/* OSD透明度调节模式 */
typedef enum tagOsdAlphaMode {
    OSD_ALPHA_MODE_REGION  = 0, /* 区域透明度模式，该OSD区域内所有像素的透明度会随着区域透明度alpha的更改而变化 */
    OSD_ALPHA_MODE_PALETTE = 1, /* 调色板透明度模式，该OSD区域内的每个像素透明度会随着对应调色板的透明度ALPHA值更改而变化 */

    OSD_ALPHA_MODE_BUTT,
} OSD_ALPHA_MODE_E;

/* OSD原始数据在内存中的存放格式枚举 */
typedef enum tagOsdAccessFormat {
    OSD_FORMAT_BIT  = 0, /* 按位存放 */
    OSD_FORMAT_BYTE = 1, /* 按字节存放 */

    OSD_FORMAT_BUTT
} OSD_ACCESS_FORMAT_E;

/* OSD数据 */
typedef enum tagOsdPixelFormat {
    OSD_PIXEL_FORMAT_1BIT = 0, /* 1比特表示1个像素 */
    OSD_PIXEL_FORMAT_2BIT = 1, /* 2比特表示1个像素 */
    OSD_PIXEL_FORMAT_4BIT = 2, /* 4比特表示1个像素 */
    OSD_PIXEL_FORMAT_BYTE = 3, /* 1个Byte表示1个像素 */

    OSD_PIXEL_FORMAT_BUTT
} OSD_PIXEL_FORMAT_E;

/* OSD叠加类型 */
typedef enum tagOsdRgn {
    OSD_RGN    = 0, /* 普通OSD，只叠加到VPS通道 */
    OSD_EX_RGN = 1, /* 扩展OSD，VPS、VENC各通道均可叠加 */

    OSD_RGN_BUTT
} OSD_RGN_E;

/* OSD单元反色模式单元尺寸 */
typedef enum tagOsdInvertBlkMode {
    OSD_INVERT_BLK_NULL       = 0, /* 非单元反色模式 */
    OSD_INVERT_BLK_16_16_SIZE = 1, /* 单元反色模式，单元尺寸：16*16 */
    OSD_INVERT_BLK_32_32_SIZE = 2, /* 单元反色模式，单元尺寸：32*32 */
    OSD_INVERT_BLK_48_48_SIZE = 3, /* 单元反色模式，单元尺寸：48*48 */
    OSD_INVERT_BLK_64_64_SIZE = 4, /* 单元反色模式，单元尺寸：64*64 */

    OSD_INVERT_BlK_BUTT
} OSD_INVERT_BLK_MODE_E;

/* YUV数据分量值 */
typedef struct tagYuvColor {
    NI_U8 u8Y; /* Y分量值 */
    NI_U8 u8U; /* U分量值 */
    NI_U8 u8V; /* V分量值 */
} YUV_COLOR_S;

/* 调色板内容结构体 */
typedef struct tagPalette {
    NI_U8       u8Alpha;    /* 调色板自带透明度值，取值范围0-31,获取到的默认值为31 */
    YUV_COLOR_S stYUVColor; /* YUV分量值 ，获取到的默认颜色[0-15]分别为: 白、黑、红、粉、蓝、绿、紫、橙、青、海军蓝、灰、棕、金、橄榄、洋红、黄*/
} PALETTE_S;

/* OSD使用的调色板 */
typedef struct tagOsdPalette {
    PALETTE_S astPalette[OSD_MAX_PALETTE_NUM]; /* OSD调色板内容 */
} OSD_PALETTE_S;

/* OSD区域参数结构体 */
typedef struct tagOsdRegion {
    OSD_RGN_E enOsdRgn; /* OSD叠加类型：正常OSD或扩展OSD */

    OSD_PIXEL_FORMAT_E enPixelFormat; /* OSD数据在内存中的像素格式，输入 */
    NI_BOOL            bMacroFormat;  /* OSD数据是否宏块格式 */
} OSD_REGION_S;

/* 填充OSD数据BUFFER结构体 */
typedef struct tagOsdBufferInfo {
    NI_U32  u32PhysAddr;  /* OSD存放物理地址 */
    NI_U32 *pu32VirtAddr; /* OSD存放虚拟地址，用户使用该地址存放OSD数据 输出 */
    NI_U32  u32Len;       /* 要创建的bufer长度。驱动创建u32Len长度的buffer供用户存放osd数据*/
} OSD_BUFFER_INFO_S;

/* OSD显示属性结构体 */
typedef struct tagOsdDispAttr {
    NI_BOOL bShow; /* OSD是否显示 */

    SIZE_S                stSize;           /* OSD数据的宽高，显示 */
    POINT_S               stStartPiont;     /* OSD区域起始坐标 */
    NI_U32                u32Layer;         /* OSD所处层 */
    OSD_ALPHA_MODE_E      enAlphaMode;      /* 透明度模式选择, 0使用OSD区域的透明度，1使用调色板透明度 */
    NI_U32                u32RegionAlpha;   /* 该OSD区域透明度，enAlphaMode位0时有效 */
    NI_BOOL               bInvert;          /* 自动反色使能标记，1使能，0不使能 */
    NI_BOOL               bSpecialFrmId;    /* 是否只在特定的frame id上显示，其他帧上不显示 */
    NI_U32                u32TargetFrameId; /* OSD在特定的frame id上显示，当bSpecialFrmId真时有效 */
    OSD_INVERT_BLK_MODE_E enInvertBlkMode;  /* OSD单元反色模式 */
} OSD_DISP_ATTR_S;

/* 扩展参数结构体 */
typedef struct tagOsdExtParam {
    NI_U32  u32GlobalAlpha; /* 调节通道中所有OSD的透明度 */
    NI_BOOL bYuvOnTop;      /* 控制yuv层处于图像的最底层或最顶层 */
} OSD_EXT_PARAM_S;

/* OSD数据行格式信息结构体 */
typedef struct tagOsdFormatInfo {
    SIZE_S             stSize;          /* OSD数据的宽高，显示 */
    POINT_S            stStartPiont;    /* OSD区域起始坐标 */
    NI_U32            *pu32OsdLineAddr; /* OSD行数据存放地址 */
    OSD_PIXEL_FORMAT_E enPixelFormat;   /* OSD数据在内存中的像素格式 */
} OSD_FORMAT_INFO_S;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
