/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef __NI_IAS_PLATE_CAPTURE_H__
#define __NI_IAS_PLATE_CAPTURE_H__

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
#define IAS_PC_MAX_OBJ_NUM    (50)
#define IAS_PC_MAX_PARAM_NUM  (8)
#define IAS_PC_NODE_NAME_LEN  (128)
#define IAS_PC_PLATE_NAME_LEN (16)

/* ========================================================================== */
/*                           结构体定义区                                     */
/* ========================================================================== */
typedef struct tagIasPdObjResult {
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
} IAS_PD_OBJ_RESULT_S;

/*检测结果内容*/
typedef struct tagIasPdResult {
    IAS_ALG_HANDLE       Handle;    /* 句柄信息 */
    IAS_TASK_IMAGE_S     stFrame;   /* 结果对应的图 */
    IAS_PD_OBJ_RESULT_S *pstObj;    /* 目标结果存放地址， 由算法库分配内存， 用户拿到后需要将信息拷贝走 */
    NI_U32               u32ObjNum; /* 目标数量 */
} IAS_PD_RESULT_S;

typedef enum tagAlgPcModelType {
    IAS_PC_MODEL_READ_FROM_MEM  = 0, /* 从内存读取模型 */
    IAS_PC_MODEL_READ_FROM_FILE = 1, /* 从文件读取模型 */
    IAS_PC_MODEL_TYPE_BUTT,
} IAS_ALG_PC_MODEL_TYPE_E;

typedef struct tagIasPcModelCfg {
    IAS_MODEL_CFG_S stModelCfgPd; /*FD 算法模型*/
    IAS_MODEL_CFG_S stModelCfgPt; /*FT 算法模型*/
    IAS_MODEL_CFG_S stModelCfgPs; /*FS 算法模型*/
} IAS_PC_CFG_S;

/*模型读取方式*/
typedef struct tagIasPcModel {
    IAS_ALG_PC_MODEL_TYPE_E enType; /*模型读取类型*/
    union {
        IAS_PC_CFG_S stPcModelCfg;                 /* UNION:enType=IAS_PC_MODEL_READ_FROM_MEM | PC 算法模型配置 */
        NI_CHAR      acName[IAS_ALG_MAX_NAME_LEN]; /* UNION:enType=IAS_PC_MODEL_READ_FROM_FILE | PC 算法模型路径 */
    };
} IAS_PC_MODEL_S;

typedef struct tagIasPrInfo {
    NI_CHAR acPlateChar[IAS_PC_PLATE_NAME_LEN]; /* 车牌字串 */
    NI_S32  s32PlateScore;                      /* 车牌置信度 */
    NI_U32  au32Rev[3];
} IAS_PR_INFO_S;

typedef struct tagIasPcObjResult {
    IAS_ID_INFO_S  stIdInfo; /* 目标ID信息 */
    IAS_RECT_S     stRect;   /* 目标在原图上的位置 */
    IAS_PR_INFO_S  stPrInfo; /* 车牌识别信息 */
    IAS_OBJ_TYPE_E enType;   /* 目标类型*/
    NI_U32         au32Rev0[3];
    NI_U32         u32PrStauts; /* PR 状态信息 1为新增ID，2为现有ID */
    NI_U16         u16Status;   /* 跟踪的状态 0：临时目标   1：目标创建  2：目标更新  3：目标丢失（目标隐藏）  4：目标删除， 跟踪关闭时无效*/
    NI_U16         au16Rev[5];
    NI_U64         u64Timestamp; /* 当前目标对应时间戳 */
    NI_U32         au32Rev1[2];
    NI_U32         u32FrameID; /* 当前目标对应帧数 */
    NI_U32         au32Rev[3];
} IAS_PC_OBJ_RESULT_S;

/*抓拍结果内容*/
typedef struct tagIasPcResult {
    IAS_ALG_HANDLE       Handle;    /* 句柄信息 */
    IAS_TASK_IMAGE_S     stFrame;   /* 结果对应的图 */
    IAS_PC_OBJ_RESULT_S *pstObj;    /* 目标结果存放地址， 由算法库分配内存， 用户拿到后需要将信息拷贝走 */
    NI_U32               u32ObjNum; /* 目标数量 */
} IAS_PC_RESULT_S;

/*车牌周界结果内容*/
typedef struct tagIasPcPgResult {
    IAS_ALG_HANDLE        Handle;    /* 句柄信息 */
    IAS_TASK_IMAGE_S      stFrame;   /* 结果对应的图 */
    IAS_PD_OBJ_RESULT_S  *pstObj;    /* 目标结果存放地址， 由算法库分配内存， 用户拿到后需要将信息拷贝走 */
    NI_U32                u32ObjNum; /* 目标数量 */
    IAS_PERIMETER_RULE_S *pstRule;   /* 触发的规则 */
    NI_U32                u32RuleId; /* 触发的规则ID */
} IAS_PC_PG_RESULT_S;

typedef struct tagIasPcCallBackFunc {
    NI_S32(*pfn_ias_pd_result)
    (IAS_ALG_CHN PcChn, IAS_PD_RESULT_S *pstResult); /* 用于获取检测结果， 如果用户注册了该函数， 算法有结果时会调用该函数, 如果不注册则需要调用接口获取 */
    NI_S32(*pfn_ias_pc_result)
    (IAS_ALG_CHN PcChn, IAS_PC_RESULT_S *pstResult); /* 用于获取抓拍结果， 如果用户注册了该函数， 算法有结果时会调用该函数, 如果不注册则需要调用接口获取 */
    NI_S32(*pfn_ias_pp_result)
    (IAS_ALG_CHN PcChn, IAS_PC_PG_RESULT_S *pstResult); /* 用于获取抓拍结果， 如果用户注册了该函数， 算法有结果时会调用该函数, 如果不注册则需要调用接口获取 */
    NI_S32(*pfn_ias_release_frame)
    (IAS_ALG_CHN PcChn, IAS_TASK_IMAGE_S *pstFrame); /* 释放frame， 如果用户注册了该函数， 有结果时会调用该函数, 如果不注册则需要调用接口获取 */
} IAS_PC_CALLBACK_FUNC_S;

typedef struct tagIasPcPileLineCfg {
    IAS_PIPELINE_MODE_E enPipeMode; /* pipe 模式 */
    NI_U32              u32BufNum;  /* 缓冲数量 */
} IAS_PC_PIPELINE_CFG_S;

typedef struct tagIasPcProcCfg {
    IAS_PROC_MODE_E enProcMode; /* 处理模式，默认单任务处理模式*/
    NI_U32          au32Rev[3];
} IAS_PC_PROC_CFG_S;

typedef struct tagIasPdAlgParam {
    NI_U16     u16ScoreThr;  /* 置信度阈值 * 1000 */
    NI_U16     u16MaxObjNum; /* 最大检测目标数 */
    IAS_RECT_S stROIRect;    /* 目标ROI区域 */
} IAS_PD_ALG_PARAM_S;

typedef struct tagIasPtAlgParam {
    NI_BOOL bPositiveTrack; /* 主动跟踪使能*/
    NI_U16  u16MaxTrackNum; /* 最大跟踪目标数 */
    NI_U16  u16Fps;         /* 跟踪帧率*1000 */
} IAS_PT_ALG_PARAM_S;

typedef struct tagIasPsAlgParam {
    NI_BOOL bEnablePs;    /* 是否开启车牌优选*/
    NI_U16  u16ScoreThr;  /* 置信度阈值 * 1000 */
    NI_U16  u16MaxPlcNum; /* 最大优选目标数 */
} IAS_PS_ALG_PARAM_S;

typedef struct tagIasPpAlgParam {
    NI_BOOL bEnablePp; /* 是否开启周界*/
} IAS_PP_ALG_PARAM_S;

typedef struct tagIasPcParam {
    IAS_PD_ALG_PARAM_S stPdParam;
    IAS_PT_ALG_PARAM_S stPtParam;
    IAS_PS_ALG_PARAM_S stPsParam;
    IAS_PP_ALG_PARAM_S stPpParam;
} IAS_PC_PARAM_S;

typedef struct tagIasPcInitParam {
    IAS_LICENSE_S          stLicense;      /*Licesnce 预留*/
    IAS_IMAGE_SIZE_S       stImageSrcSize; /* 原图大小 */
    IAS_SIZE_CLIP_S        stObjSize;      /* 检测目标尺寸 */
    IAS_PC_MODEL_S         stAlgModel;     /* 算法模型文件 */
    IAS_PC_CALLBACK_FUNC_S stFunc;         /* 注册回调函数 */
    IAS_PC_PIPELINE_CFG_S  stPipeLine;     /* Pipeline配置 */
    IAS_PC_PARAM_S         stAlgCfg;       /* 算法配置 */
    IAS_PC_PROC_CFG_S      stProcCfg;      /* 算法任务模式配置 */
} IAS_PC_INIT_PARAM_S;

typedef struct tagIasPcNodeStatus {
    NI_CHAR acNodeName[IAS_PC_NODE_NAME_LEN]; /*node名*/
    NI_S32  s32MeanCost;                      /*平均耗时， us*/
    NI_S32  s32MinCost;                       /*最小耗时， us*/
    NI_S32  s32MaxCost;                       /*最大耗时， us*/
    NI_U32  u32TaskNum;                       /*目前未完成的任务数量*/
    NI_S32  s32IsRunning;                     /* 正在运行标记 */
    NI_U32  u32Runcnt;                        /*运行计数*/
} IAS_PC_TASK_NODE_STATUS_S;

typedef struct tagIasPcCoreStatus {
    NI_BOOL bEnable;    /*是否使能*/
    NI_U32  u32TaskNum; /*任务数量*/
    NI_U64  u64SumCost; /*现有任务总耗时*/
} IAS_PC_CORE_STATUS_S;

typedef struct tagIasPcProcStatus {
    IAS_PC_CORE_STATUS_S      astCoreStatus[32]; /* core 状态*/
    IAS_PC_TASK_NODE_STATUS_S stPdStatus;        /* FD节点状态*/
    IAS_PC_TASK_NODE_STATUS_S stPtStatus;        /* FT节点状态*/
    IAS_PC_TASK_NODE_STATUS_S stPsStatus;        /* FS节点状态*/
    IAS_PC_TASK_NODE_STATUS_S stPpStatus;        /* FP节点状态*/
    IAS_PC_TASK_NODE_STATUS_S stDvpStatus;       /* DVP节点状态*/
    IAS_PC_TASK_NODE_STATUS_S stNnStatus;        /* NN节点状态*/
    IAS_PC_TASK_NODE_STATUS_S stPostStatus;      /* POST节点状态*/
} IAS_PC_PROC_STATUS_S;

typedef struct tagIasPcStatus {
    NI_U32 u32Rev[8];
} IAS_PC_STATUS_S;

typedef enum tagAlgPcParamType {
    IAS_PC_PARAM_PD_MIN_SIZE   = 0, /* 最小目标框 */
    IAS_PC_PARAM_PD_MAX_SIZE   = 1, /* 最大目标框 */
    IAS_PC_PARAM_PD_SCORE_THRE = 2, /* 检测置信度阈值 * 1000 */
    IAS_PC_PARAM_TYPE_BUTT,
} IAS_ALG_PC_PARAM_TYPE_E;

typedef struct tagAlgPcParamList {
    NI_S32        u32ParamNum;
    IAS_PARAMS_S *pastArr;
} IAS_ALG_PC_PARAM_LIST_S;

/* ========================================================================== */
/*                             接口函数定义区                                  */
/* ========================================================================== */
/* ==========================================================================
* 函数名 : NI_IAS_PC_Init
* 描  述 : 创建PC实例
* 输  入 :
*          input    -i    PcChn   通道数
*          input    -i    pstInit 初始化参数结构体
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_PC_Init(IAS_ALG_CHN          PcChn,
                      IAS_PC_INIT_PARAM_S *pstInit);

/* ==========================================================================
* 函数名 : NI_IAS_PC_DeInit
* 描  述 : PC任务反初始化
* 输  入 :
*          input    -i    PcChn   通道数
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_PC_DeInit(IAS_ALG_CHN PcChn);

/* ==========================================================================
* 函数名 : NI_IAS_PC_GetParams
* 描  述 : 获取PC算法参数
* 输  入 :
*          input     -i    PcChn   通道数
* 输  出 :
*          output    -o    pstInit 初始化参数结构体
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_PC_GetParams(IAS_ALG_CHN     PcChn,
                           IAS_PC_PARAM_S *pstAlgParam);

/* ==========================================================================
* 函数名 : NI_IAS_PC_SetParams
* 描  述 : 设置PC算法参数
* 输  入 :
*          input    -i    PcChn         通道数
*          input    -i    pstAlgParam   参数地址
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_PC_SetParams(IAS_ALG_CHN PcChn,
                           NI_VOID    *pstAlgParam);

/* ==========================================================================
* 函数名 : NI_IAS_PC_Run
* 描  述 : PC运行以及结果获取， 要兼容后续的MP模式， 送帧和结果异步
* 输  入 :
*          input    -i    PcChn          通道数
*          input    -i    pstFrame       输入图像
*          input    -i    u32TimeOutMs   超时时间
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_PC_Run(IAS_ALG_CHN       PcChn,
                     IAS_TASK_IMAGE_S *pstFrame,
                     NI_U32            u32TimeOutMs);

/* ==========================================================================
* 函数名 : NI_IAS_PC_GetRectResult
* 描  述 : 获取检测目标结果， 结果中的内存由算法库分配
* 输  入 :
*          input     -i    PcChn           通道数
*          input     -i    u32TimeOutMs    超时时间
* 输  出 :
*          output    -O    pstResult       输出信息
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_PC_GetRectResult(IAS_ALG_CHN      PcChn,
                               IAS_PD_RESULT_S *pstResult,
                               NI_U32           u32TimeOutMs);

/* ==========================================================================
* 函数名 : NI_IAS_PC_ReleaseRectResult
* 描  述 : 结果释放检测目标接口
* 输  入 :
*          input     -i    PcChn        通道数
*          input     -i    pstResult    输出信息
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_PC_ReleaseRectResult(IAS_ALG_CHN      PcChn,
                                   IAS_PD_RESULT_S *pstResult);

/* ==========================================================================
* 函数名 : NI_IAS_PC_GetCaptureResult
* 描  述 : 获取抓拍结果， 结果中的内存由算法库分配
* 输  入 :
*          input     -i    PcChn           通道数
*          input     -i    u32TimeOutMs    超时门限
* 输  出 :
*          output    -O    pstResult       结果信息
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_PC_GetCaptureResult(IAS_ALG_CHN      PcChn,
                                  IAS_PC_RESULT_S *pstResult,
                                  NI_U32           u32TimeOutMs);

/* ==========================================================================
* 函数名 : NI_IAS_PC_ReleaseCaptureResult
* 描  述 : 抓拍结果释放
* 输  入 :
*          input    -i    PcChn        通道数
*          input    -i    pstResult    结果信息
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_PC_ReleaseCaptureResult(IAS_ALG_CHN      PcChn,
                                      IAS_PC_RESULT_S *pstResult);

/* ==========================================================================
* 函数名 : NI_IAS_PC_AddPerimeterRule
* 描  述 : 增加周界规则
* 输  入 :
*          input     -i    OdChn         通道数
*          input     -i    pstRule       规则框
* 输  出 :
*          output    -o    pu32RuleId    当前规则对应所有规则的ID
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_PC_AddPerimeterRule(IAS_ALG_CHN           OdChn,
                                  IAS_PERIMETER_RULE_S *pstRule,
                                  NI_U32               *pu32RuleId);

/* ==========================================================================
* 函数名 : NI_IAS_PC_DelPerimeterRule
* 描  述 : 删除周界规则
* 输  入 :
*          input     -i    OdChn      通道数
*          input     -i    pstRule    规则框
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_PC_DelPerimeterRule(IAS_ALG_CHN           OdChn,
                                  IAS_PERIMETER_RULE_S *pstRule);

/* ==========================================================================
* 函数名 : NI_IAS_PC_GetPgResult
* 描  述 : 获取周界结果， 结果中的内存由算法库分配
* 输  入 :
*          input     -i    OdChn           通道数
*          input     -i    u32TimeOutMs    超时门限
* 输  出 :
*          output    -O    pstResult       结果信息
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_PC_GetPgResult(IAS_ALG_CHN         OdChn,
                             IAS_PC_PG_RESULT_S *pstResult,
                             NI_U32              u32TimeOutMs);

/* ==========================================================================
* 函数名 : NI_IAS_PC_ReleasePgResult
* 描  述 : 结果释周界放接口
* 输  入 :
*          input    -i    OdChn        通道数
*          input    -i    pstResult    结果信息
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_PC_ReleasePgResult(IAS_ALG_CHN         OdChn,
                                 IAS_PC_PG_RESULT_S *pstResult);

/* ==========================================================================
* 函数名 : NI_IAS_PC_GetImageToRelease
* 描  述 : 获取待释放的图片，当某一张图算法库种不再使用时，通过该接口送出
* 输  入 :
*          input     -i    PcChn           通道数
*          input     -i    u32TimeOutMs    超时门限
* 输  出 :
*          output    -o    pstFrame        当前帧信息
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_PC_GetImageToRelease(IAS_ALG_CHN       PcChn,
                                   IAS_TASK_IMAGE_S *pstFrame,
                                   NI_U32            u32TimeOutMs);

/* ==========================================================================
* 函数名 : NI_IAS_PC_GetStatus
* 描  述 : 获取当前状态码
* 输  入 :
*          input     -i    PcChn        通道数
* 输  出 :
*          output    -o    pstStatus    当前状态码
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_PC_GetStatus(IAS_ALG_CHN      PcChn,
                           IAS_PC_STATUS_S *pstStatus);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of #ifndef __NI_IAS_PLATE_CAPTURE_H__ */
