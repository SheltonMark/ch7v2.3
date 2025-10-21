/* 
 * sdk_comm_sensor.c -- sensor sample用例 
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
     
#include "ni_type.h"

#include "sdk_comm.h"

NI_S32 SDK_COMM_SEN_Start(SENSOR_ID SenId, TEST_SEN_CONFIG_S *pstSenCfg)
{
    NI_S32 s32Ret;
    SEN_DEV_ATTR_S stSenDevAttr;
    SEN_FMT_MATCH_KEY_S stSenFmtMatchKey;
    
    stSenDevAttr.enSenType = pstSenCfg->enSenType;
    stSenDevAttr.u32DevAddr = pstSenCfg->u32DevAddr;
    stSenDevAttr.u32IicDevId = pstSenCfg->u32I2cDevId;
    stSenDevAttr.u32RstGpioId = pstSenCfg->u32RstGpioId;

    s32Ret = NI_SEN_SetSenIdAttr(SenId, &stSenDevAttr);
    if (s32Ret != NI_SUCCESS)
    {
        NI_ERROR("call NI_SEN_SetSenIdAttr fail,ret=%#x\n", s32Ret);
        return s32Ret;
    }

    switch (pstSenCfg->enHdrMode)
    {
        case HDR_MODE_NONE:
            stSenFmtMatchKey.enHdrMode = SEN_HDR_MODE_NONE;
            break;
        case HDR_MODE_BUILT_IN:
            stSenFmtMatchKey.enHdrMode = SEN_HDR_MODE_BUILD_IN;
            break;
        case HDR_MODE_2To1_FRAME:
            stSenFmtMatchKey.enHdrMode = SEN_HDR_MODE_2_FRAME;
            break;
        case HDR_MODE_2To1_LINE:
            stSenFmtMatchKey.enHdrMode = SEN_HDR_MODE_2_LINE;
            break;
        case HDR_MODE_3To1_FRAME:
            stSenFmtMatchKey.enHdrMode = SEN_HDR_MODE_3_FRAME;
            break;
        case HDR_MODE_3To1_LINE:
            stSenFmtMatchKey.enHdrMode = SEN_HDR_MODE_3_LINE;
            break;
        default: 
            stSenFmtMatchKey.enHdrMode = SEN_HDR_MODE_NONE;
            break;
    }
        
    s32Ret = NI_SEN_Reset(SenId);
    if (s32Ret != NI_SUCCESS)
    {
        NI_ERROR("call NI_SEN_Reset fail,ret=%#x\n", s32Ret);
        return s32Ret;
    }
  
    stSenFmtMatchKey.enWorkMode = SEN_WORK_MODE_MASTER;
    stSenFmtMatchKey.enTransMode = (VI_INTF_TYPE_MIPI == pstSenCfg->enIntfType) ? SEN_TRANS_MODE_MIPI : SEN_TRANS_MODE_PARALLEL;
    //stSenFmtMatchKey.enHdrMode = stSenFmtMatchKey.enHdrMode;
    stSenFmtMatchKey.u32LaneNum = pstSenCfg->u32LaneNum;
    stSenFmtMatchKey.u32CfgId = 0;
    stSenFmtMatchKey.u32Fps = pstSenCfg->u32Fps * 1000;
    stSenFmtMatchKey.u32Height = pstSenCfg->u32Height;
    stSenFmtMatchKey.u32Width = pstSenCfg->u32Width;

    s32Ret = NI_SEN_SetFormat(SenId, &stSenFmtMatchKey);
    if (s32Ret != NI_SUCCESS)
    {
        NI_ERROR("call NI_SEN_SetFormat fail,ret=%#x\n", s32Ret);
        return s32Ret;
    }

    return NI_SUCCESS;
}

