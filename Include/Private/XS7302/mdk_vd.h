/* 
 * mdk_vd.h -- vd模块对外提供的API头文件
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */

#ifndef _MDK_VD_H_
#define _MDK_VD_H_

#include "ni_type.h"
#include "ni_comm_vd.h"
#include "ni_comm_mbuf.h"
/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */


#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */


/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */


/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */


/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */

/**
 * @description: 模式初始化
 * @param 无
 * @return {*}：0：成功，非0：失败
 */
NI_S32 NI_MDK_VD_Init();    

/**
 * @description: 模式去初始化
 * @param 无
 * @return {*}：0：成功，非0：失败
 */
NI_S32 NI_MDK_VD_DeInit();

/**
 * @description: 创建通道
 * @param {VD_CHN} VdChn：输入，通道号。取值范围[0,VD_MAX_CHN_NUM)
 * @param {VD_CHN_ATTR_S} *pstChnAttr：输入，通道属性
 * @return {*}: 0：成功，非0：失败
 */ 
NI_S32 NI_MDK_VD_CreateChn(VD_CHN VdChn, const VD_CHN_ATTR_S *pstChnAttr);

/**
 * @description: 销毁通道
 * @param {VD_CHN} VdChn：输入，通道号。取值范围[0,VD_MAX_CHN_NUM)
 * @return {*}: 0：成功，非0：失败
 */ 
NI_S32 NI_MDK_VD_DestroyChn(VD_CHN VdChn);

/**
 * @description: 开启检测
 * @param {VD_CHN} VdChn：输入，通道号。取值范围[0,VD_MAX_CHN_NUM)
 * @param {VD_WORK_MODE_S} *pstWorkMode：输入，工作模式
 * @return {*}: 0：成功，非0：失败
 */ 
NI_S32 NI_MDK_VD_StartDetect(VD_CHN VdChn, VD_WORK_MODE_S *pstWorkMode);

/**
 * @description: 关闭检测
 * @param {VD_CHN} VdChn：输入，通道号。取值范围[0,VD_MAX_CHN_NUM)
 * @param {VD_WORK_MODE_S} *pstWorkMode：输入，工作模式
 * @return {*}: 0：成功，非0：失败
 */ 
NI_S32 NI_MDK_VD_StopDetect(VD_CHN VdChn, VD_WORK_MODE_S *pstWorkMode);

/**
 * @description: 设置通道属性
 * @param {VD_CHN} VdChn：输入，通道号。取值范围[0,VD_MAX_CHN_NUM)
 * @param {VD_CHN_ATTR_S} *pstChnAttr：输入，通道属性
 * @return {*}: 0：成功，非0：失败
 */ 
NI_S32 NI_MDK_VD_SetChnAttr(VD_CHN VdChn, const VD_CHN_ATTR_S *pstChnAttr);

/**
 * @description: 获取通道属性
 * @param {VD_CHN} VdChn：输入，通道号。取值范围[0,VD_MAX_CHN_NUM)
 * @param {VD_CHN_ATTR_S} *pstChnAttr：输出，通道属性
 * @return {*}: 0：成功，非0：失败
 */ 
NI_S32 NI_MDK_VD_GetChnAttr(VD_CHN VdChn, VD_CHN_ATTR_S *pstChnAttr);

/**
 * @description: 设置检测区域
 * @param {VD_CHN} VdChn：输入，通道号。取值范围[0,VD_MAX_CHN_NUM)
 * @param {VD_DETECT_REGION_S} *pstDetectRegion：输入，检测区域
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VD_SetDetectRegion(VD_CHN VdChn, const VD_DETECT_REGION_S *pstDetectRegion);

/**
 * @description: 获取检测结果
 * @param {VD_CHN} VdChn：输入，通道号。取值范围[0,VD_MAX_CHN_NUM)
 * @param {VD_DATA_S} *pstVdDatan：输入，检测结果
 * @param {NI_U32} u32Timeout：输入，超时时间，单位ms
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VD_GetData(VD_CHN VdChn, VD_DATA_S *pstVdData, NI_U32 u32Timeout);

/**
 * @description: 释放检测结果
 * @param {VD_CHN} VdChn：输入，通道号。取值范围[0,VD_MAX_CHN_NUM)
 * @param {VD_DATA_S} *pstVdDatan：输入，检测结果
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VD_ReleaseData(VD_CHN VdChn, VD_DATA_S *pstVdData); 


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

