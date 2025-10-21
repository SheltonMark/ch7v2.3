/* 
 * mdk_venc.h -- venc模块对外提供的API头文件
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */

#ifndef __MDK_VENC_H__
#define __MDK_VENC_H__

#include "ni_type.h"
#include "ni_comm_venc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


 /**
 * @description: 创建编码通道。
 * @attention:1. 重复创建通道返回错误码NI_ERR_VENC_CHN_EXIST。
           2. 编码器的类型只支持PT_H264、PT_H265、PT_MJPEG、PT_JPEG四种类型，每种编码类型对应的码率控制方式参见VENC_RC_MODE_E。
           3. 编码通道属性分为动态属性和静态属性两种。其中动态属性的属性值在通道创建时配置，在通道销毁之前可被修改；静态属性的属性值在通
             道创建时配置，通道创建后不能被修改。
           4. 参数必须满足的约束参见VENC_CHN_ATTR_S结构体的具体说明。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_CHN_ATTR_S} *pstChnAttr: 输入，编码通道属性结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_CreateChn(VENC_CHN VeChn, const VENC_CHN_ATTR_S *pstChnAttr);

/**
 * @description: 销毁编码通道。
 * @attention:1. 在通道还未创建的情况下，销毁通道会返回错误码NI_ERR_VENC_CHN_UNEXIST。
           2. 通道停止接收图像之后，才能销毁通道，否则返回错误码NI_ERR_VENC_CHN_HAS_STARTED。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_DestroyChn(VENC_CHN VeChn);

/**
 * @description: 复位编码通道。
 * @attention:1. 通道没停止接收图像之前，Reset该通道返回错误码NI_ERR_VENC_NOT_PERM。
           2. Reset通道会将待编码和已编码的所有数据（包括用户取走的）清空。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_ResetChn(VENC_CHN VeChn);

/**
 * @description: 开启编码通道接收输入图像并进行编码。
 * @attention:1. 重复开启不返回错误。
           2. 只有调用本接口开启接收之后，编码器才开始接收图像进行编码。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_StartRecvPic(VENC_CHN VeChn);

/**
 * @description: 开启编码通道接收指定帧图像进行编码，超出指定帧数后自动停止接收图像。
 * @attention:1. 本接口适用于连续接收N帧并编码的场景，当N<=0时，该接口等同于接口NI_MDK_VENC_StartRecvPic。
           2. 通道已调用NI_MDK_VENC_StartRecvPic开始接收图像而未调用NI_MDK_VENC_StopRecvPic停止接收图像，或者已调用
             NI_MDK_VENC_StartRecvPicEx 后未接收到指定帧数的图像，再次调用本接口则返回错误码NI_ERR_VENC_NOT_PERM。
           3. 若已调用NI_MDK_VENC_StartRecvPic和NI_MDK_VENC_StopRecvPic开始接收图像，停止接收图像，再次调用本接口启动编码时，
             建议用户调用NI_MDK_VENC_ResetChn接口清除之前缓存的图像和码流。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_RECV_PIC_PARAM_S} *pstRecvParam: 输入，指定编码通道接收的帧数
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_StartRecvPicEx(VENC_CHN VeChn, VENC_RECV_PIC_PARAM_S *pstRecvParam);

/**
 * @description: 停止编码通道接收输入图像。
 * @attention:1. 重复调用本接口不返回错误。
           2. 调用此接口仅停止接收图像编码，码流buffer并不会被清除。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_StopRecvPic(VENC_CHN VeChn);

/**
 * @description: 修改编码通道的编码属性。
 * @attention:1. 编码通道属性分为动态属性和静态属性两种。其中动态属性的属性值在通道创建时配置，在通道销毁之前可被修改；静态属性的属性值在
             通道创建时配置，通道创建后不能被修改，调用本接口返回错误码NI_ERR_VENC_ILLEGAL_PARAM，具体参数属性参见VENC_ATTR_S、
             VENC_RC_ATTR_S、VENC_GOP_ATTR_S等结构体的参数具体说明。
           2. 改变图像宽度和高度，需要调用NI_MDK_VENC_StopRecvPic接口先停止编码，且宽度和高度不能大于最大宽度和最大高度。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_CHN_ATTR_S} *pstChnAttr: 输入，编码通道属性结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_SetChnAttr(VENC_CHN VeChn, const VENC_CHN_ATTR_S *pstChnAttr);

/**
 * @description: 获取编码通道的编码属性。
 * @attention: 如果通道还未创建，调用此接口，返回错误码NI_ERR_VENC_CHN_UNEXIST。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_CHN_ATTR_S} *pstChnAttr: 输出，编码通道属性结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_GetChnAttr(VENC_CHN VeChn, VENC_CHN_ATTR_S *pstChnAttr);

/**
 * @description: 获取编码码流数据。
 * @attention:1. 如果通道未开始编码，调用此接口返回错误码NI_ERR_VENC_CHN_NOT_START。
           2. 支持超时方式获取。
             u32Timeout = 0时，则为非阻塞获取，即如果无缓冲数据，返回错误码NI_ERR_VENC_NO_STREAM。
             u32Timeout = NI_TIMEOUT_FOREVER时，则为阻塞，即如果缓冲无数据，则会等待有数据时才返回获取成功。
             u32Timeout > 0 && u32Timeout < 0xffffffff时，则为超时，即如果缓冲无数据，则会等待用户设定的超时时间，若在设定的时间内有数据则返回获取成功，否则返回超时失败。
           3. 建议本接口获取码流与释放码流NI_MDK_VENC_ReleaseStream接口成对出现，且尽快释放码流，防止出现由于用户态获取码流，释放不及时而导致的码流 buffer满，停止编码。
           4. 如果用户长时间不获取码流，码流缓冲区就会满。当编码通道发生码流缓冲区满，就会丢弃待编码的YUV数据，直到用户获取码流，
             从而有足够的码流缓冲可以用于编码时，才开始继续编码。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_STREAM_S} *pstStream: 输出，编码码流结构体指针 
 * @param {NI_U32} u32Timeout: 输入，超时等待时间，以ms为单位，取值范围[0, +∞)，NI_TIMEOUT_NONE：非阻塞，NI_TIMEOUT_FOREVER：阻塞，大于0：超时时间
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_GetStream(VENC_CHN VeChn, VENC_STREAM_S *pstStream, NI_U32 u32Timeout);

/**
 * @description: 释放码流缓存。
 * @attention:1. 此接口应当和NI_MDK_VENC_GetStream配对起来使用，用户获取码流后必须及时释放已经获取的码流缓存，否则可能会导致码流 buffer满，
             影响编码器编码，并且用户必须按先获取先释放的顺序释放已经获取的码流缓存。
           2. 调用NI_MDK_VENC_ResetChn进行通道清空操作后，所有未释放的码流包均无效，不能再使用或者调用本接口释放这部分无效的码流缓存。
           3. 释放无效的码流会返回错误码NI_ERR_VENC_ILLEGAL_PARAM。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_STREAM_S} *pstStream: 输入，编码码流结构体指针 
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_ReleaseStream(VENC_CHN VeChn,  VENC_STREAM_S *pstStream);

/**
 * @description: 获取码流缓冲区内存信息（起始物理地址和大小）。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_STREAM_BUF_INFO_S} *pstStreamBufInfo: 输出，码流 buffer信息结构体指针 
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_GetStreamBufInfo(VENC_CHN VeChn, VENC_STREAM_BUF_INFO_S *pstStreamBufInfo);

/**
 * @description: 设置码流校验方式。
 * @attention:1. 仅JPEG编码通道支持。
           2. 码流校验有累加和校验和CRC校验两种方式，用户通过调用本接口实现配置校验方式，得出的校验值保存于VENC_STREAM_S结构体中u32CheckValue，
             可通过调用NI_MDK_VENC_GetStream接口获取码流时得到该帧的校验值。
           3. 该接口必须在调用NI_MDK_VENC_CreateChn接口之前调用，否则返回错误码NI_ERR_VENC_NOT_PERM。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_STREAM_CHECK_TYPE_E} enCheckType: 输入，码流校验类型枚举
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_SetStreamCheck(VENC_CHN VeChn, const VENC_STREAM_CHECK_TYPE_E enCheckType);//创建通道之前设置好

/**
 * @description: 获取码流校验方式。
 * @attention:仅JPEG编码通道支持。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_STREAM_CHECK_TYPE_E} *penCheckType: 输出，码流校验类型枚举指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_GetStreamCheck(VENC_CHN VeChn, VENC_STREAM_CHECK_TYPE_E *penCheckType);

/**
 * @description: 强制产生一个IDR帧。
 * @attention:1. 本接口仅支持H.264/H.265协议编码通道，如果通道不是H.264/H.265协议编码通道，则返回错误码NI_ERR_VENC_NOT_SUPPORT。
           2. 调用本接口，下帧立即编一帧IDR帧。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_ForceIDR(VENC_CHN VeChn);

/**
 * @description: 使能彩色转灰图编码。
 * @attention:1. 仅JPEG编码通道支持。
           2. 调用本接口开启伪单色编码使能后对之后的每一帧均生效，可通过调用NI_MDK_VENC_DisableFakeMonochr关闭伪单色编码使能。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_EnableFakeMonochr(VENC_CHN VeChn);

/**
 * @description: 去使能彩色转灰图编码。
 * @attention: 仅JPEG编码通道支持。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_DisableFakeMonochr(VENC_CHN VeChn);

/**
 * @description: 设置通道裁剪属性。
 * @attention:1. 支持本接口所有功能，H.26x仅为填充相应裁剪语法元素，本身YUV不裁剪。
           2. 本接口用于设置通道的裁剪属性，必须在通道创建之后，通道销毁前调用。
           3. 裁剪属性具体的参数约束条件参见VENC_CROP_ATTR_S。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_CROP_ATTR_S} *pstCropAttr: 输入，裁剪属性结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_SetChnCrop(VENC_CHN VeChn, const VENC_CROP_ATTR_S *pstCropAttr);

/**
 * @description: 设置通道裁剪属性。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_CROP_ATTR_S} *pstCropAttr: 输出，裁剪属性结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_GetChnCrop(VENC_CHN VeChn, VENC_CROP_ATTR_S *pstCropAttr);

/**
 * @description: 设置编码器输出帧率。
 * @attention:1. 本接口为非必设接口。
           2. 用本接口用于设置通道的输出帧率时，必须在通道创建之后，通道销毁前调用。
           3. 首次调用接口时，若设置输出帧率超过输入帧率，输出帧率为输入帧率；若连续调用该接口，后一次调用的输入帧率是前一次的输出帧率。
           4. 默认编码输出帧率就是输入帧率。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {NI_U32} u32OutFrmRate: 输入，输出帧率，取值范围：[0, VENC_MAX_OUTFRAME_RATE]
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_SetOutFrmRate(VENC_CHN VeChn, NI_U32 u32OutFrmRate);

/**
 * @description: 获取编码器输出帧率。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {NI_U32} *pu32OutFrmRate: 输出，输出帧率，取值范围：[0, VENC_MAX_OUTFRAME_RATE]
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_GetOutFrmRate(VENC_CHN VeChn, NI_U32 *pu32OutFrmRate);

/**
 * @description: 设置帧头帧尾属性。
 * @attention:1. XS7312/XS7302 H.26x编码不支持该功能。
           2. 本接口在创建通道之后，开启编码之前调用，在开启接收图像编码的情况下则返回错误码NI_ERR_VENC_NOT_PERM。
           3. 户设置帧头帧尾后，获取到的码流包含帧头帧尾空间，对码流进行解码时，需将帧头帧尾空间剔除，否则解码工具无法解码。
           4. 帧头帧尾空间需8Byte对齐，最大均支持32kByte。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_STRM_HEAD_ATTR_S} *pstStrmHeadAttr: 输入，帧头帧尾属性结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_SetStrmHeadAttr(VENC_CHN VeChn, const VENC_STRM_HEAD_ATTR_S *pstStrmHeadAttr);

/**
 * @description: 获取帧头帧尾属性。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_STRM_HEAD_ATTR_S} *pstStrmHeadAttr: 输出，帧头帧尾属性结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_GetStrmHeadAttr(VENC_CHN VeChn, VENC_STRM_HEAD_ATTR_S *pstStrmHeadAttr);

/**
 * @description: 设置参考帧/重构帧无损压缩方式。
 * @attention:1. 创建通道之前设置好，不能动态修改，默认为关闭。
           2. XS7312/XS7302 不支持
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_COMPRESS_E} enCompress: 输入，无损压缩方式枚举
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_SetCompressor(VENC_CHN VeChn, VENC_COMPRESS_E enCompress);

/**
 * @description: 获取参考帧/重构帧无损压缩方式。
 * @attention: XS7312/XS7302 不支持。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_COMPRESS_E} *penCompress: 输出，无损压缩方式枚举指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_GetCompressor(VENC_CHN VeChn, VENC_COMPRESS_E *penCompress);

/**
 * @description: 用户发送原始图像进行编码。
 * @attention:1. 送YUV图像太快可能导致编码来不及从而导致接口调用超时。
           2. Send接口调用成功并不代表该帧YUV编码完成，用户可以通过获取到的码流信息中的Pts来匹配相应的YUV帧数据，
             或者用户通过MBUF机制来管理YUV帧数据，从而保证YUV使用完毕。
           3. XS7312/XS7302发送的图像支持如下格式：
             PIXEL_FORMAT_YUV_PLANAR_420
             PIXEL_FORMAT_YUV_SEMIPLANAR_420
             PIXEL_FORMAT_YVU_SEMIPLANAR_420
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VIDEO_FRAME_S} pstVideoFrame: 输入，原始图像信息结构指针
 * @param {NI_U32} u32Timeout: 输入，发送图像超时时间，以ms为单位，取值范围[0, +∞)，NI_TIMEOUT_NONE：非阻塞，NI_TIMEOUT_FOREVER：阻塞，大于0：超时时间
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_SendFrm(VENC_CHN VeChn, VIDEO_FRAME_S *pstVideoFrame, NI_U32 u32Timeout);

/**
 * @description: 用设置编码通道的 ROI 属性。
 * @attention:1. ROI区域的QP设置支持相对和绝对两种QP模式，在相对QP时，支持正负数。
           2. 支持ROI区域优先级设置，优先级越高，发生ROI区域重叠的时候重叠区域选择优先级高的ROI区域的QP值。
           3. XS7312/XS7302 ROI功能跟QpMap功能不能同时启用。
           4. XS7312/XS7302如果编码器开启了旋转功能，则作用在旋转后的图像上。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_ROI_CFG_S} *pstRoiCfg: 输入，编码通道ROI属性结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_SetRoiCfg(VENC_CHN VeChn, const VENC_ROI_CFG_S *pstRoiCfg);

/**
 * @description: 获取编码通道的 ROI 属性。
 * @attention: 本接口用于获取 H.264/H.265协议编码通道的 index的 ROI 配置。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_ROI_CFG_S} *pstRoiCfg: 输出，编码通道ROI属性结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_GetRoiCfg(VENC_CHN VeChn, VENC_ROI_CFG_S *pstRoiCfg);

/**
 * @description: 一次设置编码通道的多个ROI区域属性。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_MULTI_ROI_CFG_S} *pstMultiRoiCfg: 输入，编码通道多个ROI区域属性结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_SetMultiRoiCfg(VENC_CHN VeChn, const VENC_MULTI_ROI_CFG_S *pstMultiRoiCfg);

/**
 * @description: 设置编码通道去块滤波参数。
 * @attention: 开启编码之前需设置
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_DBLK_ATTR_S} *pstDblkAttr: 输入，编码通道去块滤波属性结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_SetDblkAttr(VENC_CHN VeChn, const VENC_DBLK_ATTR_S *pstDblkAttr);

/**
 * @description: 获取编码通道去块滤波参数。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_DBLK_ATTR_S} *pstDblkAttr: 输出，编码通道去块滤波属性结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_GetDblkAttr(VENC_CHN VeChn, VENC_DBLK_ATTR_S *pstDblkAttr);

/**
 * @description: 设置编码通道的VUI配置。
 * @attention: 本接口必须在开启编码之前调用，否则返回错误码NI_ERR_VENC_NOT_PERM。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_VUI_ATTR_S} *pstVuiAttr: 输入，编码通道VUI配置结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_SetVuiAttr(VENC_CHN VeChn, const VENC_VUI_ATTR_S *pstVuiAttr);

/**
 * @description: 获取编码通道的VUI配置。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_VUI_ATTR_S} *pstVuiAttr: 输入，编码通道VUI配置结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_GetVuiAttr(VENC_CHN VeChn, VENC_VUI_ATTR_S *pstVuiAttr);

/**
 * @description: 设置H.264/H.265通道的熵编码属性。
 * @attention: 本接口可在编码通道创建之后，开启通道编码之前调用。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_H26X_ENTROPY_S} *pstH26XEntropy: 输入，编码熵编码属性结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_SetH26XEntropy(VENC_CHN VeChn, const VENC_H26X_ENTROPY_S *pstH26XEntropy);

/**
 * @description: 获取H.264/H.265通道的熵编码属性。
 * @attention: 本接口可在编码通道创建之后，开启通道编码之前调用。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_H26X_ENTROPY_S} *pstH26XEntropy: 输出，编码熵编码属性结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_GetH26XEntropy(VENC_CHN VeChn, VENC_H26X_ENTROPY_S *pstH26XEntropy);

/**
 * @description: 获取编码通道默认码控高级参数配置。
 * @attention:1. 本接口用于获取所有通道的全局RC默认值，获取的默认值不会随通道设置的RC高级参数改变而改变。
           2. 本接口用于获取所有通道的全局RC默认值，各参数的含义具体参见RC_PARAM_H26X_S和RC_PARAM_MJPEG_S。
 * @param {VENC_RC_PARAM_S} *pstRcParam: 输出，编码通道高级码控参数结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_GetDefaultRcParam(VENC_RC_PARAM_S * pstRcParam);

/**
 * @description: 设置编码通道码控高级参数。
 * @attention:1. 编码通道码率控制器的高级参数都有默认值，而不是必须调用这个接口才能启动编码通道，建议用户先调用 NI_MDK_VENC_GetRcParam接口，
             获取 RC 高级参数，然后修改相应参数，再调用本接口对高级参数进行设置。
           2. 建议用户谨慎调用本接口，若参数配置不在合理范围，可能会导致码率异常，具体参数约束条件参见VENC_RC_PARAM_S。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_RC_PARAM_S} *pstRcParam: 输入，编码通道高级码控参数结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_SetRcParam(VENC_CHN VeChn, const VENC_RC_PARAM_S * pstRcParam);

/**
 * @description: 获取编码通道码控高级参数。
 * @attention:1. 本接口用于获取编码通道码率控制的高级参数。各参数的含义请具体请参见RC_PARAM_H26X_S和RC_PARAM_MJPEG_S。
           2. 调用本接口获取的是该通道的码率控制的高级参数，会因设置的值改变而改变，如果用户想获取出厂的默认值，请调用接口NI_MDK_VENC_GetDefaultRcParam。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_RC_PARAM_S} *pstRcParam: 输出，编码通道高级码控参数结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_GetRcParam(VENC_CHN VeChn, VENC_RC_PARAM_S * pstRcParam);

/**
 * @description: 配置用户自定义的SEI数据。
 * @attention:1. 本接口用于设置用户附加信息（视频简介、版权宣告、用户自定义等数据），非必设接口，调用本接口后面编码的帧头信息中均包含该信息，
             用户可通过解析码流帧头数据获得该信息。
           2. 本接口支持H.264/H.265协议编码通道，如果通道不是H.264/H.265协议编码通道，则返回错误码NI_ERR_VENC_NOT_SUPPORT。
           3. 附加信息主要由以下参数组成：
             *pu8UserData：用户数据虚拟地址，该地址可存放用户需要添加到码流数据中的附加信息。
             u32DataSize：用户数据长度，以Byte为单位，取值范围[16, 237]。当pu8UserData配置为NULL且u32DataSize配置为0时，且表示用户关闭SEI功能。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_SEI_USRDATA_S} *pstSeiUsrData: 输入，编码通道SEI数据结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_SetSeiUserData(VENC_CHN VeChn, const VENC_SEI_USRDATA_S *pstSeiUsrData);

/**
 * @description: 获取用户自定义的SEI数据。
 * @attention:1. 支持H.264/H.265协议编码通道，如果通道不是H.264/H.265协议编码通道，则返回错误码NI_ERR_VENC_NOT_SUPPORT。
           2. 本接口调用前必须已调用NI_MDK_VENC_SetSeiUserData接口设置过用户附加信息，否则返回错误码NI_ERR_VENC_NOT_PERM。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_SEI_USRDATA_S} *pstSeiUsrData: 输出，编码通道SEI数据结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_GetSeiUserData(VENC_CHN VeChn, VENC_SEI_USRDATA_S *pstSeiUsrData);

/**
 * @description: 设置循环帧内编码块参数。
 * @attention: 本接口支持H.264/H.265协议编码通道，如果通道不是H.264/H.265协议编码通道，则返回错误码NI_ERR_VENC_NOT_SUPPORT。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_CIR_PARAM_S} *pstCirParam: 输入，编码通道CIR参数结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_SetCirParam(VENC_CHN VeChn, const VENC_CIR_PARAM_S *pstCirParam);

/**
 * @description: 获取循环帧内编码块参数。
 * @attention: 本接口支持H.264/H.265协议编码通道，如果通道不是H.264/H.265协议编码通道，则返回错误码NI_ERR_VENC_NOT_SUPPORT。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_CIR_PARAM_S} *pstCirParam: 输出，编码通道CIR参数结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_GetCirParam(VENC_CHN VeChn, VENC_CIR_PARAM_S *pstCirParam);

/**
 * @description: 设置I帧渐进解码刷新参数。
 * @attention: 本接口支持H.264/H.265协议编码通道，如果通道不是H.264/H.265协议编码通道，则返回错误码NI_ERR_VENC_NOT_SUPPORT。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_GDR_PARAM_S} *pstGdrParam: 输入，编码通道GDR参数结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_SetGdrParam(VENC_CHN VeChn, const VENC_GDR_PARAM_S *pstGdrParam);

/**
 * @description: 获取I帧渐进解码刷新参数。
 * @attention: 本接口支持H.264/H.265协议编码通道，如果通道不是H.264/H.265协议编码通道，则返回错误码NI_ERR_VENC_NOT_SUPPORT。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_GDR_PARAM_S} *pstGdrParam: 输出，编码通道GDR参数结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_GetGdrParam(VENC_CHN VeChn, VENC_GDR_PARAM_S *pstGdrParam);

/**
 * @description: 设置插入缩略图参数。
 * @attention:1. 本接口支持MJPEG/JPEG编码通道，如果通道不是MJPEG/JPEG协议编码通道，则返回错误码NI_ERR_VENC_NOT_SUPPORT。
           2. 本接口每调用一次，会且只会在下一帧生效，若在启动下一帧编码前，多次调用本接口只会插入最后一次的缩略图，用户可通过多次调用本接口实现插入多个缩略图。
           3. 本接口支持的缩略图格式有：
             VENC_JPEG_THUMB_JPEG
             VENC_JPEG_THUMB_PALETTE_RGB8
             VENC_JPEG_THUMB_RGB24
             每种格式对应的参数约束条件参见VENC_THUMBNAIL_S。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_THUMBNAIL_S} *pstVencThumbnail: 输入，JPEG编码通道待插入缩略图参数结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_InsertThumbnail(VENC_CHN VeChn, const VENC_THUMBNAIL_S *pstVencThumbnail);

/**
 * @description: 设置H.265编码通道SAO参数。
 * @attention:1. 本接口用于设置H.265协议编码通道码流的SAO配置，如果通道不是H.265协议编码通道，则返回错误码NI_ERR_VENC_NOT_SUPPORT。
           2. 本接口必须在开启编码之前调用，否则返回错误码NI_ERR_VENC_NOT_PERM。
           3. 本接口属于高级接口，用户可以选择性调用，系统默认打开SAO功能。
           4. 建议用户在调用此接口之前，先调用 NI_MDK_VENC_GetH265Sao接口，获取当前通道的 SAO配置，然后再进行设置。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_SAO_PARAM_S} *pstSaoParam: 输入，编码通道SAO参数结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_SetH265Sao(VENC_CHN VeChn, const VENC_SAO_PARAM_S *pstSaoParam);

/**
 * @description: 获取H.265编码通道SAO参数。
 * @attention: 本接口可在编码通道创建之后，开启通道编码之前调用。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_SAO_PARAM_S} *pstSaoParam: 输出，编码通道SAO参数结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_GetH265Sao(VENC_CHN VeChn, VENC_SAO_PARAM_S *pstSaoParam);

/**
 * @description: 设置编码通道强制帧内编码区域。
 * @attention:1. 本接口支持H.264/H.265协议编码通道，如果通道不是H.264/H.265协议编码通道，则返回错误码NI_ERR_VENC_NOT_SUPPORT。
           2. 调用本接口设置帧内编码区域会根据配置的坐标进行宏块(16)对齐，在码流中的看到的效果会和实际配置的坐标有所外扩。
           3. 如果编码器开启了旋转功能，则作用在旋转后的图像上。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_INTRA_AREA_S} *pstIntraArea: 输入，编码通道强制帧内编码区域参数结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_SetIntraArea(VENC_CHN VeChn, const VENC_INTRA_AREA_S *pstIntraArea);

/**
 * @description: 获取编码通道强制帧内编码区域。
 * @attention: 本接口支持H.264/H.265协议编码通道，如果通道不是H.264/H.265协议编码通道，则返回错误码NI_ERR_VENC_NOT_SUPPORT。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_INTRA_AREA_S} *pstIntraArea: 输出，编码通道强制帧内编码区域参数结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_GetIntraArea(VENC_CHN VeChn, VENC_INTRA_AREA_S *pstIntraArea);

/**
 * @description: 设置JPEG编码通道高级参数（量化表、RoiFilter）。
 * @attention:1. 本接口为非必设接口。
           2. 本接口用于设置JPEG编码参数，必须在通道创建之后，通道销毁前调用。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_JPEG_PARAM_S} *pstJpegParam: 输入，编码通道强制帧内编码区域参数结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_SetJpegParam(VENC_CHN VeChn, const VENC_JPEG_PARAM_S *pstJpegParam);

/**
 * @description: 获取JPEG编码通道高级参数（量化表、RoiFilter）。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_JPEG_PARAM_S} *pstJpegParam: 输出，编码通道强制帧内编码区域参数结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_GetJpegParam(VENC_CHN VeChn, VENC_JPEG_PARAM_S *pstJpegParam);

/**
 * @description: 获取通道对应的设备文件句柄。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_GetFd(VENC_CHN VeChn);

/**
 * @description: 设置编码器模块参数。
 * @attention: 本接口用于设置模块参数，必须在通道创建前调用。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_MOD_PARAM_S} *pstModParam: 输入，编码通道强制帧内编码区域参数结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_SetModParam(const VENC_MOD_PARAM_S *pstModParam);

/**
 * @description: 设置编码器模块参数。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_MOD_PARAM_S} *pstModParam: 输出，编码通道强制帧内编码区域参数结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_GetModParam(VENC_MOD_PARAM_S *pstModParam);

/**
 * @description: 设置编码通道码流缓存管理模式。
 * @attention:1. 本接口在通道创建之前调用，通道创建后调用本接口返回错误码NI_ERR_VENC_NOT_PERM。
           2. 支持配置VENC_STREAM_RING_BUF_MODE和VENC_STREAM_LINE_BUF_MODE两种模式，默认VENC_STREAM_RING_BUF_MODE，
             当用户配置为VENC_STREAM_LINE_BUF_MODE时，需要自行管理码流缓冲区，具体参见NI_MDK_VENC_UpdateStreamBuf和
             NI_MDK_VENC_IsStreamBufNeedUpdate接口用法。
           3. 建议用户先调用NI_MDK_VENC_GetStreamBufMode接口获取当前通道的码流缓冲区模式，若需要修改该通道的码流缓冲区模式，
             需要先销毁通道，配置好模式，然后再次创建该通道。
           4. XS7312/XS7302 只支持Ring模式。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_STREAM_BUF_MODE_E} enBufMode: 输入，编码通道码流缓存管理模式枚举
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_SetStreamBufMode(VENC_CHN VeChn, VENC_STREAM_BUF_MODE_E enBufMode);

/**
 * @description: 获取编码通道码流缓存管理模式。
 * @attention:  本接口用于获取码流缓冲区模式，建议用户先调用NI_MDK_VENC_GetStreamBufMode接口获取当前通道的码流缓冲区模式，若需要修改该通道
            的码流缓冲区模式，需要先销毁通道，配置好模式，然后再次创建该通道。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_STREAM_BUF_MODE_E} *penBufMode: 输入，编码通道码流缓存管理模式枚举指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_GetStreamBufMode(VENC_CHN VeChn, VENC_STREAM_BUF_MODE_E *penBufMode);

/**
 * @description: 新码流缓存地址。
 * @attention:1. 本接口用于码流缓冲区模式为VENC_STREAM_LINE_BUF_MODE时更新码流缓冲。本接口和NI_MDK_VENC_IsStreamBufNeedUpdate
             联合起来使用，实现Linebuffer的管理。
           2. 当码流码流缓冲区为VENC_STREAM_RING_BUF_MODE时调用本接口返回错误码NI_ERR_VENC_NOT_PERM。
           3. 本接口首次需在创建通道号后，开启编码之前调用，开启编码之前需设置2次。
           4. 调用本接口前需先调用NI_MDK_VENC_IsStreamBufNeedUpdate接口判断当前是否需要更新码流缓冲，否则新的码流缓冲不一定能配置成功。
           5. XS7312/XS7302 不支持Ring模式。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_STREAM_BUF_S} *pstStreamBuf: 输入，编码通道码流缓存描述结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_UpdateStreamBuf(VENC_CHN VeChn, VENC_STREAM_BUF_S *pstStreamBuf);

/**
 * @description: 码流缓存地址是否需要更新标记。
 * @attention:1. 本接口用于获取当前码流缓冲是否需要更新，本接口和NI_MDK_VENC_UpdateStreamBuf联合起来使用，实现Linebuffer的管理。建议用户调用
             NI_MDK_VENC_UpdateStreamBuf接口前先调用本接口判断当前是否需要更新码流缓冲。
           2. 驱动内部提供空间用于存放两块用户配置的码流缓冲区，建议用户开启编码前先通过调用两次NI_MDK_VENC_UpdateStreamBuf接口配置两块码流缓冲区，然后调用
             NI_MDK_VENC_IsStreamBufNeedUpdate接口获取当前是否需要更新码流缓冲区。当两块缓冲区有一块用完时，调用NI_MDK_VENC_UpdateStreamBuf接口会返回
             NI_TRUE，用户再次调用NI_MDK_VENC_UpdateStreamBuf接口进行码流缓冲区更新；否则返回NI_FALSE，表示用户当前不需要进行更新，若此时用户仍调用
             NI_MDK_VENC_UpdateStreamBuf接口配置码流缓冲则返回错误码NI_ERR_VENC_NOT_PERM。
           3. XS7312/XS7302 不支持Ring模式。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {NI_BOOL} *pbIsNeedUp: 输出，LineBuffer模式下码流缓存地址是否需要更新
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_IsStreamBufNeedUpdate(VENC_CHN VeChn, NI_BOOL *pbIsNeedUp);

/**
 * @description: 设置编码通道旋转模式。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_ROTATE_TYPE_E} enRotate: 输入，编码通道旋转模式枚举
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_SetFrameRotate(VENC_CHN VeChn, VENC_ROTATE_TYPE_E enRotate);

/**
 * @description: 设置编码通道旋转模式。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_ROTATE_TYPE_E} *penRotate: 输出，编码通道旋转模式枚举指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_GetFrameRotate(VENC_CHN VeChn, VENC_ROTATE_TYPE_E *penRotate);

 /**
 * @description: 设置编码通道QpMap表。
 * @attention:1. 通过本接口设置的QPMap表对所有帧有效，正常有对QpMap表的内存拷贝，驱动优先选择用户态虚拟地址进行拷贝，如果只传物理地址需确保内存数据一致性。
             如果不想有内存拷贝用户需使用Mbuf机制进行QpMap的内存管理，将bQpMapDirect置为True，并将Mbuf PoolId变量地址赋给pPrivAttr。
           2. QpMap在内存中的存储格式请参见VENC_QPMAP_INFO_S。
           3. ROI功能跟QpMap功能不能同时启用。
           4. 如果编码器开启了旋转功能，则作用在旋转后的图像上。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_QPMAP_ATTR_S} *pstQpMapAttr: 输入，编码通道QpMap参数结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_SetQpMapAttr(VENC_CHN VeChn, const VENC_QPMAP_ATTR_S *pstQpMapAttr);

/**
 * @description: 获取编码通道QpMap表。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_QPMAP_ATTR_S} *pstQpMapAttr: 输出，编码通道QpMap参数结构体指针
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_GetQpMapAttr(VENC_CHN VeChn, VENC_QPMAP_ATTR_S *pstQpMapAttr);

 /**
 * @description: 获取编码通道QpMap表。
 * @attention:1. 送YUV图像太快可能导致编码来不及从而导致接口调用超时。
           2. Send接口调用成功并不代表该帧YUV编码完成，用户可以通过获取到的码流信息中的Pts来匹配相应的YUV帧数据，或者用户通过MBUF机制来管理YUV帧数据，从而保证YUV使用完毕。
           3. 参数中带的QpMap表只对该YUV帧数据有效，因不涉及QpMap的内存拷贝，用户需要申请物理地址连续的QpMap表内存，并设置对应的QpMap表物理地址等参数，需确保内存数据一致性。
           4. 发送的图像支持如下格式：
             PIXEL_FORMAT_YUV_PLANAR_420
             PIXEL_FORMAT_YUV_SEMIPLANAR_420
             PIXEL_FORMAT_YVU_SEMIPLANAR_420
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @param {VENC_QPMAP_FRAME_S} *pstQpMapFrame: 输入，原始图像信息结构指针
 * @param {NI_U32} u32Timeout: 输入，发送图像超时时间，以ms为单位，取值范围[0, +∞)，NI_TIMEOUT_NONE：非阻塞，NI_TIMEOUT_FOREVER：阻塞，大于0：超时时间
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_SendQpMapFrm(VENC_CHN VeChn, VENC_QPMAP_FRAME_S *pstQpMapFrame, NI_U32 u32Timeout);

/**
 * @description: 使能编码通道的Cuinfo统计信息输出。
 * @attention: 接口支持H.264/H.265协议编码通道，如果通道不是H.264/H.265协议编码通道，则返回错误码NI_ERR_VENC_NOT_SUPPORT。 
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_EnableCuInfoOutput(VENC_CHN VeChn);

/**
 * @description: 去使能编码通道的Cuinfo统计信息输出。
 * @param {VENC_CHN} VeChn: 输入，编码通道号， 取值范围：[0, VENC_MAX_CHN_NUM)
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VENC_DisableCuInfoOutput(VENC_CHN VeChn);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* End of #ifndef __MDK_VENC_H__ */

