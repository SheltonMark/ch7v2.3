/*******************************************************************************
 * Copyright (C) 2020-2030 ZheJiang Chipup Technology CO.,LTD.
 *
 * 文件名 : PtzTracking.h
 *
 * 作  者 : Chipup Software Team
 * 版  本 : V1.0.0
 * 日  期 : 2022-09-09 Create
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
 * 日    期: 2022-09-09
 * 修订版本:
 * 作    者: Chipup Software Team
 * 修订备注:
 *******************************************************************************/

#ifndef _NI_PTZ_TRACKING_H_
#define _NI_PTZ_TRACKING_H_


/* ========================================================================== */
/*                          头文件区                                          */
/* ========================================================================== */
#include "ni_type.h"
#include "ni_common.h"
#include "ni_ias_od.h"

#if __cplusplus
extern "C" {
#endif /* __cplusplus */


/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */

typedef struct tagNiPtzTrackingInit
{
    NI_U16 u16IasWidth; 
    NI_U16 u16IasHeight; 

    NI_CHAR *pszModelFile; 

    NI_U32 u32Rev[16];
}NI_PTZ_TRACKING_INIT_S;


typedef struct tagNiPtzTrackingCreate
{
    NI_U32 u32Type;
    union
    {
        NI_CHAR *pszCfgFile;
        NI_PTZ_TRACKING_INIT_S stInitCfg; 
    };
}NI_PTZ_TRACKING_CREATE_S;


/*cmd*/
typedef enum tagNiPtzTrackingPtzCmdType
{
    NI_PTZ_TRACKING_PTZ_CMD_NONE,
    NI_PTZ_TRACKING_PTZ_CMD_RESET, 
    NI_PTZ_TRACKING_PTZ_CMD_SET_POS, 
    NI_PTZ_TRACKING_PTZ_CMD_BUTT,
}NI_PTZ_TRACKING_PTZ_CMD_TYPE_E;

typedef struct tagNiPtzTrackingPtzReset
{
    NI_PTZ_TRACKING_PTZ_CMD_TYPE_E enType;
}NI_PTZ_TRACKING_PTZ_RESET_S;

typedef struct tagNiPtzTrackingPtzPos
{
    NI_PTZ_TRACKING_PTZ_CMD_TYPE_E  enType;
    NI_U32                          u32PosX;
    NI_U32                          u32PosY;
    NI_U32                          u32Speed;
}NI_PTZ_TRACKING_PTZ_POS_S;

/*参数*/
typedef enum tagNiPtzTrackingParamType
{
    NI_PTZ_TRACKING_PARAM_TYPE_NONE, 
    NI_PTZ_TRACKING_PARAM_TYPE_ATTR, 
    NI_PTZ_TRACKING_PARAM_TYPE_MIRROR,
    NI_PTZ_TRACKING_PARAM_TYPE_BUTT,
}NI_PTZ_TRACKING_PARAM_TYPE_E;

typedef struct tagNiPtzTrackingAttr
{
    NI_PTZ_TRACKING_PARAM_TYPE_E enType;

    NI_U32                       u32ObjTolerance;
    NI_U32                       u32MoveKpx;
}NI_PTZ_TRACKING_ATTR_S;

typedef struct tagNiPtzTrackingMirror
{
    NI_PTZ_TRACKING_PARAM_TYPE_E enType;

    NI_BOOL                     bMirror;
    NI_BOOL                     bflip;
}NI_PTZ_TRACKING_MIRROR_S;

typedef struct tagNiPtzTrackingInterFace
{
    NI_S32 (*Run)(NI_VOID *pvHandle);
    NI_S32 (*Stop)(NI_VOID *pvHandle);
    NI_S32 (*SetStart)(NI_VOID *pvHandle, NI_BOOL bStart);
    NI_S32 (*UserPtzCtrlCmd)(NI_VOID *pvHandle, NI_VOID *pvCmd);
    NI_S32 (*SetParam)(NI_VOID *pvHandle, NI_VOID *pvParam);
    NI_S32 (*GetParam)(NI_VOID *pvHandle, NI_VOID *pvParam);
}NI_PTZ_TRACKING_INTERFACE_S;


/* 注册到内部的回调函数 */
typedef enum tagNiPtzTrackingCbFuncId
{
    NI_PTZ_TRACKING_CB_FUNC_ID_OD_RESULT_PROC, 
    NI_PTZ_TRACKING_CB_FUNC_ID_BUTT,
}NI_PTZ_TRACKING_CB_FUNC_ID_E;

typedef struct tagNiPtzTrackingOdResultProcessCb
{
    NI_PTZ_TRACKING_CB_FUNC_ID_E    enFuncId;   /* u32FuncId 为 NI_PTZ_TRACKING_CB_FUNC_ID_OD_RESULT_PROC 时的回调函数以及函数参数 */
    NI_VOID                         *pvCbCtx;
    NI_S32 (*RegOdResultProcess)(NI_VOID *pvCbCtx, IAS_OD_RESULT_S *pstResult);
} NI_PTZ_TRACKING_OD_RESULT_PROCESS_CB_S;

NI_S32 ptz_human_tracking_create(NI_VOID **pvHandle, NI_PTZ_TRACKING_CREATE_S *pstCreate);
NI_S32 ptz_human_tracking_get_func(NI_VOID *pvHandle, NI_PTZ_TRACKING_INTERFACE_S **pstInterFace);
NI_S32 ptz_human_tracking_reg_func_callback(NI_VOID *pvHandle, NI_VOID *pvCallBack);
NI_S32 ptz_human_tracking_destory(NI_VOID *pvHandle);

int HumanTrackGoBackPoint(int xCoord, int yCoord);

#if __cplusplus
}
#endif /* __cplusplus */


#endif