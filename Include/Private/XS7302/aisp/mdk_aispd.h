/*******************************************************************************
 * Copyright (C) 2020-2025 ZheJiang Chipup Technology CO.,LTD.
 *
 * 文件名 : mdk_aispd.h
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



#ifndef _MDK_AISPD_H_
#define _MDK_AISPD_H_


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "ni_type.h"
#include "ni_comm_aispd.h"
#include "ni_comm_aisp.h"


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
NI_S32 NI_MDK_AISPD_Init(ISP_DEV IspDev);
NI_S32 NI_MDK_AISPD_DeInit(ISP_DEV IspDev);
NI_S32 NI_MDK_AISPD_GetVersion(ISP_DEV IspDev, AISPD_VERSION_ATTR_S *pstVersion);
NI_S32 NI_MDK_AISPD_GetAeAttr(ISP_DEV IspDev, AISPD_AE_ALG_PARAM_S  *pstAeAlgParam);
NI_S32 NI_MDK_AISPD_SetAeAttr(ISP_DEV IspDev, AISPD_AE_ALG_PARAM_S  *pstAeAlgParam);
NI_S32 NI_MDK_AISPD_GetAwbAttr(ISP_DEV IspDev, AISPD_AWB_PARAM_S *pstAwbParam);
NI_S32 NI_MDK_AISPD_SetAwbAttr(ISP_DEV IspDev, AISPD_AWB_PARAM_S *pstAwbParam);
NI_S32 NI_MDK_AISPD_GetBlcAttr(ISP_DEV IspDev, AISPD_BLC_ATTR_S *pstBlcAttr);
NI_S32 NI_MDK_AISPD_SetBlcAttr(ISP_DEV IspDev, AISPD_BLC_ATTR_S *pstBlcAttr);
NI_S32 NI_MDK_AISPD_GetGammaAttr(ISP_DEV IspDev, AISPD_GAMMA_ATTR_S *pstGammaAttr);
NI_S32 NI_MDK_AISPD_SetGammaAttr(ISP_DEV IspDev, AISPD_GAMMA_ATTR_S *pstGammaAttr);
NI_S32 NI_MDK_AISPD_GetLscAttr(ISP_DEV IspDev, AISPD_LSC_ATTR_S *pstLcsAttr);
NI_S32 NI_MDK_AISPD_SetLscAttr(ISP_DEV IspDev, AISPD_LSC_ATTR_S *pstLcsAttr);
NI_S32 NI_MDK_AISPD_GetGicAttr(ISP_DEV IspDev, AISPD_GIC_ATTR_S *pstGicAttr);
NI_S32 NI_MDK_AISPD_SetGicAttr(ISP_DEV IspDev, AISPD_GIC_ATTR_S *pstGicAttr);
NI_S32 NI_MDK_AISPD_GetDpcAttr(ISP_DEV IspDev, AISPD_DPC_ATTR_S *pstDpcAttr);
NI_S32 NI_MDK_AISPD_SetDpcAttr(ISP_DEV IspDev, AISPD_DPC_ATTR_S *pstDpcAttr);
NI_S32 NI_MDK_AISPD_GetCcmAttr(ISP_DEV IspDev, AISPD_CCM_ATTR_S *pstCcmAttr);
NI_S32 NI_MDK_AISPD_SetCcmAttr(ISP_DEV IspDev, AISPD_CCM_ATTR_S *pstCcmAttr);
NI_S32 NI_MDK_AISPD_GetLwdrAttr(ISP_DEV IspDev, AISPD_LWDR_ATTR_S *pstLwdrAttr);
NI_S32 NI_MDK_AISPD_SetLwdrAttr(ISP_DEV IspDev, AISPD_LWDR_ATTR_S *pstLwdrAttr);
NI_S32 NI_MDK_AISPD_GetCscCdsAttr(ISP_DEV IspDev, AISPD_CSC_CDS_ATTR_S *pstCscCdsAttr);
NI_S32 NI_MDK_AISPD_SetCscCdsAttr(ISP_DEV IspDev, AISPD_CSC_CDS_ATTR_S *pstCscCdsAttr);
NI_S32 NI_MDK_AISPD_GetEeAttr(ISP_DEV IspDev, AISPD_EE_ATTR_S *pstEeAttr);
NI_S32 NI_MDK_AISPD_SetEeAttr(ISP_DEV IspDev, AISPD_EE_ATTR_S *pstEeAttr);
NI_S32 NI_MDK_AISPD_GetCfaAttr(ISP_DEV IspDev, AISPD_CFA_ATTR_S *pstCfaAttr);
NI_S32 NI_MDK_AISPD_SetCfaAttr(ISP_DEV IspDev, AISPD_CFA_ATTR_S *pstCfaAttr);
NI_S32 NI_MDK_AISPD_GetDowAttr(ISP_DEV IspDev, AISPD_DOW_ATTR_S *pstDowAttr);
NI_S32 NI_MDK_AISPD_SetDowAttr(ISP_DEV IspDev, AISPD_DOW_ATTR_S *pstDowAttr);
NI_S32 NI_MDK_AISPD_GetDrcdAttr(ISP_DEV IspDev, AISPD_DRCD_ATTR_S *pstDrcdAttr);
NI_S32 NI_MDK_AISPD_SetDrcdAttr(ISP_DEV IspDev, AISPD_DRCD_ATTR_S *pstDrcdAttr);
NI_S32 NI_MDK_AISPD_GetCnrAttr(ISP_DEV IspDev, AISPD_CNR_ATTR_S *pstCnrAttr);
NI_S32 NI_MDK_AISPD_SetCnrAttr(ISP_DEV IspDev, AISPD_CNR_ATTR_S *pstCnrAttr);
NI_S32 NI_MDK_AISPD_GetNry3dAttr(ISP_DEV IspDev, AISPD_NRY3D_ATTR_S *pstNry3dAttr);
NI_S32 NI_MDK_AISPD_SetNry3dAttr(ISP_DEV IspDev, AISPD_NRY3D_ATTR_S *pstNry3dAttr);
NI_S32 NI_MDK_AISPD_GetYhistContrastAttr(ISP_DEV IspDev, AISPD_YHIST_CONTRAST_ATTR_S *pstYhistContrastAttr);
NI_S32 NI_MDK_AISPD_SetYhistContrastAttr(ISP_DEV IspDev, AISPD_YHIST_CONTRAST_ATTR_S *pstYhistContrastAttr);
NI_S32 NI_MDK_AISPD_GetWbAttr(ISP_DEV IspDev, AISPD_WB_ATTR_S *pstWbAttr);
NI_S32 NI_MDK_AISPD_SetWbAttr(ISP_DEV IspDev, AISPD_WB_ATTR_S *pstWbAttr);
NI_S32 NI_MDK_AISPD_GetModRun(ISP_DEV IspDev, NI_CHAR *pszModName, NI_BOOL *pbRunEn);
NI_S32 NI_MDK_AISPD_SetModRun(ISP_DEV IspDev, NI_CHAR *pszModName, NI_BOOL *pbRunEn);
NI_S32 NI_MDK_AISPD_GetCfgInfo(ISP_DEV IspDev, AISP_CFG_FILE_INFO_S *pstCfgInfo);
NI_S32 NI_MDK_AISPD_ForceSync(ISP_DEV IspDev);



#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

