//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang Dahua Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:	
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#ifndef __SYSTEMM_API_H__
#define __SYSTEMM_API_H__

//#include "TdCommon.h"
#include "videoinfo_config.h"


#ifdef __cplusplus
extern "C" {
#endif


/// 系统时间结构
typedef struct SYSTEM_TIME{
    int  year;///< 年。   
	int  month;///< 月，January = 1, February = 2, and so on.   
	int  day;///< 日。   
	int  wday;///< 星期，Sunday = 0, Monday = 1, and so on   
	int  hour;///< 时。   
	int  minute;///< 分。   
	int  second;///< 秒。   
	int  isdst;///< 夏令时标识。   
}SYSTEM_TIME;

/// 系统特性结构
typedef struct SYSTEM_CAPS
{
	/// 总共内存大小，以Kilo-Bytes为单位。
	DWORD MemoryTotal;

	/// 剩余内存大小，以Kilo-Bytes为单位。将关系到应用程序的内存使用策略。
	DWORD MemoryLeft;
}SYSTEM_CAPS;

/// 衍生版本性能
/*
typedef struct SYSTEM_CAPS_EX
{
	DWORD ForNRT;			///< 是否为非实时
	BYTE ForATM;			///< 是否为ATM机
	BYTE HasAudioBoard;		///< 带音频板
	BYTE HasLoopBoard;		///< 带环通板
	BYTE HasMatrixBoard;	///< 带矩阵板
}SYSTEM_CAPS_EX;
*/
 #if 0
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
}DvrCommand_e;
 #endif
typedef enum ProductType_e
{
	ID_INVALID = -1,
	HI3515 = 0,
	HI3520 = 1,
	//HI3531 = 2,

	DVR_6004 = 2,
	DVR_6008 = 3,
	DVR_6016 = 4,	 
	DVR_7004 = 5,
	DVR_7008 = 6,
	DVR_7016 = 7,
	DVR_7016E = 8,
	DVR_8008 = 9,
	DVR_8016 = 10,
	DVR_7016H = 11,
	DVR_7032 = 12,
	DVR_9008 = 13,
	DVR_9016 = 14,
	DVR_9016N = 15,
	DVR_7008E = 16,
	DVR_8016E = 17,
	DVR_7024 = 18,
	DVR_7016N = 19,
	DVR_7008N = 20,
	DVR_7004N = 21,
	DVR_D7004E = 27,
	DVR_D7008E = 28,
	DVR_D7016E = 29,
	DVR_D7004 = 30,
	DVR_D7008 = 31,
	DVR_D7016 = 32,
	DVR_D8004 = 35,
	DVR_D8008 = 36,
	DVR_D8016 = 37,
	DVR_D8308 = 40,
	DVR_D8316 = 41,
	DVR_D8324 = 42,
	DVR_D8332 = 43,
	DVR_D8408 = 45,
	DVR_D8416 = 46,
	DVR_D8424 = 47,
	DVR_D8816 = 48,
	DVR_7116E = 50,
	DVR_9104  = 51,
	DVR_7116  = 52,
	DVR_7108E = 53,
	DVR_6116  = 54,
	DVR_7116EW = 55,
	DVR_6104  = 56,
	DVR_6108  = 57,
	DVR_7116L = 58,
	DVR_7108L = 59,
	DVR_7104E = 60,
	DVR_8308 = 61,
	DVR_8316 = 62,
	DVR_8324 = 63,
	DVR_8332 = 64,
	DVR_9016E = 65,
	DVR_9016EW = 66,
	DVR_6104L = 67,
	DVR_6108L = 68,
	DVR_9008E = 69,
	DVR_6104S = 70,
	DVR_6108S = 71,
	DVR_6104_20D_V1 = 72,
	DVR_6104_20D_V3 = 73,
	DVR_6104L_15A_V1 = 74,
	DVR_6104L_15A_V3 = 75,
	DVR_6104S_20D_V5 = 76,
	DVR_6104S_15A_V5 = 77,
	DVR_6108_20D_V1 = 78,
	DVR_6108_20D_V3 = 79,
	DVR_6108S_20D_V5 = 80,
	DVR_6104S_20D_V6 = 81,
	DVR_6116L_20D_V1 = 82,
	DVR_6108SL_20D_V5 = 83,
	DVR_6104_15A_V7 = 84,
	DVR_6108_20D_V7 = 85,
	DVR_7104HL = 86,
	DVR_7104H = 87,
	DVR_7108HL = 88,
	DVR_6104_20D_V1_RC = 89,
	DVR_6108_20D_V1_RC = 90,
	DVR_6104_15A_V1_RC = 91,
	DVR_6104F=92,
	DVR_6104_20D_V1_F=93,
	DVR_6104_20D_V1_RC_F=94,
	DVR_6104S_20D_V5_F =95,
	DVR_6104_15A_V7_TW = 96,
	DVR_6108_20D_V7_TW = 97,
	DVR_7116E_F = 98,
	DVR_7108E_F =99,

	/*
	100 到 150别占用，留给G4 G5智能设备使用
	*/
	IVA_PLATE = 100,
	IVA_HDCT = 101,
	IVA_BZS = 102,
	IVA_ATM = 103,
	IVA_SSB = 104,
	IVA_FACE = 105,
	IVA_ATM_SSB = 106,
	/*
	100 到 150别占用，留给G4 G5智能设备使用
	*/
	DVR_6108_SF = 151,
	DVR_6108ES_21_V1 = 152,
	DVR_6104S_20D_V5_B =153,
	DVR_6104_20D_V1_RC_B = 154,
	DVR_6104_20D_V10_MP = 155,
	DVR_6108_20D_V11_MP = 156,
	DVR_6104S_BN_20D_V5 = 157,
	DVR_6104SF_BN_20D_V5 = 158,
	DVR_6104SR_BN_20D_V5 = 159,
	DVR_6108S_BN_20D_V5 = 160,
	DVR_6108SE_BN_21_V1 = 161,
	DVR_6108SF_BN_21_V1 = 162,


	DVR_7116EN = 201,
	NVR_832	= 202,
	NVR_208P	= 203,
	NVR_208	= 204,
	NVR_216	= 205,
	NVR_232	= 206,
	DVR_7104EN = 207,
	DVR_7108EN = 208,
	DVR_9008EN = 209,
	DVR_9016EN = 210,
	DVR_6104N	= 211,
	NVR_216P	= 212,
	NVR_204 = 213,
	NVR_3116_8P   = 214,
	DVR_8308N	 = 215,
	NVR_204P	 = 216,
	NVR_204F	 = 217,
	NVR_204FP	 = 218,
	NVR_204SF	 = 219,
	NVR_208F	 = 220,
	NVR_216F	 = 221,
	NVR_208ES	 = 222,
	NVR_2104 = 223,
	NVR_2108 = 224,
	NVR_2116 = 225,
	NVR_3108_8P = 226,
	NVR_3132_8P = 227,
	NVR_808E	 = 228,
	NVR_816E	 = 229,
	NVR_832E	 = 230,


	IPC_TW9900 = 256,
	IPC_FSAN2400 = 260,
	IPC_OV10633 = 264,
	IPC_GC0308 = 268,
	TVR6104L	 = 300,
	TVR7104L	 = 301,
	TVR7104L_IS = 302,
	TVR7108L    = 303,
	TVR7116L =304,
	TVR7108 = 305,
	TVR8308L = 306, 
	TVR8316L   = 307,
	TVR6104L_V01 = 308, 
	TVR6108L = 309,
	TVR6116L = 310, 

	TVR7104GT = 311,
	TVR6108GT = 312,
	TVR6104G = 313,
	TVR6116G = 314,
	TVR7108G = 315,
	TVR6108C = 316,
	TVR6116C = 317,
	TVR6132C = 318,
	TVR8324     = 319,
	TVR8332     = 320,	
	TVR7116G = 321,
	TVR8316 = 322,	
	
	// Wifi-NVR
	NVR_2104L_W = 329,
	NVR_2108L_W = 330,
	//3536 new add
	NVR_3204E_P4 = 331,
	NVR_2208_P8 = 332,
	ZN_IV1202N_O = 333,
	ZN_IV3004N_O = 334,
	ZN_IV3006N_O = 335,
	ZN_IV3008N_O = 336,

	TVR7104GT_1G = 340,
	TVR7108GT_2G = 341,
	NVR_N3W_4H = 700,
	NVR_N3W_8H = 701,
	NVR_N6P_4H,
	NVR_N6P_8H,
	NVR_N6P_16H,
	NVR_N3L_4H,
	NVR_N3L_8H,
	NVR_N3L_16H,
	IPC_IT7_PRS_K,
	IPC_IC7_PRS_K,
	IPC_KCT6_WCA,
	IPC_CP7,
	IPC_CH7,
	IPC_CP3,
	IPC_CH7L,
	IPC_RH7,
	IPC_RP3,
	IPC_RH7L,
	IPC_CH9V10,
	IPC_CP7V20,
	IPC_RP7V20,
	IPC_CH3V20,
	IPC_RH3V20,
	IPC_CP3PROV11,
	IPC_RP3PROV11,
	IPC_CH7V20,
	IPC_RH7V20,
	IPC_CH7WV10,
	IPC_TC3H15CV10
} ProductType_e;

/* Sensor类型 */
typedef enum
{
	SENSOR_Q03,
	SENSOR_JXF37,
	SENSOR_IMX307,
	SENSOR_SC3235,
	SENSOR_IMX335,
	SENSOR_GC4653,
	SENSOR_SC3335,
	SENSOR_SC3336,
	SENSOR_GC2083,
	SENSOR_GC4023,
	SENSOR_0S04L10,
	SENSOR_0S04D10,
	SENSOR_BUTT
} ISP_SENSORTYPE_E;

typedef enum curr_ad_type_s
{
	TECHWELL_TYPE = 1,
	NEXTCHIP_TYPE,
	CONEXANT_TYPE,
	SHUNXIN_TYPE,
	TECHPIONT_TYPE,
	
}curr_ad_type_e;//对应use_ad_program_s 的枚举


typedef enum BoardType_e
 {
	BOARD_TYPE_ZERO = 0,
	BOARD_TYPE_NVR_204S = 2,
	BOARD_TYPE_NVR_204SL = 3,
	BOARD_TYPE_NVR_204_ANSZ = 4,	//安尼数字（安维安防）的板
	BOARD_TYPE_NVR_204_SMD = 5,
	BOARD_TYPE_NVR_204SLL = 6,
 }BoardType_e;


 typedef enum
 {
	 VENC_DAHUA = 0,
	 VENC_ZL,
	 VENC_COUNT
 }VENC_PORO_HEAD_E; 


typedef enum tagSYSTEM_OEM_TYPE
{ 
	OEM_T_GENERAL = 0,//通用 
	OEM_T_KANGLE	= 1,//康乐 
	OEM_MEI_DIAN_WEI_SHI = 2,//美电贝尔 
	OEM_FU_SHI_AN,
	OEM_AN_WEI_AN_FANG,
	OEM_CHANG_CHUN_JI_DA,
	OEM_KE_KANG_DA,
	OEM_MEI_DIAN_BEI_ER,
	OEM_MING_HONG_KAI,
	OEM_XIN_KE_AN,
	OEM_BO_RUI,
	OEM_DONG_SHUN ,
	OEM_YOU_SHENG_JIA,
	OEM_SHAO_BING,
	OEM_XU_FENG_WEI_SHI,
	OEM_BAO_XIN_SHENG,
	OEM_JU_MING_SHI,
	OEM_TIAN_WEI_SHI_BAO,
	OEM_T_NR,
}SYSTEM_OEM_TYPE;


typedef struct tagSYSTEM_CAPS_EX
{
 DWORD ForNRT;   //是否为非实时, 按位表示各个通道是否设为非实时.如0xffff表示前16路都是非实时.
 BYTE ForATM;   //是否为ATM机, 1表示不带液晶ATM,2表示带液晶ATM
 BYTE HasAudioBoard;  //带音频板
 BYTE HasLoopBoard;  //带环通板
 BYTE HasMatrixBoard; //带矩阵板
 BYTE HasPtzBoard;           //带云台板
 BYTE HasWlanBoard;        //带无线板 
  //保留位的使用
 //reserved[0]  
 //   比特位0 ：有没关机功能,0 没有,1 有;
 //   比特位1：有没重启功能,0 没有,1 有;
 //   比特位2：有没注销功能,0 没有,1 有;
 //reverved[1]  暂不使用

 BYTE reserved[2];
 SYSTEM_OEM_TYPE OEMType;
}SYSTEM_CAPS_EX;

enum system_shutdown_t {
	FUNC_SHUTDOWN_T = 0,		///关机功能位
	FUNC_REBOOT_T = 1,		///重启功能位 
	FUNC_LOGOFF_T = 2,		///注销功能位
	FUNC_SHUTDOWN_NUM
};

/// 系统升级状态
enum system_upgrade_state
{
	SYSTEM_UPGRADE_OVER = 0,
	SYSTEM_UPGRADE_INIT = 1,
	SYSTEM_UPGRADE_RUNNING = 2
};
//工程配置使用结构体
typedef struct ToolData_s
{
	 BYTE DisplayTypeBuf[32]; //产品类型
	 unsigned long long SupportedLanguage;//支持语言掩码
	 int DefaultLanguage;//默认语言
	 int AudioInputNumber;//音频输入
	 int AlarmInputNumber;//报警输入
	 int AlarmOutputNumber;//报警输出
	 int PlaybackLimitNumber; //回放路数
	 int VideoStand;//pal,ntsc
	 int D1Channels;//支持d1编码数
	 int VgaResolution;//分辨率
	 int VgaHue;
	 int VgaBrightness;
	 int VgaContrast;
	 int VgaSaturation;
	 int TvTop;
	 int TvBottom;
	 int TvLeft;
	 int TvRight;
	 int TvHue;
	 int TvBrightness;
	 int TvContrast;
	 int TvSaturation;
	 int 	Hdmi;
	 int  reserved;//用于判断配置是否有效
}ToolData_t;

typedef enum SYS_PLATFORM_E {
	 UNKNOW_MASTER = 0,
	 HI3515A_MASTER = 1,
	 HI3520D_MASTER = 2,
	 HI3520A_MASTER = 3,
	 HI3521_MASTER  = 4,
	 HI3531_MASTER  = 5,
	 HI3521A_MASTER = 6,
	 HI3520DV300_MASTER = 7,
	 HI3531A_MASTER     = 8,
	 MASTER_BUT, 
}SYS_PLATFORM_E;

/// 得到当前系统时间。这个函数可能调用的很频繁, 所有调用时应该是从内存中取得
/// 时间,而不是直接从硬件, 在定时器里更新内存中的值.
/// 
/// \param [out] pTime 指向系统时间结构SYSTEM_TIME的指针。
/// \retval 0  成功
/// \retval <0  失败
int SystemGetCurrentTime(SYSTEM_TIME * pTime);


/// 设置当前系统时间。
/// 
/// \param [in] pTime 指向系统时间结构SYSTEM_TIME的指针。
/// \retval 0  成功
/// \retval <0  失败
int SystemSetCurrentTime(SYSTEM_TIME * pTime);
//int SystemGetTimeSecond(int *pTime);


/// 得到系统的毫秒计数。两次调用之间的间隔时间就是得到的计数的差值。
/// 
/// \param 无
/// \return 当前计数。
/// \note 一般情况下，49.7天后得到计数会溢出，使用到的地方要做溢出后处理！
DWORD SystemGetMSCount(void);


/// 得到系统的微秒计数, 此函数主要用于模块性能的测试。溢出的更快。
/// 
/// \param 无
/// \return 当前计数。
DWORD SystemGetUSCount(void);


/// 得到CPU占用率。
/// 
/// \param 无
/// \return CPU占用百分比。
int SystemGetCPUUsage(void);


/// 得到系统特性。
/// 
/// \param [out] pCaps 指向系统特性结构SYSTEM_CAPS的指针。
/// \retval 0  获取成功。
/// \retval <0  获取失败。
int SystemGetCaps(SYSTEM_CAPS * pCaps);



/// 
int SystemGetCapsEx(SYSTEM_CAPS_EX * pCaps);


/// 挂起当前线程，时间到后再唤醒。
/// 
/// \param [in] dwMilliSeconds 挂起的时间。
void SystemSleep(DWORD dwMilliSeconds);


/// 
void SystemUSleep(DWORD dwMicroSeconds);


/// 让系统蜂鸣器发出单音调的声音，是同步模式，时间到才返回。
/// 
/// \param [in] dwFrequence 音调频率。
/// \param [in] dwDuration 持续的时间。
void SystemBeep(DWORD dwFrequence, DWORD dwDuration );
//查看系统是不是带有beep
int GetSystemBeepState(int *State);


/// 系统复位。
void SystemReboot(void);

/// ircut日夜切换
int SystemSetIrcut(int enable,int type);

/// 闪光灯使能
int SystemSetLight(int enable);

/// GPIO初始化
void GpioInit(int PortIndex, int BitIndex);


/// 系统关闭。
void SystemShutdown(void);


/// 应用程序收到升级包后，让系统解压。是同步调用模式，直到解压完成才返回。
/// 
/// \param [in] pData 数据包在内存中的指针。
/// \param [in] dwLength 数据长度。
/// \param [out] pProgress 当前进度百分比，函数调用过程时会被定时更新，应用程序
///        不应该修改这个值。
/// \retval 0  成功
/// \retval <0  失败
/// \note 不同的系统数据打包的格式不同，对应的完成数据打包和解压的的程序也不同。
int SystemUpgrade(BYTE* pData, DWORD dwLength, DWORD *pProgress);

//网络球单片机及IPC单片机升级接口
int SystemUpgradeC8051(char *pData, unsigned int dwLength, unsigned int *pProgress);

/*Begin: Add by chenjianqun 20061024 for New Upgrade method*/
//int SystemUpgradeEx(struct iovec *iov, DWORD *pProgress);
/*End: Add by chenjianqun 20061024 for New Upgrade method*/

/// 系统日志初始化。系统日志用来记录应用程序运行时所有的标准输出。
/// 
/// \retval 0  成功
/// \retval <0  失败
int SystemLogInit(void);

void DeviceHalInit();

/// 记录系统日志， 同时将内容写到到标准输出缓冲。
/// 
/// \retval 0  成功
/// \retval <0  失败
int SystemLogWrite(char * format, ...);


/// 获取系统序列号
int SystemGetSerialNumber(char *pData, int len);


/// 获取系统硬件版本
DWORD SystemGetHWVersion(void);
int SystemGetCodingCapacity(int* type);

/// /* 查找sensor型号*/
///
/// \param [out]  ps32SensorType  sensor的类型
/// \retval  0 成功
/// \retval <0  失败
int SystemGetSensorType(ISP_SENSORTYPE_E *ps32SensorType);

/* sensor设置镜像和翻转*/
/// \param [in] mirror  镜像
/// \param [in] flip	翻转
/// \retval  0 成功
/// \retval <0  失败
int SystemSetSetMirrorAndflip(int mirror, int flip);

/// 获取设备名称设备类型是一个字符串，长度小于8
///字符串根据设备不同分别为："IPCF4x5"，"IPCA6"，"IPCA8"，"IPCA7"，"IPVM"，"IPSD"
int SystemGetDeviceType(char* type,int size); 
/// 返回设备类型名
/// \param out name 获取的设备名
/// \retval 0 成功
/// \retval -1  出错
BOOL SystemGetProductName(char* name,int len);
/// 返回设备语言类型
/// \retval >=0 语言枚举值
/// \retval -1  出错
int SystemGetProductLanguageType();
/// 返回设备是否支持看门狗
/// \param out enable
/// \retval TRUE 支持
/// \retval FALSE 不支持
BOOL SystemGetProductWDEnable();
/// 返回设备客户标识
BOOL SystemGetProductOem(char* oem,int len);
/// 返回设备硬件版本号
BOOL SystemGetProductHWVersion(char* hwVersion,int len);
/// 返回设备软件版本号
BOOL SystemGetProductHWVersion(char* hwVersion,int len);
/// 返回设备软件加密串号
BOOL SystemGetProductEncryptedSn(char* encryptedSn,int len);
/// 返回设备SensorType类型
/// \param out enable
/// \retval TRUE 支持
/// \retval FALSE 不支持
BOOL SystemGetProductSensorType(char* sensorType,int len);
//返回设备类型7004 7008
int SystemGetProductType(void);

VENC_PORO_HEAD_E GetVencPoroType(void);

//视频遮挡返回0 表示成功index = 1 为需要遮挡 index = 0 为不需要遮挡
int Video_Shelter(int *index);

//工程配置函数
//返回值 
//错误-1
//正确 0
int DvrTooInformation(ToolData_t *ToolData);


INT SystemGetVideoInfo(VideoInfoConfig_p pVideoInfo,VideoInfoValue_p pVideoValue );


//得到flash的分区信息
//int FlashGetInformation(FlashDevice_t *FlashInformation);
int GetVideoPlayBack (int *PlayBackNum);
/// @} end of group

int NetGetMAC(const char *pEthName, char *pEthMac, const int iEthMacLen);


//获取设备型号
INT SystemGetTypeName(char *TypeName,int *len);

// 获取产品平台
int SystemGetPlatform(void);

int SlaveReboot();
int SlaveSysSetTime(SYSTEM_TIME time);
int SlaveShutDown();
int SlaveWDGCreate();
int SlaveWDGDestroy();
int SlaveWDGClear();

void SystemSetHubToUsb(int num);

#ifdef __cplusplus
}
#endif

#endif //__SYSTEMM_API_H__

