/* 
 * sdk_comm_aisp.h -- aisp sample用例头文件 
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */
 
#ifndef SDK_COMM_AISP_H
#define SDK_COMM_AISP_H
#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#include "ni_type.h"
#include "ni_common.h"
#include "ni_define.h"

#include "ni_comm_aisp.h"


/*aisp功能选择*/
typedef enum tagTestAispType
{
    TEST_AISP_MODE_SINGLE_CHN,          //各通道独立
    TEST_AISP_MODE_TWO_CHN_COMB,        //两通道融合

}TEST_AISP_TYPE_E;

typedef struct tagTestAispConfig
{
    NI_U32                      u32Width;
    NI_U32                      u32Height;
    NI_U32                      u32Fps;
    HDR_MODE_E                  enHdrMod;
    TEST_AISP_TYPE_E            enAispType;
}TEST_AISP_CONFIG_S;

/*2A初始化*/
NI_S32 SDK_COMM_AISP_Init(ISP_DEV IspDev, TEST_AISP_CONFIG_S *pstAispConfig);
/*2A去初始化*/
NI_S32 SDK_COMM_AISP_DeInit(ISP_DEV IspDev);

#ifdef __cplusplus
}
#endif

#endif	// SDK_COMM_AISP_H