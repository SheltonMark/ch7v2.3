/*
**	********************************************************************************
**                                     DVRDEF
**                          Digital Video Recoder xp
**
**   (c) Copyright 1992-2008, ZheJiang Zhiling Information Technology Stock CO.LTD.
***                            All Rights Reserved
**
**	File		: gpio.h
**	Description	: 
**	Modify		: 2008-10-09		nils		Create the file
**	********************************************************************************
*/

#ifndef _GPIO_H_
#define _GPIO_H_

#include "common.h"
//#include "system.h"

#define GPIO_PORT_COUNT		19
#define GPIO_BIT_COUNT			8

#define GPIO_DEVICE_NAME		"/dev/hi_gpio"

#define GPIO_DIR_IN 0
#define GPIO_DIR_OUT 1

typedef struct gpio_groupbit_info_s
{
	unsigned int  groupnumber;
	unsigned int  bitnumber;
	unsigned int  value;
}gpio_groupbit_info_t;

typedef gpio_groupbit_info_t gpio_groupbit_info;

#define GPIO_MAGIC_BASE 'G'
#define GPIO_SET_DIR  _IOW(GPIO_MAGIC_BASE,1,gpio_groupbit_info)
#define GPIO_GET_DIR  _IOR(GPIO_MAGIC_BASE,2,gpio_groupbit_info)
#define GPIO_READ_BIT _IOR(GPIO_MAGIC_BASE,3,gpio_groupbit_info)
#define GPIO_WRITE_BIT _IOW(GPIO_MAGIC_BASE,4,gpio_groupbit_info)



#define USER_BUFFER_SIZE	4
typedef struct UserData_s
{
	unsigned char buffer[USER_BUFFER_SIZE];
	int length;
}UserData_t;

typedef  enum GpioUseType_e
{
	GPIO_USE_TYPE_UNKNOW = 0,
	GPIO_USE_TYPE_FRONTBOARD,
	GPIO_USE_TYPE_I2C,
	GPIO_USE_TYPE_ALARM,
	GPIO_USE_TYPE_RST,
	GPIO_USE_TYPE_FAN,
	GPIO_USE_TYPE_BEEP,
	GPIO_USE_TYPE_PTZ,
	GPIO_USE_TYPE_BUT,
}GpioUseType_e;

typedef struct GpioConfig_s
{
	U16 Port;
	U16 Bit;
	RegisterDevice_t AltReg;
	GpioUseType_e GpioUseType;
}GpioConfig_t, *GpioConfig_p;

typedef struct GpioDevice_s
{
	GpioConfig_t Config;

	CHAR Name[NAME_MAX_LENGTH];
	INT Fd;
	U8 Dir;
	BOOL UseComFd;
	
	pthread_mutex_t Lock;
	INT (* Read)(U8 *pValue, struct GpioDevice_s *thiz);
	INT (* Write)(U8 Value, struct GpioDevice_s *thiz);
	INT (* Creat)(struct GpioDevice_s *thiz);
	
	VOID *Parant;
	VOID *Child;
	struct GpioDevice_s * Prev;
	struct GpioDevice_s  *Next;
	
}GpioDevice_t, *GpioDevice_p;

#if 0
typedef struct GpioDeviceList_s
{
	U32 Count;
	GpioConfig_p GpioDevice[GPIO_PORT_COUNT * GPIO_BIT_COUNT];
}GpioDeviceList_t, *GpioDeviceList_p;
#endif

extern GpioDevice_p pGpioDeviceList;

#ifdef __cplusplus
extern "C" {
#endif

INT GpioDeviceCreate(struct GpioDevice_s *thiz);
extern GpioDevice_p pGpioList;
#ifdef __cplusplus
}
#endif

#endif


