/* 
 * mkp_spi.h -- spi 模块对外接口声明
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */


#ifndef _MKP_SPI_H_
#define _MKP_SPI_H_


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "nial.h" 
#include "mkp_ioctl.h"
#include "ni_type.h"
#include "ni_debug.h"


#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

/* flash读写结构体 */
typedef struct tagSsiAttr
{
	NI_U32 u32DevId;     /* 从设备号 */
	NI_U32 u32SlaverId;  /* SPI从设备片选 */
    NI_U32 u32Addr;      /* flash地址 */
    NI_U8* pu8Rbuf;      /* 读取指针 */
    NI_U8* pu8Wbuf;      /* 写入指针 */
    NI_U32 u32Len;       /* 读/写长度 */
} SSI_ATTR_S;


typedef struct tagSpiRdWrCfg
{
	NI_U32   u32DevId;       /* 从设备号 */
    NI_U32   u32SlaverId;   /* SPI从设备片选 */
    NI_U32   u32SendCnt;    /* 发送个数 */
    NI_VOID  *pSendData;    /* 发送数据指针 */
    NI_U32   u32RecvCnt;    /* 接收个数 */
    NI_VOID  *pRecvData;    /* 接收数据指针 */
} SPI_RD_WR_CFG_S;


typedef enum tagIocNrSpi
{
    IOC_NR_SPI_RD_SENSOR,
    IOC_NR_SPI_WR_SENSOR,
    IOC_NR_SPI_RD_FLASH,
    IOC_NR_SPI_WR_FLASH,
    IOC_NR_SPI_ER_FLASH,
    IOC_NR_SPI_SET_CTRL,
    IOC_NR_SPI_RD_WR,
}SPI_IOC_NR_E;

typedef struct tagSpiSetCtrlCfg
{   
	NI_U32 u32DevId;          /* 从设备号 */
    NI_U32 u32DataFramLen;    /*配置数据帧长度，实际数据帧长度为dfs_32 +1。
                                      dfs_32可以配置值为0x3、0x4，0x5……0x1f。
                                      根据不同芯片设计不同，最大位宽也不相同，xs5012最大位宽为16*/
    NI_U32 u32TransMod;       /*传输模式：
                                        00-Transmit & Receive；
                                        01-Transmit Only；
                                        10-Receive Only；
                                        11-eeprom。*/
    NI_U32 u32Scph;           /*sclk_in的时钟相位：
                                        0：发送数据在片选下降沿开始有效，接收数据在sclk_in在第一个时钟沿采样；
                                        1：发送数据在第一个sclk_int时钟沿开始有效，接收数据在sclk_in第二个时钟沿采样。*/
    NI_U32 u32Scpol;          /*sclk_int的时钟极性：
                                        0：sclk_in非活动时为低电平；
                                        1：sclk_in非活动时为高电平。*/
    NI_U32 u32Ending;         /*大小端模式：0：小端模式；（默认）1：大端模式(帧长度为32时) ---部分芯片不支持此配置需配置为0*/
    NI_U32 u32M2l;            /*字节发送顺序：0，从MSB开始；（默认）1，从LSB开始    --部分芯片不支持此配置需配置为0*/
    NI_U32 u32buadrate;       /*输出时钟分频，只允许偶数分频，为0时禁用时钟*/             
}SPI_SET_CTRL_CFG_S;


#define IOCTL_SPI_RD_SENSOR         _IOW(IOC_MAGIC_SPI, IOC_NR_SPI_RD_SENSOR, NI_U32)
#define IOCTL_SPI_WR_SENSOR         _IOW(IOC_MAGIC_SPI, IOC_NR_SPI_WR_SENSOR, NI_U32)
#define IOCTL_SPI_WR_FLASH          _IOW(IOC_MAGIC_SPI, IOC_NR_SPI_WR_FLASH, SSI_ATTR_S)
#define IOCTL_SPI_RD_FLASH          _IOW(IOC_MAGIC_SPI, IOC_NR_SPI_RD_FLASH, SSI_ATTR_S)
#define IOCTL_SPI_ER_FLASH          _IOW(IOC_MAGIC_SPI, IOC_NR_SPI_ER_FLASH, SSI_ATTR_S)
#define IOCTL_SPI_SET_CTRL          _IOWR(IOC_MAGIC_SPI, IOC_NR_SPI_SET_CTRL, SPI_SET_CTRL_CFG_S)
#define IOCTL_SPI_RD_WR             _IOWR(IOC_MAGIC_SPI, IOC_NR_SPI_RD_WR, SPI_RD_WR_CFG_S)




/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */



#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

