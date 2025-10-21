/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NI_COMM_AISPD_H_
#define _NI_COMM_AISPD_H_

#include "ni_type.h"
#include "ni_common.h"
#include "ni_comm_aispalg.h"
#include "ni_comm_pisp.h"
#include "ni_comm_aisp.h"
#include "plat_isp_redefine.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/********************** CFG ***********************************/
typedef enum tagAispCfgType {
    AISP_CFG_TYPE_NULL       = 0x00,
    AISP_CFG_TYPE_LINE_COLOR = 0x01,
    AISP_CFG_TYPE_LINE_BLACK = 0x02,

    AISP_CFG_TYPE_WDR_COLOR = 0x03,
    AISP_CFG_TYPE_WDR_BLACK = 0x04,

    AISP_CFG_TYPE_BUTT = 0xff, /* 该枚举只占用11bit最大为0xff */
} AISPD_CFG_TYPE_E;

typedef struct tagAispdVersionComCfg {
    NI_U32  u32CrcCheck;        /*RO, 4 CRC校验 工具导出时更新 该部分值 */
    NI_CHAR aszAispVersion[24]; /*RO, 28 AISP版本 通常不要改动  用于配置文件和2a库的版本校验*/
    NI_CHAR aszSensorType[8];   /*RO, 48 sensor 类型 */
    NI_CHAR aszProductType[8];  /*RO, 56 产品类型 如BASE表示基线版本, 根据客户定制的可以标识客户名 */
    NI_CHAR aszProductVer[2];   /*RO, 58 产品类型 版本号 */
    NI_CHAR aszGenTime[6];      /*RO, 64 生成时间 */
    NI_U32  u32PageNum;         /*RO, 68 配置页数 */
} AISPD_VERSION_COM_CFG_S;

typedef union tagAispCfgModInfo {
    struct
    {
        NI_U32 enCfgType : 8;   /* RO 具体内容见 AISPD_CFG_TYPE_E 的描述 */
        NI_U32 enVideoType : 8; /* RO 具体内容见 AISP_VIDEO_TYPE_E 的描述 */
    };
    NI_U32 u32CfgInfo; /* RO */
} AISPD_CFG_INFO_U;

typedef struct tagAispdVersionPageCfg {
    NI_U32           u32CrcCheck; /* RO, 4 CRC校验 工具导出时更新 该部分值 */
    AISPD_CFG_INFO_U unCfgInfo;   /* RO, 4 配置信息  通常不要改动*/
} AISPD_VERSION_PAGE_CFG_S;
/********************** AE ***********************************/
typedef struct tagAispdAeCfg {
    AE_ATTR_S   stAeAttr;                      /*RW, AE算法属性参数 */
    AE_ROUTE_S  stAeRoute;                     /*RW, AE算法曝光路径参数 */
    AE_WIN_WT_S stAeWinWt;                     /*RW, AE算法区域权重参数 */
    NI_U8       au8Level[AISP_IMG_STYLE_BUTT]; /*RW, 风格亮度等级 */
    NI_U8       u8Reserved;                    /*RW, 无效参数强制结构体对齐 */
    // RESERVE_FORCE_STRUCT_ALIGN(Level, sizeof(NI_U8) * AISP_IMG_STYLE_BUTT, 4)               /* RW, 无效参数强制结构体对齐 */
} AISPD_AE_CFG_S;
/********************** AWB ***********************************/
typedef struct tagAispdAwbCfg {
    AWB_CT_RANGE_S      stCtRange;                      /* RW, 白块色温区间 */
    AWB_CURVE_S         stCurve;                        /* RW, 色温曲线 */
    AWB_GAIN_LIMIT_S    stLimit;                        /* RW, 白块增益限幅 */
    AWB_LIGHT_INFO_S    astLightSrc[AWB_LS_TYPE_NUM];   /* RW, 各光源点的Gain参数,灯箱标定 */
    AWB_SPECIAL_LIGHT_S stSpecialLight;                 /* RW, 特殊光源点参数 */
    NI_U32              u32ValidNumThr;                 /* RW, 白块有效个数阈值 */
    AWB_LS_WT_S         astAwbLsWt[AISP_WB_SCENE_BUTT]; /* RW, 由环境亮度决定的白块权重系数 */
} AISPD_AWB_CFG_S;
/********************** BLC ***********************************/
typedef struct tagAispdBlackLevel {
    NI_U16 u16BlcR;  /*RW, R像素黑电平偏移值, U10.0,range[0,1023], default:0x0*/
    NI_U16 u16BlcGr; /*RW, Gr像素黑电平偏移值, U10.0,range[0,1023], default:0x0*/
    NI_U16 u16BlcGb; /*RW, Gb像素黑电平偏移值, U10.0,range[0,1023], default:0x0*/
    NI_U16 u16BlcB;  /*RW, B像素黑电平偏移值, U10.0,range[0,1023], default:0x0*/
} AISPD_BLACK_LEVEL_S;

typedef struct tagAispdBlcCfg {
    AISPD_BLACK_LEVEL_S astBlcLevel[AISP_MAX_DB_NUM]; /*RW, BLC参数 AISP_MAX_DB_NUM */
} AISPD_BLC_CFG_S;
/********************** CCM ***********************************/

typedef struct tagAispdCcmRgbCfg {
    NI_S16 s16OffsetR; /*RW, CCM模块R基色的偏移量, S8.0, Range:[-256,255] default::0x0 */
    NI_S16 s16OffsetG; /*RW, CCM模块G基色的偏移量, S8.0, Range:[-256,255] default::0x0 */
    NI_S16 s16OffsetB; /*RW, CCM模块B基色的偏移量, S8.0, Range:[-256,255] default::0x0 */
} AISPD_CCM_RGB_CFG_S;

typedef struct tagAispdCcmCfg {
    NI_U8               au8Sat[AISP_MAX_DB_NUM];       /* RW, 各增益段图像饱和度 AISP_MAX_DB_NUM */
    NI_U32              au32Db[CCM_CT_NODE_NUM];       /* RW, 低中高增益切换DB值 */
    CCM_MATRIX_S        astCcmLow[CCM_CT_NODE_NUM];    /* RW, 低增益的CCM矩阵 */
    CCM_MATRIX_S        astCcmMid[CCM_CT_NODE_NUM];    /* RW, 中增益的CCM矩阵 */
    CCM_MATRIX_S        astCcmHigh[CCM_CT_NODE_NUM];   /* RW, 高增益的CCM矩阵 */
    AISPD_CCM_RGB_CFG_S astOffsetRgb[AISP_MAX_DB_NUM]; /* RW, CCM模块RGB偏移量 AISP_MAX_DB_NUM */
} AISPD_CCM_CFG_S;
/********************** DN ***********************************/
typedef struct tagAispdDnCfg {
    NI_U32 u32GainThrH;     /* RW, 日夜切换增益阈值 */
    NI_U32 u32GainThrL;     /* RW, 夜日切换增益阈值 */
    NI_U16 u16GainRThr;     /* RW, 单块Gr阈值 小于该值计入红外强度计算 */
    NI_U16 u16GainBThr;     /* RW, 单块Gr阈值 小于该值计入红外强度计算 */
    NI_U16 u16GainRAvgThr;  /* RW, 整图Gr均值阈值 红外强度计算阈值 大于该值进行切换计数增加 */
    NI_U16 u16GainBAvgThr;  /* RW, 整图Gr均值阈值 红外强度计算阈值 大于该值进行切换计数增加 */
    NI_U16 u16LightPerCent; /* RW, 红外强度占比 0-1000 红外强度小于该值切换计数增加 */
    NI_U16 u16LedDiffR;     /* RW, LED色温判断模式下diff r [0, 256] */
    NI_U16 u16LedDiffB;     /* RW, LED色温判断模式下diff b [0, 256] */
    NI_U16 u16LedDiffEnvH;  /* RW, LED亮度环境差异高阈值 [1000, 2000] */
    NI_U16 u16LedDiffEnvL;  /* RW, LED亮度环境差异低阈值 [0, 1000] */
} AISPD_DN_CFG_S;
/********************** EXP ***********************************/
typedef struct tagAispdSenExpCfg {
    NI_U32 au32ExpRatio[AISP_DOW_MAX_FRAME_NUM]; /*RW, 用于计算宽动态时的曝光比 */
} AISPD_SEN_EXP_CFG_S;
/********************** IR_LIGHT ***********************************/
typedef struct tagAispdIrLightCfg {
    NI_U16 u16TargetIllum; /* RW, 目标环境亮度  */
    NI_U8  u8Tolerance;    /* RW, 容忍误差 */
    NI_U8  u8Speed;        /* RW, 调整速度 */
    NI_U16 u16LightMin;    /* RW, 红外灯强度最小值 */
    NI_U16 u16LightMax;    /* RW, 红外灯强度最大值 */
} AISPD_IR_LIGHT_CFG_S;
/********************** LED_LIGHT ***********************************/
typedef struct tagAispdLedLightCfg {
    NI_U16 u16TargetIllum; /* RW, 目标环境亮度  */
    NI_U8  u8Tolerance;    /* RW, 容忍误差 */
    NI_U8  u8Speed;        /* RW, 调整速度 */
    NI_U16 u16LightMin;    /* RW, led灯强度最小值 */
    NI_U16 u16LightMax;    /* RW, led灯强度最大值 */
} AISPD_LED_LIGHT_CFG_S;
/********************** RGB2YUV ***********************************/

typedef struct tagRgb2YuvMatrixDb {
    NI_F32 af32Cof[3][3];              /* RW, RGB2YUV转换矩阵 */
    NI_S32 s32OfstY[AISP_MAX_DB_NUM];  /* RW, Y Offset AISP_MAX_DB_NUM */
    NI_S32 s32OfstCb[AISP_MAX_DB_NUM]; /* RW, Cb Offset AISP_MAX_DB_NUM */
    NI_S32 s32OfstCr[AISP_MAX_DB_NUM]; /* RW, Cr Offset AISP_MAX_DB_NUM */
} RGB2YUV_MATRIX_DB_S;

typedef struct tagAispdRgb2YuvCfg {
    RGB2YUV_MATRIX_DB_S stDefMatrix;                        /*RW, RGB2YUV矩阵 */
    RGB2YUV_COLOR_S     astColor[AISP_MAX_DB_NUM];          /*RW, RGB2YUV增益调整参数  AISP_MAX_DB_NUM */
    AISP_APP_COLOR_S    astColorStyle[AISP_IMG_STYLE_BUTT]; /*RW, 风格增益调整参数 */
} AISPD_RGB2YUV_CFG_S;
/********************** WB ***********************************/
typedef struct tagAispdWbGain {
    NI_U32 u32GainR;  /*RW, R像素增益系数; u.4.8,rang[0, 4095], default:0x100 */
    NI_U32 u32GainGr; /*RW, Gr像素增益系数;u.4.8,rang[0, 4095], default:0x100 */
    NI_U32 u32GainGb; /*RW, Gb像素增益系数;u.4.8,rang[0, 4095], default:0x100 */
    NI_U32 u32GainB;  /*RW, B像素增益系数;u.4.8,rang[0, 4095], default:0x100 */
} AISPD_WB_GAIN_S;

typedef struct tagAispdWbRatio {
    NI_U8 u8RGainRatio; /*RW, WB系数, 在AWB结果的基础上乘以该系数,  128表示1x*/
    NI_U8 u8BGainRatio; /*RW, WB系数, 在AWB结果的基础上乘以该系数,  128表示1x*/
} AISPD_WB_RATIO_S;

typedef struct tagAispdWbCfg {
    AISPD_WB_RATIO_S astWbRatio[AISP_MAX_DB_NUM]; /*RW, WB系数, 在AWB结果的基础上乘以该系数, 128表示1x, 根据增益进行计算 AISP_MAX_DB_NUM */
} AISPD_WB_CFG_S;
/********************** AWES ***********************************/
/********************** BLC ***********************************/
/********************** CCM ***********************************/
/********************** CFA ***********************************/
typedef struct tagAispdCfaCfg {
    ISP_CFA_PARAM_S astCfaParam[AISP_MAX_DB_NUM]; /*RW, CFA模块参数 具体要求参见PISP相关说明, 根据增益进行计算, AISP_MAX_DB_NUM*/
} AISPD_CFA_CFG_S;
/********************** SCC ***********************************/
typedef struct tagAispdSccCfg {
    ISP_SCC_PARAM_S astSccParam[AISP_MAX_DB_NUM]; /*RW, SCC模块参数 具体要求参见PISP相关说明, 根据增益进行计算, AISP_MAX_DB_NUM*/
} AISPD_SCC_CFG_S;
/********************** PCR ***********************************/
typedef struct tagAispdPcrCfg {
    ISP_PCR_PARAM_S astPcrParam[AISP_MAX_DB_NUM]; /*RW, CNR模块参数 具体要求参见PISP相关说明, 根据增益进行计算, AISP_MAX_DB_NUM*/
} AISPD_PCR_CFG_S;
/********************** CNR ***********************************/
typedef struct tagAispdCnrCfg {
    ISP_CNR_PARAM_S astCnrParam[AISP_MAX_DB_NUM]; /*RW, CNR模块参数 具体要求参见PISP相关说明, 根据增益进行计算, AISP_MAX_DB_NUM*/
} AISPD_CNR_CFG_S;
/********************** CTRL ***********************************/
/********************** DOW ***********************************/
typedef struct tagAispdDowCfg {
    ISP_DOW_PARAM_S astDowParam[AISP_MAX_DB_NUM]; /*RW, DOW模块参数 具体要求参见PISP相关说明, 根据增益进行计算, AISP_MAX_DB_NUM*/
} AISPD_DOW_CFG_S;
/********************** DPC ***********************************/
typedef struct tagAispdDpcCfg {
    ISP_DPC_PARAM_S astDpcParam[AISP_MAX_DB_NUM]; /*RW, DPC参数, 具体要求参见PISP相关说明, 根据增益进行计算, AISP_MAX_DB_NUM*/
} AISPD_DPC_CFG_S;
/********************** DRCD ***********************************/
typedef struct tagAispdDrcdCfg {
    ISP_DRCD_PARAM_S astDrcdParam[AISP_MAX_DB_NUM]; /*RW, DRCD模块参数 具体要求参见PISP相关说明, AISP_MAX_DB_NUM*/
} AISPD_DRCD_CFG_S;
/********************** EE ***********************************/
typedef struct tagAispdEeCfg {
    ISP_EE_PARAM_S astEeParam[AISP_MAX_DB_NUM]; /*RW, EE参数, 具体要求参见PISP相关说明, 根据增益进行计算, AISP_MAX_DB_NUM*/
} AISPD_EE_CFG_S;
/********************** CE ***********************************/
typedef struct tagAispdCeCfg {
    ISP_CE_PARAM_S astCeParam[AISP_MAX_DB_NUM]; /*RW, CE参数, 具体要求参见PISP相关说明, 根据增益进行计算, AISP_MAX_DB_NUM*/
} AISPD_CE_CFG_S;
/********************** GAMMA ***********************************/
#define AISPD_GAMMA_WT_CURVE_NODE_NUM 5

typedef enum tagAispdGammaIndex {
    AISPD_GAMMA_TAB_DEF  = 0,
    AISPD_GAMMA_TAB_BRT  = 1,
    AISPD_GAMMA_TAB_DARK = 2,
    AISPD_GAMMA_TAB_CTR  = 3,
    AISPD_GAMMA_TAB_NUM,
} AISPD_GAMMA_INDEX_E;

typedef struct taAispdGammaWtCurveU16 {
    NI_U16 au16Thr[AISPD_GAMMA_WT_CURVE_NODE_NUM];
    NI_U16 au16Ratio[AISPD_GAMMA_WT_CURVE_NODE_NUM];
} AISPD_GAMMA_WT_CURVE_U16_S;

typedef struct tagAispdGammaCfg {
    NI_U8                      au8GammaTab[AISPD_GAMMA_TAB_NUM][ISP_GAMMA_MAX_TABLE_LEN]; /*RW, GAMMA表, 一共4组, 默认，亮表，暗表, ChartArray Range: [0, 255]*/
    NI_U8                      au8Strength[AISP_MAX_DB_NUM];                              /*RW, 各个增益下gamma强度, AISP_MAX_DB_NUM*/
    NI_U32                     au32GammaAlpha[AISP_MAX_DB_NUM];                           /*RW, 原始数据和查表数据之间的融合，值0为查表值，越大越偏向于原始值，16为原始值, u.1.4 rang[0,16], AISP_MAX_DB_NUM*/
    AISPD_GAMMA_WT_CURVE_U16_S stCtrCurve;                                                /*RW, ctr阈值gamma强度参数*/
    NI_U8                      au8Level[AISP_IMG_STYLE_BUTT];                             /*RW, 风格亮度等级*/
} AISPD_GAMMA_CFG_S;
/********************** GIC ***********************************/
typedef struct tagAispdGicCfg {
    ISP_GIC_PARAM_S astGicParam[AISP_MAX_DB_NUM]; /*RW, GIC参数, 具体要求参见PISP相关说明, 根据增益进行计算, AISP_MAX_DB_NUM*/
} AISPD_GIC_CFG_S;
/********************** LSC ***********************************/
#define AISP_LSC_TAB_LEN ISP_LSC_MAX_TABLE_LEN

typedef struct tagAispdLscCfg {
    NI_BOOL bLscEn;                        /*RW, 是否使能镜头阴影校正 */
    POINT_S stLscPosR;                     /*RW, r分量中心坐标 U.12.0*/
    POINT_S stLscPosG;                     /*RW, g分量中心坐标 U.12.0*/
    POINT_S stLscPosB;                     /*RW, b分量中心坐标 U.12.0*/
    NI_U16  au16LscTabR[AISP_LSC_TAB_LEN]; /*RW, LSC模块R分量表 ChartArray Range:[0,4095]*/
    NI_U16  au16LscTabG[AISP_LSC_TAB_LEN]; /*RW, LSC模块G分量表 ChartArray Range:[0,4095]*/
    NI_U16  au16LscTabB[AISP_LSC_TAB_LEN]; /*RW, LSC模块B分量表 ChartArray Range:[0,4095]*/
} AISPD_LSC_CFG_S;
/********************** LWDR ***********************************/
typedef struct tagAispdLwdrParam {
    NI_BOOL bLwdrEn;      /*RW, 宽动态开关 */
    NI_BOOL bFusEn;       /*RW, 亮度融合使能 */
    NI_U8   u8GLuma;      /*RW, 全局亮度调节参数;u.1.4,rang[0,16], default:0xa */
    NI_U8   u8GContrast;  /*RW, 全局对比度调节参数;u.1.4,rang[0,16], default:0xa */
    NI_U8   u8NoiseLevel; /*RW, 噪声水平调节参数;u.5.3, rang[0,255], default:0x10 */
    NI_U8   u8Alpha;      /*RW, 输出亮度融合，u.1.4，值越大lwdr效果越弱，rang[0,16];default:0x0 */
    NI_U8   u8FusDelta;   /*RW, 亮度融合系数,u.0.8,值越大亮度抑制程度越大; default:0x40 */
    NI_U8   u8FusThr;     /*RW, 亮度融合阈值,小于该值进行亮度抑制,u.8.0, default:0xff */

    NI_U8 u8LegDsFltAlpha; /*RW, leg滤波混合强度,u.1.4,rang[0,16], default:0x8 */
    NI_U8 u8LumDsFltSize;  /*RW, leg滤波尺寸,值越大,尺寸越大; rang[0,2], default:0x0*/
    NI_U8 u8LumDsFltAlpha; /*RW, lum滤波混合强度，u.1.4，rang[0,16], default:0x8 */
    NI_U8 au8GsCoef[5];    /*RW, LWDR Gaus滤波系数，u.0.8，coef0 + (coef1+ coef2+ coef3+ coef4)*2 = 256 */

} AISPD_LWDR_PARAM_S;

typedef struct tagAispdLwdrCfg {
    NI_U8              au8FiStr[AISP_MAX_DB_NUM];                 /*RW, LWDR FI表强度, 根据增益进行计算, AISP_MAX_DB_NUM*/
    NI_U8              au8FiTbl[3][ISP_LWDR_FI_MAX_TABLE_LEN];    /*RW, LWDR FI表, 具体要求参见PISP相关说明, 根据增益进行计算, ChartArray Range: [0, 255]*/
    NI_U8              au8AlphaTbl[ISP_LWDR_ALPHA_MAX_TABLE_LEN]; /*RW, LWDR ALPHA表, 具体要求参见PISP相关说明, 根据增益进行计算, ChartArray Range: [0, 255]*/
    AISPD_LWDR_PARAM_S astLwdr[AISP_MAX_DB_NUM];                  /*RW, LWDR模块参数, 根据增益进行计算, AISP_MAX_DB_NUM*/
} AISPD_LWDR_CFG_S;
/********************** NRY2D ***********************************/
typedef struct tagAispdNr2dCfg {
    ISP_NRY2D_PARAM_S astNry2dParam[AISP_MAX_DB_NUM]; /*RW, NRY2D模块参数 具体要求参见PISP相关说明, 根据增益进行计算, AISP_MAX_DB_NUM*/
} AISPD_NRY2D_CFG_S;
/********************** CSCCDS ***********************************/
typedef struct tagAispdCscCdsCfg {
    NI_U32 u32ModuleSel; /* RW, YUV444-YUV422转换模式选择 Range: [0, 1] 0：  直接取点 1:byapss，仅输出v通道数据*/
} AISPD_CSC_CDS_CFG_S;
/********************** WB ***********************************/
/********************** YHIST ***********************************/
/********************** YHIST_CONTRAST ***********************************/
typedef struct tagAispdYhistContrastCfg {
    NI_U8             u8Enable;        /*RW, 0关闭 1手动模式 2自动模式*/
    NI_U8             au8Table[256];   /*RW, 直方图统计映射表, ChartArray Range: [0, 255]*/
    YHIST_CTR_PARAM_S stYhistCtrParam; /*RW, 除雾*/
} AISPD_YHIST_CONTRAST_CFG_S;
/********************** DECOMP ***********************************/
typedef struct tagAispdDecompCfg {
    ISP_DEC_PARAM_S stDecompParam; /*RW, DECOMP模块参数 具体要求参见PISP相关说明*/
} AISPD_DECOMP_CFG_S;
/********************** NRYFULL3D ***********************************/
typedef struct tagAispNry3dCurv {
    NI_U8 u8Thr;
    NI_U8 u8ThrOffset;
    NI_U8 u8Ratio1;
    NI_U8 u8Ratio2;
    NI_U8 u8Ratio3;
} AISP_NRY3D_CURV_S;

typedef struct tagAispdNrFull3dCfg {
    ISP_NRYFULL3D_PARAM_S astNryFull3dParam[AISP_MAX_DB_NUM]; /*RW, NRYFULL3D模块参数 具体要求参见PISP相关说明, 根据增益进行计算, AISP_MAX_DB_NUM*/
} AISPD_NRYFULL3D_CFG_S;
/********************** NRYLC3D ***********************************/
typedef struct tagAispdNryLc3dCfg {
    ISP_NRYLC3D_PARAM_S astNryLc3dParam[AISP_MAX_DB_NUM]; /*RW, NRY3D模块参数 具体要求参见PISP相关说明, 根据增益进行计算, AISP_MAX_DB_NUM*/
} AISPD_NRYLC3D_CFG_S;
/********************** RGBGIC ***********************************/
typedef struct tagAispdRgbGicCfg {
    ISP_RGB_GIC_PARAM_S astRgbGicParam[AISP_MAX_DB_NUM]; /*RW, CFA模块参数 具体要求参见PISP相关说明, 根据增益进行计算, AISP_MAX_DB_NUM*/
} AISPD_RGBGIC_CFG_S;
/********************** RGBDPC ***********************************/
typedef struct tagAispdRgbDpcCfg {
    ISP_RGB_DPC_PARAM_S astRgbDpcParam[AISP_MAX_DB_NUM]; /*RW, CFA模块参数 具体要求参见PISP相关说明, 根据增益进行计算, AISP_MAX_DB_NUM*/
} AISPD_RGBDPC_CFG_S;
/********************** SCE ***********************************/
typedef struct tagAispSceCfg {
    ISP_SCE_PARAM_S astSpecDetailEnhParam[AISP_MAX_DB_NUM]; /*RW, SPECIAL DETAIL ENHANCE模块参数 具体要求参见PISP相关说明, 根据增益进行计算, AISP_MAX_DB_NUM*/
} AISPD_SCE_CFG_S;
/********************** YGAMMA ***********************************/
typedef enum tagAispdYGammaIndex {
    AISPD_YGAMMA_TAB_DEF  = 0,
    AISPD_YGAMMA_TAB_BRT  = 1,
    AISPD_YGAMMA_TAB_DARK = 2,
    AISPD_YGAMMA_TAB_CTR  = 3,
    AISPD_YGAMMA_TAB_NUM,
} AISPD_YGAMMA_INDEX_E;

typedef struct tagAispdYGammaCfg {
    NI_BOOL abEnable[AISP_MAX_DB_NUM];                                   /*RW, 0关闭 1打开, AISP_MAX_DB_NUM*/
    NI_U32  au32Alpha[AISP_MAX_DB_NUM];                                  /*RW, 表融合系数，该系数越小Gamma提亮效果越大，rang[0,16], AISP_MAX_DB_NUM*/
    NI_U8   au8Str[AISP_MAX_DB_NUM];                                     /*RW, 各个增益下Ygamma强度, rang[0,255], AISP_MAX_DB_NUM*/
    NI_U8   au8YGammaTab[AISPD_YGAMMA_TAB_NUM][ISP_GAMMA_MAX_TABLE_LEN]; /*RW, GAMMA表, 一共4组, 默认，亮表，暗表, ChartArray Range:[0,255]*/
} AISPD_YGAMMA_CFG_S;
/********************** BACA ***********************************/
typedef struct tagAispdBacaCfg {
    ISP_BACA_PARAM_S astBacaParam[AISP_MAX_DB_NUM]; /*RW, Baca模块参数 具体要求参见PISP相关说明, 根据增益进行计算, AISP_MAX_DB_NUM*/
} AISPD_BACA_CFG_S;
/********************** CFG ***********************************/
// 配置分为几种类型
// common: 放置在页面前面开头位置，为各种模式下共有参数
// page：  表示每页都有的参数
// day：   只在白天配置中才有的参数
// night： 只在黑夜配置中才有的参数

/*//////////////////////////////////version common/page///////////////////////////////////////////*/
typedef struct tagAispdComCfg {
    AISPD_VERSION_COM_CFG_S stVersionComCfg;
    AISPD_LSC_CFG_S         stLscCfg;
    AISPD_DN_CFG_S          stDnCfg;
    AISPD_IR_LIGHT_CFG_S    stIrLightCfg;
    AISPD_LED_LIGHT_CFG_S   stLedLightCfg;
    AISPD_SEN_EXP_CFG_S     stSenExpCfg;
} AISPD_COM_CFG_S;

typedef struct tagAispdDayCfg {
    AISPD_VERSION_PAGE_CFG_S   stVersionPageCfg;
    AISPD_AE_CFG_S             stAeCfg;
    AISPD_AWB_CFG_S            stAwbCfg;
    AISPD_BLC_CFG_S            stBlcCfg;
    AISPD_DPC_CFG_S            stDpcCfg;
    AISPD_GIC_CFG_S            stGicCfg;
    AISPD_WB_CFG_S             stWbCfg;
    AISPD_DOW_CFG_S            stDowCfg;
    AISPD_DRCD_CFG_S           stDrcdCfg;
    AISPD_LWDR_CFG_S           stLwdrCfg;
    AISPD_CFA_CFG_S            stCfaCfg;
    AISPD_RGBDPC_CFG_S         stRgbDpcCfg;
    AISPD_RGBGIC_CFG_S         stRgbGicCfg;
    AISPD_CCM_CFG_S            stCcmCfg;
    AISPD_GAMMA_CFG_S          stGammaCfg;
    AISPD_CSC_CDS_CFG_S        stCscCdsCfg;
    AISPD_RGB2YUV_CFG_S        stRgb2YuvCfg;
    AISPD_SCC_CFG_S            stSccCfg;
    AISPD_CE_CFG_S             stCeCfg;
    AISPD_NRYLC3D_CFG_S        stNryLc3dCfg;
    AISPD_NRYFULL3D_CFG_S      stNryFull3dCfg;
    AISPD_CNR_CFG_S            stCnrCfg;
    AISPD_PCR_CFG_S            stPcrCfg;
    AISPD_NRY2D_CFG_S          stNry2dCfg;
    AISPD_EE_CFG_S             stEeCfg;
    AISPD_YHIST_CONTRAST_CFG_S stYhistContrastCfg;
    AISPD_DECOMP_CFG_S         stDecompCfg;
    AISPD_SCE_CFG_S            stSceCfg;
    AISPD_YGAMMA_CFG_S         stYGammaCfg;
    AISPD_BACA_CFG_S           stBacaCfg;

} AISPD_DAY_CFG_S;

typedef struct tagAispdNightCfg {
    AISPD_VERSION_PAGE_CFG_S stVersionPageCfg;
    AISPD_AE_CFG_S           stAeCfg;
    // AISPD_AWB_CFG_S                  stAwbCfg;
    AISPD_BLC_CFG_S stBlcCfg;
    AISPD_DPC_CFG_S stDpcCfg;
    AISPD_GIC_CFG_S stGicCfg;
    // AISPD_WB_CFG_S                   stWbCfg;
    AISPD_DOW_CFG_S    stDowCfg;
    AISPD_DRCD_CFG_S   stDrcdCfg;
    AISPD_LWDR_CFG_S   stLwdrCfg;
    AISPD_CFA_CFG_S    stCfaCfg;
    AISPD_RGBDPC_CFG_S stRgbDpcCfg;
    AISPD_RGBGIC_CFG_S stRgbGicCfg;
    // AISPD_CCM_CFG_S                  stCcmCfg;
    AISPD_GAMMA_CFG_S          stGammaCfg;
    AISPD_CSC_CDS_CFG_S        stCscCdsCfg;
    AISPD_RGB2YUV_CFG_S        stRgb2YuvCfg;
    AISPD_SCC_CFG_S            stSccCfg;
    AISPD_CE_CFG_S             stCeCfg;
    AISPD_NRYLC3D_CFG_S        stNryLc3dCfg;
    AISPD_NRYFULL3D_CFG_S      stNryFull3dCfg;
    AISPD_CNR_CFG_S            stCnrCfg;
    AISPD_PCR_CFG_S            stPcrCfg;
    AISPD_NRY2D_CFG_S          stNry2dCfg;
    AISPD_EE_CFG_S             stEeCfg;
    AISPD_YHIST_CONTRAST_CFG_S stYhistContrastCfg;
    AISPD_DECOMP_CFG_S         stDecompCfg;
    AISPD_SCE_CFG_S            stSceCfg;
    AISPD_YGAMMA_CFG_S         stYGammaCfg;
    AISPD_BACA_CFG_S           stBacaCfg;

} AISPD_NIGHT_CFG_S;

typedef struct tagAispdVideoCfg {
    AISPD_DAY_CFG_S   stDayCfg;
    AISPD_NIGHT_CFG_S stNightCfg;
} AISPD_VIDEO_CFG_S;

typedef struct tagAispdRamCfg {
    AISPD_COM_CFG_S   stComCfg;
    AISPD_VIDEO_CFG_S stVideoCfg;
} AISPD_RAM_CFG_S;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
