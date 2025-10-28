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

			ret = VideoVPS_SetInputFrameRate(channel, sensor_fps);
			if (ret != RETURN_OK)
			{
				PRINT_ERROR("Error(%x): VideoVPS_SetInputFrameRate!\n", ret);
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
				ret = VideoVPS_SetOutputParam(channel, ichannel, &ChnAttr);
				if (ret != RETURN_OK)
				{
					PRINT_ERROR("Error(%x): VideoVPS_SetInputFrameRate!\n", ret);
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
					PRINT_ERROR("Error(%x): VideoVPS_SetInputFrameRate!\n", ret);
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
				ret = VideoVPS_SetOutputParam(channel, ichannel, &ChnAttr);
				if (ret != RETURN_OK)
				{
					PRINT_ERROR("Error(%x): VideoVPS_SetOutputParam!\n", ret);
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
	return VideoEncoder_SetFormat(channel, dwType, pFormat);
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
