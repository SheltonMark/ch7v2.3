
#ifndef __SERIAL_CONFIG_H__
#define __SERIAL_CONFIG_H__

#include "includes.h"
#include "types.h"
#include "uart_config.h"

#ifdef SERIAL_USE_FILE_NAME
#define SERIAL_FILE_NAME	"/opt/app/etc/serialconfig.ini"
#else
#define SERIAL_FILE_NAME	APP_CONFIG_NAME
#endif

typedef struct  SerialConfig_s
{
	U16 ConsoleUsePort;
	UartConfig_t Uart;
}SerialConfig_t, *SerialConfig_p;

CONST SerialConfig_t *GetSerialAttr_Flbase(VOID);
#endif

