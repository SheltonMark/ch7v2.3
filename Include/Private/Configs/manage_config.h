#ifndef __MANAGE_CONFIG_H__
#define __MANAGE_CONFIG_H__

#include "includes.h"
#include "types.h"
#include "kernel_config.h"
#include "app_config.h"


//#define APP_CONFIG_NAME 		"/opt/app/etc/appconfig.ini"
//#define KERNEL_CONFIG_NAME	"/proc/vss2/sysconfig"

#if 1
typedef struct ManageConfig_s
{
	KernelConfig_t Kernel;
	AppConfig_t AppConfig;
}ManageConfig_t, *ManageConfig_p;


extern ManageConfig_t ManageConfig;

//INT ConfigInit(VOID);
//VOID ConfigUninit(VOID);
#endif

//#define FBD_USE_FILE_NAME

#endif



