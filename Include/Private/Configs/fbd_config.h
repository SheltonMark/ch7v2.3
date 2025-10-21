#ifndef __FBD_CONFIG_H__
#define __FBD_CONFIG_H__

#include "includes.h"
#include "types.h"
#include "uart_config.h"
#include "gpio_config.h"
//#include "manage_config.h"

#ifdef FBD_USE_FILE_NAME
#define FBD_FILE_NAME	"/opt/app/etc/fbdconfig.ini"
#else
#define FBD_FILE_NAME	APP_CONFIG_NAME
#endif


#define BOARD_KEY_MAX_ROW			10
#define BOARD_KEY_MAX_COL			10

#define TABLE_TYPE_FRONTBOARD		0
#define TABLE_TYPE_IRDA		1


typedef enum FbdComType_e
{
	FBD_TYPE_UNKNOW,
	FBD_TYPE_UART,
	FBD_TYPE_GPIO,
	FBD_TYPE_BUT,
}FbdComType_e;

typedef enum LedEnum_e
{
	LED_REC = 0,
	LED_RUN,
	LED_ALARM,
	LED_HDDFULL,
	LED_LINK,
	LED_HDD,
	LED_BUT,
}LedEnum_e;

typedef struct FbdUartConfig_s
{
	UartConfig_t Uart;
	//KeyTable[0x10000]中有三个字节，第0,1个代表协议转码，第2个代表这个转码的类型
	//类型
	U8 KeyTable[0x10000][3];
}FbdUartConfig_t, *FbdUartConfig_p;


typedef struct FbdGpioLedConfig_s
{
	GpioConfig_t Config;
	U16 Proctol;
}FbdGpioLedConfig_t, *FbdGpioLedConfig_p;

typedef struct FbdGpioConfig_s
{
	U32 RowCount;
	U32 ColCount;
	GpioConfig_p GpioRow;
	GpioConfig_p GpioCol;
	U8 **KeyTable;

	U16 IrdaTable[0x100];

	FbdGpioLedConfig_p LedTable;
	U32 LedTableSize;
}FbdGpioConfig_t, *FbdGpioConfig_p;


typedef struct FbdConfig_s
{
	union u
	{
		FbdUartConfig_t FbdUartConfig;
		FbdGpioConfig_t FbdGpioConfig;
	}u;
	FbdComType_e FbdComType;
	
}FbdConfig_t, *FbdConfig_p;

CONST FbdConfig_t  *GetFbdAttr_Flbase(VOID);
#endif

