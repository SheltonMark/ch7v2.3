
#ifndef __REGION_API_H__
#define __REGION_API_H__

//#include "base.h"
#include <pthread.h>
#include "TdCommon.h"

#define MAX_COVER			4

#define	CAP_VB_SIZE		(16 * 1024) //(24 * 1024)

#define REGION_DEFAULT_WIDTH	350
#define REGION_DEFAULT_HIGHT	280

#define SHAMSI_MONTH_COUNT_DAY(i)  ((i>=1&&i<=6)?31:30)

typedef enum OverlayType_e
{
	OVERLAY_TIME = 0,
	OVERLAY_TITLE = 1,
	OVERLAY_COVER = 2,
	OVERLAY_TYPE_BUT,
}OverlayType_e;


	
/**
*  时间通道标题种类，目前由于所有通道，显示的时间类型
*  只有2种，适应DI，适应CIF和QCIF的，因此目前不需要每个通道为
*  为时间分配内存
*/
   
#define TIME_TITLE_KINDS       (2)
#define DI_TIME_TITLE_INDEX    (0)
#define CIF_TIME_TITLE_INDEX   (1)
#define QCIF_TIME_TITLE_INDEX  (1)

typedef struct OverlayBuffer_s
{
	video_size_t Type;
	
	WORD w;
	WORD h;

	DWORD bg_color;
	DWORD fg_color;

	
	DWORD PicWidth;
	DWORD PicHight;
	
	BYTE *ArgbBuffer;
	DWORD ArgbBufferPhyAddr;
	DWORD BufferSize;
}OverlayBuffer_t, *OverlayBuffer_p;

typedef struct OverlaySubDevice_s
{
	OverlayType_e Type;
	DWORD ImageSizeMask;		//支持的分辨率的俺码
	OverlayBuffer_p pDefaultARGB;
	INT ArgbBufferCount;
	OverlayBuffer_t *ARGB;
}OverlaySubDevice_t, *OverlaySubDevice_p;


typedef struct OverlayParam_s
{
	U32 Handle;
	pthread_mutex_t Lock;
	OverlayBuffer_p pOverlayBuffer;
	WORD x;
	WORD y;

	WORD org_x;
	WORD org_y;

	INT Enable;

	DWORD bg_color;
	DWORD fg_color;


	int DevIndex;
	void *pParnet;
}OverlayParam_t, *OverlayParam_p;


typedef struct CharTime_s
{
	char CH;
	OverlaySubDevice_t CharSumDevice;
}CharTime_t, *CharTime_p;





typedef struct CrDev_s
{
	U32 Handle;
	pthread_mutex_t Lock;	

	int DevIndex;
	void *pParnet;
}CrDev_t, *CrDev_p;

typedef struct OyDev_s
{

	OverlaySubDevice_t SubDevice;
		
	//OverlayBuffer_p pOverlay;
	OverlayType_e Type;

	INT StramCount;
	OverlayParam_p Stream;//[STREAM_TYPE_BUT];
	
	
	int DevIndex;
	pthread_mutex_t OyLock;
	void *pParnet;
}OyDev_t, *OyDev_p;

typedef struct LineDev_s
{
	U32 Handle;
	int DevIndex;
	pthread_mutex_t LineLock;
	void *pParnet;
}LineDev_t,*LineDev_p;

typedef struct RnDev_s
{
	int CrDevCount;
	CrDev_p CrDev;

	int OyDevCount;
	OyDev_p OyDev;
	
	int LineDevCount;
	LineDev_p LineDev;

	void *pParent;
	int DevIndex;
	pthread_mutex_t RnDevLock;
}RnDev_t, *RnDev_p;



//#define MAX_RN_DEV_COUNT	2;
#define MAX_RN_DEV_COUNT	32 //16

typedef struct Osd_roll_s
{
	int rollEnable;
	int rollHand;
	SIZE_S stSize;
	HI_U32 *srcPhy;
    HI_VOID **srcVir;
    HI_U32 *dstPhy;
    HI_VOID **dstVir;
    int srcX;
    int dstX;
    int VpssW;
    int rgbW;
    int rgbH;
    HI_U32 *rgbPhy;
    HI_VOID **rgbVir;
   	int zoom;
}Osd_roll_t,*Osd_roll_p;

#define MAX_STR_NUM     (18)
typedef struct RegionDevice_s
{
	pthread_t RegPid;
	pthread_t RegRollPid;
	pthread_t RegLinePid;
	int StartProc;
	int RnDevCount;
	RnDev_p RnDev;

	
	CharTime_t CharTimeTable[MAX_STR_NUM];
	int CharTimeCount;
	BYTE CharToIndexTable[0xff];
	
	OverlaySubDevice_t OyTime;
	
	int ArgbBufferCount;
	DWORD ImageSizeMask;
	int DateFmt;
	int TimeFmt;
	int DateSpc;
	char TimeBuffer[30];
	pthread_mutex_t RnLock;
	int CalendarType;//日历类型
	Osd_roll_t osdRoll[16];
	pthread_mutex_t rollLock;
	//struct RegionDevice_s *Parent;
	
/*
	int OyDevCount;
	int OyHandleStart;
	OyDev_p pOyDev;
	
	int CrDevCount;
	

	OverlayDevice_t OverlayDevice;
	pthread_mutex_t     lock;
	
	RGN_HANDLE CoverHandle[MAX_CAPTURE_CHANNEL][MAX_COVER];
	INT coverCountPerChannel;
	INT coverHandleStart;
*/
	VIDEO_COVER_PARAM COVER_PARAM[VPS_MASK_MAX_CHN_NUM];
	char channeltitle[TD_OSD_MAX_NUM][1024];
	int OsdWidth[TD_OSD_MAX_NUM];
	int OsdHeight;
	int PositionX[TD_OSD_MAX_NUM];
	int PositionY[TD_OSD_MAX_NUM];
	int show[TD_OSD_MAX_NUM];
	int InvertEnable;
	unsigned int FgColor;
	unsigned int BgColor;
	unsigned int language;
	unsigned int WeekEnable;
	unsigned int LogoEnable;
	unsigned int LogoInit;
	unsigned int LogoX;
	unsigned int LogoY;
}RegionDevice_t, *RegionDevice_p;


typedef struct
{
	unsigned int x;
	unsigned int y;
	unsigned int width;
	unsigned int height;
	unsigned int imagesize;
	unsigned int xy_align;
	unsigned int wh_align;
}RectInfo;
//int RegionSetSize(int ArgbBufferCount, OverlayBuffer_t* ARGB, OverlayBuffer_p pDefaultOverlay, BOOL NeedAlign);

int RegionSetSize(int OverlayType, OverlayBuffer_p pOverlayBuffer, OverlayBuffer_p pDefaultOverlay, BOOL NeedAlign);
int RegionSetDefaultSize(OverlayBuffer_p pOverlay, VIDEO_TITLE_PARAM *pParam);
int RegionUpdateOverlay(INT Chn, StreamType_e StreamType, OverlayType_e OverlayType);
int _RegionGetPicSize(VIDEO_NORM_E enNorm, video_size_t enPicSize, SIZE_S *pstSize);
int OySelect(INT Chn, StreamType_e Type, OverlayType_e OverlayType);
int RegionSetChannelOverlay(int Chn, VIDEO_TITLE_PARAM *pParam);
int RegionRecreate(U32 RgnHandle, int DevId, RectInfo *RgnAttr);
int RegionUpdate(INT Chn, StreamType_e StreamType, OverlayType_e OverlayType);
HI_S32 SAMPLE_RGN_Init(void);
//int CaptureSetOsd(int Channel, StreamType_e VirChannel, OverlayType_e Type, VIDEO_TITLE_PARAM *pParam);
#endif

