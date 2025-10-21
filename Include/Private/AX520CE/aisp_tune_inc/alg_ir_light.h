/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _alg_ir_light_H_
#define _alg_ir_light_H_

#include "ni_type.h"

// #include "compat_cal.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define IR_LIGHT_LOCK_INIT(IspDev)   COMPAT_LOCK_INIT(IspDev)
#define IR_LIGHT_LOCK_DEINIT(IspDev) COMPAT_LOCK_DEINIT(IspDev)
#define IR_LIGHT_LOCK(IspDev)        COMPAT_LOCK(IspDev)
#define IR_LIGHT_UNLOCK(IspDev)      COMPAT_UNLOCK(IspDev)

#define IR_LIGHT_CTX_S               COMPAT_CTX_S
#define IR_LIGHT_MEM_INFO_S          COMPAT_MEM_INFO_S
#define IR_LIGHT_GET_DEV_CTX(IspDev) COMPAT_GET_DEV_CTX(IspDev, IR_LIGHT)

#define IR_LIGHT_MEM_INIT(IspDev, pstCtx)   COMPAT_MEM_INIT(IspDev, IR_LIGHT, pstCtx)
#define IR_LIGHT_MEM_DEINIT(IspDev, pstCtx) COMPAT_MEM_DEINIT(IspDev, IR_LIGHT, pstCtx)
#define IR_LIGHT_MEM_MAP(IspDev, pstCtx)    COMPAT_MEM_MAP(IspDev, IR_LIGHT, pstCtx)
#define IR_LIGHT_MEM_UMAP(IspDev, pstCtx)   COMPAT_MEM_UMAP(IspDev, IR_LIGHT, pstCtx)

#define IR_LIGHT_TRACE COMPAT_TRACE
#define IR_LIGHT_DBG   COMPAT_DBG

#define IR_LIGHT_AWES_BLK_DATA_S   COMPAT_AWES_BLK_DATA_S
#define IR_LIGHT_AWES_DATA_CHN_R   COMPAT_AWES_DATA_CHN_R /* AWES统计块的 R 通道 */
#define IR_LIGHT_AWES_DATA_CHN_G   COMPAT_AWES_DATA_CHN_G /* AWES统计块的 G 通道 */
#define IR_LIGHT_AWES_DATA_CHN_B   COMPAT_AWES_DATA_CHN_B /* AWES统计块的 B 通道 */
#define IR_LIGHT_AWES_DATA_CHN_Y   COMPAT_AWES_DATA_CHN_Y /* AWES统计块的 Y 通道 */
#define IR_LIGHT_AWES_DATA_CHN_NUM COMPAT_AWES_DATA_CHN_NUM
#define IR_LIGHT_AWES_DATA_CHN_E   COMPAT_AWES_DATA_CHN_E

#define IrLightCalcDbToMul(u32DbGain) COMPAT_CalcDbToMul((u32DbGain))

#define IrLightCalGetAwesDataByPos(pstAwes, x, y, pstBlkData) \
    COMPAT_GetAwesBlkDataByPos((pstAwes), (x), (y), (pstBlkData))
#define IrLightCalGetAwesChannelData(pstAwes, x, y, enDataChn) \
    COMPAT_GetAwesChannelData((pstAwes), (x), (y), (enDataChn))

typedef struct tagIrLightVerg {
    NI_BOOL bModify;  /* 参数更新标记 */
    NI_BOOL bDebugEn; /* 打印输出开关 */

    IR_LIGHT_METHOD_E enMethod;    /* 算法方案选择 */
    IR_LIGHT_MODE_E   enLightMode; /* 补光模式 */
    NI_U8             u8Level;     /* 补光强度设置,
                                    * enLightMode = IR_LIGHT_MODE_MAUNAL 时有效
                                    * range[0, 100], default:50 */

    IR_LIGHT_PARAM_S stLightParam; /* 补光控制参数 */

    NI_U32 au32LightLevel[IR_LIGHT_MAX_LIGHT_NUM]; /* 记录当前补光灯结果 */

    NI_U32 u32Kp;
    NI_U32 u32Ki;
    NI_U32 u32Kd;
    NI_S32 as32Delta[3];
} IR_LIGHT_VREG_S;

NI_S32 IR_LIGHT_CAL_Init(IR_LIGHT_VREG_S *pstVreg);
NI_S32 IR_LIGHT_CAL_CalcLightLevelFix(IR_LIGHT_VREG_S *pstVreg);
NI_S32 IR_LIGHT_CAL_CalcEnvDelta(IR_LIGHT_VREG_S *pstVreg, IR_LIGHT_STATISTICS_S *pstStat);
NI_S32 IR_LIGHT_CAL_CalcLightLevelVar(IR_LIGHT_VREG_S *pstVreg, IR_LIGHT_STATISTICS_S *pstStat);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
