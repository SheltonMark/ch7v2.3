/**************************************************************************************************
*
* Copyright (c) 2019-2025 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef __NI_HPF_H__
#define __NI_HPF_H__

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
#include "ni_comm_aenc.h"

#define NI_ERR_HPF_INVALID_CHNID NI_DEF_ERR(NI_ID_APP_HPF, EN_ERR_LEVEL_ERROR, ERR_INVALID_CHNID)
#define NI_ERR_HPF_ILLEGAL_PARAM NI_DEF_ERR(NI_ID_APP_HPF, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)
#define NI_ERR_HPF_CHN_EXIST     NI_DEF_ERR(NI_ID_APP_HPF, EN_ERR_LEVEL_ERROR, ERR_EXIST)
#define NI_ERR_HPF_CHN_UNEXIST   NI_DEF_ERR(NI_ID_APP_HPF, EN_ERR_LEVEL_ERROR, ERR_UNEXIST)
#define NI_ERR_HPF_NULL_PTR      NI_DEF_ERR(NI_ID_APP_HPF, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)
#define NI_ERR_HPF_NOMEN         NI_DEF_ERR(NI_ID_APP_HPF, EN_ERR_LEVEL_ERROR, ERR_NOMEM)

/* HPF LEVEL */
typedef enum tagHpfLevel {
    NI_HPF_LEVEL_NORMAL = 0, /* 适度滤波，主要去除低频噪声 */
    NI_HPF_LEVEL_AGRRESSIVE, /* 高强度滤波，有可能会影响到人声频段 */
    NI_HPF_LEVEL_BUTT
} NI_HPF_LEVE_E;

typedef struct tagHpfCfg {
    NI_HPF_LEVE_E enHpfLevel; /*滤波强度*/
} NI_HPF_CFG_S;

/**
 * @description: HPF初始化。
 * @param {HpfChn} HpfChn: 输入参数，HPF通道号，取值0~5。
 * @param {NI_STREAM_PARAM_S} stHpfCfg: 输入参数，用户传入的初始化参数。。
 * @return {*}: 0: 成功，非0: 失败
 */
NI_S32 NI_MDK_HPF_Init(HPF_CHN HpfChn, NI_STREAM_PARAM_S *pstStreamParam);

/**
 * @description: HPF处理数据。
 * @param {HpfChn} HpfChn: 输入参数，HPF通道号，取值0~5。
 * @param {AUDIO_STREAM_S} *pstSrcStream: 输入/出参数，处理的数据。
 * @return {*}: 0: 成功，非0: 失败
 */
NI_S32 NI_MDK_HPF_Process(HPF_CHN HpfChn, AUDIO_STREAM_S *pstSrcStream);

/**
 * @description: HPF销毁，释放资源。
 * @param {HpfChn} HpfChn: 输入参数，HPF通道号，取值0~5。
 * @return {*}: 成功，非0: 失败
 */
NI_S32 NI_MDK_HPF_DeInit(HPF_CHN HpfChn);

/**
 * @description: 高级用户接口，允许用户自定义HPF参数。
 * @param {HpfChn} HpfChn: 输入参数，HPF通道号，取值0~5。
 * @param {NI_HPF_CFG_S} *pstHpfCfg: 输入参数，用户传入的配置参数。
 * @return {*}: 0: 成功，非0: 失败
 */
NI_S32 NI_MDK_HPF_SetConfig(HPF_CHN HpfChn, NI_HPF_CFG_S *pstHpfCfg);

/**
 * @description: 高级用户接口，允许用户自定义HPF参数。
 * @param {HpfChn} HpfChn: 输入参数，HPF通道号，取值0~5。
 * @param {NI_HPF_CFG_S} *pstHpfCfg: 输出参数
 * @return {*}: 0: 成功，非0: 失败
 */
NI_S32 NI_MDK_HPF_GetConfig(HPF_CHN HpfChn, NI_HPF_CFG_S *pstHpfCfg);

// 接口兼容性定义
#define NI_MDK_HPF_Deinit NI_MDK_HPF_DeInit

#ifdef __cplusplus
}
#endif

#endif /* End of #ifndef __NI_HPF_H__ */