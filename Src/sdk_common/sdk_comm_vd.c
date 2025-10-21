/* 
 * sdk_comm_vd.c -- vd sample用例 
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <pthread.h>

#include "ni_comm_md.h"
#include "mdk_md.h"
// #include "mdk_mps.h"
#include "mdk_mbuf.h"
#include "mdk_sys.h"
#include "sdk_comm.h"


typedef struct tagSampleCommGetVdPara
{
    NI_BOOL   bThreadStart;
    VPS_GRP    VpsGrp;
    VD_CHN     VdChn;
    NI_U32     u32TimeOut;
    NI_U32     u32Width; 
    NI_U32     u32Height;
    NI_U32     u32SBColNum;
    NI_U32     u32SBRowNum;
    NI_U8       u8AomId;
} SDK_COMM_GET_VD_PARA_S;

typedef struct tagSampleCommGetData
{
     NI_BOOL    bThreadStart;
     VD_CHN     VdChn;
     NI_U32     u32Timeout;
     NI_U32     u32Width; 
     NI_U32     u32Height;
} SDK_COMM_GETDATA_PARA_S;

typedef struct tagSampleCommSendFrame
{
     NI_BOOL    bThreadStart;
     VD_CHN     VdChn;
     NI_U32     u32Width; 
     NI_U32     u32Height;
     NI_CHAR    acYuvFile[64];
} SDK_COMM_SENDFRAM_PARA_S;


typedef struct
{
    NI_U8 *pu8VirtAddr[3];           /* 虚拟地址, 支持YUV单独分量 */
    NI_U32 u32Width;                  /* 宽度 */
    NI_U32 u32Height;                 /* 高度 */
    NI_U32 u32LineOffset;             /* 行跨距 */
    NI_U32 u32MaxCol;
    NI_U32 u32MaxRow;
    NI_U8   bAomId;
}VD_AOM_Frame;

static pthread_t s_astTestCommVdGetDataPid[VD_MAX_CHN_NUM];
static pthread_t s_astTestCommGetVdParaPid[VD_MAX_CHN_NUM];

static  SDK_COMM_GETDATA_PARA_S s_astTestCommGetPara[VD_MAX_CHN_NUM];
static  SDK_COMM_GET_VD_PARA_S s_astTestCommGetVdPara[VD_MAX_CHN_NUM];

NI_S32  SAMPLE_VD_DrawSB(NI_U32 u32Row, NI_U32 u32Col, VD_AOM_Frame* pstVdFrameInfo)
{
    int sbWidth;
    int sbHeight;
    NI_U8 *pStartAddr = NULL;
    NI_U8 *pStart = NULL;
    
    sbWidth = pstVdFrameInfo->u32Width / (pstVdFrameInfo->u32MaxCol);
    sbHeight = pstVdFrameInfo->u32Height / (pstVdFrameInfo->u32MaxRow);

    int y = u32Row * sbHeight;
    int x = u32Col * sbWidth;
    int i = 0;
    
    if ((x + sbWidth) >= pstVdFrameInfo->u32Width)
    {
        sbWidth = pstVdFrameInfo->u32Width - x - 1;
    }
    if ((y + sbHeight) >= pstVdFrameInfo->u32Height)
    {
        sbHeight = pstVdFrameInfo->u32Height - y - 1;
    }
    
    //get win start    
    pStartAddr = pstVdFrameInfo->pu8VirtAddr[0] + y * pstVdFrameInfo->u32LineOffset + x; 
    
    pStart = pStartAddr;
    //draw v line
    for(i = 0; i < sbHeight; i++)
    {   
        *pStart = 0xff;
        pStart[sbWidth] = 0xff;
   
        pStart += pstVdFrameInfo->u32LineOffset;
    }
    
    memset(pStartAddr, 0xff, sbWidth);
    memset(pStartAddr + sbHeight * pstVdFrameInfo->u32LineOffset, 0xff, sbWidth);
    
    return NI_SUCCESS;
}

NI_S32  SAMPLE_VD_DrawFrame(MD_RESULT_S *pstData, VD_AOM_Frame *pVdFrame)
{
    NI_U32 u32Col,u32Row;
    NI_U8 *pu8Data = NI_NULL;
    NI_U8 point = 0;
    NI_U8 u8AomId = pVdFrame->bAomId;
    
    pu8Data = (NI_U8 *)pstData->stMdData.astAomResult[0].pVirAddr;
    for(u32Row = 0 ; u32Row < pVdFrame->u32MaxRow; u32Row++)
    {
        for( u32Col = 0 ; u32Col < pVdFrame->u32MaxCol; u32Col += 2 )
        {   
            
            point = *pu8Data++;
            if((point >> u8AomId) & 0x01)
            {
                 SAMPLE_VD_DrawSB(u32Row, u32Col, pVdFrame);
            }

            if((point >> u8AomId) & 0x10)
            {
                 SAMPLE_VD_DrawSB(u32Row, u32Col+1, pVdFrame);
            }
        }
    }
    return NI_SUCCESS;
}


NI_VOID*  SAMPLE_VD_GetDrawDataProc(NI_VOID *p)
{
#if 0
    SDK_COMM_GET_VD_PARA_S *pstPara; 
    
    NI_U32 u32Timeout;
    VPS_GRP   VpsGrp;
    VD_CHN VdChn;
    NI_U32 u32Width;
    NI_U32 u32Height;
    NI_U32 u32ImgSize;
	
    NI_S32 s32Ret;
    NI_U32 u32Cnt = 0;

    /*vd data*/
    MD_RESULT_S stVdData;
    NI_U8 u8AomId;

   /*ias chn*/
    VPS_CHN_E VpsChn = VPS_CHN_IAS0;
    VD_AOM_Frame stVdFrame;

    /*output file, 1 aom 1 file*/
    FILE     *pOutFile;
    NI_CHAR aszFileName[64];
    char szFilePostfix[10];
    strcpy(szFilePostfix,".yuv");  
    
    VPS_FRAME_INFO_S *pstVideoFrame = NI_NULL;
    pstVideoFrame = (VPS_FRAME_INFO_S *)malloc(sizeof(VPS_FRAME_INFO_S));
    if (NULL == pstVideoFrame)
    {
        NI_ERROR("err malloc ias data buf fail!\n");
        return NULL;
    }
    
    pstPara = ( SDK_COMM_GET_VD_PARA_S*)p;
    VpsGrp = pstPara->VpsGrp;
    VdChn = pstPara->VdChn;
    u32Width = pstPara->u32Width; 
    u32Height = pstPara->u32Height; 
    u32Timeout = pstPara->u32TimeOut;
    u8AomId = pstPara->u8AomId;

    stVdFrame.u32Width = u32Width;
    stVdFrame.u32Height = u32Height;
    stVdFrame.u32LineOffset = u32Width;
    stVdFrame.u32MaxCol = pstPara->u32SBColNum;
    stVdFrame.u32MaxRow= pstPara->u32SBRowNum;
    stVdFrame.bAomId = u8AomId;

    sprintf(aszFileName, "vd_result_data_chn%d_%dx%d_aom%d%s", VdChn, stVdFrame.u32Width, stVdFrame.u32Height, u8AomId, szFilePostfix);
    pOutFile = fopen(aszFileName, "wb");
    if(NULL == pOutFile)
    {
        NI_ERROR("Open %s Failed!\n", aszFileName);
        free(pstVideoFrame);
        pstVideoFrame = NULL;
        return NULL;
    }
    
    /*yuv420 1.5 倍  422  2倍*/
    u32ImgSize =  u32Width * u32Height * 3 / 2;

    while (NI_TRUE == pstPara->bThreadStart)
    {  
        memset(pstVideoFrame, 0, sizeof(VPS_FRAME_INFO_S));
        memset(&stVdData, 0, sizeof(MD_DATA_S));
        
	    s32Ret = NI_MDK_VPS_GetFrame(VpsGrp, VpsChn, pstVideoFrame, u32Timeout);
	    if ( NI_SUCCESS != s32Ret )
        {
            if (NI_ERR_VPS_GET_STREAM_TIMEOUT == s32Ret)
            {
                continue;
            }
       
            NI_ERROR("NI_MDK_VPS_GetFrame failed with:0x%x\n", s32Ret);
            break;
        }
        
        s32Ret = NI_MDK_MD_DetectFrame(VdChn, &pstVideoFrame->astVideoFrame[1], &stVdData, u32Timeout);
        //s32Ret = NI_MDK_VD_GetData(VdChn, &stVdData, u32Timeout);
        if (NI_SUCCESS != s32Ret)
        {   
            fwrite(pstVideoFrame->astVideoFrame[0].apVirAddr[0], u32ImgSize, 1, pOutFile);
            fflush(pOutFile);
            
            s32Ret = NI_MDK_VPS_ReleaseFrame(VpsGrp, VpsChn, pstVideoFrame);           

            if (s32Ret != NI_SUCCESS)
            {
                NI_ERROR("NI_MDK_VD_GetData failed with: %d\n", s32Ret);
                break;
            }
            
            continue;
        }
        
        if(!stVdData.bMdDataExist || !stVdData.stMdData.bAomResultValid)
        {
            NI_INFO("No Md Data find in VdData!\n");
            fwrite(pstVideoFrame->astVideoFrame[0].apVirAddr[0], u32ImgSize, 1, pOutFile);
            fflush(pOutFile);
            
            s32Ret = NI_MDK_VPS_ReleaseFrame(VpsGrp, VpsChn, pstVideoFrame);
            s32Ret = NI_MDK_VD_ReleaseData(VdChn, &stVdData);
            continue;
        }

    	stVdFrame.pu8VirtAddr[0] = (NI_U8*)(pstVideoFrame->astVideoFrame[0].apVirAddr[0]);
        
        /*动检sb 画框*/
         SAMPLE_VD_DrawFrame(&stVdData, &stVdFrame);
        
        s32Ret = NI_MDK_VD_ReleaseData(VdChn, &stVdData);
        if (NI_SUCCESS != s32Ret)
        {
            
            NI_ERROR("NI_MDK_VD_ReleaseData failed with %#x!\n", s32Ret);
            
            break;
        }
        s32Ret = NI_MDK_VPS_ReleaseFrame(VpsGrp, VpsChn, pstVideoFrame);
        if (NI_SUCCESS != s32Ret)
        {           
            NI_ERROR("NI_MDK_VPS_ReleaseFrame failed with %#x!\n", s32Ret);            
            break;
        }
        fwrite(pstVideoFrame->astVideoFrame[0].apVirAddr[0], u32ImgSize, 1, pOutFile);
        fflush(pOutFile);
        
        printf("------u32Cnt = %d\n", u32Cnt++);
    }

    fclose(pOutFile);
    
    free(pstVideoFrame);
    pstVideoFrame = NULL;
    pstPara->bThreadStart = NI_FALSE;
    
    printf("Exit %s ok!\n", __FUNCTION__);
#endif
    return NI_NULL;
}



NI_VOID * SDK_COMM_VD_GetVdDataProc(NI_VOID *p)
{
    // SDK_COMM_GETDATA_PARA_S *pstPara;   
    // NI_U32 u32Timeout;
    // VD_CHN VdChn;
    // VD_DATA_S stVdData;
    // NI_U32 u32Cnt = 0;

    // pstPara = ( SDK_COMM_GETDATA_PARA_S*)p;
    // VdChn = pstPara->VdChn;
    // u32Timeout = pstPara->u32Timeout;
 
    // while (NI_TRUE == pstPara->bThreadStart)
    // {    
    //     if(NI_SUCCESS == NI_MDK_VD_GetData(VdChn, &stVdData, u32Timeout))
    //     {
    //         u32Cnt++;
    //         printf("------------------u32Cnt = %d\n", u32Cnt);
    //         printf("bBdDataExist: %d, bMdDataExist: %d\n", stVdData.bBdDataExist, stVdData.bMdDataExist);
            
    //         if (stVdData.bMdDataExist)
    //         {
    //             printf("stAomResult: %d, %d, %d, %d\n",
    //                 stVdData.stMdData.abAomMDResult[0],
    //                 stVdData.stMdData.abAomMDResult[1],
    //                 stVdData.stMdData.abAomMDResult[2],
    //                 stVdData.stMdData.abAomMDResult[3]);
    //         }
            
    //         NI_MDK_VD_ReleaseData(VdChn, &stVdData);
    //     }
    // }

    // pstPara->bThreadStart = NI_FALSE;
    
    // printf("exit %s ok!\n", __FUNCTION__);

    return NI_NULL;
}


NI_S32  SDK_COMM_VD_StartGetData(VD_CHN VdChn, NI_U32 u32Width, NI_U32 u32Height)
{
    if(NI_TRUE == s_astTestCommGetPara[VdChn].bThreadStart)
    {
        NI_WARN("chn:%d, get stream thread is already start.\n", VdChn);
        return NI_SUCCESS;
    }
    
    s_astTestCommGetPara[VdChn].bThreadStart = NI_TRUE;
    s_astTestCommGetPara[VdChn].VdChn = VdChn; 
    s_astTestCommGetPara[VdChn].u32Width = u32Width;
    s_astTestCommGetPara[VdChn].u32Height = u32Height; 
    s_astTestCommGetPara[VdChn].u32Timeout = 1000;

    return pthread_create(&s_astTestCommVdGetDataPid[VdChn], 0, \
         SDK_COMM_VD_GetVdDataProc, (NI_VOID*)&s_astTestCommGetPara[VdChn]);
    
}

NI_S32  SDK_COMM_VD_StopGetData(VD_CHN VdChn)
{
    if (NI_TRUE == s_astTestCommGetPara[VdChn].bThreadStart)
    {
        s_astTestCommGetPara[VdChn].bThreadStart = NI_FALSE;
        pthread_join(s_astTestCommVdGetDataPid[VdChn], 0);
    }
    
    return NI_SUCCESS;
}

NI_S32  SDK_COMM_VD_StartGetDrawData(VD_CHN VdChn, NI_U8 u8AomId)
{
#if 0	
    if (NI_TRUE == s_astTestCommGetVdPara[VdChn].bThreadStart)
    {
        NI_WARN("chn:%d, get vd data thread is already start.\n", VdChn);
        return NI_SUCCESS;
    }

    VPS_GRP VpsGrp = VdChn;
    MD_CHN_ATTR_S stVdChnAttr;
    
    /*获取vdchn 的通道属性*/
    if (NI_SUCCESS != NI_MDK_VD_GetChnAttr(VdChn, &stVdChnAttr))
    {
        NI_ERROR("VdChn: %d, Can not get Chn Attribute!", VdChn);
        return NI_FAILURE;
    }
    
    s_astTestCommGetVdPara[VdChn].VpsGrp = VpsGrp;
    s_astTestCommGetVdPara[VdChn].VdChn = VdChn;
    s_astTestCommGetVdPara[VdChn].u32Width= stVdChnAttr.u32Width;
    s_astTestCommGetVdPara[VdChn].u32Height= stVdChnAttr.u32Height;
    s_astTestCommGetVdPara[VdChn].u32SBColNum = stVdChnAttr.stMdAttr.stSbParam.u8SbHorizontalNum;
    s_astTestCommGetVdPara[VdChn].u32SBRowNum = stVdChnAttr.stMdAttr.stSbParam.u8SbVerticalNum;
	
    s_astTestCommGetVdPara[VdChn].bThreadStart = NI_TRUE;
    s_astTestCommGetVdPara[VdChn].u32TimeOut = 1000;//NI_TIMEOUT_FOREVER;//1000;
    s_astTestCommGetVdPara[VdChn].u8AomId = u8AomId;
	
    return pthread_create(&(s_astTestCommGetVdParaPid[VdChn]), 0,\
                           SAMPLE_VD_GetDrawDataProc, (NI_VOID*)&s_astTestCommGetVdPara[VdChn]);
#endif
    return NI_FAILURE;
}

NI_S32  SDK_COMM_VD_StopGetDrawData(VD_CHN VdChn)
{
    if (NI_TRUE == s_astTestCommGetVdPara[VdChn].bThreadStart)
    {
        s_astTestCommGetVdPara[VdChn].bThreadStart = NI_FALSE;
        pthread_join(s_astTestCommGetVdParaPid[VdChn], 0);
    }
	
    return NI_SUCCESS;
}