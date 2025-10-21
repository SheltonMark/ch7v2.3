/* 
 * mdk_rotate.h -- rotate模块对外提供的API头文件
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */

#ifndef _MDK_ROTATE_H_
#define _MDK_ROTATE_H_


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "ni_comm_rotate.h"
#include "ni_common_video.h"


#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

/**
 * @description: 单次旋转操作接口
 * @param {ROTATE_ATTR_S} *pstRotateAttr：输入，旋转图像参数
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_ROTATE_DoRotate(ROTATE_ATTR_S *pstRotateAttr);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

