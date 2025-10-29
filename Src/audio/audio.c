#include "TdCommon.h"
#include "ni_ans.h"
#include "ni_aec.h"
#include "ni_agc.h"
#include "ni_hpf.h"
#include "ring_buffer.h"

static AC_SAMPLE_RATE AudioRate_ConToLib_Table[] = {
												AC_SR_8K,
												AC_SR_16K,
												AC_SR_32K,
												AC_SR_48K,
												AC_SR_110K,
												AC_SR_220K,
												AC_SR_441K
												};
#if 0
static int AudioStreamType_ConToLib_Table[]={
											[AudioStreamType_PCM]   = PT_PCMA,
											[AudioStreamType_G711A] = PT_G711A,
											[AudioStreamType_G711U] = PT_G711U,
											[AudioStreamType_AAC]   = PT_BYPASS,
											[AudioStreamType_G726]  = PT_G726
											};
#endif

static int AudioClock_ConToLib_Table[] = {
										[AC_SR_8K] = CODEC_MCLK_LRCK_1536X,
										[AC_SR_16K] = CODEC_MCLK_LRCK_768X,
										[AC_SR_32K] = CODEC_MCLK_LRCK_375X,
										[AC_SR_48K] = CODEC_MCLK_LRCK_250X,
										[AC_SR_441K] = CODEC_MCLK_LRCK_272X
										};
static int audio_init = 0;
static int AACDecInit = 0;
static int g_aac_decinit = 0;
static int u32PlayLength = 0;
unsigned char *ptr = NULL;
static BOOL g_aec_caps = TRUE;
int AecStart = DISABLE;
pthread_mutex_t out_lock;
pthread_mutex_t in_lock;

RING_BUFF AAC_BUFF;
RING_BUFF PCM_BUFF;

int _get_one_adts_frame(unsigned char *data, unsigned int *data_size)
{
	int ret = -1;
	int size = 0;
	//int profile = 0;
	//int channelnum = 0;
	//int sample_rate_index = 0;
	unsigned char buffer[10] = {0};

	if (!data || !data_size)
	{
		PRINT_ERROR("Error: _get_one_adts_frame failed\n");
		return -1;
	}

	ret = read_ring_buff(&AAC_BUFF, buffer, 7);
	if (0 == ret)
	{
		AAC_BUFF.head -= 7;
		if ((0xff == buffer[0]) && ((0xf1 == buffer[1]) || (0xf9 == buffer[1])))
		{
			size |= ((buffer[3] & 0x03 << 11));
			size |= (buffer[4] << 3);
			size |= ((buffer[5] & 0xe0) >> 5);
			//profile = (buffer[2] & 0xc0) >> 6;
			//sample_rate_index = (buffer[2] & 0x3c) >> 2;
			//channelnum |= (buffer[2] & 0x1) << 2;
			//channelnum |= (buffer[3] & 0xc0) >> 6;
		}
		else
		{
			PRINT_ERROR("Error: ADTS check failed\n");
			return -1;
		}
	}
	else
	{
		return -1;
	}
	
	ret = read_ring_buff(&AAC_BUFF, data, size);
	if (0 == ret)
	{
		*data_size = size;
	}
	else
	{
		return -1;
	}

	return 0;
}

/*static int Audio_To_Bytes(int n, int sample_bits)
{
    if (24 == sample_bits)
    {
        return (n * 4);
    }

    return (n * (sample_bits >> 3));
}*/

/// 创建音频设备
///
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int AudioCreate(void)
{	
	return 0;
}


/// 销毁音频设备
///
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int AudioDestory(void)
{
	pthread_mutex_lock(&out_lock);
	pthread_mutex_lock(&in_lock);
	int ret = -1;
	int channel = 0;
	//unsigned int u32Audio3AType = 0;
	AiDevice_p pAiDevice = &GlobalDevice.AiDevice;
	AoDevice_p pAoDevice = &GlobalDevice.AoDevice;
	PRINT_TRACE("qijiannan AudioDestory upgrade test\n");
	audio_lock_enter();
	PRINT_TRACE("qijiannan AudioDestory upgrade test\n");
	if (AudioStreamType_AAC == pAiDevice->stAioType && 1 == pAiDevice->g_aac_encinit)
	{
		pAiDevice->g_aac_encinit = 0;
		aac_enc_deinit();
	}
	PRINT_TRACE("qijiannan AudioDestory upgrade test\n");
	if(AudioStreamType_AAC == pAoDevice->stAioType && 1 == g_aac_decinit)
	{
		g_aac_decinit = 0;

		while (pAoDevice->g_aac_decoding)
		{
			usleep(0);
		}

		free(AAC_BUFF.buf);
		memset(&AAC_BUFF, 0, sizeof(RING_BUFF));
		AAC_BUFF.buf = NULL;

		free(PCM_BUFF.buf);
		memset(&PCM_BUFF, 0, sizeof(RING_BUFF));
		PCM_BUFF.buf = NULL;

		NeAACDecClose(pAoDevice->aac_handle);
		pAoDevice->aac_handle = NULL;
		AACDecInit = 0;
	}
	PRINT_TRACE("qijiannan AudioDestory upgrade test\n");
	ret = NI_MDK_ANS_DeInit(channel);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_ANS_DeInit failed, ret=%x!\n", ret);
	}

	ret = NI_MDK_HPF_DeInit(channel);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_HPF_DeInit failed, ret=%x!\n", ret);
	}

	ret = NI_MDK_AGC_DeInit(channel);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_AGC_DeInit failed, ret=%x!\n", ret);
	}
	PRINT_TRACE("qijiannan AudioDestory upgrade test\n");
	ret = NI_MDK_AENC_Stop(channel);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_AENC_Stop failed, ret=%x!\n", ret);
	}
	PRINT_TRACE("qijiannan AudioDestory upgrade test\n");
	ret = NI_MDK_AENC_DestroyChn(channel);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_AENC_DestroyChn failed, ret=%x!\n", ret);
	}
	PRINT_TRACE("qijiannan AudioDestory upgrade test\n");
	ret = NI_MDK_ADEC_Stop(channel);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_ADEC_Stop failed, ret=%x!\n", ret);
	}
	PRINT_TRACE("qijiannan AudioDestory upgrade test\n");

	AecStart = DISABLE;

	ret = NI_MDK_AEC_DeInit(channel);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_AEC_DeInit failed, ret=%x!\n", ret);
	}

	PRINT_TRACE("qijiannan AudioDestory upgrade test\n");

	ret = NI_MDK_ADEC_DestroyChn(channel);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_ADEC_DestroyChn failed, ret=%x!\n", ret);
	}
	PRINT_TRACE("qijiannan AudioDestory upgrade test\n");
	gpio_set_value(SPEAK_MUTE, DISABLE);

	if (ptr != NULL)
	{
		free(ptr);
		ptr = NULL;
	}
	PRINT_TRACE("qijiannan AudioDestory upgrade test\n");
	ret = NI_MDK_AUDIO_DeInit();
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_AUDIO_DeInit failed, ret=%x!\n", ret);
	}
	PRINT_TRACE("qijiannan AudioDestory upgrade test\n");
	audio_lock_leave();
	PRINT_TRACE("qijiannan AudioDestory upgrade test\n");
	audio_init = 0;

	pthread_mutex_unlock(&out_lock);
	pthread_mutex_unlock(&in_lock);
	return 0;
}


int AudioAiAoSetFormat(AUDIO_AIAOCONFIG_t pAiFormat,AUDIO_AIAOCONFIG_t pAoFormat)
{
	AiDevice_p pAiDevice = &GlobalDevice.AiDevice;
	AoDevice_p pAoDevice = &GlobalDevice.AoDevice;

	pAiDevice->stAioAttr.sample_rate  		= AudioRate_ConToLib_Table[pAiFormat.SampleRate];
	pAiDevice->stAioAttr.bit_width 			= AUDIO_BIT_WIDTH_32;
	pAiDevice->stAioAttr.channels			= 1;
	pAiDevice->stAioAttr.u32PacketPeriod 	= 40;
	pAiDevice->stAioAttr.enCodecType 		= AUDIO_INTERNAL_CODEC;
	//pAiDevice->stAioAttr.volume 			= 0;
	pAiDevice->stAioType 					= pAiFormat.streamtype;
	
	pAoDevice->stAioAttr.sample_rate 		= AudioRate_ConToLib_Table[pAoFormat.SampleRate];
	pAoDevice->stAioAttr.bit_width 			= AUDIO_BIT_WIDTH_32;
	pAoDevice->stAioAttr.channels			= 1;
	pAoDevice->stAioAttr.u32PacketPeriod 	= 40;
	pAoDevice->stAioAttr.enCodecType 		= AUDIO_INTERNAL_CODEC;
	//pAoDevice->stAioAttr.volume 			= 0;
	pAoDevice->stAioType 					= pAoFormat.streamtype;		

	return 0;
}

/// 创建音频输出设备
///
/// \param [in] iChannel 通道号
/// \retval <0 创建失败
/// \retval 0 创建成功
int AudioOutCreate(int iChannel)
{
	pthread_mutex_lock(&out_lock);
	int ret = -1;
	AIO_ATTR_S  stAoAttr;
    ADEC_ATTR_S stAdecAttr;
	NI_STREAM_PARAM_S stStreamParam;
	CODEC_PUB_ATTR_S stCodecPubAttr;
	AUDIO_WORK_MODE_E enAudioWorkMode;
	AoDevice_p pAoDevice = &GlobalDevice.AoDevice;
	
	memset(&stAoAttr,0,sizeof(AIO_ATTR_S));
	memset(&stAdecAttr,0,sizeof(ADEC_ATTR_S));
	memset(&stStreamParam, 0, sizeof(NI_STREAM_PARAM_S));
	memset(&enAudioWorkMode,0,sizeof(AUDIO_WORK_MODE_E));
	memset(&stCodecPubAttr,0,sizeof(CODEC_PUB_ATTR_S));

	if(0 == audio_init)
	{
		ret = NI_MDK_AUDIO_Init();
   		if (ret != RETURN_OK)
	    {
	        PRINT_ERROR("Error: NI_MDK_AUDIO_Init failed, ret=%d!\n", ret);
			pthread_mutex_unlock(&out_lock);
			return -1;
	    }
		audio_init = 1;
	}

	ret = NI_MDK_AO_GetPubAttr(&stAoAttr);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_AO_GetPubAttr failed, ret=%x!\n", ret);
		pthread_mutex_unlock(&out_lock);
		return -1;
	}

    stAoAttr.enWorkMode   	= AIO_MODE_I2S_MASTER;
    stAoAttr.enSampleEdge 	= AUDIO_SAMPLE_NEGEDGE;
    stAoAttr.u32CodecMclk 	= 12288000;
	stAoAttr.enBitWidth 	= pAoDevice->stAioAttr.bit_width;
	stAoAttr.u32SampleRate 	= pAoDevice->stAioAttr.sample_rate;
    stAoAttr.enBclk 		= AUDIO_BCLK_I2S0;
    stAoAttr.enLrck 		= AUDIO_LRCK_I2S0;
	stAoAttr.enCodecType 	= pAoDevice->stAioAttr.enCodecType;

	ret = NI_MDK_CODEC_GetPubAttr(&stCodecPubAttr);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_CODEC_GetPubAttr failed, ret=%x!\n", ret);
		pthread_mutex_unlock(&out_lock);
		return -1;
	}

	stCodecPubAttr.enDacRatio 		= AudioClock_ConToLib_Table[stAoAttr.u32SampleRate];
	stCodecPubAttr.u32DacSampleRate = stAoAttr.u32SampleRate;

	ret = NI_MDK_CODEC_SetPubAttr(&stCodecPubAttr);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_CODEC_SetPubAttr failed, ret=%x!\n", ret);
		pthread_mutex_unlock(&out_lock);
		return -1;
	}

	ret = NI_MDK_AO_SetPubAttr(&stAoAttr);
    if (ret != RETURN_OK)
    {
        PRINT_ERROR("Error: NI_MDK_AO_SetPubAttr failed, ret=%x!\n", ret);
		pthread_mutex_unlock(&out_lock);
		return -1;
    }

	stAdecAttr.u32PacketPeriod 	= pAoDevice->stAioAttr.u32PacketPeriod ;  //创建通道返回的音频包长度为640
	//stAdecAttr.enType 			= AudioStreamType_ConToLib_Table[pAoDevice->stAioType];
	stAdecAttr.enType 			= PT_BYPASS;
	u32PlayLength = NI_MDK_ADEC_CreateChn(iChannel, &stAdecAttr);
	if (u32PlayLength < 0)
	{
		PRINT_ERROR("Error: NI_MDK_ADEC_CreateChn failed, u32PlayLength=%d!\n", u32PlayLength);
		pthread_mutex_unlock(&out_lock);
		return -1;
	}

	ret = NI_MDK_AENC_Stop(iChannel);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_AENC_Stop failed, ret=%x!\n", ret);
	}
	
	NI_MDK_CODEC_SetAinEn(CODEC_RIGHT_CHANNEL, NI_FALSE);
	NI_MDK_CODEC_SetAinEn(CODEC_LEFT_CHANNEL, NI_FALSE);

	if(g_aec_caps)
	{
		stStreamParam.u32Depth = AUDIO_BIT_WIDTH_16;
		stStreamParam.u32Frequency = pAoDevice->stAioAttr.sample_rate;
		stStreamParam.u32PacketPeriod = pAoDevice->stAioAttr.u32PacketPeriod;
		stStreamParam.encChn = 0;
		stStreamParam.decChn = 0;
		stStreamParam.enType = PT_BYPASS;
		stStreamParam.bValid = NI_TRUE;
		ret = NI_MDK_AEC_Init(iChannel, &stStreamParam);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("Error: NI_MDK_AEC_Init failed, ret=%x!\n", ret);
			pthread_mutex_unlock(&out_lock);
			return -1;
		}

		NI_AEC_CFG_S stAecCfg;
		memset(&stAecCfg, 0, sizeof(NI_AEC_CFG_S));
		ret = NI_MDK_AEC_GetConfig(iChannel, &stAecCfg);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("Error: NI_MDK_AEC_GetConfig failed, ret=%x!\n", ret);
			pthread_mutex_unlock(&out_lock);
			return -1;
		}

		stAecCfg.enAecLevel = NI_AEC_LEVEL_NORMAL;
		stAecCfg.stSkewCfg.bSkewEnable = FALSE;
		stAecCfg.enComfortNoiseLevel = NI_AEC_COMFORT_NOISE_OFF;
		stAecCfg.s16FarRefDelay = -1;

		ret = NI_MDK_AEC_SetConfig(iChannel, &stAecCfg);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("Error: NI_MDK_AEC_SetConfig failed, ret=%x!\n", ret);
			pthread_mutex_unlock(&out_lock);
			return -1;
		}

		AecStart = ENABLE;
	}

	NI_MDK_CODEC_SetAinEn(CODEC_RIGHT_CHANNEL, NI_TRUE);
	NI_MDK_CODEC_SetAinEn(CODEC_LEFT_CHANNEL, NI_TRUE);
	NI_MDK_CODEC_SetAoutEn(CODEC_RIGHT_CHANNEL,NI_TRUE);
    NI_MDK_CODEC_SetAoutEn(CODEC_LEFT_CHANNEL,NI_TRUE);

	NI_MDK_AENC_Start(iChannel);
	NI_MDK_ADEC_Start(iChannel);

	
	if (AudioStreamType_AAC == pAoDevice->stAioType && 0 == g_aac_decinit)
	{
		pAoDevice->aac_handle = NeAACDecOpen();
		if (!pAoDevice->aac_handle)
		{
			PRINT_ERROR("Error: NeAACDecOpen failed\n");
			pthread_mutex_unlock(&out_lock);
			return -1;
		}

		NeAACDecConfigurationPtr conf = NeAACDecGetCurrentConfiguration(pAoDevice->aac_handle);
		if (!conf)
		{
			PRINT_ERROR("Error: NeAACDecGetCurrentConfiguration failed\n");
			if (pAoDevice->aac_handle)
			{
				NeAACDecClose(pAoDevice->aac_handle);
			}
			pthread_mutex_unlock(&out_lock);
			return -1;
		}

		conf->defObjectType = LC;
		conf->defSampleRate = pAoDevice->stAioAttr.sample_rate;
		conf->outputFormat = FAAD_FMT_16BIT;
		conf->dontUpSampleImplicitSBR = 1;

		unsigned char result = NeAACDecSetConfiguration(pAoDevice->aac_handle, conf);
		if (0 == result)
		{
			PRINT_ERROR("NeAACDecSetConfiguration failed!\n");
			if (pAoDevice->aac_handle)
			{
				NeAACDecClose(pAoDevice->aac_handle);
			}
			pthread_mutex_unlock(&out_lock);
			return -1;
		}

		memset(&AAC_BUFF, 0, sizeof(RING_BUFF));
		memset(&PCM_BUFF, 0, sizeof(RING_BUFF));
		AAC_BUFF.head = 0;
		AAC_BUFF.tail = 0;
		AAC_BUFF.tatol_len = AUDIO_AAC_BUFFER_SIZE;
		if (NULL == AAC_BUFF.buf)
		{
			AAC_BUFF.buf = malloc(AAC_BUFF.tatol_len);
			if (NULL == AAC_BUFF.buf)
			{
				PRINT_ERROR("AAC_BUFF.buf malloc failed!\n");
				NeAACDecClose(pAoDevice->aac_handle);
				pthread_mutex_unlock(&out_lock);
				return -1;
			}
		}
		PCM_BUFF.head = 0;
		PCM_BUFF.tail = 0;
		PCM_BUFF.tatol_len = AUDIO_PCM_BUFFER_SIZE;
		if (NULL == PCM_BUFF.buf)
		{
			PCM_BUFF.buf = malloc(PCM_BUFF.tatol_len);
			if (NULL == PCM_BUFF.buf)
			{
				PRINT_ERROR("PCM_BUFF.buf malloc failed!\n");
				free(AAC_BUFF.buf);
				AAC_BUFF.buf = NULL;
				NeAACDecClose(pAoDevice->aac_handle);
				pthread_mutex_unlock(&out_lock);
				return -1;
			}
		}
		g_aac_decinit = 1;
	}
	else if (AudioStreamType_PCM == pAoDevice->stAioType)
	{
		memset(&PCM_BUFF, 0, sizeof(RING_BUFF));
		PCM_BUFF.head = 0;
		PCM_BUFF.tail = 0;
		PCM_BUFF.tatol_len = AUDIO_PCM_BUFFER_SIZE;
		if (NULL == PCM_BUFF.buf)
		{
			PCM_BUFF.buf = malloc(PCM_BUFF.tatol_len);
			if (NULL == PCM_BUFF.buf)
			{
				PRINT_ERROR("PCM_BUFF.buf malloc failed!\n");
				pthread_mutex_unlock(&out_lock);
				return -1;
			}
		}
	}

	if (NULL == ptr)
	{
		ptr = malloc(u32PlayLength);
		if (NULL == ptr)
		{
			PRINT_ERROR("Error:malloc failed!\n");
			pthread_mutex_unlock(&out_lock);
			return -1;
		}
	}

	if (HI_SUCCESS != gpio_set_value(SPEAK_MUTE, ENABLE))
	{
		PRINT_ERROR("gpio_set_value SPEAK_MUTE failed\n");
	}

	// 这边增加600ms延迟为了规避硬件mute开启后，声音逐渐变大（800ms）造成吞字问题
	// 2023年6月20日修改为1s，适配CH7V1.1的新功放
	sleep(1);
	pthread_mutex_unlock(&out_lock);
	return 0;
}

/// 销毁音频输出设备
///
/// \param [in] iChannel 通道号
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int AudioOutDestroy(int iChannel)
{
	pthread_mutex_lock(&out_lock);
	int ret = -1;
	//unsigned int u32Audio3AType = 0;
	AoDevice_p pAoDevice = &GlobalDevice.AoDevice;

	audio_lock_enter();

	if (AudioStreamType_AAC == pAoDevice->stAioType && 1 == g_aac_decinit)
	{
		g_aac_decinit = 0;

		while(pAoDevice->g_aac_decoding)
		{
			usleep(0);
		}

		free(AAC_BUFF.buf);
		memset(&AAC_BUFF, 0, sizeof(RING_BUFF));
		AAC_BUFF.buf = NULL;

		free(PCM_BUFF.buf);
		memset(&PCM_BUFF, 0, sizeof(RING_BUFF));
		PCM_BUFF.buf = NULL;

		NeAACDecClose(pAoDevice->aac_handle);
		pAoDevice->aac_handle = NULL;
		AACDecInit = 0;
	}

	AecStart = DISABLE;

	ret = NI_MDK_AEC_DeInit(iChannel);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_AEC_DeInit failed, ret=%x!\n", ret);
	}

	ret = NI_MDK_ADEC_Stop(iChannel);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_ADEC_Stop failed, ret=%x!\n", ret);
	}

	ret = NI_MDK_ADEC_DestroyChn(iChannel);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_ADEC_DestroyChn failed, ret=%x!\n", ret);
	}

	audio_lock_leave();

	if (ptr != NULL)
	{
		free(ptr);
		ptr = NULL;
	}

	gpio_set_value(SPEAK_MUTE,DISABLE);

	pthread_mutex_unlock(&out_lock);
	return 0;
}

/// 关闭回声消除
/// 须在AO创建前使用
///
/// \param [in] iChannel 通道号
/// \param [in] Start 是否开启回声消除
/// \return>0 设置失败
/// \retval 0 设置成功
int AudioOutAecStart(int iChannel, int Start)
{
	if(ENABLE == Start)
	{
		g_aec_caps = TRUE;
	}
	else
	{
		g_aec_caps = FALSE;
	}

	PRINT_TRACE("AudioOutAecStart  g_aec_caps is %d\n", g_aec_caps);

	return 0;
}

int AudioOutGetBufSize(void)
{
	int ret = -1;
	ADEC_CHN AdChn = 0;
	ADEC_NODE_INFO_S pstNodeInfo;

	memset(&pstNodeInfo, 0, sizeof(ADEC_NODE_INFO_S));

	ret = NI_MDK_ADEC_GetChnNodeInfo(AdChn, &pstNodeInfo);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_ADEC_GetChnNodeInfo failed, ret=%x!\n", ret);
		pstNodeInfo.u32BusyNodeNum = 0;
	}

	return pstNodeInfo.u32BusyNodeNum;
}

/// 语音对讲解码数据填充
///
/// \param [in] src 解码数据缓冲地址
/// \param [in] size 解码数据缓冲长度
/// \return>0 填充失败
/// \retval 0 填充成功
int AudioOutPlay(unsigned char *src, int size)
{
	if (size < 0 || NULL == src)
	{
		return -1;
	}

	if (size > u32PlayLength || NULL == ptr || 2 * size > u32PlayLength)
	{
		PRINT_ERROR("Error: AudioOutPlay size:%d > u32PlayLength:%d!\n", size, u32PlayLength);
		return -1;
	}
	pthread_mutex_lock(&out_lock);
	int ret = -1;
	ADEC_CHN AdChn = 0;
	NeAACDecFrameInfo hInfo;
	unsigned int aac_size = 0;
	unsigned char channels = 0;
	AUDIO_STREAM_S stStreamChn0;
	unsigned int PCM_Length = 0;
	unsigned long sample_rate = 0;
	unsigned char *PCM_Buffer = NULL;
	unsigned char Buffer[2048] = {0};
	unsigned char AAC_Buffer[1024] = {0};
	AoDevice_p pAoDevice = &GlobalDevice.AoDevice;

	memset(&hInfo, 0, sizeof(NeAACDecFrameInfo));
	memset(&stStreamChn0, 0, sizeof(AUDIO_STREAM_S));

	switch (pAoDevice->stAioType)
	{
		case AudioStreamType_PCM:
			if(0 == size)
			{
				break;
			}

			ret = write_ring_buff(&PCM_BUFF, src, size);
			if (RETURN_OK != ret)
			{
				PRINT_ERROR("size %d > PCM_BUFF %d\n", size, PCM_BUFF.tatol_len);
				pthread_mutex_unlock(&out_lock);
				return 0;
			}
			
			while (1)
			{
				ret = read_ring_buff(&PCM_BUFF, ptr, u32PlayLength);
				if (RETURN_OK != ret)
				{
					break;
				}
				stStreamChn0.pu8Stream = ptr;
				stStreamChn0.u32Len = u32PlayLength;

				ret = NI_MDK_ADEC_SendStream(AdChn, &stStreamChn0, 10000);
				if (RETURN_OK != ret)
				{
					PRINT_ERROR("Error: NI_MDK_ADEC_SendStream failed, ret=%x!\n", ret);
				}
			}
			pthread_mutex_unlock(&out_lock);
			return 0;
		case AudioStreamType_G711A:
			if (0 == size)
			{
				break;
			}

			ret = g711a_2_pcm(src, size, ptr, &stStreamChn0.u32Len);
			if (ret < 0)
			{
				PRINT_ERROR("AudioOutPlay g711a_2_pcm failed\n");
				pthread_mutex_unlock(&out_lock);
				return 0;
			}
			break;
		case AudioStreamType_G711U:
			if (0 == size)
			{
				break;
			}

			ret = g711u_2_pcm(src, size, ptr, &stStreamChn0.u32Len);
			if (ret < 0)
			{
				PRINT_ERROR("AudioOutPlay g711a_2_pcm failed\n");
				pthread_mutex_unlock(&out_lock);
				return 0;
			}
			break;
		case AudioStreamType_AAC:
			if (0 == size)
			{
				break;
			}

			if (NULL == pAoDevice->aac_handle || 0 == g_aac_decinit)
			{
				PRINT_ERROR("AudioOutPlay AAC Decode is not init\n");
				pthread_mutex_unlock(&out_lock);
				return 0;
			}

			pAoDevice->g_aac_decoding = 1;
			ret = write_ring_buff(&AAC_BUFF, src, size);
			if (RETURN_OK != ret)
			{
				PRINT_ERROR("size %d > AAC_BUFF %d\n", size, AAC_BUFF.tatol_len);
				pAoDevice->g_aac_decoding = 0;
				pthread_mutex_unlock(&out_lock);
				return 0;
			}

			if (0 == AACDecInit)
			{
				ret = _get_one_adts_frame(AAC_Buffer, &aac_size);
				if (RETURN_OK != ret)
				{
					PRINT_ERROR("Error: _get_one_adts_frame failed!\n");
					pAoDevice->g_aac_decoding = 0;
					pthread_mutex_unlock(&out_lock);
					return 0;
				}

				NeAACDecInit(pAoDevice->aac_handle, AAC_Buffer, aac_size, &sample_rate, &channels);

				AAC_BUFF.head = 0;
				AACDecInit = 1;
			}

			while (1)
			{
				ret = _get_one_adts_frame(AAC_Buffer, &aac_size);
				if (RETURN_OK != ret)
				{
					break;
				}

				PCM_Buffer = (unsigned char *)NeAACDecDecode(pAoDevice->aac_handle, &hInfo, AAC_Buffer, aac_size);

				if (hInfo.error > 0)
				{
					PRINT_ERROR("error = %s\n", NeAACDecGetErrorMessage(hInfo.error));
					continue;
				}
				else if (PCM_Buffer > 0 && hInfo.samples > 0)
				{
					/*PRINT_ERROR("frame info: bytesconsumed %d, channels %d, header_type %d object_type %d, samples %d, samplerate %d\n",
						hInfo.bytesconsumed,
						hInfo.channels, hInfo.header_type,
						hInfo.object_type, hInfo.samples,
						hInfo.samplerate);*/
					PCM_Length = hInfo.samples * hInfo.channels;

					for (int i = 0, j = 0; i < PCM_Length && j < (PCM_Length / 2); i += 4, j += 2)
					{
						Buffer[j] = *(PCM_Buffer + i);
						Buffer[j + 1] = *(PCM_Buffer + i + 1);
					}

					ret = write_ring_buff(&PCM_BUFF, Buffer, PCM_Length / 2);
					if (RETURN_OK != ret)
					{
						PRINT_ERROR("size %d > AAC_BUFF %d\n", PCM_Length / 2, PCM_BUFF.tatol_len);
						PCM_Buffer = NULL;
						pAoDevice->g_aac_decoding = 0;
						pthread_mutex_unlock(&out_lock);
						return 0;
					}

					while (g_aac_decinit)
					{
						ret = read_ring_buff(&PCM_BUFF, ptr, u32PlayLength);
						if (RETURN_OK != ret)
						{
							break;
						}
						stStreamChn0.pu8Stream = ptr;
						stStreamChn0.u32Len = u32PlayLength;

						ret = NI_MDK_ADEC_SendStream(AdChn, &stStreamChn0, 10000);
						if (RETURN_OK != ret)
						{
							PRINT_ERROR("Error: NI_MDK_ADEC_SendStream failed, ret=%x!\n", ret);
						}
					}
				}
				else
				{
					PRINT_ERROR("NeAACDecDecode error \n");
				}
			}
			pAoDevice->g_aac_decoding = 0;
			PCM_Buffer = NULL;
			pthread_mutex_unlock(&out_lock);
			return 0;
		case AudioStreamType_G726:
			// frame_trans.len = fh_pcm_2_g726_16K(pAiDevice->g726_handle, frame_info.data, frame_info.len, frame_trans.data);
			break;
		/*case AudioStreamType_G726:
			frame_trans.len = fh_pcm_2_g726_32K(pAiDevice->g726_handle, frame_info.data, frame_info.len, frame_trans.data);
			pstream.type = Media_StreamType_G726_32K;
			break;
			*/
		default:
			PRINT_ERROR("this Ai encode Type not support\n");
			break;
	}

	if (u32PlayLength < stStreamChn0.u32Len)
	{
		PRINT_ERROR("Error: AudioOutPlay  stStreamChn0.u32Len:%d < u32PlayLength:%d!\n", stStreamChn0.u32Len, u32PlayLength);
		pthread_mutex_unlock(&out_lock);
		return 0;
	}

	if (0 == stStreamChn0.u32Len && 0 != size)
	{
		PRINT_ERROR("Error: AudioOutPlay  stStreamChn0.u32Len = 0!\n");
		pthread_mutex_unlock(&out_lock);
		return 0;
	}

	if (0 != size)
	{
		stStreamChn0.pu8Stream = ptr;
		// stStreamChn0.u32Len = u32PlayLength;
		ret = NI_MDK_ADEC_SendStream(AdChn, &stStreamChn0, 10000);
		if (RETURN_OK != ret)
		{
			PRINT_ERROR("Error: NI_MDK_ADEC_SendStream failed, ret=%x!\n", ret);
		}
	}
	else
	{
		if ((AudioStreamType_AAC == pAoDevice->stAioType 
			|| AudioStreamType_PCM == pAoDevice->stAioType) 
			&& PCM_BUFF.tail - PCM_BUFF.head > 0)
		{
			stStreamChn0.u32Len = PCM_BUFF.tail - PCM_BUFF.head;

			ret = read_ring_buff(&PCM_BUFF, ptr, PCM_BUFF.tail - PCM_BUFF.head);
			if (RETURN_OK != ret)
			{
				PRINT_ERROR("Error: AudioOutPlay read_ring_buff failed\n");
				pthread_mutex_unlock(&out_lock);
				return 0;
			}

			stStreamChn0.pu8Stream = ptr;
			ret = NI_MDK_ADEC_SendStream(AdChn, &stStreamChn0, 10000);
			if (RETURN_OK != ret)
			{
				PRINT_ERROR("Error: NI_MDK_ADEC_SendStream failed, ret=%x!\n", ret);
			}
		}
	}
	pthread_mutex_unlock(&out_lock);
	return 0;
}


/// 设置数据的压缩格式
///
/// \param [in] coderType 压缩格式
/// \return>0 设置失败
/// \retval 0 设置成功
int AudioOutSetFormat(int iChannel, AUDIO_AIAOCONFIG_t* pAoFormat)
{
	AoDevice_p pAoDevice 					= &GlobalDevice.AoDevice;

	pAoDevice->stAioAttr.sample_rate 		= AudioRate_ConToLib_Table[pAoFormat->SampleRate];
	pAoDevice->stAioAttr.bit_width 			= AUDIO_BIT_WIDTH_32;
	pAoDevice->stAioAttr.channels			= 1;
	pAoDevice->stAioAttr.u32PacketPeriod 	= 40;
	pAoDevice->stAioAttr.enCodecType 		= AUDIO_INTERNAL_CODEC;
	//pAoDevice->stAioAttr.volume 			= 31;
	pAoDevice->stAioType 					= pAoFormat->streamtype;

 	return 0;
	
}

/// 得到音频输入通道数
///
/// \param 无
/// \return 音频输入通道数目。
int AudioInGetChannels(void)
{
	return GlobalDevice.AiDevice.stAioAttr.channels;
}

/// 得到音频输出通道数
///
/// \param 无
/// \return 音频输入通道数目。
int AudioOutGetChannels(void)
{
	return GlobalDevice.AoDevice.stAioAttr.channels;
}


/// 创建音频输入设备
///
/// \param [in] iChannel 通道号
/// \retval <0 创建失败
/// \retval 0 创建成功
int AudioInCreate(int iChannel)
{
	pthread_mutex_lock(&in_lock);
	int ret = -1;
	AIO_ATTR_S stAiAttr;
	NI_ANS_CFG_S stAnsCfg;
	NI_HPF_CFG_S stHpfCfg;
	NI_AGC_CFG_S stAgcCfg;
	AENC_ATTR_S stAencAttr;
	//unsigned int u32Audio3AType = 0;
	NI_STREAM_PARAM_S stStreamParam;
	CODEC_PUB_ATTR_S stCodecPubAttr;
	AUDIO_WORK_MODE_E enAudioWorkMode;
	CODEC_ADC_EXT_ATTR_S pstAdcExtAttr;
	AiDevice_p pAiDevice = &GlobalDevice.AiDevice;
	AoDevice_p pAoDevice = &GlobalDevice.AoDevice;

	memset(&stAiAttr,0,sizeof(AIO_ATTR_S));
	memset(&stAnsCfg, 0, sizeof(NI_ANS_CFG_S));
	memset(&stHpfCfg, 0, sizeof(NI_HPF_CFG_S));
	memset(&stAgcCfg, 0, sizeof(NI_AGC_CFG_S));
	memset(&stAencAttr, 0, sizeof(AENC_ATTR_S));
	memset(&stCodecPubAttr, 0, sizeof(CODEC_PUB_ATTR_S));
	memset(&enAudioWorkMode, 0, sizeof(AUDIO_WORK_MODE_E));
	memset(&pstAdcExtAttr, 0, sizeof(CODEC_ADC_EXT_ATTR_S));

	if(0 == audio_init)
	{
		ret = NI_MDK_AUDIO_Init();
		if (ret != RETURN_OK)
	    {
			PRINT_ERROR("Error: NI_MDK_AUDIO_Init failed, ret=%d!\n", ret);
			pthread_mutex_unlock(&in_lock);
			return -1;
	    }

		audio_init = 1;
	}

	ret = NI_MDK_AI_GetPubAttr(&stAiAttr);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_AI_GetPubAttr failed, ret=%x!\n", ret);
		pthread_mutex_unlock(&in_lock);
		return -1;
	}

	stAiAttr.enWorkMode 	= AIO_MODE_I2S_MASTER;
	stAiAttr.enSampleEdge 	= AUDIO_SAMPLE_POSEDGE;
	stAiAttr.u32CodecMclk 	= 12288000;
	stAiAttr.enBitWidth 	= pAiDevice->stAioAttr.bit_width;
	stAiAttr.u32SampleRate 	= pAiDevice->stAioAttr.sample_rate;
	stAiAttr.enBclk 		= AUDIO_BCLK_I2S0;
	stAiAttr.enLrck 		= AUDIO_LRCK_I2S0;
	stAiAttr.enCodecType 	= pAiDevice->stAioAttr.enCodecType;

	ret = NI_MDK_AI_SetPubAttr(&stAiAttr);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_AI_SetPubAttr failed, ret=%x!\n", ret);
		pthread_mutex_unlock(&in_lock);
		return -1;
	}

	enAudioWorkMode = AUDIO_WORK_MODE_NORMAL;
	ret = NI_MDK_AUDIO_SetWorkMode(enAudioWorkMode);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("call NI_MDK_AUDIO_SetWorkMode fail,ret=%#x\n", ret);
		pthread_mutex_unlock(&in_lock);
		return -1;
	}

	stAencAttr.u32PacketPeriod 	= pAiDevice->stAioAttr.u32PacketPeriod;
	//stAencAttr.enType 			= AudioStreamType_ConToLib_Table[pAiDevice->stAioType];
	stAencAttr.enType 			= PT_BYPASS;
	ret = NI_MDK_AENC_CreateChn(iChannel, &stAencAttr);
	if (ret < 0)
	{
		PRINT_ERROR("Error: NI_MDK_AENC_CreateChn failed, ret=%x!\n", ret);
		pthread_mutex_unlock(&in_lock);
		return -1;
	}

	usleep(5000);
	ret = NI_MDK_CODEC_GetPubAttr(&stCodecPubAttr);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_CODEC_GetPubAttr failed, ret=%x!\n", ret);
		pthread_mutex_unlock(&in_lock);
		return -1;
	}

	stCodecPubAttr.enAdcRatio 		= AudioClock_ConToLib_Table[pAiDevice->stAioAttr.sample_rate];
	stCodecPubAttr.enAudioIntf 		= CODEC_INTF_I2S;
	stCodecPubAttr.enDacRatio 		= AudioClock_ConToLib_Table[pAoDevice->stAioAttr.sample_rate];
	stCodecPubAttr.enLeftSrc 		= CODEC_AIN_SOURCE_MIC_PSEUDO_DIFF;
	stCodecPubAttr.enRightSrc 		= CODEC_AIN_SOURCE_MIC_PSEUDO_DIFF;
	// stCodecPubAttr.enRole = CODEC_ROLE_SLAVE;
	stCodecPubAttr.u32AdcSampleRate = pAiDevice->stAioAttr.sample_rate;
	stCodecPubAttr.u32DacSampleRate = pAoDevice->stAioAttr.sample_rate;
	stCodecPubAttr.enBitWidth 		= AUDIO_BIT_WIDTH_24;

	ret = NI_MDK_CODEC_SetPubAttr(&stCodecPubAttr);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_CODEC_SetPubAttr failed, ret=%x!\n", ret);
		return -1;
	}

	ret = NI_MDK_CODEC_GetAdcExtAttr(&pstAdcExtAttr);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_CODEC_GetAdcExtAttr failed, ret=%x!\n", ret);
		pthread_mutex_unlock(&in_lock);
		return -1;
	}

	pstAdcExtAttr.bHpfEnable = ENABLE;
	ret = NI_MDK_CODEC_SetAdcExtAttr(&pstAdcExtAttr);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_CODEC_SetAdcExtAttr failed, ret=%x!\n", ret);
		pthread_mutex_unlock(&in_lock);
		return -1;
	}

	stStreamParam.u32Depth 			= AUDIO_BIT_WIDTH_16;
	stStreamParam.u32Frequency 		= pAoDevice->stAioAttr.sample_rate;
	stStreamParam.u32PacketPeriod 	= pAiDevice->stAioAttr.u32PacketPeriod;
	stStreamParam.encChn 			= 0;
	stStreamParam.decChn 			= 0;
	stStreamParam.enType 			= PT_BYPASS;
	stStreamParam.bValid 			= NI_TRUE;
	ret = NI_MDK_ANS_Init(iChannel, &stStreamParam);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_ANS_Init failed, ret=%x!\n", ret);
		pthread_mutex_unlock(&in_lock);
		return -1;
	}

	ret = NI_MDK_AGC_Init(iChannel, &stStreamParam);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_AGC_Init failed, ret=%x!\n", ret);
		pthread_mutex_unlock(&in_lock);
		return -1;
	}

	ret = NI_MDK_HPF_Init(iChannel, &stStreamParam);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_HPF_Init failed, ret=%x!\n", ret);
		pthread_mutex_unlock(&in_lock);
		return -1;
	}

	ret = NI_MDK_HPF_GetConfig(iChannel, &stHpfCfg);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_HPF_GetConfig failed, ret=%x!\n", ret);
		pthread_mutex_unlock(&in_lock);
		return -1;
	}

	stHpfCfg.enHpfLevel = NI_HPF_LEVEL_AGRRESSIVE;

	ret = NI_MDK_HPF_SetConfig(iChannel, &stHpfCfg);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_HPF_SetConfig failed, ret=%x!\n", ret);
		pthread_mutex_unlock(&in_lock);
		return -1;
	}

	stAnsCfg.enAnsLevel = NI_ANS_LEVEL_VERY_AGRRESSIVE;
	ret = NI_MDK_ANS_SetConfig(iChannel, &stAnsCfg);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_ANS_SetConfig failed, ret=%x!\n", ret);
		pthread_mutex_unlock(&in_lock);
		return -1;
	}

	stAgcCfg.bSimpleMode = 0;
	stAgcCfg.unAgcIntensity.stAgcModeGain.enAgcMode = NI_AGC_MODE_FIXED;
	stAgcCfg.unAgcIntensity.stAgcModeGain.s16TargetDbfs = 3;
	stAgcCfg.unAgcIntensity.stAgcModeGain.s16GaindB = 15;

	ret = NI_MDK_AGC_SetConfig(iChannel, &stAgcCfg);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_AGC_SetConfig failed, ret=%x!\n", ret);
		pthread_mutex_unlock(&in_lock);
		return -1;
	}

	if (AudioStreamType_AAC == pAiDevice->stAioType && 0 == pAiDevice->g_aac_encinit)
	{
		ret = aac_enc_init(pAiDevice->stAioAttr.sample_rate);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("Error: aac_enc_init failed, ret=%x!\n", ret);
			pthread_mutex_unlock(&in_lock);
			return -1;
		}
		pAiDevice->g_aac_encinit = 1;
	}

	NI_MDK_CODEC_SetAinEn(CODEC_RIGHT_CHANNEL, NI_TRUE);
	NI_MDK_CODEC_SetAinEn(CODEC_LEFT_CHANNEL, NI_TRUE);

	NI_MDK_CODEC_SetAoutEn(CODEC_RIGHT_CHANNEL, NI_TRUE);
	NI_MDK_CODEC_SetAoutEn(CODEC_LEFT_CHANNEL, NI_TRUE);

	ret = NI_MDK_AENC_Start(iChannel);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_AENC_Start failed, ret=%x!\n", ret);
	}
	pthread_mutex_unlock(&in_lock);
	return 0;
}

/// 销毁音频输入设备
///
/// \param [in] iChannel 通道号
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int AudioInDestroy(int iChannel)
{
	int ret = -1;
	AiDevice_p pAiDevice = &GlobalDevice.AiDevice;

	pthread_mutex_lock(&in_lock);
	audio_lock_enter();

	if(AudioStreamType_AAC == pAiDevice->stAioType && 1 == pAiDevice->g_aac_encinit)
	{
		pAiDevice->g_aac_encinit = 0;
		aac_enc_deinit();
	}

	ret = NI_MDK_HPF_DeInit(iChannel);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_HPF_DeInit failed, ret=%x!\n", ret);
	}

	ret = NI_MDK_ANS_DeInit(iChannel);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_ANS_DeInit failed, ret=%x!\n", ret);
	}
	
	ret = NI_MDK_AGC_DeInit(iChannel);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_AGC_DeInit failed, ret=%x!\n", ret);
	}

	ret = NI_MDK_AENC_Stop(iChannel);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_AENC_Stop failed, ret=%x!\n", ret);
	}

	ret = NI_MDK_AENC_DestroyChn(iChannel);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_AENC_Stop failed, ret=%x!\n", ret);
	}

	audio_lock_leave();
	pthread_mutex_unlock(&in_lock);
	return 0;
}

/// 开始音频编码
///
/// \param [in] iChannel 通道号
/// \retval <0 开始失败
/// \retval 0 开始成功
int AudioInStart(int iChannel)
{
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
	
	pCaptureDevice->EncDevice[iChannel].StreamDevice[CHL_MAIN_T].AencStart = 1;
	return 0;
}

/// 停止音频编码
///
/// \param [in] iChannel 通道号
/// \retval <0 停止失败
/// \retval 0 停止成功
int AudioInStop(int iChannel)
{
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

	pCaptureDevice->EncDevice[iChannel].StreamDevice[CHL_MAIN_T].AencStart = 0;
	return 0;
}

/// 开始语音对讲
///
/// \param [in] iChannel 通道号
/// \retval <0 开始失败
/// \retval 0 开始成功
int AudioTalkStart(int iChannel)
{
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

	if (1 == pCaptureDevice->EncDevice[iChannel].StreamDevice[CHL_MAIN_T].NetTalkStart)
	{
		PRINT_ERROR("net talk is running\n");
		return -1;
	}

	pCaptureDevice->EncDevice[iChannel].StreamDevice[CHL_MAIN_T].NetTalkStart = 1;
	return 0;
}

/// 停止语音对讲
///
/// \param [in] iChannel 通道号
/// \retval <0 停止失败
/// \retval 0 停止成功
int AudioTalkStop(int iChannel)
{
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

	pCaptureDevice->EncDevice[iChannel].StreamDevice[CHL_MAIN_T].NetTalkStart = 0;
	return 0;
}

/// 设置音频数据编码的格式。
///
/// \param [in] iChannel 通道号。
/// \param [in] pFormat 指向音频编码格式结构AUDIOIN _FORMAT的指针。
/// \retval 0  设置成功
/// \retval <0  设置失败
int AudioInSetFormat(int iChannel, AUDIO_AIAOCONFIG_t* pAiFormat)
{
	AiDevice_p pAiDevice = &GlobalDevice.AiDevice;

	pAiDevice->stAioAttr.sample_rate  		= AudioRate_ConToLib_Table[pAiFormat->SampleRate];
	pAiDevice->stAioAttr.bit_width 			= AUDIO_BIT_WIDTH_32;
	pAiDevice->stAioAttr.channels			= 1;
	pAiDevice->stAioAttr.u32PacketPeriod 	= 40;
	pAiDevice->stAioAttr.enCodecType 		= AUDIO_INTERNAL_CODEC;
	//pAiDevice->stAioAttr.volume 			= 31;
	pAiDevice->stAioType 					= pAiFormat->streamtype;
	return 0;
}



/// 设置音频输入的音量。
///
/// \param [in] iChannel 通道号。
/// \param [in] dwLVolume 左声道音量，取值0-100。
/// \param [in] dwRVolume 右声道音量，取值0-100。
/// \retval 0  设置成功
/// \retval <0  设置失败
int AudioInSetVolume(int iChannel, DWORD dwLVolume, DWORD dwRVolume)
{
	int ret = -1;
	int value = 0;
	CODEC_GAIN_ATTR_S GainAttr;
	AiDevice_p pAiDevice = &GlobalDevice.AiDevice;

	memset(&GainAttr, 0, sizeof(CODEC_GAIN_ATTR_S));

	if (dwLVolume > 100)
	{
		PRINT_ERROR("this Volume [%d] not support\n", dwLVolume);
		return -1;
	}

	value = (int)((float)dwLVolume * 48 / 100 - 19);

	if (0 == dwLVolume)
	{
		ret = NI_MDK_CODEC_EnableMute(ANALOG_CONVERT_ADC, CODEC_LEFT_CHANNEL);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("Error: NI_MDK_CODEC_EnableMute failed, ret=%x!\n", ret);
			return -1;
		}
	}
	else
	{
		ret = NI_MDK_CODEC_DisableMute(ANALOG_CONVERT_ADC, CODEC_LEFT_CHANNEL);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("Error: NI_MDK_CODEC_DisableMute failed, ret=%x!\n", ret);
			return -1;
		}
	}

	ret = NI_MDK_CODEC_GetGainAttr(CODEC_LEFT_CHANNEL, &GainAttr);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_CODEC_GetGainAttr failed, ret=%x!\n", ret);
		return -1;
	}

	GainAttr.s8AdcAnalogGain = -5;
	GainAttr.s8AdcDigitalGain = value;
	GainAttr.enBoost = CODEC_GAINBOOST_15DB;
	ret = NI_MDK_CODEC_SetGainAttr(CODEC_LEFT_CHANNEL, &GainAttr);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_CODEC_SetGainAttr failed, ret=%x!\n", ret);
		return -1;
	}

	pAiDevice->stAioAttr.volume = value;

	return 0;
}

/// 设置对讲输出的音量。
///
/// \param [in] iChannel 通道号。
/// \param [in] dwLVolume 左声道音量，取值0-100。
/// \param [in] dwRVolume 右声道音量，取值0-100。
/// \retval 0  设置成功
/// \retval <0  设置失败
int AudioTalkSetVolume(int iChannel, DWORD dwLVolume, DWORD dwRVolume)
{
	int ret = -1;
	short value = -1;
	AoDevice_p pAoDevice = &GlobalDevice.AoDevice;
	CODEC_GAIN_ATTR_S GainAttr;
	memset(&GainAttr, 0, sizeof(CODEC_GAIN_ATTR_S));

	if (dwLVolume > 100)
	{
		PRINT_ERROR("this Volume [%d] not support\n", dwLVolume);
		return -1;
	}

	value = (short)((float)(dwLVolume) - 100) * 44 / 100;

	if (0 == dwLVolume)
	{
		ret = NI_MDK_CODEC_EnableMute(ANALOG_CONVERT_DAC, CODEC_LEFT_CHANNEL);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("Error: NI_MDK_CODEC_EnableMute failed, ret=%x!\n", ret);
			return -1;
		}
	}
	else
	{
		ret = NI_MDK_CODEC_DisableMute(ANALOG_CONVERT_DAC, CODEC_LEFT_CHANNEL);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("Error: NI_MDK_CODEC_DisableMute failed, ret=%x!\n", ret);
			return -1;
		}
	}

	ret = NI_MDK_CODEC_GetGainAttr(CODEC_LEFT_CHANNEL, &GainAttr);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_CODEC_GetGainAttr failed, ret=%x!\n", ret);
		return -1;
	}

	GainAttr.s16DacDigitalGain = value;
	GainAttr.enBoost = CODEC_GAINBOOST_15DB;
	ret = NI_MDK_CODEC_SetGainAttr(CODEC_LEFT_CHANNEL, &GainAttr);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_CODEC_SetGainAttr failed, ret=%x!\n", ret);
		return -1;
	}

	pAoDevice->stAioAttr.volume = value;

	return 0;
}

/// 设置音频输出的音量。
///
/// 单向对讲和语音播报
/// \param [in] iChannel 通道号。
/// \param [in] dwLVolume 左声道音量，取值0-100。
/// \param [in] dwRVolume 右声道音量，取值0-100。
/// \retval 0  设置成功
/// \retval <0  设置失败
int AudioOutSetVolume(int iChannel, DWORD dwLVolume, DWORD dwRVolume)
{
	int ret = -1;
	short value = -1;
	AoDevice_p pAoDevice = &GlobalDevice.AoDevice;
	CODEC_GAIN_ATTR_S GainAttr;
	memset(&GainAttr,0,sizeof(CODEC_GAIN_ATTR_S));

	if(dwLVolume > 100)
	{
		PRINT_ERROR("this Volume [%d] not support\n",dwLVolume);
		return -1;
	}

	value = (short)((float)(dwLVolume) - 100) * 44 / 100;

	if (0 == dwLVolume)
	{
		ret = NI_MDK_CODEC_EnableMute(ANALOG_CONVERT_DAC, CODEC_LEFT_CHANNEL);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("Error: NI_MDK_CODEC_EnableMute failed, ret=%x!\n", ret);
			return -1;
		}
	}
	else
	{
		ret = NI_MDK_CODEC_DisableMute(ANALOG_CONVERT_DAC, CODEC_LEFT_CHANNEL);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("Error: NI_MDK_CODEC_DisableMute failed, ret=%x!\n", ret);
			return -1;
		}
	}

	ret = NI_MDK_CODEC_GetGainAttr(CODEC_LEFT_CHANNEL, &GainAttr);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_CODEC_GetGainAttr failed, ret=%x!\n", ret);
		return -1;
	}

	GainAttr.s16DacDigitalGain = value;
	GainAttr.enBoost = CODEC_GAINBOOST_15DB;
	ret = NI_MDK_CODEC_SetGainAttr(CODEC_LEFT_CHANNEL, &GainAttr);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error: NI_MDK_CODEC_SetGainAttr failed, ret=%x!\n", ret);
		return -1;
	}

	pAoDevice->stAioAttr.volume = value;

	return 0;
}


/// 得到音频输入支持的特性。
///
/// \param [out] pCaps 指向音频输入特性结构AUDIOIN_CAPS的指针。
/// \retval 0  获取成功。
/// \retval <0  获取失败。
int AudioInGetCaps(AUDIOIN_CAPS * pCaps)
{
	return 0;
}

int AudioInGetCapsEx(int iChannel, AUDIOIN_CAPS_EX * pCaps)
{
	pCaps->EncTypeMask = (1 << AudioStreamType_G711A) | (1 << AudioStreamType_G711U) | (1 << AudioStreamType_AAC);
	pCaps->SampleRateMask = (1 << AudioRate_8K);
	pCaps->SampleBiteMask = (1 << AudioBit_16);
	return 0;
}

//dec
/////////////////////////////////////////////////////////////////////////////
int  ADECGetCap(ADEC_CAP* cap)
{
	return 0;
}

int ADECCreate(int OwnerType)
{
	return 0;
}

int ADECDestroy(int adchn)
{
	return 0;
}

int ADECStart(int adchn)
{
	return 0;
}

int ADECStop(int adchn)
{
	return 0;
}

int ADECReset(int adchn)
{
	return 0;
}

int ADECPause(int adchn, int pause)
{
	return 0;
}

int ADECSetFormat(int adchn, ADEC_FORMAT* format)
{
	return 0;
}

int ADECSendFrame(int adchn, void* data, int data_len)
{
	return 0;
}

int ADECSetSwitchCallBack(ADECSwitchCallBackFunc func)
{
    return 0;
}
