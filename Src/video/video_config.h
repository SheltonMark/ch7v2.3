/**
 * @file video_config.h
 * @brief Video Global Configuration and Infrastructure
 * @details Centralized management of global variables, static lookup tables, and synchronization
 *
 * Responsibilities:
 * 1. Global runtime state (frame rate, sensor fps)
 * 2. Static configuration tables (image size, quality table)
 * 3. Synchronization primitives (osd_lock)
 *
 * Design philosophy:
 * - Eliminate scattered 'extern' declarations across modules
 * - Provide well-defined access interfaces (Getter/Setter)
 * - Hide implementation details (encapsulation)
 */

#ifndef VIDEO_CONFIG_H
#define VIDEO_CONFIG_H

#include "TdCommon.h"
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                         Frame Rate Management                              */
/* ========================================================================== */

/**
 * @brief Get current global frame rate
 * @return Current frame rate (fps)
 */
unsigned int VideoConfig_GetFrameRate(void);

/**
 * @brief Set current global frame rate
 * @param fps Frame rate to set
 */
void VideoConfig_SetFrameRate(unsigned int fps);

/**
 * @brief Get sensor frame rate
 * @return Sensor fps
 */
unsigned int VideoConfig_GetSensorFps(void);

/**
 * @brief Set sensor frame rate
 * @param fps Sensor fps to set
 */
void VideoConfig_SetSensorFps(unsigned int fps);

/* ========================================================================== */
/*                      Static Configuration Tables                           */
/* ========================================================================== */

/**
 * @brief Get image size for specified video standard and size enum
 * @param vstd Video standard (0=PAL, 1=NTSC)
 * @param size Video size enumeration (video_size_t)
 * @return Pointer to SIZE_S structure, or NULL if invalid parameters
 *
 * Example:
 *   const SIZE_S* size = VideoConfig_GetImageSize(0, VIDEO_SIZE_1080P);
 *   if (size) {
 *       width = size->width;
 *       height = size->height;
 *   }
 */
const SIZE_S* VideoConfig_GetImageSize(int vstd, video_size_t size);

/**
 * @brief Get capture quality table for main stream (entire array)
 * @return Pointer to quality table array (6 levels: 0-5)
 *
 * Usage: const CaptureImageQuality_t *table = VideoConfig_GetQualityTable();
 *        int qp = table[level].ImaxQP;  // level: 0-5
 *
 * Quality levels:
 * 0 = Lowest quality (highest compression)
 * 5 = Highest quality (lowest compression)
 */
const CaptureImageQuality_t* VideoConfig_GetQualityTable(void);

/**
 * @brief Get capture quality table for sub stream (entire array)
 * @return Pointer to sub stream quality table array (6 levels: 0-5)
 *
 * Usage: const CaptureImageQuality_t *table = VideoConfig_GetSubstreamQualityTable();
 *        int qp = table[level].ImaxQP;  // level: 0-5
 *
 * Note: Sub stream uses different QP ranges compared to main stream
 */
const CaptureImageQuality_t* VideoConfig_GetSubstreamQualityTable(void);

/**
 * @brief Get font size for specified index
 * @param index Font size index (0=large, 1=medium, 2=small)
 * @return Font size in pixels, or 0 if invalid index
 */
int VideoConfig_GetFontSize(int index);

/* ========================================================================== */
/*                       OSD Synchronization                                  */
/* ========================================================================== */

/**
 * @brief Lock OSD mutex (for OSD/Logo drawing operations)
 * @note This function blocks until lock is acquired
 *       Use VideoConfig_UnlockOsd() to release
 *
 * Usage pattern:
 *   VideoConfig_LockOsd();
 *   // ... OSD operations ...
 *   VideoConfig_UnlockOsd();
 */
void VideoConfig_LockOsd(void);

/**
 * @brief Unlock OSD mutex
 * @note Must be called after VideoConfig_LockOsd()
 */
void VideoConfig_UnlockOsd(void);

/**
 * @brief Get raw OSD mutex pointer (for advanced use cases)
 * @return Pointer to osd_lock mutex
 * @warning Only use this if you need direct mutex access
 *          Prefer VideoConfig_LockOsd()/UnlockOsd() for safety
 */
pthread_mutex_t* VideoConfig_GetOsdLock(void);

/* ========================================================================== */
/*                       Module Initialization                                */
/* ========================================================================== */

/**
 * @brief Initialize video config module
 * @return 0 on success, <0 on failure
 *
 * Notes:
 * - Initializes osd_lock mutex
 * - Sets default frame rates (30fps)
 * - Must be called before using any other functions
 */
int VideoConfig_Init(void);

/**
 * @brief Cleanup video config module
 */
void VideoConfig_Deinit(void);

#ifdef __cplusplus
}
#endif

#endif /* VIDEO_CONFIG_H */
