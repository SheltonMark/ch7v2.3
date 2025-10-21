/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef __NI_IAS_FALL_DETECT_H__
#define __NI_IAS_FALL_DETECT_H__

#include "ni_type.h"
#include "ni_debug.h"
#include "ni_common_video.h"
#include "ni_ias_define.h"
#include "ni_ias_common.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ========================================================================== */
/*                           宏定义区                                         */
/* ========================================================================== */
#define IAS_FAD_MAX_OBJ_NUM   (50)
#define IAS_FAD_MAX_PARAM_NUM (8)
#define IAS_FAD_NODE_NAME_LEN (128)

/* ========================================================================== */
/*                           结构体定义区                                     */
/* ========================================================================== */
typedef enum tagAlgFaDModelType {
    IAS_FAD_MODEL_READ_FROM_MEM  = 0, /* 从内存读取模型 */
    IAS_FAD_MODEL_READ_FROM_FILE = 1, /* 从文件读取模型 */
    IAS_FAD_MODEL_TYPE_BUTT,
} IAS_ALG_FAD_MODEL_TYPE_E;

typedef struct tagIasFadModelCfg {
    IAS_MODEL_CFG_S stModelCfgFad; /*FAD 算法模型*/
    IAS_MODEL_CFG_S stModelCfgFat; /*FAT 算法模型*/
    IAS_MODEL_CFG_S stModelCfgFar; /*FAR 算法模型*/
} IAS_FAD_CFG_S;

/*模型读取方式*/
typedef struct tagIasFadModel {
    IAS_ALG_FAD_MODEL_TYPE_E enType; /*模型读取类型*/
    union {
        IAS_FAD_CFG_S stFadModelCfg;                /* UNION:enType=IAS_FAD_MODEL_READ_FROM_MEM | FAD 算法模型配置 */
        NI_CHAR       acName[IAS_ALG_MAX_NAME_LEN]; /* UNION:enType=IAS_FAD_MODEL_READ_FROM_FILE | FAD 算法模型路径 */
    };
} IAS_FAD_MODEL_S;

typedef struct tagIasFarInfo {
    NI_S32 s32FallScore; /* 跌倒置信度 */
    NI_U32 au32Rev[7];
} IAS_FAR_INFO_S;

typedef struct tagIasFadObjResult {
    IAS_ID_INFO_S  stIdInfo;  /* 目标ID信息 */
    IAS_RECT_S     stRect;    /* 目标在原图上的位置 */
    IAS_FAR_INFO_S stFarInfo; /* 跌倒评分信息 */
    IAS_OBJ_TYPE_E enType;    /* 目标类型*/
    NI_U32         au32Rev0[3];
    NI_U32         u32PrStauts; /* PR 状态信息 1为新增ID，2为现有ID */
    NI_U16         u16Status;   /* 跟踪的状态 0：临时目标   1：目标创建  2：目标更新  3：目标丢失（目标隐藏）  4：目标删除， 跟踪关闭时无效*/
    NI_U16         au16Rev[5];
    NI_U64         u64Timestamp; /* 当前目标对应时间戳 */
    NI_U32         au32Rev1[2];
    NI_U32         u32FrameID; /* 当前目标对应帧数 */
    NI_U32         au32Rev[3];
} IAS_FAD_OBJ_RESULT_S;

typedef enum tagIasFadAlarmType {
    IAS_FAD_ALARM_NO_ALARM = 0, /* 无报警 */
    IAS_FAD_ALARM_ALARM    = 1, /* 报警 */
    IAS_FAD_ALARM_TYPE_BUTT,
} IAS_FAD_ALARM_TYPE_E;
/*抓拍结果内容*/
typedef struct tagIasFadResult {
    IAS_ALG_HANDLE        Handle;        /* 句柄信息 */
    IAS_TASK_IMAGE_S      stFrame;       /* 结果对应的图 */
    IAS_FAD_OBJ_RESULT_S *pstObj;        /* 目标结果存放地址， 由算法库分配内存， 用户拿到后需要将信息拷贝走 */
    NI_U32                u32ObjNum;     /* 目标数量 */
    IAS_FAD_ALARM_TYPE_E  enAlarmStatus; /* 当前报警状态 */
} IAS_FAD_RESULT_S;

typedef struct tagIasFadCallBackFunc {
    NI_S32(*pfn_ias_fad_result)
    (IAS_ALG_CHN FadChn, IAS_FAD_RESULT_S *pstResult); /* 用于获取抓拍结果， 如果用户注册了该函数， 算法有结果时会调用该函数, 如果不注册则需要调用接口获取 */
    NI_S32(*pfn_ias_release_frame)
    (IAS_ALG_CHN FadChn, IAS_TASK_IMAGE_S *pstFrame); /* 释放frame， 如果用户注册了该函数， 有结果时会调用该函数, 如果不注册则需要调用接口获取 */
} IAS_FAD_CALLBACK_FUNC_S;

typedef struct tagIasFadPileLineCfg {
    IAS_PIPELINE_MODE_E enPipeMode; /* pipe 模式 */
    NI_U32              u32BufNum;  /* 缓冲数量 */
} IAS_FAD_PIPELINE_CFG_S;

typedef struct tagIasFadProcCfg {
    IAS_PROC_MODE_E enProcMode; /* 处理模式，默认单任务处理模式*/
    NI_U32          au32Rev[3];
} IAS_FAD_PROC_CFG_S;

typedef struct tagIasFadAlgParam {
    NI_U16     u16ScoreThr;  /* 置信度阈值 * 1000 */
    NI_U16     u16MaxObjNum; /* 最大检测目标数 */
    IAS_RECT_S stROIRect;    /* 目标ROI区域 */
} IAS_FAD_ALG_PARAM_S;

typedef struct tagIasFadParam {
    IAS_FAD_ALG_PARAM_S stFadParam;
} IAS_FAD_PARAM_S;

typedef struct tagIasFadInitParam {
    IAS_LICENSE_S           stLicense;      /*Licesnce 预留*/
    IAS_IMAGE_SIZE_S        stImageSrcSize; /* 原图大小 */
    IAS_SIZE_CLIP_S         stObjSize;      /* 检测目标尺寸 */
    IAS_FAD_MODEL_S         stAlgModel;     /* 算法模型文件 */
    IAS_FAD_CALLBACK_FUNC_S stFunc;         /* 注册回调函数 */
    IAS_FAD_PIPELINE_CFG_S  stPipeLine;     /* Pipeline配置 */
    IAS_FAD_PARAM_S         stAlgCfg;       /* 算法配置 */
    IAS_FAD_PROC_CFG_S      stProcCfg;      /* 算法任务模式配置 */
} IAS_FAD_INIT_PARAM_S;

typedef struct tagIasFadNodeStatus {
    NI_CHAR acNodeName[IAS_FAD_NODE_NAME_LEN]; /*node名*/
    NI_S32  s32MeanCost;                       /*平均耗时， us*/
    NI_S32  s32MinCost;                        /*最小耗时， us*/
    NI_S32  s32MaxCost;                        /*最大耗时， us*/
    NI_U32  u32TaskNum;                        /*目前未完成的任务数量*/
    NI_S32  s32IsRunning;                      /* 正在运行标记 */
    NI_U32  u32Runcnt;                         /*运行计数*/
} IAS_FAD_TASK_NODE_STATUS_S;

typedef struct tagIasFadCoreStatus {
    NI_BOOL bEnable;    /*是否使能*/
    NI_U32  u32TaskNum; /*任务数量*/
    NI_U64  u64SumCost; /*现有任务总耗时*/
} IAS_FAD_CORE_STATUS_S;

typedef struct tagIasFadProcStatus {
    IAS_FAD_CORE_STATUS_S      astCoreStatus[32]; /* core 状态*/
    IAS_FAD_TASK_NODE_STATUS_S stFadStatus;       /* FD节点状态*/
    IAS_FAD_TASK_NODE_STATUS_S stFatStatus;       /* FT节点状态*/
    IAS_FAD_TASK_NODE_STATUS_S stFarStatus;       /* FS节点状态*/
    IAS_FAD_TASK_NODE_STATUS_S stDvpStatus;       /* DVP节点状态*/
    IAS_FAD_TASK_NODE_STATUS_S stNnStatus;        /* NN节点状态*/
    IAS_FAD_TASK_NODE_STATUS_S stPostStatus;      /* POST节点状态*/
} IAS_FAD_PROC_STATUS_S;

typedef struct tagIasFadStatus {
    NI_U32 u32Rev[8];
} IAS_FAD_STATUS_S;

typedef enum tagAlgFaDParamType {
    IAS_FAD_PARAM_FAD_MIN_SIZE   = 0, /* 最小目标框 */
    IAS_FAD_PARAM_FAD_MAX_SIZE   = 1, /* 最大目标框 */
    IAS_FAD_PARAM_FAD_SCORE_THRE = 2, /* 检测置信度阈值 * 1000 */
    IAS_FAD_PARAM_TYPE_BUTT,
} IAS_ALG_FAD_PARAM_TYPE_E;

typedef struct tagAlgFaDParamList {
    NI_S32        u32ParamNum;
    IAS_PARAMS_S *pastArr;
} IAS_ALG_FAD_PARAM_LIST_S;

/* ========================================================================== */
/*                             接口函数定义区                                  */
/* ========================================================================== */
/* ==========================================================================
* 函数名 : NI_IAS_FAD_Init
* 描  述 : 创建FAD实例
* 输  入 :
*          input    -i    FadChn   通道数
*          input    -i    pstInit 初始化参数结构体
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_FAD_Init(IAS_ALG_CHN           FadChn,
                       IAS_FAD_INIT_PARAM_S *pstInit);

/* ==========================================================================
* 函数名 : NI_IAS_FAD_DeInit
* 描  述 : FAD任务反初始化
* 输  入 :
*          input    -i    FadChn   通道数
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_FAD_DeInit(IAS_ALG_CHN FadChn);

/* ==========================================================================
* 函数名 : NI_IAS_FAD_GetParams
* 描  述 : 获取FAD算法参数
* 输  入 :
*          input     -i    FadChn   通道数
* 输  出 :
*          output    -o    pstInit 初始化参数结构体
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_FAD_GetParams(IAS_ALG_CHN      FadChn,
                            IAS_FAD_PARAM_S *pstAlgParam);

/* ==========================================================================
* 函数名 : NI_IAS_FAD_SetParams
* 描  述 : 设置FAD算法参数
* 输  入 :
*          input    -i    FadChn         通道数
*          input    -i    pstAlgParam   参数地址
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_FAD_SetParams(IAS_ALG_CHN FadChn,
                            NI_VOID    *pstAlgParam);

/* ==========================================================================
* 函数名 : NI_IAS_FAD_Run
* 描  述 : FAD运行以及结果获取， 要兼容后续的MP模式， 送帧和结果异步
* 输  入 :
*          input    -i    FadChn          通道数
*          input    -i    pstFrame       输入图像
*          input    -i    u32TimeOutMs   超时时间
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_FAD_Run(IAS_ALG_CHN       FadChn,
                      IAS_TASK_IMAGE_S *pstFrame,
                      NI_U32            u32TimeOutMs);

/* ==========================================================================
* 函数名 : NI_IAS_FAD_GetCaptureResult
* 描  述 : 获取抓拍结果， 结果中的内存由算法库分配
* 输  入 :
*          input     -i    FadChn           通道数
*          input     -i    u32TimeOutMs    超时门限
* 输  出 :
*          output    -O    pstResult       结果信息
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_FAD_GetCaptureResult(IAS_ALG_CHN       FadChn,
                                   IAS_FAD_RESULT_S *pstResult,
                                   NI_U32            u32TimeOutMs);

/* ==========================================================================
* 函数名 : NI_IAS_FAD_ReleaseCaptureResult
* 描  述 : 抓拍结果释放
* 输  入 :
*          input    -i    FadChn        通道数
*          input    -i    pstResult    结果信息
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_FAD_ReleaseCaptureResult(IAS_ALG_CHN       FadChn,
                                       IAS_FAD_RESULT_S *pstResult);

/* ==========================================================================
* 函数名 : NI_IAS_FAD_GetImageToRelease
* 描  述 : 获取待释放的图片，当某一张图算法库种不再使用时，通过该接口送出
* 输  入 :
*          input     -i    FadChn           通道数
*          input     -i    u32TimeOutMs    超时门限
* 输  出 :
*          output    -o    pstFrame        当前帧信息
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_FAD_GetImageToRelease(IAS_ALG_CHN       FadChn,
                                    IAS_TASK_IMAGE_S *pstFrame,
                                    NI_U32            u32TimeOutMs);

/* ==========================================================================
* 函数名 : NI_IAS_FAD_GetStatus
* 描  述 : 获取当前状态码
* 输  入 :
*          input     -i    FadChn        通道数
* 输  出 :
*          output    -o    pstStatus    当前状态码
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_FAD_GetStatus(IAS_ALG_CHN       FadChn,
                            IAS_FAD_STATUS_S *pstStatus);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of #ifndef __NI_IAS_FALL_DETECT_H__ */
