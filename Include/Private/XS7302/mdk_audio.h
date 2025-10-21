/* 
 * mdk_audio.h -- audio模块对外提供的API头文件
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */

#ifndef __MDK_AUDIO_H__
#define __MDK_AUDIO_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */
/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "ni_type.h"
#include "ni_comm_aio.h"
#include "ni_comm_aenc.h"
#include "ni_comm_adec.h"

/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */

/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */

/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */

/**
 * @description: 初始化音频控制资源
 * @param 无
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_AUDIO_Init();

/**
 * @description: 销毁音频控制相关资源
 * @param 无
 * @return {*}: 0：成功，非0：失败
 */    
NI_S32 NI_MDK_AUDIO_DeInit();

/**
 * @description: 设置音频采集通道属性
 * @param {AIO_ATTR_S} *pstAioAttr：输入，采集通道公共属性
 * @return {*}: 0：成功，非0：失败
 */  
NI_S32 NI_MDK_AI_SetPubAttr(const AIO_ATTR_S *pstAioAttr);

/**
 * @description: 获取音频采集通道属性信息
 * @param {AIO_ATTR_S} *pstAioAttr：输出，采集通道公共属性
 * @return {*}: 0：成功，非0：失败
 */ 
NI_S32 NI_MDK_AI_GetPubAttr(AIO_ATTR_S *pstAioAttr);

/**
 * @description: 创建音频编码通道
 * @param {AENC_CHN} AeChn：输入，编码通道号。取值范围[0,AENC_MAX_CHN_NUM)
 * @param {AENC_ATTR_S} *pstAencAttr：输入，编码通道属性
 * @return {*}: >0：每次采集音频包的数据长度，<0：失败
 */ 
NI_S32 NI_MDK_AENC_CreateChn(AENC_CHN AeChn, AENC_ATTR_S *pstAencAttr);

/**
 * @description: 销毁音频编码通道
 * @param {AENC_CHN} AeChn：输入，编码通道号。取值范围[0,AENC_MAX_CHN_NUM)
 * @return {*}：0：成功，非0：失败
 */ 
NI_S32 NI_MDK_AENC_DestroyChn(AENC_CHN AeChn);

/**
 * @description: 启动音频编码通道
 * @param {AENC_CHN} AeChn：输入，编码通道号。取值范围[0,AENC_MAX_CHN_NUM)
 * @return {*}：0：成功，非0：失败
 */ 
NI_S32 NI_MDK_AENC_Start(AENC_CHN AeChn);

/**
 * @description: 停止音频编码通道
 * @param {AENC_CHN} AeChn：输入，编码通道号。取值范围[0,AENC_MAX_CHN_NUM)
 * @return {*}：0：成功，非0：失败
 */ 
NI_S32 NI_MDK_AENC_Stop (AENC_CHN AeChn);

/**
 * @description: 获取音频采集数据
 * @param {AENC_CHN} AeChn：输入，编码通道号。取值范围[0,AENC_MAX_CHN_NUM)
 * @param {AUDIO_STREAM_S} *pstStream：输出，音频数据流
 * @param {NI_U32} u32TimeOut：输入，等待超时时间，单位毫秒,NI_TIMEOUT_NONE:不等待;NI_TIMEOUT_FOREVER:一直等待，直到有数据;其他值:等待时间
 * @return {*}：0：成功，非0：失败
 */ 
NI_S32 NI_MDK_AENC_GetStream(AENC_CHN AeChn, AUDIO_STREAM_S *pstStream, NI_U32 u32TimeOut);

/**
 * @description: 释放音频采集数据包
 * @param {AENC_CHN} AeChn：输入，编码通道号。取值范围[0,AENC_MAX_CHN_NUM)
 * @param {AUDIO_STREAM_S} *pstStream：输入，音频数据流
 * @return {*}：0：成功，非0：失败
 */ 
NI_S32 NI_MDK_AENC_ReleaseStream(AENC_CHN AeChn, const AUDIO_STREAM_S *pstStream);

/**
 * @description: 设置音频播放通道属性
 * @param {AIO_ATTR_S} *pstAioAttr：输入，音频播放通道属性
 * @return {*}：0：成功，非0：失败
 */  
NI_S32 NI_MDK_AO_SetPubAttr(const AIO_ATTR_S *pstAioAttr);

/**
 * @description: 获取音频播放通道属性
 * @param {AIO_ATTR_S} *pstAioAttr：输出，音频播放通道属性
 * @return {*}：0：成功，非0：失败
 */ 
NI_S32 NI_MDK_AO_GetPubAttr(AIO_ATTR_S *pstAioAttr);

/**
 * @description: 创建音频解码通道
 * @param {ADEC_CHN} AdChn：输入，解码通道号。取值范围[0,ADEC_MAX_CHN_NUM)
 * @param {ADEC_ATTR_S} *pstAdecAttr：输入，解码通道属性
 * @return {*}: >0：每次播放音频包的数据长度，<0：失败
 */
NI_S32 NI_MDK_ADEC_CreateChn(ADEC_CHN AdChn, ADEC_ATTR_S * pstAdecAttr);

/**
 * @description: 销毁音频解码通道
 * @param {ADEC_CHN} AdChn：输入，解码通道号。取值范围[0,ADEC_MAX_CHN_NUM)
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_ADEC_DestroyChn(ADEC_CHN AdChn);

/**
 * @description: 发送音频播放数据
 * @param {ADEC_CHN} AdChn：输入，解码通道号。取值范围[0,ADEC_MAX_CHN_NUM)
 * @param {AUDIO_STREAM_S} *pstStream：输入，音频数据流
 * @param {NI_U32} u32TimeOut：输入，等待超时时间，单位毫秒,NI_TIMEOUT_NONE:不等待;NI_TIMEOUT_FOREVER:一直等待，直到有数据;其他值:等待时间
 * @return {*}：0：成功，非0：失败
 */ 
NI_S32 NI_MDK_ADEC_SendStream(ADEC_CHN AdChn, const AUDIO_STREAM_S *pstStream, NI_U32 u32TimeOut);

/**
 * @description: 启动音频解码通道
 * @param {ADEC_CHN} AdChn：输入，解码通道号。取值范围[0,ADEC_MAX_CHN_NUM)
 * @return {*}：0：成功，非0：失败
 */
NI_S32 NI_MDK_ADEC_Start(ADEC_CHN AdChn);

/**
 * @description: 停止音频解码通道
 * @param {ADEC_CHN} AdChn：输入，解码通道号。取值范围[0,ADEC_MAX_CHN_NUM)
 * @return {*}：0：成功，非0：失败
 */
NI_S32 NI_MDK_ADEC_Stop (ADEC_CHN AdChn);

/**
 * @description: 暂停音频解码通道
 * @param {ADEC_CHN} AdChn：输入，解码通道号。取值范围[0,ADEC_MAX_CHN_NUM)
 * @return {*}：0：成功，非0：失败
 */
NI_S32 NI_MDK_ADEC_Pause (ADEC_CHN AdChn);

/**
 * @description: 设置音频工作模式
 * @param {AUDIO_WORK_MODE_E} enAudioWorkMode：输入，工作模式
 * @return {*}：0：成功，非0：失败
 */
NI_S32 NI_MDK_AUDIO_SetWorkMode(AUDIO_WORK_MODE_E enAudioWorkMode);

/**
 * @description: 设置左右声道模式
 * @param {AUDIO_LRCK_MODE_E} enAudioLrckMode：输入，左右声道模式
 * @return {*}：0：成功，非0：失败
 */
NI_S32 NI_MDK_AUDIO_SetLrckMode(AUDIO_LRCK_MODE_E enAudioLrckMode);

/**
 * @description: 设置编码通道最大缓存节点数
 * @param {NI_U32} u32NodeNum：输入，缓存节点数。取值范围：[0,20)，默认值：5
 * @return {*}：0：成功，非0：失败
 */
NI_S32 NI_MDK_AENC_SetNodeNum(NI_U32 u32NodeNum);

/**
 * @description: 获取编码通道最大缓存节点数
 * @param {NI_U32} *pu32NodeNum：输出，缓存节点数。取值范围：[0,20)，默认值：5
 * @return {*}：0：成功，非0：失败
 */
NI_S32 NI_MDK_AENC_GetNodeNum(NI_U32 *pu32NodeNum);

/**
 * @description: 设置解码通道最大缓存节点数
 * @param {NI_U32} u32NodeNum：输入，缓存节点数。取值范围：[0,20)，默认值：5
 * @return {*}：0：成功，非0：失败
 */
NI_S32 NI_MDK_ADEC_SetNodeNum(NI_U32 u32NodeNum);

/**
 * @description: 获取解码通道最大缓存节点数
 * @param {NI_U32} u32NodeNum：输入，缓存节点数。取值范围：[0,20)，默认值：5
 * @return {*}：0：成功，非0：失败
 */
NI_S32 NI_MDK_ADEC_GetNodeNum(NI_U32 *pu32NodeNum);

/**
 * @description: 获取解码通道节点信息
 * @param {ADEC_CHN} AdChn：输入，解码通道号。取值范围[0,ADEC_MAX_CHN_NUM)
 * @param {ADEC_NODE_INFO_S} *pstNodeInfo：输出，解码通道节点信息（busy、free、total等）。
 * @return {*}：0：成功，非0：失败
 */
NI_S32 NI_MDK_ADEC_GetChnNodeInfo(ADEC_CHN AdChn, ADEC_NODE_INFO_S *pstNodeInfo);

/**
 * @description: 清空解码通道busy节点
 * @param {ADEC_CHN} AdChn：输入，解码通道号。取值范围[0,ADEC_MAX_CHN_NUM)
 * @return {*}：0：成功，非0：失败
 */
NI_S32 NI_MDK_ADEC_ClearChnNode(ADEC_CHN AdChn);

/**
 * @description: 获取解码器正在播放或者将要播放的音频流
 * @param {ADEC_CHN} AdChn：输入，解码通道号。取值范围[0,ADEC_MAX_CHN_NUM)
 * @param {AUDIO_STREAM_S} *pstRefStream：输出，音频流信息（主要用于回声消除控制spk与mic的时序）。
 * @param {NI_U32} u32TimeOut：输入，等待超时时间，单位毫秒,NI_TIMEOUT_NONE:不等待;NI_TIMEOUT_FOREVER:一直等待，直到有数据;其他值:等待时间
 * @return {*}：0：成功，非0：失败
 */
NI_S32 NI_MDK_ADEC_GetPlayingStream(ADEC_CHN AdChn, AUDIO_STREAM_S *pstRefStream, NI_U32 u32TimeOut);

/**
 * @description: 将数据流放回到playing队列头部
 * @param {ADEC_CHN} AdChn：输入，解码通道号。取值范围[0,ADEC_MAX_CHN_NUM)
 * @param {AUDIO_STREAM_S} *pstRefStream：输入，音频流信息（主要用于回声消除控制spk与mic的时序）。
 * @return {*}：0：成功，非0：失败
 */
NI_S32 NI_MDK_ADEC_PutPlayingStream(ADEC_CHN AdChn, const AUDIO_STREAM_S *pstRefStream);

/**
 * @description: 释放playing音频帧
 * @param {ADEC_CHN} AdChn：输入，解码通道号。取值范围[0,ADEC_MAX_CHN_NUM)
 * @param {AUDIO_STREAM_S} *pstRefStream：输入，音频流信息（主要用于回声消除控制spk与mic的时序）。
 * @return {*}：0：成功，非0：失败
 */
NI_S32 NI_MDK_ADEC_ReleasePlayingStream(ADEC_CHN AdChn, const AUDIO_STREAM_S *pstRefStream);

/**
 * @description: 启动音频3A处理
 * @param {NI_U32} u32Audio3AType：输入，3A类型。
 * @return {*}：0：成功，非0：失败
 */
NI_S32 NI_MDK_AENC_Start3A(NI_U32 u32Audio3AType);

/**
 * @description: 停止音频3A处理
 * @param {NI_U32} u32Audio3AType：输入，3A类型。
 * @return {*}：0：成功，非0：失败
 */
NI_S32 NI_MDK_AENC_Stop3A(NI_U32 u32Audio3AType);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* End of #ifndef __MDK_AUDIO_H__ */

