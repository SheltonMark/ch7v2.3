/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _MDK_AISPALG_H_
#define _MDK_AISPALG_H_

#include "ni_type.h"
#include "ni_aisp_define.h"
#include "ni_comm_aispalg.h"
#include "ni_comm_aispalg_inner.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/********************** COMPAT ***********************************/

/********************** AE ***********************************/

NI_S32 NI_MDK_AE_Init(ISP_DEV IspDev);
NI_S32 NI_MDK_AE_Exit(ISP_DEV IspDev);
NI_S32 NI_MDK_AE_Reset(ISP_DEV IspDev);
NI_S32 NI_MDK_AE_SetExposureAttr(ISP_DEV IspDev, AE_ATTR_S *pstExpAttr);
NI_S32 NI_MDK_AE_GetExposureAttr(ISP_DEV IspDev, AE_ATTR_S *pstExpAttr);
NI_S32 NI_MDK_AE_SetRoute(ISP_DEV IspDev, AE_ROUTE_S *pstRoute);
NI_S32 NI_MDK_AE_GetRoute(ISP_DEV IspDev, AE_ROUTE_S *pstRoute);
NI_S32 NI_MDK_AE_SetRegion(ISP_DEV IspDev, AE_REGION_ATTR_S *pstRegion);
NI_S32 NI_MDK_AE_GetRegion(ISP_DEV IspDev, AE_REGION_ATTR_S *pstRegion);
NI_S32 NI_MDK_AE_SetWinWt(ISP_DEV IspDev, AE_WIN_WT_S *pstWinWt);
NI_S32 NI_MDK_AE_GetWinWt(ISP_DEV IspDev, AE_WIN_WT_S *pstWinWt);
NI_S32 NI_MDK_AE_Run(ISP_DEV IspDev, AE_STATISTICS_S *pstStat, AE_RESULT_S *pstResult);
NI_S32 NI_MDK_AE_DebugEn(ISP_DEV IspDev, NI_BOOL bEnable);
NI_S32 NI_MDK_AE_GetInfo(ISP_DEV IspDev, AE_INFO_S *pstAeInfo);
NI_S32 NI_MDK_AE_SetExpResult(ISP_DEV IspDev, AE_RESULT_S *pstResult);

/********************** AWB ***********************************/

NI_S32 NI_MDK_AWB_Init(ISP_DEV IspDev);
NI_S32 NI_MDK_AWB_Exit(ISP_DEV IspDev);
NI_S32 NI_MDK_AWB_Reset(ISP_DEV IspDev);
NI_S32 NI_MDK_AWB_SetAttr(ISP_DEV IspDev, AWB_ATTR_S *pstAwbAttr);
NI_S32 NI_MDK_AWB_GetAttr(ISP_DEV IspDev, AWB_ATTR_S *pstAwbAttr);
NI_S32 NI_MDK_AWB_SetRegionAttr(ISP_DEV IspDev, AWB_REGION_ATTR_S *pstAwbRegionAttr);
NI_S32 NI_MDK_AWB_GetRegionAttr(ISP_DEV IspDev, AWB_REGION_ATTR_S *pstAwbRegionAttr);
NI_S32 NI_MDK_AWB_Run(ISP_DEV IspDev, AWB_STATISTICS_S *pstStat, AWB_RESULT_S *pstResult);
NI_S32 NI_MDK_AWB_GetInfo(ISP_DEV IspDev, AWB_INFO_S *pstAwbInfo);
NI_S32 NI_MDK_AWB_SetInfo(ISP_DEV IspDev, AWB_INFO_S *pstAwbInfo);
NI_S32 NI_MDK_AWB_DebugEn(ISP_DEV IspDev, NI_BOOL bEnable);

/********************** CCM ***********************************/

NI_S32 NI_MDK_CCM_Run(ISP_DEV IspDev, CCM_PARAM_S *pstParam, CCM_MATRIX_S *pstMatrixOut);

/********************** DAYNIGHT ***********************************/

NI_S32 NI_MDK_DN_Init(ISP_DEV IspDev);
NI_S32 NI_MDK_DN_Exit(ISP_DEV IspDev);
NI_S32 NI_MDK_DN_Reset(ISP_DEV IspDev);
NI_S32 NI_MDK_DN_SetDnAttr(ISP_DEV IspDev, DN_ATTR_S *pstDnAttr);
NI_S32 NI_MDK_DN_GetDnAttr(ISP_DEV IspDev, DN_ATTR_S *pstDnAttr);
NI_S32 NI_MDK_DN_Run(ISP_DEV IspDev, DN_STATISTICS_S *pstStat, DN_RESULT_S *pstResult);
NI_S32 NI_MDK_DN_GetInfo(ISP_DEV IspDev, DN_INFO_S *pstDnInfo);
NI_S32 NI_MDK_DN_SetInfo(ISP_DEV IspDev, DN_INFO_S *pstDnInfo);
NI_S32 NI_MDK_DN_DebugEn(ISP_DEV IspDev, NI_BOOL bEnable);

/********************** IRLIGHT ***********************************/

NI_S32 NI_MDK_IR_LIGHT_Init(ISP_DEV IspDev);
NI_S32 NI_MDK_IR_LIGHT_Exit(ISP_DEV IspDev);
NI_S32 NI_MDK_IR_LIGHT_Reset(ISP_DEV IspDev);
NI_S32 NI_MDK_IR_LIGHT_SetLightAttr(ISP_DEV IspDev, IR_LIGHT_ATTR_S *pstLightAttr);
NI_S32 NI_MDK_IR_LIGHT_GetLightAttr(ISP_DEV IspDev, IR_LIGHT_ATTR_S *pstLightAttr);
NI_S32 NI_MDK_IR_LIGHT_Run(ISP_DEV IspDev, IR_LIGHT_STATISTICS_S *pstStat, IR_LIGHT_RESULT_S *pstResult);
NI_S32 NI_MDK_IR_LIGHT_GetInfo(ISP_DEV IspDev, IR_LIGHT_INFO_S *pstLightInfo);
NI_S32 NI_MDK_IR_LIGHT_SetInfo(ISP_DEV IspDev, IR_LIGHT_INFO_S *pstLightInfo);
NI_S32 NI_MDK_IR_LIGHT_DebugEn(ISP_DEV IspDev, NI_BOOL bEnable);

/********************** LEDLIGHT ***********************************/

NI_S32 NI_MDK_LED_LIGHT_Init(ISP_DEV IspDev);
NI_S32 NI_MDK_LED_LIGHT_Exit(ISP_DEV IspDev);
NI_S32 NI_MDK_LED_LIGHT_Reset(ISP_DEV IspDev);
NI_S32 NI_MDK_LED_LIGHT_SetLightAttr(ISP_DEV IspDev, LED_LIGHT_ATTR_S *pstLightAttr);
NI_S32 NI_MDK_LED_LIGHT_GetLightAttr(ISP_DEV IspDev, LED_LIGHT_ATTR_S *pstLightAttr);
NI_S32 NI_MDK_LED_LIGHT_Run(ISP_DEV IspDev, LED_LIGHT_STATISTICS_S *pstStat, LED_LIGHT_RESULT_S *pstResult);
NI_S32 NI_MDK_LED_LIGHT_GetInfo(ISP_DEV IspDev, LED_LIGHT_INFO_S *pstLightInfo);
NI_S32 NI_MDK_LED_LIGHT_SetInfo(ISP_DEV IspDev, LED_LIGHT_INFO_S *pstLightInfo);
NI_S32 NI_MDK_LED_LIGHT_DebugEn(ISP_DEV IspDev, NI_BOOL bEnable);

/********************** RGB2YUV ***********************************/
NI_S32 NI_MDK_RGB2YUV_Run(ISP_DEV IspDev, RGB2YUV_PARAM_S *pstParam, RGB2YUV_MATRIX_S *pstMatrixOut);

/********************** YHISTCTR ***********************************/

NI_S32 NI_MDK_YHIST_CTR_Init(ISP_DEV IspDev);
NI_S32 NI_MDK_YHIST_CTR_Exit(ISP_DEV IspDev);
NI_S32 NI_MDK_YHIST_CTR_Reset(NI_U32 IspDev);
NI_S32 NI_MDK_YHIST_CTR_SetAttr(ISP_DEV IspDev, YHIST_CTR_PARAM_S *pstYhistCtrParam);
NI_S32 NI_MDK_YHIST_CTR_GetAttr(ISP_DEV IspDev, YHIST_CTR_PARAM_S *pstYhistCtrParam);
NI_S32 NI_MDK_YHIST_CTR_Run(ISP_DEV IspDev, YHIST_CTR_STATISTICS_S *pstStat, YHIST_CTR_RESULT_S *pstResult);
NI_S32 NI_MDK_YHIST_CTR_DebugEn(ISP_DEV IspDev, NI_BOOL bEnable);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
