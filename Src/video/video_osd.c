/**
 * @file video_osd.c
 * @brief OSD (On-Screen Display) Module - Professional Implementation
 *
 * This module provides comprehensive OSD functionality including:
 * - OSD region management (create/destroy)
 * - OSD content display (SetTitle, SetLogo)
 * - Bitmap processing (enlarge, convert, edge detection)
 * - Coordinate calculations and overlap resolution
 */

#include "TdCommon.h"
#include "sdk_comm.h"
#include "logo.h"
#include "mdk_vps.h"
#include "ni_comm_vps.h"
#include "video_encoder.h"
#include "video_input.h"
#include "video_vps.h"
#include "video_osd.h"
#include "video_config.h"
#include "platform_adapter.h"

/* ========================================================================
 * Global Constants and Macros
 * ======================================================================== */

/* OSD boundary limits */
#define MAX_OSD_WIDTH           4096
#define MAX_OSD_HEIGHT          2160
#define MAX_CHANNEL_ID          3
#define MAX_OSD_TYPE            4
#define MAX_OSD_HANDLE          15

/* OSD pixel format divisor (bytes per pixel) */
#define OSD_FORMAT_DIV_1BIT     8
#define OSD_FORMAT_DIV_2BIT     4
#define OSD_FORMAT_DIV_4BIT     2
#define OSD_FORMAT_DIV_BYTE     1

/* OSD region handles */
#define OSD_HANDLE_BASE         0
#define OSD_HANDLE_REGION_BASE  1
#define OSD_HANDLE_REGION_END   8
#define OSD_HANDLE_LOGO_BASE    9
#define OSD_HANDLE_LOGO_END     10

/* ========================================================================
 * Layer 1: Bitmap Processing (Internal)
 * ======================================================================== */

/**
 * @brief Get the pixel data divisor for a given OSD pixel format
 * @param fmt OSD pixel format
 * @return Divisor (bytes per pixel), or -1 on error
 */
static int _osd_get_format_divisor(OSD_PIXEL_FORMAT_E fmt)
{
    switch (fmt) {
        case OSD_PIXEL_FORMAT_BYTE:
            return OSD_FORMAT_DIV_BYTE;
        case OSD_PIXEL_FORMAT_4BIT:
            return OSD_FORMAT_DIV_4BIT;
        case OSD_PIXEL_FORMAT_2BIT:
            return OSD_FORMAT_DIV_2BIT;
        case OSD_PIXEL_FORMAT_1BIT:
            return OSD_FORMAT_DIV_1BIT;
        default:
            PRINT_ERROR("Unknown OSD pixel format: %d\n", fmt);
            return -1;
    }
}

/**
 * @brief Enlarge bitmap by 2x (for 1BIT format)
 * @param psrc Source bitmap
 * @param pdest Destination bitmap
 * @param src_width Source width in pixels
 * @param src_height Source height in pixels
 */
static void _osd_enlarge_2x_1bit(unsigned char *psrc, unsigned char *pdest,
                                  int src_width, int src_height)
{
    int i, j;
    unsigned char value;
    int stride = src_width / 8;
    int dest_stride = (src_width * 2) / 8;

    for (i = 0; i < src_height; i++) {
        for (j = 0; j < stride; j++) {
            value = *(psrc + i * stride + j);

            /* Expand each bit to 2x2 pixels */
            *(pdest + (i * 2) * dest_stride + (j * 2)) =
                (value & 0x80) | ((value & 0x80) >> 1) |
                ((value & 0x40) >> 1) | ((value & 0x40) >> 2) |
                ((value & 0x20) >> 2) | ((value & 0x20) >> 3) |
                ((value & 0x10) >> 3) | ((value & 0x10) >> 4);
            *(pdest + (i * 2 + 1) * dest_stride + (j * 2)) =
                *(pdest + (i * 2) * dest_stride + (j * 2));

            *(pdest + (i * 2) * dest_stride + (j * 2 + 1)) =
                ((value & 0x8) << 4) | ((value & 0x8) << 3) |
                ((value & 0x4) << 3) | ((value & 0x4) << 2) |
                ((value & 0x2) << 2) | ((value & 0x2) << 1) |
                ((value & 0x1) << 1) | (value & 0x1);
            *(pdest + (i * 2 + 1) * dest_stride + (j * 2 + 1)) =
                *(pdest + (i * 2) * dest_stride + (j * 2 + 1));
        }
    }
}

/**
 * @brief Enlarge bitmap by 2x (for 2BIT format)
 */
static void _osd_enlarge_2x_2bit(unsigned char *psrc, unsigned char *pdest,
                                  int src_width, int src_height)
{
    int i, j;
    unsigned char value;
    int stride = src_width / 4;
    int dest_stride = (src_width * 2) / 4;

    for (i = 0; i < src_height; i++) {
        for (j = 0; j < stride; j++) {
            value = *(psrc + i * stride + j);

            *(pdest + (i * 2) * dest_stride + (j * 2)) =
                (value & 0xc0) | ((value & 0xc0) >> 2) |
                ((value & 0x30) >> 2) | ((value & 0x30) >> 4);
            *(pdest + (i * 2 + 1) * dest_stride + (j * 2)) =
                *(pdest + (i * 2) * dest_stride + (j * 2));

            *(pdest + (i * 2) * dest_stride + (j * 2 + 1)) =
                ((value & 0xc) << 4) | ((value & 0xc) << 2) |
                ((value & 0x3) << 2) | (value & 0x3);
            *(pdest + (i * 2 + 1) * dest_stride + (j * 2 + 1)) =
                *(pdest + (i * 2) * dest_stride + (j * 2 + 1));
        }
    }
}

/**
 * @brief Enlarge bitmap by 4x (for 1BIT format)
 */
static void _osd_enlarge_4x_1bit(unsigned char *psrc, unsigned char *pdest,
                                  int src_width, int src_height)
{
    int i, j, k;
    unsigned char value;
    int stride = src_width / 8;
    int dest_stride = (src_width * 4) / 8;

    for (i = 0; i < src_height; i++) {
        for (j = 0; j < stride; j++) {
            value = *(psrc + i * stride + j);

            unsigned char col0 = (value & 0x80) | ((value & 0x80) >> 1) |
                                ((value & 0x80) >> 2) | ((value & 0x80) >> 3) |
                                ((value & 0x40) >> 3) | ((value & 0x40) >> 4) |
                                ((value & 0x40) >> 5) | ((value & 0x40) >> 6);
            for (k = 0; k < 4; k++)
                *(pdest + (i * 4 + k) * dest_stride + (j * 4)) = col0;

            unsigned char col1 = ((value & 0x20) << 2) | ((value & 0x20) << 1) |
                                 ((value & 0x20)) | ((value & 0x20) >> 1) |
                                 ((value & 0x10) >> 1) | ((value & 0x10) >> 2) |
                                 ((value & 0x10) >> 3) | ((value & 0x10) >> 4);
            for (k = 0; k < 4; k++)
                *(pdest + (i * 4 + k) * dest_stride + (j * 4 + 1)) = col1;

            unsigned char col2 = ((value & 0x8) << 4) | ((value & 0x8) << 3) |
                                 ((value & 0x8) << 2) | ((value & 0x8) << 1) |
                                 ((value & 0x4) << 1) | ((value & 0x4)) |
                                 ((value & 0x4) >> 1) | ((value & 0x4) >> 2);
            for (k = 0; k < 4; k++)
                *(pdest + (i * 4 + k) * dest_stride + (j * 4 + 2)) = col2;

            unsigned char col3 = ((value & 0x2) << 6) | ((value & 0x2) << 5) |
                                 ((value & 0x2) << 4) | ((value & 0x2) << 3) |
                                 ((value & 0x1) << 3) | ((value & 0x1) << 2) |
                                 ((value & 0x1) << 1) | ((value & 0x1));
            for (k = 0; k < 4; k++)
                *(pdest + (i * 4 + k) * dest_stride + (j * 4 + 3)) = col3;
        }
    }
}

/**
 * @brief Enlarge bitmap by 4x (for 2BIT format)
 */
static void _osd_enlarge_4x_2bit(unsigned char *psrc, unsigned char *pdest,
                                  int src_width, int src_height)
{
    int i, j, k;
    unsigned char value;
    int stride = src_width / 4;
    int dest_stride = (src_width * 4) / 4;

    for (i = 0; i < src_height; i++) {
        for (j = 0; j < stride; j++) {
            value = *(psrc + i * stride + j);

            unsigned char col0 = (value & 0xc0) | ((value & 0xc0) >> 2) |
                                ((value & 0xc0) >> 4) | ((value & 0xc0) >> 6);
            for (k = 0; k < 4; k++)
                *(pdest + (i * 4 + k) * dest_stride + (j * 4)) = col0;

            unsigned char col1 = ((value & 0x30) << 2) | (value & 0x30) |
                                 ((value & 0x30) >> 2) | ((value & 0x30) >> 4);
            for (k = 0; k < 4; k++)
                *(pdest + (i * 4 + k) * dest_stride + (j * 4 + 1)) = col1;

            unsigned char col2 = ((value & 0xc) << 4) | ((value & 0xc) << 2) |
                                 (value & 0xc) | ((value & 0xc) >> 2);
            for (k = 0; k < 4; k++)
                *(pdest + (i * 4 + k) * dest_stride + (j * 4 + 2)) = col2;

            unsigned char col3 = ((value & 0x3) << 6) | ((value & 0x3) << 4) |
                                 ((value & 0x3) << 2) | (value & 0x3);
            for (k = 0; k < 4; k++)
                *(pdest + (i * 4 + k) * dest_stride + (j * 4 + 3)) = col3;
        }
    }
}

/**
 * @brief Enlarge bitmap by the specified factor
 * @param psrc Source bitmap
 * @param pdest Destination bitmap
 * @param fmt OSD pixel format
 * @param factor Enlargement factor (1, 2, or 4)
 * @param psrc_size Source bitmap size
 * @param pdest_size Output bitmap size (updated)
 * @return 0 on success, -1 on error
 */
static int _osd_bitmap_enlarge(unsigned char *psrc, unsigned char *pdest,
                                OSD_PIXEL_FORMAT_E fmt, int factor,
                                SIZE_S *psrc_size, SIZE_S *pdest_size)
{
    if (!psrc || !pdest || !psrc_size || !pdest_size) {
        PRINT_ERROR("Invalid parameters for bitmap enlargement\n");
        return -1;
    }

    if (psrc_size->u32Width == 0 || psrc_size->u32Height == 0) {
        return 0;
    }

    int div = _osd_get_format_divisor(fmt);
    if (div < 0) return -1;

    /* Update destination size */
    pdest_size->u32Width = psrc_size->u32Width * factor;
    pdest_size->u32Height = psrc_size->u32Height * factor;

    /* Clear destination buffer */
    memset(pdest, 0, (pdest_size->u32Width * pdest_size->u32Height) / div);

    /* Handle different enlargement factors */
    if (factor == 1) {
        memcpy(pdest, psrc, (pdest_size->u32Width * pdest_size->u32Height) / div);
    } else if (factor == 2) {
        if (fmt == OSD_PIXEL_FORMAT_1BIT) {
            _osd_enlarge_2x_1bit(psrc, pdest, psrc_size->u32Width, psrc_size->u32Height);
        } else if (fmt == OSD_PIXEL_FORMAT_2BIT) {
            _osd_enlarge_2x_2bit(psrc, pdest, psrc_size->u32Width, psrc_size->u32Height);
        } else {
            PRINT_ERROR("Unsupported format for 2x enlargement: %d\n", fmt);
            return -1;
        }
    } else if (factor == 4) {
        if (fmt == OSD_PIXEL_FORMAT_1BIT) {
            _osd_enlarge_4x_1bit(psrc, pdest, psrc_size->u32Width, psrc_size->u32Height);
        } else if (fmt == OSD_PIXEL_FORMAT_2BIT) {
            _osd_enlarge_4x_2bit(psrc, pdest, psrc_size->u32Width, psrc_size->u32Height);
        } else {
            PRINT_ERROR("Unsupported format for 4x enlargement: %d\n", fmt);
            return -1;
        }
    } else {
        PRINT_ERROR("Unsupported enlargement factor: %d\n", factor);
        return -1;
    }

    return 0;
}

/**
 * @brief Convert bitmap from one bit format to another
 * @param psrc Source bitmap (1BIT)
 * @param pdest Destination bitmap
 * @param dest_fmt Target pixel format
 * @param psize Bitmap size
 * @return 0 on success, -1 on error
 */
static int _osd_bitmap_convert_bits(unsigned char *psrc, unsigned char *pdest,
                                     OSD_PIXEL_FORMAT_E dest_fmt, SIZE_S *psize)
{
    if (!psrc || !pdest || !psize) {
        PRINT_ERROR("Invalid parameters for bit conversion\n");
        return -1;
    }

    int i, j;
    unsigned char value;
    int src_stride = psize->u32Width / 8;

    if (dest_fmt == OSD_PIXEL_FORMAT_2BIT) {
        int dest_stride = psize->u32Width / 4;
        for (i = 0; i < psize->u32Height; i++) {
            for (j = 0; j < src_stride; j++) {
                value = *(psrc + i * src_stride + j);

                *(pdest + i * dest_stride + (j * 2)) =
                    ((value & 0x80) >> 1) | ((value & 0x40) >> 2) |
                    ((value & 0x20) >> 3) | ((value & 0x10) >> 4);

                *(pdest + i * dest_stride + (j * 2 + 1)) =
                    ((value & 0x8) << 3) | ((value & 0x4) << 2) |
                    ((value & 0x2) << 1) | (value & 0x1);
            }
        }
    } else if (dest_fmt == OSD_PIXEL_FORMAT_4BIT) {
        int dest_stride = psize->u32Width / 2;
        for (i = 0; i < psize->u32Height; i++) {
            for (j = 0; j < src_stride; j++) {
                value = *(psrc + i * src_stride + j);

                *(pdest + i * dest_stride + (j * 4)) =
                    ((value & 0x80) >> 1) | ((value & 0x40) >> 4);
                *(pdest + i * dest_stride + (j * 4 + 1)) =
                    ((value & 0x20) << 1) | ((value & 0x10) >> 2);
                *(pdest + i * dest_stride + (j * 4 + 2)) =
                    ((value & 0x8) << 3) | ((value & 0x4));
                *(pdest + i * dest_stride + (j * 4 + 3)) =
                    ((value & 0x2) << 5) | ((value & 0x1) << 2);
            }
        }
    } else if (dest_fmt == OSD_PIXEL_FORMAT_1BIT) {
        memcpy(pdest, psrc, (psize->u32Width * psize->u32Height) / 8);
    } else {
        PRINT_ERROR("Unsupported bit conversion format: %d\n", dest_fmt);
        return -1;
    }

    return 0;
}


/**
 * @brief Add edge detection to OSD bitmap
 * @param pbmp Bitmap (2BIT format)
 * @param enlarge_factor Original enlargement factor (affects edge thickness)
 * @param psize Bitmap size
 * @return 0 on success, -1 on error
 */
static int _osd_bitmap_add_edge(unsigned char *pbmp, int enlarge_factor, SIZE_S *psize)
{
    if (!pbmp || !psize) {
        PRINT_ERROR("Invalid parameters for edge detection\n");
        return -1;
    }

    /* Static buffer for temporary bitmap */
    static unsigned char *pbmp_temp = NULL;
    if (pbmp_temp == NULL) {
        pbmp_temp = malloc(MAX_OSD_WIDTH * MAX_OSD_HEIGHT / 4);
        if (pbmp_temp == NULL) {
            PRINT_ERROR("Failed to allocate edge detection buffer\n");
            return -1;
        }
    }

    /* Copy original bitmap for edge detection */
    int size = (psize->u32Width * psize->u32Height) / 4;
    memcpy(pbmp_temp, pbmp, size);

    /* Edge detection algorithm (2BIT format) */
    int i, j, m, idx, xl, xr, yu, yd;
    unsigned char s, sum;
    int stride = psize->u32Width / 4;

    for (i = 0; i < psize->u32Height; i++) {
        for (j = 0; j < stride; j++) {
            for (m = 0; m < 4; m++) {
                s = *(pbmp_temp + i * stride + j);
                sum = (s >> (2 * (3 - m))) & 0x3;

                if (sum == 0) {
                    /* Pixel is empty, check neighbors */
                    int neighbor_sum = 0;

                    xl = (j > 0) ? j - 1 : j;
                    xr = (j < stride - 1) ? j + 1 : j;
                    yu = (i > 0) ? i - 1 : i;
                    yd = (i < psize->u32Height - 1) ? i + 1 : i;

                    /* Check all 8 neighbors */
                    if (i > 0) {
                        s = *(pbmp_temp + yu * stride + j);
                        if (((s >> (2 * (3 - m))) & 0x3) != 0) neighbor_sum++;
                    }
                    if (i < psize->u32Height - 1) {
                        s = *(pbmp_temp + yd * stride + j);
                        if (((s >> (2 * (3 - m))) & 0x3) != 0) neighbor_sum++;
                    }
                    if (j > 0) {
                        s = *(pbmp_temp + i * stride + xl);
                        if (((s >> (2 * m)) & 0x3) != 0) neighbor_sum++;
                    }
                    if (j < stride - 1) {
                        s = *(pbmp_temp + i * stride + xr);
                        if (((s >> (2 * m)) & 0x3) != 0) neighbor_sum++;
                    }

                    /* Set edge pixel if neighbor contains data */
                    if (neighbor_sum > 0) {
                        *(pbmp + i * stride + j) |= (0x2 << (2 * (3 - m)));
                    }
                }
            }
        }
    }

    return 0;
}

/**
 * @brief Debug function to print OSD error information
 * @param stMdkChn MDK channel info
 * @param stOsdDispAttr OSD display attributes
 * @return Always returns 0
 */
int _osd_error(MDK_CHN_S *stMdkChn, OSD_DISP_ATTR_S *stOsdDispAttr)
{
	SystemAsh("cat /proc/umap/osd");

	PRINT_ERROR("stMdkChn->enModId %d\n", stMdkChn->enModId);
	PRINT_ERROR("stMdkChn->s32ChnId %d\n", stMdkChn->s32ChnId);
	PRINT_ERROR("stMdkChn->s32DevId %d\n", stMdkChn->s32DevId);

	PRINT_ERROR("stOsdDispAttr->bShow %d\n", stOsdDispAttr->bShow);
	PRINT_ERROR("stOsdDispAttr->stSize.u32Height %d\n", stOsdDispAttr->stSize.u32Height);
	PRINT_ERROR("stOsdDispAttr->stSize.u32Width %d\n", stOsdDispAttr->stSize.u32Width);
	PRINT_ERROR("stOsdDispAttr->stStartPiont.s32X %d\n", stOsdDispAttr->stStartPiont.s32X);
	PRINT_ERROR("stOsdDispAttr->stStartPiont.s32Y %d\n", stOsdDispAttr->stStartPiont.s32Y);
	PRINT_ERROR("stOsdDispAttr->u32Layer %d\n", stOsdDispAttr->u32Layer);
	PRINT_ERROR("stOsdDispAttr->enAlphaMode %d\n", stOsdDispAttr->enAlphaMode);
	PRINT_ERROR("stOsdDispAttr->u32RegionAlpha %d\n", stOsdDispAttr->u32RegionAlpha);
	PRINT_ERROR("stOsdDispAttr->bInvert %d\n", stOsdDispAttr->bInvert);
	PRINT_ERROR("stOsdDispAttr->bSpecialFrmId %d\n", stOsdDispAttr->bSpecialFrmId);
	PRINT_ERROR("stOsdDispAttr->u32TargetFrameId %d\n", stOsdDispAttr->u32TargetFrameId);

	return 0;
}


/**
 * @brief Convert logo bitmap from 1BIT to target format (for logo display)
 * @param psrc Source bitmap (1BIT format)
 * @param pdest Destination bitmap
 * @param fmt Target pixel format (2BIT or 4BIT)
 * @param psize Bitmap size
 * @return 0 on success, -1 on error
 *
 * Note: This function is similar to _osd_bitmap_convert_bits but with slightly
 * different bit arrangements optimized for logo display.
 */
static int _osd_bit_switch(unsigned char *psrc, unsigned char *pdest,
                            OSD_PIXEL_FORMAT_E fmt, SIZE_S *psize)
{
    if (!psrc || !pdest || !psize) {
        PRINT_ERROR("_osd_bit_switch: Invalid parameters\n");
        return -1;
    }

    int i, j;
    unsigned char value;
    int stride = psize->u32Width / 8;

    if (fmt == OSD_PIXEL_FORMAT_4BIT) {
        /* Convert 1BIT to 4BIT for logo */
        for (i = 0; i < psize->u32Height; i++) {
            for (j = 0; j < stride; j++) {
                value = *(psrc + i * stride + j);

                *(pdest + 4 * (i * stride + j)) =
                    ((value & 0x80) >> 1) | ((value & 0x40) >> 4);
                *(pdest + 4 * (i * stride + j) + 1) =
                    ((value & 0x20) << 1) | ((value & 0x10) >> 2);
                *(pdest + 4 * (i * stride + j) + 2) =
                    ((value & 0x8) << 3) | ((value & 0x4));
                *(pdest + 4 * (i * stride + j) + 3) =
                    ((value & 0x2) << 5) | ((value & 0x1) << 2);
            }
        }
    } else if (fmt == OSD_PIXEL_FORMAT_2BIT) {
        /* Convert 1BIT to 2BIT for logo */
        for (i = 0; i < psize->u32Height; i++) {
            for (j = 0; j < stride; j++) {
                value = *(psrc + i * stride + j);

                *(pdest + 2 * (i * stride + j)) =
                    ((value & 0x80) >> 1) | ((value & 0x40) >> 2) |
                    ((value & 0x20) >> 3) | ((value & 0x10) >> 4);
                *(pdest + 2 * (i * stride + j) + 1) =
                    ((value & 0x8) << 3) | ((value & 0x4) << 2) |
                    ((value & 0x2) << 1) | (value & 0x1);
            }
        }
    } else if (fmt == OSD_PIXEL_FORMAT_1BIT) {
        /* Direct copy for 1BIT */
        memcpy(pdest, psrc, psize->u32Width * psize->u32Height / 8);
    } else if (fmt == OSD_PIXEL_FORMAT_BYTE) {
        PRINT_ERROR("_osd_bit_switch: BYTE format not supported\n");
        return -1;
    } else {
        PRINT_ERROR("_osd_bit_switch: Unknown pixel format %d\n", fmt);
        return -1;
    }

    return 0;
}

/**
 * @brief Complete OSD bitmap processing with conversion, enlargement, and edge detection
 * @param psrc Source bitmap (1BIT format)
 * @param pdest Destination bitmap
 * @param fmt Target pixel format
 * @param enlarge_factor Enlargement factor (1, 2, or 4)
 * @param psrc_size Source bitmap size
 * @return 0 on success, -1 on error
 */
static int _vsf_drv_enlarge_osd(unsigned char *psrc, unsigned char *pdest,
                                  OSD_PIXEL_FORMAT_E fmt, unsigned int enlarge_factor,
                                  SIZE_S *psrc_size)
{
    if (!psrc || !pdest || !psrc_size) {
        PRINT_ERROR("_vsf_drv_enlarge_osd: Invalid parameters\n");
        return -1;
    }

    if (psrc_size->u32Width == 0 || psrc_size->u32Height == 0) {
        return 0;
    }

    /* Temporary buffer for bit conversion (max 32 chars @ 16x16) */
    static unsigned char *temp_buf = NULL;
    if (temp_buf == NULL) {
        temp_buf = malloc(16 * 16 * 32 / 4);
        if (temp_buf == NULL) {
            PRINT_ERROR("Failed to allocate temp buffer for bit conversion\n");
            return -1;
        }
    }

    memset(temp_buf, 0, 16 * 16 * 32 / 4);

    /* Step 1: Convert 1BIT to target format */
    int ret = _osd_bitmap_convert_bits(psrc, temp_buf, fmt, psrc_size);
    if (ret != 0) {
        PRINT_ERROR("_osd_bitmap_convert_bits failed\n");
        return -1;
    }

    /* Step 2: Enlarge bitmap */
    SIZE_S dest_size;
    ret = _osd_bitmap_enlarge(temp_buf, pdest, fmt, enlarge_factor, psrc_size, &dest_size);
    if (ret != 0) {
        PRINT_ERROR("_osd_bitmap_enlarge failed\n");
        return -1;
    }

    /* Step 3: Add edge detection (only for 2BIT format) */
    if (fmt == OSD_PIXEL_FORMAT_2BIT) {
        ret = _osd_bitmap_add_edge(pdest, enlarge_factor, &dest_size);
        if (ret != 0) {
            PRINT_ERROR("_osd_bitmap_add_edge failed\n");
            return -1;
        }
    }

    return 0;
}

/* ========================================================================
 * Layer 2: Coordinate Processing (Internal)
 * ======================================================================== */

/**
 * @brief Resolve OSD overlay conflicts and adjust positions
 * @param ichannel Channel ID
 * @param type OSD type to check against
 * @param osd_size Current OSD size
 * @param enlarge_num Enlargement factor
 * @param post_point OSD position (may be adjusted)
 * @return 0 on success, -1 on error
 */
static int _osd_overlay_calculate(int ichannel, OSD_TYPE type, SIZE_S osd_size,
                                   unsigned int enlarge_num, POINT_S *post_point)
{
    CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
    PlatformAdapter* adapter = GetPlatformAdapter();

    if (!post_point || !adapter || !adapter->osd_get_disp_attr || !adapter->osd_paint_to_chn) {
        return -1;
    }

    /* Calculate OSD handle based on channel and type */
    int u32OsdHandle = (ichannel == STREAM_TYPE_FIR) ? type : (type + 4);

    MDK_CHN_S stMdkChn;
    OSD_DISP_ATTR_S stOsdDispAttr;
    memset(&stMdkChn, 0, sizeof(MDK_CHN_S));
    memset(&stOsdDispAttr, 0, sizeof(OSD_DISP_ATTR_S));

    stMdkChn.enModId = NI_ID_VPS;
    stMdkChn.s32DevId = 0;
    stMdkChn.s32ChnId = ichannel;

    /* Get existing OSD attributes */
    int ret = adapter->osd_get_disp_attr(u32OsdHandle, &stMdkChn, &stOsdDispAttr);
    if (ret != RETURN_OK || !stOsdDispAttr.bShow) {
        return 0;  /* No conflict if OSD not enabled */
    }

    int pic_height = pCaptureDevice->EncDevice[0].StreamDevice[ichannel].EncChannel_info.max_height;
    int x = stOsdDispAttr.stStartPiont.s32X;
    int y = stOsdDispAttr.stStartPiont.s32Y;
    int w = stOsdDispAttr.stSize.u32Width;

    /* Check for overlap and adjust position */
    if (post_point->s32X <= x && post_point->s32X + osd_size.u32Width > x) {
        if ((post_point->s32Y >= y && post_point->s32Y < y + osd_size.u32Height) ||
            (post_point->s32Y + osd_size.u32Height > y && post_point->s32Y + osd_size.u32Height <= y + osd_size.u32Height)) {

            /* Resolve overlap based on OSD type priority */
            switch (type) {
                case OSD_TYPE_TIME:
                case OSD_TYPE_LABLE1:
                    post_point->s32Y = y + osd_size.u32Height;
                    if (post_point->s32Y + osd_size.u32Height > pic_height) {
                        post_point->s32Y = pic_height - osd_size.u32Height;
                        stOsdDispAttr.stStartPiont.s32Y = post_point->s32Y - osd_size.u32Height;
                    }
                    break;
                case OSD_TYPE_TITLE:
                case OSD_TYPE_LABLE2:
                    if (y < osd_size.u32Height) {
                        stOsdDispAttr.stStartPiont.s32Y = post_point->s32Y + osd_size.u32Height;
                    } else {
                        post_point->s32Y = y - osd_size.u32Height;
                    }
                    break;
                default:
                    break;
            }

            /* Repaint the conflicting OSD with new position */
            stOsdDispAttr.enInvertBlkMode = OSD_INVERT_BLK_NULL;
            ret = adapter->osd_paint_to_chn(u32OsdHandle, &stMdkChn, &stOsdDispAttr, 500);
            if (ret != RETURN_OK) {
                _osd_error(&stMdkChn, &stOsdDispAttr);
                PRINT_ERROR("_osd_overlay_calculate: osd_paint_to_chn failed with %x\n", ret);
            }
        }
    } else if (post_point->s32X >= x && post_point->s32X < x + w) {
        if ((post_point->s32Y >= y && post_point->s32Y < y + osd_size.u32Height) ||
            (post_point->s32Y + osd_size.u32Height > y && post_point->s32Y + osd_size.u32Height <= y + osd_size.u32Height)) {

            /* Same conflict resolution logic */
            switch (type) {
                case OSD_TYPE_TIME:
                case OSD_TYPE_LABLE1:
                    post_point->s32Y = y + osd_size.u32Height;
                    if (post_point->s32Y + osd_size.u32Height > pic_height) {
                        post_point->s32Y = pic_height - osd_size.u32Height;
                        stOsdDispAttr.stStartPiont.s32Y = post_point->s32Y - osd_size.u32Height;
                    }
                    break;
                case OSD_TYPE_TITLE:
                case OSD_TYPE_LABLE2:
                    if (y < osd_size.u32Height) {
                        stOsdDispAttr.stStartPiont.s32Y = post_point->s32Y + osd_size.u32Height;
                    } else {
                        post_point->s32Y = y - osd_size.u32Height;
                    }
                    break;
                default:
                    break;
            }

            stOsdDispAttr.enInvertBlkMode = OSD_INVERT_BLK_NULL;
            ret = adapter->osd_paint_to_chn(u32OsdHandle, &stMdkChn, &stOsdDispAttr, 500);
            if (ret != RETURN_OK) {
                _osd_error(&stMdkChn, &stOsdDispAttr);
                PRINT_ERROR("_osd_overlay_calculate: osd_paint_to_chn failed with %x\n", ret);
            }
        }
    }

    return 0;
}

/**
 * @brief Calculate OSD coordinates with CIF conversion and overlap resolution
 * @param ichannel Channel ID
 * @param u32OsdHandle OSD handle
 * @param enlarge_num Enlargement factor
 * @param osd_size OSD size
 * @param pre_point Input point in CIF coordinates
 * @param post_point Output point in actual coordinates
 * @return 0 on success, -1 on error
 */
static int _osd_coord_calculate(int ichannel, unsigned int u32OsdHandle,
                                 unsigned int enlarge_num, SIZE_S osd_size,
                                 POINT_S pre_point, POINT_S *post_point)
{
    if (!post_point) {
        PRINT_ERROR("_osd_coord_calculate: Invalid post_point\n");
        return -1;
    }

    CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

    /* CIF reference size */
    int cif_width = 352;
    int cif_height = 288;

    /* Get actual picture size */
    int pic_width = pCaptureDevice->EncDevice[0].StreamDevice[ichannel].EncChannel_info.max_width;
    int pic_height = pCaptureDevice->EncDevice[0].StreamDevice[ichannel].EncChannel_info.max_height;

    /* Convert from CIF coordinates to actual coordinates */
    post_point->s32X = (int)((float)pre_point.s32X * pic_width / cif_width);
    post_point->s32Y = (int)((float)pre_point.s32Y * pic_height / cif_height);

    /* Clamp to picture boundaries */
    if (post_point->s32X + osd_size.u32Width > pic_width) {
        post_point->s32X = pic_width - osd_size.u32Width;
    }
    if (post_point->s32Y + osd_size.u32Height > pic_height) {
        post_point->s32Y = pic_height - osd_size.u32Height;
    }

    /* Align to 2-pixel boundary (hardware requirement) */
    if (post_point->s32X % 2) post_point->s32X--;
    if (post_point->s32Y % 2) post_point->s32Y--;

    /* Resolve overlaps with other OSD regions */
    int type = u32OsdHandle % 4;

    if (type == OSD_TYPE_TIME) {
        _osd_overlay_calculate(ichannel, OSD_TYPE_TITLE, osd_size, enlarge_num, post_point);
    } else if (type == OSD_TYPE_TITLE) {
        _osd_overlay_calculate(ichannel, OSD_TYPE_TIME, osd_size, enlarge_num, post_point);
    } else if (type == OSD_TYPE_LABLE1) {
        _osd_overlay_calculate(ichannel, OSD_TYPE_LABLE2, osd_size, enlarge_num, post_point);
    } else if (type == OSD_TYPE_LABLE2) {
        _osd_overlay_calculate(ichannel, OSD_TYPE_LABLE1, osd_size, enlarge_num, post_point);
    }

    return 0;
}						


/* ========================================================================
 * Layer 3: Public API
 * ======================================================================== */
 
int VideoOSD_SetTitleRegion()
{
	int ret = -1;
	VIDEO_TITLE_PARAM pParam;
	RegionDevice_p pRegionDevice = &GlobalDevice.RegionDevice;
	memset(&pParam, 0, sizeof(VIDEO_TITLE_PARAM));

	VideoOSD_Create();

	for (int i = 0; i < TD_OSD_MAX_NUM; i++)
	{
		pParam.index = i;
		pParam.width = pRegionDevice->OsdWidth[i];
		pParam.height = 16;
		pParam.x = pRegionDevice->PositionX[i];
		pParam.y = pRegionDevice->PositionY[i];
		pParam.fg_color = pRegionDevice->FgColor;
		pParam.bg_color = pRegionDevice->BgColor;
		pParam.enable = pRegionDevice->show[i];
		pParam.AutoBlackWhite = pRegionDevice->InvertEnable;
		pParam.raster = (unsigned char *)pRegionDevice->channeltitle[i];
		ret |= VideoOSD_SetTitle(0, &pParam);
	}

	return ret;
}

int VideoOSD_SetLogoRegion()
{
	int ret = -1;
	int channel = 0;
	VIDEO_TITLE_PARAM pParam;
	RegionDevice_p pRegionDevice = &GlobalDevice.RegionDevice;
	memset(&pParam, 0, sizeof(VIDEO_TITLE_PARAM));

	if (ENABLE != pRegionDevice->LogoEnable)
	{
		return SUCCESS;
	}

	pParam.enable = pRegionDevice->LogoEnable;
	pParam.x = pRegionDevice->LogoX;
	pParam.y = pRegionDevice->LogoY;
	pParam.fg_color = pRegionDevice->FgColor;

	ret = VideoOSD_SetLogo(channel, &pParam);

	return ret;
}

/**
 * @brief Create OSD regions for title display (Internal)
 * Creates 8 OSD handles (0-7): 4 for main stream + 4 for sub stream
 * @return 0 on success, -1 on error
 */
int VideoOSD_Create(void)
{
	OSD_REGION_S stOsdRegion;
	unsigned int u32OsdHandle = 0;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
	PlatformAdapter* adapter = GetPlatformAdapter();

	if (!adapter || !adapter->osd_create_region) {
		PRINT_ERROR("VideoOSD_Create: OSD adapter not available\n");
		return -1;
	}

	memset(&stOsdRegion, 0, sizeof(OSD_REGION_S));

	stOsdRegion.bMacroFormat = NI_FALSE;
	stOsdRegion.enOsdRgn = OSD_RGN;
	stOsdRegion.enPixelFormat = OSD_PIXEL_FORMAT_2BIT;

	for (int i = 0; i < pCaptureDevice->EncDevice[0].StreamCount; i++)
	{
		// 跳过JPEG编码通道
		if (i == CHL_3IRD_T) {
			break;
		}
		for (int j = 0; j < 4; j++)
		{
			if (adapter->osd_create_region(u32OsdHandle, &stOsdRegion) != RETURN_OK) {
				PRINT_ERROR("VideoOSD_Create: Failed to create OSD handle %d\n", u32OsdHandle);
				return -1;
			}
			u32OsdHandle++;
		}
	}

	return SUCCESS;
}

/**
 * @brief Destroy OSD regions for title display (Internal)
 * Destroys 8 OSD handles (0-7)
 * @return 0 on success, -1 on error
 */
int VideoOSD_Destroy(void)
{
	MDK_CHN_S stMdkChn;
	unsigned int u32OsdHandle = 0;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
	PlatformAdapter* adapter = GetPlatformAdapter();

	if (!adapter || !adapter->osd_clear_from_chn || !adapter->osd_destroy_region) {
		PRINT_ERROR("VideoOSD_Destroy: OSD adapter not available\n");
		return -1;
	}

	memset(&stMdkChn, 0, sizeof(MDK_CHN_S));
	stMdkChn.enModId = NI_ID_VPS;
	stMdkChn.s32DevId = 0;
	stMdkChn.s32ChnId = 0;

	VideoConfig_LockOsd();

	for (int i = 0; i < pCaptureDevice->EncDevice[0].StreamCount; i++)
	{
		// 跳过JPEG编码通道
		if (i == CHL_3IRD_T) {
			break;
		}

		for (int j = 0; j < 4; j++)
		{
			adapter->osd_clear_from_chn(u32OsdHandle, &stMdkChn, 500);
			adapter->osd_destroy_region(u32OsdHandle);
			u32OsdHandle++;
		}
		stMdkChn.s32ChnId++;
	}

	VideoConfig_UnlockOsd();

	return SUCCESS;
}

/**
 * @brief Create OSD regions for logo display (Internal)
 * Creates 2 OSD handles (9-10): handle 9 for main stream, handle 10 for sub stream
 * @return 0 on success, -1 on error
 */
int VideoOSD_CreateLogo(void)
{
	OSD_REGION_S stOsdRegion;
	unsigned int u32OsdHandle = 9;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
	PlatformAdapter* adapter = GetPlatformAdapter();

	if (!adapter || !adapter->osd_create_region) {
		PRINT_ERROR("VideoOSD_CreateLogo: OSD adapter not available\n");
		return -1;
	}

	memset(&stOsdRegion, 0, sizeof(OSD_REGION_S));

	stOsdRegion.bMacroFormat = NI_FALSE;
	stOsdRegion.enOsdRgn = OSD_RGN;
	stOsdRegion.enPixelFormat = OSD_PIXEL_FORMAT_4BIT;

	for (int i = 0; i < pCaptureDevice->EncDevice[0].StreamCount; i++)
	{
		// 跳过JPEG编码通道
		if (i == CHL_3IRD_T) {
			break;
		}
		if (adapter->osd_create_region(u32OsdHandle, &stOsdRegion) != RETURN_OK) {
			PRINT_ERROR("VideoOSD_CreateLogo: Failed to create logo handle %d\n", u32OsdHandle);
			return -1;
		}
		u32OsdHandle++;
	}

	return SUCCESS;
}

/**
 * @brief Destroy OSD regions for logo display (Internal)
 * Destroys 2 OSD handles (9-10)
 * @return 0 on success, -1 on error
 */
int VideoOSD_DestroyLogo(void)
{
	MDK_CHN_S stMdkChn;
	unsigned int u32OsdHandle = 9;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
	RegionDevice_p pRegionDevice = &GlobalDevice.RegionDevice;
	PlatformAdapter* adapter = GetPlatformAdapter();

	if (!adapter || !adapter->osd_clear_from_chn || !adapter->osd_destroy_region) {
		PRINT_ERROR("VideoOSD_DestroyLogo: OSD adapter not available\n");
		return -1;
	}

	memset(&stMdkChn, 0, sizeof(MDK_CHN_S));
	stMdkChn.enModId = NI_ID_VPS;
	stMdkChn.s32DevId = 0;
	stMdkChn.s32ChnId = 0;

	for (int i = 0; i < pCaptureDevice->EncDevice[0].StreamCount; i++)
	{
		// 跳过JPEG编码通道
		if (i == CHL_3IRD_T) {
			break;
		}

		adapter->osd_clear_from_chn(u32OsdHandle, &stMdkChn, 500);
		adapter->osd_destroy_region(u32OsdHandle);

		u32OsdHandle++;
		stMdkChn.s32ChnId++;
	}
	pRegionDevice->LogoInit = FALSE;

	return SUCCESS;
}

/**
 * @brief Set OSD language
 * @param language
 * @return 0 on success
 */
int VideoOSD_SetLanguage(int language)
{
	RegionDevice_p pRegionDevice = &GlobalDevice.RegionDevice;
	if(language == 1) {
		printf("Set the language to Chinese\n");
	}else if(language == 0) {
		printf("Set the language to English\n");
	}else {
		printf("No current language\n");
	}
	pRegionDevice->language = language;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
	for (int i = 0; i < pCaptureDevice->EncCount; i++)
	{
		for (int ichannel = 0; ichannel < pCaptureDevice->EncDevice[i].StreamCount; ichannel++)
		{
			//sample_set_channel_tosd(ichannel);
		}
	}

	return 0;
}

/**
 * @brief Set OSD title font size
 * @param channel Channel number
 * @param size Font size
 * @return 0 on success
 */
int VideoOSD_SetTitleSize(int channel, unsigned int size)
{
	RegionDevice_p pRegionDevice = &GlobalDevice.RegionDevice;
	pRegionDevice->OsdHeight = size;
	return 0;
}

/**
 * @brief Set OSD title for a channel (Internal implementation)
 * @param channel Channel ID (must be 0)
 * @param pParam Title parameters including position, size, and raster data
 * @return 0 on success, FAILED on error
 */
int VideoOSD_SetTitle(int channel, VIDEO_TITLE_PARAM *pParam)
{
    if (!pParam || !pParam->raster) {
        PRINT_ERROR("VideoSetTitle: Invalid parameters\n");
        return FAILED;
    }

    if (channel != 0) {
        PRINT_ERROR("VideoSetTitle: Invalid channel %d\n", channel);
        return FAILED;
    }

    PlatformAdapter* adapter = GetPlatformAdapter();
    if (!adapter) {
        PRINT_ERROR("VideoSetTitle: Platform adapter not initialized\n");
        return FAILED;
    }

    int i = 0;
    int j = 0;
    int ret = -1;
    int type = pParam->index;
    unsigned int fontsize = 0;
    unsigned int EnlargeNumber = 0;
    int ichannel = STREAM_TYPE_FIR;
    unsigned int u32OsdHandle = type;

    POINT_S OSD_point;
    MDK_CHN_S stMdkChn;
    OSD_REGION_S stOsdRegion;
    SRC_OSD_ATTR stSrcOsdAttr;
    OSD_PALETTE_S stOsdPalette;
    OSD_DISP_ATTR_S stOsdDispAttr;
    OSD_BUFFER_INFO_S stOsdBufInfo;
    SIZE_S picsize[STREAM_TYPE_BUT];

    memset(&OSD_point, 0, sizeof(POINT_S));
    memset(&stMdkChn, 0, sizeof(MDK_CHN_S));
    memset(&stOsdRegion, 0, sizeof(OSD_REGION_S));
    memset(&stSrcOsdAttr, 0, sizeof(SRC_OSD_ATTR));
    memset(&stOsdPalette, 0, sizeof(OSD_PALETTE_S));
    memset(&stOsdDispAttr, 0, sizeof(OSD_DISP_ATTR_S));
    memset(&stOsdBufInfo, 0, sizeof(OSD_BUFFER_INFO_S));
    memset(&picsize, 0, sizeof(SIZE_S) * STREAM_TYPE_BUT);

    RegionDevice_p pRegionDevice = &GlobalDevice.RegionDevice;
    CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

    OSD_point.s32X = pParam->x;
    OSD_point.s32Y = pParam->y;

    VideoConfig_LockOsd();

    /* Save title raster data */
    memcpy(pRegionDevice->channeltitle[type], pParam->raster, pParam->width * pParam->height / 8);

    /* Process OSD for each stream (main and sub) */
    for (; ichannel < pCaptureDevice->EncDevice[channel].StreamCount; ichannel++) {
        if (STREAM_TYPE_THI == ichannel) {
            break;  /* Skip JPEG channel */
        }

        picsize[ichannel].u32Width = pCaptureDevice->EncDevice[i].StreamDevice[ichannel].EncChannel_info.max_width;
        picsize[ichannel].u32Height = pCaptureDevice->EncDevice[i].StreamDevice[ichannel].EncChannel_info.max_height;

        /* Calculate font size based on picture resolution */
        if (STREAM_TYPE_FIR == ichannel) {
            if (0 == pRegionDevice->OsdHeight) {
                for (j = 0; j < (int)sizeof(VideoConfig_GetFontTableSize()) / (int)sizeof(int); j++) {
                    if ((VideoConfig_GetFontSize(j) * 32 / 2) < picsize[STREAM_TYPE_FIR].u32Width * 0.8) {
                        fontsize = VideoConfig_GetFontSize(j);
                        break;
                    }
                }
            } else {
                fontsize = pRegionDevice->OsdHeight;
            }
        } else {
            u32OsdHandle += 4;
            fontsize = 16;
        }

        EnlargeNumber = fontsize / 16;
        stMdkChn.enModId = NI_ID_VPS;
        stMdkChn.s32DevId = 0;
        stMdkChn.s32ChnId = ichannel;

        /* Get current OSD display attributes via adapter */
        ret = adapter->osd_get_disp_attr(u32OsdHandle, &stMdkChn, &stOsdDispAttr);
        if (ret != RETURN_OK) {
            PRINT_ERROR("osd_get_disp_attr failed with %x\n", ret);
        }

        /* Configure OSD region */
        stOsdRegion.bMacroFormat = NI_FALSE;
        stOsdRegion.enOsdRgn = OSD_RGN;
        stOsdRegion.enPixelFormat = OSD_PIXEL_FORMAT_2BIT;

        stSrcOsdAttr.stOsdRegion = stOsdRegion;
        stSrcOsdAttr.stSize.u32Width = pParam->width;
        stSrcOsdAttr.stSize.u32Height = pParam->height;
        stSrcOsdAttr.u8FgIdx = 1;
        stSrcOsdAttr.u8BgIdx = 0;
        stSrcOsdAttr.enFormat = OSD_FORMAT_BIT;

        /* Configure display attributes */
        stOsdDispAttr.bShow = pParam->enable;
        if (0 == pParam->width || 0 == pParam->height) {
            stOsdDispAttr.bShow = FALSE;
        }
        stOsdDispAttr.stSize.u32Width = pParam->width * EnlargeNumber;
        stOsdDispAttr.stSize.u32Height = fontsize;

        if (0 == stOsdDispAttr.stSize.u32Width) {
            stOsdDispAttr.stSize.u32Width = 16;
        }
        if (0 == stOsdDispAttr.stSize.u32Height) {
            stOsdDispAttr.stSize.u32Height = 16;
        }

        /* Set layer based on OSD type */
        type = u32OsdHandle % 4;
        switch (type) {
            case OSD_TYPE_TIME:
            case OSD_TYPE_TITLE:
                stOsdDispAttr.u32Layer = 0;
                break;
            case OSD_TYPE_LABLE1:
            case OSD_TYPE_LABLE2:
                stOsdDispAttr.u32Layer = 1;
                break;
            default:
                stOsdDispAttr.u32Layer = 0;
                break;
        }

        stOsdDispAttr.enAlphaMode = OSD_ALPHA_MODE_PALETTE;
        stOsdDispAttr.u32RegionAlpha = 31;
        stOsdDispAttr.bInvert = pParam->AutoBlackWhite;
        stOsdDispAttr.bSpecialFrmId = NI_FALSE;
        stOsdDispAttr.u32TargetFrameId = 0;
        /* Disable invert due to poor visual effect */
        stOsdDispAttr.bInvert = 0;

        /* Calculate OSD coordinates with overlap resolution */
        ret = _osd_coord_calculate(ichannel, u32OsdHandle, EnlargeNumber,
                                     stOsdDispAttr.stSize, OSD_point, &stOsdDispAttr.stStartPiont);
        if (ret != RETURN_OK) {
            PRINT_ERROR("_osd_coord_calculate failed\n");
			VideoConfig_UnlockOsd();
            return ret;
        }

        /* Calculate buffer size */
        stOsdBufInfo.u32Len = stOsdDispAttr.stSize.u32Width * stOsdDispAttr.stSize.u32Height / 2;
        if (stOsdBufInfo.u32Len < 256) {
            stOsdBufInfo.u32Len = 256;
        }

        /* Pre-allocate 32KB for TIME OSD to avoid memory shortage */
        if (OSD_TYPE_TIME == u32OsdHandle) {
            stOsdBufInfo.u32Len = 32 * 1024;
        }

        /* Configure palette */
        SampleOsdPaletteInit(&stOsdPalette);
        stOsdPalette.astPalette[1].u8Alpha = (int)((float)((pParam->fg_color) >> 24) / 255 * 31);
        stOsdPalette.astPalette[2].u8Alpha = (int)((float)((pParam->fg_color) >> 24) / 255 * 31);
        stOsdPalette.astPalette[3].u8Alpha = (int)((float)((pParam->fg_color) >> 24) / 255 * 31);

        /* Get OSD buffer via adapter */
        ret = adapter->osd_get_buffer(u32OsdHandle, &stOsdBufInfo);
        if (ret != RETURN_OK) {
            PRINT_ERROR("osd_get_buffer failed with %x\n", ret);
			VideoConfig_UnlockOsd();
            return ret;
        }

        /* Clear buffer and process bitmap */
        memset((NI_U8 *)stOsdBufInfo.pu32VirtAddr, 0, stOsdBufInfo.u32Len);

        ret = _vsf_drv_enlarge_osd((NI_U8 *)pRegionDevice->channeltitle[type],
                                     (NI_U8 *)stOsdBufInfo.pu32VirtAddr,
                                     stOsdRegion.enPixelFormat, EnlargeNumber, &stSrcOsdAttr.stSize);
        if (ret != RETURN_OK) {
            PRINT_ERROR("_vsf_drv_enlarge_osd failed with %x\n", ret);
			VideoConfig_UnlockOsd();
            return ret;
        }

        OSD_TEST_ConvertToLittleEndian2((NI_U8 *)stOsdBufInfo.pu32VirtAddr,
                                         stOsdRegion.enPixelFormat, stOsdBufInfo.u32Len);

        /* Set palette via adapter */
        ret = adapter->osd_set_palette(&stMdkChn, &stOsdPalette);
        if (ret != RETURN_OK) {
            PRINT_ERROR("osd_set_palette failed with %x\n", ret);
			VideoConfig_UnlockOsd();
            return ret;
        }

        /* Paint OSD to channel via adapter */
        stOsdDispAttr.enInvertBlkMode = OSD_INVERT_BLK_NULL;
        ret = adapter->osd_paint_to_chn(u32OsdHandle, &stMdkChn, &stOsdDispAttr, 500);
        if (ret != RETURN_OK) {
            PRINT_ERROR("u32OsdHandle: %d osd_paint_to_chn failed with %x\n", u32OsdHandle, ret);
            _osd_error(&stMdkChn, &stOsdDispAttr);
			VideoConfig_UnlockOsd();
            return ret;
        }

        /* Refresh OSD via adapter */
        ret = adapter->osd_refresh(u32OsdHandle, 500);
        if (ret != RETURN_OK) {
            PRINT_ERROR("osd_refresh failed with %x\n", ret);
			VideoConfig_UnlockOsd();
            return ret;
        }
    }

    usleep(50 * 1000);
	VideoConfig_UnlockOsd();

    /* Save OSD parameters */
    pRegionDevice->OsdWidth[type] = pParam->width;
    pRegionDevice->FgColor = pParam->fg_color;
    pRegionDevice->BgColor = pParam->bg_color;
    pRegionDevice->PositionX[type] = pParam->x;
    pRegionDevice->PositionY[type] = pParam->y;
    pRegionDevice->show[type] = pParam->enable;
    pRegionDevice->InvertEnable = pParam->AutoBlackWhite;

    return 0;
}


/**
 * @brief Set OSD logo for a channel (Fixed version with proper error handling)
 * @param channel Channel ID (must be 0)
 * @param pParam Logo parameters including position, enable, and color
 * @return 0 on success, FAILED on error
 */
int VideoOSD_SetLogo(int channel, VIDEO_TITLE_PARAM *pParam)
{
    if (!pParam) {
        PRINT_ERROR("VideoSetLogo: pParam is NULL\n");
        return FAILED;
    }

    int i = 0;
    int ret = -1;
    unsigned char *logo = NULL;
    unsigned int u32OsdHandle = 9;
    int ichannel = STREAM_TYPE_FIR;

    POINT_S OSD_point;
    MDK_CHN_S stMdkChn;
    OSD_REGION_S stOsdRegion;
    OSD_PALETTE_S stOsdPalette;
    OSD_DISP_ATTR_S stOsdDispAttr;
    OSD_BUFFER_INFO_S stOsdBufInfo;
    SIZE_S picsize[STREAM_TYPE_BUT];

    memset(&OSD_point, 0, sizeof(POINT_S));
    memset(&stMdkChn, 0, sizeof(MDK_CHN_S));
    memset(&stOsdRegion, 0, sizeof(OSD_REGION_S));
    memset(&stOsdPalette, 0, sizeof(OSD_PALETTE_S));
    memset(&stOsdDispAttr, 0, sizeof(OSD_DISP_ATTR_S));
    memset(&stOsdBufInfo, 0, sizeof(OSD_BUFFER_INFO_S));
    memset(&picsize, 0, sizeof(SIZE_S) * STREAM_TYPE_BUT);

    int vstd = VIDEO_STANDARD_PAL;
	unsigned int cif_width = VideoConfig_GetImageSize(vstd, VIDEO_SIZE_CIF)->u32Width;
	unsigned int cif_height = VideoConfig_GetImageSize(vstd, VIDEO_SIZE_CIF)->u32Height;

    RegionDevice_p pRegionDevice = &GlobalDevice.RegionDevice;
    CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
    PlatformAdapter* adapter = GetPlatformAdapter();

    if (!adapter) {
        PRINT_ERROR("VideoSetLogo: Failed to get platform adapter\n");
        return FAILED;
    }

    // 添加调试信息
    PRINT_INFO("VideoOSD_SetLogo: Start - channel=%d, enable=%d, LogoInit=%d\n", 
               channel, pParam->enable, pRegionDevice->LogoInit);

    if (FALSE == pRegionDevice->LogoInit) {
        PRINT_INFO("Creating Logo OSD regions...\n");
        int create_ret = VideoOSD_CreateLogo();
        PRINT_INFO("VideoOSD_CreateLogo returned: %d\n", create_ret);
        if (create_ret != SUCCESS) {
            // 创建失败时仍然继续，像重构前一样
            PRINT_WARN("Logo OSD creation failed, but continuing like original code\n");
        }
        pRegionDevice->LogoInit = TRUE;
    }
	
    VideoConfig_LockOsd();

    for (; ichannel < pCaptureDevice->EncDevice[channel].StreamCount; ichannel++)
    {
        if (STREAM_TYPE_THI == ichannel) {
            break;
        }

        PRINT_INFO("Processing logo for channel: %d, OSD handle: %d\n", ichannel, u32OsdHandle);

        picsize[ichannel].u32Width = pCaptureDevice->EncDevice[i].StreamDevice[ichannel].EncChannel_info.max_width;
        picsize[ichannel].u32Height = pCaptureDevice->EncDevice[i].StreamDevice[ichannel].EncChannel_info.max_height;

        stMdkChn.enModId = NI_ID_VPS;
        stMdkChn.s32DevId = 0;
        stMdkChn.s32ChnId = ichannel;

        ret = adapter->osd_get_disp_attr(u32OsdHandle, &stMdkChn, &stOsdDispAttr);
        if (ret != RETURN_OK) {
            PRINT_WARN("osd_get_disp_attr failed with 0x%x for handle %d, using default attributes\n", 
                      ret, u32OsdHandle);
            
            memset(&stOsdDispAttr, 0, sizeof(OSD_DISP_ATTR_S));
            
            // 设置合理的默认值
            stOsdDispAttr.bShow = FALSE;  // 默认不显示，后面会根据pParam->enable设置
            stOsdDispAttr.u32Layer = 1;
            stOsdDispAttr.enAlphaMode = OSD_ALPHA_MODE_PALETTE;
            stOsdDispAttr.u32RegionAlpha = 31;
            stOsdDispAttr.bInvert = 0;
            stOsdDispAttr.bSpecialFrmId = NI_FALSE;
            stOsdDispAttr.u32TargetFrameId = 0;
            stOsdDispAttr.enInvertBlkMode = OSD_INVERT_BLK_NULL;
            
            // 设置默认尺寸
            if (STREAM_TYPE_FIR == ichannel) {
                stOsdDispAttr.stSize.u32Width = MAIN_LOGO_WIDTH;
                stOsdDispAttr.stSize.u32Height = MAIN_LOGO_HEIGHT;
            } else if (STREAM_TYPE_SEC == ichannel) {
                stOsdDispAttr.stSize.u32Width = SUB_LOGO_WIDTH;
                stOsdDispAttr.stSize.u32Height = SUB_LOGO_HEIGHT;
            }
            
            // 设置默认位置
            stOsdDispAttr.stStartPiont.s32X = (int)((float)pParam->x * picsize[ichannel].u32Width / cif_width);
            stOsdDispAttr.stStartPiont.s32Y = (int)((float)pParam->y * picsize[ichannel].u32Height / cif_height);
        }

        stOsdRegion.bMacroFormat = NI_FALSE;
        stOsdRegion.enOsdRgn = OSD_RGN;
        stOsdRegion.enPixelFormat = OSD_PIXEL_FORMAT_4BIT;

        if (STREAM_TYPE_FIR == ichannel) {
            stOsdDispAttr.stSize.u32Width = MAIN_LOGO_WIDTH;
            stOsdDispAttr.stSize.u32Height = MAIN_LOGO_HEIGHT;
            logo = main_logo;
        } else if (STREAM_TYPE_SEC == ichannel) {
            stOsdDispAttr.stSize.u32Width = SUB_LOGO_WIDTH;
            stOsdDispAttr.stSize.u32Height = SUB_LOGO_HEIGHT;
            logo = sub_logo;
        }

        // 设置显示状态（根据参数）
        stOsdDispAttr.bShow = pParam->enable;
        stOsdDispAttr.u32Layer = 1;
        stOsdDispAttr.enAlphaMode = OSD_ALPHA_MODE_PALETTE;
        stOsdDispAttr.u32RegionAlpha = 31;
        stOsdDispAttr.bInvert = pParam->AutoBlackWhite;
        stOsdDispAttr.bSpecialFrmId = NI_FALSE;
        stOsdDispAttr.u32TargetFrameId = 0;
        // 由于反色效果不好，关闭反色
        stOsdDispAttr.bInvert = 0;

        // 计算位置（如果之前没有设置默认位置）
        stOsdDispAttr.stStartPiont.s32X = (int)((float)pParam->x * picsize[ichannel].u32Width / cif_width);
        stOsdDispAttr.stStartPiont.s32Y = (int)((float)pParam->y * picsize[ichannel].u32Height / cif_height);

        // 边界检查和对齐
        if (stOsdDispAttr.stStartPiont.s32X + stOsdDispAttr.stSize.u32Width > picsize[ichannel].u32Width) {
            stOsdDispAttr.stStartPiont.s32X = picsize[ichannel].u32Width - stOsdDispAttr.stSize.u32Width;
        }
        if (stOsdDispAttr.stStartPiont.s32Y + stOsdDispAttr.stSize.u32Height > picsize[ichannel].u32Height) {
            stOsdDispAttr.stStartPiont.s32Y = picsize[ichannel].u32Height - stOsdDispAttr.stSize.u32Height;
        }

        if (stOsdDispAttr.stStartPiont.s32X % 2) {
            stOsdDispAttr.stStartPiont.s32X--;
        }
        if (stOsdDispAttr.stStartPiont.s32Y % 2) {
            stOsdDispAttr.stStartPiont.s32Y--;
        }

        stOsdBufInfo.u32Len = stOsdDispAttr.stSize.u32Width * stOsdDispAttr.stSize.u32Height / 2;
        if (stOsdBufInfo.u32Len < 256) {
            stOsdBufInfo.u32Len = 256;
        }

        SampleOsdPaletteInit(&stOsdPalette);
        stOsdPalette.astPalette[1].u8Alpha = (int)((float)((pParam->fg_color) >> 24) / 255 * 31);
        stOsdPalette.astPalette[2].u8Alpha = (int)((float)((pParam->fg_color) >> 24) / 255 * 31);
        stOsdPalette.astPalette[3].u8Alpha = (int)((float)((pParam->fg_color) >> 24) / 255 * 31);

        PRINT_INFO("Getting buffer for handle %d, size=%d\n", u32OsdHandle, stOsdBufInfo.u32Len);
        ret = adapter->osd_get_buffer(u32OsdHandle, &stOsdBufInfo);
        if (ret != RETURN_OK) {
            PRINT_ERROR("osd_get_buffer failed with 0x%x for handle %d\n", ret, u32OsdHandle);
            // 继续处理下一个通道，不立即返回
            continue;
        }

        PRINT_INFO("Converting logo bitmap...\n");
        ret = _osd_bit_switch(logo, (NI_U8 *)stOsdBufInfo.pu32VirtAddr, stOsdRegion.enPixelFormat, &stOsdDispAttr.stSize);
        if (ret != RETURN_OK) {
            PRINT_ERROR("_osd_bit_switch failed with 0x%x for handle %d\n", ret, u32OsdHandle);
            // 继续处理下一个通道，不立即返回
            continue;
        }

        OSD_TEST_ConvertToLittleEndian2((NI_U8 *)stOsdBufInfo.pu32VirtAddr, stOsdRegion.enPixelFormat, stOsdBufInfo.u32Len);

        PRINT_INFO("Setting palette...\n");
        ret = adapter->osd_set_palette(&stMdkChn, &stOsdPalette);
        if (ret != RETURN_OK) {
            PRINT_ERROR("osd_set_palette failed with 0x%x for handle %d\n", ret, u32OsdHandle);
            // 继续处理下一个通道，不立即返回
            continue;
        }

        // OSD单元设置非单元反色模式
        stOsdDispAttr.enInvertBlkMode = OSD_INVERT_BLK_NULL;
        PRINT_INFO("Painting to channel, show=%d...\n", stOsdDispAttr.bShow);
        ret = adapter->osd_paint_to_chn(u32OsdHandle, &stMdkChn, &stOsdDispAttr, 500);
        if (ret != RETURN_OK) {
            PRINT_ERROR("u32OsdHandle: %d osd_paint_to_chn failed with 0x%x\n", u32OsdHandle, ret);
            _osd_error(&stMdkChn, &stOsdDispAttr);
            // 继续处理下一个通道，不立即返回
            continue;
        }

        PRINT_INFO("Refreshing OSD...\n");
        ret = adapter->osd_refresh(u32OsdHandle, 500);
        if (ret != RETURN_OK) {
            PRINT_ERROR("osd_refresh failed with 0x%x for handle %d\n", ret, u32OsdHandle);
            // 继续处理下一个通道，不立即返回
            continue;
        }
        
        PRINT_INFO("Logo setup completed for handle %d\n", u32OsdHandle);
        u32OsdHandle++;
    }

    usleep(50 * 1000);

    VideoConfig_UnlockOsd();
    
    // 保存参数
    pRegionDevice->LogoEnable = pParam->enable;
    pRegionDevice->LogoX = pParam->x;
    pRegionDevice->LogoY = pParam->y;

    PRINT_INFO("VideoOSD_SetLogo: Completed successfully\n");
    
    // 总是返回SUCCESS，模仿重构前的行为
    return SUCCESS;
}




int VideoOSD_Init(void)
{
    PlatformAdapter* adapter = GetPlatformAdapter();
    if (!adapter || !adapter->osd_create) {
        PRINT_ERROR("OSD adapter not available\n");
        return -1;
    }

    PRINT_INFO("===== Starting VideoOverlay_Init =====\n");

    /* OSD configuration */
    MDK_CHN_S stMdkChn;
    SAMPLE_OSD_CONFIG_S stOsdCfg;
    NI_U32 u32OsdHandle = 0;

    stMdkChn.enModId = NI_ID_VPS;
    stMdkChn.s32DevId = 0;
    stMdkChn.s32ChnId = 0;  /* Attach to first VPS channel */

    stOsdCfg.stStartPiont.s32X = 0;
    stOsdCfg.stStartPiont.s32Y = 0;
    stOsdCfg.u32Layer = 0;
    stOsdCfg.bSpecialFrmId = NI_FALSE;
    stOsdCfg.u32TargetFrameId = 0;

    /* Create OSD via adapter */
    if (adapter->osd_create(u32OsdHandle, &stMdkChn, &stOsdCfg) != 0) {
        PRINT_WARN("OSD create failed (non-fatal)\n");
        return -1;  /* OSD failure is non-fatal */
    }

    PRINT_INFO("===== VideoOverlay_Init SUCCESS =====\n");
    return 0;
}

