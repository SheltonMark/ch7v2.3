/* 
 * ni_comm_venc.h -- venc模块对外提供的头文件
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */



#ifndef __NI_COMM_VENC_H__
#define __NI_COMM_VENC_H__

#include "ni_errno.h"
#include "ni_common.h"
#include "ni_define.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef enum tagVencErrCode
{
    ERR_VENC_CHN_EXIST              = 64,   /* device or channel has existed */
    ERR_VENC_NOT_INIT               = 65,   /* device or channel not initialized */
    ERR_VENC_IRQ_FAIL               = 66,   /* device request irq failure */
    ERR_VENC_HAS_STARTED            = 67,   /* device or channel has been started*/
    ERR_VENC_NOT_START              = 68,   /* device or channel not start */
    ERR_VENC_STREAM_ERR             = 69,   /* stream data error */
    ERR_VENC_NO_STREAM              = 70,   /* NO stream  */
    ERR_VENC_BUSY                   = 71,   /* VENC is busy, please wait a moment, then try again */
    ERR_VENC_GET_STREAM_TIMEOUT     = 72,   /* Get Stream Timeout, try again */
    ERR_VENC_CALLFUNCTION           = 73,   /* Call other module callback function fail */
    ERR_VENC_BUF_EXIST              = 74,   /* buf exist */
    ERR_VENC_BUF_UNEXIST            = 75,   /* buf unexist */
    ERR_VENC_WAIT_ENCDONE_TIMEOUT   = 76

} VENC_ERR_CODE_E;

/* invlalid channel ID */
#define NI_ERR_VENC_INVALID_CHNID               NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_INVALID_CHNID)
/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define NI_ERR_VENC_ILLEGAL_PARAM               NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)
/* channel exists */
#define NI_ERR_VENC_CHN_EXIST                   NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_EXIST)
/* channel unexists */
#define NI_ERR_VENC_CHN_UNEXIST                 NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_UNEXIST)
/* using a NULL point */
#define NI_ERR_VENC_NULL_PTR                    NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)
/* try to enable or initialize system,device or channel, before configing attribute */
#define NI_ERR_VENC_NOT_CONFIG                  NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_NOT_CONFIG)
/* operation is not supported by NOW */
#define NI_ERR_VENC_NOT_SUPPORT                 NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change static attribute */
#define NI_ERR_VENC_NOT_PERM                    NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_NOT_PERM)
/* failure caused by malloc memory */
#define NI_ERR_VENC_NOMEM                       NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_NOMEM)
/* failure caused by malloc buffer */
#define NI_ERR_VENC_NOBUF                       NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_NOBUF)
/* no data in buffer */
#define NI_ERR_VENC_BUF_EMPTY                   NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_BUF_EMPTY)
/* no buffer for new data */
#define NI_ERR_VENC_BUF_FULL                    NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_BUF_FULL)
/* failure caused by not initialize module */
#define NI_ERR_VENC_NOT_INIT                    NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_VENC_NOT_INIT)
/* failure caused by request irq */
#define NI_ERR_VENC_REQUEST_IRQFAIL             NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_VENC_IRQ_FAIL)
/* failure caused by enc has started encoding */
#define NI_ERR_VENC_CHN_HAS_STARTED             NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_VENC_HAS_STARTED)
/* failure caused by enc has started encoding */
#define NI_ERR_VENC_CHN_NOT_START               NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_VENC_NOT_START)
/* failure caused by stream err */
#define NI_ERR_VENC_STREAM_ERR                  NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_VENC_STREAM_ERR)
/* failure caused by no stream */
#define NI_ERR_VENC_NO_STREAM                   NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_VENC_NO_STREAM)
/* failure caused by get stream timeout */
#define NI_ERR_VENC_GET_STREAM_TIMEOUT          NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_VENC_GET_STREAM_TIMEOUT)
/* VENC is busy, please wait a moment, then try again */
#define NI_ERR_VENC_BUSY                        NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_VENC_BUSY)
/* venc call other module callback function fail */
#define NI_ERR_VENC_CALLFUNCTION                NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_VENC_CALLFUNCTION)
/* buf exists */
#define NI_ERR_VENC_CHN_BUF_EXIST               NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_VENC_BUF_EXIST)
/* buf unxists */
#define NI_ERR_VENC_CHN_BUF_UNEXIST             NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_VENC_BUF_UNEXIST)
/* wait encode done timeout */
#define NI_ERR_VENC_CHN_WAIT_ENCDONE_TIMEOUT    NI_DEF_ERR(NI_ID_VENC, EN_ERR_LEVEL_ERROR, ERR_VENC_WAIT_ENCDONE_TIMEOUT)

#define VENC_STREAM_MAX_PACK_NUM    (3)

/* rotate type  */
typedef enum tagVencRotateType
{
	VENC_ROTATE_BYPASS    = 0,          /* BYPASS */
	VENC_ROTATE_MIRROR    = 1,          /* 水平镜像 */
	VENC_ROTATE_FLIP      = 2,          /* 垂直翻转 */
	VENC_ROTATE_90        = 3,          /* 顺时针90度旋转 */
	VENC_ROTATE_180       = 4,          /* 顺时针180度旋转 */
	VENC_ROTATE_270       = 5,          /* 顺时针270度旋转 */
	VENC_ROTATE_MIRROR_90 = 6,          /* 水平镜像+顺时针90度旋转 */
	VENC_ROTATE_FLIP_90   = 7,          /* 垂直翻转+顺时针90度旋转 */

    VENC_ROTATE_BUTT
}VENC_ROTATE_TYPE_E;

/* 帧类型枚举*/
typedef enum tagVencEncFrameType
{
	VENC_FRAME_TYPE_I       = 0xFD,     /* I帧 */
	VENC_FRAME_TYPE_P       = 0xFC,     /* P帧 */
    VENC_FRAME_TYPE_B       = 0xFE,     /* B帧 */
    VENC_FRAME_TYPE_DUMMY_I = 0xFF,     /* 虚拟I帧 */
    
    VENC_FRAME_TYPE_BUTT
}VENC_FRAME_TYPE_E;

/* 定义H.26X编码档次 */
typedef enum tagVencProfile
{
    VENC_H265_MAIN_PROFILE                  = 0,    /* main profile */
    VENC_H265_MAIN_STILL_PICTURE_PROFILE    = 1,    /* main still picture profile */
    VENC_H265_MAIN_10_PROFILE               = 2,    /* main 10 profile, not support now */
    
    VENC_H264_BASE_LINE_PROFILE             = 66,   /* baseline profile */
    VENC_H264_MAIN_PROFILE                  = 77,   /* main profile */
	VENC_H264_EXTEND_PROFILE                = 88,   /* extend profile, not support now */
	VENC_H264_HIGH_PROFILE                  = 100,  /* high profile */
    
    VENC_PROFILE_BUTT
}VENC_PROFILE_E;

/* 定义编码器码流buf模式.  */
typedef enum tagVencStreamBufMode
{
    VENC_STREAM_RING_BUF_MODE   = 0,    /* stream buf is ring mode */
    VENC_STREAM_LINE_BUF_MODE   = 1,    /* stream buf is line mode */
    
    VENC_STREAM_BUF_MODE_BUTT
}VENC_STREAM_BUF_MODE_E;

/* 定义码流校验方式 */
typedef enum tagVencStreamCheckType
{
    VENC_STREAM_CHECK_OFF       = 0,    /* 关闭码流校验功能 */
    VENC_STREAM_CHECK_SUM       = 1,    /* 以累加和校验方式生成码流校验码 */
    VENC_STREAM_CHECK_CRC       = 2,    /* 以CRC校验方式生成码流校验码 */
    
    VENC_STREAM_CHECK_BUTT
}VENC_STREAM_CHECK_TYPE_E;

/* 参考帧/重构帧压缩 */
typedef enum tagVencCompress
{
    VENC_COMPRESS_NONE    = 0,          /* 不开启压缩 */
    VENC_COMPRESS_LUMA    = 1,          /* 仅亮度压缩 */
    VENC_COMPRESS_CHROMA  = 2,          /* 仅色度压缩 */
    VENC_COMPRESS_BOTH    = 3,          /* 亮度和色度均压缩 */
    
    VENC_COMPRESS_BUTT
}VENC_COMPRESS_E;

/* RC 模式选择 */
typedef enum tagVencRcMode
{
    VENC_RC_MODE_CBR   = 0,         /* CBR模式 */
    VENC_RC_MODE_VBR   = 1,         /* VBR模式 */
    VENC_RC_MODE_FIXQP = 3,         /* 固定QP模式 */

    VENC_RC_MODE_BUTT
}VENC_RC_MODE_E;

/* 熵编码模式 */
typedef enum tagVencEntropyEncMode
{
    VENC_ENTROPY_ENC_MDDE_CAVLC  = 0,          /* CAVLC */
    VENC_ENTROPY_ENC_MDDE_CABAC  = 1,          /* CABAC */

    VENC_ENTROPY_ENC_MDDE_BUTT
}VENC_ENTROPY_ENC_MODE_E;

/* 定义H26x熵编码结构体 */
typedef struct tagVencH26xEntropy
{
    VENC_ENTROPY_ENC_MODE_E      enEntropyEncMode;           /* 熵编码模式 */
	NI_U32                       u32CabacInitFlag;           /* CABAC Tabble Init flag*/
	
}VENC_H26X_ENTROPY_S;

/* 视频编码图像裁剪属性 */
typedef struct tagVencCropAttr
{
    NI_BOOL      bEnable;               /* 视频编码图像裁剪使能 */
    RECT_S       stRect;                /* 图像裁剪矩形区域 */
}VENC_CROP_ATTR_S;

/* 视频编码帧头帧尾空间大小属性*/
typedef struct tagVencStreamHeadAttr
{
    NI_U32       u32StrmHeadSize;       /* 码流帧头空间大小，单位byte，最大支持32KB */
    NI_U32       u32StrmTailSize;       /* 码流帧尾空间大小，单位byte，最大支持32KB */
}VENC_STRM_HEAD_ATTR_S;

/* 视频码流buffer信息 */
typedef struct tagVencStreamBufInfo 
{ 
    NI_U32      u32PhyAddr; /* 码流物理地址 */
    NI_U32      u32Size;    /* 长度 */
} VENC_STREAM_BUF_INFO_S;

/* 视频码流buffer属性结构体，line buffer模式下重新配置地址时使用 */
typedef struct tagVencStreamBuf
{
    NI_U32          u32PhyAddr;
    NI_U32          u32BufSize;
}VENC_STREAM_BUF_S;

typedef struct
{
    NI_U32              u32PicMbWidth;  /* 对齐后的图像16宏块宽度，图像对齐方式HEVC:64px，H264:16px */
    NI_U32              u32PicMbHeight; /* 对齐后的图像16宏块高度，图像对齐方式HEVC:64px，H264:16px */
        
    /* 每4个bytes代表32x32(hevc)or16x16(h264)的MMS,MAD[31:27],MSE[26:17],SATD[16:0] */
    NI_U32              u32MmsInfoPhyAddr;  /* (MAD,MSE, SATD)信息所在的物理地址 */
    NI_U32              *pu32MmsInfoVirAddr; /*(MAD,MSE, SATD)信息所在的虚拟地址 */
    NI_U32              u32MmsInfoSize;

    /* 每2个bytes代表32x32(hevc)or16x16(h264)的bitcost,内存中以小端格式摆放 */
    NI_U32              u32BitsInfoPhyAddr;  /* bitcost信息所在的物理地址 */
    NI_U16              *pu16BitsInfoVirAddr; /* bitcost信息所在的虚拟地址 */
    NI_U32              u32BitsInfoSize;

    /* 每1个bytes代表16x16的qp值 */
    NI_U32              u32QpInfoPhyAddr;  /* qp信息所在的物理地址 */
    NI_U8               *pu8QpInfoVirAddr; /* qp信息所在的虚拟地址 */
    NI_U32              u32QpInfoSize;
} VENC_CU_INFO_V1_S;

/* Cuinfo组成结构体 */
typedef struct tagVencCuOutDate
{
    NI_BOOL             bValid;                 /* 当前码流CuInfo是否有效 */
    NI_U32              u32Version;             /* Version，不同的芯片型号，cuinfo保存方式不一样。xs7312上返回值1 */
    NI_BOOL             bIsH264;                /* 是否是H264编码 */
    union
    { 
        VENC_CU_INFO_V1_S  stCuinfoV1; 
    }; 
} VENC_CU_OUT_DATA_S;

typedef struct tagVencAdditionalInfo
{
    NI_U64              u64YSsimNum;        /* 分子。xs7312上不支持输出。 */
    NI_U64              u64USsimNum;        /* 分子。xs7312上不支持输出。 */
    NI_U64              u64VSsimNum;        /* 分子。xs7312上不支持输出。 */
    NI_U32              u32YSsimDem;        /* 分母。xs7312上不支持输出。 */
    NI_U32              u32UvSsimDem;       /* 分母。xs7312上不支持输出。 */
    NI_U32              u32SSEDivide256;    /* xs7312上不支持输出。 */
    NI_U32              u32SumOfQP;         /* xs7312上输出帧级QP值。 */
    NI_U32              u32SumOfQPNumber;   /* xs7312上固定值1。 */
    VENC_CU_OUT_DATA_S  stCuOutDataInfo;    /* Cuinfo信息描述。 */
}VENC_ADDITIONAL_INFO_S;

/* 定义固定码率控制结构体属性类型 */
typedef struct tagRcAttrCbr
{
    NI_U32      u32TargetBitRate;       /* 在StatTime时间内的目标码率, [10..500000]kbps */
    NI_U32      u32StatTime;            /* 码率统计时间，以秒为单位, Range:[1, 10]，
                                                    xs7312上该参数设为10代表A-CBR,其他值无意义。
                                                    A-CBR: 是在CBR的基础上整体target bitrate码率不超的情况下的主观调优，
                                                    没有任何GOP Leveld 限制，波动大小由图像内容决定。*/
}VENC_RC_ATTR_CBR_S;                                         

/* 定义可变码率控制结构体属性类型 */                     
typedef struct tagRcAttrVbr
{
    NI_U32      u32MaxBitRate;          /* 在StatTime时间内的最大码率，[10..500000]kbps */
    NI_U32      u32StatTime;            /* 码率统计时间，以秒为单位，Range:[1, 10]，
                                                    xs7312上该参数设为9代表Q-VBR，设为10代表V-VBR,其他值无意义。
                                                   */ 
    
    NI_U32      u32MinIQp;              /* I帧最小QP,[0..51], 建议设置不小于10 */
    NI_U32      u32MaxIQp;              /* I帧最大QP,[0..51] */
    NI_U32      u32MinPQp;              /* P帧最小QP,[0..51], 建议设置不小于10 */
    NI_U32      u32MaxPQp;              /* P帧最大QP,[0..51] */
}VENC_RC_ATTR_VBR_S;

/* 定义固定QP码率控制结构体属性类型 */
typedef struct tagRcAttrFixQp
{    
    NI_U32      u32FixedIQp;            /* I帧固定QP值，Jpeg编码时为quality值 */
    NI_U32      u32FixedPQp;            /* P帧固定QP值，Jpeg编码只要设u32FixedIQp即可 */
}VENC_RC_ATTR_FIXQP_S;

/* 定义码率控制结构体属性类型 */
typedef struct tagVencRcAttr
{
    VENC_RC_MODE_E      enRcMode;               /* 码率控制模式选择 */
    NI_U32              u32FrmRateNum;          /* 输入帧率的分子，帧率计算公式: u32FrmRateNum/u32FrmRateDenom, range:[1, 1048575] */
    NI_U32              u32FrmRateDenom;        /* 输入帧率的分母，取值必须小于等于u32FrmRateNum, range:[1, 65535] */
    union
    {
        VENC_RC_ATTR_CBR_S      stAttrCbr;
        VENC_RC_ATTR_VBR_S      stAttrVbr;
        VENC_RC_ATTR_FIXQP_S    stAttrFixQp;
    };
    NI_VOID             *pRcAttr;               /*the rc attribute which could be specified by user*/
}VENC_RC_ATTR_S;

/* 定义H26X RC高级属性参数 */
typedef struct tagRcParamH26x
{
    NI_BOOL     bPictureRc;             /* Adjust QP between pictures, [0,1], default: 1。xs7312上该参数无效 */
    NI_BOOL     bCtbRc;                 /* 宏块级码率控制开关。xs7312上该参数无效 */
    NI_U32      u32BlockRCSize;         /* size of block rate control : 2=16x16,1= 32x32, 0=64x64。xs7312上该参数无效 */
    NI_BOOL     bPictureSkip;           /* Allow rate control to skip pictures, [0,1], default:0。xs7312上该参数无效 */
    NI_S32      s32QpHdr;               /* The initial Quantization Parameter. If the rate control is enabled then this
                                                    * value is used only at the beginning of the encoding process.  -1 lets RC 
                                                    * calculate initial QP.  Not recommended to
                                                    * be set lower than 10. [-1,51]
                                                    */

    NI_U32      u32MinIQp;              /* Minimum QP for any I picture, [0,51], default: 10 */
    NI_U32      u32MaxIQp;              /* Maximum QP for any I picture, [u32MinIQp,51], default: 51 */
    NI_U32      u32MinPQp;              /* Minimum QP for any P picture, [0..51], default: 10 */
    NI_U32      u32MaxPQp;              /* Maximum QP for any P picture, [u32MinPQp,51], default: 51 */                                                   

    NI_BOOL     bHrdEn;                 /* Hypothetical Reference Decoder model, [0,1]. restricts the instantaneous bitrate and
                                                    * total bit amount of every coded picture. Enabling HRD will cause tight constrains
                                                    * on the operation of the rate control
                                                    */
    NI_U32      u32HrdCpbSize;          /* Size of Coded Picture Buffer in HRD (bits) */
                                            
    NI_S32      s32IntraQpDelta;    /* Intra QP delta. intraQP = QP + intraQpDelta
                                            * This can be used to change the relative quality
                                            * of the Intra pictures or to lower the size
                                            * of Intra pictures. [-12,12], Default value: -1 
                                            * xs7312上该参数用于设置qp_delta_i_p，创建通道之后，开启编码之前配置，不容许动态切换，默认值0，即host不关心。firmware配默认值。
                                            */
    NI_U32      u32FixedIntraQp;    /* Fixed QP value for all Intra pictures, [0,51], 0 = Rate control calculates intra QP。xs7312上该参数无效 */
    NI_S32      s32BitVarRangeI;    /* variations over average bits per frame for I frame, range:[10,10000],default:10000。
                                                xs7312上该参数用于设置最大I帧Size(字节单位),默认值为0，表示不控制i帧size。
                                                注意: 开启这个功能后，会有如下影响，请知悉:
                                                1、影响Gop Level的I/P比。
                                                2、特定内容下I帧Size造成的主观质量问题，并持续传播。
                                                3、特定内容为了达成I帧Size,造成的I帧内质量不均衡问题等。*/

    NI_S32      s32BitVarRangeP;    /* variations over average bits per frame for P frame, range:[10,10000],default:10000。xs7312上该参数无效 */

    NI_S32      s32TolMovingBitRate;    /* tolerance of max Moving bit rate, range:[0,2000],default:2000。xs7312上该参数无效  */
    NI_S32      s32MonitorFrames;       /* monitor frame length for moving bit rate, range:[10,120], default:-1, RC auto caculate by frame rate。xs7312上该参数无效 */
    NI_S32      s32SmoothPsnrInGOP;     /* dynamic bits allocation among GOP using SSE feed back for smooth psnr, 1 - enable, 0 - disable。
                                                     xs7312上用于配置tune-visual参数，创建通道之后，开启编码之前配好，不容许动态设置:
                                                     针对CBR:
                                                     tune-visual=AB: 
                                                     B个位数: 表示为了改善主观采用编码工具的合集，建议使用B=5。
                                                     A十位数: 表示Gop Level容许波动超出target bitrate的百分比。
                                                     举例:
                                                     约束在10%以内，就用05。
                                                     约束在20%以内，就用25。
                                                     约束在30%以内，就用35。
                                                     默认值15，调优的结果。
                                                     针对A-CBR: 只有个位数有效，十位数无影响。
                                                     */
    NI_U32      u32RcQpDeltaRange;      /* ctb rc qp delta range。 s7312上该参数无效 */
    NI_S32      s32PicQpDeltaMin;       /* minimum pic qp delta, range:[-10,-1],default:-2。xs7312上该参数无效 */
    NI_S32      s32PicQpDeltaMax;       /* maximum pic qp delta, range:[1,10],default:10。xs7312上该参数无效 */
    NI_S32      s32LongTermQpDelta;     /* QP delta of the frame using long term reference。xs7312上该参数无效 */
    NI_S32      s32ChangePos;           /* The ratio of the current bit rate to the maximum bit rate when the QP value 
                                                   * starts to be adjusted, range:[50,100]。
                                                   * xs7312上该参数表示VBR模式下当码率达到最大码率对应百分比的时候开始限制码率继续
                                                   上升，正常情形实际码率不超过设置最大码率的对应百分比，场景剧烈变化的时候实际码率
                                                   有可能超过设置最大码率的对应百分比，但不超过设置的最大码率(跟设置的值相关)。 
                                                   QVBR下默认值85，其他情况默认值为100，VBR模式下实际码率有可能超过设置的最大码率。 */
    NI_U32      u32StatRateFrms;        /* The rate controlling window in frames. RC tries to match the target bitrate inside the
                                                   * window. Each window can contain multiple GOPs and the RC adapts to the
                                                   * intra rate by calculating intraInterval. range:[0,65535],default:0 use gop frames。xs7312上该参数无效 */
    NI_U32      u32StaticSceneIbitPercent; /* I frame bits percent in static scene, range:[0,100],default:60  
                                                       * xs7312上默认值0，即host不关心该参数，创建通道之后，开启编码之前配好，不容许动态设置。i、p之间的size比。*/
    NI_S32      s32CtbRcRowQpStep;          /**<  max accumulated QP adjustment step per CTB Row by Ctb Rate Control.
                                                    QP adjustment step per CTB is ctbRcRowQpStep/ctb_per_row。xs7312上该参数[0-7]bit用于配置ipenalty_dc,
                                                    [8-15]bit用于配置ipenalty_planar,[16-23]bit用于配置ipenalty_angular,三个参数的取值范围为[0-31],默认值0x10 */
    NI_U32      u32CtbRcQpDeltaReverse;     /**< ctbrc qpdelta value reversed。
                                                           xs7312上用于设置qvbr min_bitrate_pos:
                                                           QVBR最小码率比例[0-100]， 相对于max bitrate 的百分比，需要比change pos 比例小，用来控制最低码率。
                                                           QVBR下默认值0 */
    NI_S32      s32TolCtbRcInter;           /**< Tolerance of Ctb Rate Control for INTER frames。
                                                           xs7312上用于设置qvbr best_quality:
                                                           QVBR允许的最优质量[5-500]。值越小表示质量越好。QVBR下默认值8 */
    NI_S32      s32TolCtbRcIntra;           /**< Tolerance of Ctb Rate Control for INTRA frames。
                                                            xs7312上用于设置qvbr worst_quality:
                                                            QVBR的最差质量[5-500]。QVBR下默认值20，值越小表示质量越好，该值需大于best_quality 
                                                             */                                       
}RC_PARAM_H26X_S;

/* 定义MJPEG RC高级属性参数 */
typedef struct tagRcParamMjpeg
{
    NI_U32      u32TargetBitRate;       /* target bitrate */
    NI_BOOL     bSingleRc;
    NI_U32      u32Qpmin;               /* Minimum QP for picture, [0,51] */
    NI_U32      u32Qpmax;               /* Maximum QP for picture, [u32MinIQp,51] */
    NI_S32      s32PicQpDeltaMin;       /* minimum pic qp delta, range:[-10,-1],default:-2 */
    NI_S32      s32PicQpDeltaMax;       /* maximum pic qp delta, range:[1,10],default:10  */
}RC_PARAM_MJPEG_S;

/* 定义RC高级属性参数 */
typedef struct tagVencRcParam
{
    RC_PARAM_H26X_S         stRcParamH26x;
    RC_PARAM_MJPEG_S	    stRcParamMjpeg;
}VENC_RC_PARAM_S;

typedef struct tagVencInputPicAttr
{
    NI_U32              u32MaxPicWidth;     /* 输入图像最大宽，用于申请内存使用 */
    NI_U32              u32MaxPicHeight;    /* 输入图像最大高，用于申请内存使用 */
    PIXEL_FORMAT_E      enPixelFormat;      /* 输入图像格式 */
    NI_U32              u32PicWidth;        /* 输入图像宽 */ 
    NI_U32              u32PicHeight;       /* 输入图像高 */ 
    NI_U32              au32Stride[3];      /* 输入图像各颜色分量的stride值,设置为0表示内部自动计算 */ 
}VENC_INPUT_PIC_ATTR_S;

/* 定义编码属性结构体 */
typedef struct tagVencAttr 
{ 
    PAYLOAD_TYPE_E          enType;             /* 编码类型选择 */
    VENC_PROFILE_E          enProfile;          /* 编码类型PT_H26x下有效 */   
    NI_U32                  u32StrmBufSize;     /* 需分配的码流缓存Buffer大小 */
    VENC_INPUT_PIC_ATTR_S   stInputPicAttr;     /* 编码器接收YUV帧存信息 */
    NI_VOID                 *pPrivAttr;         /* reserve,可作为扩展项 */
}VENC_ATTR_S;

/* 定义GOP属性结构体 */
typedef struct tagVencGopAttr 
{
    NI_U32      u32Gop;                 /* I帧间隔 [1..65535] */
    NI_U32      u32DummyIGop;           /* 虚拟I帧间隔, 0表示不开启该功能(该帧只参考IDR/I帧) */
    NI_U32      u32DummyIOffset;        /* 一组GOP图像内第一个虚拟I帧相对于I/IDR帧的偏移，xs7312不支持 */
                                        /* 建议用户配置u32DummyIGop值大于u32DummyIOffset值使用 */
	                                    /* 当u32DummyIGop＞u32DummyIOffset时，第一个虚拟I帧的位置为相对于I/IDR帧的偏移u32DummyIOffset；
                                                     当u32DummyIGop≤u32DummyIOffset时，第一个虚拟 I帧为I/IDR帧偏移u32DummyIGop+1位置*/
    NI_U32      u32DummyIType;          /* 1:每一个长参考帧参考I帧，2: 当前长参考参考前一个长参考帧 */
    NI_VOID     *pPrivAttr;             /* reserve,可作为扩展项 */
}VENC_GOP_ATTR_S;

/* 定义编码通道属性 */
typedef struct tagVencChnAttr 
{ 
    VENC_ATTR_S         stVeAttr;
    VENC_RC_ATTR_S      stRcAttr;
    VENC_GOP_ATTR_S     stGopAttr;      /* H26x编码使用 */
}VENC_CHN_ATTR_S;

/* 定义接收图像参数 */
typedef struct tagVencRecvPicParam
{
    NI_S32          s32RecvPicNum;         /*Number of frames received and encoded by the encoding channel*/
} VENC_RECV_PIC_PARAM_S;

/* 定义码流物理地址信息结构体 */
typedef struct tagVencStreamPacket
{
    /* 最多包含VENC_STREAM_MAX_PACK_NUM个码流包，所有码流包合起来是完整的一帧 */
    NI_U32        u32PackNum;                                    
    NI_U32        au32PackPhyAddr[VENC_STREAM_MAX_PACK_NUM];   /* 每个码流包数据的起始物理地址 */
    NI_U32        au32PackVirAddr[VENC_STREAM_MAX_PACK_NUM];   /* 每个码流包数据的起始虚拟地址 */ 
    NI_U32        au32PackLen[VENC_STREAM_MAX_PACK_NUM];       /* 每个码流包数据的长度 */
    NI_BOOL       bEndofFrame;                                 /* 帧是否结束，如果VENC_STREAM_MAX_PACK_NUM个包都没传完一帧则需多次Get */
}VENC_STREAM_PACKET_S;

/* 编码数据描述信息结构体 */
typedef struct tagVencStream
{
    VENC_STREAM_PACKET_S        stStreamPacket;
    NI_U8                       u8FrameType;            /* 帧类型，其值参见VENC_FRAME_TYPE_E枚举定义 */
    NI_U64                      u64PTS;                 /* timestamp, unit: microsecond */
    NI_U16                      u16FrameRate;
    NI_U16                      u16PicWidth;    
    NI_U16                      u16PicHeight;
    NI_U32                      u32StreamSeq;           /* 当前码流测试序号，用于判断是否丢帧 */
    NI_U32                      u32CheckValue;          /* 码流数据的校验值 */
    NI_U32                      u32RecFrameBitNum;
	VENC_ADDITIONAL_INFO_S      stAdditionalInfo;       /* 编码附加信息输出，只有H26x编码有效 */
    NI_U32                      reserved[4];            /* 保留字段 */
}VENC_STREAM_S;

/* 定义ROI 配置结构体 */
typedef struct tagVencRoiCfg
{
    NI_U32          u32Index;                   /* 8个区域的编号 */
    NI_BOOL         bEnable;                    /* 是否使能 */
    NI_BOOL         bAbsQp;                     /* ROI 区域 QP 模式。 0：相对 QP, 1：绝对 QP */
    NI_S32          s32Qp;                      /* 绝对qp取值范围[0,51],相对qp取值范围[-32,31] */
    NI_U32          u32Priority;                /* ROI区域优先级，[0-16] */
    RECT_POS_S      stRoiRect;                  /* Area for Region-Of-Interest */
}VENC_ROI_CFG_S;

typedef struct tagVencMultiRoiCfg
{
    NI_U32              u32RoiNum;                      /* 一次性设置的Roi区域个数 */
    VENC_ROI_CFG_S      astRoiRegion[VENC_MAX_ROI_NUM]; 
}VENC_MULTI_ROI_CFG_S;

/* the block parameter record specifies the various properties of a quad */
typedef struct tagVencQpMapBlockParamRecord
{
    /* 负值的时候高位为符号位， 补码。 -32到31之间 */
    NI_U32  u32QpDeltaTopLeft:6;    //[0-5]     /*deltaqp取值范围[-32,31]*/
    NI_U32  u32QpDeltaTopRight:6;   //[6-11] 
    NI_U32  u32QpDeltaBotLeft:6;    //[12-17] 
    NI_U32  u32QpDeltaBotRight:6;   //[18-23] 
    
    NI_U32  u32Enable:1;            //[24] 
    NI_U32  u32Resvd0:4;            //[25-28] 
    NI_U32  u32ForceIntra:1;        //[29] 
    NI_U32  u32AbsQp:1;             //[30]      /*绝对qp使能位，需先将SelQpDelta置1，absqp=baseqp[32]+deltaqp[-32,31]*/
    NI_U32  u32Skip:1;              //[31] 

    NI_U32  u32MinQpTopLeft:6;      //[32:37]   /*设置qp值下限，如不设置则为默认值0*/
    NI_U32  u32MinQpTopRight:6;     //[38:43] 
    NI_U32  u32MinQpBotLeft:6;      //[44:49] 
    NI_U32  u32MinQpBotRight:6;     //[50:55] 
    NI_U32  u32SelQpDelta:1;        //[56]      /*相对qp使能位，deltaqp取值范围[-32,31]，相应减少或增加图像qp值*/
    NI_U32  u32Resvd1:7;            //[57-63] 
}VENC_QPMAP_BLOCK_PARAM_RECORD;

typedef struct tagVencQpMapExtV1
{
    NI_BOOL     bQpMapDirect;       /* xs7312上用户可以通过使用mbuf机制实现qpmap内存无拷贝机制，
                                                此时需要将QpMapDirect置为True,且用户将申请的mbuf poolid赋值给u32PoolId */
    NI_U32      u32PoolId;
    NI_BOOL     bActOnIFrm;         /* QpMap是否作用在i帧上，默认i帧不起作用，仅支持CBR、VBR */
    NI_BOOL     bAbsQp;             /* 用户给的QpMap是否启用了绝对qp功能，驱动中需要set相应parameter到fw中 */
}VENC_QPMAP_EXT_V1_S;

typedef struct tagVencQpMapExt
{
    NI_U32  u32Version;             /* Version，区分不同的芯片型号，xs73x2传1 */
    union
    { 
        VENC_QPMAP_EXT_V1_S stQpMapExtV1; 
    }; 
}VENC_QPMAP_EXT_S;

typedef struct tagVencQpMapInfo
{
    NI_U16          u16QpMapWidth;              /* qp map表的宽度，即水平方向BlockUnit的个数 */
    NI_U16          u16QpMapHeight;             /* qp map表的高度，即垂直方向BlockUnit的个数 */
    NI_U32          u32QpMapPhyAddr;            /* qp map表物理地址 */
    NI_U32          u32QpMapVirAddr;            /* qp map表虚拟地址 */
    NI_U32          u32QpMapSize;               /* qp map表长度(字节数) */
    NI_U32          u32QpMapBlockUnit;          /* 0-64x64,1-32x32,2-16x16,3-8x8，xs7312该参数无效 */
    NI_VOID         *pPrivAttr;                 /* reserve,可作为扩展项，如启用可传VENC_QPMAP_EXT_S变量指针 */
}VENC_QPMAP_INFO_S;

typedef struct tagVencQpMapAttr
{
    NI_BOOL             bQpMapEn;                   /* 是否启用QpMap功能 */
    VENC_QPMAP_INFO_S   stQpMapInfo;                /* QpMap描述信息 */
}VENC_QPMAP_ATTR_S;

/* 定义H265去块滤波参数结构体 */
typedef struct tagVencDblkH265
{
    NI_U32 slice_deblocking_filter_disabled_flag;    /* 默认为0。取值范围：0或1。 */
    NI_S32 slice_beta_offset_div2;                   /* 默认为0。取值范围：[-6，6]。 */
    NI_S32 slice_tc_offset_div2;                     /* 默认为0。取值范围：[-6，6]。 */
}VENC_DBLK_H265_S;

/* 定义H264去块滤波参数结构体 */
typedef struct tagVencDblkH264
{
    NI_U32 disable_deblocking_filter_idc;          /* 取值范围：[0, 2], 默认值0，具体含义请参见H.264 协议。 */
    NI_S32 slice_alpha_c0_offset_div2;             /* 取值范围：[-6, 6], 默认值0，具体含义请参见H.264协议。 */
    NI_S32 slice_beta_offset_div2;                 /* 取值范围：[-6, 6], 默认值0，具体含义请参见H.264协议。 */
}VENC_DBLK_H264_S;

/* 定义去块滤波参数结构体 */
typedef struct tagVencDblkAttr
{
    VENC_DBLK_H264_S    stH264Dblk;
    VENC_DBLK_H265_S    stH265Dblk;
}VENC_DBLK_ATTR_S; 

typedef struct tagVencVuiTimeInfo
{
    NI_U8  timing_info_present_flag;               /* default value: 0. If 1, timing info belows will be encoded into vui. {0,1}.*/
    NI_U8  fixed_frame_rate_flag;                  /* default value: n/a. This parameter is reserved currently. */
    NI_U32 num_units_in_tick;                      /* sps中num_units_in_tick语法元素，用于保存帧率信息 */
    NI_U32 time_scale;                             /* sps中time_scale语法元素，用于保存帧率信息 */
}VENC_VUI_TIME_INFO_S;

typedef struct tagVencVuiAspectRatio
{
    NI_U8  aspect_ratio_info_present_flag;        /* default value: 0. If 1, aspectratio info belows will be encoded into vui.. */
    NI_U8  aspect_ratio_idc;                      /* 表示亮度样值的样点高宽比的比值  */
    NI_U8  overscan_info_present_flag;			  /* default value: n/a, This parameter is reserved currently. */
    NI_U8  overscan_appropriate_flag;			  /* default value: n/a. This parameter is reserved currently. */
    NI_U16 sar_width;                             /* 样点高宽比的水平尺寸. */
    NI_U16 sar_height;                  		  /* 样点高宽比的垂直尺寸. */
}VENC_VUI_ASPECT_RATIO_S;

typedef struct tagVencVuiVideoSignal
{
    NI_U8  video_signal_type_present_flag;        /* default value: 1. This parameter is reserved currently. */
    NI_U8  video_format;                          /* 国际标准编码前的制式.  */
    NI_U8  video_full_range_flag;                 /* 黑电平和亮度与色度信号的范围  */
    NI_U8  colour_description_present_flag;       /* default value: n/a. This parameter is reserved currently. */
    NI_U8  colour_primaries;					  /* default value: n/a. This parameter is reserved currently. */
    NI_U8  transfer_characteristics;			  /* default value: n/a. This parameter is reserved currently. */
    NI_U8  matrix_coefficients; 				  /* default value: n/a. This parameter is reserved currently. */
}VENC_VUI_VIDEO_SIGNAL_S;

typedef struct tagVencVuiBitstreamRestric         /* default value: n/a. This parameter is reserved currently. */
{
	NI_U8 bitstream_restriction_flag;
}VENC_VUI_BITSTREAM_RESTRIC_S;

/* 定义视频可用信息参数结构体 */
/* 仅支持sar_width、sar_height、video_full_range_flag等3个参数可配置, 开启编码之前需设置 */
typedef struct tagVencVuiAttr
{
	VENC_VUI_ASPECT_RATIO_S 	 stVuiAspectRatio; 	/* the param of aspect ratio */		
	VENC_VUI_TIME_INFO_S         stVuiTimeInfo;		/* the param of time info */	
	VENC_VUI_VIDEO_SIGNAL_S      stVuiVideoSignal;	/* the param of video signal */
    VENC_VUI_BITSTREAM_RESTRIC_S stVuiBitstreamRestric;
}VENC_VUI_ATTR_S;

/* 定义CIR高级参数结构体 */
typedef struct tagVencCirParam
{
    NI_U32      u32CirStart;           /* [0..mbTotal] First macroblock for Cyclic Intra Refresh */
    NI_U32      u32CirInterval;        /* [0..mbTotal] Macroblock interval for Cyclic Intra Refresh, 0=disabled */
}VENC_CIR_PARAM_S;

/* 定义GDR高级参数结构体 */
typedef struct tagVencGdrParam
{
    NI_U32      u32GdrDuration;         /*how many pictures it will take to do GDR, if 0, not do GDR*/
}VENC_GDR_PARAM_S;

/* 定义SAO高级参数结构体 */
typedef struct tagVencSaoParam
{
    NI_BOOL     bEnableSao;             /* Enable SAO */
}VENC_SAO_PARAM_S;

/* 定义用户指定的SEI数据结构体,当pu8UserData为NULL且u32DataSize为0时，表关闭用户sei数据功能 */
typedef struct tagVencSeiUsrData
{
    NI_U8      *pu8UserData;    /* user_data_unregistered 字段内容，最开始的16个字节是uuid */
    NI_U32      u32DataSize;    /* user_data_unregistered 字段长度 */
}VENC_SEI_USRDATA_S;

/* 定义缩略图格式枚举 */
typedef enum tagVencJpegThumbFormat
{
    VENC_JPEG_THUMB_JPEG          = 0x10,     /* Thumbnail coded using JPEG  */
    VENC_JPEG_THUMB_PALETTE_RGB8  = 0x11,     /* Thumbnail stored using 1 byte/pixel */
    VENC_JPEG_THUMB_RGB24         = 0x13      /* Thumbnail stored using 3 bytes/pixel */
} VENC_JPEG_THUMB_FORMAT_E;

/* 定义缩略图参数结构体 */
typedef struct tagVencThumbnail
{
    NI_U32                      u32Width;       /* Width in pixels of thumbnail, Range:[16,255] */
    NI_U32                      u32Height;      /* Height in pixels of thumbnail, Range:[16,255] */
    VENC_JPEG_THUMB_FORMAT_E    enFormat;       /* Format of the thumbnail */
    NI_U32                      u32DataLength;  /* Data amount in bytes. Range according to enFormat and u8Width x u8Height:
                                                                THUMB_JPEG  : total size must less than 65527;
                                                                PALETTE_RGB8: u16DataLength=3*256+u8Width*u8Height, total size must less than 65525;
                                                                THUMB_RGB24 : u16DataLength=3*u8Width*u8Height, total size must less than 65525 */
    NI_U32                      u32DataPhyAddr; /* Thumbnail data physical addr */
    NI_VOID                     *pData;         /* Thumbnail data virtual addr */
}VENC_THUMBNAIL_S;

/* 定义帧内自参考区域参数结构体 */
typedef struct tagVencIntraArea
{
    NI_BOOL         bEnableIntraArea;       /* Force the area of all frames encoding by intra macroblocks */
    NI_U32          u32X;                   /* horizontal start point */
    NI_U32          u32Y;                   /* vertical start point */
    NI_U32          u32Width;               /* width */
    NI_U32          u32Height;              /* height */
}VENC_INTRA_AREA_S;

/* 定义高级参数结构体 */
typedef struct tagVencModParam
{
    NI_S32      s32IFrmIsIdr;         /* I帧是否是均是IDR帧   1：每个I帧均为IDR帧，取值范围[0, 1],xs7312上该参数必须设置为1 */
    NI_U32      u32UseDummyIFunc;     /* 是否使用Dummy I功能，默认值0x1.当内存不足时可通过设置不使用Dummy I来降低帧存，
                                                   减少内存消耗,第xbit对应第x通道,xs7312上该参数无效 */
}VENC_MOD_PARAM_S;

typedef struct tagVencQpMapFrame
{
    VIDEO_FRAME_S       stVideoFrame;
    VENC_QPMAP_INFO_S   stQpMapInfo;
}VENC_QPMAP_FRAME_S;

typedef struct tagVencJpegParam
{
    NI_U8                   *pu8QTableLuma;     /* Quantization table for luminance [64], overrides quantization level, zigzag order */
    NI_U8                   *pu8QTableChroma;   /* Quantization table for chrominance [64], overrides quantization level, zigzag order */
    NI_U8                   *pu8NonRoiFilter;   /* non-roi filter[128], two 8x8 filter defined for luma and chroma non-roi region */
}VENC_JPEG_PARAM_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /*  __NI_COMM_VENC_H__  */

