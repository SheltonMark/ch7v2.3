#include "TdCommon.h"
#include "sdk_comm.h"
#include "logo.h"
#include "mdk_vps.h"
#include "ni_comm_vps.h"
#include "modules/include/video_encoder.h"
#include "modules/include/video_input.h"
#include "modules/include/video_osd.h"
#include "modules/include/video_vps.h"
#include "modules/include/video_frame_rate.h"

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

