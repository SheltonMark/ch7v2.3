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

/**
 * @brief Set VPS input frame rate
 *
 * Sets VPS input frame rate with special handling for 12fps.
 * For 12fps, uses FpsDenom=2 and InFps=25 to achieve 12.5fps ≈ 12fps.
 *
 * @param channel Video channel index
 * @param FrmRate Target frame rate (e.g., 12, 25, 30)
 * @return 0 on success, negative on error
 */
int VideoVPS_SetInputFrameRate(int channel, unsigned int FrmRate);

/**
 * @brief Set VPS output parameters
 *
 * Sets VPS channel output parameters (resolution, frame rate, encoding type).
 * Automatically destroys and recreates OSD/Logo when resolution changes.
 *
 * @param channel Video channel index
 * @param dwType Stream type (CHL_MAIN_T, CHL_2END_T)
 * @param ChnAttr Output attributes (width, height, fps, payload type)
 * @return 0 on success, negative on error
 */
int VideoVPS_SetOutputParam(int channel, DWORD dwType, VPS_CHN_OUT_ATTR *ChnAttr);

#endif /* VIDEO_VPS_H */
