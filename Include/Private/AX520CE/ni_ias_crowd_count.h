/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef __NI_IAS_CROWD_COUNT_H__
#define __NI_IAS_CROWD_COUNT_H__

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
#define IAS_CRC_MAX_OBJ_NUM   (50)
#define IAS_CRC_MAX_PARAM_NUM (8)
#define IAS_CRC_NODE_NAME_LEN (128)

/* ========================================================================== */
/*                           结构体定义区                                     */
/* ========================================================================== */
typedef struct tagIasCrcInfo {
    NI_U32 u32InNum;  /* 进入人数 */
    NI_U32 u32OutNum; /* 出去人数 */
    NI_U32 au32Rev[2];
} IAS_CRC_INFO_S;

typedef enum tagAlgCrdDirection {
    IAS_CRD_DIRECTION_NONE   = 0, /* 初始化状态 */
    IAS_CRD_DIRECTION_IN     = 1, /* 进入 */
    IAS_CRD_DIRECTION_OUT    = 2, /* 出去 */
    IAS_CRD_DIRECTION_IN_OUT = 3, /* 同时进出 */
    IAS_CRD_DIRECTION_BUTT,
} IAS_ALG_CRD_DIRECTION_E;

typedef struct tagIasCrdObjResult {
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
} IAS_CRC_OBJ_RESULT_S;

/*检测结果内容*/
typedef struct tagIasCrdResult {
    IAS_ALG_HANDLE        Handle;    /* 句柄信息 */
    IAS_TASK_IMAGE_S      stFrame;   /* 结果对应的图 */
    IAS_CRC_OBJ_RESULT_S *pstObj;    /* 目标结果存放地址， 由算法库分配内存， 用户拿到后需要将信息拷贝走 */
    NI_U32                u32ObjNum; /* 目标数量 */
    IAS_CRC_INFO_S        stCrcInfo; /* 人数统计信息 */
} IAS_CRC_RESULT_S;

typedef enum tagAlgCrdModelType {
    IAS_CRC_MODEL_READ_FROM_MEM  = 0, /* 从内存读取模型 */
    IAS_CRC_MODEL_READ_FROM_FILE = 1, /* 从文件读取模型 */
    IAS_CRC_MODEL_TYPE_BUTT,
} IAS_ALG_CRD_MODEL_TYPE_E;

typedef struct tagIasCrdModelCfg {
    IAS_MODEL_CFG_S stModelCfgCrd; /*CRD 算法模型*/
    IAS_MODEL_CFG_S stModelCfgCrt; /*CRT 算法模型*/
    IAS_MODEL_CFG_S stModelCfgCrc; /*CRC 算法模型*/
} IAS_CRC_CFG_S;

/*模型读取方式*/
typedef struct tagIasCrdModel {
    IAS_ALG_CRD_MODEL_TYPE_E enType; /*模型读取类型*/
    union {
        IAS_CRC_CFG_S stCrcModelCfg;                /* UNION:enType=IAS_CRC_MODEL_READ_FROM_MEM | CRC 算法模型配置 */
        NI_CHAR       acName[IAS_ALG_MAX_NAME_LEN]; /* UNION:enType=IAS_CRC_MODEL_READ_FROM_FILE | CRC 算法模型路径 */
    };
} IAS_CRC_MODEL_S;

typedef struct tagIasCrdCallBackFunc {
    NI_S32(*pfn_ias_crc_result)
    (IAS_ALG_CHN CrcChn, IAS_CRC_RESULT_S *pstResult); /* 用于获取检测结果， 如果用户注册了该函数， 算法有结果时会调用该函数, 如果不注册则需要调用接口获取 */
    NI_S32(*pfn_ias_release_frame)
    (IAS_ALG_CHN CrcChn, IAS_TASK_IMAGE_S *pstFrame); /* 释放frame， 如果用户注册了该函数， 有结果时会调用该函数, 如果不注册则需要调用接口获取 */
} IAS_CRC_CALLBACK_FUNC_S;

typedef struct tagIasCrdPipeLineCfg {
    IAS_PIPELINE_MODE_E enPipeMode; /* pipe 模式 */
    NI_U32              u32BufNum;  /* 缓冲数量 */
} IAS_CRC_PIPELINE_CFG_S;

typedef struct tagIasCrdProcCfg {
    IAS_PROC_MODE_E enProcMode; /* 处理模式，默认单任务处理模式*/
    NI_U32          au32Rev[3];
} IAS_CRC_PROC_CFG_S;

typedef enum tagAlgCrdLineDirection {
    IAS_CRC_LINE_UP_OUT_DWON_IN = 0, /* 上出下进 */
    IAS_CRC_LINE_UP_IN_DWON_OUT = 1, /* 上进下出 */
    IAS_CRC_LINE_BUTT,
} IAS_ALG_CRD_LINE_DIRECTION_E;

typedef struct tagIasCrdAlgParam {
    NI_U16                        u16ScoreThr;     /* 置信度阈值 * 1000 */
    NI_U16                        u16MaxObjNum;    /* 最大检测目标数 */
    IAS_PERIMETER_RULE_POLYGON_S  stPolygon;       /* 目标ROI区域 */
    IAS_ALG_CRD_LINE_DIRECTION_E  enLineDirection; /* 规则线判断方向 */
    IAS_PERIMETER_RULE_POLYLINE_S stPolyline;      /* 目标规则线 */
} IAS_CRD_ALG_PARAM_S;

typedef struct tagIasCrtAlgParam {
    NI_BOOL bEnableTrack; /* 开启跟踪使能 */
    NI_U16  au16Rev[8];
} IAS_CRT_ALG_PARAM_S;

typedef struct tagIasCrdParam {
    IAS_CRD_ALG_PARAM_S stCrdParam;
    IAS_CRT_ALG_PARAM_S stCrtParam;
} IAS_CRC_PARAM_S;

typedef struct tagIasCrdInitParam {
    IAS_LICENSE_S           stLicense;      /*Licesnce 预留*/
    IAS_IMAGE_SIZE_S        stImageSrcSize; /* 原图大小 */
    IAS_SIZE_CLIP_S         stObjSize;      /* 检测目标尺寸 */
    IAS_CRC_MODEL_S         stAlgModel;     /* 算法模型文件 */
    IAS_CRC_CALLBACK_FUNC_S stFunc;         /* 注册回调函数 */
    IAS_CRC_PIPELINE_CFG_S  stPipeLine;     /* Pipeline配置 */
    IAS_CRC_PARAM_S         stAlgCfg;       /* 算法配置 */
    IAS_CRC_PROC_CFG_S      stProcCfg;      /* 算法任务模式配置 */
} IAS_CRC_INIT_PARAM_S;

typedef struct tagIasCrdNodeStatus {
    NI_CHAR acNodeName[IAS_CRC_NODE_NAME_LEN]; /*node名*/
    NI_S32  s32MeanCost;                       /*平均耗时， us*/
    NI_S32  s32MinCost;                        /*最小耗时， us*/
    NI_S32  s32MaxCost;                        /*最大耗时， us*/
    NI_U32  u32TaskNum;                        /*目前未完成的任务数量*/
    NI_S32  s32IsRunning;                      /* 正在运行标记 */
    NI_U32  u32Runcnt;                         /*运行计数*/
} IAS_CRC_TASK_NODE_STATUS_S;

typedef struct tagIasCrdCoreStatus {
    NI_BOOL bEnable;    /*是否使能*/
    NI_U32  u32TaskNum; /*任务数量*/
    NI_U64  u64SumCost; /*现有任务总耗时*/
} IAS_CRC_CORE_STATUS_S;

typedef struct tagIasCrdProcStatus {
    IAS_CRC_CORE_STATUS_S      astCoreStatus[32]; /* core 状态*/
    IAS_CRC_TASK_NODE_STATUS_S stCrdStatus;       /* CRD节点状态*/
    IAS_CRC_TASK_NODE_STATUS_S stCrtStatus;       /* CRT节点状态*/
    IAS_CRC_TASK_NODE_STATUS_S stCrcStatus;       /* CRC节点状态*/
    IAS_CRC_TASK_NODE_STATUS_S stDvpStatus;       /* DVP节点状态*/
    IAS_CRC_TASK_NODE_STATUS_S stNnStatus;        /* NN节点状态*/
    IAS_CRC_TASK_NODE_STATUS_S stPostStatus;      /* POST节点状态*/
} IAS_CRC_PROC_STATUS_S;

typedef struct tagIasCrdStatus {
    NI_U32 u32Rev[8];
} IAS_CRC_STATUS_S;

typedef enum tagAlgCrcParamType {
    IAS_CRC_PARAM_CRD_MIN_SIZE   = 0, /* 最小目标框 */
    IAS_CRC_PARAM_CRD_MAX_SIZE   = 1, /* 最大目标框 */
    IAS_CRC_PARAM_CRD_SCORE_THRE = 2, /* 检测置信度阈值 * 1000 */
    IAS_CRC_PARAM_TYPE_BUTT,
} IAS_ALG_CRC_PARAM_TYPE_E;

typedef struct tagAlgCrcParamList {
    NI_S32        u32ParamNum;
    IAS_PARAMS_S *pastArr;
} IAS_ALG_CRC_PARAM_LIST_S;

/* ========================================================================== */
/*                             接口函数定义区                                  */
/* ========================================================================== */
/* ==========================================================================
* 函数名 : NI_IAS_CRC_Init
* 描  述 : 创建PC实例
* 输  入 :
*          input    -i    CrcChn   通道数
*          input    -i    pstInit 初始化参数结构体
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_CRC_Init(IAS_ALG_CHN           CrcChn,
                       IAS_CRC_INIT_PARAM_S *pstInit);

/* ==========================================================================
* 函数名 : NI_IAS_CRC_DeInit
* 描  述 : CRD任务反初始化
* 输  入 :
*          input    -i    CrcChn   通道数
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_CRC_DeInit(IAS_ALG_CHN CrcChn);

/* ==========================================================================
* 函数名 : NI_IAS_CRC_GetParams
* 描  述 : 获取CRD算法参数
* 输  入 :
*          input     -i    CrcChn   通道数
* 输  出 :
*          output    -o    pstInit 初始化参数结构体
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_CRC_GetParams(IAS_ALG_CHN      CrcChn,
                            IAS_CRC_PARAM_S *pstAlgParam);

/* ==========================================================================
* 函数名 : NI_IAS_CRC_SetParams
* 描  述 : 设置CRD算法参数
* 输  入 :
*          input    -i    CrcChn         通道数
*          input    -i    pstAlgParam   参数地址
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_CRC_SetParams(IAS_ALG_CHN CrcChn,
                            NI_VOID    *pstAlgParam);

/* ==========================================================================
* 函数名 : NI_IAS_CRC_SetFullyParams
* 描  述 : 设置CRD算法参数
* 输  入 :
*          input    -i    CrcChn         通道数
*          input    -i    pstAlgParam   参数地址
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_CRC_SetFullyParams(IAS_ALG_CHN      CrcChn,
                                 IAS_CRC_PARAM_S *pstAlgParam);

/* ==========================================================================
* 函数名 : NI_IAS_CRC_Run
* 描  述 : CRD运行以及结果获取， 要兼容后续的MP模式， 送帧和结果异步
* 输  入 :
*          input    -i    CrcChn          通道数
*          input    -i    pstFrame       输入图像
*          input    -i    u32TimeOutMs   超时时间
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_CRC_Run(IAS_ALG_CHN       CrcChn,
                      IAS_TASK_IMAGE_S *pstFrame,
                      NI_U32            u32TimeOutMs);

/* ==========================================================================
* 函数名 : NI_IAS_CRC_GetRectResult
* 描  述 : 获取检测目标结果， 结果中的内存由算法库分配
* 输  入 :
*          input     -i    CrcChn           通道数
*          input     -i    u32TimeOutMs    超时时间
* 输  出 :
*          output    -O    pstResult       输出信息
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_CRC_GetResult(IAS_ALG_CHN       CrcChn,
                            IAS_CRC_RESULT_S *pstResult,
                            NI_U32            u32TimeOutMs);

/* ==========================================================================
* 函数名 : NI_IAS_CRC_ReleaseRectResult
* 描  述 : 结果释放检测目标接口
* 输  入 :
*          input     -i    CrcChn        通道数
*          input     -i    pstResult    输出信息
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_CRC_ReleaseResult(IAS_ALG_CHN       CrcChn,
                                IAS_CRC_RESULT_S *pstResult);

/* ==========================================================================
* 函数名 : NI_IAS_CRC_GetImageToRelease
* 描  述 : 获取待释放的图片，当某一张图算法库种不再使用时，通过该接口送出
* 输  入 :
*          input     -i    CrcChn           通道数
*          input     -i    u32TimeOutMs    超时门限
* 输  出 :
*          output    -o    pstFrame        当前帧信息
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_CRC_GetImageToRelease(IAS_ALG_CHN       CrcChn,
                                    IAS_TASK_IMAGE_S *pstFrame,
                                    NI_U32            u32TimeOutMs);

/* ==========================================================================
* 函数名 : NI_IAS_CRC_GetStatus
* 描  述 : 获取当前状态码
* 输  入 :
*          input     -i    CrcChn        通道数
* 输  出 :
*          output    -o    pstStatus    当前状态码
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_CRC_GetStatus(IAS_ALG_CHN       CrcChn,
                            IAS_CRC_STATUS_S *pstStatus);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of #ifndef __NI_IAS_CROWD_COUNT_H__ */
