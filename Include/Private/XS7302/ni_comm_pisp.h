/* 
 * ni_comm_pisp.h -- pisp模块对外提供的头文件
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */


#ifndef __NI_COMM_PISP_H__
#define __NI_COMM_PISP_H__


#include "ni_common.h"
#include "ni_define.h"
#include "ni_macro.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/* invlalid device ID */
#define NI_ERR_PISP_INVALID_DEVID           NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_INVALID_DEVID)
/* invlalid channel ID */
#define NI_ERR_PISP_INVALID_SNSID           NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_INVALID_CHNID)
/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define NI_ERR_PISP_ILLEGAL_PARAM           NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)
/* lib exists */
#define NI_ERR_PISP_LIB_EXIST               NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_EXIST)
/* lib unexists */
#define NI_ERR_PISP_LIB_UNEXIST             NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_UNEXIST)
/* using a NULL point */
#define NI_ERR_PISP_NULL_PTR                NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)
/* try to enable or initialize system,device or channel, before configing attribute */
#define NI_ERR_PISP_NOT_CONFIG              NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_NOT_CONFIG)
/* operation is not supported by NOW */
#define NI_ERR_PISP_NOT_SUPPORT             NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change static attribute */
#define NI_ERR_PISP_NOT_PERM                NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_NOT_PERM)
/* failure caused by malloc memory */
#define NI_ERR_PISP_NOMEM                   NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_NOMEM)
/* failure caused by malloc buffer */
#define NI_ERR_PISP_NOBUF                   NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_NOBUF)
/* no data in buffer */
#define NI_ERR_PISP_BUF_EMPTY               NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_BUF_EMPTY)
/* no buffer for new data */
#define NI_ERR_PISP_BUF_FULL                NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_BUF_FULL)
/* not initialize */
#define NI_ERR_PISP_NOT_INIT                NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_SYS_NOTREADY)
/* not initialize */
#define NI_ERR_PISP_HAS_INIT                NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_EXIST)
/* not register sensor callback */
#define NI_ERR_PISP_SNS_UNREGISTER          NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_UNEXIST)
#define NI_ERR_PISP_ALG_LIB_UNREGISTER      NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_UNEXIST)
#define NI_ERR_PISP_REQUEST_IRQFAIL         NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_ISP_IRQ_FAIL)
#define NI_ERR_PISP_TIMEOUT                 NI_DEF_ERR(NI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_TIME_OUT)

#define PISP_ENABLE                      (1)
#define PISP_DISABLE                     (0)

/* 定义AWES和RAWHIST 类型*/
#define PISP_AWES_TYPE                   (0)
#define PISP_RAWHIST_TYPE                (1)

/*定义awes统计硬件处理有长曝光、短曝光、融合帧三种类型，其中长短共用一个处理单元，融合帧单独处理*/
#define PISP_AWES_FRM_NUM                (3)
#define PISP_AWES_CHN_NUM                (2)

/*定义rawhist统计硬件处理有长曝光、短曝光、融合帧三种类型，其中长短共用一个处理单元，融合帧单独处理*/
#define PISP_RAWHIST_FRM_NUM             (3)
#define PISP_RAWHIST_CHN_NUM             (2)

#define AWES_USE_BANK_NUM                (2)
#define RAWHIST_USE_BANK_NUM             (2)

/**************************************************************************************************
                                            BLC   
**************************************************************************************************/
/* 定义黑电平校正各分量偏移值结构体 */
typedef struct tagPispBlackLevel
{
    NI_U16           u16BlcR;               /*RW, R像素黑电平偏移值, U10.2,rang[0,4095], default:0x0*/
    NI_U16           u16BlcGr;              /*RW, Gr像素黑电平偏移值, U10.2,rang[0,4095], default:0x0*/
    NI_U16           u16BlcGb;              /*RW, Gb像素黑电平偏移值, U10.2,rang[0,4095], default:0x0*/
    NI_U16           u16BlcB;               /*RW, B像素黑电平偏移值, U10.2,rang[0,4095], default:0x0*/
}PISP_BLACK_LEVEL_S;


/* 定义黑电平校正参数结构体 */
typedef struct tagPispBlcParam
{
    NI_U32               u32BlcId;           /*RW, rang[0,2] 线性模式下配置u32BlcId=2中的黑电平值 其他u32BlcId中的值请配置为默认黑电平值
                                                   宽动态模式下u32BlcId=1表示长帧 u32BlcId=2表示短帧 其他u32BlcId中的值请配置为默认黑电平值*/
    PISP_BLACK_LEVEL_S   stBlackLevel;
}PISP_BLC_PARAM_S;


/**************************************************************************************************
									DPC 
***************************************************************************************************/
/* 定义dpc限幅阈值 */
typedef struct tagPispDpcThreshold
{
    NI_U32  u32RThrshdOfMinCenter;          /*RW, r限幅阈值, 当中心点为最小值时使用, U.12.0,rang[0,4095], default:0x11*/
    NI_U32  u32RThrshdOfMaxCenter;          /*RW, r限幅阈值, 当中心点为最大值时使用, U.12.0,rang[0,4095], default:0x11*/
    
    NI_U32  u32GrThrshdOfMinCenter;         /*RW, gr限幅阈值, 当中心点为最小值时使用, U.12.0,rang[0,4095], default:0x11*/
    NI_U32  u32GrThrshdOfMaxCenter;         /*RW, gr限幅阈值, 当中心点为最大值时使用, U.12.0,rang[0,4095], default:0x11*/
    
    NI_U32  u32GbThrshdOfMinCenter;         /*RW, gb限幅阈值, 当中心点为最小值时使用, U.12.0,rang[0,4095], default:0x11*/ 
    NI_U32  u32GbThrshdOfMaxCenter;         /*RW, gb限幅阈值, 当中心点为最大值时使用, U.12.0,rang[0,4095], default:0x11*/
     
    NI_U32  u32BThrshdOfMinCenter;          /*RW, b限幅阈值, 当中心点为最小值时使用, U.12.0,rang[0,4095], default:0x11*/
    NI_U32  u32BThrshdOfMaxCenter;          /*RW, b限幅阈值, 当中心点为最大值时使用, U.12.0,rang[0,4095], default:0x11*/
}PISP_DPC_THRESHOLD_S;


/* 定义第二级DPC限幅阈值 */
typedef struct tagPispNxtDpcThreshold
{
    NI_U32  u32MinR;                        /*RW, r最小值限幅阈值, U.12.0,rang[0,4095], default:0x32*/
    NI_U32  u32MaxR;                        /*RW, r最大值限幅阈值, U.12.0,rang[0,4095], default:0x32*/
    
    NI_U32  u32MinGr;                       /*RW, gr最小值限幅阈值, U.12.0,rang[0,4095], default:0x32*/
    NI_U32  u32MaxGr;                       /*RW, gr最大值限幅阈值, U.12.0,rang[0,4095], default:0x32*/

    NI_U32  u32MinGb;                       /*RW, gb最小值限幅阈值, U.12.0,rang[0,4095], default:0x32*/
    NI_U32  u32MaxGb;                       /*RW, gb最大值限幅阈值, U.12.0,rang[0,4095], default:0x32*/
    
    NI_U32  u32MinB;                        /*RW, b最小值限幅阈值, U.12.0,rang[0,4095], default:0x32*/
    NI_U32  u32MaxB;                        /*RW, b最大值限幅阈值, U.12.0,rang[0,4095], default:0x32*/
}PISP_NXT_DPC_THRESHOLD_S;


typedef struct tagPispDpcParam
{
    NI_BOOL                 	bEnable;         /*RW, 是否使能dpc */ 
    NI_BOOL                 	bFirstEnable;    /*RW, dpc第一级使能开关 */
    PISP_DPC_THRESHOLD_S        stThrshd;        /*RW, 跟最大最小中心点有关的DPC阈值 */
    NI_U32                  	u32GType;        /*RW, G分量处理窗方式, rang[0,1],0:方形；1:菱形 */
    NI_U32                  	u32DpcMode;      /*RW, 动态检测模式, rang[0,1],0:利用最大、最小值检测；1:利用最大、次最大、最小、次最小值检测 */
    NI_U32                  	u32SortMode;     /*RW, 排序模式, rang[0,1],0:排序时允许次大/小值与最大/小值相同; 1:排序时候不允许次大/小值与最大/小值相同 */
    NI_U32                  	u32OriginType;   /*RW, 原点类型, rang[0,1],0:原点为最值; 1:原点不一定为最值. 当u32DpcMode=1时(即利用最大、次最大、最小、次最小值检测), 该参数有效 */
    
    NI_BOOL                     bNxtEnable;      /*RW, 是否使能第二级dpc */
    PISP_NXT_DPC_THRESHOLD_S    stNxtThrshd;     /*RW, 第二级动态坏点校正阈值 */
    NI_U32                      u32NxpDpcMode;   /*RW, 第二级dpc过滤模式, Range: [0, 1]:
                                                          0：做好上一级dpc过滤后, 全部做一遍第二级dpc过滤；
                                                          1：做好上一级dpc过滤后, 若上一级dpc用最大、小值替代, 则第二级不过滤；否则用第二级dpc过滤 */
}PISP_DPC_PARAM_S;


/**************************************************************************************************
                                             GIC
***************************************************************************************************/
typedef struct tagPispGicCurv
{
    NI_U16  u16GicThr;                    /* RW, U.12.0,default:0x0*/
    NI_U16  u16GicOffset;                 /* RW, U.12.0,default:0x0*/
    NI_U16  au16GicRatio[3];              /* RW, U.2.4, default:{0x16,0x16,0x16} */
}PISP_GIC_CURV_S;


typedef struct tagPispGicParam
{
    NI_BOOL                 bEnable;             /*RW, GIC模块使能开关，default:1*/
    NI_U8                   u8GicMode;           /*RW, GIC的工作模式选择, U.1.0,在使能开启的情况下：0：gic模式 1：强力模式, default:1*/

    NI_U8                   u8StrenR;            /*RW, GIC模块R分量使能强度 U.5.0,rang[0, 16],16：强度最强 0：关闭该分量使能,default:0x0*/
    NI_U8                   u8StrenGr;           /*RW, GIC模块gr分量使能强度 U.5.0,rang[0, 16],16：强度最强 0：关闭该分量使能,default:0x0*/
    NI_U8                   u8StrenGb;           /*RW, GIC模块gb分量使能强度 U.5.0,rang[0, 16],16：强度最强 0：关闭该分量使能,default:0x0*/
    NI_U8                   u8StrenB;            /*RW, GIC模块B分量使能强度 U.5.0,rang[0, 16],16：强度最强 0：关闭该分量使能,default:0x0*/

    PISP_GIC_CURV_S         stLumaCurv;          /*RW, GIC模块亮度曲线控制结构体  低亮阈值u16GicThr rang[0, 3583],高亮阈值偏移u16GicOffset rang[0, 3583-u16GicThr],分区强度au16GicRatio[0~3] rang[0, 63]*/
    PISP_GIC_CURV_S         stContstCurv;        /*RW, GIC模块对比度曲线控制结构体 低对比度阈值u16GicThr rang[0, 3839],高对比度阈值偏移u16GicOffset rang[0, 3839-u16GicThr],分区强度au16GicRatio[0~3] rang[0, 63]*/
}PISP_GIC_PARAM_S;


/**************************************************************************************************
									DGC
***************************************************************************************************/
/* 定义数字增益系数 */
typedef struct tagPispDgcGain
{
    NI_U32          u32GainR;                 /*RW, R像素增益系数; U8.8, rang[0, 65535], default:0x100 */
    NI_U32          u32GainGr;                /*RW, Gr像素增益系数; U8.8, rang[0, 65535], default:0x100 */
    NI_U32          u32GainGb;                /*RW, Gb像素增益系数; U8.8, rang[0, 65535], default:0x100 */
    NI_U32          u32GainB;                 /*RW, B像素增益系数; U8.8, rang[0, 65535], default:0x100 */
}PISP_DGC_GAIN_S;


/* 定义数字增益参数 */
typedef struct tagPispDgcParam
{
    NI_U32               u32DgcId;                   /*RW,rang[0,2] 线性模式下配置u32DgcId=0中的增益值 其他u32DgcId中的增益值请配置为默认增益值
                                                           宽动态模式下u32DgcId=1 表示长帧 u32DgcId=0 表示短帧 其他u32DgcId中的值请配置为默认增益值*/    
    PISP_DGC_GAIN_S      stGain;    
}PISP_DGC_PARAM_S;


/**************************************************************************************************
                               CNR  色度降噪
***************************************************************************************************/
typedef struct tagPispCnrCurv
{
    NI_U8   u8Thr1;    
    NI_U8   u8Thr1Offset;
    NI_U8   u8Str1;
    NI_U8   u8Str2;
    NI_U8   u8Str3;
}PISP_CNR_CURV_S;


/*颜色紫边判断控制参数 */
typedef struct tagPispCnrParam
{
    NI_BOOL                  bCnrEn;               /* RW, 小使能开关,默认开 */
    NI_BOOL                  bColorProtEn;         /* RW, 避免颜色溢出开关,默认开 */
    NI_U8                    u8PurpleJudLum;       /* RW, 判断紫边的亮度阈值,u.0.8,rang[0,255], default:0xdc*/
    PISP_CNR_CURV_S          stCnrColorDiff;       /* RW, 均值差控制参数结构体,低色差阈值u8Thr1 rang[0, 251],高色差阈值偏移u8Thr1Offset rang[0, 251-u8Thr1],分区强度u8Str1~3 rang[0, 16] default{0x0,0x4,0x10}*/     
    PISP_CNR_CURV_S          stPurpleJud;          /* RW, 颜色紫边判断结构体,弱紫边阈值u8Thr1 rang[0, 80],强紫边阈值偏移u8Thr1Offset rang[0, 80-u8Thr1],分区强度u8Str1~3 rang[0, 16] default{0x0,0xe,0x10}*/    
}PISP_CNR_PARAM_S;


/**************************************************************************************************
									NRY3D (PRE + POST)
***************************************************************************************************/
typedef struct tagPispNry3dCurv
{
    NI_U8   u8Thr1;
    NI_U8   u8Thr1Offset;
    NI_U8   u8Ratio1;
    NI_U8   u8Ratio2;
    NI_U8   u8Ratio3;
}PISP_NRY3D_CURV_S;


typedef struct tagPispNry3d2dY
{
    NI_U8                           u8SlopeStrength;         /* RW, 2D降噪强度，值越大强度越大 u.0.6,rang[0, 63],,default 0x10*/
    NI_U8         					u8MotionRegCoef;         /* RW, 运动区2D降噪混合系数，u.0.8,rang[0, 255], default:0x10*/
    NI_U8         					u8StaticRegCoef;      	 /* RW, 静止区2D降噪混合系数，u.0.8,rang[0, 255], default:0x10*/
    PISP_NRY3D_CURV_S               stLumCurv;               /* RW, 亮度调节曲线结构体 低亮阈值u8Thr1[0, 191],高亮阈值偏移u8Thr1Offset[0, 191-u8Thr1],分区强度u8Ratio1~3[0, 63]*/
    PISP_NRY3D_CURV_S               stCtrstCurv;             /* RW, 对比度调节调节曲线结构体 低对比度阈值u8Thr1[0, 251],高对比度阈值偏移u8Thr1Offset[0, 251-u8Thr1],分区强度u8Ratio1~3[0, 63]*/
}PISP_NRY3D_2D_CFG_Y_S;


typedef struct tagPispNry3d2dUv
{
    NI_U8                           u8ColorCtrstMode;     /* RW, 色溢抑制模式，rang[0, 1] 0:关闭;1:打开*/
    NI_U8         					u8MotionRegCoef;      /* RW, 运动区2D降噪强度，u.0.8,rang[0, 255], default:0x10*/
    NI_U8         					u8StaticRegCoef;      /* RW, 静止区2D降噪强度，u.0.8,rang[0, 255], default:0x10*/
}PISP_NRY3D_2D_CFG_UV_S;


typedef struct  tagPispNry3d2dFreq
{
    NI_U8                            u8NegRatio;               /* RW, 负边增强系数, u.2.4,rang[0,63], default:0x10 */
    NI_U8                            u8PosRatio;               /* RW, 正边增强系数, u.2.4,rang[0,63], default:0x10 */
    NI_U8                            u8DetailUperThr;          /* RW, 细节上限阈值, u.8.0,rang[0,63],default:0x8 */
    NI_U8                            u8DetailLowerThr;         /* RW, 细节下限阈值, u.8.0,rang[0,63],default:0x8 */
    PISP_NRY3D_CURV_S                stFreqCurv;               /* RW, 频域滤波的细节调节曲线配置 低频阈值u8Thr1[0, 251] 高频阈值偏移u8Thr1Offset[0, 251-u8Thr1] 分区强度u8Ratio1~3[0, 16]*/
    PISP_NRY3D_CURV_S                stFreqLumCurv;            /* RW, 频域滤波的亮度调节曲线配置 低亮阈值u8Thr1[0, 191] 高亮阈值偏移u8Thr1Offset[0, 191-u8Thr1] 分区强度u8Ratio1~3[0, 16]*/
    PISP_NRY3D_CURV_S                stFreqMotionCurv;         /* RW, 频域滤波的运动调节曲线配置 静止阈值u8Thr1[0, 223] 运动阈值偏移u8Thr1Offset[0, 223-u8Thr1] 分区强度u8Ratio1~3[0, 16]*/
}PISP_NRY3D_2D_FREQ_S;


typedef struct  tagPispNryPre3d
{
    NI_U8                       u8DiffScaleY;                /* RW, Y分量Diff放大比例，U2.0, 0:比例为1, 1:比例为2, 2:比例为4, 3:比例为8 */
    NI_U8                       u8DiffScaleUv;               /* RW, Diff放大比例，U2.0, 0:比例为1, 1:比例为2, 2:比例为4, 3:比例为8 */
    NI_U8                       u8RatioP;                    /* RW, detail 2D平坦区强度，u.0.8,rang[0,255], default:0x10*/         
    NI_U8                       u8RatioD;                    /* RW, detail 2D细节区强度，u.0.8,rang[0,255], default:0x10*/     

    PISP_NRY3D_CURV_S           stMotionY;                   /* RW, Y分量 MOTION相关参数配置 静止阈值u8Thr1[0, 251] 运动阈值偏移u8Thr1Offset[0, 251-u8Thr1] 分区强度u8Ratio1[17, 255] u8Ratio2~3[0,255] u8Ratio1>=u8Ratio2>=u8Ratio3*/
    PISP_NRY3D_CURV_S           stMotionFusY;                /* RW, Y分量 MOTION FUSE相关参数配置 静止阈值u8Thr1[0, 251] 运动阈值偏移u8Thr1Offset[0, 251-u8Thr1] 分区强度u8Ratio1~3[0, 16] */
    PISP_NRY3D_CURV_S           stDiffAdjLumY;               /* RW, Y分量 DF ADJ参数配置 低亮阈值u8Thr1[0, 191] 高亮阈值偏移u8Thr1Offset[0, 191-u8Thr1] 分区强度u8Ratio1~3[16, 32]*/
    PISP_NRY3D_2D_CFG_Y_S       stNry2dCfgY;                 /* RW, Y分量 NRY2D相关参数配置 */
    PISP_NRY3D_2D_FREQ_S        stNry2dFreq;                 /* RW,频域滤波参数配置*/
    
    PISP_NRY3D_2D_CFG_UV_S      stNry2dCfgUv;
    PISP_NRY3D_CURV_S           stMotionUv;                  /* RW, Uv分量 MOTION相关参数配置 静止阈值u8Thr1[0, 251] 运动阈值偏移u8Thr1Offset[0, 251-u8Thr1] 分区强度u8Ratio1[17, 255] u8Ratio2~3[0,255] u8Ratio1>=u8Ratio2>=u8Ratio3*/  
    PISP_NRY3D_CURV_S           stMotionFusUv;               /* RW, Uv分量 MOTION FUSE相关参数配置 静止阈值u8Thr1[0, 251] 运动阈值偏移u8Thr1Offset[0, 251-u8Thr1] 分区强度u8Ratio1~3[0, 16] */    
}PISP_NRYPRE3D_S;


typedef struct  tagPispNryPost3d
{
    NI_U8                     u8DiffScale;           /* RW, Diff放大比例，U2.0, 0:比例为1, 1:比例为2, 2:比例为4, 3:比例为8 */
	NI_U8                     u8TailMode;            /* RW, 噪声拖尾模式, 0:收敛慢，尾部清晰, 1:收敛快，尾部模糊,  default:0x1 range[0,1]*/

    PISP_NRY3D_CURV_S         stMotionCurv;          /* RW, Motion结构体 静止阈值u8Thr1[0, 251] 运动阈值偏移u8Thr1Offset[0, 251-u8Thr1] 分区强度u8Ratio1[17, 255] u8Ratio2~3[0,255] u8Ratio1>=u8Ratio2>=u8Ratio3*/
    PISP_NRY3D_CURV_S         stMotionFusCurv;       /* RW, Motion FUSE结构体 静止阈值u8Thr1[0, 251] 运动阈值偏移u8Thr1Offset[0, 251-u8Thr1] 分区强度u8Ratio1~3[0, 16] */
    PISP_NRY3D_CURV_S         stDiffAdjLum;          /* RW, DF ADJ参数配置 低亮阈值u8Thr1[0, 191] 高亮阈值偏移u8Thr1Offset[0, 191-u8Thr1] 分区强度u8Ratio1~3[16, 32]*/
    PISP_NRY3D_2D_CFG_Y_S     stNry2dCfg;            /* RW, NRY2D相关参数配置 */
}PISP_NRYPOST3D_S;


typedef struct tagPispNry3d
{
	NI_BOOL                   bEnable;
	
	PISP_NRYPRE3D_S	          stNryPre3d;
	PISP_NRYPOST3D_S          stNryPost3d;	
}PISP_NRY3D_PARAM_S;


/**************************************************************************************************
                                             DOW
***************************************************************************************************/
/* 定义数字交叉宽动态选择的帧类型 */
typedef enum tagPispDowFrame
{
    PISP_DOW_FRAME_C = 0,			             /* 非合并数据输出选择融合帧 */	
    PISP_DOW_FRAME_S = 1,                        /* 非合并数据输出选择短曝光帧 */	
    PISP_DOW_FRAME_L = 2,                        /* 非合并数据输出选择长曝光帧 */	

    PISP_DOW_FRAME_BUTT
}PISP_DOW_FRAME_E;


typedef struct tagPispDowStateInfo
{
    NI_U64                  u64LongFrmY1;       /*RO, 宽动态长曝光帧曲线拟合的统计值1, U34.0 */
    NI_U64                  u64ShortFrmX1;      /*RO, 宽动态短曝光帧曲线拟合的统计值1, U34.0 */
    NI_U32                  u32ImgN1;           /*RO, 宽动态两帧曲线拟合的第一段的统计个数1, U26.0 */

    NI_U64                  u64LongFrmY2;       /*RO, 宽动态长曝光帧曲线拟合的统计值2, U34.0 */
    NI_U64                  u64ShortFrmX2;      /*RO, 宽动态短曝光帧曲线拟合的统计值2, U34.0 */
    NI_U32                  u32ImgN2;           /*RO, 宽动态两帧曲线拟合的第二段的统计个数2, U26.0 */
}PISP_DOW_STATE_INFO_S;				


typedef struct tagPispDow2Frm
{
    NI_U32          u32CmThr1;          /*RW, 融合阈值1, U8.0  */
    NI_U32          u32CmThr2;          /*RW, 融合阈值2, U12.0  */
    NI_U32          u32CmK1;            /*RW, 融合斜率1, U1.16  */
    NI_U32          u32CmK2;            /*RW, 融合斜率2, U1.16  */

    NI_U32          u32DgStr;           /*RW, 鬼影优化强度等级, U9.0, default:0x0*/
    NI_U32          u32DgVirLevel;      /*RW, 鬼影优化虚化程度, U.19.10, default:0x3000000*/
    
    NI_U32          u32LineA;           /*RW, 宽动态曝光比,需要跟sensor真实曝光比同步 u.7.10 */
    
    NI_U32          u32SefSatStr;       /*RW, 宽动态两帧融合中短帧的色彩饱和度调整强度, U0.6 */    

    NI_U32          u32LSConvert;       /*RW, 主从通道交换，=0，主通道是短曝光、辅通道是长曝光; =1，主通道是长曝光、辅通道是短曝光 */
    NI_U32          u32MSHandleEn;      /*RW, 融合时的高光抑制使能 */
}PISP_DOW_2FRM_S;


typedef struct tagPispDowParam
{
    NI_BOOL                  bEnable;            /*RW, DOW使能开关 */
    PISP_DOW_FRAME_E         enDowFrame;         /*RW, 支持长短曝光选择,具体需要根据u32LSConvert确认主通道是长或者短曝光 */
    PISP_DOW_2FRM_S          stDow2Frm;          /*RW, 2帧模式下融合参数配置 */
}PISP_DOW_PARAM_S;


/**************************************************************************************************
                                            WB                   
***************************************************************************************************/
/* 定义白平衡参数结构体 */
typedef struct tagPispWbGain
{
    NI_U32          u32GainR;                  /*RW, R像素增益系数; u.4.8,rang[0, 4095], default:0x100 */
    NI_U32          u32GainGr;                 /*RW, Gr像素增益系数;u.4.8,rang[0, 4095], default:0x100 */
    NI_U32          u32GainGb;                 /*RW, Gb像素增益系数;u.4.8,rang[0, 4095], default:0x100 */
    NI_U32          u32GainB;                  /*RW, B像素增益系数;u.4.8,rang[0, 4095], default:0x100 */
}PISP_WB_GAIN_S;


typedef struct tagPispWbParam
{
    NI_U32          u32WbId;                  /*RW, rang[0,2] 线性模式下只配置u32WbId=1时的增益值 其他u32WbId中的增益值请设置为默认增益值
                                                    宽动态模式下u32WbId=0 表示长帧 u32WbId=1 表示短帧 其他u32WbId中的值请配置为默认增益值*/    
    PISP_WB_GAIN_S  stWbGain;                 
}PISP_WB_PARAM_S;


/*************************************************************************************************
									LWDR
***************************************************************************************************/
/*局部宽动态结构体定义*/
typedef struct tagPispLwdrParam
{
    NI_BOOL         bEnable;               /*RW, 宽动态开关 */
    NI_U8           u8WLuma;               /*RW, 全局亮度调节参数;u.1.4,rang[0, 16], default:0xa */
    NI_U8           u8WContrast;           /*RW, 全局对比度调节参数;u.1.4,rang[0, 16], default:0xa */
    NI_U16          u16WGain;         	   /*RW, 增强强度调节参数;u.5.3, rang[0, 255], default:0x10 */

    NI_BOOL         bHlsEnable;            /*RW, 高亮抑制开关 */
    NI_U16          u16HlsDelta;           /*RW, 高亮抑制强度,值越大亮度抑制程度越大;位宽[7:0],取值范围[0, 255], default:0x40 */
    NI_U16          u16HlsThrhd;           /*RW, 高亮抑制阈值,小于该值进行亮度抑制,位宽[7:0],取值范围[0, 255], default:0xff */
	
	NI_U16          *pu16AlphaTab;         /*RW, 亮度控制表 */
    NI_U16          u16AlphaTabLen;        

	NI_U16          *pu16FiTab;            /*RW, WDR提亮表 */
    NI_U16          u16FiTabLen;        
}PISP_LWDR_PARAM_S;


/**************************************************************************************************
										DRCD
***************************************************************************************************/
typedef enum tagPispDrcdRawCompMode
{
    PISP_DRCD_RAW_COMP_MODE_DEFAULT = 0,                     /* 默认压缩模式,线性*/
    PISP_DRCD_RAW_COMP_MODE_WDR     = 1,                     /* 宽动态压缩模式,宽动态*/

    PISP_DRCD_RAW_COMP_MODE_BUTT
}PISP_DRCD_RAW_COMP_MODE_E;


typedef struct tagPispDrcdParam
{
    NI_BOOL                   bEnable;            /*RW, DRCD模块使能信号 */

    NI_BOOL                   bDeCompEn;          /*RW, DRCD解压使能开关,1: 使能开，sensor built in模式,0: 使能关闭，其他模式*/
    NI_BOOL                   bRawCompEn;         /*RW, raw输出时候经过曲线压缩处理u1.0*/ 
    PISP_DRCD_RAW_COMP_MODE_E enRawCompMode;      /*RW, raw输出时候经过曲线压缩的模式 模式0:默认压缩模式 模式1:宽动态优化模式 */
    
    NI_U32                    u32RawPx;           /*RW, DRCD曲线压缩选择，0-9 */
    NI_U32                    u32DrcdC;           /*RW, DRCD压缩曲线选择, 0-4 */

    NI_U16                   u16HfStrenR;         /*RW, 高频R增强比例系数,U.2.4,rang[0,63], default:0x10 */
    NI_U16                   u16HfStrenG;         /*RW, 高频G增强比例系数,U.2.4,rang[0,63], default:0x20 */
    NI_U16                   u16HfStrenB;         /*RW, 高频B增强比例系数,U.2.4,rang[0,63], default:0x10 */

    NI_U32                   u32GThrhd;           /*RW, 高频阈值, U20.0 rang[0,0xfffff], default:0x20 */
    NI_U32                   u32GDelta;           /*RW, HDR高频范围, u.5.0 rang[0,31], default:0x5 */

}PISP_DRCD_PARAM_S;


/**************************************************************************************************
                                             LSC
***************************************************************************************************/
typedef struct tagPispLscTable
{
    NI_U16                  u16TabLen;          /*RW, 颜色表的长度, ISP_LSC_MAX_TABLE_LEN */
    NI_U16                  *pu16TableR;         /*RW, R表, 成员数据类型U2.10 */
    NI_U16                  *pu16TableG;         /*RW, G表, 成员数据类型U2.10 */
    NI_U16                  *pu16TableB;         /*RW, B表, 成员数据类型U2.10 */
}PISP_LSC_TABLE_S;


typedef struct tagPispLscParam
{
    NI_BOOL                 bEnable;            /*RW, 是否使能镜头阴影校正 */
    POINT_S                 stCenterR;          /*RW, r分量中心坐标 U.12.0*/
    POINT_S                 stCenterG;          /*RW, g分量中心坐标 U.12.0*/
    POINT_S                 stCenterB;          /*RW, b分量中心坐标 U.12.0*/

    PISP_LSC_TABLE_S        stLscTab;           /*RW, LSC表 */
}PISP_LSC_PARAM_S;


/**************************************************************************************************
                                             AWES & RAWHIST
***************************************************************************************************/
/* 定义AWES统计位置 */
typedef enum tagPispAwesPosition
{
    PISP_AWES_FROM_FRAME_L             = 0,        /* 只统计长曝光帧，仅宽动态模式下支持 */
    PISP_AWES_FROM_FRAME_S             = 1,        /* 只统计短曝光帧，支持线性和宽动态模式 */
    PISP_AWES_FROM_FRAME_COMPO         = 2,        /* 只统计合成帧，支持线性和宽动态模式 */
    PISP_AWES_FROM_FRAME_L_S           = 3,        /* 统计长、短两帧统计数据，仅宽动态模式下支持 */
    PISP_AWES_FROM_FRAME_L_COMPO       = 4,        /* 统计长、合成两帧统计数据,仅宽动态模式下支持*/
    PISP_AWES_FROM_FRAME_S_COMPO       = 5,        /* 统计短、合成两帧统计数据,支持线性和宽动态模式 */
    PISP_AWES_FROM_FRAME_L_S_COMPO     = 6,        /* 统计长、短、合成三帧统计数据,仅宽动态模式下支持*/
    
    PISP_AWES_POS_BUTT
}PISP_AWES_POS_E;


typedef struct tagPispAwesParam
{   
    PISP_AWES_POS_E         enAwesPos;          /*RW, AWES统计数据来源位置  */
    NI_U16                  u16BlockHSize;      /*RW, 2A统计块横向大小,与统计区域横向大小有关, 值为16的倍数,Range: [16, 64],统计区域内横向分块个数最大为82个 */
    NI_U16                  u16BlockVSize;      /*RW, 2A统计块纵向大小,值为8的倍数， Range: [8, 64],default:0x40*/
    RECT_S                  stStatisticArea;    /*RW, 2A统计模块统计区域 */
}PISP_AWES_PARAM_S;


/* 定义一帧图像的AWES统计值信息结构体 */
typedef struct tagPispAwesData
{
    NI_U16                 *pu16AwesData;      /* RO, AWES统计的rgby值, 每个分量数据位宽为16，每帧统计数据=u32BlockNum*2*4bytes, 数据内容按照块的顺序即先水平后垂直排列，每一块内部按照 r/g/b/y排列*/
    NI_U32                 u32AwesPhyAddr;     /* RO, pu16AwesData对应的物理地址 */
}PISP_AWES_DATA_S;


typedef struct tagPispAwesStatistics
{
    NI_U32                  u32BlockNum;        /* RO, AWES统计一帧的分块总数 */
    NI_U32                  u32HBlockNum;       /* RO, AWES统计水平方向分块个数 */
    NI_U32                  u32VBlockNum;       /* RO, AWES统计垂直方向分块个数 */

    NI_U32                  u32AwesMbufBlkId;   /* RO, pu16AwesData对应缓存块的id号 */
    NI_U32                  u32AwesFrmNum;      /* RO, astAwesData数组有u32AwesFrmNum帧统计数据，帧数与enAwesPos变量及x帧HDR模式对应 */
    PISP_AWES_POS_E         enAwesPos;          /* RO, 指示当前AWES的统计数据来源 */
    PISP_AWES_DATA_S        astAwesData[3];     /* RO, AWES统计数据，硬件一次固定三帧数据，顺序为短、长、融合帧 线性模式下数据结果保存在astAwesData[0]中*/
}PISP_AWES_STATISTICS_S;


/*定义RAW_HIST统计位置*/
typedef enum tagPispRawHistPosition
{
    PISP_RAWHIST_FROM_FRAME_L             = 0,        /* 只统计长曝光帧，仅宽动态模式下支持*/
    PISP_RAWHIST_FROM_FRAME_S             = 1,        /* 只统计短曝光帧，支持线性和宽动态模式*/
    PISP_RAWHIST_FROM_FRAME_COMPO         = 2,        /* 只统计合成帧，支持线性和宽动态模式*/
    PISP_RAWHIST_FROM_FRAME_L_S           = 3,        /* 统计长、短两帧统计数据，仅宽动态模式下支持*/
    PISP_RAWHIST_FROM_FRAME_L_COMPO       = 4,        /* 统计长、合成两帧统计数据,仅宽动态模式下支持*/
    PISP_RAWHIST_FROM_FRAME_S_COMPO       = 5,        /* 统计短、合成两帧统计数据,支持线性和宽动态模式*/
    PISP_RAWHIST_FROM_FRAME_L_S_COMPO     = 6,        /* 统计长、短、合成三帧统计数据,仅宽动态模式下支持*/
    
    PISP_RAWHIST_POS_BUTT
}PISP_RAWHIST_POS_E;


typedef struct tagPispRawHistData
{    
    NI_U32                 *pu32RawHistData;   /* RO, RAWHIST统计的rgby值, 每帧统计数据:R/G/B/Y 的位宽均为 16，数据按照 R[128]/G[128]/B[128]/Y[256]顺序排列。即总大小为：2* (128*3+256)bytes*/
    NI_U32                 u32HistPhyAddr;     /* RO, apu32RawHistData对应的物理地址 */
}PISP_RAWHIST_DATA_S;


typedef struct tagPispRawHistParam
{   
    NI_BOOL                bRawHistEn;         /*RW, 是否开启raw域直方图统计功能 */
    PISP_RAWHIST_POS_E     enRawHistPos;       /*RW, RAW_HIST统计数据来源位置  */
}PISP_RAWHIST_PARAM_S;


typedef struct tagPispRawHistStatistics
{
    NI_U32                  u32HistFrmNum;                    /* RO, apu32RawHistData数组所指数据有u32HistFrmNum帧统计数据 */
    NI_U32                  u32HistMbufBlkId;                 /* RO, apu32RawHistData对应缓存块的id号 */
    PISP_RAWHIST_POS_E      enRawHistPos;                     /* RO, 指示当前RawHist的统计数据来源 */

    PISP_RAWHIST_DATA_S     astRawHistData[3];                /* RO, AWES统计数据，硬件一次固定三帧数据，顺序为短、长、融合帧 线性模式下数据结果保存在astRawHistData[0]中*/
}PISP_RAWHIST_STATISTICS_S;


/**************************************************************************************************
									CFA
***************************************************************************************************/
/* 定义BAYER格式的RGB顺序头 */
typedef enum tagPispCfaBayerSeq
{
    PISP_CFA_BAYER_SEQ_GBRG = 0,       /* 表示BAYER格式数据以GB开头 */   
    PISP_CFA_BAYER_SEQ_RGGB = 1,       /* 表示BAYER格式数据以RG开头 */

    PISP_CFA_BAYER_SEQ_BUTT
}PISP_CFA_BAYER_SEQ_E;


/* CFA参数 */
typedef struct tagPispCfaParam
{
    PISP_CFA_BAYER_SEQ_E     enBayerSeq;         /*RW, 第一行顺序标记, 0:第一行是gb行, 1:第一行是rg行,default:0x1*/
    NI_U8                    u8AALevel;          /*RW, 数值越大去伪色强度越大,0: 关闭, u.3.0, rang[0,4], default::0x4 */
    NI_U8                    u8KRatio;           /*RW, 颜色滤波矩阵的插值比较系数,U4.4,rang[0.255],default:0x30*/
}PISP_CFA_PARAM_S;


/**************************************************************************************************
									CCM
***************************************************************************************************/
typedef struct tagPispCcmMatrix
{
    NI_U32                  u32GainRR;          /*RW, Gain[0,0], CCM模块校正矩阵系数 GainRR, 有符号数S5.10, default::0x400 */
    NI_U32                  u32GainGR;          /*RW, Gain[0,1], CCM模块校正矩阵系数 GainGR, 有符号数S5.10, default::0x0 */
    NI_U32                  u32GainBR;          /*RW, Gain[0,2], CCM模块校正矩阵系数 GainBR, 有符号数S5.10, default::0x0 */
    NI_U32                  u32GainRG;          /*RW, Gain[1,0], CCM模块校正矩阵系数 GainRG, 有符号数S5.10, default::0x0 */
    NI_U32                  u32GainGG;          /*RW, Gain[1,1], CCM模块校正矩阵系数 GainGG, 有符号数S5.10, default::0x400 */
    NI_U32                  u32GainBG;          /*RW, Gain[1,2], CCM模块校正矩阵系数 GainBG, 有符号数S5.10, default::0x0 */
    NI_U32                  u32GainRB;          /*RW, Gain[2,0], CCM模块校正矩阵系数 GainRB, 有符号数S5.10, default::0x0 */
    NI_U32                  u32GainGB;          /*RW, Gain[2,1], CCM模块校正矩阵系数 GainGB, 有符号数S5.10, default::0x0 */
    NI_U32                  u32GainBB;          /*RW, Gain[2,2], CCM模块校正矩阵系数 GainBB, 有符号数S5.10, default::0x400 */
}PISP_CCM_MATRIX_S;


typedef struct tagPispCcmParam
{
    PISP_CCM_MATRIX_S        stMatrix;           /*RW, 色彩校正矩阵系数 */

    NI_U32                   u32OffsetR;         /*RW, CCM模块R基色的偏移量, 有符号数S.8.0, default::0x0 */
    NI_U32                   u32OffsetG;         /*RW, CCM模块G基色的偏移量, 有符号数S.8.0, default::0x0 */
    NI_U32                   u32OffsetB;         /*RW, CCM模块B基色的偏移量, 有符号数S.8.0, default::0x0 */
}PISP_CCM_PARAM_S;


/**************************************************************************************************
									GAMMA
***************************************************************************************************/
typedef struct tagPispGammaTable
{
    NI_U8                   *pu8GammaTab;       /*RW, 伽玛表,若为NULL将使用内部默认gamma表 */    
    NI_U32                  u32TabLen;          /*RW, 伽玛表长度,256 */           
}PISP_GAMMA_TABLE_S;


typedef struct tagPispGammaParam
{
    NI_BOOL                 bEnable;            /*RW, 0:功能关闭；1:功能打开 */    
    NI_BOOL                 bEnableInvImg;      /*RW, 图像负像是否使能 */
    NI_U32                  u32InvImgMaxVal;    /*RW, 图像负像最大值, U.8.0 rang[0, 255], default::0xff */

    PISP_GAMMA_TABLE_S      stGammaTab;         /*RW, gamma表 */
}PISP_GAMMA_PARAM_S;


/**************************************************************************************************
									CSC_CDS
***************************************************************************************************/
typedef enum tagPispCscMatrixSel
{
    PISP_CSC_MATRIX_SEL_STD  = 0,                /* 采用标准的rgb转y矩阵 */
    PISP_CSC_MATRIX_SEL_BIG  = 1,                /* 采用自定义的rgb转y大矩阵 */
    PISP_CSC_MATRIX_SEL_BUTT
}PISP_CSC_MATRIX_SEL_E;


typedef struct tagPispCscStdMatrix 
{
    NI_U32                  u32KyR;             /*RW, RGB转Y参数, U0.8 rang[0, 255], default:0x4d*/
    NI_U32                  u32KyG;             /*RW, RGB转Y参数, U0.8 rang[0, 255], default:0x96*/
    NI_U32                  u32KyB;             /*RW, RGB转Y参数, U0.8 rang[0, 255], default:0x1d*/

    NI_U32                  u32OffsetY;         /*RW, RGB转Y偏移量, S8.0 rang[0, 511], default:0x0*/
}PISP_CSC_STD_MATRIX_S;


typedef struct tagPispCscBigMatrix 
{
    NI_U32                  u32KyR;             /*RW, RGB转YUV系数矩阵里的系数, S3.10, */
    NI_U32                  u32KyG;             /*RW, RGB转YUV系数矩阵里的系数, S3.10, */
    NI_U32                  u32KyB;             /*RW, RGB转YUV系数矩阵里的系数, S3.10, */
    NI_U32                  u32KcbR;            /*RW, RGB转YUV系数矩阵里的系数, S3.10, */
    NI_U32                  u32KcbG;            /*RW, RGB转YUV系数矩阵里的系数, S3.10, */
    NI_U32                  u32KcbB;            /*RW, RGB转YUV系数矩阵里的系数, S3.10, */
    NI_U32                  u32KcrR;            /*RW, RGB转YUV系数矩阵里的系数, S3.10, */
    NI_U32                  u32KcrG;            /*RW, RGB转YUV系数矩阵里的系数, S3.10, */
    NI_U32                  u32KcrB;            /*RW, RGB转YUV系数矩阵里的系数, S3.10, */

    NI_U32                  u32OffsetY;         /*RW, Y信号偏移量, S10.0, default:0x0  */
    NI_U32                  u32OffsetCb;        /*RW, U信号偏移量, S10.0, default:0x80 */
    NI_U32                  u32OffsetCr;        /*RW, V信号偏移量, S10.0, default:0x80 */
}PISP_CSC_BIG_MATRIX_S;


typedef struct tagPispCscCdsParam
{    
    PISP_CSC_MATRIX_SEL_E    enYhistMatrixSel;    /*RW, yhist统计的rgb转y矩阵选择 */
    PISP_CSC_STD_MATRIX_S    stStdMatrix;         /*RW, 标准的rgb转y矩阵 */
    PISP_CSC_BIG_MATRIX_S    stBigMatrix;         /*RW, 自定义的rgb转y大矩阵,CSC固定使用该矩阵进行色彩空间转换 */
    
    NI_BOOL                  bAntiOverExposure;   /*RW, 过曝色度抑制开关  default:0x0 */
    NI_U8                    *pu8RestrainTab;     /*RW, 色彩抑制曲线表 */    
    NI_U32                   u32TabLen;           /*RW, 色彩抑制曲线表长度,单位byte, ISP_CSCCDS_MAX_TABLE_LEN */

    NI_U32                   u32BwMode;           /*RW, 黑白模式,rang[0,1], 0：彩色模式；1：黑白模式, default:0x0 */
    NI_U32                   u32YuvModeSel;       /*RW, YUV444-YUV422转换模式选择, rang[0,3], 0:表1：2：1滤波; 1:表1：1滤波; 2:表直接取点; 3:表byapss, default:0x2 */ 
}PISP_CSC_CDS_PARAM_S;


/**************************************************************************************************
									YHIST
***************************************************************************************************/
typedef struct tagPispYhistContrast
{
    NI_BOOL                 bEnableAdjust;      /*RW, 是否使能直方图统计配置映射表来调节对比度 */
    NI_U32                  u32TableLen;        /*RW, 直方图映射表长度, ISP_YHIST_MAX_TABLE_LEN */
    NI_U16                  *pu16Table;         /*RW, 直方图统计映射表*/  
}PISP_YHIST_CONTRAST_S;


typedef struct tagPispYhistParam
{
    NI_BOOL                 bEnable;            /*RW, 是否使能直方图统计 */
    NI_U32                  u32MinPercMend;     /*RW, 直方图统计设定的最小值边界,U.22.0,rang[0, 0x3fffff], default:0x0 */
    NI_U32                  u32MaxPercMend;     /*RW, 直方图统计设定的最大值边界,U.22.0,rang[0, 0x3fffff], default:0x0 */
    NI_U32                  u32Frate;           /*RW, 统计帧率,隔u32Frate帧统计一次, Range:[0, 255], default 0 */
}PISP_YHIST_PARAM_S;


typedef struct tagPispYhistStatistics
{
    NI_U32                  u32YhistMin;        /*RO, 直方图统计最小值 */
    NI_U32                  u32YhistMax;        /*RO, 直方图统计最大值 */
    NI_U32                  u32YhistAvg;        /*RO, 直方图统计得到亮度平均值 */
    NI_U32                  *pu32YhistData;     /*RO, 直方图统计数据, 数据位宽为32，固定256深度 */
    NI_U32                  u32PhyAddr;         /*RO, pu32YhistData对应的物理地址 */
    NI_U32                  u32MbufBlkId;       /*RO, pu32YhistData对应缓存块的id号 */
}PISP_YHIST_STATISTICS_S;


typedef struct tagPispYhistStatisticsInfo
{
    PISP_YHIST_STATISTICS_S   *pstYhistData;
    NI_U32                    u32MilliSec;
}PISP_YHIST_STATISTICS_INFO_S;


/**************************************************************************************************
                                             AF
***************************************************************************************************/
/* 均值滤波（A）和中值滤波（B）的次序及该功能块是否开启的组合:A0B0  A0B1  A1B0  A1B1  B1A1 （共五种组合）。滤波类型决定图像下采样后舍弃的下采样图像的上下左右的行数与点数*/
typedef enum tagPispAfsFilterType
{
    PISP_AF_FILTER_SEQ_A1B1 = 0, /* 先均值滤波再中值滤波。下采样后AF统计区域:x <= W-10 y <= H-10 */   
    PISP_AF_FILTER_SEQ_A1B0 = 1, /* 仅均值滤波。下采样后AF统计区域:x <= W-8 y <= H-8 */
    PISP_AF_FILTER_SEQ_A0B0 = 2, /* 中值滤波和均值滤波都关闭。下采样后AF统计区域:x <= W-6 y <= H-6 */   
    PISP_AF_FILTER_SEQ_A0B1 = 3, /* 仅中值滤波。下采样后AF统计区域:x <= W-8 y <= H-8 */
    PISP_AF_FILTER_SEQ_B1A1 = 4, /* 先中值滤波再均值滤波。下采样后AF统计区域:x <= W-10 y <= H-10 */
    PISP_AF_FILTER_SEQ_BUTT
}PISP_AF_FILTER_TYPE_E;


/* 灯光块与fswm块（算子块）宽度（高度）的比例。即 LambBlock/FswmBlock = ISP_AFS_LAMB_FSWM_RATIO_E */
typedef enum tagPispAfsLambFswmRatio
{
    PISP_AF_LAMB_FSWM_RATIO_ONE   = 1,
    PISP_AF_LAMB_FSWM_RATIO_TWO   = 2, /* 当大于2时，必须关闭偏移 */
    PISP_AF_LAMB_FSWM_RATIO_THREE = 3,
    PISP_AF_LAMB_FSWM_RATIO_FOUR  = 4,
    PISP_AF_LAMB_FSWM_RATIO_FIVE  = 5,
    PISP_AF_LAMB_FSWM_RATIO_SIX   = 6,
    PISP_AF_LAMB_FSWM_RATIO_SEVEN = 7,
    PISP_AF_LAMB_FSWM_RATIO_EIGHT = 8,
    PISP_AF_LAMB_FSWM_RATIO_BUTT
}PISP_AF_LAMB_FSWM_RATIO_E;


typedef struct tagPispAfsAntiLamp
{
    NI_BOOL                 bAntiLampLight;             /*RW, 去灯光是否使能 */
    NI_U32                  u32LampLevel;               /*RW, 灯光块判断阈值, 值越小判断的灯光块数越多。Range: [0, 0xFF] */
    NI_U32                  u32MaxLevel;                /*RW, 灯光判断上阈值,大于此值必定是灯光区域。 Range: [0, 0xFF] */
    NI_U32                  u32MidLevel;                /*RW, 灯光判断下阈值，灯光区域需大于此阈值。 Range: [0, 0xFF] */
    NI_U32                  u32KLevel;                  /*RW, 区域亮暗比下阈值, 灯光区域亮暗比需大于此阈值。Range: [0, 0xFF] */
}PISP_AF_ANTI_LAMP_S;


typedef struct tagPispBlkSize
{
    NI_U16                  u16BlkWidth;                /*RW, 算子块的宽度，需要32的整数倍 */
    NI_U16                  u16BlkHeight;               /*RW, 算子块的高度，需要16的整数倍 */
}PISP_BLK_SIZE_S;


typedef struct tagPispAfMeanFilterCoe 
{    
    NI_U32 u32CoeCenter;              /* 中心点权重 */     
    NI_U32 u32CoeEdge;                /* 边界点权重 */	   
    NI_U32 u32CoeCorner;              /* 角落点权重 */ 		
}PISP_AF_MEAN_FILTER_COE_S;


typedef struct tagPispAfMediumFilterCoe   
{    
    NI_U32 u32CoeMedium01;      /* 极值与次极值之间的噪声判断阈值 */     
    NI_U32 u32CoeMedium02;      /* 极值与次次极值之间的噪声判断阈值 */ 	    
    NI_U32 u32CoeMedium12;      /* 次极值与次次极值之间的噪声判断阈值 */ 		
}PISP_AF_MEDIUM_FILTER_COE_S;


typedef struct tagPispAfsParam
{
    NI_BOOL                     	bEnable;                /*RW, AF使能*/
    NI_BOOL                     	bOffsetEn;              /*RW, 偏移使能*/
    PISP_BLK_SIZE_S              	stImgPix;               /*RW, RAW转Y输入到AF模块的宽高 */
    PISP_AF_FILTER_TYPE_E        	enFilterType;           /*RW, 中值滤波和均值滤波是否开启的组合:A1B1  A1B0  A0B0  A0B1  B1A1。0-4，其它情况为错误的配置。*/
    RECT_POS_S                  	stRectPos;              /*RW, 设置统计区域的坐标*/
    PISP_AF_LAMB_FSWM_RATIO_E    	enLambFswmRatio;        /*RW, 灯光块Lamb与fswm块（算子块）的块个数划分比例。即 LambBlock/FswmBlock = ISP_AF_LAMB_FSWM_RATIO_E */
    PISP_BLK_SIZE_S              	stFswmBlkSize;          /*RW, 算子块的大小，宽需要32的倍数，高需要16的倍数 */
    PISP_AF_ANTI_LAMP_S          	stAntiLamp;             /*RW, 去灯光参数 */
    NI_BOOL                     	bAfDownSampleEn;        /*RW, AF下采样使能 */
    PISP_AF_MEAN_FILTER_COE_S    	stAfMeanFilterCoe;      /*RW, AF均值滤波系数 */
    PISP_AF_MEDIUM_FILTER_COE_S  	stAfMediumFilterCoe;    /*RW, AF中值滤波系数*/
}PISP_AF_PARAM_S;


typedef struct tagPispAfsStatistics
{
    NI_U32                  u32TotalValue;      /* RO, 统计的最终结果（满足条件的所有灯光块内fswm的累加值）*/
    NI_U32                  u32VaildBlkSum;     /* RO, 有效块数:去灯光使能开启时，经过去灯光后af块数；否则，有效区域的块数 */
    NI_U32                  u32LampBlkSum;      /* RO, 灯光块数:灯光块数个数（亮度上限值为max_level 。lamb块四均分块的均值最大值大于max_level的灯光块个数总和（横向和纵向）。与lamb_switch是否开启无关）*/
    NI_U32                  u32FswmVBlkSum;     /* RO, af总块数（有效区域的块数，有偏移时，fswm纵向帧）不计块偏移带来的影响 （理论上横向和纵向块数应该一致） */
    NI_U32                  u32FswmHBlkSum;     /* RO, af总块数（有效区域的块数，fswm横向） */
    NI_U32                  u32LambVBlkSum;     /* RO, af总块数（有效区域的块数，有偏移时，lamb纵向帧）（使用除lamp_level之外的阈值得到的统计） */
    NI_U32                  u32LambHBlkSum;     /* RO, af总块数（有效区域的块数，lamb横向） */
    NI_U32                  u32FswmHBlk;        /* RO, 实际处理每行的fswm块数（第一行），根据横向统计 */
    NI_U32                  u32LambHBlk;        /* RO, 实际处理每行的灯光块数（第一行），根据横向统计 */
    NI_U32                  u32AfMbufBlkId;     /* RO, pu32AfData对应缓存块的id号 */

    NI_U32                  u32PhyAddr;         /* 应用层不使用 */
    NI_U16                  *pu16AfData;        /* RO, Af统计数据 统计值空间排序:16*span*(1 + bOffsetEn) (bytes)的fswm（开偏移，fswm分横、纵）、 8*span*(1 + bOffsetEn) (bytes)的mean（开偏移，mean分横、纵）、1*span*(1 + bOffsetEn) (bytes)的lamb_flag（开偏移，lamb_flag分横、纵） ,其中span = 64的倍数
                                                   其中fswm分量占2Bytes, mean占1Byte, lamb_flag占1bit.*/
}PISP_AF_STATISTICS_S;


/**************************************************************************************************
									EE
***************************************************************************************************/
typedef struct tagPispEeLum
{
    NI_U8   u8LightHigh;           /* RW, EE高亮亮度阈值,U.8.0 rang[0,255], default 0x0 */
    NI_U8   u8OffsetThrd;          /* RW, EE偏移量阈值,U.8.0,rang[0,255], default 0x0 */
    NI_U8   u8LightNumLow;         /* RW, EE亮度计数统计低阈值,U.5.0,rang[0,31],default 0x10 */
    NI_U8   u8LightNumHigh;        /* RW, EE亮度计数统计高阈值,U.5.0,rang[0,31],default 0x0 */
}PISP_EE_LUM_S;

typedef struct tagPispEeCurv
{
    NI_U8   u8Thrd1;
    NI_U8   u8Thrd1Offset;
    NI_U8   u8Stren1;
    NI_U8   u8Stren2;
    NI_U8   u8Stren3;
}PISP_EE_CURV_S;


typedef struct tagPispEeParam
{
    NI_BOOL                 bEeBypass;              /* RW, ee模块使能开关,0：开，1：关。 */
    NI_BOOL                 bConsMode;              /* RW, ee对比度计算模式，低增益场景为0，高增益场景为1 */
    NI_BOOL                 bClipEn;                /* RW, EE正负边halo clip控制使能。0：关闭；1：打开 */
    NI_U8                   u8EdgeSmooth;           /* RW, 对边缘进行平滑处理,值越大，平滑程度越强 U.1.4,rang[0, 16], default 0x0 */
    NI_U8                   u8WidthRatio;           /* RW, 边缘宽度的总体控制程度,0为细边，16为粗边,U.1.4,rang[0, 16] default 0x8 */

    NI_U8                   u8NegStren;             /* RW, EE负（黑）边控制强度,U.2.4,rang[0, 63] ,default 0x0 */
    NI_U8                   u8NegClipRatio;         /* RW, EE负边halo控制强度,U.2.4,rang[0, 63], default 0xa */
    NI_U8                   u8PosStren;             /* RW, EE正（白）边控制强度,U.2.4,rang[0, 63], default 0x0 */
    NI_U8                   u8PosClipRatio;         /* RW, EE正边halo控制强度,U.2.4,rang[0, 63] default 0xa */

    NI_U8                   u8OffsetStren;          /* RW, 轮廓增强力度,U.4.4,rang[0, 255], default 0x10 */
               
    PISP_EE_LUM_S           stLum;                  /* RW, EE的亮度相关结构体 */  
    PISP_EE_CURV_S          stWidthCurv;            /* RW, EE边缘宽度控制相关结构体     窄边阈值u8Thrd1 rang[0, 247] 宽边阈值偏移u8Thrd1Offset rang[0, 247-u8Thrd1],分区强度u8Stren1~u8Stren3,U.2.4,rang[0, 63]*/ 
    PISP_EE_CURV_S          stThinAddCurv;          /* RW, EE的高频细节增强密度         低频阈值u8Thrd1 rang[0, 223] 高频阈值偏移u8Thrd1Offset rang[0, 223-u8Thrd1],分区强度u8Stren1~u8Stren3,U.2.4,rang[0, 63] */ 
    PISP_EE_CURV_S          stThinPowerCurv;        /* RW, EE的高频细节增强强度         低频阈值u8Thrd1 rang[0, 191] 高频阈值偏移u8Thrd1Offset rang[0, 191-u8Thrd1],分区强度u8Stren1~u8Stren3,U.2.4,rang[0, 63] */ 
    PISP_EE_CURV_S          stLumCurv;              /* RW, EE的亮度分段控制相关结构体   低亮阈值u8Thrd1 rang[0, 191] 高亮阈值偏移u8Thrd1Offset rang[0, 191-u8Thrd1],分区强度u8Stren1~u8Stren3,U.2.4,rang[0, 63]*/ 
    PISP_EE_CURV_S          stFreqCurv;             /* RW, EE的频率分段控制相关结构体   低频阈值u8Thrd1 rang[0, 223] 高频阈值偏移u8Thrd1Offset rang[0, 223-u8Thrd1],分区强度u8Stren1~u8Stren3,U.2.4,rang[0, 63]*/ 
    PISP_EE_CURV_S          stConsCurv;             /* RW, EE的对比度分段控制相关结构体 低对比度阈值u8Thrd1 rang[0, 223] 高对比度阈值偏移u8Thrd1Offset rang[0, 223-u8Thrd1],分区强度u8Stren1~u8Stren3,U.2.4,rang[0, 63]*/ 
}PISP_EE_PARAM_S;


typedef struct tagPispHdrMode
{
    HDR_MODE_E       enHdrMode;
}PISP_HDR_MODE_S;


/* 定义ISP模块级参数结构体 */
typedef struct tagPispModParam
{
    NI_BOOL     bAfBufferInit;                   /* ISP 初始化时是否为AF分配内存 */

    NI_U16      u16AwesBufBlkNum;                /* Awes统计模块的缓存块个数,  range: [3, 10] */
    NI_U16      u16AwesMaxBlockCnt;              /* Awes最大统计分块个数, range: [100, ISP_AWES_MAX_ZONE_NUM] */
    NI_U16      u16AfBufBlkNum;                  /* Af统计模块的缓存块个数,  range: [3, 16] */
    NI_U16      u16YhistBufBlkNum;               /* Yhist统计模块的缓存块个数,  range: [3, 16] */
    NI_U16      u16RawhistBufBlkNum;             /* Rawhist统计模块的缓存块个数,  range: [3, 10] */
}PISP_MOD_PARAM_S;


typedef struct tagPispPubAttr
{
    RECT_S                  stWndRect;      /*RW. 视频图像分辨率 宽度：[32, ISP_MAX_WIDTH]，建议16像素对齐。高度：[32, ISP_MAX_HEIGHT]，建议8像素对齐。*/
    NI_U32                  u32FrameRate;   /*RW, sensor 输出帧率, Range: [1, 1000] */
}PISP_PUB_ATTR_S;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif  /*  __NI_COMM_ISP_H__  */


