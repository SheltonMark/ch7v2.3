/*******************************************************************************
* Copyright (C), 2014-2020, ZheJiang Chipup Technology CO.,LTD.
*
* 文件名 : mdk_smart_enc.h
*
* 作  者 : wang_cong(233865)<wang_cong9@dahuatech.com>
* 版  本 : V1.0.0
* 日  期 : 2020年11月20日 Create
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
#ifndef __NI_COMM_SMART_ENC_H__
#define __NI_COMM_SMART_ENC_H__

/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include "ni_type.h"
#include "ni_debug.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ========================================================================== */
/*                             宏和类型定义区                                 */
/* ========================================================================== */
typedef NI_VOID *SMART_ENC_HANDLE;
//typedef unsigned long SMART_ENC_PARAM_CFG_ADDR;

/*错误代码*/
typedef enum tagSmartEncErrCode
{
	SMART_ENC_ERR_WRONG_PARAM = 0xFFFFFFFE,
	SMART_ENC_ERR_WRONG_HANDLE = 0xFFFFFFFD,
	SMART_ENC_ERR_NULL_PTR = 0xFFFFFFFC,
} SMART_ENC_ERR_CODE_E;


/*编码器编码类型*/
typedef enum tagSmartEncType
{
	SMART_ENC_TYPE_H264 = 0,
	SMART_ENC_TYPE_H265 = 1,
	SMART_ENC_TYPE_BUTT,
} SMART_ENC_TYPE_E;


/*编码模式*/
typedef enum tagSmartEncMode
{
	SMART_ENC_MODE_CBR = 0,
	SMART_ENC_MODE_VBR = 1,
	SMART_ENC_MODE_BUTT,
} SMART_ENC_MODE_E;


/*前景检测算法开启类型*/
typedef enum tagSmartEncFgMode
{
	SMART_ENC_FG_INTER_CLOSE = 0,          /*内部前景检测算法关闭*/
	SMART_ENC_FG_INTER_OPEN = 1,           /*内部前景检测算法开启*/
	SMART_ENC_FG_MODE_BUTT,
} SMART_ENC_FG_MODE_E;

/*前景检测算法开启类型*/
typedef enum tagSmartEncMdMode
{
	SMART_ENC_MD_INTER_CLOSE = 0,           /*动态场景检测算法关闭*/
	SMART_ENC_MD_INTER_OPEN = 1,            /*动态场景检测算法开启*/
	SMART_ENC_MD_MODE_BUTT,
} SMART_ENC_MD_MODE_E;



/*智能编码模式*/
typedef enum tagSmartEncRoiMode
{
	SMART_ENC_ROI_MODE_M0 = 0,        /*支持基础智能编码模式*/
	SMART_ENC_ROI_MODE_M1 = 1,        /*可支持编码器开启内部前景检测 ROI map模式，*/
	SMART_ENC_ROI_MODE_M2 = 2,        /*支持编码器开ROI map叠加外部前景检测*/
	SMART_ENC_ROI_MODE_BUTT,
} SMART_ENC_ROI_MODE_E;


/*图像大小*/
typedef struct tagSmartEncSize
{
	NI_U16	u16Width;
	NI_U16	u16Height;
} SMART_ENC_SIZE_S;


/*Qp范围*/
typedef struct tagtagSmartEncQpRange
{
	NI_U16  u16QpMax;
	NI_U16  u16QpMin;
} SMART_ENC_QP_RANGE_S;


/*编码图像类型*/
typedef enum tagSmartEncImageType
{

	SMART_ENC_IMAGE_TYPE_YUV420SP = 0x1,           /*YUV420 SemiPlanar*/
	SMART_ENC_IMAGE_TYPE_YUV422SP = 0x2,           /*YUV422 SemiPlanar*/
	SMART_ENC_IMAGE_TYPE_YUV420P = 0x3,            /*YUV420 Planar */
	SMART_ENC_IMAGE_TYPE_YUV422P = 0x4,            /*YUV422 planar */

	SMART_ENC_IMAGE_TYPE_BUTT

}SMART_ENC_IMAGE_TYPE_E;


/*图像类结构体*/
typedef struct tagSmartEncFrame
{
	SMART_ENC_IMAGE_TYPE_E   enImageType;
	NI_U8                    *apu8FrameVirAddr[3];    /*编码图像虚拟地址，分别对应Y,U,V*/
	NI_U32                   au32FramePhyAddr[3];     /*编码图像物理地址，分别对应Y,U,V*/
	NI_U16                   au16Stride[3];           /*图像跨度*/
	NI_U16                   u16Width;                /*图像的宽*/
	NI_U16                   u16Height;               /*图像的高*/
	NI_U16                   u16Reserved;
}SMART_ENC_FRAME_S;


/*roi优先级，优先级越高则分配码率越高*/
typedef enum tagSmartEncRoiPri
{
	SMART_ENC_ROI_PRI_0 = 0,          /*优先级0，背景优先级*/
	SMART_ENC_ROI_PRI_1 = 1,          /*优先级1，高于背景优先级*/
	SMART_ENC_ROI_PRI_2 = 2,          /*优先级2，高于优先级1*/
	SMART_ENC_ROI_PRI_3 = 3,          /*优先级3，最高优先级，不建议用户使用*/
	SMART_ENC_ROI_PRI_NUM,
} SMART_ENC_ROI_PRI_E;


/*目标对象区域，用户使用外部算法，将结果导入*/
typedef struct tagSmartEncObjRect
{
	SMART_ENC_ROI_PRI_E  enPri;             /*对象区域优先级*/
	NI_U16               u16StartX;         /*区域水平起点*/
	NI_U16               u16StartY;         /*区域垂直起点*/
	NI_U16               u16Height;         /*区域高度*/
	NI_U16               u16Width;          /*区域宽度*/
} SMART_ENC_OBJ_RECT_S;


/*对象区域信息*/
typedef struct tagSmartEncForeInfo
{
	NI_U32                   u32ObjRectNum;      /*对象区域数量*/
	NI_BOOL                  bSmartRoiPri;       /*前景检测是否开启优先级*/
	SMART_ENC_OBJ_RECT_S     *pstObj;            /*对象区域信息*/
} SMART_ENC_FORE_INFO_S;


/* Cuinfo组成结构体 */
typedef struct tagSmartEncCuInfo
{
	NI_BOOL             bValid;                 /* 当前CuInfoOutData是否有效 */
	NI_U32              u32CtuPerRow;           /* 横向Ctb个数 */
	NI_U32              u32CtuPerCol;           /* 纵向Ctb个数 */
	NI_U32              u32CuInfoPhyAddr;       /* Cuinfo所在的物理地址，由CuInfoTable + CtuInfo内容组成 */
	NI_U8               *pu8CuInfoVirAddr;      /* CtuInfo所在虚拟地址 */
	NI_U32              u32CuInfoTableSize;     /* CuInfoTable所占的内存大小 */
	NI_U32              u32CtbBitsPhyAddr;      /* CtbBits信息所在的物理地址 */
	NI_U8               *pu8CtbBitsVirAddr;     /* CtbBits信息所在的虚拟地址 */

	NI_U32              u32BlkId;
	NI_U8               u8FrameType;            /* 帧类型，其值参见VENC_FRAME_TYPE_E枚举定义 */
	NI_U32              u32CuInfoVersion;
} SMART_ENC_CU_INFO_S;

typedef struct tagSmartEncGopAttr
{
	NI_U32                 u32Gop;
	NI_U32                 u32DummyIGop;    /* 虚拟I帧间隔, 0表示不开启该功能(该帧只参考IDR/I帧) */
    NI_U32                 u32DummyIOffset; /* 一组GOP图像内第一个虚拟I帧相对于I/IDR帧的偏移 */
                                            /* 建议用户配置u32DummyIGop值大于u32DummyIOffset值使用 */
	                                        /* 当u32DummyIGop＞u32DummyIOffset时，第一个虚拟I帧的位置为相对于I/IDR帧的偏移u32DummyIOffset；
                                              当u32DummyIGop≤u32DummyIOffset时，第一个虚拟 I帧为I/IDR帧偏移u32DummyIGop+1位置*/
}SMART_ENC_GOP_ATTR_S;


typedef struct tagSmartEncQpMapInfo

{
	NI_U16          u16QpMapWidth;              /* qp map表的宽度，即水平方向BlockUnit的个数 */
	NI_U16          u16QpMapHeight;             /* qp map表的高度，即垂直方向BlockUnit的个数 */

	NI_U32          u32QpMapPhyAddr;            /* qp map表物理地址 */
	NI_VOID         *pQpMapVirAddr;            /* qp map表虚拟地址,用户态地址 */
	NI_U32          u32QpMapSize;
}SMART_ENC_QPMAP_FRAME_S;


/*process输入参数*/
typedef struct tagSmartEncFrameInfo
{
	SMART_ENC_FRAME_S            stDetectStream;
	SMART_ENC_FORE_INFO_S        stForeInfo;             /*外部输入的前景区域信息*/
	SMART_ENC_CU_INFO_S          stCuInfo;               /*编码器输出的cuinfoBuf*/
	NI_U16                       u16Gain;                /*当前图像增益*/
	NI_U32                       u32AverBitRate;         /*当前实时码率,单位bit/s*/
}SMART_ENC_FRAME_INFO_S;


/*智能编码参数配置地址*/
typedef struct tagSmartEncParamCfg
{
	NI_VOID                     *pParamCfgVirAddr;      /*参数配置虚拟地址*/
	NI_U32                      u32ParamCfgLen;         /*参数配置长度*/
}SMART_ENC_PARAM_CFG_S;

/*用户针对动静不同的场景设置目标码率*/
typedef struct tagSmartEncSceneTarget
{
	NI_U64   u64MotionTargetBitMin;     /*动态场景一段时间最小bit*/
	NI_U64   u64MotionTargetBitMax;     /*动态场景一段时间最大bit*/
	NI_U64   u64StaticTargetBitMin;     /*静态场景一段时间最小bit*/
	NI_U64   u64StaticTargetBitMax;     /*静态场景一段时间最大bit*/
	NI_U64   u64StateTime;              /*码率统计时间，单位为/s,范围[1, 86400]*/
}SMART_ENC_SCENE_TARGET_S;

/*用户设置判断动静场景的灵敏度*/
typedef struct tagSmartEncSceneSens
{
	NI_U32       u32MotionThr;            /*判断为动态场景的阈值，范围[10, 15000], 推荐1500*/
	NI_U32       u32StaticThr;			  /*判断为静态场景的阈值，范围[10, 15000], 推荐500*/
}SMART_ENC_SCENE_SENS_S;


/*功能模式选择*/
typedef struct tagSmartEncCreat
{
	SMART_ENC_ROI_MODE_E          enSmartMode;            /*智能编码的模式*/
	SMART_ENC_FG_MODE_E           enSmartFgMode;          /*内部检测算法是否开启*/
	SMART_ENC_MD_MODE_E           enSmartMdMode;          /*动态场景判断是否开启*/
	SMART_ENC_PARAM_CFG_S         stParamCfgAddr;         /*参数配置地址*/
	SMART_ENC_SIZE_S	          stDetectImgSize;        /*检测图像大小*/
	SMART_ENC_TYPE_E              enEncType;              /*编码器类型选择*/
} SMART_ENC_CREATE_S;


/*设置可变参数*/
typedef struct tagSmartEncSetCfg
{
	SMART_ENC_MODE_E                    enEncMode;              /*编码模式VBR/CBR*/
	SMART_ENC_SIZE_S	                stEncImgSize;           /*编码图像大小*/
	NI_U32                              u32BitRate;             /*码率,单位kbps,该码率必须和设置给编码器的码率保持一致*/
	NI_U16                              u16Quality;             /*图像质量,[0,7],4为推荐值*/
	SMART_ENC_GOP_ATTR_S                stGopAttr;              /*Gop相关属性*/
	SMART_ENC_SCENE_TARGET_S           stSceneTarget;           /*当用户开启动态场景判断时，需配置相应的码率*/
	SMART_ENC_SCENE_SENS_S             stSceneSens;             /*当用户开启动态场景判断时，需配置场景判断的灵敏度*/
	NI_U32                              u32FrmRateEnc;          /* 编码器输入帧率 range:[1, 65535]*/
    NI_U32                              u32FrmRateSmartEnc;     /* 智能编码输入帧率 range:[1, 65535] */
	//NI_S32                   s32AdjustBitRate;
}SMART_ENC_SET_CFG_S;


/*输出码控参数*/
typedef struct tagSmartEncRes
{
	NI_BOOL 		                    bQpMapEn;			 /* 是否启用QpMap功能 */
	SMART_ENC_QPMAP_FRAME_S             stQpMapFrame;
	NI_U32                              u32CtbRc;           /*CTB QP调整模式*/
	SMART_ENC_QP_RANGE_S                stQpRangeI;         /*I帧Qp范围*/
	SMART_ENC_QP_RANGE_S                stQpRangeP;         /*P帧Qp范围*/
	NI_BOOL                             bEnIDR;             /*强制I帧*/
	NI_U32                              u32Gop;             /*Gop*/
	NI_U16                              u16QpHdrMin;        /*最小Hdr*/
	NI_U32                              u32StaIPerc;        /*对应编码器参数u32StaticSceneIbitPercent*/
	NI_U32                              u32BlockRCSize;     /*对应编码器参数u32BlockRCSize*/
}SMART_ENC_RESULT_S;


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of #ifndef __NI_COMM_SMART_ENC_H__ */
