/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _PLAT_ISP_REDEFINE_H_
#define _PLAT_ISP_REDEFINE_H_

#include "mdk_pisp.h"
#include "ni_comm_pisp.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define ISP_AWES_FROM_FRAME_L         PISP_AWES_FROM_FRAME_L
#define ISP_AWES_FROM_FRAME_S         PISP_AWES_FROM_FRAME_S
#define ISP_AWES_FROM_FRAME_COMPO     PISP_AWES_FROM_FRAME_COMPO
#define ISP_AWES_FROM_FRAME_L_S       PISP_AWES_FROM_FRAME_L_S
#define ISP_AWES_FROM_FRAME_L_COMPO   PISP_AWES_FROM_FRAME_L_COMPO
#define ISP_AWES_FROM_FRAME_S_COMPO   PISP_AWES_FROM_FRAME_S_COMPO
#define ISP_AWES_FROM_FRAME_L_S_COMPO PISP_AWES_FROM_FRAME_L_S_COMPO

#define ISP_AWES_PARAM_S           PISP_AWES_PARAM_S
#define ISP_AWES_STATISTICS_S      PISP_AWES_STATISTICS_S
#define NI_MDK_ISP_GetAwesParam    NI_MDK_PISP_GetAwesParam
#define NI_MDK_ISP_SetAwesParam    NI_MDK_PISP_SetAwesParam
#define NI_MDK_ISP_GetAwesData     NI_MDK_PISP_GetAwesData
#define NI_MDK_ISP_ReleaseAwesData NI_MDK_PISP_ReleaseAwesData

#define ISP_HDR_MODE_S        PISP_HDR_MODE_S
#define NI_MDK_ISP_GetHdrMode NI_MDK_PISP_GetHdrMode

#define ISP_BLC_PARAM_S        PISP_BLC_PARAM_S
#define NI_MDK_ISP_GetBlcParam NI_MDK_PISP_GetBlcParam
#define NI_MDK_ISP_SetBlcParam NI_MDK_PISP_SetBlcParam

#define ISP_DGC_PARAM_S        PISP_DGC_PARAM_S
#define NI_MDK_ISP_GetDgcParam NI_MDK_PISP_GetDgcParam
#define NI_MDK_ISP_SetDgcParam NI_MDK_PISP_SetDgcParam

#define ISP_WB_PARAM_S        PISP_WB_PARAM_S
#define NI_MDK_ISP_GetWbParam NI_MDK_PISP_GetWbParam
#define NI_MDK_ISP_SetWbParam NI_MDK_PISP_SetWbParam

#define ISP_LWDR_PARAM_S        PISP_LWDR_PARAM_S
#define NI_MDK_ISP_GetLwdrParam NI_MDK_PISP_GetLwdrParam
#define NI_MDK_ISP_SetLwdrParam NI_MDK_PISP_SetLwdrParam

#define ISP_GAMMA_PARAM_S        PISP_GAMMA_PARAM_S
#define NI_MDK_ISP_GetGammaParam NI_MDK_PISP_GetGammaParam
#define NI_MDK_ISP_SetGammaParam NI_MDK_PISP_SetGammaParam

#define ISP_DPC_PARAM_S        PISP_DPC_PARAM_S
#define NI_MDK_ISP_GetDpcParam NI_MDK_PISP_GetDpcParam
#define NI_MDK_ISP_SetDpcParam NI_MDK_PISP_SetDpcParam

#define ISP_GIC_PARAM_S        PISP_GIC_PARAM_S
#define NI_MDK_ISP_GetGicParam NI_MDK_PISP_GetGicParam
#define NI_MDK_ISP_SetGicParam NI_MDK_PISP_SetGicParam

#define ISP_CCM_PARAM_S        PISP_CCM_PARAM_S
#define NI_MDK_ISP_GetCcmParam NI_MDK_PISP_GetCcmParam
#define NI_MDK_ISP_SetCcmParam NI_MDK_PISP_SetCcmParam

#define ISP_CNR_PARAM_S        PISP_CNR_PARAM_S
#define NI_MDK_ISP_GetCnrParam NI_MDK_PISP_GetCnrParam
#define NI_MDK_ISP_SetCnrParam NI_MDK_PISP_SetCnrParam

#define ISP_PCR_PARAM_S        PISP_PCR_PARAM_S
#define NI_MDK_ISP_GetPcrParam NI_MDK_PISP_GetPcrParam
#define NI_MDK_ISP_SetPcrParam NI_MDK_PISP_SetPcrParam

#define ISP_SCC_PARAM_S        PISP_SCC_PARAM_S
#define NI_MDK_ISP_GetSccParam NI_MDK_PISP_GetSccParam
#define NI_MDK_ISP_SetSccParam NI_MDK_PISP_SetSccParam

#define ISP_CSC_CDS_PARAM_S       PISP_CSC_CDS_PARAM_S
#define NI_MDK_ISP_GetCscCdsParam NI_MDK_PISP_GetCscCdsParam
#define NI_MDK_ISP_SetCscCdsParam NI_MDK_PISP_SetCscCdsParam

#define ISP_CFA_PARAM_S        PISP_CFA_PARAM_S
#define NI_MDK_ISP_GetCfaParam NI_MDK_PISP_GetCfaParam
#define NI_MDK_ISP_SetCfaParam NI_MDK_PISP_SetCfaParam

#define ISP_RGB_DPC_PARAM_S       PISP_RGB_DPC_PARAM_S
#define NI_MDK_ISP_GetRgbDpcParam NI_MDK_PISP_GetRgbDpcParam
#define NI_MDK_ISP_SetRgbDpcParam NI_MDK_PISP_SetRgbDpcParam

#define ISP_RGB_GIC_PARAM_S       PISP_RGB_GIC_PARAM_S
#define NI_MDK_ISP_GetRgbGicParam NI_MDK_PISP_GetRgbGicParam
#define NI_MDK_ISP_SetRgbGicParam NI_MDK_PISP_SetRgbGicParam

#define ISP_DRCD_PARAM_S        PISP_DRCD_PARAM_S
#define NI_MDK_ISP_GetDrcdParam NI_MDK_PISP_GetDrcdParam
#define NI_MDK_ISP_SetDrcdParam NI_MDK_PISP_SetDrcdParam

#define ISP_DEC_PARAM_S        PISP_DEC_PARAM_S
#define NI_MDK_ISP_GetDecParam NI_MDK_PISP_GetDecParam
#define NI_MDK_ISP_SetDecParam NI_MDK_PISP_SetDecParam

#define ISP_LSC_PARAM_S        PISP_LSC_PARAM_S
#define NI_MDK_ISP_GetLscParam NI_MDK_PISP_GetLscParam
#define NI_MDK_ISP_SetLscParam NI_MDK_PISP_SetLscParam

#define ISP_NRY2D_PARAM_S        PISP_NRY2D_PARAM_S
#define NI_MDK_ISP_GetNry2dParam NI_MDK_PISP_GetNry2dParam
#define NI_MDK_ISP_SetNry2dParam NI_MDK_PISP_SetNry2dParam

#define ISP_NRYFULL3D_PARAM_S        PISP_NRYFULL3D_PARAM_S
#define NI_MDK_ISP_GetNryFull3dParam NI_MDK_PISP_GetNryFull3dParam
#define NI_MDK_ISP_SetNryFull3dParam NI_MDK_PISP_SetNryFull3dParam

#define ISP_NRYLC3D_PARAM_S        PISP_NRYLC3D_PARAM_S
#define NI_MDK_ISP_GetNryLc3dParam NI_MDK_PISP_GetNryLc3dParam
#define NI_MDK_ISP_SetNryLc3dParam NI_MDK_PISP_SetNryLc3dParam

#define ISP_DOW_PARAM_S        PISP_DOW_PARAM_S
#define NI_MDK_ISP_GetDowParam NI_MDK_PISP_GetDowParam
#define NI_MDK_ISP_SetDowParam NI_MDK_PISP_SetDowParam

#define ISP_DOW_STATE_INFO_S       PISP_DOW_STATE_INFO_S
#define NI_MDK_ISP_GetDowStateInfo NI_MDK_PISP_GetDowStateInfo

#define ISP_EE_PARAM_S        PISP_EE_PARAM_S
#define NI_MDK_ISP_GetEeParam NI_MDK_PISP_GetEeParam
#define NI_MDK_ISP_SetEeParam NI_MDK_PISP_SetEeParam

#define ISP_CE_PARAM_S        PISP_CE_PARAM_S
#define NI_MDK_ISP_GetCeParam NI_MDK_PISP_GetCeParam
#define NI_MDK_ISP_SetCeParam NI_MDK_PISP_SetCeParam

#define ISP_AF_PARAM_S        PISP_AF_PARAM_S
#define NI_MDK_ISP_GetAfParam NI_MDK_PISP_GetAfParam
#define NI_MDK_ISP_SetAfParam NI_MDK_PISP_SetAfParam

#define ISP_AF_STATISTICS_S      PISP_AF_STATISTICS_S
#define NI_MDK_ISP_GetAfData     NI_MDK_PISP_GetAfData
#define NI_MDK_ISP_ReleaseAfData NI_MDK_PISP_ReleaseAfData

#define ISP_YCONTRAST_S         PISP_YCONTRAST_S
#define NI_MDK_ISP_GetYContrast NI_MDK_PISP_GetYContrast
#define NI_MDK_ISP_SetYContrast NI_MDK_PISP_SetYContrast

#define ISP_YHIST_PARAM_S        PISP_YHIST_PARAM_S
#define NI_MDK_ISP_GetYhistParam NI_MDK_PISP_GetYhistParam
#define NI_MDK_ISP_SetYhistParam NI_MDK_PISP_SetYhistParam

#define ISP_YHIST_STATISTICS_S      PISP_YHIST_STATISTICS_S
#define NI_MDK_ISP_GetYhistData     NI_MDK_PISP_GetYhistData
#define NI_MDK_ISP_ReleaseYhistData NI_MDK_PISP_ReleaseYhistData

#define ISP_AWES_PARAM_S       PISP_AWES_PARAM_S
#define NI_MDK_SP_GetAwesParam NI_MDK_PISP_GetAwesParam
#define NI_MDK_SP_SetAwesParam NI_MDK_PISP_SetAwesParam

#define ISP_AWES_STATISTICS_S      PISP_AWES_STATISTICS_S
#define NI_MDK_ISP_GetAwesData     NI_MDK_PISP_GetAwesData
#define NI_MDK_ISP_ReleaseAwesData NI_MDK_PISP_ReleaseAwesData

#define ISP_RAWHIST_PARAM_S        PISP_RAWHIST_PARAM_S
#define NI_MDK_ISP_GetRawHistParam NI_MDK_PISP_GetRawHistParam
#define NI_MDK_ISP_SetRawHistParam NI_MDK_PISP_SetRawHistParam

#define ISP_RAWHIST_STATISTICS_S      PISP_RAWHIST_STATISTICS_S
#define NI_MDK_ISP_GetRawHistData     NI_MDK_PISP_GetRawHistData
#define NI_MDK_ISP_ReleaseRawHistData NI_MDK_PISP_ReleaseRawHistData

#define ISP_HDR_MODE_S        PISP_HDR_MODE_S
#define NI_MDK_ISP_GetHdrMode NI_MDK_PISP_GetHdrMode
#define NI_MDK_ISP_SetHdrMode NI_MDK_PISP_SetHdrMode

#define ISP_MOD_PARAM_S        PISP_MOD_PARAM_S
#define NI_MDK_ISP_GetModParam NI_MDK_PISP_GetModParam
#define NI_MDK_ISP_SetModParam NI_MDK_PISP_SetModParam

#define ISP_SCE_PARAM_S        PISP_SCE_PARAM_S
#define NI_MDK_ISP_GetSceParam NI_MDK_PISP_GetSceParam
#define NI_MDK_ISP_SetSceParam NI_MDK_PISP_SetSceParam

#define ISP_YGAMMA_S         PISP_YGAMMA_S
#define NI_MDK_ISP_GetYGamma NI_MDK_PISP_GetYGamma
#define NI_MDK_ISP_SetYGamma NI_MDK_PISP_SetYGamma

#define ISP_BACA_PARAM_S        PISP_BACA_PARAM_S
#define NI_MDK_ISP_GetBacaParam NI_MDK_PISP_GetBacaParam
#define NI_MDK_ISP_SetBacaParam NI_MDK_PISP_SetBacaParam

#define NI_MDK_ISP_SetBt1120Mode NI_MDK_PISP_SetBt1120Mode

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
