/*
**	********************************************************************************
**                                     DVRDEF
**                          Digital Video Recoder xp
**
**   (c) Copyright 1992-2008, ZheJiang Zhiling Information Technology Stock CO.LTD.
***                            All Rights Reserved
**
**	File		: uart.h
**	Description	: 
**	Modify		: 2008-10-09		nils		Create the file
**	********************************************************************************
*/

#ifndef _UART_H_
#define _UART_H_

#include "includes.h"
#include "types.h"
//#include "system.h"
#include "gpio_config.h"

#define UART_DEVICE_HEAD 			"/dev/ttyAMA"
#define UART_THREAD_BUFFER_SIZE		128

typedef enum UseRtsType_e
{
	USE_RTS_NONE = 0,
	USE_RTS_UART,
	USE_RTS_GPIO,
}UseRtsType_e;

typedef enum UartUseType_e
{
	UART_USE_TYPE_UNKNOW,
	UART_USE_TYPE_232_COM,	//透明串口
	UART_USE_TYPE_FRONTBOARD,
	UART_USE_TYPE_PTZ,
	UART_USE_TYPE_ALARM,
	UART_USE_TYPE_SYSTEM_FAN,
	UART_USE_TYPE_SYSTEM_BEEP,
	UART_USE_TYPE_BUT,
}UartUseType_e;

typedef enum UartPort_e
{
	UartPort_MIN = 0,
	UartPort_0 = 0,		//232 (Console)
	UartPort_1,		//485
	UartPort_2,		//Frontboard
	UartPort_3,			//Reverse
	UartPort_MAX
}UartPort_e;

typedef enum UartRegs_e
{
	UART_REG_TXD = 0,
	UART_REG_RXD,
	UART_REG_CTS,
	UART_REG_RTS,
	UART_REG_BUT,
}UartRegs_e;

typedef struct UartAttr_s
 {
	DWORD	baudrate;				///< 实际的波特率值。		
	BYTE	databits;				///< 实际的数据位数。	
	BYTE	parity;					///< 奇偶校验选项，取comm_parity_t类型的枚举值。	
	BYTE	stopbits;				///< 停止位数，取comm_stopbits_t类型的枚举值。	
	BYTE	reserved;				///< 保留	
} UartAttr_t, *UartAttr_p;


typedef struct UartRegs_s
{
	RegisterDevice_t Txd;
	RegisterDevice_t Rxd;
	RegisterDevice_t Cts;
	RegisterDevice_t Rts;
}UartRegs_t, *UartRegs_p;

typedef union UartAltRegs_u
{
	UartRegs_t Regs;
	RegisterDevice_t RegsArray[UART_REG_BUT];
}UartAltRegs_u;

typedef struct UartConfig_s
{
	UartPort_e Port;
	UartUseType_e UseType;
	UartAttr_t Attr;
	UartAltRegs_u AltRegs;
	UseRtsType_e UseRtsOrGpio;
	union URG
	{
		RegisterDevice_t RtsSysReg;
		GpioConfig_t Gpio;
	}URG;
	
}UartConfig_t, *UartConfig_p;


typedef struct CommDevice_s
{
		
}CommDevice_t, *CommDevice_p;

typedef struct UartThread_s
{
	U8 *Buffer;
	U16 Read;
	U16 Write;
	U16 Ref;
}UartThread_t, *UartThread_p;

typedef struct UartDevice_s
{
	UartConfig_t Config;
	INT Fd;
	BOOL AutoConfig;
	U32  OpenFlags;
	CHAR Name[NAME_MAX_LENGTH];
	
	BOOL UseThread;
	BOOL UseMcuProctol;
	
	UartThread_t UartThread;
	GpioDevice_p pGpioDevice;		//当此串口配置存在流控，并配置为GPIO模式时使用
	
	
	INT (* CommitRead)(U32 Size, struct UartDevice_s *thiz);
	
	INT (* Read)(U8 *Buffer, U32 Size, struct UartDevice_s *thiz);
	INT (* Write)(U8 *Buffer, U32 Size, struct UartDevice_s *thiz);
	INT (* Create)(struct UartDevice_s *thiz);
	INT (* Open)(struct UartDevice_s *thiz);
	INT (* Close)(struct UartDevice_s *thiz);
	INT (* SetAttr)(UartAttr_p pUartAttr, struct UartDevice_s *thiz);
	INT (* Destory)(struct UartDevice_s *thiz);
	void * (* ThreadProc)(void *args);
	
	pthread_mutex_t WriteLock;
	pthread_mutex_t ReadLock;
	pthread_mutex_t Lock;
	
	VOID *Parant;
	VOID *Child;
	struct UartDevice_s *Prev;
	struct UartDevice_s *Next;
}UartDevice_t, *UartDevice_p;





/// 串口操作中断类型
typedef enum CommPurgeFlags
{
	commPurgeTxAbort = 0x0001,		///< 中止写操作
	commPurgeRxAbort = 0x0002,		///< 中止读操作
	commPurgeTxClear = 0x0004,		///< 清空输出缓冲
	commPurgeRxClear = 0x0008 		///< 清空输入缓冲
} CommPurgeFlags;	

typedef struct CommAttribute
{
	int		baudrate;				///< 实际的波特率值。		
	char		dataBits;				///< 实际的数据位 取值为5,6,7,8	
	char		parity;					///< 奇偶校验选项，取CommParityType类型的枚举值。	
	char		stopBits;				///< 停止位数，取CommStopBit类型的枚举值。	
	char		flag;					///< 特殊串口标志，取CommSpecialFlag类型的枚举值。	
	int		reserved[30];			///< 保留字节
} CommAttribute;


typedef enum CommStopBit
{
	commOneStopBit = 0,				///< 1 stop bit
	commOne5StopBits,				///< 1.5 stop bit
	commTwoStopBits				//< 2 stop bit
} CommStopBit;
// 串口校验位类型
typedef enum CommParityType 
{
	commNoParity = 0,				///< No parity
	commOddParity,					///< Odd parity
	commEvenParity,				///< Even parity
	commMarkParity,				///< Mark parity
	commSpaceParity				///< Space parity
} CommParityType;

/// 特殊串口标志
typedef enum CommSpecialFlag
{
	commNormal = 0,
	commRedApple
} CommSpecialFlag;

/// 串口模式
typedef enum CommMode
{
	commFullDuplex = 0,				///< 全双工
	commSemiDuplex,				///< 半双工
} CommMode;

/// 串口能力结构，128字节
typedef struct CommCaps
{
	CommMode mode;				///< 工作模式
	int internal;						///< 是否为内部使用，比如内置云台设备
	int reserved[30];					///< 保留字节
} CommCaps;


/// 串口接口描述结构，128字节
typedef struct CommPortDesc
{
	int port;							///< 物理端口号
	int reserved[31];					///< 保留字节
} CommPortDesc;


extern UartDevice_p pUartList;

#ifdef __cplusplus
extern "C" {
#endif


#if 0
extern int Uart_Device_serial232;
extern int Uart_Device_serial485;
extern int Uart_Device_frontboard;
extern int Uart_Device_reverse;
#endif
//UartDevice_p pUartDevice
INT UartDeviceCreate(struct UartDevice_s * pUartDevice);
UartDevice_p GetUartDevice(U16 UartPort);
#if 0
int UartDeviceOpen(UartPort_e Index);
int UartDeviceDestory(UartPort_e Index);
int UartDeviceClose(UartPort_e Index);
int UartDeviceRead(UartPort_e Index, void *Buffer, DWORD Length);
int UartDeviceWrite(UartPort_e Index, void *Buffer, DWORD Length);
int UartSetAttribute(UartPort_e Index, COMM_ATTR *ParmaAttribute_p);
int UartDevicePurge(UartPort_e Index, DWORD Flag);
int UartDeviceReadNoblock(UartPort_e Index, void *Buffer, DWORD Length);
int UartDeviceWriteNoblock(UartPort_e Index, void *Buffer, DWORD Length);
INT AddNewUart(UartDevice_p pUartDevice);
INT DelUart(UartDevice_p pUartDevice);
#endif

#ifdef __cplusplus
}
#endif

#endif

