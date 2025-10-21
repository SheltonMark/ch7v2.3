/* 
 * mdk_vdec.h -- vdec模块对外提供的API头文件
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */

#ifndef __MDK_VDEC_H__
#define __MDK_VDEC_H__

#include "ni_type.h"
#include "ni_comm_vdec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/**
 * @description: 创建解码通道。
 * @param {VDEC_CHN} VdChn: 编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VDEC_CHN_ATTR_S} *pstAttr: 输入，解码通道属性结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VDEC_CreateChn (VDEC_CHN VdChn, const VDEC_CHN_ATTR_S *pstAttr);

/**
 * @description: 销毁解码通道。在未调用NI_MDK_VDEC_StopRecvStream停止接收码流之前不能销毁解码通道。调用该接口之后会释放当前通道的内存资源。
 * @param {VDEC_CHN} VdChn: 编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VDEC_DestroyChn(VDEC_CHN VdChn);

/**
 * @description: 获取编码通道属性。
 * @param {VDEC_CHN} VdChn: 编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VDEC_CHN_ATTR_S} *pstChnAttr: 输出，解码通道属性结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VDEC_GetChnAttr(VDEC_CHN VdChn, VDEC_CHN_ATTR_S *pstChnAttr);

/**
 * @description: 解码通道开始接收码流数据进行解码。
 * @param {VDEC_CHN} VdChn: 编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VDEC_StartRecvStream (VDEC_CHN VdChn);

/**
 * @description: 解码通道停止接收编码数据。
 * @param {VDEC_CHN} VdChn: 编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VDEC_StopRecvStream (VDEC_CHN VdChn);

/**
 * @description:  复位解码通道。未调用NI_MDK_VDEC_StopRecvStream停止接收码流之前不能复位解码通道。调用该接口不会释放当前通道的内存资源，
             但以前接收的码流和解码后的YUV帧数据将不再有效。
 * @param {VDEC_CHN} VdChn: 编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VDEC_ResetChn(VDEC_CHN VdChn);

/**
 * @description: 解码通道停止接收编码数据。
 * @param {VDEC_CHN} VdChn: 编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VDEC_STREAM_S} *pstStream: 输入，码流数据结构体指针
 * @param {NI_U32} u32TimeOut: 输入，超时等待时间，以ms为单位
 * @param {NI_BOOL} bWaitDecDone: 输入，调用接口是否等到该帧码流解码完成后返回
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VDEC_SendStream(VDEC_CHN VdChn,VDEC_STREAM_S * pstStream,NI_U32 u32TimeOut,NI_BOOL bWaitDecDone);

/**
 * @description:  获取解码帧数据。用户调用本接口获取解码数据与释放解码数据NI_MDK_VDEC_ReleaseImage接口调用成对出现，且尽快释放帧存，
             防止出现由于用户态占用解码数据释放不及时而导致解码器无可用帧存，无法继续解码。
 * @param {VDEC_CHN} VdChn: 编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VDEC_OUTPUT_INFO_S} *pstOutputInfo: 输入，码流数据结构体指针
 * @param {NI_U32} u32TimeOut: 输入，超时等待时间，以ms为单位
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VDEC_GetImage(VDEC_CHN VdChn,VDEC_OUTPUT_INFO_S * pstOutputInfo,NI_U32 u32TimeOut);

/**
 * @description:  释放解码帧数据。此接口应当和NI_MDK_VDEC_GetImage配对起来使用，用户获取解码数据后必须及时释放已经获取的帧存，
             否则可能会导致解码器获取不到可用帧存，影响硬件解码，并且用户必须按先获取先释放的顺序释放已经获取的帧存。
 * @param {VDEC_CHN} VdChn: 编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VDEC_OUTPUT_INFO_S} *pstOutputInfo: 输出，解码帧数据结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VDEC_ReleaseImage(VDEC_CHN VdChn,VDEC_OUTPUT_INFO_S * pstOutputInfo);

/**
 * @description: 设置解码通道旋转、镜像属性。
 * @param {VDEC_CHN} VdChn: 编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VDEC_ROTATE_TYPE_E} enRotate: 输入，镜像、旋转参数枚举
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VDEC_SetFrameRotate(VDEC_CHN VdChn, VDEC_ROTATE_TYPE_E enRotate);

/**
 * @description: 获取解码通道旋转、镜像属性。
 * @param {VDEC_CHN} VdChn: 编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VDEC_ROTATE_TYPE_E} *penRotate: 输出，镜像、旋转参数枚举指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VDEC_GetFrameRotate(VDEC_CHN VdChn,VDEC_ROTATE_TYPE_E * penRotate);

/* 用于用户自定义抽帧策略，设置该属性后SendStream送入的码流帧按该策略进行抽帧输出 */
/**
 * @description: 设置解码器软抽帧参数。
 * @param {VDEC_CHN} VdChn: 编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VDEC_FRM_RATE_EXTRACT_ATTR_S} *pstExtractAttr: 输入，解码通道软抽帧结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VDEC_SetFrmRateExtractAttr(VDEC_CHN VdChn, const VDEC_FRM_RATE_EXTRACT_ATTR_S *pstExtractAttr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* End of #ifndef __MDK_VENC_H__ */


