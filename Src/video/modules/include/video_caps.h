/**
 * @file video_caps.h
 * @brief Video Capability Query Module - Header File
 *
 * This module provides functions to query video encoding capabilities,
 * including chip encoding capabilities, extra stream capabilities,
 * and supported video formats/resolutions.
 */

#ifndef VIDEO_CAPS_H
#define VIDEO_CAPS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get encoder chip capability
 *
 * Retrieves encoding power capabilities for each video channel,
 * including maximum encoding power and supported channel count.
 *
 * @param pEncChipCapability Output buffer for chip capability information
 * @return 0 on success, negative on error
 */
int VideoGetEncChipCapability(VIDEO_ENCCHIPCAPABILITY *pEncChipCapability);

/**
 * @brief Get extra stream (sub-stream) capabilities
 *
 * Retrieves information about auxiliary streams supported by the encoder,
 * including stream types and resolution mapping between main and sub streams.
 *
 * @param pStream Output buffer for extra stream information
 * @return 0 on success, negative on error
 */
int VideoGetExtCaps(VIDEO_EXT_STREAM *pStream);

/**
 * @brief Get video encoding capabilities
 *
 * Retrieves comprehensive video encoding capabilities including:
 * - Supported compression formats (H.264, H.265, MJPEG, JPEG)
 * - Supported bit rate control modes (CBR, VBR)
 * - Supported image sizes (resolutions)
 * - Media types (video, audio)
 * - OSD capabilities (cover blocks, title count, region cover)
 *
 * @param pCaps Output buffer for video capabilities
 * @return 0 on success, negative on error
 */
int VideoGetCaps(VIDEO_CAPS *pCaps);

/**
 * @brief Get maximum sensor size and frame rate
 *
 * Retrieves the maximum resolution and frame rate supported by
 * the sensor/capture device.
 *
 * @param pCaps Output buffer for maximum size information
 * @return 0 on success, negative on error
 */
int VideoGetMaxSize(VIDEO_MAX_SIZE *pCaps);

#ifdef __cplusplus
}
#endif

#endif /* VIDEO_CAPS_H */
