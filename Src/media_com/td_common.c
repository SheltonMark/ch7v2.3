
#include "TdCommon.h"
#include "toolconfig.h"
#include "mixed.h"
#include "assert.h"

/* New adapter-based video pipeline interfaces */
#include "video_pipeline.h"
#include "video_osd.h"


GlobalDevice_t GlobalDevice = 
{
	GlobalDevice_Mutex:PTHREAD_MUTEX_INITIALIZER,
};

pthread_mutex_t VideoVpssSwichMute[VPSS_MAX_GRP_NUM];


int _get_image_size(int width, int height)
{
	video_size_t size = VIDEO_SIZE_NR;
	switch(width * height)
	{
		case 704*480://NTSC
		case 704*576://PAL
			size = VIDEO_SIZE_D1;
			break;
		case 704*240://NTSC 704*240 == 352*480
			if(width == 704 && height == 240)
			{
				size = VIDEO_SIZE_HD1;
			}	
			else if(width == 352 && height == 480)
			{
				size = VIDEO_SIZE_BCIF;
			}
			break;
		case 704*288://PAL 704*288 == 352*576
			if(width == 704 && height == 240)
			{
				size = VIDEO_SIZE_HD1;
			}
			else if(width == 352 && height == 576)
			{
				size = VIDEO_SIZE_BCIF;
			}
			break;
		case 352*240://NTSC
		case 352*288://PAL
			size = VIDEO_SIZE_CIF;
			break;
		case 176*120://NTSC
		case 176*144://PAL
			size = VIDEO_SIZE_QCIF;
			break;
		case 640*480:
			size = VIDEO_SIZE_VGA;
			break;
		case 320*240:
			size = VIDEO_SIZE_QVGA;
			break;
		case 480*480://480*480 == 640*360
			if(width == 480 && height == 480)
			{
				size = VIDEO_SIZE_SVCD;
			}
			else if(width == 640 && height == 360)
			{
				size = VIDEO_SIZE_NHD;
			}
			break;
		case 640 * 368:
			size = VIDEO_SIZE_NHD;
			break;
		case 160 * 128:
			size = VIDEO_SIZE_QQVGA;
			break;
		case 800*592:
			size = VIDEO_SIZE_SVGA;
			break;
		case 1024*768:
			size = VIDEO_SIZE_XVGA;
			break;
		case 1200*800:
			size = VIDEO_SIZE_WXGA;
			break;
		case 1280*1024:
			size = VIDEO_SIZE_SXGA;
			break;
		case 1600*1024:
			size = VIDEO_SIZE_WSXGA;
			break;
		case 1600*1200:
			size = VIDEO_SIZE_UXGA;
			break;
		case 1920*1200:
			size = VIDEO_SIZE_WUXGA;
			break;
		case 240*192:
			size = VIDEO_SIZE_LTF;
			break;
		case 1280*720:
			size = VIDEO_SIZE_720;
			break;
		case 1920*1080:
			size = VIDEO_SIZE_1080;
			break;
		case 1280*960:
			size = VIDEO_SIZE_HD960;
			break;
		case 960*480://NTSC
		case 960*576://PAL
			size = VIDEO_SIZE_960H;
			break;
		// case 1280*1024:
		// 	size = VIDEO_SIZE_HD1024;
		// 	break;
		case 960*1080:
			size = VIDEO_SIZE_HALF1080;
			break;
		case 2048*1536:
			size = VIDEO_SIZE_QXGA;
			break;
		case 3840*2160:
			size = VIDEO_SIZE_UHD4K;
			break;
		case 2688*1520:
			size = VIDEO_SIZE_4M;
			break;
		case 2880*1620:
			size = VIDEO_SIZE_5M;
			break;
		case 1280*1440:
			size = VIDEO_SIZE_HALF4M;
			break;
		case 4000*3000:
			size = VIDEO_SIZE_12M;
			break;
		case 2560*1440:
			size = VIDEO_SIZE_4M_QHD;
			break;
		case 1920*1536:
			size = VIDEO_SIZE_3M;
			break;
		case 1440*900:
			size = VIDEO_SIZE_1440X900;
			break;
		case 2304*1296:
			size = VIDEO_SIZE_1296P;
			break;
		case 960*540:
			size = VIDEO_SIZE_540P;
			break;
	}

	return  size;
}

void VB_Config(int gs_enNorm)
{
	PRINT_TRACE("VB_Config success!\n");
}

void MediaSensorInit()
{
	PRINT_ERROR("MediaSensorInit start");
	int      	s32Ret = -1;
    MBUF_CONF_S stMbufConf;
	unsigned int u32Value = 0;

    NI_MDK_SYS_Exit();
    NI_MDK_MBUF_Exit();

    /* VPS 使用私有缓存池时可不用配置公共缓冲池 */
    memset(&stMbufConf, 0, sizeof(MBUF_CONF_S));
    stMbufConf.u32MaxPoolCnt = 128;

	stMbufConf.astCommPool[0].u32BlkCnt  = 0;
    stMbufConf.astCommPool[0].u32BlkSize = 1920 * 1080 * 2;		//16200KB
    strcpy(stMbufConf.astCommPool[1].acMmzName, "anonymous");
    s32Ret = NI_MDK_MBUF_SetConf(&stMbufConf);
	if (SUCCESS != s32Ret)
	{
        PRINT_ERROR("call NI_MDK_MBUF_SetConf err %#x\n", s32Ret);
        return;
    }

    s32Ret = NI_MDK_MBUF_Init();
	if (SUCCESS != s32Ret)
	{
        PRINT_ERROR("call NI_MDK_MBUF_Init err %#x\n", s32Ret);
        return;
    }

    s32Ret = NI_MDK_SYS_Init();
	if (SUCCESS != s32Ret)
	{
        PRINT_ERROR("call NI_MDK_SYS_Init err %#x\n", s32Ret);
        return;
    }


	s32Ret = NI_MDK_PM_SetClkMgrPolicy(PM_CLK_MGR_POLICY_LEVEL);
	if (SUCCESS != s32Ret)
	{
		PRINT_ERROR("call NI_MDK_PM_SetClkMgrPolicy err %#x\n", s32Ret);
		return;
	}
	
	s32Ret = NI_MDK_PM_SetClkCapLevel(PM_CLOCK_CAP_LEVEL_S5);
	if (SUCCESS != s32Ret)
	{
		PRINT_ERROR("call NI_MDK_PM_SetClkCapLevel err %#x\n", s32Ret);
		return;
	}

	/* set i2c0 pinmux */
	/* set sensor vclk pinmux */
	/* set sensor reset gpio38 */
	NI_MDK_SYS_ReadReg(0x09001014, &u32Value);
	u32Value &= 0xf0000fff;
	u32Value |= 0x222000;
	NI_MDK_SYS_WriteReg(0x09001014, u32Value);

	NI_MDK_SYS_ReadReg(0x09050004, &u32Value);
	u32Value &= ~(1 << 6);
	u32Value |= 0x40;
	NI_MDK_SYS_WriteReg(0x09050004, u32Value);

	NI_MDK_SYS_ReadReg(0x09050000, &u32Value);
	u32Value &= ~(1 << 6);
	NI_MDK_SYS_WriteReg(0x09050000, u32Value);
	usleep(20 * 1000);
	u32Value |= (1 << 6);
	NI_MDK_SYS_WriteReg(0x09050000, u32Value);
	usleep(50 * 1000);

	return;
}

void MediaSetSystemInfor(int Mode, int Type, DeviceIspInfo info)
{
	int i, j, k;
	DWORD SupportStream;
	int imagesize = VIDEO_SIZE_NR;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
	VideoInDevice_p pVideoInDevice = &GlobalDevice.VideoInDevice;
	MotionDetectDevice_p pMotionDetectDevice = &GlobalDevice.MotionDetectDevice;   //动检参数设置
	BlindDetectDevice_p pBlindDetectDevice = &GlobalDevice.BlindDetectDevice;	   //遮挡参数设置
	HumanDetectDevice_p pHumanDetectDevice = &GlobalDevice.HumanDetectDevice;  //人形参数和结构
	SensorDevice_p pSensorDevice = &GlobalDevice.SensorDevice;
	
	//暂时写死
	info.SensorType = SENSOR_0S04D10;
#ifdef DEBUG_AVBR
	AVBR_Attr *pAvbrAttr = &GlobalDevice.avbr_attr;

	pAvbrAttr->InitQp = 35;
	pAvbrAttr->IminQP = 28;
	pAvbrAttr->ImaxQP = 50;
	pAvbrAttr->PminQP = 28;
	pAvbrAttr->PmaxQP = 50;
	pAvbrAttr->MaxratePercent = 200;
	pAvbrAttr->IFrmMaxBits = 512*8*1000;
	pAvbrAttr->IPQPDelta = 3;
	pAvbrAttr->IBitProp = 5;
	pAvbrAttr->PBitProp = 1;
	pAvbrAttr->FluctuateLevel = 0;
	pAvbrAttr->StillratePercent = 30;
	pAvbrAttr->Maxstillqp = 34;
#endif
	//info.SensorType = SENSOR_GC4653;
	//PRINT_ERROR("info.SensorType is %d",info.SensorType);
	if (SENSOR_GC4653 == info.SensorType)
	{
		pSensorDevice->MaxWidth = 2560;
		pSensorDevice->MaxHeight = 1440;
		pSensorDevice->MaxFPS = 30;
		pSensorDevice->SenType = SEN_TYPE_GC4653;
		pVideoInDevice->ViMode = TEST_VI_GC4653_MIPI_2560x1440_30FPS;
		pCaptureDevice->MirrorAndflip = 1;
	}
	else if (SENSOR_GC4023 == info.SensorType)
	{
		pSensorDevice->MaxWidth = 2560;
		pSensorDevice->MaxHeight = 1440;
		pSensorDevice->MaxFPS = 30;
		pSensorDevice->SenType = SEN_TYPE_GC4023;
		pVideoInDevice->ViMode = TEST_VI_GC4023_MIPI_2560x1440_30FPS;
		pCaptureDevice->MirrorAndflip = 0;
	}
	else if (SENSOR_0S04L10 == info.SensorType)
	{
		pSensorDevice->MaxWidth = 2560;
		pSensorDevice->MaxHeight = 1440;
		pSensorDevice->MaxFPS = 30;
		pSensorDevice->SenType = SEN_TYPE_OS04L10;
		pVideoInDevice->ViMode = TEST_VI_OS04L10_MIPI_2560x1440_30FPS;
		pCaptureDevice->MirrorAndflip = 0;
	}
	else if (SENSOR_0S04D10 == info.SensorType)
	{
		pSensorDevice->MaxWidth = 2560;
		pSensorDevice->MaxHeight = 1440;
		pSensorDevice->MaxFPS = 30;
		pSensorDevice->SenType = SEN_TYPE_OS04D10;
		pVideoInDevice->ViMode = TEST_VI_OS04D10_MIPI_2560x1440_30FPS;
		pCaptureDevice->MirrorAndflip = 0;
	}
	else if (SENSOR_GC2083 == info.SensorType)
	{
		pSensorDevice->MaxWidth = 1920;
		pSensorDevice->MaxHeight = 1080;
		pSensorDevice->MaxFPS = 30;
		pSensorDevice->SenType = SEN_TYPE_GC2083;
		pVideoInDevice->ViMode = TEST_VI_GC2083_MIPI_1080P_30FPS;
		pCaptureDevice->MirrorAndflip = 0;
	}
	else if ((SENSOR_SC3335 == info.SensorType) ||
			 (SENSOR_SC3336 == info.SensorType))
	{
		pSensorDevice->MaxWidth = 2304;
		pSensorDevice->MaxHeight = 1296;
		pSensorDevice->MaxFPS = 30;
		//pSensorDevice->ISP_Format = FORMAT_2304X1296P30;
		pCaptureDevice->MirrorAndflip = 0;
	}
	else
	{
		PRINT_ERROR("MediaSetSystemInfor: Sensor(%d) is not support!\n", pSensorDevice->SenType);
		assert(0);
	}

	printf("Sensor : name = %d, MaxWidth = %d, MaxHeight = %d, MaxFPS = %d\n",
			pSensorDevice->SenType,pSensorDevice->MaxWidth,pSensorDevice->MaxHeight,pSensorDevice->MaxFPS);

	pVideoInDevice->videoInChannel = 1;
	pVideoInDevice->VDInfo[0].ViDstFps = pSensorDevice->MaxFPS;
	pVideoInDevice->vi_pic.u32Width = pSensorDevice->MaxWidth;
	pVideoInDevice->vi_pic.u32Height = pSensorDevice->MaxHeight;
	//pVideoInDevice->isp_setformat = pSensorDevice->ISP_Format;
	

	pCaptureDevice->Rotate = Mode_Rotate_0;
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

		for (j = 0; j < pCaptureDevice->EncDevice[i].StreamCount; j++)
		{
			pCaptureDevice->EncDevice[i].StreamDevice[AV_SLICE_TYPE_V_WATER].WaterMark.length = 3;
			memcpy(pCaptureDevice->EncDevice[i].StreamDevice[AV_SLICE_TYPE_V_WATER].WaterMark.data,"xky",3);
			if(pCaptureDevice->EncDevice[i].StreamDevice[j].CapChn == CHL_MAIN_T)
			{
				printf("CHL_MAIN_T chn = %d\n",j);
				pCaptureDevice->EncDevice[i].StreamDevice[j].VencStart = 1;
				pCaptureDevice->EncDevice[i].StreamDevice[j].AencStart = 1;
				pCaptureDevice->EncDevice[i].StreamDevice[j].NetTalkStart = 0;
				imagesize = _get_image_size(info.Main_MaxWidth,info.Main_MaxHeight);
				pCaptureDevice->EncDevice[i].StreamDevice[j].Format.ImageSize = imagesize;
				pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask = (1 << imagesize);

				pCaptureDevice->EncDevice[i].StreamDevice[j].Format.FramesPerSecond = pSensorDevice->MaxFPS;

				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.enable 	 = 1;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.channel	 = 0;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.width		 = pSensorDevice->MaxWidth;//2560;//3840;//1920;//2560;
			    pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.height 	 = pSensorDevice->MaxHeight;//1440;//2160;//1080;//1440;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.max_width   = info.Main_MaxWidth;		// 3840;//3840;//1920;//2560;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.max_height  = info.Main_MaxHeight;		// 2160;//2160;//1080;//1440;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.yuv_type	 = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.frame_count = pSensorDevice->MaxFPS;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.frame_time  = 1;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.bps		 = 4096 * 1024;
			    pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.enc_type	 = PT_H264;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.rc_type	 = VENC_RC_MODE_CBR;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.breath_on	 = 1;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.qt_level	 = 1;				
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.profile	 = VENC_H264_BASE_LINE_PROFILE;
			}

			if(pCaptureDevice->EncDevice[i].StreamDevice[j].CapChn == CHL_2END_T)
			{
				printf("CHL_2END_T chn = %d\n",j);
				pCaptureDevice->EncDevice[i].StreamDevice[j].VencStart = 1;
				pCaptureDevice->EncDevice[i].StreamDevice[j].Format.FramesPerSecond = pSensorDevice->MaxFPS;
	//			pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask = (1 << CAPTURE_POWER_5M_12) | (1 << CAPTURE_POWER_4M_QHD_12)| (1 << CAPTURE_POWER_3M_18)  | (1 << CAPTURE_POWER_HD1080_30) | (1 << CAPTURE_POWER_HD720_30) |(1 << CAPTURE_POWER_960H_25) |(1 << CAPTURE_POWER_D1_25);
				imagesize = _get_image_size(info.Sub_MaxWidth, info.Sub_MaxHeight);
				pCaptureDevice->EncDevice[i].StreamDevice[j].Format.ImageSize = imagesize;

				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.enable 	 = 1;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.channel	 = 1;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.width		 = 640;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.height 	 = 368;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.max_width	 = info.Sub_MaxWidth;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.max_height  = info.Sub_MaxHeight;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.yuv_type	 = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.frame_count = pSensorDevice->MaxFPS;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.frame_time  = 1;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.bps		 = 1024 * 1024;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.enc_type	 = PT_H264;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.rc_type	 = VENC_RC_MODE_CBR;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.breath_on	 = 1;				
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.profile	 = VENC_H264_BASE_LINE_PROFILE;
			}

			if(pCaptureDevice->EncDevice[i].StreamDevice[j].CapChn == CHL_JPEG_T)
			{
				printf("CHL_JPEG_T chn = %d\n",j);
				pCaptureDevice->EncDevice[i].StreamDevice[j].VencStart = 0;
				pCaptureDevice->EncDevice[i].StreamDevice[j].Format.FramesPerSecond = pSensorDevice->MaxFPS;
	//			pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask = (1 << CAPTURE_POWER_5M_12) | (1 << CAPTURE_POWER_4M_QHD_12)| (1 << CAPTURE_POWER_3M_18)  | (1 << CAPTURE_POWER_HD1080_30) | (1 << CAPTURE_POWER_HD720_30) |(1 << CAPTURE_POWER_960H_25) |(1 << CAPTURE_POWER_D1_25);
				pCaptureDevice->EncDevice[i].StreamDevice[j].Format.ImageSize = VIDEO_SIZE_NHD;
				
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.enable 	 = 1;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.channel	 = 2;
				/*初始化智能通道分辨率为480*320，为大模型分辨率
				  小模型分辨率为240*160						*/
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.width  	 = 480;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.height 	 = 320;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.max_width	 = 640;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.max_height  = 368;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.yuv_type	 = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.frame_count = pSensorDevice->MaxFPS;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.frame_time  = 1;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.bps		 = 1024 * 1024;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.enc_type 	 = PT_JPEG;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.rc_type	 = VENC_RC_MODE_FIXQP;
				pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.breath_on	 = 1;
			}
		}
	}

/**********************************音频***********************************/
	AiDevice_p pAiDevice = &GlobalDevice.AiDevice;
	AoDevice_p pAoDevice = &GlobalDevice.AoDevice;

	pAiDevice->stAioAttr.sample_rate  		= AC_SR_8K;
	pAiDevice->stAioAttr.bit_width 			= AUDIO_BIT_WIDTH_32;
	pAiDevice->stAioAttr.channels			= 1;
	pAiDevice->stAioAttr.u32PacketPeriod 	= 40;
	pAiDevice->stAioAttr.enCodecType 		= AUDIO_INTERNAL_CODEC;
	pAiDevice->stAioAttr.volume 			= 31;
	pAiDevice->stAioType 					= AudioStreamType_AAC;


	pAoDevice->stAioAttr.sample_rate  		= AC_SR_8K;
	pAoDevice->stAioAttr.bit_width 			= AUDIO_BIT_WIDTH_32;
	pAoDevice->stAioAttr.channels			= 1;
	pAoDevice->stAioAttr.u32PacketPeriod 	= 40;
	pAoDevice->stAioAttr.enCodecType 		= AUDIO_INTERNAL_CODEC;
	pAoDevice->stAioAttr.volume 			= 0;
	pAoDevice->stAioType 					= AudioStreamType_AAC;
/*************************************************************************/

/************************************* 动检参数初始化***********************************************************/	
	pMotionDetectDevice->md_cf.md_config.enable = 1;
	pMotionDetectDevice->md_cf.md_config.threshold = 90;
	pMotionDetectDevice->md_cf.md_config.framedelay = 1;

/************************************* 人形配置*****************************************************************/
	
	for (i = 0; i < pCaptureDevice->EncCount; i++)
	{
		pHumanDetectDevice->human_result[i].Result = 0;
		pHumanDetectDevice->human_result[i].x1 = 0;
		pHumanDetectDevice->human_result[i].y1 = 0;
		pHumanDetectDevice->human_result[i].x2 = 0;
		pHumanDetectDevice->human_result[i].y2 = 0;
		pHumanDetectDevice->human_param[i].Enable = 1;
		pHumanDetectDevice->human_param[i].change = 0;
		pHumanDetectDevice->human_param[i].sensiblity = 50;
	}

/************************************* 车型配置*****************************************************************/
	
	for (i = 0; i < pCaptureDevice->EncCount; i++)
	{
		pHumanDetectDevice->vehicle_result[i].Result = 0;
		pHumanDetectDevice->vehicle_result[i].x1 = 0;
		pHumanDetectDevice->vehicle_result[i].y1 = 0;
		pHumanDetectDevice->vehicle_result[i].x2 = 0;
		pHumanDetectDevice->vehicle_result[i].y2 = 0;
		pHumanDetectDevice->vehicle_param[i].Enable = 1;
		pHumanDetectDevice->vehicle_param[i].change = 0;
		pHumanDetectDevice->vehicle_param[i].sensiblity = 50;
	}

/************************************* 遮挡参数初始化*************************************************************/
	pBlindDetectDevice->cd_config.enable = 1;
	pBlindDetectDevice->cd_config.threshold = 0x96;
	pBlindDetectDevice->cd_config.framedelay  = 1;

/****************************************************************************************************************/

}


void MediaHardWareInit(int Normal)
{
	PRINT_TRACE("Normal  =%d\n",Normal);

	// ProductType_e Type;
	// Type = SystemGetProductType();
	DeviceHalInit();

	if (gpio_init(LED_SELECT) != HI_SUCCESS)
	{
		PRINT_ERROR("gpio_init IRCUT_OFF failed\n");
	}

	if (gpio_init(RED_LED) != HI_SUCCESS)
	{
		PRINT_ERROR("gpio_init RED_LED failed\n");
	}

	if (gpio_set_value(RED_LED, ENABLE) != HI_SUCCESS)
	{
		PRINT_ERROR("gpio_set_value RED_LED failed\n");
	}

	if (gpio_init(BLUE_LED) != HI_SUCCESS)
	{
		PRINT_ERROR("gpio_init BLUE_LED failed\n");
	}

	if (gpio_export(RESET_KEY) != HI_SUCCESS)
	{
		PRINT_ERROR("gpio_export BLUE_LED failed\n");
	}

	if (gpio_init(SPEAK_MUTE) != HI_SUCCESS)
	{
		PRINT_ERROR("gpio_init SPEAK_MUTE failed\n");
	}

	if (gpio_set_value(SPEAK_MUTE, DISABLE) != HI_SUCCESS)
	{
		PRINT_ERROR("gpio_set_value SPEAK_MUTE failed\n");
	}

	/********** 参数初始化************/
	/* Use new VideoPipeline_Init() instead of video_in_init() */
	if(VideoPipeline_Init() != HI_SUCCESS)
	{
		PRINT_ERROR("VideoPipeline_Init failed\n");
		return;
	}

	/* Initialize OSD after pipeline, before capture */
	if(VideoOSD_Init() != HI_SUCCESS)
	{
		PRINT_WARN("videoOSD_Init failed (non-fatal)\n");
	}

	if(AudioInCreate(CHL_MAIN_T) != HI_SUCCESS)
	{
		PRINT_ERROR("AudioCreate failed\n");
	}

#if 0
	FILE *pFile = NI_NULL;
	NI_U32 u32Size = 0;
	NI_S32 s32Ret = -1;

	// pFile = fopen("./ff16b1c44100hz.aac", "rb");
	pFile = fopen("/home/emc_debug.aac", "rb");
	//pFile = fopen("./DIYAudio.aac", "rb");
	if (NI_NULL == pFile)
	{
		NI_ERROR("open file failed ! \n");
	}
	else
	{
		AoDevice_p pAoDevice = &GlobalDevice.AoDevice;
		// pAoDevice->stAioAttr.u32PacketPeriod = 40;
		pAoDevice->stAioAttr.sample_rate = 8000;
		pAoDevice->stAioType = AudioStreamType_AAC;
		AudioOutCreate(CHL_MAIN_T);
		unsigned char pu8Stream[320] = {0};
		while (1)
		{
			u32Size = fread(pu8Stream, 1, 320, pFile);
			if (0 == u32Size)
			{
				fseek(pFile, 0, SEEK_SET);
				continue;
			}

			//AudioOutPlay(pu8Stream, u32Size);
			AudioOutPlay(&pu8Stream, u32Size);
		}
	}

#endif
	if (capture_init() != HI_SUCCESS)
	{
		PRINT_TRACE("video_venc_start failed\n");
	}

}


int write_ring_buff(RING_BUFF *ring_buf, unsigned char *input_data, unsigned int len)
{
	unsigned int t_w = 0;

	if (len > ring_buf->tatol_len || 0 == len || NULL == ring_buf->buf)
	{
		PRINT_ERROR("write_ring_buff error tatol_len = %d len = %d\n",ring_buf->tatol_len,len);
		return -1;
	}

	if (ring_buf->tail == ring_buf->head)
	{
		ring_buf->tail = 0;
		ring_buf->head = 0;
	}

	t_w = ring_buf->tail & (ring_buf->tatol_len - 1);

	if (len <= (ring_buf->tatol_len - t_w))
	{
		memcpy(ring_buf->buf + t_w, input_data, len);
	}
	else
	{
		memcpy(ring_buf->buf + t_w, input_data, (ring_buf->tatol_len - t_w));
		memcpy(ring_buf->buf, input_data + (ring_buf->tatol_len - t_w), len - (ring_buf->tatol_len - t_w));
	}

	ring_buf->tail += len;

	return 0;
}

int read_ring_buff(RING_BUFF *ring_buf, unsigned char *output_data, unsigned int len)
{
	//unsigned int t_w = 0;
	unsigned int t_r = 0;
	unsigned int cur_w = 0;
	unsigned int datalen = 0;
	cur_w = ring_buf->tail;
	datalen = cur_w - ring_buf->head;

	//空了或小于
	if (len > ring_buf->tatol_len || 0 == datalen || datalen < len || 0 == len || NULL == ring_buf->buf)
	{
		return -1;
	}

	//索引在数组的位置
	//t_w = cur_w & (ring_buf->tatol_len - 1);
	t_r = ring_buf->head & (ring_buf->tatol_len - 1);

	if (len <= (ring_buf->tatol_len - t_r))
	{
		memcpy(output_data, ring_buf->buf + t_r, len);
	}
	else
	{
		memcpy(output_data, ring_buf->buf + t_r, (ring_buf->tatol_len - t_r));
		memcpy(output_data + (ring_buf->tatol_len - t_r), ring_buf->buf, len - (ring_buf->tatol_len - t_r));
	}

	ring_buf->head += len;

	return 0;
}