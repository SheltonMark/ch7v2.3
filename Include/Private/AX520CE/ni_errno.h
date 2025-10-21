/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef __NI_ERRNO_H__
#define __NI_ERRNO_H__

#include "ni_type.h"

#define NI_SUCCESS (0)
#define NI_FAILURE (-1)

#define NI_TIMEOUT_NONE    (0)   /* 不等待，立即返回。*/
#define NI_TIMEOUT_FOREVER (~0U) /* 一直等待直到返回 */
#define NI_ETIMEOUT        (-2)  /* 等待超时 */

#define NI_INVALID_CHN    (-1)
#define NI_INVALID_WAY    (-1)
#define NI_INVALID_LAYER  (-1)
#define NI_INVALID_DEV    (-1)
#define NI_INVALID_HANDLE (-1)

#define NI_INVALID_VALUE (-1)
#define NI_INVALID_TYPE  (-1)

#define NI_ERR_APPID (0x60000000L)

typedef enum tagErrorLevel {
    EN_ERR_LEVEL_DEBUG = 0, /* debug-level */
    EN_ERR_LEVEL_INFO,      /* informational */
    EN_ERR_LEVEL_NOTICE,    /* normal but significant condition */
    EN_ERR_LEVEL_WARNING,   /* warning conditions */
    EN_ERR_LEVEL_ERROR,     /* error conditions */
    EN_ERR_LEVEL_CRIT,      /* critical conditions */
    EN_ERR_LEVEL_ALERT,     /* action must be taken immediately */

    EN_ERR_LEVEL_BUTT
} ERR_LEVEL_E;

/******************************************************************************
|----------------------------------------------------------------|
| 1 |   APP_ID   |   MOD_ID    | ERR_LEVEL |   ERR_ID            |
|----------------------------------------------------------------|
|<--><--7bits----><----8bits---><--3bits---><------13bits------->|
******************************************************************************/
#define NI_DEF_ERR(module, level, errid) ((NI_S32)((NI_ERR_APPID) | ((module) << 16) | ((level) << 13) | (errid)))

/* NOTE! the following defined all common error code,
** all module must reserved 0~63 for their common error code
*/
typedef enum tagErrorCode {
    ERR_INVALID_DEVID = 1, /* invalid device ID  */
    ERR_INVALID_GRPID = 2, /* invalid grp ID */
    ERR_INVALID_CHNID = 3, /* invalid channel ID */

    ERR_ILLEGAL_PARAM = 4, /* at lease one parameter is illegal eg, an illegal enumeration value */
    ERR_EXIST         = 5, /* resource exists */
    ERR_UNEXIST       = 6, /* resource unexists  */

    ERR_NULL_PTR = 7, /* using a NULL pointer */

    ERR_NOT_CONFIG = 8, /* try to enable device or channel, before configing attribute */

    ERR_NOT_SUPPORT = 9,  /* operation or type is not supported */
    ERR_NOT_PERM    = 10, /* operation is not permitted eg. */

    ERR_NOMEM = 11, /* failure caused by malloc memory */
    ERR_NOBUF = 12, /* failure caused by malloc buffer */

    ERR_BUF_EMPTY = 13, /* no data in buffer */
    ERR_BUF_FULL  = 14, /* no buffer for new data */

    ERR_SYS_NOTREADY = 15, /* System is not ready,maybe not initialized or  loaded. Returning the error code when
                            * opening a device file failed. */

    ERR_INVALID_ADDR = 16, /* bad address, eg. used for copy_from_user & copy_to_user  */

    ERR_BUSY = 17, /* resource is busy, eg. */

    ERR_REQ_IRQ_FAIL = 18, /* request irq failure */

    ERR_NOT_ENABLE  = 19, /* not enable */
    ERR_NOT_DISABLE = 20, /* not disable */

    ERR_TIME_OUT = 21, /* timeout */

    ERR_BUTT = 63, /* maxium code, private error code of all modules must be greater than it  */
} ERR_CODE_E;

/*
** following is an example for defining error code of VENC module
** #define NI_ERR_VENC_INVALID_STRMID NI_DEF_ERR(NI_ID_VENC, ERR_LEVEL_ERROR, ERR_INVALID_CHNID)
**
*/

#endif /* __NI_ERRNO_H__ */
