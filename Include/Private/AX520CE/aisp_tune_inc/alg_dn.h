/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _ALG_DN_H_
#define _ALG_DN_H_

#include "ni_type.h"

// #include "compat_cal.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define DN_LOCK_INIT(IspDev)   COMPAT_LOCK_INIT(IspDev)
#define DN_LOCK_DEINIT(IspDev) COMPAT_LOCK_DEINIT(IspDev)
#define DN_LOCK(IspDev)        COMPAT_LOCK(IspDev)
#define DN_UNLOCK(IspDev)      COMPAT_UNLOCK(IspDev)

#define DN_CTX_S               COMPAT_CTX_S
#define DN_MEM_INFO_S          COMPAT_MEM_INFO_S
#define DN_GET_DEV_CTX(IspDev) COMPAT_GET_DEV_CTX(IspDev, DN)

#define DN_MEM_INIT(IspDev, pstCtx)   COMPAT_MEM_INIT(IspDev, DN, pstCtx)
#define DN_MEM_DEINIT(IspDev, pstCtx) COMPAT_MEM_DEINIT(IspDev, DN, pstCtx)
#define DN_MEM_MAP(IspDev, pstCtx)    COMPAT_MEM_MAP(IspDev, DN, pstCtx)
#define DN_MEM_UMAP(IspDev, pstCtx)   COMPAT_MEM_UMAP(IspDev, DN, pstCtx)

#define DN_TRACE COMPAT_TRACE
#define DN_DBG   COMPAT_DBG

#define DN_AWES_BLK_DATA_S   COMPAT_AWES_BLK_DATA_S
#define DN_AWES_DATA_CHN_R   COMPAT_AWES_DATA_CHN_R /* AWES统计块的 R 通道 */
#define DN_AWES_DATA_CHN_G   COMPAT_AWES_DATA_CHN_G /* AWES统计块的 G 通道 */
#define DN_AWES_DATA_CHN_B   COMPAT_AWES_DATA_CHN_B /* AWES统计块的 B 通道 */
#define DN_AWES_DATA_CHN_Y   COMPAT_AWES_DATA_CHN_Y /* AWES统计块的 Y 通道 */
#define DN_AWES_DATA_CHN_NUM COMPAT_AWES_DATA_CHN_NUM
#define DN_AWES_DATA_CHN_E   COMPAT_AWES_DATA_CHN_E

#define DnCalcDbToMul(u32DbGain) COMPAT_CalcDbToMul((u32DbGain))

#define DnCalGetAwesDataByPos(pstAwes, x, y, pstBlkData) \
    COMPAT_GetAwesBlkDataByPos((pstAwes), (x), (y), (pstBlkData))
#define DnCalGetAwesChannelData(pstAwes, x, y, enDataChn) \
    COMPAT_GetAwesChannelData((pstAwes), (x), (y), (enDataChn))

typedef struct tagDnVerg {
    NI_BOOL bModify;  /* 参数更新标记 */
    NI_BOOL bDebugEn; /* 打印输出开关 */

    DN_METHOD_E enMethod;  /* 算法方案选择 */
    DN_MODE_E   enDnMode;  /* 算法模式选择 */
    DN_PARAM_S  stDnParam; /* 算法参数 */

    DN_MODE_E enDnResult; /* 算法结果 */

    NI_U16 u16N2dCnt; /* 白天切换夜间的计数 */
    NI_U16 u16D2nCnt; /* 夜晚切换白天的计数 */

    NI_U32 u32FrmCnt; /* AE稳定后的帧计数 */

    NI_U32        u32LedCtrlLum; /* LED判断需要关灯后，等待ae再次变化后再进行关灯操作 */
    DN_DBG_INFO_S stDbgInfo;     /* 调试信息 */
} DN_VREG_S;

NI_S32 DN_CAL_Init(DN_VREG_S *pstVreg);
NI_S32 DN_CAL_CalcByPhotoSens(DN_VREG_S       *pstVreg,
                              DN_STATISTICS_S *pstStat,
                              DN_RESULT_S     *pstResult);
NI_S32 DN_CAL_CalcBySoft(DN_VREG_S       *pstVreg,
                         DN_STATISTICS_S *pstStat,
                         DN_RESULT_S     *pstResult);

NI_S32 DN_CAL_SaveVreg(DN_VREG_S *pstVreg, DN_CMD_SAVE_VREG_S *pstCmd);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
