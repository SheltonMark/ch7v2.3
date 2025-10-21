#include "TdCommon.h"

#define     SIGN_BIT    (0x80)  /* Sign bit for a A-law byte. */
#define     QUANT_MASK  (0xf) /* Quantization field mask. */
#define     NSEGS       (8)        /* Number of A-law segments. */
#define     SEG_SHIFT   (4)    /* Left shift for segment number. */
#define     SEG_MASK    (0x70)  /* Segment field mask. */
#define     BIAS        (0x84)      /* Bias for linear code. */
#define     CLIP        8159

#define     G711_A_LAW  (0)
#define     G711_MU_LAW (1)

/// g711a编码
///
/// \param [in] pcm_stream    pcm数据
/// \param [in] pcm_len       pcm数据长度
/// \param [out] stream   	  g711a数据
/// \param [out] len     	  g711a数据长度
/// \retval <0 编码失败
/// \retval 0  编码成功
int pcm_2_g711a(unsigned char *pcm_stream, unsigned int pcm_len, unsigned char *stream, unsigned int *len);

/// g711u编码
///
/// \param [in] pcm_stream    pcm数据
/// \param [in] pcm_len       pcm数据长度
/// \param [out] stream   	  g711u数据
/// \param [out] len     	  g711u数据长度
/// \retval <0 编码失败
/// \retval 0  编码成功
int pcm_2_g711u(unsigned char *pcm_stream, unsigned int pcm_len, unsigned char *stream, unsigned int *len);

/// g711a解码
///
/// \param [in] stream   	  g711a数据
/// \param [in] len     	  g711a数据长度
/// \param [out] pcm_stream    pcm数据
/// \param [out] pcm_len       pcm数据长度
/// \retval <0 解码失败
/// \retval 0  解码成功
int g711a_2_pcm(unsigned char *stream, unsigned int len, unsigned char *pcm_stream, unsigned int *pcm_len);

/// g711a解码码
///
/// \param [in] stream   	  g711u数据
/// \param [in] len     	  g711u数据长度
/// \param [out] pcm_stream    pcm数据
/// \param [out] pcm_len       pcm数据长度
/// \retval <0 解码失败
/// \retval 0  解码成功
int g711u_2_pcm(unsigned char *stream, unsigned int len, unsigned char *pcm_stream, unsigned int *pcm_len);

/// aac编码
///
/// \param [in] pcm_stream    pcm数据
/// \param [in] pcm_len       pcm数据长度
/// \param [out] aac_stream   aac数据
/// \param [out] aac_len      aac数据长度
/// \retval <0 编码失败
/// \retval 0  编码成功
int pcm_2_aac(unsigned char *pcm_stream, unsigned int pcm_len, unsigned char *aac_stream, int *aac_len);

/// aac编码初始化
///
/// \param [in] sample_rate    采样率
/// \retval <0 初始化失败
/// \retval 0  初始化成功
int aac_enc_init(int sample_rate);

/// aac编码去初始化
///
/// \retval <0 去初始化失败
/// \retval 0  去初始化成功
int aac_enc_deinit();
