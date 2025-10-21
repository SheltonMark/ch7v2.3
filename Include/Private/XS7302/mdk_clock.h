/* 
 * mdk_clock.h -- clock模块对外提供的API头文件
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */

#ifndef __MDK_CLOCK_H__
#define __MDK_CLOCK_H__

#include "ni_comm_clock.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

/**
 * @description: 获取各个模块的时钟配置
 * @param {CLOCK_CFG_S} *pstClkCfg：输出，各个模块的时钟配置。
 * @return {*}: 0：成功，非0：失败
 */    
NI_S32 NI_MDK_CLOCK_GetClkCfg(CLOCK_CFG_S *pstClkCfg);

/**
 * @description: 设置各个模块的时钟配置，设置前请先调用NI_MDK_CLOCK_GetClkCfg获取当前时钟配置。
 *               注意: 请勿在具体业务模块开启后设置该模块的时钟频率或关闭时钟(尤其是具有访问AXI总线特性的模块)，否则会造成系统死机！！！
 * @param {CLOCK_MASK_E} enClkMask：输入，仅配置某个模块的掩码，支持多个模块或在一起操作,只有设置正确的模块mask，该模块的时钟配置才会生效。
 * @param {CLOCK_CFG_S} *pstClkCfg：输入，时钟配置。
 * @return {*}: 0：成功，非0：失败
 */  
NI_S32 NI_MDK_CLOCK_SetClkCfg(CLOCK_MASK_E enClkMask, const CLOCK_CFG_S *pstClkCfg);

/**
 * @description: 获取各个PLL的配置参数（高级接口，通常用户不需要调用）。
 * @param {NI_S32} s32PllId：输入，pllid 取值范围[0,CLOCK_MAX_PLL_NUM]。
 * @param {CLOCK_PLL_S} *pstClkPll：输出，时钟配置。
 * @return {*}: 0：成功，非0：失败
 */  
NI_S32 NI_MDK_CLOCK_GetPll(NI_S32 s32PllId, CLOCK_PLL_S *pstClkPll);

/**
 * @description: 高级接口，通常用户不需要调用。设置各个PLL的参数。PLL时钟是芯片稳定运行的关键参数，不推荐客户自行修改PLL配置。
 * @param {NI_S32} s32PllId：输入，pllid 取值范围[0,CLOCK_MAX_PLL_NUM]。
 * @param {CLOCK_PLL_S} *pstClkPll：输入，时钟配置。
 * @return {*}: 0：成功，非0：失败
 */  
NI_S32 NI_MDK_CLOCK_SetPll(NI_S32 s32PllId, const CLOCK_PLL_S *pstClkPll);

/**
 * @description: 高级接口，通常用户不需要调用。获取模块参数。
 * @param {CLOCK_MOD_PARAM_S} *pstModParam：输出，模块参数。
 * @return {*}: 0：成功，非0：失败
 */  
NI_S32 NI_MDK_CLOCK_GetModParam(CLOCK_MOD_PARAM_S *pstModParam);

/**
 * @description: 高级接口，通常用户不需要调用。设置模块参数。
 * @param {CLOCK_MOD_PARAM_S} *pstModParam：输入，模块参数。
 * @return {*}: 0：成功，非0：失败
 */  
NI_S32 NI_MDK_CLOCK_SetModParam(const CLOCK_MOD_PARAM_S *pstModParam);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
