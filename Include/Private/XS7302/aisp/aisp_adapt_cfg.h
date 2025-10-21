/*******************************************************************************
 * Copyright (C) 2020-2025 ZheJiang Chipup Technology CO.,LTD.
 *
 * 文件名 : aisp_adapt_cfg.h
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



#ifndef _AISP_ADAPT_CFG_H_
#define _AISP_ADAPT_CFG_H_


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
//#include "ni_define.h"
//#include "ni_comm_isp.h"
#include "ni_aisp_define.h"
#include "ni_comm_aispd.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */


/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */


/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */

typedef struct tagAispdCfg
{
    AISPD_VERSION_ATTR_S         stVersionCfg;
    AISPD_AWES_ATTR_S            stAwesCfg;
    AISPD_AE_ATTR_S              stAeCfg;
    AISPD_AWB_ATTR_S             stAwbCfg;
    AISPD_WB_ATTR_S              stWbCfg;
    AISPD_BLC_ATTR_S             stBlcCfg;
    AISPD_GAMMA_ATTR_S           stGammaCfg;
    AISPD_CCM_ATTR_S             stCcmCfg;
    AISPD_CSC_CDS_ATTR_S         stCscCdsCfg;
    AISPD_LWDR_ATTR_S            stLwdrCfg;
    AISPD_DOW_ATTR_S             stDowCfg;
    AISPD_DRCD_ATTR_S            stDrcdCfg;
    AISPD_EE_ATTR_S              stEeCfg;
    AISPD_CFA_ATTR_S             stCfaCfg;
    AISPD_CNR_ATTR_S             stCnrCfg;
    AISPD_GIC_ATTR_S             stGicCfg;
    AISPD_NRY3D_ATTR_S           stNry3dCfg;
    AISPD_LSC_ATTR_S             stLscCfg;
    AISPD_DPC_ATTR_S             stDpcCfg;
    AISPD_YHIST_CONTRAST_ATTR_S  stYhistContrastCfg;
    AISPD_STYLE_ATTR_S           stStyleCfg;
    AISPD_SEN_EXP_ATTR_S         stSenExpCfg;
    AISPD_DN_ATTR_S              stDnCfg;
    AISPD_IR_LIGHT_ATTR_S        stIrLightCfg;
    AISPD_LED_LIGHT_ATTR_S       stLedLightCfg;
}AISPD_CFG_S;


typedef struct tagAispdCfgIndex
{
    AISPD_VERSION_ATTR_S         *pstVersionCfg;
    AISPD_AWES_ATTR_S            *pstAwesCfg;
    AISPD_AE_ATTR_S              *pstAeCfg;
    AISPD_AWB_ATTR_S             *pstAwbCfg;
    AISPD_WB_ATTR_S              *pstWbCfg;
    AISPD_BLC_ATTR_S             *pstBlcCfg;
    AISPD_GAMMA_ATTR_S           *pstGammaCfg;
    AISPD_CCM_ATTR_S             *pstCcmCfg;
    AISPD_CSC_CDS_ATTR_S         *pstCscCdsCfg;
    AISPD_LWDR_ATTR_S            *pstLwdrCfg;
    AISPD_DOW_ATTR_S             *pstDowCfg;
    AISPD_DRCD_ATTR_S            *pstDrcdCfg;
    AISPD_EE_ATTR_S              *pstEeCfg;
    AISPD_CFA_ATTR_S             *pstCfaCfg;
    AISPD_CNR_ATTR_S             *pstCnrCfg;
    AISPD_GIC_ATTR_S             *pstGicCfg;
    AISPD_NRY3D_ATTR_S           *pstNry3dCfg;
    AISPD_LSC_ATTR_S             *pstLscCfg;
    AISPD_DPC_ATTR_S             *pstDpcCfg;
    AISPD_YHIST_CONTRAST_ATTR_S  *pstYhistContrastCfg;
    AISPD_STYLE_ATTR_S           *pstStyleCfg;
    AISPD_SEN_EXP_ATTR_S         *pstSenExpCfg;
    AISPD_DN_ATTR_S              *pstDnCfg;
    AISPD_IR_LIGHT_ATTR_S        *pstIrLightCfg;
    AISPD_LED_LIGHT_ATTR_S       *pstLedLightCfg;
}AISPD_CFG_INDEX_S;

/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

