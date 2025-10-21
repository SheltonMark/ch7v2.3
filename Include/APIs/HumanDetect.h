#ifndef __HUMAN_DETECT_H__
#define __HUMAN_DETECT_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "TdCommon.h"

#define IAS_WIDTH 480
#define IAS_HEIGHT 320
#define IAS_SMALL_WIDTH 240
#define IAS_SMALL_HEIGHT 160
#define IAS_HD_OC_FILE "/var/model/net_model_yolo_model_112x112_hc.bin"

typedef struct tagIasPhreadCTx
{
	IAS_ALG_CHN OdChn;
	IAS_TASK_IMAGE_S stFrame;
	NI_BOOL bPthreadIasRun;
	VPS_CHN_E u32VpsChn;
	VPS_FRAME_INFO_S stVpsFrame;
} IAS_PTHREAD_CTX_S;

typedef struct ias_result_s
{
	unsigned int Result;
 	unsigned int x1;
 	unsigned int y1;
 	unsigned int x2;
 	unsigned int y2;
}ias_result_t;

typedef struct ias_param_s
{
	unsigned int sensiblity; /* 灵敏度 1--100 */
	unsigned int Enable;
	unsigned int change;
} ias_param_t;

typedef struct perimeter_result_s
{
	PG_RESULT PolyLineResult[2][PG_MAX_NUM];
	BOOL PolyLineResultExist[2][PG_MAX_NUM];
	PG_RESULT PolyGonResult[2][PG_MAX_NUM];
	BOOL PolyGonResultExist[2][PG_MAX_NUM];
} perimeter_result_t;

typedef struct HumanDetectDevice_s
{
	ias_result_t 	human_result[2];
	ias_param_t 	human_param[2];
	ias_result_t 	vehicle_result[2];
	ias_param_t 	vehicle_param[2];

	pthread_rwlock_t result_lock;
}HumanDetectDevice_t,*HumanDetectDevice_p;

typedef struct PerimeterDetectDevice_s
{
	perimeter_result_t Result;
	pthread_rwlock_t result_lock;
} PerimeterDetectDevice_t, *PerimeterDetectDevice_p;

/*
 * 人形检测模块初始化
 */
int human_detect_start(void);

/*
 * 人形检测模块退出
 */
int human_detect_stop(void);

/*
 * 暂停/启动人形检测；
 * 暂停/启动人脸检测；
 */
int human_detect_control(void);

/*
 * 设置人形/人脸检测的阈值
 */
int human_detect_set_thresh(void);

/*
 * 人形跟踪初始化
 */
int HumanTrackInit(void);

/*
 * 人形跟踪去初始化
 */
int HumanTrackDeInit(void);

/*
 *  暂停/启动人形跟踪；
 */
int HumanTrackRun(unsigned int Enable);

/*
 * 设置人形跟踪的最小移动阈值
 */
int HumanTrackSetParam(unsigned int MoveKpx);

/*
 * 设置人形跟踪的镜像
 */
int HumanTrackSetMirror(unsigned int mirror, unsigned int flip);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /*__HUMAN_DETECT_H__*/
