/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

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

/**
 * @description: 创建智能编码通道。
 * @param {SMART_ENC_HANDLE_ATTR_S} *pstHandleAttr: 输入，智能编码功能属性结构体指针
 * @return {SMART_ENC_HANDLE}: 返回智能编码句柄
 */
SMART_ENC_HANDLE NI_MDK_SMART_ENC_Create(const SMART_ENC_HANDLE_ATTR_S *pstHandleAttr);

/**
 * @description: 设置智能编码的编码参数。
 * @param {SMART_ENC_HANDLE} hHandle: 输入，智能编码hHandle号
 * @param {SMART_ENC_PARAM} *pstParam: 输入，智能编码配置参数结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_SMART_ENC_SetParam(SMART_ENC_HANDLE hHandle, const SMART_ENC_PARAM_S *pstParam);

/**
 * @description: 获取智能编码的编码参数。
 * @param {SMART_ENC_HANDLE} hHandle: 输入，智能编码hHandle号
 * @param {SMART_ENC_PARAM} *pstParam: 输出，智能编码配置参数结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_SMART_ENC_GetParam(SMART_ENC_HANDLE hHandle, SMART_ENC_PARAM_S *pstParam);

/**
 * @description: 设置智能编码的动态场景属性。
 * @param {SMART_ENC_HANDLE} hHandle: 输入，智能编码hHandle号
 * @param {SMART_ENC_MOTION_ATTR} *pstMotionAttr: 输入，智能编码动态场景结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_SMART_ENC_SetMotionAttr(SMART_ENC_HANDLE hHandle, const SMART_ENC_MOTION_ATTR_S *pstMotionAttr);

/**
 * @description: 获取智能编码的编码参数。
 * @param {SMART_ENC_HANDLE} hHandle: 输入，智能编码hHandle号
 * @param {SMART_ENC_MOTION_ATTR} *pstMotionAttr: 输出，智能编码动态场景结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_SMART_ENC_GetMotionAttr(SMART_ENC_HANDLE hHandle, SMART_ENC_MOTION_ATTR_S *pstMotionAttr);

/**
 * @description: 开启智能编码通道接收输入图像并进行编码。
 * @param {SMART_ENC_FRAME_INFO_S} pstFrameInfo: 输入，图像信息和相关参数结构体指针
 * @param {SMART_ENC_RESULT_S} pstResult: 输出，码控参数结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_SMART_ENC_Process(SMART_ENC_HANDLE hHandle, const SMART_ENC_FRAME_INFO_S *pstFrameInfo, SMART_ENC_RESULT_S *pstResult);

/**
 * @description: 销毁智能编码通道。
 * @param {SMART_ENC_HANDLE} hHandle: 输入，智能编码handle号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_SMART_ENC_Destroy(SMART_ENC_HANDLE hHandle);

/**
 * @description: 查看智能编码参数信息。
 * @param {SMART_ENC_HANDLE} hHandle: 输入，智能编码handle号
 * @param {SMART_ENC_STATUS_S} *pstStatus: 输出，智能编码参数信息
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_SMART_ENC_QueryDebugInfo(SMART_ENC_HANDLE hHandle, SMART_ENC_STATUS_S *pstStatus);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* End of #ifndef __MDK_SMART_ENC_H__ */