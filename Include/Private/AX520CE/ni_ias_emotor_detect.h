/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef __NI_IAS_EMOTOR_DETECT_H__
#define __NI_IAS_EMOTOR_DETECT_H__

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
#define IAS_EMD_MAX_OBJ_NUM   (50)
#define IAS_EMD_MAX_PARAM_NUM (8)
#define IAS_EMD_NODE_NAME_LEN (128)

/* ========================================================================== */
/*                           结构体定义区                                     */
/* ========================================================================== */
typedef struct tagIasEmdObjResult {
    IAS_ID_INFO_S  stIdInfo;  /* 目标ID信息 */
    IAS_RECT_S     stRect;    /* 目标在原图上的位置 */
    IAS_OBJ_TYPE_E enType;    /* 目标类型*/
    NI_U16         u16Score;  /* 置信度 * 1000, */
    NI_U16         u16Status; /* 跟踪的状态 0：临时目标   1：目标创建  2：目标更新  3：目标丢失（目标隐藏）  4：目标删除， 跟踪关闭时无效*/
    NI_U32         u32PgCls;  /* PG分类 0: 刚触发报警，1: 报警后持续跟踪 */
    NI_U32         u32Rev;
    NI_U64         u64Timestamp; /* 当前目标对应时间戳 */
    NI_U32         u32FrameID;   /* 当前目标对应帧数 */
    NI_U32         au32Rev[5];
} IAS_EMD_OBJ_RESULT_S;

/*检测结果内容*/
typedef struct tagIasEmdResult {
    IAS_ALG_HANDLE        Handle;    /* 句柄信息 */
    IAS_TASK_IMAGE_S      stFrame;   /* 结果对应的图 */
    IAS_EMD_OBJ_RESULT_S *pstObj;    /* 目标结果存放地址， 由算法库分配内存， 用户拿到后需要将信息拷贝走 */
    NI_U32                u32ObjNum; /* 目标数量 */
} IAS_EMD_RESULT_S;

typedef enum tagAlgEmdModelType {
    IAS_EMD_MODEL_READ_FROM_MEM  = 0, /* 从内存读取模型 */
    IAS_EMD_MODEL_READ_FROM_FILE = 1, /* 从文件读取模型 */
    IAS_EMD_MODEL_TYPE_BUTT,
} IAS_ALG_EMD_MODEL_TYPE_E;

typedef struct tagIasEmdModelCfg {
    IAS_MODEL_CFG_S stModelCfgEmd; /*EMD 算法模型*/
    IAS_MODEL_CFG_S stModelCfgEmt; /*EMT 算法模型*/
    IAS_MODEL_CFG_S stModelCfgEmc; /*EMC 算法模型*/
} IAS_EMD_CFG_S;

/*模型读取方式*/
typedef struct tagIasEmdModel {
    IAS_ALG_EMD_MODEL_TYPE_E enType; /*模型读取类型*/
    union {
        IAS_EMD_CFG_S stEmdModelCfg;                /* UNION:enType=IAS_EMD_MODEL_READ_FROM_MEM | EMD 算法模型配置 */
        NI_CHAR       acName[IAS_ALG_MAX_NAME_LEN]; /* UNION:enType=IAS_EMD_MODEL_READ_FROM_FILE | EMD 算法模型路径 */
    };
} IAS_EMD_MODEL_S;

typedef struct tagIasEmcInfo {
    NI_S32 s32EmcScore; /* 电瓶车置信度 */
    NI_U32 au32Rev[3];
} IAS_EMC_INFO_S;

typedef struct tagIasEmcObjResult {
    IAS_ID_INFO_S  stIdInfo;  /* 目标ID信息 */
    IAS_RECT_S     stRect;    /* 目标在原图上的位置 */
    IAS_EMC_INFO_S stEmcInfo; /* 电瓶车信息 */
    IAS_OBJ_TYPE_E enType;    /* 目标类型*/
    NI_U32         au32Rev0[3];
    NI_U32         u32EmcStauts; /* 电瓶车 状态信息 1为新增ID，2为现有ID */
    NI_U16         u16Status;    /* 跟踪的状态 0：临时目标   1：目标创建  2：目标更新  3：目标丢失（目标隐藏）  4：目标删除， 跟踪关闭时无效*/
    NI_U16         au16Rev[5];
    NI_U64         u64Timestamp; /* 当前目标对应时间戳 */
    NI_U32         au32Rev1[2];
    NI_U32         u32FrameID; /* 当前目标对应帧数 */
    NI_U32         au32Rev[3];
} IAS_EMC_OBJ_RESULT_S;

typedef enum tagIasEmcAlarmType {
    IAS_EMC_ALARM_NO_ALARM = 0, /* 无报警 */
    IAS_EMC_ALARM_ALARM    = 1, /* 报警 */
    IAS_EMC_ALARM_TYPE_BUTT,
} IAS_EMC_ALARM_TYPE_E;

/*抓拍结果内容*/
typedef struct tagIasEmcResult {
    IAS_ALG_HANDLE        Handle;        /* 句柄信息 */
    IAS_TASK_IMAGE_S      stFrame;       /* 结果对应的图 */
    IAS_EMC_OBJ_RESULT_S *pstObj;        /* 目标结果存放地址， 由算法库分配内存， 用户拿到后需要将信息拷贝走 */
    NI_U32                u32ObjNum;     /* 目标数量 */
    IAS_EMC_ALARM_TYPE_E  enAlarmStatus; /* 当前报警状态 */
} IAS_EMC_RESULT_S;

typedef struct tagIasEmdCallBackFunc {
    NI_S32(*pfn_ias_emd_result)
    (IAS_ALG_CHN EmdChn, IAS_EMD_RESULT_S *pstResult); /* 用于获取检测结果， 如果用户注册了该函数， 算法有结果时会调用该函数, 如果不注册则需要调用接口获取 */
    NI_S32(*pfn_ias_emc_result)
    (IAS_ALG_CHN EmdChn, IAS_EMC_RESULT_S *pstResult); /* 用于获取抓拍结果， 如果用户注册了该函数， 算法有结果时会调用该函数, 如果不注册则需要调用接口获取 */
    NI_S32(*pfn_ias_release_frame)
    (IAS_ALG_CHN EmdChn, IAS_TASK_IMAGE_S *pstFrame); /* 释放frame， 如果用户注册了该函数， 有结果时会调用该函数, 如果不注册则需要调用接口获取 */
} IAS_EMD_CALLBACK_FUNC_S;

typedef struct tagIasEmdPipeLineCfg {
    IAS_PIPELINE_MODE_E enPipeMode; /* pipe 模式 */
    NI_U32              u32BufNum;  /* 缓冲数量 */
} IAS_EMD_PIPELINE_CFG_S;

typedef struct tagIasEmdProcCfg {
    IAS_PROC_MODE_E enProcMode; /* 处理模式，默认单任务处理模式*/
    NI_U32          au32Rev[3];
} IAS_EMD_PROC_CFG_S;

typedef struct tagIasEmdAlgParam {
    NI_U16                       u16ScoreThr;  /* 置信度阈值 * 1000 */
    NI_U16                       u16MaxObjNum; /* 最大检测目标数 */
    IAS_PERIMETER_RULE_POLYGON_S stPolygon;    /* 目标ROI区域 */
} IAS_EMD_ALG_PARAM_S;

typedef struct tagIasEmtAlgParam {
    NI_BOOL bPositiveTrack; /* 主动跟踪使能*/
    NI_U16  u16MaxTrackNum; /* 最大跟踪目标数 */
    NI_U16  u16Fps;         /* 跟踪帧率*1000 */
} IAS_EMT_ALG_PARAM_S;

typedef enum tagAlgEmcProcType {
    IAS_EMC_PROC_DEFAULT = 0, /* 默认值，默认开启电瓶车处理 */
    IAS_EMC_PROC_BIKE    = 1, /* 开启自行车报警 */
    IAS_EMC_PROC_TYPE_BUTT,
} IAS_ALG_EMC_PROC_TYPE_E;

typedef struct tagIasEmcAlgParam {
    NI_BOOL                       bEnableEmc;  /* 是否开启入梯分析*/
    NI_BOOL                       bEnableAccu; /* 是否开启精准判断*/
    IAS_ALG_EMC_PROC_TYPE_E       enProcType;  /* 处理模式 */
    NI_U16                        u16ScoreThr; /* 置信度阈值 * 1000 */
    NI_U16                        u16MaxNum;   /* 最大入梯分析目标数 */
    IAS_PERIMETER_RULE_POLYLINE_S stPolyline;  /* 触发的规则 */
} IAS_EMC_ALG_PARAM_S;

typedef struct tagIasEmdParam {
    IAS_EMD_ALG_PARAM_S stEmdParam;
    IAS_EMT_ALG_PARAM_S stEmtParam;
    IAS_EMC_ALG_PARAM_S stEmcParam;
} IAS_EMD_PARAM_S;

typedef struct tagIasEmdInitParam {
    IAS_LICENSE_S           stLicense;      /*Licesnce 预留*/
    IAS_IMAGE_SIZE_S        stImageSrcSize; /* 原图大小 */
    IAS_SIZE_CLIP_S         stObjSize;      /* 检测目标尺寸 */
    IAS_EMD_MODEL_S         stAlgModel;     /* 算法模型文件 */
    IAS_EMD_CALLBACK_FUNC_S stFunc;         /* 注册回调函数 */
    IAS_EMD_PIPELINE_CFG_S  stPipeLine;     /* Pipeline配置 */
    IAS_EMD_PARAM_S         stAlgCfg;       /* 算法配置 */
    IAS_EMD_PROC_CFG_S      stProcCfg;      /* 算法任务模式配置 */
} IAS_EMD_INIT_PARAM_S;

typedef struct tagIasEmdNodeStatus {
    NI_CHAR acNodeName[IAS_EMD_NODE_NAME_LEN]; /*node名*/
    NI_S32  s32MeanCost;                       /*平均耗时， us*/
    NI_S32  s32MinCost;                        /*最小耗时， us*/
    NI_S32  s32MaxCost;                        /*最大耗时， us*/
    NI_U32  u32TaskNum;                        /*目前未完成的任务数量*/
    NI_S32  s32IsRunning;                      /* 正在运行标记 */
    NI_U32  u32Runcnt;                         /*运行计数*/
} IAS_EMD_TASK_NODE_STATUS_S;

typedef struct tagIasEmdCoreStatus {
    NI_BOOL bEnable;    /*是否使能*/
    NI_U32  u32TaskNum; /*任务数量*/
    NI_U64  u64SumCost; /*现有任务总耗时*/
} IAS_EMD_CORE_STATUS_S;

typedef struct tagIasEmdProcStatus {
    IAS_EMD_CORE_STATUS_S      astCoreStatus[32]; /* core 状态*/
    IAS_EMD_TASK_NODE_STATUS_S stEmdStatus;       /* EMD节点状态*/
    IAS_EMD_TASK_NODE_STATUS_S stEmtStatus;       /* EMT节点状态*/
    IAS_EMD_TASK_NODE_STATUS_S stEmcStatus;       /* EMC节点状态*/
    IAS_EMD_TASK_NODE_STATUS_S stDvpStatus;       /* DVP节点状态*/
    IAS_EMD_TASK_NODE_STATUS_S stNnStatus;        /* NN节点状态*/
    IAS_EMD_TASK_NODE_STATUS_S stPostStatus;      /* POST节点状态*/
} IAS_EMD_PROC_STATUS_S;

typedef struct tagIasEmdStatus {
    NI_U32 u32Rev[8];
} IAS_EMD_STATUS_S;

typedef enum tagAlgEmdParamType {
    IAS_EMD_PARAM_EMD_MIN_SIZE   = 0, /* 最小目标框 */
    IAS_EMD_PARAM_EMD_MAX_SIZE   = 1, /* 最大目标框 */
    IAS_EMD_PARAM_EMD_SCORE_THRE = 2, /* 检测置信度阈值 * 1000 */
    IAS_EMD_PARAM_TYPE_BUTT,
} IAS_ALG_EMD_PARAM_TYPE_E;

typedef struct tagAlgEmdParamList {
    NI_S32        u32ParamNum;
    IAS_PARAMS_S *pastArr;
} IAS_ALG_EMD_PARAM_LIST_S;

/* ========================================================================== */
/*                             接口函数定义区                                  */
/* ========================================================================== */
/* ==========================================================================
* 函数名 : NI_IAS_EMD_Init
* 描  述 : 创建EMD实例
* 输  入 :
*          input    -i    EmdChn   通道数
*          input    -i    pstInit 初始化参数结构体
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_EMD_Init(IAS_ALG_CHN           EmdChn,
                       IAS_EMD_INIT_PARAM_S *pstInit);

/* ==========================================================================
* 函数名 : NI_IAS_EMD_DeInit
* 描  述 : EMD任务反初始化
* 输  入 :
*          input    -i    EmdChn   通道数
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_EMD_DeInit(IAS_ALG_CHN EmdChn);

/* ==========================================================================
* 函数名 : NI_IAS_EMD_GetParams
* 描  述 : 获取EMD算法参数
* 输  入 :
*          input     -i    EmdChn   通道数
* 输  出 :
*          output    -o    pstInit 初始化参数结构体
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_EMD_GetParams(IAS_ALG_CHN      EmdChn,
                            IAS_EMD_PARAM_S *pstAlgParam);

/* ==========================================================================
* 函数名 : NI_IAS_EMD_SetParams
* 描  述 : 设置EMD算法参数
* 输  入 :
*          input    -i    EmdChn         通道数
*          input    -i    pstAlgParam   参数地址
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_EMD_SetParams(IAS_ALG_CHN EmdChn,
                            NI_VOID    *pstAlgParam);

/* ==========================================================================
* 函数名 : NI_IAS_EMD_Run
* 描  述 : EMD运行以及结果获取， 要兼容后续的MP模式， 送帧和结果异步
* 输  入 :
*          input    -i    EmdChn          通道数
*          input    -i    pstFrame       输入图像
*          input    -i    u32TimeOutMs   超时时间
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_EMD_Run(IAS_ALG_CHN       EmdChn,
                      IAS_TASK_IMAGE_S *pstFrame,
                      NI_U32            u32TimeOutMs);

/* ==========================================================================
* 函数名 : NI_IAS_EMD_GetRectResult
* 描  述 : 获取检测目标结果， 结果中的内存由算法库分配
* 输  入 :
*          input     -i    EmdChn           通道数
*          input     -i    u32TimeOutMs    超时时间
* 输  出 :
*          output    -O    pstResult       输出信息
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_EMD_GetRectResult(IAS_ALG_CHN       EmdChn,
                                IAS_EMD_RESULT_S *pstResult,
                                NI_U32            u32TimeOutMs);

/* ==========================================================================
* 函数名 : NI_IAS_EMD_ReleaseRectResult
* 描  述 : 结果释放检测目标接口
* 输  入 :
*          input     -i    EmdChn        通道数
*          input     -i    pstResult    输出信息
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_EMD_ReleaseRectResult(IAS_ALG_CHN       EmdChn,
                                    IAS_EMD_RESULT_S *pstResult);

/* ==========================================================================
* 函数名 : NI_IAS_EMD_GetCaptureResult
* 描  述 : 获取抓拍结果， 结果中的内存由算法库分配
* 输  入 :
*          input     -i    EmdChn           通道数
*          input     -i    u32TimeOutMs    超时门限
* 输  出 :
*          output    -O    pstResult       结果信息
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_EMD_GetCaptureResult(IAS_ALG_CHN       EmdChn,
                                   IAS_EMC_RESULT_S *pstResult,
                                   NI_U32            u32TimeOutMs);

/* ==========================================================================
* 函数名 : NI_IAS_EMD_ReleaseCaptureResult
* 描  述 : 抓拍结果释放
* 输  入 :
*          input    -i    EmdChn        通道数
*          input    -i    pstResult    结果信息
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_EMD_ReleaseCaptureResult(IAS_ALG_CHN       EmdChn,
                                       IAS_EMC_RESULT_S *pstResult);

/* ==========================================================================
* 函数名 : NI_IAS_EMD_GetImageToRelease
* 描  述 : 获取待释放的图片，当某一张图算法库种不再使用时，通过该接口送出
* 输  入 :
*          input     -i    EmdChn           通道数
*          input     -i    u32TimeOutMs    超时门限
* 输  出 :
*          output    -o    pstFrame        当前帧信息
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_EMD_GetImageToRelease(IAS_ALG_CHN       EmdChn,
                                    IAS_TASK_IMAGE_S *pstFrame,
                                    NI_U32            u32TimeOutMs);

/* ==========================================================================
* 函数名 : NI_IAS_EMD_GetStatus
* 描  述 : 获取当前状态码
* 输  入 :
*          input     -i    EmdChn        通道数
* 输  出 :
*          output    -o    pstStatus    当前状态码
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_EMD_GetStatus(IAS_ALG_CHN       EmdChn,
                            IAS_EMD_STATUS_S *pstStatus);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of #ifndef __NI_IAS_EMOTOR_DETECT_H__ */
