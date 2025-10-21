#ifndef __AUDIO_CONFIG_H__
#define __AUDIO_CONFIG_H__

#include "includes.h"
#include "types.h"


#ifdef AUDIO_USE_FILE_NAME
#define AUDIO_FILE_NAME	"/opt/app/etc/audioconfig.ini"
#else
#define AUDIO_FILE_NAME	APP_CONFIG_NAME
#endif

#define MAX_AUDIO_IN_NUM	20

typedef struct MicConfig_s
{
	int DevNum;
}MicConfig_t, *MicConfig_p;

typedef struct AudioConfig_s
{
	MicConfig_t MicConfig;
	
}AudioConfig_t, *AudioConfig_p;

typedef struct AudioInConfig_s
{
	int DevCnt;//音频输入设备数量
	int ChnCnt[4];//音频输入设备通道数量
	int dev[4];
	int chn[MAX_AUDIO_IN_NUM];
	int micDev;
	int micChn;
}AudioInConfig_t,*AudioInConfig_p;

CONST AudioConfig_t  *GetAudioAttr_Flbase(VOID);
CONST AudioInConfig_t *GetAudioIn_Flbase(VOID);

#endif
