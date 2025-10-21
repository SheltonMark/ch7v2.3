/* 
 * ni_comm_clock.h -- clock模块对外提供的头文件
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */


#ifndef __NI_COMM_CLOCK_H__
#define __NI_COMM_CLOCK_H__


#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#include "ni_common.h"
#include "ni_errno.h"
#include "ni_define.h"


/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define NI_ERR_CLOCK_ILLEGAL_PARAM     NI_DEF_ERR(NI_ID_CLK, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)
/* using a NULL point */
#define NI_ERR_CLOCK_NULL_PTR          NI_DEF_ERR(NI_ID_CLK, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)

#define NI_ERR_CLOCK_NOT_PERM          NI_DEF_ERR(NI_ID_CLK, EN_ERR_LEVEL_ERROR, ERR_NOT_PERM)


typedef enum tagClkPllId
{
    CLOCK_PLLID_CPU     = 0,             /* 时钟类型为PLL */
    CLOCK_PLLID_DDR     = 1,             /* 时钟类型为PLL */
    CLOCK_PLLID_IAS     = 2,             /* 时钟类型为PLL */
    CLOCK_PLLID_1000    = 3,             /* 时钟类型为PLL */
    CLOCK_PLLID_1188    = 4,             /* 时钟类型为PLL */
    CLOCK_PLLID_ENC     = 5,             /* 时钟类型为PLL */

    CLOCK_PLLID_BUTT,
} CLOCK_PLLID_E;


typedef enum tagClkType
{
    CLOCK_TYPE_PLL = 0,             /* 时钟类型为PLL */
    CLOCK_TYPE_OSC,                 /* 时钟类型为晶振 */

    CLOCK_TYPE_BUTT,
} CLOCK_TYPE_E;


typedef enum tagClkPhase
{
    CLOCK_PHASE_0 = 0,
    CLOCK_PHASE_45,
    CLOCK_PHASE_90,
    CLOCK_PHASE_135,
    CLOCK_PHASE_180,
    CLOCK_PHASE_225, 
    CLOCK_PHASE_270,
    CLOCK_PHASE_315,
    CLOCK_PHASE_BUTT
}CLOCK_PHASE_E;

/* 定义调用时钟配置API接口时，用到的仅配置某个模块时钟的掩码，支持多个模块MASK"或"在一起配置, 请硬件不工作时配置时钟,否则可能会死机 */
typedef enum tagClockMask
{
    CLOCK_MASK_SET_VI_CLK         = 0x1,           /* 调用时钟配置API接口时，指定仅配VI模块时钟的掩码, 请硬件不工作时配置时钟 */
    CLOCK_MASK_SET_ISP_CLK        = 0x2,           /* 调用时钟配置API接口时，指定仅配ISP模块时钟的掩码, 请硬件不工作时配置时钟  */
    CLOCK_MASK_SET_VPS_CLK        = 0x4,           /* 调用时钟配置API接口时，指定仅配VPS模块时钟的掩码, 请硬件不工作时配置时钟  */
    CLOCK_MASK_SET_VENC_H26X_CLK  = 0x8,           /* 调用时钟配置API接口时，指定仅配VENC H26X模块时钟的掩码, 请硬件不工作时配置时钟  */
    CLOCK_MASK_SET_VENC_JPEG_CLK  = 0x10,          /* 调用时钟配置API接口时，指定仅配VENC JPEG模块时钟的掩码, 请硬件不工作时配置时钟  */
    CLOCK_MASK_SET_VO_CLK         = 0x20,          /* 调用时钟配置API接口时，指定仅配VO模块时钟的掩码, 请硬件不工作时配置时钟  */
    CLOCK_MASK_SET_AUDIO_CLK      = 0x40,          /* 调用时钟配置API接口时，指定仅配AUDIO模块时钟的掩码, 请硬件不工作时配置时钟  */
    CLOCK_MASK_SET_IAS_CLK        = 0x80,          /* 调用时钟配置API接口时，指定仅配IAS模块时钟的掩码, 请硬件不工作时配置时钟  */
    CLOCK_MASK_SET_IVE_CLK        = 0x100,         /* 调用时钟配置API接口时，指定仅配IVE模块时钟的掩码, 请硬件不工作时配置时钟  */
    CLOCK_MASK_SET_ZSP_CLK        = 0x200,         /* 调用时钟配置API接口时，指定仅配ZSP模块时钟的掩码, 请硬件不工作时配置时钟  */
    CLOCK_MASK_SET_DDRPORT_CLK    = 0x400,         /* 调用时钟配置API接口时，指定仅配DDR PORT模块时钟的掩码, 请硬件不工作时配置时钟  */
    CLOCK_MASK_SET_FLUXPORT_CLK   = 0x800,         /* 调用时钟配置API接口时，指定仅配FLUX PORT模块时钟的掩码, 请硬件不工作时配置时钟  */
    CLOCK_MASK_SET_TRNG_CLK       = 0x1000,        /* 调用时钟配置API接口时，指定仅配TRNG模块时钟的掩码, 请硬件不工作时配置时钟  */
    CLOCK_MASK_SET_ADC_CLK        = 0x2000,        /* 调用时钟配置API接口时，指定仅配ADC模块时钟的掩码, 请硬件不工作时配置时钟  */
    CLOCK_MASK_SET_SDIO_CLK       = 0x4000,        /* 调用时钟配置API接口时，指定仅配SDIO模块时钟的掩码, 请硬件不工作时配置时钟  */
    CLOCK_MASK_SET_USB_CLK        = 0x8000,        /* 调用时钟配置API接口时，指定仅配USB模块时钟的掩码, 请硬件不工作时配置时钟  */
    CLOCK_MASK_SET_DMA_CLK        = 0x10000        /* 调用时钟配置API接口时，指定仅配DMA模块时钟的掩码, 请硬件不工作时配置时钟  */
}CLOCK_MASK_E;

/* 定义模块参数结构体 */
typedef struct tagClockModParam
{
    NI_U32            u32AutoClkModule; /* 每个bit表示CLOCK模块是否开放给各驱动模块自适应调节时钟，1表开放，0表不开放，每bit代表的模块参见CLOCK_MASK_E */
}CLOCK_MOD_PARAM_S;


/* 定义各PLL的时钟配置结构体。注意：PLL为芯片关键配置，不推荐客户自行修改PLL参数 */
typedef struct tagClkPll
{
    NI_U8             u8PD;             /* RW, PLL总开关是否断电powerdown，0:表上电，1:表下电/断电 */
    NI_U8             u8DSMPD;          /* RW, DSM是否断电powerdown，该值一般与u8PD保持一致，0:表上电，1:表下电/断电, 默认值:1 */
    NI_U8             u8DACPD;          /* RW, DAC是否断电powerdown，该值一般与u8PD保持一致，0:表上电，1:表下电/断电, 默认值:1 */
    NI_U8             u8TestEn;         /* RW, 是否使能pll测试，通常需要设为0 */

    NI_U16            u16PreDiv;
    NI_U16            u16FbDiv;

    NI_U32            u32Frac;
    NI_U8             u8PostDiv1;       /* RW, 第一级输出分频系数，取值:[0:3],分别表示1、2、4、8分频 */
    NI_U8             u8PostDiv2;       /* RW, 第二级输出分频系数，取值:[0:3],分别表示1、2、4、8分频 */

    NI_U8             u8CpiBias;        /* RW, cpi bias配置，一般设为0 */
    NI_U8             u8CppBias;        /* RW, cpp bias配置，一般设为0 */
    NI_U8             u8GvcoBias;       /* RW, gvco bias配置，一般设为0 */

    NI_U8             u8TestSel;
}CLOCK_PLL_S;


typedef struct tagClkDelay
{
    NI_BOOL     bEnable;        /* 控制旁路时钟延迟功能,default:NI_TRUE  */
    NI_BOOL     bRevert;        /* 控制延迟时钟取反,default:NI_FALSE  */
    NI_U32      u32DelayClk;    /* 控制延迟级数, range：[0, 0x3fff], default:0x0 */
}CLOCK_DELAY_S;

typedef struct tagClkVi
{
    NI_BOOL                 abSensorVClkEn[CLOCK_SENSOR_VCLK_NUM];   /* RW, sensor VCLK时钟是否使能,default:NI_FALSE  */
    NI_U32                  au32SensorVClkFreq[CLOCK_SENSOR_VCLK_NUM];   /* sensor vclk时钟频率,  range:[CLOCK_VCLK_MIN_FREQ, CLOCK_VCLK_MAX_FREQ], 单位:KHZ */

    NI_BOOL                 bSensorPClkEn;           /* RW, sensor pixel时钟是否使能,default:NI_FALSE  */
    CLOCK_DELAY_S           stPclkDelay;             /* RW, Pixel clock 延迟、反向等配置 */

    NI_BOOL                 bViBT1120ClkEn;          /* RW, 视频输入BT1120接口时钟是否使能,default:NI_FALSE */
    
    NI_BOOL                 bViClkEn;                /* RW, 视频输入模块是否使能,请勿在vi硬件正在工作时关闭时钟, default:NI_TRUE */
    NI_U32                  u32ViFreq;               /* RW, VI模块时钟频率,  range:[CLOCK_VIU_MIN_FREQ, CLOCK_VIU_MAX_FREQ], 单位:KHZ, default:CLOCK_VIU_DEFAULT_FREQ, 请勿在vi工作时修改频率,否则可能会引起死机，
                                                        vi模块开启时会自动根据业务量调整时钟频率*/
}CLOCK_VI_S;

typedef struct tagClkVenc
{
    NI_BOOL                 bEncClkEn;               /* RW, h26x时钟是否使能,请勿在h26x硬件正在工作时关闭时钟, default:NI_TRUE */
    NI_U32                  u32EncFreq;              /* RW, h26x编码器模块时钟频率配置,  range:[CLOCK_ENC_PLL_MIN_FREQ, CLOCK_ENC_PLL_MAX_FREQ], 单位:KHZ,default:CLOCK_ENC_PLL_DEFAULT_FREQ,  请勿在硬件工作时修改频率,否则可能会引起死机，
                                                       venc模块开启时默认会自动根据业务量调整时钟频率*/

    NI_BOOL                 bJpegClkEn;              /* RW, jpeg编码器模块时钟是否使能,请勿在jpeg硬件正在工作时关闭时钟, default:NI_TRUE */
    NI_U32                  u32JpegFreq;             /* RW, jpeg模块时钟频率,  range:[CLOCK_JPEG_MIN_FREQ, CLOCK_JPEG_MAX_FREQ], 单位:KHZ,default:CLOCK_JPEG_DEFAULT_FREQ, 请勿在硬件工作时修改频率,否则可能会引起死机,
                                                        venc模块开启时默认会自动根据业务量调整时钟频率 */

    NI_BOOL                 bCpuVClkEn;              /* RW, 协cpu时钟是否使能, default:NI_FALSE */
    NI_U32                  u32CpuVFreq;             /* RW, 协处理器时钟配置,  range:[CLOCK_CPUV_MIN_FREQ, CLOCK_CPUV_MAX_FREQ],   单位:KHZ,default:CLOCK_CPUV_DEFAULT_FREQ, 请勿在硬件工作时修改频率,否则可能会引起死机 */
}CLOCK_VENC_S;


/* CVBS 时钟相关属性 */
typedef enum tagClkDac
{
    CLOCK_DAC_INTERNAL,                     /* DAC时钟选择内部移相 */
    CLOCK_DAC_EXTERNAL,                     /* DAC时钟选择外部管脚 */
    CLOCK_DAC_BUTT
}CLOCK_DAC_E;

typedef struct tagClkCvbs
{
    NI_BOOL               bCvbsClkEn;       /* RW, 视频输出CVBS接口时钟是否使能, default:NI_FALSE */
    NI_U32                u32Phase;         /* DAC时钟相位选择，0:0度, 1:45度, 2:90度, 3:135度, default:0 */
    CLOCK_DAC_E           enDacSel;         /* DAC时钟选择, default:0 */
    NI_U32                u32CvbsFreq;      /* 开启cvbs时钟后，时钟频率固定为54000, 单位KHZ，修改该值不会生效 */
}CLOCK_CVBS_S;


typedef struct tagClkLcd
{
    NI_BOOL             bVoLcdClkEn;              /* RW, 视频输出lcd接口时钟是否使能, default:NI_FALSE */
    NI_U32              u32SysFreq;               /* vo lcd输出接口的sys时钟频率，是u32SerFreq频率的1~7分频，  range:[CLOCK_LCD_SYS_MIN_FREQ, CLOCK_LCD_MAX_FREQ], 单位:KHZ,default:CLOCK_LCD_DEFAULT_FREQ, 请勿在硬件工作时修改频率,否则可能会引起死机 */
    NI_U32              u32SerFreq;               /* vo lcd输出接口的ser时钟频率，  range:[CLOCK_LCD_MAIN_MIN_FREQ, CLOCK_LCD_MAX_FREQ], 单位:KHZ,default:CLOCK_LCD_DEFAULT_FREQ, 请勿在硬件工作时修改频率,否则可能会引起死机 */
    CLOCK_DELAY_S       stLcdDelay;               /* vo lcd输出接口的时钟延迟、反向等配置 */
}CLOCK_LCD_S;

typedef enum tagClkVoSys
{
    CLOCK_VO_SYS_BT1120,
    CLOCK_VO_SYS_CVBS,
    CLOCK_VO_SYS_LCD,
    CLOCK_VO_SYS_MIPI,

    CLOCK_VO_SYS_BUTT
}CLOCK_VO_SYS_E;

typedef struct tagClkVo
{
    NI_BOOL                 bVoClkEn;             /* RW, 视频输出模块时钟是否使能,请勿在vo硬件正在工作时关闭时钟, default:NI_FALSE */
    
    CLOCK_VO_SYS_E          enSysClkSel;          /* vo sys 模块时钟选择, default:0, 请勿在硬件工作时修改,否则可能会引起死机 */
    
    CLOCK_LCD_S             stLcd;                /* vo lcd输出接口的时钟配置 */

    NI_BOOL                 bVoMipiClkEn;         /* RW, 视频输出MIPI接口时钟是否使能, default:NI_FALSE */
    NI_U32                  u32MipiFreq;          /* vo mipi输出接口的时钟频率，  range:[CLOCK_MIPI_MIN_FREQ, CLOCK_MIPI_MAX_FREQ], 单位:KHZ, default:CLOCK_MIPI_DEFAULT_FREQ, 请勿在硬件工作时修改频率,否则可能会引起死机 */
    
    CLOCK_CVBS_S            stCvbs;               /* vo cvbs输出接口的时钟配置 */

    NI_BOOL                 bVoBT1120ClkEn;       /* RW, 视频输出BT1120接口时钟是否使能, default:NI_FALSE */
    NI_U32                  u32BT1120ClkFreq;     /* 视频输出BT1120接口时钟频率, range:[CLOCK_BT1120_MIN_FREQ, CLOCK_BT1120_MAX_FREQ], default:CLOCK_BT1120_DEFAULT_FREQ, 请勿在硬件工作时修改频率,否则可能会引起死机 */
    CLOCK_DELAY_S           stBT1120Delay;        /* BT1120接口时钟延迟、反向等配置 */
}CLOCK_VO_S;

typedef enum tagClkAdc
{
    CLOCK_ADC_INTERNAL,                           /* adc时钟由内部提供 */
    CLOCK_ADC_EXTERNAL,                           /* adc时钟由外部管脚提供 */
    CLOCK_ADC_BUTT
}CLOCK_ADC_E;

/* SDIO时钟相关属性 */
typedef struct tagClkSdio
{
    NI_BOOL             bSdioClkEn;               /* RW, SDIO模块时钟是否使能 */
    CLOCK_PHASE_E       enSamplePhase;            /* 接收采样时钟相位 */
    CLOCK_PHASE_E       enDrvPhase;               /* 发送时钟相位 */
    NI_U32              u32Freq;                  /* 频率设置，单位KHz */
}CLOCK_SDIO_S;

/* 定义各模块时钟配置属性，请注意:各业务模块驱动加载后会自动调整该模块的时钟配置，故注释中的默认值并不是推荐值。*/
typedef struct tagClkCfg
{
    CLOCK_VI_S              stVi;                    /* 视频输入模块相关时钟配置 */

    NI_BOOL                 bIspClkEn;               /* RW, isp模块时钟是否使能,请勿在isp硬件正在工作时关闭时钟, default:NI_TRUE */
    NI_U32                  u32IspFreq;              /* RW, ISP模块时钟频率,    range:[CLOCK_ISP_MIN_FREQ, CLOCK_ISP_MAX_FREQ], 单位:KHZ, default:CLOCK_ISP_DEFAULT_FREQ, isp模块开启时会自动根据业务量调整时钟频率 */

    NI_BOOL                 bVpsClkEn;               /* RW, vps模块时钟是否使能,请勿在vps硬件正在工作时关闭时钟, default:NI_TRUE */
    NI_U32                  u32VpsFreq;              /* RW, ISP模块时钟频率,    range:[CLOCK_VPS_MIN_FREQ, CLOCK_VPS_MAX_FREQ], 单位:KHZ, default:CLOCK_VPS_DEFAULT_FREQ, vps模块开启时会自动根据业务量调整时钟频率 */
    
    CLOCK_VO_S              stVo;                    /* RW, VO模块的时钟频率配置, 请勿在硬件工作时修改频率,否则可能会引起死机 */

    CLOCK_VENC_S            stVenc;                  /* 编码器模块相关时钟配置 */

    NI_BOOL                 bIasClkEn;               /* RW, IAS模块(别名:VIP/GPU)时钟是否使能, default:NI_FALSE */
    NI_U32                  u32IasFreq;              /* RW, IAS模块时钟频率,    range:(0, CLOCK_IAS_PLL_MAX_FREQ], 单位:KHZ, default:CLOCK_IAS_PLL_DEFAULT_FREQ，不支持设置，若要改变ias频率请通过PLL API接口修改 */

    NI_BOOL                 bAudioClkEn;             /* RW, 音频模块时钟是否使能,请勿在audio硬件正在工作时关闭时钟, default:NI_FALSE,      audio.ko模块加载时会自动开启时钟 */
    NI_U32                  u32AudioRefFreq;         /* RW, 音频模块参考时钟频率, range:[CLOCK_AUDIO_MIN_FREQ, CLOCK_AUDIO_MAX_FREQ],单位:KHZ, default:CLOCK_AUDIO_DEFAULT_FREQ */
    NI_BOOL                 bAudioCodecClkEn;        /* RW, 音频codec时钟是否使能, default:NI_FALSE */

    NI_BOOL                 bZspClkEn;               /* RW, zsp模块时钟是否使能,请勿在zsp硬件正在工作时关闭时钟 */
    NI_U32                  u32ZspFreq;              /* RW, ZSP单元时钟配置，range:[CLOCK_ZSP_MIN_FREQ,CLOCK_ZSP_MAX_FREQ],  单位:KHZ, default:CLOCK_ZSP_DEFAULT_FREQ */

    NI_BOOL                 bIveClkEn;               /* RW, ive看m时钟是否使能,请勿在ive硬件正在工作时关闭时钟, default:NI_FALSE,      ive.ko模块加载时会自动开启时钟 */
    NI_U32                  u32IveFreq;              /* RW, IVE单元时钟配置，range:[CLOCK_IVE_MIN_FREQ,CLOCK_IVE_MAX_FREQ],  单位:KHZ, default:CLOCK_IVE_DEFAULT_FREQ, 请勿在硬件工作时修改频率,否则可能会引起死机 */

    NI_BOOL                 abDdrPortClkEn[CLOCK_DDR_PORT_NUM];   /* RW, ddr端口时钟是否使能, 固定使能, default:NI_TRUE */
    NI_U32                  u32DdrPort1Freq;         /* RW, ddr port口1时钟频率配置，该时钟频率会影响avc模块,  单位:KHZ,不推荐用户随意修改该值 */
    NI_U32                  u32DdrPort2Freq;         /* RW, ddr port口2时钟频率配置，该时钟频率会影响rotate/dvp/ive模块,  单位:KHZ,不推荐用户随意修改该值 */
    NI_U32                  u32DdrPort3Freq;         /* RW, ddr port口3时钟频率配置，该时钟频率会影响vps/vo模块,  单位:KHZ,不推荐用户随意修改该值 */
    NI_U32                  u32DdrPort4Freq;         /* RW, ddr port口4时钟频率配置，该时钟频率会影响vi/isp模块,  单位:KHZ,不推荐用户随意修改该值 */

    NI_BOOL                 abFluxPortClkEn[CLOCK_DDR_PORT_NUM];  /* RW, ddr流量统计时钟是否使能, default:NI_FALSE */

    NI_BOOL                 bTrngClkEn;              /* RW, 随机数发生器模块时钟是否使能,不推荐用户随意修改该值 */
    NI_U32                  u32TrngClkSel;            /* RW, 真随机数模块采样时钟选择: 0:来自TRNG震荡环,1:来之晶振,不推荐用户随意修改该值 */
    
    CLOCK_ADC_E             enAdcSel;                /* RW, adc时钟选择 */
    
    CLOCK_SDIO_S            stSdio0;                 /* 目前配置无效 */
    CLOCK_SDIO_S            stSdio1;                 /* 目前配置无效 */

    NI_BOOL                 bUsbClkEn;               /* RW, USB模块时钟是否使能, default:NI_FALSE */
    NI_BOOL                 bDmaClkEn;               /* RW, dma模块时钟是否使能, default:NI_FALSE */
}CLOCK_CFG_S;


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
