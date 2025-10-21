/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _ALG_AE_H__
#define _ALG_AE_H__

#include "compat_cal.h"

#include "ni_comm_ae.h"
#include "ni_comm_ae_inner.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define AE_WGT_DEBUG_PRT(fmt, ...)  // printf(RED fmt NONE_C, ##__VA_ARGS__)
#define AE_EV_DEBUG_PRT(fmt, ...)   // printf(PURPLE fmt NONE_C, ##__VA_ARGS__)
#define AE_ADJ_DEBUG_PRT(fmt, ...)  // printf(YELLOW fmt NONE_C, ##__VA_ARGS__)
#define AE_RES_DEBUG_PRT(fmt, ...)  // printf(GREEN fmt NONE_C, ##__VA_ARGS__)

#define AE_LOCK_INIT(IspDev)   COMPAT_LOCK_INIT(IspDev)
#define AE_LOCK_DEINIT(IspDev) COMPAT_LOCK_DEINIT(IspDev)
#define AE_LOCK(IspDev)        COMPAT_LOCK(IspDev)
#define AE_UNLOCK(IspDev)      COMPAT_UNLOCK(IspDev)

#define AE_CTX_S               COMPAT_CTX_S
#define AE_MEM_INFO_S          COMPAT_MEM_INFO_S
#define AE_GET_DEV_CTX(IspDev) COMPAT_GET_DEV_CTX(IspDev, AE)

#define AE_MEM_INIT(IspDev, pstCtx)   COMPAT_MEM_INIT(IspDev, AE, pstCtx)
#define AE_MEM_DEINIT(IspDev, pstCtx) COMPAT_MEM_DEINIT(IspDev, AE, pstCtx)
#define AE_MEM_MAP(IspDev, pstCtx)    COMPAT_MEM_MAP(IspDev, AE, pstCtx)
#define AE_MEM_UMAP(IspDev, pstCtx)   COMPAT_MEM_UMAP(IspDev, AE, pstCtx)

#define AE_TRACE COMPAT_TRACE
#define AE_DBG   COMPAT_DBG
#define AE_PRINT COMPAT_PRINT

#define AE_CalcDbToMul(u32DbGain) COMPAT_CalcDbToMul((u32DbGain))

#define AE_AWES_BLK_DATA_S   COMPAT_AWES_BLK_DATA_S
#define AE_AWES_DATA_CHN_R   COMPAT_AWES_DATA_CHN_R /* AWES统计块的 R 通道 */
#define AE_AWES_DATA_CHN_G   COMPAT_AWES_DATA_CHN_G /* AWES统计块的 G 通道 */
#define AE_AWES_DATA_CHN_B   COMPAT_AWES_DATA_CHN_B /* AWES统计块的 B 通道 */
#define AE_AWES_DATA_CHN_Y   COMPAT_AWES_DATA_CHN_Y /* AWES统计块的 Y 通道 */
#define AE_AWES_DATA_CHN_NUM COMPAT_AWES_DATA_CHN_NUM
#define AE_AWES_DATA_CHN_E   COMPAT_AWES_DATA_CHN_E

#define AE_LINEAR_INTERPOLATION(x, x1, x2, y1, y2) (((int)(y2) - (int)(y1)) * ((int)(x) - (int)(x1)) / ((int)(x2) - (int)(x1)) + (int)(y1))

#define AE_WT_CURVE_NODE_NUM (5) /* 权重计算曲线节点个数 */

// calc_ev
#define AE_ENV_MIN_VAL  (1)       /* 快门 * 增益 最小值 */
#define AE_ENV_MAX_VAL  (1000000) /* 快门 * 增益 最大值 */
#define AE_ENV_BASE_WGT (16)

#define AE_MODE_BASE_WGT  (16)
#define AE_BLC_SMOOTH_VAL (20)
#define AE_HLC_SMOOTH_VAL (80)

#define AE_MAX_WGT_VAL        (255)
#define AE_TAR_RATIO_BASE_VAL (1000)
// adj
#define AE_SEN_DELAY_FRAME (3)
#define AE_STEP_ACCURACY   (10000)

#define AE_STB_THR          (10)
#define AE_FLICK_CYCLE_50HZ (10000)
#define AE_FLICK_CYCLE_60HZ (8333)
#define AE_GAIN_6DB         (6000)
#define AE_GAIN_12DB        (12000)
#define AE_GAINPRO_GTHR     (10000)
#define AE_GAINPRO_STHR     (10000)

#define AE_CHECK_ZERO(num) ((num) == 0 ? 1 : (num))

typedef struct taAeWtCurveU16 {
    NI_U16 au16Thr[AE_WT_CURVE_NODE_NUM];   /* 权重阈值, 横坐标 */
    NI_U16 au16Ratio[AE_WT_CURVE_NODE_NUM]; /* 权重系数, 纵坐标 */
} AE_WT_CURVE_U16_S;

typedef struct tagAeAdjRatio {
    AE_WT_CURVE_U16_S stGainUpAdjRatio;   /* 增益向上调整(图像欠曝)计算系数曲线 */
    AE_WT_CURVE_U16_S stGainDownAdjRatio; /* 增益向下调整(图像过曝)计算系数曲线 */
    AE_WT_CURVE_U16_S stGainAdjClip;      /* 增益调整步长限幅计算系数曲线 */
    AE_WT_CURVE_U16_S stShtUpAdjRatio;    /* 快门向上调整(图像欠曝)计算系数曲线 */
    AE_WT_CURVE_U16_S stShtDownAdjRatio;  /* 快门向下调整(图像过曝)计算系数曲线 */
    AE_WT_CURVE_U16_S stShtAdjClip;       /* 快门调整步长限幅计算系数曲线 */
} AE_ADJ_RATIO_S;

typedef struct tagAeStableJudge {
    NI_U32 u32CntStb2AdjB; /* 图像欠曝从静止到调节计数,用于滤波 */
    NI_U32 u32CntStb2AdjW; /* 图像过曝从静止到调节计数,用于滤波 */
    NI_U32 u32AdjCount;    /* 图像调整步数计数 */
    NI_U32 u32RunCount;    /* 运行总计数 */
} AE_STABLE_JUDGE_S;

typedef struct tagAeAdjStep {
    NI_S32 s32GainStep;  /* 增益调整步长 */
    NI_S32 s32ShtStep;   /* 快门调整步长 */
    NI_S32 s32ApertStep; /* 光圈调整步长 */
} AE_ADJ_STEP_S;

typedef struct tagAeAdjInfo {
    AE_EXP_STATUS_E   enExpGainState;                  /* 当前曝光状态（最大，最小，正常）*/
    AE_ADJ_STATUS_E   enAdjStat;                       /* 调节标志, 用于黑白帧延时后置位 */
    AE_STABLE_JUDGE_S stStableJudge;                   /* 稳定/调整状态滤波计数 */
    NI_U32            u32CurLine;                      /* 当前曝光路径行数 */
    AE_ADJ_STEP_S     stAdjStep;                       /* 曝光、增益、光圈调节步长 */
    NI_U32            au32PreStep[AE_SEN_DELAY_FRAME]; /*n+4生效 统计生效前三帧的调节增益DB */
} AE_ADJ_INFO_S;

typedef struct tagAeVreg {
    NI_BOOL bModify;  /* 参数更新标记 */
    NI_BOOL bDebugEn; /* 打印输出开关 */

    /* 算法参数对外开放部分 */
    AE_ATTR_S        stAeAttr;   /* 算法参数, 曝光参数 */
    AE_ROUTE_S       stAeRoute;  /* 算法参数, 曝光路径 */
    AE_REGION_ATTR_S stAeRegion; /* 算法参数, 区域曝光参数 */
    AE_WIN_WT_S      stAeWinWt;  /* 算法参数, 曝光统计的区域权重 */

    /* 算法参数不对外开放部分 */
    AE_ADJ_RATIO_S stAdjRatio; /* 算法参数, 曝光调整步长比例计算参数 */

    /* 内部计算结果状态信息不对外开放部分 */
    AE_EV_ESTIMATE_S stEvEstimate; /* 曝光评估信息 */
    AE_ADJ_INFO_S    stAdjInfo;    /* 曝光调节信息 */

    AE_RESULT_S stAeResult; /* 算法结果 */
} AE_VREG_S;

NI_VOID AE_CAL_ExpConvert(AE_VREG_S *pstVreg);
NI_S32  AE_CAL_Init(AE_VREG_S *pstVreg);

NI_S32  AE_CAL_ProcessEvTarget(AE_VREG_S *pstVreg, AE_STATISTICS_S *pstStat);
NI_VOID AE_CAL_AdjStatusClear(AE_ADJ_INFO_S *pstAdjInfo);
NI_S32  AE_CAL_DetectAdj(AE_VREG_S *pstVreg, AE_ATTR_S *pstAeAttr);
NI_S32  AE_CAL_CalcExpStep(AE_VREG_S *pstVreg);
NI_S32  AE_CAL_ExpGainAdj(AE_VREG_S *pstVreg);

NI_S32 AE_COMM_255To1023(NI_S32 s32Ev);
NI_S32 AE_COMM_Log10(NI_S32 s32Ev);
NI_S32 AE_COMM_CalcDbValueSign(NI_S32 s32Slope, NI_S32 s32ValueLow, NI_S32 s32ValueHig);
NI_U16 AE_COMM_CalcWtU16(NI_S32 s32Input, AE_WT_CURVE_U16_S *pstWt);
NI_S32 AE_COMM_ConvertDB2Mul(NI_U32 u32Db);
NI_S32 AE_COMM_CheckRange(NI_S32 s32Start, NI_S32 s32End, NI_S32 s32Data, NI_CHAR *pcName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
