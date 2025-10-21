/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NI_AISP_DEBUG_H_
#define _NI_AISP_DEBUG_H_

#include "ni_type.h"
#include "ni_comm_aisp.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct tagAispAppCfg {
    NI_BOOL              bUseManualCfg; /* 自定义配置使能 */
    AISP_CFG_FILE_INFO_S stCfgFile;     /* 配置信息 */
} AISP_APP_CFG_S;

typedef enum tagAispCfgSelect {
    AISP_CFG_SELECT_USE_FLASH = 0, /* AISP 使用flash的配置 */
    AISP_CFG_SELECT_USE_MEM   = 1, /* AISP 使用内存中的配置 */
    AISP_CFG_SELECT_BUTT,
} AISP_CFG_SELECT_E;

// typedef struct tagAispCfgDebug
// {
//     AISP_CFG_ID_E           enCfgId;            /* 模块ID */
//     AISP_CFG_FILE_INFO_S    stCfgFile;          /* 配置信息 */
// } AISP_CFG_DEBUG_S;

typedef enum tagAispDbgCmd {
    AISP_DBG_CMD_PRINT_EN             = 0, /* 内部开关 AISP_CMD_BOOL_S */
    AISP_DBG_CMD_SAVE_VREG            = 1, /* 保存内部寄存器信息 AISP_CMD_SAVE_VREG_S */
    AISP_DBG_CMD_GET_VREG_ADDR        = 2, /* 获取内部寄存器地址信息 AISP_CMD_GET_VREG_S */
    AISP_DBG_CMD_GET_EXP_FROM_FILE_EN = 3, /* raw仿真功能,从raw的配置文件中获取曝光信息 AISP_CMD_BOOL_S */
    AISP_DBG_CMD_AUTOFPS_CTRL_INFO    = 4, /* 自动降帧参数配置 ADAPT_AUTO_FPS_INFO_S */
    AISP_DBG_CMD_BUTT,
} AISP_DBG_CMD_E;

typedef struct tagAispDbgCmdHead {
    AISP_DBG_CMD_E enCmd; /* 调试指令 */
} AISP_DBG_CMD_HEAD_S;

typedef struct tagAispCmdSaveVreg {
    AISP_DBG_CMD_E enCmd;           /* 调试指令 */
    NI_BOOL        bPrintEn;        /* 保存时打印开关 */
    NI_CHAR        aszFilePath[64]; /* 保存文件路径 */
} AISP_CMD_SAVE_VREG_S;

typedef struct tagAispCmdGetVregAddr {
    AISP_DBG_CMD_E       enCmd;      /* 调试指令 */
    AISP_CFG_FILE_INFO_S stVregInfo; /* 内部状态寄存器的地址信息 */
} AISP_CMD_GET_VREG_ADDR_S;

typedef struct tagAispCmdBool {
    AISP_DBG_CMD_E enCmd;   /* 调试指令 */
    NI_BOOL        bEnable; /* 开关使能类调试信息 */
} AISP_CMD_BOOL_S;

typedef struct tagAispAutoFpsDebug {
    NI_U32 u32ShtThr;  /* 快门时间调节阈值 */
    NI_U32 u32GainThr; /* 增益调节阈值 */
    NI_U32 u32ContNum; /* 延时计数值 */
} AISP_AUTOFPS_DBG_S;

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_CopyCfgToMem
 * 描  述:    将当前正在使用的配置数据拷贝到内存，返回当前正在使用的内存地址。
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
         :   bEnable       拷贝使能
         :   pu32CfgBuf    拷贝后的内存地址
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_CopyCfgToMem(ISP_DEV IspDev, NI_BOOL bEnable, NI_U32 *pu32CfgBuf);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_SelectCfgInfo
 * 描  述:    选择使用配置的方式--cvi项目用于内存切换,选用flash配置还是内存中配置,IPC项目用于标识内存中的数据允许被修改。
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
         :   enCfgSelect   配置选择
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_SelectCfgInfo(ISP_DEV IspDev, AISP_CFG_SELECT_E enCfgSelect);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_GetCfgInfo
 * 描  述:    获取配置文件首地址
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
         :   pstCfgInfo   配置文件首地址
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_GetCfgInfo(ISP_DEV IspDev, AISP_APP_CFG_S *pstCfgInfo);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_SetCfgInfo
 * 描  述:    该功能不支持
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
         :   pstCfgInfo
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_SetCfgInfo(ISP_DEV IspDev, AISP_APP_CFG_S *pstCfgInfo);

/*******************************************************************************
 * 函数名:    NI_MDK_AISPD_Init
 * 描  述:
 * 输  入:   参数          描述
 *       :
         :
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISPD_Init(ISP_DEV IspDev);

/*******************************************************************************
 * 函数名:    NI_MDK_AISPD_DeInit
 * 描  述:
 * 输  入:   参数          描述
 *       :
         :
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISPD_DeInit(ISP_DEV IspDev);

/*******************************************************************************
 * 函数名:    NI_MDK_AISPD_CmdProcess
 * 描  述:
 * 输  入:   参数          描述
 *       :
         :
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISPD_CmdProcess(ISP_DEV IspDev, NI_VOID *pvCmd);

// /*******************************************************************************
//  * 函数名:    NI_MDK_AISPD_CfgDebug
//  * 描  述:
//  * 输  入:   参数          描述
//  *       :
//          :
//  * 输  出:   无
//  * 返回值:   NI_SUCCESS: 成功
//  *          NI_FAILURE: 失败
//  *******************************************************************************/
// NI_S32 NI_MDK_AISPD_CfgDebug(ISP_DEV IspDev, AISP_CFG_DEBUG_S *pstModCfg);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
