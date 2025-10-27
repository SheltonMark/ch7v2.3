/*
 * video_vps.h -- VPS (Video Processing System) 缩放/处理模块
 *
 */

#ifndef VIDEO_VPS_H
#define VIDEO_VPS_H

#include "../../Include/APIs/TdCommon.h"
#include "../../Include/APIs/Video.h"

/**
 * @brief Apply all saved cover/mask regions to VPS channels
 *
 * This function reapplies all saved cover/mask configurations from GlobalDevice.
 * Typically called during initialization or after resolution changes.
 *
 * @return 0 on success, negative on error
 */
int VideoVPS_SetCoverRegion(void);

/**
 * @brief Set a single cover/mask region
 *
 * Sets privacy mask (cover) for a specific region. Input coordinates are in
 * CIF reference system and will be scaled to actual stream resolutions.
 *
 * @param channel Video channel index
 * @param index Mask region index (0 to VPS_MASK_MAX_CHN_NUM-1)
 * @param pParam Cover parameters (coordinates, color, enable)
 * @return 0 on success, negative on error
 */
int VideoVPS_SetCover(int channel, int index, VIDEO_COVER_PARAM *pParam);

#endif /* VIDEO_VPS_H */
