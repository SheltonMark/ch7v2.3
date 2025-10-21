
#ifndef __WATCHDOG_CONFIG_H__
#define __WATCHDOG_CONFIG_H__



#include "includes.h"
#include "types.h"


#ifdef WATCHDOG_USE_FILE_NAME
#define WATCHDOG_FILE_NAME	"/opt/app/etc/watchdogmconfig.ini"
#else
#define WATCHDOG_FILE_NAME	APP_CONFIG_NAME
#endif

typedef enum WatchdogType_e
{
	WATCHDOG_TYPE_UNKNOW = 0,
	WATCHDOG_TYPE_FBD = 1,
	WATCHDOG_TYPE_SOFT = 2,
	WATCHDOG_TYPE_BUT,
}WatchdogType_e;

typedef struct WatchdogConfig_s
{
	//UartConfig_t Uart;
	WatchdogType_e Type;
	U8 Proctol[4];

}WatchdogConfig_t, *WatchdogConfig_p;

CONST WatchdogConfig_t*GetWatchdogAttr_Flbase(VOID);

#endif

