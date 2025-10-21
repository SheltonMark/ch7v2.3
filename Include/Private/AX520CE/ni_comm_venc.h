/**************************************************************************************************
*
* Copyright (c) 2019-2025 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef __NI_COMM_VENC_H__
#define __NI_COMM_VENC_H__

#include "ni_errno.h"
#include "ni_common.h"
#include "ni_define.h"
#include "ni_private_venc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef enum tagVencErrCode {
    ERR_VENC_CHN_EXIST            = 64, /* device or channel has existed */
    ERR_VENC_NOT_INIT             = 65, /* device or channel not initialized */
    ERR_VENC_IRQ_FAIL             = 66, /* device request irq failure */
    ERR_VENC_HAS_STARTED          = 67, /* device or channel has been started*/
    ERR_VENC_NOT_START            = 68, /* device or channel not start */
    ERR_VENC_STREAM_ERR           = 69, /* stream data error */
    ERR_VENC_NO_STREAM            = 70, /* NO stream  */
    ERR_VENC_BUSY                 = 71, /* VENC is busy, please wait a moment, then try again */
    ERR_VENC_GET_STREAM_TIMEOUT   = 72, /* Get Stream Timeout, try again */
    ERR_VENC_CALLFUNCTION         = 73, /* Call other module callback function fail */
    ERR_VENC_BUF_EXIST            = 74, /* buf exist */
    ERR_VENC_BUF_UNEXIST          = 75, /* buf unexist */
    ERR_VENC_WAIT_ENCDONE_TIMEOUT = 76

} VENC_ERR_CODE_E;

/* invlalid channel ID */
#define NI_ERR_VENC_INVALID_CHNID NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_INVALID_CHNID)
/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define NI_ERR_VENC_ILLEGAL_PARAM NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)
/* channel exists */
#define NI_ERR_VENC_CHN_EXIST NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_EXIST)
/* channel unexists */
#define NI_ERR_VENC_CHN_UNEXIST NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_UNEXIST)
/* using a NULL point */
#define NI_ERR_VENC_NULL_PTR NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)
/* try to enable or initialize system,device or channel, before configing attribute */
#define NI_ERR_VENC_NOT_CONFIG NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_NOT_CONFIG)
/* operation is not supported by NOW */
#define NI_ERR_VENC_NOT_SUPPORT NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change static attribute */
#define NI_ERR_VENC_NOT_PERM NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_NOT_PERM)
/* failure caused by malloc memory */
#define NI_ERR_VENC_NOMEM NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_NOMEM)
/* failure caused by malloc buffer */
#define NI_ERR_VENC_NOBUF NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_NOBUF)
/* no data in buffer */
#define NI_ERR_VENC_BUF_EMPTY NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_BUF_EMPTY)
/* no buffer for new data */
#define NI_ERR_VENC_BUF_FULL NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_BUF_FULL)
/* failure caused by not initialize module */
#define NI_ERR_VENC_NOT_INIT NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_VENC_NOT_INIT)
/* failure caused by request irq */
#define NI_ERR_VENC_REQUEST_IRQFAIL NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_VENC_IRQ_FAIL)
/* failure caused by enc has started encoding */
#define NI_ERR_VENC_CHN_HAS_STARTED NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_VENC_HAS_STARTED)
/* failure caused by enc has started encoding */
#define NI_ERR_VENC_CHN_NOT_START NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_VENC_NOT_START)
/* failure caused by stream err */
#define NI_ERR_VENC_STREAM_ERR NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_VENC_STREAM_ERR)
/* failure caused by no stream */
#define NI_ERR_VENC_NO_STREAM NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_VENC_NO_STREAM)
/* failure caused by get stream timeout */
#define NI_ERR_VENC_GET_STREAM_TIMEOUT NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_VENC_GET_STREAM_TIMEOUT)
/* VENC is busy, please wait a moment, then try again */
#define NI_ERR_VENC_BUSY NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_VENC_BUSY)
/* venc call other module callback function fail */
#define NI_ERR_VENC_CALLFUNCTION NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_VENC_CALLFUNCTION)
/* buf exists */
#define NI_ERR_VENC_CHN_BUF_EXIST NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_VENC_BUF_EXIST)
/* buf unxists */
#define NI_ERR_VENC_CHN_BUF_UNEXIST NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_VENC_BUF_UNEXIST)
/* wait encode done timeout */
#define NI_ERR_VENC_CHN_WAIT_ENCDONE_TIMEOUT NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_VENC_WAIT_ENCDONE_TIMEOUT)

#define VENC_STREAM_MAX_PACK_NUM (3)

/* rotate type  */
typedef enum tagVencRotateType {
    VENC_ROTATE_BYPASS    = 0, /* BYPASS */
    VENC_ROTATE_MIRROR    = 1, /* 水平镜像 */
    VENC_ROTATE_FLIP      = 2, /* 垂直翻转 */
    VENC_ROTATE_90        = 3, /* 顺时针90度旋转 */
    VENC_ROTATE_180       = 4, /* 顺时针180度旋转 */
    VENC_ROTATE_270       = 5, /* 顺时针270度旋转 */
    VENC_ROTATE_MIRROR_90 = 6, /* 水平镜像+顺时针90度旋转 */
    VENC_ROTATE_FLIP_90   = 7, /* 垂直翻转+顺时针90度旋转 */

    VENC_ROTATE_BUTT
} VENC_ROTATE_TYPE_E;

/* 帧类型枚举*/
typedef enum tagVencEncFrameType {
    VENC_FRAME_TYPE_I   = 0xFD, /* I帧 */
    VENC_FRAME_TYPE_P   = 0xFC, /* P帧 */
    VENC_FRAME_TYPE_B   = 0xFE, /* B帧 */
    VENC_FRAME_TYPE_SP  = 0xFB, /* SP帧，短参考帧 */
    VENC_FRAME_TYPE_LTR = 0xFF, /* 长参考帧 */

    VENC_FRAME_TYPE_BUTT
} VENC_FRAME_TYPE_E;

/* 定义H.26X编码档次 */
typedef enum tagVencProfile {
    VENC_H265_MAIN_PROFILE               = 0, /* main profile */
    VENC_H265_MAIN_STILL_PICTURE_PROFILE = 1, /* main still picture profile */
    VENC_H265_MAIN_10_PROFILE            = 2, /* main 10 profile, not support now */

    VENC_H264_BASE_LINE_PROFILE = 66,  /* baseline profile */
    VENC_H264_MAIN_PROFILE      = 77,  /* main profile */
    VENC_H264_EXTEND_PROFILE    = 88,  /* extend profile, not support now */
    VENC_H264_HIGH_PROFILE      = 100, /* high profile */

    VENC_PROFILE_BUTT
} VENC_PROFILE_E;

/* 定义编码器码流buf模式.  */
typedef enum tagVencStreamBufMode {
    VENC_STREAM_RING_BUF_MODE = 0, /* stream buf is ring mode */
    VENC_STREAM_LINE_BUF_MODE = 1, /* stream buf is line mode */

    VENC_STREAM_BUF_MODE_BUTT
} VENC_STREAM_BUF_MODE_E;

/* 定义码流校验方式 */
typedef enum tagVencStreamCheckType {
    VENC_STREAM_CHECK_OFF = 0, /* 关闭码流校验功能 */
    VENC_STREAM_CHECK_SUM = 1, /* 以累加和校验方式生成码流校验码 */
    VENC_STREAM_CHECK_CRC = 2, /* 以CRC校验方式生成码流校验码 */

    VENC_STREAM_CHECK_BUTT
} VENC_STREAM_CHECK_TYPE_E;

/* 参考帧/重构帧压缩 */
typedef enum tagVencCompress {
    VENC_COMPRESS_NONE   = 0, /* 不开启压缩 */
    VENC_COMPRESS_LUMA   = 1, /* 仅亮度压缩 */
    VENC_COMPRESS_CHROMA = 2, /* 仅色度压缩 */
    VENC_COMPRESS_BOTH   = 3, /* 亮度和色度均压缩 */

    VENC_COMPRESS_BUTT
} VENC_COMPRESS_E;

/* RC 模式选择 */
typedef enum tagVencRcMode {
    VENC_RC_MODE_CBR   = 0, /* CBR模式，在码率统计时间内保证编码码率稳定。 */
    VENC_RC_MODE_VBR   = 1, /* VBR模式，允许在码率统计时间内编码码率波动，从而保证编码图像质量平稳。 */
    VENC_RC_MODE_FIXQP = 2, /* 固定QP模式 */
    VENC_RC_MODE_CVBR  = 3, /* CVBR模式，以 VBR 为基础，旨在提供平稳的图像质量的码控算法，同时对 VBR 的
                              码率进行限制，以满足传输带宽以及存储空间的要求。
                              瞬时码率：保证网络带宽传输。
                              长期码率：长期录制时保证存储空间。
                              短期码率：保证复杂场景下的图像质量。*/
    VENC_RC_MODE_AVBR = 4,  /* AVBR模式，自适应可变比特率，允许在码率统计时间内编码码率波动，从而保证
                              编码图像质量平稳。码率控制内部会检测当前场景的运动静止状态，在运动时采
                              用较高码率编码，静止时主动降低码率。*/
    VENC_RC_MODE_QVBR = 5,  /* QVBR模式，基于主观图像质量的可变比特率，该码控调节方式是利用实时统计
                              的 PSNR（图像质量客观评价指标）的大小来动态调整码率，从而保证编码图像质量平稳。 */
    VENC_RC_MODE_BUTT
} VENC_RC_MODE_E;

/* 熵编码模式 */
typedef enum tagVencEntropyEncMode {
    VENC_ENTROPY_ENC_MODE_CAVLC = 0, /* CAVLC */
    VENC_ENTROPY_ENC_MODE_CABAC = 1, /* CABAC */

    VENC_ENTROPY_ENC_MODE_BUTT
} VENC_ENTROPY_ENC_MODE_E;

/* 定义H26x熵编码结构体 */
typedef struct tagVencH26xEntropy {
    VENC_ENTROPY_ENC_MODE_E enEntropyEncMode; /* 熵编码模式 */
    NI_U32                  u32CabacInitFlag; /* CABAC Tabble Init flag*/

} VENC_H26X_ENTROPY_S;

/* 视频编码图像裁剪属性 */
typedef struct tagVencCropAttr {
    NI_BOOL bEnable; /* 视频编码图像裁剪使能 */
    RECT_S  stRect;  /* 图像裁剪矩形区域 */
} VENC_CROP_ATTR_S;

/* 视频编码帧头帧尾空间大小属性*/
typedef struct tagVencStreamHeadAttr {
    NI_U32 u32StrmHeadSize; /* 码流帧头空间大小，单位byte，最大支持32KB。 */
    NI_U32 u32StrmTailSize; /* 码流帧尾空间大小，单位byte，最大支持32KB。 */
} VENC_STRM_HEAD_ATTR_S;

/* 视频码流buffer属性结构体 */
typedef struct tagVencStreamBuf {
    NI_PHYS_ADDR_T PhyAddr;
    NI_SIZE_T      szBufSize;
} VENC_STREAM_BUF_S;

/* 定义固定码率控制结构体属性类型 */
typedef struct tagRcAttrCbr {
    NI_U32   u32TargetBitRate; /* 在StatTime时间内的目标码率, [10,500000]kbps */
    NI_U32   u32StatTime;      /* 码率统计时间，以秒为单位, 取值范围[1,10]。ax520上该参数无效 */
    NI_U32   u32MinIQp;        /* I帧最小QP,取值范围[0,51], 建议值10 */
    NI_U32   u32MaxIQp;        /* I帧最大QP,取值范围[u32MinIQp,51]，建议值51 */
    NI_U32   u32MinPQp;        /* P帧最小QP,取值范围[0,51], 建议值10 */
    NI_U32   u32MaxPQp;        /* P帧最大QP,取值范围[u32MinPQp,51]，建议值51 */
    NI_VOID *pPrivData;
} VENC_RC_ATTR_CBR_S;

/* 定义可变码率控制结构体属性类型 */
typedef struct tagRcAttrVbr {
    NI_U32   u32MaxBitRate; /* 在StatTime时间内的最大码率，[10,500000]kbps */
    NI_U32   u32StatTime;   /* 码率统计时间，以秒为单位，取值范围[1,10]。ax520上该参数无效。 */
    NI_S32   s32ChangePos;  /* 码率调整的压力位，取值范围[1,100],建议值85 */
    NI_U32   u32MinIQp;     /* I帧最小QP,取值范围[0,51], 建议值10 */
    NI_U32   u32MaxIQp;     /* I帧最大QP,取值范围[u32MinIQp,51]，建议值51 */
    NI_U32   u32MinPQp;     /* P帧最小QP,取值范围[0,51], 建议值10 */
    NI_U32   u32MaxPQp;     /* P帧最大QP,取值范围[u32MinPQp,51]，建议值51 */
    NI_VOID *pPrivData;
} VENC_RC_ATTR_VBR_S;

typedef struct tagRcAttrCVbr {
    NI_U32   u32MaxBitRate;         /* 在StatTime时间内的最大码率, [10,500000]kbps */
    NI_U32   u32StatTime;           /* 码率统计时间，以秒为单位, 取值范围[1,10]。ax520上该参数无效。*/
    NI_U32   u32MinIQp;             /* I帧最小QP,取值范围[0,51], 建议值10 */
    NI_U32   u32MaxIQp;             /* I帧最大QP,取值范围[u32MinIQp,51]，建议值51 */
    NI_U32   u32MinPQp;             /* P帧最小QP,取值范围[0,51], 建议值10 */
    NI_U32   u32MaxPQp;             /* P帧最大QP,取值范围[u32MinPQp,51]，建议值51 */
    NI_U32   u32ShortTermStatTime;  /* 码率短期统计时间。ax520上该参数无效。 */
    NI_U32   u32LongTermStatTime;   /* 码率长期统计时间。ax520上该参数无效。 */
    NI_U32   u32LongTermMaxBitRate; /* 编码器输出长期最大码率，以 kbps 为单位。ax520上该参数无效。 */
    NI_U32   u32LongTermMinBitRate; /* 编码器输出长期最小码率，以 kbps 为单位。ax520上该参数无效。 */
    NI_VOID *pPrivData;
} VENC_RC_ATTR_CVBR_S;

typedef struct tagRcAttrAVbr {
    NI_U32   u32MaxBitRate; /* 在StatTime时间内的最大码率，[10,500000]kbps */
    NI_U32   u32StatTime;   /* 码率统计时间，以秒为单位，取值范围[1,10]。ax520上该参数无效。 */
    NI_S32   s32ChangePos;  /* 码率调整的压力位，取值范围[1,100]，建议值85 */
    NI_U32   u32MinIQp;     /* I帧最小QP,取值范围[0,51], 建议值10 */
    NI_U32   u32MaxIQp;     /* I帧最大QP,取值范围[u32MinIQp,51]，建议值51 */
    NI_U32   u32MinPQp;     /* P帧最小QP,取值范围[0,51], 建议值10 */
    NI_U32   u32MaxPQp;     /* P帧最大QP,取值范围[u32MinPQp,51]，建议值51 */
    NI_VOID *pPrivData;
} VENC_RC_ATTR_AVBR_S;

typedef struct tagRcAttrQVbr {
    NI_U32 u32MaxBitRate;     /* 在StatTime时间内的最大码率，[10,500000]kbps */
    NI_U32 u32StatTime;       /* 码率统计时间，以秒为单位，取值范围[1,10]。ax520上该参数无效。 */
    NI_S32 s32ChangePos;      /* 码率调整的压力位，取值范围[1,100]，建议值85。运动场景下码流基本维持在
                                u32MaxBitRate * s32ChangePos的平均码率，比如运动场景下想得到1024k的平均码率，changepos=85的
                                情况下，则将maxbitrate配成1204k。低码率时，为使运动场景主观更优，建议将最高码率适当调高。 */
    NI_U32 u32MinIQp;         /* I帧最小QP,取值范围[0,51], 建议值10 */
    NI_U32 u32MaxIQp;         /* I帧最大QP,取值范围[u32MinIQp,51]，建议值51 */
    NI_U32 u32MinPQp;         /* P帧最小QP,取值范围[0,51], 建议值10 */
    NI_U32 u32MaxPQp;         /* P帧最大QP,取值范围[u32MinPQp,51]，建议值51 */
    NI_U32 s32MinBitratePos;  /* 取值范围[0,100]， 相对于max bitrate 的百分比，
                                需要比s32ChangePos比例小，用来控制最低码率，建议值0。*/
    NI_S32   s32BestQuality;  /* 允许的最优质量[5,500]。值越小表示质量越好，建议值20。 */
    NI_S32   s32WorstQuality; /* 最差质量[5,500]。建议值21，值越小表示质量越好，该值需大于BestQuality */
    NI_VOID *pPrivData;
} VENC_RC_ATTR_QVBR_S;

/* 定义固定QP码率控制结构体属性类型 */
typedef struct tagRcAttrFixQp {
    NI_U32 u32FixedIQp; /* I帧固定QP值，Jpeg编码时该参数用于设置quality值 */
    NI_U32 u32FixedPQp; /* P帧固定QP值，Jpeg编码可忽略，只要设u32FixedIQp即可 */
} VENC_RC_ATTR_FIXQP_S;

/* 定义码率控制结构体属性类型 */
typedef struct tagVencRcAttr {
    VENC_RC_MODE_E enRcMode;        /* 码率控制模式选择。H26x编码通道模式切换需要先关闭通道使能。ax520不支持avbr模式。 */
    NI_U32         u32FrmRateNum;   /* 输入帧率的分子，帧率计算公式: u32FrmRateNum/u32FrmRateDenom, range:[1, 1048575] */
    NI_U32         u32FrmRateDenom; /* 输入帧率的分母，取值必须小于等于u32FrmRateNum, range:[1, 65535] */
    union {
        VENC_RC_ATTR_CBR_S   stAttrCbr;   /* UNION: enRcMode=VENC_RC_MODE_CBR|CBR */
        VENC_RC_ATTR_VBR_S   stAttrVbr;   /* UNION: enRcMode=VENC_RC_MODE_VBR|VBR */
        VENC_RC_ATTR_CVBR_S  stAttrCVbr;  /* UNION: enRcMode=VENC_RC_MODE_CVBR|CVBR */
        VENC_RC_ATTR_AVBR_S  stAttrAVbr;  /* UNION: enRcMode=VENC_RC_MODE_AVBR|AVBR */
        VENC_RC_ATTR_QVBR_S  stAttrQVbr;  /* UNION: enRcMode=VENC_RC_MODE_QVBR|QVBR */
        VENC_RC_ATTR_FIXQP_S stAttrFixQp; /* UNION: enRcMode=VENC_RC_MODE_FIXQP|FIXQP */
    };
} VENC_RC_ATTR_S;

/* 定义RC高级属性参数 */
typedef struct tagVencRcParam {
    RC_PARAM_H26X_S  stRcParamH26x;
    RC_PARAM_MJPEG_S stRcParamMjpeg;
} VENC_RC_PARAM_S;

typedef struct tagVencInputPicAttr {
    NI_U32         u32MaxPicWidth;  /* 输入图像最大宽度，用于资源申请使用 */
    NI_U32         u32MaxPicHeight; /* 输入图像最大高度，用于资源申请使用 */
    PIXEL_FORMAT_E enPixelFormat;   /* 输入图像格式 */
    NI_U32         u32PicWidth;     /* 输入图像宽度 */
    NI_U32         u32PicHeight;    /* 输入图像高度 */
    NI_U32         au32Stride[3];   /* 输入图像各颜色分量的stride值,设置为0表示内部自动计算 */
} VENC_INPUT_PIC_ATTR_S;

/* 定义编码属性结构体 */
typedef struct tagVencAttr {
    PAYLOAD_TYPE_E enType;                /* 编码类型选择 */
    VENC_PROFILE_E enProfile;             /* 编码类型PT_H26x下有效 */
    NI_U32         u32StrmBufSize;        /* 需分配的码流缓存Buffer大小,ax520需配置比如下公式大的size:
                                            max(align((PicWidth+63/64)*10240,4096),81920)*2
                                           */
    VENC_INPUT_PIC_ATTR_S stInputPicAttr; /* 编码器接收YUV帧存信息 */
    NI_VOID              *pPrivData;      /* reserve,可作为扩展项 */
} VENC_ATTR_S;

typedef enum tagVencGopMode {
    VENC_GOPMODE_NORMAL = 0, /* IPP...PI */
    VENC_GOPMODE_ONELTR = 1,
    VENC_GOPMODE_SVC_T  = 2,
} VENC_GOP_MODE_E;

typedef enum tagVencLtrType {
    VENC_LTR_TYPE_REF_I   = 0, /* 长参考帧参考I帧 */
    VENC_LTR_TYPE_REF_LTR = 1, /* 长参考参考前一个长参考帧*/
    VENC_LTR_TYPE_BUT,
} VENC_LTR_TYPE_E;

typedef struct tagVencGopNormal {
    NI_U32 u32Gop; /* I帧间隔 */
} VENC_GOP_NORMAL_S;

typedef struct tagVencGopOneLtr {
    NI_U32          u32Gop;    /* I帧间隔 */
    NI_U32          u32LtrGop; /* Ltr帧间隔 */
    VENC_LTR_TYPE_E enLtrType;
    NI_U32          u32FirstLtrOffset; /* 一组GOP图像内第一个Ltr帧相对于I/IDR帧的偏移，ax520上该参数无效 */
} VENC_GOP_ONELTR_S;

typedef struct tagVencGopSvcT {
    NI_U32          u32Gop;       /* I帧间隔 */
    NI_U32          u32SvcLtrGop; /* svct3 level1 period */
    VENC_LTR_TYPE_E enSvcLtrType;
    NI_U32          u32SvcSpGop;
} VENC_GOP_SVC_T_S;

/* 定义GOP属性结构体 */
typedef struct tagVencGopAttr {
    VENC_GOP_MODE_E enGopMode;
    union {
        VENC_GOP_NORMAL_S stGopNomalAttr; /* UNION: enGopMode=VENC_GOPMODE_NORMAL|常规模式 */
        VENC_GOP_ONELTR_S stOneLtrAttr;   /* UNION: enGopMode=VENC_GOPMODE_ONELTR|长参考模式 */
        VENC_GOP_SVC_T_S  stSvcTAttr;     /* UNION: enGopMode=VENC_GOPMODE_SVC_T|SVC-T模式 */
    };
} VENC_GOP_ATTR_S;

/* 定义编码通道属性 */
typedef struct tagVencChnAttr {
    VENC_ATTR_S     stVeAttr;
    VENC_RC_ATTR_S  stRcAttr;
    VENC_GOP_ATTR_S stGopAttr; /* H26x编码使用 */
} VENC_CHN_ATTR_S;

/* 定义接收图像参数 */
typedef struct tagVencRecvPicParam {
    NI_S32 s32RecvPicNum; /*Number of frames received and encoded by the encoding channel*/
} VENC_RECV_PIC_PARAM_S;

/* 定义码流物理地址信息结构体 */
typedef struct tagVencStreamPacket {
    /* 最多包含VENC_STREAM_MAX_PACK_NUM个码流包，所有码流包合起来是完整的一帧 */
    NI_U32         u32PackNum;
    NI_U32         au32PackLen[VENC_STREAM_MAX_PACK_NUM];  /* 每个码流包数据的长度 */
    NI_PHYS_ADDR_T aPackPhyAddr[VENC_STREAM_MAX_PACK_NUM]; /* 每个码流包数据的起始物理地址 */
    NI_VIRT_ADDR_T aPackVirAddr[VENC_STREAM_MAX_PACK_NUM]; /* 每个码流包数据的起始虚拟地址 */
    NI_BOOL        bEndofFrame;                            /* 帧是否结束，如果VENC_STREAM_MAX_PACK_NUM个包都没传完一帧则需多次Get
                                                             直到bEndofFrame为True为止 */
    NI_BOOL bSubFrame;                                     /* MultiSlice情况下此成员为真表示是一个slice data数据
                                                             bEndofFrame=True,bSubFrame=True: 表示每个包是一个SliceData。
                                                             bEndofFrame=False,bSubFrame=True: 表示当前包是一个SliceData。*/
    NI_BOOL bCorrupt;                                      /* 直通下面开启MultiSlice时此成员可能为真，表示当前的slice data已经损坏，
                                                              用户需要自行将已经获取的slice data一并抛弃 */
} VENC_STREAM_PACKET_S;

/* 编码数据描述信息结构体 */
typedef struct tagVencStream {
    VENC_STREAM_PACKET_S   stStreamPacket;
    VENC_FRAME_TYPE_E      enFrameType; /* 帧类型，其值参见VENC_FRAME_TYPE_E枚举定义 */
    NI_U32                 u32FrameRate;
    NI_U32                 u32PicWidth;
    NI_U32                 u32PicHeight;
    NI_U32                 u32StreamSeq;     /* 当前码流测试序号，用于判断是否丢帧 */
    NI_U32                 u32CheckValue;    /* 码流数据的校验值 */
    NI_U64                 u64PTS;           /* timestamp, unit: microsecond */
    VENC_ADDITIONAL_INFO_S stAdditionalInfo; /* 编码附加信息输出，只有H26x编码有效 */
    NI_U32                 reserved[4];      /* 保留字段 */
} VENC_STREAM_S;

/* 定义ROI 配置结构体 */
typedef struct tagVencRoiCfg {
    NI_U32     u32Index;    /* 8个区域的编号 */
    NI_BOOL    bEnable;     /* 是否使能 */
    NI_BOOL    bAbsQp;      /* ROI 区域 QP 模式。 0：相对 QP, 1：绝对 QP */
    NI_S32     s32Qp;       /* 绝对qp取值范围h26x:[0,51],jpeg:[0,138],相对qp取值范围[-32,31] */
    NI_U32     u32Priority; /* ROI区域优先级，[0,16]，值越小优先级越高，h26x编码才有效 */
    NI_BOOL    bRcAqEn;     /* RcAq是否作用在ROI区域上，h26x编码才有效 */
    RECT_POS_S stRoiRect;   /* Area for Region-Of-Interest */
} VENC_ROI_CFG_S;

typedef struct tagVencMultiRoiCfg {
    NI_U32         u32RoiNum; /* 一次性设置的Roi区域个数 */
    VENC_ROI_CFG_S astRoiRegion[VENC_MAX_ROI_NUM];
} VENC_MULTI_ROI_CFG_S;

typedef struct tagVencQpMapInfo {
    NI_U32           u32QpMapWidth;     /* qp map表的宽度，即水平方向BlockUnit的个数，AX520上为(PicWidth  +31)/32 */
    NI_U32           u32QpMapHeight;    /* qp map表的高度，即垂直方向BlockUnit的个数, AX520上为(PicHeight  +31)/32 */
    NI_U32           u32QpMapSize;      /* qp map表长度(字节数),至少为 */
    NI_U32           u32QpMapBlockUnit; /* 0-64x64,1-32x32,2-16x16,3-8x8，ax520该参数无效 */
    NI_PHYS_ADDR_T   QpMapPhyAddr;      /* qp map表物理地址 */
    NI_VIRT_ADDR_T   QpMapVirAddr;      /* qp map表虚拟地址 */
    VENC_QPMAP_EXT_S stQpMapExt;        /* 扩展功能 */
} VENC_QPMAP_INFO_S;

typedef struct tagVencQpMapAttr {
    NI_BOOL           bQpMapEn;    /* 是否启用QpMap功能 */
    VENC_QPMAP_INFO_S stQpMapInfo; /* QpMap描述信息 */
} VENC_QPMAP_ATTR_S;

/* 定义去呼吸效应参数结构体 */
typedef struct TagVencDeBreathEffect {
    NI_BOOL bEnable;
    NI_S32  s32Strength0;
    NI_S32  s32Strength1;
} VENC_DEBREATH_EFFECT_S;

/* 定义去模糊效应参数结构体 */
typedef struct TagVencDeBlurringEffect {
    NI_BOOL bEnable;
} VENC_DEBLURRING_EFFECT_S;

/* 定义H.26x协议编码通道SLICE分割结构体 */
typedef struct TagVencSlicSplite {
    NI_BOOL bSplitEnable;
    NI_U32  u32SplitNum;
} VENC_SLICE_SPLIT_S;

/* 定义MV区域配置参数结构体*/
typedef struct TagVencMvCfg {
    NI_BOOL bEnable;
    RECT_S  stRect;
} VENC_MV_CFG_S;

/* 定义H265去块滤波参数结构体 */
typedef struct tagVencDblkH265 {
    NI_U32 u32SliceDeblockingFilterDisabledFlag; /* 默认为0。取值范围：0或1。 */
    NI_S32 s32SliceBetaOffsetDiv2;               /* 默认为0。取值范围：[-6,6]。 */
    NI_S32 s32SliceTcOffsetDiv2;                 /* 默认为0。取值范围：[-6,6]。 */
} VENC_DBLK_H265_S;

/* 定义H264去块滤波参数结构体 */
typedef struct tagVencDblkH264 {
    NI_U32 u32DisableDeblockingFilterIdc; /* 取值范围：[0,2], 默认值0，具体含义请参见H.264 协议。 */
    NI_S32 s32SliceAlphaC0OffsetDiv2;     /* 取值范围：[-6,6], 默认值0，具体含义请参见H.264协议。 */
    NI_S32 s32SliceBetaOffsetDiv2;        /* 取值范围：[-6,6], 默认值0，具体含义请参见H.264协议。 */
} VENC_DBLK_H264_S;

/* 定义去块滤波参数结构体 */
typedef struct tagVencDblkAttr {
    VENC_DBLK_H264_S stH264Dblk;
    VENC_DBLK_H265_S stH265Dblk;
} VENC_DBLK_ATTR_S;

typedef struct tagVencVuiTimeInfo {
    NI_U8  u8TimingInfoPresentFlag; /* default value: 0. If 1, timing info belows will be encoded into vui. {0,1}.*/
    NI_U8  u8FixedFrameRateFlag;    /* default value: n/a. This parameter is reserved currently. */
    NI_U32 u32NumUnitsInTick;       /* sps中num_units_in_tick语法元素，用于保存帧率信息 */
    NI_U32 u32TimeScale;            /* sps中time_scale语法元素，用于保存帧率信息 */
} VENC_VUI_TIME_INFO_S;

typedef struct tagVencVuiAspectRatio {
    NI_U8  u8AspectRatioInfoPresentFlag; /* default value: 0. If 1, aspectratio info belows will be encoded into vui.. */
    NI_U8  u8AspectRatioIdc;             /* 表示亮度样值的样点高宽比的比值  */
    NI_U8  u8OverscanInfoPresentFlag;    /* default value: n/a, This parameter is reserved currently. */
    NI_U8  u8OverscanAppropriateFlag;    /* default value: n/a. This parameter is reserved currently. */
    NI_U16 u16SarWidth;                  /* 样点高宽比的水平尺寸. */
    NI_U16 u16SarHeight;                 /* 样点高宽比的垂直尺寸. */
} VENC_VUI_ASPECT_RATIO_S;

typedef struct tagVencVuiVideoSignal {
    NI_U8 u8VideoSignalTypePresentFlag;   /* default value: 1. This parameter is reserved currently. */
    NI_U8 u8VideoFormat;                  /* 国际标准编码前的制式.  */
    NI_U8 u8VideoFullRangeFlag;           /* 黑电平和亮度与色度信号的范围  */
    NI_U8 u8ColourDescriptionPresentFlag; /* default value: n/a. This parameter is reserved currently. */
    NI_U8 u8ColourPrimaries;              /* default value: n/a. This parameter is reserved currently. */
    NI_U8 u8TransferCharacteristics;      /* default value: n/a. This parameter is reserved currently. */
    NI_U8 u8MatrixCoefficients;           /* default value: n/a. This parameter is reserved currently. */
} VENC_VUI_VIDEO_SIGNAL_S;

typedef struct tagVencVuiBitstreamRestric /* default value: n/a. This parameter is reserved currently. */
{
    NI_U8 u8BitstreamRestrictionFlag;
} VENC_VUI_BITSTREAM_RESTRIC_S;

/* 定义视频可用信息参数结构体 */
/* 仅支持sar_width、sar_height、video_full_range_flag等3个参数可配置, 开启编码之前需设置 */
typedef struct tagVencVuiAttr {
    VENC_VUI_ASPECT_RATIO_S      stVuiAspectRatio; /* the param of aspect ratio */
    VENC_VUI_TIME_INFO_S         stVuiTimeInfo;    /* the param of time info */
    VENC_VUI_VIDEO_SIGNAL_S      stVuiVideoSignal; /* the param of video signal */
    VENC_VUI_BITSTREAM_RESTRIC_S stVuiBitstreamRestric;
} VENC_VUI_ATTR_S;

/* 定义CIR高级参数结构体 */
typedef struct tagVencCirParam {
    NI_U32 u32CirStart;    /* [0..mbTotal] First macroblock for Cyclic Intra Refresh */
    NI_U32 u32CirInterval; /* [0..mbTotal] Macroblock interval for Cyclic Intra Refresh, 0=disabled */
} VENC_CIR_PARAM_S;

/* 定义GDR高级参数结构体 */
typedef struct tagVencGdrParam {
    NI_U32 u32GdrDuration; /*how many pictures it will take to do GDR, if 0, not do GDR*/
} VENC_GDR_PARAM_S;

/* 定义SAO高级参数结构体 */
typedef struct tagVencSaoParam {
    NI_BOOL bEnableSao; /* Enable SAO */
} VENC_SAO_PARAM_S;

/* 定义用户指定的SEI数据结构体,当pu8UserData为NULL且u32DataSize为0时，表关闭用户sei数据功能 */
typedef struct tagVencSeiUsrData {
    NI_U8 *pu8UserData; /* user_data_unregistered 字段内容，最开始的16个字节是uuid */
    NI_U32 u32DataSize; /* user_data_unregistered 字段长度 */
} VENC_SEI_USRDATA_S;

/* 定义缩略图格式枚举 */
typedef enum tagVencJpegThumbFormat {
    VENC_JPEG_THUMB_JPEG         = 0x10, /* Thumbnail coded using JPEG  */
    VENC_JPEG_THUMB_PALETTE_RGB8 = 0x11, /* Thumbnail stored using 1 byte/pixel */
    VENC_JPEG_THUMB_RGB24        = 0x13  /* Thumbnail stored using 3 bytes/pixel */
} VENC_JPEG_THUMB_FORMAT_E;

/* 定义缩略图参数结构体 */
typedef struct tagVencThumbnail {
    NI_U32                   u32Width;      /* Width in pixels of thumbnail, Range:[16,255] */
    NI_U32                   u32Height;     /* Height in pixels of thumbnail, Range:[16,255] */
    VENC_JPEG_THUMB_FORMAT_E enFormat;      /* Format of the thumbnail */
    NI_U32                   u32DataLength; /* Data amount in bytes. Range according to enFormat and u8Width x u8Height:
                                                THUMB_JPEG  : total size must less than 65527;
                                                PALETTE_RGB8: u16DataLength=3*256+u8Width*u8Height, total size must less than 65525;
                                                THUMB_RGB24 : u16DataLength=3*u8Width*u8Height, total size must less than 65525 */
    NI_U32   u32DataPhyAddr;                /* Thumbnail data physical addr */
    NI_VOID *pData;                         /* Thumbnail data virtual addr */
} VENC_THUMBNAIL_S;

/* 定义帧内自参考区域参数结构体 */
typedef struct tagVencIntraArea {
    NI_BOOL bEnableIntraArea; /* Force the area of all frames encoding by intra macroblocks */
    NI_U32  u32X;             /* horizontal start point */
    NI_U32  u32Y;             /* vertical start point */
    NI_U32  u32Width;         /* width */
    NI_U32  u32Height;        /* height */
} VENC_INTRA_AREA_S;

/* 定义高级参数结构体 */
typedef struct tagVencModParam {
    NI_S32 s32IFrmIsIdr;     /* I帧是否是均是IDR帧   1：每个I帧均为IDR帧，取值范围[0, 1],ax520上该参数必须设置为1 */
    NI_U32 u32UseDummyIFunc; /* 是否使用Dummy I功能，默认值0x1.当内存不足时可通过设置不使用Dummy I来降低帧存，
                               减少内存消耗,第xbit对应第x通道,ax520上该参数无效 */
} VENC_MOD_PARAM_S;

typedef struct tagVencQpMapFrame {
    VIDEO_FRAME_S     stVideoFrame;
    VENC_QPMAP_INFO_S stQpMapInfo;
} VENC_QPMAP_FRAME_S;

typedef struct tagVencJpegParam {
    NI_U8 *pu8QTableLuma;   /* Quantization table for luminance [64], overrides quantization level, zigzag order */
    NI_U8 *pu8QTableChroma; /* Quantization table for chrominance [64], overrides quantization level, zigzag order */
    NI_U8 *pu8NonRoiFilter; /* non-roi filter[128], two 8x8 filter defined for luma and chroma non-roi region */
} VENC_JPEG_PARAM_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*  __NI_COMM_VENC_H__  */
