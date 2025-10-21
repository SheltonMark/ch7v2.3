/* 
 * ni_comm_vd.h -- vd模块对外提供的头文件
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */



#ifndef _NI_COMM_VD_H_
#define _NI_COMM_VD_H_


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
//#include <linux/ioctl.h>
#include "ni_common.h"
#include "ni_define.h"

#include "ni_errno.h"


#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */



typedef enum tagVdErrCode
{
    ERR_VD_CHN_EXIST = 64,   /* device or channel has existed */
    ERR_VD_NOT_INIT  = 65,   /* device or channel not initialized */
    ERR_VD_IRQ_FAIL  = 66,   /* device request irq failure */
    ERR_VD_HAS_STARTED  = 67,   /* device or channel has been started*/
    ERR_VD_NOT_START    = 68,   /* device or channel not start */
    ERR_VD_STREAM_ERR   = 69,  /* stream data error */
    ERR_VD_NO_STREAM    = 70,  /* NO stream  */
    ERR_VD_BUSY         = 71,  /* VENC is busy, please wait a moment, then try again */
    ERR_VD_GET_STREAM_TIMEOUT = 72,  /* Get Stream Timeout, try again */
    ERR_VD_NO_REALSE    = 73,
    ERR_VD_CALLFUNCTION = 74,
    ERR_VD_STOP_FAILED = 75
} VD_ERR_CODE_E;

/* invlalid channel ID */
#define NI_ERR_VD_INVALID_CHNID     NI_DEF_ERR(NI_ID_VD, EN_ERR_LEVEL_ERROR, ERR_INVALID_CHNID)
/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define NI_ERR_VD_ILLEGAL_PARAM     NI_DEF_ERR(NI_ID_VD, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)
/* channel exists */
#define NI_ERR_VD_CHN_EXIST         NI_DEF_ERR(NI_ID_VD, EN_ERR_LEVEL_ERROR, ERR_EXIST)
/* channel unexists */
#define NI_ERR_VD_CHN_UNEXIST       NI_DEF_ERR(NI_ID_VD, EN_ERR_LEVEL_ERROR, ERR_UNEXIST)
/* using a NULL point */
#define NI_ERR_VD_NULL_PTR          NI_DEF_ERR(NI_ID_VD, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)
/* try to enable or initialize system,device or channel, before configing attribute */
#define NI_ERR_VD_NOT_CONFIG        NI_DEF_ERR(NI_ID_VD, EN_ERR_LEVEL_ERROR, ERR_NOT_CONFIG)
/* operation is not supported by NOW */
#define NI_ERR_VD_NOT_SUPPORT       NI_DEF_ERR(NI_ID_VD, EN_ERR_LEVEL_ERROR, ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change static attribute */
#define NI_ERR_VD_NOT_PERM          NI_DEF_ERR(NI_ID_VD, EN_ERR_LEVEL_ERROR, ERR_NOT_PERM)
/* failure caused by malloc memory */
#define NI_ERR_VD_NOMEM             NI_DEF_ERR(NI_ID_VD, EN_ERR_LEVEL_ERROR, ERR_NOMEM)
/* failure caused by malloc buffer */
#define NI_ERR_VD_NOBUF             NI_DEF_ERR(NI_ID_VD, EN_ERR_LEVEL_ERROR, ERR_NOBUF)
/* no data in buffer */
#define NI_ERR_VD_BUF_EMPTY         NI_DEF_ERR(NI_ID_VD, EN_ERR_LEVEL_ERROR, ERR_BUF_EMPTY)
/* no buffer for new data */
#define NI_ERR_VD_BUF_FULL          NI_DEF_ERR(NI_ID_VD, EN_ERR_LEVEL_ERROR, ERR_BUF_FULL)
/* failure caused by not initialize module */
#define NI_ERR_VD_NOT_INIT          NI_DEF_ERR(NI_ID_VD, EN_ERR_LEVEL_ERROR, ERR_VD_NOT_INIT)
/* failure caused by request irq */
#define NI_ERR_VD_REQUEST_IRQFAIL   NI_DEF_ERR(NI_ID_VD, EN_ERR_LEVEL_ERROR, ERR_VD_IRQ_FAIL)
/* failure caused by enc has started encoding */
#define NI_ERR_VD_CHN_HAS_STARTED   NI_DEF_ERR(NI_ID_VD, EN_ERR_LEVEL_ERROR, ERR_VD_HAS_STARTED)
/* failure caused by enc has started encoding */
#define NI_ERR_VD_CHN_NOT_START     NI_DEF_ERR(NI_ID_VD, EN_ERR_LEVEL_ERROR, ERR_VD_NOT_START)
/* failure caused by stream err */
#define NI_ERR_VD_STREAM_ERR        NI_DEF_ERR(NI_ID_VD, EN_ERR_LEVEL_ERROR, ERR_VD_STREAM_ERR)
/* failure caused by no stream */
#define NI_ERR_VD_NO_STREAM         NI_DEF_ERR(NI_ID_VD, EN_ERR_LEVEL_ERROR, ERR_VD_NO_STREAM)
/* failure caused by get stream timeout */
#define NI_ERR_VD_GET_STREAM_TIMEOUT         NI_DEF_ERR(NI_ID_VD, EN_ERR_LEVEL_ERROR, ERR_VD_GET_STREAM_TIMEOUT)
/* VENC is busy, please wait a moment, then try again */
#define NI_ERR_VD_BUSY              NI_DEF_ERR(NI_ID_VD, EN_ERR_LEVEL_ERROR, ERR_VD_BUSY)

#define NI_ERR_VD_NO_REALSE         NI_DEF_ERR(NI_ID_VD, EN_ERR_LEVEL_ERROR, ERR_VD_NO_REALSE)

#define NI_ERR_VD_CALLFUNCTION         NI_DEF_ERR(NI_ID_VD, EN_ERR_LEVEL_ERROR, ERR_VD_CALLFUNCTION)

#define NI_ERR_VD_STOP_FAILED         NI_DEF_ERR(NI_ID_VD, EN_ERR_LEVEL_ERROR, ERR_VD_STOP_FAILED)

/*VD工作模式*/
typedef enum tagVdDetectSel
{ 
    VD_DETECT_SEL_MD = 0,
    VD_DETECT_SEL_BD,
    
    VD_DETECT_SEL_BUTT     
}VD_DETECT_SEL_E;

typedef struct tagVdWorkMode
{
    VD_DETECT_SEL_E enDetectSel;
    NI_BOOL         bDiffSumEn;
    NI_BOOL         bPixLumaSumEn;
}VD_WORK_MODE_S;

/*数据格式*/
typedef struct tagVdDataBuffer
{
    NI_U32 u32PhyAddr;                   /*物理地址*/
    NI_U32 u32VirAddr;                   /*虚拟地址*/
    NI_U32 u32Size;                      /*数据长度*/
    NI_U32 u32BlkId;
}VD_DATA_BUFFER_S;

/*Md的数据结构*/
typedef struct tagVdMdData 
{
    NI_BOOL            bDiffSumValid;    /*残差和值数据是否有效*/
    NI_BOOL            bPixLumaSumValid; /*像素亮度和值是否有效*/
    NI_BOOL            bAomResultValid;  /*Aom区域结构数据是否有效*/
    NI_BOOL            abAomMDResult[VD_MAX_DETECT_REGION]; /*AOM区域动检结果*/
    VD_DATA_BUFFER_S   stDiffSum;       /*残差和值数据格式*/
    VD_DATA_BUFFER_S   stAomResult;     /*Aom区域结构数据格式*/
    VD_DATA_BUFFER_S   stPixLuma;        /*动检Y数据格式*/
}VD_MD_DATA_S;

typedef struct tagVdBdData 
{ 
    NI_BOOL bBdDetect;                  /*盲检是否检测到*/
    NI_U32  u32BdBpsum;                 /*黑电平像素点数*/
    NI_U32  u32BdTotalPixSum;           /*总像素点个数*/
}VD_BD_DATA_S; 

typedef struct tagVdData 
{ 
    VD_MD_DATA_S stMdData;              /*动检数据结构*/
    VD_BD_DATA_S stBdData;              /*盲检数据结构*/
    NI_BOOL      bMdDataExist;          /*动检数据是否存在*/
    NI_BOOL      bBdDataExist;          /*盲检数据是否存在*/
    NI_U64       u64Pts;                /*时间戳,单位微秒 */
}VD_DATA_S; 

typedef struct tagMdParam
{
    NI_U8 u8MdNoiseUpCoe;               /*动检噪声学习速度*/
    NI_U8 u8MdNoiseSlowCoe;             /*动检噪声学习降低系数*/
    NI_U8 u8MdNoiseToDiffCoe;           /*动检残差判定为非噪声的判决系数*/
    NI_U8 u8MdStudyFrameNum;            /*动检学习帧数*/
}VD_MD_PARAM_S;

typedef struct tagVdSbParam
{
    NI_U8 u8SbVerticalNum;              /*动检分割区域垂直SB个数*/
    NI_U8 u8SbHorizontalNum;            /*动检分割区域水平SB个数*/
}VD_SB_PARAM_S;

typedef struct tagAomParam
{
    NI_BOOL bAomEnable;                 /*Aom区域是否使能*/
    NI_U8   u8AomSense;                 /*运动检测区域的灵敏度*/ 
    NI_U8   u8AomDiffCoe;               /*运动检测区域残差适配系数*/ 
    NI_U16  u16AomMdthrd;               /*运动检测区域的动检阈值*/ 
    NI_U16  u16AomDiffLevel;            /*运动检测区域残差基准*/  
}VD_AOM_PARAM_S;

typedef struct tagVdDetectRegion
{
    NI_U32     u32AomId;                /*RegionID*/
    NI_BOOL    bRectDetect;             /*当前Rect是否使能检测*/
    RECT_POS_S stDetectRect;            /*Region区域*/
}VD_DETECT_REGION_S;

typedef struct tagVdMdAttr  
{
    VD_MD_PARAM_S  stMdParam;           /*动检参数*/
    VD_SB_PARAM_S  stSbParam;           /*SB参数,创建通道时修改，创建完成后，不能修改*/
    VD_AOM_PARAM_S stAomParam[VD_MAX_DETECT_REGION];       /*Aom参数*/
}VD_MD_ATTR_S; 

typedef struct tagVdBdAttr
{
     NI_U8 u8BdGate;                    /*遮挡检测判决阈值*/ 
     NI_U8 u8BdPixelBlackGate;          /*黑电平判决阈值*/
     NI_U8 u8BdPixelStep;               /*遮挡检测跳点距离*/
}VD_BD_ATTR_S;

typedef struct tagVdChnAttr 
{  
    RECT_S          stCutRect;          /*剪切区域*/
    NI_U32          u32OutWidth;        /*通道的宽度*/
    NI_U32          u32OutHeight;       /*通道的高度*/
    NI_U32          u32Interval;        /*帧间隔*/
    
    VD_MD_ATTR_S    stMdAttr;           /*动检属性*/
    VD_BD_ATTR_S    stBdAttr;           /*盲检属性*/  
}VD_CHN_ATTR_S;

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

