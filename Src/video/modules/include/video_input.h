/**
 * @file video_input.h
 * @brief Video Input Module Interface
 * @details 视频输入模块，处理 VI (Video Input) 和 Sensor 相关操作
 */

#ifndef VIDEO_INPUT_H
#define VIDEO_INPUT_H

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

#ifdef __cplusplus
}
#endif

#endif /* VIDEO_INPUT_H */
