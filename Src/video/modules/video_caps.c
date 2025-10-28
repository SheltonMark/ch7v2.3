/**
 * @file video_caps.c
 * @brief Video Capability Query Module
 *
 * This module provides functions to query video encoding capabilities,
 * including chip encoding capabilities, extra stream capabilities,
 * and supported video formats/resolutions.
 */

#include "TdCommon.h"
#include "sdk_comm.h"
#include "video_caps.h"


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

