/*******************************************************************************
 * Copyright (C) 2020-2025 ZheJiang Chipup Technology CO.,LTD.
 *
 * 文件名 : ni_aisp_define.h
 *
 * 作  者 : Chipup Software Team
 * 版  本 : V1.0.0
 * 日  期 : 2022-02-23 Create
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
 * 日    期: 2022-02-23
 * 修订版本:
 * 作    者:
 * 修订备注:
 *******************************************************************************/



#ifndef _NI_CHIP_DEFINE_H_
#define _NI_CHIP_DEFINE_H_


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "ni_errno.h"


#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */


/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */

#ifndef AISP_VER_X
    #define AISP_VER_X 1
#endif

#ifndef AISP_VER_Y
    #define AISP_VER_Y 0
#endif

#ifndef AISP_VER_Z
    #define AISP_VER_Z 0
#endif

#ifndef AISP_VER_P
    #define AISP_VER_P 0
#endif

#ifndef AISP_VER_B
    #define AISP_VER_B "B000"
#endif

#ifndef AISP_VER_GIT
    #define AISP_VER_GIT 0
#endif



#ifdef NI_AISP_DEBUG
    #define AISP_VER_D " Debug"
#else
    #define AISP_VER_D " Release"
#endif

#define NI_ID_AISP       (46)


#define __MK_AISP_GIT_VERSION(q) "git:"#q
#define MK_AISP_GIT_VERSION(q) __MK_AISP_GIT_VERSION(q)

#define __MK_AISP_VERSION(x,y,z,p) #x"."#y"."#z"."#p" "
#define MK_AISP_VERSION(x,y,z,p) __MK_AISP_VERSION(x,y,z,p)
#define __MK_AISP_CFG_VERSION(x,y,z,p) #x"."#y"."#z"."#p
#define MK_AISP_CFG_VERSION(x,y,z,p) __MK_AISP_CFG_VERSION(x,y,z,p)
#define ADAPT_VER_PRIX "_AISP_V"

#define AISP_VERSION "["CHIP_NAME ADAPT_VER_PRIX MK_AISP_VERSION(AISP_VER_X, AISP_VER_Y, AISP_VER_Z, AISP_VER_P) AISP_VER_B AISP_VER_D "]"
#define AISP_GIT_VERSION MK_AISP_GIT_VERSION(AISP_VER_GIT)
#define AISP_CFG_VERSION "["CHIP_NAME ADAPT_VER_PRIX MK_AISP_CFG_VERSION(AISP_VER_X, AISP_VER_Y, AISP_VER_Z, AISP_VER_P)"]"


#define NI_ERR_AISP_INVALID_DEVID     NI_DEF_ERR(NI_ID_AISP, EN_ERR_LEVEL_ERROR, ERR_INVALID_DEVID)
#define NI_ERR_AISP_INVALID_SNSID     NI_DEF_ERR(NI_ID_AISP, EN_ERR_LEVEL_ERROR, ERR_INVALID_CHNID)
#define NI_ERR_AISP_ILLEGAL_PARAM     NI_DEF_ERR(NI_ID_AISP, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)
#define NI_ERR_AISP_LIB_EXIST         NI_DEF_ERR(NI_ID_AISP, EN_ERR_LEVEL_ERROR, ERR_EXIST)
#define NI_ERR_AISP_LIB_UNEXIST       NI_DEF_ERR(NI_ID_AISP, EN_ERR_LEVEL_ERROR, ERR_UNEXIST)
#define NI_ERR_AISP_NULL_PTR          NI_DEF_ERR(NI_ID_AISP, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)
#define NI_ERR_AISP_NOT_CONFIG        NI_DEF_ERR(NI_ID_AISP, EN_ERR_LEVEL_ERROR, ERR_NOT_CONFIG)
#define NI_ERR_AISP_NOT_SUPPORT       NI_DEF_ERR(NI_ID_AISP, EN_ERR_LEVEL_ERROR, ERR_NOT_SUPPORT)
#define NI_ERR_AISP_NOT_PERM          NI_DEF_ERR(NI_ID_AISP, EN_ERR_LEVEL_ERROR, ERR_NOT_PERM)
#define NI_ERR_AISP_NOMEM             NI_DEF_ERR(NI_ID_AISP, EN_ERR_LEVEL_ERROR, ERR_NOMEM)
#define NI_ERR_AISP_NOBUF             NI_DEF_ERR(NI_ID_AISP, EN_ERR_LEVEL_ERROR, ERR_NOBUF)
#define NI_ERR_AISP_BUF_EMPTY         NI_DEF_ERR(NI_ID_AISP, EN_ERR_LEVEL_ERROR, ERR_BUF_EMPTY)
#define NI_ERR_AISP_BUF_FULL          NI_DEF_ERR(NI_ID_AISP, EN_ERR_LEVEL_ERROR, ERR_BUF_FULL)
#define NI_ERR_AISP_NOT_INIT          NI_DEF_ERR(NI_ID_AISP, EN_ERR_LEVEL_ERROR, ERR_SYS_NOTREADY)
#define NI_ERR_AISP_TIME_OUT          NI_DEF_ERR(NI_ID_AISP, EN_ERR_LEVEL_ERROR, ERR_TIME_OUT)


#define AISP_VERSION_NAME_LEN               (80)

#define AISP_MAX_DEV_NUM                    (1)

#define AISP_CFG_MAX_NUM                    (4)

#define AISP_MAX_DB_NUM                     (16)

#define AWES_FMT_MAX_NUM               (2)

#define AE_FMT_MAX_NUM                 (8)
#define AE_MIN_EV_TARGET               (10)
#define AE_MAX_EV_TARGET               (900)
#define AE_ROUTE_MAX_NODES             (16)
#define AE_WIN_WT_MAX_NODES            (20)


#define AWB_SCENE_MAX_NUM               (8)
#define AWB_SPECIAL_LIGHT_SRC_NUM       (5)
#define AWB_CURVE_BIN_NUM               (20)
#define AWB_LS_ENV_LUM_THR_NODE          (2)

#define CCM_CT_NODE_NUM                  (3)


#define AISP_WT_CURVE_NODE_NUM              (5)



#define AISP_DOW_MAX_FRAME_NUM          (2)
#define AISP_SEN_MAX_REG_NUM            (20)






/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */


/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

