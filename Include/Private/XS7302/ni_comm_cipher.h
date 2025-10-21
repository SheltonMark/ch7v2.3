/* 
 * ni_comm_cipher.h -- cipher模块对外提供的头文件
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */


#ifndef __NI_COMM_CIPHER_H__
#define __NI_COMM_CIPHER_H__


#include "ni_errno.h"
#include "ni_common.h"
#include "ni_define.h"
//#include "list.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/* using a NULL point */
#define NI_ERR_CIPHER_NULL_PTR          NI_DEF_ERR(NI_ID_CIPHER, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)
/* system is not ready,had not initialed or loaded*/
#define NI_ERR_CIPHER_SYS_NOTINIT       NI_DEF_ERR(NI_ID_CIPHER, EN_ERR_LEVEL_ERROR, ERR_SYS_NOTREADY)
/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define NI_ERR_CIPHER_ILLEGAL_PARAM     NI_DEF_ERR(NI_ID_CIPHER, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)
/* failure caused by request irq */
#define NI_ERR_CIPHER_REQUEST_IRQFAIL   NI_DEF_ERR(NI_ID_CIPHER, EN_ERR_LEVEL_ERROR, ERR_REQ_IRQ_FAIL)
/* encrypt time out*/
#define NI_ERR_CIPHER_TIME_OUT          NI_DEF_ERR(NI_ID_CIPHER, EN_ERR_LEVEL_ERROR, ERR_TIME_OUT)


#define CIPHER_MAX_KEY_ARRAY    (4)
#define CIPHER_MAX_REG_NUM      (4)

typedef enum tagCipherAlg
{
    CIPHER_ALG_AES = 0,
    CIPHER_ALG_DES = 1,
}CIPHER_ALG_E;

typedef enum tagCipherMod
{
    CIPHER_MOD_MEM = 0,
    CIPHER_MOD_REG = 1,
}CIPHER_MOD_E;

typedef struct tagCipherMem
{
    CIPHER_ALG_E        enCipherAlg;       /* 加密算法选择 */
    NI_U32              u32SrcPhyAddr;     /* 源数据地址 */
    NI_U32              u32DstPhyAddr;     /* 写数据目的地址 */
    NI_U32              u32DataLength;     /* 数据总长度 */
    NI_U32              u32CipherLength;   /* 加密数据长度，u32DataLength - u32CipherLength为不加密的数据长度 */   
    
    NI_U32              au32Key[CIPHER_MAX_KEY_ARRAY];       /* 加密密钥 */      
}CIPHER_MEM_S;

typedef struct tagCipherReg
{	   
    CIPHER_ALG_E        enCipherAlg;                        /* 加密算法选择 */
    NI_U32              au32CipherReg[CIPHER_MAX_REG_NUM];  /* 待加密寄存器 */
    NI_U32              au32Key[CIPHER_MAX_KEY_ARRAY];      /* 加密密钥 */    
}CIPHER_REG_S;



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif  /*  __NI_COMM_CIO_H__  */
