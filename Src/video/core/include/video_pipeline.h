/**
 * @file video_pipeline.h
 * @brief Video pipeline high-level interface
 *
 * This header provides high-level functions for video pipeline management,
 * replacing the original video_in_init() and related functions.
 */

#ifndef __VIDEO_PIPELINE_H__
#define __VIDEO_PIPELINE_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize complete video pipeline
 *
 * This function replaces video_in_init() and performs:
 * - VI (Video Input) initialization
 * - ISP initialization
 * - VPS (Video Processing/Scaling) channel creation
 * - VENC (Video Encoding) channel creation
 * - System binding (VI→VPS→VENC)
 * - VD (Video Detection) channel creation for motion detection
 *
 * @return 0 on success, -1 on failure
 */
int VideoPipeline_Init(void);

/**
 * @brief Destroy video pipeline
 *
 * This function performs cleanup in reverse order:
 * - Unbind VPS→VENC
 * - Stop and destroy VENC channels
 * - Unbind VI→VPS
 * - Destroy VPS channels
 * - Stop VI
 *
 * @return 0 on success, -1 on failure
 */
int VideoPipeline_Destroy(void);

#ifdef __cplusplus
}
#endif

#endif /* __VIDEO_PIPELINE_H__ */
