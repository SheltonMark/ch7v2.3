//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang Dahua Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:	
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#ifndef __AUDIO_API_H__
#define __AUDIO_API_H__

//#include "base.h"

#include "TdCommon.h"

#define AUDIO_ONE_FRAME_SIZE 1280
#define AUDIO_ONE_FRAME_MAXSIZE 320
#define AUDIO_AAC_BUFFER_SIZE 2048
#define AUDIO_PCM_BUFFER_SIZE 4096

#ifdef __cplusplus
extern "C" {
#endif
	
/// \defgroup AudioOutAPI API Audio Output 
/// 音频解码与输出控制设备接口，独立于回放设备。
/// \n 调用流程图:
/// \code
///    ===============================================
///               |
///          AudioCreate
///               |------------+------------------+
///               |            |                  |  
///               |      AudioOutSetFormat        | 
///               |            |                  |
///               |      AudioOutStart            |	
///        AudioSwitch         |--------------+   |
///               |      AudioOutPlay       |   |
///               |            |--------------+   |
///               |        AudioOutStop           |
///               |            |                  |
///               |------------+------------------+
///          AudioDestory
///               |
///    ===============================================
/// \endcode
/// @{

enum audio_switch_t{
	AUDIO_SWITCH_MONITOR,	///< 切换到监视输出。
	AUDIO_SWITCH_PLAY,		///< 切换到回放输出。
	AUDIO_SWITCH_SPEAK,		///语音对讲
	AUDIO_SWITCH_MIXED,
	AUDIO_SWITCH_MUTE		///< 静音。
};
	
typedef enum AudioStreamType_e
{
	AudioStreamType_PCM,
	AudioStreamType_G711A,
	AudioStreamType_G711U,
	AudioStreamType_AAC,
	AudioStreamType_G726,
	AudioStreamType_BUTT,
}AudioStreamType_e;

typedef enum AudioRate_e
{
	AudioRate_8K,
	AudioRate_16K,
	AudioRate_32K,
	AudioRate_48K,
	AudioRate_110K,
	AudioRate_220K,
	AudioRate_441K,
	AudioRate_BUTT,
}AudioRate_e;

typedef enum AC_SAMPLE_RATE
{
	AC_SR_8K = 8000,
	AC_SR_16K = 16000,
	AC_SR_32K = 32000,
	AC_SR_48K = 48000,

	AC_SR_110K = 11025,
	AC_SR_220K = 22050,
	AC_SR_441K = 44100,
	AC_SR_DUMMY = 0x1fffffff,
}AC_SAMPLE_RATE;

typedef enum Packets_AudioStreamType_e
{
	Packets_AudioStreamType_PCM8      = 7,		
	Packets_AudioStreamType_G711U     = 10,
	Packets_AudioStreamType_G711A     = 14,
	Packets_AudioStreamType_PCM16     = 16,
	Packets_AudioStreamType_G726_16K  = 17,
	Packets_AudioStreamType_G726_32K  = 19,
	Packets_AudioStreamType_AAC       = 26,
}Packets_AudioStreamType_e;

typedef enum Packets_AudioRate_e
{
	Packets_AudioRate_8K    = 2,
	Packets_AudioRate_110K  = 3,
	Packets_AudioRate_16K   = 4,
	Packets_AudioRate_220K  = 6,
	Packets_AudioRate_32K   = 7,
	Packets_AudioRate_441K  = 8,
	Packets_AudioRate_48K   = 9,
}Packets_AudioRate_e;

typedef enum AudioBit_e
{
	AudioBit_16,
	AudioBit_24,
	AudioBit_BUTT,
}AudioBit_e;

typedef struct tagAUDIOOUT_FORMAT
{
	DWORD	BitRate;	/**< 码流大小，kbps为单位，比如192kbps，128kbps。*/
	DWORD	SampleRate;	/**< 采样率，Hz为单位，比如44100Hz。*/
	DWORD	SampleBit;		/*!< 采样的位深 */
	int		EncodeType;		/*!< 编码方式，参照audio_encode_type定义 */
	DWORD	sliceSize;    // 160 or 320
	int 		reserved;   //保留位
} AUDIOOUT_FORMAT;

typedef struct AUDIO_AIAOCONFIG
{
	AudioStreamType_e 	streamtype;
	AudioRate_e 		SampleRate;
	int 				SampleBit;		/*仅支持16bit*/
}AUDIO_AIAOCONFIG_t,*AUDIO_AIAOCONFIG_p;

#define MAX_SIO_DEV 6
typedef struct
{
	AC_SAMPLE_RATE		sample_rate;
	AUDIO_BIT_WIDTH_E 	bit_width;
	AUDIO_CODEC_E 		enCodecType;		
	unsigned int 		channels;			/*通道个数 */
	unsigned int  		u32PacketPeriod;	/*一帧数据的时长 */
	int 				volume;
} AC_CONFIG;

typedef struct AiDevice_s
{
	int ComDevCount;
	int ComChanCount;
	AC_CONFIG stAioAttr;
	AudioStreamType_e stAioType;
	faacEncHandle aac_handle;
	unsigned int msize;
	unsigned int samplesInput;
	unsigned long outputSize;
	unsigned int g_aac_encinit;
	unsigned int g_aac_encoding;
	//G726_HANDLE g726_handle;
	unsigned int Audio_Record_Frame_Period;
}AiDevice_t, *AiDevice_p;

typedef struct AoDevice_s
{
	AC_CONFIG stAioAttr;
	AudioStreamType_e stAioType;
	NeAACDecHandle aac_handle;
	//G726_HANDLE g726_handle;
	unsigned int g_aac_decoding;
	unsigned int Audio_Play_Frame_Period;
}AoDevice_t, *AoDevice_p;

/// 创建音频设备
///
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int AudioCreate(void);


/// 销毁音频设备
///
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int AudioDestory(void);

/// 创建音频输出设备
///
/// \param [in] iChannel 通道号
/// \retval <0 创建失败
/// \retval 0 创建成功
int AudioOutCreate(int iChannel);

/// 销毁音频输出设备
///
/// \param [in] iChannel 通道号
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int AudioOutDestroy(int iChannel);

/// 获取AO缓冲字节数
///
/// \param 无
/// \return>0 缓冲字节数
/// \retval -1 获取失败
int AudioOutGetBufSize(void);

/// 关闭回声消除
/// 须在AO创建前使用
///
/// \param [in] iChannel 通道号
/// \param [in] Start 是否开启回声消除
/// \return>0 设置失败
/// \retval 0 设置成功
int AudioOutAecStart(int iChannel, int Start);

/// 语音对讲解码数据填充
///
/// \param [in] src 解码数据缓冲地址
/// \param [in] size 解码数据缓冲长度
/// \return>0 填充失败
/// \retval 0 填充成功
int AudioOutPlay(unsigned char *src, int size);

/// 设置数据的压缩格式
///
/// \param [in] coderType 压缩格式
/// \return>0 设置失败
/// \retval 0 设置成功
int AudioOutSetFormat(int iChannel, AUDIO_AIAOCONFIG_t* pAoFormat);

/// @}
/************************************************************************
	audio in
************************************************************************/

enum audio_encode_type
{
	NONE = 0,
	G729_8KBIT,
	G726_16KBIT,
	G726_24KBIT,
	G726_32KBIT,
	G726_40KBIT,
	PCM_8TO16BIT,
	PCM_ALAW,
	PCM_ULAW,
	ADPCM8K16BIT,
	ADPCM16K16BIT,
	G711_ALAW,   //zhuangke
	MPEG2_LAYER1,
	AMR8K16BIT,
	G711_ULAW,
	IMA_ADPCM_8K16BIT,
	AACLC_48K16BIT,
	AACLC_8K16BIT = 26,
};

typedef struct tagAUDIOIN_FORMAT
{
	DWORD	BitRate;	/**< 码流大小，kbps为单位，比如192kbps，128kbps。*/
	DWORD	SampleRate;	/**< 采样率，Hz为单位，比如44100Hz。*/
	DWORD	SampleBit;		/*!< 采样的位深 */
	int		EncodeType;		/*!< 编码方式，参照audio_encode_type定义 */
} AUDIOIN_FORMAT;

typedef struct tagAUDIOIN_CAPS
{
	DWORD	Reserved;	/**< 保留。*/
} AUDIOIN_CAPS;

typedef struct tagAUDIOIN_CAPS_EX
{
    DWORD    EncTypeMask;     /* enum AudioStreamType_e 左移掩码 */
	DWORD    SampleRateMask;  /* 支持的采样率AudioRate_e左移掩码 */
	DWORD    SampleBiteMask;   	  /* 支持的采样位数AudioBit_e左移掩码 */
    DWORD    Reserved[28];    /* 保留字节 */
} AUDIOIN_CAPS_EX;

typedef struct 
{
    int bAecOpen;                     /* 回音消除 */
    int bAnrOpen;                     /* 降噪 */
    int bAgcOpen;                     /* 自动增益 */
    int reserved[13];
} AUDIOIN_VQE;

//////////////////////////////////////////////////////////////////////////////
/// \defgroup AudioInAPI API Audio Input
/// 音频数据编码接口，独立于捕获设备。
/// \n 调用流程图:
/// \code
///    =========================================
///                   |                         
///      *AudioInGetChannels,AudioInGetCaps      
///                   |                         
///              AudioInCreate                  
///       +-----------|----------------+        	 
///       |     AudioInSetFormat       |        	
///       +-----------|                |        	 
///       |      AudioInStart          |        	 
///       | +---------|         AudioInSetVolume
///       | |  AudioInGetBuffer        |        	 
///       | +---------|                |           	
///       |      AudioInStop           |         	
///       +-----------|----------------+         	
///             AudioInDestroy                  
///                   |                         
///    =========================================
/// \endcode
/// @{

/// 音频输入格式
/*typedef struct AUDIOIN_FORMAT
{
	DWORD	BitRate;	///< 码流大小，kbps为单位，比如192kbps，128kbps。
	DWORD	SampleRate;	///< 采样率，Hz为单位，比如44100Hz。
} AUDIOIN_FORMAT;

/// 音频输入设备特性
typedef struct AUDIOIN_CAPS
{
	DWORD	Reserved;	///< 保留。
} AUDIOIN_CAPS;
*/

/// 得到音频输入通道数
///
/// \param 无
/// \return 音频输入通道数目。
int AudioInGetChannels(void);

/// 得到音频输出通道数
///
/// \param 无
/// \return 音频输入通道数目。
int AudioOutGetChannels(void);

/// 创建音频输入设备
///
/// \param [in] iChannel 通道号
/// \retval <0 创建失败
/// \retval 0 创建成功
int AudioInCreate(int iChannel);


/// 销毁音频输入设备
///
/// \param [in] iChannel 通道号
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int AudioInDestroy(int iChannel);


/// 开始音频编码
///
/// \param [in] iChannel 通道号
/// \retval <0 开始失败
/// \retval 0 开始成功
int AudioInStart(int iChannel);


/// 停止音频编码
///
/// \param [in] iChannel 通道号
/// \retval <0 停止失败
/// \retval 0 停止成功
int AudioInStop(int iChannel);

/// 开始语音对讲
///
/// \param [in] iChannel 通道号
/// \retval <0 开始失败
/// \retval 0 开始成功
int AudioTalkStart(int iChannel);

/// 停止语音对讲
///
/// \param [in] iChannel 通道号
/// \retval <0 开始失败
/// \retval 0 开始成功
int AudioTalkStop(int iChannel);

/// 设置音频数据编码的格式。
///
/// \param [in] iChannel 通道号。
/// \param [in] pFormat 指向音频编码格式结构AUDIOIN _FORMAT的指针。
/// \retval 0  设置成功
/// \retval <0  设置失败
int AudioInSetFormat(int iChannel, AUDIO_AIAOCONFIG_t* pAiFormat);

/// 设置音频数据输入输出格式
///
/// \param [in] pAiFormat 音频数据输入格式
/// \param [in] pAoFormat 音频数据输出格式
/// \retval 0  设置成功
/// \retval <0  设置失败
int AudioAiAoSetFormat(AUDIO_AIAOCONFIG_t pAiFormat,AUDIO_AIAOCONFIG_t pAoFormat);

/// 设置音频输入的音量。
///
/// \param [in] iChannel 通道号。
/// \param [in] dwLVolume 左声道音量，取值0-100。
/// \param [in] dwRVolume 右声道音量，取值0-100。
/// \retval 0  设置成功
/// \retval <0  设置失败
int AudioInSetVolume(int iChannel, DWORD dwLVolume, DWORD dwRVolume);

/// 设置对讲输出的音量。
///
/// \param [in] iChannel 通道号。
/// \param [in] dwLVolume 左声道音量，取值0-100。
/// \param [in] dwRVolume 右声道音量，取值0-100。
/// \retval 0  设置成功
/// \retval <0  设置失败
int AudioTalkSetVolume(int iChannel, DWORD dwLVolume, DWORD dwRVolume);

/// 设置音频输出的音量。
///
/// 单向对讲和语音播报
/// \param [in] iChannel 通道号。
/// \param [in] dwLVolume 左声道音量，取值0-100。
/// \param [in] dwRVolume 右声道音量，取值0-100。
/// \retval 0  设置成功
/// \retval <0  设置失败
int  AudioOutSetVolume(int iChannel, DWORD dwLVolume, DWORD dwRVolume);


/// 得到音频输入支持的特性。
///
/// \param [out] pCaps 指向音频输入特性结构AUDIOIN_CAPS的指针。
/// \retval 0  获取成功。
/// \retval <0  获取失败。
int AudioInGetCaps(AUDIOIN_CAPS * pCaps);


/// 得到音频输入支持的特性(新接口)。
///
/// \param [out] pCaps 指向音频输入特性结构AUDIOIN_CAPS的指针。
/// \retval 0  获取成功。
/// \retval <0  获取失败。
int AudioInGetCapsEx(int iChannel, AUDIOIN_CAPS_EX * pCaps);

/// @} end of group

#ifdef __cplusplus
}
#endif

#endif //__AUDIO_API_H__

