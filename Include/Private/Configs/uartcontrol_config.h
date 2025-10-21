#ifndef __UARTCONTROL_CONFIG_H__
#define __UARTCONTROL_CONFIG_H__

#include "includes.h"
#include "types.h"
#include "uart_config.h"

#ifdef PTZ_USE_FILE_NAME
#define PTZ_FILE_NAME	"/opt/app/etc/ptzconfig.ini"
#else
#define PTZ_FILE_NAME	APP_CONFIG_NAME
#endif

typedef enum UartType_e
{
	TYPE_UNKNOW = 0,
	TYPE_COM_UART = 1,
	TYPE_MCU_UART = 2,
	TYPE_BUT,
}UartType_e;

typedef struct UartConConfig_s
{
	UartConfig_t Uart;
	UartType_e Type;

}UartConConfig_t, *UartConConfig_p;


CONST UartConConfig_t*GetUartConAttr_Flbase(VOID);
#endif