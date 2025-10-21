/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef __MDK_XSNN_H__
#define __MDK_XSNN_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */
/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "ni_type.h"
#include "ni_comm_xsnn.h"

/* 推送任务 */
NI_S32 NI_MDK_NN_ForwardByBin(XSNN_FORWARD_BY_BIN_CTRL_S *pstForwardCtrl, NI_U64 *pu64HandleId);

/* 查询任务结果 */
NI_S32 NI_MDK_NN_QueryResult(NI_U64 u64HandleId, XSNN_WORK_RESULT_S *pstWorkResult, NI_U32 u32TimeoutMs);

NI_S32 NI_MDK_NN_Init(NI_VOID);

NI_S32 NI_MDK_NN_DeInit(NI_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* End of #ifndef __MDK_XSNN_H__ */
