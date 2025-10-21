/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef __NI_ANS_H__
#define __NI_ANS_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "ni_type.h"
#include "ni_debug.h"
#include "ni_macro.h"
#include "ni_common.h"
#include "ni_comm_aio.h"
#include "ni_comm_aenc.h"
#include "ni_comm_adec.h"
/* ========================================================================== */
/*                         宏和类型定义区                                     */
/* ========================================================================== */

#define NI_ERR_ANS_INVALID_CHNID NI_DEF_ERR(NI_ID_APP_ANS, EN_ERR_LEVEL_ERROR, ERR_INVALID_CHNID)
#define NI_ERR_ANS_ILLEGAL_PARAM NI_DEF_ERR(NI_ID_APP_ANS, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)
#define NI_ERR_ANS_CHN_EXIST     NI_DEF_ERR(NI_ID_APP_ANS, EN_ERR_LEVEL_ERROR, ERR_EXIST)
#define NI_ERR_ANS_CHN_UNEXIST   NI_DEF_ERR(NI_ID_APP_ANS, EN_ERR_LEVEL_ERROR, ERR_UNEXIST)
#define NI_ERR_ANS_NULL_PTR      NI_DEF_ERR(NI_ID_APP_ANS, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)
#define NI_ERR_ANS_NOMEN         NI_DEF_ERR(NI_ID_APP_ANS, EN_ERR_LEVEL_ERROR, ERR_NOMEM)

/*降噪强度*/
typedef enum tagAnsLevel{
    NI_ANS_LEVEL_GRACEFUL = 0,       /*轻度降噪*/
    NI_ANS_LEVEL_NORMAL,            /*中等强度降噪*/
    NI_ANS_LEVEL_AGRRESSIVE,         /*高强度降噪，可能会有明显声音损失*/
	NI_ANS_LEVEL_VERY_AGRRESSIVE,    /*超高强度降噪，可能会有明显声音损失*/
    NI_ANS_LEVEL_BUTT
}NI_ANS_LEVE_E;

/*回声消除配置参数*/
typedef struct tagAnsCfg {
    NI_ANS_LEVE_E enAnsLevel; /*降噪强度*/
} NI_ANS_CFG_S;

typedef void *AnsHandle;

/* ========================================================================== */
/*                         函数定义区                                     */
/* ========================================================================== */

//------------------以下4个NI_MDK_开头的为ANS新接口，注意：新老两套接口不可以混用----------------------
//------------------对于新接入客户，建议直接使用新接口，更简单易用，风格更统一。------------------------
/**
 * @description: 初始化AEC库。
 * @param {ANS_CHN} AnsChn: 输入参数，ANS通道号，取值0~5。
 * @param {NI_STREAM_PARAM_S} *pstStreamParam: 输入参数，用户传入的初始化参数。
 * @return {*}: 0: 成功，非0: 失败
 */
NI_S32 NI_MDK_ANS_Init(ANS_CHN AnsChn, NI_STREAM_PARAM_S *pstStreamParam);

/**
 * @description: ANS库反初始化，释放资源。
 * @param {ANS_CHN} AnsChn: 输入参数，ANS通道号，取值0~5。
 * @return {*}: 成功，非0: 失败
 */
NI_S32 NI_MDK_ANS_DeInit(ANS_CHN AnsChn);

/**
 * @description: 回声消除处理函数，传入原始数据，输出处理好的数据。
 * @param {ANS_CHN} AnsChn: 输入参数，ANS通道号，取值0~5。
 * @param {AUDIO_STREAM_S} *pstSrcStream: 输入参数，待处理的数据。
 * @param {AUDIO_STREAM_S} *pstResStream: 输出参数，处理好的数据。
 * @return {*}: 0: 成功，非0: 失败
 */
NI_S32 NI_MDK_ANS_Process(ANS_CHN AnsChn, AUDIO_STREAM_S *pstSrcStream, AUDIO_STREAM_S *pstResStream);

/**
 * @description: 高级用户接口，允许用户自定义ANS参数。对ANS不够了解的用户，建议不使用。
 * @param {ANS_CHN} AnsChn: 输入参数，ANS通道号，取值0~5。
 * @param {NI_ANS_CFG_S} *pstAnsCfg: 输入参数，用户传入的初始化参数。
 * @return {*}: 0: 成功，非0: 失败
 */
NI_S32 NI_MDK_ANS_SetConfig(ANS_CHN AnsChn, NI_ANS_CFG_S *pstAnsCfg);

#ifdef __cplusplus
}
#endif

#endif /* End of #ifndef __NI_ANS_H__ */