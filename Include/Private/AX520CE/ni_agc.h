/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef __NI_AGC_H__
#define __NI_AGC_H__

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
#include "agc.h"  //老版本接口
/* ========================================================================== */
/*                         宏和类型定义区                                     */
/* ========================================================================== */

#define NI_ERR_AGC_INVALID_CHNID NI_DEF_ERR(NI_ID_APP_AGC, EN_ERR_LEVEL_ERROR, ERR_INVALID_CHNID)
#define NI_ERR_AGC_ILLEGAL_PARAM NI_DEF_ERR(NI_ID_APP_AGC, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)
#define NI_ERR_AGC_CHN_EXIST     NI_DEF_ERR(NI_ID_APP_AGC, EN_ERR_LEVEL_ERROR, ERR_EXIST)
#define NI_ERR_AGC_CHN_UNEXIST   NI_DEF_ERR(NI_ID_APP_AGC, EN_ERR_LEVEL_ERROR, ERR_UNEXIST)
#define NI_ERR_AGC_NULL_PTR      NI_DEF_ERR(NI_ID_APP_AGC, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)
#define NI_ERR_AGC_NOMEN         NI_DEF_ERR(NI_ID_APP_AGC, EN_ERR_LEVEL_ERROR, ERR_NOMEM)

// AGC工作模式，自适应放大/固定增益放大
typedef enum tagAgcMode {
    NI_AGC_MODE_ADAPTIVE = 2,
    NI_AGC_MODE_FIXED    = 3,
    NI_AGC_MODE_BUTT
} NI_AGC_MODE_E;

// 增益控制强度
typedef enum tagAgcLevel {
    NI_AGC_LEVEL_GRACEFUL,   /*低强度放大*/
    NI_AGC_LEVEL_NORMAL,     /*正常放大，default模式*/
    NI_AGC_LEVEL_STRONG,     /*高强度放大*/
    NI_AGC_LEVEL_AGRRESSIVE, /*激进放大*/
    NI_AGC_LEVEL_HUGE,       /*超高强度放大*/
    NI_AGC_LEVEL_MAX,        /*最大增益*/
    NI_AGC_LEVEL_BUTT
} NI_AGC_LEVE_E;

// 用户自定义AGC工作模式和增益数值
typedef struct tagAgcGain {
    NI_AGC_MODE_E enAgcMode;     /*自适应增益，还是固定增益模式*/
    NI_S16        s16TargetDbfs; /*最大增益和满量程比的增益数，默认3dB，Range:[0, 31]*/
    NI_S16        s16GaindB;     /*数字放大增益, 默认9dB*/
} NI_AGC_GAIN_S;

// 简便模式，还是用户自定义模式（联合体）
typedef union tagAgcIntensity {
    NI_AGC_LEVE_E enAgcLevel;    /*bSimpleMode = 1, 简便模式下，强度等级*/
    NI_AGC_GAIN_S stAgcModeGain; /*bSimpleMode = 0, 用户自定义模式下，AGC详细配置*/
} NI_AGC_INTENSITY_U;

// AGC配置
typedef struct tagAgcCfg {
    NI_BOOL            bSimpleMode;    /*简便模式，还是用户自定义模式 */
    NI_AGC_INTENSITY_U unAgcIntensity; /*增益强度配置*/
} NI_AGC_CFG_S;

/* ========================================================================== */
/*                         函数定义区                                     */
/* ========================================================================== */

//------------------以下4个NI_MDK_开头的为AGC新接口，注意：新老两套接口不可以混用----------------------
//------------------对于新接入客户，建议直接使用新接口，更简单易用，风格更统一。------------------------
/**
 * @description: 初始化AEC库。
 * @param {AGC_CHN} AgcChn: 输入参数，AGC通道号，取值0~5。
 * @param {NI_STREAM_PARAM_S} *pstStreamParam: 输入参数，用户传入的初始化参数。
 * @return {*}: 0: 成功，非0: 失败
 */
NI_S32 NI_MDK_AGC_Init(AGC_CHN AgcChn, NI_STREAM_PARAM_S *pstStreamParam);

/**
 * @description: AGC库反初始化，释放资源。
 * @param {AGC_CHN} AgcChn: 输入参数，AGC通道号，取值0~5。
 * @return {*}: 成功，非0: 失败
 */
NI_S32 NI_MDK_AGC_DeInit(AGC_CHN AgcChn);

/**
 * @description: 回声消除处理函数，传入原始数据，输出处理好的数据。
 * @param {AGC_CHN} AgcChn: 输入参数，AGC通道号，取值0~5。
 * @param {AUDIO_STREAM_S} *pstSrcStream: 输入参数，待处理的数据。
 * @param {AUDIO_STREAM_S} *pstResStream: 输出参数，处理好的数据。
 * @return {*}: 0: 成功，非0: 失败
 */
NI_S32 NI_MDK_AGC_Process(AGC_CHN AgcChn, AUDIO_STREAM_S *pstSrcStream, AUDIO_STREAM_S *pstResStream);

/**
 * @description: 高级用户接口，允许用户自定义AGC参数。对AGC不够了解的用户，建议不使用。
 * @param {AGC_CHN} AgcChn: 输入参数，AGC通道号，取值0~5。
 * @param {NI_AGC_CFG_S} *pstAgcCfg: 输入参数，用户传入的初始化参数。
 * @return {*}: 0: 成功，非0: 失败
 */
NI_S32 NI_MDK_AGC_SetConfig(AGC_CHN AgcChn, NI_AGC_CFG_S *pstAgcCfg);

#ifdef __cplusplus
}
#endif

#endif /* End of #ifndef __NI_AGC_H__ */