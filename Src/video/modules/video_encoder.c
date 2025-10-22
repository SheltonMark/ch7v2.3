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
/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "video_encoder.h"
#include "../../Include/APIs/TdCommon.h"
#include "../../Include/APIs/Video.h"
#include "../../Include/APIs/Capture.h"
#include "platform_adapter.h"
#include "PrintGrade.h"
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

    /* 场景一：帧率是动态属性，可以动态修改 - Get → Set */

    memset(&stChnAttr, 0, sizeof(VENC_CHN_ATTR_S));
    ret = adapter->venc_get_chn_attr(VencChn, &stChnAttr);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to get VENC channel %d attributes\n", VencChn);
        return VENC_FAILURE;
    }

    stChnAttr.stRcAttr.u32FrmRateNum = fps;

    ret = adapter->venc_set_chn_attr(VencChn, &stChnAttr);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to set VENC channel %d attributes\n", VencChn);
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

    /* 场景一：码率是动态属性，可以动态修改 - Get → Set */

    memset(&stChnAttr, 0, sizeof(VENC_CHN_ATTR_S));
    ret = adapter->venc_get_chn_attr(VencChn, &stChnAttr);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to get VENC channel %d attributes\n", VencChn);
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

    /* 场景一：QP是动态属性，可以动态修改 - Get → Set */

    memset(&stChnAttr, 0, sizeof(VENC_CHN_ATTR_S));
    ret = adapter->venc_get_chn_attr(VencChn, &stChnAttr);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to get VENC channel %d attributes\n", VencChn);
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
            return VENC_FAILURE;
    }

    ret = adapter->venc_set_chn_attr(VencChn, &stChnAttr);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to set VENC channel %d attributes\n", VencChn);
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

    if (gop <= 0 || gop > 300) {
        PRINT_ERROR("Invalid GOP %d\n", gop);
        return VENC_FAILURE;
    }

    PRINT_INFO("Setting VENC channel %d GOP to %d\n", VencChn, gop);

    /* 场景一：GOP是动态属性，可以动态修改 - Get → Set */

    memset(&stChnAttr, 0, sizeof(VENC_CHN_ATTR_S));
    ret = adapter->venc_get_chn_attr(VencChn, &stChnAttr);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to get VENC channel %d attributes\n", VencChn);
        return VENC_FAILURE;
    }

    stChnAttr.stGopAttr.stGopNomalAttr.u32Gop = gop;

    ret = adapter->venc_set_chn_attr(VencChn, &stChnAttr);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to set VENC channel %d attributes\n", VencChn);
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
/**
 * @brief 设置编码类型（H264/H265）
 * @note 场景三：修改编码类型需要销毁重建通道
 *       Stop → Destroy → Get → Modify → Create → Start
 */
int VideoEncoder_SetCodecType(int VencChn, PAYLOAD_TYPE_E codecType, VENC_PROFILE_E profile)
{
    PlatformAdapter *adapter = GetPlatformAdapter();
    VENC_CHN_ATTR_S stChnAttr;
    int ret;

    if (!adapter) {
        PRINT_ERROR("Platform adapter not initialized\n");
        return VENC_FAILURE;

    if (VencChn < 0 || VencChn >= MAX_VENC_CHN_NUM) {
        PRINT_ERROR("Invalid VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    if (codecType != PT_H264 && codecType != PT_H265) {
        PRINT_ERROR("Invalid codec type %d (only H264/H265 supported)\n", codecType);
        return VENC_FAILURE;
    }

    PRINT_INFO("Setting VENC channel %d codec type to %s, profile %d\n", 
               VencChn, (codecType == PT_H264) ? "H264" : "H265", profile);

    /* 场景三：Get → Stop → Destroy → Create → Start */

    /* Step 1: Get current attributes */
    memset(&stChnAttr, 0, sizeof(VENC_CHN_ATTR_S));
    ret = adapter->venc_get_chn_attr(VencChn, &stChnAttr);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to get VENC channel %d attributes\n", VencChn);
        return VENC_FAILURE;
    }

    /* Step 2: Stop receiving pictures */
    ret = adapter->venc_stop(VencChn);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to stop VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    /* Step 3: Destroy channel */
    ret = adapter->venc_destroy(VencChn);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to destroy VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    /* Step 4: Modify codec type and profile */
    stChnAttr.stVeAttr.enType = codecType;
    stChnAttr.stVeAttr.enProfile = profile;

    /* Step 5: Create channel with new codec type */
    ret = adapter->venc_create(VencChn, &stChnAttr);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to create VENC channel %d with codec %d\n", VencChn, codecType);
        return VENC_FAILURE;
    }

    /* Step 6: Start receiving pictures */
    ret = adapter->venc_start(VencChn);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to start VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    PRINT_INFO("Successfully changed VENC channel %d codec type\n", VencChn);
    return VENC_SUCCESS;
}
}

/**
 * @brief 设置编码通道分辨率
 * @note 场景四：修改分辨率需要VPS+VENC联动
 *       VENC_Stop → VPS_DisableChn → VPS_GetAttr → VPS_SetAttr(new_resolution) → VPS_EnableChn
 *       → VENC_GetAttr → VENC_SetAttr(new_resolution) → VENC_Start
 */
int VideoEncoder_SetResolution(int VencChn, int width, int height)
{
    PlatformAdapter *adapter = GetPlatformAdapter();
    VPS_CHN_ATTR_S vpsAttr;
    VENC_CHN_ATTR_S vencAttr;
    int VpsGrp = 0;  /* AX520CE只使用VpsGrp=0 */
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

    /* 场景四流程：VENC Stop → VPS Disable → VPS Set → VPS Enable → VENC Set → VENC Start */

    /* Step 1: Stop VENC receiving pictures */
    ret = adapter->venc_stop(VencChn);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to stop VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    /* Step 2: Disable VPS channel */
    ret = adapter->vps_disable_chn(VpsGrp, VencChn);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to disable VPS channel grp=%d chn=%d\n", VpsGrp, VencChn);
        adapter->venc_start(VencChn);  /* Try to recover */
        return VENC_FAILURE;
    }

    /* Step 3: Get VPS channel attributes */
    memset(&vpsAttr, 0, sizeof(VPS_CHN_ATTR_S));
    ret = adapter->vps_get_chn_attr(VpsGrp, VencChn, &vpsAttr);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to get VPS channel %d attributes\n", VencChn);
        adapter->vps_enable_chn(VpsGrp, VencChn);  /* Try to recover */
        adapter->venc_start(VencChn);
        return VENC_FAILURE;
    }

    /* Step 4: Modify VPS resolution */
    vpsAttr.u32OutWidth = width;
    vpsAttr.u32OutHeight = height;

    /* Step 5: Set VPS channel attributes */
    ret = adapter->vps_set_chn_attr(VpsGrp, VencChn, &vpsAttr);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to set VPS channel %d attributes\n", VencChn);
        adapter->vps_enable_chn(VpsGrp, VencChn);  /* Try to recover */
        adapter->venc_start(VencChn);
        return VENC_FAILURE;
    }

    /* Step 6: Enable VPS channel */
    ret = adapter->vps_enable_chn(VpsGrp, VencChn);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to enable VPS channel grp=%d chn=%d\n", VpsGrp, VencChn);
        adapter->venc_start(VencChn);  /* Try to recover */
        return VENC_FAILURE;
    }

    /* Step 7: Get VENC channel attributes */
    memset(&vencAttr, 0, sizeof(VENC_CHN_ATTR_S));
    ret = adapter->venc_get_chn_attr(VencChn, &vencAttr);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to get VENC channel %d attributes\n", VencChn);
        adapter->venc_start(VencChn);  /* Try to recover */
        return VENC_FAILURE;
    }

    /* Step 8: Modify VENC resolution */
    vencAttr.stVeAttr.stInputPicAttr.u32PicWidth = width;
    vencAttr.stVeAttr.stInputPicAttr.u32PicHeight = height;
    vencAttr.stVeAttr.stInputPicAttr.au32Stride[0] = width; /* maybe fix align */
    vencAttr.stVeAttr.stInputPicAttr.au32Stride[1] = width; /* maybe fix align */

    /* Step 9: Set VENC channel attributes */
    ret = adapter->venc_set_chn_attr(VencChn, &vencAttr);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to set VENC channel %d attributes\n", VencChn);
        adapter->venc_start(VencChn);  /* Try to recover */
        return VENC_FAILURE;
    }

    /* Step 10: Start VENC receiving pictures */
    ret = adapter->venc_start(VencChn);
    if (ret != VENC_SUCCESS) {
        PRINT_ERROR("Failed to restart VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    PRINT_INFO("Successfully changed VENC channel %d resolution to %dx%d\n", VencChn, width, height);
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
    PlatformAdapter *adapter = GetPlatformAdapter();

    if (!adapter) {
        PRINT_ERROR("Platform adapter not initialized\n");
        return VENC_FAILURE;
    }

    if (VencChn < 0 || VencChn >= MAX_VENC_CHN_NUM) {
        PRINT_ERROR("Invalid VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    if (!adapter->venc_set_mirror) {
        PRINT_ERROR("Mirror interface not available\n");
        return VENC_FAILURE;
    }

    PRINT_INFO("Setting VENC channel %d mirror=%d flip=%d\n", VencChn, bMirror, bFlip);

    return adapter->venc_set_mirror(VencChn, bMirror, bFlip);
}

/*
 * ===== Additional interfaces (Phase 3) =====
 */

int VideoEncoder_SetChannelAttr(int VencChn, VENC_CHN_ATTR_S* pAttr)
{
    PlatformAdapter *adapter = GetPlatformAdapter();

    if (!adapter) {
        PRINT_ERROR("Platform adapter not initialized\n");
        return VENC_FAILURE;
    }

    if (!pAttr) {
        PRINT_ERROR("Channel attribute pointer is NULL\n");
        return VENC_FAILURE;
    }

    if (VencChn < 0 || VencChn >= MAX_VENC_CHN_NUM) {
        PRINT_ERROR("Invalid VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    if (!adapter->venc_set_chn_attr) {
        PRINT_ERROR("SetChnAttr interface not available\n");
        return VENC_FAILURE;
    }

    PRINT_INFO("Setting VENC channel %d attributes\n", VencChn);

    return adapter->venc_set_chn_attr(VencChn, pAttr);
}

int VideoEncoder_SetStreamCheck(int VencChn, int checkMode)
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

    if (!adapter->venc_set_stream_check) {
        PRINT_ERROR("SetStreamCheck interface not available\n");
        return VENC_FAILURE;
    }

    PRINT_INFO("Setting VENC channel %d stream check mode to %d\n", VencChn, checkMode);

    return adapter->venc_set_stream_check(VencChn, checkMode);
}

int VideoEncoder_SetOutFrameRate(int VencChn, int frameRate)
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

    if (frameRate <= 0 || frameRate > 120) {
        PRINT_ERROR("Invalid frame rate %d\n", frameRate);
        return VENC_FAILURE;
    }

    if (!adapter->venc_set_out_frame_rate) {
        PRINT_ERROR("SetOutFrameRate interface not available\n");
        return VENC_FAILURE;
    }

    PRINT_INFO("Setting VENC channel %d output frame rate to %d\n", VencChn, frameRate);

    return adapter->venc_set_out_frame_rate(VencChn, frameRate);
}

int VideoEncoder_GetRcParam(int VencChn, VENC_RC_PARAM_S* pRcParam)
{
    PlatformAdapter *adapter = GetPlatformAdapter();

    if (!adapter) {
        PRINT_ERROR("Platform adapter not initialized\n");
        return VENC_FAILURE;
    }

    if (!pRcParam) {
        PRINT_ERROR("RC param pointer is NULL\n");
        return VENC_FAILURE;
    }

    if (VencChn < 0 || VencChn >= MAX_VENC_CHN_NUM) {
        PRINT_ERROR("Invalid VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    if (!adapter->venc_get_rc_param) {
        PRINT_ERROR("GetRcParam interface not available\n");
        return VENC_FAILURE;
    }

    return adapter->venc_get_rc_param(VencChn, pRcParam);
}

int VideoEncoder_SetRcParam(int VencChn, VENC_RC_PARAM_S* pRcParam)
{
    PlatformAdapter *adapter = GetPlatformAdapter();

    if (!adapter) {
        PRINT_ERROR("Platform adapter not initialized\n");
        return VENC_FAILURE;
    }

    if (!pRcParam) {
        PRINT_ERROR("RC param pointer is NULL\n");
        return VENC_FAILURE;
    }

    if (VencChn < 0 || VencChn >= MAX_VENC_CHN_NUM) {
        PRINT_ERROR("Invalid VENC channel %d\n", VencChn);
        return VENC_FAILURE;
    }

    if (!adapter->venc_set_rc_param) {
        PRINT_ERROR("SetRcParam interface not available\n");
        return VENC_FAILURE;
    }

    PRINT_INFO("Setting VENC channel %d RC parameters\n", VencChn);

    return adapter->venc_set_rc_param(VencChn, pRcParam);
}

// ============================================================================
// External declarations for UpdateChannelConfig
// ============================================================================

// QP tables from video.c
extern CaptureImageQuality_t CaptureQtTable[6];
extern CaptureImageQuality_t CHL_2END_T_CaptureQtTable[6];

// Sensor frame rate
extern unsigned int sensor_fps;

// OSD lock
extern pthread_mutex_t osd_lock;

// OSD/LOGO functions from video.c
extern void _set_title(void);
extern void _set_cover(void);
extern void _set_logo(void);
extern void _venc_error(VENC_CHN_ATTR_S *pAttr);

// ============================================================================
// Helper functions for VideoEncoder_UpdateChannelConfig
// ============================================================================

/// 根据通道号获取对应的QP表
static const CaptureImageQuality_t* _get_qp_table(int VencChn)
{
	return (VencChn == CHL_2END_T) ? CHL_2END_T_CaptureQtTable : CaptureQtTable;
}

/// 填充CBR模式的QP参数
static void _fill_rc_cbr_qp(VENC_CHN_ATTR_S *pAttr, int qt_level, int VencChn)
{
	const CaptureImageQuality_t *qp_table = _get_qp_table(VencChn);
	pAttr->stRcAttr.stAttrCbr.u32MinIQp = qp_table[qt_level - 1].IminQP;
	pAttr->stRcAttr.stAttrCbr.u32MaxIQp = qp_table[qt_level - 1].ImaxQP;
	pAttr->stRcAttr.stAttrCbr.u32MinPQp = qp_table[qt_level - 1].PminQP;
	pAttr->stRcAttr.stAttrCbr.u32MaxPQp = qp_table[qt_level - 1].PmaxQP;
}

/// 填充QVBR模式的QP参数
static void _fill_rc_qvbr_qp(VENC_CHN_ATTR_S *pAttr, int qt_level, int VencChn)
{
	const CaptureImageQuality_t *qp_table = _get_qp_table(VencChn);
	pAttr->stRcAttr.stAttrQVbr.u32MinIQp = qp_table[qt_level - 1].IminQP;
	pAttr->stRcAttr.stAttrQVbr.u32MaxIQp = qp_table[qt_level - 1].ImaxQP;
	pAttr->stRcAttr.stAttrQVbr.u32MinPQp = qp_table[qt_level - 1].PminQP;
	pAttr->stRcAttr.stAttrQVbr.u32MaxPQp = qp_table[qt_level - 1].PmaxQP;
}

/// 填充VENC通道属性的RC参数
static int _fill_venc_rc_attr(VENC_CHN_ATTR_S *pAttr, channel_info *info,
                               int VencChn, unsigned int MaxBitRate)
{
	switch (pAttr->stRcAttr.enRcMode)
	{
		case VENC_RC_MODE_CBR:
			pAttr->stRcAttr.stAttrCbr.u32StatTime = 1;
			pAttr->stRcAttr.stAttrCbr.u32TargetBitRate = MIN(info->bps, MaxBitRate);
			_fill_rc_cbr_qp(pAttr, info->qt_level, VencChn);
			break;

		case VENC_RC_MODE_QVBR:
			pAttr->stRcAttr.stAttrQVbr.u32StatTime = 1;
			pAttr->stRcAttr.stAttrQVbr.u32MaxBitRate = MIN(info->bps, MaxBitRate);
			pAttr->stRcAttr.stAttrQVbr.s32ChangePos = 85;
			_fill_rc_qvbr_qp(pAttr, info->qt_level, VencChn);
			pAttr->stRcAttr.stAttrQVbr.s32MinBitratePos = 32;
			if (PT_H265 == pAttr->stVeAttr.enType) {
				pAttr->stRcAttr.stAttrQVbr.s32BestQuality  = 50;
				pAttr->stRcAttr.stAttrQVbr.s32WorstQuality = 60;
			} else {
				pAttr->stRcAttr.stAttrQVbr.s32BestQuality  = 60;
				pAttr->stRcAttr.stAttrQVbr.s32WorstQuality = 70;
			}
			break;

		default:
			PRINT_ERROR("unknown enRcMode %d\n", pAttr->stRcAttr.enRcMode);
			return -1;
	}
	return 0;
}

int VideoEncoder_UpdateChannelConfig(int channel, DWORD dwType, channel_info *info)
{
	int EncCnt = 0;
	int ret = FALSE;
	int jpeg_fps = 4;				//设置抓图通道的帧率为4
	unsigned int VencChn = 0;
	unsigned int MaxBitRate = 0;
	BOOL VencVhnDestroy = FALSE;
	VENC_CHN_ATTR_S VENC_ChnAttr;
	int jpeg_chn = STREAM_TYPE_THI; // 抓图为第三通道
	SensorDevice_p pSensorDevice = &GlobalDevice.SensorDevice;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

	memset(&VENC_ChnAttr,0,sizeof(VENC_CHN_ATTR_S));

	EncCnt = pCaptureDevice->EncCount;

	if (NULL == info || channel >= EncCnt)
	{
		pthread_mutex_unlock(&osd_lock);
		PRINT_ERROR("channel %d parameter error\n", channel);
		return ret;
	}

	if (0 == info->frame_count)
	{
		pthread_mutex_unlock(&osd_lock);
		PRINT_ERROR("channel %d parameter FramesPerSecond %d error\n", channel, info->frame_count);
		return ret;
	}

	// 校验支持的码流的通道，上层传递通道方式为掩码
	if (!((pCaptureDevice->EncDevice[channel].SupportStream >> dwType) & 0x01))
	{
		pthread_mutex_unlock(&osd_lock);
		PRINT_ERROR("not support dwType = %d\n", dwType);
		return ret;
	}

	// 获取编码通道号
	for (int i = 0; i < pCaptureDevice->EncDevice[channel].StreamCount; i++)
	{
		if (dwType == pCaptureDevice->EncDevice[channel].StreamDevice[i].CapChn)
		{
			VencChn = i;
		}
	}

	ret = VideoEncoder_GetChannelAttr(VencChn, &VENC_ChnAttr);
	if (ret != RETURN_OK)
	{
		pthread_mutex_unlock(&osd_lock);
		PRINT_ERROR("Error(%d - %x): VideoEncoder_GetChannelAttr\n", ret, ret);
		return -1;
	}

	//仅当编码类型和编码等级发生修改时，需要销毁编码通道，其他情况均不销毁
	if (VENC_ChnAttr.stVeAttr.enType != info->enc_type || VENC_ChnAttr.stVeAttr.enProfile != info->profile)
	{
		ret = VideoEncoder_DestroyChannel(VencChn);
		if (ret != RETURN_OK)
		{
			pthread_mutex_unlock(&osd_lock);
			PRINT_ERROR("Error(%d - %x): VideoEncoder_DestroyChannel\n", ret, ret);
			return -1;
		}

		//关闭码流校验功能
		ret = VideoEncoder_SetStreamCheck(VencChn, VENC_STREAM_CHECK_OFF);
		if (ret != RETURN_OK)
		{
			pthread_mutex_unlock(&osd_lock);
			PRINT_ERROR("Error(%d - %x): VideoEncoder_SetStreamCheck\n", ret, ret);
			return -1;
		}

		// 设置编码通道的buff大小，仅在销毁重建时才能修改
		//这边直接将编码通道的buff大小设置为最大，可以规避小分辨率切换到大分辨率需要销毁重建的问题
		if (STREAM_TYPE_SEC == VencChn)
		{
			VENC_ChnAttr.stVeAttr.u32StrmBufSize = 245761;
		}
		else
		{
			VENC_ChnAttr.stVeAttr.u32StrmBufSize = pSensorDevice->MaxHeight * pSensorDevice->MaxWidth / 2;
		}

		VencVhnDestroy = TRUE;
	}


	VENC_ChnAttr.stVeAttr.enType = info->enc_type;
	VENC_ChnAttr.stVeAttr.enProfile = info->profile;
	VENC_ChnAttr.stVeAttr.stInputPicAttr.u32PicWidth = info->width;
	VENC_ChnAttr.stVeAttr.stInputPicAttr.u32PicHeight = info->height;
	VENC_ChnAttr.stRcAttr.enRcMode = info->rc_type;
	// 这边判断下sensor帧率和用户配置帧率的大小
	// 若sensor帧率大于用户配置帧率，则以用户配置帧率为准
	// 若sensor帧率小于用户配置帧率，则以sensor帧率为准
	VENC_ChnAttr.stRcAttr.u32FrmRateNum = (sensor_fps > info->frame_count) ? info->frame_count : sensor_fps;

	VENC_ChnAttr.stGopAttr.stGopNomalAttr.u32Gop = MAX(VENC_ChnAttr.stRcAttr.u32FrmRateNum, 1) * 2;
	VENC_ChnAttr.stVeAttr.stInputPicAttr.au32Stride[0] = info->width;
	VENC_ChnAttr.stVeAttr.stInputPicAttr.au32Stride[1] = info->width;
	VENC_ChnAttr.stVeAttr.stInputPicAttr.au32Stride[2] = 0;
	MaxBitRate = VENC_ChnAttr.stRcAttr.u32FrmRateNum / VENC_ChnAttr.stRcAttr.u32FrmRateDenom * info->width * info->height * 8 * 3 / 2000;

	// 使用辅助函数填充RC参数，简化代码
	ret = _fill_venc_rc_attr(&VENC_ChnAttr, info, VencChn, MaxBitRate);
	if (ret != 0)
	{
		pthread_mutex_unlock(&osd_lock);
		return ret;
	}

	//根据码流是否被销毁来创建/设置编码通道
	if(TRUE == VencVhnDestroy)
	{
		ret = VideoEncoder_CreateChannel(VencChn, &VENC_ChnAttr);
		if (ret != RETURN_OK)
		{
			pthread_mutex_unlock(&osd_lock);
			PRINT_ERROR("Error(%d - %x): channel: %d VideoEncoder_CreateChannel\n", ret, ret, VencChn);
			_venc_error(&VENC_ChnAttr);
			return -1;
		}
	}
	else
	{
		ret = VideoEncoder_SetChannelAttr(VencChn, &VENC_ChnAttr);
		if (ret != RETURN_OK)
		{
			pthread_mutex_unlock(&osd_lock);
			PRINT_ERROR("Error(%d - %x): VideoEncoder_SetChannelAttr\n", ret, ret);
			return -1;
		}
	}

	//当分辨率发生改变时，OSD和LOGO被销毁了，在这边重新创建
	if (info->width != pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.max_width ||
		info->height != pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.max_height)
	{
		//这边先保存分辨率配置，设置OSD、LOGO、隐私遮蔽需要使用该参数
		pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.max_width = info->width;
		pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.max_height = info->height;

		_set_title();
		_set_cover();
		_set_logo();
	}


	if (STREAM_TYPE_FIR == VencChn)
	{
		VENC_RC_PARAM_S stRcParam;
		memset(&stRcParam, 0, sizeof(VENC_RC_PARAM_S));

		ret = VideoEncoder_GetRcParam(VencChn, &stRcParam);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("Error(%d - %x): VideoEncoder_GetRcParam\n", ret, ret);
			return ret;
		}

		stRcParam.stRcParamH26x.s32MaxISize = 512 * 1024;

		ret = VideoEncoder_SetRcParam(VencChn, &stRcParam);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("Error(%d - %x): VideoEncoder_SetRcParam\n", ret, ret);
			return ret;
		}

		PRINT_INFO("set main stream max I frame size 512KB\n");
	}

	//保存配置
	pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.bps = info->bps;
	pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.rc_type = info->rc_type;
	pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.profile = info->profile;
	pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.qt_level = info->qt_level;
	pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.enc_type = info->enc_type;
	pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.frame_count = info->frame_count;

	//由于抓图通道与子码流YUV通道绑定，所以需要跟随子码流分辨率调整分辨率
	if (CHL_2END_T == VencChn)
	{
		ret = VideoEncoder_Stop(jpeg_chn);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("Error(%d - %x): VideoEncoder_GetChannelAttr\n", ret, ret);
			return ret;
		}
		// 抓图通道设置参数与子码流一致
		ret = VideoEncoder_GetChannelAttr(jpeg_chn, &VENC_ChnAttr);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("Error(%d - %x): VideoEncoder_GetChannelAttr\n", ret, ret);
			return ret;
		}
		//设置抓图通道的输入帧率
		VENC_ChnAttr.stRcAttr.u32FrmRateNum = (sensor_fps > info->frame_count) ? info->frame_count : sensor_fps;
		VENC_ChnAttr.stVeAttr.stInputPicAttr.u32PicWidth = info->width;
		VENC_ChnAttr.stVeAttr.stInputPicAttr.u32PicHeight = info->height;
		VENC_ChnAttr.stVeAttr.stInputPicAttr.au32Stride[0] = info->width;
		VENC_ChnAttr.stVeAttr.stInputPicAttr.au32Stride[1] = info->width;
		VENC_ChnAttr.stVeAttr.stInputPicAttr.au32Stride[2] = 0;

		ret = VideoEncoder_SetChannelAttr(jpeg_chn, &VENC_ChnAttr);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("Error(%d - %x): VideoEncoder_SetChannelAttr\n", ret, ret);
			return ret;
		}

		//当子码流帧率小于抓图通道帧率时，以子码流帧率为准
		jpeg_fps = (jpeg_fps > info->frame_count) ? info->frame_count : jpeg_fps;

		//设置抓图通道的输出帧率
		ret = VideoEncoder_SetOutFrameRate(jpeg_chn, jpeg_fps);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("Error(%d - %x): channel: %d VideoEncoder_SetOutFrameRate\n", ret, ret, VencChn);
			return -1;
		}

		ret = VideoEncoder_Start(jpeg_chn);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("Error(%d - %x): VideoEncoder_Start\n", ret, ret);
			return ret;
		}

		pCaptureDevice->EncDevice[channel].StreamDevice[jpeg_chn].EncChannel_info.max_width = info->width;
		pCaptureDevice->EncDevice[channel].StreamDevice[jpeg_chn].EncChannel_info.max_height = info->height;
	}

	return ret;
}
