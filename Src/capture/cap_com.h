#ifndef __CAP_COM_H__
#define	__CAP_COM_H__

#include "apis.h"
//#include "VideoEnc.h"
//#include "VideoIn.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include<semaphore.h>



//#include "fy_common.h"
//#include "fy_comm_video.h"
//#include "fy_comm_sys.h"
//#include "fy_comm_vo.h"
//#include "fy_comm_vi.h"
//#include "fy_comm_venc.h"
//#include "mpi_vb.h"
//#include "mpi_sys.h"
//#include "mpi_vi.h"
//#include "mpi_vo.h"
//#include "mpi_venc.h"
//#include "list.h"



#define AVENC_S_FMT	(0x8001)
#define AVENC_S_START	(0x8002)
#define AVENC_S_STOP	(0x8003)

#define	JPEG_S_FMT		(0x8101)
#define	JPEG_S_START	(0x8102)
#define	JPEG_S_STOP	(0x8103)

#define	CAP_SET_TITLE	(0X8201)

typedef struct CAP_PARA_s
{
	unsigned int cmd;
	unsigned int chl;
	unsigned int type;
	unsigned char para[180];	
}CAP_PARA_t;

typedef struct CAP_FMT_s
{
	unsigned char	Compression;		///< 压缩标准，取video_comp_t类型的值。 
	unsigned char	BitRateControl; 	///< 码流控制模式，取video_brc_t类型的值。 
	unsigned char	ImageSize; 			///< 图像大小，取video_size_t类型的值。 
	unsigned char	ImageQuality;		///< 图像质量，取值1-6，值越大，图像质量越好,只有在可变码流下才起作用。
	unsigned char	FramesPerSecond;	///< 每秒钟的帧数。一般PAL制1-25帧/秒，NTSC制1-30帧/秒。 
	unsigned char	AVOption;			///< 音视频选项，最低位表示视频，其次位表示音频 
	unsigned short	BitRate;			///< 参考码流值，Kbps为单位
}CAP_FMT_t;

typedef struct CAP_TITLE_s
{
	int x;
	int y;
	int width;
	int height;
	int enable;
}CAP_TITLE_t;

typedef struct CAP_EN_s
{
	unsigned int rev[5];
}CAP_EN_t;

#define	MAX_CAP_COM_NR	(16 * 200)

typedef struct CAP_COM_s
{
	unsigned int 		read;
	unsigned int 		write;
	sem_t sem;
	pthread_mutex_t   write_mtx;
	CAP_PARA_t			cmdbuf[MAX_CAP_COM_NR];
}CAP_COM_t;

int CapCMDInit(void);
int CapCMDRead(CAP_PARA_t *pCapPara);
int CapCMDWrite(CAP_PARA_t *pCapPara);
void *CapCMDProcess(void *p);

#endif
