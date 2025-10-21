/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _HAL_REG_H_
#define _HAL_REG_H_

#include "ni_common_video.h"
#include "ni_comm_aisp.h"
#include "mdk_aispalg.h"
#include "ni_comm_aispalg.h"
#include "ni_aisp_debug.h"
#include "plat_hal_vreg.h"
#include "list.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum tagD2nChgStep {
    D2N_CHG_CFG   = 5, /* 切换配置和颜色 */
    D2N_CHG_IRCUT = 2, /* 等3帧切换ircut */
    D2N_CHG_LIGHT = 1, /* 开灯 */
} D2N_CHG_STEP_E;

typedef enum tagN2dChgStep {
    N2D_CHG_LIGHT = 5, /* 关灯 */
    N2D_CHG_IRCUT = 4, /* 切IRCUT */
    N2D_CHG_CFG   = 3, /* 切换配置和颜色 */
} N2D_CHG_STEP_E;

typedef struct tagAdaptWebInfo {
    AISP_APP_GAMMA_S       stGamma;
    AISP_APP_COLOR_S       stColor;
    AISP_APP_BACKLIGHT_S   stBackLight;
    AISP_APP_DAY_NIGHT_S   stDayNight;
    AISP_APP_LIGHT_S       stSmartLight;
    AISP_APP_NR_S          stNr;
    AISP_APP_EE_S          stEe;
    AISP_APP_EXP_S         stExp;
    AISP_APP_WB_S          stWb;
    AISP_IMG_STYLE_E       enStyle;
    AISP_AFPS_CTRL_S       stAutoFps;
    AISP_APP_DEFOG_S       stDefog;
    AISP_APP_FAST_AE_EXP_S stFastAeExp;
} ADAPT_WEB_INFO_S;

typedef union tagAdaptStatFlag {
    NI_U32 au32Status[1];
    struct
    {
        NI_U32 bForceSync : 1; /* [0] */
        NI_U32 bAeFirst : 1;   /* [1] */
        NI_U32 bCfgDbgEn : 1;  /* [2] */
        NI_U32 u32RunTime : 1; /* [3] aisp_run运行次数计数，只会有0/1两种状态，表示偶数次和奇数次 */
        NI_U32 bDnChange : 1;  /* [4] */

        NI_U32 u32SyncFrmCnt : 3; /* [5:7] 0 - 7 */

        NI_U32 u32UseCfgIndex : 4;  /* [8:11] 0 - 15 */
        NI_U32 u32N2dFlag : 3;      /* [12:14] 0 - 7 */
        NI_U32 u32D2nFlag : 3;      /* [15:17] 0 - 7 */
        NI_U32 u32DnEnv : 3;        /* [18:20] 0 - 7 */
        NI_U32 enDnResult : 3;      /* [21:23] 0 - 7 AISP_DN_MODE_E */
        NI_U32 bDebugPrintEn : 1;   /* [24] */
        NI_U32 bOpenAutoLight : 1;  /* [25] */
        NI_U32 bGetExpFromFile : 1; /* [26] 仿真需要, 从raw信息文件获取曝光信息替换AE结果 */
        NI_U32 bRunning : 1;        /* [27] */
        NI_U32 enCombMode : 3;      /* [28 - 30] 0 - 7 AISP_COMB_MODE_E */
    };
} ADAPT_STATE_FLAG_U;

typedef struct tagAdaptGainAdjInfo {
    NI_BOOL bGainAdjFlag; /*增益变化，模块需要调节标志*/
    NI_S32  s32Gain;      /*保存的gain*/
    NI_BOOL bMaxFlag;     /*使用最大*/
    NI_S32  s32Level;     /*插值的小端*/
    NI_S32  s32Slope;     /*差值的中间系数*/
} ADAPT_GAIN_ADJ_INFO_S;

typedef struct tagAdaptAutoFpsInfo {
    NI_U32 u32CalFps;  /* 计算出来的Fps */
    NI_U32 u32LastFps; /* 上一次Fps */
    NI_U32 u32Const;   /* 自动降帧延时计数 */

    NI_U32 u32ShtThr;  /* 快门时间调节阈值 */
    NI_U32 u32GainThr; /* 增益调节阈值 */
    NI_U32 u32ContNum; /* 延时计数值 */

} ADAPT_AUTO_FPS_INFO_S;

typedef struct tagAdaptVreg {
    /* 用户参数 */
    AISP_VIDEO_FMT_INFO_S stVideoFmt;
    AISP_CFG_FILE_INFO_S  stCfgFile;
    ADAPT_WEB_INFO_S      stWeb;
    AISP_MOD_RUN_U        unModRun;

    /* 内部状态信息 */
    ADAPT_STATE_FLAG_U unSateFlag;

    NI_U16 u16GainRDiff; /* RGain 差值 */
    NI_U32 u32MaxGain;   /* 曝光的最大增益 */
    NI_U32 u32MaxSht;    /* 曝光的最大增益 */

    NI_S32                s32EnvLum;
    AISP_SEN_EXP_S        stSenExp;    /* Sensor实际的曝光信息 */
    AWB_RESULT_S          stAwbResult; /* 白平衡结果 */
    AE_RESULT_S           stAeResult;  /* AE结果 */
    ADAPT_GAIN_ADJ_INFO_S stGainAdjInfo;

    ADAPT_AUTO_FPS_INFO_S u32AutoFpsInfo; /* AutoFps信息 */

    PLAT_ADAPT_VREG_S stPlatVreg;
} ADAPT_VREG_S;

NI_VOID ADAPT_CAL_InitWebApp(ADAPT_VREG_S *pstVreg);
NI_VOID ADAPT_CAL_InitVreg(ADAPT_VREG_S *pstVreg);

/* 平台相关的函数 */
NI_S32 ADAPT_ISP_GetHdrMode(ISP_DEV IspDev, HDR_MODE_E *penHdrMode);
NI_S32 ADAPT_CTRL_RunModules(ISP_DEV IspDev);
NI_S32 ADAPT_PLAT_CopyCfgToMem(ISP_DEV IspDev, NI_BOOL bEnable, NI_U32 *pu32CfgBuf);
NI_S32 ADAPT_PLAT_Init(ISP_DEV IspDev);
NI_S32 ADAPT_PLAT_CheckParam(ISP_DEV IspDev, AISP_INIT_PARAM_S *pstInitParam);
#if ((XSCHIP_ID == 0x71560100) || (XSCHIP_ID == 0x71561100))
NI_S32 ADAPT_PLAT_GetAispData(ISP_DEV IspDev, NI_U32 *pu32Addr);
NI_S32 ADAPT_PLAT_SetAispData(ISP_DEV IspDev, NI_U32 *pu32Addr);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
