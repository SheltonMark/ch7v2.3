/* 
 * mdk_xsnn.h -- xsnn模块对外提供的API头文件
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */

#ifndef __MDK_XSNN_H__
#define __MDK_XSNN_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */
/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "ni_type.h"
#include "ni_comm_xsnn.h"

/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */

/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */

/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */
/*******************************************************************************
* 函数名  : NI_MDK_NN_ForwardByBin
* 描  述  : 创建NN计算任务
* 输  入  : - NnHandle    任务句柄指针
*         : - pstForwardCtrl  前处理数据
* 输  出  : NnHandle    任务句柄指针
* 返回值  :  NI_SUCCESS  : 成功
*           NI_FAILURE: 失败
*******************************************************************************/
NI_S32 NI_MDK_NN_ForwardByBin(NN_HANDLE *NnHandle,  XSNN_FORWARD_BY_BIN_CTRL_S *pstForwardCtrl, NI_BOOL bInstant);


/*******************************************************************************
* 函数名  : NI_MDK_NN_QueryResult
* 描  述  : 获取音频采集数据
* 输  入  : - enNnId        运行在那个NN核
*         : - NnHandle    任务句柄
*         : - pstWorkResult  任务结果
*         : - u32TimeOut 等待超时时间，单位毫秒,NI_TIMEOUT_NONE:不等待;NI_TIMEOUT_FOREVER:一直等待，直到有数据;其他值:等待时间
* 输  出  : 无
* 返回值  : NI_SUCCESS : 成功
*           NI_FAILURE : 失败
*******************************************************************************/
NI_S32 NI_MDK_NN_QueryResult(NN_ID_E enNnId, NN_HANDLE NnHandle, XSNN_WORK_RESULT_S *pstWorkResult, NI_U32 u32TimeoutMs);


/*******************************************************************************
* 函数名  : NI_MDK_NN_DestroyAll
* 描  述	: 销毁释放XSNN中申请的资源
* 输  入	: 
* 输  出	:
* 返回值  : NI_SUCCESS : 成功
*			NI_FAILURE : 失败
*******************************************************************************/
NI_S32 NI_MDK_NN_DestroyAll(NI_VOID);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* End of #ifndef __MDK_XSNN_H__ */

