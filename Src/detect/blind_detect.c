#include "TdCommon.h"

/// 创建黑屏检测设备
/// 
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int BlindDetectCreate(void)
{
	int i = 0;
	int ret = 0;
	int state = 0;
	int STATE = 0x10;
	MD_CHN VdChn = 0;
	MD_CHN_ATTR_S stMdChnAttr;
	MD_WORK_MODE_S stWorkMode;
	MD_DETECT_REGION_S stDetectRegion[MD_MAX_DETECT_REGION];

	MD_CD_Check_p check = &GlobalDevice.md_cd_check;

	memset(&stWorkMode, 0, sizeof(MD_WORK_MODE_S));
	memset(&stMdChnAttr, 0, sizeof(MD_CHN_ATTR_S));
	memset(&stDetectRegion, 0, sizeof(MD_DETECT_REGION_S));

	state = (STATE & check->md_cd_state);
	if(state != 0)
	{
		PRINT_ERROR("[ERROR]: BlindDetect already exits\n");
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

		// for (i = 0; i < VD_MAX_DETECT_REGION; i++)
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

		if (NULL == check->md_data)
		{
			check->md_data = malloc(stMdChnAttr.stMdAttr.stSbParam.u8SbHorizontalNum * stMdChnAttr.stMdAttr.stSbParam.u8SbVerticalNum / 2 + 1);
			if (NULL == check->md_data)
			{
				PRINT_ERROR("malloc error\n");
				return -1;
			}
			memset(check->md_data, 0, stMdChnAttr.stMdAttr.stSbParam.u8SbHorizontalNum * stMdChnAttr.stMdAttr.stSbParam.u8SbVerticalNum / 2 + 1);
		}
		check->init = 1;
	}

	stWorkMode.enDetectSel = MD_DETECT_SEL_BD_MD;
	// stWorkMode.bDiffSumEn = NI_FALSE;
	// stWorkMode.bPixLumaSumEn = NI_FALSE;
	//ret = NI_MDK_MD_SetWorkMode(VdChn, &stWorkMode);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("[ERROR]: NI_MDK_MD_SetWorkMode failed, ret=%x\n", ret);
		return ret;
	}

	check->md_cd_state = (STATE | check->md_cd_state);

	if (0 == check->md_cd_is_running)
	{
		check->md_cd_is_running = 1;
		pthread_t thread;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		ret = pthread_create(&thread, &attr,(void *)MD_CD_check, NULL);
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


/// 销毁黑屏检测设备
/// 
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int BlindDetectDestory(void)
{
	MD_CHN VdChn = 0;
	MD_WORK_MODE_S stWorkMode;
	MD_CD_Check_p check = &GlobalDevice.md_cd_check;
	
	memset(&stWorkMode,0,sizeof(MD_WORK_MODE_S));
	if(check->md_cd_is_running)
	{
		pthread_mutex_lock(&check->MD_CD_lock);
		check->md_cd_is_running = 0;
		check->md_cd_state = 0;
		pthread_mutex_unlock(&check->MD_CD_lock);
		while(1)
		{
			if(check->request_to_stop == 1)
			{
				break;
			}
			else
			{
				usleep(20*1000);
			}
		}
		memset(&GlobalDevice.md_cd_check,0,sizeof(MD_CD_Check_t));

		stWorkMode.enDetectSel = MD_DETECT_SEL_MD;
		// stWorkMode.bDiffSumEn = NI_FALSE;
		// stWorkMode.bPixLumaSumEn = NI_FALSE;
		//NI_MDK_VD_StopDetect(VdChn, &stWorkMode);

		stWorkMode.enDetectSel = MD_DETECT_SEL_BD;
		//NI_MDK_VD_StopDetect(VdChn, &stWorkMode);
		NI_MDK_MD_DeInit(VdChn);
	}
	else
	{
		PRINT_ERROR("[ERROR]: BlindDetect has been destroyed\n");
	}

	return 0;
}



/// 执行黑屏检测。立即返回。
/// 
/// \param [out] pData 指向一个DWORD值的指针，DWORD值是各个报警输入通道电平状态的
///       掩码。低通道在低位，高通道在高位。高电平置1，低电平置0，不存在的通道置0。
/// \retval 0  检测成功
/// \retval <0  检测失败
int BlindDetectGetState(DWORD* pData)
{
	return 0;
}

/// 黑屏检测捕获的详细结果。
/// 
/// \retval 0  设置成功
/// \retval <0  设置失败
int BlindDetectGetResult(int channel, int * result)
{	
	MD_CD_Check_p check = &GlobalDevice.md_cd_check;

	pthread_mutex_lock(&check->MD_CD_lock);
	*result = check->cd_result;
	pthread_mutex_unlock(&check->MD_CD_lock);
	//printf("%d\n",*result);
	return 0;
}

/// 设置黑屏检测捕获的参数。
/// 
/// \param [in] channel 通道号。
/// \param [in] pParam 指向黑屏检测参数结构BLIND_DETECT_ PARAM的指针。
/// \retval 0  设置成功
/// \retval <0  设置失败
int BlindDetectSetParameter(int channel, BLIND_DETECT_PARAM *pParam)
{
	int ret = 0;
	int STATE = 0x10;
	MD_CHN VdChn = 0;
	MD_CHN_ATTR_S stVdChnAttr;
	MD_WORK_MODE_S stWorkMode;
	MD_CD_Check_p check = &GlobalDevice.md_cd_check;
	BlindDetectDevice_p pBlindDetectDevice = &GlobalDevice.BlindDetectDevice;

	if (pParam->Level > 100 || pParam->Level < 0)
	{
		PRINT_ERROR("Please input right Level(between 0-100)! Level:%s\n", pParam->Level);
		return -1;
	}

	memset(&stVdChnAttr, 0, sizeof(MD_CHN_ATTR_S));
	memset(&stWorkMode, 0, sizeof(MD_WORK_MODE_S));
	stWorkMode.enDetectSel = MD_DETECT_SEL_BD;
	// stWorkMode.bDiffSumEn = NI_FALSE;
	// stWorkMode.bPixLumaSumEn = NI_FALSE;

	switch (pParam->Enable)
	{
		case (0):
			if (check->md_cd_state & STATE)
			{
				//ret = NI_MDK_VD_StopDetect(VdChn, &stWorkMode);
				if (RETURN_OK != ret)
				{
					PRINT_ERROR("[ERROR]: NI_MDK_VD_StopDetect failed, ret=%x\n", ret);
				}
				check->md_cd_state ^= STATE;
				pthread_mutex_lock(&check->MD_CD_lock);
				check->cd_result = 0;
				pthread_mutex_unlock(&check->MD_CD_lock);
			}
			return ret;
		case (1):
			if (!(check->md_cd_state & STATE))
			{
				//ret = NI_MDK_VD_StartDetect(VdChn, &stWorkMode);
				if (RETURN_OK != ret)
				{
					PRINT_ERROR("[ERROR]: NI_MDK_VD_StartDetect failed, ret=%x\n", ret);
				}
				check->md_cd_state |= STATE;
			}
			break;
		default:
			PRINT_ERROR("Please input right state(0 or 1)!\n");
			return -1;
	}

	ret = NI_MDK_MD_GetChnAttr(VdChn, &stVdChnAttr);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("[ERROR]: NI_MDK_VD_GetChnAttr failed, ret=%x\n", ret);
		return ret;
	}

	stVdChnAttr.stBdAttr.u8BdPixelBlackGate = (int)((float)62 * pParam->Level / 100 + 65);

	ret = NI_MDK_MD_SetChnAttr(VdChn, &stVdChnAttr);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("[ERROR]: NI_MDK_VD_SetChnAttr failed, ret=%x\n", ret);
		return ret;
	}

	pBlindDetectDevice->cd_config.enable = pParam->Enable;
	pBlindDetectDevice->cd_config.threshold = stVdChnAttr.stBdAttr.u8BdPixelBlackGate;

	return 0;
}

/// 得到黑屏支持的特性。
/// 
/// \param [out] pCaps 指向黑屏特性结构BLIND_DETECT _CAPS的指针。
/// \retval 0  获取成功。
/// \retval <0  获取失败。
int BlindDetectGetCaps(BLIND_DETECT_CAPS * pCaps)
{
	return 0;
}


