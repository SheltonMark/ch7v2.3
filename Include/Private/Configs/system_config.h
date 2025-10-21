#ifndef __SYSTEM_CONFIG_H__
#define __SYSTEM_CONFIG_H__

#include "includes.h"
#include "types.h"
#include "gpio_config.h"
#include "uart_config.h"
//#include "system.h"

#ifdef SYSTEM_USE_FILE_NAME
#define SYSTEM_FILE_NAME	"/opt/app/etc/systemconfig.ini"
#else
#define SYSTEM_FILE_NAME	APP_CONFIG_NAME
#endif

#define MAC_ADDR_SIZE		6
#define OEM_SIZE		3
//typedef SYSTEM_OEM_TYPE OemType_e;
#define AD_CONFIG_NAME "/opt/sav/Config/adworkmode"
#define AD_CONFIG_SIZE (64)

typedef enum VencProctlType_e
{
    VENC_TYPE_DAHUA = 0,
    VENC_TYPE_ZL,
    VENC_TYPE_BUT,
}VencProctlType_e;



typedef enum HardwareVersion_e
{
	HARDWARE_VERSION_0 = 0,
	HARDWARE_VERSION_1,
	HARDWARE_VERSION_2,
	HARDWARE_VERSION_3,
	HARDWARE_VERSION_4,
	HARDWARE_VERSION_5,
	HARDWARE_VERSION_6,
	HARDWARE_VERSION_7,
	HARDWARE_VERSION_BUT,
}HardwareVersion_e;

typedef enum ChannelCount
{
	CHANNEL_UNKNOW = -1,
	CHANNEL_0 = 0,
	CHANNEL_1,
	CHANNEL_2,
	CHANNEL_3,
	CHANNEL_4,
	CHANNEL_5,
	CHANNEL_6,
	CHANNEL_7,
	CHANNEL_8,
	CHANNEL_9,
	CHANNEL_10,
	CHANNEL_11,
	CHANNEL_12,
	CHANNEL_13,
	CHANNEL_14,
	CHANNEL_15,
	CHANNEL_16,
	CHANNEL_17,
	CHANNEL_18,
	CHANNEL_19,
	CHANNEL_20,
	CHANNEL_21,
	CHANNEL_22,
	CHANNEL_23,
	CHANNEL_24,
	CHANNEL_25,
	CHANNEL_26,
	CHANNEL_27,
	CHANNEL_28,
	CHANNEL_29,
	CHANNEL_30,
	CHANNEL_31,
	CHANNEL_32,
	CHANNEL_33,
	CHANNEL_34,
	CHANNEL_35,
	CHANNEL_36,
	
}ChannelCount_e;



typedef enum DDRSize_e
{
	DDR_SIZE_UNKNOW = -1,
	DDR_SIZE_1G,
	DDR_SIZE_2G,
	DDR_SIZE_4G,
	DDR_SIZE_BUTT,
}DDRSize_e;

typedef enum EncryptVersion_e
{
	ENCRYPT_VERSION_0 = 0,
	ENCRYPT_VERSION_1,
	ENCRYPT_VERSION_2,
	ENCRYPT_VERSION_3 ,
	ENCRYPT_VERSION_BUT,
}EncryptVersion_e;
#if 0
typedef enum DvrCommand_e
{
	ENCRYPT_VERSION = 0,
	PRODUCT_TYPE,
	VIDEO_CHANNEL, 
	MATHERBOARD_VERSION,
	FRONTBOARD_VERSION,
	NVR_FUNCTION,
}DvrCommand_e;
#endif




typedef struct DvrHwid_s
{
	DWORD	Reverse					:8;
	DWORD	Reverse1				:8;
	DWORD	Reverse2				:8;
//#ifdef _IVS_
	DWORD	Reverse3				:4;
	DWORD   IVSChanNum              :4;
/*#else
	DWORD	Reverse3				:8;
#endif*/

//#ifdef _IVS_
	DWORD 	IVSType					:4;
/*#else
	DWORD	Reverse4				:4;
#endif*/
	DWORD	HiMatherBoardVer		:3;		//37~39 主板硬件版本高3位
	DWORD   ISSmart					:1;
	DWORD	IsNvr					:1;		//40
	DWORD   SataNum                 :4;		//41-44
	DWORD	FrontboardVersion		:3;		//45~47
	DWORD	MatherBoardVersion		:3;		//48~50
	DWORD	VideoChannel			:5;		//51~55
	DWORD	ProductType				:6;		//56~61
	DWORD	EncryptVersion			:2;		//62~63
}DvrHwid_t;



//风扇的配置
typedef enum FanType_e
{
	FAN_TYPE_NONE = 0,
	FAN_TYPE_GPIO,
	FAN_TYPE_MCU_UART,
	FAN_TYPE_BUT,
}FanType_e;


typedef enum SysRestType_e
{
	SYS_REST_TYPE_NONE,
	SYS_REST_TYPE_MCU,			//前面板单片机关机
	SYS_REST_TYPE_GPIO,			//前面板GPIO
}SysRestType_e;

typedef struct FanConfig_s
{
	union FANC_u
	{
		GpioConfig_t Gpio;
		UartConfig_t Uart;
	}FANC_u;
	
	FanType_e Type;
}FanConfig_t, *FanConfig_p;

typedef struct SysRestConfig_s
{
	U8 Proctol[4];
	SysRestType_e Type;
}SysRestConfig_t, *SysRestConfig_p;

typedef FanConfig_t BeepConfig_t;
typedef FanConfig_p BeepConfig_p;
typedef FanType_e BeepType_e;


typedef union SerialNumber_u
{
	DvrHwid_t Hwid;
	u64_t u64Id;
}SerialNumber_u;

typedef struct EncryptConfig_s
{
	SerialNumber_u SerialNumber;
	BYTE MacAddr[MAC_ADDR_SIZE];
	BYTE MacAddr1[MAC_ADDR_SIZE];
	DWORD Oem;
}EncryptConfig_t, *EncryptConfig_p;

typedef enum CVBS_SCENE_e
{
    SD_TV = 1,    //TV 输出
    SD_ASSIT = 2, //辅助输出
}CVBS_SCENE_e;

typedef enum CVBS_MODE_e
{
    SD_WBC = 1,     // 回写方式输出
    SD_INDEPEND= 2, //独立输出
}CVBS_MODE_e;


typedef struct SdAttr_s
{
    u8 enable;
	u8 dev_id;
	CVBS_MODE_e mode;
	CVBS_SCENE_e scene;
}SdAttr_t;


typedef struct CVBSConfig_s
{
    u8 num;
    SdAttr_t SdAttr[5];
}CVBSConfig_t;


typedef enum AO_MODE_e
{
    AudioOutMode = 1,     // AudioOut模式
    MicOutMode = 2, // Mic Out模式
    MixOutMode =3,  // 混合输出模式
}AO_MODE_e;

typedef struct AoAttr_s
{
	u8 dev_id;
	AO_MODE_e mode;
}AoAttr_t;

typedef struct AOConfig_s
{
    u8 num;  //目前只有204c为0
    AoAttr_t AoAttr[5];
}AOConfig_t;

CONST CVBSConfig_t  *GetCVBSConfig_Flbase(VOID);
CONST AOConfig_t  *GetAOConfig_Flbase(VOID);

typedef struct DDRConfig_s
{
	u32 size;
}DDRConfig_t;
#if 0
typedef struct GpioDynamicSet
{
	int groupnum;
	int bitnum;
}GpioDynamicSet_t,*GpioDynamicSet_p;

#endif

typedef struct SystemConfig_s
{
	FanConfig_t Fan;
	BeepConfig_t Beep;
	SysRestConfig_t SysRest;
	EncryptConfig_t Encrypt;
	CVBSConfig_t CVBS;
	DDRConfig_t DDR;
	AOConfig_t  AoMode;
	//GpioDynamicSet_t  GpioDynaSet;
}SystemConfig_t, *SystemConfig_p;


CONST SystemConfig_t*GetSystemAttr_Flbase(VOID);

typedef enum DvrCommand_e
{
 VIDEO_CHANNEL = 0,
 AUDIO_CHANNEL,
 ALARM_IN, 
 ALARM_OUT,
 HDMI,
 VIDEO_CHIP,
 DSP_CHIP,
 ANALOG_AUDIO_MODE,
 AUDIO_SURPPORT,
 MATRIX_SURPPORT,
 LOOP_SURPPORT,
 ENCRYPT_VERSION,
 REAL_TIME,
 DISK_INTERFACE,
 LCD_SURPPORT,
 CDRW_SURPPORT,
 HD_VERSION,
 AUDIO_INTERFACE,
 DISK_NUM,
 NET_CARD,
 DSP_NUM,
 IS_ATM,
 DVR7004_PATCH,
 FRONT_DIGIT,
 FROMT_SHIFT,
 Extended_Video,
 PLAYBACK,
 PRODUCT_TYPE,
 FRONTBOARD_TYPE,
 PRODUCT_SUB_TYPE,
 OEM_TYPE,
 OEM_PATCH,
 GET_D1_COUNT,
 GET_VENC_PROCTL_TYPE,
 GET_AD_WORK_MODE,
 DDR_SIZE,
// #ifdef _IVS_
 IVS_Functionnew,
 IVS_Type,
 IVS_CHANNUM,
// #endif
}DvrCommand_e;


extern INT DvrGetHwidInformation(DvrCommand_e DvrCommand);


#endif

