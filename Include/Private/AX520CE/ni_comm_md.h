/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NI_COMM_MD_H_
#define _NI_COMM_MD_H_

#include "ni_type.h"
#include "ni_common.h"
#include "ni_define.h"
#include "ni_errno.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef NI_S32 MD_CHN;

#define MD_MAX_CHN_NUM       (2) /* 最大通道数 */
#define MD_MAX_DETECT_REGION (4) /* 最多检测区域 */

#define NI_ID_MD (23) /* MD ID */

/* 错误码 */
typedef enum tagMdErrCode {
    ERR_MD_NOT_INIT        = 0x3c, /* 未初始化 */
    ERR_MD_CHN_HAS_STARTED = 0x3d, /* 重复开启检查使能 */
    ERR_MD_CHN_NOT_START   = 0x3e, /* 检测使能未开启 */
    ERR_MD_TIMEOUT         = 0x3f, /* 超时 */
    ERR_MD_YUV_TYPE_ERR    = 0x40, /* YUV 图像类型错误 */
} MD_ERR_CODE_E;

#define NI_ERR_MD_INVALID_CHNID   NI_DEF_ERR(NI_ID_MD, EN_ERR_LEVEL_ERROR, ERR_INVALID_CHNID)
#define NI_ERR_MD_ILLEGAL_PARAM   NI_DEF_ERR(NI_ID_MD, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)
#define NI_ERR_MD_CHN_EXIST       NI_DEF_ERR(NI_ID_MD, EN_ERR_LEVEL_ERROR, ERR_EXIST)
#define NI_ERR_MD_CHN_UNEXIST     NI_DEF_ERR(NI_ID_MD, EN_ERR_LEVEL_ERROR, ERR_UNEXIST)
#define NI_ERR_MD_NULL_PTR        NI_DEF_ERR(NI_ID_MD, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)
#define NI_ERR_MD_NOMEN           NI_DEF_ERR(NI_ID_MD, EN_ERR_LEVEL_ERROR, ERR_NOMEM)
#define NI_ERR_MD_NOT_INIT        NI_DEF_ERR(NI_ID_MD, EN_ERR_LEVEL_ERROR, ERR_MD_NOT_INIT)
#define NI_ERR_MD_CHN_HAS_STARTED NI_DEF_ERR(NI_ID_MD, EN_ERR_LEVEL_ERROR, ERR_MD_CHN_HAS_STARTED)
#define NI_ERR_MD_CHN_NOT_START   NI_DEF_ERR(NI_ID_MD, EN_ERR_LEVEL_ERROR, ERR_MD_CHN_NOT_START)
#define NI_ERR_MD_TIMEOUT         NI_DEF_ERR(NI_ID_MD, EN_ERR_LEVEL_ERROR, ERR_MD_TIMEOUT)
#define NI_ERR_MD_YUV_TYPE_ERR    NI_DEF_ERR(NI_ID_MD, EN_ERR_LEVEL_ERROR, ERR_MD_YUV_TYPE_ERR)

/* 盲检模式 */
typedef enum tagMdBdMode {
    MD_BD_MODE_LUMA  = 0, /* 根据画面亮度检测 */
    MD_BD_MODE_SCENE = 1, /* 根据场景环境检测 */

    MD_BD_MODE_BUTT
} MD_BD_MODE_E;

/* 检测模式 */
typedef enum tagMdDetectSel {
    MD_DETECT_SEL_STOP  = 0, /* 同时停止动检盲检 */
    MD_DETECT_SEL_MD    = 1, /* 只开启动检 */
    MD_DETECT_SEL_BD    = 2, /* 只开启盲检 */
    MD_DETECT_SEL_BD_MD = 3, /* 同时开启动检盲检 */

    MD_DETECT_SEL_BUTT
} MD_DETECT_SEL_E;

/* 工作模式 */
typedef struct tagMdWorkMode {
    MD_DETECT_SEL_E enDetectSel; /* 检测模式 */
} MD_WORK_MODE_S;

/* 数据格式 */
typedef struct tagMdDataBuffer {
    NI_U32   u32PhyAddr; /* 物理地址 */
    NI_VOID *pVirAddr;   /* 虚拟地址 */
    NI_U32   u32Size;    /* 数据长度 */
} MD_DATA_BUFFER_S;

/* 动检数据 */
typedef struct tagMdMdData {
    NI_BOOL          bAomResultValid;                     /* Aom区域结构数据是否有效 */
    NI_BOOL          abAomMDResult[MD_MAX_DETECT_REGION]; /* Aom区域动检结果 */
    MD_DATA_BUFFER_S astAomResult[MD_MAX_DETECT_REGION];  /* Aom区域子块动检结果 */
} MD_DATA_S;

/* 盲检数据 */
typedef struct tagMdBdData {
    NI_BOOL bBdDetect;        /* 盲检结果 */
    NI_U32  u32BdBpsum;       /* 黑电平像素点数, 仅模式MD_BD_MODE_LUMA有效 */
    NI_U32  u32BdTotalPixSum; /* 总像素点个数, 仅模式MD_BD_MODE_LUMA有效 */
    NI_U32  u32BdCurVal;      /* 当前画面评分，仅模式MD_BD_MODE_SCENE有效, 遮挡越多评分越低 */
} MD_BD_DATA_S;

/* MD 结果 */
typedef struct tagMdResult {
    NI_BOOL      bBdDataExist; /* 盲检数据是否存在 */
    NI_BOOL      bMdDataExist; /* 动检数据是否存在 */
    MD_DATA_S    stMdData;     /* 动检数据 */
    MD_BD_DATA_S stBdData;     /* 盲检数据 */
} MD_RESULT_S;

/* 动检参数 */
typedef struct tagMdParam {
    NI_U8 u8MdNoiseUpCoe;     /* 动检噪声学习速度, [0,127] */
    NI_U8 u8MdNoiseSlowCoe;   /* 动检噪声学习降低系数, [0,7] */
    NI_U8 u8MdNoiseToDiffCoe; /* 动检残差判定为非噪声的判决系数, [0,15] */
    NI_U8 u8MdStudyFrameNum;  /* 动检学习帧数 */
} MD_PARAM_S;

/* 画面分块参数 */
typedef struct tagMdSbParam {
    NI_U8 u8SbHorizontalNum; /* 动检分割区域水平SB个数, [1,80], 初始化后无法修改 */
    NI_U8 u8SbVerticalNum;   /* 动检分割区域垂直SB个数, [1,80], 初始化后无法修改 */
} MD_SB_PARAM_S;

/* 动检 Aom 参数 */
typedef struct tagAomParam {
    NI_BOOL bAomEnable;      /* Aom区域是否使能 */
    NI_U8   u8AomSense;      /* 运动检测区域的灵敏度, [0,127] */
    NI_U8   u8AomDiffCoe;    /* 运动检测区域残差适配系数, [0,63] */
    NI_U16  u16AomMdthrd;    /* 运动检测区域的动检阈值, [0,1800] */
    NI_U16  u16AomDiffLevel; /* 运动检测区域残差基准,[0,4095] */
} MD_AOM_PARAM_S;

/* 动检区域参数 */
typedef struct tagMdDetectRegion {
    NI_U32     u32AomId;     /* Aom ID, [0,3] */
    NI_BOOL    bRectDetect;  /* 当前Rect是否使能检测 */
    RECT_POS_S stDetectRect; /* Region区域 */
} MD_DETECT_REGION_S;

/* 动检参数 */
typedef struct tagMdMdAttr {
    MD_PARAM_S     stMdParam;                        /* 动检参数 */
    MD_SB_PARAM_S  stSbParam;                        /* 分块参数 */
    MD_AOM_PARAM_S stAomParam[MD_MAX_DETECT_REGION]; /* Aom参数 */
} MD_MD_ATTR_S;

/* 盲检参数 */
typedef struct tagMdBdAttr {
    MD_BD_MODE_E enBdMode;           /* 遮挡检测模式, 推荐模式MD_BD_MODE_SCENE */
    NI_U8        u8BdGate;           /* 遮挡检测判决阈值, [0,100], 仅模式MD_BD_MODE_LUMA有效 */
    NI_U8        u8BdPixelBlackGate; /* 黑电平判决阈值, [0,255], 仅模式MD_BD_MODE_LUMA有效 */
    NI_U8        u8BdPixelStep;      /* 遮挡检测跳点距离, [0,255], 仅模式MD_BD_MODE_LUMA有效 */
    NI_U8        u8BdFrameCount;     /* 遮挡检测窗口累计帧数, 每次判断参考窗口内累计的帧数, [1,127], 仅模式MD_BD_MODE_SCENE有效, 推荐值5 */
    NI_U32       u32BdThd;           /* 遮挡检测画面判决阈值, [0,3000], 仅模式MD_BD_MODE_SCENE有效, 推荐值700 */
} MD_BD_ATTR_S;

/* 通道属性 */
typedef struct tagMdChnAttr {
    NI_U32       u32Width;     /* 检测图像宽度, [16,1280], 初始化后无法修改 */
    NI_U32       u32Height;    /* 检测图像高度, [16,1280], 初始化后无法修改 */
    NI_U32       u32Interval;  /* 帧间隔, [1,255], 初始化后无法修改, 使用时请将u32DetPerFrm配置为1 */
    NI_U32       u32DetPerFrm; /* 检测间隔帧数, [1,255], 初始化后无法修改, 使用时请将u32Interval配置为1 */
    MD_MD_ATTR_S stMdAttr;     /* 动检属性 */
    MD_BD_ATTR_S stBdAttr;     /* 盲检属性 */
} MD_CHN_ATTR_S;

#ifdef __cplusplus
}
#endif

#endif
