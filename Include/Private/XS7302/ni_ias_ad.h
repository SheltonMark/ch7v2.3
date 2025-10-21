/*******************************************************************************
* Copyright (C) 2020-2022 ZheJiang xinsheng Technology CO., LTD.
*
* 文件名 : ni_ias_ad.h
*
* 作  者 : Chipup Software Team
* 版  本 : V1.0.0
* 日  期 : 2022年03月04日 Create
*
* 描  述 : 
*
*       1. 硬件说明
*          无。
*
*       2. 程序结构说明。
*          无。
*
*       3. 使用说明。
*          无。
*
*       4. 局限性说明。
*          无。
*
*       5. 其他说明。
*          无。
*
* 修订历史: 
* 日    期: 
* 修订版本:
* 作    者:
* 修订备注:
*******************************************************************************/
#ifndef __NI_IAS_AD_H__
#define __NI_IAS_AD_H__

/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "ni_type.h"
#include "ni_debug.h"
#include "ni_common_video.h"
#include "ni_ias_define.h"
#include "ni_ias_common.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

/* ========================================================================== */
/*                           宏定义区                                         */
/* ========================================================================== */
#define IAS_AD_NODE_NAME_LEN  (128)
#define IAS_AD_MAX_PARAM_NUM  (8)

/* ========================================================================== */
/*                           结构体定义区                                     */
/* ========================================================================== */
typedef enum tagIasAdProcType
{
    IAS_AD_PROC_NO_CRY  = 0,  /* 无哭泣 */         
    IAS_AD_PROC_CRY     = 1,  /* 哭泣 */         
    IAS_AD_PROC_TYPE_BUTT,                
} IAS_AD_PROC_TYPE_E;

typedef struct tagIasAdObjResult
{       
    NI_U16             u16Score;      /* 置信度 * 1000, */
    IAS_AD_PROC_TYPE_E enCryStatus;  /* 哭泣状态 */
    NI_U32             au32Rev[7];
} IAS_AD_OBJ_RESULT_S;

/*检测结果内容*/
typedef struct tagIasAdResult
{
    IAS_ALG_HANDLE        Handle;     /* 句柄信息 */
    IAS_TASK_WAVE_S       stFrame;    /* 结果对应的数据 */
    IAS_AD_OBJ_RESULT_S   stResult;   /* 目标结果 */
} IAS_AD_RESULT_S;

typedef enum tagAlgAdModelType
{
    IAS_AD_MODEL_READ_FROM_MEM  = 0,     /* 从内存读取模型 */         
    IAS_AD_MODEL_READ_FROM_FILE = 1,     /* 从文件读取模型 */         
    IAS_AD_MODEL_TYPE_BUTT,                
} IAS_ALG_AD_MODEL_TYPE_E;

typedef struct tagIasAdModelCfg
{
    IAS_MODEL_CFG_S      stModelCfgAd;    /*AD 算法模型*/      
} IAS_AD_CFG_S;

/*模型读取方式*/
typedef struct tagIasAdModel
{
    IAS_ALG_AD_MODEL_TYPE_E   enType;                           /*模型读取类型*/
    union 
    {
        IAS_AD_CFG_S       stAdModelCfg;                        /* OD 算法模型配置 */
        NI_CHAR            acName[IAS_ALG_MAX_NAME_LEN];        /* OD 算法模型路径 */
    };
} IAS_AD_MODEL_S;

typedef struct tagIasAdCallBackFunc
{
    NI_S32 (*pfn_ias_ad_result)(IAS_ALG_CHN AdChn, IAS_AD_RESULT_S *pstResult);     /* 用于获取检测结果， 如果用户注册了该函数， 算法有结果时会调用该函数, 如果不注册则需要调用接口获取 */
} IAS_AD_CALLBACK_FUNC_S;

typedef struct tagIasAdPipeLineCfg
{
    IAS_PIPELINE_MODE_E   enPipeMode; /* pipe 模式 */
    NI_U32                u32BufNum;  /* 缓冲数量 */ 
    NI_BOOL               bSerial;    /* 开启串行，不使用线程， 在PIPE模式下设置为TRUE，其余模式为FALSE */ 
} IAS_AD_PIPELINE_CFG_S;

typedef struct tagIasAdProcCfg
{
    IAS_PROC_MODE_E enProcMode;   /* 处理模式，默认单任务处理模式*/
    NI_U32          au32Rev[3];
} IAS_AD_PROC_CFG_S;

typedef struct tagIasAdAlgParam
{
    NI_U16 u16ScoreThr; /* 置信度阈值 * 1000 */
    NI_U16 au16Rev[7];
} IAS_AD_ALG_PARAM_S;

typedef enum tagIasAdAlarmType
{
    IAS_AD_ALARM_CONTINUED_TIME = 0,  /* 报警持续时间 */         
    IAS_AD_ALARM_MULTI_FRAME    = 1,  /* 多帧数据报警 */         
    IAS_AD_ALARM_TYPE_BUTT,                
} IAS_AD_ALARM_TYPE_E;

typedef struct tagIasAdAlarmContinueTimeParam
{
    NI_U32 u32AlarmContinueTime; // 报警持续时间
    NI_U16 au16Rev[6];
} IAS_AD_ALARM_CONTINUE_TIME_PARAM_S;

typedef struct tagIasAdAlarmMultiFrameParam
{
    NI_U16 u16AlarmNum; // 报警数
    NI_U16 au16Rev[7];
} IAS_AD_ALARM_MULTI_FRAME_PARAM_S;

typedef struct tagIasAdAlarmParam
{
    IAS_AD_ALARM_TYPE_E enAlarmType; /* 报警类型 */
    union 
    {
        IAS_AD_ALARM_CONTINUE_TIME_PARAM_S stAdAlarmTimeParam; /* 报警持续时间模式下参数 */
        IAS_AD_ALARM_MULTI_FRAME_PARAM_S   stAdAlarmParams;    /* 报警帧模式下参数 */  
    };
} IAS_AD_ALARM_PARAM_S;

typedef enum tagIasAdAlarmModeType
{
    IAS_AD_ALARM_ACCURACY_MODE = 0,  /* 精确模式 */
    IAS_AD_ALARM_MEDIAN_MODE   = 1,  /* 平衡模式 */
    IAS_AD_ALARM_FAST_MODE     = 2,  /* 快速模式 */
    IAS_AD_ALARM_MODE_TYPE_BUTT,                
} IAS_AD_ALARM_MODE_TYPE_E;

typedef struct tagIasAdParam
{
    IAS_AD_ALG_PARAM_S        stAdParam;
    IAS_AD_ALARM_PARAM_S      stAdAlarmParam;
    NI_U32                    u32SampleFreq;
    IAS_AD_ALARM_MODE_TYPE_E  enAlarmModeType;
    NI_U32                    u32Gain;        // 默认为0, 为自适应配置增益，若非0时，则手动调节增益 
} IAS_AD_PARAM_S;

typedef struct tagIasAdInitParam
{
    IAS_LICENSE_S                   stLicense;            /*Licesnce 预留*/
    IAS_AD_MODEL_S                  stAlgModel;           /* 算法模型文件 */
    IAS_AD_CALLBACK_FUNC_S          stFunc;               /* 注册回调函数 */
    IAS_AD_PIPELINE_CFG_S           stPipeLine;           /* Pipeline配置 */
    IAS_AD_PARAM_S                  stAlgCfg;             /* 算法配置 */
    IAS_AD_PROC_CFG_S               stProcCfg;            /* 算法任务模式配置 */
} IAS_AD_INIT_PARAM_S;

typedef struct tagIasAdNodeStatus
{
    NI_CHAR acNodeName[IAS_AD_NODE_NAME_LEN]; /*node名*/
    NI_S32  s32MeanCost;                        /*平均耗时， us*/
    NI_S32  s32MinCost;                         /*最小耗时， us*/
    NI_S32  s32MaxCost;                         /*最大耗时， us*/
    NI_U32  u32TaskNum;                         /*目前未完成的任务数量*/
    NI_S32  s32IsRunning;                       /* 正在运行标记 */
    NI_U32  u32Runcnt;                          /*运行计数*/
} IAS_AD_TASK_NODE_STATUS_S;

typedef struct tagIasAdCoreStatus
{
    NI_BOOL   bEnable;    /*是否使能*/
    NI_U32    u32TaskNum; /*任务数量*/
    NI_U64    u64SumCost; /*现有任务总耗时*/
} IAS_AD_CORE_STATUS_S;

typedef struct tagIasAdProcStatus
{
    IAS_AD_CORE_STATUS_S       astCoreStatus[32]; /* core 状态*/
    IAS_AD_TASK_NODE_STATUS_S  stAdStatus;        /* AD节点状态*/
    IAS_AD_TASK_NODE_STATUS_S  stPreStatus;        /* 预处理节点状态*/
    IAS_AD_TASK_NODE_STATUS_S  stNnStatus;        /* NN节点状态*/
    IAS_AD_TASK_NODE_STATUS_S  stPostStatus;      /* POST节点状态*/
} IAS_AD_PROC_STATUS_S;

typedef struct tagIasAdStatus
{
    NI_U32 u32Rev[8];
} IAS_AD_STATUS_S;

typedef enum tagAlgAdParamType
{
    IAS_AD_PARAM_AD_SCORE_THRE = 0, /* 配置检测阈值 */
    IAS_AD_PARAM_TYPE_BUTT,
} IAS_ALG_AD_PARAM_TYPE_E;

typedef struct tagAlgAdParamList
{
    NI_S32        u32ParamNum;
    IAS_PARAMS_S *pastArr;
} IAS_ALG_AD_PARAM_LIST_S;

/* ========================================================================== */
/*                             接口函数定义区                                  */
/* ========================================================================== */
/* ========================================================================== 
* 函数名 : NI_IAS_AD_Init
* 描  述 : 创建OD实例
* 输  入 : 
*          input    -i    AdChn   通道数
*          input    -i    pstInit 初始化参数结构体
* 输  出 : 无
* 返  回 : 状态码 
========================================================================== */
NI_S32 NI_IAS_AD_Init(IAS_ALG_CHN          AdChn, 
                      IAS_AD_INIT_PARAM_S *pstInit);

/* ========================================================================== 
* 函数名 : NI_IAS_AD_DeInit
* 描  述 : OD任务反初始化
* 输  入 : 
*          input    -i    AdChn   通道数
* 输  出 : 无
* 返  回 : 状态码 
========================================================================== */
NI_S32 NI_IAS_AD_DeInit(IAS_ALG_CHN AdChn);

/* ========================================================================== 
* 函数名 : NI_IAS_AD_Run
* 描  述 : OD运行以及结果获取， 要兼容后续的MP模式， 送帧和结果异步
* 输  入 : 
*          input    -i    AdChn          通道数
*          input    -i    pstFrame       输入图像
*          input    -i    u32TimeOutMs   超时时间
* 输  出 : 无
* 返  回 : 状态码 
========================================================================== */
NI_S32 NI_IAS_AD_Run(IAS_ALG_CHN      AdChn,
                     IAS_TASK_WAVE_S *pstFrame, 
                     NI_U32           u32TimeOutMs);

/* ========================================================================== 
* 函数名 : NI_IAS_AD_GetResult
* 描  述 : 获取检测目标结果， 结果中的内存由算法库分配
* 输  入 : 
*          input     -i    AdChn           通道数
*          input     -i    u32TimeOutMs    超时时间
* 输  出 : 
*          output    -O    pstResult       输出信息
* 返  回 : 状态码 
========================================================================== */
NI_S32 NI_IAS_AD_GetResult(IAS_ALG_CHN      AdChn, 
                           IAS_AD_RESULT_S *pstResult, 
                           NI_U32           u32TimeOutMs);

/* ========================================================================== 
* 函数名 : NI_IAS_AD_ReleaseResult
* 描  述 : 结果释放检测目标接口
* 输  入 : 
*          input     -i    AdChn        通道数
*          input     -i    pstResult    输出信息
* 输  出 : 无
* 返  回 : 状态码 
========================================================================== */
NI_S32 NI_IAS_AD_ReleaseResult(IAS_ALG_CHN      AdChn, 
                               IAS_AD_RESULT_S *pstResult);

/* ========================================================================== 
* 函数名 : NI_IAS_AD_GetParams
* 描  述 : 获取AD算法参数
* 输  入 : 
*          input     -i    AdChn   通道数
* 输  出 : 
*          output    -o    pstInit 初始化参数结构体
* 返  回 : 状态码 
========================================================================== */
NI_S32 NI_IAS_AD_GetParams(IAS_ALG_CHN     AdChn, 
                           IAS_AD_PARAM_S *pstAlgParam);

/* ========================================================================== 
* 函数名 : NI_IAS_AD_SetParams
* 描  述 : 设置AD算法参数
* 输  入 : 
*          input    -i    AdChn          通道数
*          input    -i    pstAlgParam    参数地址
* 输  出 : 无
* 返  回 : 状态码 
========================================================================== */
NI_S32 NI_IAS_AD_SetParams(IAS_ALG_CHN  AdChn, 
                           NI_VOID     *pstAlgParam);

/* ========================================================================== 
* 函数名 : NI_IAS_AD_GetStatus
* 描  述 : 获取当前状态码
* 输  入 : 
*          input     -i    AdChn        通道数
* 输  出 : 
*          output    -o    pstStatus    当前状态码
* 返  回 : 状态码 
========================================================================== */
NI_S32 NI_IAS_AD_GetStatus(IAS_ALG_CHN      AdChn, 
                           IAS_AD_STATUS_S *pstStatus);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of #ifndef __NI_IAS_AD_H__ */




