/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _MDK_OSD_H_
#define _MDK_OSD_H_

/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "ni_common.h"
#include "ni_comm_osd.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */

/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */

/*结合调色板、OSD的概念，以"绘画"的概念展示给用户OSD画到某个物理通道视频流的概念
合并建立连接关系和设置显示的两个API接口，改用绘画Paint概念，将OSD绘画到物理通道上，并带是否显示的属性。
重复绘画同一个OSD到同一个物理通道，认为是改变该OSD在物理通道上的显示属性，可以移动OSD、变更透明度等。*/

/**
 * @description: 设置调色板信息(包括颜色和透明度)
 * @param {MDK_CHN_S} pstMdkChn: 输入，模块和通道属性结构体
 * @param {OSD_PALETTE_S} pstOsdPalette: 输入，OSD调色板属性结构体
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_OSD_SetPalette(const MDK_CHN_S *pstMdkChn, const OSD_PALETTE_S *pstOsdPalette);

/**
 * @description: 获取调色板信息(包括颜色和透明度)
 * @param {MDK_CHN_S} pstMdkChn: 输入，模块和通道属性结构体
 * @param {OSD_PALETTE_S} pstOsdPalette: 输出，OSD调色板属性结构体
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_OSD_GetPalette(const MDK_CHN_S *pstMdkChn, OSD_PALETTE_S *pstOsdPalette);

/**
 * @description: 设置通道内OSD_RGN类型点阵层调色板信息(包括颜色和透明度)
 * @param {MDK_CHN_S} pstMdkChn: 输入，模块和通道属性结构体
 * @param {NI_U8} u8Layer: 输入，指定点阵层
 * @param {OSD_PALETTE_S} pstOsdPalette: 输入，OSD调色板属性结构体
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_OSD_SetLayerPalette(const MDK_CHN_S *pstMdkChn, const NI_U8 u8Layer, const OSD_PALETTE_S *pstOsdPalette);

/**
 * @description: 获取通道内OSD_RGN类型点阵层调色板信息(包括颜色和透明度)
 * @param {MDK_CHN_S} pstMdkChn: 输入，模块和通道属性结构体
 * @param {NI_U8} u8Layer: 输入，指定点阵层
 * @param {OSD_PALETTE_S} pstOsdPalette: 输出，OSD调色板属性结构体
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_OSD_GetLayerPalette(const MDK_CHN_S *pstMdkChn, const NI_U8 u8Layer, OSD_PALETTE_S *pstOsdPalette);

/**
 * @description: 创建指定OSD Handle区域
 * @param {NI_U32} u32OsdHandle: 输入，OSD句柄
 * @param {OSD_REGION_S} pstOsdRegion: 输入，OSD区域属性结构体
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_OSD_Create(NI_U32 u32OsdHandle, const OSD_REGION_S *pstOsdRegion);

/**
 * @description: 销毁指定OSD Handle区域
 * @param {NI_U32} u32OsdHandle: 输入，OSD句柄
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_OSD_Destroy(NI_U32 u32OsdHandle);

/**
 * @description: 将指定OSD画到对应的通道中（同时可以设置显示属性）
 * @param {NI_U32} u32OsdHandle: 输入，OSD句柄
 * @param {MDK_CHN_S} pstMdkChn: 输入，模块和通道属性结构体
 * @param {OSD_DISP_ATTR_S} pstOsdDispAttr: 输入，OSD显示属性结构体
 * @param {NI_U32} u32TimeOutMSec: 输入，等待功能执行阻塞超时
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_OSD_PaintToChn(NI_U32 u32OsdHandle, const MDK_CHN_S *pstMdkChn, const OSD_DISP_ATTR_S *pstOsdDispAttr, NI_U32 u32TimeOutMSec);

/**
 * @description: 将指定OSD从对应的通道中清除
 * @param {NI_U32} u32OsdHandle: 输入，OSD句柄
 * @param {MDK_CHN_S} pstMdkChn: 输入，模块和通道属性结构体
 * @param {NI_U32} u32TimeOutMSec: 输入，等待功能执行阻塞超时
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_OSD_ClearFromChn(NI_U32 u32OsdHandle, const MDK_CHN_S *pstMdkChn, NI_U32 u32TimeOutMSec);

/**
 * @description: 获取OSD的显示属性
 * @param {NI_U32} u32OsdHandle: 输入，OSD句柄
 * @param {MDK_CHN_S} pstMdkChn: 输入，模块和通道属性结构体
 * @param {OSD_DISP_ATTR_S} pstOsdDispAttr: 输出，模块和通道属性结构体
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_OSD_GetDispAttr(NI_U32 u32OsdHandle, const MDK_CHN_S *pstMdkChn, OSD_DISP_ATTR_S *pstOsdDispAttr);

/**
 * @description: 获取指定OSD数据的Buff信息
 * @param {NI_U32} u32OsdHandle: 输入，OSD句柄
 * @param {OSD_BUFFER_INFO_S} pstOsdBufInfo: OSD buffer信息结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_OSD_GetBuffer(NI_U32 u32OsdHandle, OSD_BUFFER_INFO_S *pstOsdBufInfo);

/**
 * @description: 将所有通道中的指定OSD刷新
 * @attention:1. 本接口需要和NI_MDK_OSD_GetBuffer配合使用。
 * @param {NI_U32} u32OsdHandle: 输入，OSD句柄
 * @param {NI_U32} u32TimeOutMSec: 输入，等待功能执行阻塞超时
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_OSD_Refresh(NI_U32 u32OsdHandle, NI_U32 u32TimeOutMSec);

/**
 * @description: 一次性显示指定通道中的所有OSD（调用本接口，则所有pstMdkChn通道上的OSD都会显示，与HideAll配合使用）
 * @attention:1. 应用场景: 先配了多个OSD（处于隐藏状态），然后一起在同一帧生效， 如: 联动抓拍多个车牌叠加到同一帧上时使用
 * @param {MDK_CHN_S} pstMdkChn: 输入，模块和通道属性结构体
 * @param {NI_U32} u32TimeOutMSec: 输入，等待功能执行阻塞超时
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_OSD_ShowAll(MDK_CHN_S *pstMdkChn, NI_U32 u32TimeOutMSec);

/**
 * @description: 一次性隐藏指定通道中的所有OSD（调用本接口，则所有pstMdkChn通道上的OSD都会隐藏）
 * @param {MDK_CHN_S} pstMdkChn: 输入，模块和通道属性结构体
 * @param {NI_U32} u32TimeOutMSec: 输入，等待功能执行阻塞超时
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_OSD_HideAll(MDK_CHN_S *pstMdkChn, NI_U32 u32TimeOutMSec);

/**
 * @description: 设置扩展参数（全局透明度、yuv层显示优先级），非必设接口
 * @param {MDK_CHN_S} pstMdkChn: 输入，模块和通道属性结构体
 * @param {OSD_EXT_PARAM_S } pstOsdExtParam: 输入 扩展参数
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_OSD_SetExtParam(MDK_CHN_S *pstMdkChn, const OSD_EXT_PARAM_S *pstOsdExtParam);

/**
 * @description: 联动通道OSD显示开关（调用本接口，则pstMdkChn指定直通通道上的联动通道OSD全部显示或隐藏）
 * @param {MDK_CHN_S} pstMdkChn: 输入，模块和通道属性结构体
 * @param {NI_BOOL} bOsdSkipFlag: 输入，联动通道OSD显示开关状态，0：开启联动通道显示，1：关闭联动通道显示
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_OSD_SkipRelateChn(const MDK_CHN_S *pstMdkChn, NI_BOOL bOsdSkipFlag);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
