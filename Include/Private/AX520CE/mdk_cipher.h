/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef __MDK_CIPHER_H__
#define __MDK_CIPHER_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */
#include "ni_type.h"
#include "ni_comm_cipher.h"

/*******************************************************************************
 * 函数名  : NI_MDK_CIPHER_EncryptMem
 * 描  述  : 配置参数，进行mem加密
 * 输  入  :
 * 输  出  : 无
 * 返回值  : NI_SUCCESS  : 成功
 *           NI_FAILURE  : 失败
 *******************************************************************************/
NI_S32 NI_MDK_CIPHER_EncryptMem(CIPHER_MEM_S *pstCipherMem);

/*******************************************************************************
 * 函数名  : NI_MDK_CIPHER_EncryptReg
 * 描  述  : 配置参数，进行reg加密
 * 输  入  :
 * 输  出  : 无
 * 返回值  : NI_SUCCESS  : 成功
 *           NI_FAILURE  : 失败
 *******************************************************************************/
NI_S32 NI_MDK_CIPHER_EncryptReg(CIPHER_REG_S *pstCipherReg);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* End of #ifndef __MDK_CIPHER_H__ */
