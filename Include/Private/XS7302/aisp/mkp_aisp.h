/*******************************************************************************
 * Copyright (C) 2020-2025 ZheJiang Chipup Technology CO.,LTD.
 *
 * 文件名 : mkp_aisp.h
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



#ifndef _MKP_AISP_H_
#define _MKP_AISP_H_


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include <linux/ioctl.h>
#include "ni_type.h"


#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#define IOC_MAGIC_AISP      'y'

#define  AISP_MAKE_FD(AispDev)                (AispDev)
#define  AISP_MAKE_DEV(fd)                    (fd)


typedef enum tagIocNrAisp
{   
    IOC_NR_AISP_MK_DEV,
    IOC_NR_AISP_GET_VREG,
    IOC_NR_AISP_MALLOC_VREG,
    IOC_NR_AISP_FREE_VREG,

    IOC_NR_AISP_BUTT
} IOC_NR_AISP_E;

typedef enum tagAispModId
{
    AISP_MOD_ID_AISP        = 0,
    AISP_MOD_ID_AE          = 1,
    AISP_MOD_ID_AWB         = 2,
    AISP_MOD_ID_CCM         = 3,
    AISP_MOD_ID_CSC         = 4,
    AISP_MOD_ID_DN          = 5,
    AISP_MOD_ID_IR_LIGHT    = 6,
    AISP_MOD_ID_YHIST       = 7,
    AISP_MOD_ID_LED_LIGHT   = 8,
    AISP_MOD_ID_BUTT
}AISP_MOD_ID_E;

typedef struct tagAispVerg
{
    NI_U32                  u32PhyAddr;
    NI_U32                  u32Len;
    AISP_MOD_ID_E           enModId;
}AISP_VREG_S;


#define IOCTL_AISP_MAKE_DEV         _IOW(IOC_MAGIC_AISP, IOC_NR_AISP_MK_DEV, NI_U32)
#define IOCTL_AISP_GET_VREG         _IOWR(IOC_MAGIC_AISP, IOC_NR_AISP_GET_VREG, AISP_VREG_S)
#define IOCTL_AISP_MALLOC_VREG      _IOWR(IOC_MAGIC_AISP, IOC_NR_AISP_MALLOC_VREG, AISP_VREG_S)
#define IOCTL_AISP_FREE_VREG        _IOWR(IOC_MAGIC_AISP, IOC_NR_AISP_FREE_VREG, AISP_VREG_S)

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

