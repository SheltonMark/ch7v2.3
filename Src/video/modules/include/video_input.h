/**
 * @file video_input.h
 * @brief Video Input Module Interface
 * @details 视频输入模块，处理 VI (Video Input) 和 Sensor 相关操作
 */

#ifndef VIDEO_INPUT_H
#define VIDEO_INPUT_H

#include "../../Include/APIs/TdCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Return codes */
#define VI_SUCCESS  0
#define VI_FAILURE  -1

/**
 * @brief 设置镜像和翻转
 * @param channel 通道号
 * @param mirror 镜像标志 (0=关闭, 1=开启)
 * @param flip 翻转标志 (0=关闭, 1=开启)
 * @return VI_SUCCESS 成功, VI_FAILURE 失败
 * @note 这是 VI/Sensor 层面的操作，需要 Disable → Set → Enable VI 设备
 */
int VideoInput_SetMirrorAndFlip(int channel, int mirror, int flip);

/**
 * @brief 处理输入格式变化（ISP 帧率同步）
 *
 * 当通道分辨率或帧率变化时，需要同时更新 ISP、VPS 和 Sensor 的帧率设置。
 * 这是三者联动的关键功能。
 *
 * fps 从 pVideoInDevice->VDInfo[channel].ViDstFps 读取，如果是 4K 分辨率则固定为 15fps
 *
 * 内部流程：
 * - 确定目标帧率（4K=15fps，否则从 GlobalDevice 读取）
 * - 获取当前 Sensor 信息和帧率
 * - 获取/设置 ISP 公共属性（帧率）
 * - 获取/设置 ISP 视频格式信息
 * - 处理 ISP 自动降帧控制
 * - 获取/设置 VPS Group 输入帧率
 * - 更新全局设备配置
 *
 * @param channel   物理通道号
 * @param pstSize   分辨率（宽度和高度）
 * @return VI_SUCCESS 成功, VI_FAILURE 失败
 */
int VideoInput_HandleFormatChange(int channel, SIZE_S pstSize);

#ifdef __cplusplus
}
#endif

#endif /* VIDEO_INPUT_H */
