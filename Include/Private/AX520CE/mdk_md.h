/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _MDK_MD_H_
#define _MDK_MD_H_

#include "ni_type.h"
#include "ni_comm_md.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @description: 初始化。
 * @param {MD_CHN} MdChn：输入，ChnID号。
 * @param {MD_CHN_ATTR_S} *pstChnAttr：输入，通道属性。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_MD_Init(MD_CHN MdChn, MD_CHN_ATTR_S *pstChnAttr);

/**
 * @description: 去初始化。
 * @param {MD_CHN} MdChn：输入，ChnID号。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_MD_DeInit(MD_CHN MdChn);

/**
 * @description: 获取工作模式。
 * @param {MD_CHN} MdChn：输入，ChnID号。
 * @param {MD_WORK_MODE_S} *pstWorkMode：输出，工作模式。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_MD_GetWorkMode(MD_CHN MdChn, MD_WORK_MODE_S *pstWorkMode);

/**
 * @description: 设置工作模式。
 * @param {MD_CHN} MdChn：输入，ChnID号。
 * @param {MD_WORK_MODE_S} *pstWorkMode：输入，工作模式。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_MD_SetWorkMode(MD_CHN MdChn, MD_WORK_MODE_S *pstWorkMode);

/**
 * @description: 设置通道属性。
 * @param {MD_CHN} MdChn：输入，ChnID号。
 * @param {MD_CHN_ATTR_S} *pstChnAttr：输入，通道属性。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_MD_SetChnAttr(MD_CHN MdChn, const MD_CHN_ATTR_S *pstChnAttr);

/**
 * @description: 获取通道属性。
 * @param {MD_CHN} MdChn：输入，ChnID号。
 * @param {MD_CHN_ATTR_S} *pstChnAttr：输出，通道属性。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_MD_GetChnAttr(MD_CHN MdChn, MD_CHN_ATTR_S *pstChnAttr);

/**
 * @description: 设置检测区域。
 * @param {MD_CHN} MdChn：输入，ChnID号。
 * @param {MD_DETECT_REGION_S} *pstDetectRegion：输入，检测区域。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_MD_SetDetectRegion(MD_CHN MdChn, const MD_DETECT_REGION_S *pstDetectRegion);

/**
 * @description: 获取检测区域。
 * @param {MD_CHN} MdChn：输入，ChnID号。
 * @param {NI_U8} *pu8MaxRegID：输出，配置检测区域次数。
 * @param {MD_DETECT_REGION_S} **pstDetectRegion：输出，检测区域。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_MD_GetDetectRegion(MD_CHN MdChn, NI_U8 *pu8MaxRegID, MD_DETECT_REGION_S **pstDetectRegion);

/**
 * @description: 检测运行。
 * @param {MD_CHN} MdChn：输入，ChnID号。
 * @param {VIDEO_FRAME_S} *pstFrame：输入，检测图像。
 * @param {MD_RESULT_S} *pstResult：输出，检测结果。
 * @param {NI_U32} u32TimeOut：输入，超时时间。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_MD_DetectFrame(MD_CHN MdChn, const VIDEO_FRAME_S *pstFrame, MD_RESULT_S *pstResult, NI_U32 u32TimeOut);

#ifdef __cplusplus
}
#endif

#endif
