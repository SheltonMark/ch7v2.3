/*******************************************************************************
 * Copyright (C) 2020-2025 ZheJiang Chipup Technology CO.,LTD.
 *
 * 文件名 : ni_comm_aisp.h
 *
 * 作  者 : Chipup Software Team
 * 版  本 : V1.0.0
 * 日  期 : 2022-02-23 Create
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
 * 日    期: 2022-02-23
 * 修订版本:
 * 作    者:
 * 修订备注:
 *******************************************************************************/

#ifndef _NI_COMM_AISP_H_
#define _NI_COMM_AISP_H_
    
/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */

#include "ni_type.h"
#include "ni_common.h"
#include "ni_aisp_define.h"

#ifdef __cplusplus
extern "C" {
#endif

#define AISP_MAX_LIGHT_NUM            (4)

/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */
typedef struct tagAispRegion
{
    NI_U16          u16StartX;      /* 起始点比例，U1.10 */
    NI_U16          u16StartY;      /* 起始点比例，U1.10 */
    NI_U16          u16EndX;        /* 结束点比例，U1.10 */
    NI_U16          u16EndY;        /* 结束点比例，U1.10 */
} AISP_REGION_S;

typedef struct tagAispAppColor
{
    NI_U8   u8Brt;            /* 亮度 0~100 */    
    NI_U8   u8Ctr;            /* 对比度 0~100 */  
    NI_U8   u8Sat;            /* 饱和度 0~100 */  
    NI_U8   u8Hue;            /* 色调 0~100 */  
} AISP_APP_COLOR_S;

typedef struct tagAispAppGamma
{
    NI_U8                       u8Level;            /* 0~100 */
} AISP_APP_GAMMA_S;

typedef enum tagAispBackLightMode
{
    AISP_BACKLIGHT_MODE_NONE = 0,       /* 关闭背光 */
    AISP_BACKLIGHT_MODE_BLC,            /* 背光补偿 */
    AISP_BACKLIGHT_MODE_WDR,            /* 宽动态 */
    AISP_BACKLIGHT_MODE_AHL,            /* 强光抑制 */
    AISP_BACKLIGHT_MODE_SSA,            /* 场景自适应 */
    AISP_BACKLIGHT_MODE_BUTT
}AISP_BACKLIGHT_MODE_E;

typedef struct tagAispAppBackLight
{
    AISP_BACKLIGHT_MODE_E       enBackLightMode;    /* 背光模式 */
    NI_U8                       u8Level;            /* 0~100 */
}AISP_APP_BACKLIGHT_S;

typedef struct tagAispVideoFmtInfo
{
    NI_U32  u32ImgWidth;        /*图像宽度*/
    NI_U32  u32ImgHeight;       /*图像高度*/
    NI_U32  u32Fps;             /*图像帧率*1000*/
} AISP_VIDEO_FMT_INFO_S;

typedef enum tagAispDnMode
{
    AISP_DN_MODE_AUTO = 0,      /* 自动白天晚上切换 */
    AISP_DN_MODE_DAY,           /* 强制白天 */
    AISP_DN_MODE_NIGHT,         /* 强制晚上 */
    AISP_DN_MODE_BUTT,
} AISP_DN_MODE_E;

typedef struct tagAispDnAttr
{
    NI_U16                  u16D2nValue;    /* 日夜切换灵敏度 */
    NI_U16                  u16N2dValue;    /* 夜日切换灵敏度 */
    NI_U16                  u16Delay;       /* 切换延时时间 */
    NI_U32                  u32D2nGainThr;  /* 日夜切换增益阈值 */
    NI_U32                  u32N2dGainThr;  /* 夜日切换增益阈值 */
} AISP_DN_ATTR_S;

typedef struct tagAispAppDayNight
{
    AISP_DN_MODE_E          enDayNight;
    AISP_DN_ATTR_S          stDnAttr;        /* 自动切换时的参数 */
} AISP_APP_DAY_NIGHT_S;

typedef enum tagAispLightMode
{
    AISP_LIGHT_MODE_AUTO        = 0,            /* 自动补光 */
    AISP_LIGHT_MODE_CLOSE       = 1,            /* 关闭补光 */
    AISP_LIGHT_RATE_PRIO        = 2,            /* 倍率优先*/
    AISP_LIGHT_MODE_MANUAL      = 3,            /* 手动补光 */
    AISP_LIGHT_MODE_BUTT,
}AISP_LIGHT_MODE_E;

typedef struct tagAispAppLight
{
    AISP_LIGHT_MODE_E   enLightMode;           /* 补光模式 */
    NI_U8               u8Level;               /* 0 ~ 100 manual模式时有效 */
} AISP_APP_LIGHT_S;

typedef struct tagAispNrAttr
{
    NI_BOOL             bEnable;            /* 使能开关 */
    NI_U8               u8Level;            /* 0~100 */
}AISP_NR_ATTR_S;

typedef struct tagAispAppNr
{
    AISP_NR_ATTR_S        stNry2d;
    AISP_NR_ATTR_S        stNry3d;
}AISP_APP_NR_S;

typedef struct tagAispAppEe
{
    NI_BOOL             bEnable;            /* 使能开关 */
    NI_U8               u8Level;            /* 0~100 */
}AISP_APP_EE_S;

typedef enum tagAispExpMode
{
    AISP_EXP_MODE_AUTO           = 0,             /* 自动曝光 */
    AISP_EXP_MODE_SHUTTER_PRIO   = 1,             /* 快门优先 */
    AISP_EXP_MODE_GAIN_PRIO      = 2,             /* 增益优先 */
    AISP_EXP_MODE_IRIS_PRIO      = 3,             /* 光圈优先 */
    AISP_EXP_MODE_MANUAL         = 4,             /* 手动曝光 */
    AISP_EXP_MODE_ANTI_FLICKER   = 5,             /* 防闪烁曝光 */
    AISP_EXP_MODE_BUTT,
}AISP_EXP_MODE_E;

typedef struct tagAispExpRegion
{
    NI_BOOL             bRegionEn;
    AISP_REGION_S       stRoiParam;
}AISP_EXP_REGION_S;

typedef struct tagAispExpRanges
{
    NI_U32      u32GainMin;   /* 0 -100 */
    NI_U32      u32GainMax;   /* 0 -100 */
    NI_U32      u32ShutterMin;  /* us */
    NI_U32      u32ShutterMax;  /* us */
    NI_U32      u32ApertMin;    /* 0 - 100 */
    NI_U32      u32ApertMax;    /* 0 - 100 */
}AISP_EXP_RANGE_S;

typedef struct tagAispAntiFlicker
{
    NI_BOOL         bAutoSht;                    /* 自动模式使能，关闭自动模式时，依据stExpRange调节快门时间*/
    NI_U32          u32AntiFlikerFreq;           /* 防闪烁频率 50hz,60hz*/
}AISP_ANTI_FLICKER_S;

typedef struct tagAispAppExp
{
    NI_U32                       u32Ev;             /* 曝光等级 0 - 100 */
    AISP_EXP_MODE_E              enExpMode;         /* 曝光调节模式 */
    AISP_EXP_REGION_S            stRoiExp;          /* 区域曝光参数 */
    AISP_EXP_RANGE_S             stExpRange;        /* 曝光参数范围，自动模式下无效 */
    AISP_ANTI_FLICKER_S          stAntiFlick;       /* 防闪烁参数 */
}AISP_APP_EXP_S;

typedef enum tagAispWbScene
{
    AISP_WB_SCENE_NORMAL = 0,            /* 自动白平衡 */
    AISP_WB_SCENE_SUNNY,                 /* 自然光 */
    AISP_WB_SCENE_LAMP,                  /* 路灯 */
    AISP_WB_SCENE_OUTDOOR,               /* 室外 */
    AISP_WB_SCENE_INDOOR,                /* 室内 */
    AISP_WB_SCENE_BUTT,
}AISP_WB_SCENE_E;


typedef struct tagAispWbRegion
{
    NI_BOOL             bRegionEn;
    AISP_REGION_S       stRegion;
}AISP_WB_REGION_S;


typedef struct tagAispWbUserGain
{
    NI_BOOL     bEnable;
    NI_U32      u32Rgain;
    NI_U32      u32Bgain;
}AISP_WB_USR_GAIN_S;


typedef struct tagAispAppWb
{
    AISP_WB_SCENE_E         enSceneInfo;         /* 场景信息 */
    AISP_WB_REGION_S        stRegionWb;          /* 区域白平衡 */
    AISP_WB_USR_GAIN_S      stUsrDefGain;        /* 用于自定义增益 */
}AISP_APP_WB_S;


typedef struct tagAispSenExp
{
    NI_U32      u32FrameNum;
    NI_U32      au32Sht[AISP_DOW_MAX_FRAME_NUM];
    NI_U32      u32Gain;
} AISP_SEN_EXP_S;

typedef struct tagAispSenInfo
{
    NI_U32  u32MaxGain;                                 /*最大sensor gain DB U10.10*/
    NI_U32  u32GainAcc;                                 /*增益表精度 DB 最大sensor gain U10.10*/
    NI_U32  u32LinePer500ms;                            /* 500ms对应曝光行数 */
    NI_U32  u32ExpLinePer1000Ms;                        /* 1000ms对应曝光行数 */
    NI_U32  au32MaxExpRatio[AISP_DOW_MAX_FRAME_NUM];   
    NI_U32  au32MinExpRatio[AISP_DOW_MAX_FRAME_NUM];
}AISP_SEN_INFO_S;

typedef struct tagAispLightResult
{
    NI_U32 au32LightLevel[AISP_MAX_LIGHT_NUM];
}AISP_LIGHT_RESULT_S;

typedef struct tagAispRegisterFunc
{
    NI_S32 (*pfn_sen_get_info)(ISP_DEV IspDev, AISP_SEN_INFO_S *pstSenInfo);
    NI_S32 (*pfn_sen_set_exp)(ISP_DEV IspDev, AISP_SEN_EXP_S *pstSenExp);
    NI_S32 (*pfn_sen_get_exp)(ISP_DEV IspDev, AISP_SEN_EXP_S *pstSenExp);
    NI_S32 (*pfn_sen_set_ir_cut)(ISP_DEV IspDev, NI_U32 u32IrCut);
    NI_S32 (*pfn_sen_set_led_light)(ISP_DEV IspDev, AISP_LIGHT_RESULT_S *pstLedLight);
    NI_S32 (*pfn_sen_set_iris)(ISP_DEV IspDev, NI_U32 u32Iris);
    NI_S32 (*pfn_sen_set_ir_light)(ISP_DEV IspDev, AISP_LIGHT_RESULT_S *pstIrLight);
    NI_S32 (*pfn_sen_get_photosense)(ISP_DEV IspDev, NI_U32 *pu32PsValue);
} AISP_REGISTER_FUNC_S;

typedef struct tagAispCfgFileInfo
{
    NI_VOID                 *pCfgFile;                 /*配置地址*/
    NI_U32                  u32PhysAddr;               /*配置文件存放物理地址*/
    NI_U32                  u32CfgLen;                 /*配置长度，单位为字节*/
} AISP_CFG_FILE_INFO_S;

typedef enum tagAispCombMod
{
    AISP_MODE_SINGLE_CHN               = 0,            /* 单通道模式 */
    AISP_MODE_TWO_CHN_COMB             = 1,            /* 两通道融合 */
    AISP_MODE_SINGLE_CHN_DUPLICATE     = 2,            /* 单通道复制 */
    
    AISP_MODE_BUTT,
}AISP_COMB_MODE_E;

typedef struct tagAispInitParam
{
    AISP_COMB_MODE_E        enCombMode;         /*多通道组合模式*/
    AISP_VIDEO_FMT_INFO_S   stVideoFmt;         /*图像格式*/
    AISP_CFG_FILE_INFO_S    stCfgFile;          /*配置信息*/
    AISP_REGISTER_FUNC_S    stRegisterFunc;     /*向aisp库注册的外部函数*/
} AISP_INIT_PARAM_S;

typedef enum tagAispImgStyle
{
    AISP_IMG_STYLE_SOFT_COLOR        = 0,     /* 柔和 */
    AISP_IMG_STYLE_STANDAR_COLOR     = 1,     /* 标准 */
    AISP_IMG_STYLE_BRIGHT_COLOR      = 2,     /* 艳丽 */
    AISP_IMG_STYLE_BUTT,
}AISP_IMG_STYLE_E;

typedef struct tagAispVersion
{
    NI_CHAR             aszMdkVersion[AISP_VERSION_NAME_LEN];
    NI_CHAR             aszCfgVersion[AISP_VERSION_NAME_LEN];
} AISP_VERSION_S;

typedef struct tagAispAeResult
{
    NI_S32      s32ShtTime;               /* 快门时间 单位us */
    NI_S32      s32SysGain;               /* 增益, 单位 DB*1000 */
    NI_S32      s32ApertValue;            /* 光圈值 */

    NI_U32      u32AverY;                 /*平均亮度*/
    NI_U32      u32EnvLum;                /*环境亮度*/

    NI_U32      u32EvTarget;              /* 目标亮度 */
    NI_U32      u32EvCur;                 /* 当前亮度亮度 */
} AISP_AE_RESULT_S;


typedef struct tagAispAppCfg
{
    NI_BOOL                 bUseManualCfg;      /* 自定义配置使能 */
    AISP_CFG_FILE_INFO_S    stCfgFile;          /* 配置信息 */
} AISP_APP_CFG_S;


typedef enum tagAispCfgSelect
{
    AISP_CFG_SELECT_USE_FLASH       = 0,        /* AISP 使用flash的配置 */
    AISP_CFG_SELECT_USE_MEM         = 1,        /* AISP 使用内存中的配置 */
    AISP_CFG_SELECT_BUTT,
}AISP_CFG_SELECT_E;



typedef struct tagAispMAwesInitParam
{
    NI_U32                  u32BufNum;
    NI_U32                  u32AwesPos;   /* 参见 PISP_AWES_POS_E 结构体 */
}AISP_MAWES_INIT_PARAM_S;

typedef struct tagAispMAwesData
{
    NI_U16                 *pu16AwesData;      /* RO, AWES统计的rgby值, 每帧统计数据=u32BlockNum*2*4bytes. */
    NI_U32                 u32AwesPhyAddr;     /* RO, pu16AwesData对应的物理地址 */
}AISP_MAWES_DATA_S;

typedef struct tagAispMAwesStatistics
{
    NI_VOID                 *pvHandle;          /* RO, AWES统计句柄，用于释放时使用，用户不可修改 */
    NI_U32                  u32BlockNum;        /* RO, AWES统计一帧的分块总数 */
    NI_U32                  u32HBlockNum;       /* RO, AWES统计水平方向分块个数 */
    NI_U32                  u32VBlockNum;       /* RO, AWES统计垂直方向分块个数 */

    NI_U32                  u32AwesFrmNum;      /* RO, astAwesData数组有u32AwesFrmNum帧统计数据，帧数与enAwesPos变量及x帧HDR模式对应 */
    NI_U32                  u32AwesPos;         /* RO, 参见 PISP_AWES_POS_E 结构体 */
    AISP_MAWES_DATA_S       astAwesData[3];     /* RO, AWES统计数据，硬件一次固定三帧数据，顺序为短、长、融合帧 线性模式下数据结果保存在astAwesData[0]中*/
}AISP_MAWES_STATISTICS_S;

typedef struct tagAispDnResult
{
    AISP_DN_MODE_E          enDnResult;     /*日夜环境结果*/
    NI_U32                  u32DnEnv;       /*当前日夜环境*/
}AISP_DN_RESULT_S;


typedef struct tagAispAFspCtrl
{
    NI_BOOL             bEnable;        /* 自动切换帧率使能 */
    NI_U32              u32NewFps;      /* 发生自动转换后的帧率 * 1000, u32NewFps 仅支持设置为初始化帧率的一半 */
    NI_U32              u32GainThr;     /* 发生帧率切换时的增益阈值 */
}AISP_AFPS_CTRL_S;

#ifdef __cplusplus
}
#endif


#endif



