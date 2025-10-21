/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NI_COMM_AISPALG_INNER_H_
#define _NI_COMM_AISPALG_INNER_H_

#include "ni_type.h"
#include "ni_common.h"
#include "ni_aisp_define.h"
#include "ni_comm_aispalg.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/********************** COMPAT ***********************************/

/********************** AE ***********************************/

typedef enum tagAeDbgCmd {
    AE_DBG_CMD_PRINT_EN  = 0, /* 内部开关 */
    AE_DBG_CMD_SAVE_VREG = 1, /* 保存内部寄存器信息 */
    AE_DBG_CMD_BUTT,
} AE_DBG_CMD_E;

typedef struct tagAeDbgCmdHead {
    AE_DBG_CMD_E enCmd; /* 调试指令 */
} AE_DBG_CMD_HEAD_S;

typedef struct tagAeCmdPrintEn {
    AE_DBG_CMD_E enCmd;    /* 调试指令 */
    NI_BOOL      bPrintEn; /* 调试打印开关 */
} AE_CMD_PRINT_EN_S;

typedef struct tagAeCmdSaveVreg {
    AE_DBG_CMD_E enCmd;           /* 调试指令 */
    NI_BOOL      bPrintEn;        /* 保存时打印开关 */
    NI_CHAR      aszFilePath[64]; /* 保存文件路径 */
} AE_CMD_SAVE_VREG_S;

/********************** AWB ***********************************/

typedef enum tagAwbDbgCmd {
    AWB_DBG_CMD_PRINT_EN  = 0, /* 内部开关 */
    AWB_DBG_CMD_SAVE_VREG = 1, /* 保存内部寄存器信息 */
    AWB_DBG_CMD_BUTT,
} AWB_DBG_CMD_E;

typedef struct tagAwbDbgCmdHead {
    AWB_DBG_CMD_E enCmd; /* 调试指令 */
} AWB_DBG_CMD_HEAD_S;

typedef struct tagAwbCmdPrintEn {
    AWB_DBG_CMD_E enCmd;    /* 调试指令 */
    NI_BOOL       bPrintEn; /* 调试打印开关 */
} AWB_CMD_PRINT_EN_S;

typedef struct tagAwbCmdSaveVreg {
    AWB_DBG_CMD_E enCmd;           /* 调试指令 */
    NI_BOOL       bPrintEn;        /* 保存时打印开关 */
    NI_CHAR       aszFilePath[64]; /* 保存文件路径 */
} AWB_CMD_SAVE_VREG_S;

#if 0
/* 兼容旧版本 */
/**********************************************************/
typedef enum tagAispAwesModeV0
{
    AISP_AWES_MODE_FRAME_S_V0,  /* 只选用短曝光帧给算法*/
    AISP_AWES_MODE_FRAME_L_V0,                     /* 只选用长曝光帧给算法*/
    AISP_AWES_MODE_FRAME_COMPO_V0,                 /* 只选用合成帧给算法*/
    AISP_AWES_MODE_FRAME_L_S_V0,                   /* 选用长、短两帧统计数据给算法 */
    AISP_AWES_MODE_FRAME_L_COMPO_V0,               /* 选用长、合成两帧统计数据给算法 */
    AISP_AWES_MODE_FRAME_S_COMPO_V0,               /* 选用短、合成两帧统计数据给算法 */
    AISP_AWES_MODE_FRAME_L_S_COMPO_V0,             /* 选用长、短、合成帧三帧统计数据给算法 */

    AISP_AWES_MODE_BUTT_V0
}AISP_AWES_MODE_V0_E;

#define AISP_AWES_MODE_E               AISP_AWES_MODE_V0_E
#define AISP_AWES_MODE_FRAME_S         AISP_AWES_MODE_FRAME_S_V0
#define AISP_AWES_MODE_FRAME_L         AISP_AWES_MODE_FRAME_L_V0
#define AISP_AWES_MODE_FRAME_COMPO     AISP_AWES_MODE_FRAME_COMPO_V0
#define AISP_AWES_MODE_FRAME_L_S       AISP_AWES_MODE_FRAME_L_S_V0
#define AISP_AWES_MODE_FRAME_L_COMPO   AISP_AWES_MODE_FRAME_L_COMPO_V0
#define AISP_AWES_MODE_FRAME_S_COMPO   AISP_AWES_MODE_FRAME_S_COMPO_V0
#define AISP_AWES_MODE_FRAME_L_S_COMPO AISP_AWES_MODE_FRAME_L_S_COMPO_V0
typedef struct tagAwbDbgInfoV0
{
    NI_U16     u16TC;
    NI_U16     u16GainR;
    NI_U16     u16GainB;
} AWB_DBG_INFO_V0_S;
#define AWB_DBG_INFO_S                 AWB_DBG_INFO_V0_S

typedef struct tagAwbExtInfoV0
{
    NI_S32      s32EnvLum;
    NI_S32      s32DrInfo;
    NI_VOID     *pCalcBuf;
} AWB_EXT_INFO_V0_S;
#define AWB_EXT_INFO_S                 AWB_EXT_INFO_V0_S

typedef struct tagAwbResultV0
{
    NI_BOOL bValid;
    NI_U16  u16GainR;
    NI_U16  u16GainB;
    NI_U16  u16GainGr;
    NI_U16  u16GainGb;
    NI_U16  u16LowCtRatio;
    NI_U16  u16HigCtRatio;
    NI_U16  u16MulCt;
    NI_U16  u16Rev;
    NI_S32  s32Weight;
}AWB_RESULT_V0_S;
#define AISP_AWB_RESULT_S              AWB_RESULT_V0_S

typedef struct tagAwesInfoV0
{
    NI_BOOL                 bValid;             /* 有效标志 */
    NI_U32                  u32DataWidth;       /* 统计数据位宽 */
    NI_U32                  u32BlockNum;        /* AWES统计一帧的分块总数 */
    NI_U32                  u32HBlockNum;       /* AWES统计水平方向分块个数 */
    NI_U32                  u32VBlockNum;       /* AWES统计垂直方向分块个数 */
    NI_VOID                 *pvAwesData;        /* AWES统计的rgby值, 每帧统计数据=u32BlockNum*2*4bytes. */
    AISP_AWES_MODE_E        AwesMode;           /* AWES统计模式 */
}AWES_INFO_V0_S;
#define AISP_AWES_INFO_S               AWES_INFO_V0_S
/**********************************************************/
#endif

/********************** CCM ***********************************/

#define RESERVE_FORCE_STRUCT_ALIGN(name, size, align)  // NI_U8 unused##name[(align) - ((align) - ((size) % align))];
#define CCM_MAX_DB_NUM                                 (AISP_MAX_DB_NUM)

typedef struct tagCcmTuningParam {
    NI_U32 u32CtValue;                                                 /* RW, 色温信息, 以WB的RGain等效色温信息 */
    NI_S32 s32SysGain;                                                 /* RW, 增益信息 */
    NI_U8  au8Sat[CCM_MAX_DB_NUM];                                     /* RW, 各增益段图像饱和度
                                                                        * range[0, 255], default:128 */
    RESERVE_FORCE_STRUCT_ALIGN(Sat, sizeof(NI_U8) * CCM_MAX_DB_NUM, 4) /* RW, 无效参数强制结构体对齐 */
    CCM_MATRIX_S astCcm[CCM_CT_NODE_NUM];                              /* RW, 不同色温下的CCM矩阵 */
} CCM_TUNING_PARAM_S;

typedef struct tagCcmTuningResult {
    NI_U8 u8Sat;
    RESERVE_FORCE_STRUCT_ALIGN(Sat, sizeof(NI_U8), 4) /* RW, 无效参数强制结构体对齐 */
    NI_F32 af32CcmWt[CCM_CT_NODE_NUM];
} CCM_TUNING_RESULT_S;

/********************** DAYNIGHT ***********************************/

typedef enum tagDnDbgCmd {
    DN_DBG_CMD_PRINT_EN  = 0, /* 内部开关 */
    DN_DBG_CMD_SAVE_VREG = 1, /* 保存内部寄存器信息 */
    DN_DBG_CMD_BUTT,
} DN_DBG_CMD_E;

typedef struct tagDnDbgCmdHead {
    DN_DBG_CMD_E enCmd; /* 调试指令 */
} DN_DBG_CMD_HEAD_S;

typedef struct tagDnCmdPrintEn {
    DN_DBG_CMD_E enCmd;    /* 调试指令 */
    NI_BOOL      bPrintEn; /* 调试打印开关 */
} DN_CMD_PRINT_EN_S;

typedef struct tagDnCmdSaveVreg {
    DN_DBG_CMD_E enCmd;           /* 调试指令 */
    NI_BOOL      bPrintEn;        /* 保存时打印开关 */
    NI_CHAR      aszFilePath[64]; /* 保存文件路径 */
} DN_CMD_SAVE_VREG_S;

/********************** IRLIGHT ***********************************/

typedef enum tagIrLightDbgCmd {
    IR_LIGHT_DBG_CMD_PRINT_EN  = 0, /* 内部开关 */
    IR_LIGHT_DBG_CMD_SAVE_VREG = 1, /* 保存内部寄存器信息 */
    IR_LIGHT_DBG_CMD_BUTT,
} IR_LIGHT_DBG_CMD_E;

typedef struct tagIrLightDbgCmdHead {
    IR_LIGHT_DBG_CMD_E enCmd; /* 调试指令 */
} IR_LIGHT_DBG_CMD_HEAD_S;

typedef struct tagIrLightCmdPrintEn {
    IR_LIGHT_DBG_CMD_E enCmd;    /* 调试指令 */
    NI_BOOL            bPrintEn; /* 调试打印开关 */
} IR_LIGHT_CMD_PRINT_EN_S;

typedef struct tagIrLightCmdSaveVreg {
    IR_LIGHT_DBG_CMD_E enCmd;           /* 调试指令 */
    NI_BOOL            bPrintEn;        /* 保存时打印开关 */
    NI_CHAR            aszFilePath[64]; /* 保存文件路径 */
} IR_LIGHT_CMD_SAVE_VREG_S;

/********************** LEDLIGHT ***********************************/

typedef enum tagLedLightDbgCmd {
    LED_LIGHT_DBG_CMD_PRINT_EN  = 0, /* 内部开关 */
    LED_LIGHT_DBG_CMD_SAVE_VREG = 1, /* 保存内部寄存器信息 */
    LED_LIGHT_DBG_CMD_BUTT,
} LED_LIGHT_DBG_CMD_E;

typedef struct tagLedLightDbgCmdHead {
    LED_LIGHT_DBG_CMD_E enCmd; /* 调试指令 */
} LED_LIGHT_DBG_CMD_HEAD_S;

typedef struct tagLedLightCmdPrintEn {
    LED_LIGHT_DBG_CMD_E enCmd;    /* 调试指令 */
    NI_BOOL             bPrintEn; /* 调试打印开关 */
} LED_LIGHT_CMD_PRINT_EN_S;

typedef struct tagLedLightCmdSaveVreg {
    LED_LIGHT_DBG_CMD_E enCmd;           /* 调试指令 */
    NI_BOOL             bPrintEn;        /* 保存时打印开关 */
    NI_CHAR             aszFilePath[64]; /* 保存文件路径 */
} LED_LIGHT_CMD_SAVE_VREG_S;

/********************** RGB2YUV ***********************************/

/********************** YHISTCTR ***********************************/

typedef enum tagYhistCtrDbgCmd {
    YHIST_CTR_DBG_CMD_PRINT_EN  = 0, /* 内部开关 */
    YHIST_CTR_DBG_CMD_SAVE_VREG = 1, /* 保存内部寄存器信息 */
    YHIST_CTR_DBG_CMD_BUTT,
} YHIST_CTR_DBG_CMD_E;

typedef struct tagYhistCtrDbgCmdHead {
    YHIST_CTR_DBG_CMD_E enCmd; /* 调试指令 */
} YHIST_CTR_DBG_CMD_HEAD_S;

typedef struct tagYhistCtrCmdPrintEn {
    YHIST_CTR_DBG_CMD_E enCmd;    /* 调试指令 */
    NI_BOOL             bPrintEn; /* 调试打印开关 */
} YHIST_CTR_CMD_PRINT_EN_S;

typedef struct tagYhistCtrCmdSaveVreg {
    YHIST_CTR_DBG_CMD_E enCmd;           /* 调试指令 */
    NI_BOOL             bPrintEn;        /* 保存时打印开关 */
    NI_CHAR             aszFilePath[64]; /* 保存文件路径 */
} YHIST_CTR_CMD_SAVE_VREG_S;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
