/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef __NI_IAS_OD_H__
#define __NI_IAS_OD_H__

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
#define IAS_OD_MAX_OBJ_NUM    (50)
#define IAS_OD_MAX_PARAM_NUM  (8)
#define IAS_OD_NODE_NAME_LEN  (128)
#define IAS_OD_OT_MAX_CHN_NUM (16)

/* ========================================================================== */
/*                           结构体定义区                                     */
/* ========================================================================== */
typedef struct tagIasOdObjResult {
    IAS_ID_INFO_S  stIdInfo;  /* 目标ID信息 */
    IAS_RECT_S     stRect;    /* 目标在原图上的位置 */
    IAS_OBJ_TYPE_E enType;    /* 目标类型*/
    NI_U16         u16Score;  /* 置信度 * 1000, */
    NI_U16         u16Status; /* 跟踪的状态 0：临时目标   1：目标创建  2：目标更新  3：目标丢失（目标隐藏）  4：目标删除， 跟踪关闭时无效*/
    NI_U32         u32PgCls;  /* PG分类 0: 刚触发报警，1: 报警后持续跟踪 */
    NI_U32         au32Rev[3];
} IAS_OD_OBJ_RESULT_S;

/*检测结果内容*/
typedef struct tagIasOdResult {
    IAS_ALG_HANDLE       Handle;    /* 句柄信息 */
    IAS_TASK_IMAGE_S     stFrame;   /* 结果对应的图 */
    IAS_OD_OBJ_RESULT_S *pstObj;    /* 目标结果存放地址， 由算法库分配内存， 用户拿到后需要将信息拷贝走 */
    NI_U32               u32ObjNum; /* 目标数量 */
} IAS_OD_RESULT_S;

typedef enum tagAlgOdModelType {
    IAS_OD_MODEL_READ_FROM_MEM  = 0, /* 从内存读取模型 */
    IAS_OD_MODEL_READ_FROM_FILE = 1, /* 从文件读取模型 */
    IAS_OD_MODEL_TYPE_BUTT,
} IAS_ALG_OD_MODEL_TYPE_E;

typedef struct tagIasOdModelCfg {
    IAS_MODEL_CFG_S stModelCfgOd; /*OD 算法模型*/
    IAS_MODEL_CFG_S stModelCfgOt; /*OT 算法模型*/
    IAS_MODEL_CFG_S stModelCfgOc; /*OC 算法模型*/
} IAS_OD_CFG_S;

/*模型读取方式*/
typedef struct tagIasOdModel {
    IAS_ALG_OD_MODEL_TYPE_E enType; /*模型读取类型*/
    union {
        IAS_OD_CFG_S stOdModelCfg;                 /* UNION:enType=IAS_OD_MODEL_READ_FROM_MEM | OD 算法模型配置 */
        NI_CHAR      acName[IAS_ALG_MAX_NAME_LEN]; /* UNION:enType=IAS_OD_MODEL_READ_FROM_FILE | OD 算法模型路径 */
    };
} IAS_OD_MODEL_S;

/*周界结果内容*/
typedef struct tagIasPgResult {
    IAS_ALG_HANDLE        Handle;    /* 句柄信息 */
    IAS_TASK_IMAGE_S      stFrame;   /* 结果对应的图 */
    IAS_OD_OBJ_RESULT_S  *pstObj;    /* 目标结果存放地址， 由算法库分配内存， 用户拿到后需要将信息拷贝走 */
    NI_U32                u32ObjNum; /* 目标数量 */
    IAS_PERIMETER_RULE_S *pstRule;   /* 触发的规则 */
    NI_U32                u32RuleId; /* 触发的规则ID */
} IAS_PG_RESULT_S;

typedef struct tagIasOdCallBackFunc {
    NI_S32(*pfn_ias_od_result)
    (IAS_ALG_CHN OdChn, IAS_OD_RESULT_S *pstResult); /* 用于获取检测结果， 如果用户注册了该函数， 算法有结果时会调用该函数, 如果不注册则需要调用接口获取 */
    NI_S32(*pfn_ias_pg_result)
    (IAS_ALG_CHN OdChn, IAS_PG_RESULT_S *pstResult); /* 用于获取周界结果， 如果用户注册了该函数， 算法有结果时会调用该函数, 如果不注册则需要调用接口获取 */
    NI_S32(*pfn_ias_release_frame)
    (IAS_ALG_CHN OdChn, IAS_TASK_IMAGE_S *pstFrame); /* 释放frame， 如果用户注册了该函数， 有结果时会调用该函数, 如果不注册则需要调用接口获取 */
} IAS_OD_CALLBACK_FUNC_S;

typedef struct tagIasOdPileLineCfg {
    IAS_PIPELINE_MODE_E enPipeMode; /* pipe 模式 */
    NI_U32              u32BufNum;  /* 缓冲数量 */
} IAS_OD_PIPELINE_CFG_S;

typedef struct tagIasOdProcCfg {
    IAS_PROC_MODE_E enProcMode; /* 处理模式，默认单任务处理模式*/
    NI_U32          au32Rev[3];
} IAS_OD_PROC_CFG_S;

typedef struct tagIasOdAlgParam {
    NI_U16 u16ScoreThr;  /* 置信度阈值 * 1000 */
    NI_U16 u16MaxObjNum; /* 最大检测目标数 */
} IAS_OD_ALG_PARAM_S;

typedef struct tagIasOcAlgParam {
    NI_BOOL bEnableOc;   /* 是否开启分类*/
    NI_U16  u16ScoreThr; /* 置信度阈值 * 1000 */
} IAS_OC_ALG_PARAM_S;

typedef struct tagIasOtAlgParam {
    NI_BOOL bPositiveTrack; /* 主动跟踪使能*/
    NI_U16  u16MaxTrackNum; /* 最大跟踪目标数 */
    NI_U16  u16Fps;         /* 跟踪帧率*1000 */
    NI_U16  u16ChnNum;      /* 跟踪帧视频通道数 */
    NI_U16  au16Rev[7];
} IAS_OT_ALG_PARAM_S;

typedef enum tagAlgPgLineType {
    IAS_OD_PG_LINE_DEFAULT = 0, /* 默认处理模式 */
    IAS_OD_PG_LINE_DIRECT  = 1, /* 根据输入点，切换方向模式 */
    IAS_OD_PG_LINE_BUTT,
} IAS_ALG_PG_LINE_TYPE_E;

typedef struct tagIasPgAlgParam {
    NI_BOOL                bEnablePg;     /* 是否开启周界*/
    IAS_ALG_PG_LINE_TYPE_E enLineType;    /* 绊线处理模式 */
    NI_U16                 u16EnterSense; /* 绊线灵敏度, 取值范围[1， 10]，默认建议5，数值越大表明越希望检测到小幅度的移动*/
    NI_U16                 au16Rev[5];
} IAS_PG_ALG_PARAM_S;

typedef enum tagAlgOfProcType {
    IAS_OD_OF_PROC_LEVEL_1 = 0, /* 静止目标过滤等级1 */
    IAS_OD_OF_PROC_LEVEL_2 = 1, /* 静止目标过滤等级2, 该模式下新增内存消耗160KB */
    IAS_OD_OF_PROC_LEVEL_3 = 2, /* 静止目标过滤等级3, 该模式下新增内存消耗160KB */
    IAS_OD_OF_PROC_BUTT,
} IAS_ALG_OF_PROC_TYPE_E;

typedef enum tagAlgOfFilterType {
    IAS_OD_OF_FILTER_DEFAULT  = 0, /* 所有目标类型均进行过滤 */
    IAS_OD_OF_FILTER_NO_HUMAN = 1, /* 人形目标不进行过滤 */
    IAS_OD_OF_FILTER_NO_CAR   = 2, /* 车型目标不进行过滤 */
    IAS_OD_OF_FILTER_BUTT,
} IAS_ALG_OF_FILTER_TYPE_E;

typedef struct tagIasOfAlgParam {
    NI_BOOL                  bEnableOf;    /* 是否开启静止目标过滤*/
    IAS_ALG_OF_PROC_TYPE_E   enProcType;   /* 静止目标过滤等级配置*/
    IAS_ALG_OF_FILTER_TYPE_E enFilterType; /* 静止目标过滤等级配置 */
} IAS_OF_ALG_PARAM_S;

typedef struct tagIasOdParam {
    IAS_OD_ALG_PARAM_S stOdParam;
    IAS_OT_ALG_PARAM_S stOtParam;
    IAS_PG_ALG_PARAM_S stPgParam;
    IAS_OC_ALG_PARAM_S stOcParam;
    IAS_OF_ALG_PARAM_S stOfParam;
} IAS_OD_PARAM_S;

typedef struct tagIasOdInitParam {
    IAS_LICENSE_S          stLicense;      /*Licesnce 预留*/
    IAS_IMAGE_SIZE_S       stImageSrcSize; /* 原图大小 */
    IAS_SIZE_CLIP_S        stObjSize;      /* 检测目标尺寸 */
    IAS_OD_MODEL_S         stAlgModel;     /* 算法模型文件 */
    IAS_OD_CALLBACK_FUNC_S stFunc;         /* 注册回调函数 */
    IAS_OD_PIPELINE_CFG_S  stPipeLine;     /* Pipeline配置 */
    IAS_OD_PARAM_S         stAlgCfg;       /* 算法配置 */
    IAS_OD_PROC_CFG_S      stProcCfg;      /* 算法任务模式配置 */
} IAS_OD_INIT_PARAM_S;

typedef struct tagIasOdNodeStatus {
    NI_CHAR acNodeName[IAS_OD_NODE_NAME_LEN]; /*node名*/
    NI_S32  s32MeanCost;                      /*平均耗时， us*/
    NI_S32  s32MinCost;                       /*最小耗时， us*/
    NI_S32  s32MaxCost;                       /*最大耗时， us*/
    NI_U32  u32TaskNum;                       /*目前未完成的任务数量*/
    NI_S32  s32IsRunning;                     /* 正在运行标记 */
    NI_U32  u32Runcnt;                        /*运行计数*/
} IAS_OD_TASK_NODE_STATUS_S;

typedef struct tagIasOdCoreStatus {
    NI_BOOL bEnable;    /*是否使能*/
    NI_U32  u32TaskNum; /*任务数量*/
    NI_U64  u64SumCost; /*现有任务总耗时*/
} IAS_OD_CORE_STATUS_S;

typedef struct tagIasOdProcStatus {
    IAS_OD_CORE_STATUS_S      astCoreStatus[32]; /* core 状态*/
    IAS_OD_TASK_NODE_STATUS_S stOdStatus;        /* OD节点状态*/
    IAS_OD_TASK_NODE_STATUS_S stOtStatus;        /* OT节点状态*/
    IAS_OD_TASK_NODE_STATUS_S stPgStatus;        /* PG节点状态*/
    IAS_OD_TASK_NODE_STATUS_S stDvpStatus;       /* DVP节点状态*/
    IAS_OD_TASK_NODE_STATUS_S stNnStatus;        /* NN节点状态*/
    IAS_OD_TASK_NODE_STATUS_S stPostStatus;      /* POST节点状态*/
} IAS_OD_PROC_STATUS_S;

typedef struct tagIasOdStatus {
    NI_U32 u32Rev[8];
} IAS_OD_STATUS_S;

typedef enum tagAlgOdParamType {
    IAS_OD_PARAM_OD_MIN_SIZE           = 0,  /* 最小目标框 */
    IAS_OD_PARAM_OD_MAX_SIZE           = 1,  /* 最大目标框 */
    IAS_OD_PARAM_OD_SCORE_THRE         = 2,  /* 检测置信度阈值 * 1000 */
    IAS_OD_PARAM_OD_PRINT              = 4,  /* 打印标记 */
    IAS_OD_PARAM_PG_SENSE              = 5,  /* 周界灵敏度 */
    IAS_OD_PARAM_PG_DIRECTION          = 6,  /* 周界方向 */
    IAS_OD_PARAM_PG_CLASS              = 7,  /* 周界目标类别 */
    IAS_OD_PARAM_PG_INSIDE_FLAG        = 8,  /* 周界区域报警类型 */
    IAS_OD_PARAM_PG_APPEAR_FLAG        = 9,  /* 周界区域出现功能 */
    IAS_OD_PARAM_PG_CROSSING_FLAG      = 10, /* 周界区域穿越功能 */
    IAS_OD_PARAM_PG_WANDER_TIME_MIN    = 11, /* 周界最短徘徊时间 */
    IAS_OD_PARAM_PG_WANDER_TIME_REPEAT = 12, /* 周界徘徊重复报警时间 */
    IAS_OD_PARAM_PG_FENCE_FLAG         = 13, /* 周界虚拟围栏报警类型 */
    IAS_OD_PARAM_TYPE_BUTT,
} IAS_ALG_OD_PARAM_TYPE_E;

typedef struct tagAlgOdParamList {
    NI_S32        u32ParamNum;
    IAS_PARAMS_S *pastArr;
} IAS_ALG_OD_PARAM_LIST_S;

typedef struct tagUsrAlgOdParamList {
    NI_S32       u32ParamNum;
    IAS_PARAMS_S astParams[IAS_OD_MAX_PARAM_NUM];
} IAS_ALG_USER_OD_PARAM_LIST_S;

/* ========================================================================== */
/*                             接口函数定义区                                  */
/* ========================================================================== */
/* ==========================================================================
* 函数名 : NI_IAS_OD_Init
* 描  述 : 创建OD实例
* 输  入 :
*          input    -i    OdChn   通道数
*          input    -i    pstInit 初始化参数结构体
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_OD_Init(IAS_ALG_CHN          OdChn,
                      IAS_OD_INIT_PARAM_S *pstInit);

/* ==========================================================================
* 函数名 : NI_IAS_OD_DeInit
* 描  述 : OD任务反初始化
* 输  入 :
*          input    -i    OdChn   通道数
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_OD_DeInit(IAS_ALG_CHN OdChn);

/* ==========================================================================
* 函数名 : NI_IAS_OD_GetParams
* 描  述 : 获取OD算法参数
* 输  入 :
*          input     -i    OdChn   通道数
* 输  出 :
*          output    -o    pstInit 初始化参数结构体
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_OD_GetParams(IAS_ALG_CHN     OdChn,
                           IAS_OD_PARAM_S *pstAlgParam);

/* ==========================================================================
* 函数名 : NI_IAS_OD_SetParams
* 描  述 : 设置OD算法参数
* 输  入 :
*          input    -i    OdChn          通道数
*          input    -i    pstAlgParam    参数地址
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_OD_SetParams(IAS_ALG_CHN OdChn,
                           NI_VOID    *pstAlgParam);

/* ==========================================================================
* 函数名 : NI_IAS_OD_Run
* 描  述 : OD运行以及结果获取， 要兼容后续的MP模式， 送帧和结果异步
* 输  入 :
*          input    -i    OdChn          通道数
*          input    -i    pstFrame       输入图像
*          input    -i    u32TimeOutMs   超时时间
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_OD_Run(IAS_ALG_CHN       OdChn,
                     IAS_TASK_IMAGE_S *pstFrame,
                     NI_U32            u32TimeOutMs);

/* ==========================================================================
* 函数名 : NI_IAS_OD_GetResult
* 描  述 : 获取检测目标结果， 结果中的内存由算法库分配
* 输  入 :
*          input     -i    OdChn           通道数
*          input     -i    u32TimeOutMs    超时时间
* 输  出 :
*          output    -O    pstResult       输出信息
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_OD_GetResult(IAS_ALG_CHN      OdChn,
                           IAS_OD_RESULT_S *pstResult,
                           NI_U32           u32TimeOutMs);

/* ==========================================================================
* 函数名 : NI_IAS_OD_ReleaseResult
* 描  述 : 结果释放检测目标接口
* 输  入 :
*          input     -i    OdChn        通道数
*          input     -i    pstResult    输出信息
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_OD_ReleaseResult(IAS_ALG_CHN      OdChn,
                               IAS_OD_RESULT_S *pstResult);

/* ==========================================================================
* 函数名 : NI_IAS_AddPerimeterRule
* 描  述 : 增加周界规则
* 输  入 :
*          input     -i    OdChn         通道数
*          input     -i    pstRule       规则框
* 输  出 :
*          output    -o    pu32RuleId    当前规则对应所有规则的ID
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_AddPerimeterRule(IAS_ALG_CHN           OdChn,
                               IAS_PERIMETER_RULE_S *pstRule,
                               NI_U32               *pu32RuleId);

/* ==========================================================================
* 函数名 : NI_IAS_DelPerimeterRule
* 描  述 : 删除周界规则
* 输  入 :
*          input     -i    OdChn      通道数
*          input     -i    pstRule    规则框
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_DelPerimeterRule(IAS_ALG_CHN           OdChn,
                               IAS_PERIMETER_RULE_S *pstRule);

/* ==========================================================================
* 函数名 : NI_IAS_PG_GetResult
* 描  述 : 获取周界结果， 结果中的内存由算法库分配
* 输  入 :
*          input     -i    OdChn           通道数
*          input     -i    u32TimeOutMs    超时门限
* 输  出 :
*          output    -O    pstResult       结果信息
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_PG_GetResult(IAS_ALG_CHN      OdChn,
                           IAS_PG_RESULT_S *pstResult,
                           NI_U32           u32TimeOutMs);

/* ==========================================================================
* 函数名 : NI_IAS_PG_ReleaseResult
* 描  述 : 结果释周界放接口
* 输  入 :
*          input    -i    OdChn        通道数
*          input    -i    pstResult    结果信息
* 输  出 : 无
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_PG_ReleaseResult(IAS_ALG_CHN      OdChn,
                               IAS_PG_RESULT_S *pstResult);

/* ==========================================================================
* 函数名 : NI_IAS_OD_GetImageToRelease
* 描  述 : 获取待释放的图片，当某一张图算法库种不再使用时，通过该接口送出
* 输  入 :
*          input     -i    OdChn           通道数
*          input     -i    u32TimeOutMs    超时门限
* 输  出 :
*          output    -o    pstFrame        当前帧信息
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_OD_GetImageToRelease(IAS_ALG_CHN       OdChn,
                                   IAS_TASK_IMAGE_S *pstFrame,
                                   NI_U32            u32TimeOutMs);

/* ==========================================================================
* 函数名 : NI_IAS_OD_GetStatus
* 描  述 : 获取当前状态码
* 输  入 :
*          input     -i    OdChn        通道数
* 输  出 :
*          output    -o    pstStatus    当前状态码
* 返  回 : 状态码
========================================================================== */
NI_S32 NI_IAS_OD_GetStatus(IAS_ALG_CHN      OdChn,
                           IAS_OD_STATUS_S *pstStatus);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of #ifndef __DH_IAS_OD_H__ */
