/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef __MDK_CODEC_H__
#define __MDK_CODEC_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "ni_comm_codec.h"

/**
 * @description: 设置codec的通用属性
 * @param {CODEC_PUB_ATTR_S} *pstPubAttr：输入，公共属性。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_CODEC_SetPubAttr(CODEC_PUB_ATTR_S *pstPubAttr);

/**
 * @description: 获取codec的通用属性
 * @param {CODEC_PUB_ATTR_S} *pstPubAttr：输出，公共属性。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_CODEC_GetPubAttr(CODEC_PUB_ATTR_S *pstPubAttr);

/**
 * @description: 设置通道增益属性
 * @param {CODEC_CHANNEL_E} enChannel：输入，通道(left/right)。
 * @param {CODEC_GAIN_ATTR_S} *pstGainAttr：输入，通道增益属性。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_CODEC_SetGainAttr(CODEC_CHANNEL_E enChannel, CODEC_GAIN_ATTR_S *pstGainAttr);

/**
 * @description: 获取通道增益属性
 * @param {CODEC_CHANNEL_E} enChannel：输入，通道(left/right)。
 * @param {CODEC_GAIN_ATTR_S} *pstGainAttr：输入，通道增益属性。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_CODEC_GetGainAttr(CODEC_CHANNEL_E enChannel, CODEC_GAIN_ATTR_S *pstGainAttr);

/**
 * @description: 开启静音模式
 * @param {CODEC_ANALOG_CONVERT_E} enConvert：输入，DAC/ADC。
 * @param {CODEC_CHANNEL_E} enChannel：输入，通道(left/right)。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_CODEC_EnableMute(CODEC_ANALOG_CONVERT_E enConvert, CODEC_CHANNEL_E enChannel);

/**
 * @description: 关闭静音模式
 * @param {CODEC_ANALOG_CONVERT_E} enConvert：输入，DAC/ADC。
 * @param {CODEC_CHANNEL_E} enChannel：输入，通道(left/right)。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_CODEC_DisableMute(CODEC_ANALOG_CONVERT_E enConvert, CODEC_CHANNEL_E enChannel);

/**
 * @description: 设置ALC（automatic level control）属性
 * @param {CODEC_ALC_ATTR_S} *pstAlcAttr：输入，ALC属性。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_CODEC_SetAlcAttr(const CODEC_ALC_ATTR_S *pstAlcAttr);

/**
 * @description: 获取ALC（automatic level control）属性
 * @param {CODEC_ALC_ATTR_S} *pstAlcAttr：输出，ALC属性。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_CODEC_GetAlcAttr(CODEC_ALC_ATTR_S *pstAlcAttr);

/**
 * @description: 设置codec 模拟通道采样输入的使能开关
 * @param {CODEC_CHANNEL_E} enChannel：输入，通道(left/right)。
 * @param {NI_BOOL} bEn：输入，开关。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_CODEC_SetAinEn(CODEC_CHANNEL_E enChannel, NI_BOOL bEn);

/**
 * @description: 设置codec 模拟通道输出播放的使能开关
 * @param {CODEC_CHANNEL_E} enChannel：输入，通道(left/right)。
 * @param {NI_BOOL} bEn：输入，开关。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_CODEC_SetAoutEn(CODEC_CHANNEL_E enChannel, NI_BOOL bEn);

/**
 * @description: 设置codec的ADC扩展属性
 * @param {CODEC_ADC_EXT_ATTR_S} *pstAdcExtAttr：输入，扩展属性（DEM/DWA.HPF/POP etc）。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_CODEC_SetAdcExtAttr(const CODEC_ADC_EXT_ATTR_S *pstAdcExtAttr);

/**
 * @description: 获取ADC扩展属性
 * @param {CODEC_ADC_EXT_ATTR_S} *pstAdcExtAttr：输出，扩展属性（DEM/DWA.HPF/POP etc）。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_CODEC_GetAdcExtAttr(CODEC_ADC_EXT_ATTR_S *pstAdcExtAttr);

/**
 * @description: 设置DAC的扩展属性
 * @param {CODEC_DAC_EXT_ATTR_S} *pstDacExtAttr：输入，扩展属性（DEM/DWA/MIX/POP etc）。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_CODEC_SetDacExtAttr(const CODEC_DAC_EXT_ATTR_S *pstDacExtAttr);

/**
 * @description: 设置DAC的扩展属性
 * @param {CODEC_DAC_EXT_ATTR_S} *pstDacExtAttr：输出，扩展属性（DEM/DWA/MIX/POP etc）。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_CODEC_GetDacExtAttr(CODEC_DAC_EXT_ATTR_S *pstDacExtAttr);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
