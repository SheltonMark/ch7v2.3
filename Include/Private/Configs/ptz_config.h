
#ifndef __PTZ_CONFIG_H__
#define __PTZ_CONFIG_H__

#include "includes.h"
#include "types.h"
#include "uart_config.h"

#ifdef PTZ_USE_FILE_NAME
#define PTZ_FILE_NAME	"/opt/app/etc/ptzconfig.ini"
#else
#define PTZ_FILE_NAME	APP_CONFIG_NAME
#endif

typedef enum PtzType_e
{
	PTZ_TYPE_UNKNOW = 0,
	PTZ_TYPE_COM_UART = 1,
	PTZ_TYPE_MCU_UART = 2,
	PTZ_TYPE_BUT,
}PtzType_e;

typedef struct PtzConfig_s
{
	UartConfig_t Uart;
	PtzType_e Type;

}PtzConfig_t, *PtzConfig_p;


CONST PtzConfig_t*GetPtzAttr_Flbase(VOID);
#endif

