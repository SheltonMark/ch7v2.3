/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NI_COMM_AISPALG_H_
#define _NI_COMM_AISPALG_H_

#include "ni_type.h"
#include "ni_common.h"
#include "ni_aisp_define.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/********************** COMPAT ***********************************/

typedef enum tagAeAwesDataChn {
    COMPAT_AWES_DATA_CHN_R   = 0, /* AWES统计块的 R 通道 */
    COMPAT_AWES_DATA_CHN_G   = 1, /* AWES统计块的 G 通道 */
    COMPAT_AWES_DATA_CHN_B   = 2, /* AWES统计块的 B 通道 */
    COMPAT_AWES_DATA_CHN_Y   = 3, /* AWES统计块的 Y 通道 */
    COMPAT_AWES_DATA_CHN_NUM = 4,
} COMPAT_AWES_DATA_CHN_E;

typedef struct tagCompatAwesBlkData {
    NI_U16 u16R; /* AWES统计块的 R 通道值, 为归一化到10bit的数据 */
    NI_U16 u16G; /* AWES统计块的 G 通道值, 为归一化到10bit的数据 */
    NI_U16 u16B; /* AWES统计块的 B 通道值, 为归一化到10bit的数据 */
    NI_U16 u16Y; /* AWES统计块的 Y 通道值, 为归一化到10bit的数据 */
} COMPAT_AWES_BLK_DATA_S;

typedef enum tagHwStatisticsPos {
    HW_STATISTICS_POS_FRAME_L,             /* 只选用长曝光帧给算法 */
    HW_STATISTICS_POS_FRAME_S,             /* 只选用短曝光帧给算法 */
    HW_STATISTICS_POS_FRAME_COMPO,         /* 只选用合成帧给算法 */
    HW_STATISTICS_POS_FRAME_L_S,           /* 选用长、短两帧统计数据给算法 */
    HW_STATISTICS_POS_FRAME_L_COMPO,       /* 选用长、合成两帧统计数据给算法 */
    HW_STATISTICS_POS_FRAME_S_COMPO,       /* 选用短、合成两帧统计数据给算法 */
    HW_STATISTICS_POS_FRAME_L_S_COMPO,     /* 选用长、短、合成帧三帧统计数据给算法 */
    HW_STATISTICS_POS_FRAME_L_S_ALTERNATE, /* 选用长、短两帧交替统计数据给算法*/

    HW_STATISTICS_POS_BUTT
} HW_STATISTICS_POS_E;

typedef enum tagHwStatisticsFormat {
    HW_STATISTICS_AWES_FORMAT_RGBY  = 0, /* 统计数据硬件排好后, 内存中排列顺序为rgbyrgby...rgbyrgby */
    HW_STATISTICS_AWES_FORMAT_GR_YB = 1, /* 统计数据硬件排好后, 内存中排列顺序为grgrybyb...grgrybyb */
    HW_STATISTICS_AWES_FORMAT_BR_YG = 2, /* 统计数据硬件排好后, 内存中排列顺序为brbrygyg...brbrygyg */
} HW_STATISTICS_AWES_FORMAT_E;

typedef struct tagHwStatisticsAwes {
    NI_U32                      u32DataWidth;     /* AWES统计数据位宽 */
    NI_U32                      u32AwesMbufBlkId; /* AWES统计数据缓存块Id */
    NI_U32                      u32BlockNum;      /* AWES统计一帧的分块总数 */
    NI_U32                      u32HBlockNum;     /* AWES统计水平方向分块个数 */
    NI_U32                      u32VBlockNum;     /* AWES统计垂直方向分块个数 */
    HW_STATISTICS_POS_E         enAwesPos;        /* AWES统计数据位置 */
    HW_STATISTICS_AWES_FORMAT_E enFormat;         /* AWES统计数据在内存中的存放格式 */
    NI_VOID                    *pavAwesData[3];   /* AWES统计数据值, 每个分量数据位宽为16，
                                                   * 每帧统计数据量 = u32BlockNum * 2 * 4 bytes,
                                                   * 数据内容按照块的顺序即先水平后垂直排列，
                                                   * 每一块内部按照 r/g/b/y排列
                                                   * 当 enAwesPos = HW_STATISTICS_POS_FRAME_S 时
                                                   * 仅pavAwesData[0] 有效 表示只有短帧统计数据
                                                   * 当 enAwesPos = HW_STATISTICS_POS_FRAME_L 时
                                                   * 仅pavAwesData[0] 有效 表示只有长帧统计数据
                                                   * 当 enAwesPos = HW_STATISTICS_POS_FRAME_COMPO 时
                                                   * 仅pavAwesData[0] 有效 表示只有合成帧统计数据
                                                   * 当 enAwesPos = HW_STATISTICS_POS_FRAME_L_S 时
                                                   * pavAwesData[0] 表示长帧统计数据
                                                   * pavAwesData[1] 表示短帧统计数据
                                                   * 当 enAwesPos = HW_STATISTICS_POS_FRAME_L_COMPO 时
                                                   * pavAwesData[0] 表示长帧统计数据
                                                   * pavAwesData[1] 表示合成帧统计数据
                                                   * 当 enAwesPos = HW_STATISTICS_POS_FRAME_S_COMPO 时
                                                   * pavAwesData[0] 表示短帧统计数据
                                                   * pavAwesData[1] 表示合成帧统计数据
                                                   * 当 enAwesPos = HW_STATISTICS_POS_FRAME_L_S_COMPO 时
                                                   * pavAwesData[0] 表示长帧统计数据
                                                   * pavAwesData[1] 表示短帧统计数据
                                                   * pavAwesData[2] 表示合成帧统计数据 */
} HW_STATISTICS_AWES_S;

typedef struct tagHwStatisticsYhist {
    NI_U32   u32MbufBlkId; /*RO, pu32YhistData对应缓存块的id号 */
    NI_U32   u32YhistMin;  /*RO, 直方图统计最小值 */
    NI_U32   u32YhistMax;  /*RO, 直方图统计最大值 */
    NI_U32   u32YhistAvg;  /*RO, 直方图统计得到亮度平均值 */
    NI_VOID *pvYhistData;  /*RO, 直方图统计数据, 数据位宽为32，固定256深度 */
} HW_STATISTICS_YHIST_S;

typedef struct tagHwStatisticsRawhist {
    NI_U32              u32HistMbufBlkId; /* RO, apu32RawHistData对应缓存块的id号 */
    NI_U32              u32HdrOutputLS;   /* RO, 指示宽动态长短帧交替模式下, 当前输出的是长帧还是短帧, 0:短帧, 1:长帧 */
    HW_STATISTICS_POS_E enRawHistPos;     /* RO, 指示当前RawHist的统计数据来源 */
    NI_U32             *pu32RawHistData;  /* RO, RAWHIST统计的rgby值, 每帧统计数据:R/G/B/Y 的位宽均为 16，数据按照 R[128]/G[128]/B[128]/Y[256]顺序排列。即总大小为：2* (128*3+256)bytes*/
} HW_STATISTICS_RAWHIST_S;

typedef struct tagHwStatistics {
    HW_STATISTICS_AWES_S    stAwes;    /* 见 HW_STATISTICS_AWES_S 中的描述 */
    HW_STATISTICS_YHIST_S   stYhist;   /* 见 HW_STATISTICS_YHIST_S 中的描述 */
    HW_STATISTICS_RAWHIST_S stRawhist; /* 见 HW_STATISTICS_RAWHIST_S 中的描述 */
} HW_STATISTICS_S;

typedef struct tagNmlzImgRegion {
    NI_U16 u16StartX; /* RW, 启点 X 坐标, range:[0, u16EndX], default:0 */
    NI_U16 u16StartY; /* RW, 启点 Y 坐标, range:[0, u16EndY], default:0 */
    NI_U16 u16EndX;   /* RW, 终点 X 坐标, range:[u16StartX, 1024], default:1024 */
    NI_U16 u16EndY;   /* RW, 终点 Y 坐标, range:[u16StartY, 1024], default:1024 */
} NMLZ_IMG_REGION_S;

/********************** AE ***********************************/

#define AE_MAX_DB_NUM       (AISP_MAX_DB_NUM)          /* 最大支持的DB个数, 最大的DB增益为 AE_MAX_DB_NUM * 6 */
#define AE_ROUTE_MIN_NODES  (AISP_AE_ROUTE_MIN_NODES)  /* 曝光路径最小节点数 */
#define AE_ROUTE_MAX_NODES  (AISP_AE_ROUTE_MAX_NODES)  /* 曝光路径最大节点数 */
#define AE_WIN_WT_MAX_NODES (AISP_AE_WIN_WT_MAX_NODES) /* 区域权重的坐标点数,水平垂直相同 */

typedef enum tagAeMode {
    AE_MODE_NORMAL = 0, /* 普通模式 */
    AE_MODE_BLC,        /* 背光补偿 */
    AE_MODE_HLC,        /* 高光抑制 */
    AE_MODE_BUTT,
} AE_MODE_E;

typedef struct tagAeAttr {
    NI_U8     u8AeInterval;                    /* RW, AE运行间隔帧数, range[0, 255], default:0 */
    NI_U8     u8AeSpeed;                       /* RW, AE调节速度, range[1, 255], default:128 */
    NI_U8     u8AeTolerance;                   /* RW, AE误差容忍度, range[0, 255], default:2 */
    NI_U8     u8AeStartTor;                    /* RW, AE启动调节容忍度, range[0, 255], default:3, u8AeStartTor > u8AeTolerance */
    NI_U8     u8AeTarget;                      /* RW, AE目标亮度, range[0, 255], default:90 */
    NI_U8     au8AeTargetRatio[AE_MAX_DB_NUM]; /* RW, 目标亮度比例, 真实目标亮度 = u8AeTarget * u8EvRatio / 128, range:[0, 255], U1.7 default:128 AISP_MAX_DB_NUM */
    NI_U8     u8AeWhiteDelay;                  /* RW, 图像偏亮时延迟调节帧数, range:[0, 255], default:2 */
    NI_U8     u8AeBlackDelay;                  /* RW, 图像偏暗时延迟调节帧数, range:[0, 255], default:5 */
    AE_MODE_E enAeMode;                        /* RW, AE模式, default:AE_MODE_NORMAL*/
    NI_U16    u16AeBaseLowThr;                 /* RW, AE普通模式亮度裁切低阈值, range[0, u16AeBaseHigThr], default:300 */
    NI_U16    u16AeBaseHigThr;                 /* RW, AE普通模式亮度裁切高阈值, range[u16AeBaseLowThr, 65535], default:10000 */
    NI_U16    u16AeBaseStr;                    /* RW, AE普通模式低亮权重, range[16, 255] default:160 U4.4 */
    NI_U8     u8AeModeStartPos;                /* RW, AE补偿模式启动偏离点, range[0, u8AeModeEndPos] default:20 */
    NI_U8     u8AeModeEndPos;                  /* RW, AE补偿模式终止偏离点, range[u8AeModeStartPos, 255] default:210 */
    NI_U8     u8AeModeStr;                     /* RW, AE补偿模式强度, range[16, 255] default:64 U4.4 */
    NI_U8     u8AeMaxOffset;                   /* RW, AE限幅, range[0, 255] default:16 */
} AE_ATTR_S;

typedef struct tagAeRouteNode {
    NI_S32 s32ShtTime;    /* RW, 快门时间, 单位us, range[0, 1000000] */
    NI_S32 s32SysGain;    /* RW, DB增益 * 1000 range[0, AE_MAX_DB_NUM  * 6] */
    NI_S32 s32ApePercent; /* RW, 无效 */
} AE_ROUTE_NODE_S;

typedef struct tagAeRoute {
    NI_U8           u8AeRouteTotalNum;                /*RW, AE曝光路径节点数, range:[AE_ROUTE_MIN_NODES, AE_ROUTE_MAX_NODES] */
    NI_U8           u8AeRouteMaxShtPos;               /*RW, AE曝光路径满曝光时间坐标, 用于在切换不同分辨率和帧率情况下对曝光路径表曝光时间做自适应, range:[0, AE_ROUTE_MAX_NODES - 1] */
    AE_ROUTE_NODE_S astRouteNode[AE_ROUTE_MAX_NODES]; /*RW, AE曝光路径节点描述 */
} AE_ROUTE_S;

#define AE_IMG_REGION_S NMLZ_IMG_REGION_S

typedef struct tagAeRegionAttr {
    NI_BOOL         bEnable;  /*RW, 区域曝光使能, range:[0, 1], default:0 */
    AE_IMG_REGION_S stRegion; /*RW, 曝光统计的区域, 具体见 AE_IMG_REGION_S 的描述 */
} AE_REGION_ATTR_S;

typedef struct tagAeWinWt {
    NI_U8 au8AeWeight[AE_WIN_WT_MAX_NODES][AE_WIN_WT_MAX_NODES]; /* RW, 曝光统计的区域权重, 禁止全设置为0, range:[0, 15], default:1 */
} AE_WIN_WT_S;

#define AE_STATISTICS_AWES_FORMAT_E HW_STATISTICS_AWES_FORMAT_E
#define AE_STATISTICS_POS_E         HW_STATISTICS_POS_E
#define AE_STATISTICS_AWES_S        HW_STATISTICS_AWES_S

typedef struct tagAeStatisticsHw {
    AE_STATISTICS_AWES_S stAwes; /* 见 AE_STATISTICS_AWES_S 中的描述 */
} AE_STATISTICS_HW_S;

typedef struct tagAeStatisticsSw {
    NI_U32   u32EnvLumValue; /* 当前环境亮度信息 */
    NI_U32   u32SysGain;     /* 当前增益信息 */
    NI_U32   u32SysSht;      /* 当前快门信息 */
    NI_U32   u32BufSize;     /* 无效 pvCalcBuf指向的内存大小 */
    NI_VOID *pvCalcBuf;      /* 无效 算法计算用中间数据缓存地址, pvCalcBuf需要的大小可以通过接口获取 */
} AE_STATISTICS_SW_S;

typedef struct tagAeStatistics {
    AE_STATISTICS_HW_S stStatHw; /* 见 AE_STATISTICS_HW_S 中的描述 */
    AE_STATISTICS_SW_S stStatSw; /* 见 AE_STATISTICS_SW_S 中的描述 */
} AE_STATISTICS_S;

typedef struct tagAeResult {
    NI_BOOL bExpSwitch;     /* 是否发生曝光量互换 */
    NI_S32  s32ShtTime;     /* 快门时间, 单位us */
    NI_S32  s32SysGain;     /* DB增益 * 1000 */
    NI_S32  s32SysMultGain; /* 倍数增益 * 1000 */
    NI_S32  s32EnvLum;      /* 环境亮度 */
    NI_S32  s32ApertValue;  /* 无效 */
} AE_RESULT_S;

typedef enum tagAeExpStatus {
    AE_EXP_STATUS_MIN = 0, /* 曝光调节到最小 */
    AE_EXP_STATUS_NORMAL,  /* 正常曝光调节 */
    AE_EXP_STATUS_MAX,     /* 曝光调节到最大 */
    AE_EXP_STATUS_BUTT,
} AE_EXP_STATUS_E;

typedef enum tagAeAdjStatus {
    AE_ADJ_STATUS_STABLE = 0, /* 曝光调节稳定 */
    AE_ADJ_STATUS_TO_BRT,     /* 曝光向大调节 */
    AE_ADJ_STATUS_TO_DARK,    /* 曝光向小调节 */
    AE_ADJ_STATUS_BUTT,
} AE_ADJ_STATUS_E;

typedef struct tagAeEvEstimate {
    NI_S32 s32EvTarget;       /* 图像目标亮度 range[0, 255] */
    NI_S32 s32EvTarget10bit;  /* 图像目标亮度转化为10bit的值, 由s32EvTarget非线性转化而来, range[0, 1023] */
    NI_S32 s32EvCur;          /* 算法统计的当前画面亮度 */
    NI_S32 s32EvModeRatio;    /* 测光比, 单位:倍数 * 1000 */
    NI_S32 s32EvDelta;        /* 当前画面亮度亮度和图像目标亮度的差值 */
    NI_S32 as32AbsEvDelta[3]; /* 记录的前三次亮度差值 */
} AE_EV_ESTIMATE_S;

typedef struct tagAeExpStatusInfo {
    AE_EXP_STATUS_E  enExpStatus;  /* 曝光状态 */
    AE_ADJ_STATUS_E  enAdjStatus;  /* 曝光调整状态 */
    AE_EV_ESTIMATE_S stEvEstimate; /* 曝光评估信息 */
} AE_EXP_STATUS_INFO_S;

typedef struct tagAeInfo {
    AE_RESULT_S          stExpResult; /* AE算法计算出的曝光结果 */
    AE_EXP_STATUS_INFO_S stExpStatus; /* AE算法计算出的当前曝光的状态信息 */
} AE_INFO_S;

/********************** AWB ***********************************/

#define AWB_CURVE_BIN_NUM         (AISP_AWB_CURVE_BIN_NUM)         /* 色温曲线横坐标点数 */
#define AWB_SPECIAL_LIGHT_SRC_NUM (AISP_AWB_SPECIAL_LIGHT_SRC_NUM) /* 特殊光源点最大个数 */
#define AWB_LS_ENV_LUM_THR_NODE   (AISP_AWB_LS_ENV_LUM_THR_NODE)   /* 环境亮度阈值点数 */
#define AWB_MOOTH_FRAME_NUM       (32)                             /* WB结果滤波缓存帧数 */

typedef struct tagAwbCtRange {
    NI_U16 u16TCMax; /* RW, TC最大值, range:[u16TCMin, 4096], default:2200 */
    NI_U16 u16TCMin; /* RW, TC最小值, range:[0, u16TCMax], default:800 */
} AWB_CT_RANGE_S;

typedef struct tagAwbCurve {
    NI_U16 au16CurveUp[AWB_CURVE_BIN_NUM];   /* RW, 色温曲线的上限, range:[au16CurveDown, 4096],
                                              * default:{1240, 1230, 1220, 1210, 1200, 1180, 1150, 800, 760, 550, 500, 470, 400, 400, 400, 400, 400, 400, 400, 400} */
    NI_U16 au16CurveDown[AWB_CURVE_BIN_NUM]; /*RW, 色温曲线的下线限, range:[0, au16CurveUp],
                                              * default:{940, 930, 920, 910, 890, 800, 770, 550, 520, 400, 370, 350, 381, 250, 250, 250, 250, 250, 250, 250} */
} AWB_CURVE_S;

typedef struct tagAwbGainLimit {
    NI_U16 u16RgainMin; /*RW, 白块的RGain下限, range:[0, u16RgainMax], default:256 */
    NI_U16 u16RgainMax; /*RW, 白块的RGain上限, range:[u16RgainMin, 4096], default:900 */
    NI_U16 u16BgainMin; /*RW, 白块的BGain下限, range:[0, u16RgainMax], default:300  */
    NI_U16 u16BgainMax; /*RW, 白块的BGain上限, range:[u16RgainMin, 4096], default:1100  */
} AWB_GAIN_LIMIT_S;

typedef struct tagAwbLightInfo {
    NI_U16 u16GainR; /*RW, 光源点的RGain, range:[0, 4096] */
    NI_U16 u16GainB; /*RW, 光源点的BGain, range:[0, 4096]  */
} AWB_LIGHT_INFO_S;

typedef struct tagAwbSlsWt {
    NI_U8  u8RatioMulCtH;  /*RW, 无效 */
    NI_U8  u8RatioMulCtL;  /*RW, 无效 */
    NI_U8  u8RaitoEnvLumH; /*RW, 光源点的环境亮度权重系数Heigh, range:[0, 255] */
    NI_U8  u8RaitoEnvLumL; /*RW, 光源点的环境亮度权重系数Low, range:[0, 255] */
    NI_U16 u16EnvLumH;     /*RW, 光源点的权重系数的环境亮度阈值Heigh, range:[u16EnvLumL, 60000] */
    NI_U16 u16EnvLumL;     /*RW, 光源点的权重系数的环境亮度阈值Low, range:[0, u16EnvLumH] */
} AWB_SPECIAL_LIGHT_WT_S;

typedef struct tagAwbSpecialLightInfo {
    AWB_GAIN_LIMIT_S       stGainLimt; /* RW, 光源点WB增益阈值, 详见 AWB_GAIN_LIMIT_S 的描述 */
    AWB_SPECIAL_LIGHT_WT_S stWt;       /* RW, 光源点权重系数参数, 详见 AWB_SPECIAL_LIGHT_WT_S 的描述  */
} AWB_SPECIAL_LIGHT_INFO_S;

typedef struct tagAwbSpecialLight {
    NI_U8                    u8ValidNum;                              /* RW, 光源点个数, range:[0, AWB_SPECIAL_LIGHT_SRC_NUM], default:0 */
    AWB_SPECIAL_LIGHT_INFO_S astLightInfo[AWB_SPECIAL_LIGHT_SRC_NUM]; /* RW, 光源点参数信息, 数据有效数据个数由 u8ValidNum 决定 */
} AWB_SPECIAL_LIGHT_S;

typedef enum tagAwbLightSrcType {
    AWB_LS_TYPE_A,    /* A 光源 */
    AWB_LS_TYPE_TL84, /* TL84 光源 */
    AWB_LS_TYPE_CWF,  /* CWF 光源 */
    AWB_LS_TYPE_D50,  /* D50 光源 */
    AWB_LS_TYPE_D65,  /* D65 光源 */
    AWB_LS_TYPE_D75,  /* D75 光源 */
    AWB_LS_TYPE_NUM,
} AWB_LIGHT_SRC_TYPE_E;

typedef struct tagAwbLsWt {
    NI_BOOL bEnable;                                           /* RW, 各光源点权重计算使能 */
    NI_U8   au8LsWt[AWB_LS_ENV_LUM_THR_NODE][AWB_LS_TYPE_NUM]; /* RW, 光源点的环境亮度权重系数, range:[0, 255] */
    NI_U16  au16LsEnvThr[AWB_LS_ENV_LUM_THR_NODE];             /* RW, 各光源点的权重系数的环境亮度阈值,
                                                                * 数组下标大的元素值要大于等于数据下标小的元素值, range:[0, 60000] */
} AWB_LS_WT_S;

typedef struct tagAwbAttr {
    AWB_CT_RANGE_S      stCtRange;                    /* RW, 有效白块的色温极限区间 详见 AWB_CT_RANGE_S 的描述 */
    AWB_CURVE_S         stCurve;                      /* RW, 有效白块所在的色温区间 详见 AWB_CURVE_S 的描述 */
    AWB_GAIN_LIMIT_S    stLimit;                      /* RW, 有效白块的增益范围 详见 AWB_GAIN_LIMIT_S 的描述 */
    AWB_LIGHT_INFO_S    astLightSrc[AWB_LS_TYPE_NUM]; /* RW, 各光源点的Gain参数, 由灯箱标定, 详见 AWB_GAIN_LIMIT_S 的描述 */
    AWB_SPECIAL_LIGHT_S stSpecialLight;               /* RW, 特殊光源点参数 */
    NI_U32              u32ValidNumThr;               /* RW, 白块有效个数阈值, range:[0, 255] */
    AWB_LS_WT_S         stLsWt;                       /* RW, 由环境亮度决定的白块权重系数 */
    NI_U32              au32Reserve[8];               /* RW, 无效 */
} AWB_ATTR_S;

#define AWB_IMG_REGION_S NMLZ_IMG_REGION_S

typedef struct tagAwbRegionAttr {
    NI_BOOL          bRegionEn; /* RW, 区域白平衡使能, range:[0, 1], default:0 */
    AWB_IMG_REGION_S stRegion;  /* RW, 计算白平衡的区域, 详见 AWB_IMG_REGION_S 的描述 */
} AWB_REGION_ATTR_S;

typedef struct tagAwbStatisticsSw {
    NI_U32   u32EnvLumValue; /* 当前环境亮度信息 */
    NI_U32   u32SysGain;     /* 当前增益信息 */
    NI_U32   u32SysSht;      /* 当前快门信息 */
    NI_U32   u32BufSize;     /* 无效 pvCalcBuf指向的内存大小 */
    NI_VOID *pvCalcBuf;      /* 无效 算法计算用中间数据缓存地址, pvCalcBuf需要的大小可以通过接口获取 */
} AWB_STATISTICS_SW_S;

#define AWB_STATISTICS_AWES_FORMAT_E HW_STATISTICS_AWES_FORMAT_E
#define AWB_STATISTICS_POS_E         HW_STATISTICS_POS_E
#define AWB_STATISTICS_AWES_S        HW_STATISTICS_AWES_S

typedef struct tagAwbStatisticsHw {
    AWB_STATISTICS_AWES_S stAwes; /* 见 AWB_STATISTICS_AWES_S 中的描述 */
} AWB_STATISTICS_HW_S;

typedef struct tagAwbStatistics {
    AWB_STATISTICS_HW_S stStatHw; /* 见 AE_STATISTICS_HW_S 中的描述 */
    AWB_STATISTICS_SW_S stStatSw; /* 见 AE_STATISTICS_SW_S 中的描述 */
} AWB_STATISTICS_S;

typedef struct tagAwbResult {
    NI_U16 u16GainR;  /* GainR 结果 */
    NI_U16 u16GainB;  /* GainB 结果 */
    NI_U16 u16GainGr; /* GainGr 结果 */
    NI_U16 u16GainGb; /* GainGb 结果 */
} AWB_RESULT_S;

typedef struct tagAwbSmooth {
    NI_BOOL bFull;                          /* 标记 au16GainR/B 数组是否写满 */
    NI_U16  au16GainR[AWB_MOOTH_FRAME_NUM]; /* 记录历史的GainR信息 */
    NI_U16  au16GainB[AWB_MOOTH_FRAME_NUM]; /* 记录历史的GainB信息 */
    NI_U16  u16Pointer;                     /* 最近一次的GainR/B数据在数组中的索引 */
    NI_U32  u32SumR;                        /* 记录的历史GainR的总和 */
    NI_U32  u32SumB;                        /* 记录的历史GainB的总和 */
    NI_U16  u16Rgain;                       /* 滤波后的RGain */
    NI_U16  u16Bgain;                       /* 滤波后的BGain */
} AWB_SMOOTH_S;

typedef struct tagAwbInfo {
    AWB_RESULT_S stAwbResult; /* AWB结果信息 */

    NI_U16 u16TC;    /* 当前的TC均值 */
    NI_U16 u16GainR; /* 当前的GainR均值 */
    NI_U16 u16GainB; /* 当前的GainB均值 */

    AWB_SMOOTH_S stSmooth; /* 滤波迭代数据 */
} AWB_INFO_S;

/********************** CCM ***********************************/

#define CCM_CT_NODE_NUM (AISP_CCM_CT_NODE_NUM)

typedef struct tagCcmMatrix {
    NI_U32 u32CtValue;    /* RW, CCM矩阵的色温信息, 以WB的RGain等效色温信息 */
    NI_F32 af32Ccm[3][3]; /* RW, u32CtValue色温下的CCM矩阵
                           * 要求 af32Ccm[i][0] + af32Ccm[i][1] + af32Ccm[i][1] 和为 1.0 */
} CCM_MATRIX_S;

typedef struct tagCcmParam {
    NI_U32 u32CtValue;                    /* RW, 当前色温信息, 以WB的RGain等效色温信息 */
    NI_U8  u8Sat;                         /* RW, 图像饱和度
                                           * range[0, 255], default:128 */
    CCM_MATRIX_S astCcm[CCM_CT_NODE_NUM]; /* RW, 不同色温下的CCM矩阵 */
} CCM_PARAM_S;

/********************** DAYNIGHT ***********************************/

typedef enum tagDnMode {
    DN_MODE_AUTO  = 0, /* 自动模式 */
    DN_MODE_DAY   = 1, /* 白天模式 */
    DN_MODE_NIGHT = 2, /* 夜间模式 */
    DN_MODE_BUTT,
} DN_MODE_E;

typedef enum tagDnMethod {
    DN_METHOD_PHOTOSENS = 0, /* 光敏方案 */
    DN_METHOD_SOFT      = 1, /* 软光敏方案 */
    DN_METHOD_BUTT,
} DN_METHOD_E;

typedef struct tagDnParam {
    NI_U16 u16D2nValue;   /* RW, 日夜切换灵敏度,光敏方案时有效, range[0, u16N2dValue - 1], default:90 */
    NI_U16 u16N2dValue;   /* RW, 夜日切换灵敏度,光敏方案有效, range[u16D2nValue + 1, 1024], default:150 */
    NI_U16 u16Delay;      /* RW, 切换延时时间,以帧为单位, range[10, 0xFFFF], default:20 */
    NI_U32 u32D2nGainThr; /* RW, 日夜切换灵敏度增益阈值,软光敏方案有效, range[u32N2dGainThr - 6000, 72000], default:36000 */
    NI_U32 u32N2dGainThr; /* RW, 夜日切换灵敏度增益阈值,软光敏方案有效, range[6000, u32D2nGainThr - 6000], default:30000  */

    NI_U16 u16GainRThr; /* RW, Gr阈值,软光敏方案有效,Gr差阈值, range[10, 256], default:40 */
    NI_U16 u16GainBThr; /* RW, Gb阈值,软光敏方案有效,Gb差阈值, range[10, 256], default:40 */

    NI_U16 u16GainRAvgThr;  /* RW, Gr均值阈值,软光敏方案有效,黑白模式有效, range[256, 1024], default:320  */
    NI_U16 u16GainBAvgThr;  /* RW, Gb均值阈值,软光敏方案有效,黑白模式有效, range[256, 1024], default:340  */
    NI_U16 u16LightPerCent; /* RW, 环境光强度比例阈值,软光敏方案有效,彩色模式时,值越小灵敏度越高,黑白模式时,值越大灵敏度越高, range[0, 1000], default:500 */

    NI_U16 u16LedDiffR;    /* RW, LED色温判断模式下diff r [0, 256] default:40 */
    NI_U16 u16LedDiffB;    /* RW, LED色温判断模式下diff b [0, 256] default:40 */
    NI_U16 u16LedDiffEnvH; /* RW, LED亮度环境差异高阈值 [1000, 2000] default:1500 */
    NI_U16 u16LedDiffEnvL; /* RW, LED亮度环境差异低阈值 [0, 1000] default:500 */
} DN_PARAM_S;

typedef struct tagDnAttr {
    DN_METHOD_E enMethod;  /* RW, 方案选择 */
    DN_MODE_E   enDnMode;  /* RW, 日夜模式 */
    DN_PARAM_S  stDnParam; /* RW, 日夜判断参数 */
} DN_ATTR_S;

typedef struct tagDnStatisticsSw {
    NI_U32 u32PsValue;     /* 硬光敏模式下的adc值 range:[0, 1023] */
    NI_U32 u32SysGain;     /* 当前增益信息,单位DB * 1000 */
    NI_U32 s32SysMultGain; /* 当前增益信息,单位倍 * 1000 */
    NI_U32 u32SysSht;      /* 当前快门信息 */
    NI_U32 u32SysFps;      /* 帧率信息（原始帧率）单位：帧率*1000 */
    NI_S32 s32EvTarget;    /* 当前的目标亮度值 */
    NI_S32 s32EvCur;       /* 当前的图像亮度值 */

    NI_U16  u16GainR;  /* 当前环境WB的GainR值 */
    NI_U16  u16GainB;  /* 当前环境WB的GainB值 */
    NI_BOOL bAeStable; /* AE稳定状态 true false*/
} DN_STATISTICS_SW_S;

#define DN_STATISTICS_AWES_FORMAT_E HW_STATISTICS_AWES_FORMAT_E
#define DN_STATISTICS_POS_E         HW_STATISTICS_POS_E
#define DN_STATISTICS_AWES_S        HW_STATISTICS_AWES_S

typedef struct tagDnStatisticsHw {
    DN_STATISTICS_AWES_S stAwes; /* 见 DN_STATISTICS_AWES_S 中的描述 */
} DN_STATISTICS_HW_S;

typedef struct tagDnStatistics {
    DN_STATISTICS_HW_S stStatHw; /* 见 AE_STATISTICS_HW_S 中的描述 */
    DN_STATISTICS_SW_S stStatSw; /* 见 AE_STATISTICS_SW_S 中的描述 */
} DN_STATISTICS_S;

typedef struct tagDnResult {
    NI_BOOL bDayEn;   /* 日夜结果, 0表示晚上, 1表示白天 */
    NI_BOOL bIrCutEn; /* 滤光片结果, 0表示晚上, 1表示白天 */
    NI_U32  u32DnEnv; /* 当前环境的日夜状态信息, 1表示白天, 2表示晚上 */
} DN_RESULT_S;

typedef struct tagDnRecord {
    NI_BOOL bPreValid;   /* 标记记录保存的AE稳定后的数据是否有效 */
    NI_U32  u32PreGain;  /* 记录的AE稳定后的的增益信息 */
    NI_U32  u32PreSht;   /* 记录的AE稳定后的的快门信息 */
    NI_U32  u32PreEnv;   /* 记录的AE稳定后的的环境亮度信息 */
    NI_U16  u16PreGainR; /* 记录的AE稳定后的的RGain信息 */
    NI_U16  u16PreGainB; /* 记录的AE稳定后的的BGain信息 */
} DN_RECORD_S;

typedef struct tagDnDbgInfo {
    NI_U32 u32EnvLum;    /* RO, 当前环境亮度判断信息，u32EnvLum = 当前曝光*当前增益 单位：us*倍 */
    NI_U32 u32D2nEnvThr; /* RO, 白切黑亮度阈值， u32EnvLumThr 需大于此值才可以触发切换 u32D2nEnvThr = u32D2nGainThr * 当前原始帧率下的最大曝光值 单位：us*倍*/
    NI_U32 u32N2dEnvThr; /* RO, 黑切白亮度阈值， u32EnvLumThr 需小于此值才可以触发切换 u32N2dEnvThr = u32N2dGainThr * 当前原始帧率下的最大曝光值 单位：us*倍*/
    NI_U16 u16Gr;        /* RO, 当前画面Gr增益, 当此值u16Gr > u16GainRAvgThr 或 u16Gb > u16GainBAvgThr时，符合红外黑切白要求 */
    NI_U16 u16Gb;        /* RO, 当前画面Gb增益, 当此值u16Gr > u16GainRAvgThr 或 u16Gb > u16GainBAvgThr时，符合红外黑切白要求 */
    NI_U16 u16IrPerCent; /* RO, 当前画面红外占比，当此值小于配置中的u16IrPerCent时，符合红外黑切白要求 */

    DN_RECORD_S stDnRecord; /* RO, 灯切换前后 统计信息记录*/
} DN_DBG_INFO_S;

typedef struct tagDnInfo {
    NI_BOOL       bDayEn;    /* 日夜结果, 0表示晚上, 1表示白天 */
    NI_BOOL       bIrCutEn;  /* 滤光片结果, 0表示晚上, 1表示白天 */
    NI_U32        u32DnEnv;  /* 当前环境的日夜状态信息, 1表示白天, 2表示晚上 */
    DN_DBG_INFO_S stDbgInfo; /* 调试信息 */
} DN_INFO_S;

/********************** IRLIGHT ***********************************/

#define IR_LIGHT_MAX_LIGHT_NUM (AISP_MAX_LIGHT_NUM)

typedef enum tagIrLightMethod {
    IR_LIGHT_METHOD_SINGLE_FIX = 0, /* 单灯亮度固定 */
    IR_LIGHT_METHOD_SINGLE_VAR = 1, /* 单灯亮度自适应调整 */
    // IR_LIGHT_METHOD_NEAR_FAR         = 2,  /* 近光灯远光灯分别控制 */
    IR_LIGHT_METHOD_BUTT,
} IR_LIGHT_METHOD_E;

typedef enum tagIrLightMode {
    IR_LIGHT_MODE_AUTO   = 0, /* 自动补光 */
    IR_LIGHT_MODE_CLOSE  = 1, /* 关闭补光 */
    IR_LIGHT_RATE_PRIO   = 2, /* 倍率优先 目前不支持 */
    IR_LIGHT_MODE_MAUNAL = 3, /* 手动补光 */
    IR_LIGHT_MODE_BUTT,
} IR_LIGHT_MODE_E;

typedef struct tagIrLightParam {
    NI_U16 u16TargetIllum; /* RW, 目标照度, enMethod = IR_LIGHT_METHOD_SINGLE_VAR
                            * 且 enLightMode = IR_LIGHT_MODE_AUTO 时有效,
                            * range[0, 10000], default:180 */
    NI_U8 u8Tolerance;     /* RW, 调整到目标照度的容忍误差, enMethod = IR_LIGHT_METHOD_SINGLE_VAR
                            * 且 enLightMode = IR_LIGHT_MODE_AUTO 时有效,
                            * range[0, 255], default:50 */
    NI_U8 u8Speed;         /* RW, 调节速度, enMethod = IR_LIGHT_METHOD_SINGLE_VAR
                            * 且 enLightMode = IR_LIGHT_MODE_AUTO 时有效,
                            * range[0, 255], default:128 */
    NI_U16 u16LightMin;    /* RW, 补光灯开启触发点, 由补光的特性决定,
                            * 需要根据实际的补光灯进行测试后决定该值的大小
                            * range[3200, u16LightMax], default:3200 */
    NI_U16 u16LightMax;    /* RW, 补光灯开启饱和点,  由补光的特性决定,
                            * 需要根据实际的补光灯进行测试后决定该值的大小
                            * range[u16LightMin, 9000], default:8700 */
    NI_U32 u32SysGainMax;  /* RW, 增益的最大值, 表示当前系统可以达到的最大增益
                            * 当前版本无效
                            * range[0, 72000], default:72000 */
    NI_U32 u32SysShtMax;   /* RW, 快门的最大值, 表示当前系统可以达到的最大快门时间
                            * 当前版本无效
                            * range[0, 100000], default:40000*/
} IR_LIGHT_PARAM_S;

typedef struct tagIrLightAttr {
    IR_LIGHT_METHOD_E enMethod;    /* RW, 方案选择 */
    IR_LIGHT_MODE_E   enLightMode; /* RW, 补光模式 */
    NI_U8             u8Level;     /* RW, 补光灯强度设置,
                                    * enLightMode = IR_LIGHT_MODE_MAUNAL 时有效 或者
                                    * enMethod = IR_LIGHT_METHOD_SINGLE_FIX 时有效
                                    * range[0, 100], default:50 */
    IR_LIGHT_PARAM_S stLightParam; /* RW, 补光灯控制参数 */
} IR_LIGHT_ATTR_S;

typedef struct tagIrLightStatisticsSw {
    NI_U8  u8IsBlack;      /* 是否为黑白图像 0:表示图像为彩色, 1:表示图像为黑白 */
    NI_U8  u8DnEnv;        /* 当前的日夜环境 1:表示白天环境, 2:表示夜晚环境*/
    NI_U32 u32EnvLumValue; /* 当前环境亮度信息 */
    NI_U32 u32SysGain;     /* 当前增益信息 */
    NI_U32 u32SysSht;      /* 当前快门信息 */
    NI_S32 s32EvTarget;    /* 当前的目标亮度值 */
    NI_S32 s32EvCur;       /* 当前的图像亮度值 */
} IR_LIGHT_STATISTICS_SW_S;

#define IR_LIGHT_STATISTICS_AWES_FORMAT_E HW_STATISTICS_AWES_FORMAT_E
#define IR_LIGHT_STATISTICS_POS_E         HW_STATISTICS_POS_E
#define IR_LIGHT_STATISTICS_AWES_S        HW_STATISTICS_AWES_S

typedef struct tagIrLightStatisticsHw {
    IR_LIGHT_STATISTICS_AWES_S stAwes; /* 见 IR_LIGHT_STATISTICS_AWES_S 中的描述 */
} IR_LIGHT_STATISTICS_HW_S;

typedef struct tagIrLightStatistics {
    IR_LIGHT_STATISTICS_HW_S stStatHw; /* 见 AE_STATISTICS_HW_S 中的描述 */
    IR_LIGHT_STATISTICS_SW_S stStatSw; /* 见 AE_STATISTICS_SW_S 中的描述 */
} IR_LIGHT_STATISTICS_S;

typedef struct tagIrLightResult {
    NI_U32 au32LightLevel[IR_LIGHT_MAX_LIGHT_NUM]; /* 补光灯强度 */
} IR_LIGHT_RESULT_S;

typedef struct tagIrLightInfo {
    NI_U32 au32LightLevel[IR_LIGHT_MAX_LIGHT_NUM]; /* 补光灯强度 */
    NI_U32 u32Kp;
    NI_U32 u32Ki;
    NI_U32 u32Kd;
    NI_S32 as32Delta[3];
} IR_LIGHT_INFO_S;

/********************** LEDLIGHT ***********************************/

#define LED_LIGHT_MAX_LIGHT_NUM (AISP_MAX_LIGHT_NUM)

typedef enum tagLedLightMethod {
    LED_LIGHT_METHOD_SINGLE_FIX = 0, /* 单灯亮度固定 */
    LED_LIGHT_METHOD_SINGLE_VAR = 1, /* 单灯亮度自适应调整 */
    // LED_LIGHT_METHOD_NEAR_FAR         = 2,  /* 近光灯远光灯分别控制 */
    LED_LIGHT_METHOD_BUTT,
} LED_LIGHT_METHOD_E;

typedef enum tagLedLightMode {
    LED_LIGHT_MODE_AUTO   = 0, /* 自动补光 */
    LED_LIGHT_MODE_CLOSE  = 1, /* 关闭补光 */
    LED_LIGHT_RATE_PRIO   = 2, /* 倍率优先 目前不支持 */
    LED_LIGHT_MODE_MAUNAL = 3, /* 手动补光 */
    LED_LIGHT_MODE_BUTT,
} LED_LIGHT_MODE_E;

typedef struct tagLedLightParam {
    NI_U16 u16TargetIllum; /* RW, 目标照度, enMethod = LED_LIGHT_METHOD_SINGLE_VAR
                            * 且 enLightMode = LED_LIGHT_MODE_AUTO 时有效,
                            * range[0, 10000], default:180 */
    NI_U8 u8Tolerance;     /* RW, 调整到目标照度的容忍误差, enMethod = LED_LIGHT_METHOD_SINGLE_VAR
                            * 且 enLightMode = LED_LIGHT_MODE_AUTO 时有效,
                            * range[0, 255], default:50 */
    NI_U8 u8Speed;         /* RW, 调节速度, enMethod = LED_LIGHT_METHOD_SINGLE_VAR
                            * 且 enLightMode = LED_LIGHT_MODE_AUTO 时有效,
                            * range[0, 255], default:128 */
    NI_U16 u16LightMin;    /* RW, 补光灯开启触发点, 由补光的特性决定,
                            * 需要根据实际的补光灯进行测试后决定该值的大小
                            * range[3200, u16LightMax], default:3200 */
    NI_U16 u16LightMax;    /* RW, 补光灯开启饱和点,  由补光的特性决定,
                            * 需要根据实际的补光灯进行测试后决定该值的大小
                            * range[u16LightMin, 9000], default:8700 */
    NI_U32 u32SysGainMax;  /* RW, 增益的最大值, 表示当前系统可以达到的最大增益
                            * 当前版本无效
                            * range[0, 72000], default:72000 */
    NI_U32 u32SysShtMax;   /* RW, 快门的最大值, 表示当前系统可以达到的最大快门时间
                            * 当前版本无效
                            * range[0, 100000], default:40000*/
} LED_LIGHT_PARAM_S;

typedef struct tagLedLightAttr {
    LED_LIGHT_METHOD_E enMethod;    /* RW, 方案选择 */
    LED_LIGHT_MODE_E   enLightMode; /* RW, 补光模式 */
    NI_U8              u8Level;     /* RW, 补光灯强度设置,
                                     * enLightMode = LED_LIGHT_MODE_MAUNAL 时有效 或者
                                     * enMethod = IR_LIGHT_METHOD_SINGLE_FIX 时有效
                                     * range[0, 100], default:50 */
    LED_LIGHT_PARAM_S stLightParam; /* RW, 白光灯控制参数 */
} LED_LIGHT_ATTR_S;

typedef struct tagLedLightStatisticsSw {
    NI_U8  u8IsBlack;      /* 是否为黑白图像 0:表示图像为彩色, 1:表示图像为黑白 */
    NI_U8  u8DnEnv;        /* 当前的日夜环境 1:表示白天环境, 2:表示夜晚环境*/
    NI_U32 u32EnvLumValue; /* 当前环境亮度信息 */
    NI_U32 u32SysGain;     /* 当前增益信息 */
    NI_U32 u32SysSht;      /* 当前快门信息 */
    NI_S32 s32EvTarget;    /* 当前的目标亮度值 */
    NI_S32 s32EvCur;       /* 当前的图像亮度值 */
} LED_LIGHT_STATISTICS_SW_S;

#define LED_LIGHT_STATISTICS_AWES_FORMAT_E HW_STATISTICS_AWES_FORMAT_E
#define LED_LIGHT_STATISTICS_POS_E         HW_STATISTICS_POS_E
#define LED_LIGHT_STATISTICS_AWES_S        HW_STATISTICS_AWES_S

typedef struct tagLedLightStatisticsHw {
    LED_LIGHT_STATISTICS_AWES_S stAwes; /* 见 LED_LIGHT_STATISTICS_AWES_S 中的描述 */
} LED_LIGHT_STATISTICS_HW_S;

typedef struct tagLedLightStatistics {
    LED_LIGHT_STATISTICS_HW_S stStatHw; /* 见 AE_STATISTICS_HW_S 中的描述 */
    LED_LIGHT_STATISTICS_SW_S stStatSw; /* 见 AE_STATISTICS_SW_S 中的描述 */
} LED_LIGHT_STATISTICS_S;

typedef struct tagLedLightResult {
    NI_U32 au32LightLevel[LED_LIGHT_MAX_LIGHT_NUM]; /* 补光灯强度 */
} LED_LIGHT_RESULT_S;

typedef struct tagLedLightInfo {
    NI_U32 au32LightLevel[LED_LIGHT_MAX_LIGHT_NUM]; /* 补光灯强度 */
    NI_U32 u32Kp;
    NI_U32 u32Ki;
    NI_U32 u32Kd;
    NI_S32 as32Delta[3];
} LED_LIGHT_INFO_S;

/********************** RGB2YUV ***********************************/

typedef struct tagRgb2YuvMatrix {
    NI_F32 af32Cof[3][3]; /* RW, RGB2YUV转换矩阵 */
    NI_S32 s32OfstY;      /* RW, Y Offset */
    NI_S32 s32OfstCb;     /* RW, Cb Offset */
    NI_S32 s32OfstCr;     /* RW, Cr Offset */
} RGB2YUV_MATRIX_S;

typedef struct tagRgb2YuvColor {
    NI_U8 u8Brt; /*RW, 亮度调整参数, range[0, 100], default:50 */
    NI_U8 u8Ctr; /*RW, 对比度调整参数, range[0, 100], default:50 */
    NI_U8 u8Sat; /*RW, 饱和度调整参数, range[0, 100], default:50 */
    NI_U8 u8Hue; /*RW, 色调调整参数, range[0, 100], default:50 */
} RGB2YUV_COLOR_S;

typedef struct tagRgb2Yuvparam {
    RGB2YUV_COLOR_S  stColor;  /*RW, RGB2YUV调整参数 */
    RGB2YUV_MATRIX_S stMatrix; /*RW, RGB2YUV矩阵 */
} RGB2YUV_PARAM_S;

/********************** YHISTCTR ***********************************/

#define YHIST_CTR_MAX_DB_NUM         (AISP_MAX_DB_NUM) /* 最大支持的DB个数, 最大的DB增益为 AE_MAX_DB_NUM * 6 */
#define YHIST_CTR_STATISTICS_YHIST_S HW_STATISTICS_YHIST_S

typedef struct tagYhistCtrStatisticsHw {
    YHIST_CTR_STATISTICS_YHIST_S stYhist; /* 见 YHIST_CTR_STATISTICS_YHIST_S 中的描述 */
} YHIST_CTR_STATISTICS_HW_S;

typedef struct tagYhistCtrStatisticsSw {
    NI_U32 u32SysGain; /* 当前增益信息,单位DB * 1000 */
} YHIST_CTR_STATISTICS_SW_S;

typedef struct tagYhistCtrStatistics {
    YHIST_CTR_STATISTICS_HW_S stStatHw; /* 见 YHIST_CTR_STATISTICS_HW_S 中的描述 */
    YHIST_CTR_STATISTICS_SW_S stStatSw; /* 见 YHIST_CTR_STATISTICS_SW_S 中的描述 */
} YHIST_CTR_STATISTICS_S;

typedef struct tagYhistCtrCurve {
    NI_U16 au16Thr[5];   /*阈值，range(0,255)*/
    NI_U32 au32Ratio[5]; /*强度*/
} YHIST_CTR_CURVE_S;

typedef struct tagYhistCtrThrCalcParam {
    NI_U8             u8Mode;         /*0:手动模式; 1:自动模式*/
    NI_U32            u32Thr;         /*手动设置的thr值，即截断阈值*/
    NI_U32            u32DrCalcRatio; /*自动模式下用于计算动态范围的上下限像素比例,该参数表示比例*/
    YHIST_CTR_CURVE_S stThresh;       /*根据u32DrCalcRatio算出的上下限像素灰度差值来计算截断阈值*/
} YHIST_CTR_THR_CALC_PARAM;

typedef struct tagYhistCtrAdjParam {
    NI_U32 u32AdjThresh; /*调节阈值*/
    NI_U8  u8AdjSpeed;   /*调节速度,1--16*/
} YHIST_CTR_ADJ_PARAM_S;

typedef struct tagYhistCtrMapParam {
    NI_U8 u8StartValue; /*起始映射值,range(0,255)*/
    NI_U8 u8EndValue;   /*截止映射值,range(0,255)*/
} YHIST_CTR_MAP_PARAM_S;

typedef struct tagYhistCtrLumCurveParam {
    NI_U8 u8Th1;    /*阈值u8Th1,range(0,255)*/
    NI_U8 u8Th2;    /*阈值u8Th2,u8Th1 < u8Th2,range(0,255)*/
    NI_U8 u8Th3;    /*阈值u8Th3,u8Th2 < u8Th3,range(0,255)*/
    NI_U8 u8Th4;    /*阈值u8Th4,u8Th3 < u8Th4, u8Th4 < 255, range(0,255)*/
    NI_U8 u8Ratio1; /*低亮强度,range(0,16)*/
    NI_U8 u8Ratio2; /*中亮强度,range(0,16)*/
    NI_U8 u8Ratio3; /*高亮强度,range(0,16)*/
} YHIST_STR_LUM_CURVE_PARAM_S;

typedef struct tagYhistCtrParam {
    YHIST_CTR_ADJ_PARAM_S       stYhistCtrAdjParam;                        /*调节参数*/
    NI_U8                       au8GlbStr[AISP_MAX_DB_NUM];                /*整体强度参数，range(0,255), AISP_MAX_DB_NUM*/
    YHIST_STR_LUM_CURVE_PARAM_S astYHistStrLumCurveParam[AISP_MAX_DB_NUM]; /*不同亮度下的强度, AISP_MAX_DB_NUM*/
    YHIST_CTR_MAP_PARAM_S       astYhistCtrMapParam[YHIST_CTR_MAX_DB_NUM]; /*映射参数, AISP_MAX_DB_NUM*/
    YHIST_CTR_THR_CALC_PARAM    stYhistCtrThrCalcParam;                    /*阈值计算参数*/
} YHIST_CTR_PARAM_S;

typedef struct tagYhistCtrResult {
    NI_U16 u16avgLumY;   /*平均亮度值*/
    NI_U16 u16bitDepth;  /*数据位深*/
    NI_U16 u16tabLength; /*表的长度*/
    NI_U8 *pu8CtrTab;    /*亮度映射表*/
} YHIST_CTR_RESULT_S;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
