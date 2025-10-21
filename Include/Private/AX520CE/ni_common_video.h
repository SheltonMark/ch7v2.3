/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef __NI_COMM_VIDEO_H__
#define __NI_COMM_VIDEO_H__

#include "ni_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/*视频制式*/
typedef enum tagVideoFormat {
    VIDEO_FORMAT_720P_25FPS        = 0,
    VIDEO_FORMAT_720P_30FPS        = 1,
    VIDEO_FORMAT_720P_50FPS        = 2,
    VIDEO_FORMAT_720P_54FPS        = 3,
    VIDEO_FORMAT_720P_59FPS        = 4,
    VIDEO_FORMAT_720P_60FPS        = 5,
    VIDEO_FORMAT_1080P_25FPS       = 6,
    VIDEO_FORMAT_1080P_30FPS       = 7,
    VIDEO_FORMAT_1080P_50FPS       = 8,
    VIDEO_FORMAT_1080P_60FPS       = 9,
    VIDEO_FORMAT_480P_30FPS        = 10,
    VIDEO_FORMAT_480P_60FPS        = 11,
    VIDEO_FORMAT_576P_25FPS        = 12,
    VIDEO_FORMAT_576P_50FPS        = 13,
    VIDEO_FORMAT_960P_15FPS        = 14,
    VIDEO_FORMAT_960P_25FPS        = 15,
    VIDEO_FORMAT_960P_30FPS        = 16,
    VIDEO_FORMAT_2048x1536_25FPS   = 17,
    VIDEO_FORMAT_2048x1536_30FPS   = 18,
    VIDEO_FORMAT_2560x1440_25FPS   = 19,
    VIDEO_FORMAT_2560x1440_30FPS   = 20,
    VIDEO_FORMAT_3MP_20FPS         = 21,
    VIDEO_FORMAT_3MP_25FPS         = 22,
    VIDEO_FORMAT_3MP_30FPS         = 23,
    VIDEO_FORMAT_4K_25FPS          = 24,
    VIDEO_FORMAT_4K_30FPS          = 25,
    VIDEO_FORMAT_640x364_50FPS     = 26,
    VIDEO_FORMAT_640x356_60FPS     = 27,
    VIDEO_FORMAT_960H_25FPS        = 28,
    VIDEO_FORMAT_960H_30FPS        = 29,
    VIDEO_FORMAT_960H_50FPS        = 30,
    VIDEO_FORMAT_960H_60FPS        = 31,
    VIDEO_FORMAT_2304x1296_25FPS   = 32,
    VIDEO_FORMAT_2304x1296_30FPS   = 33,
    VIDEO_FORMAT_2688x1520_25FPS   = 34,
    VIDEO_FORMAT_2688x1520_30FPS   = 35,
    VIDEO_FORMAT_2688x1944_30FPS   = 36,
    VIDEO_FORMAT_2592x1944_20FPS   = 37,
    VIDEO_FORMAT_3072x1728_20FPS   = 38,
    VIDEO_FORMAT_3072x1728_25FPS   = 39,
    VIDEO_FORMAT_3072x2048_18FPS   = 40,
    VIDEO_FORMAT_3840x2160_12_5FPS = 41,
    VIDEO_FORMAT_3840x2160_15FPS   = 42,
    VIDEO_FORMAT_3840x2160_25FPS   = 43,
    VIDEO_FORMAT_3840x2160_30FPS   = 44,

    VIDEO_FORMAT_BUTT
} VIDEO_FORMAT_E;


typedef enum tagPicRes
{
    PIC_QCIF = 0,
    PIC_CIF,
    PIC_HD1,
    PIC_D1,
    PIC_960H,
    
    PIC_QVGA,    /* 320 * 240 */
    PIC_VGA,     /* 640 * 480 */    
    PIC_XGA,     /* 1024 * 768 */   
    PIC_SXGA,    /* 1400 * 1050 */    
    PIC_UXGA,    /* 1600 * 1200 */    
    PIC_QXGA,    /* 2048 * 1536 */

    PIC_WVGA,    /* 854 * 480 */
    PIC_WSXGA,   /* 1680 * 1050 */      
    PIC_WUXGA,   /* 1920 * 1200 */
    PIC_WQXGA,   /* 2560 * 1600 */
    
    PIC_HD720,   /* 1280 * 720 */
    PIC_HD1080,  /* 1920 * 1080 */

    PIC_500W,    /* 2560*2048 */
    PIC_800W,    /* 3200*2400 */

    PIC_ITU4K,   /* 3840*2160 */
    PIC_DCI4K,   /* 4096*2160 */    
    
    PIC_BUTT
}PIC_RES_E;

/* define picture format */
typedef enum tagPixelFormat {
    PIXEL_FORMAT_RGB_1BPP = 0,
    PIXEL_FORMAT_RGB_2BPP = 1,
    PIXEL_FORMAT_RGB_4BPP = 2,
    PIXEL_FORMAT_RGB_8BPP = 3,
    PIXEL_FORMAT_RGB_444  = 4,
    PIXEL_FORMAT_RGB_4444 = 5,
    PIXEL_FORMAT_RGB_555  = 6,
    PIXEL_FORMAT_RGB_565  = 7,
    PIXEL_FORMAT_RGB_1555 = 8,

    PIXEL_FORMAT_RGB_888         = 9,
    PIXEL_FORMAT_RGB_8888        = 10,
    PIXEL_FORMAT_RGB_PLANAR_888  = 11,
    PIXEL_FORMAT_RGB_BAYER_8BPP  = 12,
    PIXEL_FORMAT_RGB_BAYER_10BPP = 13,
    PIXEL_FORMAT_RGB_BAYER_12BPP = 14,
    PIXEL_FORMAT_RGB_BAYER_14BPP = 15,
    PIXEL_FORMAT_RGB_BAYER       = 16,

    PIXEL_FORMAT_YUV_A422 = 17,
    PIXEL_FORMAT_YUV_A444 = 18,

    PIXEL_FORMAT_YUV_PLANAR_422 = 19,
    PIXEL_FORMAT_YUV_PLANAR_420 = 20,
    PIXEL_FORMAT_YUV_PLANAR_444 = 21,

    PIXEL_FORMAT_YUV_SEMIPLANAR_422         = 22,
    PIXEL_FORMAT_YUV_SEMIPLANAR_420         = 23,
    PIXEL_FORMAT_YUV_SEMIPLANAR_420_MB4x4   = 24, /* semi-planar YUV420 storaged by macro block 4x4 */
    PIXEL_FORMAT_YUV_SEMIPLANAR_420_MB16x16 = 25, /* semi-planar YUV420 storaged by macro block 16x16 */
    PIXEL_FORMAT_YUV_SEMIPLANAR_420_MB32x32 = 26, /* semi-planar YUV420 storaged by macro block 32x32 */
    PIXEL_FORMAT_YVU_SEMIPLANAR_420         = 27,
    PIXEL_FORMAT_YUV_SEMIPLANAR_444         = 28,

    PIXEL_FORMAT_UYVY_PACKAGE_422 = 29,
    PIXEL_FORMAT_YUYV_PACKAGE_422 = 30,
    PIXEL_FORMAT_VYUY_PACKAGE_422 = 31,

    PIXEL_FORMAT_SINGLE = 32,

    PIXEL_FORMAT_BUTT
} PIXEL_FORMAT_E;


typedef enum tagVideoNorm
{
    VIDEO_ENCODING_MODE_PAL=0,
    VIDEO_ENCODING_MODE_NTSC,
    VIDEO_ENCODING_MODE_AUTO,
    
    VIDEO_ENCODING_MODE_BUTT
} VIDEO_NORM_E;

/* The sequence for yuv */
typedef enum tagYuvSeq {
    YUV_SEQ_UYVY = 0,
    YUV_SEQ_VYUY = 1,
    YUV_SEQ_YUYV = 2,
    YUV_SEQ_YVYU = 3,

    YUV_SEQ_BUTT
} YUV_SEQ_E;

typedef enum tagVideoField {
    VIDEO_FIELD_FRAME      = 0, /* whole frame */
    VIDEO_FIELD_TOP        = 1, /* even field of one frame */
    VIDEO_FIELD_BOTTOM     = 2, /* odd field of one frame */
    VIDEO_FIELD_INTERLACED = 3, /* two interlaced fields of one frame */

    VIDEO_FIELD_BUTT
} VIDEO_FIELD_E;

typedef struct tagVideoFrame {
    NI_U32         u32Width;      /* the video width */
    NI_U32         u32Height;     /* the video height */
    VIDEO_FIELD_E  enField;       /* the video field type */
    PIXEL_FORMAT_E enPixelFormat; /* the pixels saved format in memory */

    NI_PHYS_ADDR_T aphysAddr[3];  /* video frame physical address */
    NI_VOID       *apVirAddr[3];  /* video frame virtual address */
    NI_U32         au32Stride[3]; /* the stride of one line pixels in memory */

    NI_U32 u32BlkId; /* mbuf block id */

    NI_U16 u16OffsetTop;    /* top offset of show area */
    NI_U16 u16OffsetBottom; /* bottom offset of show area */
    NI_U16 u16OffsetLeft;   /* left offset of show area */
    NI_U16 u16OffsetRight;  /* right offset of show area */

    NI_U64 u64Pts;     /* picture time stamp */
    NI_U32 u32TimeRef; /* picture time reference sequence */

    NI_U32 u32PrivateData; /* private data */
} VIDEO_FRAME_S;

typedef struct tagBitMap {
    PIXEL_FORMAT_E enPixelFormat; /* Bitmap's pixel format */

    NI_U32   u32Width;  /* Bitmap's width */
    NI_U32   u32Height; /* Bitmap's height */
    NI_VOID *pData;     /* Address of Bitmap's data */
} BITMAP_S;

/* 定义RGB三分量的颜色值 */
typedef struct tagColorRgb {
    NI_U32 u32Gr; /* gr分量的颜色值 */
    NI_U32 u32R;  /* r分量的颜色值 */
    NI_U32 u32B;  /* b分量的颜色值 */
    NI_U32 u32Gb; /* gb分量的颜色值 */
} COLOR_RGB_S;

typedef struct tagColorYuv {
    NI_U8 u8Y; /* Y分量值 */
    NI_U8 u8U; /* U分量值 */
    NI_U8 u8V; /* V分量值 */
} COLOR_YUV_S;

/* 定义BAYER格式的RGB顺序头 */
typedef enum tagBayerSeq {
    BAYER_SEQ_GBRG = 0, /* 表示BAYER格式数据以GB开头 */
    BAYER_SEQ_RGGB = 1, /* 表示BAYER格式数据以RG开头 */
    BAYER_SEQ_BGGR = 2, /* 表示BAYER格式数据以BG开头 */
    BAYER_SEQ_GRBG = 3, /* 表示BAYER格式数据以GR开头 */

    BAYER_SEQ_BUTT
} BAYER_SEQ_E;

/* 定义宽动态模式 */
typedef enum tagHdrMode {
    HDR_MODE_NONE = 0, /* 线性模式 */
    HDR_MODE_BUILT_IN, /* sensor端合成HDR模式 */

    HDR_MODE_2To1_FRAME, /* 2帧合成1帧HDR模式,按帧合成 */
    HDR_MODE_3To1_FRAME, /* 3帧合成1帧HDR模式,按帧合成 */

    HDR_MODE_2To1_LINE, /* 2帧合成1帧HDR模式,按行合成 */
    HDR_MODE_3To1_LINE, /* 3帧合成1帧HDR模式,按行合成 */

    HDR_MODE_BUTT
} HDR_MODE_E;

/* 定义旋转类型枚举 */
typedef enum tagRotateType {
    ROTATE_BYPASS    = 0, /* BYPASS */
    ROTATE_MIRROR    = 1, /* 水平镜像 */
    ROTATE_FLIP      = 2, /* 垂直翻转 */
    ROTATE_90        = 3, /* 顺时针90度旋转 */
    ROTATE_180       = 4, /* 顺时针180度旋转 */
    ROTATE_270       = 5, /* 顺时针270度旋转 */
    ROTATE_MIRROR_90 = 6, /* 水平镜像+顺时针90度旋转 */
    ROTATE_FLIP_90   = 7, /* 垂直翻转+顺时针90度旋转 */

    ROTATE_BUTT
} ROTATE_TYPE_E;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* _NI_COMM_VIDEO_H_ */
