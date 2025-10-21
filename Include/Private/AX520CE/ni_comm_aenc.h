/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef __NI_COMM_AENC_H__
#define __NI_COMM_AENC_H__

#include "ni_errno.h"
#include "ni_common.h"
#include "ni_define.h"
#include "nial.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef enum tagAencErrCode {
    ERR_AENC_HAS_STARTED        = 64, /* device or channel has been started*/
    ERR_AENC_NOT_START          = 65, /* device or channel not start */
    ERR_AENC_BYPASS_CHN_UNSTART = 66
} AENC_ERR_CODE_E;

/* invlalid device ID */
#define NI_ERR_AENC_INVALID_DEVID NI_DEF_ERR(NI_ID_AENC, EN_ERR_LEVEL_ERROR, ERR_INVALID_DEVID)
/* invlalid channel ID */
#define NI_ERR_AENC_INVALID_CHNID NI_DEF_ERR(NI_ID_AENC, EN_ERR_LEVEL_ERROR, ERR_INVALID_CHNID)
/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define NI_ERR_AENC_ILLEGAL_PARAM NI_DEF_ERR(NI_ID_AENC, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)
/* channel exists */
#define NI_ERR_AENC_CHN_EXIST NI_DEF_ERR(NI_ID_AENC, EN_ERR_LEVEL_ERROR, ERR_EXIST)
/* channel unexists */
#define NI_ERR_AENC_CHN_UNEXIST NI_DEF_ERR(NI_ID_AENC, EN_ERR_LEVEL_ERROR, ERR_UNEXIST)
/* using a NULL point */
#define NI_ERR_AENC_NULL_PTR NI_DEF_ERR(NI_ID_AENC, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)
/* try to enable or initialize system,device or channel, before configing attribute */
#define NI_ERR_AENC_NOT_CONFIG NI_DEF_ERR(NI_ID_AENC, EN_ERR_LEVEL_ERROR, ERR_NOT_CONFIG)
/* operation is not supported by NOW */
#define NI_ERR_AENC_NOT_SUPPORT NI_DEF_ERR(NI_ID_AENC, EN_ERR_LEVEL_ERROR, ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change static attribute */
#define NI_ERR_AENC_NOT_PERM NI_DEF_ERR(NI_ID_AENC, EN_ERR_LEVEL_ERROR, ERR_NOT_PERM)
/* failure caused by malloc memory */
#define NI_ERR_AENC_NOMEM NI_DEF_ERR(NI_ID_AENC, EN_ERR_LEVEL_ERROR, ERR_NOMEM)
/* failure caused by malloc buffer */
#define NI_ERR_AENC_NOBUF NI_DEF_ERR(NI_ID_AENC, EN_ERR_LEVEL_ERROR, ERR_NOBUF)
/* no data in buffer */
#define NI_ERR_AENC_BUF_EMPTY NI_DEF_ERR(NI_ID_AENC, EN_ERR_LEVEL_ERROR, ERR_BUF_EMPTY)
/* no buffer for new data */
#define NI_ERR_AENC_BUF_FULL NI_DEF_ERR(NI_ID_AENC, EN_ERR_LEVEL_ERROR, ERR_BUF_FULL)
/* system is not ready,had not initialed or loaded*/
#define NI_ERR_AENC_SYS_NOTINIT NI_DEF_ERR(NI_ID_AENC, EN_ERR_LEVEL_ERROR, ERR_SYS_NOTREADY)
/* failure caused by enc has started encoding */
#define NI_ERR_AENC_CHN_HAS_STARTED NI_DEF_ERR(NI_ID_AENC, EN_ERR_LEVEL_ERROR, ERR_AENC_HAS_STARTED)
/* failure caused by enc has started encoding */
#define NI_ERR_AENC_CHN_NOT_START NI_DEF_ERR(NI_ID_AENC, EN_ERR_LEVEL_ERROR, ERR_AENC_NOT_START)

#define NI_ERR_AENC_BYPASS_CHN_UNSTART NI_DEF_ERR(NI_ID_AENC, EN_ERR_LEVEL_ERROR, ERR_AENC_BYPASS_CHN_UNSTART)

#define NI_ERR_AENC_NOT_DISABLE NI_DEF_ERR(NI_ID_AENC, EN_ERR_LEVEL_ERROR, ERR_NOT_DISABLE)

/* audio enc attributes type */
typedef struct tagAencAttr {
    PAYLOAD_TYPE_E enType;          /* enc type */
    NI_U32         u32PacketPeriod; /* package period, ms */
} AENC_ATTR_S;

typedef struct tagAudioStream {
    struct nial_list_head list;

    NI_U8 *pu8Stream;    /* the virtual address of stream */
    NI_U32 u32PhyAddr;   /* the physics address of stream */
    NI_U32 u32Len;       /* stream lenth, by bytes */
    NI_U64 u64TimeStamp; /* frame time stamp, unit: microsecond */
    NI_U32 u32Seq;       /* frame seq,if stream is not a valid frame,u32Seq is 0*/
    NI_U32 index;
} AUDIO_STREAM_S;

typedef enum tagAudio3AType {
    AUDIO_AEC = 0x1,
    AUDIO_ANS = 0x2,
    AUDIO_AGC = 0x4,  // 暂不支持
} AUDIO_3A_TYPE_E;

// 音频流信息
typedef struct tagStreamParam {
    NI_BOOL        bValid;
    PAYLOAD_TYPE_E enType;           // enc type
    NI_U32         u32PacketPeriod;  // 打包周期 ms为单位
    NI_U32         u32Depth;         // 采样位深/16位/24位
    NI_U32         u32Frequency;     // 采样率，以Hz为单位，比如16000Hz。
    AENC_CHN       encChn;           // 音频源从哪个编码通道来
    ADEC_CHN       decChn;           // 音频从哪个解码通道播放
} NI_STREAM_PARAM_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*  __NI_COMM_AENC_H__  */
