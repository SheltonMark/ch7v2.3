/* 
 * ni_comm_vi.h -- vi模块对外提供的头文件
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */



#ifndef _NI_COMMON_VI_H_
#define _NI_COMMON_VI_H_


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "ni_errno.h"
#include "ni_common.h"
#include "ni_define.h"


#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#define VI_MAX_DATA_LINE            (16)

/* invlalid device ID */
#define NI_ERR_VI_INVALID_DEVID     NI_DEF_ERR(NI_ID_VI, EN_ERR_LEVEL_ERROR, ERR_INVALID_DEVID)
/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define NI_ERR_VI_ILLEGAL_PARAM     NI_DEF_ERR(NI_ID_VI, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)
/* using a NULL point */
#define NI_ERR_VI_NULL_PTR          NI_DEF_ERR(NI_ID_VI, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)
/* try to enable or initialize system,device or channel, before configing attribute */
#define NI_ERR_VI_NOT_CONFIG        NI_DEF_ERR(NI_ID_VI, EN_ERR_LEVEL_ERROR, ERR_NOT_CONFIG)
/* operation is not supported by NOW */
#define NI_ERR_VI_NOT_SUPPORT       NI_DEF_ERR(NI_ID_VI, EN_ERR_LEVEL_ERROR, ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change static attribute */
#define NI_ERR_VI_NOT_PERM          NI_DEF_ERR(NI_ID_VI, EN_ERR_LEVEL_ERROR, ERR_NOT_PERM)
/* failure caused by malloc memory */
#define NI_ERR_VI_NOMEM             NI_DEF_ERR(NI_ID_VI, EN_ERR_LEVEL_ERROR, ERR_NOMEM)
/* failure caused by malloc buffer */
#define NI_ERR_VI_NOBUF             NI_DEF_ERR(NI_ID_VI, EN_ERR_LEVEL_ERROR, ERR_NOBUF)
/* no data in buffer */
#define NI_ERR_VI_BUF_EMPTY         NI_DEF_ERR(NI_ID_VI, EN_ERR_LEVEL_ERROR, ERR_BUF_EMPTY)
/* no buffer for new data */
#define NI_ERR_VI_BUF_FULL          NI_DEF_ERR(NI_ID_VI, EN_ERR_LEVEL_ERROR, ERR_BUF_FULL)
/* failure caused by request irq */
#define NI_ERR_VI_REQUEST_IRQFAIL   NI_DEF_ERR(NI_ID_VI, EN_ERR_LEVEL_ERROR, ERR_REQ_IRQ_FAIL)
/* failure caused by VI dev not enable */
#define NI_ERR_VI_DEV_NOT_ENABLED   NI_DEF_ERR(NI_ID_VI, EN_ERR_LEVEL_ERROR, ERR_NOT_ENABLE)
/* failure caused by VI dev not disable */
#define NI_ERR_VI_DEV_NOT_DISABLE   NI_DEF_ERR(NI_ID_VI, EN_ERR_LEVEL_ERROR, ERR_NOT_DISABLE)
/* failure caused by timeout */
#define NI_ERR_VI_GET_DATA_TIMEOUT  NI_DEF_ERR(NI_ID_VI, EN_ERR_LEVEL_ERROR, ERR_TIME_OUT)
/* failure caused by timeout */
#define NI_ERR_VI_HAS_EXIST         NI_DEF_ERR(NI_ID_VI, EN_ERR_LEVEL_ERROR, ERR_EXIST)

/* interface tpye of video input */
typedef enum tagViIntfType
{
    VI_INTF_TYPE_DC,                      /* digatal camera type */  
    VI_INTF_TYPE_MIPI,                    /* MIPI type */
    VI_INTF_TYPE_BT1120,                  /* BT.1120 progressive type */
    
    VI_INTF_TYPE_BUTT
} VI_INTF_TYPE_E;


/* VI工作模式选择  */
typedef enum tagViWorkMode
{
    VI_WORK_MODE_NORMAL = 0,              /* 正常模式 */
    VI_WORK_MODE_SENCLK_COLORBAR,         /* sensor时钟驱动彩条 */
    VI_WORK_MODE_SYSCLK_COLORBAR,         /* 系统时钟驱动彩条测试模式 */
    VI_WORK_MODE_SEND_DDR_FRAME,          /* 读DDR帧存送给ISP处理模式 */

    VI_WORK_MODE_NONDIRECT,               /* 非直通模式 */
    
    VI_WORK_MODE_LOWMEM_CAPTURE,          /* 低内存抓图模式,仅用于抓图工具,不建议用户使用 */
    
    VI_WORK_MODE_BUTT    
}VI_WORK_MODE_E;

/* 定义BAYER格式的RGB顺序头 */
typedef enum tagBayerRgbSeq
{
    BAYER_SEQ_GBRG = 0,                   /* 表示BAYER格式数据以GB开头 */	
    BAYER_SEQ_RGGB = 1,                   /* 表示BAYER格式数据以RG开头 */
    BAYER_SEQ_BGGR = 2,                   /* 表示BAYER格式数据以BG开头 */
    BAYER_SEQ_GRBG = 3                    /* 表示BAYER格式数据以GR开头 */
}BAYER_RGB_SEQ_E;

/* 同步模式 */
typedef enum tagViSyncMode
{
    VI_SYNC_EXTERNAL = 0,                 /* 外同步模式 */		
    VI_SYNC_INTERNAL = 1                  /* 内同步模式 */
}VI_SYNC_MODE_E;

/* 行同步有效信号极性配置 */
typedef enum tagViHSyncValidPol
{
    VI_HSYNC_VALID_LOW = 0,		          /* 行同步有效信号低有效 */		
    VI_HSYNC_VALID_HIGH = 1               /* 行同步有效信号高有效 */	
}VI_HSYNC_VAILD_POL_E;

/* 帧同步有效信号极性配置 */
typedef enum tagViVSyncValidPol
{
    VI_VSYNC_VALID_LOW = 0,	              /* 帧同步有效信号低有效 */	
    VI_VSYNC_VALID_HIGH = 1               /* 帧同步有效信号高有效 */
}VI_VSYNC_VAILD_POL_E;

/* 定义同步数据的类型 */
typedef enum tagViSyncType
{
    VI_SYNC_VALID_START   = 0,	          /* 有效行起始的同步字；Hispi模式下表SOF */
    VI_SYNC_VALID_END     = 1,            /* 有效行结束的同步字；Hispi模式下表EOL */
    VI_SYNC_INVALID_START = 2,	          /* 无效行起始的同步字；Hispi模式下表SOL */
    VI_SYNC_INVALID_END   = 3,            /* 无效行结束的同步字；Hispi模式下表EOF */
    VI_SYNC_TYPE_BUTT
}VI_SYNC_TYPE_E;

/* 内同步头数据 */
typedef struct tagViInterSyncData
{	
    VI_SYNC_TYPE_E enSyncType;            /* 表示同步字的类型 */

    NI_U32       u32Data0;	              /* 内同步头第一个同步字数据 */
    NI_U32       u32Data1;	              /* 内同步头第二个同步字数据 */
	NI_U32       u32Data2;                /* 内同步头第三个同步字数据 */
    NI_U32       u32Data3;	              /* 内同步头第四个同步字数据, DOW开启时，为短曝光帧的第四个同步字 */
} VI_INTER_SYNC_DATA_S;

/* 内同步头 */
typedef struct tagViInterSyncHead
{	
    NI_U32	                u32SyncNum;         /* 内同步头同步字数,代表多少个同步字,HDR开启时,必须为4个同步字 */
    VI_INTER_SYNC_DATA_S    astSyncData[4];     /* 4种类型的同步字:*/
} VI_INTER_SYNC_HEAD_S;


/* Blank information of the input timing */
typedef struct tagViTimingBlank
{
    NI_U16 u16ImageVDummy ;    /* 图像上端行消隐区行数 */
    NI_U16 u16ImageHDummy ;    /* 图像左端场消隐区点数 */
}VI_TIMING_BLANK_S;

/* synchronization information about the interface timing */
typedef struct tagViSyncCfg
{    
    VI_SYNC_MODE_E          enSyncMode;     /* 内同步or外同步 */
    VI_INTER_SYNC_HEAD_S    stSyncHead;     /* 内同步头同步信息 */
    VI_HSYNC_VAILD_POL_E    enHSyncPolar;   /* 行同步有效信号极性配置 */  
    VI_VSYNC_VAILD_POL_E    enVSyncPolar;   /* 帧同步有效信号极性配置 */
    NI_BOOL                 bClkInvEn;      /* 时钟反向 */
} VI_SYNC_CFG_S;

/* down frame */

typedef enum tagViDownMode
{
    VI_DOWN_DEL_CURRENT = 0,            /* 删除当前帧，eg:step为5时,删除当前帧保留4帧 */	
    VI_DOWN_PICK_ONE = 1,               /* 提取当前帧（仅一帧，仅测试模式）*/
    VI_DOWN_PICK_CONTINUE =2,           /* 提取当前帧（连续），eg:step为5时,保留当前帧删除其余4帧*/
}VI_DOWN_MODE_E;

typedef struct tagViDownFrmCfg
{    
    VI_DOWN_MODE_E          enDownMode;     /* 抽帧模式 */
    NI_U32                  u32Step;        /* 抽帧步长设置，0-255,0表示不抽帧 */
} VI_DOWN_FRAME_S;

/* the attributes of a Video interface device */
typedef struct tagViDevAttr
{
    VI_INTF_TYPE_E      enIntfType;       /* Interface type */
    NI_U32              u32DataSelMask;   /* 低16bit有效，VI 16根线与sensor数据线的连接关系，若第X根线与sensor的有效数据线相连，
                                                     则将第Xbit置1, 否则置0, X范围: [0,15] */
    NI_BOOL             bDataRev;         /* 特殊情况，sensor的硬件走线与vi 数据线接反时，如sensor_data0与VI_data7相连时，此处配true，反之false*/
    BAYER_RGB_SEQ_E     enRgbSeq;         /* 表示sensor输入的RGB格式排列顺序 */
    YUV_SEQ_E           enYuvSeq;         /* YUV格式排列顺序,bt1120 数据输出格式 */        
    RECT_S              stCapRect;        /* 图像区域提取的矩形区域位置,要求宽高8对齐，起始位置2对齐，用于将sensor传输的多余数据裁剪掉 */
    SIZE_S              stDestSize;       /* 输出图像的总宽高,包括消隐区 */
    HDR_MODE_E          enHdrMode;        /* HDR模式 */   
    VI_SYNC_CFG_S       stSyncCfg;        /* 时序信息配置 */
    VI_DOWN_FRAME_S     stDownFrmCfg;     /* 抽帧配置*/
    NI_BOOL             bCompress;        /* 无损压缩 */
} VI_DEV_ATTR_S;

typedef enum tagViColorBarMode
{
    VI_COLORBAR_VERTICAL = 0,
    VI_COLORBAR_HORIZONTAL,
    VI_COLORBAR_SQUARE,
    VI_COLORBAR_PURE
}VI_COLORBAR_MODE_E;

/* 定义RGB三分量的颜色值 */
typedef struct tagViColorRgb
{
    NI_U32   u32R;            /* r分量的颜色值 */
    NI_U32   u32B;            /* b分量的颜色值 */
    NI_U32   u32G;            /* g分量的颜色值 */
}VI_COLOR_RGB_S;


/* 检测信息 */
typedef struct tagViTimerDetect
{
    NI_U32              u32LastOutFrmCnt;    /* 上一次isif输出的帧计数 */
    NI_U32              u32TimerDetSucCnt;   /* 定时器检测为正确的次数 */
    NI_U32              u32TimerResetCnt;    /* 定时器执行软复位的次数 */
    NI_U32              u32TimerRunCnt;      /* 定时器中断触发运行的次数 */
    NI_U32              u32NoErrNoOutCnt;    /* 定时器监测到的状态寄存器没有报错，也没有输出图像的次数 */
    NI_U32              u32DetErrCnt;        /* 定时器监测到的状态寄存器报错的次数 */
    NI_U32              u32ViResetCnt;       /* 执行VI软复位的计数 */
    NI_U32              u32ViErrCnt;         /* 检测到VI状态异常的计数*/
}VI_TIMER_DETECT_S;


/* 调试信息 */
typedef struct tagViDebugInfo
{
    NI_U32      u32SenFrmCnt;                   /* sensor输入的帧计数 */
    NI_U32      u32OutFrmCnt;                   /* 输出给ISP模块的帧计数 */
    NI_U32      u32SenWidth;                    /* sensor输入的图像宽度 */
    NI_U32      u32SenHeight;                   /* sensor输入的图像高度 */
    VI_TIMER_DETECT_S stDetect;
}VI_DEBUG_INFO_S;

/* IPI Data Types */
typedef enum tagMipiDataType
{
    MIPI_DATA_YUV420_8          = 0x18,
    MIPI_DATA_YUV420_10         = 0x19, 
    MIPI_DATA_YUV420_8_LEG      = 0x1A,
    MIPI_DATA_YUV420_8_SHIFT    = 0x1C,
    MIPI_DATA_YUV420_10_SHIFT   = 0x1D,
    MIPI_DATA_YUV422_8          = 0x1E,
    MIPI_DATA_YUV422_10         = 0x1F,  
    MIPI_DATA_RGB444            = 0x20,
    MIPI_DATA_RGB555            = 0x21,
    MIPI_DATA_RGB565            = 0x22,
    MIPI_DATA_RGB666            = 0x23,
    MIPI_DATA_RGB888            = 0x24,
    MIPI_DATA_RAW6              = 0x28,
    MIPI_DATA_RAW7              = 0x29,
    MIPI_DATA_RAW8              = 0x2A,
    MIPI_DATA_RAW10             = 0x2B,
    MIPI_DATA_RAW12             = 0x2C,
    MIPI_DATA_RAW14             = 0x2D,
}MIPI_DATA_TYPE_E;

typedef enum tagMipiHDRDataIdentify
{
    MIPI_HDR_DATA_IDENTIFY_VIRTCHN  = 0,            /* 通过MIPI协议中的虚拟通道解析传输的视频数据 */
    MIPI_HDR_DATA_IDENTIFY_DATATYPE = 1,            /* 通过MIPI协议中的DATA TYPE字段解析传输的视频数据 */
    MIPI_HDR_DATA_IDENTIFY_SYNCCODE = 2,            /* 通过同步码方式解析MIPI传输的视频数据 */
}MIPI_HDR_DATA_IDENTIFY_E;

/* 工作模式参数 */
typedef struct tagViWorkParam
{
    VI_WORK_MODE_E       enWorkMode;        /* 工作模式 */
    NI_BOOL              bColorBarMove;     /* 彩条是否移动 */
    VI_COLORBAR_MODE_E   enColorBarMode;    /* 彩条模式 */
    VI_COLOR_RGB_S       stSingleColor;     /* 单色屏测试模式下的参数配置 */
} VI_WORK_PARAM_S;


/* 定义MIPI数据时序信息 */
typedef struct tagMipiDataTiming
{
    HDR_MODE_E           enHdrMode;             /* HDR模式 */

    MIPI_HDR_DATA_IDENTIFY_E enHdrDataIdentify;  /* sensor HDR模式下的数据解析方式 */
    NI_U32               u32SyncCodeNum;        /* clipper 暂不适用,内同步头个数,最大支持4个同步头 */
    NI_U16               au16SyncCode[3][4];    /* 同步头码 */    
    NI_U32               au32DataTypeCode[3];   /* clipper 暂不适用,HDR模式下不同曝光帧的数据类型，MIPI_HDR_DATA_IDENTIFY_DATATYPE时有效 */
    
    NI_U32               u32FsetBitIndex;       /* clipper 暂不适用,sony的fset值在第几bit位，0~15 */
    NI_U32               u32FsetVal;            /* clipper 暂不适用,sony的fset值强制赋为一个值 */      
}MIPI_DATA_TIMING_S;


/* 定义MIPI参数 */
typedef struct tagViMipiParam
{
    MIPI_DATA_TYPE_E     enDataType;            /* MIPI CSI-2的数据类型 */
    NI_U32               u32LaneNum;            /* 有效lane数目:
                                                            0:1个数据lane有效（lane0有效）。
                                                            1:2个数据lane有效（lane0和lane1有效）。
                                                            2:3个数据lane有效（lane0，lane1和lane2有效）
                                                            3:4个数据lane有效（lane0，lane1，lane2和lane3有效）。*/
    NI_U32               u32Freq;               /* upper margin of frequency range, Mbps */
    MIPI_DATA_TIMING_S   stDataTiming;          /* MIPI传输的数据时序信息 */
    NI_U8                au8IdMap[4];           /* MIPI的虚拟通道与VP ID之间的映射，eg:au8VirtIdMap[3] = 0表示将MIPI的虚拟通道3映射到
                                                   VP的ID0。VP传输给后级顺序为ID0、ID1、ID2、ID3。如果MIPI只使用两个虚拟通道则只使
                                                   用VP的ID0，ID1 。保证VP的4个ID映射到不同的ID
                                                   针对于宽动态时，要将MIPI传过来的长帧放在ID1，短帧放在ID0 */
} VI_MIPI_PARAM_S;


/* 定义用户发送raw数据信息结构体 */
typedef struct tagViRawDataInfo
{
    NI_U32                u32Width;             /* 图像有效宽高 */
    NI_U32                u32Height;            /* 图像有效宽高 */
    PIXEL_FORMAT_E        enPixelFormat;        /* taurus 要求灌源格式仅支持UYVY格式和raw格式 */
    NI_U32                u32FrameNum;          /* 仅硬件读DDR数据有效，软件上传图像帧数,是减去1的配置，0表示1帧。 */
    NI_U32                u32PhyAddr;           /* 软件导数据源的基地址,16byte对齐,每一帧数据存储空间的大小,16byte对齐，宽动态按照短、中、长顺序排列。 */
}VI_RAW_DATA_INFO_S;

typedef struct tagViRawDataEx
{
    VIDEO_FRAME_S   astFrame[4];        /* 图像数据信息 */
    NI_U32          u32FrameNum;        /* 图像帧数 */
    NI_U32          u32TimeOut;         /* 超时时间 */
}VI_RAW_DATA_EX_S;

typedef enum tagViSensorClk
{
    VI_SENSOR_PCLK         = 0x0,               /* 选择sensor的pclk */
    VI_SENSOR_VCLK         = 0x1,               /* 选择sensor的vclk */
    
    VI_SENSOR_CLK_BUTT
}VI_SENSOR_CLK_E;

typedef enum tagViMultiEyeClk
{
    VI_MULTIEYE_SENSOR_VCLK     = 0x00,
    VI_MULTIEYE_SENSOR_PCLK         =0x01,

    VI_MULTIEYE_CLK_BUTT
}VI_MULTIEYE_CLK_E;

typedef struct tagViMultiEyeClkCfg
{
    NI_BOOL             bClkRevrse;
    VI_MULTIEYE_CLK_E   enClkSel;
}VI_MULTIEYE_CLK_CFG_S;


/* 定义多目参数 */
typedef struct tagViMultiEyeParam
{
    NI_BOOL             bMultiEnable;           /* 是否使能多目功能 */
//    VI_SENSOR_CLK_E     enClkSel;               /* 选择sensor的clk时钟类型 */
    VI_MULTIEYE_CLK_CFG_S   stClkCfg;               /* 多目时钟配置 */
    
    NI_U32              u32VVldMode;            /* 0:代表u32VVld为持续几拍的信号; 1:代表u32VVld持续几行的信号 */
    NI_U32              u32VVldBefor;           /* u32VVld上升沿距离第一个u32HVld上升沿的拍数 */
    NI_U32              u32VVld;                /* u32VVldMode为0时，表持续u32VVld拍; u32VVldMode为1时，表持续u32VVld行 */
    NI_U32              u32HVld;                /* 每行高电平持续的拍数 */
    NI_U32              u32HTotal;              /* 一行的总时间 */
    NI_U32              u32VTotal;              /* 一帧的总行数 */
    NI_U32              u32FrameSize;           /* 一帧的拍数, 计算方式为s_clk/fps */
} VI_MULTIEYE_PARAM_S;

/* 定义VI设备、编解码通道映射    */

typedef enum tagViIntfId
{
    VI_INTF_NONE = 0,           
    
    VI_INTF0_ID0 = 1,
    VI_INTF0_ID1,    
    VI_INTF0_ID2,
    VI_INTF0_ID3, 
    
    VI_INTF1_ID0,
    VI_INTF1_ID1,    
    VI_INTF1_ID2,
    VI_INTF1_ID3,
    
    VI_INTF_BUTT,
}VI_INTF_ID_E;

typedef enum tagViEncId
{
    VI_ENC_NONE = 0,

    VI_ENC_ID0,
    VI_ENC_ID1,
    VI_ENC_ID2,
    VI_ENC_ID3,
    VI_ENC_ID4,

    VI_ENC_BUTT,
}VI_ENC_ID_E;

typedef enum tagViDecSel
{
    VI_DEC_NONE = 0,

    VI_DEC0_SEL0,
    VI_DEC0_SEL1,
    VI_DEC1_SEL0,
    VI_DEC1_SEL1,

    VI_DEC_BUTT,
}VI_DEC_SEL_E;


typedef struct tagViMapParam
{
    NI_BOOL         bEnableMap;         /* 是否使能用户自定义映射关系 */
    VI_INTF_ID_E    enSelIntf;          /* 选择设备对应的INTFx_IDx，若为VI_INTF_NONE 则该设备不使用        */    
    VI_ENC_ID_E     enSelEnc;           /* 选择INTFx_IDx对接的编码通道，若为VI_ENC_NONE 则该INTFx_IDx数据流不上内存 */
    VI_DEC_SEL_E    enSelDec;           /* 选择编码通道ID对接的解码通道，若为VI_DEC_NONE 则该数据不发送给ISP。*/
}VI_MAP_PARAM_S;


/* 定义获取图像数据参数 */
typedef struct tagViGetFrmParam
{    
    NI_S32                  s32Depth;            /*RW, 设置底层驱动缓存图像帧数,rang[3 ~ 20] */
    PIXEL_FORMAT_E          enPixelFormat;       /*RW, 上传DDR存储格式  */
    /* 每行存储的空间，以BYTE为单位，当为0时，行与行之间紧密排列。
     * au32Stride[0]: 存放YUV的Y数据。
     * au32Stride[1]: NV12时存放uv数据，IYUV时存放U数据
     * au32Stride[2]: IYUV时存放V数据 */
    NI_U32                  au32Stride[3];
}VI_GET_FRM_PARAM_S;

typedef NI_S32 (*VI_INTCALLBACK_PF)(NI_VOID); 

typedef struct tagViIntCallBackParam
{
    VI_INTCALLBACK_PF pfnLongExpIntCallBack;
    VI_INTCALLBACK_PF pfnShortExpIntCallBack;
}VI_INT_CALLBACK_PARAM_S;

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

