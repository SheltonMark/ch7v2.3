/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef __NI_COMMON_UVC_H__
#define __NI_COMMON_UVC_H__

/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "ni_type.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct tagUvcAudioConfig {
    NI_U32 u32Card;
    NI_U32 u32Device;

    NI_U32 u32Channels;
    NI_U32 u32Rate;
    NI_U32 u32Bits;
    NI_U32 u32PeriodSize;
    NI_U32 u32PeriodCount;

    NI_S32(*FillBuf)
    (NI_CHAR *pBuf, NI_U32 u32Len, NI_VOID *pData);
} UVC_AUDIO_CONFIG;

typedef enum tagUvcPixFormat {
    UVC_PIX_FMT_MJPEG,
    UVC_PIX_FMT_H264,
    UVC_PIX_FMT_NV12,
    UVC_PIX_FMT_YUYV,
    UVC_PIX_FMT_MAX,
} UVC_VIDEO_PIX_FMT;

typedef struct tagUvcVideoFrameInfo {
    NI_U32 u32Width;
    NI_U32 u32Height;
} UVC_VIDEO_FRAME_INFO;

typedef struct tagUvcVideoFormatInfo {
    UVC_VIDEO_PIX_FMT           enFcc;
    const UVC_VIDEO_FRAME_INFO *pstFrames;
} UVC_VIDEO_FORMAT_INFO;

typedef struct tagUvcVideoBuf {
    NI_CHAR *pBuf;
    NI_U32   u32BufLen;
    NI_VOID *pFrame;
} UVC_VIDEO_BUF;

typedef struct tagUvcVideoOperations {
    NI_U32(*SetBufSize)
    (UVC_VIDEO_PIX_FMT enFcc, NI_U32 u32Width, NI_U32 u32Height,
     NI_VOID *pData);
    NI_S32(*StreamOn)
    (UVC_VIDEO_PIX_FMT enFcc, NI_U32 u32Width, NI_U32 u32Height,
     NI_VOID *pData);
    NI_S32(*FillBufMmap)
    (UVC_VIDEO_PIX_FMT enFcc, UVC_VIDEO_BUF *pstBuf, NI_VOID *pData);
    NI_S32(*FillBufUserptr)
    (UVC_VIDEO_PIX_FMT enFcc, UVC_VIDEO_BUF *pstBuf, NI_VOID *pData);
    NI_VOID(*StreamOff)
    (UVC_VIDEO_PIX_FMT enFcc, NI_VOID *pData);
} UVC_VIDEO_OPERATIONS;

enum ioMethod {
    IO_METHOD_MMAP,
    IO_METHOD_USERPTR,
};

typedef struct tagUvcVideoConfig {
    NI_CHAR                     *pDev;
    const UVC_VIDEO_FORMAT_INFO *pstFormats;
    NI_S32                       s32FormatCount;
    const UVC_VIDEO_OPERATIONS  *pstOps;
    NI_U32                       u32BufCount;
    enum ioMethod                io;
} UVC_VIDEO_CONFIG;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NI_COMMON_UVC_H__ */
