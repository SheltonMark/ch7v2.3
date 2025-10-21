#include "TdCommon.h"

RING_BUFF ENCODE_PCM_BUFF;

static short seg_aend[8] = 
{
	0x1F, 0x3F, 0x7F, 0xFF,
	0x1FF, 0x3FF, 0x7FF, 0xFFF
};

static short seg_uend[8] = 
{
	0x3F, 0x7F, 0xFF, 0x1FF,
	0x3FF, 0x7FF, 0xFFF, 0x1FFF
};

unsigned char _u2a[128] = 
{
	/* u- to A-law conversions */
	1, 1, 2, 2, 3, 3, 4, 4,
	5, 5, 6, 6, 7, 7, 8, 8,
	9, 10, 11, 12, 13, 14, 15, 16,
	17, 18, 19, 20, 21, 22, 23, 24,
	25, 27, 29, 31, 33, 34, 35, 36,
	37, 38, 39, 40, 41, 42, 43, 44,
	46, 48, 49, 50, 51, 52, 53, 54,
	55, 56, 57, 58, 59, 60, 61, 62,
	64, 65, 66, 67, 68, 69, 70, 71,
	72, 73, 74, 75, 76, 77, 78, 79,
	81, 82, 83, 84, 85, 86, 87, 88,
	89, 90, 91, 92, 93, 94, 95, 96,
	97, 98, 99, 100, 101, 102, 103, 104,
	105, 106, 107, 108, 109, 110, 111, 112,
	113, 114, 115, 116, 117, 118, 119, 120,
	121, 122, 123, 124, 125, 126, 127, 128
};

unsigned char _a2u[128] = 
{
	/* A- to u-law conversions */
	1, 3, 5, 7, 9, 11, 13, 15,
	16, 17, 18, 19, 20, 21, 22, 23,
	24, 25, 26, 27, 28, 29, 30, 31,
	32, 32, 33, 33, 34, 34, 35, 35,
	36, 37, 38, 39, 40, 41, 42, 43,
	44, 45, 46, 47, 48, 48, 49, 49,
	50, 51, 52, 53, 54, 55, 56, 57,
	58, 59, 60, 61, 62, 63, 64, 64,
	65, 66, 67, 68, 69, 70, 71, 72,
	73, 74, 75, 76, 77, 78, 79, 79,
	80, 81, 82, 83, 84, 85, 86, 87,
	88, 89, 90, 91, 92, 93, 94, 95,
	96, 97, 98, 99, 100, 101, 102, 103,
	104, 105, 106, 107, 108, 109, 110, 111,
	112, 113, 114, 115, 116, 117, 118, 119,
	120, 121, 122, 123, 124, 125, 126, 127
};

static short _search(int val, short *table, int size)
{
	unsigned int i = 0;
	for (i = 0; i < size; i++)
	{
		if (val <= *table++)
		{
			return (i);
		}		
	}
	return (size);
}

/*
 * _linear2alaw() - Convert a 16-bit linear PCM value to 8-bit A-law
 *
 * _linear2alaw() accepts an 16-bit integer and encodes it as A-law data.
 *
 *Linear Input CodeCompressed Code
 *---------------------------------------
 *0000000wxyza000wxyz
 *0000001wxyza001wxyz
 *000001wxyzab010wxyz
 *00001wxyzabc011wxyz
 *0001wxyzabcd100wxyz
 *001wxyzabcde101wxyz
 *01wxyzabcdef110wxyz
 *1wxyzabcdefg111wxyz
 *
 * For further information see John C. Bellamy's Digital Telephony, 1982,
 * John Wiley & Sons, pps 98-111 and 472-476.
 */
unsigned char _linear2alaw(int pcm_val) /* 2's complement (16-bit range) */
{
	unsigned int seg = 0;
	unsigned int mask = 0;
	unsigned char aval = 0;

	pcm_val = pcm_val >> 3;

	if (pcm_val >= 0)
	{
		mask = 0xD5; /* sign (7th) bit = 1 */
	}
	else
	{
		mask = 0x55; /* sign bit = 0 */
		pcm_val = -pcm_val - 1;
	}

	/* Convert the scaled magnitude to segment number. */
	seg = _search(pcm_val, seg_aend, 8);

	/* Combine the sign, segment, and quantization bits. */

	if (seg >= 8) /* out of range, return maximum value. */
	{
		return (unsigned char)(0x7F ^ mask);
	}
	else
	{
		aval = (unsigned char)seg << SEG_SHIFT;
		if (seg < 2)
		{
			aval |= (pcm_val >> 1) & QUANT_MASK;
		}
		else
		{
			aval |= (pcm_val >> seg) & QUANT_MASK;
		}
		return (aval ^ mask);
	}
}

/*
 * _alaw2linear() - Convert an A-law value to 16-bit linear PCM
 *
 */
int _alaw2linear(unsigned char a_val)
{
	unsigned int t = 0;
	unsigned int seg = 0;

	a_val ^= 0x55;

	t = (a_val & QUANT_MASK) << 4;
	seg = ((unsigned)a_val & SEG_MASK) >> SEG_SHIFT;

	switch (seg)
	{
		case(0):
			t += 8;
			break;
		case(1):
			t += 0x108;
			break;
		default:
			t += 0x108;
			t <<= seg - 1;
	}

	return ((a_val & SIGN_BIT) ? t : -t);
}

/*
 * _linear2ulaw() - Convert a linear PCM value to u-law
 *
 * In order to simplify the encoding process, the original linear magnitude
 * is biased by adding 33 which shifts the encoding range from (0 - 8158) to
 * (33 - 8191). The result can be seen in the following encoding table:
 *
 *Biased Linear Input CodeCompressed Code
 *---------------------------------------
 *00000001wxyza000wxyz
 *0000001wxyzab001wxyz
 *000001wxyzabc010wxyz
 *00001wxyzabcd011wxyz
 *0001wxyzabcde100wxyz
 *001wxyzabcdef101wxyz
 *01wxyzabcdefg110wxyz
 *1wxyzabcdefgh111wxyz
 *
 * Each biased linear code has a leading 1 which identifies the segment
 * number. The value of the segment number is equal to 7 minus the number
 * of leading 0's. The quantization interval is directly available as the
 * four bits wxyz.  * The trailing bits (a - h) are ignored.
 *
 * Ordinarily the complement of the resulting code word is used for
 * transmission, and so the code word is complemented before it is returned.
 *
 * For further information see John C. Bellamy's Digital Telephony, 1982,
 * John Wiley & Sons, pps 98-111 and 472-476.
 */
unsigned char _linear2ulaw(short pcm_val) /* 2's complement (16-bit range) */
{
	short seg = 0;
	short mask = 0;
	unsigned char uval = 0;

	/* Get the sign and the magnitude of the value. */
	pcm_val = pcm_val >> 2;
	if (pcm_val < 0)
	{
		pcm_val = -pcm_val;
		mask = 0x7F;
	}
	else
	{
		mask = 0xFF;
	}
	if (pcm_val > CLIP)
	{
		pcm_val = CLIP; /* clip the magnitude */
	}
		
	pcm_val += (BIAS >> 2);

	/* Convert the scaled magnitude to segment number. */
	seg = _search(pcm_val, seg_uend, 8);

	/*
	 * Combine the sign, segment, quantization bits;
	 * and complement the code word.
	 */
	if (seg >= 8) /* out of range, return maximum value. */
	{
		return (unsigned char)(0x7F ^ mask);
	}
	else
	{
		uval = (unsigned char)(seg << 4) | ((pcm_val >> (seg + 1)) & 0xF);
		return (uval ^ mask);
	}
}

/*
 * _ulaw2linear() - Convert a u-law value to 16-bit linear PCM
 *
 * First, a biased linear code is derived from the code word. An unbiased
 * output can then be obtained by subtracting 33 from the biased code.
 *
 * Note that this function expects to be passed the complement of the
 * original code word. This is in keeping with ISDN conventions.
 */
short _ulaw2linear(unsigned char u_val)
{
	short t = 0;

	/* Complement to obtain normal u-law value. */
	u_val = ~u_val;

	/*
	 * Extract and bias the quantization bits. Then
	 * shift up by the segment number and subtract out the bias.
	 */
	t = ((u_val & QUANT_MASK) << 3) + BIAS;
	t <<= ((unsigned)u_val & SEG_MASK) >> SEG_SHIFT;
	return ((u_val & SIGN_BIT) ? (BIAS - t) : (t - BIAS));
}

/* A-law to u-law conversion */
unsigned char _alaw2ulaw(unsigned char aval)
{
	aval &= 0xff;
	return (unsigned char)((aval & 0x80) ? (0xFF ^ _a2u[aval ^ 0xD5]) : (0x7F ^ _a2u[aval ^ 0x55]));
}

/* u-law to A-law conversion */
unsigned char _ulaw2alaw(unsigned char uval)
{
	uval &= 0xff;
	return (unsigned char)((uval & 0x80) ? (0xD5 ^ (_u2a[0xFF ^ uval] - 1)) : (unsigned char)(0x55 ^ (_u2a[0x7F ^ uval] - 1)));
}

int _g711_encode(char *a_psrc, char *a_pdst, unsigned int in_data_len, unsigned char type)
{
	unsigned int i = 0;
	short *psrc = NULL;
	unsigned int out_data_len = 0;

	if (NULL == a_psrc|| NULL == a_pdst)
	{
		PRINT_ERROR("_g711_encode a_psrc or a_pdst null!!!\n");
		return -1;
	}

	if (0 == in_data_len)
	{
		PRINT_ERROR("_g711_encode in_data_len = 0!!!\n");
		return -1;
	}

	psrc = (short *)a_psrc;
	out_data_len = in_data_len / sizeof(short);

	if (G711_A_LAW == type)
	{
		for (i = 0; i < out_data_len; i++)
		{
			a_pdst[i] = (char)_linear2alaw(psrc[i]);
		}
	}
	else if (G711_MU_LAW == type)
	{
		for (i = 0; i < out_data_len; i++)
		{
			a_pdst[i] = (char)_linear2ulaw(psrc[i]);
		}
	}
	else
	{
		PRINT_ERROR("_g711_encode unknown type= %d\n", type);
		return -1;
	}

	return i;
}

int _g711_decode(char *a_psrc, char *a_pdst, unsigned int in_data_len, unsigned char type)
{
	unsigned int i = 0;
	short *pdst = NULL;
	unsigned int out_data_len = 0;

	if (NULL == a_psrc || NULL == a_pdst)
	{
		PRINT_ERROR("_g711_decode a_psrc or a_pdst null!!!\n");
		return -1;
	}

	if (0 == in_data_len)
	{
		PRINT_ERROR("_g711_decode in_data_len = 0!!!\n");
		return -1;
	}

	pdst = (short *)a_pdst;
	out_data_len = in_data_len / sizeof(char);

	if (G711_A_LAW == type)
	{
		for (i = 0; i < out_data_len; i++)
		{
			pdst[i] = (short)_alaw2linear((unsigned char)a_psrc[i]);
		}
	}
	else if (G711_MU_LAW == type)
	{
		for (i = 0; i < out_data_len; i++)
		{
			pdst[i] = (short)_ulaw2linear((unsigned char)a_psrc[i]);
		}
	}
	else
	{
		PRINT_ERROR("_g711_decode unknown type= %d\n", type);
		return -1;
	}

	return (i * sizeof(short));
}

/// g711a编码
///
/// \param [in] pcm_stream    pcm数据
/// \param [in] pcm_len       pcm数据长度
/// \param [out] stream   	  g711a数据
/// \param [out] len     	  g711a数据长度
/// \retval <0 编码失败
/// \retval 0  编码成功
int pcm_2_g711a(unsigned char *pcm_stream, unsigned int pcm_len, unsigned char *stream, unsigned int *len)
{
	int ret = -1;

	if (NULL == pcm_stream || NULL == stream)
	{
		PRINT_ERROR("pcm_2_g711a pcm_stream or stream null!!!\n");
		return -1;
	}

	if (0 == pcm_len)
	{
		PRINT_ERROR("pcm_2_g711a pcm_len = 0!!!\n");
		return -1;
	}

	ret = _g711_encode((char *)pcm_stream, (char *)stream, pcm_len, G711_A_LAW);
	if(ret < 0)
	{
		PRINT_ERROR("pcm_2_g711a _g711_encode error!!!\n");
		return -1;
	}

	*len  = ret;
	return 0;
}

/// g711u编码
///
/// \param [in] pcm_stream    pcm数据
/// \param [in] pcm_len       pcm数据长度
/// \param [out] stream   	  g711u数据
/// \param [out] len     	  g711u数据长度
/// \retval <0 编码失败
/// \retval 0  编码成功
int pcm_2_g711u(unsigned char *pcm_stream, unsigned int pcm_len, unsigned char *stream, unsigned int *len)
{
	int ret = -1;

	if (NULL == pcm_stream || NULL == stream)
	{
		PRINT_ERROR("pcm_2_g711u pcm_stream or stream null!!!\n");
		return -1;
	}

	if (0 == pcm_len)
	{
		PRINT_ERROR("pcm_2_g711u pcm_len = 0!!!\n");
		return -1;
	}

	ret = _g711_encode((char *)pcm_stream, (char *)stream, pcm_len, G711_MU_LAW);
	if (ret < 0)
	{
		PRINT_ERROR("pcm_2_g711u _g711_encode error!!!\n");
		return -1;
	}

	*len = ret;

	return 0;
}

/// g711a解码
///
/// \param [in] stream   	  g711a数据
/// \param [in] len     	  g711a数据长度
/// \param [out] pcm_stream    pcm数据
/// \param [out] pcm_len       pcm数据长度
/// \retval <0 解码失败
/// \retval 0  解码成功
int g711a_2_pcm(unsigned char *stream, unsigned int len, unsigned char *pcm_stream, unsigned int *pcm_len)
{
	int ret = -1;

	if (NULL == pcm_stream || NULL == stream)
	{
		PRINT_ERROR("g711a_2_pcm pcm_stream or stream null!!!\n");
		return -1;
	}

	if (0 == len)
	{
		PRINT_ERROR("g711a_2_pcm len = 0!!!\n");
		return -1;
	}

	ret = _g711_decode((char *)stream, (char *)pcm_stream, len, G711_A_LAW);
	if (ret < 0)
	{
		PRINT_ERROR("g711a_2_pcm _g711_encode error!!!\n");
		return -1;
	}

	*pcm_len = ret;

	return 0;
}

/// g711a解码码
///
/// \param [in] stream   	  g711u数据
/// \param [in] len     	  g711u数据长度
/// \param [out] pcm_stream    pcm数据
/// \param [out] pcm_len       pcm数据长度
/// \retval <0 解码失败
/// \retval 0  解码成功
int g711u_2_pcm(unsigned char *stream, unsigned int len, unsigned char *pcm_stream, unsigned int *pcm_len)
{
	int ret = -1;

	if (NULL == pcm_stream || NULL == stream)
	{
		PRINT_ERROR("g711u_2_pcm pcm_stream or stream null!!!\n");
		return -1;
	}

	if (0 == len)
	{
		PRINT_ERROR("g711u_2_pcm len = 0!!!\n");
		return -1;
	}

	ret = _g711_decode((char *)stream, (char *)pcm_stream, len, G711_MU_LAW);
	if (ret < 0)
	{
		PRINT_ERROR("g711u_2_pcm _g711_encode error!!!\n");
		return -1;
	}

	*pcm_len = ret;

	return 0;
}

/// aac编码初始化
///
/// \param [in] sample_rate    采样率
/// \retval <0 初始化失败
/// \retval 0  初始化成功
int aac_enc_init(int sample_rate)
{
	int ret = -1;
	unsigned int msize = 0;
	char *pVersion = NULL;
	char *pCopyRight = NULL;
	unsigned long samplesInput = 0;
	unsigned long maxBytesOutput = 0;
	faacEncConfigurationPtr myFormat;
	AiDevice_p pAiDevice = &GlobalDevice.AiDevice;

	memset(&myFormat, 0, sizeof(faacEncConfigurationPtr));
	if (-1 == faacEncGetVersion(&pVersion, &pCopyRight))
	{
		return -1;
	}

	pAiDevice->aac_handle = faacEncOpen(sample_rate, 1, &samplesInput, &maxBytesOutput);
	if (NULL == pAiDevice->aac_handle)
	{
		PRINT_ERROR("Error: faacEncOpen failed\n");
		return -1;
	}

	myFormat = faacEncGetCurrentConfiguration(pAiDevice->aac_handle);
	myFormat->aacObjectType = LOW;
	myFormat->mpegVersion = MPEG4;
	myFormat->useTns = 0;
	myFormat->allowMidside = 1;
	myFormat->bandWidth = 0;
	myFormat->inputFormat = FAAC_INPUT_16BIT;
	ret = faacEncSetConfiguration(pAiDevice->aac_handle, myFormat);
	if (ret != 1)
	{
		faacEncClose(pAiDevice->aac_handle);
		pAiDevice->aac_handle = NULL;
		PRINT_ERROR("Error: faacEncSetConfiguration failed, ret=%x!\n", ret);
		return -1;
	}
	if (FAAC_INPUT_16BIT == myFormat->inputFormat)
	{
		msize = samplesInput * sizeof(short);
	}
	else if (FAAC_INPUT_24BIT == myFormat->inputFormat)
	{
		msize = samplesInput * sizeof(char) * 3;
	}
	else if (FAAC_INPUT_32BIT == myFormat->inputFormat)
	{
		msize = samplesInput * sizeof(int);
	}
	else if (FAAC_INPUT_FLOAT == myFormat->inputFormat)
	{
		msize = samplesInput * sizeof(float);
	}
	else
	{
		msize = samplesInput;
	}

	pAiDevice->msize = msize;
	pAiDevice->samplesInput = samplesInput;
	pAiDevice->outputSize = maxBytesOutput;

	ENCODE_PCM_BUFF.head = 0;
	ENCODE_PCM_BUFF.tail = 0;
	ENCODE_PCM_BUFF.tatol_len = AUDIO_PCM_BUFFER_SIZE;
	if (NULL == ENCODE_PCM_BUFF.buf)
	{
		ENCODE_PCM_BUFF.buf = malloc(ENCODE_PCM_BUFF.tatol_len);
		if (NULL == ENCODE_PCM_BUFF.buf)
		{
			PRINT_ERROR("ENCODE_PCM_BUFF.buf malloc failed!\n");
			faacEncClose(pAiDevice->aac_handle);
			pAiDevice->aac_handle = NULL;
			return -1;
		}
	}

	return 0;
}

/// aac编码去初始化
///
/// \retval <0 去初始化失败
/// \retval 0  去初始化成功
int aac_enc_deinit()
{
	AiDevice_p pAiDevice = &GlobalDevice.AiDevice;

	while (pAiDevice->g_aac_encoding)
	{
		usleep(0);
	}

	free(ENCODE_PCM_BUFF.buf);
	memset(&ENCODE_PCM_BUFF, 0, sizeof(RING_BUFF));
	ENCODE_PCM_BUFF.buf = NULL;

	faacEncClose(pAiDevice->aac_handle);

	pAiDevice->aac_handle = NULL;

	return 0;
}


/// aac编码
///
/// \param [in] pcm_stream    pcm数据
/// \param [in] pcm_len       pcm数据长度
/// \param [out] aac_stream   aac数据
/// \param [out] aac_len      aac数据长度
/// \retval <0 编码失败
/// \retval 0  编码成功
int pcm_2_aac(unsigned char *pcm_stream, unsigned int pcm_len, unsigned char *aac_stream, int *aac_len)
{
	int ret = -1;
	AiDevice_p pAiDevice = &GlobalDevice.AiDevice;
	unsigned char aac_Buffer[AUDIO_AAC_BUFFER_SIZE] = {0};
	int aac_size = 0;

	if (NULL == pcm_stream || NULL == aac_stream)
	{
		PRINT_ERROR("AudioInaacEncode error\n");
		return -1;
	}
	pAiDevice->g_aac_encoding = 1;
	ret = write_ring_buff(&ENCODE_PCM_BUFF, pcm_stream, pcm_len);
	if (0 != ret)
	{
		PRINT_ERROR("size %d > ENCODE_PCM_BUFF %d\n", pcm_len, ENCODE_PCM_BUFF.tatol_len);
		pAiDevice->g_aac_encoding = 0;
		return -1;
	}

	if (ENCODE_PCM_BUFF.tail - ENCODE_PCM_BUFF.head < pAiDevice->msize)
	{
		pAiDevice->g_aac_encoding = 0;
		return -1;
	}

	ret = read_ring_buff(&ENCODE_PCM_BUFF, aac_Buffer, pAiDevice->msize);
	if (0 != ret)
	{
		PRINT_ERROR("read_ring_buff fail\n");
		pAiDevice->g_aac_encoding = 0;
		return -1;
	}

	aac_size = faacEncEncode(pAiDevice->aac_handle, (int *)aac_Buffer, pAiDevice->samplesInput, aac_stream, pAiDevice->outputSize);
	if (aac_size <= 0)
	{
		PRINT_ERROR("faacEncEncode fail , aac_size = %d\n", aac_size);
		pAiDevice->g_aac_encoding = 0;
		return -1;
	}
	*aac_len = aac_size;
	pAiDevice->g_aac_encoding = 0;

	return 0;
}
