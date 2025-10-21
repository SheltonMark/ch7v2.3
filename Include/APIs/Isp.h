#ifndef __IMAGE_API_H__
#define __IMAGE_API_H__

#ifdef __cplusplus
extern "C"{
#endif

typedef enum AE_MODE_e
{
	AE_MODE_AUTO,
	AE_MODE_TIME,
	AE_MDOE_GAIN,
	AE_MODE_MANUAL,
	AE_MODE_BUTT_E
}AE_MODE;

typedef enum AE_TIME_e
{
    AE_TIME_1_3,
	AE_TIME_1_4,
	AE_TIME_1_5,
    AE_TIME_1_6,
	AE_TIME_1_8,
	AE_TIME_1_10,
    AE_TIME_1_12,
	AE_TIME_1_15,
    AE_TIME_1_25,
	AE_TIME_1_30,
    AE_TIME_1_50,
	AE_TIME_1_60,
    AE_TIME_1_100,
	AE_TIME_1_120,
    AE_TIME_1_150,
    AE_TIME_1_200,
    AE_TIME_1_250,
    AE_TIME_1_500,
    AE_TIME_1_750,
    AE_TIME_1_1000,
    AE_TIME_1_2000,
    AE_TIME_1_4000,
    AE_TIME_1_10000,
    AE_TIME_1_100000,
    AE_TIME_BUTT
}AE_TIME_e;

// 曝光时间的单位是微秒(us)
typedef enum AE_TIMES
{
	AE_TIMES_1_3 = 333333,
	AE_TIMES_1_4 = 250000,
	AE_TIMES_1_5 = 200000,
	AE_TIMES_1_6 = 166666,
	AE_TIMES_1_8 = 125000,
	AE_TIMES_1_10 = 100000,
	AE_TIMES_1_12 = 83333,
	AE_TIMES_1_15 = 66666,
	AE_TIMES_1_25 = 40000,
	AE_TIMES_1_30 = 33333,
	AE_TIMES_1_50 = 20000,
	AE_TIMES_1_60 = 16666,
	AE_TIMES_1_100 = 10000,
	AE_TIMES_1_120 = 8333,
	AE_TIMES_1_150 = 6666,
	AE_TIMES_1_200 = 5000,
	AE_TIMES_1_250 = 4000,
	AE_TIMES_1_500 = 2000,
	AE_TIMES_1_750 = 1333,
	AE_TIMES_1_1000 = 1000,
	AE_TIMES_1_2000 = 500,
	AE_TIMES_1_4000 = 250,
	AE_TIMES_1_10000 = 100,
	AE_TIMES_1_100000 = 10,
} AE_TIMES;

typedef enum LIGHT_MODE_e
{
	LIGHT_MODE_SHUT,
	LIGHT_MODE_BACK,
	LIGHT_MODE_FRONT,
	LIGHT_MODE_BUTT
}LIGHT_MODE_e;

typedef enum FLASHING_MODE_e
{
	FLASHING_MODE_SHUT,
	FLASHING_MODE_FLASH,
	FLASHING_MODE_LIGHT,
	FLASHING_MODE_BUTT
}FLASHING_MODE_e;

typedef enum IRCUT_MODE_e
{
	IRCUT_MODE_DAY,
	IRCUT_MODE_NIGHT,
	IRCUT_MODE_AUTO,
	IRCUT_MODE_BUTT
}IRCUT_MODE_e;

typedef enum AWB_MODE_e
{
	AWB_MODE_AUTO,
	AWB_MODE_LOCK,
	AWB_MODE_WHITE,
	AWB_MODE_WARM,
	AWB_MODE_NATURAL,
	AWB_MODE_SUN,
	AWB_MODE_MANUAL,
	AWB_MODE_BUTT
}AWB_MODE_e;

typedef enum NR_MODE_e
{
	NR_MODE_SHUT,
	NR_MODE_2D,
	NR_MODE_3D,
	NR_MODE_BUTT
}NR_MODE_e;

typedef enum DAY_STATUS_e
{
	DAY_STATUS_DAY,
	DAY_STATUS_NIGHT,
	DAY_STATUS_BUTT
}DAY_STATUS_e;

typedef enum FLASH_HZ_e
{
	FLASH_HZ_50,
	FLASH_HZ_60,
	FLASH_HZ_BUTT,
}FLASH_HZ_e;
	
typedef enum LightCode
{
    faceboardInfraredLight,		///< 红外灯  
    faceBoardFlashLight, 		///< 补光灯（白光）
    faceBoardRedLight,			///< LED灯（红光）
    faceBoardBlueLight,			///< LED灯（蓝光）
	faceBoardWhiteLight,		///< LED灯（白光）
	faceBoardLightNUM
}LightCode;	
	
/// 指示灯参数
typedef struct LightState
{
	LightCode code; 		///< 指示灯编号，参见LightCode
	int state;				///< 输出状态，0-关闭，1-开启
	unsigned int strength;	///< 光亮强度(0-100)
	short onCycle;			///< 指示灯闪烁的点亮时间，单位为毫秒，onCycle和offCycle任何一个为0表示不闪烁
	short offCycle; 		///< 指示灯闪烁的熄灭时间，单位为毫秒
	COLORREF color; 		///< 指示灯颜色
}LightState;

typedef struct
{
	unsigned int InfraredLight;		/* 红外灯亮度 */
	unsigned int FlashLight;		/* 白光灯亮度 */
}LIGHT_STRENGTH;

typedef struct
{
	LIGHT_STRENGTH light_strength;
	unsigned int FlashLight_state;	
	unsigned int InfraredLight_state;
}LIGHT_STATE;

/* 日夜模式类型 */
typedef enum
{
	ISP_VISION_DAY 			= 0,	/* 白天模式 */
	ISP_VISION_INFRARED		= 1,	/* 黑白夜视模式 */
	ISP_VISION_COLOR		= 2,	/* 暖光夜视模式 */
	ISP_VISION_MAX,
}ISP_DAYNIGHT_VISION_E;

/*日夜模式*/
typedef struct
{
	unsigned int jtime;
	LIGHT_STATE light_state;
	ISP_DAYNIGHT_VISION_E daynight_vision;
}ISP_DAYNIGHT_VISION,*ISP_DAYNIGHT_VISION_P;

typedef enum AntiFlick_Mode
{
	AntiFlick_OFF,		//关闭防闪烁
	AntiFlick_50,		//50HZ防闪烁
	AntiFlick_60,		//60HZ防闪烁
	AntiFlick_BUTT
}AntiFlick_Mode;

typedef struct WbGain
{
	unsigned int Rgain;
	unsigned int Bgain;
} WB_GAIN_S;

/// ISP支持设置的特性结构
typedef struct ISP_IMAGE_CAPS
{
	unsigned int Nr3dEnabled;			///< 置1表示支持设置3D降噪，置0表示不支持设置3D降噪。
} ISP_IMAGE_CAPS;

int gpio_init(unsigned int gpio);

int gpio_export(unsigned int gpio);

int gpio_set_direction(unsigned int gpio, const char *direction);

int gpio_set_value(unsigned int gpio, unsigned int value);

/**
 *  @brief      初始化ISP
 *  @brief      提供给XS内部使用
 * @param[in]   viInfo    VI配置
 * @return      0 或 -1
 */
int isp_init(SAMPLE_VI_INFO_S viInfo);

//关闭日夜检测线程
//升级时使用
int image_smart_deinit();

//富瀚调试工具
int ImageDebugControl(BOOL state);

//设置亮度
//value 0-100
int ImageSetBrightness(int channel, unsigned int level);

//设置对比度
//value 0-100
int ImageSetContrast(int channel, unsigned int level);

//饱和度
//value 0-100
int ImageSetSaturation(int channel, unsigned int level);

//锐度
//value 0-100
int ImageSetSharpeness(int channel, unsigned int level);

/**
 *  @brief      设置自动曝光模式
 *
 * @param[in]   mode               设置模式:
 *                                 mode 0:自动, intt和gain的最大值由intt_us和gain_level决定
 *                                 mode 1:曝光时间手动, 手动曝光由intt_us决定,gain最大值由gain_level决定
 *                                 mode 2:曝光增益手动, 手动增益由gain_level决定,intt最大值由intt_us决定
 *                                 mode 3:均手动, 手动曝光由intt_us决定,手动增益由gain_level决定
 * @param[in]   ae_time_us         可以选择FHADV_ISP_AE_TIME_LEVEL枚举类型中的一个, 也可以直接输入参数, 单位是微秒(us)
 * @param[in]   gain_level         增益等级，0 ~ 100, 0代表增益1倍, 100代表增益最大值
 * @return      0
 */
int ImageSetAeMode(int channel, AE_MODE mode, AE_TIME_e intt_us,unsigned int gain_level);

/**
 *  @brief      设置自动降帧
 *
 * 目前设置为30DB自动降帧
 * @param[in]   enable            开关
 * @return      0 或 -1
 */
int ImageSetAutoFps(int channel, int enable);

/**
 * @brief 		获取曝光速度列表
 * 
 * @param[in] channel 			通道号
 * @param[in] antiFlickMode 	防闪烁模式，对应AntiFlick_Mode类型的值。
 * @param[out] speeds 			速度数组，数组长度必须足以放下所有等级的速度值，
 * 								从0递增以曝光等级作为下标，表示对应等级的具体曝光速度。
 * 								正数表示曝光时间，微秒为单位。
 *								比如20000表示曝光时间为20us，0表示自动曝光，在数组中下标一定为0(自动曝光等级)。
 * @param[out] num 				列表长度
 * @return int 
 */
int ImageGetExposureSpeeds(int channel, int antiFlickMode, int* speeds, int* num);

/**
 *  @brief      设置宽动态参数
 *
 * @param[in]   mode    0:关闭, 1:开启
 * @param[in]   level   0~100 值越大，动态范围越大, 必须sensor库实现get_user_ltm_curve函数
 * @return      0 或 -1
 */
int ImageSetLtmCfg(int channel, unsigned int mode, unsigned int level);

/**
 *  @brief      设置前光或背光补偿
 *
 * @param[in]   mode    0:关闭, 1:开启背光补偿, 2:开启前光补偿
 * @param[in]   level   0~100 值越大，动态范围越大
 * @param[in]   left    背光补偿区域left值
 * @param[in]   top     背光补偿区域top值
 * @param[in]   right   背光补偿区域right值
 * @param[in]   bottom  背光补偿区域bottom值
 * @return      0 或 -1
 */
int ImageSetLightCompensation(int channel, LIGHT_MODE_e mode, unsigned int level, unsigned int left, unsigned int top, unsigned int right, unsigned int bottom);


/**
 *  @brief      设置防闪烁或防补光过曝
 *
 * @param[in]   mode    0:关闭, 1:开启防闪烁, 2:防补光过曝
 * @param[in]   gainDay2Night   频率值
 * @return      0 或 -1
 */
int ImageSetFlashing(int channel, FLASHING_MODE_e mode, FLASH_HZ_e frequency);

/**
 *  @brief      设置日夜切换
 *
 * @param[in]   mode    0:白天, 1:晚上, 2:自动
 * @param[in]   gainDay2Night   白天到晚上切换，光亮度增益
 * @param[in]   gainNight2Day   晚上到晚上切换，光亮度增益
 * @return      0 或 -1
 */
int ImageSetIrcut(int channel, IRCUT_MODE_e mode, unsigned int gainDay2Night, unsigned int gainNight2Day);

/**
 *  @brief      设置IRCUT
 *  @brief      提供给XS内部使用
 * @param[in]   IspDev    ISP设备号
 * @param[in]   state   	白天为1；夜晚为0
 * @return      0 或 -1
 */
int ircut(ISP_DEV IspDev, unsigned int state);

/**
 *  @brief      设置红外灯
 *  @brief      提供给XS内部使用
 * @param[in]   IspDev    ISP设备号
 * @param[in]   pstIrLight   白天为0；夜晚为8000；
 * 				数组所有参数均一起发生变化
 * @return      0 或 -1
 */
int ir_light(ISP_DEV IspDev, AISP_LIGHT_RESULT_S *pstIrLight);

/**
 *  @brief      设置夜视模式
 *
 * @param[in]   enVision    对应的夜视模式
 * @return      0
 */
int ImageSetNightVision(ISP_DAYNIGHT_VISION_E enVision);

/**
 *  @brief      一键遮蔽红外灯设置
 * @param[in]   state    是否开启一键遮蔽
 * @return      0
 */
int ImageSetHideState(BOOL state);

/**
 *  @brief      设置补光灯亮度
 * 只能设置白光灯和红外灯亮度
 * @param[in]   lightcode  灯号
 * @param[in]   strength   对应灯号的亮度
 * @return      0 或 -1
 */
int ImageSetLightStrength(LightCode lightcode, unsigned int strength);

/**
 *  @brief      获取日夜状态
 *
 * @param[out]   status    0:白天, 1:晚上
 * @return      0 或 -1
 */
int ImageGetDayStatus(DAY_STATUS_e* status);

/**
 *  @brief      设置日夜切换抖动
 *
 * @param[in]   offset   		第一次震荡饱和度阈值增加offset
 * @param[in]   jitter_time   	抖动帧，第一次震荡经过jitter_time帧后恢复正常
 * @return      0 或 -1
 */
int ImageSetJitter(int channel, unsigned int jitter_time);

/**
 *  @brief      设置自动白平衡增益
 *
 * @param[in]   mode    设置模式：
 *                      mode 0:自动白平衡
 *						mode 1:锁定白平衡
 *                      mode 2:白炽灯
 *                      mode 3:暖光灯
 *                      mode 4:自然光
 *                      mode 5:日光灯
 *						mode 6:手动白平衡
 * @param[in]	r		0-100 红色增益
 * @param[in]	g		0-100 绿色增益
 * @param[in]	b 		0-100 蓝色增益
 * @return      0 或 -1
 */
int ImageSetAwbGain(int channel, AWB_MODE_e mode, unsigned int r, unsigned int g, unsigned int b);

/**
 *  @brief      设置2d降噪或3d降噪
 *
 * @param[in]   mode    0:关闭, 1:普通2d, 2:专家3d
 * @param[in]   air_level	0-100 空域降噪等级
 * @param[in]   time_level	0-100 时域降噪等级
 * @param[in]	mtd_level	0-100 运动等级（去拖影）
 * @return      0 或 -1
 */
int ImageSetNr2Or3(int channel, NR_MODE_e mode, unsigned int air_level, unsigned int time_level, unsigned int mtd_level);

/**
 *  @brief      设置去紫边使能
 *
 * @param[in]   mode    0:关闭, 1:打开
 * @param[in]	level	0-100
 * @return      0 或 -1
 */
int ImageSetAntiPurpleBoundary(int channel, unsigned int mode, unsigned int level);

/**
 *  @brief      设置坏点矫正
 *
 * @param[in]   mode    0:关闭, 1:打开
 * @param[in]	level	0-100
 * @return      0 或 -1
 */
int ImageSetDynamicDpc(int channel, unsigned int mode, unsigned int level);

/// 得到ISP支持设置的特性。
///
///
/// \retval 0  获取成功。
/// \retval <0  获取失败。
int ImageGetIspCaps(ISP_IMAGE_CAPS *pCaps);

#ifdef __cplusplus
}
#endif

#endif
