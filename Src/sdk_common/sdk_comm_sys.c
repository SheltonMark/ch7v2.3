/* 
 * sdk_comm_sys.c -- sys sample用例 
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "mdk_sys.h"
#include "mdk_mbuf.h"
#include "mdk_pm.h"
#include "ni_debug.h"
#include "PrintGrade.h"

NI_S32 SDK_COMM_SYS_Init()
{
    NI_S32 s32Ret;
    MBUF_CONF_S stMbufConf;

    NI_MDK_SYS_Exit();
    NI_MDK_MBUF_Exit();

    /* VPS 使用私有缓存池时可不用配置公共缓冲池 */
    memset(&stMbufConf, 0, sizeof(MBUF_CONF_S));
    stMbufConf.u32MaxPoolCnt = 128;
    
    stMbufConf.astCommPool[0].u32BlkCnt = 0;
    stMbufConf.astCommPool[0].u32BlkSize = 3840*2160*2;
    strcpy(stMbufConf.astCommPool[0].acMmzName, "anonymous");
    
    stMbufConf.astCommPool[1].u32BlkCnt = 0;
    stMbufConf.astCommPool[1].u32BlkSize = 1920*1088*2;
    strcpy(stMbufConf.astCommPool[1].acMmzName, "anonymous");

    stMbufConf.astCommPool[2].u32BlkCnt = 0;
    stMbufConf.astCommPool[2].u32BlkSize = 1280*720*2;
    strcpy(stMbufConf.astCommPool[2].acMmzName, "anonymous"); 
#if 0
    stMbufConf.astCommPool[3].u32BlkCnt = 0;
    stMbufConf.astCommPool[3].u32BlkSize = 720*576*2;
    strcpy(stMbufConf.astCommPool[3].acMmzName, "anonymous"); 
#endif
    s32Ret = NI_MDK_MBUF_SetConf(&stMbufConf);
    if (s32Ret != NI_SUCCESS)
    {
        NI_ERROR("call NI_MDK_MBUF_SetConf err %#x\n", s32Ret);
        return s32Ret;
    }
    
    s32Ret = NI_MDK_MBUF_Init();
    if (s32Ret != NI_SUCCESS)
    {
        NI_ERROR("call NI_MDK_MBUF_Init err %#x\n", s32Ret);
        return s32Ret;
    }
    
    s32Ret = NI_MDK_SYS_Init();
    if (s32Ret != NI_SUCCESS)
    {
        NI_ERROR("call NI_MDK_SYS_Init err %#x\n", s32Ret);
        return s32Ret;
    }
    
    return NI_SUCCESS;
}

NI_S32 SDK_COMM_SYS_Exit()
{
    NI_MDK_SYS_Exit();
    NI_MDK_MBUF_Exit();

    return NI_SUCCESS;
}

