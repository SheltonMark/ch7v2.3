
#ifndef __RTC_CONFIG_H__
#define __RTC_CONFIG_H__

#include "includes.h"
#include "types.h"
#include "i2c_config.h"

#ifdef RTC_USE_FILE_NAME
#define RTC_FILE_NAME	"/opt/app/etc/rtcconfig.ini"
#else
#define RTC_FILE_NAME	APP_CONFIG_NAME
#endif

#define MAX_DEVICE_NAMELEN (12)

typedef enum Rtcype_e
{
	RTC_TYPE_UNKNOW = 0,
	RTC_TYPE_I2C,
	RTC_TYPE_BUILTIN,
	RTC_TYPE_BUT,
}RtcType_e;

typedef enum Rtc_access_type_e{
	ACCESS_TYPE_UNKNOW = 0,
	ACCESS_TYPE_ALM,
	ACCESS_TYPE_TIME,
	ACCESS_TYPE_REG,
	ACCESS_TYPE_BUTT,
}Rtc_access_type_e;

typedef struct InnerRtcConfig_s
{
	INT Fd;
	INT Type;
	CHAR Name[MAX_DEVICE_NAMELEN];
}InnerRtcConfig_t, *InnerRtcConfig_p;

typedef struct RtcConfig_s
{
	RtcType_e Type;
	I2cConfig_t I2c;
	InnerRtcConfig_t Inner;
}RtcConfig_t, *RtcConfig_p;


//modify by daijm 2013-03-29
#define RTC_DEVICE_NAME "/dev/hi_rtc"
typedef struct {
        unsigned int  year;
        unsigned int  month;
        unsigned int  date;
        unsigned int  hour;
        unsigned int  minute;
        unsigned int  second;
        unsigned int  weekday;
} rtc_time_t;

typedef	struct {
	unsigned char reg;
	unsigned char val;
} reg_data_t;

typedef enum temp_sel_mode {
	TEMP_SEL_FIXMODE  = 0,
	TEMP_SEL_OUTSIDE,
	TEMP_SEL_INTERNAL,
} temp_sel_mode;

typedef struct {
	temp_sel_mode mode;
	int value;
} reg_temp_mode_t; 

#define HI_RTC_AIE_ON		_IO('p', 0x01)
#define HI_RTC_AIE_OFF		_IO('p', 0x02)
#define HI_RTC_ALM_SET		_IOW('p', 0x07,  rtc_time_t)
#define HI_RTC_ALM_READ		_IOR('p', 0x08,  rtc_time_t)
#define HI_RTC_RD_TIME		_IOR('p', 0x09,  rtc_time_t)
#define HI_RTC_SET_TIME		_IOW('p', 0x0a,  rtc_time_t)
#define HI_RTC_RESET		_IOW('p', 0x0b,  rtc_time_t)
#define HI_RTC_REG_SET		_IOW('p', 0x0c,  reg_data_t)
#define HI_RTC_REG_READ		_IOR('p', 0x0d,  reg_data_t)

#define HI_RTC_SET_TEMP_MODE	_IOW('p', 0x0e,  reg_temp_mode_t)
//modify by daijm 2013-03-29

CONST RtcConfig_t*GetRtcAttr_Flbase(VOID);
#endif

