#ifndef __MEDIA_H__
#define __MEDIA_H__

#define MEDIA_AUDIO_ONE_FRAME_SIZE 1024

typedef enum Media_SizeType_e
{
	Media_HD_400W,		//2560*1440
	Media_SD_640_360,	//640*360
	Media_SizeType_BUTT,
}Media_SizeType_e;

typedef enum Media_Enc_Type_e
{
	Media_Enc_Type_H264,
	Media_Enc_Type_H265,
	Media_Enc_Type_BUTT,
}Media_Enc_Type_e;

typedef enum Media_PicSize_e
{
	Media_PicSize_400W,
	Media_PicSize_640_360,
	Media_PicSize_BUTT,
}Media_PicSize_e;

typedef enum Media_StreamType_e
{
	Media_StreamType_H265_I,		
	Media_StreamType_H265_P,
	Media_StreamType_H264_I,		
	Media_StreamType_H264_P,
	Media_StreamType_PCM,		//not support
	Media_StreamType_G711A,
	Media_StreamType_G711U,		//not support
	Media_StreamType_AAC,		//not support
	Media_StreamType_G726,
	Media_StreamType_BUTT,
}Media_StreamType_e;

typedef enum Media_Rotate_e
{
	Media_Rotate_0,
	Media_Rotate_180,
	Media_Rotate_BUTT,
}Media_Rotate_e;

typedef struct Media_CaptureAbility
{
	Media_SizeType_e HD;
	Media_SizeType_e SD;
	int	PictureFlip;		//画面翻转
	int VoiceSpeek;			//语音对讲
	int TFCard;				//TF卡支持
}Media_CaptureAbility_t,*Media_CaptureAbility_p;

typedef struct Media_StreamAttr
{
	char *data; 
	int len;
	unsigned long long pts;		//时间戳 单位[微秒]
	unsigned int bps;			//码率	单位[kbps]
	unsigned char fps;				//编码帧率
}Media_StreamAttr_t,*Media_StreamAttr_p;

typedef struct Media_VideoStream
{
	Media_StreamType_e type;
	Media_StreamAttr_t stream_attr;
}Media_VideoStream_t,*Media_VideoStream_p;

typedef struct Media_Video_Format
{
	int type;						//0:HD 1:SD
	Media_PicSize_e	picSize;
    int frame_count;
    int bps;						//码率 单位[kps]
    Media_Enc_Type_e enc_type;
	//FH_H264_PROFILE profile;
}Media_Video_Format_t,*Media_Video_Format_p;

#ifdef __cplusplus
extern "C"{
#endif

/**
 *  @brief      获取视频码流被调函数类型
 *
 * @param[in]   channel    				通道号
 * @param[in]   type    				0:HD	1:SD
 * @param[in]   stream 					音视频码流属性   	
 */
typedef int (*Media_GetStream_Fun)(int channel, int type, Media_VideoStream_t stream);	

/**
 *  @brief      获取视频码流回调
 *
 * @param[in]   Media_GetStream_Fun    用户自定义函数体
 */
void Media_GetStream_Callback(Media_GetStream_Fun fun);

/**
 *  @brief      获取音频码流被调函数类型
 *
 * @param[in]   stream 					音视频码流属性   	
 */
typedef int (*Media_GetAudioStream_Fun)(Media_VideoStream_t stream);	

/**
 *  @brief      获取音频码流回调
 *
 * @param[in]   Media_GetStream_Fun    用户自定义函数体
 */
void Media_GetAudioStream_Callback(Media_GetAudioStream_Fun fun);

/**
 *  @brief      设备初始化
 *
 * @return      0 或 -1
 */
int Media_Video_Init();

/**
 *  @brief      音频去初始化
 *
 * @return      0 或 -1
 */
void Media_Audio_UnInit();

/**
 *  @brief      获取设备能力
 *
 * @param[in]   Caps    设备能力(1:打开 0:关门)
 * @return      0 或 -1
 */
void Media_GetAbility(Media_CaptureAbility_p Caps);

/**
 *  @brief      获取jpeg图片数据
 *
 * @param[in]   data    jpeg图片数据首地址
 * @param[in]   len     jpeg图片数据长度
 * @return      0 或 -1
 */
int Media_GetJpegData(char *data, unsigned int *len);

/**
 *  @brief      设置视频旋转属性
 *
 * @param[in]   type    		0:HD	1:SD
 * @param[in]   param  			旋转角度
 * @return      0 或 -1
 */
int Media_SetRotate(int type, Media_Rotate_e param);

/**
 *  @brief      播放音频数据
 *
 * @param[in]   data    		pcm 8k 16bit音频数据首地址
 * @param[in]   len  			音频数据长度
 */
void Media_AudioSendAo(unsigned char *data, int len);

/**
 *  @brief      动检初始化
 *
 * @return      0 或 -1
 */
int Media_MotionInit();

/**
 *  @brief      动检去初始化
 *
 * @return      0 或 -1
 */
int Media_MotionUnInit();

/**
 *  @brief      设置动检灵敏度
 *
 * @param[in]   sensitiveness    	0-255  动检灵敏度
 * @return      0 或 -1
 */
int Media_MotionSetParam(int sensitiveness);

/**
 *  @brief      获取动检结果
 *
 * @param[in]   result    	0:无运动 1:检测到运动
 * @return      0 或 -1
 */
int Media_MotionGetResult(int *result);

/**
 *  @brief      设置曝光灯状态
 *
 * @param[in]   enable    	0:关闭 1:打开
 */
void Media_LightStatus(int enable);

/**
 *  @brief      申请强制I帧
 *
 * @param[in]   type    	0:HD 1:SD
 */
void Media_Video_IRequest(int type);

/**
 *  @brief      设置视频编码参数
 *
 * @param[in]   pVideo    	视频编码参数
 * @return      0 或 -1
 */
int Media_Video_SetFormat(Media_Video_Format_t pVideo);

/**
 *  @brief      获取环境状态
 *
 * @param[in]   status    	0:白天 1:黑夜
 */
void Media_GetDayStatus(int* status);

/**
 *  @brief      设置ircut使能
 *
 * @param[in]   enable    	0:关闭(白天) 1:打开(黑夜)
 */
void Media_SetIrcut(int enable);

#ifdef __cplusplus
}
#endif

#endif
