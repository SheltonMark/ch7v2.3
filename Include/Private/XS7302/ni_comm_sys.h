/* 
 * ni_comm_sys.h -- sys模块对外提供的头文件
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */


#ifndef _NI_COMM_SYS_H_
#define _NI_COMM_SYS_H_


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
typedef enum tagSysErrCode
{
    ERR_SYS_UNMAP_FAIL = 64,
    ERR_SYS_FREE_FAIL  = 65,

}SYS_ERR_CODE_E;

/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define NI_ERR_SYS_ILLEGAL_PARAM        NI_DEF_ERR(NI_ID_SYS, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)
/* using a NULL point */
#define NI_ERR_SYS_NULL_PTR             NI_DEF_ERR(NI_ID_SYS, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)
/* try to enable or initialize system,device or channel, before configing attribute */
#define NI_ERR_SYS_NOT_CONFIG           NI_DEF_ERR(NI_ID_SYS, EN_ERR_LEVEL_ERROR, ERR_NOT_CONFIG)
/* operation is not supported by NOW */
#define NI_ERR_SYS_NOT_SUPPORT          NI_DEF_ERR(NI_ID_SYS, EN_ERR_LEVEL_ERROR, ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change static attribute */
#define NI_ERR_SYS_NOT_PERM             NI_DEF_ERR(NI_ID_SYS, EN_ERR_LEVEL_ERROR, ERR_NOT_PERM)
/* failure caused by malloc buffer */
#define NI_ERR_SYS_NOBUF                NI_DEF_ERR(NI_ID_SYS, EN_ERR_LEVEL_ERROR, ERR_NOBUF)
/* no data in buffer */
#define NI_ERR_SYS_EMPTY                NI_DEF_ERR(NI_ID_SYS, EN_ERR_LEVEL_ERROR, ERR_BUF_EMPTY)
/* not initialize */
#define NI_ERR_SYS_NOT_INIT             NI_DEF_ERR(NI_ID_SYS, EN_ERR_LEVEL_ERROR, ERR_SYS_NOTREADY)
/* has initialize */
#define NI_ERR_SYS_HAS_INITED           NI_DEF_ERR(NI_ID_SYS, EN_ERR_LEVEL_ERROR, ERR_EXIST)
/* unmap fail */
#define NI_ERR_SYS_UNMAP_FAIL           NI_DEF_ERR(NI_ID_SYS, EN_ERR_LEVEL_ERROR, ERR_SYS_UNMAP_FAIL)
/* free fail */
#define NI_ERR_SYS_FREE_FAIL            NI_DEF_ERR(NI_ID_SYS, EN_ERR_LEVEL_ERROR, ERR_SYS_FREE_FAIL)


/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */
typedef struct tagMdkSysConf
{
    /* stride of picture buffer must be aligned with this value.
     * you can choose a value from 1 to 1024, and it must be multiple of 16.
     */
    NI_U32 u32AlignWidth;  

}MDK_SYS_CONF_S;

typedef struct tagSysChipHwInfo
{
    /*
    * supported list: "XS7320","XS7320A","XS7320B","XS7310","XS7310A",
    * "XS7302","XS7312","XS7312Q"
    */
    NI_CHAR szChipId[16];

    NI_CHAR reserved[32];
}SYS_CHIP_HWINFO_S;


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

