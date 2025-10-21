/*******************************************************************************
* Copyright (C) 2020- 2025 ZheJiang Chipup Technology CO.,LTD. 
*
* 文件名 : mdk_smart_enc.h
*
* 作  者 : Chipup Software Team
* 版  本 : V1.0.0
* 日  期 : 2021年10月21日 Create
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
* 日    期:
* 修订版本:
* 作    者:
* 修订备注:
*******************************************************************************/
#ifndef __MDK_SMART_ENC_H__
#define __MDK_SMART_ENC_H__


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "ni_comm_smart_enc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/* ========================================================================== */
/*                               函数声明区                                   */
/* ========================================================================== */

SMART_ENC_HANDLE NI_MDK_SMART_ENC_Create(SMART_ENC_CREATE_S *pstCreate);

NI_S32 NI_MDK_SMART_ENC_Destroy(SMART_ENC_HANDLE hHandle);

NI_S32 NI_MDK_SMART_ENC_SetCfg(SMART_ENC_HANDLE hHandle, SMART_ENC_SET_CFG_S *pstEncSetCfg);

NI_S32 NI_MDK_SMART_ENC_Process(SMART_ENC_HANDLE hHandle, SMART_ENC_FRAME_INFO_S *pstDetectInfo, SMART_ENC_RESULT_S *pstResult);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* End of #ifndef __MDK_SMART_ENC_H__ */