/**
 * @file video_frame_rate.c
 * @brief Video Frame Rate Adaptation Module Implementation
 */

#include "TdCommon.h"
#include "sdk_comm.h"
#include "video_frame_rate.h"
#include "../modules/include/video_vps.h"
#include "../modules/include/video_encoder.h"

extern unsigned int gFrameRate;   // Global frame rate variable (video.c)
extern unsigned int sensor_fps;   // Global sensor fps variable (video.c)

/**
 * @brief Sensor frame rate change callback (top-level interface)
 */
int get_frame_rate(float FrameRate)
{
	unsigned int u32CurFrame;
	int ret;

	u32CurFrame = (unsigned int)FrameRate;
	if (u32CurFrame != gFrameRate)
	{
		/* 检测到帧率变化（2倍或0.5倍关系） */
		if ((gFrameRate == u32CurFrame * 2) || (gFrameRate == u32CurFrame / 2))
		{
			PRINT_INFO("Sensor frame rate changing: %d -> %d (2x or 0.5x detected)\n", gFrameRate, u32CurFrame);

			/* 同步应用新帧率到所有编码通道 */
			ret = VideoFrameRate_HandleSensorChange(0, u32CurFrame);
			if (ret != RETURN_OK) {
				PRINT_ERROR("Error(%x): VideoFrameRate_HandleSensorChange failed\n", ret);
				return FAILED;
			}

			PRINT_INFO("Sensor frame rate applied successfully: %d fps\n", u32CurFrame);
		}

		gFrameRate = u32CurFrame;
		PRINT_INFO("gFrameRate updated to %d\n", gFrameRate);
	}

	return SUCCESS;
}

/**
 * @brief Handle sensor frame rate change event (internal implementation)
 */
int VideoFrameRate_HandleSensorChange(int channel, unsigned int new_sensor_fps)
{
    int ret;
    CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

    // Validate parameters
    if (channel >= pCaptureDevice->EncCount) {
        PRINT_ERROR("Invalid channel %d\n", channel);
        return -1;
    }

    PRINT_INFO("Sensor frame rate changing: %d -> %d\n", sensor_fps, new_sensor_fps);

    // Step 1: Set VPS input frame rate (sensor -> VPS)
    ret = VideoVPS_SetInputFrameRate(channel, new_sensor_fps);
    if (ret != RETURN_OK) {
        PRINT_ERROR("Error(%x): VideoVPS_SetInputFrameRate failed\n", ret);
        return -1;
    }

    // Step 2: Update all encoding channels
    for (int ichannel = 0; ichannel < pCaptureDevice->EncDevice[channel].StreamCount; ichannel++)
    {
        // Skip JPEG channel (STREAM_TYPE_THI)
        if (STREAM_TYPE_THI == ichannel) {
            break;
        }

        // Prepare VPS output attributes (keep current resolution, update fps)
        VPS_CHN_OUT_ATTR vpsAttr;
        memset(&vpsAttr, 0, sizeof(VPS_CHN_OUT_ATTR));

        vpsAttr.OutWidth = pCaptureDevice->EncDevice[channel].StreamDevice[ichannel].EncChannel_info.max_width;
        vpsAttr.OutHeight = pCaptureDevice->EncDevice[channel].StreamDevice[ichannel].EncChannel_info.max_height;
        vpsAttr.OutFps = pCaptureDevice->EncDevice[channel].StreamDevice[ichannel].EncChannel_info.frame_count;
        vpsAttr.EnPayLoad = pCaptureDevice->EncDevice[channel].StreamDevice[ichannel].EncChannel_info.enc_type;

        PRINT_INFO("Channel %d: updating fps (resolution=%dx%d, fps=%d, codec=%d)\n",
                   ichannel, vpsAttr.OutWidth, vpsAttr.OutHeight, vpsAttr.OutFps, vpsAttr.EnPayLoad);

        // Stop encoder
        ret = VideoEncoder_Stop(ichannel);
        if (ret != RETURN_OK) {
            PRINT_ERROR("Error(%x): VideoEncoder_Stop channel %d failed\n", ret, ichannel);
            VideoEncoder_Start(ichannel);  // Try recover
            continue;
        }

        // Update VPS output parameters
        ret = VideoVPS_SetOutputParam(channel, ichannel, &vpsAttr);
        if (ret != RETURN_OK) {
            PRINT_ERROR("Error(%x): VideoVPS_SetOutputParam channel %d failed\n", ret, ichannel);
            VideoEncoder_Start(ichannel);  // Try recover
            continue;
        }

        // Prepare channel_info for VENC update
        channel_info info;
        memset(&info, 0, sizeof(channel_info));

        info.width = vpsAttr.OutWidth;
        info.height = vpsAttr.OutHeight;
        info.frame_count = vpsAttr.OutFps;
        info.bps = pCaptureDevice->EncDevice[channel].StreamDevice[ichannel].EncChannel_info.bps;
        info.rc_type = pCaptureDevice->EncDevice[channel].StreamDevice[ichannel].EncChannel_info.rc_type;
        info.profile = pCaptureDevice->EncDevice[channel].StreamDevice[ichannel].EncChannel_info.profile;
        info.qt_level = pCaptureDevice->EncDevice[channel].StreamDevice[ichannel].EncChannel_info.qt_level;
        info.enc_type = pCaptureDevice->EncDevice[channel].StreamDevice[ichannel].EncChannel_info.enc_type;

        // Update VENC parameters (without osd_lock - caller should handle)
        ret = VideoEncoder_UpdateChannelConfig(channel, ichannel, &info);
        if (ret != RETURN_OK) {
            PRINT_ERROR("Error(%x): VideoEncoder_UpdateChannelConfig channel %d failed\n", ret, ichannel);
            VideoEncoder_Start(ichannel);  // Try recover
            continue;
        }

        // Start encoder
        ret = VideoEncoder_Start(ichannel);
        if (ret != RETURN_OK) {
            PRINT_ERROR("Error(%x): VideoEncoder_Start channel %d failed\n", ret, ichannel);
            continue;
        }

        PRINT_INFO("Channel %d: frame rate updated successfully\n", ichannel);
    }

    // Update global sensor_fps variable
    sensor_fps = new_sensor_fps;

    PRINT_INFO("Sensor frame rate change completed: all channels updated to %d fps\n", new_sensor_fps);
    return RETURN_OK;
}
