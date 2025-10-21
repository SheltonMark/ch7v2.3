/* 
 * mdk_hash.h -- hash模块对外提供的API头文件
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */

#ifndef _MKP_HASH_H_
#define _MKP_HASH_H_


/* ========================================================================== *
 * 包含头文件                                                                 *
 * ========================================================================== */
#include "nial.h"
#include "mkp_ioctl.h"
#include "ni_type.h"
#include "ni_debug.h"
#include "ni_errno.h"


#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

/* ========================================================================== */
/* 错误码                                                                     */
/* ========================================================================== */


/* 空指针错误 */
//#define NI_ERR_HASH_NULL_PTR          (ERR_NULL_PTR)    /*方案一*/

/* 映射寄存器失败，内存分配错误 */
//#define NI_ERR_HASH_NOMEM             (ERR_NOMEM)

/* 注册IRQ失败 */
//#define NI_ERR_HASH_REQUEST_IRQFAIL   NI_DEF_ERR(NI_ID_MDK, EN_ERR_LEVEL_ERROR, ERR_REQ_IRQ_FAIL)   /* 方案二*/

/* 无效的地址，消息的起始地址或消息长度有误*/
//#define NI_ERR_HASH_INVALID_ADDR      NI_DEF_ERR(NI_ID_MDK, EN_ERR_LEVEL_ERROR, ERR_INVALID_ADDR)

/* Hash配置的算法类型，HMAC使能，秘钥类型，用户秘钥配置有误 */
//#define NI_ERR_HASH_ILLEGAL_PARAM     NI_DEF_ERR(NI_ID_HASH, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM) /* 方案三*/

/* Hash超时，进程长时间无法访问硬件 */
//#define NI_ERR_HASH_TIMEOUT           NI_DEF_ERR(NI_ID_HASH, EN_ERR_LEVEL_ERROR, ERR_TIME_OUT)

/* Hash运算过程，异常结束, 出现此错误，说明硬件层面可能出现问题 */
//#define NI_ERR_HASH_FAILURE           NI_FAILURE

/* ========================================================================== */
/* 宏和类型定义区                                   */
/* ========================================================================== */

#define HASH_USER_KEY_LEN	(32) /* 用户秘钥字符数组长度 */
#define HASH_VALUE_LEN	    (32) /* Hash值字符数组长度 */

//#define HASH_MAKE_FD(HashDev)        (HashDev)

/* Hash算法*/
typedef enum tagHashAlg
{
	HASH_ALG_SHA256 = 0x002,      /* 使用SHA-256算法 */

    HASH_ALG_BUTT
}HASH_ALG_E;

/* Hash秘钥类型*/
typedef enum tagHashKeyType
{
	HASH_KEY_TYPE_HARD = 0,  
	HASH_KEY_TYPE_USER,
	
    HASH_KEY_TYPE_BUTT 
}HASH_KEY_TYPE_E;

/* ========================================================================== *
 * 数据结构定义区                                                             *
 * ========================================================================== */

/* Hash配置 */
typedef struct tagHashCfg
{
	HASH_ALG_E          enAlg;                          /* Hash算法 */            
	HASH_KEY_TYPE_E     enKeyType;                      /* 秘钥类型*/
	NI_BOOL             bHmacEnable;	                /* HMAC使能*/
    
    NI_U32              u32MsgAddr;                     /* 消息的起始地址*/  
	NI_U32              u32MsgLen;	                    /* 消息的长度*/ 

    NI_CHAR             acUserkey[HASH_USER_KEY_LEN];   /* 用户秘钥字符数组*/
} HASH_CFG_S;

/* ========================================================================== *
 * IOCTL 相关结构定义                                                         *
 * ========================================================================== */

/* Hash计算参数，用于在用户层和驱动层之间传递参数 */
typedef struct tagHashCalcHashParam
{
    const HASH_CFG_S    *pstHashCfg;
    NI_CHAR             *pcHashValue;
}HASH_CALC_HASH_PARAM_S;

/* IOCTL命令编号 */
typedef enum tagIocNrHash
{  
//    IOC_NR_HASH_MAKE_DEV  = 0,
    IOC_NR_HASH_CALC_HASH = 0,
        
    IOC_NR_HASH_BUTT
} IOC_NR_HASH_E;

/* 构造IOCTL命令 */

//#define IOCTL_HASH_MAKE_DEV               _IOW(IOC_MAGIC_HASH, IOC_NR_HASH_MAKE_DEV, HASH_CALC_HASH_PARAM_S)
#define IOCTL_HASH_CALC_HASH              _IOW(IOC_MAGIC_HASH, IOC_NR_HASH_CALC_HASH, HASH_CALC_HASH_PARAM_S)

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

