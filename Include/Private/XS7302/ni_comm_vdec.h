/* 
 * ni_comm_vdec.h -- vdec模块对外提供的头文件
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */


#ifndef __NI_COMM_VDEC_H__
#define __NI_COMM_VDEC_H__

#include "ni_errno.h"
#include "ni_common.h"
#include "ni_define.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef enum tagVdecErrCode
{
    ERR_VDEC_CHN_EXIST = 64,   /* device or channel has existed */
    ERR_VDEC_NOT_INIT  = 65,   /* device or channel not initialized */
    ERR_VDEC_IRQ_FAIL  = 66,   /* device request irq failure */
    ERR_VDEC_HAS_STARTED  = 67,   /* device or channel has been started*/
    ERR_VDEC_NOT_START    = 68,   /* device or channel not start */
    ERR_VDEC_STREAM_ERR   = 69,  /* stream data error */
    ERR_VDEC_NO_FRAME    = 70,  /* NO stream  */
    ERR_VDEC_BUSY         = 71,  /* VENC is busy, please wait a moment, then try again */
    ERR_VDEC_GET_FRAME_TIMEOUT = 72,  /* Get Stream Timeout, try again */
    ERR_VDEC_WAIT_BUFFER_TIMEOUT = 73,
    ERR_VDEC_CALLFUNC_TIMEOUT = 74,
    ERR_VDEC_WAIT_EVENT_TIMEOUT = 75,

} VDEC_ERR_CODE_E;

/* invlalid device ID */
#define NI_ERR_VDEC_INVALID_GRPID     NI_DEF_ERR(NI_ID_VDEC, EN_ERR_LEVEL_ERROR, ERR_INVALID_GRPID)
/* invlalid channel ID */
#define NI_ERR_VDEC_INVALID_CHNID     NI_DEF_ERR(NI_ID_VDEC, EN_ERR_LEVEL_ERROR, ERR_INVALID_CHNID)
/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define NI_ERR_VDEC_ILLEGAL_PARAM     NI_DEF_ERR(NI_ID_VDEC, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)
/* channel exists */
#define NI_ERR_VDEC_CHN_EXIST         NI_DEF_ERR(NI_ID_VDEC, EN_ERR_LEVEL_ERROR, ERR_EXIST)
/* channel unexists */
#define NI_ERR_VDEC_CHN_UNEXIST       NI_DEF_ERR(NI_ID_VDEC, EN_ERR_LEVEL_ERROR, ERR_UNEXIST)
/* using a NULL point */
#define NI_ERR_VDEC_NULL_PTR          NI_DEF_ERR(NI_ID_VDEC, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)
/* try to enable or initialize system,device or channel, before configing attribute */
#define NI_ERR_VDEC_NOT_CONFIG        NI_DEF_ERR(NI_ID_VDEC, EN_ERR_LEVEL_ERROR, ERR_NOT_CONFIG)
/* operation is not supported by NOW */
#define NI_ERR_VDEC_NOT_SUPPORT       NI_DEF_ERR(NI_ID_VDEC, EN_ERR_LEVEL_ERROR, ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change static attribute */
#define NI_ERR_VDEC_NOT_PERM          NI_DEF_ERR(NI_ID_VDEC, EN_ERR_LEVEL_ERROR, ERR_NOT_PERM)
/* failure caused by malloc memory */
#define NI_ERR_VDEC_NOMEM             NI_DEF_ERR(NI_ID_VDEC, EN_ERR_LEVEL_ERROR, ERR_NOMEM)
/* failure caused by malloc buffer */
#define NI_ERR_VDEC_NOBUF             NI_DEF_ERR(NI_ID_VDEC, EN_ERR_LEVEL_ERROR, ERR_NOBUF)
/* no data in buffer */
#define NI_ERR_VDEC_BUF_EMPTY         NI_DEF_ERR(NI_ID_VDEC, EN_ERR_LEVEL_ERROR, ERR_BUF_EMPTY)
/* no buffer for new data */
#define NI_ERR_VDEC_BUF_FULL          NI_DEF_ERR(NI_ID_VDEC, EN_ERR_LEVEL_ERROR, ERR_BUF_FULL)
/* failure caused by not initialize module */
#define NI_ERR_VDEC_NOT_INIT          NI_DEF_ERR(NI_ID_VDEC, EN_ERR_LEVEL_ERROR, ERR_VDEC_NOT_INIT)
/* failure caused by request irq */
#define NI_ERR_VDEC_REQUEST_IRQFAIL   NI_DEF_ERR(NI_ID_VDEC, EN_ERR_LEVEL_ERROR, ERR_VDEC_IRQ_FAIL)
/* failure caused by enc has started encoding */
#define NI_ERR_VDEC_CHN_HAS_STARTED   NI_DEF_ERR(NI_ID_VDEC, EN_ERR_LEVEL_ERROR, ERR_VDEC_HAS_STARTED)
/* failure caused by enc has started encoding */
#define NI_ERR_VDEC_CHN_NOT_START     NI_DEF_ERR(NI_ID_VDEC, EN_ERR_LEVEL_ERROR, ERR_VDEC_NOT_START)
/* failure caused by stream err */
#define NI_ERR_VDEC_STREAM_ERR        NI_DEF_ERR(NI_ID_VDEC, EN_ERR_LEVEL_ERROR, ERR_VDEC_STREAM_ERR)
/* failure caused by no stream */
#define NI_ERR_VDEC_NO_FRAME         NI_DEF_ERR(NI_ID_VDEC, EN_ERR_LEVEL_ERROR, ERR_VDEC_NO_FRAME)
/* failure caused by get stream timeout */
#define NI_ERR_VDEC_GET_FRAME_TIMEOUT       NI_DEF_ERR(NI_ID_VDEC, EN_ERR_LEVEL_ERROR, ERR_VDEC_GET_FRAME_TIMEOUT)
/* VENC is busy, please wait a moment, then try again */
#define NI_ERR_VDEC_BUSY                    NI_DEF_ERR(NI_ID_VDEC, EN_ERR_LEVEL_ERROR, ERR_VDEC_BUSY)
#define NI_ERR_VDEC_WAIT_BUFFER_TIMEOUT     NI_DEF_ERR(NI_ID_VDEC, EN_ERR_LEVEL_ERROR, ERR_VDEC_WAIT_BUFFER_TIMEOUT)
#define NI_ERR_VDEC_CALLFUNC_TIMEOUT        NI_DEF_ERR(NI_ID_VDEC, EN_ERR_LEVEL_ERROR, ERR_VDEC_CALLFUNC_TIMEOUT)
#define NI_ERR_VDEC_WAIT_EVENT_TIMEOUT      NI_DEF_ERR(NI_ID_VDEC, EN_ERR_LEVEL_ERROR, ERR_VDEC_WAIT_EVENT_TIMEOUT)

/** Picture coding type */
typedef enum DecPicCodingType 
{
    DEC_PIC_TYPE_I           = 0,
    DEC_PIC_TYPE_P           = 1,
    DEC_PIC_TYPE_B           = 2,
    DEC_PIC_TYPE_D           = 3,
    DEC_PIC_TYPE_FI          = 4,
    DEC_PIC_TYPE_BI          = 5
} VDEC_PIC_CODING_TYPE_E;

/* rotate type  */
typedef enum tagVdecRotateType
{
	VDEC_ROTATE_BYPASS    = 0,          /* BYPASS */
	VDEC_ROTATE_MIRROR    = 1,          /* 水平镜像 */
	VDEC_ROTATE_FLIP      = 2,          /* 垂直翻转 */
	VDEC_ROTATE_90        = 3,          /* 顺时针90度旋转 */
	VDEC_ROTATE_180       = 4,          /* 顺时针180度旋转 */
	VDEC_ROTATE_270       = 5,          /* 顺时针270度旋转 */
	VDEC_ROTATE_MIRROR_90 = 6,          /* 水平镜像+顺时针90度旋转 */
	VDEC_ROTATE_FLIP_90   = 7,          /* 垂直翻转+顺时针90度旋转 */
    VDEC_ROTATE_BUTT
}VDEC_ROTATE_TYPE_E;

typedef enum tagVideoSendMode 
{ 
    VIDEO_SEND_MODE_STREAM = 0,     /* send by stream */ 
    VIDEO_SEND_MODE_FRAME     ,     /* send by frame  */ 
    VIDEO_SEND_MODE_BUTT 
}VDEC_SEND_MODE_E; 

typedef struct tagVdecStream
{
    /* 码流信息描述结构体，物理地址跟虚拟地址不能同时为0。
         如果传了物理地址则会使用给的物理地址进行解码，不会进行码流数据的拷贝，但对物理地址有对齐的要求。
         如果用户没传物理地址则CreateChn的时候必须设置StreamBufSize大小，码流会拷贝进码流缓存里。 */
    NI_U32  u32StmPhyAddr;  /* 码流所在物理地址，码流必须物理内存连续 */
    NI_U8*  pu8StmVirAddr;  /* 码流所在虚拟地址 */ 
    NI_U32  u32Len;         /* 码流长度 */ 
    NI_U64  u64PTS;         /* 时间戳 */ 
    NI_BOOL bEndOfStream;   /* 是否发完所有码流,当SendMode为STREAM模式时有效 */
}VDEC_STREAM_S; 

/* cropping info */
typedef struct tagVdecCropParams 
{
    NI_U32 u32CropLeftOffset;
    NI_U32 u32CropOutWidth;
    NI_U32 u32CropTopOffset;
    NI_U32 u32CropOutHeight;
}VDEC_CROP_PARAMS_S;

/* Video sequence information. */
typedef struct tagDecSequenceInfo 
{
    NI_U32 u32SarWidth;                    /**< sample aspect ratio */
    NI_U32 u32SarHeight;                   /**< sample aspect ratio */
    VDEC_CROP_PARAMS_S stCropParams; /**< Cropping parameters for the picture */
    NI_U32 u32NnumOfRefFrames;   /**< Maximum number of reference frames */
    NI_U32 u32BitDepthLuma;    /* Bit depth of stored picture */
    NI_U32 u32BitDepthChroma;
}VDEC_SEQUENCE_INFO_S;

/* Picture specific information. */
typedef struct tagDecPicInfo
{
    VDEC_PIC_CODING_TYPE_E enPicCodingType; /**< Picture coding type */
    NI_BOOL bIsCorrupted;             /**< Tells whether picture is corrupted */
    NI_U32 u32CyclesPerMb;            /**< Avarage decoding time in cycles per mb */
    NI_U32 u32PicId;                   /**< Identifier for the picture to be decoded */
    NI_U32 u32DecodeId;
}VDEC_PIC_INFO_S;

/* Structure to carry information about decoded pictures. */
typedef struct tagVdecPicture
{
    NI_BOOL                 bIsUsed;
    VIDEO_FRAME_S           stVideoFrame;
    VDEC_SEQUENCE_INFO_S    stSequenceInfo; /**< Sequence coding parameters used */
    VDEC_PIC_INFO_S         stPicInfo;          /**< Picture specific parameters */
}VDEC_PICTURE_S;

typedef struct tagVdecOutputInfo 
{
    VDEC_PICTURE_S stPictures[VDEC_MAX_OUTPUT_PER_CHN];
}VDEC_OUTPUT_INFO_S;

typedef struct tagVDecPpConfig
{
    NI_BOOL bPpEn;    /* PP unit enabled */
    PIXEL_FORMAT_E enOutFormat;
    NI_BOOL bCropEn;  /* whether cropping is enabled */
    RECT_S  stCropRect;
    NI_BOOL bScaleEn;  /* whether scaling is enabled */
    SIZE_S  stScaleSize;

    /* 设置为0表示不使用stride */
    NI_U32 au32Stride[3];
} VDEC_PP_CONFIG_S;

typedef struct tagVdecChnAttr
{ 
    PAYLOAD_TYPE_E      enType;             /* 解码协议类型 */ 
    VDEC_SEND_MODE_E    enSendMode;         /* 码流发送方式 */
    NI_U32              u32StreamBufSize;   /* 码流Buffer大小，当设置为0时，驱动不分配码流缓存，用户需要指定码流的物理地址和长度 */
    NI_U32              u32MaxPicWidth;     /* 通道解码最大宽度 */
    NI_U32              u32MaxPicHeight;    /* 通道解码最大高度 */
    NI_U32              u32FrameBufCnt;     /* 申请多少yuv帧存个数 */
    VDEC_PP_CONFIG_S    astPpCfg[VDEC_MAX_OUTPUT_PER_CHN]; 
    NI_BOOL             bYuvWriteBack;      /* 当输出yuv帧存满时，驱动内部是否主动丢弃老的帧 */
    NI_BOOL             bOutputReordering;  /* H26x解码类型有效 */
}VDEC_CHN_ATTR_S;

/* PP输出的抽帧输出由用户自定义 */
typedef struct tagVdecFrmRateExtractAttr
{
    NI_U32  u32InFps;                               /* 通道输入码流的帧率 */
    NI_U32  au32OutFps[VDEC_MAX_OUTPUT_PER_CHN];    /* 通道输出帧率 */
}VDEC_FRM_RATE_EXTRACT_ATTR_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /*  __NI_COMM_VDEC_H__  */

