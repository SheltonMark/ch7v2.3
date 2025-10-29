//#include "Capture.h"
#include "TdCommon.h"
#include "cap_com.h"
#include "sdk_comm.h"
#include "ni_ans.h"
#include "ni_aec.h"
#include "ni_agc.h"
#include "ni_hpf.h"
#include "../video/video_config.h"

#define STREAM_BUFF_SIZE 512 * 1024
#define JPEG_BUFF_SIZE 100 * 1024

int StreamBuffSiZeTable[] = {512 * 1024, 256 * 1024, 100 * 1024};

int jpeglen = 0;
unsigned long long jpeg_pts = 0;
unsigned char jpeg_buff[JPEG_BUFF_SIZE + 1] = {0};

extern int AecStart;
extern unsigned int crc32_table[256];
extern int ias_ad_write_pcm(unsigned char *pcm_buff, unsigned int pcm_length);
//由于CP3无降帧逻辑，降帧回调函数不会被调用，所以这边将初始化帧率设置为30帧，防止帧率为0导致设备无法出图

static int Packets_AudioStreamType_ConToLib_Table[]={
													[AudioStreamType_PCM]   = Packets_AudioStreamType_PCM16,
													[AudioStreamType_G711A] = Packets_AudioStreamType_G711A,
													[AudioStreamType_G711U] = Packets_AudioStreamType_G711U,
													[AudioStreamType_AAC]   = Packets_AudioStreamType_AAC,
													[AudioStreamType_G726]  = Packets_AudioStreamType_G726_16K
													};

static int Packets_AudioRate_ConToLib_Table[] = {
												[AC_SR_8K] = Packets_AudioRate_8K,
												[AC_SR_16K] = Packets_AudioRate_16K,
												[AC_SR_32K] = Packets_AudioRate_32K,
												[AC_SR_48K] = Packets_AudioRate_48K,
												[AC_SR_110K] = Packets_AudioRate_110K,
												[AC_SR_220K] = Packets_AudioRate_220K,
												[AC_SR_441K] = Packets_AudioRate_441K
												};

static BOOL g_audio_stream_running = FALSE;
static BOOL g_video_stream_running[VENC_MAX_CHN_NUM] = {FALSE};
static pthread_t g_audio_stream_pid;
static pthread_t g_video_stream_pid[VENC_MAX_CHN_NUM];
static int g_video_stream_channel[VENC_MAX_CHN_NUM];
pthread_mutex_t audio_lock;
pthread_mutex_t jpeg_lock;
pthread_rwlock_t jpeg_rwlock = PTHREAD_RWLOCK_INITIALIZER;


unsigned int _crc_32_check(unsigned char* buffer, unsigned int len, unsigned int crc)
{

	unsigned int i;	
	if ((buffer != NULL) && (len != 0))
	{
		for (i = 0; i < len; i ++)
		{
			crc = crc32_table[(crc ^ buffer[i]) & 0xff] ^ (crc >> 8);
		}
	}
	return ~crc;
}

unsigned int _capture_sum_32_check(unsigned char* buffer, unsigned int len)
{
	unsigned int ret;
	unsigned int num = (len >> 2);
	unsigned int res = len & 0x3;
	unsigned int i = 0;
	static const unsigned int mask[4] = {0x0, 0x000000ff, 0x0000ffff, 0x00ffffff};
	unsigned int *p = (unsigned int *)buffer;

	if (res > 0)
	{
		ret = p[num] & mask[res];
	}
	else
	{
		ret = 0;
	}

	for ( i= 0; i < num; i ++)
	{
		ret += *(p + i);
	}

	return ret;
}

static void _capture_crc_result_to_header(Water_Verify_p pWaterVerify, unsigned int crc, Waterverify_Type_e type)
{
	if (pWaterVerify != NULL)
	{
		pWaterVerify->verify_result[0] = crc % 256;
		pWaterVerify->verify_result[1] = (crc>> 8) % 256;
		pWaterVerify->verify_result[2] = (crc >> 16) % 256;
		pWaterVerify->verify_result[3] = (crc>> 24);
		pWaterVerify->reserved1 = 0x0;
		pWaterVerify->reserved2 = 0x0;
		pWaterVerify->verify_type= type;	
		pWaterVerify->type = 0x88;
	}
}


int _capture_head_to_packets(int Chn, StreamType_e Streamtype, AV_SLICE_TYPE_e AVType, void *buffer,int StreamLen
	, PRI_STREAM_PACK_p PriStreamPack,AV_SLICE_HEADER_p pAvHeader,AV_SLICE_TAILS_p pAvTail, int PacketSize, int *PacketLength, int *DataLength, SLICE_LOCATION_t *pSliceLocation
	,uint64 pts)
{
	//VENC_STREAM_S *pstStream = NULL;
//	FH_AC_FRAME_S *pAencStream = NULL;

	int PriStreamPackCnt = 1;
	SLICE_LOCATION_t SliceLocation;
	int TotalLen = 0;
	VENC_CHN_ATTR_S pstChnAttr;
	int i;
	unsigned int ms;
	unsigned int avStreamCrcRest = 0;
	//unsigned int avStreamSumRest = 0;

	//unsigned short IVSRegionLen = 0;

	//unsigned char *pFrameinfoIVSRegion = NULL;
	//unsigned int *pPoint = NULL;
	//unsigned int Point = 0;
	//int j = 0;
	//memset(HeadBuffer, 0, 1024*2);
	
	time_t clock;
	HI_S32 s32Ret;
	struct tm tm = {0};
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
	AiDevice_p pAiDevice = &GlobalDevice.AiDevice;
	unsigned int current_fps = VideoConfig_GetFrameRate();

	SLINCE_DATE_TIME_t SlinceDateTime;
	memset(pAvHeader, 0, sizeof(AV_SLICE_HEADER_t));
	memset(pAvTail, 0, sizeof(AV_SLICE_TAILS_t));
	memset(&SlinceDateTime, 0, sizeof(SLINCE_DATE_TIME_t));
	int EncChn = pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].EncChannel_info.channel;

//	unsigned int avSequence;//序列号
	//unsigned int AligneV =0;
	//unsigned int TotalLenTmp =0;
	//视频
	pAvHeader->Type = AVType;
//	static int u32Seq = 0;

	if (AVType == AV_SLICE_TYPE_V_H264_I || AVType == AV_SLICE_TYPE_V_H264_P || AVType == AV_SLICE_TYPE_V_JPEG \
		|| AVType == AV_SLICE_TYPE_V_H265_I || AVType == AV_SLICE_TYPE_V_H265_P)
	{
		//pAVSequence = &pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].VSequence;
		//pstStream = (VENC_STREAM_S *)buffer;
//		avSequence = u32Seq;
		if (1 + 2 > PacketSize)
		{
			printf("海思视频码流包的个数太多，封装失败\n");
			return -1;
		}
		
		for (i = 0; i < 1; i ++)
		{
			//if (pstStream->pstPack[i].u32Len[0] != 0)
			{
				TotalLen += StreamLen; 			
				PriStreamPack[PriStreamPackCnt].addr = (unsigned char *)buffer;
				PriStreamPack[PriStreamPackCnt].offset = 0;
				PriStreamPack[PriStreamPackCnt].length = StreamLen;
				PriStreamPack[PriStreamPackCnt].unusedLen = PriStreamPack[PriStreamPackCnt].length;

				(PriStreamPackCnt)++;
			}
			//TotalLenTmp = TotalLen;
		}
		//avStreamCrcRest = ~avStreamCrcRest;
		if (AVType == AV_SLICE_TYPE_V_H264_P)
		{
			pAvHeader->ComHeader.expandLength = 8;
			pAvHeader->ComHeader.avSliceType = 0xFC;
			
			if (pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].WaterMark.enable)
			{
				pAvHeader->ComHeader.expandLength += 8;
			}

			pAvHeader->ExternHeader.H264PSlice.PicMask = 0x80;
			pAvHeader->ExternHeader.H264PSlice.VStreamMask = 0x81;
			pAvHeader->ExternHeader.H264PSlice.VStreamRev = pCaptureDevice->VencStreamInfo[Streamtype].reframe[Chn];
			pAvHeader->ExternHeader.H264PSlice.VStreamType = 0x02;

			switch(BASE_PSLICE_REFBYBASE)
			{
				case BASE_IDRSLICE:
					pAvHeader->ComHeader.avSubSliceType = PACK_GENERAL_PSLICE;
					pCaptureDevice->VencStreamInfo[Streamtype].relFrames[Chn]++;
					pAvHeader->ComHeader.resv0 = pCaptureDevice->VencStreamInfo[Streamtype].relFrames[Chn];
				break;
#ifdef CHIP_TYPE_hi3521dv100
				case BASE_PSLICE_REFTOIDR:
					pAvHeader->ComHeader.avSubSliceType = PACK_BASE_REFTOIDR;
					pCaptureDevice->VencStreamInfo[Streamtype].relFrames[Chn]++;
					pAvHeader->ComHeader.resv0 = pCaptureDevice->VencStreamInfo[Streamtype].relFrames[Chn];
				break;
#endif
				case BASE_PSLICE_REFBYBASE:
					pAvHeader->ComHeader.avSubSliceType = PACK_BASE_PSLICE_REFBYBASE;
					pCaptureDevice->VencStreamInfo[Streamtype].relFrames[Chn]++;
					pAvHeader->ComHeader.resv0 = pCaptureDevice->VencStreamInfo[Streamtype].relFrames[Chn];
				break;
				case BASE_PSLICE_REFBYENHANCE:
					pAvHeader->ComHeader.avSubSliceType = PACK_BASE_PSLICE_REFBYENHANCE;
					pCaptureDevice->VencStreamInfo[Streamtype].relFrames[Chn]++;
					pAvHeader->ComHeader.resv0 = pCaptureDevice->VencStreamInfo[Streamtype].relFrames[Chn];
				break;
				case ENHANCE_PSLICE_REFBYENHANCE:
					pAvHeader->ComHeader.avSubSliceType = PACK_ENHANCE_PSLICE_REFBYENHANCE;
					pCaptureDevice->VencStreamInfo[Streamtype].relFrames[Chn]++;
					pAvHeader->ComHeader.resv0 = pCaptureDevice->VencStreamInfo[Streamtype].relFrames[Chn];
				break;	 
				case ENHANCE_PSLICE_NOTFORREF:
					pAvHeader->ComHeader.avSubSliceType = PACK_ENHANCE_PSLICE_NOTFORREF;
					pCaptureDevice->VencStreamInfo[Streamtype].relFrames[Chn]++;
					pAvHeader->ComHeader.resv0 = pCaptureDevice->VencStreamInfo[Streamtype].relFrames[Chn];
				break;					
				default:
					pAvHeader->ComHeader.avSubSliceType = PACK_GENERAL_PSLICE;
					pCaptureDevice->VencStreamInfo[Streamtype].relFrames[Chn]++;
					pAvHeader->ComHeader.resv0 = pCaptureDevice->VencStreamInfo[Streamtype].relFrames[Chn];
					printf("Unknown P frame type %d!\n",BASE_IDRSLICE);
				break;
			}

			s32Ret = NI_MDK_VENC_GetChnAttr(EncChn, &pstChnAttr);
			if(s32Ret != HI_SUCCESS)
			{
				printf("NI_MDK_VENC_GetChnAttr chn[%d] failed with %#x!\n", EncChn, s32Ret);
				return -1;
			}

			pAvHeader->ExternHeader.H264PSlice.PicInterlace = 0;
			if (pCaptureDevice->Rotate == Mode_Rotate_90 || pCaptureDevice->Rotate == Mode_Rotate_270)
			{
				pAvHeader->ExternHeader.H264PSlice.PicWidth = pstChnAttr.stVeAttr.stInputPicAttr.u32PicHeight / 8;
				pAvHeader->ExternHeader.H264PSlice.PicHeight = pstChnAttr.stVeAttr.stInputPicAttr.u32PicWidth / 8;
			}
			else
			{
				pAvHeader->ExternHeader.H264PSlice.PicWidth = pstChnAttr.stVeAttr.stInputPicAttr.u32PicWidth / 8;
				pAvHeader->ExternHeader.H264PSlice.PicHeight = pstChnAttr.stVeAttr.stInputPicAttr.u32PicHeight / 8;
			}
			/*if(EncChn == pCaptureDevice->exVencFun[0].Venc)
			{
				pAvHeader->ExternHeader.H264PSlice.VFps = pCaptureDevice->exVencFun[0].Format.FramesPerSecond;
			}
			else*/
			{
				if ((current_fps < pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].Format.FramesPerSecond) && (0 != current_fps))
				{
					pAvHeader->ExternHeader.H264PSlice.VFps = current_fps;
				}
				else
				{
					pAvHeader->ExternHeader.H264PSlice.VFps = pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].Format.FramesPerSecond;
				}
			}
			
			SliceLocation.FrameType = 0; // P帧

			if (pAvHeader->ExternHeader.H264PSlice.PicWidth == 0 || pAvHeader->ExternHeader.H264PSlice.PicHeight == 0 
				|| (pstChnAttr.stVeAttr.stInputPicAttr.u32PicWidth >> 3) > 0xff || (pstChnAttr.stVeAttr.stInputPicAttr.u32PicHeight >> 3) > 0xff)
			{
				pAvHeader->ComHeader.expandLength += 4;
				pAvHeader->ExternHeader.JpgeSlice.VStreamMask = 0x81;
				pAvHeader->ExternHeader.JpgeSlice.VStreamType = 0x02;
				pAvHeader->ExternHeader.JpgeSlice.VStreamRev = pCaptureDevice->VencStreamInfo[Streamtype].reframe[Chn];
				if ((current_fps < pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].EncChannel_info.frame_count) && (0 != current_fps))
				{
					pAvHeader->ExternHeader.JpgeSlice.VFps = current_fps;
				}
				else
				{
					pAvHeader->ExternHeader.JpgeSlice.VFps = pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].EncChannel_info.frame_count;
				}

				pAvHeader->ExternHeader.JpgeSlice.PicMask = 0x82;
				pAvHeader->ExternHeader.JpgeSlice.PicInterlace = 0;
				pAvHeader->ExternHeader.JpgeSlice.Rev1 = 0;
				pAvHeader->ExternHeader.JpgeSlice.Rev2 = 0;
				if (pCaptureDevice->Rotate == Mode_Rotate_90 || pCaptureDevice->Rotate == Mode_Rotate_270)
				{
					pAvHeader->ExternHeader.JpgeSlice.PicWidth[0] = (pstChnAttr.stVeAttr.stInputPicAttr.u32PicHeight & 0xff);
					pAvHeader->ExternHeader.JpgeSlice.PicWidth[1] = pstChnAttr.stVeAttr.stInputPicAttr.u32PicHeight / 256;
					pAvHeader->ExternHeader.JpgeSlice.PicHeight[0] = (pstChnAttr.stVeAttr.stInputPicAttr.u32PicWidth & 0xff);
					pAvHeader->ExternHeader.JpgeSlice.PicHeight[1] = pstChnAttr.stVeAttr.stInputPicAttr.u32PicWidth / 256;
				}
				else
				{
					pAvHeader->ExternHeader.JpgeSlice.PicWidth[0] = (pstChnAttr.stVeAttr.stInputPicAttr.u32PicWidth & 0xff);
					pAvHeader->ExternHeader.JpgeSlice.PicWidth[1] = pstChnAttr.stVeAttr.stInputPicAttr.u32PicWidth / 256;
					pAvHeader->ExternHeader.JpgeSlice.PicHeight[0] = (pstChnAttr.stVeAttr.stInputPicAttr.u32PicHeight & 0xff);
					pAvHeader->ExternHeader.JpgeSlice.PicHeight[1] = pstChnAttr.stVeAttr.stInputPicAttr.u32PicHeight / 256;
				}
			}
		}
		else if (AVType == AV_SLICE_TYPE_V_H264_I)
		{
			pAvHeader->ComHeader.expandLength = 8;
			pAvHeader->ComHeader.avSliceType = 0xFD;
			pAvHeader->ComHeader.avSubSliceType = 0x0;
			pCaptureDevice->VencStreamInfo[Streamtype].refIdrFrames[Chn] = 0;
			pCaptureDevice->VencStreamInfo[Streamtype].relFrames[Chn] = 0;
			pAvHeader->ComHeader.resv0 = 0;
			if (pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].WaterMark.enable)
			{
				pAvHeader->ComHeader.expandLength += 8;
			}
			pAvHeader->ExternHeader.H264ISlice.PicMask = 0x80;
			pAvHeader->ExternHeader.H264ISlice.VStreamMask= 0x81;
			pAvHeader->ExternHeader.H264ISlice.VStreamRev = pCaptureDevice->VencStreamInfo[Streamtype].reframe[Chn];

			pAvHeader->ExternHeader.H264ISlice.VStreamType = 0x02;
			
			s32Ret = NI_MDK_VENC_GetChnAttr(EncChn, &pstChnAttr);
			if(s32Ret != HI_SUCCESS)
			{
				printf("NI_MDK_VENC_GetChnAttr chn[%d] failed with %#x!\n", EncChn, s32Ret);
				return -1;
			}
			//stVencChnAttr.stVencAttr.u32PicWidth = 352;
			//stVencChnAttr.stVencAttr.u32PicHeight = 288;
			if(pstChnAttr.stVeAttr.stInputPicAttr.u32PicWidth== 1440 && pstChnAttr.stVeAttr.stInputPicAttr.u32PicHeight == 900)
			{
				pstChnAttr.stVeAttr.stInputPicAttr.u32PicHeight = pstChnAttr.stVeAttr.stInputPicAttr.u32PicHeight + 4;
			}
			pAvHeader->ExternHeader.H264ISlice.PicInterlace = 0;
			if(pCaptureDevice->Rotate == Mode_Rotate_90 || pCaptureDevice->Rotate == Mode_Rotate_270)
			{
				pAvHeader->ExternHeader.H264ISlice.PicWidth = pstChnAttr.stVeAttr.stInputPicAttr.u32PicHeight / 8;
				pAvHeader->ExternHeader.H264ISlice.PicHeight = pstChnAttr.stVeAttr.stInputPicAttr.u32PicWidth / 8;
			}
			else
			{				
				pAvHeader->ExternHeader.H264ISlice.PicWidth = pstChnAttr.stVeAttr.stInputPicAttr.u32PicWidth / 8;
				pAvHeader->ExternHeader.H264ISlice.PicHeight = pstChnAttr.stVeAttr.stInputPicAttr.u32PicHeight / 8;
			}
			/*if(EncChn == pCaptureDevice->exVencFun[0].Venc)
			{
				pAvHeader->ExternHeader.H264ISlice.VFps = pCaptureDevice->exVencFun[0].Format.FramesPerSecond;
			}
			else*/
			{
				if ((current_fps < pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].EncChannel_info.frame_count) && (0 != current_fps))
				{
					pAvHeader->ExternHeader.H264ISlice.VFps = current_fps;
				}
				else
				{
					pAvHeader->ExternHeader.H264ISlice.VFps = pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].EncChannel_info.frame_count;
				}
			}
			SliceLocation.FrameType = 1;
			if(pAvHeader->ExternHeader.H264ISlice.PicWidth == 0 || pAvHeader->ExternHeader.H264ISlice.PicHeight == 0
				|| (pstChnAttr.stVeAttr.stInputPicAttr.u32PicWidth >>3) > 0xff || (pstChnAttr.stVeAttr.stInputPicAttr.u32PicHeight >>3) > 0xff)
			{	
				pAvHeader->ComHeader.expandLength += 4;
				pAvHeader->ExternHeader.JpgeSlice.VStreamMask = 0x81;
				pAvHeader->ExternHeader.JpgeSlice.VStreamType = 0x02;
				pAvHeader->ExternHeader.JpgeSlice.VStreamRev = pCaptureDevice->VencStreamInfo[Streamtype].reframe[Chn];
				if ((current_fps < pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].EncChannel_info.frame_count) && (0 != current_fps))
				{
					pAvHeader->ExternHeader.JpgeSlice.VFps = current_fps;
				}
				else
				{
					pAvHeader->ExternHeader.JpgeSlice.VFps = pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].EncChannel_info.frame_count;
				}
				pAvHeader->ExternHeader.JpgeSlice.PicMask = 0x82;
				pAvHeader->ExternHeader.JpgeSlice.PicInterlace = 0;
				pAvHeader->ExternHeader.JpgeSlice.Rev1 = 0;
				pAvHeader->ExternHeader.JpgeSlice.Rev2 = 0;
				if(pCaptureDevice->Rotate == Mode_Rotate_90 || pCaptureDevice->Rotate == Mode_Rotate_270)
				{					
					pAvHeader->ExternHeader.JpgeSlice.PicWidth[0] =  (pstChnAttr.stVeAttr.stInputPicAttr.u32PicHeight & 0xff);
					pAvHeader->ExternHeader.JpgeSlice.PicWidth[1] = pstChnAttr.stVeAttr.stInputPicAttr.u32PicHeight / 256;
					pAvHeader->ExternHeader.JpgeSlice.PicHeight[0] = (pstChnAttr.stVeAttr.stInputPicAttr.u32PicWidth & 0xff);
					pAvHeader->ExternHeader.JpgeSlice.PicHeight[1] = pstChnAttr.stVeAttr.stInputPicAttr.u32PicWidth /256;
				}
				else
				{
					pAvHeader->ExternHeader.JpgeSlice.PicWidth[0] = (pstChnAttr.stVeAttr.stInputPicAttr.u32PicWidth & 0xff);
					pAvHeader->ExternHeader.JpgeSlice.PicWidth[1] = pstChnAttr.stVeAttr.stInputPicAttr.u32PicWidth/256;
					pAvHeader->ExternHeader.JpgeSlice.PicHeight[0] = (pstChnAttr.stVeAttr.stInputPicAttr.u32PicHeight & 0xff);
					pAvHeader->ExternHeader.JpgeSlice.PicHeight[1] = pstChnAttr.stVeAttr.stInputPicAttr.u32PicHeight/ 256;
				}
			}
		}
		else if(AVType == AV_SLICE_TYPE_V_H265_P)
		{
			//int RefType = 0;

			pAvHeader->ComHeader.expandLength = 8;
			pAvHeader->ComHeader.avSliceType = 0xFC;
			if (pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].WaterMark.enable)
			{
				pAvHeader->ComHeader.expandLength += 8;
			}

			pAvHeader->ExternHeader.H264PSlice.PicMask = 0x80;
			pAvHeader->ExternHeader.H264PSlice.VStreamMask = 0x81;
			pAvHeader->ExternHeader.H264PSlice.VStreamRev = pCaptureDevice->VencStreamInfo[Streamtype].reframe[Chn];

			pAvHeader->ExternHeader.H264PSlice.VStreamType = 0x0C;
			switch(BASE_IDRSLICE)
			{
				case BASE_IDRSLICE:
					pAvHeader->ComHeader.avSubSliceType = PACK_GENERAL_PSLICE;
					pCaptureDevice->VencStreamInfo[Streamtype].relFrames[Chn]++;
					pAvHeader->ComHeader.resv0 = pCaptureDevice->VencStreamInfo[Streamtype].relFrames[Chn];
				break;
#ifdef CHIP_TYPE_hi3521dv100
				case BASE_PSLICE_REFTOIDR:
					pAvHeader->ComHeader.avSubSliceType = PACK_BASE_REFTOIDR;
					pCaptureDevice->VencStreamInfo[Streamtype].relFrames[Chn]++;
					pAvHeader->ComHeader.resv0 = pCaptureDevice->VencStreamInfo[Streamtype].relFrames[Chn];
				break;
#endif
				case BASE_PSLICE_REFBYBASE:
					pAvHeader->ComHeader.avSubSliceType = PACK_BASE_PSLICE_REFBYBASE;
					pCaptureDevice->VencStreamInfo[Streamtype].relFrames[Chn]++;
					pAvHeader->ComHeader.resv0 = pCaptureDevice->VencStreamInfo[Streamtype].relFrames[Chn];
				break;
				case BASE_PSLICE_REFBYENHANCE:
					pAvHeader->ComHeader.avSubSliceType = PACK_BASE_PSLICE_REFBYENHANCE;
					pCaptureDevice->VencStreamInfo[Streamtype].relFrames[Chn]++;
					pAvHeader->ComHeader.resv0 = pCaptureDevice->VencStreamInfo[Streamtype].relFrames[Chn];
				break;
				case ENHANCE_PSLICE_REFBYENHANCE:
					pAvHeader->ComHeader.avSubSliceType = PACK_ENHANCE_PSLICE_REFBYENHANCE;
					pCaptureDevice->VencStreamInfo[Streamtype].relFrames[Chn]++;
					pAvHeader->ComHeader.resv0 = pCaptureDevice->VencStreamInfo[Streamtype].relFrames[Chn];
				break;	 
				case ENHANCE_PSLICE_NOTFORREF:
					pAvHeader->ComHeader.avSubSliceType = PACK_ENHANCE_PSLICE_NOTFORREF;
					pCaptureDevice->VencStreamInfo[Streamtype].relFrames[Chn]++;
					pAvHeader->ComHeader.resv0 = pCaptureDevice->VencStreamInfo[Streamtype].relFrames[Chn];
				break;					
				default:
					pAvHeader->ComHeader.avSubSliceType = PACK_GENERAL_PSLICE;
					pCaptureDevice->VencStreamInfo[Streamtype].relFrames[Chn]++;
					pAvHeader->ComHeader.resv0 = pCaptureDevice->VencStreamInfo[Streamtype].relFrames[Chn];
					printf("Unknown P frame type %d!\n",BASE_IDRSLICE);
				break;
			}

			s32Ret = NI_MDK_VENC_GetChnAttr(EncChn, &pstChnAttr);
			if (s32Ret != HI_SUCCESS)
			{
				printf("NI_MDK_VENC_GetChnAttr chn[%d] failed with %#x!\n", EncChn, s32Ret);
				return -1;
			}

			pAvHeader->ExternHeader.H264PSlice.PicInterlace = 0;
			if (pCaptureDevice->Rotate == Mode_Rotate_90 || pCaptureDevice->Rotate == Mode_Rotate_270)
			{
				pAvHeader->ExternHeader.H264PSlice.PicWidth = pstChnAttr.stVeAttr.stInputPicAttr.u32PicHeight / 8;
				pAvHeader->ExternHeader.H264PSlice.PicHeight = pstChnAttr.stVeAttr.stInputPicAttr.u32PicWidth / 8;
			}
			else
			{
				pAvHeader->ExternHeader.H264PSlice.PicWidth = pstChnAttr.stVeAttr.stInputPicAttr.u32PicWidth / 8;
				pAvHeader->ExternHeader.H264PSlice.PicHeight = pstChnAttr.stVeAttr.stInputPicAttr.u32PicHeight / 8;
			}
			/*if(EncChn == pCaptureDevice->exVencFun[0].Venc)
			{
				pAvHeader->ExternHeader.H264PSlice.VFps = pCaptureDevice->exVencFun[0].Format.FramesPerSecond;
			}
			else*/
			{
				if ((current_fps < pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].EncChannel_info.frame_count) && (0 != current_fps))
				{
					pAvHeader->ExternHeader.H264PSlice.VFps = current_fps;
				}
				else
				{
					pAvHeader->ExternHeader.H264PSlice.VFps = pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].EncChannel_info.frame_count;
				}
			}
			SliceLocation.FrameType = 0; // P帧

			if (pAvHeader->ExternHeader.H264PSlice.PicWidth == 0 || pAvHeader->ExternHeader.H264PSlice.PicHeight == 0 || (pstChnAttr.stVeAttr.stInputPicAttr.u32PicWidth >> 3) > 0xff || (pstChnAttr.stVeAttr.stInputPicAttr.u32PicHeight >> 3) > 0xff)
			{
				pAvHeader->ComHeader.expandLength += 4;
				pAvHeader->ExternHeader.JpgeSlice.VStreamMask = 0x81;
				pAvHeader->ExternHeader.JpgeSlice.VStreamType = 0x0C;
				pAvHeader->ExternHeader.JpgeSlice.VStreamRev = pCaptureDevice->VencStreamInfo[Streamtype].reframe[Chn];
				if ((current_fps < pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].EncChannel_info.frame_count) && (0 != current_fps))
				{
					pAvHeader->ExternHeader.JpgeSlice.VFps = current_fps;
				}
				else
				{
					pAvHeader->ExternHeader.JpgeSlice.VFps = pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].EncChannel_info.frame_count;
				}
				pAvHeader->ExternHeader.JpgeSlice.PicMask = 0x82;
				pAvHeader->ExternHeader.JpgeSlice.PicInterlace = 0;
				pAvHeader->ExternHeader.JpgeSlice.Rev1 = 0;
				pAvHeader->ExternHeader.JpgeSlice.Rev2 = 0;
				if (pCaptureDevice->Rotate == Mode_Rotate_90 || pCaptureDevice->Rotate == Mode_Rotate_270)
				{
					pAvHeader->ExternHeader.JpgeSlice.PicWidth[0] = (pstChnAttr.stVeAttr.stInputPicAttr.u32PicHeight & 0xff);
					pAvHeader->ExternHeader.JpgeSlice.PicWidth[1] = pstChnAttr.stVeAttr.stInputPicAttr.u32PicHeight / 256;
					pAvHeader->ExternHeader.JpgeSlice.PicHeight[0] = (pstChnAttr.stVeAttr.stInputPicAttr.u32PicWidth & 0xff);
					pAvHeader->ExternHeader.JpgeSlice.PicHeight[1] = pstChnAttr.stVeAttr.stInputPicAttr.u32PicWidth / 256;
				}
				else
				{
					pAvHeader->ExternHeader.JpgeSlice.PicWidth[0] = (pstChnAttr.stVeAttr.stInputPicAttr.u32PicWidth & 0xff);
					pAvHeader->ExternHeader.JpgeSlice.PicWidth[1] = pstChnAttr.stVeAttr.stInputPicAttr.u32PicWidth / 256;
					pAvHeader->ExternHeader.JpgeSlice.PicHeight[0] = (pstChnAttr.stVeAttr.stInputPicAttr.u32PicHeight & 0xff);
					pAvHeader->ExternHeader.JpgeSlice.PicHeight[1] = pstChnAttr.stVeAttr.stInputPicAttr.u32PicHeight / 256;
				}
			}
		
		}
		else if (AVType == AV_SLICE_TYPE_V_H265_I)
		{
			pAvHeader->ComHeader.expandLength = 8;
			pAvHeader->ComHeader.avSliceType = 0xFD;
			pAvHeader->ComHeader.avSubSliceType = 0x0;
			pCaptureDevice->VencStreamInfo[Streamtype].refIdrFrames[Chn] = 0;
			pCaptureDevice->VencStreamInfo[Streamtype].relFrames[Chn] = 0;
			pAvHeader->ComHeader.resv0 = 0;
			if (pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].WaterMark.enable)
			{
				pAvHeader->ComHeader.expandLength += 8;
			}
			pAvHeader->ExternHeader.H264ISlice.PicMask = 0x80;
			pAvHeader->ExternHeader.H264ISlice.VStreamMask= 0x81;
			pAvHeader->ExternHeader.H264ISlice.VStreamRev = pCaptureDevice->VencStreamInfo[Streamtype].reframe[Chn];

			pAvHeader->ExternHeader.H264ISlice.VStreamType = 0x0C;
			
			s32Ret = NI_MDK_VENC_GetChnAttr(EncChn, &pstChnAttr);
			if(s32Ret != HI_SUCCESS)
			{
				printf("NI_MDK_VENC_GetChnAttr chn[%d] failed with %#x!\n", EncChn, s32Ret);
				return -1;
			}

			pAvHeader->ExternHeader.H264ISlice.PicInterlace = 0;
			if(pCaptureDevice->Rotate == Mode_Rotate_90 || pCaptureDevice->Rotate == Mode_Rotate_270)
			{
				pAvHeader->ExternHeader.H264ISlice.PicWidth = pstChnAttr.stVeAttr.stInputPicAttr.u32PicHeight / 8;
				pAvHeader->ExternHeader.H264ISlice.PicHeight = pstChnAttr.stVeAttr.stInputPicAttr.u32PicWidth / 8;
			}
			else
			{
				pAvHeader->ExternHeader.H264ISlice.PicWidth = pstChnAttr.stVeAttr.stInputPicAttr.u32PicWidth / 8;
				pAvHeader->ExternHeader.H264ISlice.PicHeight = pstChnAttr.stVeAttr.stInputPicAttr.u32PicHeight / 8;
			}
			/*if(EncChn == pCaptureDevice->exVencFun[0].Venc)
			{
				pAvHeader->ExternHeader.H264ISlice.VFps = pCaptureDevice->exVencFun[0].Format.FramesPerSecond;
			}
			else*/
			{
				if ((current_fps < pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].EncChannel_info.frame_count) && (0 != current_fps))
				{
					pAvHeader->ExternHeader.H264ISlice.VFps = current_fps;
				}
				else
				{
					pAvHeader->ExternHeader.H264ISlice.VFps = pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].EncChannel_info.frame_count;
				}
			}
			SliceLocation.FrameType = 1;
			
			if(pAvHeader->ExternHeader.H264ISlice.PicWidth == 0 || pAvHeader->ExternHeader.H264ISlice.PicHeight == 0
				|| (pstChnAttr.stVeAttr.stInputPicAttr.u32PicWidth >>3) > 0xff || (pstChnAttr.stVeAttr.stInputPicAttr.u32PicHeight >>3) > 0xff)
			{	
				pAvHeader->ComHeader.expandLength += 4;
				pAvHeader->ExternHeader.JpgeSlice.VStreamMask = 0x81;
				pAvHeader->ExternHeader.JpgeSlice.VStreamType = 0x0C;
				pAvHeader->ExternHeader.JpgeSlice.VStreamRev = pCaptureDevice->VencStreamInfo[Streamtype].reframe[Chn];
				if ((current_fps < pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].EncChannel_info.frame_count) && (0 != current_fps))
				{
					pAvHeader->ExternHeader.JpgeSlice.VFps = current_fps;
				}
				else
				{
					pAvHeader->ExternHeader.JpgeSlice.VFps = pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].EncChannel_info.frame_count;
				}
				pAvHeader->ExternHeader.JpgeSlice.PicMask = 0x82;
				pAvHeader->ExternHeader.JpgeSlice.PicInterlace = 0;
				pAvHeader->ExternHeader.JpgeSlice.Rev1 = 0;
				pAvHeader->ExternHeader.JpgeSlice.Rev2 = 0;
				if(pCaptureDevice->Rotate == Mode_Rotate_90 || pCaptureDevice->Rotate == Mode_Rotate_270)
				{					
					pAvHeader->ExternHeader.JpgeSlice.PicWidth[0] =  (pstChnAttr.stVeAttr.stInputPicAttr.u32PicHeight & 0xff);
					pAvHeader->ExternHeader.JpgeSlice.PicWidth[1] = pstChnAttr.stVeAttr.stInputPicAttr.u32PicHeight / 256;
					pAvHeader->ExternHeader.JpgeSlice.PicHeight[0] = (pstChnAttr.stVeAttr.stInputPicAttr.u32PicWidth & 0xff);
					pAvHeader->ExternHeader.JpgeSlice.PicHeight[1] = pstChnAttr.stVeAttr.stInputPicAttr.u32PicWidth /256;
				}
				else
				{
					pAvHeader->ExternHeader.JpgeSlice.PicWidth[0] = (pstChnAttr.stVeAttr.stInputPicAttr.u32PicWidth & 0xff);
					pAvHeader->ExternHeader.JpgeSlice.PicWidth[1] = pstChnAttr.stVeAttr.stInputPicAttr.u32PicWidth/256;
					pAvHeader->ExternHeader.JpgeSlice.PicHeight[0] = (pstChnAttr.stVeAttr.stInputPicAttr.u32PicHeight & 0xff);
					pAvHeader->ExternHeader.JpgeSlice.PicHeight[1] = pstChnAttr.stVeAttr.stInputPicAttr.u32PicHeight/ 256;
				}
			}

			
		}
		else if (AVType == AV_SLICE_TYPE_V_JPEG)
		{
			pAvHeader->ComHeader.expandLength = 0;
			pAvHeader->ComHeader.avSliceType = 0xFB;
			SliceLocation.FrameType = 4;	//P帧
		}
		else
		{
			printf("未知码流类型\n");
			return -1;
		}
		//u32Seq++;
		pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].VSequence++;
		pAvHeader->ComHeader.avSequence = pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].VSequence;
	}
	else if(AVType == AV_SLICE_TYPE_A)
	{
		
//		pAencStream = (FH_AC_FRAME_S *)buffer;
//		avSequence = 1;//pAencStream->u32Seq;
		
//		TotalLen += pAencStream->len - 4;
		TotalLen += StreamLen;			
		if(TotalLen ==0)
		{
			printf("Audio NULL:\n");
		}
		//PRINT_TRACE("pAencStream->u32Len = %d\n", pAencStream->u32Len);
		pAvHeader->ComHeader.expandLength = 4;
		pAvHeader->ComHeader.avSliceType = 0xF0;
		pAvHeader->ExternHeader.ASlice.AFomatMask = 0x83;
		pAvHeader->ExternHeader.ASlice.AenSoundmode = 1;
		pAvHeader->ExternHeader.ASlice.AenPayloadType = Packets_AudioStreamType_ConToLib_Table[pAiDevice->stAioType];
		pAvHeader->ExternHeader.ASlice.AenSamplerate = Packets_AudioRate_ConToLib_Table[pAiDevice->stAioAttr.sample_rate];

/*		PriStreamPack[1].addr = (unsigned char *)pAencStream->data;
		PriStreamPack[1].offset = 4;
		PriStreamPack[1].length = pAencStream->len -4;
		PriStreamPack[1].unusedLen = PriStreamPack[1].length;*/
		PriStreamPack[1].addr = (unsigned char *)buffer;
		PriStreamPack[1].offset = 0;
		PriStreamPack[1].length = StreamLen;
		PriStreamPack[1].unusedLen = PriStreamPack[PriStreamPackCnt].length;

		PriStreamPackCnt = 2;
		SliceLocation.FrameType = 3;

		pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].ASequence++;
		pAvHeader->ComHeader.avSequence = pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].ASequence;
	}
	else if(AVType == AV_SLICE_TYPE_V_WATER)
	{
		WaterData_t WaterData;
		memset(&WaterData, 0, sizeof(WaterData_t));
		
		pAvHeader->ComHeader.avSliceType = 0xF1;
		pAvHeader->ComHeader.expandLength = 8;
		pAvHeader->ComHeader.avSequence = 0;
		
		pAvHeader->ComHeader.avSequence = 0;
		pAvHeader->ComHeader.avSubSliceType = 0x06;
		pAvHeader->ComHeader.resv0 = 0;

		strcpy(WaterData.Head, "TEXT");
		WaterData.data_length += sizeof(WaterData.Head);
		WaterData.buffer_length = pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].WaterMark.length;
		if (WaterData.buffer_length > WATERMARK_MAX_LEN)
		{
			printf("水印数据太长\n");
			return -1;
		}
		//PRINT_TRACE("WaterData.buffer_length %d \n",WaterData.buffer_length);
		WaterData.data_length += sizeof(WaterData.buffer_length);
//		WaterData.crc = _capture_sum_32_check((unsigned char*)(pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].WaterMark.data), WaterData.buffer_length);	//水印校验信息
		WaterData.crc = _crc_32_check((unsigned char*)(pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].WaterMark.data), WaterData.buffer_length, 0xffffffff);  //水印校验信息

		WaterData.data_length += sizeof(WaterData.crc );
		memcpy(WaterData.buffer, pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].WaterMark.data, WaterData.buffer_length);
		WaterData.data_length += WaterData.buffer_length;
		
	//	avStreamSumRest = 0x0;		
	//	avStreamSumRest += _capture_sum_32_check((unsigned char *)(&WaterData),WaterData.data_length);
	//	_capture_crc_result_to_header(&pAvHeader->ExternHeader.WaterVerify, avStreamSumRest, VERIFY_TYPE_CRCSUM);
		avStreamCrcRest = 0x0;
		avStreamCrcRest = _crc_32_check((unsigned char *)(&WaterData),
						WaterData.data_length, ~avStreamCrcRest);

		_capture_crc_result_to_header(&pAvHeader->ExternHeader.WaterVerify, avStreamCrcRest, VERIFY_TYPE_CRC32);

		PriStreamPack[1].addr = (unsigned char *)&WaterData;
		PriStreamPack[1].offset = 0;
		PriStreamPack[1].length = WaterData.data_length;
		PriStreamPack[1].unusedLen = PriStreamPack[1].length;
		
		PriStreamPackCnt = 2;
		SliceLocation.FrameType = 5;
		TotalLen += WaterData.data_length;
	}
	else
	{
		pAvHeader->ComHeader.avSequence = 0;
		printf("未知码流类型\n");
		return -1;
	}

	//PRINT_TRACE("Chn = %d, type = %d, AVType = %d, comp = %d\n"
	//	, Chn, Streamtype, AVType, pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].Format.Compression);
	clock = pts/1000/1000;
	gmtime_r(&clock, &tm);

	
	//PRINT_TRACE("comp == %d\n", comp);
	

	pAvHeader->ComHeader.avHead = ZLAV_HEADER;
	pAvTail->avTail = ZLAV_TAIL;

	TotalLen += sizeof(AV_SLICE_HEADER_COM_t);
	TotalLen += sizeof(AV_SLICE_TAILS_t);
	TotalLen += pAvHeader->ComHeader.expandLength;


	pAvHeader->ComHeader.avSliceLength = TotalLen;
	pAvTail->avSliceLength = TotalLen;
	
	

//	pAvHeader->ComHeader.avSliceLength = TotalLen;
//	pAvTail->avSliceLength = TotalLen;
	SliceLocation.FrameLength = TotalLen;
	
	
	pAvHeader->ComHeader.avChnNum = Chn;
//	pAvHeader->ComHeader.avSequence = avSequence;
	
	SlinceDateTime.year = tm.tm_year + 1900 - 2000;
	SlinceDateTime.month = tm.tm_mon + 1;
	SlinceDateTime.day = tm.tm_mday;
	SlinceDateTime.hour = tm.tm_hour;
	SlinceDateTime.minute = tm.tm_min;
	SlinceDateTime.second = tm.tm_sec;

	memcpy(&pAvHeader->ComHeader.ClockTimes, &SlinceDateTime, sizeof(SLINCE_DATE_TIME_t));
	
	ms = pts/1000;
	pAvHeader->ComHeader.msTimes = (ms % 0xffff);

	for (i = 0; i < sizeof(AV_SLICE_HEADER_COM_t) - 1; i++)
	{
		pAvHeader->ComHeader.CheckSum += *((unsigned char *)(&pAvHeader->ComHeader) + i);	
	}
/*IVS&码流头*/
	/*if (AVType == AV_SLICE_TYPE_V_H264_P )
	{
			memcpy((unsigned char *)HeadBuffer,(unsigned char *)&pAvHeader->ComHeader, sizeof(AV_SLICE_HEADER_COM_t));

			PriStreamPack[0].addr = (unsigned char *)&HeadBuffer;
			PriStreamPack[0].offset = 0;
			PriStreamPack[0].length = sizeof(AV_SLICE_HEADER_COM_t)+pAvHeader->ComHeader.expandLength+IVSRegionLen;
			PriStreamPack[0].unusedLen = PriStreamPack[0].length;
	}
	else if (AVType == AV_SLICE_TYPE_V_H264_I )
	{
			memcpy((unsigned char *)HeadBuffer,(unsigned char *)&pAvHeader->ComHeader, sizeof(AV_SLICE_HEADER_COM_t));
			memcpy((unsigned char *)(HeadBuffer+sizeof(AV_SLICE_HEADER_COM_t)),(unsigned char *)&pAvHeader->ExternHeader, pAvHeader->ComHeader.expandLength);
			PriStreamPack[0].addr = (unsigned char *)&HeadBuffer;
			PriStreamPack[0].offset = 0;
			PriStreamPack[0].length = sizeof(AV_SLICE_HEADER_COM_t)+IVSRegionLen+ pAvHeader->ComHeader.expandLength;
			PriStreamPack[0].unusedLen = PriStreamPack[0].length;
	}
	else*/
	{
		PriStreamPack[0].addr = (unsigned char *)pAvHeader;
		PriStreamPack[0].offset = 0;
		PriStreamPack[0].length = sizeof(AV_SLICE_HEADER_COM_t) + pAvHeader->ComHeader.expandLength;
		PriStreamPack[0].unusedLen = PriStreamPack[0].length;
	}


	PriStreamPack[PriStreamPackCnt].addr = (unsigned char *)pAvTail;
	PriStreamPack[PriStreamPackCnt].offset = 0;
	PriStreamPack[PriStreamPackCnt].length = sizeof(AV_SLICE_TAILS_t);
	PriStreamPack[PriStreamPackCnt].unusedLen = PriStreamPack[PriStreamPackCnt].length;


	*PacketLength = PriStreamPackCnt;
	*DataLength = TotalLen;
	memcpy(pSliceLocation, &SliceLocation, sizeof(SLICE_LOCATION_t));
	return 0;
}

int _capture_packets_to_app(int Chn, StreamType_e Streamtype, AV_SLICE_TYPE_e AvType, PRI_STREAM_PACK_p PriStreamPack,int PriStreamPackCnt, int TotalLen)
{
//	PRINT_TRACE("Chn=%d Streamtype =%d AvType=%d PriStreamPackCnt=%d TotalLen=%d\n ",Chn,Streamtype,AvType,PriStreamPackCnt,TotalLen);
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
	HMEMORY2 pBlockStart=NULL;
	A_PACKAGE_p pAPackage = &GlobalDevice.APackage;
	HMEMORY2 pBlockNext=NULL;
	HMEMORY2 pBlockStartp1=NULL;
	HMEMORY2 pBlockStartp2=NULL;
	PIPELINE_ITEM itm;
	unsigned int  buflen = 0;
	unsigned char AV_Type;	
	unsigned int avStreamSumRest = 0;
	unsigned int AligneV =0;
	unsigned int AligneVLen =0;
	unsigned char Alignebuf[4] = {0xff, 0xff, 0xff, 0xff};
	//StreamType_e Localstreamtype;
//	unsigned int TotalLenTmp =0;
	int i = 0;

	//int VeChn = 0;
	switch (AvType)
	{
		case AV_SLICE_TYPE_V_H264_I:
		case AV_SLICE_TYPE_V_H265_I:
			AV_Type = I_FRAME;
			break;
		case AV_SLICE_TYPE_V_H264_P:
		case AV_SLICE_TYPE_V_H265_P:
			AV_Type = P_FRAME;
			break;
		case AV_SLICE_TYPE_V_JPEG:
			AV_Type = JPEG_FRAME;
			break;
		case AV_SLICE_TYPE_A:
			AV_Type = AUDIO_FRAME;
			break;
		case AV_SLICE_TYPE_V_WATER:
			AV_Type = WATER_FRAME;
			break;
		case AV_SLICE_TYPE_BUT:
			AV_Type = UNDEFINE_FRAME;
			break;
		default :
			printf("Unkonw AvType!\n");
			AV_Type = UNDEFINE_FRAME;
			break;
	}
	if(AV_Type == UNDEFINE_FRAME)
	{
		printf("Unkonw AvType!\n");
		return -1;
	}

	if(AvType == AV_SLICE_TYPE_V_H264_I \
		||AvType == AV_SLICE_TYPE_V_H264_P \
		||AvType == AV_SLICE_TYPE_V_JPEG \
		||AvType == AV_SLICE_TYPE_V_H265_I \
		||AvType == AV_SLICE_TYPE_V_H265_P)
	{	//音频帧4字节对齐
		//PRINT_TRACE("TotalLen = %d\n",TotalLen);
		AligneV = TotalLen % 4;
		if(AligneV !=0 )
		{
			AligneVLen =  4 - AligneV;
		}
		
	}
	else
	{}
	AligneVLen  = 0;
	if(Streamtype == STREAM_TYPE_FIR || Streamtype == STREAM_TYPE_SEC)
	{
		pthread_mutex_lock(&pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].mutex);
		//Audio
		if(AvType == AV_SLICE_TYPE_A) 	
		{
			//Audio缓存为空
			if(pAPackage->Package_Steam[Streamtype].Package_Chn[Chn].PackagFlag== NI_FALSE)
			{	
				for (i = 0; i <=  PriStreamPackCnt; i ++)
				{
					memcpy(pAPackage->Package_Steam[Streamtype].Package_Chn[Chn].PackageBuf+buflen,\
						PriStreamPack[i].addr+PriStreamPack[i].offset, PriStreamPack[i].length);
						//fwrite((HI_U8 *)PriStreamPack[i].addr+PriStreamPack[i].offset, 1, PriStreamPack[i].length, pfdout_ENC);
					buflen =  buflen + PriStreamPack[i].length;
				}
				pAPackage->Package_Steam[Streamtype].Package_Chn[Chn].PackagFlag= NI_TRUE;
				pAPackage->Package_Steam[Streamtype].Package_Chn[Chn].PackagLen =  buflen;
				pthread_mutex_unlock(&pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].mutex);
				return NI_SUCCESS;
			}
			else
			{	
				//A+A
				pBlockStart =Memory2Alloc(TotalLen+pAPackage->Package_Steam[Streamtype].Package_Chn[Chn].PackagLen,TRUE,TRUE);				
				if(pBlockStart ==NULL)
				{
					printf(" Memory2Alloc failed!\n");
					pthread_mutex_unlock(&pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].mutex);
					return NI_FALSE;
				}
				Memory2Write(pBlockStart, pAPackage->Package_Steam[Streamtype].Package_Chn[Chn].PackageBuf, \
					pAPackage->Package_Steam[Streamtype].Package_Chn[Chn].PackagLen);
				//fwrite((HI_U8 *)pAPackage.Package_Steam[Streamtype].Package_Chn[Chn].PackageBuf, 1, pAPackage.Package_Steam[Streamtype].Package_Chn[Chn].PackagLen, pfdout_ENC);
				Memory2WriteFinish(pBlockStart,AUDIO_FRAME);
				for (i = 0; i <=  PriStreamPackCnt; i ++)
				{
					Memory2Write(pBlockStart, PriStreamPack[i].addr+PriStreamPack[i].offset, PriStreamPack[i].length);	
					//fwrite((HI_U8 *)PriStreamPack[i].addr+PriStreamPack[i].offset, 1, PriStreamPack[i].length, pfdout_ENC);

				}
				Memory2WriteFinish(pBlockStart,AV_Type);
				pAPackage->Package_Steam[Streamtype].Package_Chn[Chn].PackagFlag = NI_FALSE;
			}
									

		}
		//Video
		else
		{			
			//Audio缓存为空
			if ((pAPackage->Package_Steam[Streamtype].Package_Chn[Chn].PackagFlag == NI_FALSE)||(AvType == AV_SLICE_TYPE_V_H264_I) || (AvType == AV_SLICE_TYPE_V_H265_I))
			{
				//Video
				pBlockStart=Memory2Alloc(TotalLen+AligneVLen,FALSE,TRUE);	
				if(pBlockStart ==NULL)
				{
					PRINT_ERROR("Memory2Alloc failed \n");
					pthread_mutex_unlock(&pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].mutex);
					return NI_FALSE;
				}

				for (i = 0; i < PriStreamPackCnt; i ++)
				{			
					Memory2Write(pBlockStart, PriStreamPack[i].addr+PriStreamPack[i].offset, PriStreamPack[i].length);
					//fwrite((HI_U8 *)PriStreamPack[i].addr+PriStreamPack[i].offset, 1, PriStreamPack[i].length, pfdout_ENC);
				}
				// 4字节对齐
				Memory2Write(pBlockStart, Alignebuf, AligneVLen);	
				//fwrite((HI_U8 *)Alignebuf, 1, AligneVLen, pfdout_ENC);
				//Tail
				Memory2Write(pBlockStart, PriStreamPack[PriStreamPackCnt].addr+PriStreamPack[PriStreamPackCnt].offset, PriStreamPack[PriStreamPackCnt].length);
				//fwrite((HI_U8 *)PriStreamPack[PriStreamPackCnt].addr+PriStreamPack[PriStreamPackCnt].offset, 1, PriStreamPack[PriStreamPackCnt].length, pfdout_ENC);
				Memory2WriteFinish(pBlockStart,AV_Type);
				//I帧校验	
				if (AvType == AV_SLICE_TYPE_V_H264_I || AvType == AV_SLICE_TYPE_V_H265_I)
				{
					unsigned int H264len = 0;
					unsigned int CalSumlen = 0;
					//unsigned int BufVerifyBegin;
					//Water_Verify_t WaterVerify;
					H264len = 0;
					pBlockStartp1 = Memory2GetBuffer(pBlockStart);
					for(i=1;i<PriStreamPackCnt;i++)
					{
						H264len +=PriStreamPack[i].length;
					}
					H264len += AligneVLen;
		//			PRINT_TRACE("H264len=%ld Memory2GetDataLength(pBlockStartp1=%d PriStreamPackCnt=%d\n",H264len,Memory2GetDataLength(pBlockStartp1),PriStreamPackCnt);
					CalSumlen=MIN(H264len,Memory2GetDataLength(pBlockStart)-PriStreamPack[0].length);
					avStreamSumRest += _capture_sum_32_check((pBlockStartp1 + PriStreamPack[0].length),CalSumlen);

					H264len -= CalSumlen;

					pBlockNext = pBlockStart;
					while(H264len > 0)
					{
						pBlockNext = Memory2Next(pBlockNext);
						if(pBlockNext ==NULL)
						{
							break;
						}
						pBlockStartp2 = Memory2GetBuffer(pBlockNext);
						CalSumlen=MIN(H264len,Memory2GetDataLength(pBlockNext));
						avStreamSumRest += _capture_sum_32_check(pBlockStartp2,CalSumlen);

						H264len -= CalSumlen;
					}

					
					/*if (pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].WaterMark.enable)
					{
						_capture_crc_result_to_header(&WaterVerify, avStreamSumRest, VERIFY_TYPE_CRCSUM);

						if(pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].Format.ImageSize == VIDEO_SIZE_5M
							|| pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].Format.ImageSize == VIDEO_SIZE_4M
							|| pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].Format.ImageSize == VIDEO_SIZE_4M_QHD)
						{
							BufVerifyBegin = sizeof(AV_SLICE_HEADER_COM_t) + sizeof(AV_SLICE_HEADER_u) -sizeof(Water_Verify_t);
						}
						else
						{
							BufVerifyBegin = sizeof(AV_SLICE_HEADER_COM_t) + sizeof(H264_I_SLICE_HEADERS_t) -sizeof(Water_Verify_t);
						}
						memcpy(pBlockStartp1+BufVerifyBegin,&WaterVerify,sizeof(Water_Verify_t));
					}*/
						
				}
			
			}
			else
			{

				 pBlockStart=Memory2Alloc(TotalLen+pAPackage->Package_Steam[Streamtype].Package_Chn[Chn].PackagLen+\
				 	AligneVLen,FALSE,TRUE);	
				if(pBlockStart ==NULL)
				{
					printf("%s: Memory2Alloc failed!\n", __FUNCTION__);
					pthread_mutex_unlock(&pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].mutex);
					return NI_FALSE;
				}

				Memory2Write(pBlockStart, pAPackage->Package_Steam[Streamtype].Package_Chn[Chn].PackageBuf,\
					pAPackage->Package_Steam[Streamtype].Package_Chn[Chn].PackagLen);	
				//fwrite((HI_U8 *)pAPackage.Package_Steam[Streamtype].Package_Chn[Chn].PackageBuf, 1, pAPackage.Package_Steam[Streamtype].Package_Chn[Chn].PackagLen, pfdout_ENC);
				Memory2WriteFinish(pBlockStart,AUDIO_FRAME);

				for (i = 0; i < PriStreamPackCnt; i ++)
				{
					Memory2Write(pBlockStart, PriStreamPack[i].addr+PriStreamPack[i].offset, PriStreamPack[i].length);	
					//fwrite((HI_U8 *)PriStreamPack[i].addr+PriStreamPack[i].offset, 1, PriStreamPack[i].length, pfdout_ENC);

				}
				// 4字节对齐
				Memory2Write(pBlockStart,Alignebuf, AligneVLen);	
				//fwrite((HI_U8 *)Alignebuf, 1, AligneVLen, pfdout_ENC);
				//Tail
				Memory2Write(pBlockStart, PriStreamPack[PriStreamPackCnt].addr+PriStreamPack[PriStreamPackCnt].offset,\
				PriStreamPack[PriStreamPackCnt].length);
				//fwrite((HI_U8 *)PriStreamPack[PriStreamPackCnt].addr+PriStreamPack[PriStreamPackCnt].offset, 1, PriStreamPack[PriStreamPackCnt].length, pfdout_ENC);
				Memory2WriteFinish(pBlockStart,AV_Type);
				
				pAPackage->Package_Steam[Streamtype].Package_Chn[Chn].PackagFlag = NI_FALSE;
			}
		}
		
		pthread_mutex_unlock(&pCaptureDevice->EncDevice[Chn].StreamDevice[Streamtype].mutex);
	}
	else if (Streamtype == STREAM_TYPE_THI || Streamtype == STREAM_TYPE_FOU)
	{
		pBlockStart=Memory2Alloc(TotalLen+AligneVLen,FALSE,TRUE);	
		if(pBlockStart ==NULL) 
		{
			printf("%s: Memory2Alloc failed!\n", __FUNCTION__);
			return NI_FALSE;
		}

		for (i = 0; i < PriStreamPackCnt; i ++)
		{
		
			Memory2Write(pBlockStart, PriStreamPack[i].addr+PriStreamPack[i].offset, PriStreamPack[i].length);
		}
		// 4字节对齐
		Memory2Write(pBlockStart, Alignebuf, AligneVLen);		
		//Tail
		Memory2Write(pBlockStart, PriStreamPack[PriStreamPackCnt].addr+PriStreamPack[PriStreamPackCnt].offset, \
		PriStreamPack[PriStreamPackCnt].length);
		Memory2WriteFinish(pBlockStart,AV_Type);
	}
	else
	{	
		printf("Unknown Streamtype\n");
		return NI_FALSE;
		
	}

	/*VeChn = Chn;

	hMemory = pBlockStart;
	Localstreamtype = STREAM_TYPE_FIR;//pCaptureDevice->EncDevice[VeChn% pCaptureDevice->EncCount].StreamDevice[VeChn / pCaptureDevice->EncCount].CapChn;
	
	if(CaptureLocalStream)
	{
		(*CaptureLocalStream)(Chn,hMemory,Localstreamtype);
	}*/
	 //printf("PipelinePush %d %d %d\n",Chn,Streamtype,AvType);
	itm.chn = 0;
	itm.hMemory = pBlockStart;
	itm.use = PACKET_USE_LOCAL_CAPTURE;
	if (AvType == AV_SLICE_TYPE_V_JPEG)
	{
		itm.stream_type = CHL_JPEG_T;
	}
	else
	{
		itm.stream_type = Streamtype;
		if(Streamtype == STREAM_TYPE_FOU && AvType == AV_SLICE_TYPE_A)
		{
			itm.use = PACKET_USE_AUDIO_IN;
		}
	}
	
	PipelinePush(&itm);
	Memory2Release(pBlockStart);
	return HI_SUCCESS;
	
}

void *_capture_multi_media_stream_process(void *p)
{
	int i = 0;
	//int j = 0;
	int ret = -1;
	int chan = 0;
	//int EncCount = 0;
	int Frametype = 0;
	int drop_frame = FALSE;
	//int bind;
    //VENC_STREAM_BUF_MODE_E enBufMode;
	VENC_CHN_ATTR_S pstChnAttr;
    VENC_STREAM_S stream;
	PRI_STREAM_PACK_t PriStreamPack[40] = {0};
	AV_SLICE_HEADER_t AvHeader;
	AV_SLICE_TAILS_t AvTail;
	int PacketLength = 0;
	int DataLength = 0;
	SLICE_LOCATION_t SliceLocation;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
	//unsigned long long pts = 0;
	Media_VideoStream_t pstream;
	
	
	chan = (int)(*((int *)p));
	unsigned int max_buff_len = StreamBuffSiZeTable[chan] + 1 ;
	unsigned char *buff = malloc(max_buff_len);
	unsigned int len = 0;
	unsigned int bufflen = 0;
	//AiDevice_p pAiDevice = &GlobalDevice.AiDevice;
	//FH_AC_FRAME_S frame_info,frame_trans;
	//FH_CHN_STATUS pstVencStatus;
	//frame_info.data = malloc(pAiDevice->Audio_Record_Frame_Period);
	//frame_info.len = pAiDevice->Audio_Record_Frame_Period;
	//frame_trans.data = malloc(pAiDevice->Audio_Record_Frame_Period);	

	//EncCount = pCaptureDevice->EncCount;

	chan = (int)(*((int*)p));

	memset(&stream, 0, sizeof(VENC_STREAM_S));
	memset(&pstChnAttr, 0, sizeof(VENC_CHN_ATTR_S));
	memset(&AvTail, 0, sizeof(AV_SLICE_TAILS_t));
	memset(&pstream, 0, sizeof(Media_VideoStream_t));
	memset(&SliceLocation, 0, sizeof(SLICE_LOCATION_t));
	memset(&AvHeader, 0, sizeof(AV_SLICE_HEADER_t));

	pthread_t pthread;
	pid_t pid;
	pid_t tid;
	pid = getpid();
	tid = syscall(SYS_gettid);
	pthread = pthread_self();

	PRINT_ERROR("MediaStreamProcess[%d] thread pid %d tid %d thread id 0x%x\n",chan, pid, tid, pthread);
	PRINT_ERROR("chan is %d max_buff_len is %d\n", chan, max_buff_len);

	while(g_video_stream_running[chan] == TRUE)
	{
		ret = NI_MDK_VENC_GetStream(chan, &stream, 300);
		if (ret != RETURN_OK)
        {
        	//printf("video lose\n");
			usleep(0);
        	continue ;
        }

		ret = NI_MDK_VENC_GetChnAttr(chan, &pstChnAttr);
		if (ret != RETURN_OK)
		{
			printf("MediaStreamProcess[%d] NI_MDK_VENC_GetChnAttr failed,ret = %#x\n", chan, ret);
			NI_MDK_VENC_ReleaseStream(chan, &stream);
			// printf("video lose\n");
			usleep(0);
			continue;
		}

		if (pstChnAttr.stVeAttr.enType == PT_H264)
        {
			Frametype = stream.enFrameType == VENC_FRAME_TYPE_I?AV_SLICE_TYPE_V_H264_I:AV_SLICE_TYPE_V_H264_P;

			for (int n = 0; n < stream.stStreamPacket.u32PackNum; n++)
			{
				if (len + stream.stStreamPacket.au32PackLen[n] > max_buff_len)
				{
					PRINT_ERROR("Stream length is %d(%d + %d), larger than buffer size(%d)\n",
								len + stream.stStreamPacket.au32PackLen[n], len, len + stream.stStreamPacket.au32PackLen[n], max_buff_len);
					len = 0;
					NI_MDK_VENC_ReleaseStream(chan, &stream);
					drop_frame = TRUE;
					break;
				}
				memcpy(buff+len, (void *)stream.stStreamPacket.aPackVirAddr[n], stream.stStreamPacket.au32PackLen[n]);
				len += stream.stStreamPacket.au32PackLen[n];
				bufflen = len;
			}

			if (TRUE == drop_frame)
			{
				drop_frame = FALSE;
				usleep(0);
				continue;
			}

			if (stream.stStreamPacket.bEndofFrame)
			{
				len = 0;
			}
			else
			{
				NI_MDK_VENC_ReleaseStream(chan, &stream);
				usleep(0);
				continue;
			}
//			for(i = 0;i < EncCount;i++)
			{
//				for(j = 0;j < pCaptureDevice->EncDevice[i].StreamCount;j++)
				{
//					if(pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.channel == chan && pCaptureDevice->EncDevice[i].StreamDevice[j].VencStart);
					if(pCaptureDevice->EncDevice[0].StreamDevice[chan].VencStart)
					{
						if (_capture_head_to_packets(i, chan, Frametype, buff, bufflen, PriStreamPack, (AV_SLICE_HEADER_p)&AvHeader, (AV_SLICE_TAILS_p)&AvTail, 40, &PacketLength, &DataLength, &SliceLocation, stream.u64PTS))
						{
							printf("封装码流至私有缓冲失败\n");
						}
						else
						{
							if(_capture_packets_to_app(i, chan, Frametype, PriStreamPack, PacketLength, DataLength))
							{
								printf("私有缓冲数据到队列缓冲失败\n");
							}
						}
						
						if (pCaptureDevice->EncDevice[0].StreamDevice[chan].CapChn == CHL_MAIN_T && Frametype == AV_SLICE_TYPE_V_H264_I
						&& pCaptureDevice->EncDevice[0].StreamDevice[chan].WaterMark.enable == TRUE)
						{
							if (_capture_head_to_packets(i, chan, AV_SLICE_TYPE_V_WATER, buff, bufflen, PriStreamPack, (AV_SLICE_HEADER_p)&AvHeader, (AV_SLICE_TAILS_p)&AvTail, 40, &PacketLength, &DataLength, &SliceLocation, stream.u64PTS))
							{
								printf("添加水印帧失败\n");
							}
							else
							{
								if(_capture_packets_to_app(i, chan, AV_SLICE_TYPE_V_WATER, PriStreamPack, PacketLength, DataLength))
								{
									printf("私有缓冲数据到队列缓冲失败\n");
								}
							}
						}	
					}	
				}
			}
        }
       	else if(pstChnAttr.stVeAttr.enType == PT_H265)
		{
			Frametype = stream.enFrameType == VENC_FRAME_TYPE_I?AV_SLICE_TYPE_V_H265_I:AV_SLICE_TYPE_V_H265_P;

			for(int n = 0; n < stream.stStreamPacket.u32PackNum; n++)
			{
				if (len + stream.stStreamPacket.au32PackLen[n] > max_buff_len)
				{
					PRINT_ERROR("Stream length is %d(%d + %d), larger than buffer size(%d)\n",
								len + stream.stStreamPacket.au32PackLen[n], len, len + stream.stStreamPacket.au32PackLen[n], max_buff_len);
					len = 0;
					NI_MDK_VENC_ReleaseStream(chan, &stream);
					drop_frame = TRUE;
					break;
				}
				memcpy(buff+len, (void *)stream.stStreamPacket.aPackVirAddr[n], stream.stStreamPacket.au32PackLen[n]);
				len += stream.stStreamPacket.au32PackLen[n];
				bufflen = len;
			}

			if (TRUE == drop_frame)
			{
				drop_frame = FALSE;
				usleep(0);
				continue;
			}

			if (stream.stStreamPacket.bEndofFrame)
			{
				len = 0;
			}
			else
			{
				NI_MDK_VENC_ReleaseStream(chan, &stream);
				usleep(0);
				continue;
			}
//			for(i = 0;i < EncCount;i++)
			{
//				for(j = 0;j < pCaptureDevice->EncDevice[i].StreamCount;j++)
				{
//					if(pCaptureDevice->EncDevice[i].StreamDevice[j].EncChannel_info.channel == chan && pCaptureDevice->EncDevice[i].StreamDevice[j].VencStart);
					if(pCaptureDevice->EncDevice[0].StreamDevice[chan].VencStart)
					{
						if (_capture_head_to_packets(i, chan, Frametype, buff, bufflen, PriStreamPack, (AV_SLICE_HEADER_p)&AvHeader, (AV_SLICE_TAILS_p)&AvTail, 40, &PacketLength, &DataLength, &SliceLocation, stream.u64PTS))
						{
							printf("封装码流至私有缓冲失败\n");
						}
						else
						{
							if(_capture_packets_to_app(i, chan, Frametype, PriStreamPack, PacketLength, DataLength))
							{
								printf("私有缓冲数据到队列缓冲失败\n");
							}
						}
						
						if (pCaptureDevice->EncDevice[0].StreamDevice[chan].CapChn == CHL_MAIN_T && Frametype == AV_SLICE_TYPE_V_H265_I
						&& pCaptureDevice->EncDevice[0].StreamDevice[chan].WaterMark.enable == TRUE)
						{
							if (_capture_head_to_packets(i, chan, AV_SLICE_TYPE_V_WATER, buff, bufflen, PriStreamPack, (AV_SLICE_HEADER_p)&AvHeader, (AV_SLICE_TAILS_p)&AvTail, 40, &PacketLength, &DataLength, &SliceLocation, stream.u64PTS))
							{
								printf("添加水印帧失败\n");
							}
							else
							{
								if(_capture_packets_to_app(i, chan, AV_SLICE_TYPE_V_WATER, PriStreamPack, PacketLength, DataLength))
								{
									printf("私有缓冲数据到队列缓冲失败\n");
								}
							}
						}
					}	
				}
			}
		}

		NI_MDK_VENC_ReleaseStream(chan,&stream);

		usleep(0);
	}
	free(buff);
	return NULL;
}

void *_capture_audio_stream_process(void *p)
{
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
	AiDevice_p pAiDevice = &GlobalDevice.AiDevice;
	int i = 0;
	//int j = 0;	
	int ret = -1;
	AENC_CHN AeChn = 0;
	Media_VideoStream_t pstream;
	PRI_STREAM_PACK_t PriStreamPack[40] = {0};
	AV_SLICE_HEADER_t AvHeader;
	AV_SLICE_TAILS_t AvTail;
	int PacketLength = 0;
	int DataLength = 0;
	SLICE_LOCATION_t SliceLocation;
	
	//unsigned long long pts = 0;
	AUDIO_STREAM_S stStreamChn0;
	AUDIO_STREAM_S stResStreamChn0;
	unsigned int audio_length = 0;
	unsigned int pcm_length = 0;
	int aac_size = 0;
	unsigned char pcm_buff[AUDIO_ONE_FRAME_SIZE+1];
	unsigned char buff[AUDIO_ONE_FRAME_SIZE+1];
	memset(buff, 0, AUDIO_ONE_FRAME_SIZE+1);
	memset(pcm_buff, 0, AUDIO_ONE_FRAME_SIZE + 1);
	i = 0;
	int net_talk_chn = STREAM_TYPE_FOU;//语音对讲为第四通道
	memset(&AvTail, 0, sizeof(AV_SLICE_TAILS_t));
	memset(&AvHeader, 0, sizeof(AV_SLICE_HEADER_t));
	memset(&stStreamChn0, 0, sizeof(AUDIO_STREAM_S));
	memset(&pstream, 0, sizeof(Media_VideoStream_t));
	memset(&stResStreamChn0, 0, sizeof(AUDIO_STREAM_S));
	memset(&SliceLocation, 0, sizeof(SLICE_LOCATION_t));

	pthread_t pthread;
	pid_t pid;
	pid_t tid;
	pid = getpid();
	tid = syscall(SYS_gettid);
	pthread = pthread_self();

	PRINT_ERROR("AudioStreamProcess thread pid %d tid %d thread id 0x%x\n", pid, tid, pthread);

	while (TRUE == g_audio_stream_running)
	{
		pthread_mutex_lock(&audio_lock);
		ret = NI_MDK_AENC_GetStream(AeChn, &stStreamChn0, 20);
		if (ret != RETURN_OK)
		{
			//printf("audio lose\n");
			pthread_mutex_unlock(&audio_lock);
			usleep(20);
			continue;
		}

		memset(buff, 0, AUDIO_ONE_FRAME_SIZE + 1);
		memset(pcm_buff, 0, AUDIO_ONE_FRAME_SIZE + 1);

		pstream.stream_attr.pts = stStreamChn0.u64TimeStamp;

		memcpy(buff, stStreamChn0.pu8Stream, stStreamChn0.u32Len);
		pcm_length = stStreamChn0.u32Len;
		NI_MDK_AENC_ReleaseStream(AeChn, &stStreamChn0);

		if (AecStart)
		{
			stStreamChn0.u32Len = pcm_length;
			stStreamChn0.pu8Stream = buff;

			stResStreamChn0.u32Len = pcm_length;
			stResStreamChn0.pu8Stream = pcm_buff;

			NI_MDK_AEC_Process(AeChn, &stStreamChn0, &stResStreamChn0);

			memset(buff, 0, AUDIO_ONE_FRAME_SIZE + 1);
			memcpy(buff, pcm_buff, pcm_length);
		}

		stStreamChn0.u32Len 		= pcm_length;
		stStreamChn0.pu8Stream 		= buff;

		NI_MDK_HPF_Process(AeChn, &stStreamChn0);

		stStreamChn0.u32Len 		= pcm_length;
		stStreamChn0.pu8Stream 		= buff;

		stResStreamChn0.u32Len 		= pcm_length;
		stResStreamChn0.pu8Stream 	= pcm_buff;

		NI_MDK_ANS_Process(AeChn, &stStreamChn0, &stResStreamChn0);

		memset(buff, 0, AUDIO_ONE_FRAME_SIZE + 1);
		memcpy(buff, pcm_buff, pcm_length);

		stStreamChn0.u32Len 		= pcm_length;
		stStreamChn0.pu8Stream 		= buff;

		stResStreamChn0.u32Len 		= pcm_length;
		stResStreamChn0.pu8Stream 	= pcm_buff;

		NI_MDK_AGC_Process(AeChn, &stStreamChn0, &stResStreamChn0);

		pthread_mutex_unlock(&audio_lock);

		ias_ad_write_pcm(pcm_buff, pcm_length);      //分流ai pcm数据给哭声检测
		memset(buff, 0, AUDIO_ONE_FRAME_SIZE + 1);

		switch (pAiDevice->stAioType)
		{
			case AudioStreamType_PCM:
				memcpy(buff, pcm_buff, pcm_length);
				audio_length = pcm_length;
				pstream.type = Media_StreamType_PCM;
				break;
			case AudioStreamType_G711A:
				ret = pcm_2_g711a(pcm_buff, pcm_length, buff, &audio_length);
				if (0 != ret)
				{
					usleep(0);
					continue;
				}
				pstream.type = Media_StreamType_G711A;
				break;
			case AudioStreamType_G711U:
				ret = pcm_2_g711u(pcm_buff, pcm_length, buff, &audio_length);
				if (0 != ret)
				{
					usleep(0);
					continue;
				}
				pstream.type = Media_StreamType_G711U;
				break;
			case AudioStreamType_AAC:
				if (NULL == pAiDevice->aac_handle || 0 == pAiDevice->g_aac_encinit)
				{
					usleep(10);
					continue;
				}
				ret = pcm_2_aac(pcm_buff, pcm_length, buff, &aac_size);
				if (0 != ret)
				{
					usleep(0);
					continue;
				}
				else
				{
					audio_length = aac_size;
				}
				pstream.type = Media_StreamType_AAC;
				break;
			case AudioStreamType_G726:
				// frame_trans.len = fh_pcm_2_g726_16K(pAiDevice->g726_handle, frame_info.data, frame_info.len, frame_trans.data);
				pstream.type = Media_StreamType_G726;
				break;
			/*case AudioStreamType_G726:
				frame_trans.len = fh_pcm_2_g726_32K(pAiDevice->g726_handle, frame_info.data, frame_info.len, frame_trans.data);
				pstream.type = Media_StreamType_G726_32K;
				break;
				*/
			default:
				PRINT_ERROR("this Ai encode Type not support\n");
				break;
		}

		
		
		//FILE *fp;
		//fp = fopen("./8856v200_chn_0.pcm", "a+");
		//for (i = 0; i <= PacketLength; i++)
		//{
			//fwrite(buff, 1, stStreamChn0.u32Len, fp);
			//fflush(fp);
		//}
		//fclose(fp);

		//		for(i = 0;i < EncCount;i++)
		{
		//			for(j = 0;j < pCaptureDevice->EncDevice[i].StreamCount;j++)
			{
		//				if(pCaptureDevice->EncDevice[i].StreamDevice[j].CapChn == CHL_MAIN_T && pCaptureDevice->EncDevice[i].StreamDevice[j].AencStart)
				if (_capture_head_to_packets(i, CHL_MAIN_T, AV_SLICE_TYPE_A, buff, audio_length, PriStreamPack, (AV_SLICE_HEADER_p)&AvHeader, (AV_SLICE_TAILS_p)&AvTail, 40, &PacketLength, &DataLength, &SliceLocation, pstream.stream_attr.pts))
				{
					printf("封装码流至私有缓冲失败\n");
				}
				else
				{
					
			/*		FILE *fp;
					fp = fopen("8856v200_chn_0.pcm","a+");
					for(i = 0; i <=PacketLength;i++)
					{
						fwrite(PriStreamPack[i].addr+PriStreamPack[i].offset,1,PriStreamPack[i].length,fp);
						fflush(fp);
					}
					fclose(fp);*/
					if(pCaptureDevice->EncDevice[i].StreamDevice[CHL_MAIN_T].AencStart)
					{
						if(_capture_packets_to_app(i, CHL_MAIN_T, AV_SLICE_TYPE_A, PriStreamPack, PacketLength, DataLength))
						{
							printf("私有缓冲数据到队列缓冲失败\n");
						}

						if (_capture_packets_to_app(i, CHL_2END_T, AV_SLICE_TYPE_A, PriStreamPack, PacketLength, DataLength))
						{
							printf("私有缓冲数据到队列缓冲失败\n");
						}
					}

					if(pCaptureDevice->EncDevice[i].StreamDevice[CHL_MAIN_T].NetTalkStart)
					{
						if(_capture_packets_to_app(i, net_talk_chn, AV_SLICE_TYPE_A, PriStreamPack, PacketLength, DataLength))
						{
							printf("私有缓冲数据到队列缓冲失败\n");
						}
					}
				}
			}
		}

		usleep(0);
	}

	return NULL;
}

void *_capture_jpeg_stream_process(void *p)
{
	int ret = -1;
	VENC_STREAM_S stream;
	unsigned int len = 0;
	int jpeg_chn = STREAM_TYPE_THI; // 抓图为第三通道

	memset(&stream, 0, sizeof(VENC_STREAM_S));

	pthread_t pthread;
	pid_t pid;
	pid_t tid;
	pid = getpid();
	tid = syscall(SYS_gettid);
	pthread = pthread_self();

	PRINT_ERROR("JpegStreamProcess thread pid %d tid %d thread id 0x%x\n", pid, tid, pthread);
	while (TRUE == g_video_stream_running[jpeg_chn])
	{
		ret = NI_MDK_VENC_GetStream(jpeg_chn, &stream, 300);
		if (ret != RETURN_OK)
		{
			usleep(0);
			continue;
		}

		for (int n = 0; n < stream.stStreamPacket.u32PackNum; n++)
		{
			len += stream.stStreamPacket.au32PackLen[n];
		}

		if (len > JPEG_BUFF_SIZE || 0 == len) 
		{
			PRINT_ERROR("Stream length is %d, larger than buffer size(%d)\n", len, JPEG_BUFF_SIZE);
			len = 0;
			NI_MDK_VENC_ReleaseStream(jpeg_chn, &stream);
			usleep(0);
			continue;
		}

		len = 0;

		pthread_mutex_lock(&jpeg_lock);
		for (int i = 0; i < stream.stStreamPacket.u32PackNum; i++)
		{
			memcpy(jpeg_buff + len, (void *)stream.stStreamPacket.aPackVirAddr[i], stream.stStreamPacket.au32PackLen[i]);
			len += stream.stStreamPacket.au32PackLen[i];
		}

		jpeg_pts = stream.u64PTS;
		jpeglen = len;
		pthread_mutex_unlock(&jpeg_lock);

		len = 0;
		NI_MDK_VENC_ReleaseStream(jpeg_chn, &stream);
		usleep(0);
	}
	return NULL;
}

void *_capture_jpeg_to_app_process(void *p)
{
	int i = 0;
	int DataLength = 0;
	int PacketLength = 0;
	AV_SLICE_TAILS_t AvTail;
	AV_SLICE_HEADER_t AvHeader;
	SLICE_LOCATION_t SliceLocation;
	int jpeg_chn = STREAM_TYPE_THI; // 抓图为第三通道
	PRI_STREAM_PACK_t PriStreamPack[4] = {0};
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

	memset(&AvTail, 0, sizeof(AV_SLICE_TAILS_t));
	memset(&AvHeader, 0, sizeof(AV_SLICE_HEADER_t));
	memset(&SliceLocation, 0, sizeof(SLICE_LOCATION_t));

	pthread_t pthread;
	pid_t pid;
	pid_t tid;
	pid = getpid();
	tid = syscall(SYS_gettid);
	pthread = pthread_self();

	PRINT_ERROR("_capture_jpeg_to_app_process thread pid %d tid %d thread id 0x%x\n", pid, tid, pthread);
	pthread_rwlock_init(&jpeg_rwlock, NULL);
	
	while (TRUE == g_video_stream_running[STREAM_TYPE_FOU])
	{
		if (ENABLE == pCaptureDevice->EncDevice[0].StreamDevice[jpeg_chn].VencStart)
		{
			pthread_mutex_lock(&jpeg_lock);
			if (_capture_head_to_packets(i, jpeg_chn, AV_SLICE_TYPE_V_JPEG, jpeg_buff, jpeglen, PriStreamPack, (AV_SLICE_HEADER_p)&AvHeader, (AV_SLICE_TAILS_p)&AvTail, 40, &PacketLength, &DataLength, &SliceLocation, jpeg_pts))
			{
				printf("封装码流至私有缓冲失败\n");
			}
			else
			{
				if (_capture_packets_to_app(i, jpeg_chn, AV_SLICE_TYPE_V_JPEG, PriStreamPack, PacketLength, DataLength))
				{
					printf("私有缓冲数据到队列缓冲失败\n");
				}
			}
			pthread_mutex_unlock(&jpeg_lock);
			PRINT_INFO("_capture_jpeg_to_app_process DataLength %d\n", DataLength);
			pthread_rwlock_wrlock(&jpeg_rwlock);
			pCaptureDevice->EncDevice[0].StreamDevice[jpeg_chn].VencStart = DISABLE;
			pthread_rwlock_unlock(&jpeg_rwlock);
		}
		else
		{
			usleep(50 *1000);
		}
	}
	return NULL;
}

/// 得到捕获的通道数目。不包括语音捕获的通道，语音捕获的通道是最大捕获通道号加1。
/// 
/// \return捕获的通道数目。
int CaptureGetChannels(void)
{
	return 0;
}

/// 创建捕获设备
/// 
/// \param [in] channel 通道号。
/// \retval 0 创建失败
/// \retval 0 创建成功
int CaptureCreate(int channel)
{
	return 0;
}


/// 销毁捕获设备
/// 
/// \param [in] channel 通道号。
/// \retval 0 销毁失败
/// \retval 0 销毁成功
int CaptureDestroy(int channel)
{
	return 0;
}

/// 强制编码器产生I帧。 
/// 
/// \param [in] channel 通道号。
/// \param [in] dwType
/// \retval 0  设置成功。
/// \retval <0  设置失败。
int CaptureForceIFrame(int  channel, DWORD dwType)
{
	int i = 0;
	int ret = -1;
	int VencChn = 0;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
	
	if (!((pCaptureDevice->EncDevice[channel].SupportStream >> dwType) & 0x01))
	{
		PRINT_ERROR("不支持的码流类型dwType = %d\n",dwType);
		return -1;
	}

	for (i = 0; i < pCaptureDevice->EncDevice[channel].StreamCount; i++)
	{
		if (pCaptureDevice->EncDevice[channel].StreamDevice[i].CapChn == dwType)
		{
			VencChn = i;
		}
	}
	
	ret = NI_MDK_VENC_ForceIDR(VencChn);
	if (ret != RETURN_OK)
	{
		PRINT_ERROR("Error FH_VENC_RequestIDR ret = %#x\n", ret);
	}
	
	return ret;
}


/// 抓取YUV数据
///
///
/// \param [in]  len 	 picData申请的内存大小
/// \param [out] picData YUV数据
/// \param [out] picSize YUV图片的分辨率
/// \retval 0  抓取成功
/// \retval -1  抓取失败。
int CaptureGetYuvPic(char *picData, unsigned int len, SIZE_S *picSize)
{
	if (NULL == picData || NULL == picSize || 0 == len)
	{
		PRINT_ERROR("CaptureGetYuvPic error\n");
		return -1;
	}

	int ret = -1;
	VPS_GRP VpsGrp = 0;
	void *pVirtAddr = NULL;
	unsigned int timeout = 50;
	VPS_FRAME_INFO_S stFrameInfo;
	VPS_CHN_E VpsChn = VPS_CHN_ENC1;

	memset(&stFrameInfo, 0, sizeof(VPS_FRAME_INFO_S));

	while (1)
	{
		ret = NI_MDK_VPS_GetFrame(VpsGrp, VpsChn, &stFrameInfo, timeout);
		if (RETURN_OK != ret)
		{
			PRINT_ERROR("Error(%d - %x): NI_MDK_VPS_GetFrame\n", ret, ret);
			usleep(10 * 1000);
			continue;
		}
		else
		{
			PRINT_INFO("CaptureGetYuvPic success\n");
			break;
		}
	}

	if (len < stFrameInfo.u32FrameSize)
	{
		PRINT_ERROR("CaptureGetYuvPic len %d < stFrameInfo.u32FrameSize %d\n", len, stFrameInfo.u32FrameSize);
		ret = NI_MDK_VPS_ReleaseFrame(VpsGrp, VpsChn, &stFrameInfo);
		if (RETURN_OK != ret)
		{
			PRINT_ERROR("Error(%d - %x): NI_MDK_VPS_ReleaseFrame\n", ret, ret);
		}
		return -1;
	}

	pVirtAddr = NI_MDK_SYS_Mmap(stFrameInfo.astVideoFrame[0].aphysAddr[0], stFrameInfo.u32FrameSize);

	memcpy(picData, pVirtAddr, stFrameInfo.u32FrameSize);

	ret = NI_MDK_SYS_Munmap(pVirtAddr, stFrameInfo.u32FrameSize);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("Error(%d - %x): NI_MDK_SYS_Munmap\n", ret, ret);
		ret = NI_MDK_VPS_ReleaseFrame(VpsGrp, VpsChn, &stFrameInfo);
		if (RETURN_OK != ret)
		{
			PRINT_ERROR("Error(%d - %x): NI_MDK_VPS_ReleaseFrame\n", ret, ret);
		}
		return -1;
	}

	picSize->u32Height = stFrameInfo.astVideoFrame[0].u32Height;
	picSize->u32Width = stFrameInfo.astVideoFrame[0].u32Width;

	ret = NI_MDK_VPS_ReleaseFrame(VpsGrp, VpsChn, &stFrameInfo);
	if (RETURN_OK != ret)
	{
		PRINT_ERROR("Error(%d - %x): NI_MDK_VPS_ReleaseFrame\n", ret, ret);
		return -1;
	}

	pVirtAddr = NULL;
	return 0;
}

/// 开始编码。在视频制式、捕获选项、图像质量、帧率、码流控制方式、分辨率、编码
/// 模式等参数被改变时，应用程序会停止捕获，然后重新开始捕获。
/// 
/// \param [in] channel 通道号。
/// \param [in] dwType 参数表示启动捕获的选项，是capture_start_t类型的值的组合。
/// \retval 0  开始成功
/// \retval 0  开始失败
int CaptureStart(int  channel, DWORD dwType)
{
	PRINT_TRACE("CaptureStart>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>channel[%d]\n",dwType);

	int ret = -1,i;
	int jpeg = STREAM_TYPE_THI;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

	for (i = 0; i < pCaptureDevice->EncDevice[channel].StreamCount; i++)
	{
		if (pCaptureDevice->EncDevice[channel].StreamDevice[i].CapChn == dwType && dwType == CHL_JPEG_T)
		{	
			ret = NI_MDK_VENC_StartRecvPic(jpeg);
			if (ret)
			{
				PRINT_ERROR("Error(%x): NI_MDK_VENC_StartRecvPic chn(%d)!\n", ret, dwType);
			}

			pthread_rwlock_wrlock(&jpeg_rwlock);
			pCaptureDevice->EncDevice[channel].StreamDevice[i].VencStart = ENABLE;
			pthread_rwlock_unlock(&jpeg_rwlock);

			return 0;
		}
	}

	// ret = NI_MDK_VENC_StartRecvPic(dwType);
	// if(ret)
	// {
	// 	PRINT_ERROR("Error(%x): NI_MDK_VENC_StartRecvPic chn(%d)!\n", ret, dwType);
	// 	return -1;
	// }

	return 0;
}


/// 停止编码，当前操作立即返回。
/// 
/// \param [in]  channel 通道号。
/// \param [in]  dwType
/// \retval 0  停止成功
/// \retval 0  停止失败
int CaptureStop(int  channel, DWORD dwType)
{
	PRINT_TRACE("CaptureStop>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>channel[%d]\n",dwType);
	
	int i;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

	for (i = 0; i < pCaptureDevice->EncDevice[channel].StreamCount; i++)
	{
		if (pCaptureDevice->EncDevice[channel].StreamDevice[i].CapChn == dwType && dwType == CHL_JPEG_T)
		{
			return RETURN_OK;
		}
	}

	// ret = NI_MDK_VENC_StopRecvPic(dwType);
	// if(ret)
	// {
	// 	PRINT_ERROR("Error(%x): NI_MDK_VENC_StopRecvPic chn(%d)!\n", ret, dwType);
	// 	return -1;
	// }

	return 0;
}

void audio_lock_enter()
{
	pthread_mutex_lock(&audio_lock);
}

void audio_lock_leave()
{
	pthread_mutex_unlock(&audio_lock);
}

// 开始视频流捕获
// 仅格式化SD卡使用
int capture_video_stream_start(void)
{
	int i = 0;
	int ret = -1;
	int EncCnt = 0;
	int venc_channel = 0;
	TEST_VENC_CONFIG_S stVencCfg;
	int jpeg_chn = STREAM_TYPE_THI; // 抓图为第三通道
	struct channel_info *info = NULL;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
	EncCnt = pCaptureDevice->EncCount;

	memset(&stVencCfg, 0, sizeof(TEST_VENC_CONFIG_S));

	for (; i < EncCnt; i++)
	{
		for (venc_channel = 0; venc_channel < pCaptureDevice->EncDevice[i].StreamCount; venc_channel++)
		{
			if (CHL_3IRD_T != venc_channel)
			{
				if (TRUE == g_video_stream_running[info->channel])
				{
					PRINT_ERROR("capture_video_stream_start g_video_stream_running[%d] %d\n", info->channel, g_video_stream_running[info->channel]);
					continue;
				}

				g_video_stream_channel[info->channel] = info->channel;

				g_video_stream_running[info->channel] = TRUE;

				if (pthread_create(&g_video_stream_pid[info->channel], 0, _capture_multi_media_stream_process, &g_video_stream_channel[info->channel]))
				{
					PRINT_ERROR("capture_video_stream_start pthread_create chanel %d failed\n", info->channel);
					g_video_stream_running[info->channel] = FALSE;
					return ret;
				}
			}
		}
	}

	ret = VideoSetRotate(0, pCaptureDevice->Rotate);
	if (ret)
	{
		PRINT_ERROR("Error: VideoSetRotate failed %d(%x)\n", ret, ret);
		return ret;
	}

	g_video_stream_running[jpeg_chn] = TRUE;

	if (pthread_create(&g_video_stream_pid[jpeg_chn], 0, _capture_jpeg_stream_process, NULL))
	{
		PRINT_ERROR("capture_video_stream_start pthread_create chanel %d failed\n", jpeg_chn);
		g_video_stream_running[jpeg_chn] = FALSE;
		return ret;
	}

	return ret;
}

// 停止视频流捕获
// 仅格式化SD卡使用
int capture_video_stream_stop(void)
{
	int i = 0;
	int channel = 0;
	int vencChannel = 0;
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

	for (int i = 0; i < VENC_MAX_CHN_NUM; i++)
	{
		if (TRUE == g_video_stream_running[i])
		{
			g_video_stream_running[i] = FALSE;
			pthread_join(g_video_stream_pid[i], 0);
		}
	}

	for (i = 0; i < pCaptureDevice->EncCount; i++)
	{
		for (vencChannel = 0; vencChannel < pCaptureDevice->EncDevice[i].StreamCount; vencChannel++)
		{
			channel = pCaptureDevice->EncDevice[i].StreamDevice[vencChannel].EncChannel_info.channel;
			SDK_COMM_VENC_Stop(channel);
		}
	}

	return 0;
}

int capture_init(void)
{
	int venc_channel = 0;
	int ret = -1,i;
	int EncCnt = 0;
	struct channel_info* info;
	int jpeg_chn = STREAM_TYPE_THI; //抓图为第三通道
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
	EncCnt = pCaptureDevice->EncCount;

	for(i = 0;i < EncCnt;i++)
	{
		for(venc_channel = 0;venc_channel < pCaptureDevice->EncDevice[i].StreamCount;venc_channel++)
		{
			info = &pCaptureDevice->EncDevice[i].StreamDevice[venc_channel].EncChannel_info;

			if (CHL_3IRD_T != venc_channel)
			{
				if (TRUE == g_video_stream_running[info->channel])
				{
					PRINT_ERROR("capture_init g_video_stream_running[%d] %d\n", info->channel, g_video_stream_running[info->channel]);
					continue;
				}

				g_video_stream_channel[info->channel] = info->channel;

				g_video_stream_running[info->channel] = TRUE;

				if (pthread_create(&g_video_stream_pid[info->channel], 0, _capture_multi_media_stream_process, &g_video_stream_channel[info->channel]))
				{
					PRINT_ERROR("capture_init pthread_create chanel %d failed\n", info->channel);
					g_video_stream_running[info->channel] = FALSE;
				}
			}
		}
	}

	ret = VideoSetRotate(0,pCaptureDevice->Rotate);
	if (ret)
	{
		printf("Error: VideoSetRotate failed %d(%x)\n", ret, ret);
	}

	if(TRUE == g_audio_stream_running)
	{
		PRINT_ERROR("capture_init g_audio_running %d\n", g_audio_stream_running);
		return 0;
	}

	g_audio_stream_running = TRUE;
	
	if(pthread_create(&g_audio_stream_pid, 0, _capture_audio_stream_process, NULL))
	{
		PRINT_ERROR("capture_init pthread_create _capture_audio_stream_process failed\n");
		g_audio_stream_running = FALSE;
	}

	if (TRUE == g_video_stream_running[jpeg_chn])
	{
		PRINT_ERROR("capture_init g_jpeg_running %d\n", g_video_stream_running[jpeg_chn]);
		return 0;
	}

	g_video_stream_running[jpeg_chn] = TRUE;

	if (pthread_create(&g_video_stream_pid[jpeg_chn], 0, _capture_jpeg_stream_process, NULL))
	{
		PRINT_ERROR("capture_init pthread_create chanel %d failed\n", jpeg_chn);
		g_video_stream_running[jpeg_chn] = FALSE;
	}

	jpeg_chn++;
	g_video_stream_running[jpeg_chn] = TRUE;
	if (pthread_create(&g_video_stream_pid[jpeg_chn], 0, _capture_jpeg_to_app_process, NULL))
	{
		PRINT_ERROR("capture_init pthread_create chanel %d failed\n", jpeg_chn);
		g_video_stream_running[jpeg_chn] = FALSE;
	}

	//创建编码自适应线程
	/*if (video_venc_adaptive_start())
	{
		PRINT_ERROR("_venc_adaptive_process pthread_create failed\n");
		return -1;
	}*/

	return 0;
}

void capture_stop_stream_proc()
{
	for(int i = 0; i < VENC_MAX_CHN_NUM; i++)
	{
		if(TRUE == g_video_stream_running[i])
		{
			g_video_stream_running[i] = FALSE;
        	pthread_join(g_video_stream_pid[i],0);
		}
	}

	if(TRUE == g_audio_stream_running)
	{
		g_audio_stream_running = FALSE;
        pthread_join(g_audio_stream_pid,0);
	}

	//video_venc_adaptive_stop();
}
