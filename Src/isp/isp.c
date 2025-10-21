#include "TdCommon.h"
#include "LtmCfg.h"

static int g_Ircut = 0;
static int g_Ircut_pro = 0;
BOOL ChangeFrmStart = DISABLE; // 降帧开启标志位
static pthread_t g_SmartIrcut;
static pthread_t g_SmartIrcut_pro;
static pthread_mutex_t g_smartIRMutex;
static unsigned int ircut_state = DISABLE;
static unsigned int irlight_state = DISABLE;
static unsigned int cover = DISABLE;
static int irStaCurr = AISP_DN_MODE_DAY;
static ISP_DAYNIGHT_VISION_E daynight_vision = ISP_VISION_INFRARED;

extern int LightControl(LightState lightstate);

LIGHT_STATE light_state =
{
	.FlashLight_state = DISABLE,
	.InfraredLight_state = DISABLE,
	.light_strength =
		{
			.FlashLight = 50,
			.InfraredLight = 50,
		},
};

static AE_TIMES Image_ConToLib_Table[] = {
										AE_TIMES_1_3,
										AE_TIMES_1_4,
										AE_TIMES_1_5,
										AE_TIMES_1_6,
										AE_TIMES_1_8,
										AE_TIMES_1_10,
										AE_TIMES_1_12,
										AE_TIMES_1_15,
										AE_TIMES_1_25,
										AE_TIMES_1_30,
										AE_TIMES_1_50,
										AE_TIMES_1_60,
										AE_TIMES_1_100,
										AE_TIMES_1_120,
										AE_TIMES_1_150,
										AE_TIMES_1_200,
										AE_TIMES_1_250,
										AE_TIMES_1_500,
										AE_TIMES_1_750,
										AE_TIMES_1_1000,
										AE_TIMES_1_2000,
										AE_TIMES_1_4000,
										AE_TIMES_1_10000,
										AE_TIMES_1_100000};

static WB_GAIN_S gain[3][4] =
{
	{
		{26, 25},
		{26, 21},
		{23, 20},
		{30, 25},
	},
	{
		{30, 25},
		{30, 20},
		{30, 23},
		{31, 22},
	},
	{
		{53, 25},
		{50, 25},
		{35, 47},
		{30, 47},
	}
};

int _image_smart()
{
	return 0;
}

int _image_dn_switch(AISP_DN_MODE_E cur_status)
{
	int ret = -1;
	ISP_DEV IspDev = 0;
	LightState light;
	AISP_AFPS_CTRL_S pstAFpsCtrl;
	AISP_APP_DAY_NIGHT_S pstDayNight;
	SensorDevice_p pSensorDevice = &GlobalDevice.SensorDevice;
	VideoInDevice_p pVideoInDevice = &GlobalDevice.VideoInDevice;
	
	memset(&light, 0, sizeof(LightState));
	memset(&pstAFpsCtrl, 0, sizeof(AISP_AFPS_CTRL_S));
	memset(&pstDayNight, 0, sizeof(AISP_APP_DAY_NIGHT_S));

	ret = NI_MDK_AISP_GetDayNight(IspDev, &pstDayNight);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_GetDayNight failed %#x\n", ret);
		return ret;
	}

	if (AISP_DN_MODE_DAY == cur_status)
	{
		light.code = faceBoardFlashLight;
		light.strength = light_state.light_strength.FlashLight;
		light.state = DISABLE;
		LightControl(light);
		light_state.FlashLight_state = DISABLE;
		pstDayNight.enDayNight = AISP_DN_MODE_DAY;
		ret = NI_MDK_AISP_SetDayNight(IspDev, &pstDayNight);
		if (RETURN_OK != ret)
		{
			PRINT_ERROR("NI_MDK_AISP_SetDayNight failed %#x\n", ret);
			return ret;
		}

		if(ENABLE == ChangeFrmStart)
		{
			ret = NI_MDK_AISP_GetAutoFps(IspDev, &pstAFpsCtrl);
			if (RETURN_OK != ret)
			{
				PRINT_ERROR("NI_MDK_AISP_GetAutoFps failed %#x\n", ret);
				return -1;
			}

			pstAFpsCtrl.bEnable = DISABLE;

			ret = NI_MDK_AISP_SetAutoFps(IspDev, &pstAFpsCtrl);
			if (RETURN_OK != ret)
			{
				PRINT_ERROR("NI_MDK_AISP_SetAutoFps failed %#x\n", ret);
				return -1;
			}
			ChangeFrmStart = DISABLE;
		}

		printf("Switch to Day\n");
		irStaCurr = AISP_DN_MODE_DAY;
	}
	else if (AISP_DN_MODE_NIGHT == cur_status)
	{
		if (ISP_VISION_INFRARED == daynight_vision)
		{
			light.code = faceBoardFlashLight;
			light.strength = light_state.light_strength.FlashLight;
			light.state = DISABLE;
			LightControl(light);
			light_state.FlashLight_state = DISABLE;
			pstDayNight.enDayNight = AISP_DN_MODE_NIGHT;
			ret = NI_MDK_AISP_SetDayNight(IspDev, &pstDayNight);
			if (RETURN_OK != ret)
			{
				PRINT_ERROR("NI_MDK_AISP_SetDayNight failed %#x\n", ret);
				return ret;
			}
			printf("Switch to Night\n");
		}
		else if (ISP_VISION_COLOR == daynight_vision)
		{
			pstDayNight.enDayNight = AISP_DN_MODE_DAY;
			ret = NI_MDK_AISP_SetDayNight(IspDev, &pstDayNight);
			if (RETURN_OK != ret)
			{
				PRINT_ERROR("NI_MDK_AISP_SetDayNight failed %#x\n", ret);
				return ret;
			}
			usleep(50 *1000);
			light.code = faceBoardFlashLight;
			light.strength = light_state.light_strength.FlashLight;
			light.state = ENABLE;
			LightControl(light);
			light_state.FlashLight_state = ENABLE;
			printf("Switch to WarmLight\n");
		}
		else
		{
			PRINT_ERROR("unknown daynight_vision  %d!\n", daynight_vision);
			ret = 0;
		}

		//这边降帧增加sensor类型判断，当前仅CH7系列需要降帧，CP3不需要
		if (DISABLE == ChangeFrmStart && ((SEN_TYPE_OS04L10 == pSensorDevice->SenType) || (SEN_TYPE_GC4653 == pSensorDevice->SenType) || (SEN_TYPE_OS04D10 == pSensorDevice->SenType)))
		{
			ret = NI_MDK_AISP_GetAutoFps(IspDev, &pstAFpsCtrl);
			if (RETURN_OK != ret)
			{
				PRINT_ERROR("NI_MDK_AISP_GetAutoFps failed %#x\n", ret);
				return -1;
			}

			pstAFpsCtrl.bEnable = ENABLE;
			// 30DB以后开启降帧
			pstAFpsCtrl.u32GainThr = 30 * 1000;
			// 降帧的帧率根据当前制式来决定
			pstAFpsCtrl.u32NewFps = pVideoInDevice->VDInfo[0].ViDstFps * 1000 / 2;
			pstAFpsCtrl.enMode = AISP_AUTO_FPS_BY_MULTIPLES;
			pstAFpsCtrl.u8Multiples = 2;

			ret = NI_MDK_AISP_SetAutoFps(IspDev, &pstAFpsCtrl);
			if (RETURN_OK != ret)
			{
				PRINT_ERROR("NI_MDK_AISP_SetAutoFps failed %#x\n", ret);
				return -1;
			}

			ChangeFrmStart = ENABLE;
		}
		irStaCurr = AISP_DN_MODE_NIGHT;
	}
	else
	{
		PRINT_ERROR("unknown cur_status type %d!\n", cur_status);
		ret = 0;
	}

	//此处增加100ms延迟，以保证图像参数加载完成
	//未加载完成会导致后续设置宽动态时，将参数复原
	usleep(100 * 1000);

	return ret;
}

int _image_smart_ircut_pro()
{
	int ret = -1;
	ISP_DEV IspDev = 0;
	AISP_DN_RESULT_S pstDnRes;
	static int lastVision = ISP_VISION_INFRARED;

	memset(&pstDnRes, 0, sizeof(AISP_DN_RESULT_S));
	
	while (g_Ircut_pro)
	{
		ret = NI_MDK_AISP_GetDnResult(IspDev, &pstDnRes);
		if (RETURN_OK != ret)
		{
			PRINT_ERROR("NI_MDK_AISP_DnGetResult failed %#x\n", ret);
		}

		if (AISP_DN_MODE_NIGHT == irStaCurr && lastVision != daynight_vision)
		{
			ret = _image_dn_switch(AISP_DN_MODE_NIGHT);
			if (RETURN_OK != ret)
			{
				PRINT_ERROR("_image_dn_switch failed %#x\n", ret);
			}

			lastVision = daynight_vision;
		}

		if(irStaCurr == pstDnRes.u32DnEnv)
		{
			usleep(100 * 1000);
			continue;
		}

		pthread_mutex_lock(&g_smartIRMutex);
		ret = _image_dn_switch(pstDnRes.u32DnEnv);
		if (RETURN_OK != ret)
		{
			PRINT_ERROR("_image_dn_switch failed %#x\n", ret);
		}
		pthread_mutex_unlock(&g_smartIRMutex);
	}

	return 0;
}

/**
 *  @brief      获取不同sensor类型下的日夜切换阈值
 *
 * @param[in]   gainDay2Night   白天到晚上切换，光亮度增益
 * gainDay2Night 传值分别为50 60 70
 * @param[in]   gainNight2Day   晚上到晚上切换，光亮度增益
 * gainNight2Day 传值分别为70 80 90
 * @param[out]   D2nGainThr   白天到晚上的阈值
 * @param[out]   N2dGainThr   晚上到白天的阈值
 *
 */
void _isp_get_dn_gain_thr(unsigned int gainDay2Night, unsigned int gainNight2Day, unsigned int *D2nGainThr, unsigned int *N2dGainThr)
{
	// ProductType_e Type;
	// Type = SystemGetProductType();
	SensorDevice_p pSensorDevice = &GlobalDevice.SensorDevice;

	if (SEN_TYPE_OS04D10 == pSensorDevice->SenType)
	{
		*D2nGainThr = (70 - gainDay2Night) * 3 + 120;
		if (ISP_VISION_INFRARED == daynight_vision)
		{
			*N2dGainThr = gainNight2Day - 70 + 345;
		}
		else if (ISP_VISION_COLOR == daynight_vision)
		{
			*N2dGainThr = gainNight2Day - 70 + 367;
		}
	}
	else
	{
		*D2nGainThr = (70 - gainDay2Night) * 8 + 120;
		if (ISP_VISION_INFRARED == daynight_vision)
		{
			*N2dGainThr = gainNight2Day - 70 + 390;
		}
		else if (ISP_VISION_COLOR == daynight_vision)
		{
			*N2dGainThr = gainNight2Day - 70 + 355;
		}
	}

	return;
}

int isp_init(SAMPLE_VI_INFO_S viInfo)
{
	int ret = 0;
	NI_BOOL sbIsp2AStart = NI_TRUE;
	TEST_ISP_CONFIG_S stIspCfg;

	memset(&stIspCfg, 0, sizeof(TEST_ISP_CONFIG_S));
    memset(&stIspCfg,0,sizeof(stIspCfg));
    stIspCfg.u32Width = viInfo.stSenCfg.u32Width;
    stIspCfg.u32Height = viInfo.stSenCfg.u32Height;
    stIspCfg.u32Fps = viInfo.stSenCfg.u32Fps;
    ret = SDK_COMM_ISP_Start(0,&stIspCfg,sbIsp2AStart,!!viInfo.stSenCfg.enHdrMode);
    if(ret != NI_SUCCESS)
    {
		PRINT_ERROR("call SDK_COMM_ISP_Start err %#x\n", ret);
    }

	return ret;
}

//图像调试工具
int ImageDebugControl(BOOL state)
{
	if (DISABLE == state)
	{
		if (g_Ircut)
		{
			g_Ircut = 0;
			pthread_join(g_SmartIrcut, NULL);
		}
	}
	else if(ENABLE == state)
	{
		g_Ircut = 1;
		pthread_create(&g_SmartIrcut, NULL, (void *)_image_smart, NULL);
	}

	return RETURN_OK;
}

//设置亮度
int ImageSetBrightness(int channel, unsigned int level)
{
	int ret = -1;
	ISP_DEV IspDev = 0;
	AISP_APP_COLOR_S pstColor;

	memset(&pstColor, 0, sizeof(AISP_APP_COLOR_S));
	if (level > 100)
	{
		PRINT_ERROR("this level [%d] not support\n",level);
		return -1;
	}

	ret = NI_MDK_AISP_GetColor(IspDev, &pstColor);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_GetColor failed %#x\n", ret);
		return -1;
	}

	pstColor.u8Brt = level;

	ret = NI_MDK_AISP_SetColor(IspDev, &pstColor);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_SetColor failed %#x\n", ret);
		return -1;
	}

	return 0;
}

//设置对比度
int ImageSetContrast(int channel, unsigned int level)
{
	int ret = -1;
	ISP_DEV IspDev = 0;
	AISP_APP_COLOR_S pstColor;

	memset(&pstColor, 0, sizeof(AISP_APP_COLOR_S));
	if (level > 100)
	{
		PRINT_ERROR("this level [%d] not support\n", level);
		return -1;
	}

	ret = NI_MDK_AISP_GetColor(IspDev, &pstColor);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_GetColor failed %#x\n", ret);
		return -1;
	}

	pstColor.u8Ctr = level;

	ret = NI_MDK_AISP_SetColor(IspDev, &pstColor);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_SetColor failed %#x\n", ret);
		return -1;
	}

	return 0;
}

//设置饱和度
int ImageSetSaturation(int channel, unsigned int level)
{
	int ret = -1;
	ISP_DEV IspDev = 0;
	AISP_APP_COLOR_S pstColor;

	memset(&pstColor, 0, sizeof(AISP_APP_COLOR_S));
	if (level > 100)
	{
		PRINT_ERROR("this level [%d] not support\n", level);
		return -1;
	}

	ret = NI_MDK_AISP_GetColor(IspDev, &pstColor);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_GetColor failed %#x\n", ret);
		return -1;
	}

	pstColor.u8Sat = level;

	ret = NI_MDK_AISP_SetColor(IspDev, &pstColor);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_SetColor failed %#x\n", ret);
		return -1;
	}

	return 0;
}

//设置锐度
int ImageSetSharpeness(int channel, unsigned int level)
{
	int ret = -1;
	ISP_DEV IspDev = 0;
	AISP_APP_EE_S pstEe;

	memset(&pstEe, 0, sizeof(AISP_APP_EE_S));
	if (level > 100)
	{
		PRINT_ERROR("this level [%d] not support\n", level);
		return -1;
	}

	ret = NI_MDK_AISP_GetEeAttr(IspDev, &pstEe);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_GetEeAttr failed %#x\n", ret);
		return -1;
	}

	pstEe.bEnable = 1;
	pstEe.u8Level = level;

	ret = NI_MDK_AISP_SetEeAttr(IspDev, &pstEe);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_SetEeAttr failed %#x\n", ret);
		return -1;
	}

	return 0;
}

//设置自动曝光模式
int ImageSetAeMode(int channel, AE_MODE mode, AE_TIME_e intt_us,unsigned int gain_level)
{
	int ret = -1;
	ISP_DEV IspDev = 0;
	AISP_APP_EXP_S pstExp;

	memset(&pstExp, 0, sizeof(AISP_APP_EXP_S));
	ret = NI_MDK_AISP_GetExpAttr(IspDev, &pstExp);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_GetExpAttr failed %#x\n", ret);
		return -1;
	}
	
	switch (mode)
	{
		case AE_MODE_AUTO:
			pstExp.enExpMode = AISP_EXP_MODE_AUTO;
			break;
		case AE_MODE_TIME:
			pstExp.enExpMode = AISP_EXP_MODE_SHUTTER_PRIO;
			pstExp.stExpRange.u32ShutterMin = Image_ConToLib_Table[intt_us];
			pstExp.stExpRange.u32ShutterMax = Image_ConToLib_Table[intt_us];
			break;
		case AE_MDOE_GAIN:
			pstExp.enExpMode = AISP_EXP_MODE_GAIN_PRIO;
			pstExp.stExpRange.u32GainMax = gain_level;
			pstExp.stExpRange.u32GainMin = gain_level;
			break;
		case AE_MODE_MANUAL:
			pstExp.enExpMode = AISP_EXP_MODE_MANUAL;
			pstExp.stExpRange.u32ShutterMin = Image_ConToLib_Table[intt_us];
			pstExp.stExpRange.u32ShutterMax = Image_ConToLib_Table[intt_us];
			pstExp.stExpRange.u32GainMax = gain_level;
			pstExp.stExpRange.u32GainMin = gain_level;
			break;
		default:
			PRINT_ERROR("this mode [%d] not support\n",mode);
			return -1;
	}

	ret = NI_MDK_AISP_SetExpAttr(IspDev, &pstExp);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_SetExpAttr failed %#x\n", ret);
		return -1;
	}
	return 0;
}

// 设置自动降帧
int ImageSetAutoFps(int channel, int enable)
{
	int ret = -1;
	ISP_DEV IspDev = 0;
	AISP_AFPS_CTRL_S pstAFpsCtrl;
	VideoInDevice_p pVideoInDevice = &GlobalDevice.VideoInDevice;

	memset(&pstAFpsCtrl,0,sizeof(AISP_AFPS_CTRL_S));

	ret = NI_MDK_AISP_GetAutoFps(IspDev, &pstAFpsCtrl);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_GetAutoFps failed %#x\n", ret);
		return -1;
	}

	if(ENABLE == enable)
	{
		pstAFpsCtrl.bEnable = ENABLE;
	}
	else
	{
		pstAFpsCtrl.bEnable = DISABLE;
	}

	pstAFpsCtrl.u32NewFps = pVideoInDevice->VDInfo[0].ViDstFps * 1000 / 2;
	//30DB以后开启降帧
	pstAFpsCtrl.u32GainThr = 30 * 1000;
	pstAFpsCtrl.enMode = AISP_AUTO_FPS_BY_MULTIPLES;
	pstAFpsCtrl.u8Multiples = 2;

	ret = NI_MDK_AISP_SetAutoFps(IspDev, &pstAFpsCtrl);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_SetAutoFps failed %#x\n", ret);
		return -1;
	}

	return ret;
}

//获取曝光速度列表
int ImageGetExposureSpeeds(int channel, int antiFlickMode, int* speeds, int* num)
{
	speeds[0] = 0;
	*num = 1;
	int count = sizeof(Image_ConToLib_Table) / sizeof(AE_TIMES);
	if (0 == antiFlickMode)//关闭防闪烁
	{
		for (size_t i = 8; i < count-1; i++)
		{
			speeds[i-8+1] = (int)Image_ConToLib_Table[i];
			(*num)++;
		}
	}
	else if (1 == antiFlickMode)//开启50HZ防闪烁
	{
		//不同防闪烁模式曝光时间有区别
		speeds[1] = AE_TIMES_1_25;
		speeds[2] = AE_TIMES_1_50;
		speeds[3] = AE_TIMES_1_100;
		*num = 4;
	}
	else if (2 == antiFlickMode)//开启60HZ防闪烁
	{
		//不同防闪烁模式曝光时间有区别
		speeds[1] = AE_TIMES_1_30;		
		speeds[2] = AE_TIMES_1_60;
		speeds[3] = AE_TIMES_1_120;
		*num = 4;
	}

	return 0;
}

//设置宽动态参数
int ImageSetLtmCfg(int channel, unsigned int mode, unsigned int level)
{
	int ret = -1;
	ISP_DEV IspDev = 0;
	AISP_APP_EXP_S pstExp;
	AISP_APP_BACKLIGHT_S pstBackLight;

	memset(&pstExp, 0, sizeof(AISP_APP_EXP_S));
	memset(&pstBackLight, 0, sizeof(AISP_APP_BACKLIGHT_S));
	if (level > 100)
	{
		PRINT_ERROR("this level [%d] not support\n", level);
		return -1;
	}

	ret = NI_MDK_AISP_GetBackLight(IspDev, &pstBackLight);
	if (RETURN_OK != ret) 
	{
		PRINT_ERROR("NI_MDK_AISP_GetBackLight failed %#x\n", ret);
		return -1;
	}

	ret = NI_MDK_AISP_GetExpAttr(IspDev, &pstExp);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_GetExpAttr failed %#x\n", ret);
		return -1;
	}

	if(0 == level)
	{
		pstBackLight.enBackLightMode = AISP_BACKLIGHT_MODE_NONE;
	}
	else
	{
		pstBackLight.enBackLightMode = AISP_BACKLIGHT_MODE_WDR;
	}

	pstBackLight.u8Level = level;
	pstExp.stRoiExp.bRegionEn = 0;

	ret = NI_MDK_AISP_SetBackLight(IspDev, &pstBackLight);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_SetBackLight failed %#x\n", ret);
		return -1;
	}

	ret = NI_MDK_AISP_SetExpAttr(IspDev, &pstExp);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_GetExpAttr failed %#x\n", ret);
		return -1;
	}

	return 0;
}
//8192，8192
//设置前光或背光补偿
int ImageSetLightCompensation(int channel, LIGHT_MODE_e mode, unsigned int level, unsigned int left, unsigned int top, unsigned int right, unsigned int bottom)
{
	int ret = -1;
	ISP_DEV IspDev = 0;
	AISP_APP_EXP_S pstExp;
	unsigned int value = 100;
	AISP_APP_BACKLIGHT_S pstBackLight;

	memset(&pstExp, 0, sizeof(AISP_APP_EXP_S));
	memset(&pstBackLight, 0, sizeof(AISP_APP_BACKLIGHT_S));
	if (level > 100)
	{
		PRINT_ERROR("this level [%d] not support\n",level);
		return -1;
	}

	if(right > 8192 || bottom > 8192 || left > 8192 || top > 8192)
	{
		PRINT_ERROR("left: [%d] top: [%d] right: [%d] bottom: [%d] is too big\n",left, top, right, bottom);
		return -1;
	}

	ret = NI_MDK_AISP_GetBackLight(IspDev, &pstBackLight);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_GetBackLight failed %#x\n",ret);
		return -1;
	}

	ret = NI_MDK_AISP_GetExpAttr(IspDev, &pstExp);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_GetExpAttr failed %#x\n", ret);
		return -1;
	}

	if(AISP_BACKLIGHT_MODE_WDR == pstBackLight.enBackLightMode)
	{
		return 0;
	}

	switch (mode)
	{
		case LIGHT_MODE_SHUT:
			pstBackLight.enBackLightMode = AISP_BACKLIGHT_MODE_NONE;
			pstExp.stRoiExp.bRegionEn = 0;
			break;
		case LIGHT_MODE_BACK:
			pstBackLight.enBackLightMode = AISP_BACKLIGHT_MODE_BLC;
			pstBackLight.u8Level = value;

			pstExp.stRoiExp.bRegionEn = 1;
			pstExp.stRoiExp.stRoiParam.u16StartX = (unsigned short)((float)left / 8192 * 1024);
			pstExp.stRoiExp.stRoiParam.u16StartY = (unsigned short)((float)top / 8192 * 1024);
			pstExp.stRoiExp.stRoiParam.u16EndX 	 = (unsigned short)((float)right / 8192 * 1024);
			pstExp.stRoiExp.stRoiParam.u16EndY 	 = (unsigned short)((float)bottom / 8192 * 1024);

			break;
		case LIGHT_MODE_FRONT:
			pstBackLight.enBackLightMode = AISP_BACKLIGHT_MODE_AHL;
			pstBackLight.u8Level = level;

			pstExp.stRoiExp.bRegionEn = 0;
			break;
		default:
			PRINT_ERROR("this mode [%d] not support\n",mode);
			return -1;
	}

	ret = NI_MDK_AISP_SetBackLight(IspDev, &pstBackLight);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_SetBackLight failed %#x\n", ret);
		return -1;
	}

	ret = NI_MDK_AISP_SetExpAttr(IspDev, &pstExp);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_GetExpAttr failed %#x\n", ret);
		return -1;
	}
	return 0;
}

//设置防闪烁或防补光过曝
int ImageSetFlashing(int channel, FLASHING_MODE_e mode, FLASH_HZ_e frequency)
{
	int ret = -1;
	ISP_DEV IspDev = 0;
	AISP_APP_EXP_S pstExp;
	AISP_APP_LIGHT_S pstSmartLight;

	memset(&pstExp, 0, sizeof(AISP_APP_EXP_S));
	memset(&pstSmartLight, 0, sizeof(AISP_APP_LIGHT_S));

	ret = NI_MDK_AISP_GetExpAttr(IspDev, &pstExp);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_GetExpAttr failed %#x\n", ret);
		return -1;
	}

	ret = NI_MDK_AISP_GetSmartLight(IspDev, &pstSmartLight);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_GetExpAttr failed %#x\n", ret);
		return -1;
	}

	switch(mode)
	{
		case FLASHING_MODE_SHUT:
			pstExp.stAntiFlick.bAutoSht = 0;

			pstSmartLight.enLightMode = AISP_LIGHT_MODE_MANUAL;
			pstSmartLight.u8Level = 100;
			break;
		case FLASHING_MODE_FLASH:
			switch (pstExp.enExpMode)	
			{
				case(AISP_EXP_MODE_AUTO):
					pstExp.stAntiFlick.bAutoSht = 1;
					break;
				case(AISP_EXP_MODE_SHUTTER_PRIO):
					pstExp.stAntiFlick.bAutoSht = 0;
					pstExp.stExpRange.u32GainMax = 100;
					pstExp.stExpRange.u32GainMin = 0;
					break;
				case(AISP_EXP_MODE_GAIN_PRIO):
					pstExp.stAntiFlick.bAutoSht = 0;
					pstExp.stExpRange.u32ShutterMin = 10;
					if (FLASH_HZ_50 == frequency)
					{
						pstExp.stExpRange.u32ShutterMax = AE_TIMES_1_25;
					}
					else if (FLASH_HZ_60 == frequency)
					{
						pstExp.stExpRange.u32ShutterMax = AE_TIMES_1_30;
					}
					break;
				case (AISP_EXP_MODE_MANUAL): 
					pstExp.stAntiFlick.bAutoSht = 0;
					break;
				default:
					PRINT_ERROR("unknown pstExp.enExpMode [%d]\n", pstExp.enExpMode);
					return -1;
			}

			pstExp.enExpMode = AISP_EXP_MODE_ANTI_FLICKER;

			if (FLASH_HZ_50 == frequency)
			{
				pstExp.stAntiFlick.u32AntiFlikerFreq = 50;
			}
			else if (FLASH_HZ_60 == frequency)
			{
				pstExp.stAntiFlick.u32AntiFlikerFreq = 60;
			}
			else
			{
				PRINT_ERROR("flash hz type not support [%d]\n",frequency);
				return -1;
			}

			break;
		case FLASHING_MODE_LIGHT:
			pstSmartLight.enLightMode = AISP_LIGHT_MODE_AUTO;
			break;
		default:
			PRINT_ERROR("this mode [%d] not support\n",mode);
			return -1;
	}

	ret = NI_MDK_AISP_SetExpAttr(IspDev, &pstExp);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_SetExpAttr failed %#x\n", ret);
		return -1;
	}

	ret = NI_MDK_AISP_SetSmartLight(IspDev, &pstSmartLight);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_GetExpAttr failed %#x\n", ret);
		return -1;
	}

	return 0;
}

//关闭日夜检测线程
//升级时使用
int image_smart_deinit()
{
	if (g_Ircut_pro)
	{
		g_Ircut_pro = 0;
		pthread_join(g_SmartIrcut_pro, NULL);
	}

	return SUCCESS;
}

int ImageSetIrcut(int channel, IRCUT_MODE_e mode, unsigned int gainDay2Night, unsigned int gainNight2Day)
{
	int ret = -1;
	ISP_DEV IspDev = 0;
	unsigned int D2nGainThr = 0;
	unsigned int N2dGainThr = 0;
	AISP_APP_DAY_NIGHT_S pstDayNight;

	memset(&pstDayNight, 0, sizeof(AISP_APP_DAY_NIGHT_S));

	ret = NI_MDK_AISP_GetDayNight(IspDev, &pstDayNight);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_GetDayNight failed %#x\n", ret);
		return -1;
	}

	switch (mode)
	{
		case IRCUT_MODE_DAY:
			if (g_Ircut_pro)
			{
				g_Ircut_pro = 0;
				pthread_join(g_SmartIrcut_pro, NULL);
			}
			pthread_mutex_lock(&g_smartIRMutex);
			ret = _image_dn_switch(AISP_DN_MODE_DAY);
			if (RETURN_OK != ret)
			{
				PRINT_ERROR("_image_dn_switch failed %#x\n", ret);
			}
			pthread_mutex_unlock(&g_smartIRMutex);
			break;
		case IRCUT_MODE_NIGHT:
			if (g_Ircut_pro)
			{
				g_Ircut_pro = 0;
				pthread_join(g_SmartIrcut_pro, NULL);
			}
			pthread_mutex_lock(&g_smartIRMutex);
			ret = _image_dn_switch(AISP_DN_MODE_NIGHT);
			if (RETURN_OK != ret)
			{
				PRINT_ERROR("_image_dn_switch failed %#x\n", ret);
			}
			pthread_mutex_unlock(&g_smartIRMutex);
			break;
		case IRCUT_MODE_AUTO:
			//由于原厂SDK调整，接口设置增益值无效，需调整灵敏度设置
			_isp_get_dn_gain_thr(gainDay2Night, gainNight2Day, &D2nGainThr, &N2dGainThr);
			pstDayNight.stDnAttr.u16D2nValue = D2nGainThr;
			pstDayNight.stDnAttr.u16N2dValue = N2dGainThr;

			ret = NI_MDK_AISP_SetDayNight(IspDev, &pstDayNight);
			if (RETURN_OK != ret)
			{
				PRINT_ERROR("NI_MDK_AISP_SetDayNight failed %#x\n", ret);
				return -1;
			}

			if (0 == g_Ircut_pro)
			{
				g_Ircut_pro = 1;
				pthread_create(&g_SmartIrcut_pro, NULL, (void *)_image_smart_ircut_pro, NULL);
			}
			break;
		default:
			PRINT_ERROR("this mode [%d] not support\n",mode);
			return -1;
	}

	return 0;
}

//获取日夜状态
int ImageGetDayStatus(DAY_STATUS_e* status)
{
	//int cur_status;
	pthread_mutex_lock(&g_smartIRMutex);
	if (AISP_DN_MODE_DAY == irStaCurr)
	{
		usleep(300*1000);
		*status = DAY_STATUS_DAY;
	}
	else if (AISP_DN_MODE_NIGHT == irStaCurr)
	{
		usleep(300 * 1000);
		*status = DAY_STATUS_NIGHT;
	}
	else
	{
		usleep(300 * 1000);
		*status = DAY_STATUS_DAY;
	}
	pthread_mutex_unlock(&g_smartIRMutex);
	return 0;
}

//设置IRCUT
//提供给XS内部使用
int ircut(ISP_DEV IspDev, unsigned int state)
{
	static int last_state = -1;

	if(last_state != state && DISABLE == cover)
	{
		if (ENABLE == state)
		{
			Ircut(DISABLE);
			ircut_state = DISABLE;
		}
		else
		{
			Ircut(ENABLE);
			ircut_state = ENABLE;
		}
		last_state = state;
	}
	usleep(0);
	return 0;
}

//设置补光灯
//提供给XS内部使用
int ir_light(ISP_DEV IspDev, AISP_LIGHT_RESULT_S *pstIrLight)
{
	LightState light;
	static int strength = -1;
	memset(&light, 0, sizeof(light));
	light.code = faceboardInfraredLight;
	light.strength = pstIrLight->au32LightLevel[0] / 100;

	if (strength != pstIrLight->au32LightLevel[0] && DISABLE == cover)
	{
		if(0 == pstIrLight->au32LightLevel[0])
		{
			light.state = DISABLE;
			LightControl(light);
			irlight_state = DISABLE;
			PRINT_ERROR("ir_light irlight_state %d\n", irlight_state);
		}
		else
		{
			light.state = ENABLE;
			LightControl(light);
			irlight_state = ENABLE;
			PRINT_ERROR("ir_light irlight_state %d\n", irlight_state);
		}
		strength = pstIrLight->au32LightLevel[0];
		light_state.light_strength.InfraredLight = light.strength;
	}

	usleep(0);
	return 0;
}

//设置补光灯亮度
int ImageSetLightStrength(LightCode lightcode, unsigned int strength)
{
	int ret = -1;
	LightState light;
	memset(&light, 0, sizeof(light));

	switch (lightcode)
	{
		case (faceboardInfraredLight):
			light_state.light_strength.InfraredLight = strength;
			ret = RETURN_OK;
			break;
		case (faceBoardFlashLight):
			if (light_state.light_strength.FlashLight != strength && ENABLE == light_state.FlashLight_state)
			{
				light.code = faceBoardFlashLight;
				light.strength = strength;
				light.state = ENABLE;
				LightControl(light);
			}
			light_state.light_strength.FlashLight = strength;
			ret = RETURN_OK;
			break;
		default:
			PRINT_ERROR("ImageSetLightStrength lightcode %d is not support\n", lightcode);
			break;
	}

	return ret;
}

//设置夜间模式
int ImageSetNightVision(ISP_DAYNIGHT_VISION_E enVision)
{
	if (daynight_vision != enVision)
	{
		pthread_mutex_lock(&g_smartIRMutex);

		daynight_vision = enVision;

		pthread_mutex_unlock(&g_smartIRMutex);
	}

	return 0;
}

int ImageSetHideState(BOOL state)
{
	int ret = -1;
	LightState light;
	ISP_DEV IspDev = 0;
	AISP_MOD_RUN_U punModeRun;

	memset(&light, 0, sizeof(light));
	memset(&punModeRun, 0, sizeof(AISP_MOD_RUN_U));

	light.code = faceboardInfraredLight;
	light.strength = light_state.light_strength.InfraredLight;

	ret = NI_MDK_AISP_GetModRunState(IspDev, &punModeRun);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_GetModRunState failed %#x\n", ret);
	}

	if (DISABLE == state)
	{
		PRINT_ERROR("ImageSetHideState irlight_state %d\n", irlight_state);
		if (ENABLE == irlight_state)
		{
			PRINT_ERROR("ImageSetHideState LightControl faceboardInfraredLight DISABLE\n");
			light.state = DISABLE;
			LightControl(light);
		}

		punModeRun.bitAispState = 0;

		ret = NI_MDK_AISP_SetModRunState(IspDev, &punModeRun);
		if (RETURN_OK != ret)
		{
			PRINT_ERROR("NI_MDK_AISP_SetModRunState failed %#x\n", ret);
		}
		cover = ENABLE;
	}
	else if (ENABLE == state)
	{
		PRINT_ERROR("ImageSetHideState irlight_state %d\n", irlight_state);
		if (ENABLE == irlight_state)
		{
			PRINT_ERROR("ImageSetHideState LightControl faceboardInfraredLight ENABLE\n");
			light.state = ENABLE;
			LightControl(light);
		}

		punModeRun.bitAispState = 1;

		ret = NI_MDK_AISP_SetModRunState(IspDev, &punModeRun);
		if (RETURN_OK != ret)
		{
			PRINT_ERROR("NI_MDK_AISP_SetModRunState failed %#x\n", ret);
		}
		cover = DISABLE;
	}

	return 0;
}


//设置日夜切换抖动
int ImageSetJitter(int channel, unsigned int jitter_time)
{
	int ret = -1;
	ISP_DEV IspDev = 0;
	AISP_APP_DAY_NIGHT_S pstDayNight;

	memset(&pstDayNight, 0, sizeof(AISP_APP_DAY_NIGHT_S));
	if(jitter_time < 2 || jitter_time > 10)
	{
		PRINT_ERROR("this jitter_time [%d] not support\n",jitter_time);
		return -1;
	}

	ret = NI_MDK_AISP_GetDayNight(IspDev, &pstDayNight);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_GetDayNight failed %#x\n", ret);
		return -1;
	}
	//日夜切换的抖动时间单位是ms
	pstDayNight.stDnAttr.u16Delay = jitter_time * 1000;

	ret = NI_MDK_AISP_SetDayNight(IspDev, &pstDayNight);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_SetDayNight failed %#x\n", ret);
		return -1;
	}

	return 0;
}

//设置自动白平衡增益
int ImageSetAwbGain(int channel, AWB_MODE_e mode, unsigned int r, unsigned int g, unsigned int b)
{
	int ret = -1;
	int type = -1;
	ISP_DEV IspDev = 0;
	AISP_APP_WB_S pstWb;
	AISP_MOD_RUN_U punModeRun;
	SensorDevice_p pSensorDevice = &GlobalDevice.SensorDevice;
	memset(&pstWb, 0, sizeof(AISP_APP_WB_S));

	if (SEN_TYPE_GC4023 == pSensorDevice->SenType)
	{
		type = 0;
	}
	else if (SEN_TYPE_OS04L10 == pSensorDevice->SenType)
	{
		type = 1;
	}
	else if (SEN_TYPE_OS04D10 == pSensorDevice->SenType)
	{
		type = 2;
	}
	else
	{
		type = 0;
		PRINT_ERROR("unknown sensor type %d\n", pSensorDevice->SenType);
	}

	ret = NI_MDK_AISP_GetWbAttr(IspDev, &pstWb);
	if (RETURN_OK != ret)
	{	
		PRINT_ERROR("NI_MDK_AISP_GetWbAttr failed %#x\n",ret);
		return -1;
	}

	ret = NI_MDK_AISP_GetModRunState(IspDev, &punModeRun);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_GetModRunState failed %#x\n", ret);
	}

	switch(mode)
	{
		case AWB_MODE_AUTO:
			punModeRun.bitAwbState = 1;
			pstWb.enSceneInfo = AISP_WB_SCENE_NORMAL;
			pstWb.stUsrDefGain.bEnable = 0;
			break;
		case AWB_MODE_LOCK:
			punModeRun.bitAwbState = 0;
			break;
		case AWB_MODE_WHITE:
			punModeRun.bitAwbState = 1;
			pstWb.enSceneInfo = AISP_WB_SCENE_NORMAL;
			pstWb.stUsrDefGain.bEnable = 1;
			pstWb.stUsrDefGain.u32Bgain = gain[type][0].Bgain;
			pstWb.stUsrDefGain.u32Rgain = gain[type][0].Rgain;
			break;
		case AWB_MODE_WARM:
			punModeRun.bitAwbState = 1;
			pstWb.enSceneInfo = AISP_WB_SCENE_NORMAL;
			pstWb.stUsrDefGain.bEnable = 1;
			pstWb.stUsrDefGain.u32Bgain = gain[type][1].Bgain;
			pstWb.stUsrDefGain.u32Rgain = gain[type][1].Rgain;
			break;
		case AWB_MODE_NATURAL:
			punModeRun.bitAwbState = 1;
			pstWb.enSceneInfo = AISP_WB_SCENE_NORMAL;
			pstWb.stUsrDefGain.bEnable = 1;
			pstWb.stUsrDefGain.u32Bgain = gain[type][2].Bgain;
			pstWb.stUsrDefGain.u32Rgain = gain[type][2].Rgain;
			break;
		case AWB_MODE_SUN:
			punModeRun.bitAwbState = 1;
			pstWb.enSceneInfo = AISP_WB_SCENE_NORMAL;
			pstWb.stUsrDefGain.bEnable = 1;
			pstWb.stUsrDefGain.u32Bgain = gain[type][3].Bgain;
			pstWb.stUsrDefGain.u32Rgain = gain[type][3].Rgain;
			break;
		case AWB_MODE_MANUAL:
			punModeRun.bitAwbState = 1;
			pstWb.enSceneInfo = AISP_WB_SCENE_NORMAL;
			pstWb.stUsrDefGain.bEnable = 1;
			pstWb.stUsrDefGain.u32Bgain = b;
			pstWb.stUsrDefGain.u32Rgain = r;
			break;
		default:
			PRINT_ERROR("this mode [%d] not support\n",mode);
			return -1;
	}

	ret = NI_MDK_AISP_SetModRunState(IspDev, &punModeRun);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_GetModRunState failed %#x\n", ret);
	}

	ret = NI_MDK_AISP_SetWbAttr(IspDev, &pstWb);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_GetWbAttr failed %#x\n", ret);
		return -1;
	}

	return 0;
}

//设置2d降噪或3d降噪
//因降噪效果问题，暂关闭该接口设置
int ImageSetNr2Or3(int channel, NR_MODE_e mode, unsigned int air_level, unsigned int time_level, unsigned int mtd_level)
{
	int ret = -1;
	ISP_DEV IspDev = 0;
	AISP_APP_NR_S pstNr;

	memset(&pstNr, 0, sizeof(AISP_APP_NR_S));

	ret = NI_MDK_AISP_GetNrAttr(IspDev, &pstNr);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_GetNrAttr failed %#x\n",ret);
		return -1;
	}

	switch(mode)
	{
		case NR_MODE_SHUT:
			pstNr.stNry2d.bEnable = 0;
			pstNr.stNry3d.bEnable = 0;
			break;
		case NR_MODE_2D:
			pstNr.stNry2d.bEnable = 1;
			pstNr.stNry2d.u8Level = air_level;
			pstNr.stNry3d.bEnable = 0;
			break;
		case NR_MODE_3D:
			pstNr.stNry2d.bEnable = 1;
			pstNr.stNry2d.u8Level = air_level;
			pstNr.stNry3d.bEnable = 1;
			pstNr.stNry3d.u8Level = time_level;
			break;
		default:
			PRINT_ERROR("this mode [%d] not support\n",mode);
			return -1;
	}

	ret = NI_MDK_AISP_SetNrAttr(IspDev, &pstNr);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_SetNrAttr failed %#x\n", ret);
		return -1;
	}

	return 0;
}

//设置去紫边使能
int ImageSetAntiPurpleBoundary(int channel, unsigned int mode, unsigned int level)
{
	int ret = -1;
	ISP_DEV IspDev = 0;
	PISP_CNR_PARAM_S pstCnrParam;

	memset(&pstCnrParam, 0, sizeof(PISP_CNR_PARAM_S));

	if (level > 100)
	{
		PRINT_ERROR("this level [%d] not support\n",level);
		return -1;
	}

	ret = NI_MDK_PISP_GetCnrParam(IspDev, &pstCnrParam);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_PISP_GetCnrParam failed %#x\n", ret);
		return -1;
	}

	switch(mode)
	{
		case 0:
			pstCnrParam.bCnrEn = 0;
			break;
		case 1:
			pstCnrParam.bCnrEn = 1;
			break;
		default:
			PRINT_ERROR("this mode [%d] not support\n",mode);
			return -1;
	}

	ret = NI_MDK_PISP_SetCnrParam(IspDev, &pstCnrParam);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_PISP_SetCnrParam failed %#x\n", ret);
		return -1;
	}

	return 0;
}

//设置坏点矫正
int ImageSetDynamicDpc(int channel, unsigned int mode, unsigned int level)
{
	int ret = -1;
	ISP_DEV IspDev = 0;
	PISP_DPC_PARAM_S pstDpcParam;

	memset(&pstDpcParam, 0, sizeof(PISP_DPC_PARAM_S));

	if (level > 100)
	{
		PRINT_ERROR("this level [%d] not support\n",level);
		return -1;
	}

	ret = NI_MDK_PISP_GetDpcParam(IspDev, &pstDpcParam);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_PISP_GetDpcParam failed %#x\n", ret);
		return -1;
	}

	switch(mode)
	{
		case 0:
			pstDpcParam.bDpcEn = 0;
			break;
		case 1:
			pstDpcParam.bDpcEn = 1;
			break;
		default:
			PRINT_ERROR("this mode [%d] not support\n",mode);
			return -1;
	}

	ret = NI_MDK_PISP_SetDpcParam(IspDev, &pstDpcParam);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_PISP_SetDpcParam failed %#x\n", ret);
		return -1;
	}

	return 0;
}

/// 得到ISP支持设置的特性。
///
/// 
/// \retval 0  获取成功。
/// \retval <0  获取失败。
int ImageGetIspCaps(ISP_IMAGE_CAPS *pCaps)
{
	pCaps->Nr3dEnabled = 1;

	return 0;
}
