/* 
 * ni_comm_rotate.h -- rotate模块对外提供的头文件
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */



#ifndef _NI_COMM_ROTATE_H_
#define _NI_COMM_ROTATE_H_


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */


#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#include "ni_errno.h"
#include "ni_common_video.h"


/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */
/*invlalid device ID*/
#define NI_ERR_ROTATE_INVALID_DEVID     NI_DEF_ERR(NI_ID_ROTATE, EN_ERR_LEVEL_ERROR, ERR_INVALID_DEVID)
/*invlalid channel ID*/
#define NI_ERR_ROTATE_INVALID_CHNID     NI_DEF_ERR(NI_ID_ROTATE, EN_ERR_LEVEL_ERROR, ERR_INVALID_CHNID)
/*at lease one parameter is illagal ,eg, an illegal enumeration value*/
#define NI_ERR_ROTATE_ILLEGAL_PARAM     NI_DEF_ERR(NI_ID_ROTATE, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)
/*lib exists*/
#define NI_ERR_ROTATE_LIB_EXIST         NI_DEF_ERR(NI_ID_ROTATE, EN_ERR_LEVEL_ERROR, ERR_EXIST)
/*lib unexists*/
#define NI_ERR_ROTATE_LIB_UNEXIST       NI_DEF_ERR(NI_ID_ROTATE, EN_ERR_LEVEL_ERROR, ERR_UNEXIST)
/*using a NULL point*/
#define NI_ERR_ROTATE_NULL_PTR          NI_DEF_ERR(NI_ID_ROTATE, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)
/*try to enable or initialize system,device or channel, before configing attribute*/
#define NI_ERR_ROTATE_NOT_CONFIG        NI_DEF_ERR(NI_ID_ROTATE, EN_ERR_LEVEL_ERROR, ERR_NOT_CONFIG)
/*operation is not supported by NOW*/
#define NI_ERR_ROTATE_NOT_SUPPORT       NI_DEF_ERR(NI_ID_ROTATE, EN_ERR_LEVEL_ERROR, ERR_NOT_SUPPORT)
/*operation is not permitted ,eg, try to change static attribute*/
#define NI_ERR_ROTATE_NOT_PERM          NI_DEF_ERR(NI_ID_ROTATE, EN_ERR_LEVEL_ERROR, ERR_NOT_PERM)
/*failure caused by malloc memory*/
#define NI_ERR_ROTATE_NOMEM             NI_DEF_ERR(NI_ID_ROTATE, EN_ERR_LEVEL_ERROR, ERR_NOMEM)
/*failure caused by malloc buffer*/
#define NI_ERR_ROTATE_NOBUF             NI_DEF_ERR(NI_ID_ROTATE, EN_ERR_LEVEL_ERROR, ERR_NOBUF)
/*no data in buffer*/
#define NI_ERR_ROTATE_BUF_EMPTY         NI_DEF_ERR(NI_ID_ROTATE, EN_ERR_LEVEL_ERROR, ERR_BUF_EMPTY)
/*no buffer for new data*/
#define NI_ERR_ROTATE_BUF_FULL          NI_DEF_ERR(NI_ID_ROTATE, EN_ERR_LEVEL_ERROR, ERR_BUF_FULL)
/*not initialize*/
#define NI_ERR_ROTATE_NOT_INIT          NI_DEF_ERR(NI_ID_ROTATE, EN_ERR_LEVEL_ERROR, ERR_SYS_NOTREADY)

#define NI_ERR_ROTATE_REQUEST_IRQFAIL   NI_DEF_ERR(NI_ID_ROTATE, EN_ERR_LEVEL_ERROR, ERR_REQ_IRQ_FAIL)

#define NI_ERR_ROTATE_TIMEOUT           NI_DEF_ERR(NI_ID_ROTATE, EN_ERR_LEVEL_ERROR, ERR_TIME_OUT)


#if 0
/*源图像类型*/
typedef enum tagSrcType
{
    SRC_TYPE_RGB1 = 0,
    SRC_TYPE_NV12 = 1,
    SRC_TYPE_NV21 = 2,
    SRC_TYPR_IYUV = 3,
    SRC_TYPE_YUV4 = 4,
    SRC_TYPE_U8   = 5,

    SRC_TYPE_BUTT,
}SRC_TYPE_E;

typedef struct tagRotateIntStat
{
    NI_BOOL bIntErr;          /*中断未清除又接收到新的中断*/
    NI_BOOL bDstErr;          /*结果图像宽度大于配置的STRIDE*/
    NI_BOOL bSrcErr;          /*源图像宽度大于配置的STRIDE*/
    NI_BOOL bStrideErr;       /*图像（源或者目标）存在STRIDE非16对齐*/
    NI_BOOL bSrcSizeErr;      /*图像宽或高非偶数*/
    NI_BOOL bStartErr;        /*非IDLE状态接收到写start寄存器*/ 
    NI_BOOL bTimeOut;         /*超时中断发生*/
    NI_BOOL bNormal;          /*正常结束*/
}ROTATE_INT_STAT_S; 
#endif

typedef enum tagIdleState
{
    STATE_BUSY = 0,
    STATE_IDLE = 1,

    STATE_BUTT,
}IDLE_STATE_E;


/*旋转、翻转类型*/
typedef enum tagRotateType
{
	ROTATE_BYPASS    = 0, //BYPASS
	ROTATE_MIRROR    = 1, //水平镜像
	ROTATE_FLIP      = 2, //垂直翻转
	ROTATE_90        = 3, //90度旋转
	ROTATE_180       = 4, //180度旋转
	ROTATE_270       = 5, //270度旋转
	ROTATE_MIRROR_90 = 6, //水平翻转+90度旋转
	ROTATE_FLIP_90   = 7, //垂直翻转+90度旋转

    ROTATE_BUTT
}ROTATE_TYPE_E;


typedef struct tagRotateAttr
{
    PIXEL_FORMAT_E enSrcType;          /* 源图像类型 */
    ROTATE_TYPE_E  enRotateType;       /* 旋转翻转类型 */
    NI_U32         u32SrcWidth;        /* 源图像宽度 最大8192 */
    NI_U32         u32SrcHeight;       /* 源图像高度 最大8192 */
    NI_U32         au32SrcAddr[3];     /* 源图像平面x基地址, 16字节对齐 */
    NI_U32         au32SrcStride[3];   /* 源图像平面xStride 最大4*8192 */
    NI_U32         au32DstAddr[3];     /* 结果图像平面x基地址 16字节对齐 */
    NI_U32         au32DstStride[3];   /* 结果图像平面xStride 最大4*8192 */
}ROTATE_ATTR_S;

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

