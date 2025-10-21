/*******************************************************************************
 * Copyright (C) 2020-2025 ZheJiang Chipup Technology CO.,LTD.
 *
 * 文件名 : ni_comm_aispalg.h
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



#ifndef _NI_COMM_ALG_H_
#define _NI_COMM_ALG_H_


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "ni_type.h"
#include "ni_common.h"
#include "ni_aisp_define.h"


#ifdef __cplusplus
extern "C"{
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
typedef struct tagAispMemInfo
{
    NI_U32                  u32Len;         
    NI_U32                  u32PhyAddr;
    NI_U32                  *pu32VirAddr;
}AISP_MEM_INFO_S;


typedef struct tagAispImgRegion
{   
    NI_U16                  u16StartX;  /**/
    NI_U16                  u16StartY;
    NI_U16                  u16EndX;
    NI_U16                  u16EndY;
} AISP_IMG_REGION_S;

typedef enum tagAispAwesPos
{
    AISP_AWES_POS_BEFORE_WB = 0,            
    AISP_AWES_POS_AFTER_COMP = 1,
    AISP_AWES_POS_EXP_0 = 2,
    AISP_AWES_POS_EXP_1 = 3,
    AISP_AWES_POS_EXP_2 = 4,
    AISP_AWES_POS_BUTT,
}AISP_AWES_POS_E;

typedef enum tagAispAwesMode
{
    AISP_AWES_MODE_FRAME_S                = 0,  /* 只选用短曝光帧给算法*/
    AISP_AWES_MODE_FRAME_L,                     /* 只选用长曝光帧给算法*/
    AISP_AWES_MODE_FRAME_COMPO,                 /* 只选用合成帧给算法*/ 
    AISP_AWES_MODE_FRAME_L_S,                   /* 选用长、短两帧统计数据给算法 */
    AISP_AWES_MODE_FRAME_L_COMPO,               /* 选用长、合成两帧统计数据给算法 */
    AISP_AWES_MODE_FRAME_S_COMPO,               /* 选用短、合成两帧统计数据给算法 */
    AISP_AWES_MODE_FRAME_L_S_COMPO,             /* 选用长、短、合成帧三帧统计数据给算法 */

    AISP_AWES_MODE_BUTT
}AISP_AWES_MODE_E;

typedef struct tagAispAwesInfo
{
    NI_BOOL                 bValid;             /* 有效标志 */
    NI_U32                  u32DataWidth;       /* 统计数据位宽 */
    NI_U32                  u32BlockNum;        /* AWES统计一帧的分块总数 */
    NI_U32                  u32HBlockNum;       /* AWES统计水平方向分块个数 */
    NI_U32                  u32VBlockNum;       /* AWES统计垂直方向分块个数 */
    NI_VOID                 *pvAwesData;         /* AWES统计的rgby值, 每帧统计数据=u32BlockNum*2*4bytes. */
    AISP_AWES_MODE_E        AwesMode;           /* AWES统计模式 */
}AISP_AWES_INFO_S;

typedef struct tagAispYhistInfo
{
    NI_BOOL                 bVaild;
    NI_U32                  u32YhistMin;        /*RO, 直方图统计最小值 */
    NI_U32                  u32YhistMax;        /*RO, 直方图统计最大值 */
    NI_U32                  u32YhistAvg;        /*RO, 直方图统计得到亮度平均值 */
    NI_U32                  *pu32YhistData;     /*RO, 直方图统计数据, 固定256深度 */
} AISP_YHIST_INFO_S;

typedef struct tagAispStaticsInfo
{
    AISP_AWES_INFO_S               astAwesInfo[AISP_AWES_POS_BUTT];
    AISP_YHIST_INFO_S              stYhistInfo;
}AISP_STATICS_INFO_S;

typedef struct tagAispGainAdjInfo
{
    NI_BOOL       bGainAdjFlag;    /*增益变化，模块需要调节标志*/
    NI_S32        s32Gain;         /*保存的gain*/
    NI_BOOL       bMaxFlag;        /*使用最大*/
    NI_S32        s32Level;        /*插值的小端*/
    NI_S32        s32Slope;        /*差值的中间系数*/
} AISP_GAIN_ADJ_INFO_S;

typedef struct tagAispAwbResult
{   
    NI_BOOL bValid;
    NI_U16  u16GainR;
    NI_U16  u16GainB;
    NI_U16  u16GainGr;
    NI_U16  u16GainGb;
    NI_U16  u16LowCtRatio;
    NI_U16  u16HigCtRatio;
    NI_U16  u16MulCt;
    NI_U16  u16Rev;
    NI_S32  s32Weight;
}AISP_AWB_RESULT_S;

typedef struct tagAispExpResult
{
    NI_BOOL     bExpSwitch;
    NI_S32      s32ShtTime;
    NI_S32      s32SysGain;
    NI_S32      s32ApertValue;
}AISP_EXP_RESULT_S;


typedef enum tagAispExpStatus
{
    AISP_EXP_STATUS_MIN = 0,
    AISP_EXP_STATUS_NORMAL,
    AISP_EXP_STATUS_MAX,
    AISP_EXP_STATUS_BUTT,
} AISP_EXP_STATUS_E;

typedef enum tagAispAdjStatus
{
    AISP_ADJ_STATUS_STABLE = 0,
    AISP_ADJ_STATUS_ADJ,
    AISP_ADJ_STATUS_BUTT,
} AISP_ADJ_STATUS_E;

typedef struct tagAispEvEstiMate
{
    NI_S32                    s32EvTarget;
    NI_S32                    s32EvCur;
    NI_S32                    s32AverYCur;
    NI_S32                    s32Tolerance;
    NI_S32                    s32EvDelta;
    NI_U16                    u16EvLumRatio;
    NI_U16                    u16Rev;
}AISP_EV_ESTIMATE_S;

typedef struct tagAispAeStatus
{
    AISP_EXP_STATUS_E           enExpStatus;
    AISP_ADJ_STATUS_E           enAdjStatus;
    AISP_EV_ESTIMATE_S          stEvEstimate;
}AISP_AE_STATUS_S;

typedef struct tagAispDrInfo
{
    NI_BOOL                 bValid;
    NI_U16                  u16DrInfo;          /*动态范围*/
    NI_U16                  u16Var;             /*方差*/
    NI_U16                  u16AvgDarkValue;    /*暗区平均亮度*/
    NI_U16                  u16AvgBrtValue;     /*亮区平均亮度*/
}AISP_DR_INFO_S;

typedef struct tagAispEnvLum
{
    NI_BOOL                 bValid;             /*参数是否有效*/
    NI_S32                  s32AverY;           /*平均亮度*/
    NI_S32                  s32EnvLum;          /*环境亮度*/
}AISP_EV_INFO_S;

typedef struct tagAlgAeResult
{
    NI_BOOL                 bValid;
    NI_U8                   u8Interval;
    NI_U8                   au8Rev[3];
    AISP_EXP_RESULT_S       stExpResult;
    AISP_AE_STATUS_S        stExpStatus;
}ALG_AE_RESULT_S;

typedef struct tagAeRouteNode
{
    NI_S32                  s32ShtTime;                        /*RW, 快门时间, 单位us, u32.0*/
    NI_S32                  s32SysGain;                        /*RW, 增益, 单位 Db*1000*/
    NI_S32                  s32ApePercent;                     /*RW, 光圈, 无效*/
} AE_ROUTE_NODE_S;

typedef struct tagAeRoute
{
    NI_U32                  u32TotalNum;                       /*RW, 曝光路线节点数*/
    AE_ROUTE_NODE_S         astRouteNode[AE_ROUTE_MAX_NODES];  /*RW, 曝光路线节点描述*/
} AE_ROUTE_S;

typedef struct tagAeWinWt
{
    NI_BOOL                 bEnable;                          /*RW, 使能窗口权重*/
    NI_U8                   au8WtH[AE_WIN_WT_MAX_NODES];      /*RW, 横向权重*/
    NI_U8                   au8WtV[AE_WIN_WT_MAX_NODES];      /*RW, 纵向权重*/
} AE_WIN_WT_S;

typedef struct tagAeEvTarget
{
    NI_U16                  u16Ev;                              /*RW, 目标ev */
    NI_U8                   au8EvRatio[AISP_MAX_DB_NUM];        /*RW, 目标ev 系数 U1.7 */
} AE_EV_TARGET_S;

typedef struct tagAeDelay
{
    NI_U16                  u16WhiteDelay;                    /*RW, 图像偏亮时延迟调节帧数*/
    NI_U16                  u16BlackDelay;                    /*RW, 图像偏暗时延迟调节帧数*/
    NI_U16                  u16Stable2AdjDelay;               /*RW, 图像从稳定到调整状态的延迟调节帧数*/
} AE_DELAY_S;

typedef struct tagAeBackLight
{
    NI_BOOL                 bEnable;                          /*RW, 背光补偿开关*/
    NI_U16                  u16HistRatioWt;                   /*RW, 需要补偿区域的权重 64表示X1*/
    NI_U16                  u16MaxLevel;                      /*RW, 背光补偿最大强度限制 64表示X1*/
} AE_BACK_LIGHT_S;

typedef struct tagAeAttr
{
    NI_U8                   u8AeInterval;                     /*RW, ae运行频率 单位帧*/
    NI_U16                  u16AeSpeed;                       /*RW, 调节速度 0~256, 256 速度最快 */
    NI_U16                  u16AeTolerance;                   /*RW, 曝光误差容忍度 */
    AE_EV_TARGET_S          stAeEvTarget;                     /*RW, 目标亮度*/
    AE_DELAY_S              stAeDelay;                        /*RW, 光线变化后delay n帧后开始调节 */
    AE_BACK_LIGHT_S         stAeBackLight;                    /*RW, 背光补偿*/
} AE_ATTR_S;

typedef struct tagAeRegionAttr
{   
    NI_BOOL                       bRegion;         /*RW, 区域曝光使能 */
    AISP_IMG_REGION_S             stRegion;        /*RW, 区域区域 */
} AE_REGION_ATTR_S;

typedef struct tagAwbExtInfo
{   
    NI_S32      s32EnvLum;
    NI_S32      s32DrInfo;
    NI_VOID     *pCalcBuf;
} AWB_EXT_INFO_S;

typedef struct tagAwbCTRange
{        
    NI_U16 u16TCMax;                /*RW, TC最大值 */
    NI_U16 u16TCMin;                /*RW, TC最大值 */
} AWB_CT_RANGE_S;

typedef struct tagAwbCurve
{        
    NI_U16 au16CurveUp[AWB_CURVE_BIN_NUM];      /*RW, 色温曲线的上限 */
    NI_U16 au16CurveDown[AWB_CURVE_BIN_NUM];    /*RW, 色温曲线的下线限 */
} AWB_CURVE_S;

typedef struct tagAwbGainLimit
{        
    NI_U16 u16RgainMin;         /*RW, 白块的RGain下限 */
    NI_U16 u16RgainMax;         /*RW, 白块的RGain上限 */
    NI_U16 u16BgainMin;         /*RW, 白块的BGain下限 */
    NI_U16 u16BgainMax;         /*RW, 白块的BGain上限 */
} AWB_GAIN_LIMIT_S;

typedef struct tagAwbLightInfo
{   
    NI_U16     u16GainR;        /*RW, 光源点的RGain */
    NI_U16     u16GainB;        /*RW, 光源点的BGain */
} AWB_LIGHT_INFO_S;

typedef struct tagAwbSlsWt
{   
    NI_U8  u8RatioMulCtH;       /*RW, 无效 */
    NI_U8  u8RatioMulCtL;       /*RW, 无效 */
    NI_U8  u8RaitoEnvLumH;      /*RW, 光源点的环境亮度权重系数Heigh */
    NI_U8  u8RaitoEnvLumL;      /*RW, 光源点的环境亮度权重系数Low */
    NI_U16 u16EnvLumH;          /*RW, 光源点的环境亮度阈值Heigh */
    NI_U16 u16EnvLumL;          /*RW, 光源点的环境亮度阈值Low */
} AWB_SPECIAL_LIGHT_WT_S;

typedef struct tagAwbSpecialLightInfo
{   
    AWB_GAIN_LIMIT_S            stGainLimt; /* RW, 光源点增益阈值 */
    AWB_SPECIAL_LIGHT_WT_S      stWt;       /* RW, 光源点权重系数参数 */
} AWB_SPECIAL_LIGHT_INFO_S;

typedef struct tagAwbSpecialLightt
{   
    NI_U8                       u8ValidNum;      /* RW, 光源点个数 */
    AWB_SPECIAL_LIGHT_INFO_S    astLightInfo[AWB_SPECIAL_LIGHT_SRC_NUM]; /* RW, 光源点参数 */
} AWB_SPECIAL_LIGHT_S;

typedef enum tagAwbLightSrcType
{
    AWB_LS_TYPE_A       = 0,  
    AWB_LS_TYPE_TL84,
    AWB_LS_TYPE_CWF,
    AWB_LS_TYPE_D50,
    AWB_LS_TYPE_D65,
    AWB_LS_TYPE_D75,
    AWB_LS_TYPE_NUM,
}AWB_LIGHT_SRC_TYPE_E;

typedef struct tagAwbLsWt
{ 
  NI_BOOL bEnable;                  /* RW, 权重计算使能 */
  NI_U8   au8LsWt[AWB_LS_ENV_LUM_THR_NODE][AWB_LS_TYPE_NUM]; /* RW, 各光源点权重参数 */
  NI_U16  au16LsEnvThr[AWB_LS_ENV_LUM_THR_NODE];             /* RW, 各光源点环境亮度参数 */
} AWB_LS_WT_S;

typedef struct tagAwbAttr
{  
    AWB_CT_RANGE_S          stCtRange;      /* RW, 白块色温区间 */
    AWB_CURVE_S             stCurve;        /* RW, 色温曲线 */
    AWB_GAIN_LIMIT_S        stLimit;        /* RW, 白块增益限幅 */
    AWB_LIGHT_INFO_S        astLightSrc[AWB_LS_TYPE_NUM];   /* RW, 各光源点的Gain参数,灯箱标定 */
    AWB_SPECIAL_LIGHT_S     stSpecialLight;     /* RW, 特殊光源点参数 */
    NI_U32                  u32ValidNumThr;     /* RW, 白块有效个数阈值 */
    AWB_LS_WT_S             stLsWt;             /* RW, 由环境亮度决定的白块权重系数 */
    NI_U32                  au32Reserve[8];     /* RW, 无效 */
} AWB_ATTR_S;

typedef struct tagAwbV100RegionaAttr
{   
    NI_BOOL                      bRegion;
    AISP_IMG_REGION_S            stRegion;
} AWB_REGION_ATTR_S;

typedef struct tagAwbDbgInfo
{  
    NI_U16     u16TC;
    NI_U16     u16GainR;
    NI_U16     u16GainB;
} AWB_DBG_INFO_S;

typedef struct tagCcmMatrix
{
    NI_U32              u32CtValue;     /*RW, 色温值 */
    NI_F32              af32Ccm[3][3];  /*RW, ccm矩阵 */
}CCM_MATRIX_S;

typedef struct tagCcmDbgInfo
{
    NI_U8               au8Sat;
    NI_F32              af32CcmWt[CCM_CT_NODE_NUM];
}CCM_DBG_INFO_S;

typedef struct tagCcmParam
{
    NI_U8               au8Sat[AISP_MAX_DB_NUM];        /*RW, 各个增益下的饱和度参数 */
    CCM_MATRIX_S        astCcm[CCM_CT_NODE_NUM];        /*RW, 不同色温下的ccm矩阵 */
}CCM_PARAM_S;

/**********************RGB2YUV***********************************/

typedef struct tagRgb2YuvMatrix
{
    NI_F32 af32Cof[3][3];       /*RW, RGB2YUV矩阵 */
    NI_S32 s32OfstY;            /*RW, Y Offset */
    NI_S32 s32OfstCb;           /*RW, Cb Offset */
    NI_S32 s32OfstCr;           /*RW, Cr Offset */
} RGB2YUV_MATRIX_S;

typedef struct tagRgb2YuvColor
{    
    NI_U8       u8Brt;      /*RW, 亮度调整参数 */
    NI_U8       u8Ctr;      /*RW, 对比度调整参数 */
    NI_U8       u8Sat;      /*RW, 饱和度调整参数 */
    NI_U8       u8Hue;      /*RW, 色调调整参数 */
} RGB2YUV_COLOR_S;

typedef struct tagRgb2Yuvparam
{   
    RGB2YUV_MATRIX_S         stDefMatrix;  /*RW, RGB2YUV矩阵 */
    RGB2YUV_COLOR_S          astColor[AISP_MAX_DB_NUM]; /*RW, rgb2yuv调整参数 */
} RGB2YUV_PARAM_S;

/**********************YHIST_CTR***********************************/
// typedef struct tagYhistCtrUsrParam
// {
//     NI_BOOL  bEnable;   /*使能*/
//     NI_U8    u8Level;   /*强度，0——100*/
// }YHIST_CTR_USR_PARAM_S;

// typedef struct tagYhistCtrCurve
// {
//     NI_U16  au16Thr[5];
//     NI_U16  au16Ratio[5];
// }YHIST_CTR_CURVE_S;

// typedef struct tagYhistCtrThrCalcParam
// {
//     NI_U8  u8Mode;                /*0:手动模式; 1:自动模式*/
//     NI_U16 u16Thr;                /*手动设置的thr值，即截断阈值*/
//     NI_U32 u32DrCalcRatio;        /*自动模式下用于计算动态范围的上下限像素比例,该参数表示比例*/
//     YHIST_CTR_CURVE_S stThresh;   /*根据u32DrCalcRatio算出的上下限像素灰度差值来计算截断阈值*/

// }YHIST_CTR_THR_CALC_PARAM;

// typedef struct tagYhistCtrAdjParam
// {
//     NI_U32 u32AdjThresh;                                       /*调节阈值*/
//     NI_U32 u32AdjSpeed;                                        /*调节速度,1--16*/
// }YHIST_CTR_ADJ_PARAM_S;

// typedef struct tagYhistCtrMapParam
// {
//     NI_U32 u32StartValue;                                      /*起始映射值*/
//     NI_U32 u32EndValue;                                        /*截止映射值 max 255*/
// }YHIST_CTR_MAP_PARAM_S;

// typedef struct tagYhistCtrParam
// {       
//     YHIST_CTR_ADJ_PARAM_S        stYhistCtrAdjParam;                        /*调节参数*/
//     YHIST_CTR_MAP_PARAM_S        astYhistCtrMapParam[AISP_MAX_DB_NUM];      /*映射参数*/
//     YHIST_CTR_THR_CALC_PARAM     stYhistCtrThrCalcParam;                    /*阈值计算参数*/
// }YHIST_CTR_PARAM_S;

/********************** DN ***********************************/
typedef enum tagDnMode
{
    DN_MODE_AUTO  = 0,      /* 自动模式 */
    DN_MODE_DAY   = 1,      /* 白天模式 */
    DN_MODE_NIGHT = 2,      /* 夜间模式 */
    DN_MODE_BUTT,
}DN_MODE_E;

typedef enum tagDnMethod
{
    DN_METHOD_PHOTOSENS     = 0,       /* 光敏方案 */
    DN_METHOD_SOFT          = 1,       /* 软光敏方案 */
    DN_METHOD_BUTT,
}DN_METHOD_E;

typedef struct tagDnParam
{
    NI_U16                  u16D2nValue;        /* RW, 日夜切换灵敏度,光敏方案时有效, range[0, u16N2dValue - 1], default:90 */
    NI_U16                  u16N2dValue;        /* RW, 夜日切换灵敏度,光敏方案有效, range[u16D2nValue + 1, 1024], default:150 */
    NI_U16                  u16Delay;           /* RW, 切换延时时间,以帧为单位, range[10, 0xFFFF], default:20 */
    NI_U32                  u32D2nGainThr;      /* RW, 日夜切换灵敏度增益阈值,软光敏方案有效, range[u32N2dGainThr + 6000, 72000], default:36000 */
    NI_U32                  u32N2dGainThr;      /* RW, 夜日切换灵敏度增益阈值,软光敏方案有效, range[6000, u32D2nGainThr - 6000], default:30000  */
    NI_U32                  u32EnvLumThr;       /* RW, 环境亮度阈值,无效 */

    NI_U16                  u16GainRThr;        /* RW, Gr阈值,软光敏方案有效,彩色模式为Gr差阈值,黑白模式为红外块的Gr阈值, range[256, 1024], default:彩色:40,黑白:300 */
    NI_U16                  u16GainBThr;        /* RW, Gb阈值,软光敏方案有效,彩色模式为Gb差阈值,黑白模式为红外块的Gb阈值, range[256, 1024], default:彩色:40,黑白:300 */

    NI_U16                  u16GainRAvgThr;     /* RW, Gr均值阈值,软光敏方案有效,黑白模式有效, range[256, 1024], default:320  */
    NI_U16                  u16GainBAvgThr;     /* RW, Gb均值阈值,软光敏方案有效,黑白模式有效, range[256, 1024], default:340  */
    NI_U16                  u16IrPerCent;       /* RW, 环境光强度比例阈值,,软光敏方案有效,彩色模式时,值越小灵敏度越高,黑白模式时,值越大灵敏度越高, range[0, 1000], default:500 */
}DN_PARAM_S;

typedef struct tagDnAttr
{ 
    DN_METHOD_E         enMethod;         /* RW, 方案选择 */
    DN_MODE_E           enDnMode;         /* RW, 日夜模式 */
    DN_PARAM_S          stDnParam;        /* RW, 日夜判断参数 */
}DN_ATTR_S;

typedef struct tagDnSceneInfo
{
    NI_U32               u32EnvLumValue;     /* 当前环境亮度信息 */
    NI_U32               u32SysGain;         /* 当前增益信息 */
    // NI_U32               u32SysSht;          /* 当前快门信息 */
    
    NI_U16               u16Var;             /* 无效 */
    NI_U16               u16DrInfo;          /* 无效 */
    NI_U16               u16GainR;           /* 当前环境WB的GainR值 */
    NI_U16               u16GainB;           /* 当前环境WB的GainR值 */
    NI_U16               *pu16Hist;          /* 无效 */

    NI_S32               s32EvTarget;        /* 当前的目标亮度值 */
    NI_S32               s32EvCur;           /* 当前的图像亮度值 */
}DN_SCNEN_INFO_S;


typedef struct tagDnResult
{
    NI_BOOL                   bDayEn;                   /* 日夜结果, 0表示晚上, 1表示白天 */
    NI_BOOL                   bIrCutEn;                 /* 滤光片结果, 0表示晚上, 1表示白天 */
    NI_U32                    u32DnEnv;                 /* 当前环境的日夜状态信息, 1表示白天, 2表示晚上 */
}DN_RESULT_S;

typedef struct tagDnDbgInfo
{    
    NI_U32                    u32EnvLum;               /* RO, 当前环境亮度判断信息，u32EnvLum = 当前曝光*当前增益 单位：us*倍 */
    //白切黑
    NI_U32                    u32D2nEnvThr;            /* RO, 白切黑亮度阈值， u32EnvLum 需大于此值才可以触发切换 u32D2nEnvThr = u32D2nGainThr * 当前原始帧率下的最大曝光值 单位：us*倍*/
    //黑切白
    NI_U32                    u32N2dEnvThr;            /* RO, 黑切白亮度阈值， u32EnvLum 需小于此值才可以触发切换 u32N2dEnvThr = u32N2dGainThr * 当前原始帧率下的最大曝光值 单位：us*倍*/  
    NI_U16                    u16Gr;                   /* RO, 当前画面Gr增益, 当此值u16Gr > u16GainRAvgThr 或 u16Gb > u16GainBAvgThr时，符合红外黑切白要求 */
    NI_U16                    u16Gb;                   /* RO, 当前画面Gb增益, 当此值u16Gr > u16GainRAvgThr 或 u16Gb > u16GainBAvgThr时，符合红外黑切白要求 */
    NI_U16                    u16IrPerCent;            /* RO, 当前画面红外占比，当此值小于配置中的u16IrPerCent时，符合红外黑切白要求 */
} DN_DBG_INFO_S;

/********************** IR LIGHT ***********************************/
#define IR_LIGHT_MAX_LIGHT_NUM   (4)

typedef enum tagIrLightMode
{
    IR_LIGHT_MODE_AUTO        = 0,            /* 关闭补光 */
    IR_LIGHT_MODE_CLOSE       = 1,            /* 自动补光 */
    IR_LIGHT_RATE_PRIO        = 2,            /* 倍率优先 目前不支持 */
    IR_LIGHT_MODE_MANUAL      = 3,            /* 手动补光 */
    IR_LIGHT_MODE_BUTT,
}IR_LIGHT_MODE_E;

typedef struct tagIrLightUsr
{
    IR_LIGHT_MODE_E     enLightMode;           /*RW, 补光模式 */
    NI_U8               u8Level;               /*RW, 补光灯强度,manual模式时有效 [0:100] */
}IR_LIGHT_USR_S;

typedef struct tagIrLightAttr
{
    IR_LIGHT_USR_S                  stIrLightUsr;       /*RW, 补光灯参数 */
    NI_U16                          u16TargetIllum;     /*RW, 目标亮度 [0:10000] */
    NI_U8                           u8Tolerance;        /*RW, 容忍误差 [0:100] */
    NI_U8                           u8Speed;            /*RW, 调节速度 [0:100] */
    NI_U16                          u16IrLightMin;      /*RW, 红外灯触发点 */
    NI_U16                          u16IrLightMax;      /*RW, 红外灯饱和点 */
    NI_U8                           u8IrLightNum;       /*RW, 灯的个数 */
    NI_U8                           au8Reserved[3];
}IR_LIGHT_ATTR_S;

typedef struct tagIrLightSceneInfo
{
    NI_BOOL                bMaxExp;             /*RW, 当前是否为最大曝光 */
    NI_U32                 u32SysGain;          /*RW, 当前增益信息 */
    NI_U32                 u32SysSht;           /*RW, 当前快门信息 */
    NI_U16                 u16CurAverY;         /*RW, 当前图像亮度均值 */
    NI_U16                 u16CurEv;            /*RW, 当前图像亮度 */
    NI_U16                 u16TargetEv;         /*RW, 目标图像亮度 */
    NI_U8                  u8IsNight;           /*RW, 夜间场景 */
    NI_U8                  au8Reserved[1];
}IR_LIGHT_SCNEN_INFO_S;

typedef struct tagIrLightResult
{
    NI_U32                    au32LightLevel[IR_LIGHT_MAX_LIGHT_NUM];            /*RW, 红外灯强度 */
    NI_U8                     u8LightChange;                                     /*RW, 红外灯强度改变, 0:未发生改变, 1:发生改变 */
    NI_U8                     au8Reserved[7];
}IR_LIGHT_RESULT_S;

/********************** IR LIGHT ***********************************/
#define LED_LIGHT_MAX_LIGHT_NUM   (4)

typedef enum tagLedLightMode
{
    LED_LIGHT_MODE_AUTO       = 0,            /* 自动补光 */
    LED_LIGHT_MODE_CLOSE      = 1,            /* 关闭补光 */
    LED_LIGHT_RATE_PRIO       = 2,            /* 倍率优先 目前不支持 */
    LED_LIGHT_MODE_MANUAL     = 3,            /* 手动补光 */
    LED_LIGHT_MODE_BUTT,
}LED_LIGHT_MODE_E;

typedef struct tagLedLightUsr
{
    LED_LIGHT_MODE_E     enLightMode;           /*RW, 补光模式 */
    NI_U8                u8Level;                /*RW, 补光灯强度,manual模式时有效 [0:100] */
}LED_LIGHT_USR_S;

typedef struct tagLedLightAttr
{
    LED_LIGHT_USR_S                 stLedLightUsr;       /*RW, 补光灯参数 */
    NI_U16                          u16TargetIllum;      /*RW, 目标亮度 [0:10000] */
    NI_U8                           u8Tolerance;         /*RW, 容忍误差 [0:100] */
    NI_U8                           u8Speed;             /*RW, 调节速度 [0:100] */
    NI_U16                          u16LedLightMin;      /*RW, 红外灯触发点 */
    NI_U16                          u16LedLightMax;      /*RW, 红外灯饱和点 */
    NI_U8                           u8LedLightNum;       /*RW, 灯的个数 */
    NI_U8                           au8Reserved[3];
}LED_LIGHT_ATTR_S;

typedef struct tagLedLightSceneInfo
{
    NI_BOOL                bMaxExp;             /*RW, 当前是否为最大曝光 */
    NI_U32                 u32SysGain;          /*RW, 当前增益信息 */
    NI_U32                 u32SysSht;           /*RW, 当前快门信息 */
    NI_U16                 u16CurAverY;         /*RW, 当前图像亮度均值 */
    NI_U16                 u16CurEv;            /*RW, 当前图像亮度 */
    NI_U16                 u16TargetEv;         /*RW, 目标图像亮度 */
    NI_U8                  u8IsNight;           /*RW, 夜间场景 */
    NI_U8                  au8Reserved[1];
}LED_LIGHT_SCNEN_INFO_S;

typedef struct tagLedLightResult
{
    NI_U32                    au32LightLevel[LED_LIGHT_MAX_LIGHT_NUM];            /*RW, 白光灯强度 */
    NI_U8                     u8LightChange;                                      /*RW, 白光灯强度改变, 0:未发生改变, 1:发生改变 */
    NI_U8                     au8Reserved[7];
}LED_LIGHT_RESULT_S;


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

