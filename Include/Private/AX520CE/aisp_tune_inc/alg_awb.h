/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _ALG_AWB_H_
#define _ALG_AWB_H_

#include "ni_type.h"
#include "ni_comm_awb.h"
#include "ni_comm_awb_inner.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define AWB_LOCK_INIT(IspDev)   COMPAT_LOCK_INIT(IspDev)
#define AWB_LOCK_DEINIT(IspDev) COMPAT_LOCK_DEINIT(IspDev)
#define AWB_LOCK(IspDev)        COMPAT_LOCK(IspDev)
#define AWB_UNLOCK(IspDev)      COMPAT_UNLOCK(IspDev)

#define AWB_CTX_S               COMPAT_CTX_S
#define AWB_MEM_INFO_S          COMPAT_MEM_INFO_S
#define AWB_GET_DEV_CTX(IspDev) COMPAT_GET_DEV_CTX(IspDev, AWB)

#define AWB_MEM_INIT(IspDev, pstCtx)   COMPAT_MEM_INIT(IspDev, AWB, pstCtx)
#define AWB_MEM_DEINIT(IspDev, pstCtx) COMPAT_MEM_DEINIT(IspDev, AWB, pstCtx)
#define AWB_MEM_MAP(IspDev, pstCtx)    COMPAT_MEM_MAP(IspDev, AWB, pstCtx)
#define AWB_MEM_UMAP(IspDev, pstCtx)   COMPAT_MEM_UMAP(IspDev, AWB, pstCtx)

#define AWB_TRACE COMPAT_TRACE
#define AWB_DBG   COMPAT_DBG

#define AWB_AWES_BLK_DATA_S   COMPAT_AWES_BLK_DATA_S
#define AWB_AWES_DATA_CHN_R   COMPAT_AWES_DATA_CHN_R /* AWES统计块的 R 通道 */
#define AWB_AWES_DATA_CHN_G   COMPAT_AWES_DATA_CHN_G /* AWES统计块的 G 通道 */
#define AWB_AWES_DATA_CHN_B   COMPAT_AWES_DATA_CHN_B /* AWES统计块的 B 通道 */
#define AWB_AWES_DATA_CHN_Y   COMPAT_AWES_DATA_CHN_Y /* AWES统计块的 Y 通道 */
#define AWB_AWES_DATA_CHN_NUM COMPAT_AWES_DATA_CHN_NUM
#define AWB_AWES_DATA_CHN_E   COMPAT_AWES_DATA_CHN_E

// #define AWB_MOOTH_FRAME_NUM         (32) /* WB结果滤波缓存帧数 */
#define AWB_CURVE_GAIN_ACC      (64)                                          /* 色温曲线中色温点步长大小 */
#define AWB_CURVE_GAIN_MAX      (AWB_CURVE_GAIN_ACC * AWB_CURVE_BIN_NUM)      /* 步长 * 点数 = 最大Gain */
#define AWB_DEF_LS_WT_SENCE_NUM (4)                                           /* 环境亮度阈值节点个数 */
#define AWB_LS_WT_MAP_BIN       (AWB_LS_TYPE_NUM + AWB_SPECIAL_LIGHT_SRC_NUM) /* 色温点最大总个数 标准光源点个数 + 特殊光源点个数 */

#define AWB_HIST_ACC      (64)                            /* 色温统计分块权重分块步长大小 */
#define AWB_HIST_BIN      (40)                            /* 色温统计分块权重R/B方向分块个数 */
#define AWB_HIST_GAIN_MAX (AWB_HIST_ACC * AWB_HIST_BIN)   /* 步长 * 点数 = 最大Gain */
#define AWB_HIST_BUF_SIZE (NI_ALIGN8(sizeof(AWB_HIST_S))) /* 色温统计分块需要的内存大小 */

#define AWB_CLUSTER_NUM (20)

#define AWB_MAP_BIN      (AWB_HIST_BIN * 2)
#define AWB_MAP_ACC      (AWB_HIST_ACC / 2)
#define AWB_MAP_BUF_SIZE (sizeof(AWB_WT_MAP_S))

#define AWB_TOTAL_BUF_SIZE (AWB_HIST_BUF_SIZE + AWB_MAP_BUF_SIZE)

#define AWB_SLS_RATIO_NUM  (4)
#define AWB_NRLS_RATIO_NUM (5)

typedef struct tagAlgAwbHist {
    NI_U16 au16HistBuf[AWB_HIST_BIN][AWB_HIST_BIN];
    NI_U16 u16ValidNum;
    NI_U16 u16TotalNum;
} AWB_HIST_S;

typedef struct tagAlgAwbWtMap {
    NI_U8 au8WtBuf[AWB_MAP_BIN][AWB_MAP_BIN];
} AWB_WT_MAP_S;

typedef struct tagAwbWhiteBlkInfo {
    NI_U32 u32SumR;     /* 白块的 R 通道总和 */
    NI_U32 u32SumG;     /* 白块的 G 通道总和 */
    NI_U32 u32SumB;     /* 白块的 B 通道总和 */
    NI_U32 u32SumY;     /* 白块的 Y 通道总和 */
    NI_U32 u32ValidCnt; /* 白块的个数 */
} AWB_WHITE_BLK_INFO_S;

// typedef struct tagAwbSmooth
// {
//     NI_BOOL   bFull;            /* 标记 au16GainR/B 数组是否写满 */
//     NI_U16    au16GainR[AWB_MOOTH_FRAME_NUM]; /* 记录历史的GainR信息 */
//     NI_U16    au16GainB[AWB_MOOTH_FRAME_NUM]; /* 记录历史的GainB信息 */
//     NI_U16    u16Pointer;       /* 最近一次的GainR/B数据在数组中的索引 */
//     NI_U32    u32SumR;  /* 记录的历史GainR的总和 */
//     NI_U32    u32SumB;  /* 记录的历史GainB的总和 */
//     NI_U16    u16Rgain; /* 滤波后的RGain */
//     NI_U16    u16Bgain; /* 滤波后的BGain */
// }AWB_SMOOTH_S;

// typedef struct tagAwbLsHist
// {
//     NI_U32     au32LsHistBuf[AWB_LS_TYPE_NUM]; /* 白块在各个标准光源点占比统计 */
//     NI_U16     u16ValidCnt; /* 有效白块总个数 */
// }AWB_LS_HIST_S;

// typedef struct tagAwbDefLsWt
// {
//     NI_U8  au8DefLsWt[AWB_DEF_LS_WT_SENCE_NUM][AWB_LS_TYPE_NUM]; /* 各个标准光源点权重系数纵坐标 */
//     NI_U16 au16EnvLumThr[AWB_DEF_LS_WT_SENCE_NUM]; /* 标准光源点权重系数横坐标 */
// }AWB_DEF_LS_WT_S;

typedef struct tagAwbLs {
    NI_U8 u8X;
    NI_U8 u8Y;
    NI_U8 u8Percent;
    NI_U8 u8Id;
} AWB_LS_S;

typedef struct tagAlgAwbCluster {
    AWB_LS_S astLs[AWB_CLUSTER_NUM];
    NI_U16   u16ValidNum;
    NI_U16   u16Sum;
} AWB_CLUSTER_S;

// typedef struct tagAwbLsWtMapInfo
// {
//     NI_U16      u16GainR;  /* 光源的RGain */
//     NI_U16      u16GainB;  /* 光源的RGain */
//     NI_U16      u16LsWt;   /* 光源权重系数 */
// }AWB_LS_WT_MAP_INFO_S;

typedef struct tagAwbCtEvlThr {
    NI_U16 u16ThrLowR;
    NI_U16 u16ThrHigR;
    NI_U16 u16ThrLowB;
    NI_U16 u16ThrHigB;
} AWB_CT_EVL_THR_S;

typedef struct tagAwbCtEvlAttr {
    AWB_CT_EVL_THR_S stLowCt;
    AWB_CT_EVL_THR_S stHigCt;
} AWB_CT_EVL_ATTR_S;

typedef struct tagAwbCtEvl {
    NI_U32 u32SumLow;
    NI_U32 u32SumHig;
    NI_U32 u32Cnt;
} AWB_CT_EVL_S;

typedef struct tagAwbSlsCalcInfo {
    NI_U8 u8StartX;
    NI_U8 u8StartY;
    NI_U8 u8EndX;
    NI_U8 u8EndY;
    NI_U8 u8CenX;
    NI_U8 u8CenY;
    NI_U8 u8Ratio;
    NI_U8 au8Rev[1];
} AWB_SLS_CALC_INFO_S;

typedef struct tagAwbLsCalcInfo {
    NI_U8 u8CenX;
    NI_U8 u8CenY;
    NI_U8 u8Ratio;
    NI_U8 au8Rev[1];
} AWB_LS_CALC_INFO_S;

typedef struct tagAwbVreg {
    NI_BOOL bModify;  /* 参数更新标记 */
    NI_BOOL bDebugEn; /* 打印输出开关 */

    AWB_ATTR_S        stAwbAttr;                        /* 算法参数 */
    AWB_REGION_ATTR_S stAwbRegionAttr;                  /* 区域白平衡算法参数 */
    AWB_CT_EVL_ATTR_S stCtEvlAttr;                      /* 色温估计计算参数 内部固定,不对外开放 */
    NI_U8             au8LsSpWtRto[AWB_NRLS_RATIO_NUM]; /* 相对标准光源点偏移的权重, 横坐标为偏移距离 */
    NI_U8             au8SlsSpWtRto[AWB_SLS_RATIO_NUM]; /* 特殊光源点偏移权重, 横坐标为偏移距离 */

    AWB_SMOOTH_S stSmooth; /* 滤波迭代数据 */

#if 1                                                               /* 每帧都会重新计算, 后续考虑是否可以通过公共缓存分配 */
    AWB_WHITE_BLK_INFO_S stWhiteBlkSum;                             /* 所有白块累加信息 每帧先清0 */
    AWB_CT_EVL_S         stCtEvl;                                   /* 色温估计 0:256 -> 0:1 */
    AWB_SLS_CALC_INFO_S  astSpLightInfo[AWB_SPECIAL_LIGHT_SRC_NUM]; /* 特殊光源点信息 */
    AWB_LS_CALC_INFO_S   astNrLightInfo[AWB_LS_TYPE_NUM];           /* 标准光源点信息 */
#endif

    AWB_RESULT_S stAwbResultCur; /* 当前帧AWB算法结果 */
    AWB_RESULT_S stAwbResult;    /* 滤波后的AWB算法结果 */

    NI_U16 u16TC;       /* 当前的TC均值 */
    NI_U16 u16GainR;    /* 当前的GainR均值 */
    NI_U16 u16GainB;    /* 当前的GainB均值 */
    NI_U32 u32LowCtPer; /* 低色温占比 */
    NI_U32 u32HigCtPer; /* 高色温占比 */
} AWB_VREG_S;

NI_S32 AWB_CAL_CalcRegionWb(AWB_VREG_S       *pstVreg,
                            AWB_STATISTICS_S *pstStat,
                            AWB_RESULT_S     *pstResult);
NI_S32 AWB_CAL_CalcWb(AWB_VREG_S       *pstVreg,
                      AWB_STATISTICS_S *pstStat,
                      AWB_RESULT_S     *pstResult);
NI_S32 AWB_CAL_Init(AWB_VREG_S *pstVreg);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
