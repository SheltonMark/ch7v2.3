/*
 * video_encoder.h -- VENC编码模块接口定义
 *
 * 本模块负责视频编码相关的所有操作，包括通道管理、动态参数设置、编码控制等。
 * 所有操作通过platform_adapter抽象层调用SDK，不直接使用NI_MDK_*或SDK_COMM_*函数。
 *
 * Phase 2: VENC模块重构
 *
 * Created as part of video pipeline modularization refactoring.
 */

#ifndef _VIDEO_ENCODER_H_
#define _VIDEO_ENCODER_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "mdk_venc.h"
#include "ni_comm_venc.h"


/* Forward declarations - avoid circular dependencies */
#ifndef DWORD
typedef unsigned int DWORD;
#endif
typedef struct channel_info channel_info;

/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */

/* 最大编码通道数 */
#define MAX_VENC_CHN_NUM    16

/* 返回值定义 */
#define VENC_SUCCESS        0
#define VENC_FAILURE        (-1)

/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */

/*
 * ===== 初始化接口 =====
 */

/**
 * @brief 创建VENC编码通道
 * @param VencChn 编码通道号 [0, MAX_VENC_CHN_NUM-1]
 * @param pAttr 通道属性配置
 * @return VENC_SUCCESS 成功, VENC_FAILURE 失败
 */
int VideoEncoder_CreateChannel(int VencChn, VENC_CHN_ATTR_S* pAttr);

/**
 * @brief 销毁VENC编码通道
 * @param VencChn 编码通道号
 * @return VENC_SUCCESS 成功, VENC_FAILURE 失败
 */
int VideoEncoder_DestroyChannel(int VencChn);

/**
 * @brief 启动VENC编码通道接收图像
 * @param VencChn 编码通道号
 * @return VENC_SUCCESS 成功, VENC_FAILURE 失败
 */
int VideoEncoder_Start(int VencChn);

/**
 * @brief 停止VENC编码通道接收图像
 * @param VencChn 编码通道号
 * @return VENC_SUCCESS 成功, VENC_FAILURE 失败
 */
int VideoEncoder_Stop(int VencChn);

/*
 * ===== 动态参数设置（可被adaptive模块复用） =====
 */

/**
 * @brief 设置编码通道帧率
 * @param VencChn 编码通道号
 * @param fps 目标帧率
 * @return VENC_SUCCESS 成功, VENC_FAILURE 失败
 * @note 会先停止通道，修改参数后重启
 */
int VideoEncoder_SetFrameRate(int VencChn, int fps);

/**
 * @brief 设置编码通道码率
 * @param VencChn 编码通道号
 * @param bitrate_kbps 目标码率（kbps）
 * @return VENC_SUCCESS 成功, VENC_FAILURE 失败
 * @note 会先停止通道，修改参数后重启
 */
int VideoEncoder_SetBitRate(int VencChn, int bitrate_kbps);

/**
 * @brief 设置编码通道QP范围
 * @param VencChn 编码通道号
 * @param minQp P帧最小QP值
 * @param maxQp P帧最大QP值
 * @param minIQp I帧最小QP值
 * @param maxIQp I帧最大QP值
 * @return VENC_SUCCESS 成功, VENC_FAILURE 失败
 */
int VideoEncoder_SetQP(int VencChn, int minQp, int maxQp, int minIQp, int maxIQp);

/**
 * @brief 设置编码通道GOP大小
 * @param VencChn 编码通道号
 * @param gop GOP大小（I帧间隔）
 * @return VENC_SUCCESS 成功, VENC_FAILURE 失败
 */
int VideoEncoder_SetGOP(int VencChn, int gop);

/**
 * @brief 设置编码通道码率控制模式
 * @param VencChn 编码通道号
 * @param rcMode 码率控制模式（CBR/VBR/AVBR/FIXQP）
 * @return VENC_SUCCESS 成功, VENC_FAILURE 失败
 */
int VideoEncoder_SetRcMode(int VencChn, VENC_RC_MODE_E rcMode);

/**
 * @brief 设置编码类型（H264/H265）
 * @param VencChn 编码通道号
 * @param codecType 编码类型 (PT_H264/PT_H265)
 * @param profile 编码等级
 * @return VENC_SUCCESS 成功, VENC_FAILURE 失败
 * @note 场景三：需要销毁重建通道 - Stop → Destroy → Create → Start
 */
int VideoEncoder_SetCodecType(int VencChn, PAYLOAD_TYPE_E codecType, VENC_PROFILE_E profile);

/**
 * @brief 设置编码通道分辨率
 * @param VencChn 编码通道号
 * @param width 目标宽度
 * @param height 目标高度
 * @return VENC_SUCCESS 成功, VENC_FAILURE 失败
 * @note 场景四：需要VPS联动
 */
int VideoEncoder_SetResolution(int VencChn, int width, int height);

/*
 * ===== 编码控制 =====
 */

/**
 * @brief 请求IDR帧
 * @param VencChn 编码通道号
 * @return VENC_SUCCESS 成功, VENC_FAILURE 失败
 */
int VideoEncoder_RequestIDR(int VencChn);

/**
 * @brief 使能/禁用帧内刷新
 * @param VencChn 编码通道号
 * @param enable 1=使能, 0=禁用
 * @return VENC_SUCCESS 成功, VENC_FAILURE 失败
 */
int VideoEncoder_EnableIntraRefresh(int VencChn, int enable);

/*
 * ===== 属性获取 =====
 */

/**
 * @brief 获取编码通道属性
 * @param VencChn 编码通道号
 * @param pAttr 输出通道属性
 * @return VENC_SUCCESS 成功, VENC_FAILURE 失败
 */
int VideoEncoder_GetChannelAttr(int VencChn, VENC_CHN_ATTR_S* pAttr);

/**
 * @brief 设置编码通道属性
 * @param VencChn 编码通道号
 * @param pAttr 通道属性配置
 * @return VENC_SUCCESS 成功, VENC_FAILURE 失败
 * @note 通道必须先停止才能设置属性
 */
int VideoEncoder_SetChannelAttr(int VencChn, VENC_CHN_ATTR_S* pAttr);

/**
 * @brief 获取编码流信息
 * @param VencChn 编码通道号
 * @param pStream 输出流信息
 * @return VENC_SUCCESS 成功, VENC_FAILURE 失败
 */
int VideoEncoder_GetStreamInfo(int VencChn, VENC_STREAM_S* pStream);

/**
 * @brief 设置码流校验模式
 * @param VencChn 编码通道号
 * @param checkMode 校验模式（VENC_STREAM_CHECK_ON/OFF）
 * @return VENC_SUCCESS 成功, VENC_FAILURE 失败
 */
int VideoEncoder_SetStreamCheck(int VencChn, int checkMode);

/**
 * @brief 设置编码输出帧率
 * @param VencChn 编码通道号
 * @param frameRate 输出帧率
 * @return VENC_SUCCESS 成功, VENC_FAILURE 失败
 */
int VideoEncoder_SetOutFrameRate(int VencChn, int frameRate);

/**
 * @brief 获取码率控制参数
 * @param VencChn 编码通道号
 * @param pRcParam 输出码率控制参数
 * @return VENC_SUCCESS 成功, VENC_FAILURE 失败
 */
int VideoEncoder_GetRcParam(int VencChn, VENC_RC_PARAM_S* pRcParam);

/**
 * @brief 设置码率控制参数
 * @param VencChn 编码通道号
 * @param pRcParam 码率控制参数
 * @return VENC_SUCCESS 成功, VENC_FAILURE 失败
 */
int VideoEncoder_SetRcParam(int VencChn, VENC_RC_PARAM_S* pRcParam);

/*
 * ===== 旋转/镜像（特殊功能） =====
 */

/**
 * @brief 设置编码图像旋转角度
 * @param channel 编码通道号
 * @param Rotate 旋转角度（0/90/180/270）
 * @return VENC_SUCCESS 成功, VENC_FAILURE 失败
 */
int VideoEncoder_SetRotate(int channel, int Rotate);

/**
 * @brief 设置编码图像镜像/翻转
 * @param VencChn 编码通道号
 * @param bMirror 1=水平镜像, 0=不镜像
 * @param bFlip 1=垂直翻转, 0=不翻转
 * @return VENC_SUCCESS 成功, VENC_FAILURE 失败
 */
int VideoEncoder_SetMirror(int VencChn, int bMirror, int bFlip);
/**
 * @brief 更新编码通道的完整配置
 * @param channel 视频输入通道 (0-based)
 * @param dwType 码流类型 (CHL_MAIN_T/CHL_2END_T/CHL_JPEG_T)
 * @param info 编码参数配置
 * @return 0成功，-1失败
 * 
 * 功能：
 * - 根据参数变化决定是否重建编码器
 * - 更新编码器属性（分辨率、帧率、码率、QP等）
 * - 触发OSD/LOGO重建（分辨率变化时）
 * - 微调主码流RC参数
 * - 同步JPEG通道（子码流变化时）
 */
int VideoEncoder_UpdateChannelConfig(int channel, DWORD dwType, channel_info *info);

#ifdef __cplusplus
}
#endif

#endif /* _VIDEO_ENCODER_H_ */
