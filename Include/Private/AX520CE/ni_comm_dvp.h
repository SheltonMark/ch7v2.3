/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NI_COMM_DVP_H_
#define _NI_COMM_DVP_H_

/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "ni_errno.h"
#include "ni_type.h"
#include "ni_debug.h"
#include "ni_define.h"
#include "ni_common.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum tagDvpErrCode {
    ERR_DVP_NOT_INIT     = 64, /* failed by mod not init */
    ERR_DVP_INTERRUPT    = 65, /* failed by interrupt */
    ERR_DVP_NO_FREE_NODE = 66, /* failed by no free node */
} DVP_ERR_CODE_E;

/* open dvp device failed */
#define NI_ERR_DVP_OPEN_DEV_FAILED NI_DEF_ERR(NI_ID_DVP, EN_ERR_LEVEL_ERROR, ERR_UNEXIST)
/* failure caused by mod not be inited */
#define NI_ERR_DVP_NOT_INIT NI_DEF_ERR(NI_ID_DVP, EN_ERR_LEVEL_ERROR, ERR_DVP_NOT_INIT)
/* invlalid handle */
#define NI_ERR_DVP_INVALID_HANDLE NI_DEF_ERR(NI_ID_DVP, EN_ERR_LEVEL_ERROR, ERR_INVALID_DEVID)
/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define NI_ERR_DVP_ILLEGAL_PARAM NI_DEF_ERR(NI_ID_DVP, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)
/* using a NULL point */
#define NI_ERR_DVP_NULL_PTR NI_DEF_ERR(NI_ID_DVP, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)
/* operation is not supported by NOW */
#define NI_ERR_DVP_NOT_SUPPORT NI_DEF_ERR(NI_ID_DVP, EN_ERR_LEVEL_ERROR, ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change static attribute */
#define NI_ERR_DVP_NOT_PERM NI_DEF_ERR(NI_ID_DVP, EN_ERR_LEVEL_ERROR, ERR_NOT_PERM)
/* failure caused by malloc memory */
#define NI_ERR_DVP_NOMEM NI_DEF_ERR(NI_ID_DVP, EN_ERR_LEVEL_ERROR, ERR_NOMEM)
/* failure caused by no free node */
#define NI_ERR_DVP_NO_FREE_NODE NI_DEF_ERR(NI_ID_DVP, EN_ERR_LEVEL_ERROR, ERR_DVP_NO_FREE_NODE)
/* failure caused by request irq */
#define NI_ERR_DVP_REQUEST_IRQFAIL NI_DEF_ERR(NI_ID_DVP, EN_ERR_LEVEL_ERROR, ERR_REQ_IRQ_FAIL)
/* failure caused by timeout */
#define NI_ERR_DVP_JOB_TIMEOUT NI_DEF_ERR(NI_ID_DVP, EN_ERR_LEVEL_ERROR, ERR_TIME_OUT)
/* failure caused by interrupt */
#define NI_ERR_DVP_INTERRUPT NI_DEF_ERR(NI_ID_DVP, EN_ERR_LEVEL_ERROR, ERR_DVP_INTERRUPT)

/* Definition of the DVP handle */
typedef NI_S32 DVP_HANDLE;

/* DVP callback function */
typedef NI_VOID (*DVP_FUNC_CALLBACK)(NI_VOID *pParam1, NI_VOID *pParam2);

typedef enum tagDvpOsdPixelFmt {
    DVP_OSD_PIXEL_FMT_1BIT = 0,           /* 1比特表示1个像素，用于字符点阵 */
    DVP_OSD_PIXEL_FMT_2BIT,               /* 2比特表示1个像素，用于字符点阵 */
    DVP_OSD_PIXEL_FMT_4BIT,               /* 4比特表示1个像素，用于字符点阵 */
    DVP_OSD_PIXEL_FMT_YUV_SEMIPLANAR_420, /* YUV420图像 */

    DVP_OSD_PIXEL_FMT_BUTT
} DVP_OSD_PIXEL_FMT_E;

typedef struct tagDvpCscParam {
    NI_U32 u32KrY; /* YUV转R分量的系数0 默认：0x12a 取值范围：[0, 0x7ff] */
    NI_U32 u32KrV; /* YUV转R分量的系数1 默认：0x199 取值范围：[0, 0x7ff] */
    NI_U32 u32KgY; /* YUV转G分量的系数0 默认：0x12a 取值范围：[0, 0x7ff] */
    NI_U32 u32KgU; /* YUV转G分量的系数1 默认：0x79c 取值范围：[0, 0x7ff] */
    NI_U32 u32KgV; /* YUV转G分量的系数2 默认：0x730 取值范围：[0, 0x7ff] */
    NI_U32 u32KbY; /* YUV转B分量的系数0 默认：0x12a 取值范围：[0, 0x7ff] */
    NI_U32 u32KbU; /* YUV转B分量的系数1 默认：0x204 取值范围：[0, 0x7ff] */
} DVP_CSC_PARAM_S;

typedef struct tagDvpImage {
    NI_U16 u16Width;  /* 图像宽度 */
    NI_U16 u16Height; /* 图像高度 */

    PIXEL_FORMAT_E enPixelFmt; /* 图像格式, 输入：仅支持PIXEL_FORMAT_YUV_SEMIPLANAR_420和PIXEL_FORMAT_YUV_PLANAR_420格式，
                                            输出：XS7320支持PIXEL_FORMAT_YUV_SEMIPLANAR_420和PIXEL_FORMAT_YUV_PLANAR_420格式
                                                  XS7312支持PIXEL_FORMAT_YUV_SEMIPLANAR_420、PIXEL_FORMAT_YUV_PLANAR_420、
                                                  PIXEL_FORMAT_RGB_888和PIXEL_FORMAT_SINGLE格式 */

    NI_U32 au32PhyAddr[3]; /* 图像物理地址,依次表Y/U/V分量物理地址 */
    NI_U16 au16Stride[3];  /* 图像跨度, 依次表Y/U/V跨度 */

    NI_U8 u8IsCompress; /* 图像数据是否是压缩格式 XS7312不支持*/
} DVP_IMAGE_S;

/* 调色板内容结构体 */
typedef struct tagDvpPalette {
    NI_U8       u8Alpha;    /* 调色板透明度，值越大越不透明，取值范围: [0,31],0表完全透明，31表完全不透明 */
    COLOR_YUV_S stYUVColor; /* YUV分量值，当字符点阵为DVP_PIXEL_FMT_1BIT/2BIT/4BIT格式时，像素值对应[0,1]/[0,3]/[0,15]的颜色 */
} DVP_PALETTE_S;

/* DVP OSD透明度调节模式 */
typedef enum tagDvpOsdAlphaMode {
    DVP_OSD_ALPHA_MODE_REGION  = 0, /* 区域透明度模式，该OSD区域内所有像素的透明度会随着区域透明度alpha的更改而变化 */
    DVP_OSD_ALPHA_MODE_PALETTE = 1, /* 调色板透明度模式，该OSD区域内的每个像素透明度会随着对应调色板的透明度ALPHA值更改而变化 */

    DVP_OSD_ALPHA_MODE_BUTT,
} DVP_OSD_ALPHA_MODE_E;

/* OSD单元反色模式单元尺寸 */
typedef enum tagInvertBlkMode {
    INVERT_BLK_NULL       = 0, /* 非单元反色模式 */
    INVERT_BLK_16_16_SIZE = 1, /* 单元反色模式，单元尺寸：16*16 */
    INVERT_BLK_32_32_SIZE = 2, /* 单元反色模式，单元尺寸：32*32 */
    INVERT_BLK_48_48_SIZE = 3, /* 单元反色模式，单元尺寸：48*48 */
    INVERT_BLK_64_64_SIZE = 4, /* 单元反色模式，单元尺寸：64*64 */

    INVERT_BlK_BUTT
} INVERT_BLK_MODE_E;

/* OSD单元反色模式通路id */
typedef enum tagInvertSrcId {
    INVERT_SRC_ID_0_0 = 0, /* 开启单元反色模块通路源id */
    INVERT_SRC_ID_0_1 = 1,
    INVERT_SRC_ID_0_2 = 2,
    INVERT_SRC_ID_1_0 = 3,
    INVERT_SRC_ID_1_1 = 4,
    INVERT_SRC_ID_1_2 = 5,

    INVERT_SRC_ID_BUTT
} INVERT_SRC_ID_E;

typedef struct tagDvpOsd {
    POINT_S             stPosition; /* 将OSD图像叠加到指定的目标图像位置(stPosition) */
    NI_U16              u16Width;   /* OSD图像宽度，为0则不叠加 */
    NI_U16              u16Height;  /* OSD图像高度，为0则不叠加 */
    DVP_OSD_PIXEL_FMT_E enPixelFmt; /* OSD图像格式 */

    NI_U32 u32PhyAddr; /* OSD图像所在的物理地址 */

    DVP_OSD_ALPHA_MODE_E enAlphaMode;                     /* 透明度模式选择, 0使用OSD区域的透明度，1使用调色板透明度 */
    NI_U8                u8RegionAlpha;                   /* 区域透明度,值越大越不透明，取值范围: [0,31],0表完全透明，31表完全不透明 */
    DVP_PALETTE_S        astPalette[DVP_MAX_PALETTE_NUM]; /* 当enPixelFmt为DVP_PIXEL_FMT_1BIT/2BIT/4BIT格式时，像素值对应[0,1]/[0,3]/[0,15]的颜色 */
    NI_BOOL              bInvert;                         /* 自动反色使能标记，1使能，0不使能 */
    INVERT_BLK_MODE_E    enInvertMode;                    /* OSD单元反色模式 */
    INVERT_SRC_ID_E      enInvertId;                      /* OSD单元反色模式通路源id */
} DVP_OSD_S;

typedef struct tagDvpStrokeStyle {
    COLOR_YUV_S stLineColor;  /* 矩形框颜色, YUV色彩空间 */
    NI_U32      u32LineWidth; /* 矩形框线宽 */
} DVP_STROKE_STYLE_S;

typedef enum tagDvpDataType {
    DVP_DATA_TYPE_INT8    = 0,
    DVP_DATA_TYPE_FLOAT16 = 1,

    DVP_DATA_TYPE_BUTT
} DVP_DATA_TYPE_E;

typedef struct tagDvpNormalization {
    NI_BOOL         bEnable;        /* 是否开启减均值归一化 */
    NI_U8           u8PreShiftInt8; /* 当输出数据格式为int8时的预偏移值 */
    NI_U32          u32MeanVal;     /* 所减均值 1024量化后 */
    NI_U32          u32Coef;        /* 归一化系数,1024量化后 */
    DVP_DATA_TYPE_E enOutDataType;  /* 输出数据类型 */
} DVP_NORMALIZATION_S;

typedef struct tagDvpCmd {
    DVP_IMAGE_S stSrc; /* 源图像 */

    RECT_S stCutRect; /* 剪切属性,对源图像stSrc做剪切 */

    DVP_IMAGE_S stDst; /* 输出图像，将源图像stSrc经stCutRect参数剪切、缩放到stDst指定的区域，并叠加遮挡块astMaskRect、叠加OSD（stOsd） */

    NI_U32 u32FilterHalfWinY;  /* 缩放Y分量的滤波窗口，range:[0, 3]，默认为3。 */
    NI_U32 u32FilterHalfWinUV; /* 缩放UV分量的滤波窗口，range:[0, 1]，默认为0。 仅 AX520 支持 */

    RECT_S      astMaskRect[DVP_MAX_MASK_NUM]; /* 遮挡块, 宽高全0则不叠加 */
    COLOR_YUV_S stMaskColor;                   /* 遮挡块颜色 */

    DVP_OSD_S stOsd; /* 待叠加前景图像 */

    DVP_NORMALIZATION_S stNormalization; /* 均值归一化参数 XS7320不支持 */
} DVP_CMD_S;

typedef struct tagDvpCmdArray {
    NI_U32    u32CmdCnt;                    /* 表示astCmdArray数组中前u32CmdCnt个命令有效, 取值:[0, DVP_MAX_CMD_NUM] */
    DVP_CMD_S astCmdArray[DVP_MAX_CMD_NUM]; /* 前u32CmdCnt个有效命令，最大不超过DVP_MAX_CMD_NUM个 */

    NI_U32 u32TimeOutMSec; /* 0：非阻塞模式，函数调用会立刻返回，而不关心任务中的命令是否已经完成。
                              非0：阻塞模式，等待任务完成的超时时间，单位毫秒。当超时返回或中断返回时，已提交的操作仍会在未来的某个时刻完成。*/
} DVP_CMD_ARRAY_S;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
