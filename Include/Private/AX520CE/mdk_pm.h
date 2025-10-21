/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef __MDK_PM_H__
#define __MDK_PM_H__

#include "ni_comm_pm.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @description: 获取时钟配置策略
 * @param {PM_CLK_MGR_POLICY_E} *penClkMgrPolicy： 输出，时钟配置策略。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PM_GetClkMgrPolicy(PM_CLK_MGR_POLICY_E *penClkMgrPolicy);

/**
 * @description: 设置时钟配置策略
 * @param {PM_CLK_MGR_POLICY_E} *penClkMgrPolicy：输出，时钟配置策略。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PM_SetClkMgrPolicy(const PM_CLK_MGR_POLICY_E enClkMgrPolicy);

/**
 * @description: 获取时钟能力等级配置。PM_CLK_MGR_POLICY_LEVEL情况下使用。
 * @param {PM_CLK_CAP_LEVEL_E} *penClkCapLevel：输出，时钟能力等级。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PM_GetClkCapLevel(PM_CLK_CAP_LEVEL_E *penClkCapLevel);

/**
 * @description: 设置时钟能力等级配置。PM_CLK_MGR_POLICY_LEVEL情况下使用。
 * @param {PM_CLK_CAP_LEVEL_E} enClkCapLevel：输入，时钟能力等级。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PM_SetClkCapLevel(const PM_CLK_CAP_LEVEL_E enClkCapLevel);

/**
 * @description: 获取各个模块的时钟配置
 * @param {PM_CLOCK_CFG_S} *pstClkCfg：输出，各个模块的时钟配置。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PM_GetClkCfg(PM_CLOCK_CFG_S *pstClkCfg);

/**
 * @description: 设置各个模块的时钟配置，设置前请先调用NI_MDK_PM_GetClkCfg获取当前时钟配置。
 *               注意:
 * 请勿在具体业务模块开启后设置该模块的时钟频率或关闭时钟(尤其是具有访问AXI总线特性的模块)！！！
 * @param {PM_CLOCK_MASK_E}
 * enClkMask：输入，仅配置某个模块的掩码，支持多个模块或在一起操作,只有设置正确的模块mask，该模块的时钟配置才会生效。
 * @param {PM_CLOCK_CFG_S} *pstClkCfg：输入，时钟配置。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PM_SetClkCfg(PM_CLOCK_MASK_E enClkMask, const PM_CLOCK_CFG_S *pstClkCfg);

/**
 * @description: 获取sensor模块的时钟配置。
 * @param {PM_CLOCK_SENSOR_S} pstClkSensor：输出，sensor时钟配置。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PM_GetSensorClk(PM_CLOCK_SENSOR_S *pstClkSensorCfg);

/**
 * @description: 设置sensor模块的时钟配置
 * 请勿在具体业务模块开启后设置该模块的时钟频率或关闭时钟(尤其是具有访问AXI总线特性的模块)！！！
 * @param {PM_CLOCK_SENSOR_S} pstClkSensor：输入，sensor时钟配置。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PM_SetSensorClk(const PM_CLOCK_SENSOR_S *pstClkSensorCfg);

/**
 * @description: 获取各个PLL的配置参数（高级接口，通常用户不需要调用）。
 * @param {NI_S32} s32PllId：输入，pllid 取值范围[0,CLOCK_MAX_PLL_NUM]。
 * @param {PM_CLOCK_PLL_S} *pstClkPll：输出，时钟配置。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PM_GetPll(NI_S32 s32PllId, PM_CLOCK_PLL_S *pstClkPll);

/**
 * @description:
 * 高级接口，通常用户不需要调用。设置各个PLL的参数。PLL时钟是芯片稳定运行的关键参数，不推荐客户自行修改PLL配置。
 * @param {NI_S32} s32PllId：输入，pllid 取值范围[0,CLOCK_MAX_PLL_NUM]。
 * @param {PM_CLOCK_PLL_S} *pstClkPll：输入，时钟配置。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PM_SetPll(NI_S32 s32PllId, const PM_CLOCK_PLL_S *pstClkPll);

/**
 * @description: 高级接口，通常用户不需要调用。获取模块参数。
 * @param {PM_CLOCK_MOD_PARAM_S} *pstModParam：输出，模块参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PM_GetModParam(PM_CLOCK_MOD_PARAM_S *pstModParam);

/**
 * @description: 高级接口，通常用户不需要调用。设置模块参数。
 * @param {PM_CLOCK_MOD_PARAM_S} *pstModParam：输入，模块参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PM_SetModParam(const PM_CLOCK_MOD_PARAM_S *pstModParam);

/**
 * @description: 高级接口，通常用户不需要调用。获取电压参数。
 * @param {PM_VOLTAGE_TYPE_E} enType：输入，模块参数。
 * @param {PM_VOLTAGE_ATTR_S} *pstVolAttr：输出，模块参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PM_GetVoltageAttr(PM_VOLTAGE_TYPE_E enType, PM_VOLTAGE_ATTR_S *pstVolAttr);

/**
 * @description: 高级接口，通常用户不需要调用。设置电压参数。
 * @param {PM_VOLTAGE_TYPE_E} enType：输入，模块参数。
 * @param {PM_VOLTAGE_ATTR_S} *pstVolAttr：输入，模块参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PM_SetVoltageAttr(PM_VOLTAGE_TYPE_E enType, const PM_VOLTAGE_ATTR_S *pstVolAttr);

/**
 * @description: 高级接口，通常用户不需要调用。获取频率参数。
 * @param {NI_U32} u32CpuId：输入，模块参数。
 * @param {PM_CPU_FREQ_E} *penCpuFreq：输出，模块参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PM_GetCpuFreq(NI_U32 u32CpuId, PM_CPU_FREQ_E *penCpuFreq);

/**
 * @description: 高级接口，通常用户不需要调用。设置频率参数。
 * @param {NI_U32} u32CpuId：输入，模块参数。
 * @param {PM_CPU_FREQ_E} enCpuFreq：输入，模块参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PM_SetCpuFreq(NI_U32 u32CpuId, const PM_CPU_FREQ_E enCpuFreq);

/**
 * @description: 高级接口，通常用户不需要调用。获取策略参数。
 * @param {NI_U32} u32CpuId：输入，模块参数。
 * @param {PM_CPU_GOVERNORS_E} *penCpuGovernors：输出，模块参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PM_GetCpuGovernors(NI_U32 u32CpuId, PM_CPU_GOVERNORS_E *penCpuGovernors);

/**
 * @description: 高级接口，通常用户不需要调用。设置策略参数。
 * @param {NI_U32} u32CpuId：输入，模块参数。
 * @param {PM_CPU_GOVERNORS_E} enCpuGovernors：输入，模块参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PM_SetCpuGovernors(NI_U32 u32CpuId, const PM_CPU_GOVERNORS_E enCpuGovernors);

/**
 * @description: 高级接口，通常用户不需要调用。停止或者启动CPU。
 * @param {NI_U32} u32CpuId：输入，模块参数。
 * @param {NI_BOOL} bHalt：输入，模块参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PM_SetCpuHalt(NI_U32 u32CpuId, NI_BOOL bHalt);

/**
 * @description: 高级接口，通常用户不需要调用。获取CPU当前状态。
 * @param {NI_U32} u32CpuId：输入，模块参数。
 * @param {NI_BOOL} pbHalt：输出，模块参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PM_GetCpuHalt(NI_U32 u32CpuId, NI_BOOL *pbHalt);

/**
 * @description: 高级接口，通常用户不需要调用。获取芯片温度。
 * @param {NI_U32} u32CpuId：输出，模块参数。
 * @param {NI_U32} pu32Temperature：输入，模块参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PM_GetChipTemperature(NI_U32 u32ChipId, NI_U32 *pu32Temperature);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
