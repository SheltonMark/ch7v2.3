/* 
 * sdk_comm_vps.c -- vps sample用例 
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */

#include <limits.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>


#include "sdk_comm.h"
#include "mdk_vps.h"
#include "ni_macro.h"
#include "ni_define.h"

typedef struct tagVpsTestSendFrm
{
    VPS_GRP     VpsGrp;
    NI_BOOL     bThreadStart;
    NI_BOOL     bSendOnece;
    PIXEL_FORMAT_E  enPixelFormat; /* 发送图像的格式 */
    NI_U32      u32Width;
    NI_U32      u32Height;
    NI_U32      u32TimeOut;
    NI_CHAR     acFileName[128];
    NI_U32      u32SendFrmCnt; /* 已经发送的图像帧数*/
    NI_U32      u32InPutFps;
    
    NI_U32      u32ReadStart;
    NI_U32      u32ReadInterval;
    NI_U32      u32ReadCnt;
    NI_BOOL     bRead2MemBefor;

    NI_BOOL     bStepDebug;
}VPS_TEST_SENDFRM_PARA_S;

typedef struct tagVpsSendMemDesc
{
    NI_BOOL             bUsed;
    NI_BOOL             bActive;
    NI_U32              u32PhyAddr;
    NI_VOID             *pVirAddr;
    NI_U32              u32Id;
}VPS_SEND_MEM_DESC_S;

typedef struct tagVpsTestGetFramePara
{
     NI_BOOL    bThreadStart;
     VPS_GRP    VpsGrp;
     VPS_CHN_E  VpsChn;
     NI_U32     u32TimeOut;
     NI_U32     u32GetFrmNum;
     NI_BOOL    bSaveFile;
}VPS_TEST_GET_FRAME_PARA_S;

static pthread_t gastVpsSendFrmPid[VPS_MAX_GRP_NUM];
static VPS_TEST_SENDFRM_PARA_S gastVpsSendFrmPara[VPS_MAX_GRP_NUM];
static VPS_SEND_MEM_DESC_S g_stVpsSendMemSesc[VPS_MAX_GRP_NUM][100];

static pthread_t s_astVpsGetFrmPid[VPS_MAX_GRP_NUM][VPS_GRP_MAX_CHN_NUM];
static VPS_TEST_GET_FRAME_PARA_S s_astVpsGetFrmPara[VPS_MAX_GRP_NUM][VPS_GRP_MAX_CHN_NUM];

static NI_CHAR gaVpsTime[25];
static NI_S32 as32EncDestChnId[VPS_MAX_GRP_NUM][VPS_GRP_MAX_CHN_NUM] = {0};

////////////////////////////////////////////////////////////////////////////////////////////////////////
static NI_U32 SDK_COMM_VPS_GetTimeMs(NI_VOID)
{
    struct timeval cur_time;

    gettimeofday(&cur_time, NULL);

    return (cur_time.tv_sec * 1000 + cur_time.tv_usec / 1000);
}


static NI_S32 SDK_COMM_VPS_GetCurrentTime(NI_CHAR **pszTmpTimeName)
{
    time_t sttime;
    struct tm *ptime;
    NI_U32 i;

    NI_CHAR *pszTimeName;
    NI_CHAR *pszTempTimeName;

    /* 获取当前时间 */
    time(&sttime);
    ptime = gmtime(&sttime);

    pszTimeName = gaVpsTime;
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

#if 0
/* Description: yuv420p convert to BigMacroBlock(16x16) */
static void SDK_COMM_VPS_YUV420pToMb(NI_U32 u32OriWidth, NI_U32 u32OriHeight, NI_CHAR *pcLineBuf, NI_CHAR *pcMbBuf)
{
    NI_CHAR *y, *u, *v;
    NI_U32 i, j, k, row, row_16 = ((u32OriHeight + 16 - 1) & (~(16 - 1))) / 16;
    NI_U32 mbPerRow = ((u32OriWidth + 16 - 1) & (~(16 - 1))) / 16;
    NI_U32 mbTotal = 0;
    NI_CHAR *startAddr, *tileAddr, *cbTileAddr, *crTileAddr, *srcAddr;
	NI_CHAR *cbStartAddr;
	NI_CHAR *crStartAddr;
    NI_CHAR *pcTmpMbBuf = pcMbBuf;
    
    y = pcLineBuf;
	u = y + u32OriWidth * u32OriHeight;
	v = u + u32OriWidth * u32OriHeight / 4;
    
    for (i = 0; i < row_16; i++)
    {
        startAddr = y + i * 16 * ((u32OriWidth + 16 - 1) & (~(16 - 1)));
        cbStartAddr = u + i * 8 * ((u32OriWidth + 16 - 1) & (~(16 - 1)))/2;
        crStartAddr = v + i * 8 * ((u32OriWidth + 16 - 1) & (~(16 - 1)))/2;
        
        for(j = 0; j < mbPerRow; j++)
        {
            tileAddr = startAddr + 16 * j;
            cbTileAddr = cbStartAddr + 8 * j;
            crTileAddr = crStartAddr + 8 * j;
            
            for(k = 0; k < 4; k++)
            {
                for (row = 0; row < 4; row++)
                {
                    //luma
                    if ((i * 16 + k * 4 + row) < u32OriHeight) 
                    {
                        srcAddr = tileAddr + (k * 4 + row) * ((u32OriWidth + 16 - 1) & (~(16 - 1)));
                        memcpy(pcTmpMbBuf, srcAddr, 16);
                        pcTmpMbBuf += 16;
                    }
                    else
                    {
                        memset(pcMbBuf,0,16);
                        pcTmpMbBuf += 16;
                    }
                }

                //cb
                for (row = 0; row < 2; row++)
                {
                    if ((i * 8 + k * 2 + row) < u32OriHeight / 2) 
                    {
                        srcAddr = cbTileAddr + (k * 2 + row) * ((u32OriWidth + 16 - 1) & (~(16 - 1)))/2;
                        memcpy(pcTmpMbBuf,srcAddr,8);
                        pcTmpMbBuf += 8;
                    }
                    else
                    {
                        memset(pcTmpMbBuf,0,8);
                        pcTmpMbBuf += 8;
                    }
                }  

                //cr
                for (row = 0; row < 2; row++)
                {
                    if ((i * 8 + k * 2 + row) < u32OriHeight / 2) 
                    {
                        srcAddr = crTileAddr + (k * 2 + row) * ((u32OriWidth + 16 - 1) & (~(16 - 1)))/2;
                        memcpy(pcTmpMbBuf, srcAddr, 8);
                        pcTmpMbBuf += 8;
                    }
                    else
                    {
                        memset(pcTmpMbBuf, 0, 8);
                        pcTmpMbBuf += 8;
                    }
                }  
            }
            
            memset(pcTmpMbBuf, 0, 16);
            pcTmpMbBuf += 16;
            mbTotal++;
            if (mbTotal % 5 == 0)
            {
                memset(pcTmpMbBuf, 0, 48);
                pcTmpMbBuf += 48;
            }
        }
    }

	return;
}
#endif

/* Description: yuv420sp convert to BigMacroBlock(16x16) */
static void SDK_COMM_VPS_YUV420spToMb(NI_U32 u32OriWidth, NI_U32 u32OriHeight, NI_CHAR *pcLineBuf, NI_CHAR *pcMbBuf)
{
    NI_CHAR *y, *uv;
    NI_U32 i, j, k, m, row, row_16 = ((u32OriHeight + 16 - 1) & (~(16 - 1))) / 16;
    NI_U32 mbPerRow = ((u32OriWidth + 16 - 1) & (~(16 - 1))) / 16;
    NI_U32 mbTotal = 0;
    NI_CHAR *startAddr, *tileAddr, *cbcrTileAddr, *srcAddr;
	NI_CHAR *cbcrStartAddr;
    NI_CHAR *pcTmpMbBuf = pcMbBuf;

    y = pcLineBuf;
	uv = y + u32OriWidth * u32OriHeight;

    //纵向宏块计数
    for (i = 0; i < row_16; i++)
    {
        startAddr = y + i * 16 * ((u32OriWidth + 16 - 1) & (~(16 - 1)));
        cbcrStartAddr = uv + i * 8 * ((u32OriWidth + 16 - 1) & (~(16 - 1)));

        //横向宏块计数
        for (j = 0; j < mbPerRow; j++)
        {
            tileAddr = startAddr + 16 * j;
            cbcrTileAddr = cbcrStartAddr + 16 * j;
            
            for (k = 0; k < 4; k++)  //纵向4份
            {
                for (row = 0; row < 4; row++) //每份4行，每行有16个像素
                {
                    //luma
                    if ((i * 16 + k * 4 + row) < u32OriHeight)  
                    {
                        srcAddr = tileAddr + (k * 4 + row) * ((u32OriWidth + 16 - 1) & (~(16 - 1)));
                        memcpy(pcTmpMbBuf, srcAddr, 16);
                        pcTmpMbBuf += 16;
                    }
                    else
                    {
                        memset(pcTmpMbBuf,0,16);
                        pcTmpMbBuf += 16;
                    }
                }

                //cbcr
                for (row = 0; row < 2; row++) //每份2行，每行有16个像素
                {
                    if ((i * 8 + k * 2 + row) < u32OriHeight / 2) 
                    {
                        srcAddr = cbcrTileAddr + (k * 2 + row) * ((u32OriWidth + 16 - 1) & (~(16 - 1)));                        

						for (m = 0 ; m < 8; m++)
                        {
                            //cb
                            pcTmpMbBuf[m] = srcAddr[2 * m];
							//cr
							pcTmpMbBuf[16+m] = srcAddr[2 * m + 1];
                        }

						if (0 == row)
						{
							pcTmpMbBuf += 8;
						}
						else
						{
							pcTmpMbBuf += 24;

						}
                    }
                    else
                    {
                        memset(pcTmpMbBuf,0,16);
                        pcTmpMbBuf += 16;
                    }
                }  
            }
            
            memset(pcTmpMbBuf, 0, 16);
            pcTmpMbBuf += 16;
            mbTotal++;
            if (mbTotal % 5 == 0)
            {
                memset(pcTmpMbBuf, 0, 48);
                pcTmpMbBuf += 48;
            }
        }
    }

	return;
}


static NI_S32 SDK_COMM_VPS_GetFilePostfix(PIXEL_FORMAT_E enPixelFormat, char *szFilePostfix)
{    
    switch(enPixelFormat)
    {     
        case PIXEL_FORMAT_RGB_888:
        {
            strcpy(szFilePostfix, ".rgb888");

            break;
        }
        case PIXEL_FORMAT_RGB_PLANAR_888:
        {

            strcpy(szFilePostfix, ".rgb888p");

            break;
        }
        case PIXEL_FORMAT_YUV_SEMIPLANAR_420:
        {
            strcpy(szFilePostfix, ".nv12");

            break;
        }
        case PIXEL_FORMAT_YVU_SEMIPLANAR_420:
        {

            strcpy(szFilePostfix, ".nv21");

            break;
        }
        case PIXEL_FORMAT_UYVY_PACKAGE_422:
        {
            strcpy(szFilePostfix, ".uyvy");

            break;
        }
        case PIXEL_FORMAT_YUYV_PACKAGE_422:
        {

            strcpy(szFilePostfix, ".yuyv");

            break;
        }
        case PIXEL_FORMAT_YUV_PLANAR_420:
        {
            strcpy(szFilePostfix, ".yuv420p");

            break;
        }
        case PIXEL_FORMAT_YUV_PLANAR_444:
        {

            strcpy(szFilePostfix, ".yuv444");

            break;
        }
        case PIXEL_FORMAT_SINGLE:
        {

            strcpy(szFilePostfix, ".yuv400");

            break;
        }
        default:
        {
            NI_ERROR("PixelFormat type(%d) err!\n", enPixelFormat);

            return NI_FAILURE;
        }
    }

    return NI_SUCCESS;
}


static NI_S32 SDK_COMM_VPS_WriteFile(FILE *pFile, NI_VOID *pSaveVirAddr,
    NI_U32 u32PicWidth, NI_U32 u32PicHeight, NI_U32 u32Stride, NI_BOOL bSaveStride, NI_U32 *pu32OffsetSize)
{
    NI_U32 i = 0;
    
    if (u32Stride <= u32PicWidth)
    {
        if (u32Stride != 0 && u32Stride != u32PicWidth)
        {
            NI_ERROR("Stride=%d < %d.\n", u32Stride, u32PicWidth);
        }
                
        fwrite(pSaveVirAddr, u32PicWidth * u32PicHeight, 1, pFile);

        *pu32OffsetSize = u32PicWidth * u32PicHeight;
    }
    else
    {
        if (NI_FALSE == bSaveStride)
        {
            NI_CHAR *pBuf = (NI_CHAR *)pSaveVirAddr;
                
            for(i = 0; i < u32PicHeight; i++)
            {
                fwrite(pBuf, u32PicWidth, 1, pFile);

                pBuf += u32Stride;
            }
        }
        else
        {
            fwrite(pSaveVirAddr, u32Stride * u32PicHeight, 1, pFile);
        }

        *pu32OffsetSize = u32Stride * u32PicHeight;
    }

    return NI_SUCCESS;
}


static NI_S32 SDK_COMM_VPS_SaveFile(FILE *pFile, VIDEO_FRAME_S *pstVideoFrame)
{
    NI_U32 *pu32Stride, u32PicWidth, u32PicHeight;
    NI_U32 i;
    NI_BOOL bSaveStride = NI_TRUE; /* 是否把stride部分数据也写下来 */
    NI_VOID *pSaveVirAddr = NI_NULL;
    NI_U32 u32PartSize = 0;

    pu32Stride = pstVideoFrame->au32Stride;

    switch(pstVideoFrame->enPixelFormat)
    {     
        case PIXEL_FORMAT_RGB_888:
        {
            if (NI_NULL == pstVideoFrame->apVirAddr[0])
            {
                NI_ERROR("VirAddr is NI_NULL.\n");

                return NI_FAILURE;
            }
                
            pSaveVirAddr = pstVideoFrame->apVirAddr[0];
            
            /* 一行实际的像素数是宽度的3倍 */
            u32PicWidth = pstVideoFrame->u32Width * 3;
            u32PicHeight = pstVideoFrame->u32Height;

            SDK_COMM_VPS_WriteFile(pFile, pSaveVirAddr, u32PicWidth, u32PicHeight, pu32Stride[0], bSaveStride, &u32PartSize);
            break;
        }
        
        case PIXEL_FORMAT_RGB_PLANAR_888:
        case PIXEL_FORMAT_YUV_PLANAR_444:
        {
            if (NI_NULL == pstVideoFrame->apVirAddr[0])
            {
                NI_ERROR("VirAddr is NI_NULL.\n");

                return NI_FAILURE;
            }

            u32PartSize = 0;
            for (i = 0 ; i < 3; i++)
            {
                u32PicWidth = pstVideoFrame->u32Width;
                u32PicHeight = pstVideoFrame->u32Height;
                
                if (NI_NULL == pstVideoFrame->apVirAddr[i])
                {
                    pSaveVirAddr = (NI_VOID *)((NI_U32)pSaveVirAddr + u32PartSize);
                }
                else
                {
                    pSaveVirAddr = pstVideoFrame->apVirAddr[i];
                }

                SDK_COMM_VPS_WriteFile(pFile, pSaveVirAddr, u32PicWidth, u32PicHeight, pu32Stride[i], bSaveStride, &u32PartSize);
            }

            break;
        }
        
        case PIXEL_FORMAT_YUV_SEMIPLANAR_420:
        case PIXEL_FORMAT_YVU_SEMIPLANAR_420:
        {

            if (NI_NULL == pstVideoFrame->apVirAddr[0])
            {
                NI_ERROR("VirAddr is NI_NULL.\n");

                return NI_FAILURE;
            }

            u32PartSize = 0;
            for (i = 0 ; i < 2; i++)
            {
                u32PicWidth = pstVideoFrame->u32Width;
                if (1 == i)
                {
                    /* 色度分量高度减半 */
                    u32PicHeight = pstVideoFrame->u32Height / 2;
                }
                else
                {
                    u32PicHeight = pstVideoFrame->u32Height;
                }
                
            
                if (NI_NULL == pstVideoFrame->apVirAddr[i])
                {
                    pSaveVirAddr = (NI_VOID *)((NI_U32)pSaveVirAddr + u32PartSize);
                }
                else
                {
                    pSaveVirAddr = pstVideoFrame->apVirAddr[i];
                }

                SDK_COMM_VPS_WriteFile(pFile, pSaveVirAddr, u32PicWidth, u32PicHeight, pu32Stride[i], bSaveStride, &u32PartSize);
            }

            break;
        }
        
        case PIXEL_FORMAT_UYVY_PACKAGE_422:
        case PIXEL_FORMAT_YUYV_PACKAGE_422:
        {

            if (NI_NULL == pstVideoFrame->apVirAddr[0])
            {
                NI_ERROR("VirAddr is NI_NULL.\n");

                return NI_FAILURE;
            }
                
            pSaveVirAddr = pstVideoFrame->apVirAddr[0];
            
            /* 一行实际的像素数是宽度的2倍 */
            u32PicWidth = pstVideoFrame->u32Width * 2;
            u32PicHeight = pstVideoFrame->u32Height;

            SDK_COMM_VPS_WriteFile(pFile, pSaveVirAddr, u32PicWidth, u32PicHeight, pu32Stride[0], bSaveStride, &u32PartSize);

            break;
        }
        case PIXEL_FORMAT_YUV_PLANAR_420:
        {
            if (NI_NULL == pstVideoFrame->apVirAddr[0])
            {
                NI_ERROR("VirAddr is NI_NULL.\n");

                return NI_FAILURE;
            }

            u32PartSize = 0;
            for (i = 0 ; i < 3; i++)
            {
                if (0 == i)
                {
                    u32PicWidth = pstVideoFrame->u32Width;
                    u32PicHeight = pstVideoFrame->u32Height;
                }
                else
                {
                    /* 色度分量宽度高度减半 */
                    u32PicWidth = pstVideoFrame->u32Width / 2;
                    u32PicHeight = pstVideoFrame->u32Height / 2;
                }
                
                if (NI_NULL == pstVideoFrame->apVirAddr[i])
                {
                    pSaveVirAddr = (NI_VOID *)((NI_U32)pSaveVirAddr + u32PartSize);
                }
                else
                {
                    pSaveVirAddr = pstVideoFrame->apVirAddr[i];
                }

                SDK_COMM_VPS_WriteFile(pFile, pSaveVirAddr, u32PicWidth, u32PicHeight, pu32Stride[i], bSaveStride, &u32PartSize);
            }

            break;
        }

        case PIXEL_FORMAT_SINGLE:
        {

            if (NI_NULL == pstVideoFrame->apVirAddr[0])
            {
                NI_ERROR("VirAddr is NI_NULL.\n");

                return NI_FAILURE;
            }
                
            pSaveVirAddr = pstVideoFrame->apVirAddr[0];
            
            u32PicWidth = pstVideoFrame->u32Width;
            u32PicHeight = pstVideoFrame->u32Height;
            
            SDK_COMM_VPS_WriteFile(pFile, pSaveVirAddr, u32PicWidth, u32PicHeight, pu32Stride[0], bSaveStride, &u32PartSize);

            break;
        }
        
        default:
        {
            NI_ERROR("PixelFormat type(%d) err!\n", pstVideoFrame->enPixelFormat);

            return NI_FAILURE;
        }
    }

    fflush(pFile);

    return NI_SUCCESS;
}

#if 0
static unsigned int SDK_COMM_VPS_ReadWriteAddress(unsigned int Address, unsigned char * Buffer, unsigned int Length, unsigned int Mode)
{
    int Fd_p = -1;
    unsigned int MapAddressPiont_p;
    unsigned int m_Address;
    unsigned int m_Offset;
    unsigned int page_size = 0x1000;
    
    //取4K对齐的地址和偏移量
    m_Address = Address & 0xFFFFF000;
    m_Offset = Address & 0x00000FFF;
    
    if (NULL == Buffer || m_Offset + Length > page_size)
    {
        NI_ERROR("Buffer == NULL or length is to long, must <= 0x1000\n");

        return NI_FAILURE;
    }
    
    if (-1 == Fd_p)
    {
        if ((Fd_p = open("/dev/mem", O_RDWR | O_SYNC)) < 0)
        {
            return NI_FAILURE;
        }
        
        MapAddressPiont_p = ( unsigned int )mmap((void *)m_Address, page_size   \
            , PROT_READ | PROT_WRITE, MAP_SHARED, Fd_p, m_Address);
        
        if ( 0 == MapAddressPiont_p )
        {
            NI_ERROR("Map 0x%x Failed\n", m_Address);
            close(Fd_p);
            Fd_p = -1;
            
            return NI_FAILURE;
        }
    }
    
    if (0 == Mode)
    {
        memcpy(Buffer, (unsigned char *)MapAddressPiont_p + m_Offset, Length);
    }
    else
    {
        memcpy((unsigned char *)MapAddressPiont_p + m_Offset, Buffer, Length);
    }
    
    munmap( ( void *)MapAddressPiont_p, page_size);
    close(Fd_p);
    Fd_p = -1;

    return NI_SUCCESS;
}

static NI_U32 SDK_COMM_VPS_MemRead(NI_U32 Address)
{
    NI_U32 u32RegValue = 0;
    NI_S32 s32Ret = NI_SUCCESS;

    s32Ret = SDK_COMM_VPS_ReadWriteAddress(Address, (NI_U8 *)&u32RegValue, 4, 0);
    if (s32Ret != NI_SUCCESS)
    {
        return 0xffffffff;
    }
    
    return u32RegValue;
}


static NI_S32 SDK_COMM_VPS_MemWrite(NI_U32 Address, NI_U32 u32RegValue)
{
    NI_S32 s32Ret = NI_SUCCESS;

    s32Ret = SDK_COMM_VPS_ReadWriteAddress(Address, (NI_U8 *)&u32RegValue, 4, 1);
    if (s32Ret != NI_SUCCESS)
    {
        return NI_FAILURE;
    }
    
    return NI_SUCCESS;
}


static NI_S32 SDK_COMM_VPS_PlanToSemi(NI_U8* pY, NI_S32 yStride,
                                   NI_U8* pU, NI_S32 uStride,
                                   NI_U8* pV, NI_S32 vStride,
                                   NI_S32 picWidth, NI_S32 picHeight)
{
    NI_S32 i;
    NI_U8* pTmpU, *ptu;
    NI_U8* pTmpV, *ptv;

    NI_S32 s32HafW = picWidth >> 1 ;
    NI_S32 s32HafH = picHeight >> 1 ;
    NI_S32 s32Size = s32HafW * s32HafH;

    pTmpU = (NI_U8 *)malloc(s32Size);
    ptu = pTmpU;
    pTmpV = (NI_U8 *)malloc(s32Size);
    ptv = pTmpV;
    if ((pTmpU == NULL) || (pTmpV == NULL))
    {
        NI_ERROR("malloc buf failed\n");

        return NI_FAILURE;
    }

    memcpy(pTmpU, pU, s32Size);
    memcpy(pTmpV, pV, s32Size);

    for (i = 0; i<s32Size >> 1; i++)
    {
        //*pU++ = *pTmpV++;
        *pU++ = *pTmpU++;
        *pU++ = *pTmpV++;

    }
    for (i = 0; i<s32Size >> 1; i++)
    {
        //*pV++ = *pTmpV++;
        *pV++ = *pTmpU++;
        *pV++ = *pTmpV++;
    }

    free(ptu);
    free(ptv);
    ptu = NULL;
    ptv = NULL;
    return NI_SUCCESS;
}
#endif


static NI_VOID *SDK_COMM_VPS_SendFrameProc(NI_VOID *p)
{
    NI_S32 s32Ret = NI_SUCCESS;
    VPS_GRP     VpsGrp;
    VPS_TEST_SENDFRM_PARA_S *pstPara;
    FILE *pfd = NI_NULL;
    VPS_FRAME_INFO_S stFrameInfo;
    NI_U32 u32PhyAddr = 0;
    NI_VOID *pVirAddr = NULL;
    NI_U32 u32FrameSize = 0;
    NI_U32 u32FileSize = 0;
    NI_U32 u32FrameNum = 0;
    NI_CHAR *pFileName;
    //NI_U32 u32TimeOut;
    NI_U32 u32VpsDebugPhyAddr = 0;
    NI_U32 *pVpsDebugVirAddr = NULL;
    NI_BOOL     bRead2MemBefor;
    NI_U32      u32ReadStart;
    NI_U32      u32ReadInterval;
    NI_U32      u32ReadCnt;
    NI_U32 i;
    NI_BOOL bVpsDebug;
    NI_U32  u32InPutFps;
    NI_U32  u32InPutIntervalMs;
    NI_U32  u32LastInPutTimeMs = 0;
    NI_U32  u32CurInPutTimeMs = 0;
    NI_U32  u32VpsWorkHz = 40 * 1000000; /* VPS工作频率40M,有处理上限 */
    NI_U32  u32VpsWorkMAxFps;
    VPS_ROTATE_TYPE_E enRotate;
    VPS_SEND_MEM_DESC_S stVpsSendMacroMemSesc[2] = {{0, 0, 0, 0, 0}};
    NI_U32      u32MacroBlkCnt;

    pstPara = (VPS_TEST_SENDFRM_PARA_S*)p;
    VpsGrp = pstPara->VpsGrp;
    pFileName = pstPara->acFileName;   
    //u32TimeOut = pstPara->u32TimeOut;
    bRead2MemBefor = pstPara->bRead2MemBefor;
    u32ReadStart = pstPara->u32ReadStart;
    u32ReadInterval = pstPara->u32ReadInterval;
    u32ReadCnt = pstPara->u32ReadCnt;
    u32InPutFps = pstPara->u32InPutFps;
    bVpsDebug = pstPara->bStepDebug;
    u32VpsWorkMAxFps = u32VpsWorkHz / (pstPara->u32Width * pstPara->u32Height);
    
    if (pstPara->u32InPutFps > u32VpsWorkMAxFps)
    {
        u32InPutFps = u32VpsWorkMAxFps;
    }

    u32InPutIntervalMs = 1000 / u32InPutFps;

    NI_INFO("pstPara->u32Width=%d, pstPara->u32Heigh=%d.\n", pstPara->u32Width, pstPara->u32Height);
    NI_INFO("bRead2MemBefor=%d, u32InPutFps=%d, InPutIntervalMs=%d.\n", 
        bRead2MemBefor, u32InPutFps, u32InPutIntervalMs);
    
    if (NI_TRUE == bRead2MemBefor)
    {
        NI_INFO("ReadStart=%d, ReadInterval=%d, ReadCnt=%d.\n", 
            u32ReadStart, u32ReadInterval, u32ReadCnt);
    }

    if (pstPara->enPixelFormat != PIXEL_FORMAT_YUV_SEMIPLANAR_420)
    {
        NI_ERROR("Input File PixelFormat(%d) is not Nv12.\n", pstPara->enPixelFormat);

        return NI_NULL;
    }

    NI_Clear(&stFrameInfo);

    pfd = fopen(pFileName, "rb");
    if (!pfd)
    {
        NI_ERROR("open file[%s] failed!\n", pFileName);

        return NI_NULL;
    }

    u32FrameSize = pstPara->u32Width * pstPara->u32Height * 3 / 2;

    fseek(pfd, 0, SEEK_END);     
    u32FileSize = ftell(pfd);
    u32FrameNum = NI_Div(u32FileSize, u32FrameSize);
    NI_INFO("file[%s]: FrameNum is %d, FrameSize=%d.\n", pFileName, u32FrameNum, u32FrameSize);
    
    rewind(pfd);

    for ( i = 0 ; i < 100; i++ )
    {
        g_stVpsSendMemSesc[VpsGrp][i].bUsed = NI_FALSE;
        g_stVpsSendMemSesc[VpsGrp][i].bActive = NI_FALSE;
        g_stVpsSendMemSesc[VpsGrp][i].u32Id = i;
        g_stVpsSendMemSesc[VpsGrp][i].u32PhyAddr = 0;
        g_stVpsSendMemSesc[VpsGrp][i].pVirAddr = NI_NULL;
    }

    NI_MDK_SYS_MmzAlloc(&u32VpsDebugPhyAddr, (NI_VOID **)&pVpsDebugVirAddr, 4096, "vps_debug", NULL);
    NI_INFO("VpsDebugPhyAddr=%#x, pVpsDebugVirAddr=%#x, ofset 0 is debug mode switch, ofset 1 is send frm enable, offset 2 is print switch.\n", 
        u32VpsDebugPhyAddr, (NI_U32)pVpsDebugVirAddr);
    NI_ClearSize((NI_VOID *)pVpsDebugVirAddr, 4096);

    if (NI_TRUE == bVpsDebug)
    {
        pVpsDebugVirAddr[0] = 1;
    }

    if (NI_TRUE == bRead2MemBefor)
    {
        if (u32ReadCnt > 100 || u32ReadCnt < 2 || (u32ReadStart + (u32ReadCnt - 1) * (u32ReadInterval + 1)) > (u32FrameNum - 1))
        {
            NI_ERROR("is Read2MemBefor but Read Frm Num Error!\n");
            NI_ERROR("ReadStart=%d, ReadCnt=%d, ReadInterval=%d.\n", 
                u32ReadStart, u32ReadCnt, u32ReadInterval);
            fclose(pfd);
            
            return NULL;
        }

        fseek(pfd, u32ReadStart * u32FrameSize, SEEK_SET);

        NI_MDK_SYS_MmzAlloc(&u32PhyAddr, &pVirAddr, u32ReadCnt * u32FrameSize, "vps_test", NULL );

        for ( i = 0 ; i < u32ReadCnt; i++ )
        {
            fread(((NI_U8 *)pVirAddr + i * u32FrameSize), u32FrameSize, 1, pfd);
            fseek(pfd, u32ReadInterval * u32FrameSize, SEEK_CUR);

            g_stVpsSendMemSesc[VpsGrp][i].bUsed = NI_TRUE;
            g_stVpsSendMemSesc[VpsGrp][i].bActive = NI_TRUE;
            g_stVpsSendMemSesc[VpsGrp][i].u32PhyAddr = u32PhyAddr + i * u32FrameSize;
            g_stVpsSendMemSesc[VpsGrp][i].pVirAddr = (NI_U8 *)pVirAddr + i * u32FrameSize;
        }
    }
    else
    {
        /* 申请2帧大小的内存 */
        NI_MDK_SYS_MmzAlloc(&u32PhyAddr, &pVirAddr, 2 * u32FrameSize, "vps_test", NULL);
        u32ReadCnt = 2;

        for ( i = 0 ; i < u32ReadCnt; i++ )
        {
            g_stVpsSendMemSesc[VpsGrp][i].bUsed = NI_TRUE;
            g_stVpsSendMemSesc[VpsGrp][i].bActive = NI_TRUE;
            g_stVpsSendMemSesc[VpsGrp][i].u32PhyAddr = u32PhyAddr + i * u32FrameSize;
            g_stVpsSendMemSesc[VpsGrp][i].pVirAddr = (NI_U8 *)pVirAddr + i * u32FrameSize;
        }
    }

{
    NI_U32  MacroBlkSize = 0;
    
    u32MacroBlkCnt = NI_Min(2, u32ReadCnt);
    MacroBlkSize = NI_Align(2 * u32FrameSize, 2048); /* 硬件要求2K对齐 */

    /* 申请2帧存放宏块数据的内存 */
    NI_MDK_SYS_MmzAlloc(&u32PhyAddr, &pVirAddr, MacroBlkSize * 2, "vps_test_macro", NULL);

    for ( i = 0 ; i < u32MacroBlkCnt; i++ )
    {
        stVpsSendMacroMemSesc[i].bUsed = NI_TRUE;
        stVpsSendMacroMemSesc[i].bActive = NI_TRUE;
        stVpsSendMacroMemSesc[i].u32PhyAddr = u32PhyAddr + i * MacroBlkSize;
        stVpsSendMacroMemSesc[i].pVirAddr = (NI_U8 *)pVirAddr + i * MacroBlkSize;

        if (NI_TRUE == bRead2MemBefor)
        {
            SDK_COMM_VPS_YUV420spToMb(pstPara->u32Width, pstPara->u32Height, 
                (NI_CHAR *)(g_stVpsSendMemSesc[VpsGrp][i].pVirAddr), 
                (NI_CHAR *)(stVpsSendMacroMemSesc[i].pVirAddr));
        }
    }
}

    while (NI_TRUE == pstPara->bThreadStart)
    {
        /* 这里可以进行送帧控制，在控制台敲mem命令将u32VpsDebugPhyAddr写1才进行送1帧 */
        if (1 == pVpsDebugVirAddr[0])
        {
            if (0 == pVpsDebugVirAddr[1])
            {
                usleep(10000);
            
                continue;
            }
            else
            {
                pVpsDebugVirAddr[1] = 0;
            }
        }

        s32Ret = NI_MDK_VPS_GetGrpRotate(VpsGrp, &enRotate);
        if (s32Ret != NI_SUCCESS)
        {
            NI_ERROR("NI_MDK_VPS_GetGrpRotate %#x.\n", s32Ret);

            break;
        }

        if (NI_TRUE != bRead2MemBefor)
        {
            /* 从文件读取一帧数据, 送去编码 */
            fread(g_stVpsSendMemSesc[VpsGrp][pstPara->u32SendFrmCnt % u32ReadCnt].pVirAddr, u32FrameSize, 1, pfd);
        }

        if (VPS_ROTATE_90 == enRotate || VPS_ROTATE_270 == enRotate
            || VPS_ROTATE_FLIP_90 == enRotate || VPS_ROTATE_MIRROR_90 == enRotate)
        {
            stFrameInfo.astVideoFrame[0].enPixelFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_420_MB16x16;

            if (NI_TRUE != bRead2MemBefor)
            {
                /* 在这里做宏块数据转换操作 */
                SDK_COMM_VPS_YUV420spToMb(pstPara->u32Width, pstPara->u32Height, 
                    (NI_CHAR *)(g_stVpsSendMemSesc[VpsGrp][pstPara->u32SendFrmCnt % u32ReadCnt].pVirAddr), 
                    (NI_CHAR *)(stVpsSendMacroMemSesc[pstPara->u32SendFrmCnt % u32MacroBlkCnt].pVirAddr));
            }
            
            stFrameInfo.astVideoFrame[0].aphysAddr[0] = stVpsSendMacroMemSesc[pstPara->u32SendFrmCnt % u32MacroBlkCnt].u32PhyAddr;
            stFrameInfo.astVideoFrame[0].u32Width = pstPara->u32Width;
            stFrameInfo.astVideoFrame[0].u32Height = NI_Align16(pstPara->u32Height);
        }
        else
        {
            stFrameInfo.astVideoFrame[0].enPixelFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
            stFrameInfo.astVideoFrame[0].aphysAddr[0] =  g_stVpsSendMemSesc[VpsGrp][pstPara->u32SendFrmCnt % u32ReadCnt].u32PhyAddr;
            stFrameInfo.astVideoFrame[0].u32Width = pstPara->u32Width;
            stFrameInfo.astVideoFrame[0].u32Height = pstPara->u32Height;
        }
        
        stFrameInfo.u32TimeStamp = pstPara->u32SendFrmCnt;

        /* 按一定时间间隔进行发送 */
        u32CurInPutTimeMs = SDK_COMM_VPS_GetTimeMs();
        if (u32CurInPutTimeMs - u32LastInPutTimeMs < u32InPutIntervalMs)
        {
            /* 粗略休眠 */
            usleep((u32InPutIntervalMs - (u32CurInPutTimeMs - u32LastInPutTimeMs)) * 1000);
        }

        s32Ret= NI_MDK_VPS_TestSendFrame(VpsGrp, &stFrameInfo);
        if (s32Ret != NI_SUCCESS)
        {
            NI_ERROR("NI_MDK_VPS_TestSendFrame %#x.\n", s32Ret);
        }

        if (1 == pVpsDebugVirAddr[2])
        {
            NI_INFO("VPSSendFrame %d: PhyAddr=%#x Success.\n", pstPara->u32SendFrmCnt, stFrameInfo.astVideoFrame[0].au32PhyAddr[0]);
        }

        /* 记录发送成功的时间 */
        u32LastInPutTimeMs = SDK_COMM_VPS_GetTimeMs();

        pstPara->u32SendFrmCnt++;

        if (NI_TRUE != bRead2MemBefor)
        {
            if (pstPara->u32SendFrmCnt % u32FrameNum == 0)
            {
                if (NI_TRUE == pstPara->bSendOnece)
                {
                    break;
                }
                else
                {
                    /* 读到文件尾部后从头开始读*/
                    rewind(pfd);
                }
            }
        }
        else
        {
            if (NI_TRUE == pstPara->bSendOnece && pstPara->u32SendFrmCnt == u32ReadCnt)
            {
                break;
            }
        }
    }

    if (g_stVpsSendMemSesc[VpsGrp][0].u32PhyAddr != 0)
    {
        NI_MDK_SYS_MmzFree(g_stVpsSendMemSesc[VpsGrp][0].u32PhyAddr, g_stVpsSendMemSesc[VpsGrp][0].pVirAddr);
    }

    if (stVpsSendMacroMemSesc[0].u32PhyAddr != 0)
    {
        NI_MDK_SYS_MmzFree(stVpsSendMacroMemSesc[0].u32PhyAddr, stVpsSendMacroMemSesc[0].pVirAddr);
    }

    if (u32VpsDebugPhyAddr != 0)
    {
        NI_MDK_SYS_MmzFree(u32VpsDebugPhyAddr, pVpsDebugVirAddr);
    }
    
    fclose(pfd);
    pstPara->bThreadStart = NI_FALSE;

    NI_INFO("pthread exit ok, SendFrm %d frame.\n", pstPara->u32SendFrmCnt);
   
    return NI_NULL;
}


NI_VOID *SDK_COMM_VPS_GetFrameProc(NI_VOID *p)
{
    VPS_TEST_GET_FRAME_PARA_S *pstPara = (VPS_TEST_GET_FRAME_PARA_S*)p;
    NI_CHAR aszFileName[256];
    VPS_FRAME_INFO_S stVpsFrameInfo;
    VPS_FRAME_INFO_S *pstVpsFrameInfo = &stVpsFrameInfo;
    FILE *pFile = NI_NULL;
    char szFilePostfix[10];
    NI_S32 s32Ret;
    NI_U32 u32LastPicWidth = 0, u32LastPicHeight = 0;
    VPS_GRP VpsGrp;
    VPS_CHN_E VpsChn;
    NI_U32 u32Timeout;
    NI_BOOL    bSaveFile;
    NI_CHAR *lpszTimeName;
    NI_U32 u32GetFrmDebugPhyAddr = 0;
    NI_U32 *pGetFrmDebugVirAddr = NULL;
    NI_BOOL bGetFrmDebug = NI_FALSE;
    VIDEO_FRAME_S *pstVideoFrame = NI_NULL;
    NI_VOID *pVirtAddr = NULL;
    
    VpsGrp = pstPara->VpsGrp;
    VpsChn = pstPara->VpsChn;
    u32Timeout = pstPara->u32TimeOut;
    bSaveFile = pstPara->bSaveFile;
    
    pstPara->u32GetFrmNum = 0;

    pstVideoFrame = &pstVpsFrameInfo->astVideoFrame[0];

    NI_MDK_SYS_MmzAlloc(&u32GetFrmDebugPhyAddr, (NI_VOID **)&pGetFrmDebugVirAddr, 4096, "vps_getfrm_debug", NULL);
    NI_INFO("GetFrmDebugPhyAddr=%#x, GetFrmDebugVirAddr=%#x, ofset 0 is debug mode switch, ofset 1 is get frm enable, offset 2 is print switch.\n", 
        u32GetFrmDebugPhyAddr, (NI_U32)pGetFrmDebugVirAddr);
    NI_ClearSize((NI_VOID *)pGetFrmDebugVirAddr, 4096);

    if (NI_TRUE == bGetFrmDebug)
    {
        pGetFrmDebugVirAddr[0] = 1;
    }

    while (NI_TRUE == pstPara->bThreadStart)
    {
        /* 这里可以进行送帧控制，在控制台敲mem命令将GetFrmDebugPhyAddr写1才进行送1帧 */
        if (1 == pGetFrmDebugVirAddr[0])
        {
            if (0 == pGetFrmDebugVirAddr[1])
            {
                usleep(10000);
            
                continue;
            }
            else
            {
                pGetFrmDebugVirAddr[1] = 0;
            }
        }
        
        NI_Clear(pstVpsFrameInfo);

        if (1 == pGetFrmDebugVirAddr[2])
        {
            NI_INFO("VpsGrp(%d),VpsChn(%d): start get GetFrame %d.\n", 
                VpsGrp, VpsChn, pstPara->u32GetFrmNum);
        }
        
        s32Ret = NI_MDK_VPS_GetFrame(VpsGrp, VpsChn, pstVpsFrameInfo, u32Timeout);
        if ( NI_SUCCESS != s32Ret )
        {
            if (NI_ERR_VPS_GET_STREAM_TIMEOUT == s32Ret || NI_ERR_VPS_CHN_NOT_ENABLE == s32Ret)
            {
                continue;
            }            
            NI_ERROR("NI_MDK_VPS_GetFrame failed with %#x!\n", s32Ret);
            
            break;
        }

        /* for debug */
        if (1 == pGetFrmDebugVirAddr[2])
        {
            // NI_PRINT("VpsGrp(%d),VpsChn(%d), FrmNum[%d]: TimeStamp=%#x, %d x %d, Pts=%lld, PixelFormat=%d.\n"
            //     , VpsGrp, VpsChn, pstPara->u32GetFrmNum, pstVpsFrameInfo->u32TimeStamp, 
            //     pstVideoFrame->u32Width, pstVideoFrame->u32Height, 
            //     pstVideoFrame->u64Pts, pstVideoFrame->enPixelFormat);

            // NI_PRINT("VpsGrp(%d),VpsChn(%d), FrmNum[%d]: PhyAddr=[%#x,%#x,%#x], VirAddr=[%#x,%#x,%#x], stride=[%d,%d,%d].\n"
            //     , VpsGrp, VpsChn, pstPara->u32GetFrmNum, pstVideoFrame->aphysAddr[0], 
            //     pstVideoFrame->aphysAddr[1], pstVideoFrame->aphysAddr[2], 
            //     (NI_U32)pstVideoFrame->apVirAddr[0], (NI_U32)pstVideoFrame->apVirAddr[1],
            //     (NI_U32)pstVideoFrame->apVirAddr[2], pstVideoFrame->au32Stride[0],
            //     pstVideoFrame->au32Stride[1], pstVideoFrame->au32Stride[2]);
        }

        /* 保存文件 */
        if (NI_TRUE == bSaveFile)
        {
            pVirtAddr = NI_NULL;
            
            if ((pstVideoFrame->u32Width != u32LastPicWidth) || 
                (pstVideoFrame->u32Height != u32LastPicHeight))
            {
                if (pFile != NI_NULL)
                {
                    fclose(pFile);
                    pFile = NI_NULL;
                }

                SDK_COMM_VPS_GetCurrentTime(&lpszTimeName);
                s32Ret = SDK_COMM_VPS_GetFilePostfix(pstVideoFrame->enPixelFormat, szFilePostfix);
                if(s32Ret != NI_SUCCESS)
                {
                    NI_ERROR("SDK_COMM_VPS_GetFilePostfix [%d] failed with %d!\n", \
                           pstVideoFrame->enPixelFormat, s32Ret);
                    NI_MDK_VPS_ReleaseFrame(VpsGrp, VpsChn, pstVpsFrameInfo);

                    break;
                }
                
                memset(aszFileName, 0, 256);
                sprintf(aszFileName, "vps_grp%d_chn%d_%dx%d_%s%s", 
                    VpsGrp, VpsChn, pstVideoFrame->u32Width,
                    pstVideoFrame->u32Height,lpszTimeName, szFilePostfix);
                
                pFile = fopen(aszFileName, "wb");
                if (!pFile)
                {
                    NI_ERROR("VpsGrp(%d),VpsChn(%d): open file[%s] failed, thread exit!\n", VpsGrp, VpsChn, aszFileName);
                    pstPara->bThreadStart = NI_FALSE;
                    NI_MDK_VPS_ReleaseFrame(VpsGrp, VpsChn, pstVpsFrameInfo);
                    
                    break;
                }

                NI_INFO("VpsGrp(%d),VpsChn(%d): open file[%s] ok!\n", VpsGrp, VpsChn, aszFileName);
                
                u32LastPicWidth = pstVideoFrame->u32Width;
                u32LastPicHeight = pstVideoFrame->u32Height;
            }

            if (0 == pstVpsFrameInfo->astVideoFrame[0].aphysAddr[0] || 0 == pstVpsFrameInfo->u32FrameSize)
            {
                NI_ERROR("VpsGrp(%d),VpsChn(%d):GetFrame u32PhyAddr is 0 !\n", VpsGrp, VpsChn);
                NI_MDK_VPS_ReleaseFrame(VpsGrp, VpsChn, pstVpsFrameInfo);

                break;
            }

            if (NI_NULL == pstVpsFrameInfo->astVideoFrame[0].apVirAddr[0])
            {
                pVirtAddr = NI_MDK_SYS_Mmap(pstVpsFrameInfo->astVideoFrame[0].aphysAddr[0], pstVpsFrameInfo->u32FrameSize);
                if (NI_NULL == pVirtAddr)
                {
                    NI_ERROR("VpsGrp(%d),VpsChn(%d): Mmap u32PhyAddr[%#x], Len=%#x err !\n", 
                        VpsGrp, VpsChn, pstVpsFrameInfo->astVideoFrame[0].aphysAddr[0], pstVpsFrameInfo->u32FrameSize);
                    NI_MDK_VPS_ReleaseFrame(VpsGrp, VpsChn, pstVpsFrameInfo);

                    break;
                }

                pstVpsFrameInfo->astVideoFrame[0].apVirAddr[0] = pVirtAddr;
                pstVpsFrameInfo->astVideoFrame[0].apVirAddr[1] = NI_NULL;
                pstVpsFrameInfo->astVideoFrame[0].apVirAddr[2] = NI_NULL;
            }
                
            SDK_COMM_VPS_SaveFile(pFile, pstVideoFrame);

            if (pVirtAddr != NI_NULL)
            {
                NI_MDK_SYS_Munmap(pVirtAddr, pstVpsFrameInfo->u32FrameSize);
            }
        }

        s32Ret = NI_MDK_VPS_ReleaseFrame(VpsGrp, VpsChn, pstVpsFrameInfo);
        if (NI_SUCCESS != s32Ret)
        {
            NI_ERROR("NI_MDK_VPS_ReleaseFrame failed with %#x!\n", s32Ret);
        }
        
        if (1 == pGetFrmDebugVirAddr[2])
        {
            NI_INFO("VpsGrp(%d),VpsChn(%d): get GetFrame %d Success.\n", 
                VpsGrp, VpsChn, pstPara->u32GetFrmNum);
        }
        
        pstPara->u32GetFrmNum++;
    }

    if (pFile != NI_NULL)
    {
        fclose(pFile);
    }

    if (u32GetFrmDebugPhyAddr != 0)
    {
        NI_MDK_SYS_MmzFree(u32GetFrmDebugPhyAddr, pGetFrmDebugVirAddr);
    }
    
    pstPara->bThreadStart = NI_FALSE;
    
    NI_INFO("pthread exit ok, GetFrmNum=%d.\n", pstPara->u32GetFrmNum);

    return NI_NULL;
}


NI_S32 SDK_COMM_VPS_Start(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, TEST_VPS_CONFIG_S *pstVpsCfg)
{
    NI_S32         s32Ret = NI_SUCCESS;
    VPS_GRP_ATTR_S stGrpAttr;
    MDK_CHN_S      stSrcChn;
    MDK_CHN_S      stDestChn;

    memset(&stGrpAttr, 0, sizeof(VPS_GRP_ATTR_S));
    stGrpAttr.u32InFps    = pstVpsCfg->u32InFps;
    stGrpAttr.u32InWidth  = pstVpsCfg->u32InWidth;
    stGrpAttr.u32InHeight = pstVpsCfg->u32InHeight;
    stGrpAttr.bMultiEye   = NI_FALSE;

    if (NI_TRUE == pstVpsCfg->bYuv2Ldc)
	{
        stGrpAttr.enWorkMode                    = VPS_WORK_MODE_OFFLINE;
        stGrpAttr.stOffLineAttr.bCropEn         = NI_FALSE;
        stGrpAttr.stOffLineAttr.enGetGrpFrmMode = VPS_GETGRPFRM_MODE_NONE;
        stGrpAttr.stOffLineAttr.u32BufNum       = 1;
        stGrpAttr.stOffLineAttr.u32OutFps = pstVpsCfg->u32InFps;
    }
	else
	{
        stGrpAttr.enWorkMode = VPS_WORK_MODE_ONLINE;
    }

    s32Ret = NI_MDK_VPS_SetGrpAttr(VpsGrp, &stGrpAttr);
    if(s32Ret != NI_SUCCESS)
	{
    	PRINT_ERROR(" NI_MDK_VPS_SetGrpAttr failed %#x\n", s32Ret);
        return s32Ret;
    }
    if (VpsChn <= VPS_CHN_IAS1)
	{
        VPS_CHN_ATTR_S stChnAttr;
        memset(&stChnAttr, 0, sizeof(VPS_CHN_ATTR_S));
        stChnAttr.bCropEn      = NI_FALSE;
        stChnAttr.u32OutWidth  = pstVpsCfg->u32OutWidth;
        stChnAttr.u32OutHeight = pstVpsCfg->u32OutHeight;

        if (VpsChn <= VPS_CHN_ENC2)
		{
            stChnAttr.enAttrType            = VPS_CHN_ATTR_TYPE_ENC;
            stChnAttr.stEncAttr.enType      = pstVpsCfg->enPayLoad;
            stChnAttr.stEncAttr.u32YStride  = pstVpsCfg->u32OutWidth;
            stChnAttr.stEncAttr.u32CStride  = pstVpsCfg->u32OutWidth;
            stChnAttr.stEncAttr.enOutFormat = pstVpsCfg->enOutFormat;
            stChnAttr.stEncAttr.u32OutFps = pstVpsCfg->u32OutFps;
            stChnAttr.stEncAttr.bCompress = VPS_CHN_ENC0 == VpsChn ? NI_TRUE : NI_FALSE;
            if (NI_TRUE == pstVpsCfg->bOnLineMode)
			{
                stChnAttr.stEncAttr.enWorkMode = VPS_WORK_MODE_ONLINE;
            } 
			else 
			{
                stChnAttr.stEncAttr.enWorkMode = VPS_WORK_MODE_OFFLINE;
            }

            if (VpsChn == 0)
            {
				//主码流通道离线单帧存
                stChnAttr.stEncAttr.u32OfflineBufNum = 1;
                stChnAttr.stEncAttr.u32BufLineNum = 144; 
            }
            else
            {
				//其他通道离线两帧存
                stChnAttr.stEncAttr.u32OfflineBufNum = 2;
            }
            
            if (-1 != pstVpsCfg->enEncBindChn)
			{
                /* 绑定数据源 */
                stSrcChn.enModId  = NI_ID_VPS;
                stSrcChn.s32DevId = VpsGrp;
                stSrcChn.s32ChnId = VpsChn;
                stDestChn.enModId                = NI_ID_VENC;
                stDestChn.s32DevId               = 0;
                stDestChn.s32ChnId               = pstVpsCfg->enEncBindChn;
                as32EncDestChnId[VpsGrp][VpsChn] = pstVpsCfg->enEncBindChn;
                NI_MDK_SYS_Bind(&stSrcChn, &stDestChn);
            }
        }
		else if (VPS_CHN_IAS0 == VpsChn || VPS_CHN_IAS1 == VpsChn)
		{
            stChnAttr.enAttrType                = VPS_CHN_ATTR_TYPE_IAS;
            stChnAttr.stIasAttr.enOutFormat     = pstVpsCfg->enOutFormat;
            stChnAttr.stIasAttr.u32BuffNum      = 3;
            stChnAttr.stIasAttr.u32OutFps       = pstVpsCfg->u32OutFps;
            stChnAttr.stIasAttr.enIasRel2EncChn = VPS_CHN_IAS_RELATE2NONE;
        }

        s32Ret = NI_MDK_VPS_CreateChn(VpsGrp, VpsChn, &stChnAttr);
        if (s32Ret != NI_SUCCESS)
		{
            PRINT_ERROR(" NI_MDK_VPS_CreateChn VpsGrp %d VpsChn %d failed %#x\n", VpsGrp, VpsChn,s32Ret);
            return s32Ret;
        }
    }

    if (VpsChn != VPS_CHN_DISP)
	{
        s32Ret = NI_MDK_VPS_EnableChn(VpsGrp, VpsChn);
        if (s32Ret != NI_SUCCESS)
		{
            PRINT_ERROR("NI_MDK_VPS_EnableChn %d VpsChn %d failed %#x\n", VpsGrp, VpsChn,s32Ret);
            return s32Ret;
        }
    }
    return NI_SUCCESS;
}

NI_S32 SDK_COMM_VPS_Stop(VPS_GRP VpsGrp, VPS_CHN_E VpsChn)
{
    MDK_CHN_S stSrcChn;
    MDK_CHN_S stDestChn;

    NI_MDK_VPS_DisableChn(VpsGrp, VpsChn);
        
    if (VpsChn <= VPS_CHN_IAS1)
    {
        NI_MDK_VPS_DestroyChn(VpsGrp, VpsChn);
    }

    if(VpsChn <= VPS_CHN_ENC2)
    {
        /* 去绑定数据源 */
        stSrcChn.enModId = NI_ID_VPS;
        stSrcChn.s32ChnId = VpsChn;
        stSrcChn.s32DevId = VpsGrp;
        
        stDestChn.enModId = NI_ID_VENC;
        stDestChn.s32DevId = 0;
        stDestChn.s32ChnId = as32EncDestChnId[VpsGrp][VpsChn];
        
        NI_MDK_SYS_UnBind(&stSrcChn, &stDestChn);
    }

    return NI_SUCCESS;
}


NI_S32 SDK_COMM_VPS_StartSendFrame(VPS_GRP VpsGrp, NI_U32 u32Width, NI_U32 u32Height, NI_U32 u32InPutFps, NI_CHAR *pFileName, PIXEL_FORMAT_E enPixelFormat, NI_BOOL bSendOnece, NI_BOOL bStepDebug)
{
    if (VpsGrp >= VPS_MAX_GRP_NUM)
    {
        NI_ERROR("VpsGrp is %d must < %d.\n", VpsGrp, VPS_MAX_GRP_NUM);

        return NI_FAILURE;
    }
    
    if (NI_TRUE == gastVpsSendFrmPara[VpsGrp].bThreadStart)
    {
        NI_WARN("VpsGrp:%d, send frame thread is already start.\n", VpsGrp);

        return NI_SUCCESS;
    }
    
    gastVpsSendFrmPara[VpsGrp].VpsGrp = VpsGrp;
    gastVpsSendFrmPara[VpsGrp].bSendOnece = bSendOnece;
    gastVpsSendFrmPara[VpsGrp].enPixelFormat = enPixelFormat;
    gastVpsSendFrmPara[VpsGrp].u32Width= u32Width;
    gastVpsSendFrmPara[VpsGrp].u32Height = u32Height;
    gastVpsSendFrmPara[VpsGrp].bThreadStart = NI_TRUE;
    gastVpsSendFrmPara[VpsGrp].u32TimeOut = 1000;   /*休眠时间可根据实际情况自己设置ms*/
    gastVpsSendFrmPara[VpsGrp].u32SendFrmCnt = 0;
    gastVpsSendFrmPara[VpsGrp].u32InPutFps = u32InPutFps;
    strncpy(gastVpsSendFrmPara[VpsGrp].acFileName, pFileName, 128);

    gastVpsSendFrmPara[VpsGrp].bRead2MemBefor = NI_TRUE;
    gastVpsSendFrmPara[VpsGrp].u32ReadCnt = 10;
    gastVpsSendFrmPara[VpsGrp].u32ReadStart = 0;
    gastVpsSendFrmPara[VpsGrp].u32ReadInterval = 1;

    gastVpsSendFrmPara[VpsGrp].bStepDebug = bStepDebug;
    
    return pthread_create(&gastVpsSendFrmPid[VpsGrp], 0,\
                           SDK_COMM_VPS_SendFrameProc, (NI_VOID*)&gastVpsSendFrmPara[VpsGrp]);
}


NI_S32 SDK_COMM_VPS_StopSendFrame(VPS_GRP VpsGrp)
{
    if (NI_TRUE == gastVpsSendFrmPara[VpsGrp].bThreadStart)
    {    
        gastVpsSendFrmPara[VpsGrp].bThreadStart = NI_FALSE;
        pthread_join(gastVpsSendFrmPid[VpsGrp], 0);
    }
    
    return NI_SUCCESS;
}


NI_S32 SDK_COMM_VPS_StartGetFrame(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, NI_BOOL bSaveFile)
{
    if (NI_TRUE == s_astVpsGetFrmPara[VpsGrp][VpsChn].bThreadStart)
    {
        NI_WARN("grp:%d, chn:%d, GetFrame thread is already start.\n", VpsGrp, VpsChn);

        return NI_SUCCESS;
    }
    s_astVpsGetFrmPara[VpsGrp][VpsChn].VpsGrp = VpsGrp;
    s_astVpsGetFrmPara[VpsGrp][VpsChn].VpsChn = VpsChn;
    s_astVpsGetFrmPara[VpsGrp][VpsChn].bThreadStart = NI_TRUE;
    s_astVpsGetFrmPara[VpsGrp][VpsChn].u32TimeOut = 1000;
    s_astVpsGetFrmPara[VpsGrp][VpsChn].u32GetFrmNum = 0;
    s_astVpsGetFrmPara[VpsGrp][VpsChn].bSaveFile = bSaveFile;
    return pthread_create(&s_astVpsGetFrmPid[VpsGrp][VpsChn], 0,\
                           SDK_COMM_VPS_GetFrameProc, (NI_VOID*)&s_astVpsGetFrmPara[VpsGrp][VpsChn]);
}


NI_S32 SDK_COMM_VPS_StopGetFrame(VPS_GRP VpsGrp, VPS_CHN_E VpsChn)
{

    if (NI_TRUE == s_astVpsGetFrmPara[VpsGrp][VpsChn].bThreadStart)
    {
        s_astVpsGetFrmPara[VpsGrp][VpsChn].bThreadStart = NI_FALSE;
        pthread_join(s_astVpsGetFrmPid[VpsGrp][VpsChn], 0);
    }
    
    return NI_SUCCESS;
}

