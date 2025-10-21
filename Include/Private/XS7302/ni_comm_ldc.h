/*******************************************************************************
 * Copyright (C) 2020-2025 ZheJiang Chipup Technology CO.,LTD.
 *
 * 文件名 : ni_comm_ldc.h
 *
 * 作  者 : Chipup Software Team
 * 版  本 : V1.0.0
 * 日  期 : 2020年9月8日 Create
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
 * 日    期: 
 * 修订版本:
 * 作    者:
 * 修订备注:
 *******************************************************************************/

#ifndef __NI_COMM_LDC_H__
#define __NI_COMM_LDC_H__


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "ni_type.h"
#include "ni_errno.h"
#include "ni_common_video.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */


/* ========================================================================== */
/*                         宏和类型定义区                                     */
/* ========================================================================== */

/* invlalid device ID */
#define NI_ERR_LDC_INVALID_DEVID     NI_DEF_ERR(NI_ID_LDC, EN_ERR_LEVEL_ERROR, ERR_INVALID_DEVID)
/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define NI_ERR_LDC_ILLEGAL_PARAM     NI_DEF_ERR(NI_ID_LDC, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)
/* using a NULL point */
#define NI_ERR_LDC_NULL_PTR          NI_DEF_ERR(NI_ID_LDC, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)
/* try to enable or initialize system,device or channel, before configing attribute */
#define NI_ERR_LDC_NOT_CONFIG        NI_DEF_ERR(NI_ID_LDC, EN_ERR_LEVEL_ERROR, ERR_NOT_CONFIG)
/* operation is not supported by NOW */
#define NI_ERR_LDC_NOT_SUPPORT       NI_DEF_ERR(NI_ID_LDC, EN_ERR_LEVEL_ERROR, ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change static attribute */
#define NI_ERR_LDC_NOT_PERM          NI_DEF_ERR(NI_ID_LDC, EN_ERR_LEVEL_ERROR, ERR_NOT_PERM)
/* failure caused by malloc memory */
#define NI_ERR_LDC_NOMEM             NI_DEF_ERR(NI_ID_LDC, EN_ERR_LEVEL_ERROR, ERR_NOMEM)
/* failure caused by malloc buffer */
#define NI_ERR_LDC_NOBUF             NI_DEF_ERR(NI_ID_LDC, EN_ERR_LEVEL_ERROR, ERR_NOBUF)
/* no data in buffer */
#define NI_ERR_LDC_BUF_EMPTY         NI_DEF_ERR(NI_ID_LDC, EN_ERR_LEVEL_ERROR, ERR_BUF_EMPTY)
/* no buffer for new data */
#define NI_ERR_LDC_BUF_FULL          NI_DEF_ERR(NI_ID_LDC, EN_ERR_LEVEL_ERROR, ERR_BUF_FULL)
/* failure caused by request irq */
#define NI_ERR_LDC_REQUEST_IRQFAIL   NI_DEF_ERR(NI_ID_LDC, EN_ERR_LEVEL_ERROR, ERR_REQ_IRQ_FAIL)
/* failure caused by VI dev not enable */
#define NI_ERR_LDC_DEV_NOT_ENABLED   NI_DEF_ERR(NI_ID_LDC, EN_ERR_LEVEL_ERROR, ERR_NOT_ENABLE)
/* failure caused by VI dev not disable */
#define NI_ERR_LDC_DEV_NOT_DISABLE   NI_DEF_ERR(NI_ID_LDC, EN_ERR_LEVEL_ERROR, ERR_NOT_DISABLE)
/* failure caused by timeout */
#define NI_ERR_LDC_TIMEOUT  NI_DEF_ERR(NI_ID_LDC, EN_ERR_LEVEL_ERROR, ERR_TIME_OUT)



typedef struct tagLdcDevAttr
{
    NI_U32      u32MapStride;           /* map表stride*/
    NI_U32      u32MapPhy;              /* map表的物理地址，需16字节对齐*/
    NI_U32      u32MapInfoSize;         /* map info表的总大小，单位字节*/
    NI_U32      u32MapInfoPhy;          /* map info表的物理地址，需16字节对齐*/
    NI_U32      u32Width;               /* 分辨率，目前支持如下分辨率：
                                          2688*1944、2688*1520、1920*1080、1280*720、
                                          3840*2160、4096*2160、2560*1440、2048*1536、
                                          720*480*/
    NI_U32      u32Height;
}LDC_DEV_ATTR_S;

typedef enum tagLdcFrmStatus
{
    LDC_FRM_NOT_PROCESSED = 0,
    LDC_FRM_PROCESS_SUCCESS,
    LDC_FRM_PROCESS_TIMEOUT,
    LDC_FRM_PROCESS_INTERRUPT,
    LDC_FRM_PROCESS_CANCEL
}LDC_FRM_STATUS_E;

typedef struct tagLdcCorrectParam
{
    VIDEO_FRAME_S       stInFrame;
    VIDEO_FRAME_S       stOutFrame;
    LDC_FRM_STATUS_E    enStatus;           /* 当前帧的处理状态 */
}LDC_CORRECT_PARAM_S;

typedef struct tagLdcCorrectParamEx
{
    VIDEO_FRAME_S       stInFrame;          /* 矫正数据源 */
    VIDEO_FRAME_S       stOutFrame;         /* 矫正后数据存储地址 */
    
    NI_U32              u32MapStride;       /* map表stride*/
    NI_U32              u32MapPhy;          /* map表的物理地址，需16字节对齐*/
    NI_U32              u32MapInfoSize;     /* map info表的总大小，单位字节*/
    NI_U32              u32MapInfoPhy;      /* map info表的物理地址，需16字节对齐*/
    
    LDC_FRM_STATUS_E    enStatus;           /* 当前帧的处理状态 */
}LDC_CORRECT_PARAM_EX_S;



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of #ifndef __NI_COMM_LDC_H__ */


