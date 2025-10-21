/* 
 * sdk_comm_aisp.c
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <pthread.h>

#include "ni_type.h"
#include "ni_macro.h"
#include "ni_debug.h"
#include "ni_define.h"

#include "ni_sensor.h"
#include "mdk_sys.h"
#include "mdk_aisp.h"

#include "sdk_comm_aisp.h"
#include "TdCommon.h"
static NI_VOID* gapvAispCfgAddr[ISP_MAX_DEV_NUM] = {NI_NULL};
static NI_U32 gau32AispCfgPhyAddr[ISP_MAX_DEV_NUM] = {0};
static ISP_DEV gau32AispDev[ISP_MAX_DEV_NUM] = {0};
static pthread_t gastAispRunPid[ISP_MAX_DEV_NUM];

static NI_BOOL gabAispThreadRun[ISP_MAX_DEV_NUM] = {0};


static NI_S32 SenGetInfo(ISP_DEV IspDev, AISP_SEN_INFO_S *pstSenInfo)
{
    SEN_INFO_S stSenInfo;
    NI_U32 i;

    NI_SEN_GetCurInfo(IspDev, &stSenInfo); 
    pstSenInfo->u32MaxGain = stSenInfo.stSenExpInfo.stGainInfo.u32MaxGain;
    pstSenInfo->u32GainAcc = stSenInfo.stSenExpInfo.stGainInfo.u32GainAcc;
    pstSenInfo->u32LinePer500ms = stSenInfo.stSenExpInfo.stShtInfo.u32ExpLinePre500Ms;
    pstSenInfo->u32ExpLinePer1000Ms = stSenInfo.stSenExpInfo.stShtInfo.u32ExpLinePre1000Ms;
    for (i = 0; i < AISP_DOW_MAX_FRAME_NUM; i++)
    {
        pstSenInfo->au32MaxExpRatio[i] = stSenInfo.stSenExpInfo.stShtInfo.au32ExpHeight[i];
        pstSenInfo->au32MinExpRatio[i] = stSenInfo.stSenExpInfo.stShtInfo.au32MinExpLine[i];
    }
    pstSenInfo->u32MinSht = stSenInfo.stSenExpInfo.stShtInfo.au32MinExpLine[0];
    /* printf("get sen info max gain %d acc %d lineper500 %d, lineper1000 %d\n", \
         pstSenInfo->u32MaxGain, pstSenInfo->u32GainAcc, pstSenInfo->u32LinePer500ms, pstSenInfo->u32ExpLinePer1000Ms);*/

    return NI_SUCCESS;
}

static NI_S32 SenSetExp(ISP_DEV IspDev, AISP_SEN_EXP_S *pstSenExp)
{
    SEN_EXP_VALUE_S stExpValue;
    NI_U32 i;

    stExpValue.stGainValue.u32Gain = pstSenExp->u32Gain;
    stExpValue.stShtValue.u32FrameNum = pstSenExp->u32FrameNum;
    for ( i = 0 ; i < pstSenExp->u32FrameNum; i++ )
    {
        stExpValue.stShtValue.au32Sht[i] = pstSenExp->au32Sht[i];
    }

    if (0)
    {
        NI_INFO("sen ShtLine[0] %d ShtLine[1] %d s32SysGain %d frame %d\n", \
            stExpValue.stShtValue.au32Sht[0],
            stExpValue.stShtValue.au32Sht[1],
            stExpValue.stGainValue.u32Gain,
            stExpValue.stShtValue.u32FrameNum);
    }

    NI_SEN_SetExp(IspDev, &stExpValue);

    return NI_SUCCESS;
}

static NI_S32 SenSetFps(ISP_DEV IspDev, NI_U32 u32Fps, AISP_SET_FPS_MODE_E enMode)
{
    return NI_SEN_SetFps(IspDev, u32Fps, enMode);
}

static int SenGetExp(int IspDev, AISP_SEN_EXP_S *pstSenExp)
{
    SEN_EXP_VALUE_S stExpValue;
    uint32_t i;

    NI_SEN_GetExp(IspDev, &stExpValue);

    if (0)
    {
        printf("INFO: sen ShtLine[0] %d ShtLine[1] %d s32SysGain %d frame %d\n",
               stExpValue.stShtValue.au32Sht[0],
               stExpValue.stShtValue.au32Sht[1],
               stExpValue.stGainValue.u32Gain,
               stExpValue.stShtValue.u32FrameNum);
    }

    pstSenExp->u32Gain = stExpValue.stGainValue.u32Gain;
    pstSenExp->u32FrameNum = stExpValue.stShtValue.u32FrameNum;
    for (i = 0; i < pstSenExp->u32FrameNum; i++)
    {
        pstSenExp->au32Sht[i] = stExpValue.stShtValue.au32Sht[i];
    }

    return 0;
}

NI_VOID *SDK_COMMM_AISP_ThreadRun(NI_VOID *p)
{
    NI_S32 s32Ret;
    ISP_DEV IspDev;
    SENSOR_ID SenId = 0;
    SEN_INFO_S pstSenInfo;
    unsigned int err_num = 0;
    SEN_FMT_MATCH_KEY_S pstFmtMatchKey;

    VideoInDevice_p pVideoInDevice = &GlobalDevice.VideoInDevice;

    IspDev = *(ISP_DEV *)p;

    NI_INFO("start IspDev %d\n", IspDev);
    pthread_t pthread;
    pid_t pid;
    pid_t tid;
    pid = getpid();
    tid = syscall(SYS_gettid);
    pthread = pthread_self();

    PRINT_ERROR("AISP_ThreadRun( thread pid %d tid %d thread id 0x%x\n", pid, tid, pthread);
    while(gabAispThreadRun[IspDev])
    {

        s32Ret = NI_MDK_AISP_Run(IspDev);
        if (s32Ret != NI_SUCCESS)
        {
            err_num ++;
            PRINT_ERROR("isp run err!s32Ret:%#x\n", s32Ret);
        }
        else
        {
            err_num = 0;
        }

        // ISP异常超过5S后尝试恢复
        // ISP异常超过两分钟设备自动重启
        if (err_num >= 5 && err_num < 60 * 2)
        {
            PRINT_ERROR("Sensor error, Reset Sensor now\n");
            s32Ret = NI_SEN_GetCurInfo(SenId, &pstSenInfo);
            if (s32Ret != RETURN_OK)
            {
                PRINT_ERROR("Error NI_SEN_GetCurInfo ret = %x\n", s32Ret);
                continue;
            }

            s32Ret = NI_SEN_Reset(SenId);
            if (s32Ret != NI_SUCCESS)
            {
                NI_ERROR("call NI_SEN_Reset fail,ret=%#x\n", s32Ret);
                continue;
            }

            pstSenInfo.stSenFmtMatchKey.u32Fps = pVideoInDevice->VDInfo[0].ViDstFps * 1000;
            pstFmtMatchKey = pstSenInfo.stSenFmtMatchKey;
            s32Ret = NI_SEN_SetFormat(SenId, &pstFmtMatchKey);
            if (s32Ret != RETURN_OK)
            {
                PRINT_ERROR("Error NI_SEN_SetFormat ret = %x\n", s32Ret);
                continue;
            }
        }
        else if (err_num >= 60 * 2) 
        {
            PRINT_ERROR("Sensor error, System will reboot now\n");
            PRINT_ERROR("Sensor error, System will reboot now\n");
            PRINT_ERROR("Sensor error, System will reboot now\n");
            SystemAsh("reboot -f");
        }
 
        usleep(0);
        //usleep(1000);
    }

    NI_INFO("exit isp run thread:%d\n", IspDev);
    
    return NULL;
}

static NI_S32 AispInit(ISP_DEV IspDev, TEST_AISP_CONFIG_S *pstAispConfig)
{
    NI_S32 s32Ret = NI_SUCCESS;
    AISP_INIT_PARAM_S stInitParam; 
    NI_U32 u32Size;
    NI_CHAR cBuf[32];
    AISP_APP_BACKLIGHT_S stBlackLight;
    FILE *fp;
    SEN_DEV_ATTR_S stSenDevAttr;
    NI_CHAR aszAispCfgName[128];
    AISP_APP_DAY_NIGHT_S stDayNight;
        
    memset(&stInitParam, 0, sizeof(AISP_INIT_PARAM_S));

    if(TEST_AISP_MODE_SINGLE_CHN == pstAispConfig->enAispType)
    {
        stInitParam.enCombMode = AISP_MODE_SINGLE_CHN;
    }
    else if(TEST_AISP_MODE_TWO_CHN_COMB == pstAispConfig->enAispType)
    {
        stInitParam.enCombMode = AISP_MODE_TWO_CHN_COMB;
    }
    
    stInitParam.stVideoFmt.u32ImgWidth = pstAispConfig->u32Width;
    stInitParam.stVideoFmt.u32ImgHeight = pstAispConfig->u32Height;
    stInitParam.stVideoFmt.u32Fps = pstAispConfig->u32Fps * 1000;
    stInitParam.stRegisterFunc.pfn_sen_get_info = SenGetInfo;
    stInitParam.stRegisterFunc.pfn_sen_set_exp = SenSetExp;
    stInitParam.stRegisterFunc.pfn_sen_get_exp = SenGetExp;
    stInitParam.stRegisterFunc.pfn_sen_set_ir_cut = ircut;
    stInitParam.stRegisterFunc.pfn_sen_set_led_light = NI_NULL;
    stInitParam.stRegisterFunc.pfn_sen_set_iris = NI_NULL;
    stInitParam.stRegisterFunc.pfn_sen_set_ir_light = ir_light;
    stInitParam.stRegisterFunc.pfn_sen_get_photosense = NI_NULL;
    stInitParam.stRegisterFunc.pfn_sen_set_fps = SenSetFps;
    stInitParam.stRegisterFunc.pfn_sen_set_vts = NI_NULL;

    s32Ret = NI_SEN_GetSenIdAttr(IspDev, &stSenDevAttr);
    if (s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("call NI_SEN_GetSenIdAttr fail, ret=%#x\n", s32Ret);
        return s32Ret;
    }

    memset(aszAispCfgName, 0, sizeof(aszAispCfgName));

    //printf("sensor id %d\n", stSenDevAttr.enSenType);
    if (SEN_TYPE_IMX307 == stSenDevAttr.enSenType)
    {
        sprintf(aszAispCfgName, "aisp_cfg_imx307.bin");
    }
    else if (SEN_TYPE_OS04A10 == stSenDevAttr.enSenType)
    {
        sprintf(aszAispCfgName, "aisp_cfg_os04a10.bin");
    }
    else if (SEN_TYPE_OS05A10 == stSenDevAttr.enSenType)
    {
        sprintf(aszAispCfgName, "aisp_cfg_os05a10.bin");
    }
    else if (SEN_TYPE_OS04C10 == stSenDevAttr.enSenType)
    {
        sprintf(aszAispCfgName, "aisp_cfg_os04c10.bin");
    }
    else if (SEN_TYPE_SC200AI == stSenDevAttr.enSenType)
    {
        sprintf(aszAispCfgName, "aisp_cfg_sc200ai.bin");
    }
    else if (SEN_TYPE_SC2239P == stSenDevAttr.enSenType)
    {
        sprintf(aszAispCfgName, "/opt/app/aisp_cfg_sc2239p_base.bin");
    }
    else if (SEN_TYPE_GC4023 == stSenDevAttr.enSenType)
    {
        sprintf(aszAispCfgName, "/opt/app/aisp_cfg_gc4023.bin");
    }
    else if (SEN_TYPE_OS04L10 == stSenDevAttr.enSenType)
    {
        sprintf(aszAispCfgName, "/opt/app/aisp_cfg_OS04L10.bin");
    }
    else if (SEN_TYPE_OS04D10 == stSenDevAttr.enSenType)
    {
        sprintf(aszAispCfgName, "/opt/app/aisp_cfg_OS04D10.bin");
    }
    else if (SEN_TYPE_GC4653 == stSenDevAttr.enSenType)
    {
        sprintf(aszAispCfgName, "/opt/app/aisp_cfg_GC4653.bin");
    }
    else if (SEN_TYPE_GC2083 == stSenDevAttr.enSenType)
    {
        sprintf(aszAispCfgName, "/opt/app/aisp_cfg_GC2083.bin");
    }
    else
    {
        sprintf(aszAispCfgName, "/opt/app/aisp_cfg.bin");
    }

    printf("cfg file name %s\n", aszAispCfgName);

    fp = fopen(aszAispCfgName, "rb");
    if (NI_NULL == fp)
    {
        NI_ERROR("open file error\n");
        return NI_FAILURE;
    }

    fseek(fp, 0, SEEK_END);
    u32Size = ftell(fp);
    rewind(fp);

    sprintf(cBuf, "aisp_cfg%d", IspDev);
    s32Ret = NI_MDK_SYS_MmzAlloc(&gau32AispCfgPhyAddr[IspDev], &gapvAispCfgAddr[IspDev], u32Size, cBuf, NI_NULL);
    if(s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("NI_MDK_SYS_MmzAlloc fail ret is %#X", s32Ret);
        return NI_FAILURE;
    }
    fread(gapvAispCfgAddr[IspDev], u32Size, 1, fp);
    fclose(fp);
    stInitParam.stCfgFile.pCfgFile = gapvAispCfgAddr[IspDev];
    stInitParam.stCfgFile.u32CfgLen = u32Size;
    stInitParam.stCfgFile.u32PhyAddr = gau32AispCfgPhyAddr[IspDev];

    s32Ret = NI_MDK_AISP_Init(IspDev, &stInitParam);
    if(s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("NI_MDK_AISP_Init %#X\n", s32Ret);
        return NI_FAILURE;
    }

    if (HDR_MODE_2To1_LINE == pstAispConfig->enHdrMod)
    {
        stBlackLight.enBackLightMode = AISP_BACKLIGHT_MODE_WDR;
        stBlackLight.u8Level = 50;
        NI_MDK_AISP_SetBackLight(IspDev, &stBlackLight);
    }
    else
    {
        stBlackLight.enBackLightMode = AISP_BACKLIGHT_MODE_NONE;
        stBlackLight.u8Level = 50;
        NI_MDK_AISP_SetBackLight(IspDev, &stBlackLight);
    }

    stDayNight.enDayNight = AISP_DN_MODE_DAY;
    stDayNight.enDnMethod = AISP_DN_METHOD_SOFT;
    stDayNight.stDnAttr.u16D2nValue = 49;
    stDayNight.stDnAttr.u16N2dValue = 50;
    stDayNight.stDnAttr.u16Delay = 20;
    stDayNight.stDnAttr.u32D2nGainThr = 36000;
    stDayNight.stDnAttr.u32N2dGainThr = 26000;
    s32Ret = NI_MDK_AISP_SetDayNight(IspDev, &stDayNight);
    if(s32Ret != NI_SUCCESS)
    {
        NI_ERROR("NI_MDK_AISP_SetDayNight %d\n", s32Ret);
        return NI_FAILURE;
    }
        
    return NI_SUCCESS;
}


NI_S32 SDK_COMM_AISP_Init(ISP_DEV IspDev, TEST_AISP_CONFIG_S *pstAispConfig)
{
    NI_S32 s32Ret = NI_FAILURE;
    
    s32Ret = AispInit(IspDev, pstAispConfig);
    if(s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("IspDev %d SDK_COMM_AISP_Init->AispInit %d\n", IspDev, s32Ret);
        return NI_FAILURE;
    }

    gabAispThreadRun[IspDev] = NI_TRUE;
    gau32AispDev[IspDev] = IspDev;

    pthread_create(&gastAispRunPid[IspDev], 0, SDK_COMMM_AISP_ThreadRun, (NI_VOID*)&gau32AispDev[IspDev]);
    
    return NI_SUCCESS;
}

NI_S32 SDK_COMM_AISP_DeInit(ISP_DEV IspDev)
{
    NI_S32 s32Ret = -1;
    
    gabAispThreadRun[IspDev] = NI_FALSE;
    pthread_join(gastAispRunPid[IspDev], 0);
    s32Ret = NI_MDK_AISP_DeInit(IspDev);
    if (s32Ret != NI_SUCCESS)
    {
        NI_ERROR("IspDev %d NI_MDK_AISP_DeInit err %#x\n", IspDev, s32Ret);
        return s32Ret;
    }

    NI_MDK_SYS_MmzFree(gau32AispCfgPhyAddr[IspDev], gapvAispCfgAddr[IspDev]);
    
    return  s32Ret;
}
