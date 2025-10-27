#include "TdCommon.h"
#include "sdk_comm.h"
#include "logo.h"
#include "mdk_vps.h"
#include "ni_comm_vps.h"
#include "modules/include/video_encoder.h"
#include "modules/include/video_input.h"
#include "modules/include/video_osd.h"
#include "modules/include/video_vps.h"

extern int capture_video_stream_start(void);
extern int capture_video_stream_stop(void);

//编码自适应线程标志位
BOOL VencAdaptiveRunning = FALSE;
//编码自适应线程的pid
pthread_t venc_pid;
unsigned int gFrameRate = 30;
unsigned int sensor_fps = 30;

pthread_mutex_t osd_lock;
pthread_rwlock_t ChangeFrmLock = PTHREAD_RWLOCK_INITIALIZER;
pthread_rwlock_t ChangeParamLock = PTHREAD_RWLOCK_INITIALIZER;

//sensor降帧标志位
BOOL ChangeFrmRate = FALSE;
// 用户修改编码配置标志位
// 这边暂时使用最大通道数为4，后续在枪球上再进行调整
BOOL ChangeChnParam[VENC_MAX_CHN_NUM] = {FALSE};
// 用户修改编码配置后保存的临时配置文件
channel_info ChnParam[VENC_MAX_CHN_NUM] = {0};

extern BOOL ChangeFrmStart;

int FontSiZeTable[] = {64, 32, 16};

static VENC_PROFILE_E H264Profile_Table[]={
										VENC_H264_BASE_LINE_PROFILE,
										VENC_H264_MAIN_PROFILE,
										VENC_H264_HIGH_PROFILE
										};

static VENC_PROFILE_E H265Profile_Table[]={VENC_H265_MAIN_PROFILE};

CaptureImageQuality_t CaptureQtTable[6] =
{
	{
		.ImaxQP = 50,
		.IminQP = 40,
		.PmaxQP = 50,
		.PminQP = 42
	},
	{
		.ImaxQP = 50,
		.IminQP = 36,
		.PmaxQP = 50,
		.PminQP = 38
	},
	{
		.ImaxQP = 44,
		.IminQP = 32,
		.PmaxQP = 48,
		.PminQP = 34
	},
	{
		.ImaxQP = 44,
		.IminQP = 28,
		.PmaxQP = 48,
		.PminQP = 30
	},
	{
		.ImaxQP = 44,
		.IminQP = 20,
		.PmaxQP = 48,
		.PminQP = 20
	},			
	{
		.ImaxQP = 44,
		.IminQP = 10,
		.PmaxQP = 48,
		.PminQP = 10
	},
};

CaptureImageQuality_t CHL_2END_T_CaptureQtTable[6] =
{
	{
		.ImaxQP = 50,
		.IminQP = 20,
		.PmaxQP = 50,
		.PminQP = 20
	},
	{
		.ImaxQP = 50,
		.IminQP = 18,
		.PmaxQP = 50,
		.PminQP = 18
	},
	{
		.ImaxQP = 50,
		.IminQP = 16,
		.PmaxQP = 50,
		.PminQP = 16
	},
	{
		.ImaxQP = 50,
		.IminQP = 14,
		.PmaxQP = 50,
		.PminQP = 14
	},
	{
		.ImaxQP = 50,
		.IminQP = 12,
		.PmaxQP = 50,
		.PminQP = 12
	},			
	{
		.ImaxQP = 50,
		.IminQP = 10,
		.PmaxQP = 50,
		.PminQP = 10
	},
};

SIZE_S imageSize[2][VIDEO_SIZE_NR] =
{
	{ // PAL
		{704, 576},			// D1	
		{704, 288},			// HD1	
		{352, 576},			// BCIF	
		{352, 288},			// CIF	
		{176, 144},			// QCIF	
		{640, 480},			// VGA	
		{320, 240},			// QVGA	
		{480, 480},			// SVCD	
		{160, 128},			// QQVGA
		{800, 592},			// SVGA	
		{1024, 768},		// XVGA	
		{1280, 800},		// WXGA	
		{1280, 1024},		// SXGA	
		{1600, 1024},		// WSXGA
		{1600, 1200},		// UXGA	
		{1920, 1200},		// WUXGA
		{240, 192},			// LTF	
		{1280, 720},		// 720P	
		{1920, 1080},		// 1080P
		{1280, 960},		// HD960
		{960, 576},			// 960H
		{1280, 1024},		// HD1024
		{960, 1080},		// HALF1080
		{2048, 1536},		// QXGA
		{3840, 2160},		// UHD4K
		{2688, 1520},		// 4M
		{2880, 1620},		// 5M
		{1280, 1440},		// HALF4M
		{4000, 3000},		// 12M
		{2560, 1440},		// 4MQHD
		{1920, 1536},		// 3M
		{1440, 900},		// 1440*900
		{640, 368},			// NHD
		{2304, 1296},		// 1296P
		{960, 544},			// 540P
	},						
	{ // NTSC
		{704, 480},			// D1	
		{704, 240},			// HD1	
		{352, 480},			// BCIF	
		{352, 240},			// CIF	
		{176, 120},			// QCIF	
		{640, 480},			// VGA	
		{320, 240},			// QVGA	
		{480, 480},			// SVCD	
		{160, 128},			// QQVGA
		{800, 592},			// SVGA	
		{1024, 768},		// XVGA	
		{1280, 800},		// WXGA	
		{1280, 1024},		// SXGA	
		{1600, 1024},		// WSXGA
		{1600, 1200},		// UXGA	
		{1920, 1200},		// WUXGA
		{240, 192},			// LTF	
		{1280, 720},		// 720P	
		{1920, 1080},		// 1080P
		{1280, 960},		// HD960
		{960, 480},			// 960H
		{1280, 1024},		// HD1024
		{960, 1080},		// HALF1080
		{2048, 1536},		// QXGA
		{3840, 2160},		// UHD4K
		{2688, 1520},		// 4M
		{2880, 1620},		// 5M
		{1280, 1440},		// HALF4M
		{4000, 3000},		// 12M
		{2560, 1440},		// 4MQHD
		{1920, 1536},		// 3M
		{1440, 900},		// 1440*900
		{640, 368},			// NHD
		{2304, 1296},		// 1296P
		{960, 544},			// 540P
	}
};



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

	pstSize->u32Width = imageSize[vstd][enPicSize].u32Width;
	pstSize->u32Height = imageSize[vstd][enPicSize].u32Height;
	if(pstSize->u32Width == 0 && pstSize->u32Height == 0)
	{
		return FAILED;
	}
	return SUCCESS;
}

int _get_pic_type(VIDEO_FORMAT * pFormat,struct channel_info *info)
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

/// 设置VPS输出的帧率和分辨率
///
/// \param [in] channel 通道号。
/// \param [in] dwType 编码通道的掩码
/// \param [in] ChnAttr 修改VPS输出帧率和分辨率的结构体。
/// \retval 0  设置成功
/// \retval 0  设置失败
int _video_set_vps_out_param(int channel, DWORD dwType, VPS_CHN_OUT_ATTR *ChnAttr)
{
	int EncCnt = 0;
	int ret = FAILED;
	VPS_GRP VpsGrp = 0;
	unsigned int VencChn = 0;
	VPS_GRP_ATTR_S VPS_GrpAttr;
	VPS_CHN_ATTR_S VPS_ChnAttr;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

	memset(&VPS_GrpAttr,0,sizeof(VPS_GRP_ATTR_S));
	memset(&VPS_ChnAttr,0,sizeof(VPS_CHN_ATTR_S));
	
	//视频输入的数量
	EncCnt = pCaptureDevice->EncCount;

	if (NULL == ChnAttr || channel >= EncCnt)
	{
		PRINT_ERROR("channel %d parameter error\n", channel);
		return ret;
	}

	if(0 == ChnAttr->OutFps)
	{
		PRINT_ERROR("channel %d parameter OutFps %d error\n", channel, ChnAttr->OutFps);
		return ret;
	}

	//校验支持的码流的通道，上层传递通道方式为掩码
	if (!((pCaptureDevice->EncDevice[channel].SupportStream >> dwType) & 0x01))
	{
		PRINT_ERROR("not support dwType = %d\n", dwType);
		return ret;
	}

	//获取编码通道号
	for (int i = 0; i < pCaptureDevice->EncDevice[channel].StreamCount; i++)
	{
		if (dwType == pCaptureDevice->EncDevice[channel].StreamDevice[i].CapChn)
		{
			VencChn = i;
		}
	}

	ret = NI_MDK_VPS_DisableChn(VpsGrp, VencChn);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error(%d - %x): NI_MDK_VPS_DisableChn\n", ret, ret);
		return ret;
	}

	// 当VPS通道分辨率改变时，OSD和LOGO坐标需要根据分辨率来调整，所以这边先将OSD和LOGO销毁
	// 由于设计到分辨率配置的问题，销毁后需等到编码修改完成后再创建
	if (ChnAttr->OutHeight != pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.max_height ||
		ChnAttr->OutWidth != pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.max_width)
	{
		VideoOSD_Destroy();
	}
	
	ret = NI_MDK_VPS_GetChnAttr(VpsGrp, VencChn, &VPS_ChnAttr);
	if (ret != RETURN_OK)
	{
		pthread_mutex_unlock(&osd_lock);
		PRINT_ERROR("Error(%d - %x): NI_MDK_VPS_GetChnAttr\n", ret, ret);
		return ret;
	}

	VPS_ChnAttr.u32OutHeight = ChnAttr->OutHeight;
	VPS_ChnAttr.u32OutWidth = ChnAttr->OutWidth;
	VPS_ChnAttr.stEncAttr.u32YStride = VPS_ChnAttr.u32OutWidth;
	VPS_ChnAttr.stEncAttr.u32CStride = VPS_ChnAttr.u32OutWidth;
	VPS_ChnAttr.stEncAttr.enType = ChnAttr->EnPayLoad;

	// 这边判断下sensor帧率和用户配置帧率的大小
	// 若sensor帧率大于用户配置帧率，则以用户配置帧率为准
	// 若sensor帧率小于用户配置帧率，则以sensor帧率为准
	VPS_ChnAttr.stEncAttr.u32OutFps = (sensor_fps > ChnAttr->OutFps) ? ChnAttr->OutFps : sensor_fps;

	// 为保证降帧后帧率为整数，输出帧率的分母值为2，所以这边的输出帧率需要*2
	if (12 == sensor_fps)
	{
		VPS_ChnAttr.stEncAttr.u32OutFps *= 2;
	}
	
	ret = NI_MDK_VPS_SetChnAttr(VpsGrp, VencChn, &VPS_ChnAttr);
	if (ret != RETURN_OK)
	{
		pthread_mutex_unlock(&osd_lock);
		PRINT_ERROR("Error(%d - %x): NI_MDK_VPS_SetChnAttr\n", ret, ret);
		return ret;
	}

	ret = NI_MDK_VPS_EnableChn(VpsGrp, VencChn);
	if (ret != RETURN_OK)
	{
		pthread_mutex_unlock(&osd_lock);
		PRINT_ERROR("Error(%d - %x): NI_MDK_VPS_EnableChn\n", ret, ret);
		return ret;
	}

	return ret;
}


/// 设置VPS输入帧率
///	
/// \param [in] channel 通道号。
/// \param [in] FrmRate 输入的帧率。
/// \retval 0  设置成功
/// \retval 0  设置失败
int _video_set_vps_in_frame_rate(int channel, unsigned int FrmRate)
{
	int ret = FAILED;
	VPS_GRP VpsGrp = 0;
	VPS_GRP_ATTR_S stGrpAttr;
	VPS_MOD_PARAM_S stGrpModParam;

	memset(&stGrpAttr, 0, sizeof(VPS_GRP_ATTR_S));
	memset(&stGrpModParam, 0, sizeof(VPS_MOD_PARAM_S));

	if (0 == FrmRate)
	{
		PRINT_ERROR("_video_set_vps_in_frame_rate FrmRate:%d error!\n", FrmRate);
		return ret;
	}

	ret = NI_MDK_VPS_GetModParam(VpsGrp, &stGrpModParam);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_VPS_GetGrpAttr failed with %#x!\n", ret);
		return ret;
	}

	if (12 == FrmRate)
	{
		// 在PAL制下，sensor帧率为25帧，降帧后帧率为12.5
		//为保证降帧后帧率为整数12，这边将输出帧率的分母值配为2，输出帧率的分子值设置为24
		stGrpModParam.u32FpsDenom = 2;
	}
	else
	{
		stGrpModParam.u32FpsDenom = 1;
	}

	ret = NI_MDK_VPS_SetModParam(VpsGrp, &stGrpModParam);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_VPS_SetModParam failed with %#x!\n", ret);
		return ret;
	}

	ret = NI_MDK_VPS_GetGrpAttr(VpsGrp, &stGrpAttr);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_VPS_GetGrpAttr failed with %#x!\n", ret);
		return ret;
	}

	if (12 == FrmRate)
	{
		stGrpAttr.u32InFps = 25;
	}
	else
	{
		stGrpAttr.u32InFps = FrmRate;
	}

	ret = NI_MDK_VPS_SetGrpAttr(VpsGrp, &stGrpAttr);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_VPS_SetGrpAttr failed with %#x!\n", ret);
		return ret;
	}

	return ret;
}

/// 编码自适应线程
///	用于维护用户编码参数修改以及夜晚降帧策略
/// 该线程仅在升级时销毁
/// 
void *_venc_adaptive_process()
{
	int channel = 0;
	int ret = FAILED;
	channel_info info;
	VPS_CHN_OUT_ATTR ChnAttr;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

	memset(&ChnAttr, 0, sizeof(VPS_CHN_OUT_ATTR));

	while (VencAdaptiveRunning)
	{
		sensor_fps = gFrameRate;

		//当设备降帧时
		if(TRUE == ChangeFrmRate)
		{
			pthread_rwlock_wrlock(&ChangeFrmLock);
			ChangeFrmRate = FALSE;
			pthread_rwlock_unlock(&ChangeFrmLock);

			ret = _video_set_vps_in_frame_rate(channel, sensor_fps);
			if (ret != RETURN_OK)
			{
				PRINT_ERROR("Error(%x): _video_set_vps_in_frame_rate!\n", ret);
				continue;
			}

			for (int ichannel = 0; ichannel < pCaptureDevice->EncDevice[channel].StreamCount; ichannel++)
			{
				// 不设置编码通道
				if (STREAM_TYPE_THI == ichannel)
				{
					break;
				}

				ret = VideoEncoder_Stop(ichannel);
				if (ret != RETURN_OK)
				{
					PRINT_ERROR("Error(%x): VideoEncoder_Stop chn(%d)!\n", ret, ichannel);
					ret = VideoEncoder_Start(ichannel);
					if (ret != RETURN_OK)
					{
						PRINT_ERROR("Error(%x): VideoEncoder_Start chn(%d)!\n", ret, ichannel);
					}
					continue;
				}

				ChnAttr.OutWidth = pCaptureDevice->EncDevice[channel].StreamDevice[ichannel].EncChannel_info.max_width;
				ChnAttr.OutHeight = pCaptureDevice->EncDevice[channel].StreamDevice[ichannel].EncChannel_info.max_height;
				//这边每次都是用临时配置的值传参，因为后面编码设置后，配置会保存，这样就会导致用户配置丢失
				ChnAttr.OutFps = ChnParam[ichannel].frame_count;
				ChnAttr.EnPayLoad = ChnParam[ichannel].enc_type; 
				PRINT_INFO("ichannel %d, ChnAttr.OutWidth %d\n", ichannel,ChnAttr.OutWidth);
				PRINT_INFO("ichannel %d, ChnAttr.OutHeight %d\n",  ichannel,ChnAttr.OutHeight);
				PRINT_INFO("ichannel %d, ChnAttr.OutFps %d\n", ichannel, ChnAttr.OutFps);
				PRINT_INFO("ichannel %d, ChnAttr.EnPayLoad %d\n", ichannel, ChnAttr.EnPayLoad);
				ret = _video_set_vps_out_param(channel, ichannel, &ChnAttr);
				if (ret != RETURN_OK)
				{
					PRINT_ERROR("Error(%x): _video_set_vps_in_frame_rate!\n", ret);
					ret = VideoEncoder_Start(ichannel);
					if (ret != RETURN_OK)
					{
						PRINT_ERROR("Error(%x): VideoEncoder_Start chn(%d)!\n", ret, ichannel);
					}
					continue;
				}
				
				memset(&info, 0, sizeof(channel_info));

				info.width = ChnAttr.OutWidth;
				info.height = ChnAttr.OutHeight;
				info.frame_count = ChnAttr.OutFps;
				info.bps = pCaptureDevice->EncDevice[channel].StreamDevice[ichannel].EncChannel_info.bps;
				info.rc_type = pCaptureDevice->EncDevice[channel].StreamDevice[ichannel].EncChannel_info.rc_type;
				info.profile = pCaptureDevice->EncDevice[channel].StreamDevice[ichannel].EncChannel_info.profile;
				info.qt_level = pCaptureDevice->EncDevice[channel].StreamDevice[ichannel].EncChannel_info.qt_level;
				info.enc_type = pCaptureDevice->EncDevice[channel].StreamDevice[ichannel].EncChannel_info.enc_type;
				PRINT_INFO("ichannel %d, info.profile %d\n", ichannel, info.profile);
				PRINT_INFO("ichannel %d, info.rc_type %d\n",  ichannel,info.rc_type);
				PRINT_INFO("ichannel %d, info.enc_type %d\n", ichannel, info.enc_type);
				PRINT_INFO("ichannel %d, info.bps %d\n", ichannel, info.bps);
				PRINT_INFO("ichannel %d, info.width %d\n",  ichannel,info.width);
				PRINT_INFO("ichannel %d, info.height %d\n",  ichannel,info.height);
				PRINT_INFO("ichannel %d, info.qt_level%d\n",  ichannel,info.qt_level);
				PRINT_INFO("ichannel %d, info.frame_count %d\n", ichannel, info.frame_count);
				ret = VideoEncoder_UpdateChannelConfig(channel, ichannel, &info);
				if (ret != RETURN_OK)
				{
					PRINT_ERROR("Error(%x): _video_set_vps_in_frame_rate!\n", ret);
					ret = VideoEncoder_Start(ichannel);
					if (ret != RETURN_OK)
					{
						PRINT_ERROR("Error(%x): VideoEncoder_Start chn(%d)!\n", ret, ichannel);
					}
					continue;
				}

				ret = VideoEncoder_Start(ichannel);
				if (ret != RETURN_OK)
				{
					PRINT_ERROR("Error(%x): VideoEncoder_Start chn(%d)!\n", ret, ichannel);
					continue;
				}
			}
		}

		//当用户修改配置时
		for (int ichannel = 0; ichannel < pCaptureDevice->EncDevice[channel].StreamCount; ichannel++)
		{
			//不设置编码通道
			if (STREAM_TYPE_THI == ichannel)
			{
				break;
			}

			if(TRUE == ChangeChnParam[ichannel])
			{
				pthread_rwlock_wrlock(&ChangeParamLock);
				ChangeChnParam[ichannel] = FALSE;
				memset(&info, 0, sizeof(channel_info));
				memcpy(&info, &ChnParam[ichannel], sizeof(channel_info));
				pthread_rwlock_unlock(&ChangeParamLock);
				PRINT_INFO("ichannel %d, info.profile %d\n", ichannel, info.profile);
				PRINT_INFO("ichannel %d, info.rc_type %d\n", ichannel,info.rc_type);
				PRINT_INFO("ichannel %d, info.enc_type %d\n", ichannel, info.enc_type);
				PRINT_INFO("ichannel %d, info.bps %d\n", ichannel,info.bps);
				PRINT_INFO("ichannel %d, info.width %d\n", ichannel,info.width);
				PRINT_INFO("ichannel %d, info.height %d\n", ichannel,info.height);
				PRINT_INFO("ichannel %d, info.qt_level%d\n",ichannel, info.qt_level);
				PRINT_INFO("ichannel %d, info.frame_count %d\n", ichannel, info.frame_count);
				ret = VideoEncoder_Stop(ichannel);
				if (ret != RETURN_OK)
				{
					PRINT_ERROR("Error(%x): VideoEncoder_Stop chn(%d)!\n", ret, ichannel);
					continue;
				}

				ChnAttr.OutWidth = info.width;
				ChnAttr.OutHeight = info.height;
				ChnAttr.OutFps = info.frame_count;
				ChnAttr.EnPayLoad = info.enc_type;
				ret = _video_set_vps_out_param(channel, ichannel, &ChnAttr);
				if (ret != RETURN_OK)
				{
					PRINT_ERROR("Error(%x): _video_set_vps_out_param!\n", ret);
					ret = VideoEncoder_Start(ichannel);
					if (ret != RETURN_OK)
					{
						PRINT_ERROR("Error(%x): VideoEncoder_Start chn(%d)!\n", ret, ichannel);
					}
					continue;
				}
				
				ret = VideoEncoder_UpdateChannelConfig(channel, ichannel, &info);
				if (ret != RETURN_OK)
				{
					PRINT_ERROR("Error(%x): VideoEncoder_UpdateChannelConfig!\n", ret);
					ret = VideoEncoder_Start(ichannel);
					if (ret != RETURN_OK)
					{
						PRINT_ERROR("Error(%x): VideoEncoder_Start chn(%d)!\n", ret, ichannel);
					}
					continue;
				}

				ret = VideoEncoder_Start(ichannel);
				if (ret != RETURN_OK)
				{
					PRINT_ERROR("Error(%x): VideoEncoder_Start chn(%d)!\n", ret, ichannel);
					continue;
				}
			}
		}
		
		usleep(30*1000);
	}

	return NULL;
}

extern void _set_aisp_run(BOOL run);

	
#ifdef DEBUG_AVBR
int VideoSetAVBRAttr(int iChannel, AVBR_Attr* pAVBR)
{
	AVBR_Attr *pAvbrAttr = &GlobalDevice.avbr_attr;
	pAvbrAttr->InitQp = pAVBR->InitQp;
	pAvbrAttr->IminQP = pAVBR->IminQP;
	pAvbrAttr->ImaxQP = pAVBR->ImaxQP;
	pAvbrAttr->PminQP = pAVBR->PminQP;
	pAvbrAttr->PmaxQP = pAVBR->PmaxQP;
	pAvbrAttr->MaxratePercent = pAVBR->MaxratePercent;
	pAvbrAttr->IFrmMaxBits = pAVBR->IFrmMaxBits;
	pAvbrAttr->IPQPDelta = pAVBR->IPQPDelta;
	pAvbrAttr->IBitProp = pAVBR->IBitProp;
	pAvbrAttr->PBitProp = pAVBR->PBitProp;
	pAvbrAttr->FluctuateLevel = pAVBR->FluctuateLevel;
	pAvbrAttr->StillratePercent = pAVBR->StillratePercent;
	pAvbrAttr->Maxstillqp = pAVBR->Maxstillqp;
}
#endif

int VideoGetEncChipCapability(VIDEO_ENCCHIPCAPABILITY *pEncChipCapability)
{
	PRINT_TRACE(">>>>>>>>>>>>	%s	 %d \n", __FUNCTION__, __LINE__);
	int i, j;
	const CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
	//VideoInDevice_p pVideoInDevice = &GlobalDevice.VideoInDevice; 
//	SystemDevice_p pSystemDevice = &GlobalDevice.SystemDevice;
	int EncCount;
	
	/*if(pSystemDevice->ZeroChannel == ENABLE)	

	{
		EncCount = pCaptureDevice->EncCount - 1;
	}
	else*/
	{
		EncCount =	pCaptureDevice->EncCount;
	}
	//PRINT_TRACE("VideoGetEncChipCapability==>1\n");
	pEncChipCapability->EncChipNR =  EncCount;
	for (i = 0; i < EncCount; i++)
	{
		for (j = 0; j < pCaptureDevice->EncDevice[i].StreamCount; j++)
		{
			if (pCaptureDevice->EncDevice[i].StreamDevice[j].CapChn == CHL_MAIN_T)
			{	
				if((pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask >> CAPTURE_POWER_4M_QHD_30) & 0x01)
				{
					pEncChipCapability->EncChipInfo[i].nMaxEncodePower = DEF_ENC_POWER_4M_QHD_30+DEF_ENC_POWER_D1_30;// DEF_ENC_POWER_CIF;
				}
				else if((pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask >> CAPTURE_POWER_UHD4K_15) & 0x01)
				{
					pEncChipCapability->EncChipInfo[i].nMaxEncodePower = DEF_ENC_POWER_UHD4K_15+DEF_ENC_POWER_D1_15;// DEF_ENC_POWER_CIF;
				}
				else if((pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask >> CAPTURE_POWER_HD1080_30) & 0x01)
				{
					pEncChipCapability->EncChipInfo[i].nMaxEncodePower = DEF_ENC_POWER_HD1080+DEF_ENC_POWER_D1;// DEF_ENC_POWER_CIF;
				}
				else if((pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask >> CAPTURE_POWER_4M_15) & 0x01)
				{
					pEncChipCapability->EncChipInfo[i].nMaxEncodePower = DEF_ENC_POWER_4M_15+ DEF_ENC_POWER_CIF;
				}
				else if((pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask >> CAPTURE_POWER_5M_12) & 0x01)
				{
					pEncChipCapability->EncChipInfo[i].nMaxEncodePower = DEF_ENC_POWER_5M_12+ DEF_ENC_POWER_CIF;
				}
				else if((pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask >> CAPTURE_POWER_QXGA_18) & 0x01)
				{
					pEncChipCapability->EncChipInfo[i].nMaxEncodePower = DEF_ENC_POWER_QXGA_18+ DEF_ENC_POWER_CIF;
				}
				else if((pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask >> CAPTURE_POWER_3M_18) & 0x01)
				{
					pEncChipCapability->EncChipInfo[i].nMaxEncodePower = DEF_ENC_POWER_3M_18+ DEF_ENC_POWER_CIF;
				}				
				else if((pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask >> CAPTURE_POWER_HD1080_15) & 0x01)
				{
					pEncChipCapability->EncChipInfo[i].nMaxEncodePower = DEF_ENC_POWER_HD1080_15+ DEF_ENC_POWER_CIF;
				}
				else if((pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask >> CAPTURE_POWER_HALF_HD1080) & 0x01)
				{
					pEncChipCapability->EncChipInfo[i].nMaxEncodePower = DEF_ENC_POWER_HALF_HD1080 + DEF_ENC_POWER_CIF;
				}
				else	if((pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask >> CAPTURE_POWER_HD720_30) & 0x01)
				{
					pEncChipCapability->EncChipInfo[i].nMaxEncodePower = DEF_ENC_POWER_HD720+ DEF_ENC_POWER_CIF;
				}
				else if((pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask >> CAPTURE_POWER_HALF_HD1080_15) & 0x01)
				{
					pEncChipCapability->EncChipInfo[i].nMaxEncodePower = DEF_ENC_POWER_HALF_HD1080_15 + DEF_ENC_POWER_CIF;
				}
				else if((pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask >> CAPTURE_POWER_HD720_15) & 0x01)
				{
					pEncChipCapability->EncChipInfo[i].nMaxEncodePower = DEF_ENC_POWER_HD720_15+ DEF_ENC_POWER_CIF;
				}
				else if((((pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask >> CAPTURE_POWER_960H_25) & 0x01)
				   || ((pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask >> CAPTURE_POWER_960H_6) & 0x01)))
				{
					pEncChipCapability->EncChipInfo[i].nMaxEncodePower = DEF_ENC_POWER_960H+ DEF_ENC_POWER_CIF;
				}
				else if ((((pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask >> CAPTURE_POWER_D1_25) & 0x01)
				   || ((pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask >> CAPTURE_POWER_D1_6) & 0x01)))
				{
					pEncChipCapability->EncChipInfo[i].nMaxEncodePower = DEF_ENC_POWER_D1 + DEF_ENC_POWER_QCIF;
				}
				else
				{
					pEncChipCapability->EncChipInfo[i].nMaxEncodePower = DEF_ENC_POWER_CIF + DEF_ENC_POWER_QCIF;
				}

				pEncChipCapability->EncChipInfo[i].nMaxSupportChannel = DEF_SUPPORT_CHL;
				pEncChipCapability->EncChipInfo[i].bChannelMaxSetSync = 0;
				pEncChipCapability->EncChipInfo[i].nExpandChannel = 0;		
				break;
			}
		}

		if (j == pCaptureDevice->EncDevice[i].StreamCount)
		{
			PRINT_TRACE("找不到通道%d, 的最大能力，使用默认值CIF\n", i);
			pEncChipCapability->EncChipInfo[i].nMaxEncodePower = DEF_ENC_POWER_CIF + DEF_ENC_POWER_QCIF;
			pEncChipCapability->EncChipInfo[i].nMaxSupportChannel = DEF_SUPPORT_CHL;
			pEncChipCapability->EncChipInfo[i].bChannelMaxSetSync = 0;
			pEncChipCapability->EncChipInfo[i].nExpandChannel = 0;		
		}
	}
	
//	PRINT_TRACE("g_sysInfor.vencChanne = %d\n", g_sysInfor.vencChannel);
	return 0;
}

//辅码流能力获取
int VideoGetExtCaps(VIDEO_EXT_STREAM *pStream)
{
	int EncCnt,i,j;
	int FirCap = 0,SceCap = 0;
	//VideoInDevice_p pVideoInDevice = &GlobalDevice.VideoInDevice;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
	
	EncCnt = pCaptureDevice->EncCount;
	pStream->ExtraStream = pCaptureDevice->EncDevice[0].SupportStream;
	for(i = 0;i < EncCnt;i++)
	{
		for(j = 0;j < pCaptureDevice->EncDevice[i].StreamCount;j++)
		{
			if(pCaptureDevice->EncDevice[i].StreamDevice[j].CapChn == CHL_MAIN_T)
			{
				FirCap = pCaptureDevice->EncDevice[i].StreamDevice[j].Format.ImageSize;
			}

			if(pCaptureDevice->EncDevice[i].StreamDevice[j].CapChn == CHL_2END_T)
			{
				SceCap = pCaptureDevice->EncDevice[i].StreamDevice[j].Format.ImageSize;
			}
		}
	}

	pStream->CaptureSizeMask[FirCap]  =  (1 << SceCap);

	return 0;
}

/// 得到支持的特性。
/// 
/// \param [out] pCaps 指向捕获特性结构VIDEO_CAPS的指针。
/// \retval 0  获取成功。
/// \retval 0  获取失败。
int VideoGetCaps(VIDEO_CAPS * pCaps)
{
	//PRINT_TRACE(">>>>>>>>>>>>	%s	 %d \n", __FUNCTION__, __LINE__);
	//以下没有做接口封装
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
//	SystemDevice_p pSystemDevice = &GlobalDevice.SystemDevice;
	int i;

	memset(pCaps, 0, sizeof(VIDEO_CAPS)); 

	//CAPTURE_POWER_CIF_6
	for(i=0;i<pCaptureDevice->EncCount;i++)
	{
		memcpy(&pCaps->ChnCap[i], &pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].Caps, sizeof(VIDEO_CHN_CAP));

		pCaps->ChnCap[i].Compression = (1 << VIDEO_COMP_H264) | (1 << VIDEO_COMP_H265);
		pCaps->ChnCap[i].BitRateControl = (1 << VIDEO_BRC_CBR) | (1 << VIDEO_BRC_VBR);
		pCaps->ChnCap[i].Media = (1 << CAPTURE_START_VIDEO) | (1 << CAPTURE_START_AUDIO);
		pCaps->ChnCap[i].CoverBlocks = VPS_MASK_MAX_CHN_NUM;
		pCaps->ChnCap[i].TitleCount = TD_OSD_MAX_NUM;//包括时间标题与通道标题
		pCaps->ChnCap[i].WaterMask = 0;//暂时不用水印
		pCaps->ChnCap[i].RegionCover = (VPS_MASK_MAX_CHN_NUM > 0 ? 1 : 0);

		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_HD1080_30) & 0x01
			|| (pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_HD1080_15) & 0x01)
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_1080);
		}
		if (((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_HD720_30) & 0x01)
			|| ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_HD720_15) & 0x01))
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_720);
		}
		if (((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_960H_25) & 0x01)
			|| ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_960H_6) & 0x01))
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_960H);
		}
		if (((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_D1_25) & 0x01)
			|| ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_D1_6) & 0x01))
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_D1);
		}
		if (((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_CIF_25) & 0x01)
			|| ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_CIF_6) & 0x01))
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_CIF);
		}
		if (((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_QCIF_25) & 0x01)
			|| ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_QCIF_6) & 0x01))
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_QCIF);
		}
		if (((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_BCIF_25) & 0x01)
			|| ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_BCIF_6) & 0x01))
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_BCIF);
		}
		if (((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_HALF_HD1080) & 0x01)
			|| ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_HALF_HD1080_15) & 0x01))
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_HALF1080);
		}						
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_QXGA_18) & 0x01)
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_QXGA);
		}
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_3M_18) & 0x01)
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_3M);
		}		
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_4M_15) & 0x01)
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_4M);
		}
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_5M_12) & 0x01)
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_5M);
		}
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_half4M_12) & 0x01)
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_HALF4M);
		}
		if (((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_4M_QHD_12) & 0x01)
			|| ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_4M_QHD_30) & 0x01))
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_4M_QHD);
		}
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_NHD_30) & 0x01)
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_NHD);
		}
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_1296P_30) & 0x01)
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_1296P);
		}
		//if()
		//pCaps->ChnCap[i].JpegImageSize |= (1 << VIDEO_SIZE_CIF);
		//pCaps->ChnCap[i].JpegImageSize |= (1 << VIDEO_SIZE_D1);
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_HD1080_30) & 0x01)
		{
			pCaps->ChnCap[i].JpegImageSize |= (1 << VIDEO_SIZE_1080);
		}
		if (((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_HD720_30) & 0x01)
			|| ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_HD720_15) & 0x01))
		{
			pCaps->ChnCap[i].JpegImageSize |= (1 << VIDEO_SIZE_720);
		}
		if (((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_960H_25) & 0x01)
			|| ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_960H_6) & 0x01))
		{
			pCaps->ChnCap[i].JpegImageSize |= (1 << VIDEO_SIZE_960H);
		}
		if (((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_D1_25) & 0x01)
			|| ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_D1_6) & 0x01))
		{
			pCaps->ChnCap[i].JpegImageSize |= (1 << VIDEO_SIZE_D1);
		}
		if (((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_CIF_25) & 0x01)
			|| ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_CIF_6) & 0x01))
		{
			pCaps->ChnCap[i].JpegImageSize |= (1 << VIDEO_SIZE_CIF);
		}
		if (((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_QCIF_25) & 0x01)
			|| ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_QCIF_6) & 0x01))
		{
			pCaps->ChnCap[i].JpegImageSize|= (1 << VIDEO_SIZE_QCIF);
		}
		if (((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_BCIF_25) & 0x01)
			|| ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_BCIF_6) & 0x01))
		{
			pCaps->ChnCap[i].JpegImageSize |= (1 << VIDEO_SIZE_BCIF);
		}
		if (((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_HALF_HD1080) & 0x01)
			|| ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_HALF_HD1080_15) & 0x01))
		{
			pCaps->ChnCap[i].JpegImageSize |= (1 << VIDEO_SIZE_HALF1080);
		}	
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_QXGA_18) & 0x01)
		{
			pCaps->ChnCap[i].JpegImageSize |= (1 << VIDEO_SIZE_QXGA);
		}
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_3M_18) & 0x01)
		{
			pCaps->ChnCap[i].JpegImageSize |= (1 << VIDEO_SIZE_3M);
		}		
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_5M_12) & 0x01)
		{
			pCaps->ChnCap[i].JpegImageSize |= (1 << VIDEO_SIZE_5M);
		}
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_4M_15) & 0x01)
		{
			pCaps->ChnCap[i].JpegImageSize |= (1 << VIDEO_SIZE_4M);
		}
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_half4M_12) & 0x01)
		{
			pCaps->ChnCap[i].JpegImageSize |= (1 << VIDEO_SIZE_HALF4M);
		}
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_4M_QHD_12) & 0x01)
		{
			pCaps->ChnCap[i].JpegImageSize |= (1 << VIDEO_SIZE_4M_QHD);
		}
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_NHD_30) & 0x01)
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_NHD);
		}
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_1296P_30) & 0x01)
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_1296P);
		}
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_QVGA_25) & 0x01)
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_QVGA);
		}

		if (((pCaptureDevice->EncDevice[0].SupportStream >> CHL_JPEG_T)&0x1) == 0)
		{
			//PRINT_TRACE("找不到默认的JPEG编码压缩能力，强制设置为CIF\n");
			pCaps->ChnCap[i].JpegImageSize |= (1 << VIDEO_SIZE_CIF);
		}
	}	
	return 0;
}

/// 得到设备支持的最大分辨率与帧率
/// 
/// \param [out] pCaps 指向捕获特性结构VIDEO_MAX_SIZE的指针。
/// \retval 0  获取成功。
/// \retval 0  获取失败。
int VideoGetMaxSize(VIDEO_MAX_SIZE *pCaps)
{
	SensorDevice_p pSensorDevice = &GlobalDevice.SensorDevice;
	pCaps->maxWidth = pSensorDevice->MaxWidth;
	pCaps->maxHeight = pSensorDevice->MaxHeight;
	pCaps->maxFPS = pSensorDevice->MaxFPS;
	return 0;
}

int VideoCreate(int index)
{
	return 0;
}

int VideoDestroy(int index)
{
	return 0;
}

// 创建音频编码通道
// 仅格式化SD卡使用
int VideoVencCreate(void)
{
	return 0;
}

// 销毁音频编码通道
// 仅格式化SD卡使用
int VideoVencDestroy(void)
{
	return 0;
}

/// 设置OSD字体大小。
///
/// \param [in] channel 通道号。
/// \param [in] size 字体大小
/// \retval 0  设置成功。
int VideoSetTitleSize(int channel,unsigned int size)
{
	return VideoOSD_SetTitleSize(channel, size);
}

/// 设置捕获数据压缩的格式。
/// 
/// \param [in] channel 通道号。
/// \param [in] dwType
/// \param [in] pFormat 指向捕获格式结构CAPTURE_FORMAT的指针。
/// \retval 0  设置成功
/// \retval 0  设置失败
int VideoSetFormat(int channel, DWORD dwType, VIDEO_FORMAT * pFormat)
{
    int i = 0;        
    int ret = -1;
    int EncCnt = 0;
    struct channel_info info;
    SIZE_S pstSize;
    unsigned int VencChn = 0;
    VPS_CHN_ATTR_S VPS_ChnAttr;
    VENC_CHN_ATTR_S VENC_ChnAttr;
    CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
    VideoInDevice_p pVideoInDevice = &GlobalDevice.VideoInDevice;

    EncCnt = pCaptureDevice->EncCount;
    memset(&info, 0, sizeof(info));
    memset(&pstSize, 0, sizeof(SIZE_S));
    memset(&VPS_ChnAttr, 0, sizeof(VPS_CHN_ATTR_S));
    memset(&VENC_ChnAttr, 0, sizeof(VENC_CHN_ATTR_S));

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

    ChnParam[VencChn].profile = info.profile;
    ChnParam[VencChn].rc_type = info.rc_type;
    ChnParam[VencChn].bps = pFormat->BitRate;
    ChnParam[VencChn].enc_type = info.enc_type;
    ChnParam[VencChn].width = pstSize.u32Width;
    ChnParam[VencChn].height = pstSize.u32Height;
    ChnParam[VencChn].qt_level = pFormat->ImageQuality;
    ChnParam[VencChn].frame_count = pFormat->FramesPerSecond;
    PRINT_INFO("info.profile %d\n" ,info.profile);
    PRINT_INFO("info.rc_type %d\n", info.rc_type);
    PRINT_INFO("info.enc_type %d\n" ,info.enc_type);
    PRINT_INFO("pFormat->BitRate %d\n" ,pFormat->BitRate);
    PRINT_INFO("pstSize.u32Width %d\n", pstSize.u32Width);
    PRINT_INFO("pstSize.u32Height %d\n" ,pstSize.u32Height);
    PRINT_INFO("pFormat->ImageQuality %d\n", pFormat->ImageQuality);
    PRINT_INFO("pFormat->FramesPerSecond %d\n", pFormat->FramesPerSecond);

    pthread_rwlock_wrlock(&ChangeParamLock);
    ChangeChnParam[VencChn] = TRUE;
    pthread_rwlock_unlock(&ChangeParamLock);

    return ret;
}

/// 设置标题叠加。
///
/// \param [in] channel 通道号。
/// \param [in] pParam 指向标题叠加结构VIDEO_TITLE_PARAM的指针。
/// \retval 0  设置成功。
/// \retval <0  设置失败。
int VideoSetTitle(int channel, VIDEO_TITLE_PARAM *pParam)
{
	/* Wrapper function - forward to video_osd module */
	return VideoOSD_SetTitle(channel, pParam);
}

/// 设置logo叠加。
///
/// \param [in] channel 通道号。
/// \param [in] pParam 指向标题叠加结构VIDEO_TITLE_PARAM的指针。
/// \retval 0  设置成功。
/// \retval !0  设置失败。
int VideoSetLogo(int channel, VIDEO_TITLE_PARAM *pParam)
{
	/* Wrapper function - forward to video_osd module */
	return VideoOSD_SetLogo(channel, pParam);
}

/// 设置覆盖区域。
/// 
/// \param [in] channel 通道号。
/// \param [in] pParam 指向视频覆盖结构VIDEO_COVER_PARAM的指针。
/// \retval 0  设置成功。
/// \retval <0  设置失败。
int VideoSetCover(int channel,int index, VIDEO_COVER_PARAM *pParam)
{
	return VideoVPS_SetCover(channel, index, pParam);
}

/// 获取上一帧的qp。
/// 
/// \param [in] channel 通道号。
/// \param [in] dwType
/// \param [in] qp 返回上一帧的qp。
/// \retval 0  设置成功
/// \retval 0  设置失败
int VideoGetLastQP(int channel, DWORD dwType, unsigned int * lastqp)
{
#if 0
	int ret,i;
	int EncCnt;	
	unsigned int VencChn;
	//FH_CHN_STATUS pstVencStatus;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
	EncCnt = pCaptureDevice->EncCount;
	
	if (channel >= EncCnt)
	{
		PRINT_ERROR("通道%d入参非法\n",channel);
		return EINVAL;
	}
	
	if (!((pCaptureDevice->EncDevice[channel].SupportStream >> dwType) & 0x01))
	{
		PRINT_ERROR("不支持的码流类型dwType = %d\n",dwType);
		return NOTSUPPORT;
	}
	
	for (i = 0; i < pCaptureDevice->EncDevice[channel].StreamCount; i++)
	{
		if (pCaptureDevice->EncDevice[channel].StreamDevice[i].CapChn == dwType)
		{
			VencChn = i;
		}
	}

	//ret = FH_VENC_GetChnStatus(VencChn, &pstVencStatus);
    if (ret != RETURN_OK)
    {
        PRINT_ERROR("Error(%d - %x): FH_VENC_GetChnStatus\n", ret, ret);
        return GETCHN_ERR;
    }

	//*lastqp = pstVencStatus.lastqp;
#endif
	return 0;
}

/// 设置单一通道的属性。
/// 
/// \param [in] channel 通道号。
/// \param [in] dwType
/// \param [in] QP_FORMAT 设置的参数。
/// \retval 0  设置成功
/// \retval 0  设置失败
int VideoSetQP(int channel, DWORD dwType, QP_FORMAT *pFormat)
{
#if 0
	int ret,i;
	int EncCnt;	
	unsigned int VencChn;
	//FH_VENC_RC_CHANGEPARAM param;
	//FH_VENC_RC_ATTR pstVencrcattr;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
	VideoInDevice_p pVideoInDevice = &GlobalDevice.VideoInDevice;
	EncCnt = pCaptureDevice->EncCount;
	
	if (NULL == pFormat || channel >= EncCnt)
	{
		PRINT_ERROR("通道%d入参非法\n",channel);
		return EINVAL;
	}
	
	if (!((pCaptureDevice->EncDevice[channel].SupportStream >> dwType) & 0x01))
	{
		PRINT_ERROR("不支持的码流类型dwType = %d\n",dwType);
		return NOTSUPPORT;
	}
	
	for (i = 0; i < pCaptureDevice->EncDevice[channel].StreamCount; i++)
	{
		if (pCaptureDevice->EncDevice[channel].StreamDevice[i].CapChn == dwType)
		{
			VencChn = i;
		}
	}
	
	//ret = FH_VENC_GetRCAttr(VencChn,&pstVencrcattr);
    if (ret != RETURN_OK)
    {
        PRINT_ERROR("Error(%d - %x): FH_VENC_GetRCAttr\n", ret, ret);
        return GETRC_ERR;
    }

	param.bitrate = pFormat->BitRate;
	param.ImaxQP  = pFormat->ImaxQP;
	param.IminQP  = pFormat->IminQP; 
	param.PmaxQP  = pFormat->PmaxQP;
	param.PminQP  = pFormat->PminQP;
	switch (pstVencrcattr.rc_type)
	{
		case(FH_RC_H265_AVBR):
			param.I_BitProp	      = pstVencrcattr.h265_avbr.I_BitProp;
			param.P_BitProp       = pstVencrcattr.h265_avbr.P_BitProp;
			param.FrameRate       = pstVencrcattr.h265_avbr.FrameRate;
			param.IFrmMaxBits     = pstVencrcattr.h265_avbr.IFrmMaxBits;
			param.maxrate_percent = pstVencrcattr.h265_avbr.maxrate_percent;
		case(FH_RC_H264_AVBR):
			param.I_BitProp 	  = pstVencrcattr.h265_avbr.I_BitProp;
			param.P_BitProp 	  = pstVencrcattr.h265_avbr.P_BitProp;
			param.FrameRate 	  = pstVencrcattr.h265_avbr.FrameRate;
			param.IFrmMaxBits	  = pstVencrcattr.h265_avbr.IFrmMaxBits;
			param.maxrate_percent = pstVencrcattr.h265_avbr.maxrate_percent;
		break;
		dafult:
			PRINT_ERROR("rc_type: %d not support!\n",pstVencrcattr.rc_type);
			return NOTSUPPORT;
	}
	
	ret = FH_VENC_SetRcChangeParam(VencChn,&param);
    if (ret != RETURN_OK)
    {
        PRINT_ERROR("Error(%d - %x): FH_VENC_SetRcChangeParam\n", ret, ret);
        return SETRC_ERR;
    }
#endif	
	return 0;
}


/// 设置图形模式。
/// 
/// \param [in]  channel 通道号。
/// \param [in]  图像模式。
/// \retval 0  设置成功
/// \retval <0  设置失败
int VideoSetRotate(int channel,ImagMode_e Rotate)
{
	return VideoEncoder_SetRotate(channel, Rotate);
}

//param [in]  channel 通道号
//param [in]  mirror 镜像，0: 恢复镜像，1: 使能镜像 in
//param [in]  flip 翻转，0: 恢复翻转，1: 使能翻转 in
int VideoSetMirrorAndflip(int channel, int mirror, int flip)
{
	/* Forward to video_input module */
	return VideoInput_SetMirrorAndFlip(channel, mirror, flip);
}

/// 设置捕获时间和格式。应用程序需要定时调用此函数与捕获的时间进行同步。
/// 
/// \param [in] channel 通道号。
/// \param [in] pTime 指向系统时间结构SYSTEM_TIME的指针。
/// \param [in] datefmt 日期格式，取date_fmt枚举值。
/// \param [in] datespc 日期分割符，取date_spec枚举值。
/// \param [in] timefmt 时间格式，取time_fmt枚举值。
/// \retval 0  设置成功。
/// \retval 0  设置失败。
int VideoSetTime(int channel, const SYSTEM_TIME * pTime, int datefmt,int datespc,int timefmt,int WeekEnable)
{
	return 0;
}

int VideoSetOSDLanguage(int language)
{
	return VideoOSD_SetLanguage(language);
}

/// 设置视频制式。
/// 
/// \param [in] channel 通道号。
/// \param [in] dwStandard 视频制式，取video_standard_t类型的值。
/// \retval 0  设置成功
/// \retval <0  设置失败
int VideoSetVstd(int channel,DWORD dwStandard)
{
	return VideoInput_SetVstd(channel, dwStandard);
}

int video_venc_adaptive_start()
{
	VencAdaptiveRunning = TRUE;
	return pthread_create(&venc_pid, 0, _venc_adaptive_process, NULL);
}

int video_venc_adaptive_stop()
{
	//在升级时销毁编码自适应线程
	if(TRUE == VencAdaptiveRunning)
	{
		VencAdaptiveRunning = FALSE;
        pthread_join(venc_pid,0);
	}

	return 0;
}

int get_frame_rate(float FrameRate)
{
	unsigned int u32CurFrame;

	u32CurFrame = (unsigned int)FrameRate;
	if (u32CurFrame != gFrameRate)
	{
		if((gFrameRate == (unsigned int)((float)FrameRate * 2)) || gFrameRate == (u32CurFrame / 2))
		{
			pthread_rwlock_wrlock(&ChangeFrmLock);
			ChangeFrmRate = TRUE;
			pthread_rwlock_unlock(&ChangeFrmLock);
		}

		gFrameRate = u32CurFrame;
		PRINT_ERROR("gFrameRate %d\n", gFrameRate);
	}

	return SUCCESS;
}
