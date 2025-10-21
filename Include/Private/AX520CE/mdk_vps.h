/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _MDK_VPS_H_
#define _MDK_VPS_H_

/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "ni_common.h"
#include "ni_comm_vps.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @description: 设置VPS通道的属性参数
 * @param {VPS_GRP} VpsGrp: VPS组号, 取值0~1
 * @param {VPS_GRP_ATTR_S} *pstGrpAttr: VPS组的属性参数
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_SetGrpAttr(VPS_GRP VpsGrp, const VPS_GRP_ATTR_S *pstGrpAttr);

/**
 * @description: 获取VPS通道的属性参数
 * @param {VPS_GRP} VpsGrp: VPS组号, 取值0~1
 * @param {VPS_GRP_ATTR_S} *pstGrpAttr: VPS组的属性参数
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_GetGrpAttr(VPS_GRP VpsGrp, VPS_GRP_ATTR_S *pstGrpAttr);

/**
 * @description: 设置VPS通道的属性参数
 * @param {VPS_GRP} VpsGrp: VPS组号, 取值0~1
 * @param {VPS_CHN_E} VpsChn: VPS通道号，取值0~4
 * @param {VPS_CHN_ATTR_S} *pstChnAttr: VPS通道的属性参数
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_SetChnAttr(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, const VPS_CHN_ATTR_S *pstChnAttr);

/**
 * @description: 获取VPS通道的属性参数
 * @param {VPS_GRP} VpsGrp: VPS组号, 取值0~1
 * @param {VPS_CHN_E} VpsChn: VPS通道号，取值0~4
 * @param {VPS_CHN_ATTR_S} *pstChnAttr: 通道属性参数
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_GetChnAttr(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, VPS_CHN_ATTR_S *pstChnAttr);

/**
 * @description: 使能通道
 * @param {VPS_GRP} VpsGrp: VPS组号, 取值0~1
 * @param {VPS_CHN_E} VpsChn: VPS通道号，取值0~4
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_EnableChn(VPS_GRP VpsGrp, VPS_CHN_E VpsChn);

/**
 * @description: 去使能通道
 * @param {VPS_GRP} VpsGrp: VPS组号, 取值0~1
 * @param {VPS_CHN_E} VpsChn: VPS通道号，取值0~4
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_DisableChn(VPS_GRP VpsGrp, VPS_CHN_E VpsChn);

/**
 * @description: 创建Ias数据通道
 * @param {VPS_GRP} VpsGrp：VPS组号, 取值0~1
 * @param {VPS_CHN_E} VpsChn：VPS通道号，取值0~4
 * @param {VPS_CHN_ATTR_S} *pstChnAttr：Ias DISP数据通道属性句柄
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_CreateChn(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, const VPS_CHN_ATTR_S *pstChnAttr);

/**
 * @description: 销毁DISP IAS数据通道
 * @param {VPS_GRP} VpsGrp： VPS组号, 取值0~1
 * @param {VPS_CHN_E} VpsChn： VPS通道号，取值0~4
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_DestroyChn(VPS_GRP VpsGrp, VPS_CHN_E VpsChn);

/**
 * @description: 获取Ias数据
 * @param {VPS_GRP} VpsGrp：VPS组号, 取值0~1
 * @param {VPS_CHN_E} VpsChn：VPS通道号，取值0~4
 * @param {VPS_FRAME_INFO_S} *pstFrameInfo：获取的IAS数据包对象
 * @param {NI_U32} u32Timeout：等待超时时间，单位毫秒
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_GetFrame(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, VPS_FRAME_INFO_S *pstFrameInfo, NI_U32 u32Timeout);

/**
 * @description: 释放Ias数据包
 * @param {VPS_GRP} VpsGrp：VPS组号, 取值0~1
 * @param {VPS_CHN_E} VpsChn：VPS通道号，取值0~4
 * @param {VPS_FRAME_INFO_S} *pstFrameInfo：需要释放的帧数据内容
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_ReleaseFrame(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, VPS_FRAME_INFO_S *pstFrameInfo);

/**
 * @description: 设置VPS grp隐私遮挡参数，删除一个已经叠加的遮挡时遮挡参数传全0
 * @param {VPS_GRP} VpsGrp：VPS组号, 取值0~1
 * @param {VPS_MASK_ATTR_S} *pstMaskAttr：Mask 属性参数
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_SetGrpMaskAttr(VPS_GRP VpsGrp, const VPS_MASK_ATTR_S *pstMaskAttr);

/**
 * @description: 获取组通道MASK 属性参数
 * @param {VPS_GRP} VpsGrp：VPS组号, 取值0~1
 * @param {VPS_MASK_ATTR_S} *pstMaskAttr：Mask 属性参数
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_GetGrpMaskAttr(VPS_GRP VpsGrp, VPS_MASK_ATTR_S *pstMaskAttr);

/**
 * @description: 开启VPS组通道隐私遮挡使能
 * @param {VPS_GRP} VpsGrp：VPS组号, 取值0~1
 * @param {NI_U8} u8MaskId：隐私遮挡序号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_EnableGrpMask(VPS_GRP VpsGrp, NI_U8 u8MaskId);

/**
 * @description: 关闭VPS组通道隐私遮挡使能
 * @param {VPS_GRP} VpsGrp：VPS组号, 取值0~1
 * @param {NI_U8} u8MaskId：隐私遮挡序号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_DisableGrpMask(VPS_GRP VpsGrp, NI_U8 u8MaskId);

/**
 * @description: 设置VPS通道隐私遮挡参数，删除一个已经叠加的遮挡时遮挡参数传全0
 * @param {VPS_GRP} VpsGrp：VPS组号, 取值0~1
 * @param {VPS_CHN_E} VpsChn：VPS通道号，取值范围0~3
 * @param {VPS_MASK_ATTR_S} *pstMaskAttr：Mask 属性参数
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_SetChnMaskAttr(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, const VPS_MASK_ATTR_S *pstMaskAttr);

/**
 * @description: 获取通道MASK属性参数
 * @param {VPS_GRP} VpsGrp: VPS组号, 取值0~1
 * @param {VPS_CHN_E} VpsChn: VPS通道号，取值范围0~3
 * @param {VPS_MASK_ATTR_S} *pstMaskAttr：Mask 属性参数
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_GetChnMaskAttr(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, VPS_MASK_ATTR_S *pstMaskAttr);

/**
 * @description: 开启VPS通道隐私遮挡使能
 * @param {VPS_GRP} VpsGrp: VPS组号, 取值0~1
 * @param {VPS_CHN_E} VpsChn: VPS通道号，取值范围0~3
 * @param {NI_U8} u8MaskId：隐私遮挡块序号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_EnableChnMask(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, NI_U8 u8MaskId);

/**
 * @description: 关闭VPS通道隐私遮挡使能
 * @param {VPS_GRP} VpsGrp: VPS组号, 取值0~1
 * @param {VPS_CHN_E} VpsChn: VPS通道号，取值范围0~3
 * @param {NI_U8} u8MaskId：隐私遮挡块序号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_DisableChnMask(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, NI_U8 u8MaskId);

/**
 * @description: 设置通道底层驱动使用的缓存块数
 * @param {VPS_GRP} VpsGrp: VPS组号, 取值0~1
 * @param {VPS_CHN_E} VpsChn: VPS通道号，取值范围0~9
 * @param {NI_U32} u32Depth：输入的缓存深度，即缓存块数
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_SetFrameDepth(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, NI_U32 u32Depth);

/**
 * @description: 获取通道底层驱动使用的缓存块数
 * @param {VPS_GRP} VpsGrp: VPS组号, 取值0~1
 * @param {VPS_CHN_E} VpsChn: VPS通道号，取值范围0~9
 * @param {NI_U32} *pu32Depth：获取的缓存深度，即缓存块数
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_GetFrameDepth(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, NI_U32 *pu32Depth);

/**
 * @description: 设置组通道的旋转镜像参数
 * @param {VPS_GRP} VpsGrp: VPS组号, 取值0~1
 * @param {VPS_ROTATE_TYPE_E} enRotate：设置的旋转镜像配置
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_SetGrpRotate(VPS_GRP VpsGrp, VPS_ROTATE_TYPE_E enRotate);

/**
 * @description: 获取组通道的旋转镜像参数
 * @param {VPS_GRP} VpsGrp: VPS组号, 取值0~1
 * @param {VPS_ROTATE_TYPE_E} *penRotate：获取的旋转镜像状态
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_GetGrpRotate(VPS_GRP VpsGrp, VPS_ROTATE_TYPE_E *penRotate);

/**
 * @description: 用于从GRP中获取yuv图像，该接口在VPS_GRP_ATTR_S.VPS_OFFLINE_ATTR_S.VPS_GETGRPFRM_MODE_E模式不为VPS_GETGRPFRM_MODE_NONE时调用，否则获取不到图像
 * @param {VPS_GRP} VpsGrp: VPS组号, 取值0~1
 * @param {VIDEO_FRAME_S} *pstGrpFrame：获取帧数据给用户
 * @param {NI_U32} u32Timeout：获取数据的最大等待超时时间，单位毫秒
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_GetGrpFrame(VPS_GRP VpsGrp, VIDEO_FRAME_S *pstGrpFrame, NI_U32 u32Timeout);

/**
 * @description: 用于把从grp中获取的图像经处理后传回给驱动
 * @param {VPS_GRP} VpsGrp: VPS组号, 取值0~1
 * @param {VPS_SEND_GRPFRM_S} *pstSendGrpFrm：用户传输给驱动的图像数据
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_SendGrpFrame(VPS_GRP VpsGrp, const VPS_SEND_GRPFRM_S *pstSendGrpFrm);

/**
 * @description: 从vps中获取模块参数
 * @param {VPS_GRP} VpsGrp: VPS组号, 取值0~1
 * @param {VPS_MOD_PARAM_S} *pstGrpModParam: 获取的模块参数
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_GetModParam(VPS_GRP VpsGrp, VPS_MOD_PARAM_S *pstGrpModParam);

/**
 * @description: 设置模块参数
 * @param {VPS_GRP} VpsGrp: VPS组号, 取值0~1
 * @param {VPS_MOD_PARAM_S} *pstGrpModParam：输入的模块参数
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_SetModParam(VPS_GRP VpsGrp, VPS_MOD_PARAM_S *pstGrpModParam);

/**
 * @description: 获取组通道的裁剪窗口，用于数字变倍
 * @param {VPS_GRP} VpsGrp: VPS组号, 取值0~1
 * @param {VPS_CROP_S} *pstGrpCrop：当前裁剪的参数
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_GetGrpCrop(VPS_GRP VpsGrp, VPS_CROP_S *pstGrpCrop);

/**
 * @description: 设置组通道的裁剪窗口，用于数字变倍
 * @param {VPS_GRP} VpsGrp: VPS组号, 取值0~1
 * @param {VPS_CROP_S} *pstGrpCrop：设置裁剪的参数
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_SetGrpCrop(VPS_GRP VpsGrp, const VPS_CROP_S *pstGrpCrop);

/**
 * @description: 设置CSC参数 非必设接口
 * @param {VPS_GRP} VpsGrp: VPS组号, 取值0~1
 * @param {VPS_CHN_E} VpsChn：VPS通道号，只支持VPS_CHN_IAS0
 * @param {VPS_CSC_PARAM_S} *pstCscPararm：YUV转RGB转换系数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_SetChnCscParam(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, const VPS_CSC_PARAM_S *pstCscPararm);

/**
 * @description: 获取CSC参数 非必设接口
 * @param {VPS_GRP} VpsGrp: VPS组号, 取值0~1
 * @param {VPS_CHN_E} VpsChn：VPS通道号，只支持VPS_CHN_IAS0
 * @param {VPS_CSC_PARAM_S} *pstCscPararm：YUV转RGB转换系数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_GetChnCscParam(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, VPS_CSC_PARAM_S *pstCscPararm);

/**
 * @description: 发送图像给指定的vps chn通道
 * @param {VPS_GRP} VpsGrp: VPS组号, 取值0~1
 * @param {VPS_CHN_E} VpsChn: VPS通道号，取值范围0~9
 * @param {VPS_SEND_CHNFRM_S} *pstSendChnFrm: 用户输入给驱动的数据
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_SendChnFrame(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, const VPS_SEND_CHNFRM_S *pstSendChnFrm);

/**
 * @description: 用于把从用户图像传给驱动
 * @param {VPS_GRP} VpsGrp: VPS组号, 取值0~1
 * @param {VPS_SEND_GRPFRM_S} *pstSendGrpFrm：用户传输给驱动的图像数据
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_TestSendFrame(VPS_GRP VpsGrp, const VPS_FRAME_INFO_S *pstFrameInfo);

/**
 * @description: 设置旋转参数
 * @param {VPS_GRP} VpsGrp: VPS组号, 取值0~1
 * @param {VPS_ROTATE_PARAM_S} *pstRotateParam: 旋转相关参数
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_SetGrpRotateParam(VPS_GRP VpsGrp, VPS_ROTATE_PARAM_S *pstRotateParam);

/**
 * @description: 获取旋转参数
 * @param {VPS_GRP} VpsGrp: VPS组号, 取值0~1
 * @param {VPS_ROTATE_PARAM_S} *pstRotateParam: 旋转相关参数
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_GetGrpRotateParam(VPS_GRP VpsGrp, VPS_ROTATE_PARAM_S *pstRotateParam);

/**
 * @description: 设置VPS模块相关帧率，帧率的快速配置接口
 * @param {VPS_GRP} VpsGrp: 输入，GRP组号
 * @param {VPS_ALL_FPS_S} *pstAllFps: 输入，帧率配置的快速接口
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_SetAllFps(VPS_GRP VpsGrp, const VPS_ALL_FPS_S *pstAllFps);

/**
 * @description: 获取VPS模块相关帧率
 * @param {VPS_GRP} VpsGrp: 输入，GRP组号
 * @param {VPS_ALL_FPS_S} *pstAllFps: 输出，VPS模块的相关帧率数值
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_GetAllFps(VPS_GRP VpsGrp, VPS_ALL_FPS_S *pstAllFps);

/**
 * @description: 获取通道对应的设备文件句柄。
 * @param {VPS_GRP} VpsGrp: VPS组号, 取值0~1
 * @param {VPS_CHN_E} VpsChn: VPS通道号，取值范围0~9
 * @return {*}: >0：成功，<0：失败
 */
NI_S32 NI_MDK_VPS_GetFd(VPS_GRP VpsGrp, VPS_CHN_E VpsChn);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
