/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef __NI_COMM_SMART_ENC_H__
#define __NI_COMM_SMART_ENC_H__

/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include "ni_type.h"
#include "ni_debug.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ========================================================================== */
/*                             宏和类型定义区                                 */
/* ========================================================================== */
typedef NI_VOID *SMART_ENC_HANDLE;
typedef struct SMART_ENC_HANDLE_s
{
    SMART_ENC_HANDLE Handle[VENC_MAX_CHN_NUM];  //智能编码句柄
    bool curInfoEnable[VENC_MAX_CHN_NUM];       //通道curinfo使能
    pthread_mutex_t	Lock;                       //数据锁
}SMART_ENC_HANDLE_s, SMART_ENC_HANDLE_p;
// typedef unsigned long SMART_ENC_PARAM_CFG_ADDR;

/* 错误代码 */
typedef enum tagSmartEncErrCode {
    SMART_ENC_ERR_WRONG_PARAM  = 0xFFFFFFFE,
    SMART_ENC_ERR_WRONG_HANDLE = 0xFFFFFFFD,
    SMART_ENC_ERR_NULL_PTR     = 0xFFFFFFFC,
} SMART_ENC_ERR_CODE_E;

/* 编码器编码类型 */
typedef enum tagSmartEncType {
    SMART_ENC_TYPE_H264 = 0, /* h264编码模式 */
    SMART_ENC_TYPE_H265 = 1, /* h265编码模式 */
    SMART_ENC_TYPE_BUTT
} SMART_ENC_TYPE_E;

/* 编码模式 */
typedef enum tagSmartEncMode {
    SMART_ENC_MODE_CBR = 0, /* CBR码控模式 */
    SMART_ENC_MODE_VBR = 1, /* VBR码控模式 */
    SMART_ENC_MODE_BUTT
} SMART_ENC_MODE_E;

/* 智能编码模式 */
typedef enum tagSmartEncRoiMode {
    SMART_ENC_ROI_MODE_M0 = 0, /* 支持基础智能编码模式 */
    SMART_ENC_ROI_MODE_M1 = 1, /* 可支持编码器开启ROI map模式 */
    SMART_ENC_ROI_MODE_M2 = 2, /* 支持编码器开ROI map叠加前景检测 */
    SMART_ENC_ROI_MODE_BUTT
} SMART_ENC_ROI_MODE_E;

/* Qp范围 */
typedef struct tagtagSmartEncQpRange {
    NI_U16 u16QpMax; /* QP的最大值，取值范围：[0,51] */
    NI_U16 u16QpMin; /* QP的最小值，取值范围：[0,51] */
} SMART_ENC_QP_RANGE_S;

/* 编码图像类型 */
typedef enum tagSmartEncImageType {
    SMART_ENC_IMAGE_TYPE_YUV420SP = 0x1, /* YUV420 SemiPlanar */
    SMART_ENC_IMAGE_TYPE_YUV422SP = 0x2, /* YUV422 SemiPlanar */
    SMART_ENC_IMAGE_TYPE_YUV420P  = 0x3, /* YUV420 Planar */
    SMART_ENC_IMAGE_TYPE_YUV422P  = 0x4, /* YUV422 planar */
    SMART_ENC_IMAGE_TYPE_BUTT
} SMART_ENC_IMAGE_TYPE_E;

/* 图像类结构体 */
typedef struct tagSmartEncFrame {
    SMART_ENC_IMAGE_TYPE_E enImageType;         /* 图像的类型 */
    NI_U8                 *apu8FrameVirAddr[3]; /* 编码图像虚拟地址，分别对应Y,U,V */
    NI_U32                 au32FramePhyAddr[3]; /* 编码图像物理地址，分别对应Y,U,V */
    NI_U16                 au16Stride[3];       /* 图像跨度 */
    NI_U16                 u16Width;            /* 图像的宽 */
    NI_U16                 u16Height;           /* 图像的高 */
    NI_U16                 u16Reserved;
} SMART_ENC_FRAME_S;

/* roi优先级，优先级越高则分配码率越高 */
typedef enum tagSmartEncRoiPri {
    SMART_ENC_ROI_PRI_0 = 0, /* 优先级0，背景优先级 */
    SMART_ENC_ROI_PRI_1 = 1, /* 优先级1，高于背景优先级 */
    SMART_ENC_ROI_PRI_2 = 2, /* 优先级2，高于优先级1 */
    SMART_ENC_ROI_PRI_3 = 3, /* 优先级3，最高优先级，不建议用户使用 */
    SMART_ENC_ROI_PRI_NUM
} SMART_ENC_ROI_PRI_E;

/* 目标对象区域，用户使用外部算法，将结果导入 */
typedef struct tagSmartEncObjRect {
    SMART_ENC_ROI_PRI_E enPri;     /* 对象区域优先级 */
    NI_U16              u16StartX; /* 区域水平起点 */
    NI_U16              u16StartY; /* 区域垂直起点 */
    NI_U16              u16Height; /* 区域高度 */
    NI_U16              u16Width;  /* 区域宽度 */
} SMART_ENC_OBJ_RECT_S;

/* 对象区域信息 */
typedef struct tagSmartEncForeInfo {
    NI_U32                u32ObjRectNum; /* 对象区域数量 */
    SMART_ENC_OBJ_RECT_S *pstObj;        /* 对象区域信息 */
} SMART_ENC_FORE_INFO_S;

/* Cuinfo组成结构体 */
typedef struct tagSmartEncCuInfo {
    NI_U32 u32PicMbWidth;  /* 对齐后的图像16宏块宽度，图像对齐方式HEVC:64px，H264:16px */
    NI_U32 u32PicMbHeight; /* 对齐后的图像16宏块高度，图像对齐方式HEVC:64px，H264:16px */

    /* 每4个bytes代表32x32(hevc)or16x16(h264)的MMS,MAD[31:27],MSE[26:17],SATD[16:0] */
    NI_U32  u32MmsInfoPhyAddr;  /* (MAD,MSE, SATD)信息所在的物理地址 */
    NI_U32 *pu32MmsInfoVirAddr; /* (MAD,MSE, SATD)信息所在的虚拟地址 */
    NI_U32  u32MmsInfoSize;

    /* 每2个bytes代表32x32(hevc)or16x16(h264)的bitcost,内存中以小端格式摆放 */
    NI_U32  u32BitsInfoPhyAddr;  /* bitcost信息所在的物理地址 */
    NI_U16 *pu16BitsInfoVirAddr; /* bitcost信息所在的虚拟地址 */
    NI_U32  u32BitsInfoSize;

    /* 每1个bytes代表16x16的qp值 */
    NI_U32 u32QpInfoPhyAddr; /* qp信息所在的物理地址 */
    NI_U8 *pu8QpInfoVirAddr; /* qp信息所在的虚拟地址 */
    NI_U32 u32QpInfoSize;
} SMART_ENC_CU_INFO_S;

/* QpMap帧相关信息 */
typedef struct tagSmartEncQpMapInfo

{
    NI_U16 u16QpMapWidth;  /* qp map表的宽度，即水平方向BlockUnit的个数 */
    NI_U16 u16QpMapHeight; /* qp map表的高度，即垂直方向BlockUnit的个数 */

    NI_PHYS_ADDR_T  QpMapPhyAddr;  /* qp map表物理地址 */
    NI_VIRT_ADDR_T *pQpMapVirAddr; /* qp map表虚拟地址,用户态地址 */
    NI_U32          u32QpMapSize;
} SMART_ENC_QPMAP_FRAME_S;

/* process输入参数 */
typedef struct tagSmartEncFrameInfo {
    SMART_ENC_CU_INFO_S   stCuInfo;       /* 编码器输出的cuinfoBuf */
    NI_U16                u16Gain;        /* 当前图像增益 */
    NI_U32                u32AverBitRate; /* 当前实时码率,单位bit/s */
    SMART_ENC_FORE_INFO_S stForeInfo;     /* 外部输入的前景区域信息 ax520上该参数无效*/
    SMART_ENC_FRAME_S     stDetectStream; /* 检测图像相关信息 ax520上该参数无效*/
} SMART_ENC_FRAME_INFO_S;

/* 智能编码参数配置地址 */
typedef struct tagSmartEncParamCfg {
    NI_VOID *pParamCfgVirAddr; /* 参数配置虚拟地址 */
    NI_U32   u32ParamCfgLen;   /* 参数配置长度 */
} SMART_ENC_PARAM_CFG_S;

/* 智能编码动态场景参数 */
typedef struct tagSmartEncSceneTarget {
    NI_U64 u64MotionTargetBitMin; /* 动态场景一段时间最小bit */
    NI_U64 u64MotionTargetBitMax; /* 动态场景一段时间最大bit */
    NI_U64 u64StaticTargetBitMin; /* 静态场景一段时间最小bit */
    NI_U64 u64StaticTargetBitMax; /* 静态场景一段时间最大bit */
    NI_U64 u64StateTime;          /* 码率统计时间，单位为/s,范围[1, 86400] */
} SMART_ENC_SCENE_TARGET_S;

/* 智能编码动静态阈值参数 */
typedef struct tagSmartEncSceneSens {
    NI_U32 u32MotionThr; /* 判断为动态场景的阈值，范围[10, 15000], 推荐1500 */
    NI_U32 u32StaticThr; /* 判断为静态场景的阈值，范围[10, 15000], 推荐500 */
} SMART_ENC_SCENE_SENS_S;

/* 设置动态场景相关参数 */
typedef struct tagSmartEncMotionAttr {
    SMART_ENC_SCENE_TARGET_S stSceneTarget; /* 当用户开启动态场景判断时，需配置相应的码率 */
    SMART_ENC_SCENE_SENS_S   stSceneSens;   /* 当用户开启动态场景判断时，需配置场景判断的灵敏度 */
} SMART_ENC_MOTION_ATTR_S;

/* 功能模式选择 */
typedef struct tagSmartEncHandleAttr {
    SMART_ENC_TYPE_E        enEncType;       /* 编码器类型选择, 0:h264 1:h265 */
    SIZE_S                  stEncImgSize;    /* 编码图像大小 */
    SMART_ENC_ROI_MODE_E    enSmartMode;     /* 智能编码的模式, 0:M0 1:M1 2:M2 */
    SMART_ENC_PARAM_CFG_S   stCfgFileAddr;   /* 配置参数文件地址 */
    NI_BOOL                 bMotionMod;      /* 动态场景判断是否开启 */
    SMART_ENC_MOTION_ATTR_S stMotionAttr;    /* 开启动态场景时配置参数 */
    NI_BOOL                 bForeGroundMode; /* 内部检测算法是否开启 ax520上该参数无效*/
    SIZE_S                  stDetectImgSize; /* 检测图像大小 ax520上该参数无效*/
} SMART_ENC_HANDLE_ATTR_S;

/* 设置配置参数(可动态修改) */
typedef struct tagSmartEncSetParam {
    SMART_ENC_MODE_E enEncMode;          /* 编码模式VBR/CBR */
    NI_U32           u32BitRate;         /* 目标码率,单位kbps,该码率必须和设置给编码器的码率保持一致 */
    NI_U32           u32BitRateUpLimit;  /* 码率上下波动限制,单位1% */
    NI_U32           u32MaxBitRate;      /* 最大码率,单位kbps,用于VBR模式 */
    NI_U32           u32MinBitRate;      /* 最小码率,单位kbps,用于VBR模式 */
    NI_U16           u16Quality;         /* 图像质量,[0,7],4为推荐值 */
    NI_U32           u32Gop;             /* Gop参数 */
    NI_U32           u32FrmRateEnc;      /* 编码器输入帧率 range:[1, 65535] */
    NI_U32           u32FrmRateSmartEnc; /* 智能编码输入帧率 range:[1, 65535] */
} SMART_ENC_PARAM_S;

/* 输出码控参数 */
typedef struct tagSmartEncRes {
    NI_BOOL                 bQpMapEn;         /* 是否启用QpMap功能 */
    SMART_ENC_QPMAP_FRAME_S stQpMapFrame;     /* QpMap帧相关参数 */
    SMART_ENC_QP_RANGE_S    stQpRangeI;       /* I帧Qp范围 */
    SMART_ENC_QP_RANGE_S    stQpRangeP;       /* P帧Qp范围 */
    NI_BOOL                 bEnIDR;           /* 强制I帧 */
    NI_U32                  u32TargetBitRate; /* 智能编码输出码率,单位kbps*/
    NI_U16                  u16QpHdrMin;      /* 最小Hdr */
    NI_U32                  u32Gop;           /* 输出Gop大小*/
    NI_U32                  u32CtbRc;         /* CTB QP调整模式 ax520上该参数无效 */
    NI_U32                  u32StaIPerc;      /* 对应编码器参数u32StaticSceneIbitPercent */
    NI_U32                  u32BlockRCSize;   /* 对应编码器参数u32BlockRCSize ax520上该参数无效*/
} SMART_ENC_RESULT_S;

/* 智能编码状态信息 */
typedef struct tagSmartEncStatus {
    NI_F64 f64TimeOfFrame;      /* 智能编码编码一帧耗时 */
    NI_F64 f64MaxTimeOfFrame;   /* 智能编码编码一帧最大耗时 */
    NI_F64 f64MinTimeOfFrame;   /* 智能编码编码一帧最小耗时 */
    NI_F64 f64AverTimeOfFrame;  /* 智能编码编码一帧平均耗时 */
    NI_U32 u32MmzSize;          /* 智能编码所占内存大小 */
    NI_U32 u32SuccessCnt;       /* 智能编码成功帧数 */
    NI_U32 u32ErrorCnt;         /* 智能编码失败帧数 */
    NI_U32 u32SetParamCnt;      /* 调用SetParam接口次数 */
    NI_U32 u32GetParamCnt;      /* 调用GetParam接口次数 */
    NI_U32 u32SetMotionAttrCnt; /* 调用SetMotion接口次数 */
    NI_U32 u32GetMotionAttrCnt; /* 调用GetMotion接口次数 */
    NI_U32 u32ProcessCnt;       /* 调用Process接口次数 */
} SMART_ENC_STATUS_S;

typedef struct SmartEncAttr
{
    SMART_ENC_TYPE_E enEncType;			//编码方式
    SMART_ENC_MODE_E enEncMode;			//码率方式
    SMART_ENC_ROI_MODE_E enSmartMode;	//智能编码方式	
    NI_U32 u32BitRate;		            //目标码率			
	NI_U16 u16Quality;                  //图像质量
    NI_U32 u32Gop;                      
    NI_U32 u32Height;                   
    NI_U32 u32Width;
}SMART_ENC_ATTR;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of #ifndef __NI_COMM_SMART_ENC_H__ */
