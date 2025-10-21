/* 
 * sdk_comm_isp.c -- isp sample用例 
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "ni_comm_pisp.h"
#include "mdk_pisp.h"
#include "mdk_sys.h"
#include "sdk_comm_aisp.h"
#include "sdk_comm.h"

#include "mdk_aisp.h"
#include "ni_comm_aisp.h"
#include "ni_sensor.h"

static TEST_COMM_ISP_THREAD_PARA_S gastIspPara[ISP_MAX_DEV_NUM];


NI_S32 SDK_COMM_ISP_Start(ISP_DEV IspDev, TEST_ISP_CONFIG_S *pstIspConfig, NI_BOOL bRunThread2A, NI_BOOL bHdrMode)
{
    NI_S32 s32Ret = NI_SUCCESS;
    PISP_PUB_ATTR_S stPubAttr = {0};
    PISP_HDR_MODE_S stHdrMode = {0};
    TEST_AISP_CONFIG_S stAispConfig = {0};
    
    stPubAttr.stWndRect.s32X = 0;
    stPubAttr.stWndRect.s32Y = 0;
    stPubAttr.stWndRect.u32Width = pstIspConfig->u32Width;
    stPubAttr.stWndRect.u32Height = pstIspConfig->u32Height;
    stPubAttr.u32FrameRate = pstIspConfig->u32Fps;

    s32Ret = NI_MDK_PISP_SetPubAttr(IspDev, &stPubAttr);
    if(s32Ret != NI_SUCCESS)
    {
        NI_ERROR("NI_MDK_ISP_SetPubAttr %d\n", s32Ret);
        return NI_FAILURE;
    }

    s32Ret =NI_MDK_PISP_Init(IspDev);
    if(s32Ret != NI_SUCCESS)
    {
        NI_ERROR("NI_MDK_ISP_Init %d\n", s32Ret);
        return NI_FAILURE;
    }

    stHdrMode.enHdrMode = (bHdrMode ? HDR_MODE_2To1_LINE : HDR_MODE_NONE);
    s32Ret = NI_MDK_PISP_SetHdrMode(IspDev, &stHdrMode);
    if(s32Ret != NI_SUCCESS)
    {
        NI_ERROR("NI_MDK_ISP_SetHdrMode %d\n", s32Ret);
        return NI_FAILURE;
    }
    
    stAispConfig.u32Height = pstIspConfig->u32Height;
    stAispConfig.u32Width = pstIspConfig->u32Width;
    stAispConfig.u32Fps = pstIspConfig->u32Fps;
    stAispConfig.enHdrMod = (bHdrMode ? HDR_MODE_2To1_LINE : HDR_MODE_NONE);
    stAispConfig.enAispType = TEST_AISP_MODE_SINGLE_CHN;

    if (NI_TRUE == bRunThread2A)
    {    
        SDK_COMM_AISP_Init(IspDev, &stAispConfig);
        gastIspPara[IspDev].bThreadStart = NI_TRUE;
        gastIspPara[IspDev].IspDev = IspDev;
    }
    else
    {
        gastIspPara[IspDev].bThreadStart = NI_FALSE;
    }

    return NI_SUCCESS;
}


NI_S32 SDK_COMM_ISP_Stop(ISP_DEV IspDev)
{
    if(NI_TRUE == gastIspPara[IspDev].bThreadStart)
    {
        gastIspPara[IspDev].bThreadStart = NI_FALSE;
        SDK_COMM_AISP_DeInit(IspDev);
    }

    NI_MDK_PISP_Exit(IspDev);
    
    return NI_SUCCESS;
}



