/* 
 * mdk_vps.h -- vps模块对外提供的API头文件
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */

#ifndef _MDK_VPS_H_
#define _MDK_VPS_H_


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "ni_common.h"

#include "ni_comm_vps.h"


#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

/**
 * @description: 设置VPS的GRP属性
 * @param {VPS_GRP} VpsGrp: 输入，GRP组号
 * @param {VPS_GRP_ATTR_S} *pstGrpAttr: 输入，组属性配置信息
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_SetGrpAttr(VPS_GRP VpsGrp, const VPS_GRP_ATTR_S *pstGrpAttr);

/**
 * @description: 获取VPS的GRP属性
 * @param {VPS_GRP} VpsGrp: 输入，GRP组号
 * @param {VPS_GRP_ATTR_S} *pstGrpAttr: 输出，组属性配置信息
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_GetGrpAttr(VPS_GRP VpsGrp, VPS_GRP_ATTR_S *pstGrpAttr);

/**
 * @description: 设置VPS的旋转属性
 * @param {VPS_GRP} VpsGrp: 输入，GRP组号
 * @param {VPS_ROTATE_TYPE_E} enRotate: 输入，旋转模式
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_SetGrpRotate(VPS_GRP VpsGrp, VPS_ROTATE_TYPE_E enRotate);

/**
 * @description: 设置VPS的旋转属性
 * @param {VPS_GRP} VpsGrp: 输入，GRP组号
 * @param {VPS_ROTATE_TYPE_E} *penRotate: 输出，旋转模式
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_GetGrpRotate(VPS_GRP VpsGrp, VPS_ROTATE_TYPE_E *penRotate);

/**
 * @description: 设置VPS的CHN属性
 * @param {VPS_GRP} VpsGrp: 输入，GRP组号
 * @param {VPS_CHN_E} VpsChn: 输入，CHN通道号
 * @param {VPS_ROTATE_TYPE_E} *penRotate: 输入，通道属性
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_SetChnAttr(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, const VPS_CHN_ATTR_S *pstChnAttr);

/**
 * @description: 设置VPS的CHN属性
 * @param {VPS_GRP} VpsGrp: 输入，GRP组号
 * @param {VPS_CHN_E} VpsChn: 输入，CHN通道号
 * @param {VPS_CHN_ATTR_S} *pstChnAttr: 输出，通道属性
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_GetChnAttr(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, VPS_CHN_ATTR_S *pstChnAttr);

/**
 * @description: 使能VPS的CHN通道
 * @param {VPS_GRP} VpsGrp: 输入，GRP组号
 * @param {VPS_CHN_E} VpsChn: 输入，CHN通道号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_EnableChn(VPS_GRP VpsGrp, VPS_CHN_E VpsChn);

/**
 * @description: 去使能VPS的CHN通道
 * @param {VPS_GRP} VpsGrp: 输入，GRP组号
 * @param {VPS_CHN_E} VpsChn: 输入，CHN通道号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_DisableChn(VPS_GRP VpsGrp, VPS_CHN_E VpsChn);

/**
 * @description: 创建VPS的CHN通道
 * @param {VPS_GRP} VpsGrp: 输入，GRP组号
 * @param {VPS_CHN_E} VpsChn: 输入，CHN通道号
 * @param {VPS_CHN_ATTR_S} *pstChnAttr: 输入，通道属性
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_CreateChn(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, const VPS_CHN_ATTR_S *pstChnAttr);

/**
 * @description: 销毁VPS的CHN通道
 * @param {VPS_GRP} VpsGrp: 输入，GRP组号
 * @param {VPS_CHN_E} VpsChn: 输入，CHN通道号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_DestroyChn(VPS_GRP VpsGrp, VPS_CHN_E VpsChn);

/**
 * @description: 从GRP的CHN里面获取帧数据
 * @param {VPS_GRP} VpsGrp: 输入，GRP组号
 * @param {VPS_CHN_E} VpsChn: 输入，CHN通道号
 * @param {VPS_FRAME_INFO_S} *pstFrameInfo: 输出，帧数据相关信息
 * @param {NI_U32} u32Timeout: 输入，超时等待时间
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_GetFrame(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, VPS_FRAME_INFO_S *pstFrameInfo, NI_U32 u32Timeout);

/**
 * @description: 释放对应GRP的CHN里面获取帧数据
 * @param {VPS_GRP} VpsGrp: 输入，GRP组号
 * @param {VPS_CHN_E} VpsChn: 输入，CHN通道号
 * @param {VPS_FRAME_INFO_S} *pstFrameInfo: 输入，帧数据相关信息
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_ReleaseFrame(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, VPS_FRAME_INFO_S *pstFrameInfo);

 /**
 * @description: 获取双目帧数据
 * @param {VPS_GRP} VpsGrp: 输入，GRP组号
 * @param {VPS_CHN_E} VpsChn: 输入，CHN通道号
 * @param {VPS_FRAME_INFO_S} astFrameInfo[ 2 ]: 输出，双目的两路帧数据
 * @param {NI_U32} u32Timeout: 输入，超时等待时间
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VPS_GetMultiEyeFrame(VPS_GRP VpsGrp,VPS_CHN_E VpsChn,VPS_FRAME_INFO_S astFrameInfo [ 2 ],NI_U32 u32Timeout);

/**
* @description: 释放双目帧数据
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @param {VPS_CHN_E} VpsChn: 输入，CHN通道号
* @param {VPS_FRAME_INFO_S} astFrameInfo[ 2 ]: 输入，待释放的双目数据
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_ReleaseMultiEyeFrame(VPS_GRP VpsGrp, VPS_CHN_E VpsChn,VPS_FRAME_INFO_S astFrameInfo [ 2 ]);

/**
* @description: 设置帧切换长参考帧属于
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @param {VPS_CHN_E} VpsChn: 输入，CHN通道号
* @param {VPS_ENC_SCD_LRF_ATTR_S} *pstEncScdLrfAttr: 输入，设置帧切换属性配置信息
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_SetEncChnScdLrfAttr(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, const VPS_ENC_SCD_LRF_ATTR_S *pstEncScdLrfAttr);

/**
* @description: 获取帧切换长参考帧属于
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @param {VPS_CHN_E} VpsChn: 输入，CHN通道号
* @param {VPS_ENC_SCD_LRF_ATTR_S} *pstEncScdLrfAttr: 输出，设置帧切换属性配置信息
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_GetEncChnScdLrfAttr(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, VPS_ENC_SCD_LRF_ATTR_S *pstEncScdLrfAttr);

/**
* @description: 设置通道MASK属性
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @param {VPS_CHN_E} VpsChn: 输入，CHN通道号
* @param {VPS_MASK_ATTR_S} *pstMaskAttr: 输人，设置通道的MASK属性的配置信息
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_SetMaskAttr(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, const VPS_MASK_ATTR_S *pstMaskAttr);

/**
* @description: 获取通道MASK属性
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @param {VPS_CHN_E} VpsChn: 输入，CHN通道号
* @param {VPS_MASK_ATTR_S} *pstMaskAttr: 输出，获取通道的MASK属性的配置信息
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_GetMaskAttr(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, VPS_MASK_ATTR_S *pstMaskAttr);

/**
* @description: 使能通道的某个MASK属性
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @param {VPS_CHN_E} VpsChn: 输入，CHN通道号
* @param {NI_U8} u8MaskId: 输入，MASK的ID号
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_EnableMask(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, NI_U8 u8MaskId);

/**
* @description: 去使能通道的某个MASK属性
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @param {VPS_CHN_E} VpsChn: 输入，CHN通道号
* @param {NI_U8} u8MaskId: 输入，MASK的ID号
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_DisableMask(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, NI_U8 u8MaskId);

/**
* @description: 一次性使能所以MASK属性，MASK的快速配置接口
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_EnableAllMask(VPS_GRP VpsGrp);

/**
* @description: 去使能GRP下的MASK配置
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_DisableAllMask(VPS_GRP VpsGrp);

/**
* @description: 配置GRP的MASK属性,多MASK的快速配置接口
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @param {VPS_ALL_MASK_ATTR_S} *pstAllMaskAttr: 输入，MASK属性配置信息
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_SetAllMaskAttr(VPS_GRP VpsGrp, const VPS_ALL_MASK_ATTR_S *pstAllMaskAttr);

/**
* @description: 获取GRP的MASK属性
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @param {VPS_ALL_MASK_ATTR_S} *pstAllMaskAttr: 输出，MASK属性配置信息
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_GetAllMaskAttr(VPS_GRP VpsGrp, VPS_ALL_MASK_ATTR_S *pstAllMaskAttr);

/**
* @description: 设置ENC通道的帧缓存队列深度
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @param {VPS_CHN_E} VpsChn: 输入，CHN通道号
* @param {NI_U32} u32Depth: 输入，对应通道的帧缓存深度
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_SetFrameDepth(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, NI_U32 u32Depth);

/**
* @description: 获取ENC通道的帧缓存队列深度
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @param {VPS_CHN_E} VpsChn: 输入，CHN通道号
* @param {NI_U32} u32Depth: 输出，对应通道的缓存深度
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_GetFrameDepth(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, NI_U32 *pu32Depth);

/**
* @description: 设置通道的帧率
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @param {VPS_CHN_E} VpsChn: 输入，CHN通道号
* @param {NI_U32} u32OutFps: 输入，设置通道帧率数值
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_SetFps(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, NI_U32 u32OutFps);

/**
* @description: 获取通道的帧率
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @param {VPS_CHN_E} VpsChn: 输入，CHN通道号
* @param {NI_U32} *pu32OutFps: 输出，获取通道的帧率
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_GetFps(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, NI_U32 *pu32OutFps);

/**
* @description: 设置IAS通道YUV转GRB分量的系数
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @param {VPS_CHN_E} VpsChn: 输入，CHN通道号
* @param {VPS_GSG_K_S} *pstGsgK: 输入，YUV转GRB分量的系数配置信息
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_SetChnCscRatio(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, const VPS_GSG_K_S *pstGsgK);

/**
* @description: 获取IAS通道YUV转GRB分量的系数
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @param {VPS_CHN_E} VpsChn: 输入，CHN通道号
* @param {VPS_GSG_K_S} *pstGsgK: 输出，YUV转GRB分量的系数配置信息
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_GetChnCscRatio(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, VPS_GSG_K_S *pstGsgK);

/**
* @description: 测试模式下，灌源帧数据接口
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @param {VPS_FRAME_INFO_S} *pstFrameInfo: 输入，灌源帧的配置信息
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_TestSendFrame(VPS_GRP VpsGrp, const VPS_FRAME_INFO_S * pstFrameInfo);

/**
* @description: 使能测试模式下，灌源数据接口
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_EnableTestMode(NI_VOID);

/**
* @description: 去使能测试模式下，灌源数据接口
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_DisableTestMode(NI_VOID);

/**
* @description: 获取GRP的帧数据接口
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @param {VIDEO_FRAME_S} *pstGrpFrame: 输出，GRP帧数据接口
* @param {NI_U32} u32Timeout: 输入，超时时间
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_GetGrpFrame(VPS_GRP VpsGrp, VIDEO_FRAME_S *pstGrpFrame, NI_U32 u32Timeout);

/**
* @description: 从GRP发送帧数据接口
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @param {VPS_SEND_GRPFRM_S} *pstSendGrpFrm: 输入，要发送的帧数据配置信息
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_SendGrpFrame(VPS_GRP VpsGrp, const VPS_SEND_GRPFRM_S *pstSendGrpFrm);

/**
* @description: 设置双拼属性
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @param {VPS_GRP_STITCH_ATTR_S} *pstGrpStitchAttr: 输入，双拼属性配置信息
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_SetGrpStitchAttr(VPS_GRP VpsGrp, const VPS_GRP_STITCH_ATTR_S *pstGrpStitchAttr);

/**
* @description: 获取双拼属性
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @param {VPS_GRP_STITCH_ATTR_S} *pstGrpStitchAttr: 输出，双拼属性配置信息
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_GetGrpStitchAttr(VPS_GRP VpsGrp, VPS_GRP_STITCH_ATTR_S *pstGrpStitchAttr);

/**
* @description: 启动双拼配置
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_StartGrpStitch(VPS_GRP VpsGrp);

/**
* @description: 停止双拼配置
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_StopGrpStitch(VPS_GRP VpsGrp);

/**
* @description: 获取拼接组原始帧存数据和目标拼接帧存，用户将获取到的两帧原始图像进行拼接融合,
                拼接后数据写到目标拼接帧存，调用send接口送回vps
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @param {VPS_STITCH_GRP_FRAME_S} *pstStitchGrpFrame: 输出，双拼的帧配置信息
* @param {NI_U32} u32Timeout: 输入，获取超时时间
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_GetStitchGrpFrame(VPS_GRP VpsGrp, VPS_STITCH_GRP_FRAME_S *pstStitchGrpFrame, NI_U32 u32Timeout);

/**
* @description: 发送拼接组原始帧存数据和目标拼接帧存，用户将获取到的两帧原始图像进行拼接融合,
                拼接后数据写到目标拼接帧存，调用send接口送回vps
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @param {VPS_STITCH_GRP_FRAME_S} *pstStitchGrpFrame: 输入，发送拼接后数据写到目标拼接帧存
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_SendStitchGrpFrame(VPS_GRP VpsGrp, VPS_STITCH_GRP_FRAME_S *pstStitchGrpFrame);

/**
* @description: 释放双拼相关的帧数据
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @param {VPS_STITCH_GRP_FRAME_S} *pstStitchGrpFrame: 输入，发送拼接后数据写到目标拼接帧存
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_ReleaseStitchGrpFrame(VPS_GRP VpsGrp, VPS_STITCH_GRP_FRAME_S *pstStitchGrpFrame);

/**
* @description: 获取模块参数配置信息
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @param {VPS_MOD_PARAM_S} *pstGrpModParam: 输出，获取模块参数配置信息
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_GetModParam(VPS_GRP VpsGrp,VPS_MOD_PARAM_S *pstGrpModParam);

/**
* @description: 设置模块参数配置信息
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @param {VPS_MOD_PARAM_S} *pstGrpModParam: 输入，模块参数配置信息
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_SetModParam(VPS_GRP VpsGrp,VPS_MOD_PARAM_S *pstGrpModParam);

/**
* @description: 获取GRP离线剪切参数配置信息
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @param {VPS_CROP_S} *pstGrpCrop: 输出，GRP离线剪切参数配置信息
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_GetGrpCrop(VPS_GRP VpsGrp, VPS_CROP_S *pstGrpCrop);

/**
* @description: 设置GRP离线剪切参数配置信息
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @param {VPS_CROP_S} *pstGrpCrop: 输入，GRP离线剪切参数配置信息
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_SetGrpCrop(VPS_GRP VpsGrp, const VPS_CROP_S *pstGrpCrop);

/**
* @description: 设置GRP的buf内存，防止GRP多次切换造成的内存碎片问题
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @param {VPS_GRP_PARAM_S} *pstGrpParam: 输入，GRP离线剪切参数配置信息
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_SetGrpParam(VPS_GRP VpsGrp,const VPS_GRP_PARAM_S *pstGrpParam);

/**
* @description: 获取GRP的buf大小，防止GRP切换造成的内存碎片问题
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @param {VPS_GRP_PARAM_S} *pstGrpParam: 输入，GRP离线剪切参数配置信息
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_GetGrpParam(VPS_GRP VpsGrp, VPS_GRP_PARAM_S *pstGrpParam);

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
* @description: 使能黑白模式(支持XS7312)
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @param {VPS_CHN_E} VpsChn: 输入，CHN通道号
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_EnableFakeMonochr(VPS_GRP VpsGrp, VPS_CHN_E VpsChn);

/**
* @description: 去使能黑白模式
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @param {VPS_CHN_E} VpsChn: 输入，CHN通道号
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_DisableFakeMonochr(VPS_GRP VpsGrp, VPS_CHN_E VpsChn);

/**
* @description: 双拼送帧接口，把双拼后的帧存发送下去，并根据bReleaseFrame决定是否立即释放发送的帧，
                如果不立即释放，必须显式调用NI_MDK_VPS_ReleaseFrame接口释放该帧(默认立即释放)
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @param {VIDEO_FRAME_S} *pstStitchFrame: 输入，帧数据的配置信息
* @param {NI_BOOL} bReleaseFrame: 输入，是否释放帧数据
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_SendStitchFrame(VPS_GRP VpsGrp, VIDEO_FRAME_S *pstStitchFrame, NI_BOOL bReleaseFrame);

/**
* @description: 释放USER占用的指定帧数据
* @param {VPS_GRP} VpsGrp: 输入，GRP组号
* @param {VIDEO_FRAME_S} *pstFrame: 输入，帧数据的配置据信息
* @return {*}: 0：成功，非0：失败
*/
NI_S32 NI_MDK_VPS_ReleaseFrame2(VPS_GRP VpsGrp, VIDEO_FRAME_S *pstFrame);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

