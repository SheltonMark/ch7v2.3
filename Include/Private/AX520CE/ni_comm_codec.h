/**************************************************************************************************
*
* Copyright (c) 2019-2025 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef __NI_COMM_CODEC_H__
#define __NI_COMM_CODEC_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "ni_common.h"
#include "ni_errno.h"
#include "ni_comm_aio.h"

/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define NI_ERR_CODEC_ILLEGAL_PARAM NI_DEF_ERR(NI_ID_CODEC, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)
/* using a NULL point */
#define NI_ERR_CODEC_NULL_PTR NI_DEF_ERR(NI_ID_CODEC, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)
/* using a NULL point */
#define NI_ERR_CODEC_NOT_PERM NI_DEF_ERR(NI_ID_CODEC, EN_ERR_LEVEL_ERROR, ERR_NOT_PERM)

/*可以配置的boost 增益值枚举*/
typedef enum tagCodecGainBoost {
    CODEC_GAINBOOST_0DB = 0,  // 0dB
    CODEC_GAINBOOST_3DB,      // 3dB
    CODEC_GAINBOOST_6DB,      // 6dB
    CODEC_GAINBOOST_9DB,      // 9dB
    CODEC_GAINBOOST_12DB,     // 12dB
    CODEC_GAINBOOST_15DB,     // 15dB
    CODEC_GAINBOOST_18DB,     // 18dB
    CODEC_GAINBOOST_21DB,     // 21dB

    CODEC_GAINBOOST_BUTT,
} CODEC_GAINBOOST_E;

typedef enum tagCodecIntf {
    CODEC_INTF_PCM                 = 0,
    CODEC_INTF_I2S                 = 1,
    CODEC_INTF_I2S_RIGHT_JUSTIFIED = 2,
    CODEC_INTF_I2S_LEFT_JUSTIFIED  = 3,
    CODEC_INTF_BUTT
} CODEC_INTF_E;

typedef enum tagCodecRole { CODEC_ROLE_MASTER = 0,
                            CODEC_ROLE_SLAVE  = 1,
                            CODEC_ROLE_BUTT } CODEC_ROLE_E;

typedef enum tagCodecMclkLrckRatio {
    CODEC_MCLK_LRCK_375X = 0,
    CODEC_MCLK_LRCK_750X = 1,
    CODEC_MCLK_LRCK_768X = 2,
    CODEC_MCLK_LRCK_1536X,
    CODEC_MCLK_LRCK_1500X,
    CODEC_MCLK_LRCK_500X,
    CODEC_MCLK_LRCK_250X,
    CODEC_MCLK_LRCK_384X,
    CODEC_MCLK_LRCK_544X,
    CODEC_MCLK_LRCK_1088X,
    CODEC_MCLK_LRCK_512X,
    CODEC_MCLK_LRCK_1024X,
    CODEC_MCLK_LRCK_136X,
    CODEC_MCLK_LRCK_272X,
    CODEC_MCLK_LRCK_128X,
    CODEC_MCLK_LRCK_256X,
    CODEC_MCLK_LRCK_BUTT,
} CODEC_MCLK_LRCK_RATIO_E;

typedef enum tagCodecChannel {
    CODEC_RIGHT_CHANNEL = 0,
    CODEC_LEFT_CHANNEL  = 1,
    CODEC_CHANNEL_BUTT,
} CODEC_CHANNEL_E;

/*模拟采样输入源选择类型*/
typedef enum tagCodecAInSource {
    CODEC_AIN_SOURCE_MIC_SINGAL_END  = 0, /* mic 是单端信号输入*/
    CODEC_AIN_SOURCE_MIC_PSEUDO_DIFF = 1, /* mic 是差分信号输入*/
    CODEC_AIN_SOURCE_LIN,                 /* line in 信号输入（均为单端信号）*/

    CODEC_AIN_SOURCE_BUTT,
} CODEC_AIN_SOURCE_E;

/*采样率及通道选择的公共属性*/
typedef struct tagCodecPubAttr {
    CODEC_INTF_E            enAudioIntf;       // 数字传输接口
    CODEC_ROLE_E            enRole;            // codec 的角色，master or slave
    CODEC_MCLK_LRCK_RATIO_E enAdcRatio;        // mclk与lrck之间的比例关系
    CODEC_MCLK_LRCK_RATIO_E enDacRatio;        // mclk与lrck之间的比例关系
    CODEC_AIN_SOURCE_E      enRightSrc;        // 输入数据源接口类型：MIC、Line in
    CODEC_AIN_SOURCE_E      enLeftSrc;         // 输入数据源接口类型：MIC、Line in
    NI_U32                  u32AdcSampleRate;  // 采样率，单位Hz,eg:44.1KHz -> 44100，取值范围[8000,96000]
    NI_U32                  u32DacSampleRate;  // 采样率，单位Hz,eg:44.1KHz -> 44100，取值范围[8000,96000]
    AUDIO_BIT_WIDTH_E       enBitWidth;        // 采样深度，目前仅支持24bit
} CODEC_PUB_ATTR_S;

/*增益模块配置属性*/
typedef struct tagCodecGainAttr {
    NI_S8  s8AdcAnalogGain;     // ADC模拟增益，取值范围[-27,36]，单位DB
    NI_S8  s8AdcDigitalGain;    // ADC数字增益，取值范围[-50,36]，单位DB
    NI_S16 s16DacDigitalGain;   // DAC数字增益，取值范围[-108,0]对应[-54,0]。
                                //  每个step对应0.5DB，eg:-108->-54DB,-107->-53.5
    CODEC_GAINBOOST_E enBoost;  // boost控制
} CODEC_GAIN_ATTR_S;

/*增益模块配置属性*/
typedef struct tagCodecChanGainAttr {
    CODEC_CHANNEL_E   enChannel;
    CODEC_GAIN_ATTR_S stGainAttr;
} CODEC_CHAN_GAIN_ATTR_S;

typedef struct tagCodecChanEn {
    CODEC_CHANNEL_E enChannel;
    NI_BOOL         bChanEn;
} CODEC_CHAN_EN_S;

typedef enum tagCodecAnalogConvert {
    ANALOG_CONVERT_ADC = 0,
    ANALOG_CONVERT_DAC = 1,
    ANALOG_CONVERT_BUTT,
} CODEC_ANALOG_CONVERT_E;

typedef struct tagCodecChanMute {
    CODEC_ANALOG_CONVERT_E enConvert;
    CODEC_CHANNEL_E        enChannel;
    NI_BOOL                bMuteEn;
} CODEC_CHAN_MUTE_S;

/* ALC decay time */
typedef enum tagCodecAlcDcyt {
    CODEC_ALC_DCYT_104US   = 0,
    CODEC_ALC_DCYT_208US   = 1,
    CODEC_ALC_DCYT_416US   = 2,
    CODEC_ALC_DCYT_932US   = 3,
    CODEC_ALC_DCYT_1864US  = 4,
    CODEC_ALC_DCYT_3728US  = 5,
    CODEC_ALC_DCYT_7456US  = 6,
    CODEC_ALC_DCYT_14912US = 7,
    CODEC_ALC_DCYT_29824US = 8,
    CODEC_ALC_DCYT_59648US = 9,
    CODEC_ALC_DCYT_119296US,
    CODEC_ALC_DCYT_238592US,
    CODEC_ALC_DCYT_477184US,
    CODEC_ALC_DCYT_954368US,
    CODEC_ALC_DCYT_BUTT
} CODEC_ALC_DCYT_E;

/* ALC attack time */
typedef enum tagCodecAlcAtkt {
    CODEC_ALC_ATKT_62P5US = 0,
    CODEC_ALC_ATKT_125US,
    CODEC_ALC_ATKT_250US,
    CODEC_ALC_ATKT_500US,
    CODEC_ALC_ATKT_1000US,
    CODEC_ALC_ATKT_2000US,
    CODEC_ALC_ATKT_4000US,
    CODEC_ALC_ATKT_8000US,
    CODEC_ALC_ATKT_16000US,
    CODEC_ALC_ATKT_32000US,
    CODEC_ALC_ATKT_64000US,
    CODEC_ALC_ATKT_128000US,
    CODEC_ALC_ATKT_256000US,
    CODEC_ALC_ATKT_512000US,
    CODEC_ALC_ATKT_BUTT
} CODEC_ALC_ATKT_E;

/* ALC hold time */
typedef enum tagCodecAlcHolt {
    CODEC_ALC_HOLT_0US = 0,
    CODEC_ALC_HOLT_250US,
    CODEC_ALC_HOLT_500US,
    CODEC_ALC_HOLT_1MS,
    CODEC_ALC_HOLT_2MS,
    CODEC_ALC_HOLT_4MS,
    CODEC_ALC_HOLT_8MS,
    CODEC_ALC_HOLT_16MS,
    CODEC_ALC_HOLT_32MS,
    CODEC_ALC_HOLT_64MS,
    CODEC_ALC_HOLT_128MS,
    CODEC_ALC_HOLT_256MS,
    CODEC_ALC_HOLT_512MS,
    CODEC_ALC_HOLT_1024MS,
    CODEC_ALC_HOLT_2048MS,
    CODEC_ALC_HOLT_4096MS,
    CODEC_ALC_HOLT_BUTT
} CODEC_ALC_HOLT_E;

/* ALC noise gate thread */
typedef enum tagCodecAlcNgth {
    CODEC_ALC_NGTH__84DB = 0, /* -84db */
    CODEC_ALC_NGTH__78DB,
    CODEC_ALC_NGTH__72DB,
    CODEC_ALC_NGTH__66DB,
    CODEC_ALC_NGTH__60DB,
    CODEC_ALC_NGTH__54DB,
    CODEC_ALC_NGTH__48DB,
    CODEC_ALC_NGTH__42DB,
    CODEC_ALC_NGTH_BUTT
} CODEC_ALC_NGTH_E;

/* ALC zero-cross timeout */
typedef enum tagCodecAlcZctr {
    CODEC_ALC_ZCTR_832US = 0,
    CODEC_ALC_ZCTR_1664US,
    CODEC_ALC_ZCTR_3328US,
    CODEC_ALC_ZCTR_6656US,
    CODEC_ALC_ZCTR_NOLIMINT,
    CODEC_ALC_ZCTR_BUTT
} CODEC_ALC_ZCTR_E;

/*ALC配置属性*/
typedef struct tagCodecAlcAttr {
    NI_BOOL          bAlcEn; /* ALC 功能开关 */
    CODEC_ALC_DCYT_E enDecayTime;
    CODEC_ALC_ATKT_E enAttackTime;
    CODEC_ALC_HOLT_E enHoldTime;
    NI_S32           s32TargetLevel; /* ALC target level,取值范围[-34,-4],需要2对齐 */
    CODEC_ALC_NGTH_E enNoiseGate;    /* 噪声阈值 */
    CODEC_ALC_ZCTR_E enZeroCrossTimeOut;
} CODEC_ALC_ATTR_S;

/* POP sound control  */
typedef enum tagCodecPopCtrl {
    CODEC_POP_CTRL_DISABLE = 0,
    CODEC_POP_CTRL_WEAK,
    CODEC_POP_CTRL_MIDDLE,
    CODEC_POP_CTRL_STRONG,
    CODEC_POP_CTRL_BUTT
} CODEC_POP_CTRL_E;

/*ADC扩展属性配置*/
typedef struct tagCodecAdcExtAttr {
    NI_BOOL          bDemEnable; /* DEM使能 */
    NI_BOOL          bDwaEnable; /* DWA使能 */
    NI_BOOL          bInvEnable; /* 数据翻转使能 */
    NI_BOOL          bHpfEnable; /* 高通滤波使能 */
    CODEC_POP_CTRL_E enPopCtrl;  /* pop音控制使能 */
} CODEC_ADC_EXT_ATTR_S;

typedef enum tagCodecDacInv {
    CODEC_DAC_INV_NORMAL = 0,
    CODEC_DAC_INV_LEFT,  /* 左通道翻转 */
    CODEC_DAC_INV_RIGHT, /* 右通道翻转 */
    CODEC_DAC_INV_BOTH,  /* 左、右通道均翻转 */
    CODEC_DAC_INV_BUTT
} CODEC_DAC_INV_E;

/*DAC扩展属性配置*/
typedef struct tagCodecDacExtAttr {
    NI_BOOL         bDemEnable; /* DEM使能 */
    NI_BOOL         bDwaEnable; /* DWA使能 */
    CODEC_DAC_INV_E enMixInv;
    NI_BOOL         bInvEnable; /* 左右声道混合使能 */
    NI_BOOL         bDeemph;    /* De-emphasis 滤波器使能开关，
                                 目前支持：(1) ratio为256，采样率为48KHz
                                         (2) ratio为256，采样率为44.1KHz
                                         (3) ratio为384，采样率为32KHz*/
    CODEC_POP_CTRL_E enPopCtrl; /* pop音控制使能 */
} CODEC_DAC_EXT_ATTR_S;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
