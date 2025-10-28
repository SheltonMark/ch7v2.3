/**
 * @file video_frame_rate.h
 * @brief Video Frame Rate Adaptation Module
 * @details Handles sensor frame rate changes and applies to all encoding channels
 *
 */

#ifndef VIDEO_FRAME_RATE_H
#define VIDEO_FRAME_RATE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Sensor frame rate change callback (called by ISP/sensor driver)
 * @param FrameRate New sensor frame rate (float value)
 * @return SUCCESS on success, FAILED on failure
 *
 * This is the top-level interface called by external modules (e.g., ISP driver).
 * When significant frame rate change is detected (2x or 0.5x relationship),
 * all encoding channels are updated.
 *
 * Detection logic: triggers when (old_fps == new_fps * 2) || (old_fps == new_fps / 2)
 * Example: 30fps ↔ 15fps (night mode frame rate reduction)
 *
 * Notes:
 * - Synchronous operation (may take 200-500ms)
 * - Updates global variables: gFrameRate and sensor_fps
 */
int get_frame_rate(float FrameRate);

/**
 * @brief Handle sensor frame rate change event (internal implementation)
 * @param channel Video input channel (usually 0)
 * @param new_sensor_fps New sensor frame rate
 * @return 0 on success, <0 on failure
 *
 * Operation sequence:
 * 1. Set VPS input frame rate (sensor → VPS)
 * 2. For each encoding channel:
 *    - Stop encoder
 *    - Update VPS output frame rate
 *    - Update VENC frame rate attributes
 *    - Start encoder
 *
 * Notes:
 * - This function is synchronous and may take 200-500ms
 * - All encoding channels are updated atomically
 * - OSD/Logo are NOT rebuilt (resolution unchanged)
 */
int VideoFrameRate_HandleSensorChange(int channel, unsigned int new_sensor_fps);

#ifdef __cplusplus
}
#endif

#endif /* VIDEO_FRAME_RATE_H */
