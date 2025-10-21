
/*

修改记录：
1.添加接口VDECGetShowPTS，获取当前显示的时间戳

调用流程：
	
	VDECCreate
		|
	VDECStart
		|
	VDECSetFormat
		|
	VDECSendFrame
		|
	VDECStop
		|
	VDECDestory

*/


#ifndef __DEC_API_H__
#define __DEC_API_H__

#ifdef __cplusplus
extern "C" {
#endif


/////////////////////////////////////////////////////////////////////
//结构定义

/*
错误码定义
*/
enum
{
	VDEC_RESULT_SUCCESS = 0,
	VDEC_RESULT_ERROR = -1,
	VDEC_RESULT_LACK_CAPACITY = -2,			//能力不足
	VDEC_RESULT_NO_BUFFER = -3,				//缓冲满
	VDEC_RESULT_INVALID_PARAM = -4,			//参数不对
	VDEC_RESULT_ENCODE_NOSUPPORT = -5,		//编码格式不支持
	VDEC_RESULT_NO_MEM = -6,
};

/*
拥有解码通道的拥有者类型
*/
enum
{
	DEC_OWNER_TYPE_PLAY = 0,		//回放
	DEC_OWNER_TYPE_MIXED,			//网络接入模块
	DEC_OWNER_TYPE_QUICKPLAY,			//即时回放模块

};

/*
视频编码类型
*/
enum
{
	VDEC_ENCODE_TYPE_H264 = 0,
	VDEC_ENCODE_TYPE_JPEG,
};

/*
视频解码格式
*/
typedef struct VDEC_VIDEO_FORMAT
{
	int encode;
	int width;
	int height;
	int max_fps;					//最大的帧率，用来计算用掉多少解码能力。不用设到真正的解码器上，真正设置帧率用VDECSetFPS
}VDEC_VIDEO_FORMAT;

typedef struct VDEC_CAP
{
	int encode_mask;
	int vdec_ability;				//最大解码能力，长×宽×帧率×个数/1000
}VDEC_CAP;

/*
矩形区域
*/
typedef struct VDEC_RECT
{
	int left;
	int top;
	int width;
	int height;
}VDEC_RECT;

/*
音频解码格式
*/
enum
{
	ADEC_BK_PCM8 =				7,
	ADEC_BK_G729 = 				8,
	ADEC_BK_IMA =				9,
	ADEC_BK_PCM_MULAW =			(10),	//BK_G711u
	ADEC_BK_G721 =				(11),
	ADEC_BK_PCM8_VWIS =			(12),
	ADEC_BK_MS_ADPCM =			(13),
	ADEC_BK_G711a =				(14),
	ADEC_BK_AMR_NB =			(15),
	ADEC_BK_PCM16 =				(16),
	ADEC_BK_AAC	=				(17),
};

typedef struct ADEC_FORMAT
{
	int		BitRate;		/**< 码流大小，kbps为单位，比如192kbps，128kbps。*/
	int		SampleRate;		/**< 采样率，Hz为单位，比如44100Hz。*/
	int		SampleBit;		/*!< 采样的位深 */
	int		EncodeType;		/*!< 编码方式，参照audio_encode_type定义 */
}ADEC_FORMAT;

typedef struct ADEC_CAP
{
	int encode_mask;		//支持协议的类型
}ADEC_CAP;

typedef struct Pic_Info_t
{		
	void* hand;
	int   width;
	int  height;
	
}Pic_Info;
typedef struct Resize_Info_t
{
	 int   type;
	 int s32Xpos;//人脸在大图中的坐标和大小，进行抠图缩放
	 int s32Ypos;
	 int Width;
	 int Height;
	 int u32Width;
	 int u32Height;
} Resize_Info;
////////////////////////////////////////////////////////////////////////////////////
//视频解码能力

typedef struct JPEG_Info_t
{
        void* hand;
        int   width;
        int  height;

}JPEG_Info;

typedef struct Resize_SIZE_t
{
        int type;
        int s32Xpos;//
        int s32Ypos;
        int Width;
        int Height;

    int u32Width;
    int u32Height;
} Resize_SIZE;

/*
支持的能力
*/
int  VDECGetCap(VDEC_CAP* cap);

/*
创建解码通道。
OwnerType：拥有者类型，预览解码或回放解码
返回值：成功 ret>=0,为解码通道号；失败 ret=VDEC_RESULT_ERROR
*/
int  VDECCreate(int OwnerType);

/*
销毁解码通道。
vdchn：解码通道号。调用VDECCreate返回的值。
*/
int  VDECDestory(int vdchn);

/*
开始/结束解码，结束时销毁解码资源。
*/
int  VDECStart(int vdchn);
int  VDECStop(int vdchn);

/*
设置解码格式分辨率
VDEC_VIDEO_FORMAT：解码格式
返回值：ret=VDEC_RESULT_SUCCESS 成功；ret=VDEC_RESULT_LACK_CAPACITY 解码能力不足；other 其他错误
*/
int  VDECSetFormat(int vdchn, VDEC_VIDEO_FORMAT* format); 

/*
往解码库送数据
time_stamp：时间戳，微秒为单位。控制播放速度，=0表示播放速度由帧率控制
返回值：ret=VDEC_RESULT_SUCCESS 成功；ret=VDEC_RESULT_NO_BUFFER缓冲满；other 其他错误
*/
int  VDECSendFrame(int vdchn, void* data, int data_len, long long time_stamp); 

/*
清空缓存
clear_vo：是不是清空当前正在显示的画面
*/
int  VDECReset(int vdchn, int clear_vo);

/*
暂停与继续，单帧播放
*/
int  VDECPause(int vdchn, int pause);
int  VDECStep(int vdchn);

/*
设置帧率
*/
int  VDECSetFPS(int vdchn, int fps);

/*
获取缓冲区里未解码的帧数，和解码后未显示的帧数
left_frames：返回未解码帧数
left_pics：返回解码未显示帧数
*/
int  VDECGetLeftFrames(int vdchn, int* left_frames, int* left_pics);
int  VDECGetShowPTS(int vdchn, long long* time_stamp);


/*
设置显示区域
src：缩放区域，全0表示不缩放。相对与dest的坐标。
dest：移动后的位置。相对于图形层。
*/
int  VDECSetRect(int vdchn, VDEC_RECT* src, VDEC_RECT* dest);
int  VDECGetFullRect(VDEC_RECT* rc);

/*
显示还是隐藏。3531通道隐藏播放就停止的问题，这个问题媒体库解决。
*/
void VDECShow(int vdchn, int show);


/////////////////////////////////////////////////////////////////////
//音频解码通道

/*
支持的能力
*/
int  ADECGetCap(ADEC_CAP* cap);

/*
创建解码通道。
OwnerType：拥有者类型，预览解码或回放解码
返回值：成功 ret>=0,为解码通道号；失败 ret=VDEC_RESULT_ERROR
*/
int ADECCreate(int OwnerType);

/*
销毁解码通道。
vdchn：解码通道号。调用VDECCreate返回的值。
*/
int ADECDestroy(int adchn);

/*
开始/结束解码，结束时销毁解码资源。
*/
int ADECStart(int adchn);
int ADECStop(int adchn);

/*
清空缓存
*/
int ADECReset(int adchn);

/*
暂停与继续
*/
int ADECPause(int adchn, int pause);

/*
设置编码格式
*/
int ADECSetFormat(int adchn, ADEC_FORMAT* format);

/*
送数据
参数：hisi 4个字节的头由底层加
*/
int ADECSendFrame(int adchn, void* data, int data_len);

/*
添加（不是设置，就是可以添加多个回调）音频切换的回调
ADECSwitchCallBackFunc：回调函数

回调函数的参数
OwnerType：VDEC_OWNER_TYPE_PLAY或VDEC_OWNER_TYPE_MIXED或-1表示其他音频
channel：对应不同类型的通道号
*/
typedef void (*ADECSwitchCallBackFunc)(int OwnerType, int channel);
int ADECSetSwitchCallBack(ADECSwitchCallBackFunc func);

/*************************************************
data 源数据
data_len 长度
format 源数据格式
ImageSize 目的图片大小
hand   目的图片句柄 (需要在应用释放packet包)
***************************************************/
//int JPEGResize(void* data, int data_len, VDEC_VIDEO_FORMAT* format,Resize_SIZE ImageSize  ,JPEG_Info* Jpeg_Info);

int JPEGResizeToRGB(void* data, int data_len, VDEC_VIDEO_FORMAT* format,Resize_SIZE jpeg_size  ,JPEG_Info* Jpeg_Info);



#ifdef __cplusplus
}
#endif

#endif //__DEC_API_H__