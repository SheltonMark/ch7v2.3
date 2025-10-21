/**************************************************************************************************
*
* Copyright (c) 2019-2025 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef __NI_MDK_AEC_H__
#define __NI_MDK_AEC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */
/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "ni_common.h"
#include "ni_type.h"
#include "ni_comm_aio.h"
#include "ni_comm_aenc.h"
#include "ni_comm_adec.h"
#include "aec_core.h"
#include "echo_cancellation.h"

/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */

/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */
#define NI_ERR_AEC_INVALID_CHNID NI_DEF_ERR(NI_ID_APP_AEC, EN_ERR_LEVEL_ERROR, ERR_INVALID_CHNID)
#define NI_ERR_AEC_ILLEGAL_PARAM NI_DEF_ERR(NI_ID_APP_AEC, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)
#define NI_ERR_AEC_CHN_EXIST     NI_DEF_ERR(NI_ID_APP_AEC, EN_ERR_LEVEL_ERROR, ERR_EXIST)
#define NI_ERR_AEC_CHN_UNEXIST   NI_DEF_ERR(NI_ID_APP_AEC, EN_ERR_LEVEL_ERROR, ERR_UNEXIST)
#define NI_ERR_AEC_NULL_PTR      NI_DEF_ERR(NI_ID_APP_AEC, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)
#define NI_ERR_AEC_NOMEN         NI_DEF_ERR(NI_ID_APP_AEC, EN_ERR_LEVEL_ERROR, ERR_NOMEM)

// 回声消除强度
typedef enum tagAecLevel {
    NI_AEC_LEVEL_NONE = 0,   /*不做回声消除*/
    NI_AEC_LEVEL_NORMAL,     /*中等强度回声消除*/
    NI_AEC_LEVEL_AGRRESSIVE, /*高强度回声消除，可能会有明显声音损失*/
    NI_AEC_LEVEL_BUTT
} NI_AEC_LEVEL_E;

// 兼容处理
#define NI_AEC_LEVE_E NI_AEC_LEVEL_E

// 时钟补偿配置
typedef struct tagAecSkewCfg {
    NI_BOOL bSkewEnable; /*是否打开时钟补偿处理*/
    NI_S32  s32Skew;     /*skew值，只有打开补偿才有效*/
} NI_AEC_SKEW_CFG_S;

/* 舒适度噪声强度 */
typedef enum tagAecComfortNoiseLevel {
    NI_AEC_COMFORT_NOISE_OFF = 0, /* 关闭 */
    NI_AEC_COMFORT_NOISE_LEVEL_1, /* 舒适度噪声等级1，等级越大强度越大 */
    NI_AEC_COMFORT_NOISE_LEVEL_2, /* 舒适度噪声等级2 */
    NI_AEC_COMFORT_NOISE_LEVEL_3, /* 舒适度噪声等级3 */
    NI_AEC_COMFORT_NOISE_BUTT
} NI_AEC_COMFORT_NOISE_LEVEL_E;

/* 回声消除配置参数 */
typedef struct tagAecCfg {
    NI_AEC_LEVEL_E               enAecLevel;          /* 回声消除强度 */
    NI_AEC_SKEW_CFG_S            stSkewCfg;           /* 时钟补偿配置 */
    NI_S16                       s16FarRefDelay;      /* 远端参考信号系统时延, Range:[-1, 500], 其中 -1 为自适应计算远端参考信号系统时延 */
    NI_AEC_COMFORT_NOISE_LEVEL_E enComfortNoiseLevel; /* 舒适度噪声强度 */
    NI_BOOL                      bExtFilterEnable;    /* 是否启用扩展滤波器，默认不开启，且从核AEC处理本参数无效(不支持开启)
                                                         主要用于在回声路径较长或延迟较大等复杂场景下提高回声消除的效果，适合对通话质量要求较高的环境。
                                                         但是，启用后会带来额外的计算开销，需要在实际应用中结合设备性能和应用场景综合考量。 */
} NI_AEC_CFG_S;

/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */

/**
 * @description: 初始化AEC库。
 * @param {AEC_CHN} AecChn: 输入参数，AEC通道号，取值0。
 * @param {NI_STREAM_PARAM_S} *pstStreamParam: 输入参数，用户传入的初始化参数。
 * @return {*}: 0: 成功，非0: 失败
 */
NI_S32 NI_MDK_AEC_Init(AEC_CHN AecChn, NI_STREAM_PARAM_S *pstStreamParam);

/**
 * @description: AEC库反初始化，释放资源。
 * @param {AEC_CHN} AecChn: 输入参数，AEC通道号，取值0。
 * @return {*}: 0: 成功，非0: 失败
 */
NI_S32 NI_MDK_AEC_DeInit(AEC_CHN AecChn);

/**
 * @description: 回声消除处理函数，传入原始数据，输出处理好的数据。
 * @param {AEC_CHN} AecChn: 输入参数，AEC通道号，取值0。
 * @param {AUDIO_STREAM_S} *pstSrcStream: 输入参数，待处理的数据。
 * @param {AUDIO_STREAM_S} *pstResStream: 输出参数，处理好的数据。
 * @return {*}: 0: 成功，非0: 失败
 */
NI_S32 NI_MDK_AEC_Process(AEC_CHN AecChn, AUDIO_STREAM_S *pstSrcStream, AUDIO_STREAM_S *pstResStream);

/**
 * @description: 回声消除扩展处理函数，传入原始数据和参考帧数据，输出处理好的数据。
 * @param {AEC_CHN} AecChn: 输入参数，AEC通道号，取值0。
 * @param {AUDIO_STREAM_S} *pstSrcStream: 输入参数，待处理的数据。
 * @param {AUDIO_STREAM_S} *pstRefStream: 输入参数，参考帧的数据。
 * @param {AUDIO_STREAM_S} *pstResStream: 输出参数，处理好的数据。
 * @return {*}: 0: 成功，非0: 失败
 */
NI_S32 NI_MDK_AEC_ProcessExt(AEC_CHN AecChn, AUDIO_STREAM_S *pstSrcStream, AUDIO_STREAM_S *pstRefStream, AUDIO_STREAM_S *pstResStream);

/**
 * @description: 高级用户接口，允许用户自定义AEC参数。对AEC不够了解的用户，建议不使用。
 * @param {AEC_CHN} AecChn: 输入参数，AEC通道号，取值0。
 * @param {NI_AEC_CFG_S} *pstAecCfg: 输入参数，用户传入的初始化参数。
 * @return {*}: 0: 成功，非0: 失败
 */
NI_S32 NI_MDK_AEC_SetConfig(AEC_CHN AecChn, NI_AEC_CFG_S *pstAecCfg);

/**
 * @description: 获取AEC参数。
 * @param {AEC_CHN} AecChn: 输入参数，AEC通道号，取值0。
 * @param {NI_AEC_CFG_S} *pstAecCfg: 输出参数，AEC参数。
 * @return {*}: 0: 成功，非0: 失败
 */
NI_S32 NI_MDK_AEC_GetConfig(AEC_CHN AecChn, NI_AEC_CFG_S *pstAecCfgExt);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* End of #ifndef __NI_MDK_AEC_H__ */
