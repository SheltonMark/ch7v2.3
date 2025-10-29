/**
 * @file video_input.c
 * @brief Video Input Module Implementation
 * @details 视频输入模块，处理 VI (Video Input) 和 Sensor 相关操作
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "video_input.h"
#include "../../adapter/include/platform_adapter.h"
#include "../../Include/APIs/TdCommon.h"
#include "PrintGrade.h"
#include "../../Include/APIs/Video.h"  /* For GlobalDevice access */
#include "../../Include/APIs/Capture.h"
#include "../../Include/APIs/HumanDetect.h"

/* External references to global device */
extern GlobalDevice_t GlobalDevice;

/**
 * @brief 根据 Sensor 类型和 mirror/flip 状态确定 Bayer 序列
 */
static int DetermineBayerSequence(int SenType, int mirrorcurr, int flipcurr, VI_DEV_ATTR_S* pDevAttr)
{
    if (SEN_TYPE_OS04L10 == SenType)
    {
        if (0 == mirrorcurr)
        {
            if (0 == flipcurr)
            {
                pDevAttr->enRgbSeq = BAYER_SEQ_BGGR;
            }
            else if (1 == flipcurr)
            {
                pDevAttr->enRgbSeq = BAYER_SEQ_GRBG;
            }
            else
            {
                PRINT_ERROR("Invalid flipcurr %d\n", flipcurr);
                return VI_FAILURE;
            }
        }
        else if (1 == mirrorcurr)
        {
            if (0 == flipcurr)
            {
                pDevAttr->enRgbSeq = BAYER_SEQ_GBRG;
            }
            else if (1 == flipcurr)
            {
                pDevAttr->enRgbSeq = BAYER_SEQ_RGGB;
            }
            else
            {
                PRINT_ERROR("Invalid flipcurr %d\n", flipcurr);
                return VI_FAILURE;
            }
        }
        else
        {
            PRINT_ERROR("Invalid mirrorcurr %d\n", mirrorcurr);
            return VI_FAILURE;
        }
    }
    else if (SEN_TYPE_OS04D10 == SenType)
    {
        if (0 == mirrorcurr)
        {
            if (0 == flipcurr)
            {
                pDevAttr->enRgbSeq = BAYER_SEQ_RGGB;
            }
            else if (1 == flipcurr)
            {
                pDevAttr->enRgbSeq = BAYER_SEQ_GBRG;
            }
            else
            {
                PRINT_ERROR("Invalid flipcurr %d\n", flipcurr);
                return VI_FAILURE;
            }
        }
        else if (1 == mirrorcurr)
        {
            if (0 == flipcurr)
            {
                pDevAttr->enRgbSeq = BAYER_SEQ_GRBG;
            }
            else if (1 == flipcurr)
            {
                pDevAttr->enRgbSeq = BAYER_SEQ_BGGR;
            }
            else
            {
                PRINT_ERROR("Invalid flipcurr %d\n", flipcurr);
                return VI_FAILURE;
            }
        }
        else
        {
            PRINT_ERROR("Invalid mirrorcurr %d\n", mirrorcurr);
            return VI_FAILURE;
        }
    }
    /* Other sensor types: do nothing */

    return VI_SUCCESS;
}

/**
 * @brief 设置镜像和翻转
 */
int VideoInput_SetMirrorAndFlip(int channel, int mirror, int flip)
{
    PlatformAdapter *adapter = GetPlatformAdapter();
    static int last_flip = -1;
    static int last_mirror = -1;
    VI_DEV ViDev = 0;
    SENSOR_ID SenId = 0;
    VI_DEV_ATTR_S pstDevAttr;
    int mirrorcurr, flipcurr;
    int ret;

    SensorDevice_p pSensorDevice = &GlobalDevice.SensorDevice;
    CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

    if (!adapter) {
        PRINT_ERROR("Platform adapter not initialized\n");
        return VI_FAILURE;
    }

    /* 参数校验 */
    if ((0 != mirror && 1 != mirror) || (0 != flip && 1 != flip))
    {
        PRINT_ERROR("Invalid parameters: mirror=%d flip=%d (must be 0 or 1)\n", mirror, flip);
        return VI_FAILURE;
    }

    /* 去重：如果参数未变化，直接返回 */
    if (last_mirror == mirror && last_flip == flip)
    {
        PRINT_INFO("Mirror/Flip unchanged (mirror=%d flip=%d), skipping\n", mirror, flip);
        return VI_SUCCESS;
    }

    PRINT_INFO("Setting mirror=%d flip=%d for channel=%d\n", mirror, flip, channel);

    /* 计算实际的 mirror/flip 值 (XOR with device default) */
    mirrorcurr = (mirror ^ pCaptureDevice->MirrorAndflip);
    flipcurr = (flip ^ pCaptureDevice->MirrorAndflip);

    /* Step 1: Get VI device attributes */
    memset(&pstDevAttr, 0, sizeof(pstDevAttr));
    ret = adapter->vi_get_dev_attr(ViDev, &pstDevAttr);
    if (ret != VI_SUCCESS)
    {
        PRINT_ERROR("Failed to get VI device %d attributes\n", ViDev);
        return VI_FAILURE;
    }

    /* Step 2: Disable VI device */
    ret = adapter->vi_disable_dev(ViDev);
    if (ret != VI_SUCCESS)
    {
        PRINT_ERROR("Failed to disable VI device %d\n", ViDev);
        return VI_FAILURE;
    }

    /* Step 3: Set sensor mirror */
    ret = adapter->sensor_set_mirror(SenId, mirrorcurr);
    if (ret != VI_SUCCESS)
    {
        PRINT_ERROR("Failed to set sensor %d mirror to %d\n", SenId, mirrorcurr);
        adapter->vi_enable_dev(ViDev);  /* Try to recover */
        return VI_FAILURE;
    }

    /* Step 4: Set sensor flip */
    ret = adapter->sensor_set_flip(SenId, flipcurr);
    if (ret != VI_SUCCESS)
    {
        PRINT_ERROR("Failed to set sensor %d flip to %d\n", SenId, flipcurr);
        adapter->vi_enable_dev(ViDev);  /* Try to recover */
        return VI_FAILURE;
    }

    /* Step 5: Determine Bayer sequence based on sensor type and mirror/flip */
    ret = DetermineBayerSequence(pSensorDevice->SenType, mirrorcurr, flipcurr, &pstDevAttr);
    if (ret != VI_SUCCESS)
    {
        PRINT_ERROR("Failed to determine Bayer sequence\n");
        adapter->vi_enable_dev(ViDev);
        return VI_FAILURE;
    }

    /* Step 6: Set VI device attributes */
    ret = adapter->vi_set_dev_attr(ViDev, &pstDevAttr);
    if (ret != VI_SUCCESS)
    {
        PRINT_ERROR("Failed to set VI device %d attributes\n", ViDev);
        adapter->vi_enable_dev(ViDev);
        return VI_FAILURE;
    }

    /* Step 7: Enable VI device */
    ret = adapter->vi_enable_dev(ViDev);
    if (ret != VI_SUCCESS)
    {
        PRINT_ERROR("Failed to enable VI device %d\n", ViDev);
        return VI_FAILURE;
    }

    /* Step 8: Notify HumanTrack module */
    ret = HumanTrackSetMirror(mirror, flip);
    if (ret != 0)
    {
        PRINT_ERROR("HumanTrackSetMirror failed with %#x (continuing anyway)\n", ret);
        /* Not a critical failure, continue */
    }

    /* Update last values */
    last_flip = flip;
    last_mirror = mirror;

    PRINT_INFO("Successfully set mirror=%d flip=%d\n", mirror, flip);
    return VI_SUCCESS;
}

/**
 * @brief 处理输入格式变化（ISP 帧率同步）
 *
 * 当通道分辨率或帧率变化时，需要同时更新 ISP、VPS 和 Sensor 的帧率设置
 * 这是三者联动的关键功能
 *
 * fps 从 pVideoInDevice->VDInfo[channel].ViDstFps 读取，如果是 4K 分辨率则固定为 15fps
 *
 * @param channel     物理通道号
 * @param pstSize     分辨率 (宽度和高度)
 * @return 0 成功，<0 失败
 */
int VideoInput_HandleFormatChange(int channel, SIZE_S pstSize)
{
    int ret = VI_SUCCESS;
    int fps = 0;
    int sensor_fps = 0;
    ISP_DEV IspDev = 0;
    SENSOR_ID SenId = 0;
    VPS_GRP VpsGrp = 0;

    SEN_INFO_S stSenInfo;
    PISP_PUB_ATTR_S stPubAttr;
    AISP_VIDEO_FMT_INFO_S stVideoFmtInfo;
    AISP_AFPS_CTRL_S stAFpsCtrl;
    VPS_GRP_ATTR_S stGrpAttr;

    PlatformAdapter *adapter = GetPlatformAdapter();
    VideoInDevice_p pVideoInDevice = &GlobalDevice.VideoInDevice;

    if (!adapter) {
        PRINT_ERROR("Platform adapter not initialized\n");
        return VI_FAILURE;
    }

    /* ===== Step 0: 确定目标帧率 ===== */
#ifdef SUPPORT_4K
    if (pstSize.u32Width == 3840 && pstSize.u32Height == 2160) {
        fps = 15;  /* 4K resolution fixed to 15fps */
    }
    else
#endif
    {
        /* 从 GlobalDevice 读取目标帧率 */
        if (30 == pVideoInDevice->VDInfo[channel].ViDstFps) {
            fps = 30;
        }
        else if (25 == pVideoInDevice->VDInfo[channel].ViDstFps) {
            fps = 25;
        }
    }

    if (!fps) {
        PRINT_INFO("VI: Keep current fps (fps=0)\n");
        return VI_SUCCESS;
    }

    PRINT_INFO("VI: HandleFormatChange - channel=%d, %dx%d@%dfps\n",
               channel, pstSize.u32Width, pstSize.u32Height, fps);

    /* ===== Step 1: 获取当前 Sensor 信息 ===== */
    memset(&stSenInfo, 0, sizeof(SEN_INFO_S));
    ret = adapter->sensor_get_cur_info(SenId, &stSenInfo);
    if (ret != VI_SUCCESS) {
        PRINT_ERROR("Failed to get sensor current info\n");
        return ret;
    }

    sensor_fps = stSenInfo.stSenFmtAttr.u32Fps / 1000;  /* Convert from milliHz to Hz */
    PRINT_INFO("VI: Current sensor fps=%d\n", sensor_fps);

    /* ===== Step 2: 如果帧率未变化，则直接返回 ===== */
    if (stSenInfo.stSenFmtAttr.u32Fps == (fps * 1000)) {
        PRINT_INFO("VI: Sensor fps unchanged (%d), skipping\n", fps);
        return VI_SUCCESS;
    }

    /* ===== Step 3: 获取 ISP 公共属性 ===== */
    memset(&stPubAttr, 0, sizeof(PISP_PUB_ATTR_S));
    ret = adapter->isp_get_pub_attr(IspDev, &stPubAttr);
    if (ret != VI_SUCCESS) {
        PRINT_ERROR("Failed to get ISP public attributes\n");
        return ret;
    }

    /* ===== Step 4: 设置 ISP 帧率 ===== */
    stPubAttr.u32FrameRate = fps;
    ret = adapter->isp_set_pub_attr(IspDev, &stPubAttr);
    if (ret != VI_SUCCESS) {
        PRINT_ERROR("Failed to set ISP public attributes\n");
        return ret;
    }
    PRINT_INFO("VI: Set ISP frame rate to %d fps\n", fps);

    /* ===== Step 5: 获取 ISP 视频格式信息 ===== */
    memset(&stVideoFmtInfo, 0, sizeof(AISP_VIDEO_FMT_INFO_S));
    ret = adapter->isp_get_video_fmt_attr(IspDev, &stVideoFmtInfo);
    if (ret != VI_SUCCESS) {
        PRINT_ERROR("Failed to get ISP video format attributes\n");
        return ret;
    }

    /* ===== Step 6: 设置 ISP 视频格式帧率 ===== */
    stVideoFmtInfo.u32Fps = fps * 1000;  /* Convert to milliHz */
    ret = adapter->isp_set_video_fmt_attr(IspDev, &stVideoFmtInfo);
    if (ret != VI_SUCCESS) {
        PRINT_ERROR("Failed to set ISP video format attributes\n");
        return ret;
    }
    PRINT_INFO("VI: Set ISP video fmt fps to %d (0x%x milliHz)\n", fps, stVideoFmtInfo.u32Fps);

    /* ===== Step 7: 获取 ISP 自动帧率控制设置 ===== */
    memset(&stAFpsCtrl, 0, sizeof(AISP_AFPS_CTRL_S));
    ret = adapter->isp_get_auto_fps(IspDev, &stAFpsCtrl);
    if (ret != VI_SUCCESS) {
        PRINT_WARN("Failed to get ISP auto FPS control (continuing anyway)\n");
        /* Not a critical failure */
    } else if (stAFpsCtrl.bEnable) {
        /* 如果启用了自动降帧，需要更新降帧目标帧率为 sensor 帧率的一半 */
        stAFpsCtrl.u32NewFps = stVideoFmtInfo.u32Fps / 2;
        ret = adapter->isp_set_auto_fps(IspDev, &stAFpsCtrl);
        if (ret != VI_SUCCESS) {
            PRINT_WARN("Failed to set ISP auto FPS (continuing anyway)\n");
            /* Not a critical failure */
        }
        PRINT_INFO("VI: Updated ISP auto FPS to %d\n", stAFpsCtrl.u32NewFps);
    }

    /* ===== Step 8: 设置 VPS Group 输入帧率 ===== */
    if (!adapter->vps_get_grp_attr || !adapter->vps_set_grp_attr) {
        PRINT_ERROR("VPS group attribute operations not supported\n");
        return VI_FAILURE;
    }

    memset(&stGrpAttr, 0, sizeof(VPS_GRP_ATTR_S));
    ret = adapter->vps_get_grp_attr(VpsGrp, &stGrpAttr);
    if (ret != VI_SUCCESS) {
        PRINT_ERROR("Failed to get VPS group attributes\n");
        return ret;
    }

    stGrpAttr.u32InFps = fps;
    ret = adapter->vps_set_grp_attr(VpsGrp, &stGrpAttr);
    if (ret != VI_SUCCESS) {
        PRINT_ERROR("Failed to set VPS group attributes\n");
        return ret;
    }
    PRINT_INFO("VI: Set VPS group input fps to %d\n", fps);

    /* ===== Step 9: 更新全局设备配置 ===== */
    pVideoInDevice->VDInfo[channel].ViDstFps = fps;

    PRINT_INFO("VI: Format change completed successfully: %dx%d@%dfps\n", pstSize.u32Width, pstSize.u32Height, fps);
    return VI_SUCCESS;
}

int VideoInput_SetVstd(int channel,DWORD dwStandard)
{
	SIZE_S pstSize;
	int ret = -1;
	VideoInDevice_p pVideoInDevice = &GlobalDevice.VideoInDevice;

    pstSize.u32Width = pVideoInDevice->vi_pic.u32Width;
    pstSize.u32Height = pVideoInDevice->vi_pic.u32Height;

    if(dwStandard == VIDEO_STANDARD_PAL)
	{
		pVideoInDevice->VDInfo[channel].ViDstFps = 25;
	}
	else if(dwStandard == VIDEO_STANDARD_NTSC)
	{
		pVideoInDevice->VDInfo[channel].ViDstFps = 30;
	}

	/* Use the new modularized ISP/Sensor/VPS synchronization function */
	ret = VideoInput_HandleFormatChange(channel, pstSize);
	if (ret != RETURN_OK) {
		PRINT_ERROR("VideoInput_HandleFormatChange failed: %x\n", ret);
		/* Continue processing - not critical for video standard change */
	}

	return 0;
}


