/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NI_AISP_DEFINE_H_
#define _NI_AISP_DEFINE_H_

#include "ni_type.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define AISP_VERSION_NAME_LEN (80) /* 版本信息字符串最大长度 */
#define AISP_MAX_DEV_NUM      (2)  /* 最大设备个数 */
#define AISP_MAX_MOD_NUM      (1)  /* 定义模块控制占用u64数量 */
#define AISP_MAX_DB_NUM       (16) /* 最大支持的DB个数, 最大的DB增益为 AISP_MAX_DB_NUM * 6 */

#define AISP_DOW_MAX_FRAME_NUM (2) /* DOW的最大帧数 */

#define AISP_AE_ROUTE_MIN_NODES  (2)  /* 曝光路径最小节点数 */
#define AISP_AE_ROUTE_MAX_NODES  (16) /* 曝光路径最大节点数 */
#define AISP_AE_WIN_WT_MAX_NODES (10) /* 区域权重的坐标点数, 水平垂直相同 */

#define AISP_AWB_CURVE_BIN_NUM         (20) /* 色温曲线横坐标点数 */
#define AISP_AWB_SPECIAL_LIGHT_SRC_NUM (5)  /* 特殊光源点最大个数 */
#define AISP_AWB_LS_ENV_LUM_THR_NODE   (2)  /* 环境亮度阈值点数 */

#define AISP_CCM_CT_NODE_NUM (3) /* 色温节点个数 */

#define AISP_MAX_LIGHT_NUM (1) /* 补光灯最大可调个数 */

#define AISP_MAX_WIDTH  (3072)
#define AISP_MAX_HEIGHT (1944)
#define AISP_MAX_FPS    (120000)

typedef union tagAispModRun {
    NI_U64 au64Status[AISP_MAX_MOD_NUM];
    struct
    {
        NI_U64 bitAispState : 1;           /* [01] */
        NI_U64 bitAeState : 1;             /* [02] */
        NI_U64 bitAwbState : 1;            /* [03] */
        NI_U64 bitBlcState : 1;            /* [04] */
        NI_U64 bitGammaState : 1;          /* [05] */
        NI_U64 bitLscState : 1;            /* [06] */
        NI_U64 bitGicState : 1;            /* [07] */
        NI_U64 bitDpcState : 1;            /* [08] */
        NI_U64 bitCcmState : 1;            /* [09] */
        NI_U64 bitLwdrState : 1;           /* [10] */
        NI_U64 bitRgb2yuvState : 1;        /* [11] */
        NI_U64 bitCscCdsState : 1;         /* [12] */
        NI_U64 bitEeState : 1;             /* [13] */
        NI_U64 bitCeState : 1;             /* [14] */
        NI_U64 bitCfaState : 1;            /* [15] */
        NI_U64 bitDowState : 1;            /* [16] */
        NI_U64 bitDrcdState : 1;           /* [17] */
        NI_U64 bitCnrState : 1;            /* [18] */
        NI_U64 bitPcrState : 1;            /* [19] */
        NI_U64 bitSccState : 1;            /* [20] */
        NI_U64 bitNry2dState : 1;          /* [21] */
        NI_U64 bitNryLc3dState : 1;        /* [22] */
        NI_U64 bitYhistContrastState : 1;  /* [23] */
        NI_U64 bitWbState : 1;             /* [24] */
        NI_U64 bitDnState : 1;             /* [25] */
        NI_U64 bitIrLightState : 1;        /* [26] */
        NI_U64 bitLedLightState : 1;       /* [27] */
        NI_U64 bitExpState : 1;            /* [28] */
        NI_U64 bitNryFull3dState : 1;      /* [29] */
        NI_U64 bitYhistLuminanceState : 1; /* [30] */
        NI_U64 bitYGammaState : 1;         /* [31] */
        NI_U64 bitRgbGicState : 1;         /* [32] */
        NI_U64 bitRgbDpcState : 1;         /* [33] */
        NI_U64 bitDecompState : 1;         /* [34] */
        NI_U64 bitFastAeState : 1;         /* [35] */
        NI_U64 bitBacaState : 1;           /* [36] */
    };
} AISP_MOD_RUN_U;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
