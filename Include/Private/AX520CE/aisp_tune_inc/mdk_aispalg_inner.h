/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _MDK_AISPALG_INNER_H_
#define _MDK_AISPALG_INNER_H_

#include "ni_type.h"
#include "ni_aisp_define.h"
#include "ni_comm_aispalg.h"
#include "ni_comm_aispalg_inner.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/********************** COMPAT ***********************************/

/********************** AE ***********************************/

NI_S32 NI_MDK_AE_Connect(ISP_DEV IspDev);
NI_S32 NI_MDK_AE_DisConnect(ISP_DEV IspDev);
NI_S32 NI_MDK_AE_CmdProcess(ISP_DEV IspDev, NI_VOID *pvCmd);

/********************** AWB ***********************************/

NI_S32 NI_MDK_AWB_Connect(ISP_DEV IspDev);
NI_S32 NI_MDK_AWB_DisConnect(ISP_DEV IspDev);
NI_S32 NI_MDK_AWB_CmdProcess(ISP_DEV IspDev, NI_VOID *pvCmd);

// NI_S32 NI_MDK_AWB_GetDbgInfo(ISP_DEV IspDev, AWB_DBG_INFO_S *pstAwbDbgInfo);
// NI_S32 NI_MDK_AWB_GetCalcInfo(ISP_DEV IspDev, AISP_AWES_INFO_S *pstAwes, AWB_EXT_INFO_S *pstExtInfo, AISP_AWB_RESULT_S *pstResult);

/********************** CCM ***********************************/

NI_S32 NI_MDK_CCM_GetTuningInfo(ISP_DEV IspDev, CCM_TUNING_PARAM_S *pstParam, CCM_TUNING_RESULT_S *pstResult);

/********************** DAYNIGHT ***********************************/

NI_S32 NI_MDK_DN_Connect(ISP_DEV IspDev);
NI_S32 NI_MDK_DN_DisConnect(ISP_DEV IspDev);
NI_S32 NI_MDK_DN_CmdProcess(ISP_DEV IspDev, NI_VOID *pvCmd);

/********************** IRLIGHT ***********************************/

NI_S32 NI_MDK_IR_LIGHT_Connect(ISP_DEV IspDev);
NI_S32 NI_MDK_IR_LIGHT_DisConnect(ISP_DEV IspDev);
NI_S32 NI_MDK_IR_LIGHT_CmdProcess(ISP_DEV IspDev, NI_VOID *pvCmd);

/********************** LEDLIGHT ***********************************/

NI_S32 NI_MDK_LED_LIGHT_Connect(ISP_DEV IspDev);
NI_S32 NI_MDK_LED_LIGHT_DisConnect(ISP_DEV IspDev);
NI_S32 NI_MDK_LED_LIGHT_CmdProcess(ISP_DEV IspDev, NI_VOID *pvCmd);

/********************** RGB2YUV ***********************************/

/********************** YHISTCTR ***********************************/

NI_S32 NI_MDK_YHIST_CTR_Connect(ISP_DEV IspDev);
NI_S32 NI_MDK_YHIST_CTR_DisConnect(ISP_DEV IspDev);
NI_S32 NI_MDK_YHIST_CTR_CmdProcess(ISP_DEV IspDev, NI_VOID *pvCmd);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
