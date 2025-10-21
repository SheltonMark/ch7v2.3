/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _ALG_YHIST_CTR_H__
#define _ALG_YHIST_CTR_H__

#include "ni_type.h"

// #include "compat_cal.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define YHIST_CTR_LOCK_INIT(IspDev)   COMPAT_LOCK_INIT(IspDev)
#define YHIST_CTR_LOCK_DEINIT(IspDev) COMPAT_LOCK_DEINIT(IspDev)
#define YHIST_CTR_LOCK(IspDev)        COMPAT_LOCK(IspDev)
#define YHIST_CTR_UNLOCK(IspDev)      COMPAT_UNLOCK(IspDev)

#define YHIST_CTR_CTX_S               COMPAT_CTX_S
#define YHIST_CTR_MEM_INFO_S          COMPAT_MEM_INFO_S
#define YHIST_CTR_GET_DEV_CTX(IspDev) COMPAT_GET_DEV_CTX(IspDev, YHIST)

#define YHIST_CTR_MEM_INIT(IspDev, pstCtx)   COMPAT_MEM_INIT(IspDev, YHIST, pstCtx)
#define YHIST_CTR_MEM_DEINIT(IspDev, pstCtx) COMPAT_MEM_DEINIT(IspDev, YHIST, pstCtx)
#define YHIST_CTR_MEM_MAP(IspDev, pstCtx)    COMPAT_MEM_MAP(IspDev, YHIST, pstCtx)
#define YHIST_CTR_MEM_UMAP(IspDev, pstCtx)   COMPAT_MEM_UMAP(IspDev, YHIST, pstCtx)

#define YHIST_CTR_TRACE COMPAT_TRACE
#define YHIST_CTR_DBG   COMPAT_DBG

/*以下两个宏分别为直方图分段数，以及每个分段的灰度级数，显然相乘应该为256*/
#define YHIST_SECT_NUM        (16)
#define YHIST_SECT_IND_NUM    (16)
#define YHIST_TAB_DIFF_THRESH (5)

typedef struct tagYhistCtrVerg {
    NI_BOOL               bModify;                          /* 参数更新标记 */
    NI_BOOL               bDebugEn;                         /* 打印输出开关 */
    NI_BOOL               bStableFlag;                      /*调节稳定标志*/
    NI_U32                u32CurGain;                       /*当前增益*/
    NI_U8                 au8DestYhistTab[256];             /*目标映射表*/
    NI_U8                 au8CurYhistTab[256];              /*当前映射表*/
    NI_U32                au32YhistSectPre[YHIST_SECT_NUM]; /*分段直方图*/
    YHIST_CTR_PARAM_S     stAlg;                            /*算法参数*/
    YHIST_CTR_MAP_PARAM_S stCurMapParam;                    /*当前增益下，映射参数*/
    NI_U8                 au8CurLumRatio[256];              /*当前增益下，不同亮度对应的强度*/
} YHIST_CTR_VREG_S;

NI_S32                YHIST_CTR_CAL_Init(YHIST_CTR_VREG_S *pstVreg);
YHIST_CTR_MAP_PARAM_S ALG_YHIST_CTR_MAP_Calc(ISP_DEV IspDev, NI_U32 u32CurGain, YHIST_CTR_VREG_S *pstVreg);
NI_S32                ALG_YHIST_CTR_Calc(ISP_DEV IspDev, NI_U32 *pu32YhistData, YHIST_CTR_VREG_S *pstVreg);
NI_S32                ALG_YHIST_CTR_HIST_SECT_STA(ISP_DEV IspDev, NI_U32 *pu32YhistData, YHIST_CTR_VREG_S *pstVreg, NI_U32 *pu32HistVar);
NI_S32                ALG_YHIST_CTR_AdjByStep(ISP_DEV IspDev, YHIST_CTR_VREG_S *pstVreg);
NI_S32                ALG_YHIST_CTR_CalcAvgLumY(ISP_DEV IspDev, NI_U32 *pu32YhistData, YHIST_CTR_VREG_S *pstVreg, NI_U16 *pu16avgLumY);
NI_S32                YHIST_CTR_CAL_SaveVreg(YHIST_CTR_VREG_S *pstVreg, YHIST_CTR_CMD_SAVE_VREG_S *pstCmd);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
