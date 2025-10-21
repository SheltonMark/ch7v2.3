/**
 * @file video_overlay.h
 * @brief Video overlay (OSD) interface
 *
 * This header provides functions for OSD initialization,
 * replacing the original SDK_COMM_OSD_Creat() direct calls.
 */

#ifndef __VIDEO_OVERLAY_H__
#define __VIDEO_OVERLAY_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize video overlay (OSD)
 *
 * This function should be called after VideoPipeline_Init()
 * and before capture_init(), as OSD depends on VPS channel resolution.
 *
 * @return 0 on success, -1 on failure (non-fatal)
 */
int VideoOverlay_Init(void);

/**
 * @brief Destroy video overlay (OSD)
 *
 * @return 0 on success, -1 on failure
 */
int VideoOverlay_Destroy(void);

#ifdef __cplusplus
}
#endif

#endif /* __VIDEO_OVERLAY_H__ */
