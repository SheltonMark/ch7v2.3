/*******************************************************************************
 * Copyright (C) 2020-2025 ZheJiang Chipup Technology CO.,LTD.
 *
 * 文件名 : ni_comm_aispd.h
 *
 * 作  者 : Chipup Software Team
 * 版  本 : V1.0.0
 * 日  期 : 2022-02-23 Create
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
 * 日    期: 2022-02-23
 * 修订版本:
 * 作    者:
 * 修订备注:
 *******************************************************************************/
#ifndef _NI_COMM_AISPD_ONLINE_H_
#define _NI_COMM_AISPD_ONLINE_H_


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "ni_define.h"
#include "ni_comm_pisp.h"
#include "ni_aisp_define.h"
#include "ni_comm_aispalg.h"
#include "ni_comm_aisp.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */


/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */


/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */
typedef struct tagAispdAwesParam
{   
    NI_U16                   u16ImgW;           /* RW, 输入图像宽度 请根据实际使用的分辨率进行设置 不需要改动*/
    NI_U16                   u16ImgH;           /* RW, 输入图像高度 请根据实际使用的分辨率进行设置 不需要改动*/
    PISP_AWES_PARAM_S         stAwesParam;      /* RW, PISP中AWES参数设置,具体要求参见PISP相关说明 */
}AISPD_AWES_PARAM_S;

typedef struct tagAispdAwesCfg
{    
    AISPD_AWES_PARAM_S       astAwesParam[AWES_FMT_MAX_NUM];  /*RW, AWES统计参数设置, 根据图像宽高选择具体使用哪一个索引下的参数 */
}AISPD_AWES_ATTR_S;

typedef struct tagAispdAeAlgParam
{
    AE_ATTR_S          stAeAttr;                        /*RW, AE算法属性参数 */
    AE_ROUTE_S         stRoute;                         /*RW, AE算法曝光路径参数 */
    AE_WIN_WT_S        stWinWt;                         /*RW, AE算法区域权重参数 */
}AISPD_AE_ALG_PARAM_S;

typedef struct tagAispdAeParam
{   
    NI_U16                   u16ImgWidth;               /*RW, 输入图像宽度 请根据实际使用的分辨率进行设置 不需要改动*/
    NI_U16                   u16ImgHeight;              /*RW, 输入图像高度 请根据实际使用的分辨率进行设置 不需要改动*/
    NI_U16                   u16ImgFps;                 /*RW, 输入图像帧率 请根据实际使用的帧率进行设置 不需要改动*/
    AISPD_AE_ALG_PARAM_S     stAeAlg;                   /*RW, AE算法参数 */
}AISPD_AE_PARAM_S;


typedef struct tagAispdAeCfg
{    
    AISPD_AE_PARAM_S       astAeParam[AE_FMT_MAX_NUM];  /*RW, AE模块参数 根据图像宽高帧率选择具体使用哪一个索引下的参数*/
}AISPD_AE_ATTR_S;


typedef struct tagAispdAwbParam
{
    AWB_ATTR_S             stAlgAwb;                    /*RW, AWB算法参数 */
}AISPD_AWB_PARAM_S;

typedef struct tagAispdAwbCfg
{
    AISPD_AWB_PARAM_S       astAwbParam[AWB_SCENE_MAX_NUM]; /*RW, AWB模块参数 根据用户接口的白平衡场景(enSceneInfo)选择使用哪个索引下的参数 */
}AISPD_AWB_ATTR_S;

typedef struct tagAispdWbRatio
{
    NI_U16              u16RGainRatio;      /*RW, WB系数, 在AWB结果的基础上乘以该系数,  128表示1x*/
    NI_U16              u16BGainRatio;      /*RW, WB系数, 在AWB结果的基础上乘以该系数,  128表示1x*/
}AISPD_WB_RATIO_S;

typedef struct tagAispdWbCfg
{
    AISPD_WB_RATIO_S       astWbRatio[ISP_WB_MAX_ID_NUM][AISP_MAX_DB_NUM];  /*RW, WB系数, 在AWB结果的基础上乘以该系数, 128表示1x, 根据增益进行计算*/
}AISPD_WB_ATTR_S;

typedef struct tagAispdBlcCfg
{  
    PISP_BLACK_LEVEL_S      astBlcLevel[ISP_BLC_MAX_ID_NUM][AISP_MAX_DB_NUM];  /*RW, BLC参数, 具体要求参见PISP相关说明, 根据增益进行计算*/
}AISPD_BLC_ATTR_S;


typedef struct tagAispdDpcCfg
{    
    PISP_DPC_PARAM_S        astDpcParam[ISP_DPC_MAX_ID_NUM][AISP_MAX_DB_NUM]; /*RW, DPC参数, 具体要求参见PISP相关说明, 根据增益进行计算*/
}AISPD_DPC_ATTR_S;

typedef struct tagAispdLwdrParam
{
    NI_BOOL         bEnable;               /*RW, 宽动态开关 */
    NI_U8           u8WLuma;               /*RW, 全局亮度调节参数;u.1.4,rang[0,16], default:0xa */
    NI_U8           u8WContrast;           /*RW, 全局对比度调节参数;u.1.4,rang[0,16], default:0xa */
    NI_U16          u16WGain;         	    /*RW, 增强强度调节参数;u.5.3, rang[0,255], default:0x10 */

    NI_BOOL         bHlsEnable;            /*RW, 高亮抑制开关 */
    NI_U16          u16HlsDelta;           /*RW, 高亮抑制强度,值越大亮度抑制程度越大;位宽[7:0],取值范围[0,255], default:0x40 */
    NI_U16          u16HlsThrhd;           /*RW, 高亮抑制阈值,小于该值进行亮度抑制,位宽[7:0],取值范围[0,255], default:0xff */
}AISPD_LWDR_PARAM_S;


typedef struct tagAispdLwdrCfg
{
    NI_U16                      au16FiTbl[2][ISP_LWDR_MAX_FI_TABLE_LEN];             /*RW, LWDR FI表, 具体要求参见PISP相关说明, 根据增益进行计算*/
    NI_U16                      au16AlphaTbl[ISP_LWDR_MAX_ALPHA_TABLE_LEN];          /*RW, LWDR ALPHA表, 具体要求参见PISP相关说明, 根据增益进行计算*/
    AISPD_LWDR_PARAM_S          astLwdr[AISP_MAX_DB_NUM];                            /*RW, LWDR模块参数, 根据增益进行计算*/
}AISPD_LWDR_ATTR_S;

typedef struct tagAispdGicCfg
{   
    PISP_GIC_PARAM_S            astGicParam[ISP_GIC_MAX_ID_NUM][AISP_MAX_DB_NUM];   /*RW, GIC参数, 具体要求参见PISP相关说明, 根据增益进行计算*/
}AISPD_GIC_ATTR_S;

typedef struct tagAispdEeCfg
{   
    PISP_EE_PARAM_S             astEeParam[AISP_MAX_DB_NUM];                /*RW, EE参数, 具体要求参见PISP相关说明, 根据增益进行计算*/
}AISPD_EE_ATTR_S;

typedef struct tagAispdCcmCfg
{   
    CCM_PARAM_S             stCcmParam;         /*RW, CCM算法参数*/
}AISPD_CCM_ATTR_S;

typedef struct tagAispdCdsCfg
{
    NI_BOOL                 bAntiOverExposure;  /*RW, 过曝色度抑制开关  default:0x0 */
    NI_U8                   au8RestrainTab[ISP_CSCCDS_MAX_TABLE_LEN];     /*RW, 色彩抑制曲线表 */
}AISPD_CDS_ATTR_S;

typedef struct tagAispdCscCfg
{
    RGB2YUV_PARAM_S      stCscParam;                        /*RW, RGB2YUV算法参数 */
    AISPD_CDS_ATTR_S     astCdsParam[AISP_MAX_DB_NUM];      /*RW, CSC模块参数算法参数, 根据增益进行计算*/
    NI_U32               u32BwMode;          /*RW, 黑白模式,rang[0,1], 0：彩色模式；1：黑白模式, default:0x0 */
    NI_U32               u32YuvModeSel;      /*RW, YUV444-YUV422转换模式选择, rang[0,3], 0:表1：2：1滤波; 1:表1：1滤波; 2:表直接取点; 3:表byapss, default:0x2 */ 
}AISPD_CSC_CDS_ATTR_S;

typedef struct tagAispdLscParam
{
    NI_BOOL                 bEnable;            /*RW, 是否使能镜头阴影校正 */
    POINT_S                 stCenterR;          /*RW, r分量中心坐标 U.12.0*/
    POINT_S                 stCenterG;          /*RW, g分量中心坐标 U.12.0*/
    POINT_S                 stCenterB;          /*RW, b分量中心坐标 U.12.0*/
}AISPD_LSC_PARAM_S;

typedef struct tagAispdLscCfg
{
    AISPD_LSC_PARAM_S        astLscParam[AISP_MAX_DB_NUM];          /*RW, LSC模块参数*/
    NI_U16                   au16LscTabR[ISP_LSC_MAX_TABLE_LEN];    /*RW, LSC模块R分量表*/
    NI_U16                   au16LscTabG[ISP_LSC_MAX_TABLE_LEN];    /*RW, LSC模块G分量表*/
    NI_U16                   au16LscTabB[ISP_LSC_MAX_TABLE_LEN];    /*RW, LSC模块B分量表*/
}AISPD_LSC_ATTR_S;

typedef struct tagAispdCfaCfg
{   
    PISP_CFA_PARAM_S        astCfaParam[AISP_MAX_DB_NUM];       /*RW, CFA模块参数 具体要求参见PISP相关说明, 根据增益进行计算*/
}AISPD_CFA_ATTR_S;

typedef struct tagAispdDowCfg
{
    PISP_DOW_PARAM_S        astDowParam[AISP_MAX_DB_NUM];   /*RW, DOW模块参数 具体要求参见PISP相关说明, 根据增益进行计算*/
}AISPD_DOW_ATTR_S;

typedef struct tagAispdCnrCfg
{
    PISP_CNR_PARAM_S       astCnrParam[AISP_MAX_DB_NUM];    /*RW, CNR模块参数 具体要求参见PISP相关说明, 根据增益进行计算*/
} AISPD_CNR_ATTR_S;

typedef enum tagAispdGammaIndex
{
    AISPD_GAMMA_TAB_DEF = 0,
    AISPD_GAMMA_TAB_BRT = 1,
    AISPD_GAMMA_TAB_DARK = 2,
    AISPD_GAMMA_TAB_CTR = 3,
    AISPD_GAMMA_TAB_NUM,
} AISPD_GAMMA_INDEX_E;

typedef struct taAdaptWtCurveU16
{
    NI_U16  au16Thr[AISP_WT_CURVE_NODE_NUM];
    NI_U16  au16Ratio[AISP_WT_CURVE_NODE_NUM];
} AISPD_WT_CURVE_U16_S;

typedef struct tagAlgGammaParam
{  
    NI_U16                  au16GammaTab[AISPD_GAMMA_TAB_NUM][ISP_GAMMA_MAX_TABLE_LEN]; /*RW, GAMMA表, 一共4组, 默认，亮表，暗表, */
    NI_U8                   au8Strength[AISP_MAX_DB_NUM];    /*RW, 各个增益下gamma强度 */
    AISPD_WT_CURVE_U16_S    stCtrCurve;                      /*RW, ctr阈值gamma强度参数 */
}AISPD_GAMMA_PARAM_S;

typedef struct tagAispdGammaCfg
{
    AISPD_GAMMA_PARAM_S      stGammaParam;      /*RW, gamma模块参数 */
}AISPD_GAMMA_ATTR_S;

typedef struct tagAispdDrcdCfg
{
    PISP_DRCD_PARAM_S   stDrcdParam;            /*RW, DRCD模块参数 具体要求参见PISP相关说明*/
} AISPD_DRCD_ATTR_S;

typedef struct tagAispdNr3dCfg
{
    PISP_NRY3D_PARAM_S      astNry3dParam[AISP_MAX_DB_NUM]; /*RW, DRCD模块参数 具体要求参见PISP相关说明, 根据增益进行计算*/
} AISPD_NRY3D_ATTR_S;

typedef struct tagAispdYhistContrastCfg
{
    NI_BOOL                    bEnable;                /*RW, 0关闭 1打开*/
    NI_U16                     au16Table[256];         /*RW, 直方图统计映射表*/
}AISPD_YHIST_CONTRAST_ATTR_S;

typedef struct tagAispdStyleParam
{
    NI_U32              u32EvLevel;              /*RW, 亮度调整系数 */
    NI_U8               u8GammaLevel;            /*RW, gamma等级调整系数 */
    NI_U8               u8Nr2dLevel;             /*RW, 2D降噪等级调整系数 */
    NI_U8               u8Nr3dLevel;             /*RW, 3D降噪等级调整系数 */
    NI_U8               u8EeLevel;               /*RW, 锐化等级调整系数 */
    RGB2YUV_COLOR_S     stColor;                 /*RW, rgb2yuv调整系数 */
}AISPD_STYLE_PARAM_S;

typedef struct tagAispdStyleCfg
{    
    AISPD_STYLE_PARAM_S            astStyleParam[AISP_IMG_STYLE_BUTT]; /*RW, 图像风格粗调参数 */
}AISPD_STYLE_ATTR_S;


typedef struct tagAispdSenExpParam
{    
    NI_U32           au32ExpRatio[AISP_DOW_MAX_FRAME_NUM];  /*RW, 用于计算宽动态时的曝光比 */
}AISPD_SEN_EXP_PARAM_S;


typedef struct tagAispdSenExpCfg
{    
    AISPD_SEN_EXP_PARAM_S         stSenExpParam;    /*RW, sensor曝光参数 */
}AISPD_SEN_EXP_ATTR_S;

typedef struct tagAispdDnCfg
{
    DN_METHOD_E          enMethod;      /* RW, 日夜算法方式 */

    NI_U32               u32D2nGainThr;  /* RW, 日夜切换增益阈值 */
    NI_U32               u32N2dGainThr;  /* RW, 夜日切换增益阈值 */
    NI_U32               u32EnvLumThr;   /* RW, 环境亮度阈值 */

    NI_U16               u16GainRThr;   /* RW, 单块Gr阈值 小于该值计入红外强度计算 */
    NI_U16               u16GainBThr;   /* RW, 单块Gr阈值 小于该值计入红外强度计算 */

    NI_U16               u16GainRAvgThr; /* RW, 整图Gr均值阈值 红外强度计算阈值 大于该值进行切换计数增加 */
    NI_U16               u16GainBAvgThr; /* RW, 整图Gr均值阈值 红外强度计算阈值 大于该值进行切换计数增加 */
    NI_U16               u16IrPerCent;   /* RW, 红外强度占比 0-1000 红外强度小于该值切换计数增加 */
    NI_U32               u32Rev[8];      /* RW, 无效 */
}AISPD_DN_ATTR_S;

typedef struct tagAispdIrLightCfg
{
    NI_U16                          u16TargetIllum;     /* RW, 无效 目标环境亮度  */
    NI_U8                           u8Tolerance;        /* RW, 无效 容忍误差 */
    NI_U8                           u8Speed;            /* RW, 无效 调整速度 */
    NI_U16                          u16IrLightMin;      /* RW, 无效 红外灯强度最小值 */
    NI_U16                          u16IrLightMax;      /* RW, 无效 红外灯强度最大值 */
    NI_U8                           u8IrLightNum;       /* RW, 无效 红外灯个数 */
    NI_U8                           au8Reserved[3];     /* RW, 无效 */
    NI_U32                          u32Rev[10];         /* RW, 无效 */
}AISPD_IR_LIGHT_ATTR_S;

typedef struct tagAispdLedLightCfg
{
    NI_U16                          u16TargetIllum;       /* RW, 无效 目标环境亮度  */
    NI_U8                           u8Tolerance;          /* RW, 无效 容忍误差 */
    NI_U8                           u8Speed;              /* RW, 无效 调整速度 */
    NI_U16                          u16LedLightMin;       /* RW, 无效 红外灯强度最小值 */
    NI_U16                          u16LedLightMax;       /* RW, 无效 红外灯强度最大值 */
    NI_U8                           u8LedLightNum;        /* RW, 无效 红外灯个数 */
    NI_U8                           au8Reserved[3];       /* RW, 无效 */
    NI_U32                          u32Rev[10];           /* RW, 无效 */
}AISPD_LED_LIGHT_ATTR_S;


#if 0
typedef struct tagAispdVersionCfg
{
    NI_CHAR             aszCfgVersion[AISP_VERSION_NAME_LEN];
}AISPD_VERSION_ATTR_S;
#else
typedef struct tagAispdVersionCfg
{
    NI_U32              u32CrcCheck;           /*RW, 4 CRC校验 工具导出时更新 该部分值 */
    NI_CHAR             aszAispVersion[24];    /*RO, 28 AISP版本 通常不要改动  用于配置文件和2a库的版本校验*/  //
    NI_CHAR             aszCfgType[12];        /*RO, 40 配置类型 LINE_COLOR WDR_CLOLR LINE_BLACK WDR_BLACK 通常不要改动*/
    NI_CHAR             aszSensorType[8];      /*RO, 48 sensor 类型 */
    NI_CHAR             aszProductType[8];     /*RO, 56 产品类型 如BASE表示基线版本, 根据客户定制的可以标识客户名 */
    NI_CHAR             aszProductVer[2];      /*RO, 58 产品类型 版本号 */
    NI_CHAR             aszGenTime[6];         /*RO, 64 生成时间 */
    NI_CHAR             aszRev[16];            /*RO, IPC 预留,  CVI 没有该字段*/
}AISPD_VERSION_ATTR_S;

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

