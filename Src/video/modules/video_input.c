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
