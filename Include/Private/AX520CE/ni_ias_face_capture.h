/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef __NI_IAS_FACE_CAPTURE_H__
#define __NI_IAS_FACE_CAPTURE_H__

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
#define IAS_FC_MAX_OBJ_NUM   (50)
#define IAS_FC_MAX_PARAM_NUM (8)
#define IAS_FC_NODE_NAME_LEN (128)

/* ========================================================================== */
/*                           结构体定义区                                     */
/* ========================================================================== */
typedef struct tagIasFdObjResult {
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
} IAS_FD_OBJ_RESULT_S;

/*检测结果内容*/
typedef struct tagIasFdResult {
    IAS_ALG_HANDLE       Handle;    /* 句柄信息 */
    IAS_TASK_IMAGE_S     stFrame;   /* 结果对应的图 */
    IAS_FD_OBJ_RESULT_S *pstObj;    /* 目标结果存放地址， 由算法库分配内存， 用户拿到后需要将信息拷贝走 */
    NI_U32               u32ObjNum; /* 目标数量 */
} IAS_FD_RESULT_S;

typedef enum tagAlgFcModelType {
    IAS_FC_MODEL_READ_FROM_MEM  = 0, /* 从内存读取模型 */
    IAS_FC_MODEL_READ_FROM_FILE = 1, /* 从文件读取模型 */
    IAS_FC_MODEL_TYPE_BUTT,
} IAS_ALG_FC_MODEL_TYPE_E;

typedef struct tagIasFcModelCfg {
    IAS_MODEL_CFG_S stModelCfgFd; /*FD 算法模型*/
    IAS_MODEL_CFG_S stModelCfgFt; /*FT 算法模型*/
    IAS_MODEL_CFG_S stModelCfgFs; /*FS 算法模型*/
} IAS_FC_CFG_S;

/*模型读取方式*/
typedef struct tagIasFcModel {
    IAS_ALG_FC_MODEL_TYPE_E enType; /*模型读取类型*/
    union {
        IAS_FC_CFG_S stFcModelCfg;                 /* UNION:enType=IAS_FC_MODEL_READ_FROM_MEM | FC 算法模型配置 */
        NI_CHAR      acName[IAS_ALG_MAX_NAME_LEN]; /* UNION:enType=IAS_FC_MODEL_READ_FROM_FILE | FC 算法模型路径 */
    };
} IAS_FC_MODEL_S;

typedef struct tagIasFsInfo {
    NI_S32 s32PupilDistance; /* 人脸瞳距 */
    NI_S32 s32FaceScore;     /* 人脸评分 */
    NI_S32 s32FaceAngle;     /* 人脸角度 */
    NI_S32 s32FaceClarity;   /* 人脸清晰度 */
    NI_S32 s32IsMatchType;   /* 是否与类型匹配 */
    NI_U32 au32Rev[3];
} IAS_FS_INFO_S;

typedef struct tagIasFcObjResult {
    IAS_ID_INFO_S  stIdInfo;    /* 目标ID信息 */
    IAS_RECT_S     stRect;      /* 目标在原图上的位置 */
    IAS_FS_INFO_S  stFsInfo;    /* 人脸优选评分信息 */
    IAS_OBJ_TYPE_E enType;      /* 目标类型*/
    NI_U32         u32FsStauts; /* FS 状态信息 1为新增ID，2为现有ID */
    NI_U16         u16Status;   /* 跟踪的状态 0：临时目标   1：目标创建  2：目标更新  3：目标丢失（目标隐藏）  4：目标删除， 跟踪关闭时无效*/
    NI_U16         au16Rev[3];
    NI_U64         u64Timestamp; /* 当前目标对应时间戳 */
    NI_U32         u32FrameID;   /* 当前目标对应帧数 */
    NI_U32         au32Rev[5];
} IAS_FC_OBJ_RESULT_S;

/*抓拍结果内容*/
typedef struct tagIasFcResult {
    IAS_ALG_HANDLE       Handle;    /* 句柄信息 */
    IAS_TASK_IMAGE_S     stFrame;   /* 结果对应的图 */
    IAS_FC_OBJ_RESULT_S *pstObj;    /* 目标结果存放地址， 由算法库分配内存， 用户拿到后需要将信息拷贝走 */
    NI_U32               u32ObjNum; /* 目标数量 */
} IAS_FC_RESULT_S;

/*人脸周界结果内容*/
typedef struct tagIasFcPgResult {
    IAS_ALG_HANDLE        Handle;    /* 句柄信息 */
    IAS_TASK_IMAGE_S      stFrame;   /* 结果对应的图 */
    IAS_FD_OBJ_RESULT_S  *pstObj;    /* 目标结果存放地址， 由算法库分配内存， 用户拿到后需要将信息拷贝走 */
    NI_U32                u32ObjNum; /* 目标数量 */
    IAS_PERIMETER_RULE_S *pstRule;   /* 触发的规则 */
    NI_U32                u32RuleId; /* 触发的规则ID */
} IAS_FC_PG_RESULT_S;

typedef struct tagIasFcCallBackFunc {
    NI_S32(*pfn_ias_fd_result)
    (IAS_ALG_CHN FcChn, IAS_FD_RESULT_S *pstResult); /* 用于获取检测结果， 如果用户注册了该函数， 算法有结果时会调用该函数, 如果不注册则需要调用接口获取 */
    NI_S32(*pfn_ias_fc_result)
    (IAS_ALG_CHN FcChn, IAS_FC_RESULT_S *pstResult); /* 用于获取抓拍结果， 如果用户注册了该函数， 算法有结果时会调用该函数, 如果不注册则需要调用接口获取 */
    NI_S32(*pfn_ias_fp_result)
    (IAS_ALG_CHN FcChn, IAS_FC_PG_RESULT_S *pstResult); /* 用于获取抓拍结果， 如果用户注册了该函数， 算法有结果时会调用该函数, 如果不注册则需要调用接口获取 */
    NI_S32(*pfn_ias_release_frame)
    (IAS_ALG_CHN FcChn, IAS_TASK_IMAGE_S *pstFrame); /* 释放frame， 如果用户注册了该函数， 有结果时会调用该函数, 如果不注册则需要调用接口获取 */
} IAS_FC_CALLBACK_FUNC_S;

typedef struct tagIasFcPileLineCfg {
    IAS_PIPELINE_MODE_E enPipeMode; /* pipe 模式 */
    NI_U32              u32BufNum;  /* 缓冲数量 */
} IAS_FC_PIPELINE_CFG_S;

typedef struct tagIasFcProcCfg {
    IAS_PROC_MODE_E enProcMode; /* 处理模式，默认单任务处理模式*/
    NI_U32          au32Rev[3];
} IAS_FC_PROC_CFG_S;

typedef struct tagIasFdAlgParam {
    NI_U16     u16ScoreThr;  /* 置信度阈值 * 1000 */
    NI_U16     u16MaxObjNum; /* 最大检测目标数 */
    IAS_RECT_S stROIRect;    /* 目标ROI区域 */
} IAS_FD_ALG_PARAM_S;

typedef struct tagIasFtAlgParam {
    NI_BOOL bPositiveTrack; /* 主动跟踪使能*/
    NI_U16  u16MaxTrackNum; /* 最大跟踪目标数 */
    NI_U16  u16Fps;         /* 跟踪帧率*1000 */
} IAS_FT_ALG_PARAM_S;

typedef struct tagIasFsAlgParam {
    NI_BOOL bEnableFs;   /* 是否开启分类*/
    NI_U16  u16ScoreThr; /* 置信度阈值 * 1000 */
    NI_U16  u16MaxFsNum; /* 最大优选目标数 */
} IAS_FS_ALG_PARAM_S;

typedef struct tagIasFpAlgParam {
    NI_BOOL bEnableFp; /* 是否开启周界*/
} IAS_FP_ALG_PARAM_S;

typedef struct tagIasFcParam {
    IAS_FD_ALG_PARAM_S stFdParam;
    IAS_FT_ALG_PARAM_S stFtParam;
    IAS_FS_ALG_PARAM_S stFsParam;
    IAS_FP_ALG_PARAM_S stFpParam;
} IAS_FC_PARAM_S;

typedef struct tagIasFcInitParam {
    IAS_LICENSE_S          stLicense;      /*Licesnce 预留*/
    IAS_IMAGE_SIZE_S       stImageSrcSize; /* 原图大小 */
    IAS_SIZE_CLIP_S        stFaceSize;     /* 检测目标尺寸 */
    IAS_FC_MODEL_S         stAlgModel;     /* 算法模型文件 */
    IAS_FC_CALLBACK_FUNC_S stFunc;         /* 注册回调函数 */
    IAS_FC_PIPELINE_CFG_S  stPipeLine;     /* Pipeline配置 */
    IAS_FC_PARAM_S         stAlgCfg;       /* 算法配置 */
    IAS_FC_PROC_CFG_S      stProcCfg;      /* 算法任务模式配置 */
} IAS_FC_INIT_PARAM_S;

typedef struct tagIasFcNodeStatus {
    NI_CHAR acNodeName[IAS_FC_NODE_NAME_LEN]; /*node名*/
    NI_S32  s32MeanCost;                      /*平均耗时， us*/
    NI_S32  s32MinCost;                       /*最小耗时， us*/
    NI_S32  s32MaxCost;                       /*最大耗时， us*/
    NI_U32  u32TaskNum;                       /*目前未完成的任务数量*/
    NI_S32  s32IsRunning;                     /* 正在运行标记 */
    NI_U32  u32Runcnt;                        /*运行计数*/
} IAS_FC_TASK_NODE_STATUS_S;

typedef struct tagIasFcCoreStatus {
    NI_BOOL bEnable;    /*是否使能*/
    NI_U32  u32TaskNum; /*任务数量*/
    NI_U64  u64SumCost; /*现有任务总耗时*/
} IAS_FC_CORE_STATUS_S;

typedef struct tagIasFcProcStatus {
    IAS_FC_CORE_STATUS_S      astCoreStatus[32]; /* core 状态*/
    IAS_FC_TASK_NODE_STATUS_S stFdStatus;        /* FD节点状态*/
    IAS_FC_TASK_NODE_STATUS_S stFtStatus;        /* FT节点状态*/
    IAS_FC_TASK_NODE_STATUS_S stFsStatus;        /* FS节点状态*/
    IAS_FC_TASK_NODE_STATUS_S stFpStatus;        /* FP节点状态*/
    IAS_FC_TASK_NODE_STATUS_S stDvpStatus;       /* DVP节点状态*/
    IAS_FC_TASK_NODE_STATUS_S stNnStatus;        /* NN节点状态*/
    IAS_FC_TASK_NODE_STATUS_S stPostStatus;      /* POST节点状态*/
} IAS_FC_PROC_STATUS_S;

typedef struct tagIasFcStatus {
    NI_U32 u32Rev[8];
} IAS_FC_STATUS_S;

typedef enum tagAlgFcParamType {
    IAS_FC_PARAM_FD_MIN_SIZE   = 0, /* 最小目标框 */
    IAS_FC_PARAM_FD_MAX_SIZE   = 1, /* 最大目标框 */
    IAS_FC_PARAM_FD_SCORE_THRE = 2, /* 检测置信度阈值 * 1000 */
    IAS_FC_PARAM_TYPE_BUTT,
} IAS_ALG_FC_PARAM_TYPE_E;

typedef struct tagAlgFcParamList {
    NI_S32        u32ParamNum;
    IAS_PARAMS_S *pastArr;
} IAS_ALG_FC_PARAM_LIST_S;

/* ========================================================================== */
/*                             接口函数定义区                                  */
/* ========================================================================== */
/* ==========================================================================
* 函数名 : NI_IAS_FC_Init
* 描  述 : 创建FC实例
* 输  入 :
*          input    -i    FcChn   通道数
*          input    -i    pstInit 初始化参数结构体
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_FC_Init(IAS_ALG_CHN          FcChn,
                      IAS_FC_INIT_PARAM_S *pstInit);

/* ==========================================================================
* 函数名 : NI_IAS_FC_DeInit
* 描  述 : Fc任务反初始化
* 输  入 :
*          input    -i    FcChn   通道数
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_FC_DeInit(IAS_ALG_CHN FcChn);

/* ==========================================================================
* 函数名 : NI_IAS_FC_GetParams
* 描  述 : 获取FC算法参数
* 输  入 :
*          input     -i    FcChn   通道数
* 输  出 :
*          output    -o    pstInit 初始化参数结构体
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_FC_GetParams(IAS_ALG_CHN     FcChn,
                           IAS_FC_PARAM_S *pstAlgParam);

/* ==========================================================================
* 函数名 : NI_IAS_FC_SetParams
* 描  述 : 设置FC算法参数
* 输  入 :
*          input    -i    FcChn         通道数
*          input    -i    pstAlgParam   参数地址
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_FC_SetParams(IAS_ALG_CHN FcChn,
                           NI_VOID    *pstAlgParam);

/* ==========================================================================
* 函数名 : NI_IAS_FC_Run
* 描  述 : FC运行以及结果获取， 要兼容后续的MP模式， 送帧和结果异步
* 输  入 :
*          input    -i    FcChn          通道数
*          input    -i    pstFrame       输入图像
*          input    -i    u32TimeOutMs   超时时间
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_FC_Run(IAS_ALG_CHN       FcChn,
                     IAS_TASK_IMAGE_S *pstFrame,
                     NI_U32            u32TimeOutMs);

/* ==========================================================================
* 函数名 : NI_IAS_FC_GetRectResult
* 描  述 : 获取检测目标结果， 结果中的内存由算法库分配
* 输  入 :
*          input     -i    FcChn           通道数
*          input     -i    u32TimeOutMs    超时时间
* 输  出 :
*          output    -O    pstResult       输出信息
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_FC_GetRectResult(IAS_ALG_CHN      FcChn,
                               IAS_FD_RESULT_S *pstResult,
                               NI_U32           u32TimeOutMs);

/* ==========================================================================
* 函数名 : NI_IAS_FC_ReleaseRectResult
* 描  述 : 结果释放检测目标接口
* 输  入 :
*          input     -i    FcChn        通道数
*          input     -i    pstResult    输出信息
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_FC_ReleaseRectResult(IAS_ALG_CHN      FcChn,
                                   IAS_FD_RESULT_S *pstResult);

/* ==========================================================================
* 函数名 : NI_IAS_FC_GetCaptureResult
* 描  述 : 获取抓拍结果， 结果中的内存由算法库分配
* 输  入 :
*          input     -i    FcChn           通道数
*          input     -i    u32TimeOutMs    超时门限
* 输  出 :
*          output    -O    pstResult       结果信息
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_FC_GetCaptureResult(IAS_ALG_CHN      FcChn,
                                  IAS_FC_RESULT_S *pstResult,
                                  NI_U32           u32TimeOutMs);

/* ==========================================================================
* 函数名 : NI_IAS_FC_ReleaseCaptureResult
* 描  述 : 抓拍结果释放
* 输  入 :
*          input    -i    FcChn        通道数
*          input    -i    pstResult    结果信息
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_FC_ReleaseCaptureResult(IAS_ALG_CHN      FcChn,
                                      IAS_FC_RESULT_S *pstResult);

/* ==========================================================================
* 函数名 : NI_IAS_FC_AddPerimeterRule
* 描  述 : 增加周界规则
* 输  入 :
*          input     -i    OdChn         通道数
*          input     -i    pstRule       规则框
* 输  出 :
*          output    -o    pu32RuleId    当前规则对应所有规则的ID
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_FC_AddPerimeterRule(IAS_ALG_CHN           OdChn,
                                  IAS_PERIMETER_RULE_S *pstRule,
                                  NI_U32               *pu32RuleId);

/* ==========================================================================
* 函数名 : NI_IAS_FC_DelPerimeterRule
* 描  述 : 删除周界规则
* 输  入 :
*          input     -i    OdChn      通道数
*          input     -i    pstRule    规则框
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_FC_DelPerimeterRule(IAS_ALG_CHN           OdChn,
                                  IAS_PERIMETER_RULE_S *pstRule);

/* ==========================================================================
* 函数名 : NI_IAS_FC_GetPgResult
* 描  述 : 获取周界结果， 结果中的内存由算法库分配
* 输  入 :
*          input     -i    OdChn           通道数
*          input     -i    u32TimeOutMs    超时门限
* 输  出 :
*          output    -O    pstResult       结果信息
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_FC_GetPgResult(IAS_ALG_CHN         OdChn,
                             IAS_FC_PG_RESULT_S *pstResult,
                             NI_U32              u32TimeOutMs);

/* ==========================================================================
* 函数名 : NI_IAS_FC_ReleasePgResult
* 描  述 : 结果释周界放接口
* 输  入 :
*          input    -i    OdChn        通道数
*          input    -i    pstResult    结果信息
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_FC_ReleasePgResult(IAS_ALG_CHN         OdChn,
                                 IAS_FC_PG_RESULT_S *pstResult);

/* ==========================================================================
* 函数名 : NI_IAS_FC_GetImageToRelease
* 描  述 : 获取待释放的图片，当某一张图算法库种不再使用时，通过该接口送出
* 输  入 :
*          input     -i    FcChn           通道数
*          input     -i    u32TimeOutMs    超时门限
* 输  出 :
*          output    -o    pstFrame        当前帧信息
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_FC_GetImageToRelease(IAS_ALG_CHN       FcChn,
                                   IAS_TASK_IMAGE_S *pstFrame,
                                   NI_U32            u32TimeOutMs);

/* ==========================================================================
* 函数名 : NI_IAS_FC_GetStatus
* 描  述 : 获取当前状态码
* 输  入 :
*          input     -i    FcChn        通道数
* 输  出 :
*          output    -o    pstStatus    当前状态码
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_FC_GetStatus(IAS_ALG_CHN      FcChn,
                           IAS_FC_STATUS_S *pstStatus);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of #ifndef __NI_IAS_FACE_CAPTURE_H__ */
