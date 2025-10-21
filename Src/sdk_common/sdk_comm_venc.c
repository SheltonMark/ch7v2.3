/* 
 * sdk_comm_venc.h -- venc sample用例 
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */

#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#include "sdk_comm.h"
#include "mdk_venc.h"
#include "ni_macro.h"
#include "ni_define.h"
#include "TdCommon.h"

/* Polynomial for CRC32 */
#define QUOTIENT 0x04c11db7

typedef struct tagTestCommGetStream
{
    NI_BOOL    bThreadStart;
    VENC_CHN   VeChn;
    NI_U32     u32Timeout;
    NI_U32     u32GetFrmNum; /* 已经接收的图像帧数*/
    NI_BOOL    bSaveFile;
}TEST_COMM_GETSTREAM_PARA_S;

typedef struct tagSendMemDesc
{
    NI_BOOL             bUsed;
    NI_BOOL             bActive;
    NI_U32              u32PhyAddr;
    NI_VOID             *pVirAddr;
    NI_U32              u32Id;
}SEND_MEM_DESC_S;

typedef struct tagVencTestSendFrm
{
     NI_BOOL     bThreadStart;
     VENC_CHN    VeChn;
     NI_U32      u32Width;
     NI_U32      u32Height;
     NI_U32      u32TimeOut;
     char        acFileName[128];
     PIXEL_FORMAT_E enEncType;   
     NI_BOOL     bRead2MemBefor;
     NI_U32      u32ReadStart;
     NI_U32      u32ReadInterval;
     NI_U32      u32ReadCnt;
     NI_BOOL     bVencDebug;
}VENC_TEST_SENDFRM_PARA_S;

////////////////////////////////////GetStream//////////////////////////////////////////////////
static pthread_t s_astTestCommVencGetStreamPid[VENC_MAX_CHN_NUM];
static NI_U32 s_as32TestCommSnapCnt[VENC_MAX_CHN_NUM];
static TEST_COMM_GETSTREAM_PARA_S s_astTestCommVencPara[VENC_MAX_CHN_NUM];
//static NI_BOOL s_bHasLoaded = NI_FALSE;

//static pthread_t ssTestCommGetStreamPid;
//static TEST_COMM_GETSTREAM_PARA_S sstTestCommStreamParam;
static NI_U32 s_au32Crctab[256];
static NI_CHAR gaVencTime[25];

//////////////////////////////SendFrm////////////////////////////////////////////
static pthread_t s_astVencSendFrmmPid[VENC_MAX_CHN_NUM];
static VENC_TEST_SENDFRM_PARA_S s_astVencSendFrmPara[VENC_MAX_CHN_NUM];
static SEND_MEM_DESC_S g_stSendMemSesc[VENC_MAX_CHN_NUM][100];
static NI_BOOL  bUseVencSendFrm[VENC_MAX_CHN_NUM] = {NI_FALSE};
static NI_BOOL  bVencSendSyncWithGet[VENC_MAX_CHN_NUM] = {NI_TRUE};


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

#if 0
static NI_S32 SampleCommVencTestGetActiveId(VENC_CHN VeChn, NI_U32 u32UseNum)
{
    NI_S32 i;
    
    for ( i = 0 ; i < u32UseNum; i++ )
    {
        if (NI_TRUE == g_stSendMemSesc[VeChn][i].bActive)
        {
            return i;
        }
    }

    return -1;
}



static NI_U32 SDK_COMM_VENC_GetTimeMs(NI_VOID)
{
    struct timeval cur_time;

    gettimeofday(&cur_time, NULL);

    return (cur_time.tv_sec * 1000 + cur_time.tv_usec / 1000);
}
#endif

static NI_S32 SDK_COMM_VENC_GetCurrentTime(NI_CHAR **pszTmpTimeName)
{
    time_t sttime;
    struct tm *ptime;
    NI_U32 i;

    NI_CHAR *pszTimeName;
    NI_CHAR *pszTempTimeName;

    /* 获取当前时间 */
    time(&sttime);
    ptime = gmtime(&sttime);

    pszTimeName = gaVencTime;
    memset(pszTimeName, 0, 25);
    memcpy(pszTimeName, asctime(ptime), 24);
    pszTempTimeName = pszTimeName; 
    
    for(i = 0; i < 24; i++)
    {
        if((':' == *pszTimeName) || (' ' == *pszTimeName))
        {
            *pszTimeName = '-';
        }
        
        pszTimeName++;
    }

    pszTimeName = pszTempTimeName + 4;
    *(pszTimeName + 20) = '\0';

    *pszTmpTimeName = pszTimeName;
      
    return NI_SUCCESS;
}


/******************************************************************************
* funciton : save snap stream
******************************************************************************/
static NI_S32 SDK_COMM_VENC_SaveSnap(VENC_CHN VeChn, VENC_STREAM_S *pstStream)
{
    VENC_STREAM_BUF_MODE_E enBufMode;
    VENC_STRM_HEAD_ATTR_S stStrmHeadAttr;
    NI_VOID *pAddr = NI_NULL;
    char acFile[128]  = {0};
    FILE *pFile;
    NI_U32 i;

    if (NI_MDK_VENC_GetStreamBufMode(VeChn, &enBufMode) != NI_SUCCESS)
    {
        NI_ERROR("VeChn(%d): NI_MDK_VENC_GetStreamBufMode return Error.\n", VeChn);

        return NI_FAILURE;
    }

    if (NI_MDK_VENC_GetStrmHeadAttr(VeChn, &stStrmHeadAttr) != NI_SUCCESS)
    {
        NI_ERROR("VeChn(%d): NI_MDK_VENC_GetStrmHeadAttr return Error.\n", VeChn);

        return NI_FAILURE;
    }

    if ((stStrmHeadAttr.u32StrmHeadSize != 0 || stStrmHeadAttr.u32StrmTailSize != 0) 
        && pstStream->stStreamPacket.u32PackNum != 1)
    {
        NI_ERROR("VeChn(%d): StreamHead[%d,%d], PackNum=%d, conflicting.\n", 
            VeChn, stStrmHeadAttr.u32StrmHeadSize, stStrmHeadAttr.u32StrmTailSize, 
            pstStream->stStreamPacket.u32PackNum);

        return NI_FAILURE;
    }

    sprintf(acFile, "snap_chn%d_%d.jpg", VeChn, s_as32TestCommSnapCnt[VeChn]);
    pFile = fopen(acFile, "wb");
    if (pFile == NULL)
    {
        printf("%s\n","open file err");

        return NI_FAILURE;
    }

    for (i = 0 ; i < pstStream->stStreamPacket.u32PackNum; i++)
    {
        if (VENC_STREAM_LINE_BUF_MODE == enBufMode)
        {
            pAddr = NI_MDK_SYS_Mmap(pstStream->stStreamPacket.aPackPhyAddr[i], pstStream->stStreamPacket.au32PackLen[i]);
        }
        else
        {
            pAddr = (NI_VOID *)(pstStream->stStreamPacket.aPackVirAddr[i]);
        }

        fwrite((NI_VOID *)((NI_U8*)pAddr + stStrmHeadAttr.u32StrmHeadSize), 
            1, pstStream->stStreamPacket.au32PackLen[i] - stStrmHeadAttr.u32StrmHeadSize - stStrmHeadAttr.u32StrmTailSize, pFile);

        if (VENC_STREAM_LINE_BUF_MODE == enBufMode)
        {
            NI_MDK_SYS_Munmap(pAddr, pstStream->stStreamPacket.au32PackLen[i]);
        }
    }

    fclose(pFile);
    s_as32TestCommSnapCnt[VeChn]++;
    
    return NI_SUCCESS;
}


static NI_S32 SDK_COMM_VENC_SaveStream(VENC_CHN VeChn, FILE *pFd, VENC_STREAM_S *pstStream, NI_U32 *pu32FileSize)
{
    VENC_STREAM_BUF_MODE_E enBufMode;
    VENC_STRM_HEAD_ATTR_S stStrmHeadAttr;
    NI_VOID *pAddr = NI_NULL;
    NI_U32 i;

    if (NI_NULL == pFd || NI_NULL == pstStream)
    {
        return NI_SUCCESS;
    }

    if (NI_MDK_VENC_GetStreamBufMode(VeChn, &enBufMode) != NI_SUCCESS)
    {
        NI_ERROR("VeChn(%d): NI_MDK_VENC_GetStreamBufMode return Error.\n", VeChn);

        return NI_FAILURE;
    }

    if (NI_MDK_VENC_GetStrmHeadAttr(VeChn, &stStrmHeadAttr) != NI_SUCCESS)
    {
        NI_ERROR("VeChn(%d): NI_MDK_VENC_GetStrmHeadAttr return Error.\n", VeChn);

        return NI_FAILURE;
    }

    if ((stStrmHeadAttr.u32StrmHeadSize != 0 || stStrmHeadAttr.u32StrmTailSize != 0) 
        && pstStream->stStreamPacket.u32PackNum != 1)
    {
        NI_ERROR("VeChn(%d): StreamHead[%d,%d], PackNum=%d, conflicting.\n", 
            VeChn, stStrmHeadAttr.u32StrmHeadSize, stStrmHeadAttr.u32StrmTailSize, 
            pstStream->stStreamPacket.u32PackNum);

        return NI_FAILURE;
    }

    for (i = 0 ; i < pstStream->stStreamPacket.u32PackNum; i++)
    {
        if (VENC_STREAM_LINE_BUF_MODE == enBufMode)
        {
            pAddr = NI_MDK_SYS_Mmap(pstStream->stStreamPacket.aPackPhyAddr[i], pstStream->stStreamPacket.au32PackLen[i]);
        }
        else
        {
            pAddr = (NI_VOID *)(pstStream->stStreamPacket.aPackVirAddr[i]);
        }

        fwrite((NI_VOID *)((NI_U8*)pAddr + stStrmHeadAttr.u32StrmHeadSize), 
            1, pstStream->stStreamPacket.au32PackLen[i] - stStrmHeadAttr.u32StrmHeadSize - stStrmHeadAttr.u32StrmTailSize, pFd);
        fflush(pFd);

        *pu32FileSize += (pstStream->stStreamPacket.au32PackLen[i] - stStrmHeadAttr.u32StrmHeadSize - stStrmHeadAttr.u32StrmTailSize);

        if (VENC_STREAM_LINE_BUF_MODE == enBufMode)
        {
            NI_MDK_SYS_Munmap(pAddr, pstStream->stStreamPacket.au32PackLen[i]);
        }
    }

    return NI_SUCCESS;
}


static NI_S32 SDK_COMM_VENC_GetFilePostfix(PAYLOAD_TYPE_E enPayload, char *szFilePostfix)
{
    if (PT_H264 == enPayload)
    {
        strcpy(szFilePostfix, "h264");
    }
    else if (PT_H265 == enPayload)
    {
        strcpy(szFilePostfix, "h265");
    }
    else if (PT_JPEG == enPayload)
    {
        strcpy(szFilePostfix, "jpg");
    }
    else if (PT_MJPEG == enPayload)
    {
        strcpy(szFilePostfix, "mjpeg");
    }
    else if (PT_MP4VIDEO == enPayload)
    {
        strcpy(szFilePostfix, "mp4");
    }
    else
    {
        printf("payload type err!\n");
        return NI_FAILURE;
    }
    return NI_SUCCESS;
}


// static NI_CHAR *SDK_COMM_VENC_GetCheckType(VENC_STREAM_CHECK_TYPE_E enCheckType)
// {
//     NI_CHAR *p = NI_NULL;
    
//     switch(enCheckType)
//     {
//         case VENC_STREAM_CHECK_OFF:
//             p = "OFF";
//             break;
            
//         case VENC_STREAM_CHECK_SUM:
//             p = "SUM";
//             break;
            
//         case VENC_STREAM_CHECK_CRC:
//             p = "CRC";
//             break;
            
//         default:
//             p = "-";
//     }

//     return p;
// }


static NI_S32 SDK_COMM_VENC_SaveCuInformation(VENC_CHN VeChn, VENC_STREAM_S *pstStream, NI_U32 u32FrmNum, FILE *pFile)
{

    if (NI_NULL == pFile || NI_NULL == pstStream)
    {
        NI_ERROR("VeChn[%d],FrmNum[%d]: pFile[%#x] or pstStream[%#x] is null.\n", 
            VeChn, u32FrmNum, (NI_U32)pFile, (NI_U32)pstStream);

        return NI_FAILURE;
    }
    
    return NI_SUCCESS;
}


static NI_VOID *SDK_COMM_VENC_GetVencStreamProc(NI_VOID *p)
{
    VENC_CHN_ATTR_S stVencChnAttr;
    TEST_COMM_GETSTREAM_PARA_S *pstPara;
    NI_CHAR aszFileName[64];
    FILE *pFile = NI_NULL;
    FILE *pH26xCuInfoFile = NI_NULL;
    char szFilePostfix[10];
    NI_CHAR *lpszTimeName;
    VENC_STREAM_S stStreamForGet;
    VENC_STREAM_S *pstStream = &stStreamForGet;
    NI_S32 s32Ret;
    NI_U32 u32LastPicWidth = 0, u32LastPicHeight = 0;
    VENC_CHN VencChn;
    PAYLOAD_TYPE_E enPayLoadType;
    NI_U32 u32Timeout;
    NI_BOOL    bSaveFile;
    NI_BOOL    bCheckBs = NI_TRUE;
    NI_U32 u32CheckValue = 0;
    VENC_STREAM_CHECK_TYPE_E enCheckType;
    NI_U32 u32GetStmDebugPhyAddr = 0;
    NI_U32 *pGetStmDebugVirAddr = NULL;
    NI_BOOL bGetStmDebug = NI_FALSE;
    NI_BOOL bSaveCuInfo = NI_TRUE;
    NI_CHAR aszCuInfoFileName[64];
    NI_U32 u32FileTotalSize = 0;
    NI_BOOL bSpecCapEn = NI_FALSE;
    NI_BOOL bSpecCapStart = NI_FALSE;
    NI_U32  u32SpecCapNum = 0;
    
    pstPara = (TEST_COMM_GETSTREAM_PARA_S*)p;
    VencChn = pstPara->VeChn;
    u32Timeout = pstPara->u32Timeout;
    pstPara->u32GetFrmNum = 0;
    bSaveFile = pstPara->bSaveFile;

    s32Ret = NI_MDK_VENC_GetChnAttr(VencChn, &stVencChnAttr);
    if(s32Ret != NI_SUCCESS)
    {
        NI_ERROR("NI_MDK_VENC_GetChnAttr chn[%d] failed with %#x!\n", VencChn, s32Ret);

        return NI_NULL;
    }

	s32Ret = NI_MDK_VENC_GetStreamCheck(VencChn, &enCheckType);
    if(s32Ret != NI_SUCCESS)
    {
        NI_ERROR("NI_MDK_VENC_GetStreamCheck chn[%d] failed with %#x!\n", VencChn, s32Ret);

        return NI_NULL;
    }

	NI_DBG("-------SaveFile[%d]----CheckType[%s]-----------\n", bSaveFile, SDK_COMM_VENC_GetCheckType(enCheckType));

    NI_MDK_SYS_MmzAlloc(&u32GetStmDebugPhyAddr, (NI_VOID **)&pGetStmDebugVirAddr, 4096, "venc_getstm_debug", NULL);
    NI_INFO("GetStmDebugPhyAddr=%#x, GetStmDebugVirAddr=%#x, ofset 0 is debug mode switch, ofset 1 is get stream enable, offset 2 is print switch.\n", 
        u32GetStmDebugPhyAddr, (NI_U32)pGetStmDebugVirAddr);
    NI_PRINT("Addr[%#x]: Specific capture switch.\n", u32GetStmDebugPhyAddr + 4 * 3);
    NI_PRINT("Addr[%#x]: Specific capture time(sec).\n", u32GetStmDebugPhyAddr + 4 * 4);
    
    NI_ClearSize((NI_VOID *)pGetStmDebugVirAddr, 4096);

    if (NI_TRUE == bGetStmDebug)
    {
        pGetStmDebugVirAddr[0] = 1;
    }

    enPayLoadType = stVencChnAttr.stVeAttr.enType;

    /* 获取码流保存文件名*/
    s32Ret = SDK_COMM_VENC_GetFilePostfix(enPayLoadType, szFilePostfix);
    if(s32Ret != NI_SUCCESS)
    {
        NI_ERROR("SDK_COMM_VENC_GetFilePostfix [%d] failed with %#x!\n", \
               stVencChnAttr.stVeAttr.enType, s32Ret);

        goto _Exit;
    }
    
    if (NI_TRUE == bSaveFile)
    {
        NI_Clear(aszFileName);
        SDK_COMM_VENC_GetCurrentTime(&lpszTimeName);
        sprintf(aszFileName, "stream_chn%d_%s.%s", VencChn, lpszTimeName, szFilePostfix);

        if (enPayLoadType != PT_JPEG)
        {
            pFile = fopen(aszFileName, "wb");
            if (!pFile)
            {
                NI_ERROR("open file[%s] failed!\n", aszFileName);

                return NI_NULL;
            }
            
            NI_PRINT("open file[%s] ok!\n", aszFileName);
        }

        s_as32TestCommSnapCnt[VencChn] = 0;
    }
    
    while (NI_TRUE == pstPara->bThreadStart)
    {
        /* 这里可以进行送帧控制，在控制台敲mem命令将GetStmDebugPhyAddr写1才进行送1帧 */
        if (1 == pGetStmDebugVirAddr[0])
        {
            if (0 == pGetStmDebugVirAddr[1])
            {
                usleep(10000);
            
                continue;
            }
            else
            {
                pGetStmDebugVirAddr[1] = 0;
            }
        }
        
        memset(pstStream, 0, sizeof(VENC_STREAM_S));
            
        s32Ret = NI_MDK_VENC_GetStream(VencChn, pstStream, u32Timeout);
        if ( NI_SUCCESS != s32Ret )
        {            
            if ( NI_ERR_VENC_GET_STREAM_TIMEOUT == s32Ret )
            {
                continue;
            }
            pstPara->bThreadStart = NI_FALSE;

            break;
        }

        if (NI_TRUE == bUseVencSendFrm[VencChn] && NI_TRUE == bVencSendSyncWithGet[VencChn])
        {
            if (1 == pGetStmDebugVirAddr[2])
            {
                NI_INFO("GetStream %d Frame: u32StreamSeq=%d, u64PTS=%lld.!!\n", pstPara->u32GetFrmNum, 
                        pstStream->u32StreamSeq, pstStream->u64PTS);
            }
            
            if (pstStream->u64PTS > 100)
            {
                NI_ERROR("pstStream->u64PTS=%lld > 100.\n", pstStream->u64PTS);
            }
            else
            {
                g_stSendMemSesc[VencChn][pstStream->u64PTS].bActive = NI_TRUE;
            }
        }

        /* 码流校验 */
        if (NI_TRUE == bCheckBs && 1 == pstStream->stStreamPacket.u32PackNum)
        {
            if (VENC_STREAM_CHECK_SUM == enCheckType)
            {
                u32CheckValue = checksum((NI_U8 *)pstStream->stStreamPacket.aPackVirAddr[0], 
                    pstStream->stStreamPacket.au32PackLen[0]);
                if (u32CheckValue != pstStream->u32CheckValue)
                {
                    NI_ERROR("vechn:%d check value not match.u32CheckValue=%#x, stStream.u32CheckValue=%#x\n", VencChn, 
                        u32CheckValue, pstStream->u32CheckValue);
                }
            }
            else if (VENC_STREAM_CHECK_CRC == enCheckType)
            {
                u32CheckValue = crc32((NI_U8 *)pstStream->stStreamPacket.aPackVirAddr[0], 
                    pstStream->stStreamPacket.au32PackLen[0]);
                if (u32CheckValue != pstStream->u32CheckValue)
                {
                    NI_ERROR("vechn:%d check value not match.u32CheckValue=%#x, stStream.u32CheckValue=%#x\n", VencChn, 
                        u32CheckValue, pstStream->u32CheckValue);
                }
            }
        }
        
        /* for debug */
        if (1 == pGetStmDebugVirAddr[2])
        {
            // NI_PRINT("VeChn[%d],FrmNum[%d]: PackNum=%d,Eof=%d,StreamVirAddr[0x%x],StreamLen[%d],FrameType[%d],PTS[%lld],StreamSeq[%d].\n"
            //     , VencChn, pstPara->u32GetFrmNum, pstStream->stStreamPacket.u32PackNum
            //     , pstStream->stStreamPacket.bEndofFrame, pstStream->stStreamPacket.aPackVirAddr[0]
            //     , pstStream->stStreamPacket.au32PackLen[0], pstStream->enFrameType
            //     , pstStream->u64PTS, pstStream->u32StreamSeq);

            NI_PRINT("VeChn[%d],FrmNum[%d]: CheckValue[0x%x].\n"
                , VencChn, pstPara->u32GetFrmNum, pstStream->u32CheckValue);

            NI_PRINT("VeChn[%d],FrmNum[%d]: u32PicQP[%#x],u32BsSizeAve[%#x]\n"
                , VencChn, pstPara->u32GetFrmNum
                , pstStream->stAdditionalInfo.u32PicQP
                , pstStream->stAdditionalInfo.u32BsSizeAve
            );

            NI_PRINT("VeChn[%d],FrmNum[%d]: CuinfoValid[%d],IsH264[%d].\n"
                , VencChn, pstPara->u32GetFrmNum, pstStream->stAdditionalInfo.stCuOutDataInfo.bValid
                , pstStream->stAdditionalInfo.stCuOutDataInfo.bIsH264
                );
        }

        if (1 == pGetStmDebugVirAddr[3] && pGetStmDebugVirAddr[4] > 0 && NI_FALSE == bSpecCapEn)
        {
            pGetStmDebugVirAddr[3] = 0;
            bSpecCapEn = NI_TRUE;
            bSpecCapStart = NI_TRUE;
            u32SpecCapNum = pGetStmDebugVirAddr[4] * pstStream->u32FrameRate;
            
            NI_PRINT("VeChn[%d] Goto Specific capture FrmNum=%d.\n", VencChn, u32SpecCapNum);
        }

        if (NI_TRUE == bSaveFile || NI_TRUE == bSpecCapEn)
        {
            if ((u32LastPicWidth == 0) || (u32LastPicHeight == 0))
            {
                u32LastPicWidth = pstStream->u32PicWidth;
                u32LastPicHeight = pstStream->u32PicHeight;
            }
            
            if ((pstStream->u32PicWidth != u32LastPicWidth) 
                || (pstStream->u32PicHeight != u32LastPicHeight)
                || (u32FileTotalSize > NI_GB) 
                || NI_TRUE == bSpecCapStart)
            {
                if ((PT_H264 == enPayLoadType || PT_H265 == enPayLoadType) 
                    && VENC_FRAME_TYPE_I != pstStream->enFrameType)
                {
                    goto _Next;
                }
                
                if (pFile != NI_NULL)
                {
                    fclose(pFile);
                    pFile = NI_NULL;
                }

                NI_Clear(aszFileName);
                SDK_COMM_VENC_GetCurrentTime(&lpszTimeName);
                if (NI_TRUE == bSpecCapEn)
                {
                    sprintf(aszFileName, "stream_chn%d_%dx%d_%s_cap[%d].%s", VencChn, pstStream->u32PicWidth,
                        pstStream->u32PicHeight, lpszTimeName, u32SpecCapNum, szFilePostfix);
                }
                else
                {
                    sprintf(aszFileName, "stream_chn%d_%dx%d_%s.%s", VencChn, pstStream->u32PicWidth,
                        pstStream->u32PicHeight, lpszTimeName, szFilePostfix);
                }
                
                pFile = fopen(aszFileName, "wb");
                if (!pFile)
                {
                    NI_ERROR("open file[%s] failed!\n", aszFileName);

                    break;
                }

                NI_PRINT("open file[%s] ok!\n", aszFileName);
                
                u32LastPicWidth = pstStream->u32PicWidth;
                u32LastPicHeight = pstStream->u32PicHeight;
                bSpecCapStart = NI_FALSE;
                u32FileTotalSize = 0;
            }

        _Next:
            if ((PT_MJPEG == enPayLoadType) || (PT_H264 == enPayLoadType) || (PT_H265 == enPayLoadType))
            {
                s32Ret = SDK_COMM_VENC_SaveStream(VencChn, pFile, pstStream, &u32FileTotalSize);
                if (NI_SUCCESS != s32Ret)
                {
                    NI_ERROR("save stream failed!\n");

                    break;
                }

                if (NI_TRUE == pstStream->stAdditionalInfo.stCuOutDataInfo.bValid && NI_TRUE == bSaveCuInfo)
                {
                    if (NI_NULL == pH26xCuInfoFile)
                    {
                        sprintf(aszCuInfoFileName, "stream_chn%d_%s_cuinfo.txt", VencChn, szFilePostfix);

                        pH26xCuInfoFile = fopen(aszCuInfoFileName, "w");
                        if (!pH26xCuInfoFile)
                        {
                            NI_ERROR("open file[%s] failed!\n", aszCuInfoFileName);
                        }
                        else
                        {
                            NI_PRINT("open file[%s] ok!\n", aszCuInfoFileName);
                        }
                    }
                    
                    SDK_COMM_VENC_SaveCuInformation(VencChn, pstStream, pstPara->u32GetFrmNum, pH26xCuInfoFile);
                }

                if (1 == pGetStmDebugVirAddr[2])
                {
                    NI_INFO("VeChn(%d): SaveStream(%d) StreamVirAddr[0x%x], PhyAddr[0x%x], StreamLen[%d] Success.\n",
                        VencChn, pstPara->u32GetFrmNum, pstStream->stStreamPacket.au32PackVirAddr[0], 
                        pstStream->stStreamPacket.au32PackPhyAddr[0], pstStream->stStreamPacket.au32PackLen[0]);
                }

                /* 指定帧数抓完后关闭文件，走重新创建文件流程 */
                if(NI_TRUE == bSpecCapEn && NI_FALSE == bSpecCapStart)
                {
                    u32SpecCapNum--;
                    if (0 == u32SpecCapNum)
                    {
                        bSpecCapEn = NI_FALSE;
                        if (pFile != NI_NULL)
                        {
                            fclose(pFile);
                            pFile = NI_NULL;
                        }

                        u32FileTotalSize = (NI_U32)NI_GB * 2;
                    }
                }
            }
            else if (PT_JPEG == enPayLoadType)
            {
                s32Ret = SDK_COMM_VENC_SaveSnap(VencChn, pstStream);
            }
        }

        s32Ret = NI_MDK_VENC_ReleaseStream(VencChn, pstStream);
        if (NI_SUCCESS != s32Ret)
        {
            NI_ERROR("NI_MDK_VENC_ReleaseStream failed with %#x!\n", s32Ret);
            
            break;
        }
		
        pstPara->u32GetFrmNum++;
    }

_Exit:
    if (u32GetStmDebugPhyAddr != 0)
    {
        NI_MDK_SYS_MmzFree(u32GetStmDebugPhyAddr, pGetStmDebugVirAddr);
    }

    if (pFile != NI_NULL)
    {
        fclose(pFile);
        pFile = NI_NULL;
    }

    if (pH26xCuInfoFile != NI_NULL)
    {
        fclose(pH26xCuInfoFile);
        pH26xCuInfoFile = NI_NULL;
    }

    pstPara->bThreadStart = NI_FALSE;
    
    NI_INFO("exit %s ok!\n", __FUNCTION__);

    return NI_NULL;
}


static NI_VOID *SDK_COMM_VENC_StartSendFrameProc(NI_VOID *p)
{
    VENC_CHN VeChn;
    VENC_TEST_SENDFRM_PARA_S *pstPara;
    NI_U32 i;
    FILE *pfd = NI_NULL;
    NI_S32 s32Ret;
    VIDEO_FRAME_S stVideoFrame;
    NI_U32 u32PhyAddr;
    NI_VOID *pVirAddr = NULL;
    NI_U32 u32FrameSize;
    NI_U32 u32FileSize = 0;
    NI_U32 u32FrameNum = 0;
    NI_CHAR *pFileName;
    NI_U32 u32FrameCnt = 0;
    NI_U32 u32TimeOut;
    NI_U32 u32PicWidth, u32PicHeight;
    NI_BOOL     bRead2MemBefor;
    NI_U32      u32ReadStart;
    NI_U32      u32ReadInterval;
    NI_U32      u32ReadCnt;
    NI_S32      s32MemId = 0;
    NI_U32      u32VencDebugPhyAddr = 0;
    NI_U32      *pVencDebugVirAddr = NULL;
    NI_BOOL     bVencDebug = NI_FALSE;
    NI_U32  u32FrameBlkAlign = 4 * NI_KB; /* 每一帧yuv需要的对齐方式 */
    NI_U32  u32FrameBlkSize;  /* 每一帧yuv需要的申请内存大小 */
    NI_U32      u32PerDebugSendCnt = 1, u32DebugSendCnt = 0;

    pstPara = (VENC_TEST_SENDFRM_PARA_S*)p;
    VeChn = pstPara->VeChn;
    pFileName = pstPara->acFileName;   
    u32TimeOut = pstPara->u32TimeOut;
    bRead2MemBefor = pstPara->bRead2MemBefor;
    u32ReadStart = pstPara->u32ReadStart;
    u32ReadInterval = pstPara->u32ReadInterval;
    u32ReadCnt = pstPara->u32ReadCnt;
    bVencDebug = pstPara->bVencDebug;
        
    pfd = fopen(pFileName, "rb");
    if (!pfd)
    {
        NI_ERROR("open file[%s] failed!\n", pFileName);
        return NULL;
    }

    /*计算一帧的大小 yuv420   1.5 倍   yuv422  2倍*/
    u32FrameSize = (pstPara->u32Width) * (pstPara->u32Height) * 3 / 2;

    if (PIXEL_FORMAT_YUV_SEMIPLANAR_420_MB32x32 == pstPara->enEncType)
    {
        u32PicWidth = NI_Align(pstPara->u32Width, 32);
        u32PicHeight = NI_Align(pstPara->u32Height, 32);
    
        u32FrameSize = NI_Align(u32PicWidth * u32PicHeight * 3 / 2, 2048);
    }
    else if(PIXEL_FORMAT_YUV_SEMIPLANAR_420_MB16x16 == pstPara->enEncType)
    {
        u32PicWidth = NI_Align(pstPara->u32Width, 16);
        u32PicHeight = NI_Align(pstPara->u32Height, 16);
    
        u32FrameSize = (u32PicWidth / 16) * (u32PicHeight / 16);
       
        /* 2K内存可以包含5个宏块，要往大里取整 */
        u32FrameSize = (u32FrameSize + 4) / 5;
    
        u32FrameSize = u32FrameSize << 11; /* 每5个宏块占2048个Byte */
    }  
    else if ((PIXEL_FORMAT_UYVY_PACKAGE_422 == pstPara->enEncType) 
            || (PIXEL_FORMAT_YUYV_PACKAGE_422 == pstPara->enEncType))
    {
        u32FrameSize = pstPara->u32Width * pstPara->u32Height * 2;
    }
    else if (PIXEL_FORMAT_YUV_PLANAR_444 == pstPara->enEncType)
    {
        u32FrameSize = pstPara->u32Width * pstPara->u32Height * 3;
    }
    else if (PIXEL_FORMAT_SINGLE == pstPara->enEncType)
    {
        u32FrameSize = pstPara->u32Width * pstPara->u32Height;
    }
    else if ((PIXEL_FORMAT_YUV_SEMIPLANAR_420 == pstPara->enEncType) || (PIXEL_FORMAT_YUV_PLANAR_420 == pstPara->enEncType))
    {
        u32FrameSize = pstPara->u32Width * pstPara->u32Height * 3 / 2;
    }

    fseek(pfd, 0, SEEK_END);     
    u32FileSize = ftell(pfd);
    u32FrameNum = u32FileSize/(u32FrameSize);
    u32FrameBlkSize = NI_Align(u32FrameSize, u32FrameBlkAlign);

    NI_INFO("u32FrameNum is %d, FrameSize=%d, FrameBlkSize=%d.\n", 
        u32FrameNum, u32FrameSize, u32FrameBlkSize);
    
    rewind(pfd);

    for ( i = 0 ; i < 100; i++ )
    {
        g_stSendMemSesc[VeChn][i].bUsed = NI_FALSE;
        g_stSendMemSesc[VeChn][i].bActive = NI_FALSE;
        g_stSendMemSesc[VeChn][i].u32Id = i;
        g_stSendMemSesc[VeChn][i].u32PhyAddr = 0;
        g_stSendMemSesc[VeChn][i].pVirAddr = NI_NULL;
    }

    s32Ret = NI_MDK_SYS_MmzAlloc(&u32VencDebugPhyAddr, (NI_VOID **)&pVencDebugVirAddr, 4096, "venc_debug", NULL);
    if(s32Ret != NI_SUCCESS)
    {
        NI_ERROR("NI_MDK_SYS_MmzAlloc  malloc 4096 failed!\n");
        fclose(pfd);
        return NI_NULL;
    }
    
    NI_INFO("VencDebugPhyAddr=%#x, VencDebugVirAddr=%#x, ofset 0 is debug mode switch, ofset 1 is send frm enable, offset 2 is print switch.\n", 
        u32VencDebugPhyAddr, (NI_U32)pVencDebugVirAddr);
    NI_ClearSize((NI_VOID *)pVencDebugVirAddr, 4096);

    if (NI_TRUE == bVencDebug)
    {
        pVencDebugVirAddr[0] = 1;
    }

    if (NI_TRUE == bRead2MemBefor)
    {
        if (u32ReadCnt > 100 || (u32ReadStart + (u32ReadCnt - 1) * (u32ReadInterval + 1)) > (u32FrameNum - 1))
        {
            NI_ERROR("is Read2MemBefor but Read Frm Num Error!\n");
            NI_ERROR("VeChn %d: ReadStart=%d, ReadCnt=%d, ReadInterval=%d.\n", 
                VeChn, u32ReadStart, u32ReadCnt, u32ReadInterval);
            fclose(pfd);
            NI_MDK_SYS_MmzFree(u32VencDebugPhyAddr, pVencDebugVirAddr);
            
            return NULL;
        }

        fseek(pfd, u32ReadStart * u32FrameSize, SEEK_SET);

        s32Ret = NI_MDK_SYS_MmzAlloc(&u32PhyAddr, &pVirAddr, u32ReadCnt * u32FrameBlkSize, "venc_yuv_send", NULL);
        if(s32Ret != NI_SUCCESS)
        {
            NI_ERROR("NI_MDK_SYS_MmzAlloc malloc 4096 failed!\n");

            return NI_NULL;
        }

        for ( i = 0 ; i < u32ReadCnt; i++ )
        {
            fread(((NI_U8 *)pVirAddr + i * u32FrameBlkSize), u32FrameSize, 1, pfd);
            fseek(pfd, u32ReadInterval * u32FrameSize, SEEK_CUR);

            g_stSendMemSesc[VeChn][i].bUsed = NI_TRUE;
            g_stSendMemSesc[VeChn][i].bActive = NI_TRUE;
            g_stSendMemSesc[VeChn][i].u32PhyAddr = u32PhyAddr + i * u32FrameBlkSize;
            g_stSendMemSesc[VeChn][i].pVirAddr = (NI_U8 *)pVirAddr + i * u32FrameBlkSize;
        }
    }
    else
    {
        s32Ret = NI_MDK_SYS_MmzAlloc(&u32PhyAddr, &pVirAddr, 2 * u32FrameBlkSize, "venc_yuv_send", NULL);
        if(s32Ret != NI_SUCCESS)
        {
            NI_ERROR("NI_MDK_SYS_MmzAlloc malloc 4096 failed!\n");

            return NI_NULL;
        }
        u32ReadCnt = 2;

        for ( i = 0 ; i < u32ReadCnt; i++ )
        {
            g_stSendMemSesc[VeChn][i].bUsed = NI_TRUE;
            g_stSendMemSesc[VeChn][i].bActive = NI_TRUE;
            g_stSendMemSesc[VeChn][i].u32PhyAddr = u32PhyAddr + i * u32FrameBlkSize;
            g_stSendMemSesc[VeChn][i].pVirAddr = (NI_U8 *)pVirAddr + i * u32FrameBlkSize;
        }
    }
    
    memset(&stVideoFrame, 0, sizeof(VIDEO_FRAME_S));
    stVideoFrame.u32Width = pstPara->u32Width;
    stVideoFrame.u32Height = pstPara->u32Height;
    stVideoFrame.enPixelFormat = pstPara->enEncType;

    NI_INFO("pstPara->bThreadStart = %d\n", pstPara->bThreadStart);

    u32FrameCnt = 0;
    s32MemId = 0;
    while (NI_TRUE == pstPara->bThreadStart)
    {
        /* 这里可以进行送帧控制，在控制台敲mem命令将u32VencDebugPhyAddr写1才进行送1帧 */
        if (1 == pVencDebugVirAddr[0])
        {
            if (0 == pVencDebugVirAddr[1])
            {
                usleep(10000);
            
                continue;
            }
            else
            {
                u32DebugSendCnt++;
                if (u32DebugSendCnt == u32PerDebugSendCnt)
                {
                    pVencDebugVirAddr[1] = 0;
                    u32DebugSendCnt = 0;
                }
            }
        }
        
        if(NI_TRUE == bVencSendSyncWithGet[VeChn] 
            && NI_FALSE == g_stSendMemSesc[VeChn][s32MemId].bActive)
        {
            usleep(10000);
            continue;
        }

        if (1 == pVencDebugVirAddr[2])
        {
            NI_INFO("VeChn %d: FrameCnt=%d, s32MemId=%d.\n", 
                VeChn, u32FrameCnt, s32MemId);
        }
        
        g_stSendMemSesc[VeChn][s32MemId].bActive = NI_FALSE;

        if (NI_FALSE == bRead2MemBefor)
        {
            /* 从文件读取一帧数据, 送去编码*/
            fread(((NI_U8 *)g_stSendMemSesc[VeChn][s32MemId].pVirAddr), u32FrameSize, 1, pfd);
        }
      
        stVideoFrame.aphysAddr[0] = g_stSendMemSesc[VeChn][s32MemId].u32PhyAddr;
    	stVideoFrame.apVirAddr[0] = g_stSendMemSesc[VeChn][s32MemId].pVirAddr;
        stVideoFrame.u32TimeRef = u32FrameCnt;
        stVideoFrame.u64Pts = s32MemId;
        
        if (NI_MDK_VENC_SendFrm(VeChn, &stVideoFrame, u32TimeOut) != NI_SUCCESS)
        {
            g_stSendMemSesc[VeChn][s32MemId].bActive = NI_TRUE;
            
            continue;
        }

        if (1 == pVencDebugVirAddr[2])
        {
            NI_INFO("Send %d Frame Success, BlkId[%d], PhyAddr[%#x], VirAddr[%#x].\n",
                u32FrameCnt, s32MemId, g_stSendMemSesc[VeChn][s32MemId].u32PhyAddr, (NI_U32)g_stSendMemSesc[VeChn][s32MemId].pVirAddr);
        }

        u32FrameCnt++;
        s32MemId++;

        /* 轮了一圈内存数据，从头开始 */
        if (s32MemId == u32ReadCnt)
        {
            s32MemId = 0;
        }
        
        if (NI_FALSE == bRead2MemBefor && (u32FrameCnt % u32FrameNum == 0))
        {
            /* 读到文件尾部后从头开始读*/
            rewind(pfd);
        }
    }

    if (u32VencDebugPhyAddr != 0)
    {
        NI_MDK_SYS_MmzFree(u32VencDebugPhyAddr, pVencDebugVirAddr);
    }
    
    NI_MDK_SYS_MmzFree(u32PhyAddr, pVirAddr);    
    fclose(pfd);
    pstPara->bThreadStart = NI_FALSE;

    NI_INFO("SendFrm %d frame ok.\n", u32FrameCnt);
   
    return NULL;
}


/* 开启编码模块内部送yuv线程 */
NI_S32 SDK_COMM_VENC_StartSendFrame(VENC_CHN VeChn, NI_U32 u32Width, NI_U32 u32Height, NI_CHAR *pFileName, PIXEL_FORMAT_E  enEncType, NI_BOOL bVencDebug)
{
    if (NI_TRUE == s_astVencSendFrmPara[VeChn].bThreadStart)
    {
        NI_WARN("chn:%d, send stream thread is already start.\n", VeChn);
        return NI_SUCCESS;
    }
    s_astVencSendFrmPara[VeChn].VeChn = VeChn;
    s_astVencSendFrmPara[VeChn].u32Width= u32Width;
    s_astVencSendFrmPara[VeChn].u32Height = u32Height;
    s_astVencSendFrmPara[VeChn].bThreadStart = NI_TRUE;
    s_astVencSendFrmPara[VeChn].u32TimeOut = 10000;/*休眠时间可根据实际情况自己设置 ms*/
    s_astVencSendFrmPara[VeChn].enEncType = enEncType;
    s_astVencSendFrmPara[VeChn].bRead2MemBefor = NI_TRUE;
    s_astVencSendFrmPara[VeChn].u32ReadCnt = 10;
    s_astVencSendFrmPara[VeChn].u32ReadStart = 0;
    s_astVencSendFrmPara[VeChn].u32ReadInterval = 5;
    s_astVencSendFrmPara[VeChn].bVencDebug = bVencDebug;

    strcpy(s_astVencSendFrmPara[VeChn].acFileName, pFileName);

    NI_INFO("acFileName = %s\n", s_astVencSendFrmPara[VeChn].acFileName);

    bUseVencSendFrm[VeChn] = NI_TRUE;

    return pthread_create(&s_astVencSendFrmmPid[VeChn], 0,\
                           SDK_COMM_VENC_StartSendFrameProc, (NI_VOID*)&s_astVencSendFrmPara[VeChn]);
}


/* 关闭编码模块内部送yuv线程 */
NI_S32 SDK_COMM_VENC_StopSendFrame(VENC_CHN VeChn)
{
    if (NI_TRUE == s_astVencSendFrmPara[VeChn].bThreadStart)
    {    
        s_astVencSendFrmPara[VeChn].bThreadStart = NI_FALSE;
        pthread_join(s_astVencSendFrmmPid[VeChn], 0);
    }

    bUseVencSendFrm[VeChn] = NI_FALSE;
    
    return NI_SUCCESS;
}


/******************************************************************************
* 每个grp-chn开启一个独立的线程获取码流
******************************************************************************/
NI_S32 SDK_COMM_VENC_StartGetStream(VENC_CHN VeChn, NI_BOOL bSaveFile)
{  
    if(NI_TRUE == s_astTestCommVencPara[VeChn].bThreadStart)
    {
        NI_WARN("VeChn:%d, get stream thread is already start.\n", VeChn);
        return NI_SUCCESS;
    }
    
    s_astTestCommVencPara[VeChn].bThreadStart = NI_TRUE;
    s_astTestCommVencPara[VeChn].VeChn = VeChn; 
    s_astTestCommVencPara[VeChn].bSaveFile = bSaveFile;
    s_astTestCommVencPara[VeChn].u32Timeout = 5000;

    return pthread_create(&s_astTestCommVencGetStreamPid[VeChn], 0, \
        SDK_COMM_VENC_GetVencStreamProc, (NI_VOID*)&s_astTestCommVencPara[VeChn]);   
}


NI_S32 SDK_COMM_VENC_StopGetStream(VENC_CHN VeChn)
{
    if (NI_TRUE == s_astTestCommVencPara[VeChn].bThreadStart)
    {
        s_astTestCommVencPara[VeChn].bThreadStart = NI_FALSE;
        pthread_join(s_astTestCommVencGetStreamPid[VeChn], 0);
    }
    
    return NI_SUCCESS;
}


NI_S32 SDK_COMM_VENC_Start(VENC_CHN VeChn, TEST_VENC_CONFIG_S *pstVencCfg)
{
    NI_S32 s32Ret = NI_SUCCESS;
    VENC_CHN_ATTR_S stChnAttr;

    VENC_STREAM_CHECK_TYPE_E enCheckType = VENC_STREAM_CHECK_OFF;

    memset(&stChnAttr, 0, sizeof(VENC_CHN_ATTR_S));

    //common
    stChnAttr.stVeAttr.enType = pstVencCfg->enPayLoad;
    stChnAttr.stVeAttr.u32StrmBufSize = pstVencCfg->u32InWidth * pstVencCfg->u32InHeight / 2 + 40 * 4096;


    stChnAttr.stVeAttr.stInputPicAttr.u32MaxPicWidth = pstVencCfg->u32InWidth;
    stChnAttr.stVeAttr.stInputPicAttr.u32MaxPicHeight = pstVencCfg->u32InHeight;
    stChnAttr.stVeAttr.stInputPicAttr.u32PicWidth = pstVencCfg->u32InWidth;
    stChnAttr.stVeAttr.stInputPicAttr.u32PicHeight = pstVencCfg->u32InHeight;
    stChnAttr.stVeAttr.stInputPicAttr.enPixelFormat = pstVencCfg->enEncType;
    
    if ((PIXEL_FORMAT_YUV_SEMIPLANAR_420 == pstVencCfg->enEncType) 
        || (PIXEL_FORMAT_YVU_SEMIPLANAR_420 == pstVencCfg->enEncType))
    {
        stChnAttr.stVeAttr.stInputPicAttr.au32Stride[0] = pstVencCfg->u32InWidth;
        stChnAttr.stVeAttr.stInputPicAttr.au32Stride[1] = pstVencCfg->u32InWidth;
        stChnAttr.stVeAttr.stInputPicAttr.au32Stride[2] = 0;
    }
    else if (PIXEL_FORMAT_YUV_PLANAR_420 == pstVencCfg->enEncType)
    {
        stChnAttr.stVeAttr.stInputPicAttr.au32Stride[0] = pstVencCfg->u32InWidth;
        stChnAttr.stVeAttr.stInputPicAttr.au32Stride[1] = pstVencCfg->u32InWidth / 2;
        stChnAttr.stVeAttr.stInputPicAttr.au32Stride[2] = pstVencCfg->u32InWidth / 2;
    }
    else if ((PIXEL_FORMAT_UYVY_PACKAGE_422 == pstVencCfg->enEncType) 
        || (PIXEL_FORMAT_YUYV_PACKAGE_422 == pstVencCfg->enEncType))
    {
        stChnAttr.stVeAttr.stInputPicAttr.au32Stride[0] = pstVencCfg->u32InWidth;
        stChnAttr.stVeAttr.stInputPicAttr.au32Stride[1] = 0;
        stChnAttr.stVeAttr.stInputPicAttr.au32Stride[2] = 0;
    }
    else
    {
        NI_ERROR("VeChn=%d, enEncType[%d] is out of range\n", VeChn, pstVencCfg->enEncType);
        return NI_FAILURE;
    }

    stChnAttr.stRcAttr.enRcMode        = pstVencCfg->enRcMode;
    stChnAttr.stRcAttr.u32FrmRateNum   = pstVencCfg->u32InFps;
    stChnAttr.stRcAttr.u32FrmRateDenom = 1;
    if (VENC_RC_MODE_CBR == stChnAttr.stRcAttr.enRcMode)
	{
        stChnAttr.stRcAttr.stAttrCbr.u32StatTime = 1;
        stChnAttr.stRcAttr.stAttrCbr.u32MaxIQp   = 51;
        stChnAttr.stRcAttr.stAttrCbr.u32MinIQp   = 10;
        stChnAttr.stRcAttr.stAttrCbr.u32MaxPQp   = 51;
        stChnAttr.stRcAttr.stAttrCbr.u32MinPQp   = 10;

        if (pstVencCfg->u32InWidth <= 1280)
		{
            stChnAttr.stRcAttr.stAttrCbr.u32TargetBitRate = 512;
        }
		else if (pstVencCfg->u32InWidth <= 1920)
		{
            stChnAttr.stRcAttr.stAttrCbr.u32TargetBitRate = 2048;
        }
		else
		{
            stChnAttr.stRcAttr.stAttrCbr.u32TargetBitRate = 4096;
        }
    } 
	else if (VENC_RC_MODE_VBR == stChnAttr.stRcAttr.enRcMode)
	{
        stChnAttr.stRcAttr.stAttrVbr.u32StatTime   = 1;
        stChnAttr.stRcAttr.stAttrVbr.u32MaxBitRate = 2048;
        stChnAttr.stRcAttr.stAttrVbr.s32ChangePos  = 85;
        stChnAttr.stRcAttr.stAttrVbr.u32MaxIQp     = 51;
        stChnAttr.stRcAttr.stAttrVbr.u32MinIQp     = 10;
        stChnAttr.stRcAttr.stAttrVbr.u32MaxPQp     = 51;
        stChnAttr.stRcAttr.stAttrVbr.u32MinPQp     = 10;
    }
	else if (VENC_RC_MODE_CVBR == stChnAttr.stRcAttr.enRcMode)
	{
        stChnAttr.stRcAttr.stAttrCVbr.u32StatTime   = 1;
        stChnAttr.stRcAttr.stAttrCVbr.u32MaxBitRate = 2048;
        stChnAttr.stRcAttr.stAttrCVbr.u32MaxIQp     = 51;
        stChnAttr.stRcAttr.stAttrCVbr.u32MinIQp     = 10;
        stChnAttr.stRcAttr.stAttrCVbr.u32MaxPQp     = 51;
        stChnAttr.stRcAttr.stAttrCVbr.u32MinPQp     = 10;
    }
	else if (VENC_RC_MODE_AVBR == stChnAttr.stRcAttr.enRcMode)
	{
        NI_ERROR("VeChn=%d, VENC_RC_MODE_AVBR not support\n", VeChn);
        return NI_FAILURE;
    }
	else if (VENC_RC_MODE_QVBR == stChnAttr.stRcAttr.enRcMode)
	{
        stChnAttr.stRcAttr.stAttrQVbr.u32StatTime      = 1;
        stChnAttr.stRcAttr.stAttrQVbr.u32MaxBitRate    = 2048;
        stChnAttr.stRcAttr.stAttrQVbr.s32ChangePos     = 85;
        stChnAttr.stRcAttr.stAttrQVbr.u32MaxIQp        = 51;
        stChnAttr.stRcAttr.stAttrQVbr.u32MinIQp        = 10;
        stChnAttr.stRcAttr.stAttrQVbr.u32MaxPQp        = 51;
        stChnAttr.stRcAttr.stAttrQVbr.u32MinPQp        = 10;
        stChnAttr.stRcAttr.stAttrQVbr.s32MinBitratePos = 0;
        stChnAttr.stRcAttr.stAttrQVbr.s32BestQuality   = 8;
        stChnAttr.stRcAttr.stAttrQVbr.s32WorstQuality  = 20;
    }
	else
	{
        NI_ERROR("VeChn=%d, RC Mode not support\n", VeChn);
        return NI_FAILURE;
    }

    stChnAttr.stGopAttr.enGopMode             = VENC_GOPMODE_NORMAL;
    stChnAttr.stGopAttr.stGopNomalAttr.u32Gop = pstVencCfg->u32InFps * 2;
    //specific
    if (PT_H265 == pstVencCfg->enPayLoad)
    {
       stChnAttr.stVeAttr.enProfile = VENC_H265_MAIN_PROFILE;
       stChnAttr.stRcAttr.stAttrCbr.u32TargetBitRate /= 2;
    }
    else if (PT_H264 == pstVencCfg->enPayLoad)
    {
        stChnAttr.stVeAttr.enProfile = VENC_H264_MAIN_PROFILE;
    }
    else if (PT_JPEG == pstVencCfg->enPayLoad)
    {
        stChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_FIXQP;
        stChnAttr.stRcAttr.stAttrFixQp.u32FixedIQp = 70;
    }
    else if (PT_MJPEG == pstVencCfg->enPayLoad)
	{
        stChnAttr.stRcAttr.stAttrCbr.u32TargetBitRate = 51200;
    }

    
    s32Ret = NI_MDK_VENC_SetStreamCheck(VeChn, enCheckType);
    if (s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("VeChn=%d, call NI_MDK_VENC_SetStreamCheck fail, s32Ret=%#x\n", VeChn, s32Ret);
        return s32Ret;
    }

    s32Ret = NI_MDK_VENC_CreateChn(VeChn, &stChnAttr);
    if (s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("VeChn=%d, call NI_MDK_VENC_CreateChn fail, s32Ret=%#x\n", VeChn, s32Ret);
        return s32Ret;
    }
    
    if(PT_JPEG != pstVencCfg->enPayLoad)
    {
        s32Ret = NI_MDK_VENC_StartRecvPic(VeChn);
        if (s32Ret != NI_SUCCESS)
        {
            PRINT_ERROR("VeChn=%d, call NI_MDK_VENC_StartRecvPic fail, s32Ret=%#x\n", VeChn, s32Ret);
            return s32Ret;
        }
    }

    if (pstVencCfg->u32OutFps != pstVencCfg->u32InFps)
    {
        s32Ret = NI_MDK_VENC_SetOutFrmRate(VeChn, pstVencCfg->u32OutFps);
        if (s32Ret != NI_SUCCESS)
        {
            PRINT_ERROR("VeChn=%d, call NI_MDK_VENC_SetOutFrmRate fail, s32Ret=%#x\n", VeChn, s32Ret);
            return s32Ret;
        }
    }

    if (VENC_STREAM_CHECK_CRC == enCheckType)
    {
        crc32_init();
    }

    return NI_SUCCESS;
}


NI_S32 SDK_COMM_VENC_Stop(VENC_CHN VeChn)
{
    NI_MDK_VENC_StopRecvPic(VeChn);
    NI_MDK_VENC_DestroyChn(VeChn);
    
    return NI_SUCCESS;
}

