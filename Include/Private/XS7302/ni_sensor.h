/*******************************************************************************
 * Copyright (C) 2011-2013 ZheJiang Chipup Technology CO.,LTD.
 *
 * 文件名 : ni_sensor.h
 *
 * 作  者 : zhu_peng(24484)<zhu_peng@chipup.com>
 * 版  本 : V1.0.0
 * 日  期 : 2017年9月23日 Create
 *
 * 描  述 : 
 *
 *       1. 硬件说明
 *          无。
 *
 *       2. 程序结构说明。
 *          无。
 *
 *       3. 使用说明。
 *          无。
 *
 *       4. 局限性说明。
 *          无。
 *
 *       5. 其他说明。
 *          无。
 *
 * 修订历史: 
 * 日    期: 
 * 修订版本:
 * 作    者:
 * 修订备注:
 *******************************************************************************/
#ifndef _NI_SENSOR_H_
#define _NI_SENSOR_H_
     
     
#ifdef __cplusplus
     extern "C"{
#endif /* __cplusplus */


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "ni_common.h"
#include "ni_type.h"

/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */
#define  SEN_DOW_MAX_FRAME_NUM       (5)

/*sensorType*/
typedef enum tagSenType
{
    /*OS系列*/
    SEN_TYPE_OS08A10          = 0,        
    SEN_TYPE_OS05A10,  
    SEN_TYPE_OS02B10,
    SEN_TYPE_OS02G10,
    SEN_TYPE_OS02K10,
    SEN_TYPE_OS04A10,
    SEN_TYPE_OS02H10,
    SEN_TYPE_OS04C10,
    SEN_TYPE_OS03B10,
	SEN_TYPE_OS04B10,
	SEN_TYPE_OS04L10,
    SEN_TYPE_OS02M10,
    SEN_TYPE_OS04D10,
    SEN_TYPE_OS02N10,

    /*OV系列*/
    SEN_TYPE_OV4689           = 1000,  
    SEN_TYPE_OV2710,
    SEN_TYPE_OV2735,
    SEN_TYPE_OV4689_2A,
    SEN_TYPE_OV9281,
    SEN_TYPE_OV9726,
    SEN_TYPE_OV2778,
    SEN_TYPE_OV9750,
    SEN_TYPE_OV08A10,
    SEN_TYPE_OV9623,

    /*IMX系列*/
    SEN_TYPE_IMX290           = 3000,
    SEN_TYPE_IMX307,
    SEN_TYPE_IMX323,
    SEN_TYPE_IMX415,
    SEN_TYPE_IMX327,

    /*SC系列*/
    SEN_TYPE_SC2238           = 4000,    
    SEN_TYPE_SC5238,
    SEN_TYPE_SC2310,
    SEN_TYPE_SC2239,
    SEN_TYPE_SC4238,
    SEN_TYPE_SC1330,
    SEN_TYPE_SC1335,
    SEN_TYPE_SC200AI,
    SEN_TYPE_SC401AI,
    SEN_TYPE_SC2239P,
    SEN_TYPE_SC2336,
    SEN_TYPE_SC3336,
    SEN_TYPE_SC4336,
    SEN_TYPE_SC1345,
    SEN_TYPE_SC530AI,
    SEN_TYPE_SC2331,
    SEN_TYPE_SC8238,
    SEN_TYPE_SC1336,
    SEN_TYPE_SC450AI,
    SEN_TYPE_SC2336P,
    SEN_TYPE_SC233A,
    SEN_TYPE_SC1346,

    /*GC系列*/
	SEN_TYPE_GC2063           = 5000,    
    SEN_TYPE_GC2053,
    SEN_TYPE_GC2093,
    SEN_TYPE_GC4023,    
	SEN_TYPE_GC4653,    
	SEN_TYPE_GC2083,

    /*SP系列*/
    SEN_TYPE_SP2305           = 6000, 
    SEN_TYPE_SP2306,
    SEN_TYPE_SP2329,
    SEN_TYPE_SP230A,

    /*MIS系列*/
    SEN_TYPE_MIS2006          = 7000, 
    SEN_TYPE_MIS2008,
    SEN_TYPE_MIS4001,

	/*Jx系列*/
    SEN_TYPE_JXF23            = 8000, 
    SEN_TYPE_JXF35,
    SEN_TYPE_JXF37,
	SEN_TYPE_JXK06,
	SEN_TYPE_JXQ03P,
    
    /*其他系列*/
    SEN_TYPE_AR0230           = 1000000, 
    SEN_TYPE_C2398,
    SEN_TYPE_BG0808,
    SEN_TYPE_PS6210K,
	SEN_TYPE_C2399,
    SEN_TYPE_CV2003,
    
    SEN_TYPE_BUTT, 
}SEN_TYPE_E;


/*sensor工作模式*/
typedef enum tagSenWorkMode
{
    SEN_WORK_MODE_MASTER    = 0,    /*sensor工作在主模式*/        
    SEN_WORK_MODE_SLAVE     = 1,    /*sensor工作在从模式*/              
    SEN_WORK_MODE_BUTT         , 
}SEN_WORK_MODE_E;


/*sensor宽动态模式*/
typedef enum tagSenHdrMode
{
    SEN_HDR_MODE_NONE       = 0,        /*线性模式*/
    SEN_HDR_MODE_2_LINE     = 1,        /*两帧宽动态行交错模式*/
    SEN_HDR_MODE_2_FRAME    = 2,        /*两帧宽动态帧交错模式*/
    SEN_HDR_MODE_3_LINE     = 3,        /*三帧宽动态行交错模式*/
    SEN_HDR_MODE_3_FRAME    = 4,        /*三帧宽动态帧交错模式*/
    SEN_HDR_MODE_BUILD_IN   = 5,        /*sensor端合成宽动态*/
    SEN_HDR_MODE_BUTT          , 
}SEN_HDR_MODE_E;


/*sensor数据传输模式*/
typedef enum tagSenTransMode
{
    SEN_TRANS_MODE_MIPI       = 0,      /*mipi传输*/
    SEN_TRANS_MODE_PARALLEL   = 1,      /*并口传输*/
    SEN_TRANS_MODE_LVDS       = 2,      /*lvds传输*/
    SEN_TRANS_MODE_BUTT          , 
}SEN_TRANS_MODE_E;


/*sensor设备属性*/
typedef struct tagSenDevAttr
{    
    SEN_TYPE_E          enSenType;      /* sesnor类型编号 */
    NI_U32              u32RstGpioId;   /* 复位管脚gpio id */   
    NI_U32              u32IicDevId;    /* iic设备号 */   
    NI_U32              u32DevAddr;     /* 器件地址 */  
} SEN_DEV_ATTR_S;


/*sensor 输出制式*/
typedef struct tagSenFmtAttr
{    
    NI_U32 u32WidthValid;               /* 视频帧的有效宽度 */
    NI_U32 u32HeightValid;              /* 视频帧的有效高度 */
    NI_U32 u32Fps;                      /* 视频帧率 * 1000 */
} SEN_FMT_ATTR_S;


/*sensor输出的时序宽高*/
typedef struct tagSenWinSize
{    
    NI_U32 u32WidthAll;                 /* 视频帧的总宽度 */
    NI_U32 u32HeightAll;                /* 视频帧的总高度 */
} SEN_WIN_SIZE_S;


/*sensor多帧曝光模式*/
typedef enum tagShtMode
{
    SEN_SHT_MODE_SUM       = 0,      /*各曝光帧曝光时间是公用的，各帧之间曝光时间之和不能大于最大曝光时间*/
    SEN_SHT_MODE_DIFF      = 1,      /*各曝光帧曝光时间是独立的，各帧之间曝光时间之和不能各自最大曝光时间*/
    SEN_SHT_MODE_BUTT         , 
}SEN_SHT_MODE_E;


/*sensor快门曝光能力*/
typedef struct tagSenShtInfo
{    
    SEN_SHT_MODE_E  enShtMode;                                /* 快门模式 */
    NI_U32          u32ExpWidth;                              /* 视频帧的曝光宽度, 不同曝光帧之间是一致的 */
    NI_U32          u32FrameNum;                              /* 曝光帧数*/
    NI_U32          au32ExpHeight[SEN_DOW_MAX_FRAME_NUM];     /* 视频帧的最大曝光高度, 涉及到多帧宽动态，当模式为多帧宽动态时，列出各帧的最大曝光行数 */
    NI_U32          au32MinExpLine[SEN_DOW_MAX_FRAME_NUM];    /* 视频帧的最小曝光宽度, 涉及到多帧宽动态，当模式为多帧宽动态时，列出各帧的最小曝光行数 */
    NI_U32          u32ExpLinePre500Ms;                       /* 500ms对应曝光行数 */
    NI_U32          u32ExpLinePre1000Ms;                      /* 1000ms对应曝光行数 */
} SEN_SHT_INFO_S;


/*sensor增益能力*/
typedef struct tagSenGainInfo
{    
    NI_U32 u32MinGain;                  /* 最小增益 DB*1024 */
    NI_U32 u32MaxGain;                  /* 最大增益 DB*1024 */
    NI_U32 u32GainAcc;                  /* 增益表精度 DB*1024 */
} SEN_GAIN_INFO_S;


/*sensor曝光参数限制， 描述sensor的曝光能力*/
typedef struct tagSenExpInfo
{    
    SEN_SHT_INFO_S      stShtInfo;
    SEN_GAIN_INFO_S     stGainInfo;
} SEN_EXP_INFO_S;


/*sensor数据传输信息*/
typedef struct tagSenTransInfo
{    
    SEN_TRANS_MODE_E    enTransMode;        /*传输模式*/
    NI_U32              u32DataWidth;       /*数据位宽*/
    NI_U32              u32BitRate;         /*数据率，单位为M*/
    NI_U32              u32LaneNum;         /*lane数量，并口可以不关注*/
} SEN_TRANS_INFO_S;


/*sensor时钟信息*/
typedef struct tagSenClkInfo
{    
    NI_U32 u32PixClock;                 /* 视频帧的像素时钟 单位为HZ, 并口模式下有效 */
    NI_U32 u32SrcClock;                 /* 提供给Sensor的源时钟，单位为HZ */
} SEN_CLK_INFO_S;


/*sensor视频格式匹配关键字，用来切换制式*/
typedef struct tagSenFmtMatchkey
{   
    SEN_WORK_MODE_E     enWorkMode;      /* sensor  工作模式*/
    SEN_TRANS_MODE_E    enTransMode;     /* 传输模式*/
    SEN_HDR_MODE_E      enHdrMode;       /* 宽动态模式 */
    NI_U32              u32LaneNum;      /* lane数量，并口可以不关注*/
    NI_U32              u32Width;        /* 视频帧的有效宽度 */
    NI_U32              u32Height;       /* 视频帧的有效高度 */
    NI_U32              u32Fps;          /* 帧率 * 1000 */   
    NI_U32              u32CfgId;        /* 配置ID， 如果同一制式下有多种不同的配置的话，则用此字段区分，具体配置内容见sensor配置列表文档*/   
} SEN_FMT_MATCH_KEY_S;


/*当前sensor信息*/
typedef struct tagSenInfo
{   
    SEN_FMT_MATCH_KEY_S     stSenFmtMatchKey;   /* 格式信息 */
    SEN_WORK_MODE_E         enWorkMode;         /* sensor 实际工作模式*/
    SEN_HDR_MODE_E          enHdrMode;          /* sensor 实际宽动态模式 */
    SEN_FMT_ATTR_S          stSenFmtAttr;       /* sensor 实际格式信息 */      
    SEN_WIN_SIZE_S          stSenWinSize;       /* sensor 时序宽高 */                     
    SEN_EXP_INFO_S          stSenExpInfo;       /* sensor 在本配置下曝光信息 */
    SEN_TRANS_INFO_S        stSenTransInfo;     /* sensor 在本配置下数据传输信息 */
    SEN_CLK_INFO_S          stSenClk;           /* sensor 在本配置下时钟信息 */   
} SEN_INFO_S;


/*快门值 */
typedef struct tagSenShtValue
{   
    NI_U32  u32FrameNum;
    NI_U32  au32Sht[SEN_DOW_MAX_FRAME_NUM];     /*各曝光帧的快门值，单位为曝光行， 从上往下为曝光依次为高曝光到低曝光的帧*/
} SEN_SHT_VALUE_S;


/*增益值*/
typedef struct tagSenGainValue
{   
    NI_U32  u32Gain;                           /*各曝光帧的增益值一致, db*1000*/
} SEN_GAIN_VALUE_S;


typedef struct tagSenExpValue
{   
    SEN_SHT_VALUE_S      stShtValue;
    SEN_GAIN_VALUE_S     stGainValue;
} SEN_EXP_VALUE_S;

#ifdef SEN_xs5032
typedef struct tagSenReg
{    
    NI_U16 u32RegAddr;                  /* 寄存器地址 */
    NI_U16 u32RegVal;                   /* 配置值 */    
    NI_U16 u32DelayTime;                /* 配置这个寄存器需要的延时时间,ms */
}SEN_REG_S;
#else
typedef struct tagSenReg
{    
    NI_U32 u32RegAddr;                  /* 寄存器地址 */
    NI_U32 u32RegVal;                   /* 配置值 */    
    NI_U32 u32DelayTime;                /* 配置这个寄存器需要的延时时间,ms */
}SEN_REG_S;
#endif

typedef NI_S32 (*pfn_sen_cbReportFps)(NI_F32);

/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */

/*sensor 初始化， 只涉及软件功能， 不进行内存等需要释放的资源的申请， 并不操作sensor硬件*/
NI_S32 NI_SEN_Init(SENSOR_ID SenId);
NI_S32 NI_SEN_DeInit(SENSOR_ID SenId);


/*sensor设备属性配置，用来适配不同sensor，不同复位管脚，不同的iic设备地址，器件地址*/
NI_S32 NI_SEN_SetSenIdAttr(SENSOR_ID SenId, SEN_DEV_ATTR_S *pstSenDevAttr);
NI_S32 NI_SEN_GetSenIdAttr(SENSOR_ID SenId, SEN_DEV_ATTR_S *pstSenDevAttr);


/*sensor复位*/
NI_S32 NI_SEN_Reset(SENSOR_ID SenId);


/*sensor 寄存器操作函数, 用来操作对应sensor的寄存器*/
NI_S32 NI_SEN_WriteRegister(SENSOR_ID SenId, NI_U32 u32Addr, NI_U32 u32Value);
NI_S32 NI_SEN_ReadRegister(SENSOR_ID SenId, NI_U32 u32Addr, NI_U32 *pu32Value);
NI_S32 NI_SEN_WriteRegisterGrup(SENSOR_ID SenId, SEN_REG_S *pstRegGrup, NI_U32 u32RegNum);



/*sensor 格式切换函数， 切换视频格式以及宽动态*/
NI_S32 NI_SEN_SetFormat(SENSOR_ID SenId, SEN_FMT_MATCH_KEY_S *pstFmtMatchKey);


/*快门设置函数，分不同曝光帧进行设置*/
NI_S32 NI_SEN_SetSht(SENSOR_ID SenId, SEN_SHT_VALUE_S *pstExp);
NI_S32 NI_SEN_GetSht(SENSOR_ID SenId, SEN_SHT_VALUE_S *pstExp);


/*增益设置函数，分不同曝光帧进行设置*/
NI_S32 NI_SEN_SetGain(SENSOR_ID SenId, SEN_GAIN_VALUE_S *pstGain);
NI_S32 NI_SEN_GetGain(SENSOR_ID SenId, SEN_GAIN_VALUE_S *pstGain);


/*曝光量设置函数，分不同曝光帧进行设置*/
NI_S32 NI_SEN_SetExp(SENSOR_ID SenId, SEN_EXP_VALUE_S *pstExp);
NI_S32 NI_SEN_GetExp(SENSOR_ID SenId, SEN_EXP_VALUE_S *pstExp);


/*sensor  设置完制式后信息获取函数， 用于上层应用直接获取sensor相关信息以及能力，不需要去看sensor代码的实现才知道sensor的相关信息*/
NI_S32 NI_SEN_GetCurInfo(SENSOR_ID SenId, SEN_INFO_S *pstSenInfo);


/*sensor  信息查询函数， 用于上层应用直接获取对应制式下sensor相关信息以及能力，不需要去看sensor代码的实现才知道sensor的相关信息*/
NI_S32 NI_SEN_GetFmtInfo(SENSOR_ID SenId, SEN_FMT_MATCH_KEY_S *pstFmtMatchKey, SEN_INFO_S *pstSenInfo);

/* sensor 提供的通用额外功能，翻转，镜像等*/
NI_S32 NI_SEN_SetFlip(SENSOR_ID SenId, NI_BOOL bFlip);
NI_S32 NI_SEN_SetMirror(SENSOR_ID SenId, NI_BOOL bMirror);

/* sensor 在帧中断处理 */
NI_S32 NI_SEN_SetFrameIrq(SENSOR_ID SenId, NI_VOID *pData, NI_U32 u32DataLen);

/* sensor 设置帧长触发帧率调整*/
NI_S32 NI_SEN_SetVts(SENSOR_ID SenId, NI_U32 u32Vts);

/* sensor 设置帧率回调函数 */
NI_S32 NI_SEN_SetCallBackReportFps(SENSOR_ID SenId, pfn_sen_cbReportFps pfunc);

/*sensor  获取sensor所有支持的制式信息*/
// NI_S32 NI_SEN_GetSenModeInfo(SENSOR_ID SenId);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

