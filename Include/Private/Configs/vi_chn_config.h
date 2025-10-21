#ifndef _VI_CHN_CONFIG_H_
#define _VI_CHN_CONFIG_H_

#include "includes.h"
#include "types.h"
#include "i2c_config.h"
#include "gpio_config.h"


#ifdef AD_USE_FILE_NAME
#define VI_CHN_CONFIG_FILE 	"/opt/app/etc/adconfig.ini"
#else
#define VI_CHN_CONFIG_FILE	APP_CONFIG_NAME
#endif



#define VI_CHN_COUNT 32
typedef struct VIBindVpssCfg_s
{
	U32 ViChCnt;
	U32 VPSSChCnt;
	U32 ViToVPSSCnt[VI_CHN_COUNT];
	U32 ViChCvt[VI_CHN_COUNT];
	u32 ADChn[VI_CHN_COUNT];
}VIBindVpssCfg_t,*VIBindVpssCfg_p;

const VIBindVpssCfg_t *GetVIBindVpssCfg_Flbase();

typedef struct VIDevCfg_s
{
	VIBindVpssCfg_t vi_bind;	
	U32 ViDevCnt; /* VI设备数         */
	U32 ViDevGap; /* VI设备间隔数  --追加   */
	U32 ViChnGap; /* ViChn间隔数          */  
}VIDevCfg_t,*VIDevCfg_p;
const VIDevCfg_t *GetVIDevCfg_Flbase();


#endif
