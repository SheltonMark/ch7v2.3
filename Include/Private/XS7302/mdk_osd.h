/* 
 * mdk_osd.h -- osd模块对外提供的API头文件
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */

#ifndef _MDK_OSD_H_
#define _MDK_OSD_H_


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "ni_common.h"
#include "ni_comm_osd.h"

#ifdef __cplusplus
extern "C"{
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
 * @description: 设置调色板(包括颜色和透明度)
 * @param {MDK_CHN_S} pstMdkChn: 输入，模块和通道属性结构体
 * @param {OSD_PALETTE_S} pstOsdPalette: 输入，OSD调色板属性结构体
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_OSD_SetPalette(const MDK_CHN_S *pstMdkChn, const OSD_PALETTE_S *pstOsdPalette);

/**
 * @description: 获取调色板(包括颜色和透明度)
 * @param {MDK_CHN_S} pstMdkChn: 输入，模块和通道属性结构体
 * @param {OSD_PALETTE_S} pstOsdPalette: 输出，OSD调色板属性结构体
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_OSD_GetPalette(const MDK_CHN_S *pstMdkChn, OSD_PALETTE_S *pstOsdPalette);

/**
 * @description: 创建指定的OSD区域
 * @param {NI_U32} u32OsdHandle: 输入，OSD句柄
 * @param {OSD_REGION_S} pstOsdRegion: 输入，OSD区域属性结构体
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_OSD_Create(NI_U32 u32OsdHandle, const OSD_REGION_S *pstOsdRegion);

/**
 * @description: 销毁指定的OSD区域
 * @param {NI_U32} u32OsdHandle: 输入，OSD句柄
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_OSD_Destroy(NI_U32 u32OsdHandle);

/**
 * @description: 将指定的创建好的OSD画到指定模块的通道上(同时可以设置显示属性)
 * @param {NI_U32} u32OsdHandle: 输入，OSD句柄
 * @param {MDK_CHN_S} pstMdkChn: 输入，模块和通道属性结构体
 * @param {OSD_DISP_ATTR_S} pstOsdDispAttr: 输入，OSD显示属性结构体
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_OSD_PaintToChn(NI_U32 u32OsdHandle, const MDK_CHN_S *pstMdkChn, const OSD_DISP_ATTR_S *pstOsdDispAttr);

/**
 * @description: 将指定的OSD从通道中清除
 * @param {NI_U32} u32OsdHandle: 输入，OSD句柄
 * @param {MDK_CHN_S} pstMdkChn: 输入，模块和通道属性结构体
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_OSD_ClearFromChn(NI_U32 u32OsdHandle, const MDK_CHN_S *pstMdkChn);

/**
 * @description: 获取OSD的显示属性
 * @param {NI_U32} u32OsdHandle: 输入，OSD句柄
 * @param {MDK_CHN_S} pstMdkChn: 输入，模块和通道属性结构体
 * @param {OSD_DISP_ATTR_S} pstOsdDispAttr: 输出，模块和通道属性结构体
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_OSD_GetDispAttr(NI_U32 u32OsdHandle, const MDK_CHN_S *pstMdkChn, OSD_DISP_ATTR_S *pstOsdDispAttr);

/**
 * @description: 获取指定OSD buffer信息
 * @param {NI_U32} u32OsdHandle: 输入，OSD句柄
 * @param {OSD_BUFFER_INFO_S} pstOsdBufInfo: OSD buffer信息结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_OSD_GetBuffer(NI_U32 u32OsdHandle, OSD_BUFFER_INFO_S *pstOsdBufInfo);

/**
 * @description: 刷新指定OSD
 * @attention:1. 本接口需要和NI_MDK_OSD_GetBuffer配合使用。
 * @param {NI_U32} u32OsdHandle: 输入，OSD句柄
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_OSD_Refresh(NI_U32 u32OsdHandle);

/**
 * @description: 刷新所有OSD 调用本接口，则所有pstMdkChn通道上的OSD都会显示，包括原来处于隐藏状态的OSD
 * @attention:1. 应用场景: 先配了多个OSD(处于隐藏状态)，然后一起在同一帧生效， 如: 联动抓拍多个车牌叠加到同一帧上时使用
 * @param {MDK_CHN_S} pstMdkChn: 输入，模块和通道属性结构体
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_OSD_ShowAll(MDK_CHN_S *pstMdkChn);

/**
 * @description: 刷新所有OSD 调用本接口，则所有pstMdkChn通道上的OSD都会隐藏
 * @param {MDK_CHN_S} pstMdkChn: 输入，模块和通道属性结构体
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_OSD_HideAll(MDK_CHN_S *pstMdkChn);

/**
 * @description: OSD信息行数据格式转16*16宏块格式数据
 * @attention:1. xs7302 xs7312 xs7320 不支持此接口
 * @param {OSD_FORMAT_INFO_S} pstOsdDataAttr: OSD行YUV数据属性结构体
 * @param {NI_U32 } pu32OsdMacroAddr: 转换为宏块格式后数据存储地址
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_OSD_LineToMb(const OSD_FORMAT_INFO_S *pstOsdDataAttr, NI_U32 *pu32OsdMacroAddr); 

/**
 * @description: 设置扩展参数
 * @attention:1.目前只有某个物理通道上的全局透明度属性，非必设接口
 * @param {MDK_CHN_S} pstMdkChn: 输入，模块和通道属性结构体
 * @param {OSD_EXT_PARAM_S } pstOsdExtParam: 转换为宏块格式后数据存储地址
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_OSD_SetExtParam(MDK_CHN_S *pstMdkChn, const OSD_EXT_PARAM_S *pstOsdExtParam);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

