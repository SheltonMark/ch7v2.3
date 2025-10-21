/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef __NI_COMM_AIO_H__
#define __NI_COMM_AIO_H__

#include "ni_errno.h"
#include "ni_common.h"
#include "ni_define.h"
// #include "list.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/* invlalid device ID */
#define NI_ERR_AI_INVALID_DEVID NI_DEF_ERR(NI_ID_AI, EN_ERR_LEVEL_ERROR, ERR_INVALID_DEVID)
/* invlalid channel ID */
#define NI_ERR_AI_INVALID_CHNID NI_DEF_ERR(NI_ID_AI, EN_ERR_LEVEL_ERROR, ERR_INVALID_CHNID)
/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define NI_ERR_AI_ILLEGAL_PARAM NI_DEF_ERR(NI_ID_AI, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)
/* using a NULL point */
#define NI_ERR_AI_NULL_PTR NI_DEF_ERR(NI_ID_AI, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)
/* try to enable or initialize system,device or channel, before configing attribute */
#define NI_ERR_AI_NOT_CONFIG NI_DEF_ERR(NI_ID_AI, EN_ERR_LEVEL_ERROR, ERR_NOT_CONFIG)
/* operation is not supported by NOW */
#define NI_ERR_AI_NOT_SUPPORT NI_DEF_ERR(NI_ID_AI, EN_ERR_LEVEL_ERROR, ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change stati attribute */
#define NI_ERR_AI_NOT_PERM NI_DEF_ERR(NI_ID_AI, EN_ERR_LEVEL_ERROR, ERR_NOT_PERM)
/* the devide is not enabled  */
#define NI_ERR_AI_NOT_ENABLED NI_DEF_ERR(NI_ID_AI, EN_ERR_LEVEL_ERROR, ERR_UNEXIST)
/* failure caused by malloc memory */
#define NI_ERR_AI_NOMEM NI_DEF_ERR(NI_ID_AI, EN_ERR_LEVEL_ERROR, ERR_NOMEM)
/* failure caused by malloc buffer */
#define NI_ERR_AI_NOBUF NI_DEF_ERR(NI_ID_AI, EN_ERR_LEVEL_ERROR, ERR_NOBUF)
/* no data in buffer */
#define NI_ERR_AI_BUF_EMPTY NI_DEF_ERR(NI_ID_AI, EN_ERR_LEVEL_ERROR, ERR_BUF_EMPTY)
/* no buffer for new data */
#define NI_ERR_AI_BUF_FULL NI_DEF_ERR(NI_ID_AI, EN_ERR_LEVEL_ERROR, ERR_BUF_FULL)
/* system is not ready,had not initialed or loaded*/
#define NI_ERR_AI_SYS_NOTINIT NI_DEF_ERR(NI_ID_AI, EN_ERR_LEVEL_ERROR, ERR_SYS_NOTREADY)

#define NI_ERR_AI_BUSY NI_DEF_ERR(NI_ID_AI, EN_ERR_LEVEL_ERROR, ERR_BUSY)

/* invlalid device ID */
#define NI_ERR_AO_INVALID_DEVID NI_DEF_ERR(NI_ID_AO, EN_ERR_LEVEL_ERROR, ERR_INVALID_DEVID)
/* invlalid channel ID */
#define NI_ERR_AO_INVALID_CHNID NI_DEF_ERR(NI_ID_AO, EN_ERR_LEVEL_ERROR, ERR_INVALID_CHNID)
/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define NI_ERR_AO_ILLEGAL_PARAM NI_DEF_ERR(NI_ID_AO, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)
/* using a NULL point */
#define NI_ERR_AO_NULL_PTR NI_DEF_ERR(NI_ID_AO, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)
/* try to enable or initialize system,device or channel, before configing attribute */
#define NI_ERR_AO_NOT_CONFIG NI_DEF_ERR(NI_ID_AO, EN_ERR_LEVEL_ERROR, ERR_NOT_CONFIG)
/* operation is not supported by NOW */
#define NI_ERR_AO_NOT_SUPPORT NI_DEF_ERR(NI_ID_AO, EN_ERR_LEVEL_ERROR, ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change stati attribute */
#define NI_ERR_AO_NOT_PERM NI_DEF_ERR(NI_ID_AO, EN_ERR_LEVEL_ERROR, ERR_NOT_PERM)
/* the devide is not enabled  */
#define NI_ERR_AO_NOT_ENABLED NI_DEF_ERR(NI_ID_AO, EN_ERR_LEVEL_ERROR, ERR_UNEXIST)
/* failure caused by malloc memory */
#define NI_ERR_AO_NOMEM NI_DEF_ERR(NI_ID_AO, EN_ERR_LEVEL_ERROR, ERR_NOMEM)
/* failure caused by malloc buffer */
#define NI_ERR_AO_NOBUF NI_DEF_ERR(NI_ID_AO, EN_ERR_LEVEL_ERROR, ERR_NOBUF)
/* no data in buffer */
#define NI_ERR_AO_BUF_EMPTY NI_DEF_ERR(NI_ID_AO, EN_ERR_LEVEL_ERROR, ERR_BUF_EMPTY)
/* no buffer for new data */
#define NI_ERR_AO_BUF_FULL NI_DEF_ERR(NI_ID_AO, EN_ERR_LEVEL_ERROR, ERR_BUF_FULL)
/* system is not ready,had not initialed or loaded*/
#define NI_ERR_AO_SYS_NOTINIT NI_DEF_ERR(NI_ID_AO, EN_ERR_LEVEL_ERROR, ERR_SYS_NOTREADY)

#define NI_ERR_AO_BUSY NI_DEF_ERR(NI_ID_AO, EN_ERR_LEVEL_ERROR, ERR_BUSY)

#define TEST_MODE_SAMPLE_RATE (5000000 / 1024 / 1000 / 2)

/* audio sampling */
typedef enum tagAudioSampleRate {
    AUDIO_SAMPLE_RATE_8K  = 8,  /* 8kHz sampling rate */
    AUDIO_SAMPLE_RATE_16K = 16, /* 16kHz sampling rate */
    AUDIO_SAMPLE_RATE_32K = 32, /* 32kHz sampling rate */
    AUDIO_SAMPLE_RATE_48K = 48, /* 48kHz sampling rate */
    AUDIO_SAMPLE_RATE_64K = 64, /* 64kHz sampling rate */
    AUDIO_SAMPLE_RATE_96K = 96, /* 48kHz sampling rate */
} AUDIO_SAMPLE_RATE_E;

/* audio sampling width */
typedef enum tagAudioBitWidth {
    AUDIO_BIT_WIDTH_4  = 4,  /* 4bit width */
    AUDIO_BIT_WIDTH_8  = 8,  /* 8bit width */
    AUDIO_BIT_WIDTH_16 = 16, /* 16bit width */
    AUDIO_BIT_WIDTH_18 = 18, /* 18bit width */
    AUDIO_BIT_WIDTH_20 = 20, /* 20bit width */
    AUDIO_BIT_WIDTH_22 = 22, /* 22bit width */
    AUDIO_BIT_WIDTH_24 = 24, /* 24bit width */
    AUDIO_BIT_WIDTH_26 = 26, /* 26bit width */
    AUDIO_BIT_WIDTH_28 = 28, /* 28bit width */
    AUDIO_BIT_WIDTH_30 = 30, /* 30bit width */
    AUDIO_BIT_WIDTH_32 = 32, /* 32bit width */
} AUDIO_BIT_WIDTH_E;

/* audio input/output mode*/
typedef enum hiAIO_MODE_E {
    AIO_MODE_I2S_MASTER = 0,  /* I2S master mode */
    AIO_MODE_I2S_SLAVE,       /* I2S slave mode */
    AIO_MODE_PCM_MASTER_STD,  /* PCM master standard mode */
    AIO_MODE_PCM_MASTER_NSTD, /* PCM master non-standard mode */
    AIO_MODE_PCM_SLAVE_STD,   /* PCM slave standard mode */
    AIO_MODE_PCM_SLAVE_NSTD,  /* PCM slave non-standard mode */

    AIO_MODE_BUTT
} AIO_MODE_E;

/*audio sample clock edge*/
typedef enum tagAudioSampleEdeg {
    AUDIO_SAMPLE_POSEDGE = 0, /*positive edge*/
    AUDIO_SAMPLE_NEGEDGE = 1, /*negative edge*/

    AUDIO_SAMPLE_BUTT
} AUDIO_SAMPLE_EDGE_E;

typedef enum tagAudioLrck {
    AUDIO_LRCK_I2S0 = 0,
    AUDIO_LRCK_I2S1 = 1,

    AUDIO_LRCK_BUTT
} AUDIO_LRCK_E;

typedef enum tagAudioBclk {
    AUDIO_BCLK_I2S0 = 0,
    AUDIO_BCLK_I2S1 = 1,

    AUDIO_BCLK_BUTT
} AUDIO_BCLK_E;

typedef enum tagAudioCodecType { AUDIO_EXTERNAL_CODEC = 0,
                                 AUDIO_INTERNAL_CODEC,
                                 AUDIO_BUTT_CODEC } AUDIO_CODEC_E;

/* audio input and output attributes type */
typedef struct tagAioAttr {
    AIO_MODE_E          enWorkMode; /* audio work mode */
    NI_U32              u32SampleRate;
    AUDIO_SAMPLE_EDGE_E enSampleEdge; /*audio sample clock edge*/
    AUDIO_BIT_WIDTH_E   enBitWidth;   /* audio sampleing width */
    AUDIO_LRCK_E        enLrck;
    AUDIO_BCLK_E        enBclk;
    NI_U32              u32CodecMclk; /* codec Mclk exp:12288000(Hz) */
    AUDIO_CODEC_E       enCodecType;
} AIO_ATTR_S;

/* audio work mode*/
typedef enum tagAudioWorkMode {
    AUDIO_WORK_MODE_NORMAL = 0, /* 普通模式 */
    AUDIO_WORK_MODE_TEST,       /* 测试模式 */
    AUDIO_WORK_MODE_AI2AO,      /*直通模式 */
    AUDIO_WORK_MODE_DEC2ENC,    /* 输入给ENC通道数据由DEC提供*/
    AUDIO_WORK_MODE_BUTT
} AUDIO_WORK_MODE_E;

typedef enum tagAudioLrckMode {
    AUDIO_LRCK_MODE_NORMAL   = 0x0, /* 左右通道数据来自左右声道 */
    AUDIO_LRCK_MODE_EXCHANGE = 0x1, /* 左右通道数据来自右左声道 */
} AUDIO_LRCK_MODE_E;

typedef enum {
    AUDIO_CHN_LEFT = 0, /* 左声道 */
    AUDIO_CHN_RIGHT,    /* 右声道 */
} AUDIO_CHN_E;

typedef struct AudioDecNodeInfo {
    NI_U32 u32TotalNodeNum; /* total buf number  */
    NI_U32 u32FreeNodeNum;  /* free  buf number  */
    NI_U32 u32BusyNodeNum;  /* busy buf number */
} ADEC_NODE_INFO_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*  __NI_COMM_AIO_H__  */
