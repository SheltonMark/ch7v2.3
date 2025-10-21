/*******************************************************************************
 * Copyright (C) 2020-2030 ZheJiang Chipup Technology CO.,LTD.
 *
 * 文件名 : PtzOption.h
 *
 * 作  者 : Chipup Software Team
 * 版  本 : V1.0.0
 * 日  期 : 2022-09-13 Create
 *
 * 描  述 :
 *
 *       1. 硬件说明
 *          无。
 *
 *       2. 程序结构说明。
 *          无。
 *
 *       3. 使用说明。
 *          无。
 *
 *       4. 局限性说明。
 *          无。
 *
 *       5. 其他说明。
 *          无。
 *
 * 修订历史:
 * 日    期: 2022-09-13
 * 修订版本:
 * 作    者: Chipup Software Team
 * 修订备注:
 *******************************************************************************/

#ifndef _PTZ_OPTION_H_
#define _PTZ_OPTION_H_


/* ========================================================================== */
/*                          头文件区                                          */
/* ========================================================================== */
#include <stdio.h>
#include <unistd.h>

#include "ni_type.h"

#if __cplusplus
extern "C" {
#endif /* __cplusplus */

/*  电子云台命令字 */
typedef enum stPtzOptCmd
{
    PTZ_OPT_CMD_RESET, 
    PTZ_OPT_CMD_GETCAPS,                    /* 获取电子云台控制能力 PTZ_OPT_STRUCT.ptzCaps*/
    PTZ_OPT_CMD_ZOOM,                       /* 配置电子云台放大倍数 PTZ_OPT_STRUCT.zoom*/
    PTZ_OPT_CMD_FOCUS,                      /* 配置电子云台焦距 PTZ_OPT_STRUCT.focal*/
    PTZ_OPT_CMD_MOVE,                       /* 移动电子云台视角(记录flash和内存) PTZ_OPT_STRUCT.ptzMove*/
    PTZ_OPT_CMD_LOCATION,                   /* 获取电子云台当前绝对位置(实时位置，内存值) PTZ_OPT_STRUCT.ptzLocation*/
    PTZ_OPT_CMD_SPEED,                      /* 获取电子云台速度范围 PTZ_OPT_STRUCT.ptzSpeed*/
    PTZ_OPT_CMD_MOVE_CACHE,                 /* 移动电子云台视角(不记录flash，仅内存) PTZ_OPT_STRUCT.ptzMove*/
    PTZ_OPT_CMD_LOCATION_SAVE,              /* 获取电子云台保存的位置(flash保存的位置) PTZ_OPT_STRUCT.ptzLocation*/
    PTZ_OPT_CMD_BUTT,
}PTZ_OPT_CMD_E;

/*  电子云台能力集掩码 */
typedef struct stPtzOptCaps
{
    PTZ_OPT_CMD_E   enPtzCmd;                        /* 电子云台控制命令, 定义见 PTZ_OPT_CMD_E */
    NI_U32          u32CapsMask;                     /* 支持的电子云台控制命令掩码 */
}PTZ_OPT_CAPS_S;


/* 电子云台移动 范围: x轴: 0~8192, y轴: 0~8192 */
typedef struct stPtzOptMove
{
    PTZ_OPT_CMD_E enPtzCmd;                         /* 电子云台控制命令, 定义见 PTZ_OPT_CMD_E */

    NI_U32  u32PosX;                                /* 水平坐标，取值范围 0~8192 */
    NI_U32  u32PosY;                                /* 竖直坐标，取值范围 0~8192 */
    NI_U32  u32Speed;                               /* 速度，取值范围 */
}PTZ_OPT_MOVE_S;

/* 电子云台坐标 */
typedef struct stPtzOptPos
{
    PTZ_OPT_CMD_E enPtzCmd;                         /*  电子云台控制命令, 定义见 PTZ_OPT_CMD_E */

    NI_U32  u32PosX;                                /*  水平坐标，取值范围0~8192 */
    NI_U32  u32PosY;                                /*  竖直坐标，取值范围0~8192 */
}PTZ_OPT_POS_S;

/* 电子云台移动速度 */
typedef struct stPtzOptSpeedParam
{
    PTZ_OPT_CMD_E enPtzCmd;                         /*  电子云台控制命令, 定义见 PTZ_OPT_CMD_E */

    NI_U32 u32MaxSpeed;                            /*  最大速度 */
    NI_U32 u32MinSpeed;                            /*  最小速度 */
}PTZ_OPT_SPEED_PARAM_S;

typedef struct tagPtzOptInterFace
{
    NI_S32 (*Reset)(NI_VOID *pvHandle);
    NI_S32 (*UserPtzCtrlCmd)(NI_VOID *pvHandle, NI_VOID *pvCmd);
}PTZ_OPT_INTERFACE_S;


typedef struct tagPtzOptInit
{
    NI_U32  u32PosOldX;                                /* 水平坐标，取值范围 0~8192 */
    NI_U32  u32PosOldY;                                /* 竖直坐标，取值范围 0~8192 */

    NI_U32  u32PosNewX;                                /* 水平坐标，取值范围 0~8192 */
    NI_U32  u32PosNewY;                                /* 竖直坐标，取值范围 0~8192 */
}PTZ_OPT_INIT_S;

typedef struct tagPtzOptreate
{
    NI_U32 u32Type;
    union
    {
        NI_CHAR *pszCfgFile; /* 最大128 */
        PTZ_OPT_INIT_S stInitCfg; /* 不支持 */
    };
}PTZ_OPT_CREATE_S;


NI_S32 PTZ_OPT_Create(NI_VOID **pvHandle, PTZ_OPT_CREATE_S *pstCreate);
NI_S32 PTZ_OPT_GetFunc(NI_VOID *pvHandle, PTZ_OPT_INTERFACE_S **pstInterFace);
NI_S32 PTZ_OPT_Destory(NI_VOID *pvHandle);

#if __cplusplus
}
#endif /* __cplusplus */


#endif

