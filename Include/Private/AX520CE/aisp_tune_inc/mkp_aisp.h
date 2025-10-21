/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _MKP_AISP_H_
#define _MKP_AISP_H_

#if ((XSCHIP_ID == 0x73120200) || (XSCHIP_ID == 0x71560100) || (XSCHIP_ID == 0x71561100) || (XSCHIP_ID == 0x73120300) || (XSCHIP_ID == 0x00100100))
#include "nial_ioctl.h"
#else
#include <linux/ioctl.h>
#endif
#include "ni_type.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define IOC_MAGIC_AISP 'y'

#define AISP_MAKE_FD(AispDev) (AispDev)
#define AISP_MAKE_DEV(fd)     (fd)

typedef enum tagIocNrAisp {
    IOC_NR_AISP_MK_DEV,
    IOC_NR_AISP_GET_VREG,
    IOC_NR_AISP_MALLOC_VREG,
    IOC_NR_AISP_FREE_VREG,

    IOC_NR_AISP_BUTT
} IOC_NR_AISP_E;

typedef enum tagAispModId {
    AISP_MOD_ID_AISP      = 0,
    AISP_MOD_ID_AE        = 1,
    AISP_MOD_ID_AWB       = 2,
    AISP_MOD_ID_CCM       = 3,
    AISP_MOD_ID_CSC       = 4,
    AISP_MOD_ID_DN        = 5,
    AISP_MOD_ID_IR_LIGHT  = 6,
    AISP_MOD_ID_YHIST     = 7,
    AISP_MOD_ID_LED_LIGHT = 8,
    AISP_MOD_ID_IRIS      = 9,
    // AISP_MOD_ID_DATA        = 10,

    AISP_MOD_ID_BUTT
} AISP_MOD_ID_E;

typedef struct tagAispVerg {
    NI_U32        u32PhyAddr;
    NI_U32        u32Len;
    AISP_MOD_ID_E enModId;
} AISP_VREG_S;

#if ((XSCHIP_ID == 0x73120200) || (XSCHIP_ID == 0x71560100) || (XSCHIP_ID == 0x71561100) || (XSCHIP_ID == 0x73120300) || (XSCHIP_ID == 0x00100100))
#else
#define NIAL_IOWR _IOWR
#define NIAL_IOW  _IOW
#endif

#define IOCTL_AISP_MAKE_DEV    NIAL_IOWR(IOC_MAGIC_AISP, IOC_NR_AISP_MK_DEV, NI_U32)
#define IOCTL_AISP_GET_VREG    NIAL_IOWR(IOC_MAGIC_AISP, IOC_NR_AISP_GET_VREG, AISP_VREG_S)
#define IOCTL_AISP_MALLOC_VREG NIAL_IOWR(IOC_MAGIC_AISP, IOC_NR_AISP_MALLOC_VREG, AISP_VREG_S)
#define IOCTL_AISP_FREE_VREG   NIAL_IOWR(IOC_MAGIC_AISP, IOC_NR_AISP_FREE_VREG, AISP_VREG_S)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
