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
#include "video_vps.h"
#include "video_osd.h"
#include "video_config.h"
#include "video_input.h"
#include "platform_adapter.h"
#include "PrintGrade.h"



/* ========================================================================== */
/*                          全局变量定义区                                    */
/* ========================================================================== */

static VENC_PROFILE_E H264Profile_Table[]={
										VENC_H264_BASE_LINE_PROFILE,
										VENC_H264_MAIN_PROFILE,
										VENC_H264_HIGH_PROFILE
										};

static VENC_PROFILE_E H265Profile_Table[]={VENC_H265_MAIN_PROFILE};


/* ========================================================================== */
/*                          函数实现区                                        */
/* ========================================================================== */

static int _venc_error(VENC_CHN_ATTR_S *VENC_ChnAttr)
{
	// PRINT_ERROR("VENC_ChnAttr->stGopAttr.u32Gop %d\n", VENC_ChnAttr->stGopAttr.u32Gop);
	// PRINT_ERROR("VENC_ChnAttr->stGopAttr.u32DummyIGop %d\n", VENC_ChnAttr->stGopAttr.u32DummyIGop);
	// PRINT_ERROR("VENC_ChnAttr->stGopAttr.u32DummyIOffset %d\n", VENC_ChnAttr->stGopAttr.u32DummyIOffset);
	// PRINT_ERROR("VENC_ChnAttr->stGopAttr.u32DummyIType %d\n", VENC_ChnAttr->stGopAttr.u32DummyIType);

	PRINT_ERROR("VENC_ChnAttr->stVeAttr.enType %d\n", VENC_ChnAttr->stVeAttr.enType);
	PRINT_ERROR("VENC_ChnAttr->stVeAttr.enProfile %d\n", VENC_ChnAttr->stVeAttr.enProfile);
	PRINT_ERROR("VENC_ChnAttr->stVeAttr.u32StrmBufSize %d\n", VENC_ChnAttr->stVeAttr.u32StrmBufSize);
	PRINT_ERROR("VENC_ChnAttr->stVeAttr.stInputPicAttr.u32MaxPicWidth %d\n", VENC_ChnAttr->stVeAttr.stInputPicAttr.u32MaxPicWidth);
	PRINT_ERROR("VENC_ChnAttr->stVeAttr.stInputPicAttr.u32MaxPicHeight %d\n", VENC_ChnAttr->stVeAttr.stInputPicAttr.u32MaxPicHeight);
	PRINT_ERROR("VENC_ChnAttr->stVeAttr.stInputPicAttr.enPixelFormat %d\n", VENC_ChnAttr->stVeAttr.stInputPicAttr.enPixelFormat);
	PRINT_ERROR("VENC_ChnAttr->stVeAttr.stInputPicAttr.u32PicWidth %d\n", VENC_ChnAttr->stVeAttr.stInputPicAttr.u32PicWidth);
	PRINT_ERROR("VENC_ChnAttr->stVeAttr.stInputPicAttr.u32PicHeight %d\n", VENC_ChnAttr->stVeAttr.stInputPicAttr.u32PicHeight);
	PRINT_ERROR("VENC_ChnAttr->stVeAttr.stInputPicAttr.au32Stride[0]%d\n", VENC_ChnAttr->stVeAttr.stInputPicAttr.au32Stride[0]);
	PRINT_ERROR("VENC_ChnAttr->stVeAttr.stInputPicAttr.au32Stride[1] %d\n", VENC_ChnAttr->stVeAttr.stInputPicAttr.au32Stride[1]);
	PRINT_ERROR("VENC_ChnAttr->stVeAttr.stInputPicAttr.au32Stride[2] %d\n", VENC_ChnAttr->stVeAttr.stInputPicAttr.au32Stride[2]);

	PRINT_ERROR("VENC_ChnAttr->stRcAttr.enRcMode %d\n", VENC_ChnAttr->stRcAttr.enRcMode);
	PRINT_ERROR("VENC_ChnAttr->stRcAttr.u32FrmRateNum %d\n", VENC_ChnAttr->stRcAttr.u32FrmRateNum);
	PRINT_ERROR("VENC_ChnAttr->stRcAttr.u32FrmRateDenom %d\n", VENC_ChnAttr->stRcAttr.u32FrmRateDenom);

	switch (VENC_ChnAttr->stRcAttr.enRcMode)
	{
		case (VENC_RC_MODE_CBR):
			PRINT_ERROR("VENC_ChnAttr->stRcAttr.stAttrCbr.u32StatTime %d\n", VENC_ChnAttr->stRcAttr.stAttrCbr.u32StatTime);
			PRINT_ERROR("VENC_ChnAttr->stRcAttr.stAttrCbr.u32TargetBitRate %d\n", VENC_ChnAttr->stRcAttr.stAttrCbr.u32TargetBitRate);
			break;
		case (VENC_RC_MODE_QVBR):
			PRINT_ERROR("VENC_ChnAttr->stRcAttr.stAttrQVbr.u32MaxBitRate %d\n", VENC_ChnAttr->stRcAttr.stAttrQVbr.u32MaxBitRate);
			PRINT_ERROR("VENC_ChnAttr->stRcAttr.stAttrQVbr.u32StatTime %d\n", VENC_ChnAttr->stRcAttr.stAttrQVbr.u32StatTime);
			PRINT_ERROR("VENC_ChnAttr->stRcAttr.stAttrQVbr.u32MaxIQp %d\n", VENC_ChnAttr->stRcAttr.stAttrQVbr.u32MaxIQp);
			PRINT_ERROR("VENC_ChnAttr->stRcAttr.stAttrQVbr.u32MinIQp %d\n", VENC_ChnAttr->stRcAttr.stAttrQVbr.u32MinIQp);
			PRINT_ERROR("VENC_ChnAttr->stRcAttr.stAttrQVbr.u32MaxPQp %d\n", VENC_ChnAttr->stRcAttr.stAttrQVbr.u32MaxPQp);
			PRINT_ERROR("VENC_ChnAttr->stRcAttr.stAttrQVbr.u32MinPQp %d\n", VENC_ChnAttr->stRcAttr.stAttrQVbr.u32MinPQp);
			break;
		default:
			PRINT_ERROR("unknown enRcMode %d\n", VENC_ChnAttr->stRcAttr.enRcMode);
			return -1;
	}

	return 0;
}

/// 根据通道号获取对应的QP表
static const CaptureImageQuality_t* _get_qp_table(int VencChn)
{
	return (VencChn == CHL_2END_T) ? VideoConfig_GetSubstreamQualityTable() : VideoConfig_GetQualityTable();
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

static int _get_pic_size(video_size_t enPicSize, SIZE_S* pstSize)
{
	if (enPicSize >= VIDEO_SIZE_NR || enPicSize < 0)
	{
		return FAILED;
	}

	VideoInDevice_p pVideoInDevice = &GlobalDevice.VideoInDevice;
	int dstFps = pVideoInDevice->VDInfo[0].ViDstFps;
	int vstd = VIDEO_STANDARD_PAL;
	if(25 == dstFps)
	{
		vstd = VIDEO_STANDARD_PAL;
	}
	else if(30 == dstFps)
	{
		vstd = VIDEO_STANDARD_NTSC;
	}

	pstSize->u32Width = VideoConfig_GetImageSize(vstd, enPicSize)->u32Width;
	pstSize->u32Height = VideoConfig_GetImageSize(vstd, enPicSize)->u32Height;
	if(pstSize->u32Width == 0 && pstSize->u32Height == 0)
	{
		return FAILED;
	}
	return SUCCESS;
}

static int _get_pic_type(VIDEO_FORMAT * pFormat,struct channel_info *info)
{
	switch(pFormat->Compression)
	{
		case VIDEO_COMP_H264:
		{
			info->enc_type = PT_H264;
			info->profile = H264Profile_Table[pFormat->profile];
			switch(pFormat->BitRateControl)
			{
				case VIDEO_BRC_CBR:
					info->rc_type = VENC_RC_MODE_CBR;
					info->bps = info->bps*4/5;
					break;
				case VIDEO_BRC_VBR:
					info->rc_type = VENC_RC_MODE_QVBR;
					info->qt_level = pFormat->ImageQuality;
					break;
				default:
					PRINT_ERROR("H264 enRcMode (%d) not support!\n",pFormat->BitRateControl);
					return -1;
			}
			break;
		}
		
		case VIDEO_COMP_H265:
		{
			info->enc_type = PT_H265;
			info->profile = H265Profile_Table[pFormat->profile];
			switch(pFormat->BitRateControl)
			{
				case VIDEO_BRC_CBR:
					info->rc_type = VENC_RC_MODE_CBR;
					info->bps = info->bps*4/5;
					break;
				case VIDEO_BRC_VBR:
					info->rc_type = VENC_RC_MODE_QVBR;
					info->qt_level = pFormat->ImageQuality;
					break;
				default:
					PRINT_ERROR("H265 enRcMode (%d) not support!\n",pFormat->BitRateControl);
					return -1;
			}
			break;
		}
		
		case VIDEO_COMP_MJPG:
		{
			//info->enc_type = FH_MJPEG;
			switch(pFormat->BitRateControl)
			{
				case VIDEO_BRC_CBR:
					//info->rc_type = FH_RC_MJPEG_CBR;
					break;
				case VIDEO_BRC_VBR:
					//info->rc_type = FH_RC_MJPEG_VBR;
					break;
				default:
					PRINT_ERROR("MJPEG enRcMode (%d) not support!\n",pFormat->BitRateControl);
					return -1;
			}
			break;
		}

		case VIDEO_COMP_JPEG:
			//info->enc_type = FH_JPEG;
			break;
		default:
			PRINT_ERROR("enType (%d) not support!\n",pFormat->Compression);
			return -1;	
	}

	return 0;
}

/**
 * @brief 查找VENC通道号
 */
static int _find_venc_channel(int channel, DWORD dwType, unsigned int *pVencChn)
{
    CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

    if (channel >= pCaptureDevice->EncCount) {
        PRINT_ERROR("Invalid channel %d\n", channel);
        return -1;
    }

    if (!((pCaptureDevice->EncDevice[channel].SupportStream >> dwType) & 0x01)) {
        PRINT_ERROR("Stream type %d not supported\n", dwType);
        return -1;
    }

    for (int i = 0; i < pCaptureDevice->EncDevice[channel].StreamCount; i++) {
        if (pCaptureDevice->EncDevice[channel].StreamDevice[i].CapChn == dwType) {
            *pVencChn = i;
            return 0;
        }
    }

    PRINT_ERROR("VENC channel not found for dwType=%d\n", dwType);
    return -1;
}

/**
 * @brief 检测分辨率是否变化
 */
static BOOL _is_resolution_changed(int channel, int VencChn, channel_info *info)
{
    CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

    return (info->width != pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.max_width ||
            info->height != pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.max_height);
}

/**
 * @brief 重建OSD/Logo/Cover（分辨率变化时）
 */
static void _rebuild_overlays(int channel, int VencChn, channel_info *info)
{
    CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

    // 先保存分辨率配置（OSD/Logo/Cover模块需要读取）
    pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.max_width = info->width;
    pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.max_height = info->height;

    // 重建所有叠加层
    VideoOSD_SetTitleRegion();
    VideoVPS_SetCoverRegion();
    VideoOSD_SetLogoRegion();
}

/**
 * @brief 设置主码流最大I帧大小（512KB）
 */
static int _set_main_stream_max_i_frame_size(int VencChn)
{
    VENC_RC_PARAM_S stRcParam;
    int ret;

    memset(&stRcParam, 0, sizeof(VENC_RC_PARAM_S));
    ret = VideoEncoder_GetRcParam(VencChn, &stRcParam);
    if (ret != RETURN_OK) {
        PRINT_ERROR("Error(%d): VideoEncoder_GetRcParam\n", ret);
        return ret;
    }

    stRcParam.stRcParamH26x.s32MaxISize = 512 * 1024;

    ret = VideoEncoder_SetRcParam(VencChn, &stRcParam);
    if (ret != RETURN_OK) {
        PRINT_ERROR("Error(%d): VideoEncoder_SetRcParam\n", ret);
        return ret;
    }

    PRINT_INFO("Set main stream max I frame size to 512KB\n");
    return RETURN_OK;
}

/**
 * @brief 保存编码配置到GlobalDevice
 */
static void _save_channel_config(int channel, int VencChn, channel_info *info)
{
    CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

    pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.bps = info->bps;
    pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.rc_type = info->rc_type;
    pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.profile = info->profile;
    pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.qt_level = info->qt_level;
    pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.enc_type = info->enc_type;
    pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.frame_count = info->frame_count;
}

/**
 * @brief 同步JPEG通道参数（跟随子码流）
 */
static int _sync_jpeg_channel(int channel, int VencChn, channel_info *subStreamInfo)
{
    const int jpeg_chn = STREAM_TYPE_THI;
    const int jpeg_fps = 4;
    VENC_CHN_ATTR_S jpegAttr;
    CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
    int actualJpegFps;
    int ret;

    // Stop JPEG channel
    ret = VideoEncoder_Stop(jpeg_chn);
    if (ret != RETURN_OK) {
        PRINT_ERROR("Error(%d): VideoEncoder_Stop jpeg channel\n", ret);
        return ret;
    }

    // Get current JPEG attributes
    ret = VideoEncoder_GetChannelAttr(jpeg_chn, &jpegAttr);
    if (ret != RETURN_OK) {
        PRINT_ERROR("Error(%d): VideoEncoder_GetChannelAttr jpeg\n", ret);
        VideoEncoder_Start(jpeg_chn);  // Try recover
        return ret;
    }

    // Sync resolution and frame rate with sub stream
    jpegAttr.stRcAttr.u32FrmRateNum = (VideoConfig_GetSensorFps() > subStreamInfo->frame_count) ? subStreamInfo->frame_count : VideoConfig_GetSensorFps();
    jpegAttr.stVeAttr.stInputPicAttr.u32PicWidth = subStreamInfo->width;
    jpegAttr.stVeAttr.stInputPicAttr.u32PicHeight = subStreamInfo->height;
    jpegAttr.stVeAttr.stInputPicAttr.au32Stride[0] = subStreamInfo->width;
    jpegAttr.stVeAttr.stInputPicAttr.au32Stride[1] = subStreamInfo->width;
    jpegAttr.stVeAttr.stInputPicAttr.au32Stride[2] = 0;

    ret = VideoEncoder_SetChannelAttr(jpeg_chn, &jpegAttr);
    if (ret != RETURN_OK) {
        PRINT_ERROR("Error(%d): VideoEncoder_SetChannelAttr jpeg\n", ret);
        VideoEncoder_Start(jpeg_chn);  // Try recover
        return ret;
    }

    // Set output frame rate (4fps or sub stream fps, whichever is lower)
    actualJpegFps = (jpeg_fps > subStreamInfo->frame_count) ? subStreamInfo->frame_count : jpeg_fps;
    ret = VideoEncoder_SetOutFrameRate(jpeg_chn, actualJpegFps);
    if (ret != RETURN_OK) {
        PRINT_ERROR("Error(%d): VideoEncoder_SetOutFrameRate jpeg\n", ret);
        VideoEncoder_Start(jpeg_chn);  // Try recover
        return ret;
    }

    // Start JPEG channel
    ret = VideoEncoder_Start(jpeg_chn);
    if (ret != RETURN_OK) {
        PRINT_ERROR("Error(%d): VideoEncoder_Start jpeg\n", ret);
        return ret;
    }

    // Save JPEG channel config
    pCaptureDevice->EncDevice[channel].StreamDevice[jpeg_chn].EncChannel_info.max_width = subStreamInfo->width;
    pCaptureDevice->EncDevice[channel].StreamDevice[jpeg_chn].EncChannel_info.max_height = subStreamInfo->height;

    PRINT_INFO("JPEG channel synced with sub stream (%dx%d, %dfps)\n",
               subStreamInfo->width, subStreamInfo->height, actualJpegFps);
    return RETURN_OK;
}

/**
 * @brief 核心函数：应用VENC编码参数
 * @param channel 设备通道号
 * @param VencChn VENC通道号
 * @param info 编码配置
 * @param pResolutionChanged [out] 分辨率是否变化
 * @return 0成功，<0失败
 */
static int _apply_venc_config(int channel, int VencChn, channel_info *info, BOOL *pResolutionChanged)
{
    int ret;
    unsigned int MaxBitRate;
    BOOL needRecreate = FALSE;
    VENC_CHN_ATTR_S vencAttr;
    SensorDevice_p pSensorDevice = &GlobalDevice.SensorDevice;

    memset(&vencAttr, 0, sizeof(VENC_CHN_ATTR_S));

    // Get current attributes
    ret = VideoEncoder_GetChannelAttr(VencChn, &vencAttr);
    if (ret != RETURN_OK) {
        PRINT_ERROR("Error(%d): VideoEncoder_GetChannelAttr\n", ret);
        return -1;
    }

    // Check if need destroy & recreate (codec type or profile changed)
    if (vencAttr.stVeAttr.enType != info->enc_type || vencAttr.stVeAttr.enProfile != info->profile)
    {
        needRecreate = TRUE;

        // Destroy channel
        ret = VideoEncoder_DestroyChannel(VencChn);
        if (ret != RETURN_OK) {
            PRINT_ERROR("Error(%d): VideoEncoder_DestroyChannel\n", ret);
            return -1;
        }

        // Disable stream check
        ret = VideoEncoder_SetStreamCheck(VencChn, VENC_STREAM_CHECK_OFF);
        if (ret != RETURN_OK) {
            PRINT_ERROR("Error(%d): VideoEncoder_SetStreamCheck\n", ret);
            return -1;
        }

        // Set larger buffer size (avoid recreate when resolution increases)
        if (VencChn == STREAM_TYPE_SEC) {
            vencAttr.stVeAttr.u32StrmBufSize = 245761;
        } else {
            vencAttr.stVeAttr.u32StrmBufSize = pSensorDevice->MaxHeight * pSensorDevice->MaxWidth / 2;
        }
    }

    // Update all attributes
    vencAttr.stVeAttr.enType = info->enc_type;
    vencAttr.stVeAttr.enProfile = info->profile;
    vencAttr.stVeAttr.stInputPicAttr.u32PicWidth = info->width;
    vencAttr.stVeAttr.stInputPicAttr.u32PicHeight = info->height;
    vencAttr.stVeAttr.stInputPicAttr.au32Stride[0] = info->width;
    vencAttr.stVeAttr.stInputPicAttr.au32Stride[1] = info->width;
    vencAttr.stVeAttr.stInputPicAttr.au32Stride[2] = 0;

    // Update RC config
    vencAttr.stRcAttr.enRcMode = info->rc_type;
    vencAttr.stRcAttr.u32FrmRateNum = (VideoConfig_GetSensorFps() > info->frame_count) ? info->frame_count : VideoConfig_GetSensorFps();
    vencAttr.stGopAttr.stGopNomalAttr.u32Gop = MAX(vencAttr.stRcAttr.u32FrmRateNum, 1) * 2;

    MaxBitRate = vencAttr.stRcAttr.u32FrmRateNum / vencAttr.stRcAttr.u32FrmRateDenom
                 * info->width * info->height * 8 * 3 / 2000;

    ret = _fill_venc_rc_attr(&vencAttr, info, VencChn, MaxBitRate);
    if (ret != 0) {
        return ret;
    }

    // Create or Set channel
    if (needRecreate) {
        ret = VideoEncoder_CreateChannel(VencChn, &vencAttr);
        if (ret != RETURN_OK) {
            PRINT_ERROR("Error(%d): VideoEncoder_CreateChannel\n", ret);
            _venc_error(&vencAttr);
            return -1;
        }
    } else {
        ret = VideoEncoder_SetChannelAttr(VencChn, &vencAttr);
        if (ret != RETURN_OK) {
            PRINT_ERROR("Error(%d): VideoEncoder_SetChannelAttr\n", ret);
            return -1;
        }
    }

    // Main stream special handling (set max I frame size)
    if (VencChn == STREAM_TYPE_FIR) {
        ret = _set_main_stream_max_i_frame_size(VencChn);
        if (ret != RETURN_OK) {
            return ret;
        }
    }

    // Check if resolution changed
    *pResolutionChanged = _is_resolution_changed(channel, VencChn, info);

    return RETURN_OK;
}


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

int VideoEncoder_SetFormat(int channel, DWORD dwType, VIDEO_FORMAT * pFormat)
{
    int i = 0;
    int ret = -1;
    int EncCnt = 0;
    struct channel_info info;
    SIZE_S pstSize;
    unsigned int VencChn = 0;
    VPS_CHN_OUT_ATTR vpsAttr;
    CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
    VideoInDevice_p pVideoInDevice = &GlobalDevice.VideoInDevice;

    EncCnt = pCaptureDevice->EncCount;
    memset(&info, 0, sizeof(info));
    memset(&pstSize, 0, sizeof(SIZE_S));
    memset(&vpsAttr, 0, sizeof(VPS_CHN_OUT_ATTR));

    if (NULL == pFormat || channel >= EncCnt || 0 == pFormat->FramesPerSecond)
    {
        PRINT_ERROR("channel %d parameter FramesPerSecond %d error\n", channel, pFormat->FramesPerSecond);
        return -1;
    }

    if (!((pCaptureDevice->EncDevice[channel].SupportStream >> dwType) & 0x01))
    {
        PRINT_ERROR("not support dwType = %d\n",dwType);
        return -1;
    }

    for (i = 0; i < pCaptureDevice->EncDevice[channel].StreamCount; i++)
    {
        if (pCaptureDevice->EncDevice[channel].StreamDevice[i].CapChn == dwType)
        {
                VencChn = i;
        }
    }

    if (CHL_JPEG_T == dwType)
    {
        return 0;
    }

    ret = _get_pic_size(pFormat->ImageSize, &pstSize);
    if (ret)
    {
        PRINT_ERROR("_get_pic_size failed\n");
        return -1;
    }

    if(pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].CapChn == CHL_MAIN_T)
    {
        VideoInput_HandleFormatChange(channel, pstSize);
    }

    if(pVideoInDevice->VDInfo[channel].ViDstFps < pFormat->FramesPerSecond)
    {
        pFormat->FramesPerSecond = pVideoInDevice->VDInfo[channel].ViDstFps;
    }

    ret = _get_pic_type(pFormat, &info);
    if(ret)
    {
        PRINT_ERROR("_get_pic_type failed\n");
        return -1;
    }

    // Fill channel_info structure with parsed parameters
    //info.profile = info.profile;  // Already set by _get_pic_type
    //info.rc_type = info.rc_type;  // Already set by _get_pic_type
    info.bps = pFormat->BitRate;
    //info.enc_type = info.enc_type;  // Already set by _get_pic_type
    info.width = pstSize.u32Width;
    info.height = pstSize.u32Height;
    info.qt_level = pFormat->ImageQuality;
    info.frame_count = pFormat->FramesPerSecond;

    PRINT_INFO("VencChn %d: profile=%d, rc_type=%d, enc_type=%d\n", VencChn, info.profile, info.rc_type, info.enc_type);
    PRINT_INFO("VencChn %d: bps=%d, resolution=%dx%d, qt_level=%d, fps=%d\n",
               VencChn, info.bps, info.width, info.height, info.qt_level, info.frame_count);

    /* Synchronous apply: Stop → SetVPS → SetVENC → Start */

    // Step 1: Stop encoder
    ret = VideoEncoder_Stop(VencChn);
    if (ret != RETURN_OK)
    {
        PRINT_ERROR("Error(%x): VideoEncoder_Stop channel %d failed\n", ret, VencChn);
        return -1;
    }

    // Step 2: Set VPS output parameters (resolution + framerate + codec type)
    vpsAttr.OutWidth = info.width;
    vpsAttr.OutHeight = info.height;
    vpsAttr.OutFps = info.frame_count;
    vpsAttr.EnPayLoad = info.enc_type;

    ret = VideoVPS_SetOutputParam(channel, VencChn, &vpsAttr);
    if (ret != RETURN_OK)
    {
        PRINT_ERROR("Error(%x): VideoVPS_SetOutputParam channel %d failed\n", ret, VencChn);
        VideoEncoder_Start(VencChn);  // Try recover
        return -1;
    }

    // Step 3: Set VENC encoding parameters (codec, bitrate, QP, RC mode, etc.)
    ret = VideoEncoder_UpdateChannelConfig(channel, dwType, &info);
    if (ret != RETURN_OK)
    {
        PRINT_ERROR("Error(%x): VideoEncoder_UpdateChannelConfig channel %d failed\n", ret, VencChn);
        VideoEncoder_Start(VencChn);  // Try recover
        return -1;
    }

    // Step 4: Start encoder
    ret = VideoEncoder_Start(VencChn);
    if (ret != RETURN_OK)
    {
        PRINT_ERROR("Error(%x): VideoEncoder_Start channel %d failed\n", ret, VencChn);
        return -1;
    }

    PRINT_INFO("VideoEncoder_SetFormat: VencChn %d applied successfully\n", VencChn);
    return RETURN_OK;
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
   	}

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

int VideoEncoder_SetRotate(int channel, int Rotate)
{
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
	pCaptureDevice->Rotate = Rotate;
	int ret = 0;
	PlatformAdapter *adapter = GetPlatformAdapter();

    if (!adapter) {
        PRINT_ERROR("Platform adapter not initialized\n");
        return VENC_FAILURE;
    }

    if (channel < 0 || channel >= MAX_VENC_CHN_NUM) {
        PRINT_ERROR("Invalid VENC channel %d\n", channel);
        return VENC_FAILURE;
    }

    if (!adapter->venc_set_rotate) {
        PRINT_ERROR("Rotate interface not available\n");
        return VENC_FAILURE;
    }

	for(int i = CHL_MAIN_T; i < CHL_FUNCTION_NUM; i++)
	{
		for (int ichannel = 0; ichannel < pCaptureDevice->EncDevice[channel].StreamCount; ichannel++)
		{
			if (pCaptureDevice->EncDevice[channel].StreamDevice[ichannel].CapChn == i)
			{
				ret = adapter->venc_set_rotate(ichannel, Rotate);
				if(ret != 0)
				{
					PRINT_ERROR("VideoEncoder_SetRotate failed for channel %d\n", ichannel);
					return ret;
				}
			}
		}
	}

	return ret;
}

/*
 * ===== Additional interfaces =====
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



/**
 * @brief 更新编码通道的完整配置
 * @param channel 视频输入通道 (0-based)
 * @param dwType 码流类型 (CHL_MAIN_T/CHL_2END_T/CHL_JPEG_T)
 * @param info 编码参数配置
 * @return 0成功，-1失败
 */
int VideoEncoder_UpdateChannelConfig(int channel, DWORD dwType, channel_info *info)
{
	unsigned int VencChn;
	BOOL resolutionChanged = FALSE;
	int ret;

	/* Step 1: Validate parameters and find VENC channel */
	if (info == NULL || info->frame_count == 0) {
		PRINT_ERROR("Invalid parameters: info=%p, frame_count=%d\n",
		            info, info ? info->frame_count : 0);
		return -1;
	}

	ret = _find_venc_channel(channel, dwType, &VencChn);
	if (ret != 0) {
		return -1;
	}

	/* Step 2: Apply VENC encoding configuration (core logic) */
	ret = _apply_venc_config(channel, VencChn, info, &resolutionChanged);
	if (ret != RETURN_OK) {
		PRINT_ERROR("Error(%d): _apply_venc_config failed\n", ret);
		return -1;
	}

	/* Step 3: Side effect - Rebuild overlays if resolution changed */
	if (resolutionChanged) {
		_rebuild_overlays(channel, VencChn, info);
	}

	/* Step 4: Side effect - Sync JPEG channel if this is sub stream */
	if (VencChn == CHL_2END_T) {
		ret = _sync_jpeg_channel(channel, VencChn, info);
		if (ret != RETURN_OK) {
			PRINT_ERROR("Error(%d): _sync_jpeg_channel failed\n", ret);
			return -1;
		}
	}

	/* Step 5: Persist configuration */
	_save_channel_config(channel, VencChn, info);

	PRINT_INFO("VideoEncoder_UpdateChannelConfig: channel=%d VencChn=%d completed\n",
	           channel, VencChn);
	return RETURN_OK;
}
