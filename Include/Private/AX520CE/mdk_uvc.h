/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef __MDK_UVC_H_
#define __MDK_UVC_H_

/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "ni_comm_uvc.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

NI_VOID *NI_MDK_UVC_OpenVideo(UVC_VIDEO_CONFIG *pstVideoConfig, NI_VOID *pData);
NI_VOID  NI_MDK_UVC_CloseVideo(NI_VOID *pHandle);

NI_VOID *NI_MDK_UVC_OpenAudio(UVC_AUDIO_CONFIG *pstAudioConfig, NI_VOID *pData);
NI_VOID  NI_MDK_UVC_CloseAudio(NI_VOID *pHandle);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MDK_UVC_H__ */