/* 
 * sdk_comm_smart_enc.c -- smart_enc sample用例 
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#include <fcntl.h>
#include <pthread.h>
#include <sys/prctl.h>
#include <time.h>

#include "ni_comm_venc.h"
#include "mdk_venc.h"
#include "sdk_comm_smart_enc.h"
#include "ni_macro.h"
#include "ni_define.h"
#include "mdk_smart_enc.h"

/* Polynomial for CRC32 */
#define QUOTIENT 0x04c11db7

#define	MB_V_STEP		16
#define MB_H_STEP		16

#define BMB_V_STEP		32
#define BMB_H_STEP		32


#define VENC_SEND_MEM_BLK_NUM    (4)


typedef struct tagSendMemBlkDesc
{
    NI_BOOL             bInit;
    NI_U32              u32BlockNum;
    NI_U32              u32BlockSize;
    MBUF_POOL           PoolId;
}SEND_MEM_BLK_DESC_S;

typedef struct tagSampleSmartEncAverBit
{
    NI_BOOL bSmartEncEnable;
    NI_U32 u32AverBitRate;
    pthread_mutex_t mutex_lock;
}SAMPLE_SMART_ENC_AVER_BIT_S;

typedef struct tagVencTestSendFrm
{
     NI_BOOL     bThreadStart;
     VENC_CHN    VeChn;
     NI_U32      u32Width;
     NI_U32      u32Height;
     NI_U32      u32FrameSize;
     NI_U32      u32YuvFileFrameSize;
     NI_U32      u32TimeOut;
     char        acFileName[128];
	 char        *pcFileModeName;
     PAYLOAD_TYPE_E enEncType;
	 SMART_ENC_HANDLE hHandle;
	 SAMPLE_SMART_ENC_AVER_BIT_S stSmartAverBit;
}VENC_SAMPLE_SENDFRM_PARA_S;

//static pthread_t s_lock;

static pthread_t s_astTestCommVencGetStreamPid[VENC_MAX_CHN_NUM];

static pthread_t s_astVencSendStreamPid[VENC_MAX_CHN_NUM];

static NI_U32 s_as32TestCommSnapCnt[VENC_MAX_CHN_NUM];
static SDK_COMM_GETSTREAM_PARA_S s_astTestCommVencPara[VENC_MAX_CHN_NUM];

static NI_U32 s_au32Crctab[256];

static VENC_SAMPLE_SENDFRM_PARA_S s_astVencPara[VENC_MAX_CHN_NUM];

//static SEND_MEM_BLK_DESC_S g_stSendMemBlkSesc[VENC_MAX_CHN_NUM];
//static NI_U32 u32StreamNum = 0;
//static VENC_STREAM_S *s_stStream[4];

///////////////////////////////////////////////////////////////////////////////////////////////////////////

static void crc32_init()
{
  int i,  j;  
  unsigned int crc;

  for (i = 0; i < 256; i++)
  {
    crc = i << 24;
    for (j = 0; j < 8; j++)
    {
      if (crc & 0x80000000)
        crc = (crc << 1) ^ QUOTIENT;
      else
        crc = crc << 1;
    }
    s_au32Crctab[i] = crc;
  }  
}

static unsigned int crc32(unsigned char *data, int len)
{
  unsigned int        result = 0xFFFFFFFF;
  int                 i;

  for (i=0; i<len; i++)
  {
    result = (result << 8) ^ s_au32Crctab[(result >> 24)^*data++];
  }

  return ~result;
}


static unsigned int checksum(unsigned char *data, int len)
{
  unsigned int        result = 0;
  int                 i = 0, offset = 0;

  for (; i+4<=len; i+=4)
  {
#if 1
    result += (data[i] | (data[i+1]<<8) | (data[i+2]<<16) | (data[i+3]<<24));
#else
    result += ((data[i]<<24) | (data[i+1]<<16) | (data[i+2]<<8) | data[i+3]);
#endif
  }

  while(i < len) {
#if 1
    result += (data[i]) << (offset*8);
#else
    result += (data[i]) << ((3-ctx->offset)*8);
#endif
    offset++; i++;
  }  

  return result;
}

NI_U32 SAMPLE_SMART_ENC_GetBinSize(char *filename)
{
	NI_U32  size = 0;
	FILE  *fp = fopen(filename, "rb");
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		size = ftell(fp); //以字节为单位
		fclose(fp);
	}
	printf("\nfilename=%s,size=%d \n", filename, size);
	return size;
}

NI_S32 SAMPLE_SMART_ENC_ReadBin(char *path, NI_VOID *buf, NI_U32 size)
{
	FILE *infile;

	if ((infile = fopen(path, "rb")) == NULL)
	{
		printf("\nCan not open the path: %s \n", path);
		return NI_FAILURE;
	}
	fread(buf, 1, size, infile);
	fclose(infile);

	return NI_SUCCESS;
}

/******************************************************************************
* funciton : save snap stream
******************************************************************************/
static NI_S32 SDK_COMM_VENC_SaveSnap(VENC_CHN VeChn, VENC_STREAM_S *pstStream)
{
    char acFile[128]  = {0};
    FILE *pFile;
	int i;

    sprintf(acFile, "snap_chn%d_%d.jpg", VeChn, s_as32TestCommSnapCnt[VeChn]);
    pFile = fopen(acFile, "wb");
    if (pFile == NULL)
    {
        printf("%s\n","open file err");
        return NI_FAILURE;
    }
  
    //fwrite((void *)pstStream->u32StreamVirAddr, pstStream->u32StreamLen, 1, pFile);

	for (i = 0; i < pstStream->stStreamPacket.u32PackNum; i++)
	{
		fwrite((void *)pstStream->stStreamPacket.aPackVirAddr[i], pstStream->stStreamPacket.au32PackLen[i], 1, pFile);
	}
	
    fclose(pFile);
    s_as32TestCommSnapCnt[VeChn]++;

    NI_INFO("snap %s ok\n", acFile);
    
    return NI_SUCCESS;
}


static NI_S32 SDK_COMM_VENC_SaveStream(PAYLOAD_TYPE_E enType, FILE *pFd, VENC_STREAM_S *pstStream)
{
	int i;
    //fwrite((void *)pstStream->u32StreamVirAddr, pstStream->u32StreamLen, 1, pFd);
	for (i = 0; i < pstStream->stStreamPacket.u32PackNum; i++)
	{
		fwrite((void *)pstStream->stStreamPacket.aPackVirAddr[i], pstStream->stStreamPacket.au32PackLen[i], 1, pFd);
	}
    fflush(pFd);

    return NI_SUCCESS;
}


NI_S32 SDK_COMM_VENC_GetFilePostfix(PAYLOAD_TYPE_E enPayload, char *szFilePostfix)
{
    if (PT_H264 == enPayload)
    {
        strcpy(szFilePostfix, ".h264");
    }
    else if (PT_H265 == enPayload)
    {
        strcpy(szFilePostfix, ".h265");
    }
    else if (PT_JPEG == enPayload)
    {
        strcpy(szFilePostfix, ".jpg");
    }
    else if (PT_MJPEG == enPayload)
    {
        strcpy(szFilePostfix, ".mjpeg");
    }
    else if (PT_MP4VIDEO == enPayload)
    {
        strcpy(szFilePostfix, ".mp4");
    }
    else
    {
        printf("payload type err!\n");
        return NI_FAILURE;
    }
    return NI_SUCCESS;
}

#if 0
static NI_S32 VencSendMemBlkInit(VENC_CHN VeChn, NI_U32 u32BlockSize)
{
    if (NI_TRUE == g_stSendMemBlkSesc[VeChn].bInit)
    {
        return NI_SUCCESS;
    }

    g_stSendMemBlkSesc[VeChn].u32BlockNum = VENC_SEND_MEM_BLK_NUM;
    g_stSendMemBlkSesc[VeChn].u32BlockSize = u32BlockSize;

    /* 创建私有缓存池*/
    g_stSendMemBlkSesc[VeChn].PoolId = NI_MDK_MBUF_CreatePool(g_stSendMemBlkSesc[VeChn].u32BlockSize, g_stSendMemBlkSesc[VeChn].u32BlockNum, NULL);
    if (MBUF_INVALID_POOLID == g_stSendMemBlkSesc[VeChn].PoolId)
    {
        NI_ERROR("NI_MDK_MBUF_CreatePool err.\n");
        return NI_FAILURE;
    }
    
    NI_MDK_MBUF_MmapPool(g_stSendMemBlkSesc[VeChn].PoolId);

    g_stSendMemBlkSesc[VeChn].bInit = NI_TRUE;

    return NI_SUCCESS;
}

static NI_S32 VencSendMemBlkDeInit(VENC_CHN VeChn)
{
    if (NI_FALSE == g_stSendMemBlkSesc[VeChn].bInit)
    {
        return NI_SUCCESS;
    }

    NI_MDK_MBUF_MunmapPool(g_stSendMemBlkSesc[VeChn].PoolId);
    NI_MDK_MBUF_DestroyPool(g_stSendMemBlkSesc[VeChn].PoolId);
    g_stSendMemBlkSesc[VeChn].PoolId = MBUF_INVALID_POOLID;
    g_stSendMemBlkSesc[VeChn].bInit = NI_FALSE;

    return NI_SUCCESS;
}


static NI_S32 VencGetSendMemBlk(VENC_CHN VeChn, NI_U32 *pu32BlkId, NI_U32 *pu32BlkPhysAddr, NI_U32 *pu32BlkVirAddr)
{
    NI_S32 u32BlkId, u32BlkPhysAddr, u32BlkVirAddr;
    
    if (NI_TRUE != g_stSendMemBlkSesc[VeChn].bInit)
    {
        NI_ERROR("SendMemPool is not Init.\n");
        
        return NI_FAILURE;
    }
    
    u32BlkId = NI_MDK_MBUF_GetBlock(g_stSendMemBlkSesc[VeChn].PoolId, 0, NULL);
    if (MBUF_INVALID_BLKID == u32BlkId)
    {        
        return NI_FAILURE;
    }
    
    u32BlkPhysAddr = NI_MDK_MBUF_BlkId2PhysAddr(u32BlkId);
    NI_MDK_MBUF_GetBlkVirAddr(g_stSendMemBlkSesc[VeChn].PoolId, u32BlkPhysAddr, (NI_VOID **)&u32BlkVirAddr);

    *pu32BlkId = u32BlkId;
    *pu32BlkPhysAddr = u32BlkPhysAddr;
    *pu32BlkVirAddr = u32BlkVirAddr;
    
    return NI_SUCCESS;
}
#endif

static NI_S32 VencReleaseSendMemBlk(NI_U32 u32BlkId)
{    
    return NI_MDK_MBUF_ReleaseBlock(u32BlkId);
}


static NI_VOID *SAMPLE_SMART_ENC_GetVencStreamProc(NI_VOID *p)
{
    VENC_CHN_ATTR_S stVencChnAttr;
    SDK_COMM_GETSTREAM_PARA_S *pstPara;
    NI_CHAR aszFileName[64];
    FILE *apFile[SAMPLE_VENC_MAX_STREAM_CHN] = {NI_NULL};
    char szFilePostfix[10];
    VENC_STREAM_S stStream;
    NI_S32 s32Ret;
    NI_U32 au32LastPicWidth[SAMPLE_VENC_MAX_STREAM_CHN] = {0}, au32LastPicHeight[SAMPLE_VENC_MAX_STREAM_CHN] = {0};
    VENC_CHN VencChn;
    PAYLOAD_TYPE_E aenPayLoadType[SAMPLE_VENC_MAX_STREAM_CHN];
    NI_U32 u32Timeout;
    NI_S32 as32VencFd[SAMPLE_VENC_MAX_STREAM_CHN];
    NI_S32 s32MaxVencFd = 0, s32StreamChnCnt = 0;
    fd_set read_fds;
    struct timeval TimeoutVal;
    NI_U32 i = 0;
    NI_U32 u32CheckValue = 0;
	NI_U32 u32StreamLen = 0;
    
    VENC_STREAM_CHECK_TYPE_E aenCheckType[SAMPLE_VENC_MAX_STREAM_CHN];
    
    pstPara = (SDK_COMM_GETSTREAM_PARA_S*)p;
    VencChn = pstPara->VeChn;
    u32Timeout = pstPara->u32Timeout;    
    s32StreamChnCnt = pstPara->u32ValidChnCnt;

    for (i = 0; i < s32StreamChnCnt; i++)
    {
        VencChn = pstPara->astStreamChn[i].VeChn;
        pstPara->au32GetFrmNum[i] = 0;

        /* decide the stream file name, and open file to save stream */
        s32Ret = NI_MDK_VENC_GetChnAttr(VencChn, &stVencChnAttr);
        if(s32Ret != NI_SUCCESS)
        {
            NI_ERROR("NI_MDK_VENC_GetChnAttr chn[%d] failed with %#x!\n", VencChn, s32Ret);
            return NULL;
        }

        s32Ret = NI_MDK_VENC_GetStreamCheck(VencChn, &aenCheckType[i]);
        if(s32Ret != NI_SUCCESS)
        {
            NI_ERROR("NI_MDK_VENC_GetStreamCheck chn[%d] failed with %#x!\n", VencChn, s32Ret);
            return NULL;
        }
        
        aenPayLoadType[i] = stVencChnAttr.stVeAttr.enType;

        /* 获取码流保存文件名*/
        s32Ret = SDK_COMM_VENC_GetFilePostfix(aenPayLoadType[i], szFilePostfix);
        if(s32Ret != NI_SUCCESS)
        {
            NI_ERROR("SDK_COMM_VENC_GetFilePostfix [%d] failed with %#x!\n", stVencChnAttr.stVeAttr.enType, s32Ret);
            return NULL;
        }

        sprintf(aszFileName, "stream_chn%d%s", VencChn, szFilePostfix);

        if (aenPayLoadType[i] != PT_JPEG)
        {
            apFile[i] = fopen(aszFileName, "wb");
            if (!apFile[i])
            {
                NI_ERROR("open file[%s] failed!\n", aszFileName);
                return NULL;
            }
            printf("open file[%s] ok!\n", aszFileName);
        }
        
        as32VencFd[i] = NI_MDK_VENC_GetFd(VencChn);
        if (as32VencFd[i] < 0)
        {
            NI_ERROR("NI_MDK_VENC_GetFd failed with %#x!\n", as32VencFd[i]);            
            return NI_NULL;
        }

        if (s32MaxVencFd < as32VencFd[i])
        {
            s32MaxVencFd = as32VencFd[i];
        }
        
        s_as32TestCommSnapCnt[VencChn] = 0;
    }

    printf("pstPara->bThreadStart = %d\n", pstPara->bThreadStart);
    while (NI_TRUE == pstPara->bThreadStart)
    {
        FD_ZERO(&read_fds);

        for (i = 0; i < s32StreamChnCnt; i++)
        {
            FD_SET(as32VencFd[i], &read_fds);
        }
		
        TimeoutVal.tv_sec  = 3;
        TimeoutVal.tv_usec = 0;
        s32Ret = select(s32MaxVencFd + 1, &read_fds, NULL, NULL, &TimeoutVal);
        if (s32Ret < 0) 
        {
            NI_ERROR("select failed!\n");            
            return NI_NULL;
        }
        else if (0 == s32Ret) 
        {
            NI_WARN("get venc stream time out!\n");            
            continue;
        }
        else
        {
            for (i = 0; i < s32StreamChnCnt; i++)
            {
                printf("s32StreamChnCnt = %d\n", s32StreamChnCnt);
                if (FD_ISSET(as32VencFd[i], &read_fds))
                {
                    VencChn = pstPara->astStreamChn[i].VeChn;
                    
                    memset(&stStream, 0, sizeof(VENC_STREAM_S));

                    s32Ret = NI_MDK_VENC_GetStream(VencChn, &stStream, u32Timeout);
                    if ( NI_SUCCESS != s32Ret )
                    {
                        if ( NI_ERR_VENC_GET_STREAM_TIMEOUT == s32Ret )
                        {
                            continue;
                        }            
                        NI_ERROR("NI_MDK_VENC_GetStream failed with %#x!\n", s32Ret);
                        break;
                    }

                    if (NI_SUCCESS != VencReleaseSendMemBlk((NI_U32)stStream.u64PTS))
                    {
                        NI_ERROR("VencReleaseSendMemBlk %lld return error.\n", stStream.u64PTS);
                        
                        continue;
                    }

					if (NI_FALSE == stStream.stStreamPacket.bEndofFrame)
					{
						NI_WARN("stream is NULL!\n");
						continue;
					}

                    NI_INFO("GetStream %d Frame Success, u32StreamSeq=%d, u64PTS=%llu.!!\n", pstPara->au32GetFrmNum[i], 
                        stStream.u32StreamSeq, stStream.u64PTS);

                    if ((au32LastPicWidth[i] == 0) || (au32LastPicHeight[i] == 0))
                    {
                        au32LastPicWidth[i] = stStream.u32PicWidth;
                        au32LastPicHeight[i] = stStream.u32PicHeight;
                    }
                    
                    if ((stStream.u32PicWidth != au32LastPicWidth[i]) ||(stStream.u32PicHeight != au32LastPicHeight[i]))
                    {
                        if (apFile[i] != NI_NULL)
                        {
                            fclose(apFile[i]);
                            sprintf(aszFileName, "stream_chn%d_%dx%d%s", VencChn, stStream.u32PicWidth,
                                stStream.u32PicHeight,szFilePostfix);
                            apFile[i] = fopen(aszFileName, "wb");
                            if (!apFile[i])
                            {
                                NI_ERROR("open file[%s] failed!\n", aszFileName);
                                break;
                            }
                        }
                        
                        au32LastPicWidth[i] = stStream.u32PicWidth;
                        au32LastPicHeight[i] = stStream.u32PicHeight;
                    }

					for (i = 0; i < stStream.stStreamPacket.u32PackNum; i++)
					{
						u32StreamLen += stStream.stStreamPacket.au32PackLen[i];
					}

                    if (VENC_STREAM_CHECK_SUM == aenCheckType[i])
                    {

                        u32CheckValue = checksum((NI_U8 *)stStream.stStreamPacket.aPackVirAddr[0], u32StreamLen);
                        if (u32CheckValue != stStream.u32CheckValue)
                        {
                            NI_ERROR("vechn:%d check value not match.u32CheckValue=%#x, stStream.u32CheckValue=%#x,get frm %d,PackPhyAddr=%#x\n", VencChn, 
                                u32CheckValue, stStream.u32CheckValue, pstPara->au32GetFrmNum[i], stStream.stStreamPacket.au32PackPhyAddr[0]);
                        }
                    }
                    else if (VENC_STREAM_CHECK_CRC == aenCheckType[i])
                    {
                        u32CheckValue = crc32((NI_U8 *)stStream.stStreamPacket.aPackVirAddr[0], u32StreamLen);
                        if (u32CheckValue != stStream.u32CheckValue)
                        {
                            NI_ERROR("vechn:%d check value not match.u32CheckValue=%#x, stStream.u32CheckValue=%#x,get frm %d,PackPhyAddr=%#x\n", VencChn, 
                                u32CheckValue, stStream.u32CheckValue, pstPara->au32GetFrmNum[i], stStream.stStreamPacket.au32PackPhyAddr[0]);
                        }
                        else
                        {
                            //NI_INFO("vechn:%d check value is match.u32CheckValue=%#x, stStream.u32CheckValue=%#x,get frm %d,PackPhyAddr=%#x\n", VencChn, 
                            //    u32CheckValue, stStream.u32CheckValue, pstPara->au32GetFrmNum[i], stStream.stStreamPacket.au32PackPhyAddr[0]);
                        }
                    }
                    
                    if ((PT_MJPEG == aenPayLoadType[i]) || (PT_H264 == aenPayLoadType[i]) || (PT_H265 == aenPayLoadType[i]))
                    {
                        if (apFile[i] != NI_NULL)
                        {
                            s32Ret = SDK_COMM_VENC_SaveStream(aenPayLoadType[i], apFile[i], &stStream);
                            if (NI_SUCCESS != s32Ret)
                            {
                                NI_ERROR("save stream failed!\n");
                                break;
                            }
                        }
                    }
                    else if (PT_JPEG == aenPayLoadType[i])
                    {
                        s32Ret = SDK_COMM_VENC_SaveSnap(VencChn, &stStream);
                    }
                    else
                    {
                        NI_WARN("no stream saved!\n");
                    }


                    s32Ret = NI_MDK_VENC_ReleaseStream(VencChn, &stStream);
                    if (NI_SUCCESS != s32Ret)
                    {
                        NI_ERROR("NI_MDK_VENC_ReleaseStream failed with %#x!\n", s32Ret);
                        break;
                    }

                    pstPara->au32GetFrmNum[i]++;
                    //printf("get end %d, stStream.u32StreamLen=%d\n", pstPara->u32GetFrmNum, stStream.u32StreamLen);///////////////////////////
                }   
            }
        }        
    }

    for (i = 0; i < s32StreamChnCnt; i++)
    {
        if (apFile[i] != NI_NULL)
        {
            fclose(apFile[i]);
        }
    }
    
    pstPara->bThreadStart = NI_FALSE;
    
    printf("exit %s ok!\n", __FUNCTION__);

    return NULL;
}


NI_S32 SAMPLE_SMART_ENC_StopGetStream(VENC_CHN VeChn)
{
    if (NI_TRUE == s_astTestCommVencPara[VeChn].bThreadStart)
    {
        s_astTestCommVencPara[VeChn].bThreadStart = NI_FALSE;
		pthread_join(s_astTestCommVencGetStreamPid[VeChn], 0);
    }
    
    return NI_SUCCESS;
}


NI_S32 SAMPLE_SMART_ENC_StopSendStream(VENC_CHN VeChn)
{
	if (NI_TRUE == s_astVencPara[VeChn].bThreadStart)
	{	 
		s_astVencPara[VeChn].bThreadStart = NI_FALSE;
		pthread_join(s_astVencSendStreamPid[VeChn], 0);
	}

	//VencSendMemBlkDeInit(VeChn);
		
	return NI_SUCCESS;

}

NI_S32 SAMPLE_SMART_ENC_Stop(VENC_CHN VeChn)
{
	
    EXPECT_EQ(NI_SUCCESS, NI_MDK_VENC_StopRecvPic(VeChn));

	/*智能编码销毁*/
	EXPECT_EQ(NI_SUCCESS, NI_MDK_SMART_ENC_Destroy(s_astVencPara[VeChn].hHandle));
	
    EXPECT_EQ(NI_SUCCESS, NI_MDK_VENC_DestroyChn(VeChn));

	return NI_SUCCESS;
}

SMART_ENC_HANDLE SAMPLE_SMART_ENC_SmartEncInit(NI_CHAR *BinFileName, SMART_ENC_TYPE_E enEncType, SMART_ENC_MODE_E enEncMode, SMART_ENC_ROI_MODE_E enSmartMode, NI_U32 u32Gop)
{
	NI_S32 s32Ret = NI_FAILURE;
	NI_U32 u32BinSize = 0;
	NI_VOID *pParamCfgVirAddr = NULL;
    SMART_ENC_HANDLE_ATTR_S stCreate;
	SMART_ENC_HANDLE hHandle;
	SMART_ENC_PARAM_S stEncSetCfg;

	/*读取bin文件*/
	u32BinSize = SAMPLE_SMART_ENC_GetBinSize(BinFileName);
	pParamCfgVirAddr = (NI_VOID *)malloc(u32BinSize);
	SAMPLE_SMART_ENC_ReadBin(BinFileName, pParamCfgVirAddr, u32BinSize);

	/*参数初始化*/
	stCreate.enEncType = enEncType;
	stCreate.enSmartMode = enSmartMode;
	stCreate.enSmartMode = SMART_ENC_ROI_MODE_M0;
	stCreate.stEncImgSize.u32Height = 1080;
	stCreate.stEncImgSize.u32Width = 1920;
	stCreate.stDetectImgSize.u32Height = 320;
	stCreate.stDetectImgSize.u32Width = 480;
	stCreate.stCfgFileAddr.pParamCfgVirAddr = pParamCfgVirAddr;
	stCreate.stCfgFileAddr.u32ParamCfgLen = u32BinSize;

	stEncSetCfg.enEncMode = enEncMode;
	stEncSetCfg.u16Quality = 4;
	stEncSetCfg.u32Gop = u32Gop;
	stEncSetCfg.u32BitRate = 2048;

	if (SMART_ENC_TYPE_H264 == enEncType)
	{
	    stEncSetCfg.u32BitRate = 2 * stEncSetCfg.u32BitRate;
	}

	hHandle = NI_MDK_SMART_ENC_Create(&stCreate);
	if (NULL == hHandle)
	{
		printf("Creat channel failed !\n");
		free(pParamCfgVirAddr);
		pParamCfgVirAddr = NULL;
		return NI_FALSE;
	}

	//s32Ret =  NI_MDK_SMART_ENC_SetCfg(hHandle, &stEncSetCfg);
	if (s32Ret != NI_SUCCESS)
	{
		printf("Set attribute failed !\n");
	}
	free(pParamCfgVirAddr);
	pParamCfgVirAddr = NULL;
	return hHandle;

}

NI_S32 SAMPLE_SMART_ENC_VencInit(VENC_SAMPLE_SENDFRM_PARA_S *pstSmartConfig)
{
    /* 编码通道的码率控制高级参数 */
    VENC_RC_PARAM_S stRcParam;

	if (pstSmartConfig == NULL)
	{
	    printf("%s:%d input paramters error! \n",__FUNCTION__,__LINE__);
        return NI_FAILURE;
	}

	/* 初始化线程互斥锁 */
    pthread_mutex_init(&pstSmartConfig->stSmartAverBit.mutex_lock, NULL);

	EXPECT_EQ(NI_SUCCESS, NI_MDK_VENC_GetRcParam(pstSmartConfig->VeChn, &stRcParam));


	EXPECT_EQ(NI_SUCCESS, NI_MDK_VENC_SetRcParam(pstSmartConfig->VeChn, &stRcParam));

	return NI_SUCCESS;
}

NI_S32 SAMPLE_SMART_ENC_SendYUVFrameM0(NI_VOID *p)
{
	NI_U32 u32FrameNum, u32FileSize, u32FrameCnt;
	SMART_ENC_HANDLE hHandle;
	SMART_ENC_RESULT_S *pstResult;
	VENC_SAMPLE_SENDFRM_PARA_S *pstSmartEncConf = NULL;
	SMART_ENC_FRAME_INFO_S *pstDetectInfo;
	VIDEO_FRAME_S stVideoFrame;
	VENC_QPMAP_FRAME_S *pstQpMapFrame;
	NI_S32 s32Ret;
	NI_U32 u32PhyAddr, u32EncPhyAddr, u32EncVirAddr;
    NI_VOID *pVirAddr = NULL;
	NI_U32 u32BlkId = 0;
	VENC_RC_PARAM_S *pstRcParam;
	NI_CHAR szFilePostfix[10];
	NI_CHAR aszFileName[192];
	NI_U32 u32FrameSize;
	NI_U16 u16Height = 320;
	NI_U16 u16Width = 480;
	NI_U16 u16EncWidth = 1920;
	NI_U16 u16EncHeight = 1080;
	NI_U32 u32QpmapSize;
	NI_U32 u32BlkSize = 16 * 2;
	VENC_STREAM_S stStream;
	VENC_QPMAP_ATTR_S *pstQpMap;

	FILE *fpEnc = NULL;
	FILE *fpDetect = NULL;
	FILE *fpFile = NULL;

	pstSmartEncConf = (VENC_SAMPLE_SENDFRM_PARA_S *)p;

	pstResult = (SMART_ENC_RESULT_S *)malloc(sizeof(SMART_ENC_RESULT_S));
	pstDetectInfo = (SMART_ENC_FRAME_INFO_S *)malloc(sizeof(SMART_ENC_FRAME_INFO_S));
	pstQpMapFrame = (VENC_QPMAP_FRAME_S *)malloc(sizeof(VENC_QPMAP_FRAME_S));
	pstRcParam = (VENC_RC_PARAM_S *)malloc(sizeof(VENC_RC_PARAM_S));
	pstQpMap = (VENC_QPMAP_ATTR_S *)malloc(sizeof(VENC_QPMAP_ATTR_S));

	u32FrameSize = pstSmartEncConf->u32FrameSize;
	hHandle = pstSmartEncConf->hHandle;
	
	 /* MMZ申请内存 */
    s32Ret = NI_MDK_SYS_MmzAlloc(&pstDetectInfo->stDetectStream.au32FramePhyAddr[1], \
		(NI_VOID **)&pstDetectInfo->stDetectStream.apu8FrameVirAddr[1], u16Width * u16Height, "Frame", NULL);
    if(s32Ret != NI_SUCCESS)
    {
        printf("%s:%d mmz memory failed ! \n",__FUNCTION__,__LINE__);
        goto _Exit_;
    }
	pstDetectInfo->stDetectStream.apu8FrameVirAddr[0] = pstDetectInfo->stDetectStream.apu8FrameVirAddr[1];
	pstDetectInfo->stDetectStream.au32FramePhyAddr[0] = pstDetectInfo->stDetectStream.au32FramePhyAddr[1];
	pstDetectInfo->stDetectStream.apu8FrameVirAddr[1] = NULL;
	pstDetectInfo->stDetectStream.apu8FrameVirAddr[2] = NULL;
	pstDetectInfo->stDetectStream.u16Height = u16Height;
	pstDetectInfo->stDetectStream.u16Width = u16Width;
	pstDetectInfo->stDetectStream.enImageType = SMART_ENC_IMAGE_TYPE_YUV420P;
	pstDetectInfo->u16Gain = 1000;
	pstDetectInfo->u32AverBitRate = 0;

	u32QpmapSize = ((pstSmartEncConf->u32Width + u32BlkSize - 1) / u32BlkSize) * ((pstSmartEncConf->u32Height + u32BlkSize - 1) / u32BlkSize) * 8;

	 /* MMZ申请内存存放Qpmap */
    s32Ret = NI_MDK_SYS_MmzAlloc(&u32PhyAddr, &pVirAddr, u32QpmapSize, "qpmap", NULL);
    if(s32Ret != NI_SUCCESS)
    {
        printf("%s:%d mmz memory failed ! \n",__FUNCTION__,__LINE__);
        goto _Exit1_;
    }

	s32Ret = NI_MDK_SYS_MmzAlloc(&u32EncPhyAddr, (NI_VOID **)&u32EncVirAddr, u32FrameSize, "venc_yuv_send", NULL) ;
	if(s32Ret != NI_SUCCESS)
    {
        printf("%s:%d mmz memory failed ! \n",__FUNCTION__,__LINE__);
        goto _Exit0_;
    }

	/*设置编码流*/
	sprintf(aszFileName, "%s_1920x1080_yuv420p.yuv", pstSmartEncConf->acFileName);
	fpEnc = fopen(aszFileName, "rb");
	if (NULL == fpEnc)
	{
		NI_ERROR("open %s failed !\n", aszFileName);
		s32Ret = NI_FAILURE;
		goto _Exit_;
	}
	fseek(fpEnc, 0, SEEK_END);     
    u32FileSize = ftell(fpEnc);
    u32FrameNum = u32FileSize / (u16EncWidth * u16EncHeight * 3 / 2);
	rewind(fpEnc);

	/*设置检测流*/
	sprintf(aszFileName, "%s_480x320_yuv420p.yuv", pstSmartEncConf->acFileName);
	fpDetect = fopen(aszFileName, "rb");
	if (NULL == fpDetect)
	{
		NI_ERROR("open %s failed !\n", aszFileName);
		s32Ret = NI_FAILURE;
		goto _Exit_;
	}
	rewind(fpDetect);
    
	/*获取码流名称*/
	s32Ret = SDK_COMM_VENC_GetFilePostfix(pstSmartEncConf->enEncType, szFilePostfix);/* 获取码流保存文件名*/
	if(s32Ret != NI_SUCCESS)
	{
		NI_ERROR("SDK_COMM_VENC_GetFilePostfix [%d] failed with %d!\n", pstSmartEncConf->enEncType, s32Ret);
		goto _Exit_;
	}
	
	sprintf(aszFileName, "stream_%s_%c%c%s", pstSmartEncConf->pcFileModeName, pstSmartEncConf->acFileName[11], \
			pstSmartEncConf->acFileName[12], szFilePostfix);
	fpFile = fopen(aszFileName, "wb");
	if (!fpFile)
	{
		NI_ERROR("open file stream_%s_%c%c%s failed!\n", pstSmartEncConf->pcFileModeName, pstSmartEncConf->acFileName[11], \
			pstSmartEncConf->acFileName[12], szFilePostfix);
		s32Ret = NI_FAILURE;
		goto _Exit_;
	}
    
	/*获取编码器参数*/
	s32Ret = NI_MDK_VENC_GetRcParam(pstSmartEncConf->VeChn, pstRcParam);
	if (s32Ret != NI_SUCCESS)
	{
		NI_ERROR("NI_MDK_VENC_GetRcParam fail!\n");
        goto _Exit_;
	}
    
	u32FrameCnt = 0;

	memset(pVirAddr, 0, u32QpmapSize);
	pstResult->stQpMapFrame.pQpMapVirAddr = (NI_VOID *)pVirAddr;
	pstResult->stQpMapFrame.QpMapPhyAddr = u32PhyAddr;
	pstDetectInfo->stForeInfo.u32ObjRectNum = 0;

	clock_t start, stop;
	start = clock();
	
	while (u32FrameCnt < 500)
	{
	    printf("u32FrameCnt = %d \n", u32FrameCnt);
		/*if (NI_SUCCESS != VencGetSendMemBlk(pstSmartEncConf->VeChn, &u32BlkId, &u32EncPhyAddr, &u32EncVirAddr))
        {
            printf("VencGetSendMemBlk usleep \n");
            usleep(10000);

            continue;
        }*/

		fread(pstDetectInfo->stDetectStream.apu8FrameVirAddr[0], sizeof(NI_U8), \
			u16Height * u16Width, fpDetect);
		memset(pVirAddr, 0, u32QpmapSize);

        s32Ret = NI_MDK_SMART_ENC_Process(hHandle, pstDetectInfo, pstResult);
		if (s32Ret != NI_SUCCESS)
		{
		    NI_ERROR("NI_MDK_SMART_ENC_Process fail!\n");
			break;
		}
		
		// pstRcParam->stRcParamH26x.bCtbRc = pstResult->u32CtbRc;
		// pstRcParam->stRcParamH26x.u32BlockRCSize = pstResult->u32BlockRCSize;
		// pstRcParam->stRcParamH26x.u32MinIQp = pstResult->stQpRangeI.u16QpMin;
		// pstRcParam->stRcParamH26x.u32MaxIQp = pstResult->stQpRangeI.u16QpMax;
		// pstRcParam->stRcParamH26x.u32MinPQp = pstResult->stQpRangeP.u16QpMin;
		// pstRcParam->stRcParamH26x.u32MaxPQp = pstResult->stQpRangeP.u16QpMax;

		/*printf( "======press any key to Stop Send Stream===========\n");
    	getchar();*/
		
        s32Ret = NI_MDK_VENC_SetRcParam(pstSmartEncConf->VeChn, pstRcParam);
		if (s32Ret != NI_SUCCESS)
		{
		    NI_ERROR("NI_MDK_VENC_SetRcParam fail!\n");
			break;
		}

		fread((NI_U8 *)u32EncVirAddr, u16EncHeight * u16EncWidth * 3 / 2, 1, fpEnc);
		memset(&stVideoFrame, 0, sizeof(VIDEO_FRAME_S));
		memset(pstQpMapFrame, 0, sizeof(VENC_QPMAP_FRAME_S));
		
		stVideoFrame.apVirAddr[0] = (NI_VOID *)u32EncVirAddr;
		stVideoFrame.aphysAddr[0] = u32EncPhyAddr;
		stVideoFrame.u32TimeRef = u32FrameCnt;
		//stVideoFrame.u64Pts = u32BlkId;
		stVideoFrame.u32Width = u16EncWidth;
	    stVideoFrame.u32Height = u16EncHeight;
		stVideoFrame.u32BlkId = u32BlkId;
	    stVideoFrame.enPixelFormat = PIXEL_FORMAT_YUV_PLANAR_420;

		pstQpMapFrame->stVideoFrame = stVideoFrame;
		
		pstQpMapFrame->stQpMapInfo.u32QpMapHeight = pstResult->stQpMapFrame.u16QpMapHeight;
		pstQpMapFrame->stQpMapInfo.u32QpMapWidth = pstResult->stQpMapFrame.u16QpMapWidth;
		// pstQpMapFrame->stQpMapInfo.pPrivAttr = pstResult->stQpMapFrame.pQpMapVirAddr;
		pstQpMapFrame->stQpMapInfo.QpMapPhyAddr = u32PhyAddr;
		pstQpMapFrame->stQpMapInfo.u32QpMapSize = u32QpmapSize;

		pstQpMap->bQpMapEn = NI_TRUE;
		pstQpMap->stQpMapInfo = pstQpMapFrame->stQpMapInfo;

		if (u32FrameCnt % pstResult->u32Gop != 0)
		{
			s32Ret = NI_MDK_VENC_SetQpMapAttr(pstSmartEncConf->VeChn, pstQpMap);
			if (s32Ret != NI_SUCCESS)
			{
			    NI_ERROR("SendFrm failed ! s32Ret = %d\n", s32Ret);
			    VencReleaseSendMemBlk(u32BlkId);
				break;
			}
		}

		//s32Ret = NI_MDK_VENC_SendQpMapFrm(pstSmartEncConf->VeChn, pstQpMapFrame, 1000);
		s32Ret = NI_MDK_VENC_SendFrm(pstSmartEncConf->VeChn, &stVideoFrame, 1000);
		if (s32Ret != NI_SUCCESS)
		{
		    NI_ERROR("SendFrm failed ! s32Ret = %d\n", s32Ret);
		    VencReleaseSendMemBlk(u32BlkId);
			break;
		}
		
        /*GetStream*/
		memset(&stStream, 0, sizeof(VENC_STREAM_S));
		s32Ret = NI_MDK_VENC_GetStream(pstSmartEncConf->VeChn, &stStream, 10000);
		if ( NI_SUCCESS != s32Ret )
		{
			if ( NI_ERR_VENC_GET_STREAM_TIMEOUT == s32Ret )
			{
			    printf("NI_ERR_VENC_GET_STREAM_TIMEOUT\n");
				continue;
			}

			NI_ERROR("NI_MDK_VENC_GetStream failed with %#x!\n", s32Ret);
			break;
		}

		/*if (NI_SUCCESS != VencReleaseSendMemBlk((NI_U32)u32BlkId))
		{
			NI_ERROR("VencReleaseSendMemBlk %d return error.\n", u32BlkId);
						
			continue;
		}*/

		if (stStream.stStreamPacket.bEndofFrame == NI_FALSE)
		{
			continue;
		}

        /*SaveStream*/
		s32Ret = SDK_COMM_VENC_SaveStream(pstSmartEncConf->enEncType, fpFile, &stStream);	
        if (NI_SUCCESS != s32Ret)
        {
            NI_ERROR("save stream failed!\n");
            break;
        }

		s32Ret = NI_MDK_VENC_ReleaseStream(pstSmartEncConf->VeChn, &stStream);
        if (NI_SUCCESS != s32Ret)
        {
            NI_ERROR("NI_MDK_VENC_ReleaseStream failed with %#x!\n", s32Ret);
            break;
        }

		pstDetectInfo->u16Gain = 1000;

		u32FrameCnt += 1;

		if (u32FrameCnt % u32FrameNum == 0)
        {
            /* 读到文件尾部后从头开始读*/
            rewind(fpEnc);
			rewind(fpDetect);
        }
		else
		{
		    fseek(fpDetect, u16Height * u16Width / 2, SEEK_CUR);
		}

		if (u32FrameCnt % 25 == 0)
		{
		    stop = clock();
			printf("time = %f\n", (double)(stop - start) / ((clock_t)1000));
			start = clock();
		}
		printf("------------------------------------------------------------------------\n");

	}
_Exit0_:
	NI_MDK_SYS_MmzFree(u32EncPhyAddr, (NI_VOID *)u32EncVirAddr);

_Exit1_:
	NI_MDK_SYS_MmzFree(u32PhyAddr,pVirAddr);

_Exit_:
	NI_MDK_SYS_MmzFree(pstDetectInfo->stDetectStream.au32FramePhyAddr[0], pstDetectInfo->stDetectStream.apu8FrameVirAddr[0]);

    if (fpEnc != NI_NULL)
    {
        fclose(fpEnc);
    }
    
    if (fpDetect != NI_NULL)
    {
        fclose(fpDetect);
    }
	if (fpFile != NI_NULL)
    {
        fclose(fpFile);
    }

	/* 释放申请的MMZ */

	s_astVencPara[pstSmartEncConf->VeChn].bThreadStart = NI_FALSE;

	free(pstRcParam);
	free(pstQpMapFrame);
	free(pstDetectInfo);
	free(pstResult);
	free(pstQpMap);
	pstRcParam = NULL;
	pstQpMapFrame = NULL;
	pstDetectInfo = NULL;
	pstResult = NULL;
	pstQpMap = NULL;
	return s32Ret;
}


NI_S32 SAMPLE_SMART_ENC_StartSendStream(VENC_CHN VeChn, PAYLOAD_TYPE_E enType, VENC_RC_MODE_E enRcMode, NI_CHAR *pcFileName)
{
	VENC_CHN_ATTR_S stChnAttr;
	VENC_STREAM_CHECK_TYPE_E enCheckType;
	NI_U32 u32Width;
	NI_U32 u32Height;
	NI_CHAR *BinFileName = "smart_enc_cfg-h265.bin";
	SMART_ENC_TYPE_E enEncType = SMART_ENC_TYPE_H265;
	SMART_ENC_MODE_E enEncMode = SMART_ENC_MODE_VBR;
	SMART_ENC_HANDLE hHandle;
	SMART_ENC_ROI_MODE_E enSmartMode;
	NI_U32 u32Gop = 0;

	enCheckType = VENC_STREAM_CHECK_OFF;
	u32Width = 1920;
	u32Height = 1080;

	/*初始化编码器参数*/
	stChnAttr.stVeAttr.enType = enType;
	stChnAttr.stRcAttr.enRcMode = enRcMode;
	enSmartMode = SMART_ENC_ROI_MODE_M0;

	stChnAttr.stVeAttr.stInputPicAttr.enPixelFormat = PIXEL_FORMAT_YUV_PLANAR_420;
	stChnAttr.stVeAttr.stInputPicAttr.u32MaxPicHeight = 2160;
	stChnAttr.stVeAttr.stInputPicAttr.u32MaxPicWidth = 3840;
	stChnAttr.stVeAttr.stInputPicAttr.u32PicHeight = u32Height;
	stChnAttr.stVeAttr.stInputPicAttr.u32PicWidth = u32Width;
	stChnAttr.stVeAttr.u32StrmBufSize = u32Width * u32Height * 4;
	
	stChnAttr.stRcAttr.u32FrmRateDenom = 1;
	stChnAttr.stRcAttr.u32FrmRateNum = 25;

	if (PT_H264 == enType)
	{
		stChnAttr.stVeAttr.enProfile = VENC_H264_MAIN_PROFILE;
		enEncType = SMART_ENC_TYPE_H264;
		BinFileName = "smart_enc_cfg-h264.bin";

		if (VENC_RC_MODE_CBR == enRcMode)
		{
			stChnAttr.stRcAttr.stAttrCbr.u32TargetBitRate = 4096;
			stChnAttr.stRcAttr.stAttrCbr.u32StatTime = 1;
			

			// stChnAttr.stGopAttr.u32Gop = stChnAttr.stRcAttr.u32FrmRateNum * 2;
			// stChnAttr.stGopAttr.u32DummyIGop = 0;
			// stChnAttr.stGopAttr.u32DummyIOffset = 0;
			//stChnAttr.stGopAttr.u32DummyIType = 1;
			//stChnAttr.stGopAttr.pPrivAttr = 0;
			printf("u32TargetBitRate = %d\n", stChnAttr.stRcAttr.stAttrCbr.u32TargetBitRate);
			enEncMode = SMART_ENC_MODE_CBR;
			
		}
		else if (VENC_RC_MODE_VBR == enRcMode)
		{
			stChnAttr.stRcAttr.stAttrVbr.u32MaxBitRate = 4096;
			stChnAttr.stRcAttr.stAttrVbr.u32StatTime = 10;
			stChnAttr.stRcAttr.stAttrVbr.u32MinIQp = 10;
			stChnAttr.stRcAttr.stAttrVbr.u32MaxIQp = 51;
			stChnAttr.stRcAttr.stAttrVbr.u32MinPQp = 10;
			stChnAttr.stRcAttr.stAttrVbr.u32MaxPQp = 51;

			// stChnAttr.stGopAttr.u32Gop = stChnAttr.stRcAttr.u32FrmRateNum * 6;
			// stChnAttr.stGopAttr.u32DummyIGop = 0;
			// stChnAttr.stGopAttr.u32DummyIOffset = 0;
			//stChnAttr.stGopAttr.u32DummyIType = 1;
			//stChnAttr.stGopAttr.pPrivAttr = 0;
			enEncMode = SMART_ENC_MODE_VBR;
			printf("u32MaxBitRate = %d\n", stChnAttr.stRcAttr.stAttrVbr.u32MaxBitRate);
		}
		else
		{
		    NI_ERROR("This mode is not exist!\n");
			return NI_FAILURE;
		}

	}
	else if (PT_H265 == enType)
	{

		stChnAttr.stVeAttr.enProfile = VENC_H265_MAIN_PROFILE;
		enEncType = SMART_ENC_TYPE_H265;
		BinFileName = "smart_enc_cfg-h265.bin";

		if (VENC_RC_MODE_CBR == enRcMode)
		{
			stChnAttr.stRcAttr.stAttrCbr.u32StatTime = 1;
			stChnAttr.stRcAttr.stAttrCbr.u32TargetBitRate = 2048;

			// stChnAttr.stGopAttr.u32Gop = stChnAttr.stRcAttr.u32FrmRateNum * 2;
			// stChnAttr.stGopAttr.u32DummyIGop = 0;
			// stChnAttr.stGopAttr.u32DummyIOffset = 0;
			//stChnAttr.stGopAttr.u32DummyIType = 1;
			//stChnAttr.stGopAttr.pPrivAttr = 0;
			printf("u32TargetBitRate = %d\n", stChnAttr.stRcAttr.stAttrCbr.u32TargetBitRate);
			enEncMode = SMART_ENC_MODE_CBR;
		}
		else if (VENC_RC_MODE_VBR == enRcMode)
		{

			stChnAttr.stRcAttr.stAttrVbr.u32MaxBitRate = 2048;
			stChnAttr.stRcAttr.stAttrVbr.u32StatTime = 10;
			stChnAttr.stRcAttr.stAttrVbr.u32MinIQp = 10;
			stChnAttr.stRcAttr.stAttrVbr.u32MaxIQp = 51;
			stChnAttr.stRcAttr.stAttrVbr.u32MinPQp = 10;
			stChnAttr.stRcAttr.stAttrVbr.u32MaxPQp = 51;

			// stChnAttr.stGopAttr.u32Gop = stChnAttr.stRcAttr.u32FrmRateNum * 6;
			// stChnAttr.stGopAttr.u32DummyIGop = 0;
			// stChnAttr.stGopAttr.u32DummyIOffset = 0;
			//stChnAttr.stGopAttr.u32DummyIType = 1;
			//stChnAttr.stGopAttr.pPrivAttr = 0;
			enEncMode = SMART_ENC_MODE_VBR;
			printf("u32MaxBitRate = %d\n", stChnAttr.stRcAttr.stAttrVbr.u32MaxBitRate);

		}
		else
		{
		    NI_ERROR("This mode is not exist!\n");
			return NI_FAILURE;
		}

	}
	// u32Gop = stChnAttr.stGopAttr.u32Gop;

	EXPECT_EQ(NI_SUCCESS, NI_MDK_VENC_SetStreamCheck(VeChn, enCheckType));
    EXPECT_EQ(NI_SUCCESS, NI_MDK_VENC_CreateChn(VeChn, &stChnAttr));

	/*智能编码创建*/
	hHandle = SAMPLE_SMART_ENC_SmartEncInit(BinFileName, enEncType, enEncMode, enSmartMode, u32Gop);

	if (NI_TRUE == s_astVencPara[VeChn].bThreadStart)
    {
        NI_WARN("chn:%d, send stream thread is already start.\n", VeChn);

        return NI_SUCCESS;
    }

    s_astVencPara[VeChn].VeChn = VeChn;
    s_astVencPara[VeChn].u32Width= u32Width;
    s_astVencPara[VeChn].u32Height = u32Height;
    s_astVencPara[VeChn].u32TimeOut = 1000;/*休眠时间可根据实际情况自己设置 ms*/
    s_astVencPara[VeChn].enEncType = enType;
	
    s_astVencPara[VeChn].u32YuvFileFrameSize = u32Width * u32Height * 3 / 2;

	s_astVencPara[VeChn].u32FrameSize = (u32Width * u32Height * 3 / 2 + 4095) / 4096 * 4096;
	s_astVencPara[VeChn].pcFileModeName = (enEncMode) ? "VBR" : "CBR";
	strcpy(s_astVencPara[VeChn].acFileName, pcFileName);
	s_astVencPara[VeChn].hHandle = hHandle;
	

	/*编码器参数初始化*/
	EXPECT_EQ(NI_SUCCESS, SAMPLE_SMART_ENC_VencInit(&s_astVencPara[VeChn]));

	EXPECT_EQ(NI_SUCCESS, NI_MDK_VENC_StartRecvPic(VeChn));

	crc32_init();

	/*if (VencSendMemBlkInit(VeChn, s_astVencPara[VeChn].u32FrameSize) != NI_SUCCESS)
    {
        NI_WARN("chn:%d, VencSendMemBlkInit return error.\n", VeChn);
        
        return NI_FAILURE;
    }*/

    s_astVencPara[VeChn].bThreadStart = NI_TRUE;

	/*创建线程，取VPS通道ENC0和IAS通道经smart 处理后的Qpmap 发送给VENC */
	if (enSmartMode == SMART_ENC_ROI_MODE_M0)
	{
	    pthread_create(&s_astVencSendStreamPid[VeChn], 0,\
                           (NI_VOID *)SAMPLE_SMART_ENC_SendYUVFrameM0, (NI_VOID *)&s_astVencPara[VeChn]);
	}
	else
	{
	    NI_WARN("This mode is not supported !\n");
	}
	
	return NI_SUCCESS;

}


/******************************************************************************
* 每个grp-chn开启一个独立的线程获取码流
******************************************************************************/
NI_S32 SAMPLE_SMART_ENC_StartGetStream(VENC_CHN VeChn)
{  
    if(NI_TRUE == s_astTestCommVencPara[VeChn].bThreadStart)
    {
        NI_WARN("chn:%d, get stream thread is already start.\n", VeChn);
        return NI_SUCCESS;
    }
    
    s_astTestCommVencPara[VeChn].bThreadStart = NI_TRUE;
    s_astTestCommVencPara[VeChn].VeChn = VeChn; 
    s_astTestCommVencPara[VeChn].astStreamChn[0].VeChn = VeChn;
    s_astTestCommVencPara[VeChn].u32ValidChnCnt = 1;
    s_astTestCommVencPara[VeChn].u32Timeout = 10000;
    s_astTestCommVencPara[VeChn].au32GetFrmNum[0] = 0;
    s_astTestCommVencPara[VeChn].abUserSensd[0] = NI_TRUE;

    return pthread_create(&s_astTestCommVencGetStreamPid[VeChn], 0, \
        SAMPLE_SMART_ENC_GetVencStreamProc, (NI_VOID*)&s_astTestCommVencPara[VeChn]);   
}
