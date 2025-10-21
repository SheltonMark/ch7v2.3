#ifndef __TD_COMMON_H__
#define __TD_COMMON_H__

#define PRINT_MODULE_NAME "LIBMED"

#include "includes.h"
#include "types.h"
#include "common.h"

#include "sdk_comm.h"

#include "detectpriv.h"

#include "Audio.h" 
#include "AudioCodec.h"
#include "Video.h"
#include "Capture.h"
#include "ExCapture.h"

#include "LossDetect.h"

#include "MotionDetect.h"
#include "BlindDetect.h"
#include "PetDetect.h"
#include "HumanDetect.h"
#include "CryingDetect.h"
#include "Region.h"
#include "MdCdCheck.h"

#include "system_config.h"
#include "audio_config.h"

#include "vi_chn_config.h"
#include "videoinfo_config.h"

#include "MediaCom.h"
#include "ProductDefine.h"
#include "Dec.h"
#include "PrintGrade.h"

#include "Isp.h"
#include "media.h"
#include "ni_ptz_tracking.h"
#include "mdk_md.h"
#include "PrintGrade.h"

#define ENABLE     1
#define DISABLE    0
#define RETURN_OK  0

#define Ircut(x)	 SystemSetIrcut(x, 0)

#define IRCUT_ON 		2
#define IRCUT_OFF 		3
#define RED_LED			32
#define WHITE_LED 		109
#define BLUE_LED		33
#define RESET_KEY		3
#define SPEAK_MUTE		1
#define SENSOR_RESET 	38
#define LED_SELECT		34

typedef struct DeviceIspInfo
{
	ISP_SENSORTYPE_E SensorType;
	unsigned int Main_MaxWidth;
	unsigned int Main_MaxHeight;
	unsigned int Sub_MaxWidth;
	unsigned int Sub_MaxHeight;
} DeviceIspInfo;

#define LIBMED_VERSION_INFO \
do \
{ \
	PRINT_TRACE("====================================================\n"); \
	PRINT_TRACE("|libmed.a Ver: %d.%d.%d.%s Compile at %s %s\n",VERSION_MAJOR,VERSION_USERID,VERSION_GIT,GIT_SHA1, __DATE__, __TIME__); \
	PRINT_TRACE("====================================================\n"); \
}while(0);

typedef struct SystemDevice_s
{
	int hardVersion;
	int oemPatch;
	int oemtype;
	int frontboradType;
	int productType;
	int audioInChannel;
	int playbackChannel;
	int realplayChannel;
	int IsNvr;
	VENC_PORO_HEAD_E VenPoroHead;
	int ChipMaster;
	int DDRSize;
	int ZeroChannel;//由于DVR的初始化在应用层，所以设置ENABLE为 使能零通道
	int NvrMd;//NVR的模式
	int IsIVS;
	int ResmpChannel[MAX_VIDEO_IN_CHANNEL];
	AdChnFormat_e ChnFormat[MAX_VIDEO_IN_CHANNEL];
//	DWORD IsNr;
	int DenoiseLevel[MAX_VIDEO_IN_CHANNEL];
	int DeviceType;
  	pthread_mutex_t		lock4bmp;
}SystemDevice_t, *SystemDevice_p;

typedef struct GlobalDevice_s
{
	SystemDevice_t SystemDevice;
	VideoInDevice_t VideoInDevice;
	// VpssGrpConf_t    VPSSGrpConf;
	// VideoOutDevice_t VideoOutDevice;
	CaptureDevice_t CaptureDevice;
	RegionDevice_t RegionDevice;
	MotionDetectDevice_t MotionDetectDevice;
	HumanDetectDevice_t HumanDetectDevice;
	BlindDetectDevice_t BlindDetectDevice;
	PerimeterDetectDevice_t PerimeterDetectDevice;
	LostDetectDevice_t LostDetectDevice;
	//VIDEO_CHN_COVER_PARAM_t   VideochnCoverParam;
	//VB_CONF_S  stVbConf;
	//VbConf_t VbConfig;
	AiDevice_t AiDevice;
	AoDevice_t AoDevice;
		A_PACKAGE_t APackage;
		MD_CD_Check_t md_cd_check;
	SensorDevice_t SensorDevice;
	pthread_mutex_t  GlobalDevice_Mutex;
#ifdef DEBUG_AVBR
	AVBR_Attr avbr_attr;
#endif
}GlobalDevice_t,*GlobalDevice_p;

typedef struct RING_BUFF
{
    unsigned char *buf;
    unsigned int head;
    unsigned int tail;
    unsigned int tatol_len;
} RING_BUFF;

extern GlobalDevice_t GlobalDevice;

#ifdef DEBUG
#define MY_PRINT	\
	do			\
		{		\
			PRINT_TRACE("__FUNCTION__ = %s, __LINE__ = %d\n", __FUNCTION__, __LINE__);	\
			getchar();												\
		}while(0);
#else
#define MY_PRINT
#endif

#define LIB_MALLOC(p, type, n)					\
	do											\
	{											\
		(p) = (type *)malloc(sizeof(type) *( n));		\
		if (NULL == (p))							\
		{										\
			PRINT_ERROR("申请内存失败\n");		\
			return -1;						\
		}										\
		memset(p, 0, sizeof(type) *(n));				\
	}while(0)									

#define LIB_FREE(p)								\
	do											\
	{											\
		if (p)									\
		{										\
			free(p);								\
			p = NULL;							\
		}										\
	}while(0)

#define VENC_USERVB_CNT(n,vedu,flag)       (flag?((n*2) + 2*(vedu)) : (n + 2*(vedu)) )

void MediaSensorInit();
void MediaSetSystemInfor(int Mode, int Type, DeviceIspInfo info);
void MediaHardWareInit(int Normal);

/* New adapter-based video pipeline */
int VideoPipeline_Init(void);
int VideoOverlay_Init(void);
int VideoOverlay_UpdateTimestamp(int VpsGrp, int VpsChn, const char* timestamp);
int VideoOverlay_UpdateCustomText(int VpsGrp, int VpsChn, const char* text, int x, int y);

int write_ring_buff(RING_BUFF *ring_buf, unsigned char *input_data, unsigned int len);
int read_ring_buff(RING_BUFF *ring_buf, unsigned char *output_data, unsigned int len);

#endif


