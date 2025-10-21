#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

#include "includes.h"
#include "types.h"

#include "system_config.h"
#include "fbd_config.h"
#include "alarm_config.h"
#include "ptz_config.h"
#include "serial_config.h"
#include "cmos_config.h"
#include "rtc_config.h"
#include "watchdog_config.h"
#include "audio_config.h"
#include "uartcontrol_config.h"
#include "vi_chn_config.h"
//#include "mpp_config.h"
//#include "audio_io_config.h"
//#include "vi_device_config.h"
//#include "zoom_freezen_config.h"
//#include "pic_delay_config.h"
//#include "video_io_binds_config.h"
//#include "enchip_capability_config.h"
//#include "board_config.h"
//#include "vi_convert_config.h"
//#include "capture_get_cap_config.h"
//#include "capture_get_chn_config.h"

//#define APP_CONFIG_NAME		"/opt/app/etc/appconfig.ini"
//#define APP_CONFIG_NAME		"/home/wang_lin/tool/appconfig.ini"
#define APP_CONFIG_NAME		"/opt/app/system/appconfig.ini"

typedef struct AppConfig_s
{
	SystemConfig_t System;
	FbdConfig_t Frontboard;
	AlarmConfig_t Alarm;
	PtzConfig_t Ptz;
	SerialConfig_t Serial;
	CmosConfig_t Cmos;
	RtcConfig_t Rtc;
	WatchdogConfig_t Watchdog;
	AudioConfig_t Audio;
	UartConConfig_t UartContl;
	VIBindVpssCfg_t	ViChnConfig;
/*
	Mppconfig_t Mpp;
	hi35xx_audio_cfg_t AudioConf;
	ViDevAttrPort_t Videvice;
	zoom_freezen_mode_t Zoom_Mode;
	VideoPicDelay_t Pic_Delay;
	video_bind_t Vedio_bind;
	enc_chip_cap_t En_chip;
	board_cfg_t Board_cfg;
	vi_Convert_t Vi_Convert;
	getcap_t  	Capture_caps;   
	getchncap_t Getchn_caps;
*/	
}AppConfig_t, *AppConfig_p;



#endif

