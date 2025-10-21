/*******************************************************************************
 * Copyright (C) 2020-2025 ZheJiang Chipup Technology CO.,LTD.
 *
 * 文件名 : mkp_sensor.h
 *
 * 作  者 : Chipup Software Team
 * 版  本 : V1.0.0
 * 日  期 : 2022-02-23 Create
 *
 * 描  述 : 
 *
 *       1. 硬件说明
 *          无。
 *
 *       2. 程序结构说明。
 *          无。
 *
 *       3. 使用说明。
 *          无。
 *
 *       4. 局限性说明。
 *          无。
 *
 *       5. 其他说明。
 *          无。
 *
 * 修订历史: 
 * 日    期: 2022-02-23
 * 修订版本:
 * 作    者:
 * 修订备注:
 *******************************************************************************/



#ifndef _MKP_SENSOR_H_
#define _MKP_SENSOR_H_


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include <linux/ioctl.h>
#include "ni_type.h"
#include "ni_macro.h"
#include "ni_errno.h"
#include "ni_sensor.h"


#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */


#define IOC_MAGIC_SENSOR      'Z'
#define MDK_MOD_SENSOR        "SENSOR"


#define SENSOR_MAX_DEV_NUM         (1)
#define SENSOR_ISP_DGC_MAX_NUM     (3)
#define SENSOR_REG_DATA_MAX_NUM    (32)
#define SENSOR_TASK_NODE_MAX_NUM   (4)


#define  SENSOR_MAKE_FD(SenId)                  (SenId)
#define  SENSOR_MAKE_DEV(fd)                    (fd)

#define NI_ID_SENSOR       (46)

#define NI_ERR_SENSOR_INVALID_DEVID     NI_DEF_ERR(NI_ID_SENSOR, EN_ERR_LEVEL_ERROR, ERR_INVALID_DEVID)
#define NI_ERR_SENSOR_INVALID_SNSID     NI_DEF_ERR(NI_ID_SENSOR, EN_ERR_LEVEL_ERROR, ERR_INVALID_CHNID)
#define NI_ERR_SENSOR_ILLEGAL_PARAM     NI_DEF_ERR(NI_ID_SENSOR, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)
#define NI_ERR_SENSOR_LIB_EXIST         NI_DEF_ERR(NI_ID_SENSOR, EN_ERR_LEVEL_ERROR, ERR_EXIST)
#define NI_ERR_SENSOR_LIB_UNEXIST       NI_DEF_ERR(NI_ID_SENSOR, EN_ERR_LEVEL_ERROR, ERR_UNEXIST)
#define NI_ERR_SENSOR_NULL_PTR          NI_DEF_ERR(NI_ID_SENSOR, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)
#define NI_ERR_SENSOR_NOT_CONFIG        NI_DEF_ERR(NI_ID_SENSOR, EN_ERR_LEVEL_ERROR, ERR_NOT_CONFIG)
#define NI_ERR_SENSOR_NOT_SUPPORT       NI_DEF_ERR(NI_ID_SENSOR, EN_ERR_LEVEL_ERROR, ERR_NOT_SUPPORT)
#define NI_ERR_SENSOR_NOT_PERM          NI_DEF_ERR(NI_ID_SENSOR, EN_ERR_LEVEL_ERROR, ERR_NOT_PERM)
#define NI_ERR_SENSOR_NOMEM             NI_DEF_ERR(NI_ID_SENSOR, EN_ERR_LEVEL_ERROR, ERR_NOMEM)
#define NI_ERR_SENSOR_NOBUF             NI_DEF_ERR(NI_ID_SENSOR, EN_ERR_LEVEL_ERROR, ERR_NOBUF)
#define NI_ERR_SENSOR_BUF_EMPTY         NI_DEF_ERR(NI_ID_SENSOR, EN_ERR_LEVEL_ERROR, ERR_BUF_EMPTY)
#define NI_ERR_SENSOR_BUF_FULL          NI_DEF_ERR(NI_ID_SENSOR, EN_ERR_LEVEL_ERROR, ERR_BUF_FULL)
#define NI_ERR_SENSOR_NOT_INIT          NI_DEF_ERR(NI_ID_SENSOR, EN_ERR_LEVEL_ERROR, ERR_SYS_NOTREADY)
#define NI_ERR_SENSOR_TIME_OUT          NI_DEF_ERR(NI_ID_SENSOR, EN_ERR_LEVEL_ERROR, ERR_TIME_OUT)



typedef enum tagIocNrSensor
{   
    IOC_NR_SENSOR_MK_DEV,

    IOC_NR_SENSOR_SET_ATTR,
    IOC_NR_SENSOR_GET_ATTR,
    
    IOC_NR_SENSOR_GET_INFO,
    IOC_NR_SENSOR_SET_INFO,

    IOC_NR_SENSOR_SET_MATCH_KEY,
    IOC_NR_SENSOR_GET_MATCH_KEY,

    IOC_NR_SENSOR_SET_EXP,
    IOC_NR_SENSOR_SET_GAIN,
    IOC_NR_SENSOR_SET_SHT,
    IOC_NR_ISP_SET_DGAIN,
    
    IOC_NR_SENSOR_GET_EXP,
    IOC_NR_SENSOR_GET_GAIN,
    IOC_NR_SENSOR_GET_SHT,
    IOC_NR_ISP_GET_DGAIN,
    

    IOC_NR_SENSOR_BUTT
} IOC_NR_SENSOR_E;


typedef struct tagSensorDevAttr
{
    SEN_DEV_ATTR_S  stSenDevAttr;
    NI_U8          u8DevAddrByteNum;
    NI_U8          u8RegAddrByteNum;
    NI_U8          u8DataByteNum;
}SENSOR_DEV_ATTR_S;


typedef struct tagSensorRegData 
{
    NI_U8   u8DelayFrmNum;
    NI_U16  u16RegAddr;
    NI_U16  u16Data;
}SENSOR_REG_DATA_S;


typedef struct tagSensorRegs
{
    NI_U8                     u8RegNum;
    SENSOR_REG_DATA_S         astSenRegData[SENSOR_REG_DATA_MAX_NUM];
}SENSOR_REGS_S;

typedef struct tagSensorIspDGain
{
    NI_BOOL                       bGainUpdate;
    NI_U8                         u8DelayFrmNum;
    NI_U16                        au16IspDgain[SENSOR_ISP_DGC_MAX_NUM];
}SENSOR_ISP_DGAIN_S;


typedef struct tagSensorExpInfo
{
    SEN_SHT_VALUE_S             stShtValue;
    SEN_GAIN_VALUE_S            stGainValue;

    SENSOR_ISP_DGAIN_S          stIspGain;
    SENSOR_REGS_S               stSensorRegs;
}SENSOR_EXP_INFO_S;

typedef struct tagSensorExpValue
{
    SEN_SHT_VALUE_S             stShtValue;
    SEN_GAIN_VALUE_S            stGainValue;
    SENSOR_ISP_DGAIN_S          stIspGain;
}SENSOR_EXP_VALUE_S;


typedef struct tagSensorShtInfo
{
    SEN_SHT_VALUE_S             stShtValue;
    SENSOR_REGS_S               stSensorRegs;
}SENSOR_SHT_INFO_S;

typedef struct tagSensorGainInfo
{
    SEN_GAIN_VALUE_S            stGainValue;
    SENSOR_REGS_S               stSensorRegs;
}SENSOR_GAIN_INFO_S;



#define IOCTL_SENSOR_MAKE_DEV            _IOW(IOC_MAGIC_SENSOR, IOC_NR_SENSOR_MK_DEV, NI_U32)

#define IOCTL_SENSOR_SET_ATTR            _IOWR(IOC_MAGIC_SENSOR, IOC_NR_SENSOR_SET_ATTR, SENSOR_DEV_ATTR_S)
#define IOCTL_SENSOR_GET_ATTR            _IOWR(IOC_MAGIC_SENSOR, IOC_NR_SENSOR_GET_ATTR, SENSOR_DEV_ATTR_S)

#define IOCTL_SENSOR_GET_INFO            _IOWR(IOC_MAGIC_SENSOR, IOC_NR_SENSOR_GET_INFO, SEN_INFO_S)
#define IOCTL_SENSOR_SET_INFO            _IOWR(IOC_MAGIC_SENSOR, IOC_NR_SENSOR_SET_INFO, SEN_INFO_S)

#define IOCTL_SENSOR_SET_MATCH_KEY       _IOWR(IOC_MAGIC_SENSOR, IOC_NR_SENSOR_SET_MATCH_KEY, SEN_FMT_MATCH_KEY_S)
#define IOCTL_SENSOR_GET_MATCH_KEY       _IOWR(IOC_MAGIC_SENSOR, IOC_NR_SENSOR_GET_MATCH_KEY, SEN_FMT_MATCH_KEY_S)

#define IOCTL_SENSOR_SET_EXP             _IOWR(IOC_MAGIC_SENSOR, IOC_NR_SENSOR_SET_EXP, SENSOR_EXP_INFO_S)
#define IOCTL_SENSOR_SET_SHT             _IOWR(IOC_MAGIC_SENSOR, IOC_NR_SENSOR_SET_SHT, SENSOR_SHT_INFO_S)
#define IOCTL_SENSOR_SET_GAIN            _IOWR(IOC_MAGIC_SENSOR, IOC_NR_SENSOR_SET_GAIN, SENSOR_GAIN_INFO_S)
#define IOCTL_ISP_SET_DGAIN              _IOWR(IOC_MAGIC_SENSOR, IOC_NR_ISP_SET_DGAIN, SENSOR_ISP_DGAIN_S)

#define IOCTL_SENSOR_GET_EXP             _IOWR(IOC_MAGIC_SENSOR, IOC_NR_SENSOR_GET_EXP, SENSOR_EXP_VALUE_S)
#define IOCTL_SENSOR_GET_SHT             _IOWR(IOC_MAGIC_SENSOR, IOC_NR_SENSOR_GET_SHT, SEN_SHT_VALUE_S)
#define IOCTL_SENSOR_GET_GAIN            _IOWR(IOC_MAGIC_SENSOR, IOC_NR_SENSOR_GET_GAIN, SEN_GAIN_VALUE_S)
#define IOCTL_ISP_GET_DGAIN              _IOWR(IOC_MAGIC_SENSOR, IOC_NR_ISP_GET_DGAIN, SENSOR_ISP_DGAIN_S)



#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

