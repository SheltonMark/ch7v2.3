/* 
 * ni_comm_vps.h -- vps模块对外提供的头文件
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */


#ifndef __NI_COMM_VPS_H__
#define __NI_COMM_VPS_H__

#include "ni_errno.h"
#include "ni_type.h"
#include "ni_common.h"
#include "ni_define.h"
#include "list.h"
#include "ni_common_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef enum tagVpsErrCode
{    
    ERR_VPS_CHN_NOT_CREATE = 100,
    ERR_VPS_CHN_NOT_ENABLE = 101,
    ERR_VPS_NO_STREAM      = 102,  
    ERR_VPS_GET_STREAM_TIMEOUT = 103,  
    ERR_VPS_IAS_HAS_NO_NODE = 104,
    ERR_VPS_FUNC_NOT_SUPPORT = 105,
    ERR_VPS_NO_NODE        = 106,

    ERR_VPS_BUTT

} VPS_ERR_CODE_E;

#define NI_ERR_VPS_INVALID_GRPID       NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_INVALID_GRPID)

#define NI_ERR_VPS_INVALID_CHNID       NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_INVALID_CHNID)

#define NI_ERR_VPS_NULL_PTR            NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)

#define NI_ERR_VPS_ILLEGAL_PARAM       NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)

#define NI_ERR_VPS_CHN_EXIST           NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_EXIST)

#define NI_ERR_VPS_NOMEM               NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_NOMEM)

#define NI_ERR_VPS_IAS_NO_STREAM       NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_VPS_NO_STREAM)

#define NI_ERR_VPS_GET_STREAM_TIMEOUT  NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_VPS_GET_STREAM_TIMEOUT)

#define NI_ERR_VPS_GRP_NOT_CONFIG      NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_NOT_CONFIG)

#define NI_ERR_VPS_CHN_NOT_CONFIG      NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_NOT_CONFIG)

#define NI_ERR_VPS_HAS_INITED          NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_EXIST)

#define NI_ERR_VPS_NOT_INITED          NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_UNEXIST)

#define NI_ERR_VPS_NO_NODE              NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_VPS_NO_NODE)

#define NI_ERR_VPS_CHN_NOT_CREATE      NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_VPS_CHN_NOT_CREATE)

#define NI_ERR_VPS_CHN_NOT_ENABLE      NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_VPS_CHN_NOT_ENABLE)

#define NI_ERR_VPS_CHN_NOT_DISABLE     NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_NOT_DISABLE)

#define NI_ERR_VPS_IAS_HAS_NO_NODE     NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_VPS_IAS_HAS_NO_NODE)

#define NI_ERR_VPS_FUNC_NOT_SUPPORT    NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_VPS_FUNC_NOT_SUPPORT)

#define NI_ERR_VPS_FUNC_NOT_PERM       NI_DEF_ERR(NI_ID_VPS, EN_ERR_LEVEL_ERROR, ERR_NOT_PERM)

/* VPS 工作模式枚举类型 */
typedef enum tagVpsWorkMode
{
    VPS_WORK_MODE_ONLINE    = 0, /* 在线模式，ISP数据直接给到各个VPS通道 */
    VPS_WORK_MODE_OFFLINE   = 1, /* 离线模式，需要分配帧存，可以进行ISP图像旋转、裁剪、抽帧等预处理 */

    VPS_WORK_MODE_BUTT
}VPS_WORK_MODE_E;

/* 旋转类型枚举 */
typedef enum tagVpsRotateType
{
	VPS_ROTATE_BYPASS    = 0, /* BYPASS */
	VPS_ROTATE_MIRROR    = 1, /* 水平镜像 */
	VPS_ROTATE_FLIP      = 2, /* 垂直翻转 */
	VPS_ROTATE_90        = 3, /* 90度旋转 */
	VPS_ROTATE_180       = 4, /* 180度旋转 */
	VPS_ROTATE_270       = 5, /* 270度旋转 */
	VPS_ROTATE_MIRROR_90 = 6, /* 水平翻转+90度旋转 */
	VPS_ROTATE_FLIP_90   = 7, /* 垂直翻转+90度旋转 */

    VPS_ROTATE_BUTT
}VPS_ROTATE_TYPE_E;
	
/*添加行的位置说明，方便对外调用*/
typedef enum tagVpsAddLinePosition
{	
	VPS_ADD_LINE_LEFT  = 0, 	  //左面增加
	VPS_ADD_LINE_RIGHT = 1, 	  //右面增加
	VPS_ADD_LINE_TOP = 2,		  //上面增加
	VPS_ADD_LINE_BOTTOM = 3,	  //下面增加
	VPS_ADD_LINE_NONE = 4,		  //不增加
	
	VPS_ADD_LINE_BUFF
}VPS_ADDLINE_TYPE_E;

/* VPS 通道号枚举类型 */
typedef enum tagVpsChn
{
    VPS_CHN_ENC0    = 0,
    VPS_CHN_ENC1    = 1,
    VPS_CHN_ENC2    = 2,
    VPS_CHN_DISP    = 3,
    VPS_CHN_IAS0     = 4,
    VPS_CHN_IAS1     = 5,
    VPS_CHN_VIRTIAS0 = 6,    /*虚拟的IAS通道0，共用IAS0通道数据*/
    VPS_CHN_VIRTIAS1 = 7,    /*虚拟的IAS通道1，共用IAS0通道数据*/

    VPS_CHN_BUTT
}VPS_CHN_E;

/* IAS联动对应编码通道号枚举类型 */
typedef enum tagVpsChnIasRelate2Enc
{
    VPS_CHN_IAS_RELATE2NONE = 0, /* IAS通道不与编码通道联动 */
    VPS_CHN_IAS_RELATE2ENC0,     /* IAS通道与编码通道0联动 */
    VPS_CHN_IAS_RELATE2ENC1,     /* IAS通道与编码通道1联动 */
    VPS_CHN_IAS_RELATE2ENC2,     /* IAS通道与编码通道2联动 */

    VPS_CHN_IAS_RELATE_BUTT
}VPS_CHN_IAS_RELATE_E;

/* DISP 模式枚举类型 */
typedef enum tagVpsDispMode
{
    VPS_DISP_MODE_NORMAL = 0,   /* 正常模式，传给VO显示*/
    VPS_DISP_MODE_CAPTURE = 1,  /* 抓YUV图模式 */

    VPS_DISP_BANK_BUTT
}VPS_DISP_MODE_E;

/* 通道属性枚举类型 */
typedef enum tagVpsChnAttrType
{
    VPS_CHN_ATTR_TYPE_ENC  = 0, /* 编码 */
    VPS_CHN_ATTR_TYPE_DISP = 1, /* 显示 */
    VPS_CHN_ATTR_TYPE_IAS  = 2, /* 智能 */
    
    VPS_CHN_ATTR_TYPE_BUTT
}VPS_CHN_ATTR_TYPE_E;

/*用户选择组是软件控制方式，还是硬件控制方式*/
typedef enum tagVpsGrpBufCtrl
{
    VPS_GRP_BUF_CTRL_HW    = 0,               /* GRP的buf是硬件控制 */
	VPS_GRP_BUF_CTRL_SOFT  = 1,               /* GRP的buf是软件控制 */
	
	VPS_GRP_BUF_CTRL_BUTT                     
	
}VPS_GRP_BUF_CTRL_E;

/* VPS通道YUV转GRB分量的系数 */
typedef struct tagVpsGsgK 
{ 
    NI_BOOL     bUseRecommend;  /* 是否使用驱动内部推荐配置,如果设置为NI_FALSE时需要配置下面的自定义参数 */
    NI_U32      u32GsgKrY;      /* YUV转R分量的系数0 */
    NI_U32      u32GsgKrV;      /* YUV转R分量的系数1 */
    NI_U32      u32GsgKgY;      /* YUV转G分量的系数0 */
    NI_U32      u32GsgKgU;      /* YUV转G分量的系数1 */
    NI_U32      u32GsgKgV;      /* YUV转G分量的系数2 */
    NI_U32      u32GsgKbY;      /* YUV转B分量的系数0 */
    NI_U32      u32GsgKbU;      /* YUV转B分量的系数1 */
}VPS_GSG_K_S;

/* 定义用户获取GRP图像的模式 */
typedef enum tagVpsGetGrpFrmMode
{
    VPS_GETGRPFRM_MODE_NONE             = 0,   /* 用户不获取vps grg图像,由底层sdk将yuv传给下级处理模块 */
    VPS_GETGRPFRM_MODE_AFTER_ORIGIN     = 1,   /* 用户从原始位置获取vps grg图像  , 该模式下需要用户调用NI_MDK_VPS_GetGrpFrame，并在处理完后调用NI_MDK_VPS_SendGrpFrame送回，否则将因收不到yuv图像而断流 */
    VPS_GETGRPFRM_MODE_AFTER_LDC        = 2,   /* 用户从处理完ldc之后获取vps grg图像  , 该模式下需要用户调用NI_MDK_VPS_GetGrpFrame，并在处理完后调用NI_MDK_VPS_SendGrpFrame送回，否则将因收不到yuv图像而断流 */

    VPS_GETGRPFRM_MODE_BUTT
}VPS_GETGRPFRM_MODE_E;

/* 定义VPS的模块参数结构体 */
typedef struct tagVpsModParam
{
    NI_S32      s32AilgnIas;            /* IAS通道输出的每个平面的地址均需要64对齐   ，默认值：64*/
    NI_S32      s23WaitVpsDone;        /* 测试模式下，是否等待一帧完成模块配置参数:，默认值：0*/
    NI_S32      s32SrcBufNum;          /* 定义group端软件控制帧存模式下用于缓存Src源的buf块数，默认值为：0  */
    NI_S32      s32EncPrivPool;         /*enc通道使用公共换存池还是私有缓存池，       0:私有公共换存池，1：私有缓存池，默认值:0*/
    NI_S32      s32DispBufNum;         /*disp通道在VPS_DISP_MODE_CAPTURE模式的时候配置生效，默认值：0*/
    NI_S32      s32SendToLdc;          /*VPS工作在离线软件模式下，LDC工作后，决定是否把GRP数据送给LDC。 0：不送，1：送，默认值:1*/
    NI_S32      s32UseBufNum;          /*定义编码通道当前使用的帧存数 默认值为：3*/
    NI_S32      s32TrigRatioNum;        /*定义单帧存方案trigger中断触发阈值占总行数比例的值的分子 , 默认值为：1*/
    NI_S32      s32TrigRatioDenom;      /*定义单帧存方案trigger中断触发阈值占总行数比例的值的分母, 默认值为：2*/
    
    NI_U32      u32FpsDenom;            /*帧率的分母值，实际帧率是用户配置帧率除以分母 , 默认值为：1*/

}VPS_MOD_PARAM_S;

typedef struct tagVpsCrop
{
    NI_BOOL    bEnable;
    RECT_S     stRect;
}VPS_CROP_S;

typedef struct tagVpsGrpParam
{
    /* 如下接口用于解决前级不同分辨率输入、不同模式（eg:stitch模式开关）切换造成的内存碎片问题，本接口需要在NI_MDK_VPS_SetGrpAttr之前调用 
       u32MinBufSize的计算公式如下：
       开启离线模式无损压缩时：
            u32SrcWrYStride = NI_align(((u32InWidth * 68) >> 6) + 16, 16);
       不开启离线模式无损压缩时：
            u32SrcWrYStride = NI_align(u32InWidth, 16);
       u32BlockWidth = u32SrcWrYStride / 16;
       u32BlockHeight = NI_align(u32InHeight, 16) / 16;
       u32BlockNum = (u32BlockWidth * u32BlockHeight + 4) / 5;
       u32MinBufSize = u32BlockNum << 11;
    
       开启stitch模式时，需要在u32MinBufSize * 2。即：u32MinBufSize = u32MinBufSize * 2;
    */

    NI_U32 u32MinBufSize;
}VPS_GRP_PARAM_S;

/* 定义各种拼接模式下内存使用情况*/
typedef enum tagVpsStitchBufMode
{
    VPS_STITCH_BUF_MODE_DST                 = 0,  /* GRPx源内存申请参照分辨率进行申请，Dst申请的内存为GRPx源内存的2倍 */
    VPS_STITCH_BUF_MODE_NODST_UP            = 1,  /* GRPx源内存申请参照分辨率的2倍申请，GRPx源数据存放在上半部分，Dst不再申请内存，仅垂直拼接有效 */
    VPS_STITCH_BUF_MODE_NODST_DOWN          = 2,  /* GRPx源内存申请参照分辨率的2倍申请，GRPx源数据存放在下半部分，Dst不再申请内存，仅垂直拼接有效 */
    VPS_STITCH_BUF_MODE_BUTT
}VPS_STITCH_BUF_MODE_E;

/* 定义多路拼接模式*/
typedef enum tagVpsStitchMode
{
    VPS_STITCH_MODE_BLEND     = 0,      /* 水平融合拼接模式，在拼接处做融合处理 */
    VPS_STITCH_MODE_BLEND_VER     = 1,  /* 垂直融合拼接模式，在拼接处做融合处理，该模式目前不支持*/
    VPS_STITCH_MODE_NOBLEND_HOR   = 2,  /* 水平无融合拼接模式，将两路图像水平放在一起，拼接处不做融合处理 */
    VPS_STITCH_MODE_NOBLEND_VER   = 3,  /* 垂直无融合拼接模式，将两路图像垂直放在一起，拼接处不做融合处理 */
    VPS_STITCH_MODE_BUTT
}VPS_STITCH_MODE_E;


/* VPS模块离线模式下的组属性参数结构体 */
typedef struct tagVpsOffLineAttr 
{
    NI_BOOL                 bCompress;           /* 离线模式下，数据上下内存是否启用无损压缩,需要做Ldc时不能开启 */
    NI_BOOL                 bCropEn;             /* 离线模式下,图像剪切使能 */
    VPS_GRP_BUF_CTRL_E      enBufCtrl;           /* 用户选择是软件控制方式还是硬件控制方式，软件控制方式下才可以进行LDC应用和获取grp图像进行消抖应用开发 */
    VPS_GETGRPFRM_MODE_E    enGetGrpFrmMode;     /* 离线模式下用户获取grp图像的模式， enBufCtrl必须是VPS_GRP_BUF_CT RL_SOFT软件控制模式下才能获取grp图像 */

    RECT_S                  stCropRect;          /* 离线模式下,图像剪切参数 */
    NI_U32                  u32OutFps;           /* 离线模式下,抽帧后输出帧率 */ 
    NI_BOOL                 bStitchMode;         /* 是否拼接模式，所有GRP的双拼配置必须一样 */
    VPS_STITCH_MODE_E       enStitchMode;        /* 如果是双拼，进行双拼模式的选择。注：当 bStitchMode为FALSE时，该参数无效,所有GRP的双拼配置必须一样*/
    VPS_STITCH_BUF_MODE_E   enStitchBufMode;     /* 目前仅供垂直拼接使用，水平拼接使用默认设置即可。注：当 bStitchMode为FALSE时，该参数无效，所有GRP的双拼配置必须一样 */
}VPS_OFFLINE_ATTR_S;

/* VPS模块组属性参数结构体,必须在通道使能的时候配置下去*/
typedef struct tagVpsGrpAttr 
{
    NI_U32                  u32InWidth;     /* ISP输入图像宽 */
    NI_U32                  u32InHeight;    /* ISP输入图像高 */
    NI_U32                  u32InFps;       /* ISP输入图像帧率 */
    VPS_WORK_MODE_E         enWorkMode;     /* 所有Grp必须配相同的模式 */
    VPS_OFFLINE_ATTR_S      stOffLineAttr;  /* enWorkMode为VPS_WORK_MODE_OFFLINE时有效，离线模式配置参数 */
    NI_BOOL                 bMultiEye;      /* 是否应用于多目, 所有Grp配置必须一致 */
}VPS_GRP_ATTR_S;


/* VPS模块组拼接属性参数结构体 */
typedef struct tagVpsGrpStitchAttr 
{
    VPS_STITCH_MODE_E       enStitchMode;   /* 拼接模式，参与拼接的grp模式必须相同 */
    NI_BOOL                 bSyncGrp;       /* 是否根据时间戳进行各路图像的同步 */

    NI_U32                  u32OutWidth;    /* 双路拼接后的总的输出宽度，参与拼接的grp该参数须设为一致 */
    NI_U32                  u32OutHeight;   /* 双路拼接后的总的输出高度，参与拼接的grp该参数须设为一致，水平拼接模式下必须与输入高度相等 */
    
    NI_U32                  u32LutPhyAddr;  /* 亮度查找表物理地址 */
    NI_S32                  s32Coef;        /* 提亮系数 */
}VPS_GRP_STITCH_ATTR_S; 


/* 隐私遮挡设置参数结构体 */
typedef struct tagVpsMaskAttr 
{ 
    COLOR_YUV_S     stMaskColor;
    RECT_S          astMaskRect[VPS_MAX_CHN_MASK_NUM];  /* 遮挡坐标 */
}VPS_MASK_ATTR_S;

/* 所有隐私遮挡设置参数结构体 */
typedef struct tagVpsAllMaskAttr 
{
    VPS_MASK_ATTR_S astChnMaskAttr[VPS_MASK_MAX_CHN_NUM]; /* 一个group中VPS_CHN_ENC0~2，VPS_CHN_DISP,IAS1五个通道的遮挡属性。若未设置通道属性，则该通道的mask不会生效 */
}VPS_ALL_MASK_ATTR_S;


/* VPS模块SCD 长参考帧属性参数结构体 */
typedef struct tagVpsEncScdLrfAttr 
{ 
    NI_BOOL bScdEn;                 /* 画面切换检测使能 */
    NI_BOOL bUseRecommend;          /* 是否使用驱动内部推荐配置,如果设置为NI_FALSE时需要配置下面的自定义参数 */
    NI_U32  u32ScdMulti0;           /* SCD检测系数0 */
    NI_U32  u32ScdMulti1;           /* SCD检测系数1 */
    NI_U32  u32ScdTh0;              /* SCD权重系数0 */
    NI_U32  u32ScdTh1;              /* SCD权重系数1 */
} VPS_ENC_SCD_LRF_ATTR_S;

/* VPS通道编码配置参数结构体 */
typedef struct tagVpsEncChnAttr
{
    NI_U32          u32YStride;  /* 亮度stride,配置为0表示不使用stride */
    NI_U32          u32CStride;  /* 色度stride,plaar模式下uv分量相同，配置为0表示不使用stride */
    PIXEL_FORMAT_E  enOutFormat; /* 数据输出格式 */
    NI_U32          u32OutFps;   /* 编码通道输出帧率 */
}VPS_ENC_CHN_ATTR_S;

/* DISP通道属性参数结构体 */
typedef struct tagVpsDispChnAttr 
{ 
    VPS_DISP_MODE_E     enDispMode;     /* 显示通道模式 */
    PIXEL_FORMAT_E      enOutFormat;    /* DISP通道数据输出格式 */
    NI_U32              u32BuffNum;     /* 使用的buff数，
                                                    在VPS_DISP_YUV_CAPTURE模式下: 取值范围[4,20] 
                                                    在VPS_DISP_MODE_NORMAL模式下: 取值范围[2,4]  */
    NI_BOOL             bCompress;      /* 输出YUV是否开启无损压缩, 抓图模式下不容许开启无损 */
    NI_U32              au32Stride[3];  /* 对应三个颜色分量的stride0 stride1 stride2 */                                                
	NI_U32              u32OutFps;      /* 显示通道数据输出帧率 */
}VPS_DISP_CHN_ATTR_S;

/* VPS 帧数据信息 */
typedef struct tagVpsFreamInfo
{
    NI_U32        u32TimeStamp;
    NI_U32        u32FrameSize;     /* VideoFrame[0]对应的帧数据占用的内存size，字节数 */
    NI_U64        u64GrpCapPts;     /* VPS GRP获取该帧的时间信息，用于与3A数据同步。仅VPS GRP设置为离线软件模式下有用 */
    VIDEO_FRAME_S astVideoFrame[2]; /* IAS0通道对应的联动通道数据存在数组1中 */
}VPS_FRAME_INFO_S;

/* stitch 组帧信息 */
typedef struct tagVpsStitchGrpFrame
{
    NI_U32        u32SrcFrameSize;        /* VideoFrame[0]对应的帧数据占用的内存size，字节数 */
    VIDEO_FRAME_S astSrcFrame[2];         /* 拼接前grp0和grp1的原始yuv数据 */

    NI_U32        u32DstFrameBufSize;     /* 驱动层提供的拼接目的帧存大小*/
    VIDEO_FRAME_S stDstFrameBuf;          /* 驱动层提供的拼接目的帧存信息，该buf内无有效数据，提供帧存给上层应用填充拼接后图像，再送回vps给后级模块 */
}VPS_STITCH_GRP_FRAME_S;


/* IAS通道属性参数结构体 */
typedef struct tagVpsIasChnAttr 
{ 
    PIXEL_FORMAT_E          enOutFormat;        /* IAS 数据输出格式 */
    NI_U32                  u32BuffNum;         /* 使用的buff数,驱动中实际申请帧存个数为BuffNum+2 */
    NI_U32                  u32OutFps;          /* 输出帧率, 是否应用于多目, 所有Grp配置必须一致，所有ias通道的数据来源，虚拟通道还可以进一步软抽帧 */ 
    NI_U32                  au32Stride[3];      /* 主通道对应的stride0 stride1 stride2 */
    VPS_CHN_IAS_RELATE_E    enIasRel2EncChn;    /* IAS联动对应的编码通道 */
}VPS_IAS_CHN_ATTR_S;

/* VPS模块通道属性参数结构体 */
typedef struct tagVpsChnAttr
{
    NI_BOOL                 bCropEn;        /* VPS通道剪切使能开关 */
    RECT_S                  stCropRect;     /* VPS通道剪切参数，当bCutEn为TRUE时有效 */
    NI_U32	                u32OutWidth;    /* VPS通道输出图像宽 */
    NI_U32	                u32OutHeight;   /* VPS通道输出图像高 */ 

    VPS_CHN_ATTR_TYPE_E     enAttrType;     /* 选择下面union联合体的属性类型 */
    union
    {
        VPS_ENC_CHN_ATTR_S  stEncAttr;
        VPS_DISP_CHN_ATTR_S stDispAttr;
        VPS_IAS_CHN_ATTR_S  stIasAttr;
    };
}VPS_CHN_ATTR_S;

/* 定义送给vps grp组的图像数据结构 */
typedef struct tagVpsSendGrpFrm
{
    NI_BOOL             bCropEn;
    RECT_S              stCropRect;          /*剪切参数*/ 
    VIDEO_FRAME_S       stVideoFrame;
}VPS_SEND_GRPFRM_S;

/* VPS模块组设置帧率信息 */
typedef struct tagVpsAllFpsAttr 
{
    NI_U32                  u32GrpInFps;       /* ISP输入图像帧率      */
    NI_U32                  u32GrpOutFps;      /* GRP的输出帧率，必须小于等于输入帧率u32GrpInFps,离线模式下有效 */

    NI_U32                  au32ChnOutFps[VPS_GRP_MAX_CHN_NUM - 2];/* 通道帧率，不包括虚拟通道，必须小于等于组的输出帧率u32GrpOutFps*/
}VPS_ALL_FPS_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

