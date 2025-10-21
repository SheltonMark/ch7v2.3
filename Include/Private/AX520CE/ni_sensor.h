/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NI_SENSOR_H_
#define _NI_SENSOR_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "ni_common.h"
#include "ni_type.h"

#define SEN_DOW_MAX_FRAME_NUM (5)

/*sensorType*/
typedef enum tagSenType {
    /*OS系列*/
    SEN_TYPE_OS08A10 = 0,
    SEN_TYPE_OS05A10,
    SEN_TYPE_OS02B10,
    SEN_TYPE_OS02G10,
    SEN_TYPE_OS02K10,
    SEN_TYPE_OS04A10,
    SEN_TYPE_OS02H10,
    SEN_TYPE_OS04C10,
    SEN_TYPE_OS03B10,
    SEN_TYPE_OS04B10,
    SEN_TYPE_OS04L10,
    SEN_TYPE_OS02M10,
    SEN_TYPE_OS02N10,
    SEN_TYPE_OS04D10,

    /*OV系列*/
    SEN_TYPE_OV4689 = 1000,
    SEN_TYPE_OV2710,
    SEN_TYPE_OV2735,
    SEN_TYPE_OV4689_2A,
    SEN_TYPE_OV9281,
    SEN_TYPE_OV9726,
    SEN_TYPE_OV2778,
    SEN_TYPE_OV9750,
    SEN_TYPE_OV08A10,
    SEN_TYPE_OV9623,

    /*IMX系列*/
    SEN_TYPE_IMX290 = 3000,
    SEN_TYPE_IMX307,
    SEN_TYPE_IMX323,
    SEN_TYPE_IMX415,
    SEN_TYPE_IMX327,

    /*SC系列*/
    SEN_TYPE_SC2238 = 4000,
    SEN_TYPE_SC5238,
    SEN_TYPE_SC2310,
    SEN_TYPE_SC2239,
    SEN_TYPE_SC4238,
    SEN_TYPE_SC1330,
    SEN_TYPE_SC1335,
    SEN_TYPE_SC200AI,
    SEN_TYPE_SC401AI,
    SEN_TYPE_SC2239P,
    SEN_TYPE_SC2336,
    SEN_TYPE_SC3336,
    SEN_TYPE_SC4336,
    SEN_TYPE_SC1345,
    SEN_TYPE_SC530AI,
    SEN_TYPE_SC2331,
    SEN_TYPE_SC8238,
    SEN_TYPE_SC1336,
    SEN_TYPE_SC450AI,
    SEN_TYPE_SC2336P,
    SEN_TYPE_SC4336P,
    SEN_TYPE_SC5338,
    SEN_TYPE_SC430AI,

    /*GC系列*/
    SEN_TYPE_GC2063 = 5000,
    SEN_TYPE_GC2053,
    SEN_TYPE_GC2093,
    SEN_TYPE_GC4023,
    SEN_TYPE_GC4653,
    SEN_TYPE_GC2083,

    /*SP系列*/
    SEN_TYPE_SP2305 = 6000,
    SEN_TYPE_SP2306,
    SEN_TYPE_SP2329,
    SEN_TYPE_SP230A,

    /*MIS系列*/
    SEN_TYPE_MIS2006 = 7000,
    SEN_TYPE_MIS2008,
    SEN_TYPE_MIS4001,

    /*JX系列*/
    SEN_TYPE_JXF23 = 8000,
    SEN_TYPE_JXF35,
    SEN_TYPE_JXF37,
    SEN_TYPE_JXK06,
    SEN_TYPE_JXQ03P,

    /*其他系列*/
    SEN_TYPE_AR0230 = 1000000,
    SEN_TYPE_C2398,
    SEN_TYPE_BG0808,
    SEN_TYPE_PS6210K,
    SEN_TYPE_C2399,
    SEN_TYPE_CV2003,
    SEN_TYPE_BUTT,
} SEN_TYPE_E;

/*sensor工作模式*/
typedef enum tagSenWorkMode {
    SEN_WORK_MODE_MASTER = 0, /*sensor工作在主模式*/
    SEN_WORK_MODE_SLAVE  = 1, /*sensor工作在从模式*/
    SEN_WORK_MODE_BUTT,
} SEN_WORK_MODE_E;

/*sensor宽动态模式*/
typedef enum tagSenHdrMode {
    SEN_HDR_MODE_NONE     = 0, /*线性模式*/
    SEN_HDR_MODE_2_LINE   = 1, /*两帧宽动态行交错模式*/
    SEN_HDR_MODE_2_FRAME  = 2, /*两帧宽动态帧交错模式*/
    SEN_HDR_MODE_3_LINE   = 3, /*三帧宽动态行交错模式*/
    SEN_HDR_MODE_3_FRAME  = 4, /*三帧宽动态帧交错模式*/
    SEN_HDR_MODE_BUILD_IN = 5, /*sensor端合成宽动态*/
    SEN_HDR_MODE_BUTT,
} SEN_HDR_MODE_E;

/*sensor数据传输模式*/
typedef enum tagSenTransMode {
    SEN_TRANS_MODE_MIPI     = 0, /*mipi传输*/
    SEN_TRANS_MODE_PARALLEL = 1, /*并口传输*/
    SEN_TRANS_MODE_LVDS     = 2, /*lvds传输*/
    SEN_TRANS_MODE_BUTT,
} SEN_TRANS_MODE_E;

/*sensor设备属性*/
typedef struct tagSenDevAttr {
    SEN_TYPE_E enSenType;    /* sesnor类型编号 */
    NI_U32     u32RstGpioId; /* 复位管脚gpio id */
    NI_U32     u32IicDevId;  /* iic设备号 */
    NI_U32     u32DevAddr;   /* 器件地址 */
} SEN_DEV_ATTR_S;

/*sensor 输出制式*/
typedef struct tagSenFmtAttr {
    NI_U32 u32WidthValid;  /* 视频帧的有效宽度 */
    NI_U32 u32HeightValid; /* 视频帧的有效高度 */
    NI_U32 u32Fps;         /* 视频帧率 * 1000 */
} SEN_FMT_ATTR_S;

/*sensor输出的时序宽高*/
typedef struct tagSenWinSize {
    NI_U32 u32WidthAll;  /* 视频帧的总宽度, VTS寄存器值 */
    NI_U32 u32HeightAll; /* 视频帧的总高度, HTS寄存器值 */
} SEN_WIN_SIZE_S;

/*sensor多帧曝光模式*/
typedef enum tagShtMode {
    SEN_SHT_MODE_SUM  = 0, /*各曝光帧曝光时间是公用的，各帧之间曝光时间之和不能大于最大曝光时间*/
    SEN_SHT_MODE_DIFF = 1, /*各曝光帧曝光时间是独立的，各帧之间曝光时间之和不能各自最大曝光时间*/
    SEN_SHT_MODE_BUTT,
} SEN_SHT_MODE_E;

/*sensor快门曝光能力*/
typedef struct tagSenShtInfo {
    SEN_SHT_MODE_E enShtMode;                             /* 快门模式 */
    NI_U32         u32ExpWidth;                           /* 视频帧的曝光宽度, 不同曝光帧之间是一致的 */
    NI_U32         u32FrameNum;                           /* 曝光帧数*/
    NI_U32         au32ExpHeight[SEN_DOW_MAX_FRAME_NUM];  /* 视频帧的最大曝光高度, 涉及到多帧宽动态，当模式为多帧宽动态时，列出各帧的最大曝光行数 */
    NI_U32         au32MinExpLine[SEN_DOW_MAX_FRAME_NUM]; /* 视频帧的最小曝光宽度, 涉及到多帧宽动态，当模式为多帧宽动态时，列出各帧的最小曝光行数 */
    NI_U32         u32ExpLinePre500Ms;                    /* 500ms对应曝光行数 */
    NI_U32         u32ExpLinePre1000Ms;                   /* 1000ms对应曝光行数 */
} SEN_SHT_INFO_S;

/*sensor增益能力*/
typedef struct tagSenGainInfo {
    NI_U32 u32MinGain; /* 最小增益 DB*1024 */
    NI_U32 u32MaxGain; /* 最大增益 DB*1024 */
    NI_U32 u32GainAcc; /* 增益表精度 DB*1024 */
} SEN_GAIN_INFO_S;

/*sensor曝光参数限制， 描述sensor的曝光能力*/
typedef struct tagSenExpInfo {
    SEN_SHT_INFO_S  stShtInfo;  /* 快门信息 */
    SEN_GAIN_INFO_S stGainInfo; /* 增益信息 */
} SEN_EXP_INFO_S;

/*sensor数据传输信息*/
typedef struct tagSenTransInfo {
    SEN_TRANS_MODE_E enTransMode;  /*传输模式*/
    NI_U32           u32DataWidth; /*数据位宽*/
    NI_U32           u32BitRate;   /*数据率，单位为M*/
    NI_U32           u32LaneNum;   /*lane数量，并口可以不关注*/
} SEN_TRANS_INFO_S;

/*sensor时钟信息*/
typedef struct tagSenClkInfo {
    NI_U32 u32PixClock; /* 视频帧的像素时钟 单位为HZ, MIPI模式下为Sensor内部的像素时钟(fps * height_clk * width_clk) */
    NI_U32 u32SrcClock; /* 提供给Sensor的源时钟，单位为HZ */
} SEN_CLK_INFO_S;

/*sensor视频格式匹配关键字，用来切换制式*/
typedef struct tagSenFmtMatchkey {
    SEN_WORK_MODE_E  enWorkMode;  /* 工作模式 */
    SEN_TRANS_MODE_E enTransMode; /* 传输模式 */
    SEN_HDR_MODE_E   enHdrMode;   /* 宽动态模式 */
    NI_U32           u32LaneNum;  /* lane数量，并口可以不关注 */
    NI_U32           u32Width;    /* 视频帧的有效宽度 */
    NI_U32           u32Height;   /* 视频帧的有效高度 */
    NI_U32           u32Fps;      /* 帧率 * 1000 */
    NI_U32           u32CfgId;    /* 配置ID, 如果同一制式下有多种不同的配置的话, 则用此字段区分, 具体配置内容见sensor配置列表文档 */
} SEN_FMT_MATCH_KEY_S;

/*快门值 */
typedef struct tagSenShtValue {
    NI_U32 u32FrameNum;                    /*曝光帧数，线性为0，宽动态为1*/
    NI_U32 au32Sht[SEN_DOW_MAX_FRAME_NUM]; /*各曝光帧的快门值，单位为曝光行， 从上往下为曝光依次为高曝光到低曝光的帧*/
} SEN_SHT_VALUE_S;

/*增益值*/
typedef struct tagSenGainValue {
    NI_U32 u32Gain; /*各曝光帧的增益值一致, db*1000*/
} SEN_GAIN_VALUE_S;

typedef struct tagSenExpValue {
    SEN_SHT_VALUE_S  stShtValue;  /* 快门值 */
    SEN_GAIN_VALUE_S stGainValue; /* 增益值 */
} SEN_EXP_VALUE_S;

/*当前sensor信息*/
typedef struct tagSenInfo {
    SEN_FMT_MATCH_KEY_S stSenFmtMatchKey; /* 格式信息 */
    SEN_WORK_MODE_E     enWorkMode;       /* sensor 实际工作模式*/
    SEN_HDR_MODE_E      enHdrMode;        /* sensor 实际宽动态模式 */
    SEN_FMT_ATTR_S      stSenFmtAttr;     /* sensor 实际格式信息 */
    SEN_WIN_SIZE_S      stSenWinSize;     /* sensor 时序宽高 */
    SEN_EXP_INFO_S      stSenExpInfo;     /* sensor 在本配置下曝光信息 */
    SEN_TRANS_INFO_S    stSenTransInfo;   /* sensor 在本配置下数据传输信息 */
    SEN_CLK_INFO_S      stSenClk;         /* sensor 在本配置下时钟信息 */
    // SEN_EXP_VALUE_S         stInitExp;          /* sensor 在本配置下的初始曝光信息 */
} SEN_INFO_S;

typedef struct tagSenReg {
    NI_U32 u32RegAddr;   /* 寄存器地址 */
    NI_U32 u32RegVal;    /* 配置值 */
    NI_U32 u32DelayTime; /* 配置这个寄存器需要的延时帧数 */
} SEN_REG_S;

/* sensor控制的命令字 根据不同sensor可选 */
typedef enum tagSenIocCmd {
    SEN_CMD_MATCH_ONLY    = 0, /* 仅匹配制式, 但不下发寄存器 */
    SEN_CMD_BAYER_PATTERN = 1, /* 调整bayer格式 */
    SEN_CMD_SET_PWDN_ATTR = 2, /* 设置控制power down的参数 */
    SEN_CMD_PWDN_CTRL     = 3, /* 设置控制power down使能 */
    SEN_CMD_BUTT,
} SEN_IOC_CMD_E;

/* sensor 输出的 bayer */
typedef enum tagSenBayerPattern {
    SEN_BAYER_PATTERN_RGGB, /* sensor 输出的 bayer 格式为 rggb  */
    SEN_BAYER_PATTERN_BGGR, /* sensor 输出的 bayer 格式为 bggr  */
    SEN_BAYER_PATTERN_GRBG, /* sensor 输出的 bayer 格式为 grbg  */
    SEN_BAYER_PATTERN_GBRG, /* sensor 输出的 bayer 格式为 gbrg  */
    SEN_BAYER_PATTERN_BUTT, /* */
} SEN_BAYER_PATTERN_E;

typedef enum tagSenPwdnMode {
    SEN_PWDN_MODE_SOFT = 0, /* pwdn由sensor寄存器控制 */
    SEN_PWDN_MODE_GPIO = 1, /* pwdn由GPIO控制 */
    SEN_PWDN_MODE_BUTT,
} SEN_PWDN_MODE_E;

typedef struct tagSenPwdnAttr {
    NI_U32          u32PwdnIo;  /* pwdn io */
    NI_BOOL         bPwdnLevel; /* pwdn 时的io电平 range:[0, 1] default:0 */
    SEN_PWDN_MODE_E enPwdnMode; /* pwdn 控制方式, range:[0, 1] default:0 */
} SEN_PWDN_ATTR_S;

typedef union tagSenExtCmdParam {
    NI_BOOL             bMatchOnly;     /* UNION: enExtCmd=SEN_CMD_MATCH_ONLY| 时使用 */
    SEN_BAYER_PATTERN_E enBayerPattern; /* UNION: enExtCmd=SEN_CMD_BAYER_PATTERN| 时使用 */
    SEN_PWDN_ATTR_S     stPwdnAttr;     /* UNION: enExtCmd=SEN_CMD_SET_PWDN_ATTR| 时使用 */
    NI_BOOL             bPwdnEnable;    /* UNION: enExtCmd=SEN_CMD_PWDN_CTRL| 时使用 */
} SEN_EXT_CMD_PARAM_U;

typedef struct tagSenExtCmd {
    SEN_IOC_CMD_E       enExtCmd;   /* 命令字 */
    SEN_EXT_CMD_PARAM_U unCmdParam; /* 命令字参数 */
} SEN_EXT_CMD_S;

/*sensor曝光寄存器写入方式*/
typedef enum tagSenRegWriteMode {
    SEN_REG_WRITE_SYNC          = 0, /* 帧同步模式 */
    SEN_REG_WRITE_DIRECT        = 1, /* 直接下发 */
    SEN_REG_WRITE_KERNEL_DIRECT = 2, /* 内核态直接下发 */
    SEN_REG_WRITE_BUTT,
} SEN_REG_WRITE_MODE_E;

typedef enum tagSenFpsMode {
    SEN_FPS_MODE_NONE = 0, /* 固定帧率 */
    SEN_FPS_MODE_VTS  = 1, /* 通过VTS改变帧率 */
    SEN_FPS_MODE_HTS  = 2, /* 通过HTS改变帧率 */
    SEN_FPS_MODE_BUTT,
} SEN_FPS_MODE_E;

#define SENSOR_PWDN_REG_MAX_NUM (4)

typedef struct tagSenPwdnRegList {
    NI_U8  u8PwdnRegNum;                         /* power down 操作寄存器数量 */
    NI_U16 au16RegAddr[SENSOR_PWDN_REG_MAX_NUM]; /* 控制pwdn 寄存器地址 */
    NI_U16 au16Data[SENSOR_PWDN_REG_MAX_NUM];    /* 控制pwdn 值 */
} SEN_PWDN_REG_LIST_S;

typedef struct tagSenPwdnCtrlReg {
    SEN_PWDN_REG_LIST_S stPwdnRegList; /* power down 操作的寄存器 */
    SEN_PWDN_REG_LIST_S stPwupRegList; /* power up 操作的寄存器 */
} SEN_PWDN_CTRL_REG_S;

typedef struct tagSenVersion {
    NI_CHAR aszVersion[128];   /* 版本号 */
    NI_CHAR aszCodeVer[128];   /* code版本  */
    NI_CHAR aszCommitTime[64]; /* 提交时间  */
    NI_CHAR aszAuther[64];     /* 作者  */
} SEN_VERSION_S;

typedef NI_S32 (*pfn_sen_cbReportFps)(NI_F32);

/*sensor 初始化, 只涉及软件功能， 不进行内存等需要释放的资源的申请， 并不操作sensor硬件 */
NI_S32 NI_SEN_Init(SENSOR_ID SenId);
NI_S32 NI_SEN_DeInit(SENSOR_ID SenId);

/*sensor设备属性配置，用来适配不同sensor，不同复位管脚，不同的iic设备地址，器件地址*/
NI_S32 NI_SEN_SetSenIdAttr(SENSOR_ID SenId, SEN_DEV_ATTR_S *pstSenDevAttr);
NI_S32 NI_SEN_GetSenIdAttr(SENSOR_ID SenId, SEN_DEV_ATTR_S *pstSenDevAttr);

/* sensor复位 */
NI_S32 NI_SEN_Reset(SENSOR_ID SenId);

/*sensor 寄存器操作函数, 用来操作对应sensor的寄存器*/
NI_S32 NI_SEN_WriteRegister(SENSOR_ID SenId, NI_U32 u32Addr, NI_U32 u32Value);
NI_S32 NI_SEN_ReadRegister(SENSOR_ID SenId, NI_U32 u32Addr, NI_U32 *pu32Value);
NI_S32 NI_SEN_WriteRegisterGrup(SENSOR_ID SenId, SEN_REG_S *pstRegGrup, NI_U32 u32RegNum);

/*sensor 格式切换函数， 切换视频格式以及宽动态*/
NI_S32 NI_SEN_SetFormat(SENSOR_ID SenId, SEN_FMT_MATCH_KEY_S *pstFmtMatchKey);

/*快门设置函数，分不同曝光帧进行设置*/
NI_S32 NI_SEN_SetSht(SENSOR_ID SenId, SEN_SHT_VALUE_S *pstSht);
NI_S32 NI_SEN_GetSht(SENSOR_ID SenId, SEN_SHT_VALUE_S *pstSht);
NI_S32 NI_SEN_SetShtWithMode(SENSOR_ID SenId, SEN_SHT_VALUE_S *pstSht, SEN_REG_WRITE_MODE_E enWriteMode);

/*增益设置函数，分不同曝光帧进行设置*/
NI_S32 NI_SEN_SetGain(SENSOR_ID SenId, SEN_GAIN_VALUE_S *pstGain);
NI_S32 NI_SEN_GetGain(SENSOR_ID SenId, SEN_GAIN_VALUE_S *pstGain);
NI_S32 NI_SEN_SetGainWithMode(SENSOR_ID SenId, SEN_GAIN_VALUE_S *pstGain, SEN_REG_WRITE_MODE_E enWriteMode);

/*曝光量设置函数，分不同曝光帧进行设置*/
NI_S32 NI_SEN_SetExp(SENSOR_ID SenId, SEN_EXP_VALUE_S *pstExp);
NI_S32 NI_SEN_GetExp(SENSOR_ID SenId, SEN_EXP_VALUE_S *pstExp);
NI_S32 NI_SEN_SetExpWithMode(SENSOR_ID SenId, SEN_EXP_VALUE_S *pstExp, SEN_REG_WRITE_MODE_E enWriteMode);

/*sensor  设置完制式后信息获取函数， 用于上层应用直接获取sensor相关信息以及能力，不需要去看sensor代码的实现才知道sensor的相关信息*/
NI_S32 NI_SEN_GetCurInfo(SENSOR_ID SenId, SEN_INFO_S *pstSenInfo);

/*sensor  信息查询函数， 用于上层应用直接获取对应制式下sensor相关信息以及能力，不需要去看sensor代码的实现才知道sensor的相关信息*/
NI_S32 NI_SEN_GetFmtInfo(SENSOR_ID SenId, SEN_FMT_MATCH_KEY_S *pstFmtMatchKey, SEN_INFO_S *pstSenInfo);

/* sensor 提供的通用额外功能，翻转，镜像等*/
NI_S32 NI_SEN_SetFlip(SENSOR_ID SenId, NI_BOOL bFlip);
NI_S32 NI_SEN_SetMirror(SENSOR_ID SenId, NI_BOOL bMirror);

/* sensor 在帧中断处理 */
NI_S32 NI_SEN_SetFrameIrq(SENSOR_ID SenId, NI_VOID *pData, NI_U32 u32DataLen);

/* sensor 设置帧长触发帧率调整*/
NI_S32 NI_SEN_SetVts(SENSOR_ID SenId, NI_U32 u32Vts);

/* sensor 设置帧率 */
NI_S32 NI_SEN_SetFps(SENSOR_ID SenId, NI_U32 u32Fps, SEN_FPS_MODE_E enMode);

/* sensor 设置帧率回调函数 */
NI_S32 NI_SEN_SetCallBackReportFps(SENSOR_ID SenId, pfn_sen_cbReportFps pfunc);

/* 扩展命令 */
NI_S32 NI_SEN_ExtCmd(SENSOR_ID SenId, NI_VOID *pvCmd);

/* 设置sensor进入pwdn模式 */
// NI_S32 NI_SEN_SetPwdn(SENSOR_ID SenId, SEN_PWDN_MODE_E enNode, NI_BOOL bPwdn);

/* sensor 从主核同步时配置Exp */
NI_S32 NI_SEN_SetSenData(SENSOR_ID SenId, NI_U32 *pu32Addr);

/* sensor 获取版本信息 */
NI_S32 NI_SEN_GetVersion(SEN_VERSION_S *pstVersion);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
