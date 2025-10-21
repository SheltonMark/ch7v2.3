/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef __NI_COMM_PM_H__
#define __NI_COMM_PM_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "ni_common.h"
#include "ni_errno.h"
#include "ni_define.h"

/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define NI_ERR_PM_ILLEGAL_PARAM NI_DEF_ERR(NI_ID_PM, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)
/* using a NULL point */
#define NI_ERR_PM_NULL_PTR NI_DEF_ERR(NI_ID_PM, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)
/* not support */
#define NI_ERR_PM_NOT_SUPPORT NI_DEF_ERR(NI_ID_PM, EN_ERR_LEVEL_ERROR, ERR_NOT_SUPPORT)
/* not permit */
#define NI_ERR_PM_NOT_PERM NI_DEF_ERR(NI_ID_PM, EN_ERR_LEVEL_ERROR, ERR_NOT_PERM)

typedef enum tagPmClkMgrPolicy {
    PM_CLK_MGR_POLICY_AUTO   = 0, /* 自动模式，用户无需配置具体时钟参数（配置无效），驱动内部自适应 */
    PM_CLK_MGR_POLICY_LEVEL  = 1, /* 等级模式，根据用户配置的PM_CLK_CAP_LEVEL_E等级适配相应的时钟 */
    PM_CLK_MGR_POLICY_MANUAL = 2, /* 用户自定义模式，完全根据用户配置的时钟工作，用户在使用此模式时需要对媒体模块时钟有所了解 */

    PM_CLK_CFG_MODE_BUTT,
} PM_CLK_MGR_POLICY_E;

typedef enum tagPmClkCapLevel {
    PM_CLOCK_CAP_LEVEL_0 = 0, /* 默认值，不使用LEVEL模式 */

    PM_CLOCK_CAP_LEVEL_S1 = 1, /* 一般适合媒体业务单路最大分辨率为1M */
    PM_CLOCK_CAP_LEVEL_S2 = 2, /* 一般适合媒体业务单路最大分辨率为2M */
    PM_CLOCK_CAP_LEVEL_S3 = 3, /* 一般适合媒体业务单路最大分辨率为3M */
    PM_CLOCK_CAP_LEVEL_S4 = 4, /* 一般适合媒体业务单路最大分辨率为4M */
    PM_CLOCK_CAP_LEVEL_S5 = 5, /* 一般适合媒体业务单路最大分辨率为5M */

    PM_CLOCK_CAP_LEVEL_D1 = 11, /* 一般适合媒体业务双路最大分辨率为1M */
    PM_CLOCK_CAP_LEVEL_D2 = 12, /* 一般适合媒体业务双路最大分辨率为2M */
    PM_CLOCK_CAP_LEVEL_D3 = 13, /* 一般适合媒体业务双路最大分辨率为3M */
    PM_CLOCK_CAP_LEVEL_D4 = 14, /* 一般适合媒体业务双路最大分辨率为4M */
    PM_CLOCK_CAP_LEVEL_D5 = 15, /* 一般适合媒体业务双路最大分辨率为5M */

    PM_CLOCK_CAP_LEVEL_BUTT
} PM_CLK_CAP_LEVEL_E;

typedef enum tagPmClkPllId {
    PM_CLOCK_PLLID_CPU  = 0, /* 时钟类型为CPU PLL */
    PM_CLOCK_PLLID_DDR  = 1, /* 时钟类型为DDR PLL */
    PM_CLOCK_PLLID_IAS  = 2, /* 时钟类型为IAS PLL */
    PM_CLOCK_PLLID_1000 = 3, /* 时钟类型为1000M PLL */
    PM_CLOCK_PLLID_1188 = 4, /* 时钟类型为1188M PLL */
    PM_CLOCK_PLLID_ENC  = 5, /* 时钟类型为ENC PLL */

    PM_CLOCK_PLLID_BUTT,
} PM_CLOCK_PLLID_E;

typedef enum tagPmClkType {
    PM_CLOCK_TYPE_PLL = 0, /* 时钟源类型为PLL */
    PM_CLOCK_TYPE_OSC,     /* 时钟源类型为晶振 */

    PM_CLOCK_TYPE_BUTT,
} PM_CLOCK_TYPE_E;

typedef enum taPmClkPhase {
    PM_CLOCK_PHASE_0 = 0,
    PM_CLOCK_PHASE_45,
    PM_CLOCK_PHASE_90,
    PM_CLOCK_PHASE_135,
    PM_CLOCK_PHASE_180,
    PM_CLOCK_PHASE_225,
    PM_CLOCK_PHASE_270,
    PM_CLOCK_PHASE_315,

    PM_CLOCK_PHASE_BUTT
} PM_CLOCK_PHASE_E;

/* 定义调用时钟配置API接口时，用到的仅配置某个模块时钟的掩码，支持多个模块MASK"或"在一起配置,
   注意需要在硬件不工作时配置时钟,否则可能会死机 */
typedef enum tagPmClkMask {
    PM_CLOCK_MASK_SET_VI_CLK        = 0x1, /* 调用时钟配置API接口时，指定仅配VI模块时钟的掩码, 下同 */
    PM_CLOCK_MASK_SET_ISP_CLK       = 0x2,
    PM_CLOCK_MASK_SET_VPS_CLK       = 0x4,
    PM_CLOCK_MASK_SET_VENC_H26X_CLK = 0x8,
    PM_CLOCK_MASK_SET_VENC_JPEG_CLK = 0x10,
    PM_CLOCK_MASK_SET_VO_CLK        = 0x20, /* ax520 不支持 */
    PM_CLOCK_MASK_SET_AUDIO_CLK     = 0x40,
    PM_CLOCK_MASK_SET_IAS_CLK       = 0x80,
    PM_CLOCK_MASK_SET_IVE_CLK       = 0x100, /* ax520 不支持 */
    PM_CLOCK_MASK_SET_DDRPORT_CLK   = 0x200,
    PM_CLOCK_MASK_SET_TRNG_CLK      = 0x400,  /* ax520 不支持 */
    PM_CLOCK_MASK_SET_ADC_CLK       = 0x800,  /* ax520 不支持 */
    PM_CLOCK_MASK_SET_SDIO_CLK      = 0x1000, /* ax520 不支持 */
    PM_CLOCK_MASK_SET_USB_CLK       = 0x2000, /* ax520 不支持 */
    PM_CLOCK_MASK_SET_DMA_CLK       = 0x4000, /* ax520 不支持 */
    PM_CLOCK_MASK_SET_DVP_CLK       = 0x8000,
    PM_CLOCK_MASK_SET_EDP_CLK       = 0x10000, /* ax520 不支持 */

    PM_CLOCK_MASK_BUTT,
} PM_CLOCK_MASK_E;

/* 定义模块参数结构体 */
typedef struct tagPmClkModParam {
    NI_U32
    u32AutoClkModule; /* 每个bit表示CLK模块是否开放给各驱动模块自适应调节时钟，1表开放，0表不开放，具体每bit代表的模块参见CLOCK_MASK_E */
} PM_CLOCK_MOD_PARAM_S;

/* 定义各PLL的时钟配置结构体。注意：PLL为芯片关键配置，不推荐客户自行修改PLL参数 */
typedef struct tagPmClkPll {
    NI_U8 u8PD;     /* RW, PLL总开关是否断电powerdown，0:表上电，1:表下电/断电 */
    NI_U8 u8DSMPD;  /* RW, DSM是否断电powerdown，该值一般与u8PD保持一致，0:表上电，1:表下电/断电, 默认值:1 */
    NI_U8 u8DACPD;  /* RW, DAC是否断电powerdown，该值一般与u8PD保持一致，0:表上电，1:表下电/断电, 默认值:1 */
    NI_U8 u8TestEn; /* RW, 是否使能pll测试，通常需要设为0 */

    NI_U16 u16PreDiv;
    NI_U16 u16FbDiv;

    NI_U32 u32Frac;
    NI_U8  u8PostDiv1; /* RW, 第一级输出分频系数，取值:[0:3],分别表示1、2、4、8分频 */
    NI_U8  u8PostDiv2; /* RW, 第二级输出分频系数，取值:[0:3],分别表示1、2、4、8分频 */

    NI_U8 u8CpiBias;  /* RW, cpi bias配置，一般设为0 */
    NI_U8 u8CppBias;  /* RW, cpp bias配置，一般设为0 */
    NI_U8 u8GvcoBias; /* RW, gvco bias配置，一般设为0 */

    NI_U8 u8TestSel;
} PM_CLOCK_PLL_S;

typedef struct tagPmClkDelay {
    NI_BOOL bEnable;     /* 控制旁路时钟延迟功能,default:NI_TRUE  */
    NI_BOOL bRevert;     /* 控制延迟时钟取反,default:NI_FALSE  */
    NI_U32  u32DelayClk; /* 控制延迟级数, range：[0, 0x3fff], default:0x0 */
} PM_CLOCK_DELAY_S;

typedef struct tagPmClkVi {
    NI_BOOL abSensorVClkEn[CLOCK_SENSOR_VCLK_NUM];     /* RW, sensor VCLK时钟是否使能,default:NI_FALSE 用户开启流媒体业务之前务必手动开启使能。 */
    NI_U32  au32SensorVClkFreq[CLOCK_SENSOR_VCLK_NUM]; /* sensor vclk时钟频率,  range:[CLOCK_VCLK_MIN_FREQ, CLOCK_VCLK_MAX_FREQ], 单位:KHZ */

    NI_BOOL          bSensorPClkEn; /* RW, sensor pixel时钟是否使能,default:NI_FALSE 用户开启流媒体业务之前务必手动开启使能。 */
    PM_CLOCK_DELAY_S stPclkDelay;   /* RW, Pixel clock 延迟、反向等配置。一般情况下用户无需配置 */

    NI_BOOL bViBT1120ClkEn; /* RW, 视频输入BT1120接口时钟是否使能,default:NI_FALSE ax520 不支持 */

    NI_BOOL bViClkEn;  /* RW, 视频输入模块是否使能,请勿在vi硬件正在工作时关闭时钟, default:NI_TRUE */
    NI_U32  u32ViFreq; /* RW, VI模块时钟频率,  range:[CLOCK_VIU_MIN_FREQ, CLOCK_VIU_MAX_FREQ], 单位:KHZ,
                          default:CLOCK_VIU_DEFAULT_FREQ, 请勿在vi工作时修改频率,否则可能会引起死机，
                          vi模块开启时会自动根据业务量调整时钟频率*/
} PM_CLOCK_VI_S;

typedef struct tagPmClkVenc {
    NI_BOOL bEncClkEn;  /* RW, h26x时钟是否使能,请勿在h26x硬件正在工作时关闭时钟, default:NI_TRUE */
    NI_U32  u32EncFreq; /* RW, h26x编码器模块时钟频率配置,  range:[CLOCK_ENC_PLL_MIN_FREQ, CLOCK_ENC_PLL_MAX_FREQ],
                          单位:KHZ,default:CLOCK_ENC_PLL_DEFAULT_FREQ,  请勿在硬件工作时修改频率,否则可能会引起死机，
                          venc模块开启时默认会自动根据业务量调整时钟频率 */

    NI_BOOL bJpegClkEn;  /* RW, jpeg编码器模块时钟是否使能,请勿在jpeg硬件正在工作时关闭时钟, default:NI_TRUE */
    NI_U32  u32JpegFreq; /* RW, jpeg模块时钟频率,  range:[CLOCK_JPEG_MIN_FREQ, CLOCK_JPEG_MAX_FREQ],
                            单位:KHZ,default:CLOCK_JPEG_DEFAULT_FREQ, 请勿在硬件工作时修改频率,否则可能会引起死机,
                            venc模块开启时默认会自动根据业务量调整时钟频率 */

    NI_BOOL bCpuVClkEn;  /* RW, 协cpu时钟是否使能, default:NI_FALSE AX520不支持 */
    NI_U32  u32CpuVFreq; /* RW, 协处理器时钟配置,  range:[CLOCK_CPUV_MIN_FREQ, CLOCK_CPUV_MAX_FREQ],
                            单位:KHZ,default:CLOCK_CPUV_DEFAULT_FREQ, 请勿在硬件工作时修改频率,否则可能会引起死机 */
} PM_CLOCK_VENC_S;

/* CVBS 时钟相关属性 */
typedef enum tagPmClkDac {
    CLOCK_DAC_INTERNAL, /* DAC时钟选择内部移相 */
    CLOCK_DAC_EXTERNAL, /* DAC时钟选择外部管脚 */
    CLOCK_DAC_BUTT
} PM_CLOCK_DAC_E;

typedef struct tagPmClkCvbs {
    NI_BOOL        bCvbsClkEn;  /* RW, 视频输出CVBS接口时钟是否使能, default:NI_FALSE */
    NI_U32         u32Phase;    /* DAC时钟相位选择，0:0度, 1:45度, 2:90度, 3:135度, default:0 */
    PM_CLOCK_DAC_E enDacSel;    /* DAC时钟选择, default:0 */
    NI_U32         u32CvbsFreq; /* 开启cvbs时钟后，时钟频率固定为54000, 单位KHZ，修改该值不会生效 */
} PM_CLOCK_CVBS_S;

typedef struct tagPmClkLcd {
    NI_BOOL bVoLcdClkEn;         /* RW, 视频输出lcd接口时钟是否使能, default:NI_FALSE */
    NI_U32  u32SysFreq;          /* vo lcd输出接口的sys时钟频率，是u32SerFreq频率的1~7分频，  range:[CLOCK_LCD_SYS_MIN_FREQ,
                                    CLOCK_LCD_MAX_FREQ], 单位:KHZ,default:CLOCK_LCD_DEFAULT_FREQ,
                                    请勿在硬件工作时修改频率,否则可能会引起死机 */
    NI_U32 u32SerFreq;           /* vo lcd输出接口的ser时钟频率，  range:[CLOCK_LCD_MAIN_MIN_FREQ, CLOCK_LCD_MAX_FREQ],
                                    单位:KHZ,default:CLOCK_LCD_DEFAULT_FREQ, 请勿在硬件工作时修改频率,否则可能会引起死机 */
    PM_CLOCK_DELAY_S stLcdDelay; /* vo lcd输出接口的时钟延迟、反向等配置 */
} PM_CLOCK_LCD_S;

typedef enum tagPmClkVoSys {
    PM_CLOCK_VO_SYS_BT1120,
    PM_CLOCK_VO_SYS_CVBS,
    PM_CLOCK_VO_SYS_LCD,
    PM_CLOCK_VO_SYS_MIPI,

    PM_CLOCK_VO_SYS_BUTT
} PM_CLOCK_VO_SYS_E;

typedef struct tagPmClkVo {
    NI_BOOL bVoClkEn; /* RW, 视频输出模块时钟是否使能,请勿在vo硬件正在工作时关闭时钟, default:NI_FALSE */

    PM_CLOCK_VO_SYS_E enSysClkSel; /* vo sys 模块时钟选择, default:0, 请勿在硬件工作时修改,否则可能会引起死机 */

    PM_CLOCK_LCD_S stLcd; /* vo lcd输出接口的时钟配置 */

    NI_BOOL bVoMipiClkEn; /* RW, 视频输出MIPI接口时钟是否使能, default:NI_FALSE */
    NI_U32  u32MipiFreq;  /* vo mipi输出接口的时钟频率，  range:[CLOCK_MIPI_MIN_FREQ, CLOCK_MIPI_MAX_FREQ], 单位:KHZ,
                             default:CLOCK_MIPI_DEFAULT_FREQ, 请勿在硬件工作时修改频率,否则可能会引起死机 */

    PM_CLOCK_CVBS_S stCvbs; /* vo cvbs输出接口的时钟配置 */

    NI_BOOL bVoBT1120ClkEn;         /* RW, 视频输出BT1120接口时钟是否使能, default:NI_FALSE */
    NI_U32  u32BT1120ClkFreq;       /* 视频输出BT1120接口时钟频率, range:[CLOCK_BT1120_MIN_FREQ, CLOCK_BT1120_MAX_FREQ],
                                       default:CLOCK_BT1120_DEFAULT_FREQ, 请勿在硬件工作时修改频率,否则可能会引起死机 */
    PM_CLOCK_DELAY_S stBT1120Delay; /* BT1120接口时钟延迟、反向等配置 */
} PM_CLOCK_VO_S;

typedef enum tagPmClkAdc {
    PM_CLOCK_ADC_INTERNAL, /* adc时钟由内部提供 */
    PM_CLOCK_ADC_EXTERNAL, /* adc时钟由外部管脚提供 */
    PM_CLOCK_ADC_BUTT
} PM_CLOCK_ADC_E;

/* SDIO时钟相关属性 */
typedef struct tagPmClkSdio {
    NI_BOOL          bSdioClkEn;    /* RW, SDIO模块时钟是否使能 */
    PM_CLOCK_PHASE_E enSamplePhase; /* 接收采样时钟相位 */
    PM_CLOCK_PHASE_E enDrvPhase;    /* 发送时钟相位 */
    NI_U32           u32Freq;       /* 频率设置，单位KHz */
} PM_CLOCK_SDIO_S;

typedef struct tagPmClkSensor {
    NI_BOOL abSensorVClkEn[CLOCK_SENSOR_VCLK_NUM];     /* RW, sensor VCLK时钟是否使能,default:NI_FALSE 用户开启流媒体业务之前务必手动开启使能。 */
    NI_U32  au32SensorVClkFreq[CLOCK_SENSOR_VCLK_NUM]; /* sensor vclk时钟频率,  range:[CLOCK_VCLK_MIN_FREQ, CLOCK_VCLK_MAX_FREQ], 单位:KHZ */

    NI_BOOL          bSensorPClkEn; /* RW, sensor pixel时钟是否使能,default:NI_FALSE 用户开启流媒体业务之前务必手动开启使能。 */
    PM_CLOCK_DELAY_S stPclkDelay;   /* RW, Pixel clock 延迟、反向等配置。一般情况下用户无需配置 */
} PM_CLOCK_SENSOR_S;

/* 定义soc 模块时钟配置属性 */
typedef struct tagPmClkSocCfg {
    NI_BOOL abDdrPortClkEn[CLOCK_DDR_PORT_NUM];  /* RW, ddr端口时钟是否使能, 固定使能, default:NI_TRUE */
    NI_U32  au32DdrPortFreq[CLOCK_DDR_PORT_NUM]; /* RW, ddr 各port口时钟频率配置，单位:KHZ, 不推荐用户随意修改该值，频率只能是被466和1188整除的值
                                                 port0: axi_cpu 不可修改，port1: VENC prot2: DVP/XSNN port3: VI/ISP/VPS port4&port5: 不可修改。
                                                 用户可根据实际业务场景按1000或1188整数分频步长调节port口时钟 */

    NI_BOOL bTrngClkEn;    /* RW, 随机数发生器模块时钟是否使能,不推荐用户随意修改该值 */
    NI_U32  u32TrngClkSel; /* RW, 真随机数模块采样时钟选择: 0:来自PLL,1:来自晶振。不推荐用户随意修改该值 */

    PM_CLOCK_ADC_E enAdcSel; /* RW, adc时钟选择 */

    PM_CLOCK_SDIO_S astSdio[CLOCK_SDIO_MAX_NUM]; /* 目前配置无效 ax520 不支持 */

    NI_BOOL bUsbClkEn; /* RW, USB模块时钟是否使能, default:NI_FALSE */
    NI_BOOL bDmaClkEn; /* RW, dma模块时钟是否使能, default:NI_FALSE */
} PM_CLOCK_SOC_CFG_S;

/* 定义各模块时钟配置属性，请注意:各业务模块驱动加载后会自动调整该模块的时钟配置，故注释中的默认值并不是推荐值。*/
typedef struct tagPmClkCfg {
    PM_CLOCK_VI_S stVi; /* 视频输入模块相关时钟配置 */

    NI_BOOL bIspClkEn;  /* RW, ISP模块时钟是否使能,请勿在isp硬件正在工作时关闭时钟, default:NI_TRUE */
    NI_U32  u32IspFreq; /* RW, ISP模块时钟频率,    range:[CLOCK_ISP_MIN_FREQ, CLOCK_ISP_MAX_FREQ], 单位:KHZ,
                           default:CLOCK_ISP_DEFAULT_FREQ, isp模块开启时会自动根据业务量调整时钟频率 下同 */
    NI_BOOL bVpsClkEn;
    NI_U32  u32VpsFreq;

    NI_BOOL bEdpClkEn;
    NI_U32  u32EdpFreq; /* RW, EDP模块时钟频率 ax520 不支持修改 */

    NI_BOOL bDvpClkEn;
    NI_U32  u32DvpFreq; /*RW, DVP模块时钟频率，range:[CLOCK_ISP_MIN_FREQ, CLOCK_ISP_MAX_FREQ], 单位:KHZ,
                          默认使用100MHz，功耗与性能均衡。用户开启XSNN(IAS)应用时需要与u32IasFreq同步修改。如，用户需要提高XSNN智能检测的帧率，
                          可按1000或者1188整数分频的步长进行同步提升u32DvpFreq */

    PM_CLOCK_VO_S stVo; /* RW, VO模块的时钟频率配置, 请勿在硬件工作时修改频率, 否则可能会引起死机 ax520 不支持 */

    PM_CLOCK_VENC_S stVenc; /* 编码器模块相关时钟配置 */

    NI_BOOL bIasClkEn;
    NI_U32  u32IasFreq; /* RW, IAS(XSNN)智能模块时钟频率，默认使用200MHz，功耗与性能均衡。一般场景下的换算公式：（594MHz*所有智能业务总帧率）/ 30。
                         注意：调整改频率的同时需要同步修改u32DvpFreq */

    NI_BOOL bAudioClkEn;      /* RW， AUDIO模块时钟使能，默认关闭 */
    NI_U32  u32AudioRefFreq;  /* RW， AUDIO模块时钟频率，range:[CLOCK_AUDIO_MIN_FREQ, CLOCK_AUDIO_MAX_FREQ], 单位:KHZ,
                          default:CLOCK_AUDIO_DEFAULT_FREQ。一般场景下(16~32bit 位宽，8k~16k采样率)使用默认时钟即可，特殊场景可根据需求自行调节。 */
    NI_BOOL bAudioCodecClkEn; /* RW， 音频CODEC模块时钟使能，默认关闭 */

    NI_BOOL bIveClkEn;
    NI_U32  u32IveFreq; /* RW, IVE模块时钟频率 ax520 不支持 */

    PM_CLOCK_SOC_CFG_S stClkSocCfg; /* Soc相关模块时钟配置 */
} PM_CLOCK_CFG_S;

typedef enum tagPmCpuGovernors {
    PM_CPU_GOVER_USERSPACE    = 0, /* 用户模式，即手动模式，配置此模式后，由用户自己设定频率，SDK不会对CPU进行自动调频 */
    PM_CPU_GOVER_CONSERVATIVE = 1, /* 保守策略，逐级调整频率和电压 */
    PM_CPU_GOVER_ONDEMAND     = 2, /* 当前默认模式，SDK根据CPU负载自动进行实时动态调频 */
    PM_CPU_GOVER_INTERACTIVE  = 3, /* 根据CPU负载动态调频调压，比ondemand反应快*/
    PM_CPU_GOVER_PERFORMANCE  = 4, /* 高性能模式 */
    PM_CPU_GOVER_POWERSAVE    = 5, /* 功耗优先模式 */
    PM_CPU_GOVER_DISABLE      = 6, /* 关闭低功耗策略，不会进行调频，此时CPU相关的功耗配置无效 */

    PM_CPU_GOVER_BUTT,
} PM_CPU_GOVERNORS_E;

typedef enum tagPmCpuFreq {
    PM_CPU_FREQ_24M   = 24,
    PM_CPU_FREQ_250M  = 250,
    PM_CPU_FREQ_500M  = 500,
    PM_CPU_FREQ_1000M = 1000,

    PM_CPU_FREQ_BUTT,
} PM_CPU_FREQ_E;

typedef enum tagPmVoltageType {
    PM_VOLTAGE_CORE  = 0, /* CORE电压类型 */
    PM_VOLTAGE_CPU   = 1, /* CPU电压类型 */
    PM_VOLTAGE_SOC   = 2, /* SOC电压类型 */
    PM_VOLTAGE_MCU   = 3, /* MCU电压类型 */
    PM_VOLTAGE_MEDIA = 4, /* 媒体电压类型 */

    PM_VOL_BUTT
} PM_VOLTAGE_TYPE_E;

typedef enum tagPmVoltage {
    PM_VOLTAGE_DEFAULT = 0, /* 电压等级，具体值参考用户手册说明 */
    PM_VOLTAGE_UP_1    = 10,
    PM_VOLTAGE_UP_2    = 11,
    PM_VOLTAGE_UP_3    = 12,
    PM_VOLTAGE_UP_4    = 13,
    PM_VOLTAGE_UP_5    = 14,
    PM_VOLTAGE_DOWN_1  = 100,
    PM_VOLTAGE_DOWN_2  = 101,
    PM_VOLTAGE_DOWN_3  = 102,
    PM_VOLTAGE_DOWN_4  = 103,
    PM_VOLTAGE_DOWN_5  = 104,

    PM_VOLTAGE_BUTT,
} PM_VOLTAGE_E;

typedef struct tagPmVoltageAttr {
    PM_VOLTAGE_E enVoltageLevel; /* 表示目标电压等级 */
} PM_VOLTAGE_ATTR_S;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
