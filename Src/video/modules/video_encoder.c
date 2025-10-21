/*
 * video_encoder.c -- VENC编码模块实现
 *
 * 本模块负责视频编码相关的所有操作，包括通道管理、动态参数设置、编码控制等。
 * 所有操作通过platform_adapter抽象层调用SDK，不直接使用NI_MDK_*或SDK_COMM_*函数。
 *
 * Phase 2: VENC模块重构
 *
 * Created as part of video pipeline modularization refactoring.
 */

/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "video_encoder.h"
#include "platform_adapter.h"
#include "PrintGrade.h"

/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */

/* ========================================================================== */
/*                          内部函数声明区                                    */
/* ========================================================================== */

/* ========================================================================== */
/*                          全局变量定义区                                    */
/* ========================================================================== */

/* ========================================================================== */
/*                          函数实现区                                        */
/* ========================================================================== */

/*
 * ===== 初始化接口实现 =====
 */

int VideoEncoder_CreateChannel(int VencChn, VENC_CHN_ATTR_S* pAttr)
{
    PlatformAdapter *adapter = GetPlatformAdapter();

    if (!adapter) {
        PRINT_ERROR("Platform adapter not initialized\n");
        return VENC_FAILURE;
    }

    if (!pAttr) {
        PRINT_ERROR("VENC channel attr is NULL\n");
        return VENC_FAILURE;
    }

    if (VencChn < 0 || VencChn >= MAX_VENC_CHN_NUM) {
        PRINT_ERROR("Invalid VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    PRINT_INFO("Creating VENC channel %d\n", VencChn);

    return adapter->venc_create(VencChn, pAttr);
}

int VideoEncoder_DestroyChannel(int VencChn)
{
    PlatformAdapter *adapter = GetPlatformAdapter();

    if (!adapter) {
        PRINT_ERROR("Platform adapter not initialized\n");
        return VENC_FAILURE;
    }

    if (VencChn < 0 || VencChn >= MAX_VENC_CHN_NUM) {
        PRINT_ERROR("Invalid VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    PRINT_INFO("Destroying VENC channel %d\n", VencChn);

    return adapter->venc_destroy(VencChn);
}

int VideoEncoder_Start(int VencChn)
{
    PlatformAdapter *adapter = GetPlatformAdapter();

    if (!adapter) {
        PRINT_ERROR("Platform adapter not initialized\n");
        return VENC_FAILURE;
    }

    if (VencChn < 0 || VencChn >= MAX_VENC_CHN_NUM) {
        PRINT_ERROR("Invalid VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    PRINT_INFO("Starting VENC channel %d\n", VencChn);

    return adapter->venc_start(VencChn);
}

int VideoEncoder_Stop(int VencChn)
{
    PlatformAdapter *adapter = GetPlatformAdapter();

    if (!adapter) {
        PRINT_ERROR("Platform adapter not initialized\n");
        return VENC_FAILURE;
    }

    if (VencChn < 0 || VencChn >= MAX_VENC_CHN_NUM) {
        PRINT_ERROR("Invalid VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    PRINT_INFO("Stopping VENC channel %d\n", VencChn);

    return adapter->venc_stop(VencChn);
}

/*
 * ===== 动态参数设置实现 =====
 */

int VideoEncoder_SetFrameRate(int VencChn, int fps)
{
    PlatformAdapter *adapter = GetPlatformAdapter();
    VENC_CHN_ATTR_S stChnAttr;
    int ret;

    if (!adapter) {
        PRINT_ERROR("Platform adapter not initialized\n");
        return VENC_FAILURE;
    }

    if (VencChn < 0 || VencChn >= MAX_VENC_CHN_NUM) {
        PRINT_ERROR("Invalid VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    if (fps <= 0 || fps > 120) {
        PRINT_ERROR("Invalid fps %d\n", fps);
        return VENC_FAILURE;
    }

    PRINT_INFO("Setting VENC channel %d frame rate to %d\n", VencChn, fps);

    /* Stop → Get → Modify → Set → Start pattern */

    /* Step 1: Stop receiving pictures */
    ret = adapter->venc_stop(VencChn);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to stop VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    /* Step 2: Get current channel attributes */
    memset(&stChnAttr, 0, sizeof(VENC_CHN_ATTR_S));
    ret = adapter->venc_get_chn_attr(VencChn, &stChnAttr);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to get VENC channel %d attributes\n", VencChn);
        adapter->venc_start(VencChn);  /* Try to restart */
        return VENC_FAILURE;
    }

    /* Step 3: Modify frame rate */
    stChnAttr.stRcAttr.u32FrmRateNum = fps;

    /* Step 4: Set new attributes */
    ret = adapter->venc_set_chn_attr(VencChn, &stChnAttr);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to set VENC channel %d attributes\n", VencChn);
        adapter->venc_start(VencChn);  /* Try to restart */
        return VENC_FAILURE;
    }

    /* Step 5: Restart receiving pictures */
    ret = adapter->venc_start(VencChn);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to restart VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    return VENC_SUCCESS;
}

int VideoEncoder_SetBitRate(int VencChn, int bitrate_kbps)
{
    PlatformAdapter *adapter = GetPlatformAdapter();
    VENC_CHN_ATTR_S stChnAttr;
    int ret;

    if (!adapter) {
        PRINT_ERROR("Platform adapter not initialized\n");
        return VENC_FAILURE;
    }

    if (VencChn < 0 || VencChn >= MAX_VENC_CHN_NUM) {
        PRINT_ERROR("Invalid VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    if (bitrate_kbps <= 0) {
        PRINT_ERROR("Invalid bitrate %d\n", bitrate_kbps);
        return VENC_FAILURE;
    }

    PRINT_INFO("Setting VENC channel %d bitrate to %d kbps\n", VencChn, bitrate_kbps);

    /* Stop → Get → Modify → Set → Start pattern */

    ret = adapter->venc_stop(VencChn);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to stop VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    memset(&stChnAttr, 0, sizeof(VENC_CHN_ATTR_S));
    ret = adapter->venc_get_chn_attr(VencChn, &stChnAttr);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to get VENC channel %d attributes\n", VencChn);
        adapter->venc_start(VencChn);
        return VENC_FAILURE;
    }

    /* Modify bitrate according to RC mode */
    if (stChnAttr.stRcAttr.enRcMode == VENC_RC_MODE_CBR) {
        stChnAttr.stRcAttr.stAttrCbr.u32TargetBitRate = bitrate_kbps * 1024;
    } else if (stChnAttr.stRcAttr.enRcMode == VENC_RC_MODE_VBR) {
        stChnAttr.stRcAttr.stAttrVbr.u32MaxBitRate = bitrate_kbps * 1024;
    } else if (stChnAttr.stRcAttr.enRcMode == VENC_RC_MODE_CVBR) {
        stChnAttr.stRcAttr.stAttrCVbr.u32MaxBitRate = bitrate_kbps * 1024;
    } else if (stChnAttr.stRcAttr.enRcMode == VENC_RC_MODE_AVBR) {
        stChnAttr.stRcAttr.stAttrAVbr.u32MaxBitRate = bitrate_kbps * 1024;
    } else if (stChnAttr.stRcAttr.enRcMode == VENC_RC_MODE_QVBR) {
        stChnAttr.stRcAttr.stAttrQVbr.u32MaxBitRate = bitrate_kbps * 1024;
    } else {
        PRINT_WARN("Unsupported RC mode %d for bitrate setting\n", stChnAttr.stRcAttr.enRcMode);
    }

    ret = adapter->venc_set_chn_attr(VencChn, &stChnAttr);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to set VENC channel %d attributes\n", VencChn);
        adapter->venc_start(VencChn);
        return VENC_FAILURE;
    }

    ret = adapter->venc_start(VencChn);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to restart VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    return VENC_SUCCESS;
}

int VideoEncoder_SetQP(int VencChn, int minQp, int maxQp, int minIQp, int maxIQp)
{
    PlatformAdapter *adapter = GetPlatformAdapter();
    VENC_CHN_ATTR_S stChnAttr;
    int ret;

    if (!adapter) {
        PRINT_ERROR("Platform adapter not initialized\n");
        return VENC_FAILURE;
    }

    if (VencChn < 0 || VencChn >= MAX_VENC_CHN_NUM) {
        PRINT_ERROR("Invalid VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    PRINT_INFO("Setting VENC channel %d QP: P[%d,%d] I[%d,%d]\n",
               VencChn, minQp, maxQp, minIQp, maxIQp);

    ret = adapter->venc_stop(VencChn);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to stop VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    /* Get current channel attributes */
    memset(&stChnAttr, 0, sizeof(VENC_CHN_ATTR_S));
    ret = adapter->venc_get_chn_attr(VencChn, &stChnAttr);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to get VENC channel %d attributes\n", VencChn);
        adapter->venc_start(VencChn);
        return VENC_FAILURE;
    }

    /* Modify QP values according to RC mode */
    switch (stChnAttr.stRcAttr.enRcMode) {
        case VENC_RC_MODE_CBR:
            stChnAttr.stRcAttr.stAttrCbr.u32MinPQp = minQp;
            stChnAttr.stRcAttr.stAttrCbr.u32MaxPQp = maxQp;
            stChnAttr.stRcAttr.stAttrCbr.u32MinIQp = minIQp;
            stChnAttr.stRcAttr.stAttrCbr.u32MaxIQp = maxIQp;
            break;
        case VENC_RC_MODE_VBR:
            stChnAttr.stRcAttr.stAttrVbr.u32MinPQp = minQp;
            stChnAttr.stRcAttr.stAttrVbr.u32MaxPQp = maxQp;
            stChnAttr.stRcAttr.stAttrVbr.u32MinIQp = minIQp;
            stChnAttr.stRcAttr.stAttrVbr.u32MaxIQp = maxIQp;
            break;
        case VENC_RC_MODE_CVBR:
            stChnAttr.stRcAttr.stAttrCVbr.u32MinPQp = minQp;
            stChnAttr.stRcAttr.stAttrCVbr.u32MaxPQp = maxQp;
            stChnAttr.stRcAttr.stAttrCVbr.u32MinIQp = minIQp;
            stChnAttr.stRcAttr.stAttrCVbr.u32MaxIQp = maxIQp;
            break;
        case VENC_RC_MODE_AVBR:
            stChnAttr.stRcAttr.stAttrAVbr.u32MinPQp = minQp;
            stChnAttr.stRcAttr.stAttrAVbr.u32MaxPQp = maxQp;
            stChnAttr.stRcAttr.stAttrAVbr.u32MinIQp = minIQp;
            stChnAttr.stRcAttr.stAttrAVbr.u32MaxIQp = maxIQp;
            break;
        case VENC_RC_MODE_QVBR:
            stChnAttr.stRcAttr.stAttrQVbr.u32MinPQp = minQp;
            stChnAttr.stRcAttr.stAttrQVbr.u32MaxPQp = maxQp;
            stChnAttr.stRcAttr.stAttrQVbr.u32MinIQp = minIQp;
            stChnAttr.stRcAttr.stAttrQVbr.u32MaxIQp = maxIQp;
            break;
        default:
            PRINT_ERROR("Unsupported RC mode %d for QP setting\n", stChnAttr.stRcAttr.enRcMode);
            adapter->venc_start(VencChn);
            return VENC_FAILURE;
    }

    ret = adapter->venc_set_chn_attr(VencChn, &stChnAttr);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to set VENC channel %d attributes\n", VencChn);
        adapter->venc_start(VencChn);
        return VENC_FAILURE;
    }

    ret = adapter->venc_start(VencChn);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to restart VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    return VENC_SUCCESS;
}

int VideoEncoder_SetGOP(int VencChn, int gop)
{
    PlatformAdapter *adapter = GetPlatformAdapter();
    VENC_CHN_ATTR_S stChnAttr;
    int ret;

    if (!adapter) {
        PRINT_ERROR("Platform adapter not initialized\n");
        return VENC_FAILURE;
    }

    if (VencChn < 0 || VencChn >= MAX_VENC_CHN_NUM) {
        PRINT_ERROR("Invalid VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    if (gop <= 0 || gop > 1000) {
        PRINT_ERROR("Invalid GOP %d\n", gop);
        return VENC_FAILURE;
    }

    PRINT_INFO("Setting VENC channel %d GOP to %d\n", VencChn, gop);

    ret = adapter->venc_stop(VencChn);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to stop VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    memset(&stChnAttr, 0, sizeof(VENC_CHN_ATTR_S));
    ret = adapter->venc_get_chn_attr(VencChn, &stChnAttr);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to get VENC channel %d attributes\n", VencChn);
        adapter->venc_start(VencChn);
        return VENC_FAILURE;
    }

    stChnAttr.stGopAttr.stGopNomalAttr.u32Gop = gop;

    ret = adapter->venc_set_chn_attr(VencChn, &stChnAttr);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to set VENC channel %d attributes\n", VencChn);
        adapter->venc_start(VencChn);
        return VENC_FAILURE;
    }

    ret = adapter->venc_start(VencChn);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to restart VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    return VENC_SUCCESS;
}

int VideoEncoder_SetRcMode(int VencChn, VENC_RC_MODE_E rcMode)
{
    PlatformAdapter *adapter = GetPlatformAdapter();
    VENC_CHN_ATTR_S stChnAttr;
    int ret;

    if (!adapter) {
        PRINT_ERROR("Platform adapter not initialized\n");
        return VENC_FAILURE;
    }

    if (VencChn < 0 || VencChn >= MAX_VENC_CHN_NUM) {
        PRINT_ERROR("Invalid VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    PRINT_INFO("Setting VENC channel %d RC mode to %d\n", VencChn, rcMode);

    ret = adapter->venc_stop(VencChn);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to stop VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    memset(&stChnAttr, 0, sizeof(VENC_CHN_ATTR_S));
    ret = adapter->venc_get_chn_attr(VencChn, &stChnAttr);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to get VENC channel %d attributes\n", VencChn);
        adapter->venc_start(VencChn);
        return VENC_FAILURE;
    }

    stChnAttr.stRcAttr.enRcMode = rcMode;

    ret = adapter->venc_set_chn_attr(VencChn, &stChnAttr);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to set VENC channel %d attributes\n", VencChn);
        adapter->venc_start(VencChn);
        return VENC_FAILURE;
    }

    ret = adapter->venc_start(VencChn);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to restart VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    return VENC_SUCCESS;
}

int VideoEncoder_SetResolution(int VencChn, int width, int height)
{
    PlatformAdapter *adapter = GetPlatformAdapter();
    VENC_CHN_ATTR_S stChnAttr;
    int ret;

    if (!adapter) {
        PRINT_ERROR("Platform adapter not initialized\n");
        return VENC_FAILURE;
    }

    if (VencChn < 0 || VencChn >= MAX_VENC_CHN_NUM) {
        PRINT_ERROR("Invalid VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) {
        PRINT_ERROR("Invalid resolution %dx%d\n", width, height);
        return VENC_FAILURE;
    }

    PRINT_INFO("Setting VENC channel %d resolution to %dx%d\n", VencChn, width, height);

    ret = adapter->venc_stop(VencChn);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to stop VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    memset(&stChnAttr, 0, sizeof(VENC_CHN_ATTR_S));
    ret = adapter->venc_get_chn_attr(VencChn, &stChnAttr);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to get VENC channel %d attributes\n", VencChn);
        adapter->venc_start(VencChn);
        return VENC_FAILURE;
    }

    stChnAttr.stVeAttr.stInputPicAttr.u32PicWidth = width;
    stChnAttr.stVeAttr.stInputPicAttr.u32PicHeight = height;

    ret = adapter->venc_set_chn_attr(VencChn, &stChnAttr);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to set VENC channel %d attributes\n", VencChn);
        adapter->venc_start(VencChn);
        return VENC_FAILURE;
    }

    ret = adapter->venc_start(VencChn);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to restart VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    return VENC_SUCCESS;
}

/*
 * ===== 编码控制实现 =====
 */

int VideoEncoder_RequestIDR(int VencChn)
{
    PlatformAdapter *adapter = GetPlatformAdapter();

    if (!adapter) {
        PRINT_ERROR("Platform adapter not initialized\n");
        return VENC_FAILURE;
    }

    if (VencChn < 0 || VencChn >= MAX_VENC_CHN_NUM) {
        PRINT_ERROR("Invalid VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    if (!adapter->venc_request_idr) {
        PRINT_ERROR("IDR request interface not available\n");
        return VENC_FAILURE;
    }

    PRINT_INFO("Requesting IDR frame for VENC channel %d\n", VencChn);

    return adapter->venc_request_idr(VencChn);
}

int VideoEncoder_EnableIntraRefresh(int VencChn, int enable)
{
    /* TODO: Implement intra refresh control */
    PRINT_WARN("VideoEncoder_EnableIntraRefresh not yet implemented\n");
    return VENC_SUCCESS;
}

/*
 * ===== 属性获取实现 =====
 */

int VideoEncoder_GetChannelAttr(int VencChn, VENC_CHN_ATTR_S* pAttr)
{
    PlatformAdapter *adapter = GetPlatformAdapter();

    if (!adapter) {
        PRINT_ERROR("Platform adapter not initialized\n");
        return VENC_FAILURE;
    }

    if (!pAttr) {
        PRINT_ERROR("Output attribute pointer is NULL\n");
        return VENC_FAILURE;
    }

    if (VencChn < 0 || VencChn >= MAX_VENC_CHN_NUM) {
        PRINT_ERROR("Invalid VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    return adapter->venc_get_chn_attr(VencChn, pAttr);
}

int VideoEncoder_GetStreamInfo(int VencChn, VENC_STREAM_S* pStream)
{
    PlatformAdapter *adapter = GetPlatformAdapter();

    if (!adapter) {
        PRINT_ERROR("Platform adapter not initialized\n");
        return VENC_FAILURE;
    }

    if (!pStream) {
        PRINT_ERROR("Output stream pointer is NULL\n");
        return VENC_FAILURE;
    }

    if (VencChn < 0 || VencChn >= MAX_VENC_CHN_NUM) {
        PRINT_ERROR("Invalid VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    return adapter->venc_get_stream(VencChn, pStream, -1);
}

/*
 * ===== 旋转/镜像实现 =====
 */

int VideoEncoder_SetRotate(int VencChn, int enRotation)
{
    PlatformAdapter *adapter = GetPlatformAdapter();

    if (!adapter) {
        PRINT_ERROR("Platform adapter not initialized\n");
        return VENC_FAILURE;
    }

    if (VencChn < 0 || VencChn >= MAX_VENC_CHN_NUM) {
        PRINT_ERROR("Invalid VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    if (!adapter->venc_set_rotate) {
        PRINT_ERROR("Rotate interface not available\n");
        return VENC_FAILURE;
    }

    PRINT_INFO("Setting VENC channel %d rotation to %d\n", VencChn, enRotation);

    return adapter->venc_set_rotate(VencChn, enRotation);
}

int VideoEncoder_SetMirror(int VencChn, int bMirror, int bFlip)
{
    /* TODO: Implement mirror/flip control */
    PRINT_WARN("VideoEncoder_SetMirror not yet implemented\n");
    return VENC_SUCCESS;
}
