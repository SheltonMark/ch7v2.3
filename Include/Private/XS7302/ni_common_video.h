/******************************************************************************

  Copyright (C), 2014-2020, ZheJiang Chipup Technology CO.,LTD.

 ******************************************************************************
  File Name      : ni_common_video.h
  Version          : Initial Draft
  Author           : Lv Zhuqing
  Created         : 2014.2.28
  Last Modified :
  Description    : 各模块媒相关的公共数据类型
  Function List  :
  DHstory        :
  1.Date          : 2014/2/28
    Author        : 24497
    Modification: Create
******************************************************************************/

#ifndef __NI_COMM_VIDEO_H__
#define __NI_COMM_VIDEO_H__

#include "ni_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


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


/*视频制式*/
typedef enum  tagVideoFormat
{
    VIDEO_FORMAT_720P_25FPS         = 0,
    VIDEO_FORMAT_720P_30FPS         = 1,
    VIDEO_FORMAT_720P_50FPS         = 2,
    VIDEO_FORMAT_720P_54FPS         = 3,
    VIDEO_FORMAT_720P_59FPS         = 4,
    VIDEO_FORMAT_720P_60FPS         = 5,
    VIDEO_FORMAT_1080P_25FPS        = 6,
    VIDEO_FORMAT_1080P_30FPS        = 7,
    VIDEO_FORMAT_1080P_50FPS        = 8,
    VIDEO_FORMAT_1080P_60FPS        = 9,
    VIDEO_FORMAT_480P_30FPS         = 10,
    VIDEO_FORMAT_480P_60FPS         = 11,
    VIDEO_FORMAT_576P_25FPS         = 12,
    VIDEO_FORMAT_576P_50FPS         = 13,
    VIDEO_FORMAT_960P_15FPS         = 14,
    VIDEO_FORMAT_960P_25FPS         = 15,
    VIDEO_FORMAT_960P_30FPS         = 16,
    VIDEO_FORMAT_2048x1536_25FPS    = 17,
    VIDEO_FORMAT_2048x1536_30FPS    = 18,
    VIDEO_FORMAT_2560x1440_25FPS    = 19,
    VIDEO_FORMAT_2560x1440_30FPS    = 20,
    VIDEO_FORMAT_3MP_20FPS          = 21,
    VIDEO_FORMAT_3MP_25FPS          = 22,
    VIDEO_FORMAT_3MP_30FPS          = 23,
    VIDEO_FORMAT_4K_25FPS           = 24,
    VIDEO_FORMAT_4K_30FPS           = 25,
    VIDEO_FORMAT_640x364_50FPS      = 26,
    VIDEO_FORMAT_640x356_60FPS      = 27,
    VIDEO_FORMAT_960H_25FPS         = 28,
    VIDEO_FORMAT_960H_30FPS         = 29,
	VIDEO_FORMAT_960H_50FPS         = 30,
    VIDEO_FORMAT_960H_60FPS         = 31,
	VIDEO_FORMAT_2304x1296_25FPS    = 32,
    VIDEO_FORMAT_2304x1296_30FPS    = 33,
    VIDEO_FORMAT_2688x1520_25FPS    = 34,
    VIDEO_FORMAT_2688x1520_30FPS    = 35,
	VIDEO_FORMAT_2688x1944_30FPS    = 36,
    VIDEO_FORMAT_2592x1944_20FPS    = 37,
    VIDEO_FORMAT_3072x1728_20FPS    = 38,
    VIDEO_FORMAT_3072x1728_25FPS    = 39,
    VIDEO_FORMAT_3072x2048_18FPS    = 40,
	VIDEO_FORMAT_3840x2160_12_5FPS  = 41,
	VIDEO_FORMAT_3840x2160_15FPS    = 42,
    VIDEO_FORMAT_3840x2160_25FPS    = 43,
    VIDEO_FORMAT_3840x2160_30FPS    = 44,

    VIDEO_FORMAT_BUTT
}VIDEO_FORMAT_E;

typedef enum tagVideoNorm
{
    VIDEO_ENCODING_MODE_PAL=0,
    VIDEO_ENCODING_MODE_NTSC,
    VIDEO_ENCODING_MODE_AUTO,
    
    VIDEO_ENCODING_MODE_BUTT
} VIDEO_NORM_E;


/* we ONLY define picture format used, all unused will be deleted!*/
typedef enum tagPixelFormat
{   
    PIXEL_FORMAT_RGB_1BPP = 0,
    PIXEL_FORMAT_RGB_2BPP,
    PIXEL_FORMAT_RGB_4BPP,
    PIXEL_FORMAT_RGB_8BPP,
    PIXEL_FORMAT_RGB_444,
    PIXEL_FORMAT_RGB_4444,
    PIXEL_FORMAT_RGB_555,
    PIXEL_FORMAT_RGB_565,
    PIXEL_FORMAT_RGB_1555, // 8

    PIXEL_FORMAT_RGB_888,
    PIXEL_FORMAT_RGB_8888,
    PIXEL_FORMAT_RGB_PLANAR_888,
    PIXEL_FORMAT_RGB_BAYER_8BPP,
    PIXEL_FORMAT_RGB_BAYER_10BPP,
    PIXEL_FORMAT_RGB_BAYER_12BPP,
    PIXEL_FORMAT_RGB_BAYER_14BPP,
    PIXEL_FORMAT_RGB_BAYER,         /* 16 bpp */

    PIXEL_FORMAT_YUV_A422,
    PIXEL_FORMAT_YUV_A444,

    PIXEL_FORMAT_YUV_PLANAR_422,
    PIXEL_FORMAT_YUV_PLANAR_420,//20
    PIXEL_FORMAT_YUV_PLANAR_444,

    PIXEL_FORMAT_YUV_SEMIPLANAR_422,
    PIXEL_FORMAT_YUV_SEMIPLANAR_420,
    PIXEL_FORMAT_YUV_SEMIPLANAR_420_MB4x4,   /* semi-planar YUV420 storaged by macro block 4x4 */
    PIXEL_FORMAT_YUV_SEMIPLANAR_420_MB16x16, /* semi-planar YUV420 storaged by macro block 16x16 */
    PIXEL_FORMAT_YUV_SEMIPLANAR_420_MB32x32, /* semi-planar YUV420 storaged by macro block 32x32 */
    PIXEL_FORMAT_YVU_SEMIPLANAR_420,
    PIXEL_FORMAT_YUV_SEMIPLANAR_444,

    PIXEL_FORMAT_UYVY_PACKAGE_422,
    PIXEL_FORMAT_YUYV_PACKAGE_422,
    PIXEL_FORMAT_VYUY_PACKAGE_422,

	PIXEL_FORMAT_SINGLE,
	
    PIXEL_FORMAT_BUTT   
} PIXEL_FORMAT_E;

/* The sequence for yuv */
typedef enum tagYuvSeq
{
    YUV_SEQ_UYVY = 0,
    YUV_SEQ_VYUY,
    YUV_SEQ_YUYV,
    YUV_SEQ_YVYU,

    YUV_SEQ_BUTT
}YUV_SEQ_E;

typedef enum tagVideoField
{
    VIDEO_FIELD_TOP         = 0x01,    /* even field */
    VIDEO_FIELD_BOTTOM      = 0x02,    /* odd field */
    VIDEO_FIELD_INTERLACED  = 0x03,    /* two interlaced fields */
    VIDEO_FIELD_FRAME       = 0x04,    /* frame */

    VIDEO_FIELD_BUTT
} VIDEO_FIELD_E;

typedef struct tagVideoFrame
{    
    NI_U32          u32Width;
    NI_U32          u32Height;    
    VIDEO_FIELD_E   u32Field;
    PIXEL_FORMAT_E  enPixelFormat;

    NI_U32          au32PhyAddr[3];
    NI_VOID        *apVirAddr[3];
    NI_U32          au32Stride[3];

    NI_U32          u32BlkId;

    NI_U16          u16OffsetTop;       /* top offset of show area */
    NI_U16          u16OffsetBottom;   /* bottom offset of show area */
    NI_U16          u16OffsetLeft;       /* left offset of show area */
    NI_U16          u16OffsetRight;    /* right offset of show area */

    NI_U64          u64Pts;
    NI_U32          u32TimeRef;

    NI_U32          u32PrivateData;
}VIDEO_FRAME_S;

typedef struct tagBitMap
{
    PIXEL_FORMAT_E enPixelFormat;  /* Bitmap's pixel format */

    NI_U32 u32Width;               /* Bitmap's width */
    NI_U32 u32Height;              /* Bitmap's height */
    NI_VOID *pData;                /* Address of Bitmap's data */
} BITMAP_S;

/* 定义RGB三分量的颜色值 */
typedef struct tagColorRgb
{
    NI_U32   u32Gr;           /* gr分量的颜色值 */
    NI_U32   u32R;            /* r分量的颜色值 */
    NI_U32   u32B;            /* b分量的颜色值 */
    NI_U32   u32Gb;           /* gb分量的颜色值 */
} COLOR_RGB_S;

typedef struct tagColorYuv
{ 
    NI_U8 u8Y;    /* Y分量值 */
    NI_U8 u8U;    /* U分量值 */
    NI_U8 u8V;    /* V分量值 */
}COLOR_YUV_S;

/* 定义RGB三分量的权重,浮点型 */
typedef struct tagRgbWeight
{
    NI_F32   f32Gr;           /* gr分量的颜色值 */
    NI_F32   f32R;            /* r分量的颜色值 */
    NI_F32   f32B;            /* b分量的颜色值 */
    NI_F32   f32Gb;           /* gb分量的颜色值 */
} RGB_WEIGHT_S;

/* 定义宽动态模式 */
typedef enum tagHdrMode
{
    HDR_MODE_NONE = 0,                 /* 线性模式 */
    HDR_MODE_BUILT_IN,                 /* sensor端合成HDR模式 */
        
    HDR_MODE_2To1_FRAME,               /* 2帧合成1帧HDR模式,按帧合成 */
    HDR_MODE_3To1_FRAME,               /* 3帧合成1帧HDR模式,按帧合成 */

    HDR_MODE_2To1_LINE,                /* 2帧合成1帧HDR模式,按行合成 */
    HDR_MODE_3To1_LINE,                /* 3帧合成1帧HDR模式,按行合成 */
    
    HDR_MODE_BUTT
} HDR_MODE_E;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* _NI_COMM_VIDEO_H_ */ 

