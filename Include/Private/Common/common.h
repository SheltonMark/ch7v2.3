/*
**	********************************************************************************
**                                     DVRDEF
**                          Digital Video Recoder xp
**
**   (c) Copyright 1992-2008, ZheJiang Zhiling Information Technology Stock CO.LTD.
***                            All Rights Reserved
**
**	File		: common.h
**	Description	: define for common.c
**	Modify		: 2008-12-18		nils		Create the file
**	********************************************************************************
*/

#ifndef _COMMON_H_
#define _COMMON_H_

#include "includes.h"
#include "types.h"




#define PAGE_SIZE						0x1000
#define MEMORY_DEVICE					"/dev/mem"

//读写类型枚举
typedef enum ReadWrite_e
{
	READ = 0,
	WRITE
}ReadWrite_e;

typedef struct RegisterDevice_s
{
	U32 Addr;
	U32 Value;
}RegisterDevice_t, *RegisterDevice_p;

#define MCU_BUFFER_SIZE		16
#define MCU_PROCTOL_HEAD		0x5aff
#define MCU_PROCTOL_HEAD_SRE		0xff5a

typedef enum McuCmd_e
{
	MCU_CMD_SET_UART_ATTR = 0x00,
	MCU_CMD_PTZ = 0x01,
	MCU_CMD_ALARM = 0x02,
	MCU_CMD_ALIVE	= 0x03,
	MCU_CMD_BEEP = 0x04,
	MCU_CMD_FAN = 0x05,
	MCU_CMD_BUT,
}McuCmd_e;

typedef struct McuProctol_s
{
	U16 Head;
	U8 Cmd;
	U8 DataLength;
	U8 DataBuffer[MCU_BUFFER_SIZE];
}McuProctol_t, *McuProctol_p;

#define MAX_NAME_LENGTH		32

extern U32 RstSysRegAddr;


#ifdef __cplusplus
extern "C" {
#endif
BOOL IsIgnoreLine(CHAR *Line);
#ifdef __cplusplus
}
#endif

#endif

