/**
 * @file video_osd.h
 * @brief Video OSD (On-Screen Display) module
 *
 * This module handles all OSD-related operations including:
 * - Bitmap manipulation (enlarge, bit-switch, edge-adding)
 * - OSD coordinate calculation
 * - OSD/Logo creation and management
 *
 * All SDK-specific calls are abstracted through platform adapter layer.
 */

#ifndef __VIDEO_OSD_H__
#define __VIDEO_OSD_H__

#include "TdCommon.h"
#include "sdk_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================
 * OSD Region Management Functions
 * ======================================================================== */

/**
 * @brief Apply all configured OSD titles from device settings
 * Reads title configurations from RegionDevice and applies them to all channels
 * @return 0 on success, <0 on failure
 */
int VideoOSD_SetTitleRegion(void);

/**
 * @brief Apply configured logo from device settings
 * Reads logo configuration from RegionDevice and applies it if enabled
 * @return 0 on success, <0 on failure
 */
int VideoOSD_SetLogoRegion(void);

/**
 * @brief Create OSD regions for title display
 * Creates 8 OSD handles (0-7): 4 for main stream + 4 for sub stream
 * Uses 2BIT pixel format for titles
 * @return 0 on success, -1 on error
 */
int VideoOSD_Create(void);

/**
 * @brief Destroy OSD regions for title display
 * Destroys 8 OSD handles (0-7)
 * @return 0 on success, -1 on error
 */
int VideoOSD_Destroy(void);

/**
 * @brief Create Logo regions for logo display
 * Creates 2 OSD handles (9-10): handle 9 for main stream, handle 10 for sub stream
 * Uses 4BIT pixel format for logos
 * @return 0 on success, -1 on error
 */
int VideoOSD_CreateLogo(void);

/**
 * @brief Destroy Logo regions for logo display
 * Destroys 2 OSD handles (9-10)
 * Sets LogoInit flag to FALSE
 * @return 0 on success, -1 on error
 */
int VideoOSD_DestroyLogo(void);

/**
 * @brief Initialize video overlay (OSD) system
 * Creates basic OSD configuration and attaches to VPS channel
 * @return 0 on success, -1 on failure (non-fatal)
 */
int VideoOSD_Init(void);

/**
 * @brief Set OSD language
 * @param language Language code (0 = English, 1 = Chinese)
 * @return 0 on success, <0 on failure
 */
int VideoOSD_SetLanguage(int language);

/**
 * @brief Set OSD title font size
 * @param channel Channel number
 * @param size Font size
 * @return 0 on success
 */
int VideoOSD_SetTitleSize(int channel, unsigned int size);

/**
 * @brief Set OSD title for a channel
 * @param channel Channel number (must be 0)
 * @param pParam Title parameters (position, size, raster data, colors, etc.)
 * @return 0 on success, FAILED on error
 */
int VideoOSD_SetTitle(int channel, VIDEO_TITLE_PARAM *pParam);

/**
 * @brief Set logo for a channel
 * @param channel Channel number (must be 0)
 * @param pParam Logo parameters (position, enable, fg_color, AutoBlackWhite)
 * @return 0 on success, FAILED on error
 */
int VideoOSD_SetLogo(int channel, VIDEO_TITLE_PARAM *pParam);

#ifdef __cplusplus
}
#endif

#endif /* __VIDEO_OSD_H__ */
