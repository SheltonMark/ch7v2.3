#ifndef __GRAPHICRIV_H__
#define __GRAPHICRIV_H__

#include <sdk_comm.h>
#include "MotionDetect.h"
#include "BlindDetect.h"

#define MD_AREA_NUM_WIDTH 7
#define MD_AREA_NUM_HEIGHT 5

typedef struct
{
    MD_CONFIG md_config;
	int ilevel;
	int win[25];
	int result;
	pthread_mutex_t md_cd_lock; 
}XKY_MDConfig_t;

typedef struct
{
	unsigned int init;
	unsigned char *md_data;
	unsigned int cd_result;
	unsigned int md_cd_state;
	unsigned int request_to_stop;
	unsigned int md_cd_is_running;

	pthread_mutex_t MD_CD_lock;
}MD_CD_Check_t, *MD_CD_Check_p;

typedef struct MotionDetectDevice_s
{
    XKY_MDConfig_t md_cf;  /* 动检配置 */
	int g_md_proc_start;
	MOTION_DETECT_RESULT md_re;
	int state_change;
}MotionDetectDevice_t, *MotionDetectDevice_p;

typedef struct BlindDetectDevice_s
{
	CDConfig_t cd_config;
}BlindDetectDevice_t, *BlindDetectDevice_p;

typedef struct LostDetectDevice_s
{
    
}LostDetectDevice_t, *LostDetectDevice_p;

#endif
