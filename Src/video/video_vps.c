/*
 * video_vps.c -- VPS (Video Processing System) 缩放/处理模块实现
 *
 * 本模块负责 VPS 的所有操作，包括通道管理、输入/输出帧率调整等。
 * 所有操作通过 platform_adapter 抽象层调用 SDK，不直接使用 NI_MDK_* 函数。
 *
 * Phase 3: VPS 模块提取
 *
 * Created as part of video pipeline modularization refactoring.
 */

/* ========================================================================== */
/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "video_vps.h"
#include "../../Include/APIs/TdCommon.h"
#include "../../Include/APIs/Video.h"
#include "../../Include/APIs/Capture.h"
#include "video_osd.h"
#include "video_config.h"
#include "platform_adapter.h"
#include "PrintGrade.h"

/**
 * @brief Internal helper: Set a single mask region for all applicable VPS channels
 *
 * @param channel Video channel index
 * @param index Mask region index
 * @param mask_x X position of mask (scaled coordinates)
 * @param mask_y Y position of mask (scaled coordinates)
 * @param mask_width Width of mask (scaled)
 * @param mask_height Height of mask (scaled)
 * @param enable Enable/disable flag (ENABLE or DISABLE)
 * @return 0 on success, negative on error
 */
static int _vps_set_single_cover(
    int channel,
    int index,
    int mask_x,
    int mask_y,
    int mask_width,
    int mask_height,
    int enable
)
{
    int ret = -1;
    VPS_GRP VpsGrp = 0;
    VPS_MASK_ATTR_S pstMaskAttr;
    CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
    PlatformAdapter* adapter = GetPlatformAdapter();

    if (!adapter) {
        PRINT_ERROR("_vps_set_single_cover: Platform adapter not initialized\n");
        return FAILED;
    }

    memset(&pstMaskAttr, 0, sizeof(VPS_MASK_ATTR_S));

    /* Process main and sub streams (skip JPEG channel) */
    for (int ichannel = STREAM_TYPE_FIR; ichannel < pCaptureDevice->EncDevice[channel].StreamCount; ichannel++)
    {
        if (STREAM_TYPE_THI == ichannel) {
            break;  /* Skip JPEG channel */
        }

        /* Get current mask attributes via adapter */
        ret = adapter->vps_get_chn_mask_attr(VpsGrp, ichannel, &pstMaskAttr);
        if (ret != RETURN_OK) {
            PRINT_ERROR("vps_get_chn_mask_attr failed with %x\n", ret);
            return ret;
        }

        /* Set mask color (YUV black: Y=0, U=128, V=128) */
        pstMaskAttr.stMaskColor.u8Y = 0;
        pstMaskAttr.stMaskColor.u8U = 128;
        pstMaskAttr.stMaskColor.u8V = 128;

        /* Set mask rectangle coordinates (already scaled by caller) */
        pstMaskAttr.astMaskRect[index].s32X = mask_x;
        pstMaskAttr.astMaskRect[index].s32Y = mask_y;
        pstMaskAttr.astMaskRect[index].u32Width = mask_width;
        pstMaskAttr.astMaskRect[index].u32Height = mask_height;

        /* Apply YUV 4:2:0 alignment (coordinates must be even) */
        if (pstMaskAttr.astMaskRect[index].s32X % 2) {
            pstMaskAttr.astMaskRect[index].s32X--;
        }
        if (pstMaskAttr.astMaskRect[index].s32Y % 2) {
            pstMaskAttr.astMaskRect[index].s32Y--;
        }
        if (pstMaskAttr.astMaskRect[index].u32Width % 2) {
            pstMaskAttr.astMaskRect[index].u32Width--;
        }
        if (pstMaskAttr.astMaskRect[index].u32Height % 2) {
            pstMaskAttr.astMaskRect[index].u32Height--;
        }

        /* Ensure minimum dimensions (2x2 pixels) */
        if (0 == pstMaskAttr.astMaskRect[index].u32Width) {
            pstMaskAttr.astMaskRect[index].u32Width = 2;
        }
        if (0 == pstMaskAttr.astMaskRect[index].u32Height) {
            pstMaskAttr.astMaskRect[index].u32Height = 2;
        }

        /* Set mask attributes via adapter */
        ret = adapter->vps_set_chn_mask_attr(VpsGrp, ichannel, &pstMaskAttr);
        if (ret != RETURN_OK) {
            PRINT_ERROR("vps_set_chn_mask_attr failed with %x\n", ret);
            return ret;
        }

        /* Enable or disable mask via adapter */
        if (ENABLE == enable) {
            ret = adapter->vps_enable_chn_mask(VpsGrp, ichannel, index);
            if (ret != RETURN_OK) {
                PRINT_ERROR("vps_enable_chn_mask failed with %x\n", ret);
                return ret;
            }
        }
        else if (DISABLE == enable) {
            ret = adapter->vps_disable_chn_mask(VpsGrp, ichannel, index);
            if (ret != RETURN_OK) {
                PRINT_ERROR("vps_disable_chn_mask failed with %x\n", ret);
                return ret;
            }
        }
        else {
            PRINT_ERROR("Invalid enable flag: %d (must be ENABLE or DISABLE)\n", enable);
            return -1;
        }
    }

    return 0;
}

/**
 * @brief Apply all saved cover regions to VPS channels
 *
 * This function reapplies all saved cover/mask configurations from GlobalDevice.
 * It's typically called during initialization or after resolution changes.
 *
 * Batch operation that processes all mask regions (0 to VPS_MASK_MAX_CHN_NUM-1)
 * for main and sub streams.
 *
 * @return 0 on success, negative on error
 */
int VideoVPS_SetCoverRegion()
{
    int i = 0;
    int j = 0;
    int ret = -1;
    int channel = 0;
    VPS_GRP VpsGrp = 0;
    unsigned int x = 0;
    unsigned int y = 0;
    unsigned int w = 0;
    unsigned int h = 0;
    VPS_MASK_ATTR_S pstMaskAttr;
    SIZE_S picsize[STREAM_TYPE_BUT];
    RegionDevice_p pRegionDevice = &GlobalDevice.RegionDevice;
    CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
    PlatformAdapter* adapter = GetPlatformAdapter();

    if (!adapter) {
        PRINT_ERROR("VideoVPS_SetCoverRegion: Platform adapter not initialized\n");
        return FAILED;
    }

    memset(&pstMaskAttr, 0, sizeof(VPS_MASK_ATTR_S));
    memset(&picsize, 0, sizeof(SIZE_S) * STREAM_TYPE_BUT);

    /* CIF reference coordinate system */
    int vstd = VIDEO_STANDARD_PAL;
	unsigned int cif_width = VideoConfig_GetImageSize(vstd, VIDEO_SIZE_CIF)->u32Width;
	unsigned int cif_height = VideoConfig_GetImageSize(vstd, VIDEO_SIZE_CIF)->u32Height;

    /* Get actual stream resolutions for scaling */
    for (i = 0; i < pCaptureDevice->EncCount; i++)
    {
        for (j = 0; j < pCaptureDevice->EncDevice[i].StreamCount; j++)
        {
            picsize[j].u32Width = pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.max_width;
            picsize[j].u32Height = pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.max_height;
        }
    }

    /* Process all VPS channels (main/sub, skip JPEG) */
    for (int ichannel = STREAM_TYPE_FIR; ichannel < pCaptureDevice->EncDevice[channel].StreamCount; ichannel++)
    {
        if (STREAM_TYPE_THI == ichannel) {
            break;  /* Skip JPEG channel */
        }

        /* Get current mask attributes via adapter */
        ret = adapter->vps_get_chn_mask_attr(VpsGrp, ichannel, &pstMaskAttr);
        if (ret != RETURN_OK) {
            PRINT_ERROR("vps_get_chn_mask_attr failed with %x\n", ret);
            return ret;
        }

        /* Process all mask regions (batch apply) */
        for (i = 0; i < VPS_MASK_MAX_CHN_NUM; i++)
        {
            /* Extract coordinates from saved configuration */
            x = pRegionDevice->COVER_PARAM[i].rect.left;
            y = pRegionDevice->COVER_PARAM[i].rect.top;
            w = pRegionDevice->COVER_PARAM[i].rect.right - pRegionDevice->COVER_PARAM[i].rect.left;
            h = pRegionDevice->COVER_PARAM[i].rect.bottom - pRegionDevice->COVER_PARAM[i].rect.top;

            /* Set mask color (YUV black) */
            pstMaskAttr.stMaskColor.u8Y = 0;
            pstMaskAttr.stMaskColor.u8U = 128;
            pstMaskAttr.stMaskColor.u8V = 128;

            /* Scale coordinates from CIF to actual channel resolution */
            pstMaskAttr.astMaskRect[i].s32X = (int)((float)x * picsize[ichannel].u32Width / cif_width);
            pstMaskAttr.astMaskRect[i].s32Y = (int)((float)y * picsize[ichannel].u32Height / cif_height);
            pstMaskAttr.astMaskRect[i].u32Width = (int)((float)w * picsize[ichannel].u32Width / cif_width);
            pstMaskAttr.astMaskRect[i].u32Height = (int)((float)h * picsize[ichannel].u32Height / cif_height);

            /* Apply YUV 4:2:0 alignment (coordinates must be even) */
            if (pstMaskAttr.astMaskRect[i].s32X % 2) {
                pstMaskAttr.astMaskRect[i].s32X--;
            }
            if (pstMaskAttr.astMaskRect[i].s32Y % 2) {
                pstMaskAttr.astMaskRect[i].s32Y--;
            }
            if (pstMaskAttr.astMaskRect[i].u32Width % 2) {
                pstMaskAttr.astMaskRect[i].u32Width--;
            }
            if (pstMaskAttr.astMaskRect[i].u32Height % 2) {
                pstMaskAttr.astMaskRect[i].u32Height--;
            }

            /* Ensure minimum dimensions */
            if (0 == pstMaskAttr.astMaskRect[i].u32Width) {
                pstMaskAttr.astMaskRect[i].u32Width = 2;
            }
            if (0 == pstMaskAttr.astMaskRect[i].u32Height) {
                pstMaskAttr.astMaskRect[i].u32Height = 2;
            }
        }

        /* Apply all mask attributes in one batch via adapter */
        ret = adapter->vps_set_chn_mask_attr(VpsGrp, ichannel, &pstMaskAttr);
        if (ret != RETURN_OK) {
            PRINT_ERROR("vps_set_chn_mask_attr failed with %x\n", ret);
            return ret;
        }
    }

    return ret;
}

/**
 * @brief Set a single cover/mask region on video channel
 *
 * This function sets privacy mask (cover) for a specific region. The input
 * coordinates are in CIF reference system (352x288) and will be automatically
 * scaled to actual stream resolutions (main/sub).
 *
 * @param channel Video channel index
 * @param index Mask region index (0 to VPS_MASK_MAX_CHN_NUM-1)
 * @param pParam Cover parameters (coordinates, color, enable)
 * @return 0 on success, negative on error
 */
int VideoVPS_SetCover(int channel, int index, VIDEO_COVER_PARAM *pParam)
{
    int ret = -1;
    unsigned int x = 0;
    unsigned int y = 0;
    unsigned int w = 0;
    unsigned int h = 0;
    SIZE_S picsize[STREAM_TYPE_BUT];
    RegionDevice_p pRegionDevice = &GlobalDevice.RegionDevice;
    CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

    /* CIF reference coordinate system (352x288 for PAL) */
    int vstd = VIDEO_STANDARD_PAL;
	unsigned int cif_width = VideoConfig_GetImageSize(vstd, VIDEO_SIZE_CIF)->u32Width;
	unsigned int cif_height = VideoConfig_GetImageSize(vstd, VIDEO_SIZE_CIF)->u32Height;

    if (!pParam) {
        PRINT_ERROR("VideoVPS_SetCover: pParam is NULL\n");
        return FAILED;
    }

    memset(&picsize, 0, sizeof(SIZE_S) * STREAM_TYPE_BUT);

    /* Get actual stream resolution for coordinate scaling */
    for (int i = 0; i < pCaptureDevice->EncCount; i++)
    {
        for (int j = 0; j < pCaptureDevice->EncDevice[i].StreamCount; j++)
        {
            picsize[j].u32Width = pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.max_width;
            picsize[j].u32Height = pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.max_height;
        }
    }

    /* Extract rectangle coordinates from parameter */
    x = pParam->rect.left;
    y = pParam->rect.top;
    w = pParam->rect.right - pParam->rect.left;
    h = pParam->rect.bottom - pParam->rect.top;

    /* Scale coordinates from CIF reference to actual main stream resolution */
    unsigned int mask_x = (unsigned int)((float)x * picsize[STREAM_TYPE_FIR].u32Width / cif_width);
    unsigned int mask_y = (unsigned int)((float)y * picsize[STREAM_TYPE_FIR].u32Height / cif_height);
    unsigned int mask_width = (unsigned int)((float)w * picsize[STREAM_TYPE_FIR].u32Width / cif_width);
    unsigned int mask_height = (unsigned int)((float)h * picsize[STREAM_TYPE_FIR].u32Height / cif_height);

    /* Apply mask via internal helper (handles all channels and adapter calls) */
    ret = _vps_set_single_cover(channel, index,
                                (int)mask_x, (int)mask_y,
                                (int)mask_width, (int)mask_height,
                                pParam->enable);
    if (ret != RETURN_OK) {
        PRINT_ERROR("VideoVPS_SetCover: _vps_set_single_cover failed with %x\n", ret);
        return ret;
    }

    /* Save configuration to global device for later reference */
    pRegionDevice->COVER_PARAM[index].rect = pParam->rect;
    pRegionDevice->COVER_PARAM[index].color = pParam->color;
    pRegionDevice->COVER_PARAM[index].enable = pParam->enable;

    return 0;
}

/**
 * @brief Set VPS input frame rate
 *
 * This function sets the VPS input frame rate with special handling for 12fps.
 * For 12fps, it uses FpsDenom=2 and InFps=25 to achieve 12.5fps ≈ 12fps integer rate.
 *
 * @param channel Video channel index
 * @param FrmRate Target frame rate (e.g., 12, 25, 30)
 * @return 0 on success, FAILED on error
 */
int VideoVPS_SetInputFrameRate(int channel, unsigned int FrmRate)
{
    int ret = FAILED;
    VPS_GRP VpsGrp = 0;
    VPS_GRP_ATTR_S stGrpAttr;
    VPS_MOD_PARAM_S stGrpModParam;
    PlatformAdapter* adapter = GetPlatformAdapter();

    if (!adapter) {
        PRINT_ERROR("VideoVPS_SetInputFrameRate: Platform adapter not initialized\n");
        return FAILED;
    }

    memset(&stGrpAttr, 0, sizeof(VPS_GRP_ATTR_S));
    memset(&stGrpModParam, 0, sizeof(VPS_MOD_PARAM_S));

    if (0 == FrmRate) {
        PRINT_ERROR("VideoVPS_SetInputFrameRate: FrmRate %d invalid\n", FrmRate);
        return ret;
    }

    /* Step 1: Get VPS module parameters via adapter */
    ret = adapter->vps_get_mod_param(VpsGrp, &stGrpModParam);
    if (RETURN_OK != ret) {
        PRINT_ERROR("vps_get_mod_param failed with %#x\n", ret);
        return ret;
    }

    /* Step 2: Set timestamp denominator based on frame rate */
    if (12 == FrmRate) {
        /* For PAL (25fps sensor), reduce to 12fps by setting denominator=2
         * This makes output fps = 25/2 = 12.5 ≈ 12fps (integer) */
        stGrpModParam.u32FpsDenom = 2;
    }
    else {
        /* Normal frame rates use denominator=1 */
        stGrpModParam.u32FpsDenom = 1;
    }

    /* Step 3: Apply module parameters via adapter */
    ret = adapter->vps_set_mod_param(VpsGrp, &stGrpModParam);
    if (RETURN_OK != ret) {
        PRINT_ERROR("vps_set_mod_param failed with %#x\n", ret);
        return ret;
    }

    /* Step 4: Get VPS group attributes via adapter */
    ret = adapter->vps_get_grp_attr(VpsGrp, &stGrpAttr);
    if (RETURN_OK != ret) {
        PRINT_ERROR("vps_get_grp_attr failed with %#x\n", ret);
        return ret;
    }

    /* Step 5: Set input frame rate */
    if (12 == FrmRate) {
        /* For 12fps, keep sensor at 25fps (will be divided by denominator=2) */
        stGrpAttr.u32InFps = 25;
    }
    else {
        /* Normal frame rates: set directly */
        stGrpAttr.u32InFps = FrmRate;
    }

    /* Step 6: Apply group attributes via adapter */
    ret = adapter->vps_set_grp_attr(VpsGrp, &stGrpAttr);
    if (RETURN_OK != ret) {
        PRINT_ERROR("vps_set_grp_attr failed with %#x\n", ret);
        return ret;
    }

    return ret;
}

/**
 * @brief Set VPS output parameters (resolution, frame rate, encoding type)
 *
 * This function sets VPS channel output parameters. When resolution changes,
 * it automatically destroys and recreates OSD/Logo regions.
 *
 * @param channel Video channel index
 * @param dwType Stream type (CHL_MAIN_T, CHL_2END_T)
 * @param ChnAttr Output attributes (width, height, fps, payload type)
 * @return 0 on success, FAILED on error
 */
int VideoVPS_SetOutputParam(int channel, DWORD dwType, VPS_CHN_OUT_ATTR *ChnAttr)
{
    int EncCnt = 0;
    int ret = FAILED;
    VPS_GRP VpsGrp = 0;
    unsigned int VencChn = 0;
    VPS_CHN_ATTR_S VPS_ChnAttr;
    CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
    PlatformAdapter* adapter = GetPlatformAdapter();
    extern unsigned int sensor_fps;

    if (!adapter) {
        PRINT_ERROR("VideoVPS_SetOutputParam: Platform adapter not initialized\n");
        return FAILED;
    }

    memset(&VPS_ChnAttr, 0, sizeof(VPS_CHN_ATTR_S));

    /* Video input channel count */
    EncCnt = pCaptureDevice->EncCount;

    /* Validate parameters */
    if (NULL == ChnAttr || channel >= EncCnt) {
        PRINT_ERROR("VideoVPS_SetOutputParam: channel %d parameter error\n", channel);
        return ret;
    }

    if (0 == ChnAttr->OutFps) {
        PRINT_ERROR("VideoVPS_SetOutputParam: channel %d OutFps %d invalid\n",
                    channel, ChnAttr->OutFps);
        return ret;
    }

    /* Validate supported stream type */
    if (!((pCaptureDevice->EncDevice[channel].SupportStream >> dwType) & 0x01)) {
        PRINT_ERROR("VideoVPS_SetOutputParam: dwType %d not supported\n", dwType);
        return ret;
    }

    /* Get encoder channel number for this stream type */
    for (int i = 0; i < pCaptureDevice->EncDevice[channel].StreamCount; i++) {
        if (dwType == pCaptureDevice->EncDevice[channel].StreamDevice[i].CapChn) {
            VencChn = i;
        }
    }

    /* Step 1: Disable VPS channel via adapter */
    ret = adapter->vps_disable_chn(VpsGrp, VencChn);
    if (ret != RETURN_OK) {
        PRINT_ERROR("vps_disable_chn failed with %#x\n", ret);
        return ret;
    }

    /* Step 2: If resolution changed, destroy OSD/Logo (will be recreated later) */
    if (ChnAttr->OutHeight != pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.max_height ||
        ChnAttr->OutWidth != pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.max_width)
    {
        VideoOSD_Destroy();
        VideoOSD_DestroyLogo();
    }

    /* Step 3: Get current VPS channel attributes via adapter */
    ret = adapter->vps_get_chn_attr(VpsGrp, VencChn, &VPS_ChnAttr);
    if (ret != RETURN_OK) {
        PRINT_ERROR("vps_get_chn_attr failed with %#x\n", ret);
        return ret;
    }

    /* Debug: Print GetChnAttr result */
    /*PRINT_INFO("[VPS_DEBUG] Before modify - enAttrType=%d, OutWidth=%d, OutHeight=%d\n",
               VPS_ChnAttr.enAttrType, VPS_ChnAttr.u32OutWidth, VPS_ChnAttr.u32OutHeight);
    PRINT_INFO("[VPS_DEBUG] Before modify - EncAttr: YStride=%d, CStride=%d, enType=%d, OutFps=%d\n",
               VPS_ChnAttr.stEncAttr.u32YStride, VPS_ChnAttr.stEncAttr.u32CStride,
               VPS_ChnAttr.stEncAttr.enType, VPS_ChnAttr.stEncAttr.u32OutFps);
    PRINT_INFO("[VPS_DEBUG] Before modify - EncAttr: enWorkMode=%d, OfflineBufNum=%d, bCompress=%d, enOutFormat=%d\n",
               VPS_ChnAttr.stEncAttr.enWorkMode, VPS_ChnAttr.stEncAttr.u32OfflineBufNum,
               VPS_ChnAttr.stEncAttr.bCompress, VPS_ChnAttr.stEncAttr.enOutFormat);*/

    /* Step 4: Update channel attributes */
    VPS_ChnAttr.u32OutHeight = ChnAttr->OutHeight;
    VPS_ChnAttr.u32OutWidth = ChnAttr->OutWidth;
    VPS_ChnAttr.stEncAttr.u32YStride = VPS_ChnAttr.u32OutWidth;
    VPS_ChnAttr.stEncAttr.u32CStride = VPS_ChnAttr.u32OutWidth;
    VPS_ChnAttr.stEncAttr.enType = ChnAttr->EnPayLoad;

    /* Step 5: Set output frame rate (limited by sensor fps) */
    VPS_ChnAttr.stEncAttr.u32OutFps = (VideoConfig_GetSensorFps() > ChnAttr->OutFps) ? ChnAttr->OutFps : VideoConfig_GetSensorFps();

    /* Special handling for 12fps: multiply by 2 for timestamp calculation */
    if (12 == VideoConfig_GetSensorFps()) {
        VPS_ChnAttr.stEncAttr.u32OutFps *= 2;
    }

    /* Step 6: Apply channel attributes via adapter */
    ret = adapter->vps_set_chn_attr(VpsGrp, VencChn, &VPS_ChnAttr);
    if (ret != RETURN_OK) {
        PRINT_ERROR("vps_set_chn_attr failed with %#x\n", ret);
        return ret;
    }

    /* Step 7: Re-enable VPS channel via adapter */
    ret = adapter->vps_enable_chn(VpsGrp, VencChn);
    if (ret != RETURN_OK) {
        PRINT_ERROR("vps_enable_chn failed with %#x\n", ret);
        return ret;
    }

    return ret;
}



