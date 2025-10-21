/* 
 * ni_comm_vo.h -- vo模块对外提供的头文件
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */

#ifndef _NI_COMM_VO_H_
#define _NI_COMM_VO_H_


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "ni_errno.h"
#include "ni_common_video.h"
#include "ni_common.h"


#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */


typedef enum tagVoErrCode
{    
    ERR_VO_CHN_NOT_CONFIG      = 100,
    ERR_VO_BT_INTF_CONFIG      = 101,
    ERR_VO_LCD_INTF_CONFIG     = 102,
    ERR_VO_MIPI_INTF_CONFIG    = 103,
    ERR_VO_FUNC_NOT_SUPPORT    = 104,
    ERR_VO_BUTT
} VO_ERR_CODE_E;


/* invlalid device ID */
#define NI_ERR_VO_INVALID_DEVID     NI_DEF_ERR(NI_ID_VO, EN_ERR_LEVEL_ERROR, ERR_INVALID_DEVID)
#define NI_ERR_VO_INVALID_CHNID     NI_DEF_ERR(NI_ID_VO, EN_ERR_LEVEL_ERROR, ERR_INVALID_CHNID)
#define NI_ERR_VO_NOT_INIT          NI_DEF_ERR(NI_ID_VO, EN_ERR_LEVEL_ERROR, ERR_UNEXIST)
/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define NI_ERR_VO_ILLEGAL_PARAM     NI_DEF_ERR(NI_ID_VO, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)
/* using a NULL point */
#define NI_ERR_VO_NULL_PTR          NI_DEF_ERR(NI_ID_VO, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)
/* try to enable or initialize system,device or channel, before configing attribute */
#define NI_ERR_VO_NOT_CONFIG        NI_DEF_ERR(NI_ID_VO, EN_ERR_LEVEL_ERROR, ERR_NOT_CONFIG)
/* operation is not supported by NOW */
#define NI_ERR_VO_NOT_SUPPORT       NI_DEF_ERR(NI_ID_VO, EN_ERR_LEVEL_ERROR, ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change static attribute */
#define NI_ERR_VO_NOT_PERM          NI_DEF_ERR(NI_ID_VO, EN_ERR_LEVEL_ERROR, ERR_NOT_PERM)
/* failure caused by malloc memory */
#define NI_ERR_VO_NOMEM             NI_DEF_ERR(NI_ID_VO, EN_ERR_LEVEL_ERROR, ERR_NOMEM)
/* failure caused by malloc buffer */
#define NI_ERR_VO_NOBUF             NI_DEF_ERR(NI_ID_VO, EN_ERR_LEVEL_ERROR, ERR_NOBUF)
/* no data in buffer */
#define NI_ERR_VO_BUF_EMPTY         NI_DEF_ERR(NI_ID_VO, EN_ERR_LEVEL_ERROR, ERR_BUF_EMPTY)
/* no buffer for new data */
#define NI_ERR_VO_BUF_FULL          NI_DEF_ERR(NI_ID_VO, EN_ERR_LEVEL_ERROR, ERR_BUF_FULL)
/* failure caused by request irq */
#define NI_ERR_VO_REQUEST_IRQFAIL   NI_DEF_ERR(NI_ID_VO, EN_ERR_LEVEL_ERROR, ERR_REQ_IRQ_FAIL)
/* failure caused by VO not enable */
#define NI_ERR_VO_NOT_ENABLED       NI_DEF_ERR(NI_ID_VO, EN_ERR_LEVEL_ERROR, ERR_NOT_ENABLE)
/* failure caused by VO not disable */
#define NI_ERR_VO_NOT_DISABLE       NI_DEF_ERR(NI_ID_VO, EN_ERR_LEVEL_ERROR, ERR_NOT_DISABLE)
/* failure caused by VO func not support */
#define NI_ERR_VO_FUNC_NOT_SUPPORT  NI_DEF_ERR(NI_ID_VO, EN_ERR_LEVEL_ERROR, ERR_VO_FUNC_NOT_SUPPORT)
/* vo bt1120 interface not config */
#define NI_ERR_VO_BT_INTF_CONFIG   NI_DEF_ERR(NI_ID_VO, EN_ERR_LEVEL_ERROR, ERR_VO_BT_INTF_CONFIG)
/* vo lcd interface not config */
#define NI_ERR_VO_LCD_INTF_CONFIG  NI_DEF_ERR(NI_ID_VO, EN_ERR_LEVEL_ERROR, ERR_VO_LCD_INTF_CONFIG)
/* vo mipi interface not config */
#define NI_ERR_VO_MIPI_INTF_CONFIG  NI_DEF_ERR(NI_ID_VO, EN_ERR_LEVEL_ERROR, ERR_VO_MIPI_INTF_CONFIG)

/* failure caused by timeout */
#define NI_ERR_VO_SEND_FRMAE_TIMEOUT  NI_DEF_ERR(NI_ID_VO, EN_ERR_LEVEL_ERROR, ERR_TIME_OUT)


/* 输出接口类型 */
typedef enum tagVoIntfType
{
    VO_INTF_BT1120  = (0x01L<<0),
    VO_INTF_BT656   = (0x01L<<1),
    VO_INTF_LCD     = (0x01L<<2),
    VO_INTF_MIPI    = (0x01L<<3),
    VO_INTF_CVBS    = (0x01L<<4),
    VO_INTF_VGA     = (0x01L<<5),
    VO_INTF_HDMI    = (0x01L<<6),
    
}VO_INTF_TYPE_E;


typedef enum tagVoIntfSync
{
    VO_SYNC_PAL = 0,
    VO_SYNC_NTSC,

    VO_SYNC_720P24,
    VO_SYNC_720P25,
    VO_SYNC_720P30,
    VO_SYNC_720P50,
    VO_SYNC_720P60,

    VO_SYNC_1080P24,
    VO_SYNC_1080P25,
    VO_SYNC_1080P30,
    VO_SYNC_1080P50,
    VO_SYNC_1080P60,
    
    VO_SYNC_1080I50,
    VO_SYNC_1080I60,

    VO_SYNC_576P25,
    VO_SYNC_576P30,
    VO_SYNC_576P50,
    VO_SYNC_576p60,
    
    VO_SYNC_480P60,

    VO_SYNC_800x480_P25,
    VO_SYNC_800x480_P30,
    VO_SYNC_800x480_P50,
    VO_SYNC_800x480_P60,

    VO_SYNC_800x1280_P25,
    VO_SYNC_800x1280_P30,
    VO_SYNC_800x1280_P50,
    VO_SYNC_800x1280_P60,
    
    VO_SYNC_1024x600_P25,
    VO_SYNC_1024x600_P30,
    VO_SYNC_1024x600_P50,
    VO_SYNC_1024x600_P60,

    VO_SYNC_800x600_60,            /* VESA 800 x 600 at 60 Hz (non-interlaced) */
    VO_SYNC_1024x768_60,           /* VESA 1024 x 768 at 60 Hz (non-interlaced) */
    VO_SYNC_1280x1024_60,          /* VESA 1280 x 1024 at 60 Hz (non-interlaced) */
    VO_SYNC_1366x768_60,           /* VESA 1366 x 768 at 60 Hz (non-interlaced) */
    VO_SYNC_1440x900_60,           /* VESA 1440 x 900 at 60 Hz (non-interlaced) CVT Compliant */
    VO_SYNC_1280x800_60,           /* 1280*800@60Hz VGA@60Hz*/
    VO_SYNC_1600x1200_60,          /* VESA 1600 x 1200 at 60 Hz (non-interlaced) */
    VO_SYNC_1680x1050_60,          /* VESA 1680 x 1050 at 60 Hz (non-interlaced) */
    VO_SYNC_1920x1200_60,          /* VESA 1920 x 1600 at 60 Hz (non-interlaced) CVT (Reduced Blanking)*/
    VO_SYNC_640x480_60,            /* VESA 640 x 480 at 60 Hz (non-interlaced) CVT */
    VO_SYNC_960H_PAL,              /* ITU-R BT.1302 960 x 576 at 50 Hz (interlaced)*/
    VO_SYNC_960H_NTSC,             /* ITU-R BT.1302 960 x 480 at 60 Hz (interlaced)*/

    VO_SYNC_4K_12_5,               /* ITU-R 3840 x 2160 at 12.5 Hz */
    VO_SYNC_4K_15,                 /* ITU-R 3840 x 2160 at 15 Hz */
    VO_SYNC_2560x1440_25,
    VO_SYNC_2560x1440_30,
    VO_SYNC_2048x1536_30,
    VO_SYNC_3072x1728_20,
    
    VO_SYNC_2304x1296_25,
    VO_SYNC_2048x1152_30,
    VO_SYNC_2560x1920_20,
    VO_SYNC_2880x1920_20,
    VO_SYNC_3840x2160_12_5,
    VO_SYNC_3840x2160_15,
    VO_SYNC_2048x1536_25,
    VO_SYNC_3072x1728_25,
    
    VO_SYNC_USER,
    
    VO_SYNC_BUTT
} VO_INTF_SYNC_E;

/* 定义CVBS 视频制式枚举 */
typedef enum tagVoCvbsFormat
{
    VO_CVBS_FORMAT_NTSC_M      = 0,    /* 3.579545 MHz, U.S.，many others */
    VO_CVBS_FORMAT_NTSC_J      = 1,    /* 3.579545 MHz, Japan */
    VO_CVBS_FORMAT_NTSC_4_43   = 2,    /* 4.433620 MHz, Transcoding */
    VO_CVBS_FORMAT_PAL         = 3,    /* 4.433619 MHz, Many,include China,PAL-B/G/H/I/D/K */
    VO_CVBS_FORMAT_PAL_M       = 4,    /* 3.575612 MHz, Brazil */
    VO_CVBS_FORMAT_PAL_N       = 5,    /* 3.582056 MHz, Argentina,Paraguay,Uruguay */
    VO_CVBS_FORMAT_PAL_CN      = 6,    /* 4.433619 MHz */
    VO_CVBS_FORMAT_PAL_60      = 7,    /* 4.433619 MHz, Many 1990s-onwards VCR player sold in Europe */
    
    VO_CVBS_FORMAT_BUTT
}VO_CVBS_FORMAT_E;


/* rotate type  */
typedef enum tagVoRotateType
{
    VO_ROTATE_BYPASS      = 0, //BYPASS
    VO_ROTATE_MIRROR      = 1, //水平镜像
    VO_ROTATE_FLIP        = 2, //垂直翻转
    VO_ROTATE_MIRROR_FLIP = 3, //水平翻转+垂直翻转

    VO_ROTATE_BUTT
}VO_ROTATE_TYPE_E;


/* 定义视频输出格式枚举 */
typedef enum tagVoPixelFormat
{
    VO_OUT_FORMAT_RGB888 = 0, /* [0:7], [8:15], [16:23]*/
    VO_OUT_FORMAT_BRG888,
    VO_OUT_FORMAT_GBR888,
    VO_OUT_FORMAT_BGR888,
    VO_OUT_FORMAT_RBG888,
    VO_OUT_FORMAT_GRB888,
    VO_OUT_FORMAT_RGB666_BL,/* [0:5], [8:13], [16:21]*/ /* B表示一个BYTE，L表示低位，如VO_OUT_FORMAT_RGB666_BL表示0~5bit，8~13bit，16~21bit有效，其他以此类推 */
    VO_OUT_FORMAT_BRG666_BL,
    VO_OUT_FORMAT_GBR666_BL,
    VO_OUT_FORMAT_BGR666_BL,
    VO_OUT_FORMAT_RBG666_BL,
    VO_OUT_FORMAT_GRB666_BL,
    VO_OUT_FORMAT_RGB666_CH,/* [6:11], [12:17],[18:23]*/ /* C表示连续比特有效，H表示高位，如VO_OUT_FORMAT_RGB666_CH表示6~23bit有效 其他以此类推 */
    VO_OUT_FORMAT_BRG666_CH,
    VO_OUT_FORMAT_GBR666_CH,
    VO_OUT_FORMAT_BGR666_CH,
    VO_OUT_FORMAT_RBG666_CH,
    VO_OUT_FORMAT_GRB666_CH,
    VO_OUT_FORMAT_RGB666_CL,/* [0:5], [6:11], [12:17]*/
    VO_OUT_FORMAT_BRG666_CL,
    VO_OUT_FORMAT_GBR666_CL,
    VO_OUT_FORMAT_BGR666_CL,
    VO_OUT_FORMAT_RBG666_CL,
    VO_OUT_FORMAT_GRB666_CL,
    VO_OUT_FORMAT_BGR565_BL,/* [0:4], [8:13], [16:20]*/
    VO_OUT_FORMAT_RGB565_BL,
    VO_OUT_FORMAT_BGR565_CH,/* [8:12], [13:18], [19:23]*/
    VO_OUT_FORMAT_RGB565_CH,
    VO_OUT_FORMAT_BGR565_CL,/* [0:4], [5:10], [11:15]*/
    VO_OUT_FORMAT_RGB565_CL,
    VO_OUT_FORMAT_BGR444_CH,/* [12:15], [16:19], [20:23]*/
    VO_OUT_FORMAT_BGR444_CL,/* [0:3], [4:7], [8:11]*/

    VO_OUT_FORMAT_YUV422_YCRCB_CL,  /* [0:7],[8:15] */
    VO_OUT_FORMAT_YUV422_CRCBY_CL,  /* [0:7],[8:15] */
    VO_OUT_FORMAT_YUV422_YCRCB_CH,  /* [8:15],[16:23] */
    VO_OUT_FORMAT_YUV422_CRCBY_CH,  /* [8:15],[16:23] */
    VO_OUT_FORMAT_YUV422_YCRCB_LH,  /* [0:7],[16:23] */
    VO_OUT_FORMAT_YUV422_CRCBY_LH,  /* [0:7],[16:23] */
    VO_OUT_FORMAT_YUV422_YCBCR_CL,  /* [0:7],[8:15] */
    VO_OUT_FORMAT_YUV422_CBCRY_CL,  /* [0:7],[8:15] */
    VO_OUT_FORMAT_YUV422_YCBCR_CH,  /* [8:15],[16:23] */
    VO_OUT_FORMAT_YUV422_CBCRY_CH,  /* [8:15],[16:23] */
    VO_OUT_FORMAT_YUV422_YCBCR_LH,  /* [0:7],[16:23] */
    VO_OUT_FORMAT_YUV422_CBCRY_LH,  /* [0:7],[16:23] */

    VO_OUT_FORMAT_YUV444_YCBCR, /* [0:7], [8:15], [16:23]*/
    VO_OUT_FORMAT_YUV444_CRYCB,
    VO_OUT_FORMAT_YUV444_CBCRY,
    VO_OUT_FORMAT_YUV444_CRCBY,
    VO_OUT_FORMAT_YUV444_YCRCB,
    VO_OUT_FORMAT_YUV444_CBYCR,

    /*为串行输出格式 */
    VO_OUT_FORMAT_SERIAL_RGB888_H,  /* [23:16] */
    VO_OUT_FORMAT_SERIAL_RGB888_M,  /* [15:8] */
    VO_OUT_FORMAT_SERIAL_RGB888_L,  /* [7:0] */
    VO_OUT_FORMAT_SERIAL_RBG888_H,  /* [23:16] */
    VO_OUT_FORMAT_SERIAL_RBG888_M,  /* [15:8] */
    VO_OUT_FORMAT_SERIAL_RBG888_L,  /* [7:0] */
    VO_OUT_FORMAT_SERIAL_GBR888_H,  /* [23:16] */
    VO_OUT_FORMAT_SERIAL_GBR888_M,  /* [15:8] */
    VO_OUT_FORMAT_SERIAL_GBR888_L,  /* [7:0] */
    VO_OUT_FORMAT_SERIAL_BGR888_H,  /* [23:16] */
    VO_OUT_FORMAT_SERIAL_BGR888_M,  /* [15:8] */
    VO_OUT_FORMAT_SERIAL_BGR888_L,  /* [7:0] */
    VO_OUT_FORMAT_SERIAL_GRB888_H,  /* [23:16] */
    VO_OUT_FORMAT_SERIAL_GRB888_M,  /* [15:8] */
    VO_OUT_FORMAT_SERIAL_GRB888_L,  /* [7:0] */
    VO_OUT_FORMAT_SERIAL_BRG888_H,  /* [23:16] */
    VO_OUT_FORMAT_SERIAL_BRG888_M,  /* [15:8] */
    VO_OUT_FORMAT_SERIAL_BRG888_L,  /* [7:0] */
    VO_OUT_FORMAT_SERIAL_RGB666_H,  /* [21:16] */
    VO_OUT_FORMAT_SERIAL_RGB666_M,  /* [13:8] */
    VO_OUT_FORMAT_SERIAL_RGB666_L,  /* [5:0] */
    VO_OUT_FORMAT_SERIAL_BGR666_H,  /* [21:16] */
    VO_OUT_FORMAT_SERIAL_BGR666_M,  /* [13:8] */
    VO_OUT_FORMAT_SERIAL_BGR666_L,  /* [5:0] */
    VO_OUT_FORMAT_SERIAL_RGB565_H,  /* [21:16] */
    VO_OUT_FORMAT_SERIAL_RGB565_M,  /* [13:8] */
    VO_OUT_FORMAT_SERIAL_RGB565_L,  /* [5:0] */
    VO_OUT_FORMAT_SERIAL_BGR565_H,  /* [21:16] */
    VO_OUT_FORMAT_SERIAL_BGR565_M,  /* [13:8] */
    VO_OUT_FORMAT_SERIAL_BGR565_L,  /* [5:0] */
    VO_OUT_FORMAT_SERIAL_RGB444_L,  /* [3:0] */
    VO_OUT_FORMAT_SERIAL_BGR444_L,  /* [3:0] */

    VO_OUT_FORMAT_SERIAL_YCRYCB_H, /* [23:16] */
    VO_OUT_FORMAT_SERIAL_YCRYCB_M, /* [15:8] */
    VO_OUT_FORMAT_SERIAL_YCRYCB_L, /* [7:0] */
    VO_OUT_FORMAT_SERIAL_CRYCBY_H, /* [23:16] */
    VO_OUT_FORMAT_SERIAL_CRYCBY_M, /* [15:8] */
    VO_OUT_FORMAT_SERIAL_CRYCBY_L, /* [7:0] */
    VO_OUT_FORMAT_SERIAL_YCBYCR_H, /* [23:16] */
    VO_OUT_FORMAT_SERIAL_YCBYCR_M, /* [15:8] */
    VO_OUT_FORMAT_SERIAL_YCBYCR_L, /* [7:0] */
    VO_OUT_FORMAT_SERIAL_CBYCRY_H, /* [23:16] */
    VO_OUT_FORMAT_SERIAL_CBYCRY_M, /* [15:8] */
    VO_OUT_FORMAT_SERIAL_CBYCRY_L, /* [7:0] */

    VO_OUT_FORMAT_SERIAL_YCBCR_H, /* [23:16] */
    VO_OUT_FORMAT_SERIAL_YCBCR_M, /* [15:8] */
    VO_OUT_FORMAT_SERIAL_YCBCR_L, /* [7:0] */
    VO_OUT_FORMAT_SERIAL_YCRCB_H, /* [23:16] */
    VO_OUT_FORMAT_SERIAL_YCRCB_M, /* [15:8] */
    VO_OUT_FORMAT_SERIAL_YCRCB_L, /* [7:0] */
    VO_OUT_FORMAT_SERIAL_CBCRY_H, /* [23:16] */
    VO_OUT_FORMAT_SERIAL_CBCRY_M, /* [15:8] */
    VO_OUT_FORMAT_SERIAL_CBCRY_L, /* [7:0] */
    VO_OUT_FORMAT_SERIAL_CRCBY_H, /* [23:16] */
    VO_OUT_FORMAT_SERIAL_CRCBY_M, /* [15:8] */
    VO_OUT_FORMAT_SERIAL_CRCBY_L, /* [7:0] */
    VO_OUT_FORMAT_SERIAL_CBYCR_H, /* [23:16] */
    VO_OUT_FORMAT_SERIAL_CBYCR_M, /* [15:8] */
    VO_OUT_FORMAT_SERIAL_CBYCR_L, /* [7:0] */
    VO_OUT_FORMAT_SERIAL_CRYCB_H, /* [23:16] */
    VO_OUT_FORMAT_SERIAL_CRYCB_M, /* [15:8] */
    VO_OUT_FORMAT_SERIAL_CRYCB_L, /* [7:0] */

    VO_OUT_FORMAT_BUTT

}VO_OUT_FORMAT_E;


typedef enum tagVoEndian
{
    VO_BIG_ENDIAN    = 0,
    VO_LITTLE_ENDIAN = 1,
} VO_ENDIAN_E;


/* 信号极性配置 */
typedef enum tagVoSyncPol
{
    VO_SYNC_LOW = 0,           /* 信号低电平 */
    VO_SYNC_HIGH = 1           /* 信号高电平 */
}VO_SYNC_POL_E;

/* RGB mode */
typedef enum tagVoRGBMode
{
    VO_RGB_888 = 0,           /* 信号低电平 */
    VO_RGB_666 = 1,           /* 信号高电平 */
    VO_RGB_565 = 2,           /* 信号高电平 */
    VO_RGB_444 = 3,           /* 信号高电平 */
}VO_RGB_MODE_E;


/* LCD接口时序配置 */
typedef struct tagVoSyncInfo
{
    NI_BOOL                 bSerialOut;     /* 是否串行输出,非串行输出时为按Byte输出 */
    VO_SYNC_POL_E           enVSync;        /* vsync空闲的电平 */
    VO_SYNC_POL_E           enHSync;        /* Hsync空闲的电平 */
    VO_SYNC_POL_E           enVldSync;      /* vld空闲的电平 */

    NI_BOOL                 bVldFlip;       /* 0:valid不翻转，1：valid会翻转 */
    NI_BOOL                 bVldBreak;      /* 0:vld不可以中断，1：vld能中断 */

    NI_U32                  u32VsyncPw;      /* vsync宽度 */
    NI_U32                  u32HsyncPw;      /* hsync宽度 */

    NI_U32                  u32VsyncBp;     /* 列消隐后廊 */
    NI_U32                  u32HsyncBp;     /* 行消隐后廊 */

    NI_U32                  u32HsyncFp;     /* 行消隐前廊 */
    NI_U32                  u32VsyncFp;     /* 列消隐前廊 */
}VO_SYNC_INFO_S;


/* BT1120接口参数配置 */
typedef struct tagVoBt1120Param
{
    NI_BOOL                  bMsbValid;      /* 是否高8bit做为输出,仅8bit模式有效 
                                                0:高8bit做为输出，1:低8bit作为输出 */
    NI_BOOL                  bNoStdSync;     /* 同步头模式,8bit模式时的同步方式，内同步下有效。
                                                1:非标准模式的同步头0xff0000xy。
                                                0:正常模式的同步头0xffff00000000xyxy。*/
}VO_BT1120_INTF_PARAM_S;


/* LCD接口参数配置 */
typedef struct tagVoLcdParam
{
    VO_ENDIAN_E             enEndian;        /*  0：大端，每个byte从高位到低位放置MSB到LSB，
                                                 1：小端，每个byte从高位到低位放置LSB到MSB */
}VO_LCD_INTF_PARAM_S;


/* MIPI接口参数配置 */
typedef struct tagVoMipiIntfParam
{
    NI_U32                  u32LaneNum;         /* 有效lane数目：
                                                   0:1个数据lane有效（lane0有效）。
                                                   1:2个数据lane有效（lane0和lane1有效）。
                                                   2:3个数据lane有效（lane0，lane1和lane2有效）
                                                   3:4个数据lane有效（lane0，lane1，lane2和lane3有效）。*/
    VO_ENDIAN_E             enEndian;           /*  0：大端，每个byte从高位到低位放置MSB到LSB，
                                                    1：小端，每个byte从高位到低位放置LSB到MSB */
    NI_U32	                u32PhyDataRate;     /* 数据输入速率，单位为Mbps */
}VO_MIPI_INTF_PARAM_S;


typedef struct tagVoMipiTxCmdInfo
{ 
   NI_U32   u32TxDevId;    /* MIPI Tx 设备号，一般配0*/

   NI_U16   u16DataType; /* 命令数据类型。根据数据个数选择数据类型，没有数据只有寄存器地址一般用0x5，
                            有寄存器地址+一个数据一般用0x23,有寄存器地址+大于等于两个数据一般用0x29。*/
   NI_U16   u16CmdSize;   /* 数据类型个数。但是只有大于两个数据时才用来表示数据个数。当没有数据时，为寄存器地址；
                           当一个数据时，低两位为地址，高两位为数据*/
   NI_U8    *pu8Cmd;      /* 命令数据指针,当小于两个数据时，可置为NULL ；否则为数据*/
}VO_MIPI_TX_CMD_INFO_S; 


/* VO 设备属性 */
typedef struct tagVoDevAttr
{
    VO_INTF_TYPE_E         enIntfType;        /* 输出接口选择 如:VO_INTF_CVBS|VO_INTF_BT1120 */
    VO_INTF_SYNC_E         enIntfSync;        /* 接口同步信息选择 */
    VO_SYNC_INFO_S         stSyncInfo;        /* 接口时序信息, 用户自定义时序时，需要保证时序的有效性 */
    VO_OUT_FORMAT_E        enOutFormat;       /* 输出像素格式选择         */
    NI_U32                 u32PaddingColor;   /* 设备填充色，用RGB表示,如红色：0xff0000              */
    NI_BOOL                bColorBar;         /* 设备数据是否为彩条模式 */
}VO_DEV_ATTR_S;


/* VO通道属性 */
typedef struct tagVoChnAttr
{
    RECT_S             stValidRect;   /* 视频层有效区域 */
    NI_BOOL            bColorBar;     /* 通道数据是否为彩条模式 xs7320 不支持 */
}VO_CHN_ATTR_S;



/* 转换系数 */
typedef struct tagVoYuv2RgbParam 
{
    NI_U32      u32KrY;      /* YUV转R分量的系数0 默认：0x12a */
    NI_U32      u32KrV;      /* YUV转R分量的系数1 默认：0x199 */
    NI_U32      u32KgY;      /* YUV转G分量的系数0 默认：0x12a */
    NI_U32      u32KgU;      /* YUV转G分量的系数1 默认：0x79c */
    NI_U32      u32KgV;      /* YUV转G分量的系数2 默认：0x730 */
    NI_U32      u32KbY;      /* YUV转B分量的系数0 默认：0x12a */
    NI_U32      u32KbU;      /* YUV转B分量的系数1 默认：0x204 */
}VO_YUV2RGB_PARAM_S;


/* Rgb2YuvCsc参数   */
typedef struct tagVoRgb2YuvParam 
{
    VO_RGB_MODE_E    enRgbMode;   /* csc输出rgb数据时四舍五入的处理格式 */
        
    NI_U32      u32KyR;           /* RGB转Y分量的系数0 默认：0x42 */
    NI_U32      u32KyG;           /* RGB转Y分量的系数1 默认：0x81 */
    NI_U32      u32KyB;           /* RGB转Y分量的系数2 默认：0x19 */
    NI_U32      u32KyOffset;      /* RGB转Y分量的offset系数 默认：0x10 */

    NI_U32      u32KuR;           /* RGB转U分量的系数0 默认：0x1dc */
    NI_U32      u32KuG;           /* RGB转U分量的系数1 默认：0x1b6 */
    NI_U32      u32KuB;           /* RGB转U分量的系数2 默认：0x70 */
    NI_U32      u32KuOffset;      /* RGB转U分量的offset系数 默认：0x80 */
    
    NI_U32      u32KvR;           /* RGB转V分量的系数0 默认：0x70 */
    NI_U32      u32KvG;           /* RGB转V分量的系数1 默认：0x1a2 */
    NI_U32      u32KvB;           /* RGB转V分量的系数2 默认：0x1ee */
    NI_U32      u32KvOffset;      /* RGB转V分量的offset系数 默认：0x80 */
}VO_RGB2YUV_PARAM_S;


typedef struct tagVoCscParam 
{
    VO_YUV2RGB_PARAM_S     stYuv2RgbParam;
    VO_RGB2YUV_PARAM_S     stRgb2YuvParam;
}VO_CSC_PARAM_S;


/* 视频层参数      */
typedef struct tagVoVideoLayerParam 
{
    RECT_S             stLayerRect;     /* 视频层区域（包括填充区域）*/
    NI_U32             u32PaddingColor; /* 视频层填充色，用RGB表示，如红色：0xff0000 xs7320 不支持               */
}VO_VIDEO_LAYER_PARAM_S;


/* VO通道数据显示来源 */
typedef struct tagVoDispSrc
{
    MOD_ID_E    enModId;
    NI_S32      s32DevId;               /* 对应ISP的设备号或VPS的组号 */
    NI_S32      s32ChnId;               /* 目前无效，配0即可 */
}VO_DISP_SRC_S;



#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

