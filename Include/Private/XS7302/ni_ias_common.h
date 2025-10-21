/*******************************************************************************
* Copyright (C) 2020-2022 ZheJiang xinsheng Technology CO., LTD.
*
* 文件名 : ni_ias_common.h
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
#ifndef __NI_IAS_ALG_COMMON_H__
#define __NI_IAS_ALG_COMMON_H__

/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */   
#include "ni_type.h"
#include "ni_debug.h"
#include "ni_common_video.h"
#include "ni_ias_define.h"

#ifdef __cplusplus
#if __cplusplus
    extern "C"{
#endif
#endif /* __cplusplus */

/* ========================================================================== */
/*                         宏和类型定义区                                     */
/* ========================================================================== */
#define NI_ID_IAS                (46)
#define IAS_ALG_MAX_NAME_LEN     (64)
#define IAS_ALG_TEST_PRINT_FLAG  (0)  /* 打印标记 */

// 错误码
#define NI_ERR_IAS_INVALID_CHNID     NI_DEF_ERR(NI_ID_IAS, EN_ERR_LEVEL_ERROR, ERR_INVALID_CHNID)
#define NI_ERR_IAS_ILLEGAL_PARAM     NI_DEF_ERR(NI_ID_IAS, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)
#define NI_ERR_IAS_NULL_PTR          NI_DEF_ERR(NI_ID_IAS, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)
#define NI_ERR_IAS_NOT_CONFIG        NI_DEF_ERR(NI_ID_IAS, EN_ERR_LEVEL_ERROR, ERR_NOT_CONFIG)
#define NI_ERR_IAS_NOT_SUPPORT       NI_DEF_ERR(NI_ID_IAS, EN_ERR_LEVEL_ERROR, ERR_NOT_SUPPORT)
#define NI_ERR_IAS_NOT_PERM          NI_DEF_ERR(NI_ID_IAS, EN_ERR_LEVEL_ERROR, ERR_NOT_PERM)
#define NI_ERR_IAS_NOMEM             NI_DEF_ERR(NI_ID_IAS, EN_ERR_LEVEL_ERROR, ERR_NOMEM)
#define NI_ERR_IAS_NOBUF             NI_DEF_ERR(NI_ID_IAS, EN_ERR_LEVEL_ERROR, ERR_NOBUF)
#define NI_ERR_IAS_BUF_EMPTY         NI_DEF_ERR(NI_ID_IAS, EN_ERR_LEVEL_ERROR, ERR_BUF_EMPTY)
#define NI_ERR_IAS_NOT_INIT          NI_DEF_ERR(NI_ID_IAS, EN_ERR_LEVEL_ERROR, ERR_SYS_NOTREADY)
#define NI_ERR_IAS_TIME_OUT          NI_DEF_ERR(NI_ID_IAS, EN_ERR_LEVEL_ERROR, ERR_TIME_OUT)

//
typedef unsigned long NI_IAS_PHY_ADDR;
typedef unsigned long NI_IAS_ADDR;
typedef unsigned long NI_IAS_LEN;
typedef NI_S32        IAS_ALG_CHN;
typedef NI_VOID*      IAS_ALG_HANDLE;

/* ========================================================================== */
/*                         结构体定义区                                     */
/* ========================================================================== */
typedef enum tagIasErrCode
{
    IAS_ERR_TIME_OUT          = 1,
    IAS_ERR_NOT_EXIST         = 2,
    IAS_ERR_INVALID_PARAM     = 3,
    IAS_ERR_NO_MEM            = 4,
    IAS_ERR_WRONG_ALG_VERSION = 5,
    IAS_ERR_WRONG_HANDLE      = 6,
    IAS_ERR_NO_BUF            = 7,
    IAS_ERR_DELAY             = 8,
} IAS_ERR_CODE_E;

typedef enum tagIasPipelineMode
{
    IAS_PIPELINE_MODE_SP  = 0, /* 算法依次处理 */
    IAS_PIPELINE_MODE_MPB = 1, /* 算法内并发处理 */
    IAS_PIPELINE_MODE_MPI = 2, /* 算法内部并发处理 */
    IAS_PIPELINE_MODE_BUTT,
} IAS_PIPELINE_MODE_E;

typedef enum tagIasProceMode
{
    IAS_PROC_MODE_SINGLE_TASK = 0, /* 算法单任务处理模式 */
    IAS_PROC_MODE_MULTI_TASK  = 1, /* 算法多任务处理模式 */
    IAS_PROC_MODE_BUTT,
} IAS_PROC_MODE_E;

typedef struct tagIasModelCfg
{
    NI_VOID             *pData;   /*模型数据存放虚拟地址*/
    NI_IAS_PHY_ADDR     lPhyAddr; /*模型数据存放物理地址*/
    NI_IAS_LEN          lLen;     /*模型数据长度*/
} IAS_MODEL_CFG_S;

typedef struct tagIasLicense
{
    NI_VOID     *pData;
    NI_U32      u32Len;
    NI_CHAR     *pcPath;
} IAS_LICENSE_S;

typedef struct tagIasIdInfo
{
    NI_S32     s32ObjId;    /* 目标 ID */
    NI_S32     s32FrameId;  /* 帧 ID */
    NI_S32     s32CameraId; /* 通道 ID */
    NI_S32     s32Rev;
} IAS_ID_INFO_S;

typedef enum tagIasObjType
{
    IAS_OBJ_TYPE_HUMAN       = 0,
    IAS_OBJ_TYPE_FACE        = 1,
    IAS_OBJ_TYPE_CAR         = 2,
    IAS_OBJ_TYPE_PERSON_BIKE = 3,
    IAS_OBJ_TYPE_PET         = 4,
    IAS_OBJ_TYPE_FACE_MASK   = 5,
    IAS_OBJ_TYPE_BIKE        = 6,
    IAS_OBJ_TYPE_EBIKE       = 7,
    IAS_OBJ_TYPE_PKG         = 8,
    IAS_OBJ_TYPE_PLATE       = 9,
    IAS_OBJ_TYPE_GES_PHONE   = 10,
    IAS_OBJ_TYPE_HEAD        = 11,
    IAS_OBJ_TYPE_GES_OK      = 12,
    IAS_OBJ_TYPE_BUTT,
} IAS_OBJ_TYPE_E;

typedef struct tagIasRect
{
    NI_S32 s32Xl;
    NI_S32 s32Yl; /* 左上点(ul) */
    NI_S32 s32Xr;
    NI_S32 s32Yr; /* 右下点(lr) */ 
} IAS_RECT_S;

typedef enum tagIasNnDataType
{
    IAS_DATA_TYPE_FP32   = 0,
    IAS_DATA_TYPE_FP16   = 1,
    IAS_DATA_TYPE_BFP16  = 2,
    IAS_DATA_TYPE_INT8   = 3,
    IAS_DATA_TYPE_UINT8  = 4,
    IAS_DATA_TYPE_INT16  = 5,
    IAS_DATA_TYPE_UINT16 = 6,

    IAS_DATA_TYPE_BUTT
}IAS_DATA_TYPE_E;

typedef struct tagPreProcNormalization
{
    NI_BOOL         bEnable;       /* 是否开启减均值归一化 */
    NI_U32          u32PreShift;   /* 预偏移值 */
    NI_U32          u32MeanVal;    /* 所减均值 1024量化后 */
    NI_U32          u32Coef;       /* 归一化系数,1024量化后 */
    IAS_DATA_TYPE_E enOutDataType; /* 输出数据类型 */
}PRE_PROC_NORMALIZATION_S;

typedef struct tagIasPoint
{
    NI_S16 s16X; /**< 横坐标 */
    NI_S16 s16Y; /**< 纵坐标 */
}IAS_POINT_S;

typedef struct tagIasImgSize
{
    NI_U16 u16ImageW; /**< 横坐标 */
    NI_U16 u16ImageH; /**< 纵坐标 */
}IAS_IMAGE_SIZE_S;

typedef struct tagIasSizeClip
{
    NI_U16 u16MaxSize; /* 目标最大尺寸 */
    NI_U16 u16MinSize; /* 目标最小尺寸 */
}IAS_SIZE_CLIP_S;

typedef struct tagIasTaskImage
{
    VIDEO_FRAME_S stFrame;
    NI_IAS_ADDR   lPrivateData;
} IAS_TASK_IMAGE_S;

typedef enum tagIasTaskExeDev
{
    IAS_TASK_EXE_DEV_MIX = 0,
    IAS_TASK_EXE_DEV_CPU = 1,
    IAS_TASK_EXE_DEV_NPU = 2,
    IAS_TASK_EXE_DEV_DVP = 3,
    IAS_TASK_EXE_DEV_DSP = 4,

    IAS_TASK_EXE_DEV_NUM
}IAS_TASK_EXE_DEV_E;

typedef enum tagIasBufMemType
{
    IAS_BUF_MEM_TYPE_FROM_SYS          = 0, /*系统申请，只有虚拟地址 */
    IAS_BUF_MEM_TYPE_FROM_MMZ_CACHED   = 1, /* MMZ申请带cache属性 */
    IAS_BUF_MEM_TYPE_FROM_MMZ_UNCACHED = 2, /* MMZ申请不带cache属性 */
    IAS_BUF_MEM_TYPE_BUTT,
} IAS_BUF_MEM_TYPE_E;

typedef struct tagIasTaskCoreInit
{
    NI_U8  au8CoreNum[IAS_TASK_EXE_DEV_NUM];      /*每一种core的数量*/
    NI_U16 au16InitTaskNum[IAS_TASK_EXE_DEV_NUM]; /*每一种core的任务队列初始任务数量*/
} IAS_TASK_CORE_INIT_S;

typedef struct tagIasTaskCoreState
{
    IAS_TASK_CORE_INIT_S stCoreState;
    NI_BOOL              bInit;
} IAS_TASK_CORE_STATE_S;

/* 管线模式 */
typedef struct tagIasPileLineCfg
{
    IAS_PIPELINE_MODE_E enPipeMode; /* pipe 模式 */
    NI_U32              u32BufNum;  /* 缓冲数量 */ 
} IAS_PIPELINE_CFG_S;

/* 周界类型 */
typedef enum tagPerimeterRuleType
{
    IAS_PERIMETER_RULE_POLYLINE = 0,     /*绊线*/         
    IAS_PERIMETER_RULE_POLYGON = 1,      /*区域*/             
    IAS_PERIMETER_RULE_BUTT,                
} IAS_PERIMETER_RULE_TYPE_E;

/* 周界方向设定 */
typedef enum tagPgDirection
{
    IAS_PG_DIRECTION_IN = 0,        /*进方向*/            
    IAS_PG_DIRECTION_OUT = 1,       /*出方向*/     
    IAS_PG_DIRECTION_BI = 2,        /*双向*/    
    IAS_PG_DIRECTION_BUTT,                
} IAS_PG_DIRECTION_E;

/* 线规则信息 */
typedef struct tagIasPerimeterRulePolyline
{
    IAS_PG_DIRECTION_E    enDirection;           /*报警方向*/
    NI_U32               u32PointNum;            /*折线的断点数量*/
    IAS_POINT_S          astPoint[XS_IAS_POLYLINE_MAX_POINTS_NUM];
} IAS_PERIMETER_RULE_POLYLINE_S;

/* 区域规则信息 */
typedef struct tagIasPerimeterRulePolygon
{
    IAS_PG_DIRECTION_E      enDirection;           /*报警方向*/
    NI_U32                  u32InsideDet;          /*区域内报警*/
    NI_U32                  u32PointNum;
    IAS_POINT_S             astPoint[XS_IAS_POLYGON_MAX_EDGES];
} IAS_PERIMETER_RULE_POLYGON_S;

/*周界规则*/
typedef struct tagIasPerimeterRule
{
    IAS_PERIMETER_RULE_TYPE_E   enType;
    union 
    {
        IAS_PERIMETER_RULE_POLYLINE_S   stPolyline;   
        IAS_PERIMETER_RULE_POLYGON_S    stPolygon;
    };
} IAS_PERIMETER_RULE_S;

typedef enum tagIasAdDataType
{       
    IAS_AD_DATA_U8    = 1,  /* 对应NI_U8数据大小 */ 
    IAS_AD_DATA_U16   = 2,  /* 对应NI_U16数据大小 */
    IAS_AD_DATA_U32   = 4,  /* 对应NI_U32数据大小 */
    IIAS_AD_DATA_BUTT,                
} IAS_AD_DATA_TYPE_E;

typedef struct tagIasTaskWAVE
{
    NI_U32              u32DataNum;   /* 当前数据点数 */
    NI_VOID            *pVData[1];    /* 当前数据 */
    NI_U32              u32Freq;      /* 采样频率 */
    IAS_AD_DATA_TYPE_E  u32DataType;  /* 对应单数据大小 1:NI_U8 , 2:NI_U16, 4:NI_U32 */
    NI_U32              u32FrameId;   /* 当前数据帧号 */
    NI_U32              au32Rev[3];
} IAS_TASK_WAVE_S;

typedef struct tagIasParams
{
    NI_S32  s32ByteSize; /**< 结构体大小，字节数 */
    NI_S32  s32Type;     /**< 结构体类型*/
    NI_S32  as32Rev[6];  /**< 保留字段*/
}IAS_PARAMS_S;

/* ========================================================================== */
/*                              函数定义区                                     */
/* ========================================================================== */
#define IAS_ALG_ALIGN(value, align) (((value + align - 1) / align) * align)
#define IAS_STRUCT_TYPE(data)    (((IAS_PARAMS_S *)data)->s32Type)
#define IAS_STRUCT_SIZE(data)    (((IAS_PARAMS_S *)data)->s32ByteSize)

NI_S32 IAS_CORE_Init(IAS_TASK_CORE_INIT_S *pstInit);
NI_S32 IAS_CORE_DeInit(NI_VOID);
NI_S32 IAS_CORE_State(IAS_TASK_CORE_STATE_S *pstState);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* End of #ifndef __NI_IAS_ALG_COMMON_H__ */

