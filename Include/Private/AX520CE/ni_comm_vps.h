/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef __NI_COMM_VPS_H__
#define __NI_COMM_VPS_H__

#include "ni_errno.h"
#include "ni_type.h"
#include "ni_common.h"
#include "ni_define.h"
#include "ni_common_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef enum tagVpsErrCode {
    ERR_VPS_CHN_NOT_CREATE     = 100,
    ERR_VPS_CHN_NOT_ENABLE     = 101,
    ERR_VPS_NO_STREAM          = 102,
    ERR_VPS_GET_STREAM_TIMEOUT = 103,
    ERR_VPS_IAS_HAS_NO_NODE    = 104,
    ERR_VPS_FUNC_NOT_SUPPORT   = 105,
    ERR_VPS_NO_NODE            = 106,

    ERR_VPS_BUTT

} VPS_ERR_CODE_E;

#define NI_ERR_VPS_INVALID_GRPID NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_INVALID_GRPID)

#define NI_ERR_VPS_INVALID_CHNID NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_INVALID_CHNID)

#define NI_ERR_VPS_NULL_PTR NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)

#define NI_ERR_VPS_ILLEGAL_PARAM NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)

#define NI_ERR_VPS_CHN_EXIST NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_EXIST)

#define NI_ERR_VPS_NOMEM NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_NOMEM)

#define NI_ERR_VPS_IAS_NO_STREAM NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_VPS_NO_STREAM)

#define NI_ERR_VPS_GET_STREAM_TIMEOUT NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_VPS_GET_STREAM_TIMEOUT)

#define NI_ERR_VPS_GRP_NOT_CONFIG NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_NOT_CONFIG)

#define NI_ERR_VPS_CHN_NOT_CONFIG NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_NOT_CONFIG)

#define NI_ERR_VPS_HAS_INITED NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_EXIST)

#define NI_ERR_VPS_NOT_INITED NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_UNEXIST)

#define NI_ERR_VPS_NO_NODE NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_VPS_NO_NODE)

#define NI_ERR_VPS_CHN_NOT_CREATE NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_VPS_CHN_NOT_CREATE)

#define NI_ERR_VPS_CHN_NOT_ENABLE NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_VPS_CHN_NOT_ENABLE)

#define NI_ERR_VPS_CHN_NOT_DISABLE NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_NOT_DISABLE)

#define NI_ERR_VPS_IAS_HAS_NO_NODE NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_VPS_IAS_HAS_NO_NODE)

#define NI_ERR_VPS_FUNC_NOT_SUPPORT NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_VPS_FUNC_NOT_SUPPORT)

#define NI_ERR_VPS_FUNC_NOT_PERM NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_NOT_PERM)

/* VPS工作模式枚举类型 */
typedef enum tagVpsWorkMode {
    VPS_WORK_MODE_ONLINE  = 0, /* 在线模式，ISP数据直接给到后级 */
    VPS_WORK_MODE_OFFLINE = 1, /* 离线模式，需要分配帧存，可以进行旋转、裁剪、抽帧等预处理 */

    VPS_WORK_MODE_BUTT
} VPS_WORK_MODE_E;

/* VPS旋转类型枚举 */
typedef enum tagVpsRotateType {
    VPS_ROTATE_BYPASS    = 0, /* BYPASS */
    VPS_ROTATE_MIRROR    = 1, /* 水平镜像 */
    VPS_ROTATE_FLIP      = 2, /* 垂直翻转 */
    VPS_ROTATE_90        = 3, /* 90度旋转 */
    VPS_ROTATE_180       = 4, /* 180度旋转 */
    VPS_ROTATE_270       = 5, /* 270度旋转 */
    VPS_ROTATE_MIRROR_90 = 6, /* 水平翻转+90度旋转 */
    VPS_ROTATE_FLIP_90   = 7, /* 垂直翻转+90度旋转 */

    VPS_ROTATE_BUTT
} VPS_ROTATE_TYPE_E;

/* VPS通道号枚举类型 */
typedef enum tagVpsChn {
    VPS_CHN_ENC0     = 0,
    VPS_CHN_ENC1     = 1,
    VPS_CHN_ENC2     = 2,
    VPS_CHN_DISP     = 3,
    VPS_CHN_IAS0     = 4,
    VPS_CHN_IAS1     = 5,
    VPS_CHN_VIRTIAS0 = 6, /*虚拟的IAS通道0，共用IAS0通道数据*/
    VPS_CHN_VIRTIAS1 = 7, /*虚拟的IAS通道1，共用IAS0通道数据*/
    VPS_CHN_VD       = 8,
    VPS_CHN_VIRTENC0 = 9, /* 虚拟ENC通道0，仅支持VPS_CHNFRM_MODE_MANUAL_SEND模式，灌源jpeg类型图像 */

    VPS_CHN_BUTT
} VPS_CHN_E;

/* VPS通道图像帧存管理模式枚举类型 */
typedef enum tagVpsChnFrmMode {
    VPS_CHNFRM_MODE_AUTO = 0,        /* 自动处理帧存模式。如果绑定至后级模块，vps图像直接由底层驱动送至后级模块。
                                      * 如果不绑定至后级模块，则用户可调用NI_MDK_VPS_GetFrame获取图像 */
    VPS_CHNFRM_MODE_MANUAL = 1,      /* 用户手动处理帧存模式。如果绑定至后级模块，需要用户调用NI_MDK_VPS_GetFrame获取图像，
                                      * 并在处理完后调用NI_MDK_VPS_SendChnFrame送回，否则后级模块将收不到图像。
                                      * 如果不绑定至后级模块，则用户可调用NI_MDK_VPS_GetFrame获取图像。
                                      * 如果用户需要在不绑定至后级模块情况下获取联动帧，则需要使用该模式 */
    VPS_CHNFRM_MODE_MANUAL_SEND = 2, /* 用户通道灌源模式。用户可直接调用NI_MDK_VPS_SendChnFrame将图像送至后级模块 */

    VPS_CHNFRM_MODE_BUTT
} VPS_CHNFRM_MODE_E;

/* VPS IAS联动对应编码通道号枚举类型 */
typedef enum tagVpsChnIasRelate2Enc {
    VPS_CHN_IAS_RELATE2NONE = 0, /* IAS通道不与编码通道联动 */
    VPS_CHN_IAS_RELATE2ENC0,     /* IAS通道与编码通道0联动 */
    VPS_CHN_IAS_RELATE2ENC1,     /* IAS通道与编码通道1联动 */
    VPS_CHN_IAS_RELATE2ENC2,     /* IAS通道与编码通道2联动 */

    VPS_CHN_IAS_RELATE_BUTT
} VPS_CHN_IAS_RELATE_E;

/* VPS通道属性枚举类型 */
typedef enum tagVpsChnAttrType {
    VPS_CHN_ATTR_TYPE_ENC  = 0, /* 编码 */
    VPS_CHN_ATTR_TYPE_DISP = 1, /* 显示 */
    VPS_CHN_ATTR_TYPE_IAS  = 2, /* 智能 */
    VPS_CHN_ATTR_TYPE_VD   = 3, /* 视频检测 */

    VPS_CHN_ATTR_TYPE_BUTT
} VPS_CHN_ATTR_TYPE_E;

/* VPS用户获取GRP图像模式枚举类型 */
typedef enum tagVpsGetGrpFrmMode {
    VPS_GETGRPFRM_MODE_NONE         = 0, /* 用户不获取vps grg图像,由底层sdk将yuv传给下级处理模块 */
    VPS_GETGRPFRM_MODE_AFTER_ORIGIN = 1, /* 用户从原始位置获取vps grp图像,该模式下需要用户调用NI_MDK_VPS_GetGrpFrame，并在处理完后调用NI_MDK_VPS_SendGrpFrame送回，否则将因收不到yuv图像而断流 */

    VPS_GETGRPFRM_MODE_BUTT
} VPS_GETGRPFRM_MODE_E;

/* VPS模块参数结构体 */
typedef struct tagVpsModParam {
    NI_S32 s32AilgnIas;    /* IAS通道输出的每个平面的地址均需要64对齐，默认值：64 */
    NI_S32 s32EncPrivPool; /* enc通道使用公共换存池还是私有缓存池，0:私有公共换存池，1：私有缓存池，默认值:0 */
    NI_U32 u32FpsDenom;    /* 帧率的分母值，实际帧率是用户配置帧率除以分母 , 默认值为：0 */
} VPS_MOD_PARAM_S;

/* VPS剪切区域参数结构体 */
typedef struct tagVpsCrop {
    NI_BOOL bEnable;
    RECT_S  stRect;
} VPS_CROP_S;

/* 定义多路拼接模式*/
typedef enum tagVpsStitchMode {
    VPS_STITCH_MODE_BLEND       = 0, /* 水平融合拼接模式，在拼接处做融合处理，该模式目前不支持 */
    VPS_STITCH_MODE_BLEND_VER   = 1, /* 垂直融合拼接模式，在拼接处做融合处理，该模式目前不支持 */
    VPS_STITCH_MODE_NOBLEND_HOR = 2, /* 水平无融合拼接模式，将两路图像水平放在一起，拼接处不做融合处理，该模式目前不支持 */
    VPS_STITCH_MODE_NOBLEND_VER = 3, /* 垂直无融合拼接模式，将两路图像垂直放在一起，拼接处不做融合处理 */
    VPS_STITCH_MODE_BUTT
} VPS_STITCH_MODE_E;

/* 定义各种拼接模式下内存使用情况*/
typedef enum tagVpsStitchBufMode {
    VPS_STITCH_BUF_MODE_DST        = 0, /* GRPx源内存申请参照分辨率进行申请，Dst申请的内存为GRPx源内存的2倍，该模式目前不支持 */
    VPS_STITCH_BUF_MODE_NODST_UP   = 1, /* GRPx源内存申请参照分辨率的2倍申请，GRPx源数据存放在上半部分，Dst不再申请内存，仅垂直拼接有效 */
    VPS_STITCH_BUF_MODE_NODST_DOWN = 2, /* GRPx源内存申请参照分辨率的2倍申请，GRPx源数据存放在下半部分，Dst不再申请内存，仅垂直拼接有效 */
    VPS_STITCH_BUF_MODE_BUTT
} VPS_STITCH_BUF_MODE_E;

/* VPS模块离线模式下的组属性参数结构体 */
typedef struct tagVpsOffLineAttr {
    NI_BOOL               bCropEn;         /* 离线模式下,图像剪切使能 */
    VPS_GETGRPFRM_MODE_E  enGetGrpFrmMode; /* 离线模式下用户获取grp图像的模式 */
    RECT_S                stCropRect;      /* 离线模式下,图像剪切参数 */
    NI_U32                u32OutFps;       /* 离线模式下,抽帧后输出帧率, Range:[VPS_MIN_INPUT_FPS, VPS_MAX_INPUT_FPS] */
    NI_U32                u32BufNum;       /* 离线模式下，缓存块数 */
    NI_BOOL               bStitchMode;     /* 是否拼接模式，所有GRP的双拼配置必须一样 */
    VPS_STITCH_MODE_E     enStitchMode;    /* 如果是双拼，进行双拼模式的选择。注：当 bStitchMode为FALSE时，该参数无效,所有GRP的双拼配置必须一样 */
    VPS_STITCH_BUF_MODE_E enStitchBufMode; /* 目前仅供垂直拼接使用，水平拼接使用默认设置即可。注：当 bStitchMode为FALSE时，该参数无效，所有GRP的双拼配置必须一样 */
} VPS_OFFLINE_ATTR_S;

/* VPS模块组属性参数结构体 */
typedef struct tagVpsGrpAttr {
    NI_U32             u32InWidth;    /* ISP输入图像宽 */
    NI_U32             u32InHeight;   /* ISP输入图像高 */
    NI_U32             u32InFps;      /* ISP输入图像帧率, Range:[VPS_MIN_INPUT_FPS, VPS_MAX_INPUT_FPS] */
    VPS_WORK_MODE_E    enWorkMode;    /* 所有Grp必须配相同的模式 */
    VPS_OFFLINE_ATTR_S stOffLineAttr; /* enWorkMode为VPS_WORK_MODE_OFFLINE时有效，离线模式配置参数 */
    NI_BOOL            bMultiEye;     /* 是否应用于多目, 所有Grp配置必须一致 */
} VPS_GRP_ATTR_S;

/* VPS隐私遮挡设置参数结构体 */
typedef struct tagVpsMaskAttr {
    COLOR_YUV_S stMaskColor;
    RECT_S      astMaskRect[VPS_MAX_CHN_MASK_NUM]; /* 遮挡坐标 */
} VPS_MASK_ATTR_S;

/* VPS通道编码配置参数结构体 */
typedef struct tagVpsEncChnAttr {
    PAYLOAD_TYPE_E    enType;           /* vps通道编码类型，需与后级模块编码类型一致,如果同时绑定H26x和JEPG格式编码器，则以H26x为准 */
    NI_U32            u32YStride;       /* 亮度stride,配置为0表示不使用stride */
    NI_U32            u32CStride;       /* 色度stride,planar模式下uv分量相同，配置为0表示不使用stride */
    PIXEL_FORMAT_E    enOutFormat;      /* 数据输出格式 */
    NI_U32            u32OutFps;        /* 编码通道输出帧率, Range:[VPS_MIN_INPUT_FPS, VPS_MAX_INPUT_FPS] */
    NI_BOOL           bCompress;        /* 有损压缩 */
    VPS_WORK_MODE_E   enWorkMode;       /* 编码通道工作模式 */
    NI_U32            u32OfflineBufNum; /* 离线模式下，缓存块数 */
    NI_U32            u32BufLineNum;    /* 离线单帧存下，buf使用行数，仅在enWorkMode=VPS_WORK_MODE_OFFLINE且u32OfflineBufNum=1时该参数有效 */
    VPS_CHNFRM_MODE_E enFrmMode;        /* 帧存控制模式 */
} VPS_ENC_CHN_ATTR_S;

/* VPS帧数据信息结构体 */
typedef struct tagVpsFreamInfo {
    NI_U32        u32TimeStamp;
    NI_U32        u32FrameSize;     /* VideoFrame[0]对应的帧数据占用的内存size，字节数 */
    NI_U64        u64GrpCapPts;     /* VPS GRP获取该帧的时间信息，用于与3A数据同步。仅VPS GRP设置为离线软件模式下有用 */
    NI_BOOL       bRelateFrmValid;  /* IAS联动帧VideoFrame[1]是否有效 */
    VIDEO_FRAME_S astVideoFrame[2]; /* IAS0通道对应的联动通道数据存在数组1中 */
} VPS_FRAME_INFO_S;

/* VPS滤波窗口能力等级枚举类型，等级越高，滤波能力越差 */
typedef enum tagVpsFilterWinType {
    VPS_FILTER_WIN_TYPE_LEVEL_0 = 0,
    VPS_FILTER_WIN_TYPE_LEVEL_1,
    VPS_FILTER_WIN_TYPE_LEVEL_2,
    VPS_FILTER_WIN_TYPE_LEVEL_3,
    VPS_FILTER_WIN_TYPE_LEVEL_4,
    VPS_FILTER_WIN_TYPE_LEVEL_5,
    VPS_FILTER_WIN_TYPE_LEVEL_6,
    VPS_FILTER_WIN_TYPE_LEVEL_7,

    VPS_FILTER_WIN_TYPE_BUTT
} VPS_FILTER_WIN_TYPE_E;

/* VPS色彩空间转换参数结构体 */
typedef struct tagVpsCscParam {
    NI_U32 u32KrY; /* YUV转R分量的系数0，默认为298（放大256倍） */
    NI_U32 u32KrV; /* YUV转R分量的系数1，默认为409（放大256倍） */
    NI_U32 u32KgY; /* YUV转G分量的系数0，默认为298（放大256倍） */
    NI_U32 u32KgU; /* YUV转G分量的系数1，默认为1948（放大256倍） */
    NI_U32 u32KgV; /* YUV转G分量的系数2，默认为1840（放大256倍) */
    NI_U32 u32KbY; /* YUV转B分量的系数0，默认为298（放大256倍） */
    NI_U32 u32KbU; /* YUV转B分量的系数1，默认为516（放大256倍） */
} VPS_CSC_PARAM_S;

/* VPS数据类型枚举类型 */
typedef enum tagVpsDataType {
    VPS_DATA_TYPE_INT8    = 0,
    VPS_DATA_TYPE_FLOAT16 = 1,

    VPS_DATA_TYPE_BUTT
} VPS_DATA_TYPE_E;

/* VPS减均值、归一化参数结构体 */
typedef struct tagVpsNormalizationParam {
    NI_BOOL         bEnable;        /* 是否开启减均值归一化 */
    NI_U8           u8PreShiftInt8; /* 当输出数据格式为int8时的预偏移值 */
    NI_U32          u32MeanVal;     /* 所减均值 1024量化后 */
    NI_U32          u32Coef;        /* 归一化系数,1024量化后 */
    VPS_DATA_TYPE_E enOutDataType;  /* 输出数据类型 */
} VPS_NORMALIZATION_PARAM_S;

/* VPS IAS通道属性参数结构体 */
typedef struct tagVpsIasChnAttr {
    PIXEL_FORMAT_E enOutFormat;                /* IAS 数据输出格式 */
    NI_U32         u32BuffNum;                 /* 使用的buff数 */
    NI_U32         u32OutFps;                  /* 输出帧率, Range:[VPS_MIN_INPUT_FPS, VPS_MAX_INPUT_FPS],
                                                  是否应用于多目, 所有Grp配置必须一致，所有ias通道的数据来源，虚拟通道还可以进一步软抽帧 */
    NI_U32                    au32Stride[3];   /* 通道对应的stride0 stride1 stride2 */
    VPS_CHN_IAS_RELATE_E      enIasRel2EncChn; /* IAS联动对应的编码通道 */
    VPS_FILTER_WIN_TYPE_E     enFilterWin;     /* 滤波窗口类型 */
    VPS_NORMALIZATION_PARAM_S stNormalization; /* 均值归一化参数 */
} VPS_IAS_CHN_ATTR_S;

/* VPS VD通道属性参数结构体 */
typedef struct tagVpsVdChnAttr {
    NI_U32                u32BuffNum;  /* 使用的buff数 */
    NI_U32                u32OutFps;   /* 输出帧率, Range:[VPS_MIN_INPUT_FPS, VPS_MAX_INPUT_FPS] */
    NI_U32                u32Stride;   /* Y分量stride */
    VPS_FILTER_WIN_TYPE_E enFilterWin; /* 滤波窗口类型 */
} VPS_VD_CHN_ATTR_S;

/* VPS模块通道属性参数结构体 */
typedef struct tagVpsChnAttr {
    NI_BOOL bCropEn;      /* VPS通道剪切使能开关 */
    RECT_S  stCropRect;   /* VPS通道剪切参数，当bCutEn为TRUE时有效 */
    NI_U32  u32OutWidth;  /* VPS通道输出图像宽 */
    NI_U32  u32OutHeight; /* VPS通道输出图像高 */

    VPS_CHN_ATTR_TYPE_E enAttrType; /* 选择下面union联合体的属性类型 */
    union {
        VPS_ENC_CHN_ATTR_S stEncAttr; /* UNION:enAttrType=VPS_CHN_ATTR_TYPE_ENC|编码通道 */
        VPS_IAS_CHN_ATTR_S stIasAttr; /* UNION:enAttrType=VPS_CHN_ATTR_TYPE_IAS|智能通道 */
        VPS_VD_CHN_ATTR_S  stVdAttr;  /* UNION:enAttrType=VPS_CHN_ATTR_TYPE_VD|动检通道 */
    };
} VPS_CHN_ATTR_S;

/* VPS用户送至组的图像帧信息结构体 */
typedef struct tagVpsSendGrpFrm {
    NI_BOOL       bCropEn;
    RECT_S        stCropRect; /* 剪切参数 */
    VIDEO_FRAME_S stVideoFrame;
} VPS_SEND_GRPFRM_S;

/* VPS用户送至通道的图像帧信息结构体 */
typedef struct tagVpsSendChnFrm {
    NI_BOOL       bCropEn;    /* 剪切使能，该参数无效 */
    RECT_S        stCropRect; /* 剪切参数，该参数无效 */
    VIDEO_FRAME_S stVideoFrame;
} VPS_SEND_CHNFRM_S;

typedef struct tagVpsRotateParam {
    NI_U32 u32WidthAll;  /* RW, 旋转后，对后级输出的总宽, 配置0，则内部自适应分配 */
    NI_U32 u32HeightAll; /* RW, 旋转后，对后级输出的总高， 配置0，则内部自适应分配 */
    NI_U16 u16WtM1A;     /* RW, U16.0*/
    NI_U16 u16WtM5A;     /* RW, U16.0*/
    NI_U16 u16NAd1A;     /* RW, U12.0*/
    NI_U16 u16NAd2A;     /* RW, U12.0*/
    NI_U16 u16WrM1A;     /* RW, U16.0*/
    NI_U16 u16WdXM1A;    /* RW, U16.0*/
} VPS_ROTATE_PARAM_S;

/* VPS模块组设置帧率信息 */
typedef struct tagVpsAllFpsAttr {
    NI_U32 u32GrpInFps;                        /* GRP输入图像帧率, Range:[VPS_MIN_INPUT_FPS, VPS_MAX_INPUT_FPS]       */
    NI_U32 u32GrpOutFps;                       /* GRP的输出帧率, Range:[VPS_MIN_INPUT_FPS, VPS_MAX_INPUT_FPS],
                                                  必须小于等于输入帧率u32GrpInFps,离线模式下有效 */
    NI_U32 au32ChnOutFps[VPS_GRP_MAX_CHN_NUM]; /* 通道帧率, Range:[VPS_MIN_INPUT_FPS, VPS_MAX_INPUT_FPS],
                                                  不包括虚拟通道，必须小于等于组的输出帧率u32GrpOutFps */
} VPS_ALL_FPS_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
