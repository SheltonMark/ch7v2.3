/* 
 * mdk_ldc.h -- ldc模块对外提供的API头文件
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */

#ifndef __MDK_LDC_H__
#define __MDK_LDC_H__


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "ni_type.h"
#include "ni_comm_ldc.h"


#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */
/**
 * @description: 设置设备属性
 * @param {LDC_DEV} LdcDev：输入，通道号。取值范围[0,LDC_MAX_DEV_NUM)
 * @param {LDC_DEV_ATTR_S} *pstDevAttr：输入，设备属性。
 * @return {*}：0：成功，非0：失败
 */
NI_S32 NI_MDK_LDC_SetDevAttr(LDC_DEV LdcDev, const LDC_DEV_ATTR_S *pstDevAttr);

/**
 * @description: 获取设备属性
 * @param {LDC_DEV} LdcDev：输入，通道号。取值范围[0,LDC_MAX_DEV_NUM)
 * @param {LDC_DEV_ATTR_S} *pstDevAttr：输出，设备属性。
 * @return {*}：0：成功，非0：失败
 */
NI_S32 NI_MDK_LDC_GetDevAttr(LDC_DEV LdcDev, LDC_DEV_ATTR_S *pstDevAttr);

/**
 * @description: 使能设备
 * @param {LDC_DEV} LdcDev：输入，通道号。取值范围[0,LDC_MAX_DEV_NUM)
 * @return {*}：0：成功，非0：失败
 */
NI_S32 NI_MDK_LDC_EnableDev(LDC_DEV LdcDev);

/**
 * @description: 去使能设备
 * @param {LDC_DEV} LdcDev：输入，通道号。取值范围[0,LDC_MAX_DEV_NUM)
 * @return {*}：0：成功，非0：失败
 */
NI_S32 NI_MDK_LDC_DisableDev(LDC_DEV LdcDev);

/**
 * @description: 单次矫正图像接口（阻塞）
 * @param {LDC_DEV} LdcDev：输入，通道号。取值范围[0,LDC_MAX_DEV_NUM)
 * @param {LDC_CORRECT_PARAM_S} *pstCorrectParam：输入，矫正图像输入图像与输出图像信息
 * @return {*}：0：成功，非0：失败
 */
NI_S32 NI_MDK_LDC_CorrectFrame(LDC_DEV LdcDev, LDC_CORRECT_PARAM_S *pstCorrectParam);

/**
 * @description: 单次矫正图像接口（非阻塞，帧与map同步）
 * @param {LDC_DEV} LdcDev：输入，通道号。取值范围[0,LDC_MAX_DEV_NUM)
 * @param {LDC_CORRECT_PARAM_EX_S} *pstCorrectFrameEx：输入，矫正图像输入图像与输出图像信息
 * @return {*}：0：成功，非0：失败
 */
NI_S32 NI_MDK_LDC_CorrectFrameEx(LDC_DEV LdcDev, LDC_CORRECT_PARAM_EX_S *pstCorrectFrameEx);

/**
 * @description: 获取NI_MDK_LDC_CorrectFrameEx接口矫正后的数据
 * @param {LDC_DEV} LdcDev：输入，通道号。取值范围[0,LDC_MAX_DEV_NUM)
 * @param {LDC_CORRECT_PARAM_EX_S} *pstCorrectFrameEx：输出，矫正图像输入图像与输出图像信息
 * @return {*}：0：成功，非0：失败
 */
NI_S32 NI_MDK_LDC_GetCorrectedFrame(LDC_DEV LdcDev, LDC_CORRECT_PARAM_EX_S *pstCorrectFrameEx);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of #ifndef __MDK_LDC_H__ */


