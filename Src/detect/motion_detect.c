//#include "MotionDetect.h"
#include "TdCommon.h"


PERIMETER_DETECT_PARAM_S gstPerimeterParam;
Quadrilateral gstMonitorArea[2] = 
{
	{
		.Points[0].x = 0,
		.Points[0].y = 0,
		.Points[1].x = 480,
		.Points[1].y = 0,
		.Points[2].x = 480,
		.Points[2].y = 320,
		.Points[3].x = 0,
		.Points[3].y = 320
	},
	{
		.Points[0].x = 0,
		.Points[0].y = 0,
		.Points[1].x = 480,
		.Points[1].y = 0,
		.Points[2].x = 480,
		.Points[2].y = 320,
		.Points[3].x = 0,
		.Points[3].y = 320
	},
};
unsigned int gTrackObject[2] = {0};
int pu32RuleId[2] = {-1, -1};

void vd_attr_init(MD_CHN_ATTR_S *pstVdChnAttr)
{
	BlindDetectDevice_p pBlindDetectDevice = &GlobalDevice.BlindDetectDevice;
	MotionDetectDevice_p pMotionDetectDevice = &GlobalDevice.MotionDetectDevice;

	pstVdChnAttr->u32Width = 352;
	pstVdChnAttr->u32Height = 288;
	pstVdChnAttr->u32Interval = pMotionDetectDevice->md_cf.md_config.framedelay;
	pstVdChnAttr->u32DetPerFrm = pMotionDetectDevice->md_cf.md_config.framedelay;
	pstVdChnAttr->stBdAttr.enBdMode = MD_BD_MODE_LUMA;
	pstVdChnAttr->stBdAttr.u8BdGate = 0x50;
	pstVdChnAttr->stBdAttr.u8BdPixelBlackGate = pBlindDetectDevice->cd_config.threshold;
	pstVdChnAttr->stBdAttr.u8BdPixelStep = 0x1f;
	pstVdChnAttr->stMdAttr.stAomParam[0].bAomEnable = NI_TRUE;
	pstVdChnAttr->stMdAttr.stAomParam[0].u8AomSense = pMotionDetectDevice->md_cf.md_config.threshold;
	pstVdChnAttr->stMdAttr.stAomParam[0].u8AomDiffCoe = 0xf;
	pstVdChnAttr->stMdAttr.stAomParam[0].u16AomMdthrd = 0xa;
	pstVdChnAttr->stMdAttr.stAomParam[0].u16AomDiffLevel = 0x100;
	pstVdChnAttr->stMdAttr.stAomParam[1].bAomEnable = NI_TRUE;
	pstVdChnAttr->stMdAttr.stAomParam[1].u8AomSense = pMotionDetectDevice->md_cf.md_config.threshold;
	pstVdChnAttr->stMdAttr.stAomParam[1].u8AomDiffCoe = 0xf;
	pstVdChnAttr->stMdAttr.stAomParam[1].u16AomMdthrd = 0xa;
	pstVdChnAttr->stMdAttr.stAomParam[1].u16AomDiffLevel = 0x100;
	pstVdChnAttr->stMdAttr.stAomParam[2].bAomEnable = NI_TRUE;
	pstVdChnAttr->stMdAttr.stAomParam[2].u8AomSense = pMotionDetectDevice->md_cf.md_config.threshold;
	pstVdChnAttr->stMdAttr.stAomParam[2].u8AomDiffCoe = 0xf;
	pstVdChnAttr->stMdAttr.stAomParam[2].u16AomMdthrd = 0xa;
	pstVdChnAttr->stMdAttr.stAomParam[2].u16AomDiffLevel = 0x100;
	pstVdChnAttr->stMdAttr.stAomParam[3].bAomEnable = NI_TRUE;
	pstVdChnAttr->stMdAttr.stAomParam[3].u8AomSense = pMotionDetectDevice->md_cf.md_config.threshold;
	pstVdChnAttr->stMdAttr.stAomParam[3].u8AomDiffCoe = 0xf;
	pstVdChnAttr->stMdAttr.stAomParam[3].u16AomMdthrd = 0xa;
	pstVdChnAttr->stMdAttr.stAomParam[3].u16AomDiffLevel = 0x100;
	pstVdChnAttr->stMdAttr.stMdParam.u8MdNoiseSlowCoe = 0x02;
	pstVdChnAttr->stMdAttr.stMdParam.u8MdNoiseUpCoe = 0x1a;
	pstVdChnAttr->stMdAttr.stMdParam.u8MdNoiseToDiffCoe = 0x3;
	pstVdChnAttr->stMdAttr.stMdParam.u8MdStudyFrameNum = 0x00;
	pstVdChnAttr->stMdAttr.stSbParam.u8SbHorizontalNum = 0x16;
	pstVdChnAttr->stMdAttr.stSbParam.u8SbVerticalNum = 0x12;
}

/// 创建动态检测设备
/// 
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int MotionDetectCreate(void)
{
	int i = 0;
	int ret = 0;
	int state = -1;
	int STATE = 0x01;
	MD_CHN VdChn = 0;
	MD_CHN_ATTR_S stMdChnAttr;
	MD_WORK_MODE_S stWorkMode;
	MD_DETECT_REGION_S stDetectRegion[MD_MAX_DETECT_REGION];

	MD_CD_Check_p check = &GlobalDevice.md_cd_check;
		
	memset(&stWorkMode, 0, sizeof(MD_WORK_MODE_S));
	memset(&stMdChnAttr, 0, sizeof(MD_CHN_ATTR_S));
	memset(&stDetectRegion, 0, sizeof(MD_DETECT_REGION_S));

	state = (STATE & check->md_cd_state);
	if (state != 0)
	{
		PRINT_ERROR("[ERROR]: MotionDetect already exits\n");
		return -1;
	}

	if (0 == check->init)
	{
		vd_attr_init(&stMdChnAttr);

		ret = NI_MDK_MD_Init(VdChn, &stMdChnAttr);
		if (RETURN_OK != ret)
		{
			PRINT_ERROR("[ERROR]: NI_MDK_MD_Init failed, ret=%x\n", ret);
			return ret;
		}

		//for (i = 0; i < VD_MAX_DETECT_REGION; i++)
		{
			stDetectRegion[i].stDetectRect.s32BeginX = 0;
			stDetectRegion[i].stDetectRect.s32BeginY = 0;
			stDetectRegion[i].stDetectRect.s32EndX = stMdChnAttr.u32Width - 1;
			stDetectRegion[i].stDetectRect.s32EndY = stMdChnAttr.u32Height - 1;
			stDetectRegion[i].u32AomId = i;
			stDetectRegion[i].bRectDetect = NI_TRUE;
		}

		ret = NI_MDK_MD_SetDetectRegion(VdChn, &stDetectRegion[0]);
		if (RETURN_OK != ret)
		{
			PRINT_ERROR("[ERROR]: NI_MDK_MD_SetDetectRegion failed, ret=%x\n", ret);
			return ret;
		}

		//这边申请内存时，需关注申请内存的大小，7302上1个字节有2个格子的数据，而520ce上1个字节只有1个格子的数据
		if(NULL == check->md_data)
		{
			check->md_data = malloc(stMdChnAttr.stMdAttr.stSbParam.u8SbHorizontalNum * stMdChnAttr.stMdAttr.stSbParam.u8SbVerticalNum + 1);
			if(NULL == check->md_data)
			{
				PRINT_ERROR("malloc error\n");
				return -1;
			}
			memset(check->md_data, 0, stMdChnAttr.stMdAttr.stSbParam.u8SbHorizontalNum * stMdChnAttr.stMdAttr.stSbParam.u8SbVerticalNum + 1);
		}
		check->init = 1;
	}

	stWorkMode.enDetectSel = MD_DETECT_SEL_BD_MD;
	// stWorkMode.bDiffSumEn = NI_FALSE;
	// stWorkMode.bPixLumaSumEn = NI_FALSE;
	ret = NI_MDK_MD_SetWorkMode(VdChn, &stWorkMode);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("[ERROR]: NI_MDK_VD_GetChnAttr failed, ret=%x\n", ret);
		return ret;
	}
	check->md_cd_state = (STATE | check->md_cd_state);

	if (0 == check->md_cd_is_running)
	{
		check -> md_cd_is_running = 1;
		pthread_t Thread;
		pthread_attr_t Attr;
		pthread_attr_init(&Attr);
		pthread_attr_setdetachstate(&Attr, PTHREAD_CREATE_DETACHED);
		ret = pthread_create(&Thread, &Attr, (void *)MD_CD_check, NULL);
		if (ret)
		{
			PRINT_ERROR("[ERROR]: create md/cd thread failed, ret=%d\n", ret);
			check->md_cd_is_running = 0;
			return ret;
		}
		check -> request_to_stop = 0;
	}

	return ret;
}


/// 销毁动态检测设备
/// 
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int MotionDetectDestory(void)
{
	VD_CHN VdChn = 0;
	MD_WORK_MODE_S stWorkMode;
	MD_CD_Check_p check = &GlobalDevice.md_cd_check;

	if(check->md_cd_is_running)
	{
		pthread_mutex_lock(&check->MD_CD_lock);
		check->md_cd_is_running = 0;
		check->md_cd_state = 0;
		pthread_mutex_unlock(&check->MD_CD_lock);
		while(1)
		{
			if(1 == check->request_to_stop)
			{
				break;
			}
			else
			{
				usleep(20*1000);
			}
		}
		memset(&GlobalDevice.md_cd_check, 0, sizeof(MD_CD_Check_t));

		stWorkMode.enDetectSel = MD_DETECT_SEL_MD;
		// stWorkMode.bDiffSumEn = NI_FALSE;
		// stWorkMode.bPixLumaSumEn = NI_FALSE;
		NI_MDK_MD_SetWorkMode(VdChn, &stWorkMode);

		stWorkMode.enDetectSel = MD_DETECT_SEL_BD;
		NI_MDK_MD_SetWorkMode(VdChn, &stWorkMode);
		NI_MDK_MD_DeInit(VdChn);
	}
	else
	{
		PRINT_ERROR("[ERROR]: MotionDetect has been destroyed\n");
	}
	return 0;
}


/// 执行动态检测。立即返回。
/// 
/// \param [out] pData 指向一个DWORD值的指针，DWORD值是各个报警输入通道电平状态
///        的掩码。低通道在低位，高通道在高位。高电平置1，低电平置0，不存在的通
///        道置0。
/// \retval 0  检测成功
/// \retval <0  检测失败
int MotionDetectGetState(DWORD* pData)
{
    return 0;
}


/// 动态检测捕获的详细结果。
/// 
/// \param [in] channel 通道号。
/// \param [in] pResult 指向动态检测参数结构MOTION_DETECT_RESULT的指针。
/// \retval 0  检测到移动
/// \retval <0  未检测到移动
int MotionDetectGetResult(int channel, MOTION_DETECT_RESULT *pResult)
{
	int i;
	int j;
	int sum = 0;
	int ret = -1;
	MD_CHN VdChn = 0;
	unsigned int height;
	unsigned int width;
	unsigned char motion_flag = 0;

	MD_CHN_ATTR_S stVdChnAttr;
	MD_CD_Check_p check = &GlobalDevice.md_cd_check;
	MotionDetectDevice_p pMotionDetectDevice = &GlobalDevice.MotionDetectDevice;

	if (DISABLE == pMotionDetectDevice->md_cf.md_config.enable || NULL == check->md_data)
	{
		return ret;	
	}

	memset(&stVdChnAttr,0,sizeof(MD_CHN_ATTR_S));
	ret = NI_MDK_MD_GetChnAttr(VdChn, &stVdChnAttr);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("[ERROR]: NI_MDK_VD_GetChnAttr failed, ret=%x\n", ret);
		return ret;
	}

	width = stVdChnAttr.stMdAttr.stSbParam.u8SbHorizontalNum;
	height = stVdChnAttr.stMdAttr.stSbParam.u8SbVerticalNum;
	pthread_mutex_lock(&check->MD_CD_lock);
	
	for(i = 0; i < height; i ++)
	{
		for (j = 0; j < width; j ++)
		{
			motion_flag = *(check->md_data + i * width  + j);

			if (motion_flag & 0x01)
			{
				pResult->win[i] |= (1 << j);
			}
		}
		pResult->win[i] &= pMotionDetectDevice->md_re.win[i];
	}
	
	pthread_mutex_unlock(&check->MD_CD_lock);

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			if((pResult->win[i] >> j) & 0x01)
			{
				sum++;
			}
		}
	}

	//这边设置为块数，最少为3块，最多为13块
	if (sum >= (int)(3 + (101 - pMotionDetectDevice->md_cf.md_config.threshold) / 10))
	{
		ret = 0;
	}
	else
	{
		ret = -1;
	}
	
	return ret;
}

/// 设置是否在动态检测成功的区域给予提示。
/// 
/// \param [in] channel 通道号。
/// \param [in] enable 为1表示提示，为0时表示不提示。
/// \retval 0  设置成功
/// \retval <0  设置失败
int MotionDetectShowHint(int channel, int enable)
{
    return 0;
}


/// 执行动态检测。立即返回。
/// 
/// \param [in] channel 通道号。
/// \param [in] pParam 指向动态检测参数结构MOTION_DETECT_PARAM的指针。
/// \retval 0  设置成功
/// \retval <0  设置失败
int MotionDetectSetParameter(int channel, MOTION_DETECT_PARAM *pParam)
{
	//int i;
	int ret = 0;
	int STATE = 0x01;
	//VD_CHN VdChn = 0;
	unsigned int height = 18;
	unsigned int width = 22;
	//unsigned int level = 0;
	//VD_CHN_ATTR_S stVdChnAttr;
	MD_WORK_MODE_S stWorkMode;
	MD_CD_Check_p check = &GlobalDevice.md_cd_check;
	MotionDetectDevice_p pMotionDetectDevice = &GlobalDevice.MotionDetectDevice;	//动检参数设置

	if (pParam->sensitiveness > 100 || pParam->sensitiveness < 0 || NULL == check->md_data)
	{
		PRINT_ERROR("Please input right Level(between 0-100)! Level:%d\n", pParam->sensitiveness);
		return -1;
	}

	//memset(&stVdChnAttr, 0, sizeof(VD_CHN_ATTR_S));
	memset(&stWorkMode, 0, sizeof(MD_WORK_MODE_S));
	stWorkMode.enDetectSel = MD_DETECT_SEL_MD;
	// stWorkMode.bDiffSumEn = NI_FALSE;
	// stWorkMode.bPixLumaSumEn = NI_FALSE;
	switch (pParam->enable)
	{
		case(0):
			if (check->md_cd_state & STATE)
			{
				/*ret = NI_MDK_VD_StopDetect(VdChn, &stWorkMode);
				if (RETURN_OK != ret)
				{
					PRINT_ERROR("[ERROR]: NI_MDK_VD_StopDetect failed, ret=%x\n", ret);
				}*/
				check->md_cd_state ^= STATE;
			}
			pthread_mutex_lock(&check->MD_CD_lock);
			memset(check->md_data, 0, height * width / 2);
			pthread_mutex_unlock(&check->MD_CD_lock);
			return ret;
		case(1):
			if (!(check->md_cd_state & STATE))
			{
				/*ret = NI_MDK_VD_StartDetect(VdChn, &stWorkMode);
				if (RETURN_OK != ret)
				{
					PRINT_ERROR("[ERROR]: NI_MDK_VD_StartDetect failed, ret=%x\n", ret);
				}*/
				check->md_cd_state |= STATE;
			}
			break;
		default:
			PRINT_ERROR("Please input right state(0 or 1)!\n");
			return -1;
	}

	/*ret = NI_MDK_VD_GetChnAttr(VdChn, &stVdChnAttr);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("[ERROR]: NI_MDK_VD_GetChnAttr failed, ret=%x\n", ret);
		return ret;
	}

	level = (int)((float)127 / 100 * pParam->sensitiveness);
	for (i = 0; i < VD_MAX_DETECT_REGION; i++)
	{
		stVdChnAttr.stMdAttr.stAomParam[i].u8AomSense = level;
	}

	ret = NI_MDK_VD_SetChnAttr(VdChn, &stVdChnAttr);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("[ERROR]: NI_MDK_VD_SetChnAttr failed, ret=%x\n", ret);
		return ret;
	}*/

	memset(&pMotionDetectDevice->md_re, 0, sizeof(MOTION_DETECT_RESULT));
	pMotionDetectDevice->md_cf.ilevel = pParam->iLevel; //灵敏度档次
	memcpy(pMotionDetectDevice->md_re.win, pParam->win, sizeof(pParam->win));
	pMotionDetectDevice->md_cf.md_config.threshold = pParam->sensitiveness; //灵敏度具体值
	pMotionDetectDevice->md_cf.md_config.framedelay = 1;		  //帧间隔
	pMotionDetectDevice->md_cf.md_config.enable = pParam->enable; //使能开关
	//for (int i = 0; i < 18; i++)
	//{
	//	printf("md_re [%d]%d\n",i,pMotionDetectDevice->md_re.win[i]);
	//}
	return 0;
}

/// 创建人形检测设备
/// 
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int HumanDetectCreate(void)
{
	ProductType_e Type;
	Type = SystemGetProductType();

	if (IPC_CH7V20 == Type || IPC_RH7V20 == Type || IPC_CH7WV10 == Type || IPC_TC3H15CV10 == Type)
	{
		if (HumanTrackInit() != RETURN_OK)
		{
			PRINT_ERROR("Sample Human Track Start failed\n");
			return FAILED;
		}
	}
	else
	{
		if (human_detect_start() != RETURN_OK)
		{
			PRINT_ERROR("Sample Human Detect Start failed\n");
			return FAILED;
		}
	}
	//这边增加500ms延迟以保证人形初始化完成，防止后续设置人形和车型灵敏度出现问题
	usleep(500 * 1000);
	return SUCCESS;
}

/// 销毁人形检测设备
/// 
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int HumanDetectDestory(void)
{
	ProductType_e Type;
	Type = SystemGetProductType();

	if (IPC_CH7V20 == Type || IPC_RH7V20 == Type || IPC_CH7WV10 == Type || IPC_TC3H15CV10 == Type)
	{
		if (HumanTrackDeInit() != RETURN_OK)
		{
			PRINT_ERROR("Sample Human Track Stop failed\n");
			return FAILED;
		}
	}
	else
	{
		if (human_detect_stop() != RETURN_OK)
		{
			PRINT_ERROR("Sample Human Detect Stop failed\n");
			return FAILED;
		}
	}

	return SUCCESS;
}

/// 设置人形跟踪的开关。
///
/// \param [in] channel 通道号。
/// \param [in] Enable 开关
/// \param [in] MoveKpx 转动角度
/// \retval 0  设置成功
/// \retval <0  设置失败
int HumanTrackSetParameter(int channel, unsigned int Enable, unsigned int MoveKpx)
{
	if (HumanTrackRun(Enable) != RETURN_OK)
	{
		PRINT_ERROR("PtzTrackingSetParameter failed\n");
		return FAILED;
	}

	/*if (HumanTrackSetParam(MoveKpx) != RETURN_OK)
	{
		PRINT_ERROR("HumanTrackSetParam failed\n");
		return FAILED;
	}*/

	return SUCCESS;
}

/// 设置人形检测的参数。立即返回。
/// 
/// \param [in] channel 通道号。
/// \param [in] pParam 指向动态检测参数结构HUMAN_DETECT_PARAM的指针。
/// \retval 0  设置成功
/// \retval <0  设置失败
int HumanDetectSetParameter(int channel, HUMAN_DETECT_PARAM *pParam)
{
	int ret = -1;
	IAS_ALG_CHN OdChn = 0;
	IAS_PARAMS_S pastArr;
	IAS_ALG_OD_PARAM_LIST_S pstAlgParam;
	HumanDetectDevice_p pHumanDetectDevice = &GlobalDevice.HumanDetectDevice;
	memset(&pastArr, 0, sizeof(IAS_PARAMS_S));
	memset(&pstAlgParam, 0, sizeof(IAS_ALG_OD_PARAM_LIST_S));

	pHumanDetectDevice->human_param[channel].Enable = pParam->Enable;

	pastArr.s32ByteSize = 4;
	pastArr.s32Type = IAS_OD_PARAM_OD_SCORE_THRE;
	
	//这边将40灵敏度以下的调整到600+，原厂反馈灵敏度调节到500以上可以降低误报率
    //APP设置低灵敏度对应值为40
	if (pParam->Level <= 40)
	{
		pastArr.as32Rev[0] = 640 - pParam->Level;
	}
	else
	{
		pastArr.as32Rev[0] = (int)500 - ((float)pParam->Level / 100 * 250);
	}

	pstAlgParam.u32ParamNum = 1;
	pstAlgParam.pastArr = &pastArr;
	
	ret = NI_IAS_OD_SetParams(OdChn, (NI_VOID *)(&pstAlgParam));
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("[ERROR]: NI_IAS_OD_SetParams failed, ret=%x\n", ret);
		return ret;
	}
	return 0;
}

/// 人形检测捕获的详细结果。
/// 
/// \param [in] channel 通道号。
/// \param [in] pResult 指向动态检测参数结构HUMAN_DETECT_RESULT的指针。
/// \retval 0  检测到移动人形
/// \retval <0  未检测到移动人形
int HumanDetectGetResult(int channel, HUMAN_DETECT_RESULT *pResult)
{
	int ret = -1;
	HumanDetectDevice_p pHumanDetectDevice = &GlobalDevice.HumanDetectDevice;
	
	pthread_rwlock_wrlock(&pHumanDetectDevice->result_lock);
	if (pHumanDetectDevice->human_param[channel].Enable)
	{
		pResult->result = pHumanDetectDevice->human_result[channel].Result;
		pResult->left = pHumanDetectDevice->human_result[channel].x1;
		pResult->top = pHumanDetectDevice->human_result[channel].y1;
		pResult->right = pHumanDetectDevice->human_result[channel].x2;
		pResult->bottom = pHumanDetectDevice->human_result[channel].y2;
	}
	else
	{
		pResult->result = 0;
		pResult->left = 0;
		pResult->top = 0;
		pResult->right = 0;
		pResult->bottom = 0;
	}

	// 上层获取检测结果后清零
	memset(&pHumanDetectDevice->human_result[channel], 0, sizeof(ias_result_t));

	pthread_rwlock_unlock(&pHumanDetectDevice->result_lock);
	if(pResult->result)
	{
		ret = 0;
	}

	return ret;
}


/// 创建车型检测设备
///
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int VehicleDetectCreate(void)
{
	return 0;
}

/// 销毁车型检测设备
///
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int VehicleDetectDestory(void)
{
	return 0;
}

/// 设置车型检测的参数。立即返回。
///
/// \param [in] channel 通道号。
/// \param [in] pParam 指向动态检测参数结构HUMAN_DETECT_PARAM的指针。
/// \retval 0  设置成功
/// \retval <0  设置失败
int VehicleDetectSetParameter(int channel, VEHICLE_DETECT_PARAM *pParam)
{
	int ret = -1;
	IAS_ALG_CHN OdChn = 0;
	IAS_PARAMS_S pastArr;
	IAS_ALG_OD_PARAM_LIST_S pstAlgParam;
	HumanDetectDevice_p pHumanDetectDevice = &GlobalDevice.HumanDetectDevice;
	memset(&pastArr, 0, sizeof(IAS_PARAMS_S));
	memset(&pstAlgParam, 0, sizeof(IAS_ALG_OD_PARAM_LIST_S));

	pHumanDetectDevice->vehicle_param[channel].Enable = pParam->Enable;

	pastArr.s32ByteSize = 4;
	pastArr.s32Type = IAS_OD_PARAM_OD_SCORE_THRE;

	// 这边将40灵敏度以下的调整到600+，原厂反馈灵敏度调节到500以上可以降低误报率
	// APP设置低灵敏度对应值为40
	if (pParam->Level <= 40)
	{
		pastArr.as32Rev[0] = 640 - pParam->Level;
	}
	else
	{
		pastArr.as32Rev[0] = (int)500 - ((float)pParam->Level / 100 * 250);
	}

	pstAlgParam.u32ParamNum = 1;
	pstAlgParam.pastArr = &pastArr;

	ret = NI_IAS_OD_SetParams(OdChn, (NI_VOID *)(&pstAlgParam));
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("[ERROR]: NI_IAS_OD_SetParams failed, ret=%x\n", ret);
		return ret;
	}

	return 0;
}

/// 车型检测捕获的详细结果。
///
/// \param [in] channel 通道号。
/// \param [in] pResult 指向动态检测参数结构VEHICLE_DETECT_RESULT的指针。
/// \retval 0  检测到车型
/// \retval <0  未检测到车型
int VehicleDetectGetResult(int channel, VEHICLE_DETECT_RESULT *pResult)
{
	int ret = -1;
	HumanDetectDevice_p pHumanDetectDevice = &GlobalDevice.HumanDetectDevice;

	pthread_rwlock_wrlock(&pHumanDetectDevice->result_lock);
	if (pHumanDetectDevice->vehicle_param[channel].Enable)
	{
		pResult->result = pHumanDetectDevice->vehicle_result[channel].Result;
		pResult->left = pHumanDetectDevice->vehicle_result[channel].x1;
		pResult->top = pHumanDetectDevice->vehicle_result[channel].y1;
		pResult->right = pHumanDetectDevice->vehicle_result[channel].x2;
		pResult->bottom = pHumanDetectDevice->vehicle_result[channel].y2;
	}
	else
	{
		pResult->result = 0;
		pResult->left = 0;
		pResult->top = 0;
		pResult->right = 0;
		pResult->bottom = 0;
	}

	// 上层获取检测结果后清零
	memset(&pHumanDetectDevice->vehicle_result[channel], 0, sizeof(ias_result_t));

	pthread_rwlock_unlock(&pHumanDetectDevice->result_lock);
	if (pResult->result)
	{
		ret = 0;
	}

	return ret;
}

int SetMonitorArea(int channel, Quadrilateral *pArea)
{
	if(NULL == pArea)
	{
		PRINT_ERROR("[ERROR]: SetMonitorArea channel error, channel=%d\n", channel);
		return FAILED;
	}

	for(int i = 0;i < 4; i++)
	{
		gstMonitorArea[channel].Points[i].x = pArea->Points[i].x;
		gstMonitorArea[channel].Points[i].y = pArea->Points[i].y;
	}

	return SUCCESS;
}

int PerimeterDetectAdd(int channel, PERIMETER_DETECT_S *pRules)
{
	int i = 0 ;
	int ret = -1;
	IAS_PERIMETER_RULE_S pstRule;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

	memset(&pstRule, 0, sizeof(IAS_PERIMETER_RULE_S));

	if (channel >= pCaptureDevice->EncCount)
	{
		PRINT_ERROR("[ERROR]: PerimeterDetectAdd channel error, channel=%d\n", channel);
		return ret;
	}

	if (PERIMETER_POLYLINE == pRules->Type)
	{
		pstRule.enType = IAS_PERIMETER_RULE_POLYLINE;
		pstRule.stPolyline.u32PointNum = pRules->Polyline.PointNum;
		if(DIRECTION_IN == pRules->Polyline.Direction)
		{
			pstRule.stPolyline.enDirection = IAS_PG_DIRECTION_IN;
		}
		else if (DIRECTION_OUT == pRules->Polyline.Direction)
		{
			pstRule.stPolyline.enDirection = IAS_PG_DIRECTION_OUT;
		}
		else if (DIRECTION_BI == pRules->Polyline.Direction)
		{
			pstRule.stPolyline.enDirection = IAS_PG_DIRECTION_BI;
		}
		else
		{
			PRINT_ERROR("[ERROR]: PerimeterDetectAdd pRules->Polyline.Direction %d error, channel=%d\n", pRules->Polyline.Direction,channel);
			return ret;
		}

		for(;i < pRules->Polyline.PointNum;i++)
		{
			pstRule.stPolyline.astPoint[i].s16X = pRules->Polyline.Point[i].x;
			pstRule.stPolyline.astPoint[i].s16Y = pRules->Polyline.Point[i].y;
		}

	}
	else if (PERIMETER_POLYGON == pRules->Type)
	{
		pstRule.enType = IAS_PERIMETER_RULE_POLYGON;
		pstRule.stPolygon.u32InsideDet = TRUE;
		pstRule.stPolygon.u32PointNum = pRules->Polygon.PointNum;
		if (DIRECTION_IN == pRules->Polygon.Direction)
		{
			pstRule.stPolygon.enDirection = IAS_PG_DIRECTION_IN;
		}
		else if (DIRECTION_OUT == pRules->Polygon.Direction)
		{
			pstRule.stPolygon.enDirection = IAS_PG_DIRECTION_OUT;
		}
		else if (DIRECTION_BI == pRules->Polygon.Direction)
		{
			pstRule.stPolygon.enDirection = IAS_PG_DIRECTION_BI;
		}
		else
		{
			PRINT_ERROR("[ERROR]: PerimeterDetectAdd pRules->Polygon.Direction %d error, channel=%d\n", pRules->Polygon.Direction, channel);
			return ret;
		}

		for (; i < pRules->Polygon.PointNum; i++)
		{
			pstRule.stPolygon.astPoint[i].s16X = pRules->Polygon.Point[i].x;
			pstRule.stPolygon.astPoint[i].s16Y = pRules->Polygon.Point[i].y;
		}

	}
	else
	{
		PRINT_ERROR("[ERROR]: PerimeterDetectAdd pRules->Type %d error, channel=%d\n", pRules->Type, channel);
		return ret;
	}

	pu32RuleId[channel]++;
	pthread_rwlock_wrlock(&gstPerimeterParam.rwlock);
	for (i = 0; i < PG_MAX_NUM; i++)
	{
		if (0 == gstPerimeterParam.Id[channel][i] && 
		(0 == gstPerimeterParam.Parm[channel][i].stPolygon.u32PointNum 
		|| 0 == gstPerimeterParam.Parm[channel][i].stPolyline.u32PointNum))
		{
			memcpy(&gstPerimeterParam.Parm[channel][i], &pstRule, sizeof(IAS_PERIMETER_RULE_S));
			gstPerimeterParam.Add[channel][i] = TRUE;
			gstPerimeterParam.Delete[channel][i] = FALSE;
			gstPerimeterParam.Id[channel][i] = pu32RuleId[channel];
			PRINT_INFO("PerimeterDetectAdd channel %d i %d add\n", channel, i);
			break;
		}
	}
	pthread_rwlock_unlock(&gstPerimeterParam.rwlock);
	PRINT_INFO("pu32RuleId[%d] %d\n", channel, pu32RuleId[channel]);
	return pu32RuleId[channel];
}

int PerimeterDetectSetParameter(int channel, PG_DETECT_PARAM *pParam)
{
	PRINT_INFO("channel %d pParam->Id %d pParam->Level %d\n", channel, pParam->Id, pParam->Level);
	pthread_rwlock_wrlock(&gstPerimeterParam.rwlock);
	for (int i = 0; i < PG_MAX_NUM; i++)
	{
		if (pParam->Id == gstPerimeterParam.Id[channel][i])
		{
			gstPerimeterParam.Level[channel][i] = pParam->Level;
			gstPerimeterParam.Change[channel][i] = TRUE;
			PRINT_INFO("PerimeterDetectSetParameter channel %d i %d set level %d\n", channel, i, pParam->Level);
			break;
		}
	}
	pthread_rwlock_unlock(&gstPerimeterParam.rwlock);

	return SUCCESS;
}

int PerimeterDetectDelete(int channel, unsigned int Id)
{
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

	if (channel >= pCaptureDevice->EncCount)
	{
		PRINT_ERROR("[ERROR]: PerimeterDetectDelete channel error, channel=%d\n", channel);
		return FAILED;
	}

	if (0 == gstPerimeterParam.RuleNum[channel])
	{
		PRINT_ERROR("[ERROR]: PerimeterDetectDelete has no rules, channel=%d\n", channel);
		return SUCCESS;
	}

	pthread_rwlock_wrlock(&gstPerimeterParam.rwlock);
	for (int i = 0; i < PG_MAX_NUM; i++)
	{
		if (Id == gstPerimeterParam.Id[channel][i])
		{
			gstPerimeterParam.Add[channel][i] = FALSE;
			gstPerimeterParam.Delete[channel][i] = TRUE;
			gstPerimeterParam.Change[channel][i] = FALSE;
			PRINT_INFO("PerimeterDetectDelete channel %d i %d del\n", channel, i);
			break;
		}
	}
	pthread_rwlock_unlock(&gstPerimeterParam.rwlock);

	return SUCCESS;
}

int PerimeterDetectGetResult(int channel, PERIMETER_DETECT_RESULT_S *pResult)
{
	if(NULL == pResult)
	{
		PRINT_ERROR("[ERROR]: channel %d PerimeterDetectGetResult error\n", channel);
		return -1;
	}

	PerimeterDetectDevice_p pPerimeterDetectDevice = &GlobalDevice.PerimeterDetectDevice;

	pthread_rwlock_wrlock(&pPerimeterDetectDevice->result_lock);

	if (PERIMETER_POLYLINE == pResult->Type)
	{
		memcpy(pResult->PolyLineResult, pPerimeterDetectDevice->Result.PolyLineResult[channel], PG_MAX_NUM * sizeof(PG_RESULT));
		memset(pPerimeterDetectDevice->Result.PolyLineResult[channel], 0, PG_MAX_NUM * sizeof(PG_RESULT));
		memset(pPerimeterDetectDevice->Result.PolyLineResultExist[channel], 0, PG_MAX_NUM * sizeof(BOOL));
	}
	else if (PERIMETER_POLYGON == pResult->Type)
	{
		memcpy(pResult->PolyGonResult, pPerimeterDetectDevice->Result.PolyGonResult[channel], PG_MAX_NUM * sizeof(PG_RESULT));
		memset(pPerimeterDetectDevice->Result.PolyGonResult[channel], 0, PG_MAX_NUM * sizeof(PG_RESULT));
		memset(pPerimeterDetectDevice->Result.PolyGonResultExist[channel], 0, PG_MAX_NUM * sizeof(BOOL));
	}

	pthread_rwlock_unlock(&pPerimeterDetectDevice->result_lock);

	return 0;
}

/// 得到动态检测支持的特性。
/// 
/// \param [out] pCaps 指向动态检测特性结构MOTION_DETECT_CAPS的指针。
/// \retval 0  获取成功。
/// \retval <0  获取失败。
int MotionDetectGetCaps(MOTION_DETECT_CAPS * pCaps)
{
	pCaps->Enabled = 1;
	pCaps->GridLines = 22;
	pCaps->GridRows = 18;
	pCaps->Result = 1;
	pCaps->Hint = 0;

    return 0;
}