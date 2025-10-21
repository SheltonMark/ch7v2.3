/* 
 * mdk_dvp.h -- dvp模块对外提供的API头文件
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */

#ifndef _MDK_DVP_H_
#define _MDK_DVP_H_


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "ni_type.h"
#include "ni_comm_dvp.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */


/**
 * @description: 批量处理多个命令，与硬件强相关的原生命令。Job任务和操作是本接口的上一级封装。
 * @param {DVP_CMD_ARRAY_S } *pstCmds：输入，命令操作。
 * @param {NI_U32} *pu32CmdsId：输出，非阻塞模式下返回该条命令组的非0 id号，用户可根据该号，在未来调用NI_MDK_DVP_WaitCmdsDone来等待硬件完成。非阻塞模式下不能为NULL，其他情况可设为NULL。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_DVP_DoCmds(DVP_CMD_ARRAY_S *pstCmds, NI_U32 *pu32CmdsId);

/**
 * @description: 阻塞等待命令组id号完成
 * @param {NI_U32} u32CmdsId：输入，非阻塞模式下返回该条命令组的非0 id号，用户可根据该号，在未来调用NI_MDK_DVP_WaitCmdsDone来等待硬件完成。非阻塞模式下不能为NULL，其他情况可设为NULL。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_DVP_WaitCmdsDone(NI_U32 u32CmdsId);

/**
 * @description: 设置CSC参数 非必设接口
 * @param {VP_CSC_PARAM_S} *pstCscPararm：输入，YUV转RGB转换系数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_DVP_SetCscParam(const DVP_CSC_PARAM_S *pstCscPararm);

/**
 * @description: 获取CSC参数 非必设接口
 * @param {VP_CSC_PARAM_S} *pstCscPararm：输出，YUV转RGB转换系数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_DVP_GetCscParam(DVP_CSC_PARAM_S *pstCscPararm);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

