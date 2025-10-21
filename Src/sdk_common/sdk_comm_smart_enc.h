/* 
 * sdk_comm_smart_enc.h -- smart_enc sample用例头文件 
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */


#ifndef _SDK_COMM_SMART_ENC_H_
#define _SDK_COMM_SMART_ENC_H_

#include "mdk_sys.h"
#include "mdk_mbuf.h"
#include "mdk_venc.h"
#include "mdk_smart_enc.h"



#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */
#define SAMPLE_VENC_MAX_STREAM_CHN      (VENC_MAX_CHN_NUM)

#define EXPECT_EQ(expected, actual) \
do{                                   \
    if (expected != actual) {                    \
        printf("\033[0;32;31m" "\nexpected failed at:\n" \
              "  >Function : %s\n"    \
              "  >Line No. : %d\n"    \
              "  >Condition: %s!=%s\n" "\033[m",   \
                __FUNCTION__, __LINE__, #expected, #actual);\
    } \
}while(0)

/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */

typedef struct tagSampleCommStreamChn
{
    VENC_CHN   VeChn;
}SDK_COMM_STREAM_CHN_S;

typedef struct tagSampleCommGetStream
{
    NI_BOOL                 bThreadStart;
    SDK_COMM_STREAM_CHN_S  astStreamChn[SAMPLE_VENC_MAX_STREAM_CHN];
    NI_U32                  u32ValidChnCnt;
	
    VENC_CHN                VeChn;
	
    NI_U32                  u32Timeout;
    NI_U32                  au32GetFrmNum[SAMPLE_VENC_MAX_STREAM_CHN]; /* 已经接收的图像帧数*/
    NI_BOOL                 abUserSensd[SAMPLE_VENC_MAX_STREAM_CHN]; 
}SDK_COMM_GETSTREAM_PARA_S;

/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */
NI_S32 SAMPLE__SMART_ENC_StartSendStream(VENC_CHN VeChn, PAYLOAD_TYPE_E enType, VENC_RC_MODE_E enRcMode, NI_CHAR *pcFileName);

NI_S32 SAMPLE__SMART_ENC_StopSendStream(VENC_CHN VeChn);

NI_S32 SAMPLE__SMART_ENC_StartGetStream(VENC_CHN VeChn);

NI_S32 SAMPLE__SMART_ENC_StopGetStream(VENC_CHN VeChn);

NI_S32 SAMPLE__SMART_ENC_Stop(VENC_CHN VeChn);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif //_SDK_COMM_SMART_ENC_H_

