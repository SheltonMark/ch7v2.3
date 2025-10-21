
#ifndef __CMOS_CONFIG_H__
#define __CMOS_CONFIG_H__

#include "includes.h"
#include "types.h"
#include "i2c_config.h"


#ifdef CMOS_USE_FILE_NAME
#define CMOS_FILE_NAME	"/opt/app/etc/cmosconfig.ini"
#else
#define CMOS_FILE_NAME	APP_CONFIG_NAME
#endif

typedef enum CmosType_e
{
	CMOS_TYPE_UNKNOW = 0,
	CMOS_TYPE_I2C,
	CMOS_TYPE_BUT,
}CmosType_e;

typedef struct CmosConfig_s
{
	CmosType_e Type;
	I2cConfig_t I2c;
}CmosConfig_t, *CmosConfig_p;

CONST CmosConfig_t  *GetCmosAttr_Flbase(VOID);
#endif

