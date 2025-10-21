/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _MDK_VI_H_
#define _MDK_VI_H_

/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "ni_comm_vi.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @description: 设置VI设备属性。改变 VI 设备属性前，需禁用所有 VI 设备。
 * @param {VI_DEV} ViDev: 输入，vi设备号。取值范围：[0,VI_MAX_DEV_NUM)
 * @param {VI_DEV_ATTR_S} *pstDevAttr: 输入，vi设备属性配置信息
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VI_SetDevAttr(VI_DEV ViDev, const VI_DEV_ATTR_S *pstDevAttr);

/**
 * @description: 获取VI设备属性
 * @param {VI_DEV} ViDev: 输入，vi设备号。取值范围：[0,VI_MAX_DEV_NUM)
 * @param {VI_DEV_ATTR_S} *pstDevAttr: 输出，vi设备属性配置信息
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VI_GetDevAttr(VI_DEV ViDev, VI_DEV_ATTR_S *pstDevAttr);

/**
 * @description: 启动VI设备。启用VI设备前必须已经设置设备属性，否则返回失败。
 * @param {VI_DEV} ViDev: 输入，vi设备号。取值范围：[0,VI_MAX_DEV_NUM)
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VI_EnableDev(VI_DEV ViDev);

/**
 * @description: 禁用VI设备
 * @param {VI_DEV} ViDev: 输入，vi设备号。取值范围：[0,VI_MAX_DEV_NUM)
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VI_DisableDev(VI_DEV ViDev);

/**
 * @description: 切换VI设备工作模式。改变VI设备工作模式前，需禁用所有VI设备。
 * @param {VI_DEV} ViDev: 输入，vi设备号。取值范围：[0,VI_MAX_DEV_NUM)
 * @param {VI_WORK_PARAM_S} *pstWorkParam: 输入，vi工作模式參數配置信息
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VI_SwitchWorkMode(VI_DEV ViDev, const VI_WORK_PARAM_S *pstWorkParam);

/**
 * @description: 查询VI设备工作模式
 * @param {VI_DEV} ViDev: 输入，vi设备号。取值范围：[0,VI_MAX_DEV_NUM)
 * @param {VI_WORK_PARAM_S} *pstWorkParam: 输出，vi工作模式參數配置信息
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VI_QueryWorkMode(VI_DEV ViDev, VI_WORK_PARAM_S *pstWorkParam);

/**
 * @description: 设置VI MIPI时序相关参数。改变MIPI属性前，需禁用VI设备。
 * @param {VI_DEV} ViDev: 输入，vi设备号。取值范围：[0,VI_MAX_DEV_NUM)
 * @param {VI_MIPI_PARAM_S} *pstMipiParam: 输入，vi mipi參數配置信息
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VI_SetMipiParam(VI_DEV ViDev, const VI_MIPI_PARAM_S *pstMipiParam);

/**
 * @description: 获取VI MIPI时序配置参数
 * @param {VI_DEV} ViDev: 输入，vi设备号。取值范围：[0,VI_MAX_DEV_NUM)
 * @param {VI_MIPI_PARAM_S} *pstMipiParam: 输出，vi mipi參數配置信息
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VI_GetMipiParam(VI_DEV ViDev, VI_MIPI_PARAM_S *pstMipiParam);

/**
 * @description: 复位VI MIPI CSI
 * @param {VI_DEV} ViDev: 输入，vi设备号。取值范围：[0,VI_MAX_DEV_NUM)
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VI_ResetMipi(VI_DEV ViDev);

/**
 * @description: 查询VI调试信息
 * @param {VI_DEV} ViDev: 输入，vi设备号。取值范围：[0,VI_MAX_DEV_NUM)
 * @param {VI_DEBUG_INFO_S} *pstDebugInfo: 输出，vi调试信息
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VI_QueryDebugInfo(VI_DEV ViDev, VI_DEBUG_INFO_S *pstDebugInfo);

/**
 * @description: 设置VI线序。本接口对并口和BT时序sensor有效，对MIPI接口无效
 * @param {VI_DEV} ViDev: 输入，vi设备号。取值范围：[0,VI_MAX_DEV_NUM)
 * @param {NI_U8} au8DataLine: 输入，vi线序配置信息
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VI_SetDataLineSel(VI_DEV ViDev, NI_U8 au8DataLine[VI_MAX_DATA_LINE]);

/**
 * @description: 获取VI设备的文件描述符
 * @param {VI_DEV} ViDev: 输入，vi设备号。取值范围：[0,VI_MAX_DEV_NUM)
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VI_GetFd(VI_DEV ViDev);

/**
 * @description: 设置裁剪区域参数。该接口调用前，需提前设置设备属性并保证设备禁用
 * @param {VI_DEV} ViDev: 输入，vi设备号。取值范围：[0,VI_MAX_DEV_NUM)
 * @param {RECT_S} *pstCapRect: 输入，vi裁剪区域參數配置信息
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VI_SetCropRect(VI_DEV ViDev, const RECT_S *pstCapRect);

/**
 * @description: 获取裁剪区域参数
 * @param {VI_DEV} ViDev: 输入，vi设备号。取值范围：[0,VI_MAX_DEV_NUM)
 * @param {RECT_S} *pstCapRect: 输出，vi裁剪区域參數配置信息
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VI_GetCropRect(VI_DEV ViDev, RECT_S *pstCapRect);

/**
 * @description:
 * 用户送图像数据给VI及后级模块。用于灌源发送指定数量的图片，会循环发送。非阻塞接口，在设备使能前调用。只有设备0支持该功能
 * @param {VI_DEV} ViDev: 输入，vi设备号。取值范围：[0,VI_MAX_DEV_NUM)
 * @param {VI_RAW_DATA_INFO_S} *pstRawData: 输入，图像数据信息
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VI_SendRawData(VI_DEV ViDev, const VI_RAW_DATA_INFO_S *pstRawData);

/**
 * @description:
 * 用户送图像数据给VI及后级模块，单帧发送。用于灌源发送指定数量的图片，仅发送一次。阻塞接口，在设备使能后调用,仅能在非直通模式下工作。只有设备0支持该功能
 * @param {VI_DEV} ViDev: 输入，vi设备号。取值范围：[0,VI_MAX_DEV_NUM)
 * @param {VI_RAW_DATA_EX_S} *pstRawDataEx: 输入，图像数据信息
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VI_SendRawDataEx(VI_DEV ViDev, const VI_RAW_DATA_EX_S *pstRawDataEx);

/**
 * @description: 设置双目时序相关参数。仅设备0与设备1支持该功能
 * @param {VI_DEV} ViDev: 输入，vi设备号。取值范围：[0,VI_MAX_DEV_NUM)
 * @param {VI_MULTIEYE_PARAM_S} *pstMultiEyeParam: 输入，双目参数配置信息
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VI_SetMultiEyeParam(VI_DEV ViDev, const VI_MULTIEYE_PARAM_S *pstMultiEyeParam);

/**
 * @description: 获取双目时序相关参数
 * @param {VI_DEV} ViDev: 输入，vi设备号。取值范围：[0,VI_MAX_DEV_NUM)
 * @param {VI_MULTIEYE_PARAM_S} *pstMultiEyeParam: 输出，双目参数配置信息
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VI_GetMultiEyeParam(VI_DEV ViDev, VI_MULTIEYE_PARAM_S *pstMultiEyeParam);

/**
 * @description:
 * 启动VI设备捕获图像功能。开启该功能，VI驱动会实时抓取Bayer、YUV图像。该接口调用前，需提前设置设备属性并禁用设备。直通模式下，该功能不支持
 * @param {VI_DEV} ViDev: 输入，vi设备号。取值范围：[0,VI_MAX_DEV_NUM)
 * @param {VI_GET_FRM_PARAM_S} *pstGetFrmParam: 输入，捕获图像参数配置信息
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VI_EnableGetFrame(VI_DEV ViDev, const VI_GET_FRM_PARAM_S *pstGetFrmParam);

/**
 * @description: 禁用 VI 捕获 Bayer、 YUV 图像的功能。调用本接口前，需禁用设备
 * @param {VI_DEV} ViDev: 输入，vi设备号。取值范围：[0,VI_MAX_DEV_NUM)
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VI_DisableGetFrame(VI_DEV ViDev);

/**
 * @description: 从VI设备获取一帧图像。宽动态时， astFrame[0]存储短帧信息， astFrame[1]存储长帧信息
 * @param {VI_DEV} ViDev: 输入，vi设备号。取值范围：[0,VI_MAX_DEV_NUM)
 * @param {VIDEO_FRAME_S} astFrame: 输出，获取图像帧数据信息
 * @param {NI_U32} u32MilliSec: 输入，超时时间
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VI_GetFrame(VI_DEV ViDev, VIDEO_FRAME_S astFrame[2], NI_U32 u32MilliSec);

/**
 * @description: 释放从VI设备获取到的图像帧存
 * @param {VI_DEV} ViDev: 输入，vi设备号。取值范围：[0,VI_MAX_DEV_NUM)
 * @param {VIDEO_FRAME_S} astFrame: 输入，获取图像帧数据信息
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VI_ReleaseFrame(VI_DEV ViDev, VIDEO_FRAME_S astFrame[2]);

/**
 * @description:
 * 配置设备与VI内部编解码通道映射关系。该接口仅在MIPI传输YUV数据的时候需要用户调用配置编解码通道需要在NI_MDK_VI_SetDevAttr之前调用
 * @param {VI_DEV} ViDev: 输入，vi设备号。取值范围：[0,VI_MAX_DEV_NUM)
 * @param {VI_MAP_PARAM_S} *pstMapParam: 输入，通道映射关系配置信息
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VI_SetMapParam(VI_DEV ViDev, const VI_MAP_PARAM_S *pstMapParam);

/**
 * @description: 获取设备与 VI 内部编解码通道映射关系
 * @param {VI_DEV} ViDev: 输入，vi设备号。取值范围：[0,VI_MAX_DEV_NUM)
 * @param {VI_MAP_PARAM_S} *pstMapParam: 输出，通道映射关系配置信息
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VI_GetMapParam(VI_DEV ViDev, VI_MAP_PARAM_S *pstMapParam);

/**
 * @description: 设置中断回调函数。通过该接口注册的回调会在 VI
 * 中断中被调用，因此不能在注册函数中睡眠。函数执行时间要求尽可能的短
 * @param {VI_DEV} ViDev: 输入，vi设备号。取值范围：[0,VI_MAX_DEV_NUM)
 * @param {VI_INT_CALLBACK_PARAM_S} *pstIntCallBackParam: 输入，中断回调参数配置信息
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VI_SetIntCallBack(VI_DEV ViDev, const VI_INT_CALLBACK_PARAM_S *pstIntCallBackParam);

/**
 * @description: 设置图像剪切区域参数。仅非直通模式下可调用该接口。起始点坐标、裁剪区域宽高必须 2 对齐
 * @param {VI_DEV} ViDev: 输入，vi设备号。取值范围：[0,VI_MAX_DEV_NUM)
 * @param {RECT_S} *pstCapRect: 输入，裁剪区域参数配置信息
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VI_GetEisCropRect(VI_DEV ViDev, RECT_S *pstCapRect);

/**
 * @description: 获取图像剪切区域参数
 * @param {VI_DEV} ViDev: 输入，vi设备号。取值范围：[0,VI_MAX_DEV_NUM)
 * @param {RECT_S} *pstCapRect: 输出，裁剪区域参数配置信息
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VI_SetEisCropRect(VI_DEV ViDev, const RECT_S *pstCapRect);

/**
 * @description: 设置距离sensor帧头多少行触发中断。接收长曝光 u32TriggerLine 行后，调用用户通过 NI_MDK_VI_SetIntCallBack
 * 接口注册的回调函数
 * @param {VI_DEV} ViDev: 输入，vi设备号。取值范围：[0,VI_MAX_DEV_NUM)
 * @param {NI_U32} u32TriggerLine: 输入，距离sensor帧头中断的行数配置
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VI_SetIntTriggerLine(VI_DEV ViDev, NI_U32 u32TriggerLine);

/**
 * @description: 获取距离sensor帧头多少行触发中断
 * @param {VI_DEV} ViDev: 输入，vi设备号。取值范围：[0,VI_MAX_DEV_NUM)
 * @param {NI_U32} *pu32TriggerLine: 输出，距离sensor帧头中断的行数配置
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VI_GetIntTriggerLine(VI_DEV ViDev, NI_U32 *pu32TriggerLine);

/**
 * @description: 使能将一路VI数据同时输出到两路ISP功能。两路图像均来自同一sensor。仅非直通模式下支持
 * @param {VI_DEV} ViDev: 输入，vi设备号。取值范围：[0,VI_MAX_DEV_NUM)
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VI_EnableDuplicate(VI_DEV ViDev);

/**
 * @description: 去使能将一路 VI 数据同时输出到两路 ISP 功能
 * @param {VI_DEV} ViDev: 输入，vi设备号。取值范围：[0,VI_MAX_DEV_NUM)
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VI_DisableDuplicate(VI_DEV ViDev);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
