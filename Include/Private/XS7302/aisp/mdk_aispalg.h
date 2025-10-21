/*******************************************************************************
 * Copyright (C) 2020-2025 ZheJiang Chipup Technology CO.,LTD.
 *
 * 文件名 : mdk_aispalg.h
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



#ifndef _MDK_AISPALG_H_
#define _MDK_AISPALG_H_


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "ni_type.h"
#include "ni_comm_aispalg.h"


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

/**********************AE***********************************/
NI_S32 NI_MDK_AE_MemInit(ISP_DEV IspDev);
NI_S32 NI_MDK_AE_MemExit(ISP_DEV IspDev);
NI_S32 NI_MDK_AE_Init(ISP_DEV IspDev);
NI_S32 NI_MDK_AE_DeInit(ISP_DEV IspDev);
NI_S32 NI_MDK_AE_ResetParam(ISP_DEV IspDev);
NI_S32 NI_MDK_AE_Process(ISP_DEV IspDev, AISP_AWES_INFO_S *pstAwes, AISP_EV_INFO_S *pstEvInfo, AISP_DR_INFO_S *pstDrInfo, ALG_AE_RESULT_S *pstResult);
NI_S32 NI_MDK_AE_SetExposureAttr(ISP_DEV IspDev, AE_ATTR_S *pstExpAttr);
NI_S32 NI_MDK_AE_GetExposureAttr(ISP_DEV IspDev, AE_ATTR_S *pstExpAttr);
NI_S32 NI_MDK_AE_SetRoute(ISP_DEV IspDev, AE_ROUTE_S *pstRoute);
NI_S32 NI_MDK_AE_GetRoute(ISP_DEV IspDev, AE_ROUTE_S *pstRoute);
NI_S32 NI_MDK_AE_SetRegion(ISP_DEV IspDev, AE_REGION_ATTR_S *pstRegion);
NI_S32 NI_MDK_AE_GetRegion(ISP_DEV IspDev, AE_REGION_ATTR_S *pstRegion);
NI_S32 NI_MDK_AE_SetWinWt(ISP_DEV IspDev, AE_WIN_WT_S *pstWinWt);
NI_S32 NI_MDK_AE_GetWinWt(ISP_DEV IspDev, AE_WIN_WT_S *pstWinWt);


/**********************AWB***********************************/
NI_S32 NI_MDK_AWB_MemInit(ISP_DEV IspDev);
NI_S32 NI_MDK_AWB_MemExit(ISP_DEV IspDev);
NI_S32 NI_MDK_AWB_Init(ISP_DEV IspDev);
NI_S32 NI_MDK_AWB_DeInit(ISP_DEV IspDev);
NI_S32 NI_MDK_AWB_ResetParam(ISP_DEV IspDev);
NI_S32 NI_MDK_AWB_Process(ISP_DEV IspDev, AISP_AWES_INFO_S *pstAwes, NI_S32 s32StartFame, NI_S32 s32EndFrame, AWB_EXT_INFO_S *pstExtInfo, AISP_AWB_RESULT_S *pstResult);
NI_S32 NI_MDK_AWB_SetAttr(ISP_DEV IspDev, AWB_ATTR_S *pstAwbAttr);
NI_S32 NI_MDK_AWB_GetAttr(ISP_DEV IspDev, AWB_ATTR_S *pstAwbAttr);
NI_S32 NI_MDK_AWB_GetRegionAttr(ISP_DEV IspDev, AWB_REGION_ATTR_S *pstAwbRegionAttr);
NI_S32 NI_MDK_AWB_SetRegionAttr(ISP_DEV IspDev, AWB_REGION_ATTR_S *pstAwbRegionAttr);
NI_S32 NI_MDK_AWB_GetDbgInfo(ISP_DEV IspDev, AWB_DBG_INFO_S *pstAwbDbgInfo);
NI_S32 NI_MDK_AWB_GetCalcInfo(ISP_DEV IspDev, AISP_AWES_INFO_S *pstAwes, AWB_EXT_INFO_S *pstExtInfo, AISP_AWB_RESULT_S *pstResult);

/**********************RGB2YUV***********************************/
NI_S32 NI_MDK_RGB2YUV_Process(ISP_DEV IspDev, RGB2YUV_COLOR_S *pstColor, RGB2YUV_MATRIX_S *pstDefMatrix, RGB2YUV_MATRIX_S *pstMatrixOut);

/**********************CCM***********************************/
NI_S32 NI_MDK_CCM_Process(ISP_DEV IspDev, CCM_PARAM_S      *pstCcmParam, AISP_AWB_RESULT_S *pstAwbResult, AISP_GAIN_ADJ_INFO_S *pstGainAdjInfo, CCM_MATRIX_S *pstMatrixOut);
NI_S32 NI_MDK_CCM_GetDebugInfo(ISP_DEV IspDev, CCM_PARAM_S      *pstCcmParam, AISP_AWB_RESULT_S *pstAwbResult, AISP_EXP_RESULT_S *pstExpResult, CCM_DBG_INFO_S *pstCcmDbgInfo);

/**********************YHIST_CTRL***********************************/
//NI_S32 NI_MDK_YHIST_CTR_Init(ISP_DEV IspDev);
//NI_S32 NI_MDK_YHIST_CTR_DeInit(ISP_DEV IspDev);
//NI_S32 NI_MDK_YHIST_CTR_ResetParam(NI_U32 IspDev);
//NI_S32 NI_MDK_YHIST_CTR_SetUsrParam(ISP_DEV IspDev, YHIST_CTR_USR_PARAM_S *pstUsrParam);
//NI_S32 NI_MDK_YHIST_CTR_SetParam(ISP_DEV IspDev, YHIST_CTR_PARAM_S *pstYhistCtrParam);
//NI_S32 NI_MDK_YHIST_CTR_Process(ISP_DEV IspDev, AISP_GAIN_ADJ_INFO_S *pstGainAdjInfo, AISP_YHIST_INFO_S *pstYhist,NI_U16 **pu16HistTab);

/**********************DN***********************************/
NI_S32 NI_MDK_DN_MemInit(ISP_DEV IspDev);
NI_S32 NI_MDK_DN_MemDeInit(ISP_DEV IspDev);
NI_S32 NI_MDK_DN_Init(ISP_DEV IspDev);
NI_S32 NI_MDK_DN_Exit(ISP_DEV IspDev);
NI_S32 NI_MDK_DN_Reset(ISP_DEV IspDev);
NI_S32 NI_MDK_DN_SetDnAttr(ISP_DEV IspDev, DN_ATTR_S *pstDnAttr);
NI_S32 NI_MDK_DN_GetDnAttr(ISP_DEV IspDev, DN_ATTR_S *pstDnAttr);
NI_S32 NI_MDK_DN_Run(ISP_DEV IspDev, AISP_AWES_INFO_S *pstAwes, DN_SCNEN_INFO_S *pstScene, DN_RESULT_S *pstResult);
NI_S32 NI_MDK_DN_GetDbgInfo(NI_S32 s32IspDev, DN_DBG_INFO_S *pstDnDbgInfo);

/**********************ir_light***********************************/
NI_S32 NI_MDK_IR_LIGHT_MemInit(ISP_DEV IspDev);
NI_S32 NI_MDK_IR_LIGHT_MemDeInit(ISP_DEV IspDev);
NI_S32 NI_MDK_IR_LIGHT_Init(ISP_DEV IspDev);
NI_S32 NI_MDK_IR_LIGHT_Exit(ISP_DEV IspDev);
NI_S32 NI_MDK_IR_LIGHT_Reset(ISP_DEV IspDev);
NI_S32 NI_MDK_IR_LIGHT_SetLightAttr(ISP_DEV IspDev, IR_LIGHT_ATTR_S *pstLightAttr);
NI_S32 NI_MDK_IR_LIGHT_GetLightAttr(ISP_DEV IspDev, IR_LIGHT_ATTR_S *pstLightAttr);
NI_S32 NI_MDK_IR_LIGHT_Run(ISP_DEV IspDev, IR_LIGHT_SCNEN_INFO_S *pstScene, IR_LIGHT_RESULT_S *pstResult);

/**********************LED_LIGHT***********************************/
NI_S32 NI_MDK_LED_LIGHT_MemInit(ISP_DEV IspDev);
NI_S32 NI_MDK_LED_LIGHT_MemDeInit(ISP_DEV IspDev);
NI_S32 NI_MDK_LED_LIGHT_Init(ISP_DEV IspDev);
NI_S32 NI_MDK_LED_LIGHT_Exit(ISP_DEV IspDev);
NI_S32 NI_MDK_LED_LIGHT_Reset(ISP_DEV IspDev);
NI_S32 NI_MDK_LED_LIGHT_SetLightAttr(ISP_DEV IspDev, LED_LIGHT_ATTR_S *pstLightAttr);
NI_S32 NI_MDK_LED_LIGHT_GetLightAttr(ISP_DEV IspDev, LED_LIGHT_ATTR_S *pstLightAttr);
NI_S32 NI_MDK_LED_LIGHT_Run(ISP_DEV IspDev, LED_LIGHT_SCNEN_INFO_S *pstScene, LED_LIGHT_RESULT_S *pstResult);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

