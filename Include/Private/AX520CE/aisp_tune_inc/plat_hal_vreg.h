/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _PLAT_HAL_VREG_H_
#define _PLAT_HAL_VREG_H_

#include "ni_type.h"
#include <semaphore.h>
#include "list.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define AISP_BLINK_STEP_NUM 200 /* 闪烁调节步长 */

typedef struct tagAdaptMAwesNode {
    struct list_head        list;
    AISP_MAWES_STATISTICS_S stMirrowAwes;
    // ADAPT_MAWES_MBUF_S          stMAwesBuf;
} ADAPT_MAWES_NODE_S;

typedef struct tagAdaptMAwesMbuf {
    NI_U32   u32PhyAddr;
    NI_VOID *pvVirAddr;
    NI_U32   u32BufLen;
} ADAPT_MAWES_MBUF_S;

typedef struct tagAdaptMAwesList {
    struct list_head list;
    NI_U32           u32NodeCnt;
} ADAPT_MAWES_LIST_S;

typedef struct tagAdaptMawesInfo {
    NI_BOOL            bMAwesInit; /* mawes初始化标识 */
    ADAPT_MAWES_LIST_S stFreeList; /* 空闲链表 */
    ADAPT_MAWES_LIST_S stBusyList; /* 繁忙链表 */
    ADAPT_MAWES_LIST_S stUsrList;  /* 用户链表 */

    ADAPT_MAWES_NODE_S *pstMAwesDataNode;    /* 节点数据地址 */
    NI_U32              u32MAwesDataNodeNum; /* 节点数据数量 */

    AISP_MAWES_INIT_PARAM_S stMirrorAwesInit; /* 初始化参数 */
    ADAPT_MAWES_MBUF_S      stMAwesBuf;       /* 申请到的物理地址、虚拟地址和长度信息 */

    // sem_t                       stMAwesSem;     /* 信号量值 */
} ADAPT_MAWES_INFO_S;

typedef struct tagPlatAdaptVreg {
    ADAPT_MAWES_INFO_S stMawesInfo;      /* Mawes信息 */
    NI_BOOL            bNrylc3dChangEn;  /* stDfAdjLumY.u8Ratio1变更标识，防噪声跳动 */
    NI_U8              u8Nrylc3dDelayPs; /* 噪声跳动调节延时帧数 */
    NI_U8              u8ExpLastMode;    /* Exp上一帧模式 */
    NI_U8              u8ExpDelay;       /* 延时帧数，当前切换Exp模式，会延迟3帧Nry的状态 */
    NI_BOOL            bNry2dState;      /* Nry2d状态暂存 */
    NI_BOOL            bNrylc3dState;    /* Nrylc3d状态暂存 */
    NI_BOOL            bNryfull3dState;  /* Nryfull3d状态暂存 */

#if (XSCHIP_ID == 0x00100100)
    NI_U8 au8CalcBuf[16383]; /* 用于中间计算的buf，比如表的差值,awb算法等 */
#endif
} PLAT_ADAPT_VREG_S;

typedef struct tagAispWebInfo {
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
} AISP_WEB_INFO_S;

typedef struct tagAispLightInfo {
    NI_U32 au32LightLevel[AISP_MAX_LIGHT_NUM]; /* 记录当前补光灯结果 */
    NI_U32 u32Kp;
    NI_U32 u32Ki;
    NI_U32 u32Kd;
    NI_S32 as32Delta[3];
} AISP_LIGHT_INFO_S;

typedef struct tagAispDnRecord {
    NI_BOOL bPreValid;   /* 标记记录保存的AE稳定后的数据是否有效 */
    NI_U32  u32PreGain;  /* 记录的AE稳定后的的增益信息 */
    NI_U32  u32PreSht;   /* 记录的AE稳定后的的快门信息 */
    NI_U32  u32PreEnv;   /* 记录的AE稳定后的的环境亮度信息 */
    NI_U16  u16PreGainR; /* 记录的AE稳定后的的RGain信息 */
    NI_U16  u16PreGainB; /* 记录的AE稳定后的的BGain信息 */
} AISP_DN_RECORD_S;

typedef struct tagAispDnDbgInfo {
    NI_U32 u32EnvLum;    /* RO, 当前环境亮度判断信息，u32EnvLum = 当前曝光*当前增益 单位：us*倍 */
    NI_U32 u32D2nEnvThr; /* RO, 白切黑亮度阈值， u32EnvLumThr 需大于此值才可以触发切换 u32D2nEnvThr = u32D2nGainThr * 当前原始帧率下的最大曝光值 单位：us*倍*/
    NI_U32 u32N2dEnvThr; /* RO, 黑切白亮度阈值， u32EnvLumThr 需小于此值才可以触发切换 u32N2dEnvThr = u32N2dGainThr * 当前原始帧率下的最大曝光值 单位：us*倍*/
    NI_U16 u16Gr;        /* RO, 当前画面Gr增益, 当此值u16Gr > u16GainRAvgThr 或 u16Gb > u16GainBAvgThr时，符合红外黑切白要求 */
    NI_U16 u16Gb;        /* RO, 当前画面Gb增益, 当此值u16Gr > u16GainRAvgThr 或 u16Gb > u16GainBAvgThr时，符合红外黑切白要求 */
    NI_U16 u16IrPerCent; /* RO, 当前画面红外占比，当此值小于配置中的u16IrPerCent时，符合红外黑切白要求 */

    AISP_DN_RECORD_S stDnRecord; /* RO, 灯切换前后 统计信息记录*/
} AISP_DN_DBG_INFO_S;

typedef struct tagAispDnParam {
    NI_U16 u16D2nValue;   /* RW, 日夜切换灵敏度,光敏方案时有效, range[0, u16N2dValue - 1], default:90 */
    NI_U16 u16N2dValue;   /* RW, 夜日切换灵敏度,光敏方案有效, range[u16D2nValue + 1, 1024], default:150 */
    NI_U16 u16Delay;      /* RW, 切换延时时间,以帧为单位, range[10, 0xFFFF], default:20 */
    NI_U32 u32D2nGainThr; /* RW, 日夜切换灵敏度增益阈值,软光敏方案有效, range[u32N2dGainThr - 6000, 72000], default:36000 */
    NI_U32 u32N2dGainThr; /* RW, 夜日切换灵敏度增益阈值,软光敏方案有效, range[6000, u32D2nGainThr - 6000], default:30000  */

    NI_U16 u16GainRThr; /* RW, Gr阈值,软光敏方案有效,Gr差阈值, range[10, 256], default:40 */
    NI_U16 u16GainBThr; /* RW, Gb阈值,软光敏方案有效,Gb差阈值, range[10, 256], default:40 */

    NI_U16 u16GainRAvgThr;  /* RW, Gr均值阈值,软光敏方案有效,黑白模式有效, range[256, 1024], default:320  */
    NI_U16 u16GainBAvgThr;  /* RW, Gb均值阈值,软光敏方案有效,黑白模式有效, range[256, 1024], default:340  */
    NI_U16 u16LightPerCent; /* RW, 环境光强度比例阈值,软光敏方案有效,彩色模式时,值越小灵敏度越高,黑白模式时,值越大灵敏度越高, range[0, 1000], default:500 */

    NI_U16 u16LedDiffR;    /* RW, LED色温判断模式下diff r [0, 256] default:40 */
    NI_U16 u16LedDiffB;    /* RW, LED色温判断模式下diff b [0, 256] default:40 */
    NI_U16 u16LedDiffEnvH; /* RW, LED亮度环境差异高阈值 [1000, 2000] default:1500 */
    NI_U16 u16LedDiffEnvL; /* RW, LED亮度环境差异低阈值 [0, 1000] default:500 */
} AISP_DN_PARAM_S;
typedef struct tagAispDnInfo {
    NI_BOOL            bDayEn;    /* 日夜结果, 0表示晚上, 1表示白天 */
    NI_BOOL            bIrCutEn;  /* 滤光片结果, 0表示晚上, 1表示白天 */
    NI_U32             u32DnEnv;  /* 当前环境的日夜状态信息, 1表示白天, 2表示晚上 */
    AISP_DN_DBG_INFO_S stDbgInfo; /* 调试信息 */
    AISP_DN_PARAM_S    stDnParam; /* 算法参数 */
} AISP_DN_INFO_S;

typedef struct tagAispDataInfo {
    /* Ae */ /* AispExp需放在开头 */
    AISP_SEN_EXP_S stAispExp;
    AE_RESULT_S    stAeResult;
    /* sensor */
    NI_U32 u32ExpLinePer1000Ms;
    /* awb */
    AWB_INFO_S stAwbInfo;
    /* web */
    AISP_WEB_INFO_S stWeb;
    /*Led light*/
    AISP_LIGHT_INFO_S stLedLightInfo;
    /*ir light*/
    AISP_LIGHT_INFO_S stIrLightInfo;
    /*day night*/
    AISP_DN_INFO_S stDnInfo;
    DN_ATTR_S      stDnParam;
    /* stata */
    NI_U32 au32Status[1];

} AISP_DATA_INFO_S;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
