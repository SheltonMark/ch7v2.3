/*
**	********************************************************************************
**                                     DVRDEF
**                          Digital Video Recoder xp
**
**   (c) Copyright 1992-2008, ZheJiang Zhiling Information Technology Stock CO.LTD.
***                            All Rights Reserved
**
**	File		: i2c.h
**	Description	: define for cmos.c
**	Modify		: 2008-10-09		nils		Create the file
**	********************************************************************************
*/

#ifndef _I2C_H_
#define _I2C_H_

#include "includes.h"
#include "types.h"

//i2c设备结点
#define I2C_DEVICE_NAME		"/dev/gpioi2c"
#define GPIO_I2C_MAGIC_BASE 'I'

#define GPIO_I2C_READ_BYTE   _IOR(GPIO_I2C_MAGIC_BASE, 0x01, int)
#define GPIO_I2C_WRITE_BYTE  _IOW(GPIO_I2C_MAGIC_BASE, 0x02, int) 
#define GPIO_I2C_READ   GPIO_I2C_READ_BYTE
#define GPIO_I2C_WRITE   GPIO_I2C_WRITE_BYTE 
#define GPIO_I2C_INIT  _IOW(GPIO_I2C_MAGIC_BASE, 0x03, int) 


typedef struct i2c_ioctl_s
{
	U8 value;
	U16 off;
	U8 addr;
	U8 scl_port;
	U8 scl_bit;
	U8 sda_port;
	U8 sda_bit;
}i2c_ioctl_t, *i2c_ioctl_p;



typedef struct I2cConfig_s
{
	U8 ChipAddr;
	U16 RegStart;
	U8 scl_alt_value;
	U32 scl_alt_addr;
	U32 sda_alt_value;
	U8 sda_alt_addr;
	U8 scl_port;
	U8 scl_bit;
	U8 sda_port;
	U8 sda_bit;
	U8 reserve[3];
}I2cConfig_t, *I2cConfig_p;

typedef struct I2cDevice_s
{
	I2cConfig_t Config;
	
	INT Fd;
	CHAR Name[NAME_MAX_LENGTH];
	BOOL UseComFd;
	
	INT (* Read)( U16 RegOffset, U8 *Buffer, U32 Size, struct I2cDevice_s *thiz);
	INT (* Write)(U16 RegOffset, U8 *Buffer, U32 Size, struct I2cDevice_s *thiz);
	INT (* Create)(struct I2cDevice_s *thiz);
	INT (* Open)(struct I2cDevice_s *thiz);
	INT (* Close)(struct I2cDevice_s *thiz);
	INT (* Destory)(struct I2cDevice_s *thiz);
	
	void (* ThreadProc)(void *);
	
	pthread_mutex_t ReadLock;
	pthread_mutex_t WriteLock;
	
	VOID *Parant;
	VOID *Child;
	struct I2cDevice_s *Prev;
	struct I2cDevice_s *Next;
	
}I2cDevice_t, *I2cDevice_p;




#ifdef __cplusplus
extern "C" {
#endif

int CMOSRead(int offs, void * pdat, int len);
int CMOSWrite(int offs, void * pdat, int len);
INT I2cDeviceCreate(struct I2cDevice_s *thiz);

#ifdef __cplusplus
}
#endif

#endif

