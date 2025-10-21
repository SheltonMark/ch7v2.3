#ifndef __ALARM_CONFIG_H__
#define __ALARM_CONFIG_H__


#include "includes.h"
#include "types.h"
#include "uart_config.h"
#include "gpio_config.h"



#ifdef ALARM_USE_FILE_NAME
#define ALARM_FILE_NAME	"/opt/app/etc/alarmconfig.ini"
#else
#define ALARM_FILE_NAME	APP_CONFIG_NAME
#endif

#define MAX_ALARM_SLOTS		32
#define MAX_CHIP_COUNT		4

typedef enum AlarmType_e
{
	ALARM_TYPE_UNKNOW = 0,
	ALARM_TYPE_GPIO,
	ALARM_TYPE_UART_TO_MCU,
	ALARM_TYPE_PARALLEL_TO_SERIAL,
	AlARM_TYPE_BUT,
}AlarmType_e;

//通用gpio模式
typedef struct ATGpio_s
{
	GpioConfig_p Config;
}ATGpio_t, *ATGpio_p;

//uart 转mcu模式
typedef struct ATUTM_s
{
	UartConfig_t Config;
}ATUTM_t, *ATUTM_p;



typedef struct AT_PTS_s
{
	GpioConfig_t Data;
}AT_PTS_t, *AT_PTS_p;

//并转串芯片后读gpio模式
typedef struct ATPTS_s
{
	U32 ChipCount;
	GpioConfig_t Clcok;
	GpioConfig_t Load;
	AT_PTS_p pPts;
}ATPTS_t, *ATPTS_p;

typedef union AlarmConfig_u
{
	ATGpio_t Gpio;
	ATUTM_t UartToMcu;
	ATPTS_t ParallelToSerial;
}AlarmConfig_u;

typedef struct AlmConfig_s
{
	U32	Mask;
	U32 MaskSlots;
	U32 Slots;
	AlarmType_e Type;
	AlarmConfig_u u; 
}AlmConfig_t, *AlmConfig_p;


typedef struct AlarmConfig_s
{
	AlmConfig_t AlarmIn;
	AlmConfig_t AlarmOut;
}AlarmConfig_t, *AlarmConfig_p;


CONST AlarmConfig_t  *GetAlarmAttr_Flbase(VOID);
#endif

