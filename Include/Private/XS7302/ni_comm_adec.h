/* 
 * ni_comm_adec.h -- adecs模块对外提供的头文件
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */


#ifndef __NI_COMM_ADEC_H__
#define __NI_COMM_ADEC_H__

#include "ni_errno.h"
#include "ni_common.h"
#include "ni_define.h"
//#include "list.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/* invlalid device ID */
#define NI_ERR_ADEC_INVALID_DEVID     NI_DEF_ERR(NI_ID_ADEC, EN_ERR_LEVEL_ERROR, ERR_INVALID_DEVID)
/* invlalid channel ID */
#define NI_ERR_ADEC_INVALID_CHNID     NI_DEF_ERR(NI_ID_ADEC, EN_ERR_LEVEL_ERROR, ERR_INVALID_CHNID)
/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define NI_ERR_ADEC_ILLEGAL_PARAM     NI_DEF_ERR(NI_ID_ADEC, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)
/* channel exists */
#define NI_ERR_ADEC_EXIST             NI_DEF_ERR(NI_ID_ADEC, EN_ERR_LEVEL_ERROR, ERR_EXIST)
/* channel unexists */
#define NI_ERR_ADEC_UNEXIST           NI_DEF_ERR(NI_ID_ADEC, EN_ERR_LEVEL_ERROR, ERR_UNEXIST)
/* using a NULL point */
#define NI_ERR_ADEC_NULL_PTR          NI_DEF_ERR(NI_ID_ADEC, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)
/* try to enable or initialize system,device or channel, before configing attribute */
#define NI_ERR_ADEC_NOT_CONFIG        NI_DEF_ERR(NI_ID_ADEC, EN_ERR_LEVEL_ERROR, ERR_NOT_CONFIG)
/* operation is not supported by NOW */
#define NI_ERR_ADEC_NOT_SUPPORT       NI_DEF_ERR(NI_ID_ADEC, EN_ERR_LEVEL_ERROR, ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change stati attribute */
#define NI_ERR_ADEC_NOT_PERM          NI_DEF_ERR(NI_ID_ADEC, EN_ERR_LEVEL_ERROR, ERR_NOT_PERM)
/* failure caused by malloc memory */
#define NI_ERR_ADEC_NOMEM             NI_DEF_ERR(NI_ID_ADEC, EN_ERR_LEVEL_ERROR, ERR_NOMEM)
/* failure caused by malloc buffer */
#define NI_ERR_ADEC_NOBUF             NI_DEF_ERR(NI_ID_ADEC, EN_ERR_LEVEL_ERROR, ERR_NOBUF)
/* no data in buffer */
#define NI_ERR_ADEC_BUF_EMPTY         NI_DEF_ERR(NI_ID_ADEC, EN_ERR_LEVEL_ERROR, ERR_BUF_EMPTY)
/* no buffer for new data */
#define NI_ERR_ADEC_BUF_FULL          NI_DEF_ERR(NI_ID_ADEC, EN_ERR_LEVEL_ERROR, ERR_BUF_FULL)
/* system is not ready,had not initialed or loaded*/
#define NI_ERR_ADEC_SYS_NOTREADY      NI_DEF_ERR(NI_ID_ADEC, EN_ERR_LEVEL_ERROR, ERR_SYS_NOTREADY)

#define NI_ERR_ADEC_NOT_DISABLE          NI_DEF_ERR(NI_ID_ADEC, EN_ERR_LEVEL_ERROR, ERR_NOT_DISABLE)


/* audio dec attributes type */
typedef struct tagAdecAttr
{
    PAYLOAD_TYPE_E  enType;                 /* dec type */
    NI_U32          u32PacketPeriod;        /* package period, ms */
}ADEC_ATTR_S;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif  /*  __NI_COMM_ADEC_H__  */

