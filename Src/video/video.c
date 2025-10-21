#include "TdCommon.h"
#include "sdk_comm.h"
#include "logo.h"
#include "mdk_vps.h"
#include "ni_comm_vps.h"
#include "modules/include/video_encoder.h"

extern int capture_video_stream_start(void);
extern int capture_video_stream_stop(void);

//编码自适应线程标志位
BOOL VencAdaptiveRunning = FALSE;
//编码自适应线程的pid
pthread_t venc_pid;
unsigned int gFrameRate = 30;
unsigned int sensor_fps = 30;

pthread_mutex_t osd_lock;
pthread_rwlock_t ChangeFrmLock = PTHREAD_RWLOCK_INITIALIZER;
pthread_rwlock_t ChangeParamLock = PTHREAD_RWLOCK_INITIALIZER;

//sensor降帧标志位
BOOL ChangeFrmRate = FALSE;
// 用户修改编码配置标志位
// 这边暂时使用最大通道数为4，后续在枪球上再进行调整
BOOL ChangeChnParam[VENC_MAX_CHN_NUM] = {FALSE};
// 用户修改编码配置后保存的临时配置文件
channel_info ChnParam[VENC_MAX_CHN_NUM] = {0};

extern BOOL ChangeFrmStart;

int FontSiZeTable[] = {64, 32, 16};

static VENC_PROFILE_E H264Profile_Table[]={
										VENC_H264_BASE_LINE_PROFILE,
										VENC_H264_MAIN_PROFILE,
										VENC_H264_HIGH_PROFILE
										};

static VENC_PROFILE_E H265Profile_Table[]={VENC_H265_MAIN_PROFILE};

static CaptureImageQuality_t CaptureQtTable[6] =
{
	{
		.ImaxQP = 50,
		.IminQP = 40,
		.PmaxQP = 50,
		.PminQP = 42
	},
	{
		.ImaxQP = 50,
		.IminQP = 36,
		.PmaxQP = 50,
		.PminQP = 38
	},
	{
		.ImaxQP = 44,
		.IminQP = 32,
		.PmaxQP = 48,
		.PminQP = 34
	},
	{
		.ImaxQP = 44,
		.IminQP = 28,
		.PmaxQP = 48,
		.PminQP = 30
	},
	{
		.ImaxQP = 44,
		.IminQP = 20,
		.PmaxQP = 48,
		.PminQP = 20
	},			
	{
		.ImaxQP = 44,
		.IminQP = 10,
		.PmaxQP = 48,
		.PminQP = 10
	},
};

static CaptureImageQuality_t CHL_2END_T_CaptureQtTable[6] =
{
	{
		.ImaxQP = 50,
		.IminQP = 20,
		.PmaxQP = 50,
		.PminQP = 20
	},
	{
		.ImaxQP = 50,
		.IminQP = 18,
		.PmaxQP = 50,
		.PminQP = 18
	},
	{
		.ImaxQP = 50,
		.IminQP = 16,
		.PmaxQP = 50,
		.PminQP = 16
	},
	{
		.ImaxQP = 50,
		.IminQP = 14,
		.PmaxQP = 50,
		.PminQP = 14
	},
	{
		.ImaxQP = 50,
		.IminQP = 12,
		.PmaxQP = 50,
		.PminQP = 12
	},			
	{
		.ImaxQP = 50,
		.IminQP = 10,
		.PmaxQP = 50,
		.PminQP = 10
	},
};

static SIZE_S imageSize[2][VIDEO_SIZE_NR] =
{
	{ // PAL
		{704, 576},			// D1	
		{704, 288},			// HD1	
		{352, 576},			// BCIF	
		{352, 288},			// CIF	
		{176, 144},			// QCIF	
		{640, 480},			// VGA	
		{320, 240},			// QVGA	
		{480, 480},			// SVCD	
		{160, 128},			// QQVGA
		{800, 592},			// SVGA	
		{1024, 768},		// XVGA	
		{1280, 800},		// WXGA	
		{1280, 1024},		// SXGA	
		{1600, 1024},		// WSXGA
		{1600, 1200},		// UXGA	
		{1920, 1200},		// WUXGA
		{240, 192},			// LTF	
		{1280, 720},		// 720P	
		{1920, 1080},		// 1080P
		{1280, 960},		// HD960
		{960, 576},			// 960H
		{1280, 1024},		// HD1024
		{960, 1080},		// HALF1080
		{2048, 1536},		// QXGA
		{3840, 2160},		// UHD4K
		{2688, 1520},		// 4M
		{2880, 1620},		// 5M
		{1280, 1440},		// HALF4M
		{4000, 3000},		// 12M
		{2560, 1440},		// 4MQHD
		{1920, 1536},		// 3M
		{1440, 900},		// 1440*900
		{640, 368},			// NHD
		{2304, 1296},		// 1296P
		{960, 544},			// 540P
	},						
	{ // NTSC
		{704, 480},			// D1	
		{704, 240},			// HD1	
		{352, 480},			// BCIF	
		{352, 240},			// CIF	
		{176, 120},			// QCIF	
		{640, 480},			// VGA	
		{320, 240},			// QVGA	
		{480, 480},			// SVCD	
		{160, 128},			// QQVGA
		{800, 592},			// SVGA	
		{1024, 768},		// XVGA	
		{1280, 800},		// WXGA	
		{1280, 1024},		// SXGA	
		{1600, 1024},		// WSXGA
		{1600, 1200},		// UXGA	
		{1920, 1200},		// WUXGA
		{240, 192},			// LTF	
		{1280, 720},		// 720P	
		{1920, 1080},		// 1080P
		{1280, 960},		// HD960
		{960, 480},			// 960H
		{1280, 1024},		// HD1024
		{960, 1080},		// HALF1080
		{2048, 1536},		// QXGA
		{3840, 2160},		// UHD4K
		{2688, 1520},		// 4M
		{2880, 1620},		// 5M
		{1280, 1440},		// HALF4M
		{4000, 3000},		// 12M
		{2560, 1440},		// 4MQHD
		{1920, 1536},		// 3M
		{1440, 900},		// 1440*900
		{640, 368},			// NHD
		{2304, 1296},		// 1296P
		{960, 544},			// 540P
	}
};

int _osd_error(MDK_CHN_S *stMdkChn, OSD_DISP_ATTR_S *stOsdDispAttr)
{
	SystemAsh("cat /proc/umap/osd");

	PRINT_ERROR("stMdkChn->enModId %d\n", stMdkChn->enModId);
	PRINT_ERROR("stMdkChn->s32ChnId %d\n", stMdkChn->s32ChnId);
	PRINT_ERROR("stMdkChn->s32DevId %d\n", stMdkChn->s32DevId);

	PRINT_ERROR("stOsdDispAttr->bShow %d\n", stOsdDispAttr->bShow);
	PRINT_ERROR("stOsdDispAttr->stSize.u32Height %d\n", stOsdDispAttr->stSize.u32Height);
	PRINT_ERROR("stOsdDispAttr->stSize.u32Width %d\n", stOsdDispAttr->stSize.u32Width);
	PRINT_ERROR("stOsdDispAttr->stStartPiont.s32X %d\n", stOsdDispAttr->stStartPiont.s32X);
	PRINT_ERROR("stOsdDispAttr->stStartPiont.s32Y %d\n", stOsdDispAttr->stStartPiont.s32Y);
	PRINT_ERROR("stOsdDispAttr->u32Layer %d\n", stOsdDispAttr->u32Layer);
	PRINT_ERROR("stOsdDispAttr->enAlphaMode %d\n", stOsdDispAttr->enAlphaMode);
	PRINT_ERROR("stOsdDispAttr->u32RegionAlpha %d\n", stOsdDispAttr->u32RegionAlpha);
	PRINT_ERROR("stOsdDispAttr->bInvert %d\n", stOsdDispAttr->bInvert);
	PRINT_ERROR("stOsdDispAttr->bSpecialFrmId %d\n", stOsdDispAttr->bSpecialFrmId);
	PRINT_ERROR("stOsdDispAttr->u32TargetFrameId %d\n", stOsdDispAttr->u32TargetFrameId);
	return 0;
}

int _venc_error(VENC_CHN_ATTR_S *VENC_ChnAttr)
{
	// PRINT_ERROR("VENC_ChnAttr->stGopAttr.u32Gop %d\n", VENC_ChnAttr->stGopAttr.u32Gop);
	// PRINT_ERROR("VENC_ChnAttr->stGopAttr.u32DummyIGop %d\n", VENC_ChnAttr->stGopAttr.u32DummyIGop);
	// PRINT_ERROR("VENC_ChnAttr->stGopAttr.u32DummyIOffset %d\n", VENC_ChnAttr->stGopAttr.u32DummyIOffset);
	// PRINT_ERROR("VENC_ChnAttr->stGopAttr.u32DummyIType %d\n", VENC_ChnAttr->stGopAttr.u32DummyIType);

	PRINT_ERROR("VENC_ChnAttr->stVeAttr.enType %d\n", VENC_ChnAttr->stVeAttr.enType);
	PRINT_ERROR("VENC_ChnAttr->stVeAttr.enProfile %d\n", VENC_ChnAttr->stVeAttr.enProfile);
	PRINT_ERROR("VENC_ChnAttr->stVeAttr.u32StrmBufSize %d\n", VENC_ChnAttr->stVeAttr.u32StrmBufSize);
	PRINT_ERROR("VENC_ChnAttr->stVeAttr.stInputPicAttr.u32MaxPicWidth %d\n", VENC_ChnAttr->stVeAttr.stInputPicAttr.u32MaxPicWidth);
	PRINT_ERROR("VENC_ChnAttr->stVeAttr.stInputPicAttr.u32MaxPicHeight %d\n", VENC_ChnAttr->stVeAttr.stInputPicAttr.u32MaxPicHeight);
	PRINT_ERROR("VENC_ChnAttr->stVeAttr.stInputPicAttr.enPixelFormat %d\n", VENC_ChnAttr->stVeAttr.stInputPicAttr.enPixelFormat);
	PRINT_ERROR("VENC_ChnAttr->stVeAttr.stInputPicAttr.u32PicWidth %d\n", VENC_ChnAttr->stVeAttr.stInputPicAttr.u32PicWidth);
	PRINT_ERROR("VENC_ChnAttr->stVeAttr.stInputPicAttr.u32PicHeight %d\n", VENC_ChnAttr->stVeAttr.stInputPicAttr.u32PicHeight);
	PRINT_ERROR("VENC_ChnAttr->stVeAttr.stInputPicAttr.au32Stride[0]%d\n", VENC_ChnAttr->stVeAttr.stInputPicAttr.au32Stride[0]);
	PRINT_ERROR("VENC_ChnAttr->stVeAttr.stInputPicAttr.au32Stride[1] %d\n", VENC_ChnAttr->stVeAttr.stInputPicAttr.au32Stride[1]);
	PRINT_ERROR("VENC_ChnAttr->stVeAttr.stInputPicAttr.au32Stride[2] %d\n", VENC_ChnAttr->stVeAttr.stInputPicAttr.au32Stride[2]);

	PRINT_ERROR("VENC_ChnAttr->stRcAttr.enRcMode %d\n", VENC_ChnAttr->stRcAttr.enRcMode);
	PRINT_ERROR("VENC_ChnAttr->stRcAttr.u32FrmRateNum %d\n", VENC_ChnAttr->stRcAttr.u32FrmRateNum);
	PRINT_ERROR("VENC_ChnAttr->stRcAttr.u32FrmRateDenom %d\n", VENC_ChnAttr->stRcAttr.u32FrmRateDenom);

	switch (VENC_ChnAttr->stRcAttr.enRcMode)
	{
		case (VENC_RC_MODE_CBR):
			PRINT_ERROR("VENC_ChnAttr->stRcAttr.stAttrCbr.u32StatTime %d\n", VENC_ChnAttr->stRcAttr.stAttrCbr.u32StatTime);
			PRINT_ERROR("VENC_ChnAttr->stRcAttr.stAttrCbr.u32TargetBitRate %d\n", VENC_ChnAttr->stRcAttr.stAttrCbr.u32TargetBitRate);
			break;
		case (VENC_RC_MODE_QVBR):
			PRINT_ERROR("VENC_ChnAttr->stRcAttr.stAttrQVbr.u32MaxBitRate %d\n", VENC_ChnAttr->stRcAttr.stAttrQVbr.u32MaxBitRate);
			PRINT_ERROR("VENC_ChnAttr->stRcAttr.stAttrQVbr.u32StatTime %d\n", VENC_ChnAttr->stRcAttr.stAttrQVbr.u32StatTime);
			PRINT_ERROR("VENC_ChnAttr->stRcAttr.stAttrQVbr.u32MaxIQp %d\n", VENC_ChnAttr->stRcAttr.stAttrQVbr.u32MaxIQp);
			PRINT_ERROR("VENC_ChnAttr->stRcAttr.stAttrQVbr.u32MinIQp %d\n", VENC_ChnAttr->stRcAttr.stAttrQVbr.u32MinIQp);
			PRINT_ERROR("VENC_ChnAttr->stRcAttr.stAttrQVbr.u32MaxPQp %d\n", VENC_ChnAttr->stRcAttr.stAttrQVbr.u32MaxPQp);
			PRINT_ERROR("VENC_ChnAttr->stRcAttr.stAttrQVbr.u32MinPQp %d\n", VENC_ChnAttr->stRcAttr.stAttrQVbr.u32MinPQp);
			break;
		default:
			PRINT_ERROR("unknown enRcMode %d\n", VENC_ChnAttr->stRcAttr.enRcMode);
			return -1;
	}

	return 0;
}

///	点阵放大
///
/// \param [in] psrcOsd 转换前的数据
/// \param [in] enPixelFormat 需要转换的位数
/// \param [in] psrcSize 原始点阵的大小
/// \param [in] EnlargeNumber 放大倍数
/// \param [out] pdestOsd 转换后的数据
/// \retval 0  设置成功。
/// \retval <0  设置失败。
int _osd_enlarge(unsigned char *psrcOsd, unsigned char *pdestOsd, OSD_PIXEL_FORMAT_E enPixelFormat, unsigned int EnlargeNumber, SIZE_S *psrcSize, SIZE_S *pdestSize)
{
	int i = 0;
	int j = 0;
	int u32DivNum = 0;
	unsigned char value;
	unsigned char *SrcVirAddr = NULL;
	unsigned char *DestVirAddr0 = NULL;

	if (NULL == psrcOsd || NULL == pdestOsd || 0 == EnlargeNumber || NULL == psrcSize || NULL == pdestSize)
	{
		PRINT_ERROR("_osd_enlarge error!!!\n");
		return -1;
	}

	if (OSD_PIXEL_FORMAT_BYTE == enPixelFormat)
	{
		u32DivNum = 1;
	}
	else if (OSD_PIXEL_FORMAT_4BIT == enPixelFormat)
	{
		u32DivNum = 2;
	}
	else if (OSD_PIXEL_FORMAT_2BIT == enPixelFormat)
	{
		u32DivNum = 4;
	}
	else if (OSD_PIXEL_FORMAT_1BIT == enPixelFormat)
	{
		u32DivNum = 8;
	}
	else
	{
		PRINT_ERROR("_osd_enlarge unknown enPixelFormat %d!!!\n", enPixelFormat);
		return -1;
	}

	pdestSize->u32Width = psrcSize->u32Width * EnlargeNumber;
	pdestSize->u32Height = psrcSize->u32Height * EnlargeNumber;
	SrcVirAddr = (NI_U8 *)psrcOsd;
	DestVirAddr0 = (NI_U8 *)pdestOsd;

	memset(pdestOsd, 0, pdestSize->u32Width * pdestSize->u32Height / u32DivNum);

	if (1 == EnlargeNumber)
	{
		memcpy(DestVirAddr0, SrcVirAddr, pdestSize->u32Width * pdestSize->u32Height / u32DivNum);
	}
	else if (2 == EnlargeNumber)
	{
		if (OSD_PIXEL_FORMAT_1BIT == enPixelFormat)
		{
			for (i = 0; i < psrcSize->u32Height; i++)
			{
				for (j = 0; j < (psrcSize->u32Width / u32DivNum); j++)
				{
					value = *(SrcVirAddr + i * psrcSize->u32Width / u32DivNum + j);
					
					*(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j) = (value & 0x80) | ((value & 0x80) >> 1) | ((value & 0x40) >> 1) | ((value & 0x40) >> 2) | ((value & 0x20) >> 2) | ((value & 0x20) >> 3) | ((value & 0x10) >> 3) | ((value & 0x10) >> 4);
					*(DestVirAddr0 + (EnlargeNumber * i + 1) * pdestSize->u32Width / u32DivNum + EnlargeNumber * j) = *(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j);

					*(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 1) = ((value & 0x8) << 4) | ((value & 0x8) << 3) | ((value & 0x4) << 3) | ((value & 0x4) << 2) | ((value & 0x2) << 2) | ((value & 0x2) << 1) | ((value & 0x1) << 1) | (value & 0x1);
					*(DestVirAddr0 + (EnlargeNumber * i + 1) * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 1) = *(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 1);
				}
			}
		}
		else if (OSD_PIXEL_FORMAT_2BIT == enPixelFormat)
		{
			for (i = 0; i < psrcSize->u32Height; i++)
			{
				for (j = 0; j < (psrcSize->u32Width / u32DivNum); j++)
				{
					value = *(SrcVirAddr + i * psrcSize->u32Width / u32DivNum + j);

					*(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j) = (value & 0xc0) | ((value & 0xc0) >> 2) | ((value & 0x30) >> 2) | ((value & 0x30) >> 4);
					*(DestVirAddr0 + (EnlargeNumber * i + 1) * pdestSize->u32Width / u32DivNum + EnlargeNumber * j) = *(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j);

					*(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 1) = ((value & 0xc) << 4) | ((value & 0xc) << 2) | ((value & 0x3) << 2) | (value & 0x3);
					*(DestVirAddr0 + (EnlargeNumber * i + 1) * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 1) = *(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 1);
				}
			}
		}
		else
		{
			PRINT_ERROR("_osd_enlarge enPixelFormat %d is not support!!!\n", enPixelFormat);
		}
	}
	else if (4 == EnlargeNumber)
	{
		if (OSD_PIXEL_FORMAT_1BIT == enPixelFormat)
		{
			for (i = 0; i < psrcSize->u32Height; i++)
			{
				for (j = 0; j < (psrcSize->u32Width / u32DivNum); j++)
				{
					value = *(SrcVirAddr + i * psrcSize->u32Width / u32DivNum + j);
					*(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j) = (value & 0x80) | ((value & 0x80) >> 1) | ((value & 0x80) >> 2) | ((value & 0x80) >> 3) | ((value & 0x40) >> 3) | ((value & 0x40) >> 4) | ((value & 0x40) >> 5) | ((value & 0x40) >> 6);
					*(DestVirAddr0 + (EnlargeNumber * i + 1) * pdestSize->u32Width / u32DivNum + EnlargeNumber * j) = *(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j);
					*(DestVirAddr0 + (EnlargeNumber * i + 2) * pdestSize->u32Width / u32DivNum + EnlargeNumber * j) = *(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j);
					*(DestVirAddr0 + (EnlargeNumber * i + 3) * pdestSize->u32Width / u32DivNum + EnlargeNumber * j) = *(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j);

					*(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 1) = ((value & 0x20) << 2) | ((value & 0x20) << 1) | ((value & 0x20)) | ((value & 0x20) >> 1) | ((value & 0x10) >> 1) | ((value & 0x10) >> 2) | ((value & 0x10) >> 3) | ((value & 0x10) >> 4);
					*(DestVirAddr0 + (EnlargeNumber * i + 1) * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 1) = *(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 1);
					*(DestVirAddr0 + (EnlargeNumber * i + 2) * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 1) = *(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 1);
					*(DestVirAddr0 + (EnlargeNumber * i + 3) * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 1) = *(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 1);

					*(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 2) = ((value & 0x8) << 4) | ((value & 0x8) << 3) | ((value & 0x8) << 2) | ((value & 0x8) << 1) | ((value & 0x4) << 1) | ((value & 0x4)) | ((value & 0x4) >> 1) | ((value & 0x4) >> 2);
					*(DestVirAddr0 + (EnlargeNumber * i + 1) * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 2) = *(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 2);
					*(DestVirAddr0 + (EnlargeNumber * i + 2) * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 2) = *(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 2);
					*(DestVirAddr0 + (EnlargeNumber * i + 3) * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 2) = *(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 2);

					*(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 3) = ((value & 0x2) << 6) | ((value & 0x2) << 5) | ((value & 0x2) << 4) | ((value & 0x2) << 3) | ((value & 0x1) << 3) | ((value & 0x1) << 2) | ((value & 0x1) << 1) | ((value & 0x1));
					*(DestVirAddr0 + (EnlargeNumber * i + 1) * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 3) = *(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 3);
					*(DestVirAddr0 + (EnlargeNumber * i + 2) * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 3) = *(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 3);
					*(DestVirAddr0 + (EnlargeNumber * i + 3) * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 3) = *(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 3);
				}
			}
		}	
		else if (OSD_PIXEL_FORMAT_2BIT == enPixelFormat)
		{
			for (i = 0; i < psrcSize->u32Height; i++)
			{
				for (j = 0; j < (psrcSize->u32Width / u32DivNum); j++)
				{
					value = *(SrcVirAddr + i * psrcSize->u32Width / u32DivNum + j);
					*(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j) = (value & 0xc0) | ((value & 0xc0) >> 2) | ((value & 0xc0) >> 4) | ((value & 0xc0) >> 6);
					*(DestVirAddr0 + (EnlargeNumber * i + 1) * pdestSize->u32Width / u32DivNum + EnlargeNumber * j) = *(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j);
					*(DestVirAddr0 + (EnlargeNumber * i + 2) * pdestSize->u32Width / u32DivNum + EnlargeNumber * j) = *(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j);
					*(DestVirAddr0 + (EnlargeNumber * i + 3) * pdestSize->u32Width / u32DivNum + EnlargeNumber * j) = *(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j);

					*(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 1) = ((value & 0x30) << 2) | (value & 0x30) | ((value & 0x30) >> 2) | ((value & 0x30) >> 4);
					*(DestVirAddr0 + (EnlargeNumber * i + 1) * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 1) = *(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 1);
					*(DestVirAddr0 + (EnlargeNumber * i + 2) * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 1) = *(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 1);
					*(DestVirAddr0 + (EnlargeNumber * i + 3) * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 1) = *(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 1);

					*(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 2) = ((value & 0xc) << 4) | ((value & 0xc) << 2) | (value & 0xc) | ((value & 0xc) >> 2);
					*(DestVirAddr0 + (EnlargeNumber * i + 1) * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 2) = *(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 2);
					*(DestVirAddr0 + (EnlargeNumber * i + 2) * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 2) = *(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 2);
					*(DestVirAddr0 + (EnlargeNumber * i + 3) * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 2) = *(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 2);

					*(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 3) = ((value & 0x3) << 6) | ((value & 0x3) << 4) | ((value & 0x3) << 2) | (value & 0x3);
					*(DestVirAddr0 + (EnlargeNumber * i + 1) * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 3) = *(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 3);
					*(DestVirAddr0 + (EnlargeNumber * i + 2) * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 3) = *(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 3);
					*(DestVirAddr0 + (EnlargeNumber * i + 3) * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 3) = *(DestVirAddr0 + EnlargeNumber * i * pdestSize->u32Width / u32DivNum + EnlargeNumber * j + 3);
				}
			}
		}
		else
		{
			PRINT_ERROR("_osd_enlarge enPixelFormat %d is not support!!!\n", enPixelFormat);
		}
	}
	else
	{
		PRINT_ERROR("_vsf_drv_enlarge_osd EnlargeNumber:%d error!!!\n", EnlargeNumber);
		return -1;
	}

	return 0;
}

///	点阵字符转换
///	原始点阵为1BIT，根据需求转换成2BIT
/// \param [in] psrcOsd 转换前的数据
/// \param [in] enPixelFormat 需要转换的位数
/// \param [in] psrcSize 原始点阵的大小
/// \param [out] pdestOsd 转换后的数据
/// \retval 0  设置成功。
/// \retval <0  设置失败。
int _osd_bit_switch(unsigned char *psrcOsd, unsigned char *pdestOsd, OSD_PIXEL_FORMAT_E enPixelFormat, SIZE_S *psrcSize)
{
	if (NULL == psrcOsd || NULL == pdestOsd || NULL == psrcSize)
	{
		PRINT_ERROR("_osd_bit_switch error!!!\n");
		return -1;
	}

	int i = 0;
	int j = 0;
	unsigned char value = 0;
	unsigned char *SrcVirAddr = NULL;
	unsigned char *DestVirAddr0 = NULL;

	SrcVirAddr = (NI_U8 *)psrcOsd;
	DestVirAddr0 = (NI_U8 *)pdestOsd;

	if (OSD_PIXEL_FORMAT_BYTE == enPixelFormat)
	{
		PRINT_ERROR("VSF_drvNarrowOsd enPixelFormat %d is not support!!!\n", enPixelFormat);
		return -1;
	}
	else if (OSD_PIXEL_FORMAT_4BIT == enPixelFormat)
	{
		//将字符转换成4bit
		for (i = 0; i < psrcSize->u32Height; i++)
		{
			for (j = 0; j < (psrcSize->u32Width / 8); j++)
			{
				value = *(SrcVirAddr + i * psrcSize->u32Width / 8 + j);
				*(DestVirAddr0 + 4 * (i * psrcSize->u32Width / 8 + j)) = ((value & 0x80) >> 1) | ((value & 0x40) >> 4);

				*(DestVirAddr0 + 4 * (i * psrcSize->u32Width / 8 + j) + 1) = ((value & 0x20) << 1) | ((value & 0x10) >> 2);

				*(DestVirAddr0 + 4 * (i * psrcSize->u32Width / 8 + j) + 2) = ((value & 0x8) << 3) | ((value & 0x4));

				*(DestVirAddr0 + 4 * (i * psrcSize->u32Width / 8 + j) + 3) = ((value & 0x2) << 5) | ((value & 0x1) << 2);
			}
		}
	}
	else if (OSD_PIXEL_FORMAT_2BIT == enPixelFormat)
	{
		//将字符转换成2bit
		for (i = 0; i < psrcSize->u32Height; i++)
		{
			for (j = 0; j < (psrcSize->u32Width / 8); j++)
			{
				value = *(SrcVirAddr + i * psrcSize->u32Width / 8 + j);
				*(DestVirAddr0 + 2 * (i * psrcSize->u32Width / 8 + j)) = ((value & 0x80) >> 1) | ((value & 0x40) >> 2) | ((value & 0x20) >> 3) | ((value & 0x10) >> 4);

				*(DestVirAddr0 + 2 * (i * psrcSize->u32Width / 8 + j) + 1) = ((value & 0x8) << 3) | ((value & 0x4) << 2) | ((value & 0x2) << 1) | (value & 0x1);
			}
		}
	}
	else if (OSD_PIXEL_FORMAT_1BIT == enPixelFormat)
	{
		memcpy(DestVirAddr0, SrcVirAddr, psrcSize->u32Width * psrcSize->u32Height / 8);
	}
	else
	{
		PRINT_ERROR("VSF_drvNarrowOsd unknown enPixelFormat %d!!!\n", enPixelFormat);
		return -1;
	}

	return 0;
}

///	OSD描边
///
/// \param [in] psrcOsd 转换前的数据
/// \param [in] EnlargeNumber 放大倍数
///	16*16与32*32只做1像素描边，64*64做2像素描边
/// \param [in] enPixelFormat 位数
/// \param [in] psrcSize 点阵的大小
/// \retval 0  设置成功。
/// \retval <0  设置失败。
int _osd_add_edge(unsigned char *psrcOsd, unsigned int EnlargeNumber, OSD_PIXEL_FORMAT_E enPixelFormat, SIZE_S *psrcSize)
{
	if (OSD_PIXEL_FORMAT_2BIT != enPixelFormat)
	{
		PRINT_ERROR("_osd_add_edge enPixelFormat %d is not support !!!\n", enPixelFormat);
		return -1;
	}

	if (NULL == psrcOsd || NULL == psrcSize)
	{
		PRINT_ERROR("_osd_add_edge error!!!\n");
		return -1;
	}

	int i = 0;
	int j = 0;
	int m = 0;
	int xl = 0;
	int xr = 0;
	int yu = 0;
	int yd = 0;
	int sum = 0;
	int sumu = 0;
	int sumd = 0;
	int sumr = 0;
	int suml = 0;
	unsigned char s = 0;
	static unsigned char *pdestOsd = NULL;

	//最大内存为32个64*64大小的字
	if(NULL == pdestOsd)
	{
		pdestOsd = malloc(64 * 64 * 32 / 4);
		if(NULL == pdestOsd)
		{
			PRINT_ERROR("_osd_add_edge malloc error!!!\n");
			return -1;
		}
	}
	
	memset(pdestOsd, 0, 64 * 64 * 32 / 4);

	memcpy(pdestOsd, psrcOsd, psrcSize->u32Height * psrcSize->u32Width / 4);

	for (i = 0; i < psrcSize->u32Height; i++)
	{
		for(j = 0; j < psrcSize->u32Width / 4; j++)
		{	
			for(m = 0; m < 4; m++)
			{
				s = *(pdestOsd + i * psrcSize->u32Width / 4 + j);
				sum = s >> 2 * (3 - m) & 0x3;
				if(0 == sum)
				{
					xl = MAX(0, j - 1);
					xr = MIN(psrcSize->u32Width / 4 - 1, j + 1);
					yu = MAX(0, i - 1);
					yd = MIN(psrcSize->u32Height - 1, i + 1);
					if (0 == m)
					{
						sum += s >> 2 * (3 - m - 1) & 0x3;

						if(0 != i)
						{
							s = *(pdestOsd + yu * psrcSize->u32Width / 4 + j);
							sum += s >> 2 * (3 - m) & 0x3;
							sum += s >> 2 * (3 - m - 1) & 0x3;
						}

						if (psrcSize->u32Height - 1 != i)
						{
							s = *(pdestOsd + yd * psrcSize->u32Width / 4 + j);
							sum += s >> 2 * (3 - m) & 0x3;
							sum += s >> 2 * (3 - m - 1) & 0x3;
						}

						if (0 != j)
						{
							s = *(pdestOsd + i * psrcSize->u32Width / 4 + xl);
							sum += s >> 2 * m & 0x3;

							if (0 != i)
							{
								s = *(pdestOsd + yu * psrcSize->u32Width / 4 + xl);
								sum += s >> 2 * m & 0x3;
							}

							if (psrcSize->u32Height - 1 != i)
							{
								s = *(pdestOsd + yd * psrcSize->u32Width / 4 + xl);
								sum += s >> 2 * m & 0x3;
							}
						}
					}
					else if(3 == m)
					{
						sum += s >> 2 * (3 - m + 1) & 0x3;

						if (0 != i)
						{
							s = *(pdestOsd + yu * psrcSize->u32Width / 4 + j);
							sum += s >> 2 * (3 - m) & 0x3;
							sum += s >> 2 * (3 - m + 1) & 0x3;
						}

						if (psrcSize->u32Height - 1 != i)
						{
							s = *(pdestOsd + yd * psrcSize->u32Width / 4 + j);
							sum += s >> 2 * (3 - m) & 0x3;
							sum += s >> 2 * (3 - m + 1) & 0x3;
						}

						if(psrcSize->u32Width / 4 - 1 != j)
						{
							s = *(pdestOsd + i * psrcSize->u32Width / 4 + xr);
							sum += s >> 2 * m & 0x3;

							if (0 != i)
							{
								s = *(pdestOsd + yu * psrcSize->u32Width / 4 + xr);
								sum += s >> 2 * m & 0x3;
							}

							if (psrcSize->u32Height - 1 != i)
							{
								s = *(pdestOsd + yd * psrcSize->u32Width / 4 + xr);
								sum += s >> 2 * m & 0x3;
							}
						}
					}
					else
					{
						sum += s >> 2 * (3 - m + 1) & 0x3;
						sum += s >> 2 * (3 - m - 1) & 0x3;

						if (0 != i)
						{
							s = *(pdestOsd + yu * psrcSize->u32Width / 4 + j);
							sum += s >> 2 * (3 - m) & 0x3;
							sum += s >> 2 * (3 - m + 1) & 0x3;
							sum += s >> 2 * (3 - m - 1) & 0x3;
						}

						if (psrcSize->u32Height - 1 != i)
						{
							s = *(pdestOsd + yd * psrcSize->u32Width / 4 + j);
							sum += s >> 2 * (3 - m) & 0x3;
							sum += s >> 2 * (3 - m + 1) & 0x3;
							sum += s >> 2 * (3 - m - 1) & 0x3;
						}
					}
				}
				else
				{
					continue;
				}

				if(0 != sum)
				{
					*(psrcOsd + i * psrcSize->u32Width / 4 + j) |= (0x2 << 2 * (3 - m));
				}
			}
		}
	}

	if (4 == EnlargeNumber)
	{
		for (i = 0; i < psrcSize->u32Height; i++)
		{
			for (j = 0; j < psrcSize->u32Width / 4; j++)
			{
				for (m = 0; m < 4; m++)
				{
					s = *(psrcOsd + i * psrcSize->u32Width / 4 + j);
					sum = s >> 2 * (3 - m) & 0x3;
					if (0 == sum)
					{
						xl = MAX(0, j - 1);
						xr = MIN(psrcSize->u32Width / 4 - 1, j + 1);
						yu = MAX(0, i - 1);
						yd = MIN(psrcSize->u32Height - 1, i + 1);
						if (0 == m)
						{
							sumr = s >> 2 * (3 - m - 1) & 0x3;

							if (0 != i)
							{
								s = *(psrcOsd + yu * psrcSize->u32Width / 4 + j);
								sumu = s >> 2 * (3 - m) & 0x3;
							}

							if (psrcSize->u32Height - 1 != i)
							{
								s = *(psrcOsd + yd * psrcSize->u32Width / 4 + j);
								sumd = s >> 2 * (3 - m) & 0x3;
							}

							if (0 != j)
							{
								s = *(psrcOsd + i * psrcSize->u32Width / 4 + xl);
								suml = s >> 2 * m & 0x3;
							}
						}
						else if (3 == m)
						{
							suml = s >> 2 * (3 - m + 1) & 0x3;

							if (0 != i)
							{
								s = *(psrcOsd + yu * psrcSize->u32Width / 4 + j);
								sumu = s >> 2 * (3 - m) & 0x3;
							}

							if (psrcSize->u32Height - 1 != i)
							{
								s = *(psrcOsd + yd * psrcSize->u32Width / 4 + j);
								sumd = s >> 2 * (3 - m) & 0x3;
							}

							if (psrcSize->u32Width / 4 - 1 != j)
							{
								s = *(psrcOsd + i * psrcSize->u32Width / 4 + xr);
								sumr = s >> 2 * m & 0x3;
							}
						}
						else
						{
							sumr = s >> 2 * (3 - m - 1) & 0x3;
							suml = s >> 2 * (3 - m + 1) & 0x3;
							
							if (0 != i)
							{
								s = *(psrcOsd + yu * psrcSize->u32Width / 4 + j);
								sumu = s >> 2 * (3 - m) & 0x3;
							}

							if (psrcSize->u32Height - 1 != i)
							{
								s = *(psrcOsd + yd * psrcSize->u32Width / 4 + j);
								sumd = s >> 2 * (3 - m) & 0x3;
							}
						}
					}
					else
					{
						continue;
					}

					if (2 == sumr || 2 == suml || 2 == sumu || 2 == sumd)
					{
						*(psrcOsd + i * psrcSize->u32Width / 4 + j) |= (0x3 << 2 * (3 - m));
					}
				}
			}
		}
	}

	return 0;
}

///	OSD放大
///
/// \param [in] psrcOsd 放大前的数据
/// \param [in] EnlargeNumber 放大倍数
/// \param [in] psrcSize 原始点阵的大小
/// \param [out] pdestOsd 放大后的数据
/// \retval 0  设置成功。
/// \retval <0  设置失败。
int _vsf_drv_enlarge_osd(unsigned char *psrcOsd, unsigned char *pdestOsd, OSD_PIXEL_FORMAT_E enPixelFormat, unsigned int EnlargeNumber, SIZE_S *psrcSize)
{
	if (NULL == psrcOsd || NULL == pdestOsd || 0 == EnlargeNumber || NULL == psrcSize)
	{
		PRINT_ERROR("_vsf_drv_enlarge_osd error!!!\n");
		return -1;
	}

	int ret = -1;
	SIZE_S pdestSize = {0};
	static unsigned char *SrcVirAddr = NULL;

	if (NULL == SrcVirAddr)
	{
		// 目前最大为2BIT，这边先设置为32个16*16字体大小
		SrcVirAddr = malloc(16 * 16 * 32 / 4);
		if (NULL == SrcVirAddr)
		{
			PRINT_ERROR("_vsf_drv_enlarge_osd SrcVirAddr malloc failed!!!\n");
			return -1;
		}
	}

	memset(SrcVirAddr, 0, 16 * 16 * 32 / 4);

	if(0 == psrcSize->u32Height || 0 == psrcSize->u32Width)
	{
		return 0;
	}

	//将1bit的点阵数据转换成2bit的点阵数据
	ret = _osd_bit_switch((NI_U8 *)psrcOsd, SrcVirAddr, enPixelFormat, psrcSize);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("_osd_bit_switch error!!!\n");
		return -1;
	}

	//点阵放大
	ret = _osd_enlarge(SrcVirAddr, (NI_U8 *)pdestOsd, enPixelFormat, EnlargeNumber, psrcSize, &pdestSize);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("_osd_enlarge error!!!\n");
		return -1;
	}

	//描边
	ret = _osd_add_edge((NI_U8 *)pdestOsd, EnlargeNumber, enPixelFormat, &pdestSize);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("_osd_add_edge error!!!\n");
		return -1;
	}

	return 0;
}

int _osd_overlay_calculate(unsigned int ichannel, OSD_TYPE type, SIZE_S OsdSize, unsigned int EnlargeNumber, POINT_S *PostPiont)
{
	int i = 0;
	int x = -1;
	int y = -1;
	int w = -1;
	int ret = -1;
	MDK_CHN_S pstMdkChn;
	int u32OsdHandle = -1;
	OSD_DISP_ATTR_S pstOsdDispAttr;
	SIZE_S picsize[STREAM_TYPE_BUT];
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

	memset(&pstMdkChn, 0, sizeof(MDK_CHN_S));
	memset(&pstOsdDispAttr, 0, sizeof(OSD_DISP_ATTR_S));
	memset(&picsize, 0, sizeof(SIZE_S) * STREAM_TYPE_BUT);

	picsize[ichannel].u32Height = pCaptureDevice->EncDevice[i].StreamDevice[ichannel].EncChannel_info.max_height;

	if (STREAM_TYPE_FIR == ichannel)
	{
		u32OsdHandle = type;
	}
	else if (STREAM_TYPE_SEC == ichannel)
	{
		u32OsdHandle = type + 4;
	}
	else
	{
		PRINT_ERROR("unknown ichannel %d , use STREAM_TYPE_FIR\n", ichannel);
		u32OsdHandle = type;
	}

	pstMdkChn.enModId = NI_ID_VPS;
	pstMdkChn.s32DevId = 0;
	pstMdkChn.s32ChnId = ichannel;

	ret = NI_MDK_OSD_GetDispAttr(u32OsdHandle, &pstMdkChn, &pstOsdDispAttr);
	if(RETURN_OK != ret)
	{
		return SUCCESS;
	}
	
	if(ENABLE == pstOsdDispAttr.bShow)
	{
		//由于原厂在无OSD的时候会下发其他的值，所以这里判断只有使能的时候才往下走，其他情况都不做处理
	}
	else
	{
		return SUCCESS;
	}

	x = pstOsdDispAttr.stStartPiont.s32X;
	y = pstOsdDispAttr.stStartPiont.s32Y;
	w = pstOsdDispAttr.stSize.u32Width;
	
	//OSD单元设置非单元反色模式
	pstOsdDispAttr.enInvertBlkMode = OSD_INVERT_BLK_NULL;
	if (PostPiont->s32X <= x && PostPiont->s32X + OsdSize.u32Width > x)
	{
		if ((PostPiont->s32Y >= y && PostPiont->s32Y < y + OsdSize.u32Height)
		|| (PostPiont->s32Y + OsdSize.u32Height > y && PostPiont->s32Y + OsdSize.u32Height <= y + OsdSize.u32Height))
		{
			switch (type)
			{
				case (OSD_TYPE_TIME):
					PostPiont->s32Y = y + OsdSize.u32Height;
					if ((PostPiont->s32Y + OsdSize.u32Height) > picsize[ichannel].u32Height)
					{
						PostPiont->s32Y = picsize[ichannel].u32Height - OsdSize.u32Height;
						pstOsdDispAttr.stStartPiont.s32Y = PostPiont->s32Y - OsdSize.u32Height;
					}
					break;
				case (OSD_TYPE_TITLE):
					if(y < OsdSize.u32Height)
					{
						pstOsdDispAttr.stStartPiont.s32Y = PostPiont->s32Y + OsdSize.u32Height;
					}
					else
					{
						PostPiont->s32Y = y - OsdSize.u32Height;
					}
					break;
				case (OSD_TYPE_LABLE1):
					PostPiont->s32Y = y + OsdSize.u32Height;
					if ((PostPiont->s32Y + OsdSize.u32Height) > picsize[ichannel].u32Height)
					{
						PostPiont->s32Y = picsize[ichannel].u32Height - OsdSize.u32Height;
						pstOsdDispAttr.stStartPiont.s32Y = PostPiont->s32Y - OsdSize.u32Height;
					}
					break;
				case (OSD_TYPE_LABLE2):
					if (y < OsdSize.u32Height)
					{
						pstOsdDispAttr.stStartPiont.s32Y = PostPiont->s32Y + OsdSize.u32Height;
					}
					else
					{
						PostPiont->s32Y = y - OsdSize.u32Height;
					}
					break;
				default:
					PRINT_ERROR("_osd_overlay_calculate unknwon type %d\n", type);
					break;
			}

			ret = NI_MDK_OSD_PaintToChn(u32OsdHandle, &pstMdkChn, &pstOsdDispAttr, 500);
			if (RETURN_OK != ret)
			{
				_osd_error(&pstMdkChn, &pstOsdDispAttr);
				PRINT_ERROR("_osd_overlay_calculate u32OsdHandle %d NI_MDK_OSD_PaintToChn failed with %x\n", u32OsdHandle, ret);
			}
		}
		else
		{
			// do nothing
		}
	}
	else if (PostPiont->s32X >= x && PostPiont->s32X < x + w)
	{
		if ((PostPiont->s32Y >= y && PostPiont->s32Y < y + OsdSize.u32Height) 
		|| (PostPiont->s32Y + OsdSize.u32Height > y && PostPiont->s32Y + OsdSize.u32Height <= y + OsdSize.u32Height))
		{
			switch (type)
			{
				case (OSD_TYPE_TIME):
					PostPiont->s32Y = y + OsdSize.u32Height;
					if ((PostPiont->s32Y + OsdSize.u32Height) > picsize[ichannel].u32Height)
					{
						PostPiont->s32Y = picsize[ichannel].u32Height - OsdSize.u32Height;
						pstOsdDispAttr.stStartPiont.s32Y = PostPiont->s32Y - OsdSize.u32Height;
					}
					break;
				case (OSD_TYPE_TITLE):
					if (y < OsdSize.u32Height)
					{
						pstOsdDispAttr.stStartPiont.s32Y = PostPiont->s32Y + OsdSize.u32Height;
					}
					else
					{
						PostPiont->s32Y = y - OsdSize.u32Height;
					}
					break;
				case (OSD_TYPE_LABLE1):
					PostPiont->s32Y = y + OsdSize.u32Height;
					if ((PostPiont->s32Y + OsdSize.u32Height) > picsize[ichannel].u32Height)
					{
						PostPiont->s32Y = picsize[ichannel].u32Height - OsdSize.u32Height;
						pstOsdDispAttr.stStartPiont.s32Y = PostPiont->s32Y - OsdSize.u32Height;
					}
					break;
				case (OSD_TYPE_LABLE2):
					if (y < OsdSize.u32Height)
					{
						pstOsdDispAttr.stStartPiont.s32Y = PostPiont->s32Y + OsdSize.u32Height;
					}
					else
					{
						PostPiont->s32Y = y - OsdSize.u32Height;
					}
					break;
				default:
					PRINT_ERROR("_osd_overlay_calculate unknwon type %d\n", type);
					break;
			}

			ret = NI_MDK_OSD_PaintToChn(u32OsdHandle, &pstMdkChn, &pstOsdDispAttr, 500);
			if (RETURN_OK != ret)
			{
				_osd_error(&pstMdkChn, &pstOsdDispAttr);
				PRINT_ERROR("_osd_overlay_calculate u32OsdHandle %d NI_MDK_OSD_PaintToChn failed with %x\n", u32OsdHandle, ret);
			}
		}
		else
		{
			// do nothing
		}
	}
	else
	{
		//do nothing
	}

	return SUCCESS;
}

int _osd_coord_calculate(unsigned int ichannel, unsigned int u32OsdHandle, unsigned int EnlargeNumber, SIZE_S OsdSize, POINT_S PrePiont, POINT_S *PostPiont)
{
	if(NULL == PostPiont)
	{
		PRINT_ERROR("_osd_coord_calculate error!!!\n");
		return -1;
	}

	int i = 0;
	int ret = -1;
	int type = -1;
	int vstd = VIDEO_STANDARD_PAL;
	SIZE_S picsize[STREAM_TYPE_BUT];
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
	unsigned int cif_width = imageSize[vstd][VIDEO_SIZE_CIF].u32Width; /* 上层传递的坐标为在CIF下的坐标 */
	unsigned int cif_height = imageSize[vstd][VIDEO_SIZE_CIF].u32Height;

	memset(&picsize, 0, sizeof(SIZE_S) * STREAM_TYPE_BUT);

	picsize[ichannel].u32Width = pCaptureDevice->EncDevice[i].StreamDevice[ichannel].EncChannel_info.max_width;
	picsize[ichannel].u32Height = pCaptureDevice->EncDevice[i].StreamDevice[ichannel].EncChannel_info.max_height;
	PostPiont->s32X = (int)((float)PrePiont.s32X * picsize[ichannel].u32Width / cif_width);
	PostPiont->s32Y = (int)((float)PrePiont.s32Y * picsize[ichannel].u32Height / cif_height);

	type = u32OsdHandle % 4;

	if (PostPiont->s32X + OsdSize.u32Width > picsize[ichannel].u32Width)
	{
		PostPiont->s32X = picsize[ichannel].u32Width - OsdSize.u32Width;
	}
	if (PostPiont->s32Y + OsdSize.u32Height > picsize[ichannel].u32Height)
	{
		PostPiont->s32Y = picsize[ichannel].u32Height - OsdSize.u32Height;
	}

	if (PostPiont->s32X % 2)
	{
		PostPiont->s32X--;
	}

	if (PostPiont->s32Y % 2)
	{
		PostPiont->s32Y--;
	}

	if (OSD_TYPE_TIME == type)
	{
		ret = _osd_overlay_calculate(ichannel, OSD_TYPE_TITLE, OsdSize, EnlargeNumber, PostPiont);
		if(RETURN_OK != ret )
		{
			PRINT_ERROR("_osd_overlay_calculate failed !!!\n");
			return FAILED;
		}
	}
	else if (OSD_TYPE_TITLE == type)
	{
		ret = _osd_overlay_calculate(ichannel, OSD_TYPE_TIME, OsdSize, EnlargeNumber, PostPiont);
		if (RETURN_OK != ret)
		{
			PRINT_ERROR("_osd_overlay_calculate failed !!!\n");
			return FAILED;
		}
	}
	else if (OSD_TYPE_LABLE1 == type)
	{
		ret = _osd_overlay_calculate(ichannel, OSD_TYPE_LABLE2, OsdSize, EnlargeNumber, PostPiont);
		if (RETURN_OK != ret)
		{
			PRINT_ERROR("_osd_overlay_calculate failed !!!\n");
			return FAILED;
		}
	}
	else if (OSD_TYPE_LABLE2 == type)
	{
		ret = _osd_overlay_calculate(ichannel, OSD_TYPE_LABLE1, OsdSize, EnlargeNumber, PostPiont);
		if (RETURN_OK != ret)
		{
			PRINT_ERROR("_osd_overlay_calculate failed !!!\n");
			return FAILED;
		}
	}
	else
	{
		PRINT_ERROR("_osd_coord_calculate unknow type %d!!!\n", type);
		return -1;
	}

	return SUCCESS;
}

static int _get_pic_size(video_size_t enPicSize, SIZE_S* pstSize)
{
	if (enPicSize >= VIDEO_SIZE_NR || enPicSize < 0)
	{
		return FAILED;
	}

	VideoInDevice_p pVideoInDevice = &GlobalDevice.VideoInDevice;
	int dstFps = pVideoInDevice->VDInfo[0].ViDstFps;
	int vstd = VIDEO_STANDARD_PAL;
	if(25 == dstFps)
	{
		vstd = VIDEO_STANDARD_PAL;
	}
	else if(30 == dstFps)
	{
		vstd = VIDEO_STANDARD_NTSC;
	}

	pstSize->u32Width = imageSize[vstd][enPicSize].u32Width;
	pstSize->u32Height = imageSize[vstd][enPicSize].u32Height;
	if(pstSize->u32Width == 0 && pstSize->u32Height == 0)
	{
		return FAILED;
	}
	return SUCCESS;
}

int _get_pic_type(VIDEO_FORMAT * pFormat,struct channel_info *info)
{
	switch(pFormat->Compression)
	{
		case VIDEO_COMP_H264:
		{
			info->enc_type = PT_H264;
			info->profile = H264Profile_Table[pFormat->profile];
			switch(pFormat->BitRateControl)
			{
				case VIDEO_BRC_CBR:
					info->rc_type = VENC_RC_MODE_CBR;
					info->bps = info->bps*4/5;
					break;
				case VIDEO_BRC_VBR:
					info->rc_type = VENC_RC_MODE_QVBR;
					info->qt_level = pFormat->ImageQuality;
					break;
				default:
					PRINT_ERROR("H264 enRcMode (%d) not support!\n",pFormat->BitRateControl);
					return -1;
			}
			break;
		}
		
		case VIDEO_COMP_H265:
		{
			info->enc_type = PT_H265;
			info->profile = H265Profile_Table[pFormat->profile];
			switch(pFormat->BitRateControl)
			{
				case VIDEO_BRC_CBR:
					info->rc_type = VENC_RC_MODE_CBR;
					info->bps = info->bps*4/5;
					break;
				case VIDEO_BRC_VBR:
					info->rc_type = VENC_RC_MODE_QVBR;
					info->qt_level = pFormat->ImageQuality;
					break;
				default:
					PRINT_ERROR("H265 enRcMode (%d) not support!\n",pFormat->BitRateControl);
					return -1;
			}
			break;
		}
		
		case VIDEO_COMP_MJPG:
		{
			//info->enc_type = FH_MJPEG;
			switch(pFormat->BitRateControl)
			{
				case VIDEO_BRC_CBR:
					//info->rc_type = FH_RC_MJPEG_CBR;
					break;
				case VIDEO_BRC_VBR:
					//info->rc_type = FH_RC_MJPEG_VBR;
					break;
				default:
					PRINT_ERROR("MJPEG enRcMode (%d) not support!\n",pFormat->BitRateControl);
					return -1;
			}
			break;
		}

		case VIDEO_COMP_JPEG:
			//info->enc_type = FH_JPEG;
			break;
		default:
			PRINT_ERROR("enType (%d) not support!\n",pFormat->Compression);
			return -1;	
	}

	return 0;
}

int _set_title()
{

	int ret = -1;
	VIDEO_TITLE_PARAM pParam;
	RegionDevice_p pRegionDevice = &GlobalDevice.RegionDevice;
	memset(&pParam, 0, sizeof(VIDEO_TITLE_PARAM));

	osd_create();

	pthread_mutex_unlock(&osd_lock);

	for (int i = 0; i < TD_OSD_MAX_NUM; i++)
	{
		pParam.index = i;
		pParam.width = pRegionDevice->OsdWidth[i];
		pParam.height = 16;
		pParam.x = pRegionDevice->PositionX[i];
		pParam.y = pRegionDevice->PositionY[i];
		pParam.fg_color = pRegionDevice->FgColor;
		pParam.bg_color = pRegionDevice->BgColor;
		pParam.enable = pRegionDevice->show[i];
		pParam.AutoBlackWhite = pRegionDevice->InvertEnable;
		pParam.raster = (unsigned char *)pRegionDevice->channeltitle[i];
		ret |= VideoSetTitle(0, &pParam);
	}

	return ret;
}

int _set_cover()
{
	int i = 0;
	int j = 0;
	int ret = -1;
	int channel = 0;
	VPS_GRP VpsGrp = 0;
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int w = 0;
	unsigned int h = 0;
	VPS_MASK_ATTR_S pstMaskAttr;
	SIZE_S picsize[STREAM_TYPE_BUT];
	RegionDevice_p pRegionDevice = &GlobalDevice.RegionDevice;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

	memset(&pstMaskAttr, 0, sizeof(VPS_MASK_ATTR_S));
	memset(&picsize, 0, sizeof(SIZE_S) * STREAM_TYPE_BUT);

	int vstd = VIDEO_STANDARD_PAL;
	unsigned int cif_width = imageSize[vstd][VIDEO_SIZE_CIF].u32Width; /* 上层传递的坐标为在CIF下的坐标 */
	unsigned int cif_height = imageSize[vstd][VIDEO_SIZE_CIF].u32Height;

	for (i = 0; i < pCaptureDevice->EncCount; i++)
	{
		for (j = 0; j < pCaptureDevice->EncDevice[i].StreamCount; j++)
		{
			picsize[j].u32Width = pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.max_width;
			picsize[j].u32Height = pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.max_height;
		}
	}

	for (int ichannel = STREAM_TYPE_FIR; ichannel < pCaptureDevice->EncDevice[channel].StreamCount; ichannel++)
	{
		if (STREAM_TYPE_THI == ichannel)
		{
			break;
		}

		ret = NI_MDK_VPS_GetChnMaskAttr(VpsGrp, ichannel, &pstMaskAttr);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("NI_MDK_VPS_GetChnMaskAttr failed with %x\n", ret);
			return ret;
		}

		for (i = 0; i < VPS_MASK_MAX_CHN_NUM; i++)
		{
			x = pRegionDevice->COVER_PARAM[i].rect.left;
			y = pRegionDevice->COVER_PARAM[i].rect.top;
			w = pRegionDevice->COVER_PARAM[i].rect.right - pRegionDevice->COVER_PARAM[i].rect.left;
			h = pRegionDevice->COVER_PARAM[i].rect.bottom - pRegionDevice->COVER_PARAM[i].rect.top;

			pstMaskAttr.stMaskColor.u8Y = 0;
			pstMaskAttr.stMaskColor.u8U = 128;
			pstMaskAttr.stMaskColor.u8V = 128;
			pstMaskAttr.astMaskRect[i].s32X = (int)((float)x * picsize[ichannel].u32Width / cif_width);		 /* mask左上角起始点像素宽度位置 */
			pstMaskAttr.astMaskRect[i].s32Y = (int)((float)y * picsize[ichannel].u32Height / cif_height);		 /* mask左上角起始点像素高度位置 */
			pstMaskAttr.astMaskRect[i].u32Width = (int)((float)w * picsize[ichannel].u32Width / cif_width);	 /* mask宽度 */
			pstMaskAttr.astMaskRect[i].u32Height = (int)((float)h * picsize[ichannel].u32Height / cif_height); /* mask高度 */

			if (pstMaskAttr.astMaskRect[i].s32X % 2)
			{
				pstMaskAttr.astMaskRect[i].s32X--;
			}
			if (pstMaskAttr.astMaskRect[i].s32Y % 2)
			{
				pstMaskAttr.astMaskRect[i].s32Y--;
			}
			if (pstMaskAttr.astMaskRect[i].u32Width % 2)
			{
				pstMaskAttr.astMaskRect[i].u32Width--;
			}
			if (pstMaskAttr.astMaskRect[i].u32Height % 2)
			{
				pstMaskAttr.astMaskRect[i].u32Height--;
			}
			if (0 == pstMaskAttr.astMaskRect[i].u32Width)
			{
				pstMaskAttr.astMaskRect[i].u32Width = 2;
			}
			if (0 == pstMaskAttr.astMaskRect[i].u32Height)
			{
				pstMaskAttr.astMaskRect[i].u32Height = 2;
			}
		}

		ret = NI_MDK_VPS_SetChnMaskAttr(VpsGrp, ichannel, &pstMaskAttr);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("NI_MDK_VPS_SetChnMaskAttr failed with %x\n", ret);
			return ret;
		}
	}

	return ret;
}

int _set_logo()
{
	int ret = -1;
	int channel = 0;
	VIDEO_TITLE_PARAM pParam;
	RegionDevice_p pRegionDevice = &GlobalDevice.RegionDevice;
	memset(&pParam, 0, sizeof(VIDEO_TITLE_PARAM));

	if (ENABLE != pRegionDevice->LogoEnable)
	{
		return SUCCESS;
	}

	pParam.enable = pRegionDevice->LogoEnable;
	pParam.x = pRegionDevice->LogoX;
	pParam.y = pRegionDevice->LogoY;
	pParam.fg_color = pRegionDevice->FgColor;

	ret = VideoSetLogo(channel, &pParam);

	return ret;
}

/// 设置VPS输出的帧率和分辨率
///
/// \param [in] channel 通道号。
/// \param [in] dwType 编码通道的掩码
/// \param [in] ChnAttr 修改VPS输出帧率和分辨率的结构体。
/// \retval 0  设置成功
/// \retval 0  设置失败
int _video_set_vps_out_param(int channel, DWORD dwType, VPS_CHN_OUT_ATTR *ChnAttr)
{
	int EncCnt = 0;
	int ret = FAILED;
	VPS_GRP VpsGrp = 0;
	unsigned int VencChn = 0;
	VPS_GRP_ATTR_S VPS_GrpAttr;
	VPS_CHN_ATTR_S VPS_ChnAttr;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

	memset(&VPS_GrpAttr,0,sizeof(VPS_GRP_ATTR_S));
	memset(&VPS_ChnAttr,0,sizeof(VPS_CHN_ATTR_S));
	
	//视频输入的数量
	EncCnt = pCaptureDevice->EncCount;

	if (NULL == ChnAttr || channel >= EncCnt)
	{
		PRINT_ERROR("channel %d parameter error\n", channel);
		return ret;
	}

	if(0 == ChnAttr->OutFps)
	{
		PRINT_ERROR("channel %d parameter OutFps %d error\n", channel, ChnAttr->OutFps);
		return ret;
	}

	//校验支持的码流的通道，上层传递通道方式为掩码
	if (!((pCaptureDevice->EncDevice[channel].SupportStream >> dwType) & 0x01))
	{
		PRINT_ERROR("not support dwType = %d\n", dwType);
		return ret;
	}

	//获取编码通道号
	for (int i = 0; i < pCaptureDevice->EncDevice[channel].StreamCount; i++)
	{
		if (dwType == pCaptureDevice->EncDevice[channel].StreamDevice[i].CapChn)
		{
			VencChn = i;
		}
	}

	ret = NI_MDK_VPS_DisableChn(VpsGrp, VencChn);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error(%d - %x): NI_MDK_VPS_DisableChn\n", ret, ret);
		return ret;
	}

	// 当VPS通道分辨率改变时，OSD和LOGO坐标需要根据分辨率来调整，所以这边先将OSD和LOGO销毁
	// 由于设计到分辨率配置的问题，销毁后需等到编码修改完成后再创建
	if (ChnAttr->OutHeight != pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.max_height ||
		ChnAttr->OutWidth != pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.max_width)
	{
		osd_destroy();
		logo_destroy();
	}
	
	ret = NI_MDK_VPS_GetChnAttr(VpsGrp, VencChn, &VPS_ChnAttr);
	if (ret != RETURN_OK)
	{
		pthread_mutex_unlock(&osd_lock);
		PRINT_ERROR("Error(%d - %x): NI_MDK_VPS_GetChnAttr\n", ret, ret);
		return ret;
	}

	VPS_ChnAttr.u32OutHeight = ChnAttr->OutHeight;
	VPS_ChnAttr.u32OutWidth = ChnAttr->OutWidth;
	VPS_ChnAttr.stEncAttr.u32YStride = VPS_ChnAttr.u32OutWidth;
	VPS_ChnAttr.stEncAttr.u32CStride = VPS_ChnAttr.u32OutWidth;
	VPS_ChnAttr.stEncAttr.enType = ChnAttr->EnPayLoad;

	// 这边判断下sensor帧率和用户配置帧率的大小
	// 若sensor帧率大于用户配置帧率，则以用户配置帧率为准
	// 若sensor帧率小于用户配置帧率，则以sensor帧率为准
	VPS_ChnAttr.stEncAttr.u32OutFps = (sensor_fps > ChnAttr->OutFps) ? ChnAttr->OutFps : sensor_fps;

	// 为保证降帧后帧率为整数，输出帧率的分母值为2，所以这边的输出帧率需要*2
	if (12 == sensor_fps)
	{
		VPS_ChnAttr.stEncAttr.u32OutFps *= 2;
	}
	
	ret = NI_MDK_VPS_SetChnAttr(VpsGrp, VencChn, &VPS_ChnAttr);
	if (ret != RETURN_OK)
	{
		pthread_mutex_unlock(&osd_lock);
		PRINT_ERROR("Error(%d - %x): NI_MDK_VPS_SetChnAttr\n", ret, ret);
		return ret;
	}

	ret = NI_MDK_VPS_EnableChn(VpsGrp, VencChn);
	if (ret != RETURN_OK)
	{
		pthread_mutex_unlock(&osd_lock);
		PRINT_ERROR("Error(%d - %x): NI_MDK_VPS_EnableChn\n", ret, ret);
		return ret;
	}

	return ret;
}


/// 设置VPS输入帧率
///	
/// \param [in] channel 通道号。
/// \param [in] FrmRate 输入的帧率。
/// \retval 0  设置成功
/// \retval 0  设置失败
int _video_set_vps_in_frame_rate(int channel, unsigned int FrmRate)
{
	int ret = FAILED;
	VPS_GRP VpsGrp = 0;
	VPS_GRP_ATTR_S stGrpAttr;
	VPS_MOD_PARAM_S stGrpModParam;

	memset(&stGrpAttr, 0, sizeof(VPS_GRP_ATTR_S));
	memset(&stGrpModParam, 0, sizeof(VPS_MOD_PARAM_S));

	if (0 == FrmRate)
	{
		PRINT_ERROR("_video_set_vps_in_frame_rate FrmRate:%d error!\n", FrmRate);
		return ret;
	}

	ret = NI_MDK_VPS_GetModParam(VpsGrp, &stGrpModParam);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_VPS_GetGrpAttr failed with %#x!\n", ret);
		return ret;
	}

	if (12 == FrmRate)
	{
		// 在PAL制下，sensor帧率为25帧，降帧后帧率为12.5
		//为保证降帧后帧率为整数12，这边将输出帧率的分母值配为2，输出帧率的分子值设置为24
		stGrpModParam.u32FpsDenom = 2;
	}
	else
	{
		stGrpModParam.u32FpsDenom = 1;
	}

	ret = NI_MDK_VPS_SetModParam(VpsGrp, &stGrpModParam);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_VPS_SetModParam failed with %#x!\n", ret);
		return ret;
	}

	ret = NI_MDK_VPS_GetGrpAttr(VpsGrp, &stGrpAttr);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_VPS_GetGrpAttr failed with %#x!\n", ret);
		return ret;
	}

	if (12 == FrmRate)
	{
		stGrpAttr.u32InFps = 25;
	}
	else
	{
		stGrpAttr.u32InFps = FrmRate;
	}

	ret = NI_MDK_VPS_SetGrpAttr(VpsGrp, &stGrpAttr);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_VPS_SetGrpAttr failed with %#x!\n", ret);
		return ret;
	}

	return ret;
}

/// 设置编码的帧率
///	仅限降帧使用
/// \param [in] channel 通道号。
/// \param [in] dwType
/// \param [in] pFormat 指向捕获格式结构CAPTURE_FORMAT的指针。
/// \retval 0  设置成功
/// \retval 0  设置失败
int _video_set_venc_param(int channel, DWORD dwType, channel_info *info)
{
	int EncCnt = 0;
	int ret = FALSE;
	int jpeg_fps = 4;				//设置抓图通道的帧率为4
	unsigned int VencChn = 0;
	unsigned int MaxBitRate = 0;
	BOOL VencVhnDestroy = FALSE;
	VENC_CHN_ATTR_S VENC_ChnAttr;
	int jpeg_chn = STREAM_TYPE_THI; // 抓图为第三通道
	SensorDevice_p pSensorDevice = &GlobalDevice.SensorDevice;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

	memset(&VENC_ChnAttr,0,sizeof(VENC_CHN_ATTR_S));

	EncCnt = pCaptureDevice->EncCount;

	if (NULL == info || channel >= EncCnt)
	{
		pthread_mutex_unlock(&osd_lock);
		PRINT_ERROR("channel %d parameter error\n", channel);
		return ret;
	}

	if (0 == info->frame_count)
	{
		pthread_mutex_unlock(&osd_lock);
		PRINT_ERROR("channel %d parameter FramesPerSecond %d error\n", channel, info->frame_count);
		return ret;
	}

	// 校验支持的码流的通道，上层传递通道方式为掩码
	if (!((pCaptureDevice->EncDevice[channel].SupportStream >> dwType) & 0x01))
	{
		pthread_mutex_unlock(&osd_lock);
		PRINT_ERROR("not support dwType = %d\n", dwType);
		return ret;
	}

	// 获取编码通道号
	for (int i = 0; i < pCaptureDevice->EncDevice[channel].StreamCount; i++)
	{
		if (dwType == pCaptureDevice->EncDevice[channel].StreamDevice[i].CapChn)
		{
			VencChn = i;
		}
	}

	ret = NI_MDK_VENC_GetChnAttr(VencChn, &VENC_ChnAttr);
	if (ret != RETURN_OK)
	{
		pthread_mutex_unlock(&osd_lock);
		PRINT_ERROR("Error(%d - %x): NI_MDK_VENC_GetChnAttr\n", ret, ret);
		return -1;
	}

	//仅当编码类型和编码等级发生修改时，需要销毁编码通道，其他情况均不销毁
	if (VENC_ChnAttr.stVeAttr.enType != info->enc_type || VENC_ChnAttr.stVeAttr.enProfile != info->profile)
	{
		ret = NI_MDK_VENC_DestroyChn(VencChn);
		if (ret != RETURN_OK)
		{
			pthread_mutex_unlock(&osd_lock);
			PRINT_ERROR("Error(%d - %x): NI_MDK_VENC_DestroyChn\n", ret, ret);
			return -1;
		}

		//关闭码流校验功能
		ret = NI_MDK_VENC_SetStreamCheck(VencChn, VENC_STREAM_CHECK_OFF);
		if (ret != RETURN_OK)
		{
			pthread_mutex_unlock(&osd_lock);
			PRINT_ERROR("Error(%d - %x): NI_MDK_VENC_SetStreamCheck\n", ret, ret);
			return -1;
		}

		// 设置编码通道的buff大小，仅在销毁重建时才能修改
		//这边直接将编码通道的buff大小设置为最大，可以规避小分辨率切换到大分辨率需要销毁重建的问题
		if (STREAM_TYPE_SEC == VencChn)
		{
			VENC_ChnAttr.stVeAttr.u32StrmBufSize = 245761;
		}
		else
		{
			VENC_ChnAttr.stVeAttr.u32StrmBufSize = pSensorDevice->MaxHeight * pSensorDevice->MaxWidth / 2;
		}

		VencVhnDestroy = TRUE;
	}


	VENC_ChnAttr.stVeAttr.enType = info->enc_type;
	VENC_ChnAttr.stVeAttr.enProfile = info->profile;
	VENC_ChnAttr.stVeAttr.stInputPicAttr.u32PicWidth = info->width;
	VENC_ChnAttr.stVeAttr.stInputPicAttr.u32PicHeight = info->height;
	VENC_ChnAttr.stRcAttr.enRcMode = info->rc_type;
	// 这边判断下sensor帧率和用户配置帧率的大小
	// 若sensor帧率大于用户配置帧率，则以用户配置帧率为准
	// 若sensor帧率小于用户配置帧率，则以sensor帧率为准
	VENC_ChnAttr.stRcAttr.u32FrmRateNum = (sensor_fps > info->frame_count) ? info->frame_count : sensor_fps;

	VENC_ChnAttr.stGopAttr.stGopNomalAttr.u32Gop = MAX(VENC_ChnAttr.stRcAttr.u32FrmRateNum, 1) * 2;
	VENC_ChnAttr.stVeAttr.stInputPicAttr.au32Stride[0] = info->width;
	VENC_ChnAttr.stVeAttr.stInputPicAttr.au32Stride[1] = info->width;
	VENC_ChnAttr.stVeAttr.stInputPicAttr.au32Stride[2] = 0;
	MaxBitRate = VENC_ChnAttr.stRcAttr.u32FrmRateNum / VENC_ChnAttr.stRcAttr.u32FrmRateDenom * info->width * info->height * 8 * 3 / 2000;

	switch (VENC_ChnAttr.stRcAttr.enRcMode)
	{
		case (VENC_RC_MODE_CBR):
			//此处将统计时间设置为10，为A-CBR模式，可以改善画面较为复杂情况下的画质
			VENC_ChnAttr.stRcAttr.stAttrCbr.u32StatTime = 1;
			VENC_ChnAttr.stRcAttr.stAttrCbr.u32TargetBitRate = MIN(info->bps, MaxBitRate);
			//获取主子码流的QP值
			if (VencChn == CHL_MAIN_T)
			{
				VENC_ChnAttr.stRcAttr.stAttrCbr.u32MinIQp = CaptureQtTable[info->qt_level - 1].IminQP;
				VENC_ChnAttr.stRcAttr.stAttrCbr.u32MaxIQp = CaptureQtTable[info->qt_level - 1].ImaxQP;
				VENC_ChnAttr.stRcAttr.stAttrCbr.u32MinPQp = CaptureQtTable[info->qt_level - 1].PminQP;
				VENC_ChnAttr.stRcAttr.stAttrCbr.u32MaxPQp = CaptureQtTable[info->qt_level - 1].PmaxQP;
			}
			else if (VencChn == CHL_2END_T)
			{
				VENC_ChnAttr.stRcAttr.stAttrCbr.u32MinIQp = CHL_2END_T_CaptureQtTable[info->qt_level - 1].IminQP;
				VENC_ChnAttr.stRcAttr.stAttrCbr.u32MaxIQp = CHL_2END_T_CaptureQtTable[info->qt_level - 1].ImaxQP;
				VENC_ChnAttr.stRcAttr.stAttrCbr.u32MinPQp = CHL_2END_T_CaptureQtTable[info->qt_level - 1].PminQP;
				VENC_ChnAttr.stRcAttr.stAttrCbr.u32MaxPQp = CHL_2END_T_CaptureQtTable[info->qt_level - 1].PmaxQP;
			}
			break;
		case (VENC_RC_MODE_QVBR):
			// 此处必须设置统计时间为9，为Q-VBR模式，为了降低静止场景下的码率
			VENC_ChnAttr.stRcAttr.stAttrQVbr.u32StatTime = 1;
			VENC_ChnAttr.stRcAttr.stAttrQVbr.u32MaxBitRate = MIN(info->bps, MaxBitRate);
			VENC_ChnAttr.stRcAttr.stAttrQVbr.s32ChangePos = 85;
			//获取主子码流的QP值
			if (VencChn == CHL_MAIN_T)
			{
				VENC_ChnAttr.stRcAttr.stAttrQVbr.u32MinIQp = CaptureQtTable[info->qt_level - 1].IminQP;
				VENC_ChnAttr.stRcAttr.stAttrQVbr.u32MaxIQp = CaptureQtTable[info->qt_level - 1].ImaxQP;
				VENC_ChnAttr.stRcAttr.stAttrQVbr.u32MinPQp = CaptureQtTable[info->qt_level - 1].PminQP;
				VENC_ChnAttr.stRcAttr.stAttrQVbr.u32MaxPQp = CaptureQtTable[info->qt_level - 1].PmaxQP;
			}
			else if (VencChn == CHL_2END_T)
			{
				VENC_ChnAttr.stRcAttr.stAttrQVbr.u32MinIQp = CHL_2END_T_CaptureQtTable[info->qt_level - 1].IminQP;
				VENC_ChnAttr.stRcAttr.stAttrQVbr.u32MaxIQp = CHL_2END_T_CaptureQtTable[info->qt_level - 1].ImaxQP;
				VENC_ChnAttr.stRcAttr.stAttrQVbr.u32MinPQp = CHL_2END_T_CaptureQtTable[info->qt_level - 1].PminQP;
				VENC_ChnAttr.stRcAttr.stAttrQVbr.u32MaxPQp = CHL_2END_T_CaptureQtTable[info->qt_level - 1].PmaxQP;
			}
			VENC_ChnAttr.stRcAttr.stAttrQVbr.s32MinBitratePos 		= 32;
			if (PT_H265 == VENC_ChnAttr.stVeAttr.enType)
			{
				VENC_ChnAttr.stRcAttr.stAttrQVbr.s32BestQuality  = 50;
				VENC_ChnAttr.stRcAttr.stAttrQVbr.s32WorstQuality = 60;
			}
			else
			{
				VENC_ChnAttr.stRcAttr.stAttrQVbr.s32BestQuality 	= 60;
				VENC_ChnAttr.stRcAttr.stAttrQVbr.s32WorstQuality 	= 70;
			}
			break;
		default:
			PRINT_ERROR("unknown enRcMode %d\n", VENC_ChnAttr.stRcAttr.enRcMode);
			return -1;
	}

	//根据码流是否被销毁来创建/设置编码通道
	if(TRUE == VencVhnDestroy)
	{
		ret = NI_MDK_VENC_CreateChn(VencChn, &VENC_ChnAttr);
		if (ret != RETURN_OK)
		{
			pthread_mutex_unlock(&osd_lock);
			PRINT_ERROR("Error(%d - %x): channel: %d NI_MDK_VENC_CreateChn\n", ret, ret, VencChn);
			_venc_error(&VENC_ChnAttr);
			return -1;
		}
	}
	else
	{
		ret = NI_MDK_VENC_SetChnAttr(VencChn, &VENC_ChnAttr);
		if (ret != RETURN_OK)
		{
			pthread_mutex_unlock(&osd_lock);
			PRINT_ERROR("Error(%d - %x): NI_MDK_VENC_SetChnAttr\n", ret, ret);
			return -1;
		}
	}

	//当分辨率发生改变时，OSD和LOGO被销毁了，在这边重新创建
	if (info->width != pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.max_width ||
		info->height != pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.max_height)
	{
		//这边先保存分辨率配置，设置OSD、LOGO、隐私遮蔽需要使用该参数
		pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.max_width = info->width;
		pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.max_height = info->height;

		_set_title();
		_set_cover();
		_set_logo();
	}


	if (STREAM_TYPE_FIR == VencChn)
	{
		VENC_RC_PARAM_S stRcParam;
		memset(&stRcParam, 0, sizeof(VENC_RC_PARAM_S));

		ret = NI_MDK_VENC_GetRcParam(VencChn, &stRcParam);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("Error(%d - %x): NI_MDK_VENC_GetRcParam\n", ret, ret);
			return ret;
		}

		stRcParam.stRcParamH26x.s32MaxISize = 512 * 1024;

		ret = NI_MDK_VENC_SetRcParam(VencChn, &stRcParam);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("Error(%d - %x): NI_MDK_VENC_SetRcParam\n", ret, ret);
			return ret;
		}

		PRINT_INFO("set main stream max I frame size 512KB\n");
	}

	//保存配置
	pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.bps = info->bps;
	pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.rc_type = info->rc_type;
	pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.profile = info->profile;
	pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.qt_level = info->qt_level;
	pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.enc_type = info->enc_type;
	pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].EncChannel_info.frame_count = info->frame_count;

	//由于抓图通道与子码流YUV通道绑定，所以需要跟随子码流分辨率调整分辨率
	if (CHL_2END_T == VencChn)
	{
		ret = NI_MDK_VENC_StopRecvPic(jpeg_chn);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("Error(%d - %x): NI_MDK_VENC_GetChnAttr\n", ret, ret);
			return ret;
		}
		// 抓图通道设置参数与子码流一致
		ret = NI_MDK_VENC_GetChnAttr(jpeg_chn, &VENC_ChnAttr);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("Error(%d - %x): NI_MDK_VENC_GetChnAttr\n", ret, ret);
			return ret;
		}
		//设置抓图通道的输入帧率
		VENC_ChnAttr.stRcAttr.u32FrmRateNum = (sensor_fps > info->frame_count) ? info->frame_count : sensor_fps;
		VENC_ChnAttr.stVeAttr.stInputPicAttr.u32PicWidth = info->width;
		VENC_ChnAttr.stVeAttr.stInputPicAttr.u32PicHeight = info->height;
		VENC_ChnAttr.stVeAttr.stInputPicAttr.au32Stride[0] = info->width;
		VENC_ChnAttr.stVeAttr.stInputPicAttr.au32Stride[1] = info->width;
		VENC_ChnAttr.stVeAttr.stInputPicAttr.au32Stride[2] = 0;

		ret = NI_MDK_VENC_SetChnAttr(jpeg_chn, &VENC_ChnAttr);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("Error(%d - %x): NI_MDK_VENC_SetChnAttr\n", ret, ret);
			return ret;
		}

		//当子码流帧率小于抓图通道帧率时，以子码流帧率为准
		jpeg_fps = (jpeg_fps > info->frame_count) ? info->frame_count : jpeg_fps;

		//设置抓图通道的输出帧率
		ret = NI_MDK_VENC_SetOutFrmRate(jpeg_chn, jpeg_fps);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("Error(%d - %x): channel: %d NI_MDK_VENC_SetOutFrmRate\n", ret, ret, VencChn);
			return -1;
		}

		ret = NI_MDK_VENC_StartRecvPic(jpeg_chn);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("Error(%d - %x): NI_MDK_VENC_StartRecvPic\n", ret, ret);
			return ret;
		}

		pCaptureDevice->EncDevice[channel].StreamDevice[jpeg_chn].EncChannel_info.max_width = info->width;
		pCaptureDevice->EncDevice[channel].StreamDevice[jpeg_chn].EncChannel_info.max_height = info->height;
	}

	return ret;
}

/// 编码自适应线程
///	用于维护用户编码参数修改以及夜晚降帧策略
/// 该线程仅在升级时销毁
/// 
void *_venc_adaptive_process()
{
	int channel = 0;
	int ret = FAILED;
	channel_info info;
	VPS_CHN_OUT_ATTR ChnAttr;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

	memset(&ChnAttr, 0, sizeof(VPS_CHN_OUT_ATTR));

	while (VencAdaptiveRunning)
	{
		sensor_fps = gFrameRate;

		//当设备降帧时
		if(TRUE == ChangeFrmRate)
		{
			pthread_rwlock_wrlock(&ChangeFrmLock);
			ChangeFrmRate = FALSE;
			pthread_rwlock_unlock(&ChangeFrmLock);

			ret = _video_set_vps_in_frame_rate(channel, sensor_fps);
			if (ret != RETURN_OK)
			{
				PRINT_ERROR("Error(%x): _video_set_vps_in_frame_rate!\n", ret);
				continue;
			}

			for (int ichannel = 0; ichannel < pCaptureDevice->EncDevice[channel].StreamCount; ichannel++)
			{
				// 不设置编码通道
				if (STREAM_TYPE_THI == ichannel)
				{
					break;
				}

				ret = NI_MDK_VENC_StopRecvPic(ichannel);
				if (ret != RETURN_OK)
				{
					PRINT_ERROR("Error(%x): NI_MDK_VENC_StopRecvPic chn(%d)!\n", ret, ichannel);
					ret = NI_MDK_VENC_StartRecvPic(ichannel);
					if (ret != RETURN_OK)
					{
						PRINT_ERROR("Error(%x): NI_MDK_VENC_StartRecvPic chn(%d)!\n", ret, ichannel);
					}
					continue;
				}

				ChnAttr.OutWidth = pCaptureDevice->EncDevice[channel].StreamDevice[ichannel].EncChannel_info.max_width;
				ChnAttr.OutHeight = pCaptureDevice->EncDevice[channel].StreamDevice[ichannel].EncChannel_info.max_height;
				//这边每次都是用临时配置的值传参，因为后面编码设置后，配置会保存，这样就会导致用户配置丢失
				ChnAttr.OutFps = ChnParam[ichannel].frame_count;
				ChnAttr.EnPayLoad = ChnParam[ichannel].enc_type; 
				PRINT_INFO("ichannel %d, ChnAttr.OutWidth %d\n", ichannel,ChnAttr.OutWidth);
				PRINT_INFO("ichannel %d, ChnAttr.OutHeight %d\n",  ichannel,ChnAttr.OutHeight);
				PRINT_INFO("ichannel %d, ChnAttr.OutFps %d\n", ichannel, ChnAttr.OutFps);
				PRINT_INFO("ichannel %d, ChnAttr.EnPayLoad %d\n", ichannel, ChnAttr.EnPayLoad);
				ret = _video_set_vps_out_param(channel, ichannel, &ChnAttr);
				if (ret != RETURN_OK)
				{
					PRINT_ERROR("Error(%x): _video_set_vps_in_frame_rate!\n", ret);
					ret = NI_MDK_VENC_StartRecvPic(ichannel);
					if (ret != RETURN_OK)
					{
						PRINT_ERROR("Error(%x): NI_MDK_VENC_StartRecvPic chn(%d)!\n", ret, ichannel);
					}
					continue;
				}
				
				memset(&info, 0, sizeof(channel_info));

				info.width = ChnAttr.OutWidth;
				info.height = ChnAttr.OutHeight;
				info.frame_count = ChnAttr.OutFps;
				info.bps = pCaptureDevice->EncDevice[channel].StreamDevice[ichannel].EncChannel_info.bps;
				info.rc_type = pCaptureDevice->EncDevice[channel].StreamDevice[ichannel].EncChannel_info.rc_type;
				info.profile = pCaptureDevice->EncDevice[channel].StreamDevice[ichannel].EncChannel_info.profile;
				info.qt_level = pCaptureDevice->EncDevice[channel].StreamDevice[ichannel].EncChannel_info.qt_level;
				info.enc_type = pCaptureDevice->EncDevice[channel].StreamDevice[ichannel].EncChannel_info.enc_type;
				PRINT_INFO("ichannel %d, info.profile %d\n", ichannel, info.profile);
				PRINT_INFO("ichannel %d, info.rc_type %d\n",  ichannel,info.rc_type);
				PRINT_INFO("ichannel %d, info.enc_type %d\n", ichannel, info.enc_type);
				PRINT_INFO("ichannel %d, info.bps %d\n", ichannel, info.bps);
				PRINT_INFO("ichannel %d, info.width %d\n",  ichannel,info.width);
				PRINT_INFO("ichannel %d, info.height %d\n",  ichannel,info.height);
				PRINT_INFO("ichannel %d, info.qt_level%d\n",  ichannel,info.qt_level);
				PRINT_INFO("ichannel %d, info.frame_count %d\n", ichannel, info.frame_count);
				ret = _video_set_venc_param(channel, ichannel, &info);
				if (ret != RETURN_OK)
				{
					PRINT_ERROR("Error(%x): _video_set_vps_in_frame_rate!\n", ret);
					ret = NI_MDK_VENC_StartRecvPic(ichannel);
					if (ret != RETURN_OK)
					{
						PRINT_ERROR("Error(%x): NI_MDK_VENC_StartRecvPic chn(%d)!\n", ret, ichannel);
					}
					continue;
				}

				ret = NI_MDK_VENC_StartRecvPic(ichannel);
				if (ret != RETURN_OK)
				{
					PRINT_ERROR("Error(%x): NI_MDK_VENC_StartRecvPic chn(%d)!\n", ret, ichannel);
					continue;
				}
			}
		}

		//当用户修改配置时
		for (int ichannel = 0; ichannel < pCaptureDevice->EncDevice[channel].StreamCount; ichannel++)
		{
			//不设置编码通道
			if (STREAM_TYPE_THI == ichannel)
			{
				break;
			}

			if(TRUE == ChangeChnParam[ichannel])
			{
				pthread_rwlock_wrlock(&ChangeParamLock);
				ChangeChnParam[ichannel] = FALSE;
				memset(&info, 0, sizeof(channel_info));
				memcpy(&info, &ChnParam[ichannel], sizeof(channel_info));
				pthread_rwlock_unlock(&ChangeParamLock);
				PRINT_INFO("ichannel %d, info.profile %d\n", ichannel, info.profile);
				PRINT_INFO("ichannel %d, info.rc_type %d\n", ichannel,info.rc_type);
				PRINT_INFO("ichannel %d, info.enc_type %d\n", ichannel, info.enc_type);
				PRINT_INFO("ichannel %d, info.bps %d\n", ichannel,info.bps);
				PRINT_INFO("ichannel %d, info.width %d\n", ichannel,info.width);
				PRINT_INFO("ichannel %d, info.height %d\n", ichannel,info.height);
				PRINT_INFO("ichannel %d, info.qt_level%d\n",ichannel, info.qt_level);
				PRINT_INFO("ichannel %d, info.frame_count %d\n", ichannel, info.frame_count);
				ret = NI_MDK_VENC_StopRecvPic(ichannel);
				if (ret != RETURN_OK)
				{
					PRINT_ERROR("Error(%x): NI_MDK_VENC_StopRecvPic chn(%d)!\n", ret, ichannel);
					continue;
				}

				ChnAttr.OutWidth = info.width;
				ChnAttr.OutHeight = info.height;
				ChnAttr.OutFps = info.frame_count;
				ChnAttr.EnPayLoad = info.enc_type;
				ret = _video_set_vps_out_param(channel, ichannel, &ChnAttr);
				if (ret != RETURN_OK)
				{
					PRINT_ERROR("Error(%x): _video_set_vps_out_param!\n", ret);
					ret = NI_MDK_VENC_StartRecvPic(ichannel);
					if (ret != RETURN_OK)
					{
						PRINT_ERROR("Error(%x): NI_MDK_VENC_StartRecvPic chn(%d)!\n", ret, ichannel);
					}
					continue;
				}
				
				ret = _video_set_venc_param(channel, ichannel, &info);
				if (ret != RETURN_OK)
				{
					PRINT_ERROR("Error(%x): _video_set_venc_param!\n", ret);
					ret = NI_MDK_VENC_StartRecvPic(ichannel);
					if (ret != RETURN_OK)
					{
						PRINT_ERROR("Error(%x): NI_MDK_VENC_StartRecvPic chn(%d)!\n", ret, ichannel);
					}
					continue;
				}

				ret = NI_MDK_VENC_StartRecvPic(ichannel);
				if (ret != RETURN_OK)
				{
					PRINT_ERROR("Error(%x): NI_MDK_VENC_StartRecvPic chn(%d)!\n", ret, ichannel);
					continue;
				}
			}
		}
		
		usleep(30*1000);
	}

	return NULL;
}

extern void _set_aisp_run(BOOL run);
/* 切换ISP帧率 */
void _video_Isp_change(int channel,SIZE_S pstSize)
{
    int ret = -1;
    int fps = 0;
	VPS_GRP VpsGrp = 0;
	ISP_DEV IspDev = 0;
	SENSOR_ID SenId = 0;
	VideoInDevice_p pVideoInDevice = &GlobalDevice.VideoInDevice;
	//SensorDevice_p pSensorDevice =  &GlobalDevice.SensorDevice;
#ifdef SUPPORT_4K	
	if(pstSize.u32Width == 3840 && pstSize.u32Height == 2160)
	{
		fps = 15;
	}
	else
#endif
	SEN_INFO_S pstSenInfo;
	VPS_GRP_ATTR_S pstGrpAttr;
	PISP_PUB_ATTR_S pstPubAttr;
	AISP_AFPS_CTRL_S pstAFpsCtrl;
	SEN_FMT_MATCH_KEY_S pstFmtMatchKey;
	AISP_VIDEO_FMT_INFO_S pstVideoFmtInfo;

	memset(&pstSenInfo, 0, sizeof(SEN_INFO_S));
	memset(&pstGrpAttr, 0, sizeof(VPS_GRP_ATTR_S));
	memset(&pstPubAttr, 0, sizeof(PISP_PUB_ATTR_S));
	memset(&pstAFpsCtrl, 0, sizeof(AISP_AFPS_CTRL_S));
	memset(&pstFmtMatchKey, 0, sizeof(SEN_FMT_MATCH_KEY_S));
	memset(&pstVideoFmtInfo, 0, sizeof(AISP_VIDEO_FMT_INFO_S));

	ret = NI_SEN_GetCurInfo(SenId, &pstSenInfo);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error NI_SEN_GetCurInfo ret = %x\n", ret);
		return;
	}

	ret = NI_MDK_AISP_GetAutoFps(IspDev, &pstAFpsCtrl);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("NI_MDK_AISP_GetAutoFps failed %#x\n", ret);
	}

	if(30 == pVideoInDevice->VDInfo[channel].ViDstFps)
	{
		fps = 30;
	}
	if(25 == pVideoInDevice->VDInfo[channel].ViDstFps)
	{
		fps = 25;
	}

    if (!fps)
    {
        printf("Keep current fps\n");
        return;
    }

	if (pstSenInfo.stSenFmtAttr.u32Fps == (fps * 1000))
	{
		printf("keep current fps to %d\n",fps);
		return;
	}
	else
	{

		ret = NI_MDK_PISP_GetPubAttr(IspDev, &pstPubAttr);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("Error NI_MDK_PISP_GetPubAttr ret = %x\n", ret);
			return;
		}

		pstPubAttr.u32FrameRate = fps;

		ret = NI_MDK_PISP_SetPubAttr(IspDev, &pstPubAttr);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("Error NI_MDK_PISP_SetPubAttr ret = %x\n", ret);
			return;
		}

		ret = NI_MDK_AISP_GetVideoFmtAttr(IspDev, &pstVideoFmtInfo);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("Error NI_MDK_AISP_GetVideoFmtAttr ret = %x\n", ret);
			return;
		}

		pstVideoFmtInfo.u32Fps = fps * 1000;

		ret = NI_MDK_AISP_SetVideoFmtAttr(IspDev, &pstVideoFmtInfo);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("Error NI_MDK_AISP_GetVideoFmtAttr ret = %x\n", ret);
			return;
		}

		ret = NI_MDK_VPS_GetGrpAttr(VpsGrp, &pstGrpAttr);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("Error NI_MDK_VPS_GetGrpAttr ret = %x\n", ret);
			return;
		}

		pstGrpAttr.u32InFps = fps;
		ret = NI_MDK_VPS_SetGrpAttr(VpsGrp, &pstGrpAttr);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("Error NI_MDK_VPS_SetGrpAttr ret = %x\n", ret);
			return;
		}

		//修改制式时，需要判断下降帧是否开启，若开启，就重新开启，并且帧率设置为sensor帧率的一半
		if (ENABLE == pstAFpsCtrl.bEnable)
		{
			pstAFpsCtrl.u32NewFps = pstVideoFmtInfo.u32Fps / 2;
			ret = NI_MDK_AISP_SetAutoFps(IspDev, &pstAFpsCtrl);
			if (RETURN_OK != ret)
			{
				PRINT_ERROR("NI_MDK_AISP_SetAutoFps failed %#x\n", ret);
			}
		}
		
		pVideoInDevice->VDInfo[channel].ViDstFps = fps;
	    printf("Change current fps to %d\n\n", fps);
	}

	return;
}

#ifdef DEBUG_AVBR
int VideoSetAVBRAttr(int iChannel, AVBR_Attr* pAVBR)
{
	AVBR_Attr *pAvbrAttr = &GlobalDevice.avbr_attr;
	pAvbrAttr->InitQp = pAVBR->InitQp;
	pAvbrAttr->IminQP = pAVBR->IminQP;
	pAvbrAttr->ImaxQP = pAVBR->ImaxQP;
	pAvbrAttr->PminQP = pAVBR->PminQP;
	pAvbrAttr->PmaxQP = pAVBR->PmaxQP;
	pAvbrAttr->MaxratePercent = pAVBR->MaxratePercent;
	pAvbrAttr->IFrmMaxBits = pAVBR->IFrmMaxBits;
	pAvbrAttr->IPQPDelta = pAVBR->IPQPDelta;
	pAvbrAttr->IBitProp = pAVBR->IBitProp;
	pAvbrAttr->PBitProp = pAVBR->PBitProp;
	pAvbrAttr->FluctuateLevel = pAVBR->FluctuateLevel;
	pAvbrAttr->StillratePercent = pAVBR->StillratePercent;
	pAvbrAttr->Maxstillqp = pAVBR->Maxstillqp;
}
#endif

int VideoGetEncChipCapability(VIDEO_ENCCHIPCAPABILITY *pEncChipCapability)
{
	PRINT_TRACE(">>>>>>>>>>>>	%s	 %d \n", __FUNCTION__, __LINE__);
	int i, j;
	const CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
	//VideoInDevice_p pVideoInDevice = &GlobalDevice.VideoInDevice; 
//	SystemDevice_p pSystemDevice = &GlobalDevice.SystemDevice;
	int EncCount;
	
	/*if(pSystemDevice->ZeroChannel == ENABLE)	

	{
		EncCount = pCaptureDevice->EncCount - 1;
	}
	else*/
	{
		EncCount =	pCaptureDevice->EncCount;
	}
	//PRINT_TRACE("VideoGetEncChipCapability==>1\n");
	pEncChipCapability->EncChipNR =  EncCount;
	for (i = 0; i < EncCount; i++)
	{
		for (j = 0; j < pCaptureDevice->EncDevice[i].StreamCount; j++)
		{
			if (pCaptureDevice->EncDevice[i].StreamDevice[j].CapChn == CHL_MAIN_T)
			{	
				if((pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask >> CAPTURE_POWER_4M_QHD_30) & 0x01)
				{
					pEncChipCapability->EncChipInfo[i].nMaxEncodePower = DEF_ENC_POWER_4M_QHD_30+DEF_ENC_POWER_D1_30;// DEF_ENC_POWER_CIF;
				}
				else if((pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask >> CAPTURE_POWER_UHD4K_15) & 0x01)
				{
					pEncChipCapability->EncChipInfo[i].nMaxEncodePower = DEF_ENC_POWER_UHD4K_15+DEF_ENC_POWER_D1_15;// DEF_ENC_POWER_CIF;
				}
				else if((pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask >> CAPTURE_POWER_HD1080_30) & 0x01)
				{
					pEncChipCapability->EncChipInfo[i].nMaxEncodePower = DEF_ENC_POWER_HD1080+DEF_ENC_POWER_D1;// DEF_ENC_POWER_CIF;
				}
				else if((pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask >> CAPTURE_POWER_4M_15) & 0x01)
				{
					pEncChipCapability->EncChipInfo[i].nMaxEncodePower = DEF_ENC_POWER_4M_15+ DEF_ENC_POWER_CIF;
				}
				else if((pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask >> CAPTURE_POWER_5M_12) & 0x01)
				{
					pEncChipCapability->EncChipInfo[i].nMaxEncodePower = DEF_ENC_POWER_5M_12+ DEF_ENC_POWER_CIF;
				}
				else if((pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask >> CAPTURE_POWER_QXGA_18) & 0x01)
				{
					pEncChipCapability->EncChipInfo[i].nMaxEncodePower = DEF_ENC_POWER_QXGA_18+ DEF_ENC_POWER_CIF;
				}
				else if((pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask >> CAPTURE_POWER_3M_18) & 0x01)
				{
					pEncChipCapability->EncChipInfo[i].nMaxEncodePower = DEF_ENC_POWER_3M_18+ DEF_ENC_POWER_CIF;
				}				
				else if((pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask >> CAPTURE_POWER_HD1080_15) & 0x01)
				{
					pEncChipCapability->EncChipInfo[i].nMaxEncodePower = DEF_ENC_POWER_HD1080_15+ DEF_ENC_POWER_CIF;
				}
				else if((pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask >> CAPTURE_POWER_HALF_HD1080) & 0x01)
				{
					pEncChipCapability->EncChipInfo[i].nMaxEncodePower = DEF_ENC_POWER_HALF_HD1080 + DEF_ENC_POWER_CIF;
				}
				else	if((pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask >> CAPTURE_POWER_HD720_30) & 0x01)
				{
					pEncChipCapability->EncChipInfo[i].nMaxEncodePower = DEF_ENC_POWER_HD720+ DEF_ENC_POWER_CIF;
				}
				else if((pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask >> CAPTURE_POWER_HALF_HD1080_15) & 0x01)
				{
					pEncChipCapability->EncChipInfo[i].nMaxEncodePower = DEF_ENC_POWER_HALF_HD1080_15 + DEF_ENC_POWER_CIF;
				}
				else if((pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask >> CAPTURE_POWER_HD720_15) & 0x01)
				{
					pEncChipCapability->EncChipInfo[i].nMaxEncodePower = DEF_ENC_POWER_HD720_15+ DEF_ENC_POWER_CIF;
				}
				else if((((pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask >> CAPTURE_POWER_960H_25) & 0x01)
				   || ((pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask >> CAPTURE_POWER_960H_6) & 0x01)))
				{
					pEncChipCapability->EncChipInfo[i].nMaxEncodePower = DEF_ENC_POWER_960H+ DEF_ENC_POWER_CIF;
				}
				else if ((((pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask >> CAPTURE_POWER_D1_25) & 0x01)
				   || ((pCaptureDevice->EncDevice[i].StreamDevice[j].SupportPowerMask >> CAPTURE_POWER_D1_6) & 0x01)))
				{
					pEncChipCapability->EncChipInfo[i].nMaxEncodePower = DEF_ENC_POWER_D1 + DEF_ENC_POWER_QCIF;
				}
				else
				{
					pEncChipCapability->EncChipInfo[i].nMaxEncodePower = DEF_ENC_POWER_CIF + DEF_ENC_POWER_QCIF;
				}

				pEncChipCapability->EncChipInfo[i].nMaxSupportChannel = DEF_SUPPORT_CHL;
				pEncChipCapability->EncChipInfo[i].bChannelMaxSetSync = 0;
				pEncChipCapability->EncChipInfo[i].nExpandChannel = 0;		
				break;
			}
		}

		if (j == pCaptureDevice->EncDevice[i].StreamCount)
		{
			PRINT_TRACE("找不到通道%d, 的最大能力，使用默认值CIF\n", i);
			pEncChipCapability->EncChipInfo[i].nMaxEncodePower = DEF_ENC_POWER_CIF + DEF_ENC_POWER_QCIF;
			pEncChipCapability->EncChipInfo[i].nMaxSupportChannel = DEF_SUPPORT_CHL;
			pEncChipCapability->EncChipInfo[i].bChannelMaxSetSync = 0;
			pEncChipCapability->EncChipInfo[i].nExpandChannel = 0;		
		}
	}
	
//	PRINT_TRACE("g_sysInfor.vencChanne = %d\n", g_sysInfor.vencChannel);
	return 0;
}

//辅码流能力获取
int VideoGetExtCaps(VIDEO_EXT_STREAM *pStream)
{
	int EncCnt,i,j;
	int FirCap = 0,SceCap = 0;
	//VideoInDevice_p pVideoInDevice = &GlobalDevice.VideoInDevice;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
	
	EncCnt = pCaptureDevice->EncCount;
	pStream->ExtraStream = pCaptureDevice->EncDevice[0].SupportStream;
	for(i = 0;i < EncCnt;i++)
	{
		for(j = 0;j < pCaptureDevice->EncDevice[i].StreamCount;j++)
		{
			if(pCaptureDevice->EncDevice[i].StreamDevice[j].CapChn == CHL_MAIN_T)
			{
				FirCap = pCaptureDevice->EncDevice[i].StreamDevice[j].Format.ImageSize;
			}

			if(pCaptureDevice->EncDevice[i].StreamDevice[j].CapChn == CHL_2END_T)
			{
				SceCap = pCaptureDevice->EncDevice[i].StreamDevice[j].Format.ImageSize;
			}
		}
	}

	pStream->CaptureSizeMask[FirCap]  =  (1 << SceCap);

	return 0;
}

/// 得到支持的特性。
/// 
/// \param [out] pCaps 指向捕获特性结构VIDEO_CAPS的指针。
/// \retval 0  获取成功。
/// \retval 0  获取失败。
int VideoGetCaps(VIDEO_CAPS * pCaps)
{
	//PRINT_TRACE(">>>>>>>>>>>>	%s	 %d \n", __FUNCTION__, __LINE__);
	//以下没有做接口封装
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
//	SystemDevice_p pSystemDevice = &GlobalDevice.SystemDevice;
	int i;

	memset(pCaps, 0, sizeof(VIDEO_CAPS)); 

	//CAPTURE_POWER_CIF_6
	for(i=0;i<pCaptureDevice->EncCount;i++)
	{
		memcpy(&pCaps->ChnCap[i], &pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].Caps, sizeof(VIDEO_CHN_CAP));

		pCaps->ChnCap[i].Compression = (1 << VIDEO_COMP_H264) | (1 << VIDEO_COMP_H265);
		pCaps->ChnCap[i].BitRateControl = (1 << VIDEO_BRC_CBR) | (1 << VIDEO_BRC_VBR);
		pCaps->ChnCap[i].Media = (1 << CAPTURE_START_VIDEO) | (1 << CAPTURE_START_AUDIO);
		pCaps->ChnCap[i].CoverBlocks = VPS_MASK_MAX_CHN_NUM;
		pCaps->ChnCap[i].TitleCount = TD_OSD_MAX_NUM;//包括时间标题与通道标题
		pCaps->ChnCap[i].WaterMask = 0;//暂时不用水印
		pCaps->ChnCap[i].RegionCover = (VPS_MASK_MAX_CHN_NUM > 0 ? 1 : 0);

		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_HD1080_30) & 0x01
			|| (pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_HD1080_15) & 0x01)
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_1080);
		}
		if (((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_HD720_30) & 0x01)
			|| ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_HD720_15) & 0x01))
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_720);
		}
		if (((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_960H_25) & 0x01)
			|| ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_960H_6) & 0x01))
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_960H);
		}
		if (((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_D1_25) & 0x01)
			|| ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_D1_6) & 0x01))
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_D1);
		}
		if (((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_CIF_25) & 0x01)
			|| ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_CIF_6) & 0x01))
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_CIF);
		}
		if (((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_QCIF_25) & 0x01)
			|| ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_QCIF_6) & 0x01))
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_QCIF);
		}
		if (((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_BCIF_25) & 0x01)
			|| ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_BCIF_6) & 0x01))
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_BCIF);
		}
		if (((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_HALF_HD1080) & 0x01)
			|| ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_HALF_HD1080_15) & 0x01))
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_HALF1080);
		}						
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_QXGA_18) & 0x01)
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_QXGA);
		}
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_3M_18) & 0x01)
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_3M);
		}		
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_4M_15) & 0x01)
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_4M);
		}
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_5M_12) & 0x01)
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_5M);
		}
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_half4M_12) & 0x01)
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_HALF4M);
		}
		if (((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_4M_QHD_12) & 0x01)
			|| ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_4M_QHD_30) & 0x01))
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_4M_QHD);
		}
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_NHD_30) & 0x01)
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_NHD);
		}
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_FIR].SupportPowerMask >> CAPTURE_POWER_1296P_30) & 0x01)
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_1296P);
		}
		//if()
		//pCaps->ChnCap[i].JpegImageSize |= (1 << VIDEO_SIZE_CIF);
		//pCaps->ChnCap[i].JpegImageSize |= (1 << VIDEO_SIZE_D1);
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_HD1080_30) & 0x01)
		{
			pCaps->ChnCap[i].JpegImageSize |= (1 << VIDEO_SIZE_1080);
		}
		if (((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_HD720_30) & 0x01)
			|| ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_HD720_15) & 0x01))
		{
			pCaps->ChnCap[i].JpegImageSize |= (1 << VIDEO_SIZE_720);
		}
		if (((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_960H_25) & 0x01)
			|| ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_960H_6) & 0x01))
		{
			pCaps->ChnCap[i].JpegImageSize |= (1 << VIDEO_SIZE_960H);
		}
		if (((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_D1_25) & 0x01)
			|| ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_D1_6) & 0x01))
		{
			pCaps->ChnCap[i].JpegImageSize |= (1 << VIDEO_SIZE_D1);
		}
		if (((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_CIF_25) & 0x01)
			|| ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_CIF_6) & 0x01))
		{
			pCaps->ChnCap[i].JpegImageSize |= (1 << VIDEO_SIZE_CIF);
		}
		if (((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_QCIF_25) & 0x01)
			|| ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_QCIF_6) & 0x01))
		{
			pCaps->ChnCap[i].JpegImageSize|= (1 << VIDEO_SIZE_QCIF);
		}
		if (((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_BCIF_25) & 0x01)
			|| ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_BCIF_6) & 0x01))
		{
			pCaps->ChnCap[i].JpegImageSize |= (1 << VIDEO_SIZE_BCIF);
		}
		if (((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_HALF_HD1080) & 0x01)
			|| ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_HALF_HD1080_15) & 0x01))
		{
			pCaps->ChnCap[i].JpegImageSize |= (1 << VIDEO_SIZE_HALF1080);
		}	
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_QXGA_18) & 0x01)
		{
			pCaps->ChnCap[i].JpegImageSize |= (1 << VIDEO_SIZE_QXGA);
		}
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_3M_18) & 0x01)
		{
			pCaps->ChnCap[i].JpegImageSize |= (1 << VIDEO_SIZE_3M);
		}		
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_5M_12) & 0x01)
		{
			pCaps->ChnCap[i].JpegImageSize |= (1 << VIDEO_SIZE_5M);
		}
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_4M_15) & 0x01)
		{
			pCaps->ChnCap[i].JpegImageSize |= (1 << VIDEO_SIZE_4M);
		}
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_half4M_12) & 0x01)
		{
			pCaps->ChnCap[i].JpegImageSize |= (1 << VIDEO_SIZE_HALF4M);
		}
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_4M_QHD_12) & 0x01)
		{
			pCaps->ChnCap[i].JpegImageSize |= (1 << VIDEO_SIZE_4M_QHD);
		}
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_NHD_30) & 0x01)
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_NHD);
		}
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_1296P_30) & 0x01)
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_1296P);
		}
		if ((pCaptureDevice->EncDevice[i].StreamDevice[STREAM_TYPE_THI].SupportPowerMask >> CAPTURE_POWER_QVGA_25) & 0x01)
		{
			pCaps->ChnCap[i].ImageSize |= (1 << VIDEO_SIZE_QVGA);
		}

		if (((pCaptureDevice->EncDevice[0].SupportStream >> CHL_JPEG_T)&0x1) == 0)
		{
			//PRINT_TRACE("找不到默认的JPEG编码压缩能力，强制设置为CIF\n");
			pCaps->ChnCap[i].JpegImageSize |= (1 << VIDEO_SIZE_CIF);
		}
	}	
	return 0;
}

/// 得到设备支持的最大分辨率与帧率
/// 
/// \param [out] pCaps 指向捕获特性结构VIDEO_MAX_SIZE的指针。
/// \retval 0  获取成功。
/// \retval 0  获取失败。
int VideoGetMaxSize(VIDEO_MAX_SIZE *pCaps)
{
	SensorDevice_p pSensorDevice = &GlobalDevice.SensorDevice;
	pCaps->maxWidth = pSensorDevice->MaxWidth;
	pCaps->maxHeight = pSensorDevice->MaxHeight;
	pCaps->maxFPS = pSensorDevice->MaxFPS;
	return 0;
}

int VideoCreate(int index)
{
	return 0;
}

int VideoDestroy(int index)
{
	return 0;
}

// 创建音频编码通道
// 仅格式化SD卡使用
int VideoVencCreate(void)
{
	PRINT_ERROR("VideoVencCreate start\n");
	int ret = 0;

	ret = video_venc_start();
	if (ret != NI_SUCCESS)
	{
		return ret;
	}

	capture_video_stream_start();
	PRINT_ERROR("VideoVencCreate End\n");
	return ret;
}

// 销毁音频编码通道
// 仅格式化SD卡使用
int VideoVencDestroy(void)
{
	PRINT_ERROR("VideoVencDestroy Start\n");
	capture_video_stream_stop();
	PRINT_ERROR("VideoVencDestroy End\n");
	return 0;
}

/// 设置OSD字体大小。
///
/// \param [in] channel 通道号。
/// \param [in] size 字体大小
/// \retval 0  设置成功。
int VideoSetTitleSize(int channel,unsigned int size)
{
	RegionDevice_p pRegionDevice = &GlobalDevice.RegionDevice;
	pRegionDevice->OsdHeight = size;
	return 0;
}

/// 设置捕获数据压缩的格式。
/// 
/// \param [in] channel 通道号。
/// \param [in] dwType
/// \param [in] pFormat 指向捕获格式结构CAPTURE_FORMAT的指针。
/// \retval 0  设置成功
/// \retval 0  设置失败
int VideoSetFormat(int channel, DWORD dwType, VIDEO_FORMAT * pFormat)
{
	int i = 0;	
	int ret = -1;
	int EncCnt = 0;
	struct channel_info info;
	SIZE_S pstSize;
	unsigned int VencChn = 0;
	VPS_CHN_ATTR_S VPS_ChnAttr;
	VENC_CHN_ATTR_S VENC_ChnAttr;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
	VideoInDevice_p pVideoInDevice = &GlobalDevice.VideoInDevice;

	EncCnt = pCaptureDevice->EncCount;
	memset(&info, 0, sizeof(info));
	memset(&pstSize, 0, sizeof(SIZE_S));
	memset(&VPS_ChnAttr, 0, sizeof(VPS_CHN_ATTR_S));
	memset(&VENC_ChnAttr, 0, sizeof(VENC_CHN_ATTR_S));

	if (NULL == pFormat || channel >= EncCnt || 0 == pFormat->FramesPerSecond)
	{
		PRINT_ERROR("channel %d parameter FramesPerSecond %d error\n", channel, pFormat->FramesPerSecond);
		return -1;
	}

	if (!((pCaptureDevice->EncDevice[channel].SupportStream >> dwType) & 0x01))
	{
		PRINT_ERROR("not support dwType = %d\n",dwType);
		return -1;
	}

	for (i = 0; i < pCaptureDevice->EncDevice[channel].StreamCount; i++)
	{
		if (pCaptureDevice->EncDevice[channel].StreamDevice[i].CapChn == dwType)
		{
			VencChn = i;
		}
	}

	if (CHL_JPEG_T == dwType)
	{
		return 0;
	}

	ret = _get_pic_size(pFormat->ImageSize, &pstSize);
	if (ret)
	{
		PRINT_ERROR("_get_pic_size failed\n");
		return -1;
	}

	if(pCaptureDevice->EncDevice[channel].StreamDevice[VencChn].CapChn == CHL_MAIN_T)
	{
		_video_Isp_change(channel,pstSize);
	}
	
	if(pVideoInDevice->VDInfo[channel].ViDstFps < pFormat->FramesPerSecond)
	{
		pFormat->FramesPerSecond = pVideoInDevice->VDInfo[channel].ViDstFps;
	}
	
	ret = _get_pic_type(pFormat, &info);
	if(ret)
	{
		PRINT_ERROR("_get_pic_type failed\n");
		return -1;
	}

	ChnParam[VencChn].profile = info.profile;
	ChnParam[VencChn].rc_type = info.rc_type;
	ChnParam[VencChn].bps = pFormat->BitRate;
	ChnParam[VencChn].enc_type = info.enc_type;
	ChnParam[VencChn].width = pstSize.u32Width;
	ChnParam[VencChn].height = pstSize.u32Height;
	ChnParam[VencChn].qt_level = pFormat->ImageQuality;
	ChnParam[VencChn].frame_count = pFormat->FramesPerSecond;
	PRINT_INFO("info.profile %d\n" ,info.profile);
	PRINT_INFO("info.rc_type %d\n", info.rc_type);
	PRINT_INFO("info.enc_type %d\n" ,info.enc_type);
	PRINT_INFO("pFormat->BitRate %d\n" ,pFormat->BitRate);
	PRINT_INFO("pstSize.u32Width %d\n", pstSize.u32Width);
	PRINT_INFO("pstSize.u32Height %d\n" ,pstSize.u32Height);
	PRINT_INFO("pFormat->ImageQuality %d\n", pFormat->ImageQuality);
	PRINT_INFO("pFormat->FramesPerSecond %d\n", pFormat->FramesPerSecond);

	pthread_rwlock_wrlock(&ChangeParamLock);
	ChangeChnParam[VencChn] = TRUE;
	pthread_rwlock_unlock(&ChangeParamLock);

	return ret;
}

/// 设置标题叠加。
/// 
/// \param [in] channel 通道号。
/// \param [in] pParam 指向标题叠加结构VIDEO_TITLE_PARAM的指针。
/// \retval 0  设置成功。
/// \retval <0  设置失败。
int VideoSetTitle(int channel, VIDEO_TITLE_PARAM *pParam)
{

	if (NULL == pParam || NULL == pParam->raster)
	{
		PRINT_ERROR("VideoSetTitle pParam error\n");
		return FAILED;
	}

	if (channel != 0)
	{
		PRINT_ERROR("Failed to select channel\n");
		return FAILED;
	}

	int i = 0;
	int j = 0;
	int ret = -1;
	int type = pParam->index;
	unsigned int fontsize = 0;
	unsigned int EnlargeNumber = 0;
	int ichannel = STREAM_TYPE_FIR;
	unsigned int u32OsdHandle = type;

	POINT_S OSD_point;
	MDK_CHN_S stMdkChn;
	OSD_REGION_S stOsdRegion;
	SRC_OSD_ATTR stSrcOsdAttr;
	OSD_PALETTE_S stOsdPalette;
	OSD_DISP_ATTR_S stOsdDispAttr;
	OSD_BUFFER_INFO_S stOsdBufInfo;
	SIZE_S picsize[STREAM_TYPE_BUT];
	memset(&OSD_point, 0, sizeof(POINT_S));
	memset(&stMdkChn, 0, sizeof(MDK_CHN_S));
	memset(&stOsdRegion, 0, sizeof(OSD_REGION_S));
	memset(&stSrcOsdAttr, 0, sizeof(SRC_OSD_ATTR));
	memset(&stOsdPalette, 0, sizeof(OSD_PALETTE_S));
	memset(&stOsdDispAttr, 0, sizeof(OSD_DISP_ATTR_S));
	memset(&stOsdBufInfo, 0, sizeof(OSD_BUFFER_INFO_S));
	memset(&picsize, 0, sizeof(SIZE_S) * STREAM_TYPE_BUT);

	RegionDevice_p pRegionDevice = &GlobalDevice.RegionDevice;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

	OSD_point.s32X = pParam->x;
	OSD_point.s32Y = pParam->y;

	pthread_mutex_lock(&osd_lock);

	memcpy(pRegionDevice->channeltitle[type], pParam->raster, pParam->width * pParam->height / 8);

	for (; ichannel < pCaptureDevice->EncDevice[channel].StreamCount; ichannel++)
	{
		if (STREAM_TYPE_THI == ichannel)
		{
			break;
		}

		picsize[ichannel].u32Width = pCaptureDevice->EncDevice[i].StreamDevice[ichannel].EncChannel_info.max_width;
		picsize[ichannel].u32Height = pCaptureDevice->EncDevice[i].StreamDevice[ichannel].EncChannel_info.max_height;

		if(STREAM_TYPE_FIR == ichannel) 
		{
			if (0 == pRegionDevice->OsdHeight)
			{
				for (j = 0; j < (int)sizeof(FontSiZeTable) / (int)sizeof(int); j++)
				{
					if ((FontSiZeTable[j] * 32 / 2) < picsize[STREAM_TYPE_FIR].u32Width * 0.8)
					{
						fontsize = FontSiZeTable[j];
						break;
					}
				}
			}
			else
			{
				fontsize = pRegionDevice->OsdHeight;
			}
		}
		else
		{
			u32OsdHandle += 4;
			fontsize = 16;
		}

		EnlargeNumber = fontsize / 16;
		stMdkChn.enModId = NI_ID_VPS;
		stMdkChn.s32DevId = 0;
		stMdkChn.s32ChnId = ichannel;

		NI_MDK_OSD_GetDispAttr(u32OsdHandle, &stMdkChn, &stOsdDispAttr);
		// NI_MDK_OSD_ClearFromChn(u32OsdHandle, &stMdkChn);

		stOsdRegion.bMacroFormat = NI_FALSE;
		stOsdRegion.enOsdRgn = OSD_RGN;
		stOsdRegion.enPixelFormat = OSD_PIXEL_FORMAT_2BIT;

		stSrcOsdAttr.stOsdRegion = stOsdRegion;
		stSrcOsdAttr.stSize.u32Width = pParam->width;
		stSrcOsdAttr.stSize.u32Height = pParam->height;
		stSrcOsdAttr.u8FgIdx = 1;
		stSrcOsdAttr.u8BgIdx = 0;
		stSrcOsdAttr.enFormat = OSD_FORMAT_BIT;

		stOsdDispAttr.bShow = pParam->enable;
		if (0 == pParam->width || 0 == pParam->height)
		{
			stOsdDispAttr.bShow =FALSE;
		}
		stOsdDispAttr.stSize.u32Width = pParam->width * EnlargeNumber;
		stOsdDispAttr.stSize.u32Height = fontsize;

		if(0 == stOsdDispAttr.stSize.u32Width)
		{
			stOsdDispAttr.stSize.u32Width = 16;
		}

		if (0 == stOsdDispAttr.stSize.u32Height)
		{
			stOsdDispAttr.stSize.u32Height = 16;
		}

		type = u32OsdHandle % 4;
		switch (type)
		{
			case OSD_TYPE_TIME:
			case OSD_TYPE_TITLE:
				stOsdDispAttr.u32Layer = 0;
				break;
			case OSD_TYPE_LABLE1:
			case OSD_TYPE_LABLE2:
				stOsdDispAttr.u32Layer = 1;
				break;
			default:
				stOsdDispAttr.u32Layer = 0;
				break;
		}

		stOsdDispAttr.enAlphaMode = OSD_ALPHA_MODE_PALETTE;
		stOsdDispAttr.u32RegionAlpha = 31;
		stOsdDispAttr.bInvert = pParam->AutoBlackWhite;
		stOsdDispAttr.bSpecialFrmId = NI_FALSE;
		stOsdDispAttr.u32TargetFrameId = 0;
		// 由于反色效果不好，关闭反色
		stOsdDispAttr.bInvert = 0;

		ret = _osd_coord_calculate(ichannel, u32OsdHandle, EnlargeNumber, stOsdDispAttr.stSize, OSD_point, &stOsdDispAttr.stStartPiont);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("_osd_coord_calculate failed\n");
			pthread_mutex_unlock(&osd_lock);
			return ret;
		}

		stOsdBufInfo.u32Len = stOsdDispAttr.stSize.u32Width * stOsdDispAttr.stSize.u32Height / 2;

		if (stOsdBufInfo.u32Len < 256)
		{
			stOsdBufInfo.u32Len = 256;
		}

		//由于CH7L内存不足，这边预分配时间标题的最大内存32KB
		if (OSD_TYPE_TIME == u32OsdHandle)
		{
			stOsdBufInfo.u32Len = 32 * 1024;
		}

		SampleOsdPaletteInit(&stOsdPalette);
		stOsdPalette.astPalette[1].u8Alpha = (int)((float)((pParam->fg_color) >> 24) / 255 * 31);
		stOsdPalette.astPalette[2].u8Alpha = (int)((float)((pParam->fg_color) >> 24) / 255 * 31);
		stOsdPalette.astPalette[3].u8Alpha = (int)((float)((pParam->fg_color) >> 24) / 255 * 31);

		ret = NI_MDK_OSD_GetBuffer(u32OsdHandle, &stOsdBufInfo);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("NI_MDK_OSD_GetBuffer failed with %x\n", ret);
			pthread_mutex_unlock(&osd_lock);
			return ret;
		}
		
		memset((NI_U8 *)stOsdBufInfo.pu32VirtAddr, 0, stOsdBufInfo.u32Len);

		ret = _vsf_drv_enlarge_osd((NI_U8 *)pRegionDevice->channeltitle[type], (NI_U8 *)stOsdBufInfo.pu32VirtAddr, stOsdRegion.enPixelFormat, EnlargeNumber, &stSrcOsdAttr.stSize);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("VSF_drvNarrowOsd failed with %x\n", ret);
			pthread_mutex_unlock(&osd_lock);
			return ret;
		}

		OSD_TEST_ConvertToLittleEndian2((NI_U8 *)stOsdBufInfo.pu32VirtAddr, stOsdRegion.enPixelFormat, stOsdBufInfo.u32Len);

		ret = NI_MDK_OSD_SetPalette(&stMdkChn, &stOsdPalette);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("NI_MDK_OSD_SetPalette failed with %x\n", ret);
			pthread_mutex_unlock(&osd_lock);
			return ret;
		}

		//OSD单元设置非单元反色模式
		stOsdDispAttr.enInvertBlkMode = OSD_INVERT_BLK_NULL;
		ret = NI_MDK_OSD_PaintToChn(u32OsdHandle, &stMdkChn, &stOsdDispAttr, 500);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("u32OsdHandle : %d  NI_MDK_OSD_PaintToChn failed with %x\n", u32OsdHandle, ret);
			_osd_error(&stMdkChn, &stOsdDispAttr);
			pthread_mutex_unlock(&osd_lock);
			return ret;
		}

		ret = NI_MDK_OSD_Refresh(u32OsdHandle, 500);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("NI_MDK_OSD_Refresh failed with %x\n", ret);
			pthread_mutex_unlock(&osd_lock);
			return ret;
		}
	}

	usleep(50 * 1000);

	pthread_mutex_unlock(&osd_lock);

	pRegionDevice->OsdWidth[type] = pParam->width;
	pRegionDevice->FgColor = pParam->fg_color;
	pRegionDevice->BgColor = pParam->bg_color;
	pRegionDevice->PositionX[type] = pParam->x;
	pRegionDevice->PositionY[type] = pParam->y;
	pRegionDevice->show[type] = pParam->enable;
	pRegionDevice->InvertEnable = pParam->AutoBlackWhite;

	return 0;
}

/// 设置logo叠加。
/// 
/// \param [in] channel 通道号。
/// \param [in] pParam 指向标题叠加结构VIDEO_TITLE_PARAM的指针。
/// \retval 0  设置成功。
/// \retval !0  设置失败。
int VideoSetLogo(int channel, VIDEO_TITLE_PARAM *pParam)
{
	if (NULL == pParam)
	{
		PRINT_ERROR("VideoSetTitle pParam error\n");
		return FAILED;
	}

	int i = 0;
	int ret = -1;
	unsigned char *logo = NULL;
	unsigned int u32OsdHandle = 9;
	int ichannel = STREAM_TYPE_FIR;

	POINT_S OSD_point;
	MDK_CHN_S stMdkChn;
	OSD_REGION_S stOsdRegion;
	OSD_PALETTE_S stOsdPalette;
	OSD_DISP_ATTR_S stOsdDispAttr;
	OSD_BUFFER_INFO_S stOsdBufInfo;
	SIZE_S picsize[STREAM_TYPE_BUT];
	memset(&OSD_point, 0, sizeof(POINT_S));
	memset(&stMdkChn, 0, sizeof(MDK_CHN_S));
	memset(&stOsdRegion, 0, sizeof(OSD_REGION_S));
	memset(&stOsdPalette, 0, sizeof(OSD_PALETTE_S));
	memset(&stOsdDispAttr, 0, sizeof(OSD_DISP_ATTR_S));
	memset(&stOsdBufInfo, 0, sizeof(OSD_BUFFER_INFO_S));
	memset(&picsize, 0, sizeof(SIZE_S) * STREAM_TYPE_BUT);

	int vstd = VIDEO_STANDARD_PAL;
	unsigned int cif_width = imageSize[vstd][VIDEO_SIZE_CIF].u32Width; /* 上层传递的坐标为在CIF下的坐标 */
	unsigned int cif_height = imageSize[vstd][VIDEO_SIZE_CIF].u32Height;
	RegionDevice_p pRegionDevice = &GlobalDevice.RegionDevice;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

	if (FALSE == pRegionDevice->LogoInit)
	{
		logo_create();
		pRegionDevice->LogoInit = TRUE;
	}

	pthread_mutex_lock(&osd_lock);
	for (; ichannel < pCaptureDevice->EncDevice[channel].StreamCount; ichannel++)
	{
		if (STREAM_TYPE_THI == ichannel)
		{
			break;
		}

		picsize[ichannel].u32Width = pCaptureDevice->EncDevice[i].StreamDevice[ichannel].EncChannel_info.max_width;
		picsize[ichannel].u32Height = pCaptureDevice->EncDevice[i].StreamDevice[ichannel].EncChannel_info.max_height;

		stMdkChn.enModId = NI_ID_VPS;
		stMdkChn.s32DevId = 0;
		stMdkChn.s32ChnId = ichannel;

		NI_MDK_OSD_GetDispAttr(u32OsdHandle, &stMdkChn, &stOsdDispAttr);

		stOsdRegion.bMacroFormat = NI_FALSE;
		stOsdRegion.enOsdRgn = OSD_RGN;
		stOsdRegion.enPixelFormat = OSD_PIXEL_FORMAT_4BIT;

		if(STREAM_TYPE_FIR == ichannel)
		{
			stOsdDispAttr.stSize.u32Width = MAIN_LOGO_WIDTH;
			stOsdDispAttr.stSize.u32Height = MAIN_LOGO_HEIGHT;
			logo = main_logo;
		}
		else if (STREAM_TYPE_SEC == ichannel)
		{
			stOsdDispAttr.stSize.u32Width = SUB_LOGO_WIDTH;
			stOsdDispAttr.stSize.u32Height = SUB_LOGO_HEIGHT;
			logo = sub_logo;
		}
		
		stOsdDispAttr.bShow = pParam->enable;
		stOsdDispAttr.u32Layer = 1;
		stOsdDispAttr.enAlphaMode = OSD_ALPHA_MODE_PALETTE;
		stOsdDispAttr.u32RegionAlpha = 31;
		stOsdDispAttr.bInvert = pParam->AutoBlackWhite;
		stOsdDispAttr.bSpecialFrmId = NI_FALSE;
		stOsdDispAttr.u32TargetFrameId = 0;
		// 由于反色效果不好，关闭反色
		stOsdDispAttr.bInvert = 0;

		stOsdDispAttr.stStartPiont.s32X = (int)((float)pParam->x * picsize[ichannel].u32Width / cif_width);
		stOsdDispAttr.stStartPiont.s32Y = (int)((float)pParam->y * picsize[ichannel].u32Height / cif_height);

		if (stOsdDispAttr.stStartPiont.s32X + stOsdDispAttr.stSize.u32Width > picsize[ichannel].u32Width)
		{
			stOsdDispAttr.stStartPiont.s32X = picsize[ichannel].u32Width - stOsdDispAttr.stSize.u32Width;
		}
		if (stOsdDispAttr.stStartPiont.s32Y + stOsdDispAttr.stSize.u32Height > picsize[ichannel].u32Height)
		{
			stOsdDispAttr.stStartPiont.s32Y = picsize[ichannel].u32Height - stOsdDispAttr.stSize.u32Height;
		}

		if (stOsdDispAttr.stStartPiont.s32X % 2)
		{
			stOsdDispAttr.stStartPiont.s32X--;
		}

		if (stOsdDispAttr.stStartPiont.s32Y % 2)
		{
			stOsdDispAttr.stStartPiont.s32Y--;
		}

		stOsdBufInfo.u32Len = stOsdDispAttr.stSize.u32Width * stOsdDispAttr.stSize.u32Height / 2;

		if (stOsdBufInfo.u32Len < 256)
		{
			stOsdBufInfo.u32Len = 256;
		}

		SampleOsdPaletteInit(&stOsdPalette);
		stOsdPalette.astPalette[1].u8Alpha = (int)((float)((pParam->fg_color) >> 24) / 255 * 31);
		stOsdPalette.astPalette[2].u8Alpha = (int)((float)((pParam->fg_color) >> 24) / 255 * 31);
		stOsdPalette.astPalette[3].u8Alpha = (int)((float)((pParam->fg_color) >> 24) / 255 * 31);

		ret = NI_MDK_OSD_GetBuffer(u32OsdHandle, &stOsdBufInfo);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("NI_MDK_OSD_GetBuffer failed with %x\n", ret);
			pthread_mutex_unlock(&osd_lock);
			return ret;
		}

		ret = _osd_bit_switch(logo, (NI_U8 *)stOsdBufInfo.pu32VirtAddr, stOsdRegion.enPixelFormat, &stOsdDispAttr.stSize);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("_osd_bit_switch failed with %x\n", ret);
			pthread_mutex_unlock(&osd_lock);
			return ret;
		}

		OSD_TEST_ConvertToLittleEndian2((NI_U8 *)stOsdBufInfo.pu32VirtAddr, stOsdRegion.enPixelFormat, stOsdBufInfo.u32Len);

		ret = NI_MDK_OSD_SetPalette(&stMdkChn, &stOsdPalette);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("NI_MDK_OSD_SetPalette failed with %x\n", ret);
			pthread_mutex_unlock(&osd_lock);
			return ret;
		}

		//OSD单元设置非单元反色模式
		stOsdDispAttr.enInvertBlkMode = OSD_INVERT_BLK_NULL;
		ret = NI_MDK_OSD_PaintToChn(u32OsdHandle, &stMdkChn, &stOsdDispAttr, 500);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("u32OsdHandle : %d  NI_MDK_OSD_PaintToChn failed with %x\n", u32OsdHandle, ret);
			_osd_error(&stMdkChn, &stOsdDispAttr);
			pthread_mutex_unlock(&osd_lock);
			return ret;
		}

		ret = NI_MDK_OSD_Refresh(u32OsdHandle, 500);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("NI_MDK_OSD_Refresh failed with %x\n", ret);
			pthread_mutex_unlock(&osd_lock);
			return ret;
		}
		u32OsdHandle++;
	}

	usleep(50 * 1000);

	pthread_mutex_unlock(&osd_lock);
	logo = NULL;
	pRegionDevice->LogoEnable = pParam->enable;
	pRegionDevice->LogoX = pParam->x;
	pRegionDevice->LogoY = pParam->y;

	return ret;
}

/// 设置覆盖区域。
/// 
/// \param [in] channel 通道号。
/// \param [in] pParam 指向视频覆盖结构VIDEO_COVER_PARAM的指针。
/// \retval 0  设置成功。
/// \retval <0  设置失败。
int VideoSetCover(int channel,int index, VIDEO_COVER_PARAM *pParam)
{
	int ret = -1;
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int w = 0;
	unsigned int h = 0;
	VPS_GRP VpsGrp = 0;
	VPS_MASK_ATTR_S pstMaskAttr;
	SIZE_S picsize[STREAM_TYPE_BUT];
	RegionDevice_p pRegionDevice = &GlobalDevice.RegionDevice;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

	int vstd = VIDEO_STANDARD_PAL;
	unsigned int cif_width = imageSize[vstd][VIDEO_SIZE_CIF].u32Width; /* 上层传递的坐标为在CIF下的坐标 */
	unsigned int cif_height = imageSize[vstd][VIDEO_SIZE_CIF].u32Height;

	memset(&pstMaskAttr, 0, sizeof(VPS_MASK_ATTR_S));
	memset(&picsize, 0, sizeof(SIZE_S) * STREAM_TYPE_BUT);

	for (int i = 0; i < pCaptureDevice->EncCount; i++)
	{
		for (int j = 0; j < pCaptureDevice->EncDevice[i].StreamCount; j++)
		{
			picsize[j].u32Width = pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.max_width;
			picsize[j].u32Height = pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.max_height;
		}
	}

	x = pParam->rect.left;
	y = pParam->rect.top;
	w = pParam->rect.right - pParam->rect.left;
	h = pParam->rect.bottom - pParam->rect.top;

	for (int ichannel = STREAM_TYPE_FIR; ichannel < pCaptureDevice->EncDevice[channel].StreamCount; ichannel++)
	{
		if (STREAM_TYPE_THI == ichannel)
		{
			break;
		}

		ret = NI_MDK_VPS_GetChnMaskAttr(VpsGrp, ichannel, &pstMaskAttr);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("NI_MDK_VPS_GetChnMaskAttr failed with %x\n", ret);
			return ret;
		}

		pstMaskAttr.stMaskColor.u8Y = 0;
		pstMaskAttr.stMaskColor.u8U = 128;
		pstMaskAttr.stMaskColor.u8V = 128;
		pstMaskAttr.astMaskRect[index].s32X = (int)((float)x * picsize[ichannel].u32Width / cif_width);				/* mask左上角起始点像素宽度位置 */
		pstMaskAttr.astMaskRect[index].s32Y = (int)((float)y * picsize[ichannel].u32Height / cif_height);			/* mask左上角起始点像素高度位置 */
		pstMaskAttr.astMaskRect[index].u32Width = (int)((float)w * picsize[ichannel].u32Width / cif_width);		 	/* mask宽度 */
		pstMaskAttr.astMaskRect[index].u32Height = (int)((float)h * picsize[ichannel].u32Height / cif_height);		/* mask高度 */

		if (pstMaskAttr.astMaskRect[index].s32X % 2)
		{
			pstMaskAttr.astMaskRect[index].s32X--;
		}
		if (pstMaskAttr.astMaskRect[index].s32Y % 2)
		{
			pstMaskAttr.astMaskRect[index].s32Y--;
		}
		if (pstMaskAttr.astMaskRect[index].u32Width % 2)
		{
			pstMaskAttr.astMaskRect[index].u32Width--;
		}
		if (pstMaskAttr.astMaskRect[index].u32Height % 2)
		{
			pstMaskAttr.astMaskRect[index].u32Height--;
		}
		if (0 == pstMaskAttr.astMaskRect[index].u32Width)
		{
			pstMaskAttr.astMaskRect[index].u32Width = 2;
		}
		if (0 == pstMaskAttr.astMaskRect[index].u32Height)
		{
			pstMaskAttr.astMaskRect[index].u32Height = 2;
		}

		ret = NI_MDK_VPS_SetChnMaskAttr(VpsGrp, ichannel, &pstMaskAttr);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("NI_MDK_VPS_SetMaskAttr failed with %x\n", ret);
			return ret;
		}

		if(ENABLE == pParam->enable)
		{
			ret = NI_MDK_VPS_EnableChnMask(VpsGrp, ichannel, index);
			if (ret != RETURN_OK)
			{
				PRINT_ERROR("NI_MDK_VPS_EnableChn failed with %x\n", ret);
				return ret;
			}
		}
		else if(DISABLE == pParam->enable)
		{
			ret = NI_MDK_VPS_DisableChnMask(VpsGrp, ichannel, index);
			if (ret != RETURN_OK)
			{
				PRINT_ERROR("NI_MDK_VPS_DisableChn failed with %x\n", ret);
				return ret;
			}
		}
		else
		{
			PRINT_ERROR("pParam->enable:[%d] is unknown\n", pParam->enable);
			return -1;
		}
	}

	pRegionDevice->COVER_PARAM[index].rect = pParam->rect;
	pRegionDevice->COVER_PARAM[index].color = pParam->color;
	pRegionDevice->COVER_PARAM[index].enable = pParam->enable;

	return 0;
}

/// 获取上一帧的qp。
/// 
/// \param [in] channel 通道号。
/// \param [in] dwType
/// \param [in] qp 返回上一帧的qp。
/// \retval 0  设置成功
/// \retval 0  设置失败
int VideoGetLastQP(int channel, DWORD dwType, unsigned int * lastqp)
{
#if 0
	int ret,i;
	int EncCnt;	
	unsigned int VencChn;
	//FH_CHN_STATUS pstVencStatus;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
	EncCnt = pCaptureDevice->EncCount;
	
	if (channel >= EncCnt)
	{
		PRINT_ERROR("通道%d入参非法\n",channel);
		return EINVAL;
	}
	
	if (!((pCaptureDevice->EncDevice[channel].SupportStream >> dwType) & 0x01))
	{
		PRINT_ERROR("不支持的码流类型dwType = %d\n",dwType);
		return NOTSUPPORT;
	}
	
	for (i = 0; i < pCaptureDevice->EncDevice[channel].StreamCount; i++)
	{
		if (pCaptureDevice->EncDevice[channel].StreamDevice[i].CapChn == dwType)
		{
			VencChn = i;
		}
	}

	//ret = FH_VENC_GetChnStatus(VencChn, &pstVencStatus);
    if (ret != RETURN_OK)
    {
        PRINT_ERROR("Error(%d - %x): FH_VENC_GetChnStatus\n", ret, ret);
        return GETCHN_ERR;
    }

	//*lastqp = pstVencStatus.lastqp;
#endif
	return 0;
}

/// 设置单一通道的属性。
/// 
/// \param [in] channel 通道号。
/// \param [in] dwType
/// \param [in] QP_FORMAT 设置的参数。
/// \retval 0  设置成功
/// \retval 0  设置失败
int VideoSetQP(int channel, DWORD dwType, QP_FORMAT *pFormat)
{
#if 0
	int ret,i;
	int EncCnt;	
	unsigned int VencChn;
	//FH_VENC_RC_CHANGEPARAM param;
	//FH_VENC_RC_ATTR pstVencrcattr;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
	VideoInDevice_p pVideoInDevice = &GlobalDevice.VideoInDevice;
	EncCnt = pCaptureDevice->EncCount;
	
	if (NULL == pFormat || channel >= EncCnt)
	{
		PRINT_ERROR("通道%d入参非法\n",channel);
		return EINVAL;
	}
	
	if (!((pCaptureDevice->EncDevice[channel].SupportStream >> dwType) & 0x01))
	{
		PRINT_ERROR("不支持的码流类型dwType = %d\n",dwType);
		return NOTSUPPORT;
	}
	
	for (i = 0; i < pCaptureDevice->EncDevice[channel].StreamCount; i++)
	{
		if (pCaptureDevice->EncDevice[channel].StreamDevice[i].CapChn == dwType)
		{
			VencChn = i;
		}
	}
	
	//ret = FH_VENC_GetRCAttr(VencChn,&pstVencrcattr);
    if (ret != RETURN_OK)
    {
        PRINT_ERROR("Error(%d - %x): FH_VENC_GetRCAttr\n", ret, ret);
        return GETRC_ERR;
    }

	param.bitrate = pFormat->BitRate;
	param.ImaxQP  = pFormat->ImaxQP;
	param.IminQP  = pFormat->IminQP; 
	param.PmaxQP  = pFormat->PmaxQP;
	param.PminQP  = pFormat->PminQP;
	switch (pstVencrcattr.rc_type)
	{
		case(FH_RC_H265_AVBR):
			param.I_BitProp	      = pstVencrcattr.h265_avbr.I_BitProp;
			param.P_BitProp       = pstVencrcattr.h265_avbr.P_BitProp;
			param.FrameRate       = pstVencrcattr.h265_avbr.FrameRate;
			param.IFrmMaxBits     = pstVencrcattr.h265_avbr.IFrmMaxBits;
			param.maxrate_percent = pstVencrcattr.h265_avbr.maxrate_percent;
		case(FH_RC_H264_AVBR):
			param.I_BitProp 	  = pstVencrcattr.h265_avbr.I_BitProp;
			param.P_BitProp 	  = pstVencrcattr.h265_avbr.P_BitProp;
			param.FrameRate 	  = pstVencrcattr.h265_avbr.FrameRate;
			param.IFrmMaxBits	  = pstVencrcattr.h265_avbr.IFrmMaxBits;
			param.maxrate_percent = pstVencrcattr.h265_avbr.maxrate_percent;
		break;
		dafult:
			PRINT_ERROR("rc_type: %d not support!\n",pstVencrcattr.rc_type);
			return NOTSUPPORT;
	}
	
	ret = FH_VENC_SetRcChangeParam(VencChn,&param);
    if (ret != RETURN_OK)
    {
        PRINT_ERROR("Error(%d - %x): FH_VENC_SetRcChangeParam\n", ret, ret);
        return SETRC_ERR;
    }
#endif	
	return 0;
}


/// 设置图形模式。
/// 
/// \param [in]  channel 通道号。
/// \param [in]  图像模式。
/// \retval 0  设置成功
/// \retval <0  设置失败
int VideoSetRotate(int channel,ImagMode_e Rotate)
{
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
	pCaptureDevice->Rotate = Rotate;
	int ret = 0;

	for(int i = CHL_MAIN_T; i < CHL_FUNCTION_NUM; i++)
	{
		for (int ichannel = 0; ichannel < pCaptureDevice->EncDevice[channel].StreamCount; ichannel++)
		{
			if (pCaptureDevice->EncDevice[channel].StreamDevice[ichannel].CapChn == i)
			{
				// Forward to video_encoder module
				ret = VideoEncoder_SetRotate(ichannel, Rotate);
				if(ret != 0)
				{
					PRINT_ERROR("VideoEncoder_SetRotate failed for channel %d\n", ichannel);
					return ret;
				}
			}
		}
	}

	return ret;
}

//param [in]  channel 通道号
//param [in]  mirror 镜像，0: 恢复镜像，1: 使能镜像 in
//param [in]  flip 翻转，0: 恢复翻转，1: 使能翻转 in
int VideoSetMirrorAndflip(int channel,int mirror,int flip)
{
	int ret = -1;
	VI_DEV ViDev = 0;
	int flipcurr = -1;
	int mirrorcurr = -1;
	SENSOR_ID SenId = 0;
	VI_DEV_ATTR_S pstDevAttr;
	static int last_flip = -1;
	static int last_mirror = -1;
	SensorDevice_p pSensorDevice = &GlobalDevice.SensorDevice;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

	memset(&pstDevAttr, 0, sizeof(pstDevAttr));

	if(last_mirror == mirror && last_flip == flip)
	{
		return SUCCESS;
	}

	if((0 == mirror || 1 == mirror) && (0 == flip|| 1 == flip)) 
	{
		mirrorcurr = (mirror ^ pCaptureDevice->MirrorAndflip);
		flipcurr = (flip ^ pCaptureDevice->MirrorAndflip);

		ret = NI_MDK_VI_GetDevAttr(ViDev, &pstDevAttr);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("NI_MDK_VI_GetDevAttr failed with %x\n", ret);
			return ret;
		}

		ret = NI_MDK_VI_DisableDev(ViDev);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("NI_MDK_VI_DisableDev failed with %x\n", ret);
			return ret;
		}

		ret = NI_SEN_SetMirror(SenId, mirrorcurr);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("NI_SEN_SetMirror failed with %x\n", ret);
			return ret;
		}

		ret = NI_SEN_SetFlip(SenId, flipcurr);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("NI_SEN_SetFlip failed with %x\n", ret);
			return ret;
		}

		if (SEN_TYPE_OS04L10 == pSensorDevice->SenType)
		{
			if (0 == mirrorcurr)
			{
				if (0 == flipcurr)
				{
					pstDevAttr.enRgbSeq = BAYER_SEQ_BGGR;
				}
				else if (1 == flipcurr)
				{
					pstDevAttr.enRgbSeq = BAYER_SEQ_GRBG;
				}
				else
				{
					PRINT_ERROR("VideoSetMirrorAndflip mirrorcurr %d and flipcurr %d error\n", mirrorcurr, flipcurr);
					return -1;
				}
			}
			else if (1 == mirrorcurr)
			{
				if (0 == flipcurr)
				{
					pstDevAttr.enRgbSeq = BAYER_SEQ_GBRG;
				}
				else if (1 == flipcurr)
				{
					pstDevAttr.enRgbSeq = BAYER_SEQ_RGGB;
				}
				else
				{
					PRINT_ERROR("VideoSetMirrorAndflip mirrorcurr %d and flipcurr %d error\n", mirrorcurr, flipcurr);
					return -1;
				}
			}
			else
			{
				PRINT_ERROR("VideoSetMirrorAndflip mirrorcurr %d and flipcurr %d error\n", mirrorcurr, flipcurr);
				return -1;
			}
		}
		else if (SEN_TYPE_OS04D10 == pSensorDevice->SenType)
		{
			if (0 == mirrorcurr)
			{
				if (0 == flipcurr)
				{
					pstDevAttr.enRgbSeq = BAYER_SEQ_RGGB;
				}
				else if (1 == flipcurr)
				{
					pstDevAttr.enRgbSeq = BAYER_SEQ_GBRG;
				}
				else
				{
					PRINT_ERROR("VideoSetMirrorAndflip mirrorcurr %d and flipcurr %d error\n", mirrorcurr, flipcurr);
					return -1;
				}
			}
			else if (1 == mirrorcurr)
			{
				if (0 == flipcurr)
				{
					pstDevAttr.enRgbSeq = BAYER_SEQ_GRBG;
				}
				else if (1 == flipcurr)
				{
					pstDevAttr.enRgbSeq = BAYER_SEQ_BGGR;
				}
				else
				{
					PRINT_ERROR("VideoSetMirrorAndflip mirrorcurr %d and flipcurr %d error\n", mirrorcurr, flipcurr);
					return -1;
				}
			}
			else
			{
				PRINT_ERROR("VideoSetMirrorAndflip mirrorcurr %d and flipcurr %d error\n", mirrorcurr, flipcurr);
				return -1;
			}
		}
		else
		{
			//do nothing
		}

		ret = NI_MDK_VI_SetDevAttr(ViDev, &pstDevAttr);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("NI_MDK_VI_GetDevAttr failed with %x\n", ret);
			return ret;
		}

		ret = NI_MDK_VI_EnableDev(ViDev);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("NI_MDK_VI_DisableDev failed with %x\n", ret);
			return ret;
		}

		ret = HumanTrackSetMirror(mirror, flip);
		if (ret != RETURN_OK)
		{
			PRINT_ERROR("HumanTrackSetMirror failed with %x\n", ret);
		}
	}
	else 
	{
		PRINT_ERROR("Set Papram error\n");
		return -1;
	}

	last_flip = flip;
	last_mirror = mirror;

	return 0;
}

/// 设置捕获时间和格式。应用程序需要定时调用此函数与捕获的时间进行同步。
/// 
/// \param [in] channel 通道号。
/// \param [in] pTime 指向系统时间结构SYSTEM_TIME的指针。
/// \param [in] datefmt 日期格式，取date_fmt枚举值。
/// \param [in] datespc 日期分割符，取date_spec枚举值。
/// \param [in] timefmt 时间格式，取time_fmt枚举值。
/// \retval 0  设置成功。
/// \retval 0  设置失败。
int VideoSetTime(int channel, const SYSTEM_TIME * pTime, int datefmt,int datespc,int timefmt,int WeekEnable)
{
	return 0;
}

int VideoSetOSDLanguage(int language)
{
	RegionDevice_p pRegionDevice = &GlobalDevice.RegionDevice;
	if(language == 1) {
		printf("Set the language to Chinese\n");
	}else if(language == 0) {
		printf("Set the language to English\n");
	}else {
		printf("No current language\n");
	}
	pRegionDevice->language = language;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
	for (int i = 0; i < pCaptureDevice->EncCount; i++)
	{
		for (int ichannel = 0; ichannel < pCaptureDevice->EncDevice[i].StreamCount; ichannel++)
		{
			//sample_set_channel_tosd(ichannel);
		}
	}

	return 0;
}

/// 设置视频制式。
/// 
/// \param [in] channel 通道号。
/// \param [in] dwStandard 视频制式，取video_standard_t类型的值。
/// \retval 0  设置成功
/// \retval <0  设置失败
int VideoSetVstd(int channel,DWORD dwStandard)
{
	SIZE_S pstSize;
	VideoInDevice_p pVideoInDevice = &GlobalDevice.VideoInDevice;

    pstSize.u32Width = pVideoInDevice->vi_pic.u32Width;
    pstSize.u32Height = pVideoInDevice->vi_pic.u32Height;

    if(dwStandard == VIDEO_STANDARD_PAL)
	{
		pVideoInDevice->VDInfo[channel].ViDstFps = 25;
	}
	else if(dwStandard == VIDEO_STANDARD_NTSC)
	{
		pVideoInDevice->VDInfo[channel].ViDstFps = 30;
	}

	_video_Isp_change(channel,pstSize);

	return 0;
}

int TZSKWrite(int  channel,char *buf,DWORD len)
{
	return 0;
}

int video_in_init(void)
{
	int ret = -1;
	PM_CLOCK_CFG_S pstClkCfg;
	
    SAMPLE_VI_INFO_S stViInfo = {0};
	SensorDevice_p pSensorDevice = &GlobalDevice.SensorDevice;
	VideoInDevice_p pVideoInDevice = &GlobalDevice.VideoInDevice;
	// CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
	
	memset(&pstClkCfg, 0, sizeof(PM_CLOCK_CFG_S));

    stViInfo.enViMode = pVideoInDevice->ViMode;
	stViInfo.stSenCfg.enSenType = pSensorDevice->SenType;
	stViInfo.stSenCfg.enIntfType = VI_INTF_TYPE_MIPI;
	stViInfo.stSenCfg.enHdrMode = HDR_MODE_NONE;
	stViInfo.stSenCfg.u32Width = pVideoInDevice->vi_pic.u32Width;
	stViInfo.stSenCfg.u32Height = pVideoInDevice->vi_pic.u32Height;
	stViInfo.stSenCfg.u32Fps = pVideoInDevice->VDInfo[0].ViDstFps;
	stViInfo.stSenCfg.u32LaneNum = 2;
	stViInfo.stViCfg.enViWorkMode = VI_WORK_MODE_NORMAL;
	stViInfo.stSenCfg.u32I2cDevId = 0;
	stViInfo.stSenCfg.u32RstGpioId = SENSOR_RESET;

	switch (stViInfo.stSenCfg.enSenType)
	{
		case SEN_TYPE_GC4023:
			stViInfo.stSenCfg.u32DevAddr = 0x52;
			stViInfo.stSenCfg.u32ClkFreq = 27000;
			break;
		case SEN_TYPE_OS04L10:
		case SEN_TYPE_OS04D10:
			stViInfo.stSenCfg.u32DevAddr = 0x78;
			stViInfo.stSenCfg.u32ClkFreq = 24000;
			break;
		case SEN_TYPE_GC2083:
			stViInfo.stSenCfg.u32DevAddr = 0x6e;
			stViInfo.stSenCfg.u32ClkFreq = 27000;
			break;
		case SEN_TYPE_GC4653:
			stViInfo.stSenCfg.u32DevAddr = 0x52;
			stViInfo.stSenCfg.u32ClkFreq = 24000;
			break;
		default:
			printf("not support sensor type:%d\n", stViInfo.stSenCfg.enSenType);
			return -1;
	}

#if 0
	ret = NI_MDK_CLOCK_GetClkCfg(&pstClkCfg);
	if (ret != SUCCESS)
	{
		PRINT_ERROR("call NI_MDK_CLOCK_GetClkCfg err %#x\n", ret);
	}

	if ((pCaptureDevice->EncDevice[0].StreamDevice[CHL_MAIN_T].EncChannel_info.max_width > stViInfo.stSenCfg.u32Width) 
	|| (pCaptureDevice->EncDevice[0].StreamDevice[CHL_MAIN_T].EncChannel_info.max_height > stViInfo.stSenCfg.u32Height))
	{
		pstClkCfg.bIspClkEn = NI_TRUE;
		pstClkCfg.u32IspFreq = 148500;
		ret = NI_MDK_CLOCK_SetClkCfg(CLOCK_MASK_SET_ISP_CLK, &pstClkCfg);
		if (ret != SUCCESS)
		{
			PRINT_ERROR("call NI_MDK_CLOCK_SetClkCfg err %#x\n", ret);
		}

		pstClkCfg.bVpsClkEn = NI_TRUE;
		pstClkCfg.u32VpsFreq = 148500;
		ret = NI_MDK_CLOCK_SetClkCfg(CLOCK_MASK_SET_VPS_CLK, &pstClkCfg);
		if (ret != SUCCESS)
		{
			PRINT_ERROR("call NI_MDK_CLOCK_SetClkCfg err %#x\n", ret);
		}

		pstClkCfg.stVi.bViClkEn = NI_TRUE;
		pstClkCfg.stVi.u32ViFreq = 250000;
		ret = NI_MDK_CLOCK_SetClkCfg(CLOCK_MASK_SET_VI_CLK, &pstClkCfg);
		if (ret != SUCCESS)
		{
			PRINT_ERROR("call NI_MDK_CLOCK_SetClkCfg err %#x\n", ret);
		}
	}
	else
	{
		pstClkCfg.bIspClkEn = NI_TRUE;
		pstClkCfg.u32IspFreq = 237000;
		ret = NI_MDK_CLOCK_SetClkCfg(CLOCK_MASK_SET_ISP_CLK, &pstClkCfg);
		if (ret != SUCCESS)
		{
			PRINT_ERROR("call NI_MDK_CLOCK_SetClkCfg err %#x\n", ret);
		}

		pstClkCfg.bVpsClkEn = NI_TRUE;
		pstClkCfg.u32VpsFreq = 250000;
		ret = NI_MDK_CLOCK_SetClkCfg(CLOCK_MASK_SET_VPS_CLK, &pstClkCfg);
		if (ret != SUCCESS)
		{
			PRINT_ERROR("call NI_MDK_CLOCK_SetClkCfg err %#x\n", ret);
		}

		pstClkCfg.stVi.bViClkEn = NI_TRUE;
		pstClkCfg.stVi.u32ViFreq = 333000;
		ret = NI_MDK_CLOCK_SetClkCfg(CLOCK_MASK_SET_VI_CLK, &pstClkCfg);
		if (ret != SUCCESS)
		{
			PRINT_ERROR("call NI_MDK_CLOCK_SetClkCfg err %#x\n", ret);
		}
	}

	//由于编码时钟自适应的会出现降帧恢复后主码流无流问题，这边关闭时钟自适应，将编码时钟开启到最大
	pstClkCfg.stVenc.bCpuVClkEn = NI_FALSE;
	pstClkCfg.stVenc.bEncClkEn = NI_TRUE;
	pstClkCfg.stVenc.bJpegClkEn = NI_TRUE;
	pstClkCfg.stVenc.u32CpuVFreq = -1;
	pstClkCfg.stVenc.u32EncFreq = 594000;
	pstClkCfg.stVenc.u32JpegFreq = 50000;
	ret = NI_MDK_CLOCK_SetClkCfg(CLOCK_MASK_SET_VENC_H26X_CLK, &pstClkCfg);
	if (ret != SUCCESS)
	{
		PRINT_ERROR("call NI_MDK_CLOCK_SetClkCfg err %#x\n", ret);
	}

#endif

	ret = SDK_COMM_VI_Config(0,&stViInfo);
	if(ret != SUCCESS)
	{
		PRINT_ERROR("call SDK_COMM_VI_Config err %#x\n", ret);
	}
	
	ret = SDK_COMM_VI_Start(0);
	if(ret != SUCCESS)
	{
		PRINT_ERROR("call SDK_COMM_VI_Start err %#x\n", ret);
	}
	
	ret = isp_init(stViInfo);
	if(ret != NI_SUCCESS)
    {
		PRINT_ERROR("isp_init err %#x\n", ret);
    }

	ret = video_vps_start();
	if (ret == SUCCESS)
	{
		ret = video_venc_start();
	}

	VPS_GRP VpsGrp = 0;
	VPS_CHN_ATTR_S stVdChnAttr;

	memset(&stVdChnAttr, 0, sizeof(VPS_CHN_ATTR_S));

	stVdChnAttr.bCropEn = NI_FALSE;
	stVdChnAttr.stCropRect.s32X = 0;
	stVdChnAttr.stCropRect.s32Y = 0;
	stVdChnAttr.stCropRect.u32Width = pSensorDevice->MaxWidth;
	stVdChnAttr.stCropRect.u32Height = pSensorDevice->MaxHeight;
	stVdChnAttr.enAttrType = VPS_CHN_ATTR_TYPE_VD;
	stVdChnAttr.u32OutWidth = 352;
	stVdChnAttr.u32OutHeight = 288;
	stVdChnAttr.stVdAttr.enFilterWin = VPS_FILTER_WIN_TYPE_LEVEL_3;
	stVdChnAttr.stVdAttr.u32BuffNum = 3;
	stVdChnAttr.stVdAttr.u32OutFps = pVideoInDevice->VDInfo[0].ViDstFps;
	stVdChnAttr.stVdAttr.u32Stride = stVdChnAttr.u32OutWidth;
	ret = NI_MDK_VPS_CreateChn(VpsGrp, VPS_CHN_VD, &stVdChnAttr);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("[ERROR]: NI_MDK_VPS_CreateChn failed, ret=%x\n", ret);

	}

	ret = NI_MDK_VPS_EnableChn(VpsGrp, VPS_CHN_VD);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("[ERROR]: NI_MDK_VPS_EnableChn failed, ret=%x\n", ret);

	}

	return ret;
}

int video_vps_start(void)
{
	int chan_vpu = 0;
	int ret = -1,i;
	int EncCnt = 0;
	int channel = 0;
	TEST_VPS_CONFIG_S stVpsCfg;
	TEST_VENC_CONFIG_S stVencCfg;
	struct channel_info* info;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
	SensorDevice_p pSensorDevice = &GlobalDevice.SensorDevice;
	EncCnt = pCaptureDevice->EncCount;

	memset(&stVpsCfg,0,sizeof(TEST_VPS_CONFIG_S));
	memset(&stVencCfg,0,sizeof(TEST_VENC_CONFIG_S));
	for(i = 0;i < EncCnt;i++)
	{
		for(chan_vpu = 0; chan_vpu < pCaptureDevice->EncDevice[i].StreamCount; chan_vpu++)
		{
			info = &pCaptureDevice->EncDevice[i].StreamDevice[chan_vpu].EncChannel_info;
			
			//vps配置
			stVpsCfg.u32InFps = pSensorDevice->MaxFPS;
			stVpsCfg.u32OutFps = info->frame_count;
			stVpsCfg.u32InWidth = pSensorDevice->MaxWidth;
			stVpsCfg.u32InHeight = pSensorDevice->MaxHeight;
			stVpsCfg.u32OutWidth = info->max_width;
			stVpsCfg.u32OutHeight = info->max_height;
			stVpsCfg.enOutFormat = info->yuv_type;
			stVpsCfg.bOnLineMode = NI_FALSE;
       		stVpsCfg.bYuv2Ldc = NI_FALSE;
			stVpsCfg.enPayLoad = info->enc_type;
			channel = info->channel;
			if (CHL_3IRD_T == info->channel)
			{
				channel = VPS_CHN_IAS0;
				stVpsCfg.u32OutWidth = info->width;
				stVpsCfg.u32OutHeight = info->height;
			}
			else
			{
				stVpsCfg.enEncBindChn = channel;
			}

			ret = SDK_COMM_VPS_Start(0, channel, &stVpsCfg);
			if (ret != NI_SUCCESS)
			{
				PRINT_ERROR("SDK_COMM_VPS_Start fail channel is %d err %#x\n", channel, ret);
				return ret;
			}

		}
	}

	// 这边增加YUV数据管理，不使用无影响
	VPS_GRP VpsGrp = 0;
	VPS_CHN_E VpsChn = VPS_CHN_VIRTIAS0;
	NI_MDK_VPS_EnableChn(VpsGrp, VpsChn);

	return ret;
}

int video_venc_start(void)
{
	int venc_channel = 0;
	int ret = -1,i;
	int EncCnt = 0;
	TEST_VENC_CONFIG_S stVencCfg;
	struct channel_info* info;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
	EncCnt = pCaptureDevice->EncCount;

	memset(&stVencCfg,0,sizeof(TEST_VENC_CONFIG_S));

	for(i = 0;i < EncCnt;i++)
	{
		for(venc_channel = 0;venc_channel < pCaptureDevice->EncDevice[i].StreamCount;venc_channel++)
		{
			info = &pCaptureDevice->EncDevice[i].StreamDevice[venc_channel].EncChannel_info;			
			//VENC 配置
			stVencCfg.enEncType = info->yuv_type;
			stVencCfg.u32InFps = info->frame_count;
			stVencCfg.u32OutFps = stVencCfg.u32InFps;
			stVencCfg.u32InHeight = info->max_height;
			stVencCfg.u32InWidth = info->max_width;
			stVencCfg.enRcMode = VENC_RC_MODE_CBR;
			stVencCfg.enPayLoad = info->enc_type;

			if (CHL_3IRD_T == venc_channel)
			{
				MDK_CHN_S pstSrcChn;
				MDK_CHN_S pstDestChn;
				memset(&pstSrcChn, 0, sizeof(MDK_CHN_S));
				memset(&pstDestChn, 0, sizeof(MDK_CHN_S));

				pstSrcChn.enModId = NI_ID_VPS;
				pstSrcChn.s32ChnId = 1;
				pstSrcChn.s32DevId = 0;

				pstDestChn.enModId = NI_ID_VENC;
				pstDestChn.s32DevId = 0;
				pstDestChn.s32ChnId = venc_channel;

				ret = NI_MDK_SYS_Bind(&pstSrcChn, &pstDestChn);
				if (ret != NI_SUCCESS)
				{
					PRINT_ERROR("call NI_MDK_SYS_Bind err %#x\n", ret);
				}
			}	

			if (CHL_3IRD_T == venc_channel)
			{
				stVencCfg.u32OutFps  = 25;
			}		
			ret = SDK_COMM_VENC_Start(venc_channel, &stVencCfg);
			if (ret != NI_SUCCESS)
			{
				PRINT_ERROR("call SDK_COMM_VENC_Start err %#x\n", ret);
			}
		}
	}

	return ret;
}

int video_venc_adaptive_start()
{
	VencAdaptiveRunning = TRUE;
	return pthread_create(&venc_pid, 0, _venc_adaptive_process, NULL);
}

int video_venc_adaptive_stop()
{
	//在升级时销毁编码自适应线程
	if(TRUE == VencAdaptiveRunning)
	{
		VencAdaptiveRunning = FALSE;
        pthread_join(venc_pid,0);
	}

	return 0;
}

int get_frame_rate(float FrameRate)
{
	unsigned int u32CurFrame;

	u32CurFrame = (unsigned int)FrameRate;
	if (u32CurFrame != gFrameRate)
	{
		if((gFrameRate == (unsigned int)((float)FrameRate * 2)) || gFrameRate == (u32CurFrame / 2))
		{
			pthread_rwlock_wrlock(&ChangeFrmLock);
			ChangeFrmRate = TRUE;
			pthread_rwlock_unlock(&ChangeFrmLock);
		}

		gFrameRate = u32CurFrame;
		PRINT_ERROR("gFrameRate %d\n", gFrameRate);
	}

	return SUCCESS;
}

int osd_create()
{
	OSD_REGION_S stOsdRegion;
	unsigned int u32OsdHandle = 0;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

	memset(&stOsdRegion,0,sizeof(OSD_REGION_S));

	stOsdRegion.bMacroFormat 	= NI_FALSE;
	stOsdRegion.enOsdRgn 		= OSD_RGN;
	stOsdRegion.enPixelFormat 	= OSD_PIXEL_FORMAT_2BIT;

	for (int i = 0; i < pCaptureDevice->EncDevice[0].StreamCount; i++)
	{
		//跳过编码通道
		if (i == CHL_3IRD_T)
		{
			break;
		}
		for (int j = 0; j < 4; j++)
		{
			NI_MDK_OSD_Create(u32OsdHandle, &stOsdRegion);
			u32OsdHandle++;
		}
	}

	return SUCCESS;
}

int osd_destroy()
{
	MDK_CHN_S stMdkChn;
	unsigned int u32OsdHandle = 0;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

	memset(&stMdkChn,0,sizeof(MDK_CHN_S));
	stMdkChn.enModId = NI_ID_VPS;
	stMdkChn.s32DevId = 0;
	stMdkChn.s32ChnId = 0;

	pthread_mutex_lock(&osd_lock);

	for (int i = 0; i < pCaptureDevice->EncDevice[0].StreamCount; i++)
	{
		//跳过编码通道
		if (i == CHL_3IRD_T)
		{
			break;
		}

		for (int j = 0; j < 4; j++)
		{
			NI_MDK_OSD_ClearFromChn(u32OsdHandle, &stMdkChn, 500);
			NI_MDK_OSD_Destroy(u32OsdHandle);
			u32OsdHandle++;
		}
		stMdkChn.s32ChnId++;
	}

	return SUCCESS;
}

int logo_create()
{
	OSD_REGION_S stOsdRegion;
	unsigned int u32OsdHandle = 9;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

	memset(&stOsdRegion, 0, sizeof(OSD_REGION_S));

	stOsdRegion.bMacroFormat = NI_FALSE;
	stOsdRegion.enOsdRgn = OSD_RGN;
	stOsdRegion.enPixelFormat = OSD_PIXEL_FORMAT_4BIT;

	for (int i = 0; i < pCaptureDevice->EncDevice[0].StreamCount; i++)
	{
		// 跳过编码通道
		if (i == CHL_3IRD_T)
		{
			break;
		}
		NI_MDK_OSD_Create(u32OsdHandle, &stOsdRegion);
		u32OsdHandle++;
	}

	return SUCCESS;
}

int logo_destroy()
{
	MDK_CHN_S stMdkChn;
	unsigned int u32OsdHandle = 9;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
	RegionDevice_p pRegionDevice = &GlobalDevice.RegionDevice;

	memset(&stMdkChn, 0, sizeof(MDK_CHN_S));
	stMdkChn.enModId = NI_ID_VPS;
	stMdkChn.s32DevId = 0;
	stMdkChn.s32ChnId = 0;

	for (int i = 0; i < pCaptureDevice->EncDevice[0].StreamCount; i++)
	{
		// 跳过编码通道
		if (i == CHL_3IRD_T)
		{
			break;
		}

		NI_MDK_OSD_ClearFromChn(u32OsdHandle, &stMdkChn, 500);
		NI_MDK_OSD_Destroy(u32OsdHandle);

		u32OsdHandle++;
		stMdkChn.s32ChnId++;
	}
	pRegionDevice->LogoInit = FALSE;

	return SUCCESS;
}