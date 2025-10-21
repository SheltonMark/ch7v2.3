/******************************************************************************

  Copyright (C), 2014-2020, ZheJiang Chipup Technology CO.,LTD.

 ******************************************************************************
  File Name     : mkp_audio.h
  Version       : Initial Draft
  Author        : Lv Zhuqing
  Created       : 2014.5.20
  Last Modified :
  Description   : 
  Function List :
  DHstory       :
  1.Date        : 2014/5/20
    Author      : 24497
    Modification: Create
******************************************************************************/
#ifndef __MKP_AUDIO_H__
#define __MKP_AUDIO_H__

#include "ni_type.h"
#include "nial_ioctl.h"


#include "ni_common.h"
#include "ni_comm_aio.h"
#include "ni_comm_aenc.h"
#include "ni_comm_adec.h"
#include "mkp_ioctl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define AUDIO_BURST_BYTE_SIZE (64)
#define AUDIO_BURST_BIT_SIZE  (AUDIO_BURST_BYTE_SIZE*8)

#define  AUDIO_MAKE_FD(dev, chn)            ((dev << 8)|(chn))
#define  AUDIO_MAKE_DEV(fd)                 (fd >> 8)
#define  AUDIO_MAKE_CHN(fd)                 (fd & 0xff)
#define  ALIGNSIZE                          (1024)
/* 对齐操作*/
#define NIAL_align(value, align)   ((( (value) + ( (align) - 1 ) ) \
                                     / (align) ) * (align) )
#if 1

#define AENC_STREAM_MAX_NODE_NUM    (20)
#define ADEC_STREAM_MAX_NODE_NUM    (20)

#define AENC_STREAM_DEFAULT_NODE_NUM    (5)  
#define ADEC_STREAM_DEFAULT_NODE_NUM    (5)

#endif

/*#define AUDIO_CALC_BURSTNUM(enType, enBitWidth, enSampleRate, u32PacketPeriod) \
	((PT_BYPASS == (enType))?(((enBitWidth) < AUDIO_BIT_WIDTH_16)?\
	((( u32PacketPeriod )*AUDIO_BIT_WIDTH_8*(enSampleRate) + AUDIO_BURST_BIT_SIZE - 1) / AUDIO_BURST_BIT_SIZE):\
	((( u32PacketPeriod )*AUDIO_BIT_WIDTH_16*(enSampleRate) + AUDIO_BURST_BIT_SIZE - 1) / AUDIO_BURST_BIT_SIZE)):\
	(((PT_G711A == (enType)) || (PT_G711U == (enType)))?\
	(((u32PacketPeriod)*AUDIO_BIT_WIDTH_8*(enSampleRate) + AUDIO_BURST_BIT_SIZE - 1) / AUDIO_BURST_BIT_SIZE):\
	(((u32PacketPeriod)*AUDIO_BIT_WIDTH_4*(enSampleRate) + AUDIO_BURST_BIT_SIZE - 1) / AUDIO_BURST_BIT_SIZE)))	*/

static inline NI_U32 AUDIO_CalcBurstNum(PAYLOAD_TYPE_E enType, AUDIO_BIT_WIDTH_E enBitWidth, NI_U32 u32SampleRate, NI_U32 u32PacketPeriod)
{
    if (PT_BYPASS == enType)
    {
        if (enBitWidth < AUDIO_BIT_WIDTH_16)
        {
            return (u32PacketPeriod*AUDIO_BIT_WIDTH_8*u32SampleRate/1000 + AUDIO_BURST_BIT_SIZE - 1) / AUDIO_BURST_BIT_SIZE;
        }
        else
        {
            return (u32PacketPeriod*AUDIO_BIT_WIDTH_16*u32SampleRate/1000 + AUDIO_BURST_BIT_SIZE - 1) / AUDIO_BURST_BIT_SIZE;
        }
    }
    else if ((PT_G711A == enType) || (PT_G711U == enType))
    {
        return (u32PacketPeriod*AUDIO_BIT_WIDTH_8*u32SampleRate/1000 + AUDIO_BURST_BIT_SIZE - 1) / AUDIO_BURST_BIT_SIZE;
    }
    else
    {
        return (u32PacketPeriod*AUDIO_BIT_WIDTH_4*u32SampleRate/1000 + AUDIO_BURST_BIT_SIZE - 1) / AUDIO_BURST_BIT_SIZE;
    }

}

typedef struct tagAudioStreamIoCtl
{
    AUDIO_STREAM_S stStream;
    NI_U32         u32TimeOut;
}AUDIO_STREAM_IOCTL_S;


typedef struct tagAudioAdecAECRef
{
    NI_U64          u64MicPts;  /* mic采集帧对应的pts */
    AUDIO_STREAM_S  stStream;   /* mic采集帧对应的参考帧信息 */
    NI_U32          u32TimeOut;/* 获取参考帧的超时时间 */
}AUDIO_ADEC_AEC_REF_S;


/* 通道创建参数 */
typedef struct tagCreateChnParam
{
    union
    {
        AENC_ATTR_S stAencAttr;
        ADEC_ATTR_S stAdecAttr;
    };
    NI_U32 u32BufPhyAddr;/* 传出，获取通道buf的物理地址 */
    NI_U32 u32BufVirtAddr;
}CREATE_CHN_PARAM_S; 

typedef enum tagIocNrAudio
{   
    IOC_NR_AUDIO_MK_DEV_CHN = 0,
    IOC_NR_AUDIO_INIT,
    IOC_NR_AUDIO_DEINIT,

    IOC_NR_AUDIO_SET_AUDIO_WORK_MODE,
    IOC_NR_AUDIO_SET_AUDIO_LRCK_SEL_MODE,

    IOC_NR_AUDIO_SET_AI_PUB_ATTR,
    IOC_NR_AUDIO_GET_AI_PUB_ATTR,

    IOC_NR_AUDIO_SET_AO_PUB_ATTR,
    IOC_NR_AUDIO_GET_AO_PUB_ATTR,

    IOC_NR_AUDIO_CREATE_AENC_CHN,
    IOC_NR_AUDIO_DESTROY_AENC_CHN,

    IOC_NR_AUDIO_START_AENC_CHN,
    IOC_NR_AUDIO_STOP_AENC_CHN,

    IOC_NR_AUDIO_GET_STREAM,
    IOC_NR_AUDIO_RELEASE_STREAM,

    IOC_NR_AUDIO_CREATE_ADEC_CHN,
    IOC_NR_AUDIO_DESTROY_ADEC_CHN,

    IOC_NR_AUDIO_START_ADEC_CHN,
    IOC_NR_AUDIO_STOP_ADEC_CHN,

    IOC_NR_AUDIO_SEND_ADEC_STREAM,
    IOC_NR_AUDIO_REGUEST_ADEC_BUF,
    IOC_NR_AUDIO_PAUSE_ADEC_CHN,
    IOC_NR_FAAC_INIT,
    IOC_NR_FAAC_DEINIT,

    IOC_NR_FAAC_FFT,
    IOC_NR_FAAC_MDCT,
    IOC_NR_FAAC_ITER,
    IOC_NR_FAAC_MATH_OPERATION,

    IOC_NR_AEC_INIT,
    IOC_NR_AEC_DEINIT,
    IOC_NR_AEC_SET_MDF_PARAM,
    IOC_NR_AEC_RUN,
    IOC_NR_AEC_WAIT_EXE_DONE,
    IOC_NR_AUDIO_AENC_MAKE_DEV, 
    IOC_NR_AUDIO_ADEC_MAKE_DEV,

    IOC_NR_AUDIO_SET_AENC_NODE_NUM,
    IOC_NR_AUDIO_GET_AENC_NODE_NUM,

    IOC_NR_AUDIO_SET_ADEC_NODE_NUM,
    IOC_NR_AUDIO_GET_ADEC_NODE_NUM,
    IOC_NR_AUDIO_GET_ADEC_CHN_NODE_INFO,
    IOC_NR_AUDIO_ADEC_CLEAR_CHN_NODE,

    IOC_NR_AUDIO_GET_ADEC_PLAYING_STREAM,
    IOC_NR_AUDIO_PUT_ADEC_PLAYING_STREAM,
    IOC_NR_AUDIO_RELEASE_ADEC_PLAYING_STREAM,
        
    IOC_NR_AUDIO_BUTT
} IOC_NR_AUDIO_E;


#define IOCTL_AUDIO_MAKE_DEV_CHN                    _IOW(IOC_MAGIC_AUDIO, IOC_NR_AUDIO_MK_DEV_CHN, NI_U32)
#define IOCTL_AUDIO_INIT                            _IO(IOC_MAGIC_AUDIO, IOC_NR_AUDIO_INIT)
#define IOCTL_AUDIO_DEINIT                          _IO(IOC_MAGIC_AUDIO, IOC_NR_AUDIO_DEINIT)

#define IOCTL_AUDIO_SET_AUDIO_WORK_MODE             _IOW(IOC_MAGIC_AUDIO, IOC_NR_AUDIO_SET_AUDIO_WORK_MODE, AUDIO_WORK_MODE_E)
#define IOCTL_AUDIO_SET_AUDIO_LRCK_SEL_MODE         _IOW(IOC_MAGIC_AUDIO, IOC_NR_AUDIO_SET_AUDIO_LRCK_SEL_MODE, AUDIO_LRCK_MODE_E)

#define IOCTL_AUDIO_SET_AI_PUB_ATTR                 _IOW(IOC_MAGIC_AUDIO, IOC_NR_AUDIO_SET_AI_PUB_ATTR, AIO_ATTR_S)
#define IOCTL_AUDIO_GET_AI_PUB_ATTR                 _IOWR(IOC_MAGIC_AUDIO, IOC_NR_AUDIO_GET_AI_PUB_ATTR, AIO_ATTR_S)

#define IOCTL_AUDIO_SET_AO_PUB_ATTR                 _IOW(IOC_MAGIC_AUDIO, IOC_NR_AUDIO_SET_AO_PUB_ATTR, AIO_ATTR_S)
#define IOCTL_AUDIO_GET_AO_PUB_ATTR                 _IOWR(IOC_MAGIC_AUDIO, IOC_NR_AUDIO_GET_AO_PUB_ATTR, AIO_ATTR_S)
    
#define IOCTL_AUDIO_CREATE_AENC_CHN                 _IOW(IOC_MAGIC_AUDIO, IOC_NR_AUDIO_CREATE_AENC_CHN, CREATE_CHN_PARAM_S)
#define IOCTL_AUDIO_DESTROY_AENC_CHN                _IO(IOC_MAGIC_AUDIO, IOC_NR_AUDIO_DESTROY_AENC_CHN)
#define IOCTL_AUDIO_START_AENC_CHN                  _IO(IOC_MAGIC_AUDIO, IOC_NR_AUDIO_START_AENC_CHN)
#define IOCTL_AUDIO_STOP_AENC_CHN                   _IO(IOC_MAGIC_AUDIO, IOC_NR_AUDIO_STOP_AENC_CHN)
#define IOCTL_AUDIO_GET_STREAM                      _IOWR(IOC_MAGIC_AUDIO, IOC_NR_AUDIO_GET_STREAM, AUDIO_STREAM_IOCTL_S)
#define IOCTL_AUDIO_RELEASE_STREAM                  _IOWR(IOC_MAGIC_AUDIO, IOC_NR_AUDIO_RELEASE_STREAM, AUDIO_STREAM_S)

#define IOCTL_AUDIO_SET_AENC_NODE_NUM               _IOWR(IOC_MAGIC_AUDIO, IOC_NR_AUDIO_SET_AENC_NODE_NUM, NI_U32)
#define IOCTL_AUDIO_SET_ADEC_NODE_NUM               _IOWR(IOC_MAGIC_AUDIO, IOC_NR_AUDIO_SET_ADEC_NODE_NUM, NI_U32)

#define IOCTL_AUDIO_GET_AENC_NODE_NUM               _IOWR(IOC_MAGIC_AUDIO, IOC_NR_AUDIO_GET_AENC_NODE_NUM, NI_U32)
#define IOCTL_AUDIO_GET_ADEC_NODE_NUM               _IOWR(IOC_MAGIC_AUDIO, IOC_NR_AUDIO_GET_ADEC_NODE_NUM, NI_U32)

#define IOCTL_AUDIO_CREATE_ADEC_CHN                 _IOW(IOC_MAGIC_AUDIO, IOC_NR_AUDIO_CREATE_ADEC_CHN, CREATE_CHN_PARAM_S)
#define IOCTL_AUDIO_DESTROY_ADEC_CHN                _IO(IOC_MAGIC_AUDIO, IOC_NR_AUDIO_DESTROY_ADEC_CHN)
#define IOCTL_AUDIO_START_ADEC_CHN                  _IO(IOC_MAGIC_AUDIO, IOC_NR_AUDIO_START_ADEC_CHN)
#define IOCTL_AUDIO_STOP_ADEC_CHN                   _IO(IOC_MAGIC_AUDIO, IOC_NR_AUDIO_STOP_ADEC_CHN)
#define IOCTL_AUDIO_REGUEST_ADEC_BUF                _IOWR(IOC_MAGIC_AUDIO, IOC_NR_AUDIO_REGUEST_ADEC_BUF, AUDIO_STREAM_IOCTL_S)
#define IOCTL_AUDIO_SEND_ADEC_STREAM                _IOWR(IOC_MAGIC_AUDIO, IOC_NR_AUDIO_SEND_ADEC_STREAM, AUDIO_STREAM_S)
#define IOCTL_AUDIO_PAUSE_ADEC_CHN                  _IO(IOC_MAGIC_AUDIO, IOC_NR_AUDIO_PAUSE_ADEC_CHN)
#define IOCTL_AUDIO_GET_ADEC_CHN_NODE_INFO           _IOWR(IOC_MAGIC_AUDIO, IOC_NR_AUDIO_GET_ADEC_CHN_NODE_INFO, ADEC_NODE_INFO_S)
#define IOCTL_AUDIO_ADEC_CLEAR_CHN_NODE                   _IO(IOC_MAGIC_AUDIO, IOC_NR_AUDIO_ADEC_CLEAR_CHN_NODE)


#define IOCTL_FAAC_INIT                            _IO(IOC_MAGIC_AUDIO, IOC_NR_FAAC_INIT)
#define IOCTL_FAAC_DEINIT                          _IO(IOC_MAGIC_AUDIO, IOC_NR_FAAC_DEINIT)
#define IOCTL_FAAC_FFT                             _IOW(IOC_MAGIC_AUDIO, IOC_NR_FAAC_FFT, FAAC_FFT_ATTR_S)
#define IOCTL_FAAC_MDCT                            _IOW(IOC_MAGIC_AUDIO, IOC_NR_FAAC_MDCT, FAAC_MDCT_ATTR_S)
#define IOCTL_FAAC_ITER                            _IOW(IOC_MAGIC_AUDIO, IOC_NR_FAAC_ITER, FAAC_ITER_ATTR_S)
#define IOCTL_FAAC_MATH_OPERATION                  _IOW(IOC_MAGIC_AUDIO, IOC_NR_FAAC_MATH_OPERATION, FAAC_MATH_OPERATION_ATTR_S)

#define IOCTL_AEC_INIT                            _IO(IOC_MAGIC_AUDIO, IOC_NR_AEC_INIT)
#define IOCTL_AEC_DEINIT                          _IO(IOC_MAGIC_AUDIO, IOC_NR_AEC_DEINIT)
#define IOCTL_AEC_SET_MDF_PARAM                   _IOW(IOC_MAGIC_AUDIO, IOC_NR_AEC_SET_MDF_PARAM, AEC_MDF_PARAM_S)
#define IOCTL_AEC_RUN                             _IOW(IOC_MAGIC_AUDIO, IOC_NR_AEC_RUN, AEC_ATTR_S)
#define IOCTL_AEC_WAIT_EXE_DONE                   _IOW(IOC_MAGIC_AUDIO, IOC_NR_AEC_WAIT_EXE_DONE, AEC_OP_TYPE_E)
#define IOCTL_AUDIO_AENC_MAKE_DEV                   _IOW(IOC_MAGIC_AUDIO, IOC_NR_AUDIO_AENC_MAKE_DEV, NI_U32)
#define IOCTL_AUDIO_ADEC_MAKE_DEV                   _IOW(IOC_MAGIC_AUDIO, IOC_NR_AUDIO_ADEC_MAKE_DEV, NI_U32)

#define IOCTL_AUDIO_GET_ADEC_PLAYING_STREAM             _IOWR(IOC_MAGIC_AUDIO, IOC_NR_AUDIO_GET_ADEC_PLAYING_STREAM, AUDIO_STREAM_IOCTL_S)
#define IOCTL_AUDIO_PUT_ADEC_PLAYING_STREAM             _IOWR(IOC_MAGIC_AUDIO, IOC_NR_AUDIO_PUT_ADEC_PLAYING_STREAM, AUDIO_STREAM_S)
#define IOCTL_AUDIO_RELEASE_ADEC_PLAYING_STREAM         _IOWR(IOC_MAGIC_AUDIO, IOC_NR_AUDIO_RELEASE_ADEC_PLAYING_STREAM, AUDIO_STREAM_S)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef __CAL_AUDIO_H__ */


