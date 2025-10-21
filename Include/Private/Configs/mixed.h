/*
NVR V3网络接入模块头文件

几个注意点：
1.channel的值从0开始
2.添加预置点和添加巡航线，添加后的ID号不一定就是参数里面传的ID号。函数返回时，会把新的ID号通过同样的字段返回。

修改记录：
1.删除MixedSetTimeSync，添加MixedConfig_GetTime、MixedConfig_SetTime，时间同步功能由应用上面做
2.能力修改。报警能力分出：视频报警与外部报警；云台能力分出：预置点和巡航

*/





#ifndef __MIXED_API_H__
#define __MIXED_API_H__

#ifdef __cplusplus
extern "C" {
#endif


#define MAX_HIDDEN_IP 2
/////////////////////////////////////////////////////////////////////

/*
其他常量 
*/
enum
{
	 MIXED_MAX_MODE = 32,       //8
	 MIXED_MAX_CAPTURE_OPTION_ITEM = 32,
	 MIXED_MOTION_WIDTH = 22,
	 MIXED_MOTION_HEIGHT = 18,
	 MIXED_WATERMARK_MAX_LEN = 128,
	 MIXED_MAX_AUDIO_OPTION_ITEM = 16,
};

enum
{
	MIXED_RESOLUTION_UNKNOW = 0,
	MIXED_RESOLUTION_QCIF,
	MIXED_RESOLUTION_CIF,
	MIXED_RESOLUTION_HD1,
	MIXED_RESOLUTION_D1,
	MIXED_RESOLUTION_720P,
	MIXED_RESOLUTION_1080P,
	MIXED_RESOLUTION_3M,
	MIXED_RESOLUTION_5M,
	MIXED_RESOLUTION_NUM,
};

typedef struct _MixedMode
{
	unsigned int local_channels[MIXED_RESOLUTION_NUM];	
	unsigned int net_channels;		
	unsigned int net_ability;			
}MixedMode;

int MixedGetSupportMode(MixedMode* mode, int* count);


#ifdef __cplusplus
}
#endif

#endif //__MIXED_API_H__



