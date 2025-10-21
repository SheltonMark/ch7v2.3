/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef __NI_COMM_XSNN_H__
#define __NI_COMM_XSNN_H__

#include "ni_errno.h"
#include "ni_common.h"
#include "ni_define.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef enum tagXsnnErrCode {
    ERR_XSNN_REV = 0,
    ERR_XSNN_DRV_NOT_REG,
    ERR_XSNN_NULL_PTR,
    ERR_XSNN_ILLEGAL_PARAM,
    ERR_XSNN_NOMMZ,
    ERR_XSNN_NOMEM,
    ERR_XSNN_INVALID_HANDLE,
    ERR_XSNN_WAIE_RESULT_ERROR,
    ERR_XSNN_WAIE_RESULT_TIMEOUT,

    ERR_XSNN_BUTT
} XSNN_ERR_CODE_E;

#define NI_ERR_XSNN_DRV_NOT_REG         NI_DEF_ERR(NI_ID_XSNN, EN_ERR_LEVEL_ERROR, ERR_XSNN_DRV_NOT_REG)
#define NI_ERR_XSNN_NULL_PTR            NI_DEF_ERR(NI_ID_XSNN, EN_ERR_LEVEL_ERROR, ERR_XSNN_NULL_PTR)
#define NI_ERR_XSNN_ILLEGAL_PARAM       NI_DEF_ERR(NI_ID_XSNN, EN_ERR_LEVEL_ERROR, ERR_XSNN_ILLEGAL_PARAM)
#define NI_ERR_XSNN_NOMMZ               NI_DEF_ERR(NI_ID_XSNN, EN_ERR_LEVEL_ERROR, ERR_XSNN_NOMMZ)
#define NI_ERR_XSNN_NOMEM               NI_DEF_ERR(NI_ID_XSNN, EN_ERR_LEVEL_ERROR, ERR_XSNN_NOMEM)
#define NI_ERR_XSNN_INVALID_HANDLE      NI_DEF_ERR(NI_ID_XSNN, EN_ERR_LEVEL_ERROR, ERR_XSNN_INVALID_HANDLE)
#define NI_ERR_XSNN_WAIE_RESULT_ERROR   NI_DEF_ERR(NI_ID_XSNN, EN_ERR_LEVEL_ERROR, ERR_XSNN_WAIE_RESULT_ERROR)
#define NI_ERR_XSNN_WAIE_RESULT_TIMEOUT NI_DEF_ERR(NI_ID_XSNN, EN_ERR_LEVEL_ERROR, ERR_XSNN_WAIE_RESULT_TIMEOUT)

#define XSNN_MAX_DST_ADDR_NUM (4)

typedef enum tagXsnnCoreId {
    XSNN_CORE_ID_0,   /* xsnn core 0 */
    XSNN_CORE_ID_BUTT /* 用于标记枚举的结束，无特殊意义 */
} XSNN_CORE_ID_E;

typedef struct tagXsnnMmzBuf {
    NI_VOID *pvVirtAddr; /* 虚拟地址 */
    NI_U32   u32PhyAddr; /* 物理地址 */
    NI_U32   u32BufLen;  /* MMZ大小 */
    NI_U32   u32DataLen; /* 有效数据大小 */
} XSNN_MMZ_BUF_S;

typedef struct tagXsnnForwardByBin {
    XSNN_CORE_ID_E enNnId;                            /* 任务运行的CoreId */
    NI_U32         u32NnLayerNum;                     /* cnn layer 层数信息*/
    XSNN_MMZ_BUF_S stSrcAddr;                         /* 需要进行加速计算的源信息 */
    XSNN_MMZ_BUF_S stCoefAddr;                        /* cnn layer 配置信息存放的地址*/
    XSNN_MMZ_BUF_S stWtAddr;                          /* cnn 权重系数信息存放的地址*/
    XSNN_MMZ_BUF_S stMidAddr;                         /* cnn 中间数据存放的地址*/
    XSNN_MMZ_BUF_S astDstAddr[XSNN_MAX_DST_ADDR_NUM]; /* 计算后的结果信息 */
} XSNN_FORWARD_BY_BIN_CTRL_S;

typedef enum tagXsnnWorkStatus {
    XSNN_WORK_STATUS_DONE,    /* 当前任务已经计算完成，表示计算结果有效 */
    XSNN_WORK_STATUS_TIMEOUT, /* 当前任务已经计算完成，表示硬件计算超时，计算结果无效 */

    XSNN_WORK_STATUS_BUTT /* 用于标记枚举的结束，无特殊意义 */
} XSNN_WORK_STATUS_E;

typedef struct tagXsnnWorkResult {
    XSNN_WORK_STATUS_E enWorkResult; /* 查询已经发送的任务完成状态 */
    NI_U64             u64RunTime;   /* 任务成功完成时硬件计算花费的时间 */
} XSNN_WORK_RESULT_S;

/*********************************************************************************************/
/* 模型文件数据结构 所有结构体要求16字节对齐 */
#define XSNN_MAX_OUTPUT_NUM (4)

#define XSNN_SIZEOF_CHECK_EQ(dataType, nBytes) \
    typedef char SC_EQ_##dataType[(sizeof(dataType) == (nBytes)) ? 1 : -1];

typedef enum tagXsnnModeHeadType {
    XSNN_MODEL_HEAD_TYPE_NULL,
    XSNN_MODEL_HEAD_TYPE_VER,
    XSNN_MODEL_HEAD_TYPE_NN_NET,
    XSNN_MODEL_HEAD_TYPE_NN_PCD,
    XSNN_MODEL_HEAD_TYPE_NN_HD,
    XSNN_MODEL_HEAD_TYPE_NN_HC,
    XSNN_MODEL_HEAD_TYPE_NN_LABEL_MAP,
    XSNN_MODEL_HEAD_TYPE_NN_LABEL_MAP_PCD,
    XSNN_MODEL_HEAD_TYPE_BUTT,
} XSNN_MODEL_HEAD_TYPE_E;

/*************************************************/
typedef struct tagXsnnLabelMap {
    NI_U16 u16SrcLable; /* 0x00   */
    NI_U16 u16DstLable; /* 0x02   */
    NI_U8  pu8Rev[0];   /* 0x04 无特殊意义，仅用于标记该结构体的大小 */
} XSNN_LABEL_MAP_S;
XSNN_SIZEOF_CHECK_EQ(XSNN_LABEL_MAP_S, 4);

typedef struct tagXsnnLabelMapInfo {
    NI_U32  u32Crc;      /* 0x00   */
    NI_U32  u32ByteSize; /* 0x04   */
    NI_U32  u32DataSize; /* 0x08   */
    NI_CHAR aszName[16]; /* 0x0c   */
    NI_U32  u32MapNum;   /* 0x1c   */
    NI_U32  au32Rev[4];  /* 0x20   */
    NI_U8   pu8Rev[0];   /* 0x30 无特殊意义，仅用于标记该结构体的大小 */
} XSNN_LABEL_MAP_INFO_S;
XSNN_SIZEOF_CHECK_EQ(XSNN_LABEL_MAP_INFO_S, 48);

/*************************************************/
typedef struct tagXsnnNetInput {
    NI_U16 u16NetInputW; /* 0x0  tensor 的 W */
    NI_U16 u16NetInputH; /* 0x2  tensor 的 H */
    NI_U16 u16ChnNum;    /* 0x4   */
    NI_U16 u16Rev;       /* 0x6   */
    NI_U32 au32Rev[14];  /* 0x8   */
    NI_U8  pu8Rev[0];    /* 0x40 无特殊意义，仅用于标记该结构体的大小 */
} XSNN_NET_INPUT_S;
XSNN_SIZEOF_CHECK_EQ(XSNN_NET_INPUT_S, 64);

typedef struct tagXsnnNetOutputSigle {
    NI_S32 s32Wt;       /* 0x0 */
    NI_S32 s32Ht;       /* 0x4 */
    NI_S32 s32Ft;       /* 0x8 */
    NI_S32 s32DataSize; /* 0xc 数据大小，用于内部计算时进行内存申请*/
    NI_U8  pu8Rev[0];   /* 0x10 无特殊意义，仅用于标记该结构体的大小 */
} XSNN_NET_OUTPUT_SIGLE_S;
XSNN_SIZEOF_CHECK_EQ(XSNN_NET_OUTPUT_SIGLE_S, 16);

typedef struct tagXsnnNetOutput {
    NI_U32                  u32Num;                      /* 0x0 */
    XSNN_NET_OUTPUT_SIGLE_S astOut[XSNN_MAX_OUTPUT_NUM]; /* 0x4 最大支持4个输出 */
    NI_U32                  au32Rev[3];                  /* 0x44   */
    NI_U8                   pu8Rev[0];                   /* 0x50 无特殊意义，仅用于标记该结构体的大小 */
} XSNN_NET_OUTPUT_S;
XSNN_SIZEOF_CHECK_EQ(XSNN_NET_OUTPUT_S, 80);

/* 模型网络信息 */
typedef struct tagXsnnNetInfo {
    NI_U32            u32Crc;          /* 0x00   */
    NI_U32            u32ByteSize;     /* 0x04  XSNN_NET_INFO_S 结构体大小 */
    NI_U32            u32DataSize;     /* 0x08  模型长度 大小为 : XSNN_NET_INFO_S + 模型大小 */
    NI_CHAR           aszName[16];     /* 0x0c   */
    XSNN_NET_INPUT_S  stInput;         /* 0x1c 算法输入 */
    XSNN_NET_OUTPUT_S stOutput;        /* 0x5c 算法输出 */
    NI_U16            u16NnLayerNum;   /* 0xac 层数 */
    NI_U16            u16Branchs;      /* 0xae  */
    NI_U16            u16IsAlign;      /* 0xb0  */
    NI_U16            u16ProcMode;     /* 0xb2 后处理处理类型 */
    NI_U32            u32MiddleSize;   /* 0xb4 中间缓冲大小 */
    NI_U32            u32ModelCfgAddr; /* 0xb8 模型文件地址 相对于本结构体的可变参数*/
    NI_U32            u32ModelCfgSize; /* 0xbc 模型文件大小 */
    NI_U32            u32CfgCrc;       /* 0xc0  */

    NI_U32 u32ModelWtAddr; /* 0xc4 权重文件地址 */
    NI_U32 u32ModelWtSize; /* 0xc8 权重文件大小 */
    NI_U32 u32WtCrc;       /* 0xcc  */

    NI_U32 au32Rev1[12];  /* 0xd0 预留 */
    NI_U8  pu8Payload[0]; /* 0x100 可变空间，存放模型文件内容 */

    /* pu8Payload 后续存放顺序为 conf + weight */
} XSNN_NET_INFO_S;
XSNN_SIZEOF_CHECK_EQ(XSNN_NET_INFO_S, 256);

/*************************************************/
typedef struct tagXsnnModelVer {
    NI_U32  u32Crc;      /* 0x00   */
    NI_U32  u32ByteSize; /* 0x04   */
    NI_U32  u32DataSize; /* 0x08   */
    NI_CHAR aszName[16]; /* 0x0c   */
    NI_U32  au32Rev[5];  /* 0x1c   */
    NI_U8   pu8Rev[0];   /* 0x30 无特殊意义，仅用于标记该结构体的大小 */
} XSNN_MODEL_VER_S;
XSNN_SIZEOF_CHECK_EQ(XSNN_MODEL_VER_S, 48);

/*************************************************/
typedef struct tagXsnnModelHead {
    NI_U32                 u32Crc;      /* 0x00   */
    XSNN_MODEL_HEAD_TYPE_E enType;      /* 0x04   */
    NI_U32                 u32ByteSize; /* 0x08   XSNN_MODEL_HEAD_S 结构体大小 */
    NI_U32                 u32Size;     /* 0x0c   */
    NI_U32                 u32Addr;     /* 0x10   */
    NI_U32                 au32Rev[11]; /* 0x14   */
    NI_U8                  pu8Rev[0];   /* 0x40 无特殊意义，仅用于标记该结构体的大小 */
} XSNN_MODEL_HEAD_S;
XSNN_SIZEOF_CHECK_EQ(XSNN_MODEL_HEAD_S, 64);

/*************************************************/
typedef struct tagXsnnModelGrpHead {
    NI_U32 u32Crc;        /* 0x00   整个模型CRC值 */
    NI_U32 u32Version;    /* 0x04   版本号  */
    NI_U32 u32ByteSize;   /* 0x08   XSNN_MODEL_GRP_S 结构体大小 */
    NI_U32 u32TotalLen;   /* 0x0c   整个模型文件大小 */
    NI_U16 u16Chipher;    /* 0x10   0-不加密, 1-加密算法1 */
    NI_U16 u16PayloadNum; /* 0x12   内容总数量 */
    NI_U32 au32Rev[11];   /* 0x14   对齐使用  */
    NI_U8  pu8Payload[0]; /* 0x40   存放顺序为各个模型文件的首地址 */

    /* pu8Payload 后存放 u16PayloadNum 个 XSNN_MODEL_HEAD_S 数据
      u16PayloadNum * XSNN_MODEL_HEAD_S
    + XSNN_MODEL_VER_S
    + XSNN_NET_INFO_S
    + XSNN_LABEL_MAP_S
    + ...
    */
} XSNN_MODEL_GRP_HEAD_S;
XSNN_SIZEOF_CHECK_EQ(XSNN_MODEL_GRP_HEAD_S, 64);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*  __NI_COMM_XSNN_H__  */
