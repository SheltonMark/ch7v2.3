/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef __NI_COMM_PISP_H__
#define __NI_COMM_PISP_H__

#include "ni_common.h"
#include "ni_define.h"

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus */

typedef enum tagPispErrCode {
    ERR_PISP_CHN_EXIST   = 64, /*device or channel has existed*/
    ERR_PISP_NOT_INIT    = 65, /*device or channel not initialized*/
    ERR_PISP_IRQ_FAIL    = 66, /*device request irq failure*/
    ERR_PISP_HAS_STARTED = 67, /*device or channel has been started*/
    ERR_PISP_NOT_START   = 68, /*device or channel not start*/
    ERR_PISP_STREAM_ERR  = 69, /*stream data error*/
    ERR_PISP_NO_STREAM   = 70, /*NO stream */
} PISP_ERR_CODE_E;

/*invlalid device ID*/
#define NI_ERR_PISP_INVALID_DEVID NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_INVALID_DEVID)
/*invlalid channel ID*/
#define NI_ERR_PISP_INVALID_SNSID NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_INVALID_CHNID)
/*at lease one parameter is illagal ,eg, an illegal enumeration value */
#define NI_ERR_PISP_ILLEGAL_PARAM NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)
/*using a NULL point*/
#define NI_ERR_PISP_NULL_PTR NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)
/*try to enable or initialize system,device or channel, before configing attribute*/
#define NI_ERR_PISP_NOT_CONFIG NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_NOT_CONFIG)
/*operation is not supported by NOW*/
#define NI_ERR_PISP_NOT_SUPPORT NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_NOT_SUPPORT)
/*operation is not permitted ,eg, try to change static attribute*/
#define NI_ERR_PISP_NOT_PERM NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_NOT_PERM)
/*failure caused by malloc memory*/
#define NI_ERR_PISP_NOMEM NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_NOMEM)
/*failure caused by malloc buffer*/
#define NI_ERR_PISP_NOBUF NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_NOBUF)
/*no data in buffer*/
#define NI_ERR_PISP_BUF_EMPTY NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_BUF_EMPTY)
/*no buffer for new data*/
#define NI_ERR_PISP_BUF_FULL NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_BUF_FULL)
/*not initialize*/
#define NI_ERR_PISP_NOT_INIT NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_SYS_NOTREADY)
/*has been initialize*/
#define NI_ERR_PISP_HAS_INIT NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_EXIST)
/*request IRQ fail*/
#define NI_ERR_PISP_REQUEST_IRQFAIL NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_REQ_IRQ_FAIL)
/*timeout*/
#define NI_ERR_PISP_TIMEOUT NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_TIME_OUT)

#define PISP_ENABLE  (1)
#define PISP_DISABLE (0)

/*定义AWES和RAWHIST 类型*/
#define PISP_AWES_TYPE    (0)
#define PISP_RAWHIST_TYPE (1)

#define AWES_USE_BANK_NUM    (2)
#define RAWHIST_USE_BANK_NUM (2)

/**************************************************************************************************
                                            PUB
**************************************************************************************************/

typedef struct tagPispPubAttr {
    RECT_S stWndRect;    /*RW. 输入宽高*/
    NI_U32 u32FrameRate; /*RW, Sensor输出帧率, Range:[1,60]*/
} PISP_PUB_ATTR_S;

/**************************************************************************************************
                                            ModParam
**************************************************************************************************/

/*定义ISP模块级参数结构体*/
typedef struct tagPispModParam {
    NI_BOOL bAfBufferInit;       /*RW, ISP初始化时是否为AF分配内存*/
    NI_U32  u16AwesMaxBlockCnt;  /*RW, Awes最大统计分块个数, Range:[100,ISP_AWES_MAX_ZONE_NUM]*/
    NI_U16  u16AwesBufBlkNum;    /*RW, Awes统计模块的缓存块个数, Range:[3,10]*/
    NI_U16  u16AfBufBlkNum;      /*RW, Af统计模块的缓存块个数, Range:[3,16]*/
    NI_U16  u16YhistBufBlkNum;   /*RW, Yhist统计模块的缓存块个数, Range:[3,16]*/
    NI_U16  u16RawhistBufBlkNum; /*RW, Rawhist统计模块的缓存块个数, Range:[3,10]*/
} PISP_MOD_PARAM_S;

/**************************************************************************************************
                                            BLC
**************************************************************************************************/

/*定义黑电平校正各分量偏移值结构体*/
typedef struct tagPispBlackLevel {
    NI_U16 u16BlcR;  /*RW, R像素黑电平偏移值, U10.2, Range:[0,4095], default:0x0*/
    NI_U16 u16BlcGr; /*RW, Gr像素黑电平偏移值, U10.2, Range:[0,4095], default:0x0*/
    NI_U16 u16BlcGb; /*RW, Gb像素黑电平偏移值, U10.2, Range:[0,4095], default:0x0*/
    NI_U16 u16BlcB;  /*RW, B像素黑电平偏移值, U10.2, Range:[0,4095], default:0x0*/
} PISP_BLACK_LEVEL_S;

/*定义黑电平校正参数结构体*/
typedef struct tagPispBlcParam {
    PISP_BLACK_LEVEL_S stBlackLevel;
} PISP_BLC_PARAM_S;

/**************************************************************************************************
                                            DPC
***************************************************************************************************/

/*定义dpc限幅阈值*/
typedef struct tagPispDpcThreshold {
    NI_U16 u16MinThrR;  /*RW, R限幅阈值, 当中心点为黑点时使用, U12.0, Range:[0,4095]*/
    NI_U16 u16MaxThrR;  /*RW, R限幅阈值, 当中心点为白点值时使用, U12.0, Range:[0,4095]*/
    NI_U16 u16MinThrGr; /*RW, Gr限幅阈值, 当中心点为黑点时使用, U12.0, Range:[0,4095]*/
    NI_U16 u16MaxThrGr; /*RW, Gr限幅阈值, 当中心点为白点值时使用, U12.0, Range:[0,4095]*/
    NI_U16 u16MinThrGb; /*RW, Gb限幅阈值, 当中心点为黑点时使用, U12.0, Range:[0,4095]*/
    NI_U16 u16MaxThrGb; /*RW, Gb限幅阈值, 当中心点为白点值时使用, U12.0, Range:[0,4095]*/
    NI_U16 u16MinThrB;  /*RW, B限幅阈值, 当中心点为黑点时使用, U12.0, Range:[0,4095]*/
    NI_U16 u16MaxThrB;  /*RW, B限幅阈值, 当中心点为白点值时使用, U12.0, Range:[0,4095]*/
} PISP_DPC_THR_S;

typedef struct tagPispDpcParam {
    NI_BOOL        bDpcEn;       /*RW, Dpc使能, Range:[0,1]*/
    NI_BOOL        bDpcFirstEn;  /*RW, Dpc第一级使能开关, Range:[0,1]*/
    NI_U8          u8GType;      /*RW, G分量处理窗方式, Range:[0,1], 0:方形; 1:菱形*/
    NI_U8          u8DpcMode;    /*RW, 动态检测模式, Range:[0,1],0:利用最大、最小值检测; 1:利用最大、次最大、最小、次最小值检测*/
    NI_U8          u8SortMode;   /*RW, 排序模式, Range:[0,1], 0:排序时允许次大/小值与最大/小值相同; 1:排序时候不允许次大/小值与最大/小值相同*/
    NI_U8          u8OriginType; /*RW, 原点类型, Range:[0,1], 0:原点为最值; 1:原点不一定为最值. 当u32DpcMode=1时(即利用最大、次最大、最小、次最小值检测), 该参数有效*/
    PISP_DPC_THR_S stThr;        /*RW, 跟最大最小中心点有关的DPC阈值*/

    NI_BOOL        bDpcNxtEn;     /*RW, 是否使能下一级Dpc, Range:[0,1]*/
    PISP_DPC_THR_S stNxtThr;      /*RW, 下一级动态坏点校正阈值*/
    NI_U32         u32NxtDpcMode; /*RW, 下一级Dpc过滤模式, Range:[0,1]:
                                        0:做好上一级Dpc过滤后, 全部做一遍下一级Dpc过滤;
                                        1:做好上一级Dpc过滤后, 若上一级dpc用最大、小值替代, 则下一级不过滤; 否则用下一级dpc过滤*/
} PISP_DPC_PARAM_S;

/**************************************************************************************************
                                             GIC
***************************************************************************************************/
typedef struct tagPispGicCurv {
    NI_U16 u16Thr1;
    NI_U16 u16Thr2;
    NI_U16 u16Thr3;
    NI_U16 u16Thr4;

    NI_U8 u8Ratio1;
    NI_U8 u8Ratio2;
    NI_U8 u8Ratio3;
} PISP_GIC_CURV_S;

typedef struct tagPispGicCurvLum {
    NI_U16 u16Thr1;
    NI_U16 u16Thr2;
    NI_U16 u16Thr3;
    NI_U16 u16Thr4;

    NI_U8 u8Ratio1;
    NI_U8 u8Ratio2;
} PISP_GIC_LUM_CURV_S;

typedef struct tagPispGicRatio {
    NI_U8 u8R;  /*RW, Gic模块R分量使能强度, U1.4, Range:[0,16], 16:强度最强; 0:关闭该分量使能, Gic中:default:0x10; GgbGic中:default:0x0*/
    NI_U8 u8Gr; /*RW, Gic模块Gr分量使能强度, U1.4, Range:[0,16], 16:强度最强; 0:关闭该分量使能,  Gic中:default:0x10; GgbGic中:default:0x0*/
    NI_U8 u8Gb; /*RW, Gic模块Gb分量使能强度, U1.4, Range:[0,16], 16:强度最强; 0:关闭该分量使能,  Gic中:default:0x10; GgbGic中:default:0x0*/
    NI_U8 u8B;  /*RW, Gic模块B分量使能强度, U1.4, Range:[0,16], 16:强度最强; 0:关闭该分量使能,  Gic中:default:0x10; GgbGic中:default:0x0*/
} PISP_GIC_RATIO_S;

typedef struct tagPispGicSubMin {
    NI_U16 u16Thr1;  /*RW, 插值最小值差异参数1, U12.0, 宽动态:t2-t1;            线性:t2-t1和t4-t3 = 2, 4, 8, 16,...,2048*/
    NI_U16 u16Thr2;  /*RW, 插值最小值差异参数2, U12.0, 宽动态:t2-t1;             线性:t2-t1和t4-t3 = 2, 4, 8, 16,...,2048*/
    NI_U8  u8Ratio1; /*RW, 插值差异值的校正力度1, U2.4, Range:[0,63]*/
    NI_U8  u8Ratio2; /*RW, 插值差异值的校正力度2, U2.4, Range:[0,63]*/
} PISP_GIC_G_SUB_MIN_S;

typedef struct tagPispGicSubMax {
    NI_U16 u16Thr1;  /*RW, 插值最大值差异参数1, U12.0, 宽动态:t2-t1 = 2, 4, 8,...,128;            线性:t2-t1 = 2, 4, 8, 16,...,2048*/
    NI_U16 u16Thr2;  /*RW, 插值最大值差异参数2, U12.0, 宽动态:t2-t1 = 2, 4, 8,...,128;            线性:t2-t1 = 2, 4, 8, 16,...,2048*/
    NI_U8  u8Ratio1; /*RW, 插值差异值的校正力度1, U2.4, Range:[0,63]*/
} PISP_GIC_G_SUB_MAX_S;

typedef struct tagPispGicParam {
    NI_BOOL              bGicEn;         /*RW, Gic模块使能开关, U1.0, Range:[0,1], default:1*/
    NI_U32               u32GicMode;     /*RW, Gic的工作模式选择, U1.0, Range:[0,1], 在使能开启的情况下:0:标准模式 1:强力模式, default:1*/
    NI_U16               u16Ratio;       /*RW, Gic模块放大系数, 值越大, 放大力度越大. U4.4, Range:[0,255]*/
    NI_U16               u16MinThr;      /*RW, Gic模块差异阈值, 等于0时, 阈值不生效. 宽动态:U8.0;                     线性:U12.0*/
    NI_U16               au16GicRThr[4]; /*RW, R分量滤波阈值参数, au32GicRThr[0]为第一阈值,以此类推. 宽动态:U8.0, 线性:U12.0, default{0x3,0x9,0x18,0x24}*/
    NI_U16               au16GicGThr[4]; /*RW, G分量滤波阈值参数, au32GicGThr[0]为第一阈值,以此类推. 宽动态:U8.0, 线性:U12.0, default{0xc,0x1e,0x24,0x30}*/
    NI_U16               au16GicBThr[4]; /*RW, B分量滤波阈值参数, au32GicBThr[0]为第一阈值,以此类推. 宽动态:U8.0, 线性:U12.0, default{0x3,0x9,0x18,0x24}*/
    PISP_GIC_RATIO_S     stGicRatio;     /*RW, Gic模块Gr/Gb/R/B分量使能强度控制*/
    PISP_GIC_LUM_CURV_S  stLumaCurv;     /*RW, Gic模块亮度曲线结构体 Thr:宽动态:t2-t1和t4-t3 = 2, 4, 8,...,128;           线性:t2-t1和t4-t3 = 2, 4, 8, 16,...,2048,      Ratio:U2.4,Range:[0,63]*/
    PISP_GIC_CURV_S      stContCurv;     /*RW, Gic模块对比度曲线结构体 Thr:宽动态:t2-t1和t4-t3 = 2, 4, 8,...,128;线性:t2-t1和t4-t3 = 2, 4, 8, 16,...,2048, Ratio:U2.4,Range:[0,63]*/
    PISP_GIC_G_SUB_MIN_S stMinFix;       /*RW, Gic模块插值中差异最小值校正系数*/
    PISP_GIC_G_SUB_MAX_S stMaxFix;       /*RW, Gic模块插值中差异最大值校正系数*/
} PISP_GIC_PARAM_S;

/**************************************************************************************************
                                             LSC
***************************************************************************************************/
typedef struct tagPispLscTable {
    NI_U32  u32TabLen;  /*RW, 颜色表的长度, 表长ISP_LSC_MAX_TABLE_LEN = 192, 表宽:U2.10*/
    NI_U16 *pu16TableR; /*RW, R表, 成员数据类型U2.10, Range:[0,4095], 为防止非法指针（例如野指针）导致未知异常发生，请务必对pu16TableR指针分配合适的内存空间*/
    NI_U16 *pu16TableG; /*RW, G表, 成员数据类型U2.10, Range:[0,4095], 为防止非法指针（例如野指针）导致未知异常发生，请务必对pu16TableG指针分配合适的内存空间*/
    NI_U16 *pu16TableB; /*RW, B表, 成员数据类型U2.10, Range:[0,4095], 为防止非法指针（例如野指针）导致未知异常发生，请务必对pu16TableB指针分配合适的内存空间*/
} PISP_LSC_TABLE_S;

typedef struct tagPispLscParam {
    NI_BOOL bLscEn;    /*RW, Lsc使能开关, 使能关闭不查表, 但依然限幅. 可以配合限幅寄存器配为0xfff, 这样使能关闭数据透传*/
    POINT_S stLscPosR; /*RW, R分量中心坐标, U11.0, Range:[0,2047]*/
    POINT_S stLscPosG; /*RW, G分量中心坐标, U11.0, Range:[0,2047], */
    POINT_S stLscPosB; /*RW, B分量中心坐标, U11.0, Range:[0,2047], */

    PISP_LSC_TABLE_S stLscTab; /*RW, Lsc表*/
} PISP_LSC_PARAM_S;

/**************************************************************************************************
                                            DGC
***************************************************************************************************/

/*定义数字增益系数*/
typedef struct tagPispDgcGain {
    NI_U32 u32GainR;  /*RW, R像素增益系数, U8.8, Range:[0,65535], default:0x100*/
    NI_U32 u32GainGr; /*RW, Gr像素增益系数, U8.8, Range:[0,65535], default:0x100*/
    NI_U32 u32GainGb; /*RW, Gb像素增益系数, U8.8, Range:[0,65535], default:0x100*/
    NI_U32 u32GainB;  /*RW, B像素增益系数, U8.8, Range:[0,65535], default:0x100*/
} PISP_DGC_GAIN_S;

/*定义数字增益参数*/
typedef struct tagPispDgcParam {
    PISP_DGC_GAIN_S stGain; /*RW, Dgc模块R/Gr/Gb/B增益系数*/
} PISP_DGC_PARAM_S;

/**************************************************************************************************
                                            WB
***************************************************************************************************/

/*定义白平衡参数结构体*/
typedef struct tagPispWbGain {
    NI_U16 u16GainR;  /*RW, R像素增益系数, U4.8, Range:[0,4095], default:0x100*/
    NI_U16 u16GainGr; /*RW, Gr像素增益系数, U4.8, Range:[0,4095], default:0x100*/
    NI_U16 u16GainGb; /*RW, Gb像素增益系数, U4.8, Range:[0,4095], default:0x100*/
    NI_U16 u16GainB;  /*RW, B像素增益系数, U4.8, Range:[0,4095], default:0x100*/
} PISP_WB_GAIN_S;

typedef struct tagPispWbParam {
    PISP_WB_GAIN_S stWbGain; /*RW, Wb参数*/
} PISP_WB_PARAM_S;

/**************************************************************************************************
                                             DOW
***************************************************************************************************/

typedef struct tagPispHdrMode {
    HDR_MODE_E enHdrMode;
} PISP_HDR_MODE_S;

/*定义数字交叉宽动态关闭时选择的帧类型*/
typedef enum tagPispDowFrame {
    PISP_DOW_FRAME_C = 0, /*Dow数据输出选择融合帧*/
    PISP_DOW_FRAME_S = 1, /*Dow数据输出选择短曝光帧*/
    PISP_DOW_FRAME_L = 2, /*Dow数据输出选择长曝光帧*/

    PISP_DOW_FRAME_BUTT
} PISP_DOW_FRAME_E;

typedef struct tagPispDow2Frm {
    NI_U16 u16CmTh1; /*RW, DOL2模式融合阈值1, U8.0, Range:[0,255], default:0xc8*/
    NI_U16 u16CmTh2; /*RW, DOL2模式融合阈值2, U12.0, Range:[0,4095], default:0x0*/
    NI_U32 u32CmK1;  /*RW, DOL2模式融合斜率1, U1.16, Range:[0,0x1ffff], default:0x1b4*/
    NI_U32 u32CmK2;  /*RW, DOL2模式融合斜率2, U1.16, Range:[0,0x1ffff], default:0x492*/

    NI_U16 u16DghStr;      /*RW, 鬼影优化强度等级, U9.0, Range:[0,511], default:0x0*/
    NI_U32 u32DghVirLevel; /*RW, 鬼影优化虚化程度, U19.10, Range:[0,0x1fffffff], default:0x3000000*/

    NI_U16 u16LdFusion1;    /*RW, 明暗融合系数1, U9.0, Range:[0,511], default:0x96*/
    NI_U16 u16LdFusion2;    /*RW, 明暗融合系数2, U9.0, Range:[0,511], default:0x50*/
    NI_U16 u16LdFusionStr1; /*RW, 明暗融合强度1, U9.0, Range:[0,511], default:0xff*/
    NI_U16 u16LdFusionStr2; /*RW, 明暗融合强度2, U9.0, Range:[0,511], default:0x100*/

    NI_U8 u8SefSatStr;  /*RW, 宽动态两帧融合中短帧的色彩饱和度调整强度, U0.6, Range:[0,63], default:0x29*/
    NI_U8 u8LsConvert;  /*RW, 主从通道交换, Range:[0,1], 0:主通道是短曝光、辅通道是长曝光; 1:主通道是长曝光、辅通道是短曝光, default:0x0*/
    NI_U8 u8MsModeSel;  /*RW, 亮度计算选择, Range:[0,1], 0:采用高斯滤波计算方式; 1:采用亮度抑制的计算方式, 非高光弥散场景, 使用0模式; 否则, 使用1模式 default:0x1*/
    NI_U8 u8MsHandleEn; /*RW, 融合时的高光抑制使能, Range:[0,1],   default:0x1*/
} PISP_2F_COMB_CTRL_S;

typedef struct tagPispDowParam2 {
    NI_BOOL             bDowEn;        /*RW, Dow使能开关, Range:[0,1]*/
    PISP_DOW_FRAME_E    enFrameOutSel; /*RW, Dow使能开启时, 数据输出选择, Range:[0,2], 0:融合数据; 1:短曝光数据; 2:长曝光数据*/
    PISP_2F_COMB_CTRL_S stDow2Frm;     /*RW, 2帧模式下融合参数配置*/
} PISP_DOW_PARAM_S;

/**************************************************************************************************
                                            DRCD
***************************************************************************************************/

typedef struct tagPispDrcdCurv {
    NI_U32 u32Thr;
    NI_U32 u32ThrOffset;
    NI_U8  u8Ratio1;
    NI_U8  u8Ratio2;
    NI_U8  u8Ratio3;
} PISP_DRCD_CURV_S;

typedef struct tagPispDrcdDetailStr {
    NI_U32 u32Thr1; /*RW, 细节增强阈值1, U20.0,       Range:[0,0xfffff], default:0x0*/
    NI_U32 u32Thr2; /*RW, 细节增强阈值2, U20.0,       Range:[0,0xfffff],  Thr2 > Thr1; default:0xfff*/
    NI_U8  u8RStr;  /*RW, R细节增强比例系数 U1.4,         Range:[0,31], default:0x8*/
    NI_U8  u8GStr;  /*RW, G细节增强比例系数 U1.4,         Range:[0,31], default:0x10*/
    NI_U8  u8BStr;  /*RW, B细节增强比例系数 U1.4,         Range:[0,31], default:0x8*/
} PISP_DRCD_DETAIL_STR_S;

typedef struct tagPispDrcdParam {
    NI_BOOL bRawHdrEn;     /*RW, Drcd模块的压缩使能,        Range:[0,1], default:0x0*/
    NI_U8   u8AlphaGy1;    /*RW, Drcd压缩融合控制参数, 值越大, 细节越丰富, 勾边越强. U1.4,                     Range:[0,16], default:0x8*/
    NI_U8   u8AlphaGy2;    /*RW, Drcd压缩融合控制参数, 值越大, 边缘越宽. U1.4,                Range:[0,16], default:0x8*/
    NI_U16  u16CurvAlpha;  /*RW, Drcd曲线1跟曲线2的融合阈值, 值越大, 图像越亮. U1.12, Range:[0,4096], default:0x1000*/
    NI_U8   u8Compress1;   /*RW, Drcd压缩曲线1选择, 线性模式:Range:[0,8] 宽动态模式模式:Range:[0,7]*/
    NI_U8   u8Compress2;   /*RW, Drcd压缩曲线2选择, 线性模式:Range:[0,8] 宽动态模式模式:Range:[0,7]*/
    NI_U32  u32ImGZeroThr; /*RW, Drcd压缩判断阈值, 配合曲线固定使用, U20.0,               Range:[0,0xfffff], default:0x0*/

    PISP_DRCD_DETAIL_STR_S stDetailStr;       /*RW, Drcd细节增强限幅、系数*/
    PISP_DRCD_CURV_S       stAlphaFreq;       /*RW, 频率分段融合压缩控制曲线, T: Range:[0,0xfffff-144], Offset: Range:[0,0xfffff-144-T]. R:值越大, 勾边越弱, 边缘越平滑, 且颜色饱和度也越弱. U2.4,                                  Range:[0,63]*/
    PISP_DRCD_CURV_S       stDetailLum;       /*RW, 细节增强亮度控制曲线, T: Range:[0,0xfffff-32], Offset: Range:[0,0xfffff-32-T]. R:值越大, 强度越大. U2.4, Range:[0,63]*/
    PISP_DRCD_CURV_S       stDetailPosYctrst; /*RW, 细节增强正边控制曲线, T: Range:[0,0xfffff-32], Offset: Range:[0,0xfffff-32-T]. R:值越大, 强度越大. U2.4,                  Range:[0,63]*/
    PISP_DRCD_CURV_S       stDetailNegYctrst; /*RW, 细节增强负边控制曲线, T: Range:[0,0xfffff-32], Offset: Range:[0,0xfffff-32-T]. R:值越大, 强度越大. U2.4,                  Range:[0,63]*/
} PISP_DRCD_PARAM_S;

/*************************************************************************************************
                                            DEC
***************************************************************************************************/

typedef struct tagPispDecImP12b {
    NI_U16 u16DecImP1; /*RW, 解压曲线12bit坐标第一个拐点, U12.0,            Range:[0,0xfff], default:0xfff*/
    NI_U16 u16DecImPk; /*RW, 解压曲线12bit坐标第二个拐点, U12.0,            Range:[0,0xfff], default:0xfff*/
    NI_U16 u16DecImPl; /*RW, 解压曲线12bit坐标附加第三个拐点, U12.0,              Range:[0,0xfff], default:0xc20*/
    NI_U16 u16DecImPm; /*RW, 解压曲线12bit坐标附加第四个拐点, U12.0,              Range:[0,0xfff], default:0xca0*/
    NI_U16 u16DecImPn; /*RW, 解压曲线12bit坐标附加第五个拐点, U12.0,              Range:[0,0xfff], default:0xda0*/
} PISP_DEC_IM_P_12BIT_S;

typedef struct tagPispDecImP20b {
    NI_U32 u32DecImP2; /*RW, 解压曲线20bit坐标第二个拐点, U20.0, Range:[0,0xfffff], default:0xfff*/
    NI_U32 u32DecImP3; /*RW, 解压曲线20bit坐标附加第三个拐点, U20.0, Range:[0,0xfffff], default:0x8000*/
    NI_U32 u32DecImP4; /*RW, 解压曲线20bit坐标附加第四个拐点, U20.0, Range:[0,0xfffff], default:0x40000*/
    NI_U32 u32DecImP5; /*RW, 解压曲线20bit坐标附加第五个拐点, U20.0, Range:[0,0xfffff], default:0x80000*/
} PISP_DEC_IM_P_20BIT_S;

typedef struct tagPispDecIm4r1 {
    NI_U8 u8DecIm4r1;    /*RW, 解压曲线第一段斜率, U4.0, Range:[0,15], default:0x1*/
    NI_U8 u8DecIm4r1To2; /*RW, 解压曲线第二段斜率, U4.0, Range:[0,15], default:0x1*/
    NI_U8 u8DecIm4r1To3; /*RW, 解压曲线附加第三段斜率, U4.0, Range:[0,15], default:0xa*/
    NI_U8 u8DecIm4r1To4; /*RW, 解压曲线附加第四段斜率, U4.0, Range:[0,15], default:0xa*/
    NI_U8 u8DecIm4r1To5; /*RW, 解压曲线附加第五段斜率, U4.0, Range:[0,15], default:0xa*/
} PISP_DEC_SLP_S;

typedef struct tagPispDecParam {
    NI_BOOL bDeCompEn;                    /*RW, Dec模块的解压使能. Range:[0,1], default:0x0*/
    NI_U8   u8DecApd;                     /*RW, 解压曲线增加拐点控制位, 0: 仅2个拐点; 1: 增加1个拐点, 即3个拐点; 2: 增加2个拐点,
                                               即4个拐点; 3: 增加3个拐点, 即5个拐点,              U2.0, Range:[0,3], default:0x0*/
    NI_U32                u32DecImPmax;   /*RW, Dec解压出数据限幅, U20.0, Range:[0,0xfffff], default:0xfffff*/
    NI_U32                u32DecImBlcVal; /*RW, 黑电平校正值, U20.0, Range:[0,0xfffff], default:0x0*/
    PISP_DEC_IM_P_12BIT_S stDecImP12b;    /*RW, Dec解压曲线12bit坐标附加拐点*/
    PISP_DEC_IM_P_20BIT_S stDecImP20b;    /*RW, Dec解压曲线20bit坐标附加拐点*/
    PISP_DEC_SLP_S        stDecIm4r1;     /*RW, Dec解压曲线第一~五段斜率*/

} PISP_DEC_PARAM_S;

/*************************************************************************************************
                                            LWDR
***************************************************************************************************/
/*局部宽动态结构体定义*/
typedef struct tagPispLwdrParam {
    NI_BOOL bLwdrEn;      /*RW, 宽动态开关, Range:[0,1]*/
    NI_BOOL bFusEn;       /*RW, 亮度融合使能*/
    NI_U8   u8GLuma;      /*RW, 全局亮度调节参数, U1.4, Range:[0,16], default:0xa*/
    NI_U8   u8GContrast;  /*RW, 全局对比度调节参数, U1.4, Range:[0,16], default:0xa*/
    NI_U8   u8NoiseLevel; /*RW, 噪声水平调节参数, U5.3, Range:[0,255], default:0x10*/

    NI_U8 u8Alpha;    /*RW, 输出亮度融合, 值越大Lwdr效果越弱. U1.4,              Range:[0,16], default:0x0*/
    NI_U8 u8FusDelta; /*RW, 亮度融合系数, 值越大亮度抑制程度越大. U0.8, Range:[0,255], default:0x40*/
    NI_U8 u8FusThr;   /*RW, 亮度融合阈值, 小于该值进行亮度抑制, U8.0, Range:[0,255], default:0xff*/

    NI_U8 u8LegDsFltAlpha; /*RW, Leg滤波混合强度, U1.4, Range:[0,16], default:0x8*/
    NI_U8 u8LumDsFltSize;  /*RW, Leg滤波尺寸, 值越大, 尺寸越大. Range:[0,2], default:0x0*/
    NI_U8 u8LumDsFltAlpha; /*RW, Lum滤波混合强度, U1.4, Range:[0,16], default:0x8*/
    NI_U8 au8GsCoef[5];    /*RW, Lwdr Gaus滤波系数, U0.8, Range:[0,255], Coef0 + (Coef1+ Coef2+ Coef3+ Coef4)*2 = 256*/

    NI_U16 u16AlphaTabLen; /*RW, Alpha表长256, 表宽:U8.0, Range:[0,255]*/
    NI_U8 *pu8AlphaTab;    /*RW, Alpha表, 为防止非法指针（例如野指针）导致未知异常发生，请务必对pu8AlphaTab指针分配合适的内存空间*/
    NI_U16 u16FiTabLen;    /*RW, Fi表长256, 表宽:U8.0, Range:[0,255]*/
    NI_U8 *pu8FiTab;       /*RW, Fi表, 为防止非法指针（例如野指针）导致未知异常发生，请务必对pu8FiTab指针分配合适的内存空间*/
} PISP_LWDR_PARAM_S;

/**************************************************************************************************
                                            CFA
***************************************************************************************************/

typedef struct tagPispCfaRb {
    NI_BOOL bREn;         /*RW, R调节使能, Range:[0,1]*/
    NI_BOOL bBEn;         /*RW, B调节使能, Range:[0,1]*/
    NI_U8   u8RbFloorThr; /*RW, Rb下限阈值 U8.0, Range:[0,255], default 0x14*/
    NI_U8   u8RbTopThr;   /*RW, Rb上限阈值 U8.0, Range:[0,255], default 0x5f*/
    NI_U8   u8RbDltThr;   /*RW, Rb梯度阈值 U8.0, Range:[0,255], default 0x28*/
} PISP_CFA_RB_S;

typedef struct tagPispCfaParam {
    NI_U8         u8KRatio;  /*RW, 颜色滤波矩阵的插值比较系数, U4.4, Range:[0,255], default:0x10*/
    NI_U8         u8AaLevel; /*RW, 数值越大去伪色强度越大, 0:关闭, U3.0, Range:[0,4], default:0x4*/
    NI_U8         u8MmRatio; /*RW, 边缘方向最值判断比较系数, U4.4 Range:[0,255], default:0x16*/
    NI_U16        u16DThr1;  /*RW, 去横纹调节阈值1, U6.4, Range:[0,0x3ff], default:0xc0*/
    NI_U16        u16DThr2;  /*RW, 去横纹调节阈值2, U6.0, Range:[0,63],default:0xc*/
    PISP_CFA_RB_S stCfaRb;   /*RW, RB平滑优化参数*/
} PISP_CFA_PARAM_S;

/**************************************************************************************************
                                                RGBDPC
***************************************************************************************************/

/*定义dpc限幅阈值*/
typedef struct tagPispRgbDpcThreshold {
    NI_U16 u16MinThrR;  /*RW, R限幅阈值, 当中心点为黑点时使用, U8.0, Range:[0,255]*/
    NI_U16 u16MaxThrR;  /*RW, R限幅阈值, 当中心点为白点值时使用, U8.0, Range:[0,255]*/
    NI_U16 u16MinThrGr; /*RW, Gr限幅阈值, 当中心点为黑点时使用, U8.0, Range:[0,255]*/
    NI_U16 u16MaxThrGr; /*RW, Gr限幅阈值, 当中心点为白点值时使用, U8.0, Range:[0,255]*/
    NI_U16 u16MinThrGb; /*RW, Gb限幅阈值, 当中心点为黑点时使用, U8.0, Range:[0,255]*/
    NI_U16 u16MaxThrGb; /*RW, Gb限幅阈值, 当中心点为白点值时使用, U8.0, Range:[0,255]*/
    NI_U16 u16MinThrB;  /*RW, B限幅阈值, 当中心点为黑点时使用, U8.0, Range:[0,255]*/
    NI_U16 u16MaxThrB;  /*RW, B限幅阈值, 当中心点为白点值时使用, U8.0, Range:[0,255]*/
} PISP_RGB_DPC_THR_S;

typedef struct tagPispRgbDpcParam {
    NI_BOOL            bDpcEn; /*RW, Dpc使能, Range:[0,1]*/
    PISP_RGB_DPC_THR_S stThr;  /*RW, DPC阈值*/
} PISP_RGB_DPC_PARAM_S;

/**************************************************************************************************
                                                RGBGIC
***************************************************************************************************/

typedef struct tagPispRgbGicCurv {
    NI_U16 u16Thr;
    NI_U16 u16ThrOffset;

    NI_U8 u8Ratio1;
    NI_U8 u8Ratio2;
    NI_U8 u8Ratio3;
} PISP_RGB_GIC_CURV_S;

typedef struct tagPispRgbGicParam {
    NI_U32              u32GicMode;    /*RW, Gic的工作模式选择, U1.0,, Range:[0,1], 在使能开启的情况下:0:标准模式 1:强力模式, default:1*/
    NI_U8               au8GicRThr[4]; /*RW, R分量滤波阈值参数, au8GicRThr[0]为第一阈值,以此类推. U8.0 Range:[0,255], default{0x3,0x9,0x18,0x24}*/
    NI_U8               au8GicGThr[4]; /*RW, G分量滤波阈值参数, au8GicGThr[0]为第一阈值,以此类推. U8.0 Range:[0,255], default{0xc,0x1e,0x24,0x30}*/
    NI_U8               au8GicBThr[4]; /*RW, B分量滤波阈值参数, au8GicBThr[0]为第一阈值,以此类推. U8.0 Range:[0,255], default{0x3,0x9,0x18,0x24}*/
    PISP_GIC_RATIO_S    stGicRatio;    /*RW, Gic模块Gr/Gb/R/B分量使能强度控制*/
    PISP_RGB_GIC_CURV_S stLumaCurv;    /*RW, Gic模块亮度曲线结构体, T: Range:[0,175], Offset: Range:[0,175-T], R:U2.4*/
    PISP_RGB_GIC_CURV_S stContCurv;    /*RW, Gic模块对比度曲线结构体, T: Range:[0,243], Offset: Range:[0,243-T], R:U2.4*/
} PISP_RGB_GIC_PARAM_S;

/**************************************************************************************************
                                                CCM
***************************************************************************************************/

typedef struct tagPispCcmParam {
    NI_S16 as16GainMatrix[3][3]; /*RW, 色彩校正矩阵系数, S5.10, Range:[-32768,32767]         排布形式:RR GR BR
                                                                                              RG GG BG
                                                                                              RB GB BB*/

    NI_S16 s16OffsetR; /*RW, Ccm模块R基色的偏移量, S8.0, Range:[-256,255] default:0x0*/
    NI_S16 s16OffsetG; /*RW, Ccm模块G基色的偏移量, S8.0, Range:[-256,255] default:0x0*/
    NI_S16 s16OffsetB; /*RW, Ccm模块B基色的偏移量, S8.0, Range:[-256,255] default:0x0*/
} PISP_CCM_PARAM_S;

/**************************************************************************************************
                                            GAMMA
***************************************************************************************************/
typedef struct tagPispGammaTable {
    NI_U8 *pu8GammaTab; /*RW, 伽玛表, 为防止非法指针（例如野指针）导致未知异常发生，请务必对pu8GammaTab指针分配合适的内存空间*/
    NI_U32 u32TabLen;   /*RW, 伽玛表长度, 表长:256, 位宽:U8.0, Range:[0,255]*/
} PISP_GAMMA_TABLE_S;

typedef struct tagPispGammaParam {
    NI_U32             u32GammaAlpha; /*RW, 原始数据和查表数据之间的融合, 值0为查表值, 越大越偏向于原始值, 16为原始值, U1.4 Range:[0,16]*/
    PISP_GAMMA_TABLE_S stGammaTab;    /*RW, Gamma表*/
} PISP_GAMMA_PARAM_S;

/**************************************************************************************************
                                        CSC_CDS
***************************************************************************************************/

typedef struct tagPispCSCBigMatrix {
    NI_S16 as16RGBMatrix[3][3]; /*RW, RGB转YUV系数矩阵里的系数, S3.8,          Range:[-2048,2047] 排布形式:KyR      KyG  KyB
                                                                                                  KcbR KcbG KcbB
                                                                                                  KcrR KcrG KcrB*/
    NI_S16 s16OffsetY;          /*RW, Y信号偏移量, S8.0, Range:[-256,255]*/
    NI_S16 s16OffsetCb;         /*RW, U信号偏移量, S8.0, Range:[-256,255]*/
    NI_S16 s16OffsetCr;         /*RW, V信号偏移量, S8.0, Range:[-256,255]*/
} PISP_CSC_MATRIX_S;

typedef struct tagPispCscCdsParam {
    NI_U32            u32ModuleSel; /*RW, YUV444-YUV422转换模式选择, Range:[0,1] 0:  直接取点 1:byapss, 仅输出v通道数据*/
    PISP_CSC_MATRIX_S stMatrix;     /*RW, 自定义的rgb转y大矩阵, CSC固定使用该矩阵进行色彩空间转换*/
} PISP_CSC_CDS_PARAM_S;

/**************************************************************************************************
                                        BACA
***************************************************************************************************/

typedef struct tagPispBacaCurvY {
    NI_U8 u8Thr; /*RW, 曲线亮度分段控制参数 T: U8.0 Range:[0,127], Offset: Range:[0,127-T]*/
    NI_U8 u8ThrOffset;

    NI_U8 u8Ratio0; /*RW, Baca模块参数, U1.4, R: Range:[0,16]*/
    NI_U8 u8Ratio1;
    NI_U8 u8Ratio2;
    NI_U8 u8Ratio3;
    NI_U8 u8Ratio4;
} PISP_BACA_CURV_Y_S;

typedef struct tagPispBacaCurvUv {
    NI_U8 u8Thr; /*RW, uv曲线亮度分段控制参数 T: U8.0 Range:[0,191], Offset: Range:[0,191-T]*/
    NI_U8 u8ThrOffset;

    NI_U8 u8Ratio0; /*RW, Baca模块参数, U1.4, R: Range:[0,31]*/
    NI_U8 u8Ratio1;
    NI_U8 u8Ratio2;
} PISP_BACA_CURV_UV_S;

typedef struct tagPispBacaParam {
    NI_BOOL             bBacaEn;         /*RW, 亮度色度调整功能使能, Range:[0,1]*/
    NI_U8               u8Luminance;     /*RW, 亮度调节, 亮度进行整体调节参数, 用户可以对亮度分量进行整体调亮、调暗操作. U8.0, Range:[0,255]*/
    NI_U16              u16Contrast;     /*RW, 对比度调节, 整体对比度调节参数. U2.7, Range:[0,0x1ff]*/
    NI_U8               u8AvgUpdateStep; /*RW, 均值迭代更新速度参数, U3.0, Range:[0,7], 最慢2^u8AvgUpdateSetp帧内完成*/
    NI_S8               s8BacaBthr;      /*RW, 亮度提升控制, S6.0 Range:[-64,63]*/
    NI_U8               u8MnDthr;        /*RW, 亮度调整步进, U8.0      , Range:[0,255]*/
    PISP_BACA_CURV_Y_S  stBacaYAdjust;   /*RW, 曲线亮度分段控制参数          */
    PISP_BACA_CURV_UV_S stBacaUvAdjust;  /*RW, Uv曲线亮度分段控制参数*/
    RECT_S              stBacaRect;      /*RW, 统计区域大小*/
} PISP_BACA_PARAM_S;

/**************************************************************************************************
                                              SCC
***************************************************************************************************/

typedef struct tagPispSccUvScurvePar {
    NI_U8 u8Thr1;   /*RW, 增益调节曲线参数, U8.0         1. thr2 != 0, thr2-thr1 = 2, 4, 8, 16, 32, 64,128
                     2. thr2 = 0, 曲线不生效, thr1为任意值*/
    NI_U8 u8Thr2;   /*RW, 增益调节曲线参数, U8.0         限制同Thr1*/
    NI_U8 u8Ratio1; /*RW, 增益调节曲线参数, U2.6 Range:[0,255]*/
} PISP_SCC_UV_RATIO_CURVE_S;

typedef struct tagPispSccModParam {
    NI_BOOL bSccChnEn;        /*RW, 特殊颜色控制使能, Range:[0,1]*/
    NI_U16  u16SccCurveCthr;  /*RW, 特殊颜色选择参数, U8.0, Range:[0,255]*/
    NI_U8   u8SccCurveOffset; /*RW, 特殊颜色控制范围参数, U7.0, Range:[2,127]*/
    NI_U8   u8SccCurveRatio;  /*RW, 特殊颜色增强或抑制强度参数, U2.6, Range:[0,255]*/

    NI_U8                     u8Hthr;         /*RW, 颜色控制参数的阈值, U6.0, Range:[0,63]*/
    PISP_SCC_UV_RATIO_CURVE_S stUvRatioCurve; /*RW, 通道增益调节曲线参数*/
} PISP_SCC_MODULE_PARAM_S;

typedef struct tagPispSccParam {
    NI_BOOL                 bSccEn;         /*RW, Scc模块控制使能, Range:[0,1]*/
    PISP_SCC_MODULE_PARAM_S astModParam[6]; /*RW, 6组特殊颜色的控制参数*/
} PISP_SCC_PARAM_S;

/**************************************************************************************************
                                                CE
***************************************************************************************************/

typedef struct tagPispDfToKs12 {
    NI_U8  u8DfKs1; /*RW, 值越大, 消除光晕效果越好、对比度会下降, U8.0, Range:[0,255]*/
    NI_U8  u8DfKs2; /*RW, 值越大, 消除光晕效果越好、对比度会下降, U8.0, Range:[0,255]*/
    NI_U16 u16DfT0; /*RW, 值越小, 消除光晕效果越好, U4.0, Range:[0,15]*/
} PISP_CE_DF_TO_KS12_S;

typedef struct tagPispDsThr {
    NI_U8  u8DsThr1; /*RW, 小细节对比度增强的阈值t1, U6.0 1:th1 =2, 4, 8, 16, 32 2:th2-th1 = 2, 4, 8, 16, 32*/
    NI_U8  u8DsThr2; /*RW, 小细节对比度增强的阈值t2, U6.0 Range:[4, 63]*/
    NI_U16 u16DsR1;  /*RW, r1大于th1, 减弱对比度; 反之, 增强对比度, U6.0, Range:[0,63]*/
} PISP_CE_DS_THR_S;

typedef struct tagPispCeCurv {
    NI_U8 u8Thr;
    NI_U8 u8ThrOffset;
    NI_U8 u8Ratio1;
    NI_U8 u8Ratio2;
    NI_U8 u8Ratio3;
} PISP_CE_CURV_S;

typedef struct tagPispCeCurvEx {
    NI_U8 u8Thr;
    NI_U8 u8ThrOffset;
    NI_U8 u8Ratio1;
    NI_U8 u8Ratio2;
    NI_U8 u8Ratio3;
    NI_U8 u8Ratio4;
    NI_U8 u8Ratio5;
} PISP_CE_CURV_EX_S;

typedef struct tagPispKrThr {
    NI_U8  u8KrThr1; /*RW, 平坦区噪声强度控制参数T1, T表示想要抹掉的噪声范围 U8.0, T2-T1 = 2, 4, 8, 16, 32, 64, 128; T1 < T2*/
    NI_U8  u8KrThr2; /*RW, 平坦区噪声强度控制参数T2, T表示想要抹掉的噪声范围 U8.0*/
    NI_U16 u16KrR1;  /*RW, 平坦区噪声控制强度, 值越小, 噪声越少, U6.0, Range:[0,63]*/
} PISP_CE_KR_THR_S;

typedef struct tagPispLceParam {
    NI_BOOL              bCeEn;           /*RW, Ce模块控制使能, Range:[0,1]*/
    NI_BOOL              bKrEn;           /*RW, Kr调节控制使能, Range:[0,1]*/
    NI_U8                u8BlkSizeMode;   /*RW, Ce增强模式选择, 值越大, 局部性越好, Range:[0,2]*/
    NI_U8                u8HFiltMode;     /*RW, 滤波模式选择,       值越大, 增强后噪声越少, U2.0, Range:[0,3]*/
    PISP_CE_DF_TO_KS12_S stDfToKs12;      /*RW, Ce对比度增强DF相关参数*/
    PISP_CE_DS_THR_S     stDsThr;         /*RW, Ce小细节增强参数*/
    PISP_CE_KR_THR_S     stKrThr;         /*RW, Ce平坦区噪声强度控制参数*/
    PISP_CE_CURV_S       stNegCurv;       /*RW, Ce负边增强曲线 T: Range:[0,191] Offset: Range:[0,191-T] R:U8.0, Range:[0,255]*/
    PISP_CE_CURV_S       stPosCurv;       /*RW, Ce正边增强曲线 T: Range:[0,191] Offset: Range:[0,191-T] R:U8.0, Range:[0,255]*/
    PISP_CE_CURV_EX_S    stLumCurv;       /*RW, Ce亮度曲线 T: Range:[0,143] Offset: Range:[0,143-T] R:U5.0,Range:[0,16]*/
    PISP_CE_CURV_S       stNegMotionCurv; /*RW, Ce负边Motion增强曲线 T: Range:[0,191] Offset: Range:[0,191-T] R:U8.0, Range:[0,255]*/
    PISP_CE_CURV_S       stPosMotionCurv; /*RW, Ce正边Motion增强曲线 T: Range:[0,191] Offset: Range:[0,191-T] R:U8.0, Range:[0,255]*/
    PISP_CE_CURV_S       stMotionFusCurv; /*RW, Ce正边Motion Fus增强曲线 T: Range:[0,191] Offset: Range:[0,191-T] R:U7.0, Range:[0,64]*/
} PISP_CE_PARAM_S;

/**************************************************************************************************
                                    NRY3D (LC + FULL)
***************************************************************************************************/

typedef struct tagPispNry3dCoef {
    NI_U8 u8MotionIterAlpha; /*RW, Motion滤波混合参数, U1.4, Range:[0,16], default:0x8*/
    NI_U8 u8DiffIterAlpha;   /*RW, diff滤波混合参数,U1.4, Range:[0,16], default:0x8*/
    NI_U8 u8BoundRange;      /*RW, 边界调整范围, U5.0, Range:[0,31]*/
    NI_U8 u8BoundAdjScale;   /*RW, 边界调整力度, U1.4, Range:[0,16]*/
} PISP_NRY3D_COEF_S;

typedef struct tagPispNry3dCurv {
    NI_U8 u8Thr;
    NI_U8 u8ThrOffset;
    NI_U8 u8Ratio1;
    NI_U8 u8Ratio2;
    NI_U8 u8Ratio3;
} PISP_NRY3D_CURV_S;

typedef struct tagPispNry3dMotion {
    PISP_NRY3D_CURV_S stMotionCurv;     /*RW, Motion结构体 T: Range:[0,247] Offset: Range:[0,247-T] R:U8.0 r1>16*/
    PISP_NRY3D_CURV_S stMotionFuseCurv; /*RW, MotionFuse结构体 T: Range:[0,247] Offset: Range:[0,247-t] R:U1.4, Range:[0,16]*/
} PISP_NRY3D_MOTION_S;

typedef struct tagPispLc3d2dY {
    NI_U8 u8Slope;      /*RW, 2D降噪强度, 值越小强度越大 U5.3, Range:[0,255], default 0x80*/
    NI_U8 u8StaticCoef; /*RW, 静止区2D降噪混合系数, U0.8, Range:[0,255], default:0x10*/
    NI_U8 u8MotionCoef; /*RW, 运动区2D降噪混合系数, U0.8, Range:[0,255], default:0x80*/

    NI_U8 u8RefStaticCoef;    /*RW, 参考帧静止区2D降噪混合系数, U0.8, Range:[0,255], default:0x0*/
    NI_U8 u8RefMotionCoef;    /*RW, 参考帧运动区2D降噪混合系数, U0.8, Range:[0,255], default:0x80*/
    NI_U8 u8PreStaticCoef;    /*RW, 前一帧静止区2D降噪混合系数, U0.8, Range:[0,255], default:0x10*/
    NI_U8 u8PreMotionCoef;    /*RW, 前一帧运动区2D降噪混合系数, U0.8, Range:[0,255], default:0x80*/
    NI_U8 u8PreRefStaticCoef; /*RW, 前一帧参考帧静止区2D降噪混合系数,U0.8, Range:[0,255], default:0x0*/
    NI_U8 u8PreRefMotionCoef; /*RW, 前一帧参考帧运动区2D降噪混合系数,U0.8, Range:[0,255], default:0x80*/

    NI_U8             u8CtrstMode;    /*RW, 细节判断模式,值越小细节越多, 但是噪声干扰也会更多. U1.0, Range:[0,1], default:0x1*/
    PISP_NRY3D_CURV_S stY2DLumCurv;   /*RW, Y 2D亮度调节曲线结构体, T: Range:[0,191] Offset: Range:[0,191-T] R:U2.4, Range:[0,63]*/
    PISP_NRY3D_CURV_S stY2DCtrstCurv; /*RW, Y 2D对比度调节调节曲线结构体, T: Range:[0,247] Offset: Range:[0,247-T] R:U2.4, Range:[0,63]*/
} PISP_NRYLC3D_2D_CFG_Y_S;

typedef struct tagPispFull3d2dY {
    NI_U8 u8StaticCoef;    /*RW, 静止区2D降噪混合系数, U0.8, Range:[0,255], default:0x10*/
    NI_U8 u8MotionCoef;    /*RW, 运动区2D降噪混合系数, U0.8, Range:[0,255], default:0x80*/
    NI_U8 u8RefStaticCoef; /*RW, 参考帧静止区2D降噪混合系数, U0.8, Range:[0,255], default:0x0*/
    NI_U8 u8RefMotionCoef; /*RW, 参考帧运动区2D降噪混合系数, U0.8, Range:[0,255], default:0x80*/

    NI_U8 u8PreStaticCoef;    /*RW, 前一帧静止区2D降噪混合系数, U0.8, Range:[0,255], default:0x10*/
    NI_U8 u8PreMotionCoef;    /*RW, 前一帧运动区2D降噪混合系数, U0.8, Range:[0,255], default:0x80*/
    NI_U8 u8PreRefStaticCoef; /*RW, 前一帧参考帧静止区2D降噪混合系数, U0.8, Range:[0,255], default:0x0*/
    NI_U8 u8PreRefMotionCoef; /*RW, 前一帧参考帧运动区2D降噪混合系数, U0.8, Range:[0,255], default:0x80*/

    NI_U8             u8CtrstMode;    /*RW, 细节判断模式,值越小细节越多, 但是噪声干扰也会更多. U1.0, Range:[0,1], default:0x1*/
    PISP_NRY3D_CURV_S stY2DLumCurv;   /*RW, Y 2D亮度调节曲线结构体 T: Range:[0,191] Offset: Range:[0,191-T] R:U2.4, Range:[0,63]*/
    PISP_NRY3D_CURV_S stY2DCtrstCurv; /*RW, Y 2D对比度调节调节曲线结构体 T: Range:[0,239] Offset: Range:[0,239-T] R:U1.4, Range:[0,31]*/
} PISP_FULL3D_2D_CFG_Y_S;

typedef struct tagPispFull3d2dYCur {
    NI_U16 u16PThr;   /*RW, 当前帧/参考帧第1、2级降噪阈值, 第1级:U8.0, Range:[0,255], 第2级:U10.0, Range:[0,0x3ff]*/
    NI_U16 u16PThr1;  /*RW, 当前帧/参考帧第1、2级增强下限阈值, 第1级:U8.0, Range:[0,255], 第2级:U10.0, Range:[0,0x3ff]*/
    NI_U16 u16PThr2;  /*RW, 当前帧/参考帧第1、2级增强上限阈值, 第1级:U8.0, Range:[0,255], 第2级:U10.0, Range:[0,0x3ff], PThr2>=PThr1*/
    NI_U16 u16PRatio; /*RW, 当前帧/参考帧第1、2级降噪后提升系数, U2.0, Range:[0,3]*/
    NI_U16 u16PSlope; /*RW, 当前帧/参考帧第1、2级增强系数, 小于64为降噪, 大于64为增强, U2.6, Range:[0,255]*/
} PISP_FULL3D_2D_CFG_S;

typedef struct tagPispNry3d2dUv {
    NI_U8 u8StaticCoef;    /*RW, 静止区2D降噪混合系数, U0.8,Range:[0,255], default:0x10*/
    NI_U8 u8MotionCoef;    /*RW, 运动区2D降噪混合系数, U0.8,Range:[0,255], default:0x80*/
    NI_U8 u8RefStaticCoef; /*RW, 参考帧静止区2D降噪混合系数, U0.8, Range:[0,255], default:0x0*/
    NI_U8 u8RefMotionCoef; /*RW, 参考帧运动区2D降噪混合系数, U0.8, Range:[0,255],             default:0x80*/

    NI_U8 u8PreStaticCoef;    /*RW, 前一帧静止区2D降噪混合系数, U0.8, Range:[0,255], default:0x10*/
    NI_U8 u8PreMotionCoef;    /*RW, 前一帧运动区2D降噪混合系数, U0.8, Range:[0,255], default:0x80*/
    NI_U8 u8PreRefStaticCoef; /*RW, 前一帧参考帧静止区2D降噪混合系数, U0.8, Range:[0,255], default:0x0*/
    NI_U8 u8PreRefMotionCoef; /*RW, 前一帧参考帧运动区2D降噪混合系数, U0.8, Range:[0,255], default:0x80*/
} PISP_NRY3D_2D_CFG_UV_S;

typedef struct tagPispNryLc3dTail {
    NI_U8 u8MotionTransThr; /*RW, 拖尾Motion处理阈值, U8.0, Range:[0,255], default:0x0*/
    NI_U8 u8MotionJudgeCnt; /*RW, 拖尾收敛, 值越小拖尾越少, 噪声越大, U4.0, Range:[1,15], default:0x1*/
} PISP_NRYLC3D_TAIL_S;

typedef enum tagPispLc3dFltSize {
    PISP_LC3D_SIZE_SINGLE_POINT = 0, /*单点*/
    PISP_LC3D_SIZE_1X3          = 1, /*1x3*/
    PISP_LC3D_SIZE_1X5          = 2, /*1x5*/

    PISP_LC3D_SIZE_BUTT
} PISP_LC3D_FILT_SIZE_E;

typedef enum tagPispLc3dFltSizeV {
    PISP_LC3D_SIZE_SINGLE_POINT_V = 0, /*单点*/
    PISP_LC3D_SIZE_3X1            = 1, /*3x1*/

    PISP_LC3D_SIZE_V_BUTT
} PISP_LC3D_FILT_SIZE_V_E;

typedef struct tagPispNryLc3dDfFusAdj {
    NI_U16                  u16WCoef; /*RW, Y/Uv分量df融合系数,       U1.5, Range:[0,32]*/
    PISP_LC3D_FILT_SIZE_E   enDfSize; /*RW, Y/Uv分量df滤波横向尺寸配置, U2.0, Range:[0,3]*/
    PISP_LC3D_FILT_SIZE_E   enMHSize; /*RW, Y/Uv分量Motion滤波横向尺寸配置, U2.0, Range:[0,3]*/
    PISP_LC3D_FILT_SIZE_V_E enMVSize; /*RW, Y/Uv分量Motion滤波纵向尺寸配置, U1.0, Range:[0,1]*/
} PISP_NRYLC3D_DF_FUS_ADJ_S;

typedef struct tagPispNryLc3dUvTail {
    NI_U8 u8MotionTransThr; /*RW, 拖尾Motion处理阈值, U8.0, Range:[0,255], default:0x60*/
    NI_U8 u8MotionJudgeCnt; /*RW, 拖尾收敛, 值越小拖尾越少, 噪声越大, U4.0, Range:[1,15], default:0x1*/
} PISP_NRYLC3D_UV_TAIL_S;

typedef struct tagPispNryFull3dTail {
    NI_U8 u8MotionTransThr; /*RW, 拖尾Motion处理阈值, U8.0, Range:[0,255], default:0x60*/
    NI_U8 u8MotionJudgeCnt; /*RW, 拖尾收敛, 值越小拖尾越少, 噪声越大, U4.0, Range:[1,15], default:0x1*/
} PISP_NRYFULL3D_TAIL_S;

typedef struct tagPispNryFull3dBlink {
    NI_U8 u8BlinkMT1; /*RW, Y/Uv分量闪烁抑制M阈值1, U8.0, t1>=128*/
    NI_U8 u8BlinkMT2; /*RW, Y/Uv分量闪烁抑制M阈值2, U8.0, t2>=t1 若t2 != t1, 须满足t2-t1为2的N次方（N不为0）*/

    NI_U16 u16BlinkPosR1; /*RW, Y/Uv分量闪烁抑制POS阈值1, U8.0, Range:[0,255]*/
    NI_U16 u16BlinkNegR1; /*RW, Y/UV分量闪烁抑制NEG阈值1, U8.0, Range:[0,255]*/

    NI_U8 u8Th; /*RW, 闪烁抑制阈值, 值越大, 噪声闪烁越小, 太大会导致异常分层, Range:[1,15]*/
} PISP_NRYFULL3D_BLINK_S;

typedef struct tagPispNry3dUvCurv {
    NI_U16 u16Thr1;   /*RW, 特定颜色通道调节饱和度阈值1, U8.0*/
    NI_U16 u16Thr2;   /*RW, 特定颜色通道调节饱和度阈值2, U8.0 (t2- t1)满足2的N次方*/
    NI_U32 u32Ratio1; /*RW, 特定颜色通道运动调节饱和度曲线输出值1, U1.4 Range:[0,16]*/
    NI_U32 u32Ratio2; /*RW, 特定颜色通道运动调节饱和度曲线输出值2, U1.4 Range:[0,16]*/
    NI_U32 u32Ratio3; /*RW, 特定颜色通道运动调节饱和度曲线输出值3, U1.4 Range:[0,16]*/
} PISP_NRY3D_UV_CURV_S;

typedef struct tagPispUvChnParam {
    NI_BOOL              bUvSatChnEn;     /*RW, 特定颜色饱和度调整通道使能, Range:[0,1]*/
    NI_U16               u16UvCurveCthr;  /*RW, 特定颜色通道选择参数, U8.0, Range:[0,255]*/
    NI_U8                u8UvCurveOffset; /*RW, 特定颜色通道选择范围参数, U7.0 Range:[2,127]*/
    NI_U8                u8UvCurveRatio;  /*RW, 特定颜色通道强度, U2.6, Range:[0,255]*/
    NI_U8                u8UvHthr;        /*RW, 特定颜色通道选择范围阈值, U6.0, Range:[0,63]*/
    NI_U8                u8UvClip;        /*RW, 特定颜色通道饱和度调节强度clip值, U1.6, Range:[0,127]*/
    PISP_NRY3D_UV_CURV_S stUvStaCurv;     /*RW, 特定颜色通道UV调节饱和度曲线*/
} PISP_NRYFULL3D_UV_CHN12_S;

typedef struct tagPispUvChn3Param {
    NI_BOOL              bUvSatChnEn;     /*RW, 特定颜色饱和度调整通道使能, Range:[0,1]*/
    NI_U16               u16UvCurveCthr;  /*RW, 特定颜色通道选择参数, U8.0, Range:[0,255]*/
    NI_U16               u8UvCurveOffset; /*RW, 特定颜色通道1选择范围参数, U7.0 Range:[2,127]*/
    PISP_NRY3D_UV_CURV_S stUvStaCurv;     /*RW, 特定颜色通道Uv调节饱和度曲线*/
} PISP_NRYFULL3D_UV_CHN3_S;

typedef struct tagPispNry3dUvChnMSta {
    NI_U8 u8Thr;       /*RW, 特定颜色运动阈值, T: Range:[0,191] Offset: Range:[0,191-T]*/
    NI_U8 u8ThrOffset; /*RW, 特定颜色运动阈值偏移, U8.0, Range:[0,255]*/
} PISP_NRY3D_UV_CHN_M_STA_S;

typedef struct tagPispNryFull3dUvSat {
    PISP_NRY3D_UV_CHN_M_STA_S stUvStaChnM; /*RW, 特定颜色运动阈值*/
    PISP_NRYFULL3D_UV_CHN12_S stUvStaChn1; /*RW, 特定颜色饱和度调整通道1参数*/
    PISP_NRYFULL3D_UV_CHN12_S stUvStaChn2; /*RW, 特定颜色饱和度调整通道2参数*/
    PISP_NRYFULL3D_UV_CHN3_S  stUvStaChn3; /*RW, 特定颜色饱和度调整通道3参数*/
} PISP_NRYFULL3D_UV_SAT_S;

typedef struct tagPispNryFull3dUvGlobSat {
    NI_BOOL           bUvSatGblEn; /*RW, 全局饱和度调整使能, Range:[0,1]*/
    NI_U8             u8UvStaT1;   /*RW, 饱和度调整阈值1, U8.0, Range:[0,255],     default:0x4*/
    NI_U8             u8UvStaT2;   /*RW, 饱和度调整阈值2, U8.0,        Range:[0,255], (UvStaT2- UvStaT1)满足2的N次方,      default:0x8*/
    PISP_NRY3D_CURV_S stUvStaCurv; /*RW, 运动调节Uv饱和度曲线 T: Range:[0,207] Offset: Range:[0,207-T] R:U1.4, Range:[0,31]*/
} PISP_NRYFULL3D_UV_GLOB_SAT_S;

typedef struct tagPispNryFull3dUvCfg {
    NI_BOOL                      bMotionUvMinEn;  /*RW, Uv Motion取小使能, Range:[0,1]*/
    NI_U8                        u8UvAlphaMax;    /*RW, Uv时域强度最大值限幅, U8.0, Range:[0,255],          default:0xf0*/
    NI_U8                        u8UvStr1;        /*RW, Uv时域强度1, 值越大时域越强, U1.5, Range:[0,63],            default:0x20*/
    NI_U8                        u8UvStr2;        /*RW, Uv时域强度2, 值越大时域越强, U1.5, Range:[0,63],            default:0x20*/
    NI_U8                        u8MotionBlendUv; /*RW, Uv时域强度1与UV时域强度2融合系数, 系数越大越偏向强度2, U8.0,                       Range:[0,255], default:0x80*/
    PISP_NRYFULL3D_UV_GLOB_SAT_S stUvGlobSta;     /*RW, 全局饱和度参数配置*/
    PISP_NRYFULL3D_UV_SAT_S      stUvSta;         /*RW, 3D Uv饱和度参数配置 (三个通道)*/
} PISP_NRYFULL3D_UV_CFG_S;

typedef enum tagPispFull3dFltSize {
    PISP_FULL3D_SIZE_SINGLE_POINT = 0, /*单点*/
    PISP_FULL3D_SIZE_1X3          = 1, /*1x3*/
    PISP_FULL3D_SIZE_1X5          = 2, /*1x5*/
    PISP_FULL3D_SIZE_1X7          = 3, /*1x7*/

    PISP_FULL3D_SIZE_BUTT
} PISP_FULL3D_FILT_SIZE_E;

typedef enum tagPispFull3dFltSizeV {
    PISP_FULL3D_SIZE_SINGLE_POINT_V = 0, /*单点*/
    PISP_FULL3D_SIZE_3X1            = 1, /*1x3*/
    PISP_FULL3D_SIZE_5X1            = 2, /*1x5*/

    PISP_FULL3D_SIZE_V_BUTT
} PISP_FULL3D_FILT_SIZE_V_E;

typedef struct tagPispNryFull3dDfFusAdj {
    NI_U8                     u8WCoef1; /*RW, Y分量Df融合系数1,       U1.6, Range:[0,64],(wcoef1 + wcoef2) <= 64*/
    NI_U8                     u8WCoef2; /*RW, Y分量Df融合系数2,       U1.6, Range:[0,64],(wcoef1 + wcoef2) <= 64*/
    PISP_FULL3D_FILT_SIZE_E   enDfSize; /*RW, Y分量Df滤波横向尺寸配置, U2.0, Range:[0,3]*/
    PISP_FULL3D_FILT_SIZE_E   enMHSize; /*RW, Y分量Motion滤波横向尺寸配置, U2.0, Range:[0,3]*/
    PISP_FULL3D_FILT_SIZE_V_E enMVSize; /*RW, Y分量Motion滤波纵向尺寸配置, U2.0, Range:[0,3]*/
} PISP_NRYFULL3D_DF_FUS_ADJ_S;

typedef struct tagPispNryFull3dMAdj {
    NI_U8 u8DfThr1;    /*RW, Y/Uv分量Motion调整df下限阈值,         U8.0, Range:[0,255]*/
    NI_U8 u8DfThr2;    /*RW, Y/Uv分量Motion调整df上限阈值,         U8.0, t2-t1等于2的N次方(N>=0)*/
    NI_U8 u8AdjThr1;   /*RW, Y/Uv分量Motion调整Motion下限阈值,         U8.0, Range:[0,255]*/
    NI_U8 u8AdjThr2;   /*RW, Y/Uv分量Motion调整Motion上限阈值, U8.0          t2-t1等于2的N次方(N>=0)*/
    NI_U8 u8MAdjRatio; /*RW, Y/Uv分量Motion调整比例,       U1.4, Range:[0,16]*/
} PISP_NRYFULL3D_MOTION_ADJ_S;

typedef struct tagPispNryLc3dMAdj {
    NI_U8  u8DfThr1;     /*RW, Y/Uv分量Motion调整df下限阈值,         U8.0, Range:[0,255]*/
    NI_U8  u8DfThr2;     /*RW, Y/Uv分量Motion调整df上限阈值,         U8.0, t2-t1等于2的N次方（N>=0)*/
    NI_U8  u8AdjThr1;    /*RW, Y/Uv分量Motion调整Motion下限阈值,         U8.0, Range:[0,255]*/
    NI_U8  u8AdjThr2;    /*RW, Y/Uv分量Motion调整Motion上限阈值, U8.0,         t2-t1等于2的N次方(N>=0)*/
    NI_U16 u16MAdjRatio; /*RW, Y/Uv分量Motion调整比例,       U1.4, Range:[0,16]*/
} PISP_NRYLC3D_MOTION_ADJ_S;

typedef struct tagPispLc3DYFltAlpha {
    NI_U8 u8MIterAlpha;    /*RW, Motion滤波混合参数, U1.4, Range:[0,16], default:0x8*/
    NI_U8 u8DiffIterAlpha; /*RW, Df滤波混合参数, U1.4, Range:[0,16], default:0x8*/
    NI_U8 u8BoundRange;    /*RW, 边界调整范围, U5.0, Range:[0,31]*/
    NI_U8 u8BoundAdjScale; /*RW, 边界调整力度, U1.4, Range:[0,16]*/
} PISP_LC3D_YUV_FLT_ALPHA_S;

typedef struct tagPispNryLc3dYParam {
    NI_U8                     u8DiffScaleY;     /*RW, Y分量Diff放大比例, U2.0, Range:[0,3], 0:比例为1, 1:比例为2, 2:比例为4, 3:比例为8*/
    PISP_LC3D_YUV_FLT_ALPHA_S stFltAlphaY;      /*RW, Y分量滤波参数配置*/
    PISP_NRY3D_MOTION_S       stMotionY;        /*RW, Y分量 Motion/Fus相关参数配置*/
    PISP_NRY3D_CURV_S         stDfAdjLumY;      /*RW, Y分量DF ADJ参数配置 T: Range:[0,191] Offset: Range:[0,191-T] R:U2.4, Range:[0,63]*/
    PISP_NRYLC3D_2D_CFG_Y_S   stNry2dCfgY;      /*RW, Y分量 2D相关参数配置*/
    NI_U8                     u8DetailNegRatio; /*RW, 细节负边放大系数, U4.4, Range:[0,255], default:0x10*/
    NI_U8                     u8DetailPosRatio; /*RW, 细节正边放大系数, U4.4, Range:[0,255], default:0x10*/
    PISP_NRYLC3D_TAIL_S       stNry3dTailY;     /*RW, Y尾部参数配置*/
    PISP_NRYLC3D_DF_FUS_ADJ_S stDfFusAdjY;      /*RW, Y分量Df融合滤波参数配置*/
    PISP_NRYLC3D_MOTION_ADJ_S stMAdjY;          /*RW, Y分量Motion调整参数配置*/
} PISP_NRYLC3D_Y_PARAM_S;

typedef struct tagPispNryLc3dUvParam {
    NI_U32                    u32DiffScaleUv; /*RW, Uv分量 Df放大比例, U2.0,       Range:[0,3], 0:比例为1, 1:比例为2, 2:比例为4, 3:比例为8*/
    PISP_LC3D_YUV_FLT_ALPHA_S stFltAlphaUv;   /*RW, Uv分量滤波参数配置*/
    PISP_NRY3D_MOTION_S       stMotionUv;     /*RW, Uv分量 Motion/Fus相关参数配置*/
    PISP_NRY3D_2D_CFG_UV_S    stNry2dCfgUv;   /*RW, Uv分量 2D相关参数配置*/
    PISP_NRYLC3D_UV_TAIL_S    stTailMotionUv; /*RW, Uv尾部Motion参数配置*/
    PISP_NRYLC3D_DF_FUS_ADJ_S stDfFusAdjUv;   /*RW, Uv Df融合滤波参数配置*/
    PISP_NRYLC3D_MOTION_ADJ_S stMAdjUv;       /*RW, Uv Motion调整参数配置*/
} PISP_NRYLC3D_UV_PARAM_S;

typedef struct tagPispNryLc3dParam {
    NI_BOOL                 bLc3DEn;           /*RW, Lc3D 总使能, Range:[0,1]*/
    PISP_NRYLC3D_Y_PARAM_S  stNryLc3DYParams;  /*RW, Lc3DY相关参数*/
    PISP_NRYLC3D_UV_PARAM_S stNryLc3DUvParams; /*RW, Lc3DUv相关参数*/
} PISP_NRYLC3D_PARAM_S;

typedef struct tagPispNryFull3dYParam {
    NI_U8                       u8DiffScale;    /*RW, Df放大比例, U2.0, Range:[0,3], 0:比例为1, 1:比例为2, 2:比例为4, 3:比例为8*/
    PISP_NRY3D_COEF_S           stFltAlpha;     /*RW, 滤波参数配置*/
    PISP_NRY3D_MOTION_S         stMotionY;      /*RW, Y分量 Motion/Fus相关参数配置*/
    PISP_NRY3D_CURV_S           stDfAdjLumY;    /*RW, DF ADJ参数配置     T: Range:[0,191] Offset: Range:[0,191-T] R:U2.4, Range:[0,63]*/
    PISP_FULL3D_2D_CFG_Y_S      st2DCfgY;       /*RW, Y分量 2D相关参数配置*/
    PISP_FULL3D_2D_CFG_S        ast2DCfgCur[2]; /*RW, 当前帧第一二级相关参数配置*/
    PISP_FULL3D_2D_CFG_S        ast2DCfgRef[2]; /*RW, 参考帧第一二级相关参数配置*/
    PISP_NRYFULL3D_DF_FUS_ADJ_S stDfFusAdj;     /*RW, Y分量DF融合滤波参数配置*/
    PISP_NRYFULL3D_MOTION_ADJ_S stMAdjY;        /*RW, Y分量Motion调整参数配置*/
    PISP_NRYFULL3D_TAIL_S       stTailMotionY;  /*RW, 尾部Motion参数配置*/
    PISP_NRYFULL3D_BLINK_S      stBlink;        /*RW, Blink参数配置*/
} PISP_NRYFULL3D_Y_PARAM_S;

typedef struct tagPispNryFull3dUvParam {
    PISP_NRYFULL3D_UV_CFG_S     stUvCfg;  /*RW, Uv饱和度参数配置*/
    PISP_NRYFULL3D_MOTION_ADJ_S stMAdjUv; /*RW, Uv分量Motion调整参数配置*/
    PISP_NRYFULL3D_BLINK_S      stBlink;  /*RW, Blink参数配置*/
} PISP_NRYFULL3D_UV_PARAM_S;

typedef struct tagPispNryFull3dParam {
    NI_BOOL                   bFull3DEn;           /*RW, Full3D总使能, Range:[0,1]*/
    NI_BOOL                   bFull3DUvEn;         /*RW, Full3D Uv降噪使能, Range:[0,1], 约束:bFull3DEn处于关闭状态时，bFull3DUvEn不能开启*/
    PISP_NRYFULL3D_Y_PARAM_S  stNryFull3DYParams;  /*RW, Full3DY相关参数*/
    PISP_NRYFULL3D_UV_PARAM_S stNryFull3DUvParams; /*RW, Full3DUv相关参数*/
} PISP_NRYFULL3D_PARAM_S;

/**************************************************************************************************
                                             CNR
***************************************************************************************************/

typedef struct tagPispCnrTR {
    NI_U8 u8Thr;
    NI_U8 u8ThrOffset;
    NI_U8 u8Ratio1;
    NI_U8 u8Ratio2;
    NI_U8 u8Ratio3;
} PISP_CNR_T_R_S;

typedef struct tagPispCnrChnMoR {
    NI_U8 u8Ratio1;
    NI_U8 u8Ratio2;
    NI_U8 u8Ratio3;
} PISP_CNR_CHN_MO_R_S;

typedef struct tagPispCnrChnMoT {
    NI_U8 u8Thr1;
    NI_U8 u8Thr2;
    NI_U8 u8Thr3;
    NI_U8 u8Thr4;
} PISP_CNR_CHN_MO_T_S;

typedef struct tagPispCnrChn {
    NI_BOOL             bUvChnEn;         /*RW, 通道运动饱和调整使能开关, Range:[0,1]*/
    NI_U16              u16ChnCurveCthr;  /*RW, 特殊颜色选择参数, U8.0, Range:[0,255]*/
    NI_U8               u8ChnCurveOffset; /*RW, 特殊颜色控制范围参数, U7.0 Range:[0,127]*/
    NI_U8               u8ChnThr1;        /*RW, 通道Uv阈值控制1, U8.0 Range:[0,128] t1<t2．t2-t1= 2, 4, 8, 16, 32, 64, 128*/
    NI_U8               u8ChnThr2;        /*RW, 通道Uv阈值控制2, U8.0 Range:[0,255] t1<t2．t2-t1= 2, 4, 8, 16, 32, 64, 128*/
    PISP_CNR_CHN_MO_R_S stChnMoRationR;   /*RW, 通道调节强度参数,         U1.4 Range:[0,16]*/
} PISP_CNR_CHN_CTRL_S;

typedef struct tagPispCnrChnPar {
    NI_BOOL             bUvEn;          /*RW, 通道运动饱和调整使能开关, Range:[0,1]*/
    NI_U8               u8UvThr1;       /*RW, Uv阈值控制1, U8.0 Range:[0,128] t1<t2．t2-t1= 2, 4, 8, 16, 32, 64, 128*/
    NI_U8               u8UvThr2;       /*RW, Uv阈值控制2, U8.0 Range:[0,128] t1<t2．t2-t1= 2, 4, 8, 16, 32, 64, 128*/
    PISP_CNR_CHN_MO_T_S stChnMoRationT; /*RW, 通道Motion调节系数, T:U8.0 1.t2-t1和t4-t3 = 2, 4, 8, 16, 32, 64, 128 2.t1<t2<=t3<t4*/
    PISP_CNR_CHN_CTRL_S astChnCtr[2];   /*RW, 通道1、2控制参数*/
} PISP_CNR_CHN_PARAM_S;

typedef struct tagPispCnrFccLum {
    NI_U8  u8Thr1;
    NI_U8  u8Thr2;
    NI_U16 u16Ratio;
} PISP_FCC_LUM_S;

typedef struct tagPispCnrFccHst {
    NI_U8 u8Thr1;
    NI_U8 u8Thr2;
} PISP_FCC_HST_S;

typedef struct tagPispCnrFccHlSat {
    NI_U8  u8Thr1;
    NI_U8  u8Thr2;
    NI_U16 u16Ratio;
} PISP_FCC_HLSAT_S;

typedef struct tagPispCnrFccMst {
    NI_U8 u8Ms3Thr1;
    NI_U8 u8Ms3Thr2;
    NI_U8 u8Ms12Thr1;
    NI_U8 u8Ms12Thr2;
} PISP_FCC_MST_S;

typedef struct tagPispCnrFcc {
    NI_U8            u8FccMod;     /*RW, Y均值的计算方式选择, 1:均值; 0:均值和中心点的最大值 U1.0 Range:[0,1]*/
    NI_U8            u8FccTfThrd;  /*RW, 分块小于阈值的计数和, U3.0, Range:[0,7]*/
    NI_U8            u8FccNumThrd; /*RW, 饱和度阈值的计数阈值, 值越大, 越接近灰色, U4.0, Range:[0,15]*/
    NI_U8            u8FccSatThrd; /*RW, 有色灰偏离阈值, 越小, 颜色块保护越好; 越大, 伪色校正效果越好, U8.0, Range:[0,255]*/
    NI_U8            u8FccYThrd;   /*RW, Y分量亮度均值阈值, U8.0, Range:[0,255]*/
    NI_U8            u8FccCThrd;   /*RW, Y分量对比度阈值, U8.0, Range:[0,255]*/
    PISP_CNR_T_R_S   stFccSTR;     /*RW, 色差控制参数, T: Range:[0,235] Offset: Range:[0,235-T], R:U1.4, Range:[0,16]*/
    PISP_FCC_LUM_S   stFccLum;     /*RW, 亮度控制参数, T: U8.0, 1.T2-T1 = 2, 4, 8, 16, 32, 64,128 2.T1<T2 R:U1.4 Range:[0,16]*/
    PISP_FCC_HST_S   stFccHst;     /*RW, 色差区域饱和度控制参数1, T1:Range:[0,128], T2:Range:[0,255], T2-T1 = 2, 4, 8, 16, 32, 64,128 2, T1<T2*/
    PISP_FCC_HLSAT_S stFccHlSat;   /*RW, 色差区域饱和度控制参数2, T1:Range:[0,128], T2:Range:[0,255], T2-T1 = 2, 4, 8, 16, 32, 64,128 2, T1<T2 R:U1.4 Range:[0,16]*/
    PISP_FCC_MST_S   stFccMst;     /*RW, 特殊色差区域饱和度控制参数      , T1:Range:[0,128], T2:Range:[0,255], T2-T1 = 2, 4, 8, 16, 32, 64,128 2, t1<t2*/
} PISP_CNR_FCC_PARAM_S;

typedef struct tagPispCnrParam {
    NI_BOOL        bCnrEn;       /*RW, Cnr使能开关, Range:[0,1]*/
    NI_BOOL        bColorProtEn; /*RW, 避免颜色溢出开关, Range:[0,1]*/
    NI_BOOL        bFccEn;       /*RW, 伪色去除开关, Range:[0,1]*/
    PISP_CNR_T_R_S stUdTR;       /*RW, U分量的误差控制参数 T: Range:[0,247], Offset: Range:[0,247-T],          R:U1.4 Range:[0,16]*/
    PISP_CNR_T_R_S stVdTR;       /*RW, V分量的误差控制参数 T: Range:[0,247], Offset: Range:[0,247-T], R:U1.4 Range:[0,16]*/
    PISP_CNR_T_R_S stMTR;        /*RW, 静止运动控制参数 T: Range:[0,191], Offset: Range:[0,191-T], R:U1.4 Range:[0,16]*/
    PISP_CNR_T_R_S stCTR;        /*RW, 对比度控制参数 T: Range:[0,247], Offset: Range:[0,247-T], R:U1.4 Range:[0,16]*/
    PISP_CNR_T_R_S stLumTR;      /*RW, 亮度控制参数 T: Range:[0,191], Offset: Range:[0,191-T], R:U1.4 Range:[0,16]*/
    PISP_CNR_T_R_S stCoTR;       /*RW, 颜色保持控制参数 T: Range:[0,239], Offset: Range:[0,239-T], R:U1.4 Range:[0,16]*/
    PISP_CNR_T_R_S stAsTR;       /*RW, 颜色扩散抑制参数 T: Range:[0,247], Offset: Range:[0,247-T], R:U1.4 Range:[0,16]*/
    PISP_CNR_T_R_S stUvPreTR;    /*RW, Pre饱和度调节参数 T: Range:[0,247], Offset: Range:[0,247-T], R:U1.6 Range:[0,127]*/
    PISP_CNR_T_R_S stUvTR;       /*RW, 饱和度调节参数 T: Range:[0,207], Offset: Range:[0,207-T], R:U1.6 Range:[0,127]*/
    PISP_CNR_T_R_S stUvMotionTR; /*RW, Uv Motion控制参数 T: Range:[0,207], Offset: Range:[0,207-T], R:U1.4 Range:[0,16]*/

    PISP_CNR_CHN_PARAM_S stCnrChnParam; /*RW, Cnr通道参数*/
    PISP_CNR_FCC_PARAM_S stCnrFccParam; /*RW, Cnr伪色去除参数*/
} PISP_CNR_PARAM_S;

/**************************************************************************************************
                                              PCR
***************************************************************************************************/

typedef struct tagPispPcrCfg {
    NI_U16 u16PcrCurveCthr;  /*RW, 特殊颜色选择参数, U8.0, Range:[0,255]*/
    NI_U8  u8PcrCurveOffset; /*RW, 特殊颜色控制范围参数, U7.0, Range:[2,127]*/
    NI_U8  u8PcrCurveRatio;  /*RW, 特殊颜色增强或抑制强度参数, U1.6, Range:[0,127]*/
    NI_U8  u8PcrHthr;        /*RW, 通道的颜色控制参数u8PcrCurveOffset的阈值 U6.0, Range:[0,63]*/

    NI_U8 u8Thr1;   /*RW, 特定颜色通道调节饱和度阈值1, U8.0, Range:[0,255]*/
    NI_U8 u8Thr2;   /*RW, 特定颜色通道调节饱和度阈值2, U8.0, 1. t2 != 0时, thr2 - thr1 = 2, 4, 8, 16, 32, 64, 128
                          2. t2 == 0时, 曲线不生效, t1可以为任意值*/
    NI_U8 u8Ratio1; /*RW, 特定颜色通道运动调节饱和度曲线输出值1, U1.5, Range:[0,127]*/
} PISP_PCR_UV_STA_CFG_S;

typedef struct tagPispPcrParam {
    NI_BOOL               bPcrEn;        /*RW, Pcr使能开关, Range:[0,1]*/
    NI_U8                 u8PcrMode;     /*RW, 紫边去除模式, U1.0, Range:[0,1]*/
    NI_U8                 u8PcrHlCnt;    /*RW, Pcr紫边窗上阈值命中数, U7.0, Range:[0,127]*/
    NI_U8                 u8PcrUsCnt;    /*RW, Pcr紫边窗下阈值命中数, U7.0, Range:[0,127]*/
    NI_U8                 u8PcrFloorThr; /*RW, Pcr紫边窗下阈值, U8.0, Range:[0,255], u8PcrTopThr>=u8PcrFloorThr*/
    NI_U8                 u8PcrTopThr;   /*RW, Pcr紫边窗上阈值, U8.0, Range:[0,255],       u8PcrTopThr>=u8PcrFloorThr*/
    NI_U8                 u8PcrDifThr;   /*RW, Pcr紫边窗阈值差控制参数, U8.0, Range:[0,255]*/
    NI_U8                 u8PcrSatThr;   /*RW, Pcr当前点颜色阈值, U8.0, Range:[0,255]*/
    PISP_PCR_UV_STA_CFG_S astPcrChn[2];  /*RW, Pcr通道1、2参数配置*/
} PISP_PCR_PARAM_S;

/**************************************************************************************************
                                            NRY2D
***************************************************************************************************/
typedef struct tagPispNry2dCurv {
    NI_U8 u8Thr;
    NI_U8 u8ThrOffset;
    NI_U8 u8Ratio1;
    NI_U8 u8Ratio2;
    NI_U8 u8Ratio3;
} PISP_NRY2D_CURV_S;

typedef struct tagPispNry2dCurvEx {
    NI_U16 u16Thr;
    NI_U16 u16ThrOffset;
    NI_U8  u8Ratio1;
    NI_U8  u8Ratio2;
    NI_U8  u8Ratio3;
} PISP_NRY2D_CURV_EX_S;

typedef struct tagPispNry2dDns2dP {
    NI_U16 u16Dns2DThr;  /*RW, 第1、2级降噪阈值, 第1级:U8.0, Range:[0,255],         第2级:U10.0, Range:[0,0x3ff]*/
    NI_U16 u16Dns2DThr1; /*RW, 第1、2级增强下限阈值, 第1级:U8.0, Range:[0,255],      第2级:U10.0, Range:[0,0x3ff]*/
    NI_U16 u16Dns2DThr2; /*RW, 第1、2级增强上限阈值, 第1级:U8.0, Range:[0,255],  第2级:U10.0, thr2>=thr1*/
    NI_U8  u8Dns2DRatio; /*RW, 第1、2级降噪后提升系数, U2.0, Range:[0,3]*/
    NI_U8  u8Dns2DSlope; /*RW, 第1、2级增强系数, 小于64为降噪, 大于64为增强, U2.6, Range:[0,255]*/
} PISP_NRY2D_DNS2D_P_S;

typedef struct tagPispNry2dDns2d {
    NI_BOOL              bDns2dEn;        /*RW, DNS2D降噪使能, Range:[0,1]*/
    NI_U32               u32CtrstMode;    /*RW, 细节判断模式, 值越小细节越多, 但是噪声干扰也会更多. U1.0, Range:[0,1]*/
    NI_U8                u8MotionCoef;    /*RW, 运动区2D降噪混合系数, U0.8, Range:[0,255]*/
    NI_U8                u8StaticCoef;    /*RW, 静止区2D降噪混合系数, U0.8, Range:[0,255]*/
    PISP_NRY2D_CURV_S    stDns2dLum;      /*RW, DNS2D亮度调节曲线结构体, T: Range:[0,191], Offset: Range:[0,191-T] R:U2.4, Range:[0,63]*/
    PISP_NRY2D_CURV_S    stDns2dCtrst;    /*RW, DNS2D对比度调节曲线结构体, T: Range:[0,239], Offset: Range:[0,239-T] R:U2.4, Range:[0,63]*/
    PISP_NRY2D_CURV_S    stDns2dMotion;   /*RW, DNS2D运动调节曲线结构体, T: Range:[0,191], Offset: Range:[0,191-T] R:U1.4, Range:[0,16]*/
    PISP_NRY2D_DNS2D_P_S astDns2dPCfg[2]; /*RW, DNS2D第一二级相关参数配置*/
} PISP_NRY2D_DNS2D_PARAM_S;

typedef struct tagPispNry2dPost2dMotion {
    NI_U8 u8TransThr;  /*RW, Motion扩散阈值, 值越大, 运动区域越小, U3.0, Range:[0,7]*/
    NI_U8 u8TransCoef; /*RW, Motion扩散系数, 最大值16, 值越大, 越偏向运动, U5.0, Range:[0,31]*/
    NI_U8 u8ScaleThr;  /*RW, Motion转换阈值, 值越大, 运动区域越大, U4.0, Range:[0,15]*/
    NI_U8 u8ScaleCoef; /*RW, Motion动静转换系数, 最大值16, 值越大, 越运动, U5.0, Range:[0,31]*/
} PISP_NRY2D_POST2D_MOTION_S;

typedef struct tagPispNry2dPost2dY {
    NI_BOOL                    bPost2dEn;           /*RW, Post2d降噪使能, Range:[0,1]*/
    NI_U8                      u8DetailThr;         /*RW, 细节上限阈值, U8.0 Range:[0,223]*/
    NI_U8                      u8Detail2dMotionThr; /*RW, 细节2D去噪运动区判断阈值, U8.0, Range:[0,255]*/
    NI_U8                      u8Detail2dPThr;      /*RW, 细节2D去噪细节判断阈值, U5.0, Range:[0,31]*/
    NI_U8                      u8Detail2dPCoef;     /*RW, 细节2D去噪细节连续性判断系数, U2.3, Range:[0,31]*/
    PISP_NRY2D_CURV_S          stPost2dFreq;        /*RW, Post2d静止区频率调节细节曲线 T: Range:[0,251], Offset: Range:[0,251-T], R:U1.4, Range:[0,16]*/
    PISP_NRY2D_CURV_S          stPost2dMFreq;       /*RW, Post2d运动区频率调节细节曲线 T: Range:[0,247], Offset: Range:[0,247-T], R:U1.4, Range:[0,16]*/
    PISP_NRY2D_CURV_S          stPost2dLum;         /*RW, Post2d亮度调节细节曲线 T: Range:[0,191], Offset: Range:[0,191-T], R:U1.4, Range:[0,16]*/
    PISP_NRY2D_CURV_S          stPost2dM;           /*RW, Post2d运动调节细节曲线 T: Range:[0,207], Offset: Range:[0,207-T], R:U1.4, Range:[0,16]*/
    PISP_NRY2D_CURV_S          stDetail2dM;         /*RW, 运动调节细节2D去噪曲线 T: Range:[0,207], Offset: Range:[0,207-T], R:U0.6, Range:[0,63]*/
    PISP_NRY2D_CURV_S          stPost2dScaleFreq;   /*RW, 静止区频率调节细节增强曲线 T: Range:[0,231], Offset: Range:[0,231-T], R:U2.4, Range:[0,63]*/
    PISP_NRY2D_CURV_S          stPost2dScaleMFreq;  /*RW, 运动区频率调节细节增强曲线 T: Range:[0,231], Offset: Range:[0,231-T], R:U2.4, Range:[0,63]*/
    PISP_NRY2D_CURV_S          stPost2dScaleM;      /*RW, 运动调节细节增强曲线 T: Range:[0,191], Offset: Range:[0,191-T], R:U1.4, Range:[0,16]*/
    PISP_NRY2D_POST2D_MOTION_S stPost2dMotion;      /*RW, Post2d运动扩散和转换系数*/
} PISP_NRY2D_POST2D_Y_PARAM_S;

typedef struct tagPispNry2dPost2dUv {
    NI_BOOL bWorkMode;         /*RW, Uv2d工作模式, 0:强力模式(该结构体参数不生效);1:标准模式(该结构体参数生效) U1.0, Range:[0,1]*/
    NI_BOOL bPost2dUvEn;       /*RW, Post2d Uv降噪使能*/
    NI_U8   u8UvDetailThr;     /*RW, 细节上限阈值, U8.0, Range:[0,223]*/
    NI_U8   u8UvDetail2dP;     /*RW, 平坦区细节降噪混合强度, U0.8, Range:[0,255]*/
    NI_U8   u8UvDetail2dD;     /*RW, 细节区细节降噪混合强度, U0.8, Range:[0,255]*/
    NI_U8   u8UvDetail2dCoef0; /*RW, 细节区细节滤波中心点系数0, U1.4, Range:[0,16],（detail_2d_coef0 + detail_2d_coef1*2)=16*/
    NI_U8   u8UvDetail2dCoef1; /*RW, 细节区细节滤波中心点系数1, U1.4, Range:[0,16],（detail_2d_coef0 + detail_2d_coef1*2)=16*/

    PISP_NRY2D_CURV_S stPost2dUvFreq;  /*RW, Uv 静止区频率调节细节曲线 T: Range:[0,247], Offset: Range:[0,247-T], R:U1.4, Range:[0,16]*/
    PISP_NRY2D_CURV_S stPost2dUvMFreq; /*RW, Uv 运动区频率调节细节曲线 T: Range:[0,247], Offset: Range:[0,247-T], R:U1.4, Range:[0,16]*/
    PISP_NRY2D_CURV_S stPost2dUvLum;   /*RW, Uv 亮度调节细节曲线 T: Range:[0,191], Offset: Range:[0,191-T], R:U1.4, Range:[0,16]*/
    PISP_NRY2D_CURV_S stPost2dUvM;     /*RW, Uv 运动调节细节曲线 T: Range:[0,207], Offset: Range:[0,207-T], R:U1.4, Range:[0,16]*/
    PISP_NRY2D_CURV_S stPost2dChroma;  /*RW, Uv 饱和度调节细节曲线 T: Range:[0,239] Offset: Range:[0,239-T], R:U1.4, Range:[0,16]*/
} PISP_NRY2D_POST2D_UV_PARAM_S;

typedef struct tagPispNry2dPost2d {
    PISP_NRY2D_POST2D_Y_PARAM_S  stPost2dY;  /*RW, post2dY相关参数*/
    PISP_NRY2D_POST2D_UV_PARAM_S stPost2dUv; /*RW, post2dUv相关参数*/
} PISP_NRY2D_POST2D_PARAM_S;

typedef struct tagPispNry2dTCurv {
    NI_U16 u16Thr;
    NI_U16 u16ThrOffset;
} PISP_NRY2D_T_CURV_S;

typedef struct tagPispNry2dRCurv {
    NI_U8 u8Ratio1;
    NI_U8 u8Ratio2;
    NI_U8 u8Ratio3;
} PISP_NRY2D_R_CURV_S;

typedef struct tagPispNry2dY2d {
    NI_BOOL              bY2dEn;           /*RW, Y2d降噪使能, Range:[0,1]*/
    NI_U8                u8Y2dRDe;         /*RW, 力度控制, 判断是否符合树叶特性, U1.4, Range:[0,16]*/
    NI_U16               u16Y2dNoise;      /*RW, Y分量去噪强度参数, U16.0, Range:[0,0xffff]*/
    PISP_NRY2D_CURV_S    stY2dLumCurv;     /*RW, 亮度调节曲线 T: Range:[0,191], Offset: Range:[0,191-T], R:U5.0, Range:[0,16]*/
    PISP_NRY2D_CURV_EX_S stY2dCtrstCurv;   /*RW, 对比度控制调节曲线 T: Range:[0,975], Offset: Range:[0,975-T], R:U5.0, Range:[0,16]*/
    PISP_NRY2D_T_CURV_S  stY2dPosNegTCurv; /*RW, 正负边调节曲线 T: Range:[0,999], Offset: Range:[0,999-T]*/
    PISP_NRY2D_R_CURV_S  stY2dPosRCurv;    /*RW, 正边调节控制强度, R:U5.0, Range:[0,16]*/
    PISP_NRY2D_R_CURV_S  stY2dNegRCurv;    /*RW, 负边调节控制强度, R:U5.0, Range:[0,16]*/
    PISP_NRY2D_CURV_S    stY2dMotionCurv;  /*RW, Motion控制去噪强度曲线, T: Range:[0,191] Offset: Range:[0,191-T], R:U6.0, Range:[0,32]*/
} PISP_NRY2D_Y2D_PARAM_S;

typedef struct tagPispNr2dParam {
    PISP_NRY2D_POST2D_PARAM_S stPost2d;
    PISP_NRY2D_DNS2D_PARAM_S  stDns2d;
    PISP_NRY2D_Y2D_PARAM_S    stY2d;
} PISP_NRY2D_PARAM_S;

/**************************************************************************************************
                                        EE
***************************************************************************************************/

typedef struct tagPispEeCurv {
    NI_U16 u16Thr;
    NI_U16 u16ThrOffset;
    NI_U8  u8Ratio1;
    NI_U8  u8Ratio2;
    NI_U8  u8Ratio3;
} PISP_EE_CURV_S;

/*扩展曲线结构体*/
typedef struct tagPispEeExtCurv {
    NI_U8 u8Thr;
    NI_U8 u8ThrOffset1;
    NI_U8 u8ThrOffset2;
    NI_U8 u8Ratio1;
    NI_U8 u8Ratio2;
    NI_U8 u8Ratio3;
    NI_U8 u8Ratio4;
} PISP_EE_EXT_CURV_S;

typedef struct tagPispTail2dThr {
    NI_U8 u8Hthr1; /* RW, 尾部2d横向阈值1, U8.0, Range:[0,255]*/
    NI_U8 u8Hthr2; /* RW, 尾部2d横向阈值2, U8.0, Range:[0,255]*/
    NI_U8 u8Hthr3; /* RW, 尾部2d横向阈值3, U8.0, Range:[0,255]*/

    NI_U8 u8Vthr1; /* RW, 尾部2d纵向阈值1, U8.0, Range:[0,255]*/
    NI_U8 u8Vthr2; /* RW, 尾部2d纵向阈值2, U8.0, Range:[0,255]*/
    NI_U8 u8Vthr3; /* RW, 尾部2d纵向阈值3, U8.0, Range:[0,255]*/
} PISP_EE_TAIL_2D_THR_S;

typedef struct tagPispTail2d {
    PISP_EE_TAIL_2D_THR_S stThr;       /* RW, 尾部2d横向和纵向阈值 U8.0, Range:[0,255]*/
    PISP_EE_CURV_S        stLumCurv;   /* RW, 亮度调节降噪曲线配置, T: Range:[0,191] Offset: Range:[0,191-T]; R: U1.4 Range:[0,16] */
    PISP_EE_CURV_S        stMCurv;     /* RW, 运动调节降噪曲线配置, T: Range:[0,191] Offset: Range:[0,191-T]; R: U1.4 Range:[0,16] */
    PISP_EE_CURV_S        stFreqCurv;  /* RW, 静止区频率调节降噪曲线配置, T: Range:[0,247] Offset: Range:[0,247-T]; R: U1.4 Range:[0,16] */
    PISP_EE_CURV_S        stMFreqCurv; /* RW, 运动区频率调节降噪曲线配置, T: Range:[0,247] Offset: Range:[0,247-T]; R: U1.4 Range:[0,16] */
} PISP_EE_TAIL_2D_S;

typedef struct tagPispEeParam {
    NI_BOOL bEeEn;        /*RW, Ee使能开关, Range:[0,1]*/
    NI_U8   u8PosEnhMode; /*RW, 正边边缘增强模式, U2.0, Range:[0,3], 0:正常模式 1:宽边模式 2,3:窄边模式*/
    NI_U8   u8NegEnhMode; /*RW, 负边边缘增强模式, U2.0, Range:[0,3], 0:正常模式 1:宽边模式 2,3:窄边模式*/

    NI_U8  u8PosRatio;        /*RW, 正边增强系数,U2.4, Range:[0,63]*/
    NI_U8  u8NegRatio;        /*RW, 负边增强系数,U2.4, Range:[0,63]*/
    NI_U16 u16DetailPosScale; /*RW, 正边细节提取放大倍数, U4.5, Range:[0,0x1ff]*/
    NI_U16 u16DetailNegScale; /*RW, 负边细节提取放大倍数, U4.5, Range:[0,0x1ff]*/

    NI_U8 u8CtrSMode; /*RW, 对比度计算模式 U2.0, Range:[0,3], 值越大, 抗噪声干扰强度越强*/
    NI_U8 u8CtrMMode; /*RW, 对比度计算模式 U2.0, Range:[0,3]*/
    NI_U8 u8CtrDMode; /*RW, 对比度计算模式 U2.0, Range:[0,3]*/

    PISP_EE_EXT_CURV_S stFreqPosCurv; /*RW, 静止区正边频率调节曲线配置, T: Range:[0,203] Offset1: Range:[0,203-T] Offset2: Range:[0,203-T-Offset1]; R: U2.4 Range:[0,63]*/
    PISP_EE_EXT_CURV_S stFreqNegCurv; /*RW, 静止区负边频率调节曲线配置, T: Range:[0,203] Offset1: Range:[0,203-T] Offset2: Range:[0,203-T-Offset1]; R: U2.4 Range:[0,63]*/
    PISP_EE_EXT_CURV_S stMFreqCurv;   /*RW, 运动区频率调节曲线配置, T: Range:[0,231] Offset1: Range:[0,231-T] Offset2: Range:[0,231-T-Offset1];R: U2.4 Range:[0,63]*/
    PISP_EE_EXT_CURV_S stLumPosCurv;  /*RW, 正边亮度调节曲线配置, T: Range:[0,207] Offset1: Range:[0,207-T] Offset2: Range:[0,207-T-Offset1]; R: U2.4 Range:[0,63]*/
    PISP_EE_EXT_CURV_S stLumNegCurv;  /*RW, 负边亮度调节曲线配置, T: Range:[0,191] Offset1: Range:[0,191-T] Offset2: Range:[0,191-T-Offset1]; R: U2.4 Range:[0,63]*/
    PISP_EE_CURV_S     stMCurv;       /*RW, 运动调节细节曲线配置, T: Range:[0,191] Offset: Range:[0,191-T]; R:, U1.4 Range:[0,16]*/

    NI_U8 u8DetailJudgeThr;    /*RW, 细节提取时判断阈值, U4.0, Range:[0,15]*/
    NI_U8 u8DetailJudgeKRatio; /*RW, 边缘判断阈值, u4.4, Range:[0,255]*/

    PISP_EE_CURV_S stPosClipCurv;   /*RW, 正边限幅参数曲线配置, T: Range:[0,223] Offset: Range:[0,223-T]; R: U2.4 Range:[0,63]*/
    PISP_EE_CURV_S stNegClipCurv;   /*RW, 负边限幅参数曲线配置, T: Range:[0,223] Offset: Range:[0,223-T]; R: U2.4 Range:[0,63]*/
    NI_U8          u8PosClipOffset; /*RW, 正边限幅参数配置, U5.0, Range:[0,31]*/
    NI_U8          u8NegClipOffset; /*RW, 负边限幅参数配置, U5.0, Range:[0,31]*/

    NI_U8  au8DirRatio[8]; /*RW, 方向增强系数, U1.4, Range:[0,16] 沿着逆时针方向分别是:0°, 45°, 90°, 135°, 180°, 225°, 270°, 315°*/
    NI_U8  u8HfRatio;      /*RW, 小细节整体增强系数, U1.4, Range:[0,31]*/
    NI_U16 u16DltUpTh;     /*RW, 大边阈值, U9.0, Range:[0,0x1ff]*/
    NI_U16 u16DltDownTh;   /*RW, 小边阈值, U9.0, Range:[0,0x1ff]*/

    PISP_EE_CURV_S    stDetailMaxDltScale;     /*RW, 强边缘增强曲线配置,          T: Range:[0,495] Offset: Range:[0,495-T]; R: U2.4 Range:[0,63]*/
    PISP_EE_CURV_S    stMoreDetailPosScale;    /*RW, 小细节正边增强曲线配置,   T: Range:[0,231] Offset: Range:[0,231-T]; R: U2.4 Range:[0,63]*/
    PISP_EE_CURV_S    stMoreDetailNegScale;    /*RW, 小细节负边增强曲线配置, T: Range:[0,231] Offset: Range:[0,231-T]; R: U2.4 Range:[0,63]*/
    PISP_EE_CURV_S    stMoreDetailScaleMotion; /*RW, 小细节运动静止增强曲线配置, T: Range:[0,191] Offset: Range:[0,191-T]; R: U1.4 Range:[0,31]*/
    PISP_EE_TAIL_2D_S stTail2dCfg;             /* RW, 后置2D降噪参数配置*/
} PISP_EE_PARAM_S;

/**************************************************************************************************
                                       SCE
***************************************************************************************************/
typedef struct tagPispDetailCurv {
    NI_U16 u16Thr;
    NI_U16 u16ThrOffset;
    NI_U8  u8Ratio1;
    NI_U8  u8Ratio2;
    NI_U8  u8Ratio3;
} PISP_DETAIL_CURV_S;

// 特定颜色细节增强, 说明EE和CNR使能打开时, 如下参数才生效
typedef struct tagPispSpecDetailEnh {
    NI_U16             u16ColorCurveCthr;  /*RW, 特殊颜色选择参数, U8.0, Range:[0,255]*/
    NI_U8              u8ColorCurveOffset; /*RW, 特殊颜色控制范围参数, U7.0, Range:[0,127]*/
    NI_U8              u8ColorSat;         /*RW, 饱和度阈值参数, U8.0, Range:[0,255]*/
    NI_U8              u8ColorNumThr;      /*RW, 阈值参数, U4.0, Range:[0,9]*/
    NI_U8              u8DetailAddPThr;    /*RW, 细节增强阈值, U8.0, Range:[0,255]*/
    NI_U8              u8PosDetailAddTh;   /*RW, 细节增强正边阈值, U8.0, Range:[0,255]*/
    NI_U8              u8NegDetailAddTh;   /*RW, 细节增强负边阈值, U8.0, Range:[0,255]*/
    PISP_DETAIL_CURV_S stDetailAddCurv;    /*RW, 细节增强曲线配置, T: Range:[0,251], Offset: Range:[0,251-T]; R: U1.4 Range:[0,16]*/
    PISP_DETAIL_CURV_S stDetailAddMCurv;   /*RW, 细节增强运动曲线配置, T: Range:[0,191], Offset: Range:[0,191-T]; R: U1.4 Range:[0,16]*/
} PISP_SCE_PARAM_S;

/**************************************************************************************************
                                        YCONTRAST
***************************************************************************************************/

typedef struct tagPispYTab {
    NI_U32 u32TableLen; /*RW, 直方图映射表长度*/
    NI_U8 *pu8Table;    /*RW, 直方图统计映射表数据, 为防止非法指针（例如野指针）导致未知异常发生，请务必对pu8Table指针分配合适的内存空间*/
} PISP_YCONTRAST_TABLE_S;

typedef struct tagPispYContrast {
    NI_BOOL                bContrastEn;   /*RW, 是否使能直方图统计配置映射表来调节对比度*/
    PISP_YCONTRAST_TABLE_S stContrastTab; /*RW, 对比度映射表, 表长256, 表宽 U8.0, Range:[0,255]*/
} PISP_YCONTRAST_S;

/**************************************************************************************************
                                        YGAMMA
***************************************************************************************************/
typedef struct tagPispYGammaTab {
    NI_U32 u32TableLen; /*RW, YGamma表长度*/
    NI_U8 *pu8Table;    /*RW, YGamma表数据, 为防止非法指针（例如野指针）导致未知异常发生，请务必对pu8Table指针分配合适的内存空间*/
} PISP_YGAMMA_TABLE_S;

typedef struct tagPispYGamma {
    NI_BOOL             bYGammaEn;   /*RW, 是否使能配置亮度表来调节亮度*/
    NI_U32              u32Alpha;    /*RW, 表融合系数, 该系数越小Gamma提亮效果越大, Range:[0,16]*/
    PISP_YGAMMA_TABLE_S stYGammaTab; /*RW, 亮度映射表, 表长256, 表宽 U8.0, Range:[0,255]*/
} PISP_YGAMMA_S;

/**************************************************************************************************
                                        YHIST
***************************************************************************************************/
typedef struct tagPispYhistParam {
    NI_BOOL bYhistEn;       /*RW, Yhist使能, Range:[0,1]*/
    NI_U32  u32MinPercMend; /*RW, 直方图统计设定的最小值边界, 从0开始扣除u32MinPercMend个数量的点不参与计算, U22.0,Range:[0,0x7fffff], default:0x0*/
    NI_U32  u32MaxPercMend; /*RW, 直方图统计设定的最大值边界, 从0开始统计u32MaxPercMend个数量的点进行计算多余数量扣除. 即总参与统计数量为:
                                  u32MaxPercMend-u32MinPercMend U22.0,Range:[0,0x7fffff], default:0x0*/
    NI_U32 u32Frate;        /*RW, 统计帧率,隔u32Frate帧统计一次, Range:[0,255], default 0*/
    RECT_S stStatArea;      /*RW. 统计区域的大小*/
} PISP_YHIST_PARAM_S;

typedef struct tagPispYhistStatistics {
    NI_U32  u32YhistMin;   /*RO, 直方图统计最小值*/
    NI_U32  u32YhistMax;   /*RO, 直方图统计最大值*/
    NI_U32  u32YhistAvg;   /*RO, 直方图统计得到亮度平均值*/
    NI_U32 *pu32YhistData; /*RO, 直方图统计数据, 数据位宽为22, 数据个数为256, 即0-255*/
    NI_U32  u32PhyAddr;    /*RO, pu32YhistData对应的物理地址*/
    NI_U32  u32MbufBlkId;  /*RO, pu32YhistData对应缓存块的id号*/
} PISP_YHIST_STATISTICS_S;

/**************************************************************************************************
                                             AWES
***************************************************************************************************/

/*定义AWES统计位置*/
typedef enum tagPispAwesPosition {
    PISP_AWES_FROM_FRAME_L         = 0, /*只统计长曝光帧, 仅宽动态模式下支持, AX520支持*/
    PISP_AWES_FROM_FRAME_S         = 1, /*只统计短曝光帧, 支持线性和宽动态模式, AX520仅支持线性模式*/
    PISP_AWES_FROM_FRAME_COMPO     = 2, /*只统计合成帧, 支持线性和宽动态模式, AX520仅支持宽动态模式*/
    PISP_AWES_FROM_FRAME_L_S       = 3, /*统计长、短两帧统计数据, 仅宽动态模式下支持, AX520不支持*/
    PISP_AWES_FROM_FRAME_L_COMPO   = 4, /*统计长、合成两帧统计数据,仅宽动态模式下支持 AX520不支持*/
    PISP_AWES_FROM_FRAME_S_COMPO   = 5, /*统计短、合成两帧统计数据,支持线性和宽动态模式 AX520不支持*/
    PISP_AWES_FROM_FRAME_L_S_COMPO = 6, /*统计长、短、合成三帧统计数据,仅宽动态模式下支持 AX520不支持*/
    PISP_AWES_POS_BUTT
} PISP_AWES_POS_E;

typedef struct tagPispAwesParam {
    PISP_AWES_POS_E enAwesPos;       /*RW, AWES统计数据来源位置 */
    NI_U16          u16BlockHSize;   /*RW, 2A统计块横向大小, 与统计区域横向大小有关, 值为16的倍数, Range:[16, 64], 统计区域内横向分块个数最大为82个*/
    NI_U16          u16BlockVSize;   /*RW, 2A统计块纵向大小, 值为8的倍数, Range:[8, 64]*/
    RECT_S          stStatisticArea; /*RW, 2A统计模块统计区域*/
} PISP_AWES_PARAM_S;

/*定义一帧图像的AWES统计值信息结构体*/
typedef struct tagPispAwesData {
    NI_U32 *pu32AwesData;   /*RO, AWES统计的rgby值, 数据从低地址向高地址排列:R0[10]/G0[10]/B0[10]/Y0[10]...Rn[10]/Gn[10]/Bn[10]/Yn[10], n为当前分块数, 即总大小为:4*(40*n/32) bytes*/
    NI_U32  u32AwesPhyAddr; /*RO, pu16AwesData对应的物理地址*/
} PISP_AWES_DATA_S;

typedef struct tagPispAwesStatistics {
    NI_U32 u32BlockNum;  /*RO, AWES统计一帧的分块总数*/
    NI_U32 u32HBlockNum; /*RO, AWES统计水平方向分块个数*/
    NI_U32 u32VBlockNum; /*RO, AWES统计垂直方向分块个数*/

    NI_U32           u32AwesMbufBlkId; /*RO, pu16AwesData对应缓存块的id号*/
    PISP_AWES_POS_E  enAwesPos;        /*RO, 指示当前AWES的统计数据来源*/
    PISP_AWES_DATA_S astAwesData[3];   /*RO, AWES统计数据, 硬件一次固定一帧数据,存放在下标为0的的数组中*/
} PISP_AWES_STATISTICS_S;

/**************************************************************************************************
                                             RAWHIST
***************************************************************************************************/
typedef struct tagPispRawHistData {
    NI_U32 *pu32RawHistData; /*RO, RAWHIST统计的rgby值, 每帧统计数据:R/G/B/Y 的位宽均为16, 数据按照 R[128]/G[128]/B[128]/Y[256]顺序排列. 即总大小为:2* (128*3+256)bytes*/
    NI_U32  u32HistPhyAddr;  /*RO, apu32RawHistData对应的物理地址*/
} PISP_RAWHIST_DATA_S;

/*定义RAWHIST统计位置*/
typedef enum tagPispRawHistPosition {
    PISP_RAWHIST_FROM_FRAME_L             = 0, /*只统计长曝光帧, 仅宽动态模式下支持, AX520不支持*/
    PISP_RAWHIST_FROM_FRAME_S             = 1, /*只统计短曝光帧, 支持线性和宽动态模式, AX520仅支持线性模式*/
    PISP_RAWHIST_FROM_FRAME_COMPO         = 2, /*只统计合成帧, 支持线性和宽动态模式, AX520仅支持宽动态模式*/
    PISP_RAWHIST_FROM_FRAME_L_S           = 3, /*统计长、短两帧统计数据, 仅宽动态模式下支持, AX520不支持*/
    PISP_RAWHIST_FROM_FRAME_L_COMPO       = 4, /*统计长、合成两帧统计数据,仅宽动态模式下支持, AX520不支持*/
    PISP_RAWHIST_FROM_FRAME_S_COMPO       = 5, /*统计短、合成两帧统计数据,支持线性和宽动态模式, AX520不支持*/
    PISP_RAWHIST_FROM_FRAME_L_S_COMPO     = 6, /*统计长、短、合成三帧统计数据,仅宽动态模式下支持, AX520不支持*/
    PISP_RAWHIST_FROM_FRAME_L_S_ALTERNATE = 7, /*统计长、短两帧交替统计数据, 仅支持宽动态模式, AX520支持*/

    PISP_RAWHIST_POS_BUTT
} PISP_RAWHIST_POS_E;

typedef struct tagPispRawHistParam {
    NI_BOOL            bRawHistEn;   /*RW, raw域直方图统计功能使能, Range:[0,1]*/
    PISP_RAWHIST_POS_E enRawHistPos; /*RW, RawHist统计数据来源位置*/
} PISP_RAWHIST_PARAM_S;

typedef struct tagPispRawHistStatistics {
    NI_U32              u32HistMbufBlkId;  /*RO, apu32RawHistData对应缓存块的id号*/
    NI_U32              u32HdrOutputLS;    /*RO, 指示宽动态长短帧交替模式下, 当前输出的是长帧还是短帧, 0:短帧, 1:长帧*/
    PISP_RAWHIST_POS_E  enRawHistPos;      /*RO, 指示当前RawHist的统计数据来源*/
    PISP_RAWHIST_DATA_S astRawHistData[3]; /*RO, RawHist统计数据, 硬件一次固定一帧数据, 存放在下标为0的的数组中*/
} PISP_RAWHIST_STATISTICS_S;

/**************************************************************************************************
                                             AF
***************************************************************************************************/
/*均值滤波（A）和中值滤波（B）的次序及该功能块是否开启的组合:A0B0  A0B1  A1B0  A1B1  B1A1 （共五种组合）. 滤波类型决定图像下采样后舍弃的下采样图像的上下左右的行数与点数*/
typedef enum tagPispAfsFilterType {
    PISP_AF_FILTER_SEQ_A1B1 = 0, /*先均值滤波再中值滤波. 下采样后AF统计区域:x < W-10 y < H-10*/
    PISP_AF_FILTER_SEQ_A1B0 = 1, /*仅均值滤波. 下采样后AF统计区域:x < W-8 y < H-8*/
    PISP_AF_FILTER_SEQ_A0B0 = 2, /*中值滤波和均值滤波都关闭. 下采样后AF统计区域:x < W-6 y < H-6*/
    PISP_AF_FILTER_SEQ_A0B1 = 3, /*仅中值滤波. 下采样后AF统计区域:x < W-8 y < H-8*/
    PISP_AF_FILTER_SEQ_B1A1 = 4, /*先中值滤波再均值滤波. 下采样后AF统计区域:x < W-10 y < H-10*/
    PISP_AF_FILTER_SEQ_BUTT
} PISP_AF_FILTER_TYPE_E;

/*灯光块与fswm块（算子块）宽度（高度）的比例. 即 LambBlock/FswmBlock = ISP_AFS_LAMB_FSWM_RATIO_E*/
typedef enum tagPispAfsLambFswmRatio {
    PISP_AF_LAMB_FSWM_RATIO_ONE   = 1,
    PISP_AF_LAMB_FSWM_RATIO_TWO   = 2, /*当大于2时, 必须关闭偏移*/
    PISP_AF_LAMB_FSWM_RATIO_THREE = 3,
    PISP_AF_LAMB_FSWM_RATIO_FOUR  = 4,
    PISP_AF_LAMB_FSWM_RATIO_FIVE  = 5,
    PISP_AF_LAMB_FSWM_RATIO_SIX   = 6,
    PISP_AF_LAMB_FSWM_RATIO_SEVEN = 7,
    PISP_AF_LAMB_FSWM_RATIO_EIGHT = 8,
    PISP_AF_LAMB_FSWM_RATIO_BUTT
} PISP_AF_LAMB_FSWM_RATIO_E;

/*fswm和灯光块的宽高类型*/
typedef enum tagPispAfsBlkHWType {
    PISP_AF_BLK_H16XW32     = 0,
    PISP_AF_BLK_H32XW64     = 1,
    PISP_AF_BLK_H64XW128    = 2,
    PISP_AF_BLK_H128XW256   = 3,
    PISP_AF_BLK_H256XW512   = 4,
    PISP_AF_BLK_H512XW1024  = 5,
    PISP_AF_BLK_H1024XW2048 = 6,
    PISP_AF_BLK_HW_BUTT
} PISP_AF_BLK_HW_TYPE_E;

typedef struct tagPispAfsAntiLamp {
    NI_BOOL bAntiLampLight; /*RW, 去灯光使能, Range:[0,1]*/
    NI_U32  u32LampLevel;   /*RW, 去灯光参数, Range:[0,255]*/
    NI_U32  u32MaxLevel;    /*RW, 去灯光参数, Range:[0,255]*/
    NI_U32  u32MidLevel;    /*RW, 去灯光参数, Range:[0,255]*/
    NI_U32  u32KLevel;      /*RW, 去灯光参数, Range:[0,255]*/
} PISP_AF_ANTI_LAMP_S;

typedef enum tagPispAfSrcSel {
    PISP_AF_SRC_SEL_DRC_E = 0, /*DRC处理后的Y输入*/
    PISP_AF_SRC_SEL_EE_E  = 1, /*EE处理后的Y输入*/
    PISP_AF_SRC_SEL_BUTT
} PISP_AF_SRC_SEL_E;

typedef enum tagPispAfOutputSel {
    PISP_AF_OUTPUT_SEL_DDR_E    = 0, /*AF数据上DDR*/
    PISP_AF_OUTPUT_SEL_BYPASS_E = 1, /*AF数据直接送到vps, 作为附加信息*/
    PISP_AF_OUTPUT_SEL_BUTT
} PISP_AF_OUTPUT_SEL_E;

typedef struct tagPispBlkSize {
    NI_U16 u16BlkWidth;
    NI_U16 u16BlkHeight;
} PISP_BLK_SIZE_S;

typedef struct tagPispAfMeanFilterCoe {
    NI_U32 u32CoeCenter; /*中心点权重*/
    NI_U32 u32CoeEdge;   /*边界点权重*/
    NI_U32 u32CoeCorner; /*角落点权重*/
} PISP_AF_MEAN_FILTER_COE_S;

typedef struct tagPispAfMediumFilterCoe {
    NI_U32 u32CoeMedium01; /*极值与次极值之间的噪声判断阈值*/
    NI_U32 u32CoeMedium02; /*极值与次次极值之间的噪声判断阈值*/
    NI_U32 u32CoeMedium12; /*次极值与次次极值之间的噪声判断阈值*/
} PISP_AF_MEDIUM_FILTER_COE_S;

typedef struct tagPispAfsParam {
    NI_BOOL                     bEnable;             /*RW, AF使能, Range:[0,1]*/
    NI_BOOL                     bOffsetEn;           /*RW, 偏移使能, Range:[0,1]*/
    PISP_BLK_SIZE_S             stImgPix;            /*RW, Raw转Y输入到AF模块的宽高*/
    PISP_AF_FILTER_TYPE_E       enFilterType;        /*RW, 中值滤波和均值滤波是否开启的组合:A1B1                 A1B0 A0B0 A0B1 B1A1. 0-4, 其它情况为错误的配置.*/
    RECT_POS_S                  stRectPos;           /*RW, 设置统计区域的坐标*/
    PISP_AF_LAMB_FSWM_RATIO_E   enLambFswmRatio;     /*RW, 灯光块Lamb与fswm块（算子块）的块个数划分比例. 即 LambBlock/FswmBlock = ISP_AF_LAMB_FSWM_RATIO_E*/
    PISP_BLK_SIZE_S             stFswmBlkSize;       /*RW, 算子块的大小, 宽需要32的倍数, 高需要16的倍数*/
    PISP_AF_ANTI_LAMP_S         stAntiLamp;          /*RW, 去灯光参数*/
    NI_BOOL                     bAfDownSampleEn;     /*RW, AF下采样使能*/
    PISP_AF_MEAN_FILTER_COE_S   stAfMeanFilterCoe;   /*RW, AF均值滤波系数*/
    PISP_AF_MEDIUM_FILTER_COE_S stAfMediumFilterCoe; /*RW, AF中值滤波系数*/
} PISP_AF_PARAM_S;

typedef struct tagPispAfsStatistics {
    NI_U32 u32TotalValue;  /*RO, 统计的最终结果（满足条件的所有灯光块内fswm的累加值）*/
    NI_U32 u32VaildBlkSum; /*RO, 有效块数:去灯光使能开启时, 经过去灯光后af块数；否则, 有效区域的块数*/
    NI_U32 u32LampBlkSum;  /*RO, 灯光块数:灯光块数个数（亮度上限值为max_level.lamb块四均分块的均值最大值大于max_level的灯光块个数总和（横向和纵向）. 与lamb_switch是否开启无关）*/
    NI_U32 u32FswmVBlkSum; /*RO, AF总块数（有效区域的块数, 有偏移时, fswm纵向帧）不计块偏移带来的影响 （理论上横向和纵向块数应该一致）*/
    NI_U32 u32FswmHBlkSum; /*RO, AF总块数（有效区域的块数, fswm横向）*/
    NI_U32 u32LambVBlkSum; /*RO, AF总块数（有效区域的块数, 有偏移时, lamb纵向帧）（使用除lamp_level之外的阈值得到的统计）*/
    NI_U32 u32LambHBlkSum; /*RO, AF总块数（有效区域的块数, lamb横向）*/
    NI_U32 u32FswmHBlk;    /*RO, 实际处理每行的fswm块数（第一行）, 根据横向统计*/
    NI_U32 u32LambHBlk;    /*RO, 实际处理每行的灯光块数（第一行）, 根据横向统计*/
    NI_U32 u32AfMbufBlkId; /*RO, pu32AfData对应缓存块的id号*/

    NI_U32  u32PhyAddr; /*应用层不使用*/
    NI_U16 *pu16AfData; /*RO, Af统计数据 统计值空间排序:16*span*(1 + bOffsetEn) (bytes)的fswm（开偏移, fswm分横、纵）、 8*span*(1 + bOffsetEn) (bytes)的mean（开偏移, mean分横、纵）、1*span*(1 + bOffsetEn) (bytes)的lamb_flag（开偏移, lamb_flag分横、纵） ,其中span =576(AX520)
                           其中fswm分量占2Bytes, mean占1Byte, lamb_flag占1bit.*/
} PISP_AF_STATISTICS_S;

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif
