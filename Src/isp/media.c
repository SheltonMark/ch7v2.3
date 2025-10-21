#include "TdCommon.h"
#if 0
static video_size_t Media_ToCapture_Lable[] = {
												VIDEO_SIZE_4M_QHD,
												VIDEO_SIZE_NHD
												};

int Media_DeviceInit()
{
	//ircut_off
	GpioInit(0,3);
	GpioInit(0,4);
	//红外灯
	GpioInit(5,3);
	//音频mute
	GpioInit(0,2);
	//闪光灯
	GpioInit(5,4);
	
	system("echo \"dev,GPIO2,0,0\" > /proc/driver/pinctrl");
	system("echo \"dev,GPIO3,0,0\" > /proc/driver/pinctrl");
	system("echo \"dev,GPIO4,0,0\" > /proc/driver/pinctrl");

	int i,j,k;
	unsigned int SupportStream;
	VideoInDevice_p pVideoInDevice = &GlobalDevice.VideoInDevice;
	MotionDetectDevice_p pMotionDetectDevice = &GlobalDevice.MotionDetectDevice;   //动检参数设置

	pVideoInDevice->videoInChannel = 1;
	pVideoInDevice->VDInfo[0].ViDstFps = 20;

	pVideoInDevice->vi_pic.vi_size.u32Width = 2560;
	pVideoInDevice->vi_pic.vi_size.u32Height = 1440;
	pVideoInDevice->isp_setformat = FORMAT_400WP20;
	
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
	pCaptureDevice->EncCount = pVideoInDevice->videoInChannel;
	for (i = 0; i < pCaptureDevice->EncCount; i++)
	{
		pCaptureDevice->EncDevice[i].SupportStream = (1 << CHL_MAIN_T) | (1 << CHL_2END_T) | (1 << CHL_JPEG_T);
	
		for (j = 0; j < sizeof(pCaptureDevice->EncDevice[i].SupportStream) * 8; j++)
		{
			if ((pCaptureDevice->EncDevice[i].SupportStream >> j) & 0x01)
			{
				pCaptureDevice->EncDevice[i].StreamCount++;
			}
		}
	
		SupportStream = pCaptureDevice->EncDevice[i].SupportStream;
		k = 0;
		j = 0;
		while(SupportStream)
		{
			if (((SupportStream) & 0x01) && (j < pCaptureDevice->EncDevice[i].StreamCount))
			{
				pCaptureDevice->EncDevice[i].StreamDevice[j].CapChn = k;
				j++;
			}
			SupportStream >>= 1;
			k++;
		}

		for(j = 0;j < pCaptureDevice->EncDevice[i].StreamCount;j++)
		{
			if(pCaptureDevice->EncDevice[i].StreamDevice[j].CapChn == CHL_MAIN_T)
			{
				printf("CHL_MAIN_T chn = %d\n",j);
				pCaptureDevice->EncDevice[i].StreamDevice[j].VencStart = 1;
				pCaptureDevice->EncDevice[i].StreamDevice[j].AencStart = 1;
				pCaptureDevice->EncDevice[i].StreamDevice[j].Format.FramesPerSecond = 20;
				
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.enable 	 = 1;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.channel	 = 0;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.width		 = 2560;
			    pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.height 	 = 1440;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.max_width	 = 2560;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.max_height  = 1440;;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.yuv_type	 = VPU_VOMODE_BLK;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.frame_count = 20;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.frame_time  = 1;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.bps		 = 1024*1000*4/5;
			    pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.enc_type	 = FH_NORMAL_H265;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.rc_type	 = FH_RC_H265_VBR;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.breath_on	 = 0;
				
			}

			if(pCaptureDevice->EncDevice[i].StreamDevice[j].CapChn == CHL_2END_T)
			{
				printf("CHL_2END_T chn = %d\n",j);
				pCaptureDevice->EncDevice[i].StreamDevice[j].VencStart = 1;
				pCaptureDevice->EncDevice[i].StreamDevice[j].Format.FramesPerSecond = 20;
				
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.enable 	 = 1;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.channel	 = 1;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.width		 = 640;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.height 	 = 368;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.max_width	 = 640;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.max_height  = 368;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.yuv_type	 = VPU_VOMODE_TILE224;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.frame_count = 20;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.frame_time  = 1;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.bps		 = 600*1000*4/5;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.enc_type	 = FH_NORMAL_H265;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.rc_type	 = FH_RC_H265_VBR;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.breath_on	 = 0;
			}

			if(pCaptureDevice->EncDevice[i].StreamDevice[j].CapChn == CHL_JPEG_T)
			{
				printf("CHL_JPEG_T chn = %d\n",j);
				pCaptureDevice->EncDevice[i].StreamDevice[j].Format.FramesPerSecond = 20;
				
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.enable 	 = 1;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.channel	 = 2;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.width		 = 640;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.height 	 = 368;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.max_width	 = 640;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.max_height  = 368;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.yuv_type	 = VPU_VOMODE_BLK;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.frame_count = 20;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.frame_time  = 1;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.bps		 = 1024 * 1024;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.enc_type	 = FH_JPEG;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.rc_type	 = FH_RC_H265_VBR;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.breath_on	 = 0;
			}
		}
	}

	/**********************************音频***********************************/
		AiDevice_p pAiDevice = &GlobalDevice.AiDevice;
		AoDevice_p pAoDevice = &GlobalDevice.AoDevice;
	
		pAiDevice->stAioAttr.io_type	 = FH_AC_MIC_IN;
		pAiDevice->stAioAttr.sample_rate = AC_SR_8K;
		pAiDevice->stAioAttr.bit_width	 = AC_BW_16;
		pAiDevice->stAioAttr.enc_type	 = FH_PT_LPCM;
		pAiDevice->stAioAttr.channels	 = 1;
		pAiDevice->stAioAttr.period_size = MEDIA_AUDIO_ONE_FRAME_SIZE;
		pAiDevice->stAioAttr.volume 	 = 31;
		pAiDevice->stAioType			 = AudioStreamType_AAC;
		
		pAoDevice->stAioAttr.io_type	 = FH_AC_LINE_OUT;
		pAoDevice->stAioAttr.sample_rate = AC_SR_8K;
		pAoDevice->stAioAttr.bit_width	 = AC_BW_16;
		pAoDevice->stAioAttr.enc_type	 = FH_PT_LPCM;
		pAoDevice->stAioAttr.channels	 = 1;
		pAoDevice->stAioAttr.period_size = MEDIA_AUDIO_ONE_FRAME_SIZE;
		pAoDevice->stAioAttr.volume 	 = 31;
		pAoDevice->stAioType			 = AudioStreamType_AAC;
	/*************************************************************************/

	/************************************* 动检参数初始化 ***********************************************************/
		pMotionDetectDevice->md_cf.ilevel = 2;
		pMotionDetectDevice->md_cf.md_config.enable = 1;
		pMotionDetectDevice->md_cf.md_config.threshold = 150;
		pMotionDetectDevice->md_cf.md_config.framedelay = pVideoInDevice->VDInfo[0].ViDstFps;	
	
	/****************************************************************************************************************/
}

int Media_Video_Init()
{
	Media_DeviceInit();

	if(video_in_init() != HI_SUCCESS) 
	{
		PRINT_ERROR("video_in_init failed\n");
	}

	if(AudioCreate() != HI_SUCCESS)
	{
		PRINT_ERROR("AudioCreate failed\n");
	}
	
	if(video_venc_start() != HI_SUCCESS) 
	{
		PRINT_ERROR("video_venc_start failed\n");
	}

	CaptureStart(0,0);
	CaptureStart(0,1);

	if(MotionDetectCreate() != HI_SUCCESS)
	{
		PRINT_ERROR("MotionDetectCreate failed\n");
	}

	return 0;
}


void Media_Audio_UnInit()
{
	AudioDestory();
}

void Media_Video_IRequest(int type)
{
	//FH_VENC_RequestIDR(type);
}

int Media_Video_SetFormat(Media_Video_Format_t pVideo)
{
	int ret;
    struct channel_info info;
	VIDEO_FORMAT pFormat;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

	if(pVideo.enc_type == Media_Enc_Type_H264)
	{
		pFormat.Compression = VIDEO_COMP_H264; 
	}
	else if(pVideo.enc_type == Media_Enc_Type_H265)
	{
		pFormat.Compression = VIDEO_COMP_H265; 
	}
	else
	{
		PRINT_ERROR("this enc type not support %d\n",pVideo.enc_type);
		return -1;
	}

	pFormat.BitRateControl = VIDEO_BRC_VBR;
	pFormat.ImageSize = Media_ToCapture_Lable[pVideo.picSize];
	pFormat.FramesPerSecond = pVideo.frame_count;
	pFormat.BitRate = pVideo.bps;
	pFormat.profile = pVideo.profile;
	CaptureStop(0, pVideo.type);
	
	ret = VideoSetFormat(0, pVideo.type, &pFormat);
	if(ret)
	{
		PRINT_ERROR("Media_Video_SetFormat error\n");
		return -1;
	}
	
	CaptureStart(0, pVideo.type);

	return 0;
}

int Media_SetRotate(int type, Media_Rotate_e param)
{
	int rotate;

	if(type == 0 || type == 1)
	{
		if(param == Media_Rotate_0)
		{
			rotate = FH_RO_OPS_0;
		}
		else if(param == Media_Rotate_180)
		{
			rotate = FH_RO_OPS_180;
		}
		else
		{
			PRINT_ERROR("this rotate not support [%d]\n",param);
			return -1;
		}

		CaptureStop(0, type);
		FH_VPSS_SetVORotate(type, rotate);
		usleep(100);
		FH_VENC_SetRotate(type, rotate);
		CaptureStart(0, type);
	}
	else
	{
		PRINT_ERROR("this type not support [%d]\n",type);
		return -1;
	}

	return 0;
}

void Media_AudioSendAo(unsigned char *data, int len)
{
	AudioOutPlay(data,len);
}

void Media_GetAbility(Media_CaptureAbility_p Caps)
{
	Caps->HD = Media_HD_400W;
	Caps->SD = Media_SD_640_360;
	Caps->PictureFlip = 1;
	Caps->VoiceSpeek = 1;
	Caps->TFCard = 1;
}

int Media_GetJpegData(char *data, unsigned int *len)
{
	FH_VENC_STREAM stream;
	int ret;
	
	ret = FH_SYS_BindVpu2Enc(2, 2);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error(%d - %x): FH_SYS_BindVpu2Enc\n", ret, ret);
	}
	
	ret = FH_VENC_GetStream_Block(FH_STREAM_JPEG, &stream);
	if (ret == RETURN_OK)
	{
		memcpy(data, stream.jpeg_stream.start, stream.jpeg_stream.length);
		*len = stream.jpeg_stream.length;
	}
	else
	{
		PRINT_ERROR("get jpeg pic failed\n");
	}
	
	FH_VENC_ReleaseStream(stream.jpeg_stream.chan);
	
	return 0;
}

int Media_MotionInit()
{
	if(MotionDetectCreate() != HI_SUCCESS)
	{
		PRINT_ERROR("Media_MotionInit failed\n");
		return -1;
	}
}

int Media_MotionUnInit()
{
	MotionDetectDestory();

	return 0;
}

int Media_MotionSetParam(int sensitiveness)
{
	MotionDetectDevice_p pMotionDetectDevice = &GlobalDevice.MotionDetectDevice;   //动检参数设置
	VideoInDevice_p pVideoInDevice = &GlobalDevice.VideoInDevice;
	int ret;
	memset(&pMotionDetectDevice->md_cf, 0, sizeof(pMotionDetectDevice->md_cf));
	pMotionDetectDevice->md_cf.ilevel = 2;   //灵敏度档次
	
	pMotionDetectDevice->md_cf.win[0] = 0b11111;
	pMotionDetectDevice->md_cf.win[1] = 0b11111;
	pMotionDetectDevice->md_cf.win[2] = 0b11111;
	pMotionDetectDevice->md_cf.win[3] = 0b11111;
	pMotionDetectDevice->md_cf.win[4] = 0b11111;
	pMotionDetectDevice->md_cf.md_config.framedelay = pVideoInDevice->VDInfo[0].ViDstFps;  //帧间隔
	pMotionDetectDevice->md_cf.md_config.enable = 1;    //使能开关
	pMotionDetectDevice->md_cf.md_config.threshold = sensitiveness;  //灵敏度具体值

	return 0;
}

int Media_MotionGetResult(int *result)
{
	int i;
	MOTION_DETECT_RESULT pResult;

	MotionDetectGetResult(0,&pResult);

	for(i = 0;i < 5;i++)
	{
		if(pResult.win[i] != 0)
		{
			*result = 1;
			return 0;
		}
	}

	*result = 0;

	return 0;
}

void Media_LightStatus(int enable)
{
	SystemSetLight(enable);
}

void Media_GetDayStatus(int* status)
{
	ImageGetDayStatus(status);
}

void Media_SetIrcut(int enable)
{
	if(enable == 0)
	{
		sample_SmartIR_Day();
		Ircut(0);
	}
	else if(enable == 1)
	{
		sample_SmartIR_Night();
		Ircut(1);
	}
}
#endif
