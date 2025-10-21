/*******************************************************************************
 * Copyright (C) 2020-2030 ZheJiang Chipup Technology CO.,LTD.
 *
 * 文件名 : PtzTracking.h
 *
 * 作  者 : Chipup Software Team
 * 版  本 : V1.0.0
 * 日  期 : 2022-09-14 Create
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
 * 日    期: 2022-09-14
 * 修订版本:
 * 作    者: Chipup Software Team
 * 修订备注:
 *******************************************************************************/

#ifndef _PTZ_TRACKING_H_
#define _PTZ_TRACKING_H_


/* ========================================================================== */
/*                          头文件区                                          */
/* ========================================================================== */

#if __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef PTZ_VER_X
    #define PTZ_VER_X 1
#endif

#ifndef PTZ_VER_Y
    #define PTZ_VER_Y 0
#endif

#ifndef PTZ_VER_Z
    #define PTZ_VER_Z 0
#endif

#ifndef PTZ_VER_P
    #define PTZ_VER_P 0
#endif

#ifndef PTZ_VER_B
    #define PTZ_VER_B "B000"
#endif

#ifndef PTZ_VER_GIT
    #define PTZ_VER_GIT 0
#endif

#ifdef NI_PTZ_DEBUG
    #define PTZ_VER_D " Debug"
#else
    #define PTZ_VER_D " Release"
#endif

#define __MK_PTZ_GIT_VERSION(q) "git:"#q
#define MK_PTZ_GIT_VERSION(q) __MK_PTZ_GIT_VERSION(q)

#define __MK_PTZ_VERSION(x,y,z,p) #x"."#y"."#z"."#p" "
#define MK_PTZ_VERSION(x,y,z,p) __MK_PTZ_VERSION(x,y,z,p)

#define PTZ_VER_PRIX "_PTZ_V"

#define PTZ_VERSION "[" CHIP_NAME PTZ_VER_PRIX MK_PTZ_VERSION(PTZ_VER_X, PTZ_VER_Y, PTZ_VER_Z, PTZ_VER_P) PTZ_VER_B PTZ_VER_D "]"
#define PTZ_GIT_VERSION MK_PTZ_GIT_VERSION(PTZ_VER_GIT)

#define PTZ_TRACKING_STRING_MAX_LEN    (256)
#define PTZ_TRACKING_DEFAULT_MODEL_FILE                 "/home/cfg/net_model_yolo_model_480x320_pcd.bin"
#define PTZ_TRACKING_DEFAULTOPT_COORDINATE_FILE         "/opt/sav/Config/coordinate"

#define PTZ_TRACKING_DEFAULT_CFG_FILE  "/home/cfg/ptz_tracking_cfg"

typedef struct stPtzTrackingCfgPhrase
{
    NI_U16 u16IasWidth; 
    NI_U16 u16IasHeight; 

    NI_CHAR aszModelFile[PTZ_TRACKING_STRING_MAX_LEN]; 
    // NI_CHAR aszCoordinateFile[PTZ_TRACKING_STRING_MAX_LEN]; 
}PTZ_TRACKING_CFG_PHRASE_S;


#if __cplusplus
}
#endif /* __cplusplus */


#endif
