/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NI_COMM_AISP_H_
#define _NI_COMM_AISP_H_

#include "ni_type.h"
#include "ni_errno.h"
#include "ni_common.h"
#include "ni_aisp_define.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define NI_ERR_AISP_INVALID_DEVID NI_DEF_ERR(NI_ID_AISP, EN_ERR_LEVEL_ERROR, ERR_INVALID_DEVID)
#define NI_ERR_AISP_UNEXIST       NI_DEF_ERR(NI_ID_AISP, EN_ERR_LEVEL_ERROR, ERR_UNEXIST)
#define NI_ERR_AISP_ILLEGAL_PARAM NI_DEF_ERR(NI_ID_AISP, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)
#define NI_ERR_AISP_NULL_PTR      NI_DEF_ERR(NI_ID_AISP, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)
#define NI_ERR_AISP_NOT_CONFIG    NI_DEF_ERR(NI_ID_AISP, EN_ERR_LEVEL_ERROR, ERR_NOT_CONFIG)
#define NI_ERR_AISP_NOT_SUPPORT   NI_DEF_ERR(NI_ID_AISP, EN_ERR_LEVEL_ERROR, ERR_NOT_SUPPORT)
#define NI_ERR_AISP_NOT_PERM      NI_DEF_ERR(NI_ID_AISP, EN_ERR_LEVEL_ERROR, ERR_NOT_PERM)
#define NI_ERR_AISP_NOMEM         NI_DEF_ERR(NI_ID_AISP, EN_ERR_LEVEL_ERROR, ERR_NOMEM)
#define NI_ERR_AISP_NOT_INIT      NI_DEF_ERR(NI_ID_AISP, EN_ERR_LEVEL_ERROR, ERR_SYS_NOTREADY)
#define NI_ERR_AISP_TIME_OUT      NI_DEF_ERR(NI_ID_AISP, EN_ERR_LEVEL_ERROR, ERR_TIME_OUT)

typedef struct tagAispRegion {
    NI_U16 u16StartX; /* 起始点比例，U1.10 range:[0, 1024] default:0 u16StartX <= u16EndX */
    NI_U16 u16StartY; /* 起始点比例，U1.10 range:[0, 1024] default:0 u16StartY <= u16EndY */
    NI_U16 u16EndX;   /* 结束点比例，U1.10 range:[0, 1024] default:1024 u16StartX <= u16EndX */
    NI_U16 u16EndY;   /* 结束点比例，U1.10 range:[0, 1024] default:1024 u16StartY <= u16EndY */
} AISP_REGION_S;

typedef struct tagAispAppColor {
    NI_U8 u8Brt; /* 亮度   range:[0, 100] default:50 */
    NI_U8 u8Ctr; /* 对比度 range:[0, 100] default:50 */
    NI_U8 u8Sat; /* 饱和度 range:[0, 100] default:50 */
    NI_U8 u8Hue; /* 色调   range:[0, 100] default:50 */
} AISP_APP_COLOR_S;

typedef struct tagAispAppGamma {
    NI_U8 u8Level; /* gamma等级 range:[0, 100] default:50 */
} AISP_APP_GAMMA_S;

typedef enum tagAispBackLightMode {
    AISP_BACKLIGHT_MODE_NONE = 0, /* 关闭背光模式 */
    AISP_BACKLIGHT_MODE_BLC,      /* 背光补偿模式 */
    AISP_BACKLIGHT_MODE_WDR,      /* 宽动态模式 */
    AISP_BACKLIGHT_MODE_AHL,      /* 强光抑制模式 */
    AISP_BACKLIGHT_MODE_SSA,      /* 场景自适应模式(暂不支持)*/
    AISP_BACKLIGHT_MODE_BUTT
} AISP_BACKLIGHT_MODE_E;

typedef struct tagAispAppBackLight {
    AISP_BACKLIGHT_MODE_E enBackLightMode; /* 背光模式 default:AISP_BACKLIGHT_MODE_NONE */
    NI_U8                 u8Level;         /* 背光强度 range:[0, 100] default:50 */
} AISP_APP_BACKLIGHT_S;

typedef enum tagAispVideoType {
    AISP_VIDEO_TYPE_AUTO = 0, /* 悬空态或此态自动配置模式，IPC设置成IPC模式，同轴设备配置为CVI模式。 */
    AISP_VIDEO_TYPE_CVI  = 1,
    AISP_VIDEO_TYPE_TVI  = 2,
    AISP_VIDEO_TYPE_AHD  = 3,
    AISP_VIDEO_TYPE_IPC  = 4,

    AISP_VIDEO_TYPE_BUTT = 0xff,
} AISP_VIDEO_TYPE_E;

typedef struct tagAispVideoFmtInfo {
    NI_U32            u32ImgWidth;  /* 输入图像宽度 range:[0, AISP_MAX_WIDTH] */
    NI_U32            u32ImgHeight; /* 输入图像高度 range:[0, AISP_MAX_HEIGHT] */
    NI_U32            u32Fps;       /* 输入图像帧率*1000 range:[1000, AISP_MAX_FPS], 需要与sensor配置的初始FPS保持一致 */
    AISP_VIDEO_TYPE_E enVideoType;  /* 视频制式,用于区分配置类型 */
} AISP_VIDEO_FMT_INFO_S;

typedef enum tagAispDnMode {
    AISP_DN_MODE_AUTO = 0, /* 自动切换日夜模式 */
    AISP_DN_MODE_DAY,      /* 白天模式 */
    AISP_DN_MODE_NIGHT,    /* 黑夜模式 */
    AISP_DN_MODE_BUTT,
} AISP_DN_MODE_E;

typedef enum tagAispDnMethod {
    AISP_DN_METHOD_PHOTOSENS = 0, /* 光敏方案 */
    AISP_DN_METHOD_SOFT      = 1, /* 纯软件方案 */
    AISP_DN_METHOD_BUTT,
} AISP_DN_METHOD_E;

typedef struct tagAispDnAttr {
    NI_U16 u16D2nValue;   /* 日夜切换灵敏度 range:[0, 1024] default:200 */
    NI_U16 u16N2dValue;   /* 夜日切换灵敏度 range:[0, 1024] default:400 */
    NI_U16 u16Delay;      /* 切换延时时间 range:[1, 65535] default:4000 单位: ms */
    NI_U32 u32D2nGainThr; /* 日夜切换增益阈值 range:[0, 72000] default:36000 */
    NI_U32 u32N2dGainThr; /* 夜日切换增益阈值 range:[0, 72000] default:26000 */
} AISP_DN_ATTR_S;

typedef struct tagAispAppDayNight {
    AISP_DN_MODE_E   enDayNight; /* 日夜模式 default:AISP_DN_MODE_AUTO */
    AISP_DN_METHOD_E enDnMethod; /* 日夜算法方式 default:AISP_DN_METHOD_SOFT */
    AISP_DN_ATTR_S   stDnAttr;   /* 自动切换模式参数 */
} AISP_APP_DAY_NIGHT_S;

typedef enum tagAispLightMode {
    AISP_LIGHT_MODE_AUTO   = 0, /* 自动补光 */
    AISP_LIGHT_MODE_CLOSE  = 1, /* 关闭补光 */
    AISP_LIGHT_RATE_PRIO   = 2, /* 倍率优先*/
    AISP_LIGHT_MODE_MANUAL = 3, /* 手动补光 */
    AISP_LIGHT_MODE_BUTT,
} AISP_LIGHT_MODE_E;

typedef struct tagAispAppLight {
    AISP_LIGHT_MODE_E enLightMode; /* 补光模式 default:AISP_LIGHT_MODE_AUTO */
    NI_U8             u8Level;     /* 手动补光模式下灯光强度 range:[0, 100] default:50 */
} AISP_APP_LIGHT_S;

typedef struct tagAispNrAttr {
    NI_BOOL bEnable; /* 降噪模块使能 default:NI_TRUE */
    NI_U8   u8Level; /* 降噪强度 range:[0, 100] default:50 */
} AISP_NR_ATTR_S;

typedef struct tagAispAppNr {
    AISP_NR_ATTR_S stNry2d; /* 2d降噪参数 */
    AISP_NR_ATTR_S stNry3d; /* 3d降噪参数 */
} AISP_APP_NR_S;

typedef struct tagAispAppEe {
    NI_BOOL bEnable; /* ee模块使能 default:NI_TRUE */
    NI_U8   u8Level; /* ee强度 range:[0, 100] default:50 */
} AISP_APP_EE_S;

typedef struct tagAispAppDefog {
    NI_BOOL bEnable; /* 去雾模块使能 default:NI_FALSE */
    NI_U8   u8Level; /* 去雾强度 range:[0, 100] default:50 */
} AISP_APP_DEFOG_S;

typedef enum tagAispExpMode {
    AISP_EXP_MODE_AUTO         = 0, /* 自动曝光 */
    AISP_EXP_MODE_SHUTTER_PRIO = 1, /* 快门优先 */
    AISP_EXP_MODE_GAIN_PRIO    = 2, /* 增益优先 */
    AISP_EXP_MODE_IRIS_PRIO    = 3, /* 光圈优先(不支持) */
    AISP_EXP_MODE_MANUAL       = 4, /* 手动曝光 */
    AISP_EXP_MODE_ANTI_FLICKER = 5, /* 防闪烁曝光 */
    AISP_EXP_MODE_BUTT,
} AISP_EXP_MODE_E;

typedef struct tagAispExpRegion {
    NI_BOOL       bRegionEn;  /* 区域曝光使能 default:NI_FALSE */
    AISP_REGION_S stRoiParam; /* 区域曝光坐标参数 */
} AISP_EXP_REGION_S;

typedef struct tagAispExpRanges {
    NI_U32 u32GainMin;    /* 非自动模式下增益最小值 range:[0, 100] default:0   u32GainMin <= u32GainMax */
    NI_U32 u32GainMax;    /* 非自动模式下增益最大值 range:[0, 100] default:100 u32GainMin <= u32GainMax */
    NI_U32 u32ShutterMin; /* 非自动模式下曝光时间最小值 单位:us range:[10, 1000000] default:50    u32ShutterMin <= u32ShutterMax */
    NI_U32 u32ShutterMax; /* 非自动模式下曝光时间最大值 单位:us range:[10, 1000000] default:40000 u32ShutterMin <= u32ShutterMax */
    NI_U32 u32ApertMin;   /* 非自动模式下光圈最小值 range:[0, 100] default:0   u32ApertMin <= u32ApertMax */
    NI_U32 u32ApertMax;   /* 非自动模式下光圈最大值 range:[0, 100] default:100 u32ApertMin <= u32ApertMax */
} AISP_EXP_RANGE_S;

typedef struct tagAispAntiFlicker {
    NI_BOOL bAutoSht;          /* 自动模式使能，关闭自动模式时，依据stExpRange调节快门时间 default:NI_FALSE */
    NI_U32  u32AntiFlikerFreq; /* 防闪烁频率 50hz,60hz */
} AISP_ANTI_FLICKER_S;

typedef struct tagAispAppExp {
    NI_U32              u32Ev;       /* 曝光等级 range:[0, 100] default:50 */
    AISP_EXP_MODE_E     enExpMode;   /* 曝光模式选择 default:AISP_EXP_MODE_AUTO */
    AISP_EXP_REGION_S   stRoiExp;    /* 区域曝光参数 */
    AISP_EXP_RANGE_S    stExpRange;  /* 曝光参数范围，自动模式下无效 */
    AISP_ANTI_FLICKER_S stAntiFlick; /* 防闪烁参数 */
} AISP_APP_EXP_S;

typedef enum tagAispWbScene {
    AISP_WB_SCENE_NORMAL = 0, /* 自动白平衡 */
    AISP_WB_SCENE_SUNNY,      /* 晴天 */
    AISP_WB_SCENE_LAMP,       /* 路灯 */
    AISP_WB_SCENE_OUTDOOR,    /* 室外 */
    AISP_WB_SCENE_INDOOR,     /* 室内 */
    AISP_WB_SCENE_BUTT,
} AISP_WB_SCENE_E;

typedef struct tagAispWbRegion {
    NI_BOOL       bRegionEn; /* 区域白平衡使能 default:NI_FALSE */
    AISP_REGION_S stRegion;  /* 区域白平衡坐标参数 */
} AISP_WB_REGION_S;

typedef struct tagAispWbUserGain {
    NI_BOOL bEnable;  /* 手动白平衡使能 default:NI_FALSE */
    NI_U32  u32Rgain; /* 手动白平衡红色增益 range:[0, 100] default:50 */
    NI_U32  u32Bgain; /* 手动白平衡蓝色增益 range:[0, 100] default:50 */
} AISP_WB_USR_GAIN_S;

typedef struct tagAispAppWb {
    AISP_WB_SCENE_E    enSceneInfo;  /* 白平衡场景信息 */
    AISP_WB_REGION_S   stRegionWb;   /* 区域白平衡 */
    AISP_WB_USR_GAIN_S stUsrDefGain; /* 用于手动白平衡自定义增益 */
} AISP_APP_WB_S;

typedef struct tagAispAppFastAeExp {
    NI_BOOL bEnable;     /* 快启模块开关 default:NI_TRUE */
    NI_BOOL bSetEn;      /* 手动设置初始曝光值 default:NI_FALSE */
    NI_U8   u8TargetY;   /* 目标亮度, range:[0, 255] default:85 */
    NI_U32  au32Sht[3];  /* 曝光时间,单位：行, au32Sht[0]不可设 */
    NI_U32  au32Gain[3]; /* 增益,单位：dB*1000, u32Gain[0]不可设 */
} AISP_APP_FAST_AE_EXP_S;

typedef struct tagAispSenExp {
    NI_U32 u32FrameNum;                     /* 帧数,1:线性 2:二合一宽动态 */
    NI_U32 au32Sht[AISP_DOW_MAX_FRAME_NUM]; /* 曝光时间,单位：行 */
    NI_U32 u32Gain;                         /* 增益,单位：dB*1000 */
} AISP_SEN_EXP_S;

typedef struct tagAispSenInfo {
    NI_U32 u32MaxGain;                              /* 最大sensor增益 单位：dB*1000 */
    NI_U32 u32GainAcc;                              /* sensor增益精度 单位：dB*1000 */
    NI_U32 u32LinePer500ms;                         /* 500ms对应曝光行数 单位：行 */
    NI_U32 u32ExpLinePer1000Ms;                     /* 1000ms对应曝光行数 单位：行 */
    NI_U32 au32MaxExpRatio[AISP_DOW_MAX_FRAME_NUM]; /* 宽动态模式最大曝光比 */
    NI_U32 au32MinExpRatio[AISP_DOW_MAX_FRAME_NUM]; /* 宽动态模式最小曝光比 */
    NI_U32 u32StdVts;                               /* 无降帧帧长 单位：行*/
    NI_U32 u32MinSht;                               /* 最小曝光 单位：行*/
} AISP_SEN_INFO_S;

typedef struct tagAispLightResult {
    NI_U32 au32LightLevel[AISP_MAX_LIGHT_NUM]; /* 灯光强度, range:[0, 10000] */
} AISP_LIGHT_RESULT_S;

typedef enum tagAispSetFpsMode {
    AISP_SET_FPS_NONE = 0,
    AISP_SET_FPS_VTS, /* 修改VTS方式 */
    AISP_SET_FPS_HTS, /* 修改HTS方式 */
    AISP_SET_FPS_BUTT,
} AISP_SET_FPS_MODE_E;

typedef struct tagAispRegisterFunc {
    NI_S32(*pfn_sen_get_info)
    (ISP_DEV IspDev, AISP_SEN_INFO_S *pstSenInfo);
    NI_S32(*pfn_sen_set_exp)
    (ISP_DEV IspDev, AISP_SEN_EXP_S *pstSenExp);
    NI_S32(*pfn_sen_get_exp)
    (ISP_DEV IspDev, AISP_SEN_EXP_S *pstSenExp);
    NI_S32(*pfn_sen_set_ir_cut)
    (ISP_DEV IspDev, NI_U32 u32IrCut);
    NI_S32(*pfn_sen_set_led_light)
    (ISP_DEV IspDev, AISP_LIGHT_RESULT_S *pstLedLight);
    NI_S32(*pfn_sen_set_iris)
    (ISP_DEV IspDev, NI_U32 u32Iris);
    NI_S32(*pfn_sen_set_ir_light)
    (ISP_DEV IspDev, AISP_LIGHT_RESULT_S *pstIrLight);
    NI_S32(*pfn_sen_get_photosense)
    (ISP_DEV IspDev, NI_U32 *pu32PsValue);

    NI_S32(*pfn_sen_set_vts)
    (NI_S32 IspDev, NI_U32 u32Vts);
    NI_S32(*pfn_sen_set_fps)
    (NI_S32 IspDev, NI_U32 u32Fps, AISP_SET_FPS_MODE_E enMode);
    NI_S32 s32Reserve1;
    NI_S32 s32Reserve2;
} AISP_REGISTER_FUNC_S;

typedef struct tagAispCfgFileInfo {
    NI_VOID *pCfgFile;   /*配置地址*/
    NI_U32   u32PhyAddr; /*配置文件存放物理地址*/
    NI_U32   u32CfgLen;  /*配置长度，单位: 字节*/
} AISP_CFG_FILE_INFO_S;

typedef enum tagAispCombMod {
    AISP_MODE_SINGLE_CHN           = 0, /* 单通道模式 */
    AISP_MODE_TWO_CHN_COMB         = 1, /* 两通道融合 */
    AISP_MODE_SINGLE_CHN_DUPLICATE = 2, /* 单通道复制 */
    AISP_MODE_BUTT,
} AISP_COMB_MODE_E;

typedef struct tagAispInitParam {
    AISP_COMB_MODE_E      enCombMode;     /* 多通道组合模式,仅支持单通道模式 */
    AISP_VIDEO_FMT_INFO_S stVideoFmt;     /* 输入图像格式 */
    AISP_CFG_FILE_INFO_S  stCfgFile;      /* 配置信息 */
    AISP_REGISTER_FUNC_S  stRegisterFunc; /* 向aisp库注册的外部函数,用不到的函数需置null,否则程序会运行出错 */
} AISP_INIT_PARAM_S;

typedef enum tagAispImgStyle {
    AISP_IMG_STYLE_SOFT_COLOR    = 0, /* 柔和 */
    AISP_IMG_STYLE_STANDAR_COLOR = 1, /* 标准 */
    AISP_IMG_STYLE_BRIGHT_COLOR  = 2, /* 艳丽 */
    AISP_IMG_STYLE_BUTT,
} AISP_IMG_STYLE_E;

typedef struct tagAispVersion {
    NI_CHAR aszMdkVersion[AISP_VERSION_NAME_LEN]; /* MDK版本信息 */
    NI_CHAR aszCfgVersion[AISP_VERSION_NAME_LEN]; /* CFG版本信息 */
} AISP_VERSION_S;

typedef struct tagAispAeResult {
    NI_S32 s32ShtTime;    /* 快门时间 单位: us */
    NI_S32 s32SysGain;    /* 增益 单位: dB*1000 */
    NI_S32 s32ApertValue; /* 光圈值 range:[0, 100] */

    NI_U32 u32AverY;  /* 图像平均亮度 range:[0, 1023]*/
    NI_U32 u32EnvLum; /* 当前环境亮度信息 */

    NI_U32 u32EvTarget; /* AE算法的目标亮度 range:[0, 255]*/
    NI_U32 u32EvCur;    /* AE测光计算出的图像亮度 range:[0, 1023]*/
} AISP_AE_RESULT_S;

typedef struct tagAispDnResult {
    AISP_DN_MODE_E enDnResult; /* 日夜结果信息, 用户设置为Day模式, 该值为1,
                                   用户设置为夜晚模式, 该值为2，
                                   用户设置为Auto模式, 该值为算法计算的日夜信息, 与u32DnEnv相同 */
    NI_U32 u32DnEnv;           /* 算法计算的当前的环境的日夜信息, 1:表示白天, 2表示晚上 */
} AISP_DN_RESULT_S;

typedef enum tagAispAutoFpsMode {
    AISP_AUTO_FPS_BY_ROUTE = 0, /* 路径降帧模式 */
    AISP_AUTO_FPS_BY_STEPLESS,  /* 无级整数降帧模式 */
    AISP_AUTO_FPS_BY_MULTIPLES, /* 整数倍降帧模式 */
    AISP_AUTO_FPS_BY_ROUTE_HTS, /* hts方式路径降帧模式, 非必要不使用 */
    AISP_AUTO_FPS_BUTT,
} AISP_AUTO_FPS_MODE_E;

typedef struct tagAispAFpsCtrl {
    NI_BOOL              bEnable;     /* 自动切换帧率使能 */
    NI_U32               u32NewFps;   /* 发生自动转换后的帧率 * 1000, u32NewFps 仅支持设置为初始化帧率的一半 */
    NI_U32               u32GainThr;  /* 发生帧率切换时的增益阈值 */
    AISP_AUTO_FPS_MODE_E enMode;      /* 自动降帧模式 range:[0, 2] */
    NI_U8                u8Multiples; /* 降帧倍率，AISP_AUTO_FPS_BY_MULTIPLES模式使用, range:[2, 5], default:2  */
} AISP_AFPS_CTRL_S;

typedef struct tagAispMAwesInitParam {
    NI_U32 u32BufNum;
    NI_U32 u32AwesPos; /* 参见 PISP_AWES_POS_E 结构体 */
} AISP_MAWES_INIT_PARAM_S;

typedef struct tagAispMAwesData {
    NI_U16 *pu16AwesData;   /* RO, AWES统计的rgby值, 每帧统计数据=u32BlockNum*2*4bytes. */
    NI_U32  u32AwesPhyAddr; /* RO, pu16AwesData对应的物理地址 */
} AISP_MAWES_DATA_S;

typedef struct tagAispMAwesStatistics {
    NI_VOID *pvHandle;     /* RO, AWES统计句柄，用于释放时使用，用户不可修改 */
    NI_U32   u32BlockNum;  /* RO, AWES统计一帧的分块总数 */
    NI_U32   u32HBlockNum; /* RO, AWES统计水平方向分块个数 */
    NI_U32   u32VBlockNum; /* RO, AWES统计垂直方向分块个数 */

    NI_U32            u32AwesFrmNum;  /* RO, astAwesData数组有u32AwesFrmNum帧统计数据，帧数与enAwesPos变量及x帧HDR模式对应 */
    NI_U32            u32AwesPos;     /* RO, 参见 PISP_AWES_POS_E 结构体 */
    AISP_MAWES_DATA_S astAwesData[3]; /* RO, AWES统计数据，硬件一次固定三帧数据，顺序为短、长、融合帧 线性模式下数据结果保存在astAwesData[0]中*/
} AISP_MAWES_STATISTICS_S;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
