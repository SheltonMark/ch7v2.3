#ifndef __KERNEL_CONFIG_H__
#define __KERNEL_CONFIG_H__

#include "includes.h"
#include "types.h"
#include "uart_config.h"
#include "gpio_config.h"
#include "system_config.h"

#define KERNEL_CONFIG_NAME	"/proc/vss2/sysconfig"


typedef struct BaseConfig_s
{
	CHAR Platform[NAME_MAX_LENGTH];
	U16 ConsolePort;
	EncryptConfig_t Encrypt;
}BaseConfig_t, *BaseConfig_p;

typedef struct KernelConfig_s
{
	BaseConfig_t Base;
	UartConfig_t Uart[UartPort_MAX];
	GpioConfig_t Gpio[GPIO_PORT_COUNT][GPIO_BIT_COUNT];
	
}KernelConfig_t, *KernelConfig_p;

INT KernelConfigInit(void);


#endif
