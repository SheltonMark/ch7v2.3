/* 
 * ni_comm_ive.h -- ive模块对外提供的头文件
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */

#ifndef __NI_COMM_IVE_H__
#define __NI_COMM_IVE_H__


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "ni_type.h"
#include "ni_errno.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */


/* ========================================================================== */
/*                         宏和类型定义区                                     */
/* ========================================================================== */
typedef NI_U32 IVE_HANDLE;
typedef unsigned long  IVE_PHY_ADDR;
typedef unsigned long  IVE_GRP_ID;

#define IVE_MAX_RECT_NUM (16)
typedef enum tagIveImageType
{ 
    IVE_IMAGE_TYPE_U8C1           =  0x0,      /*单通道，每个像素用8bit无符号数表示*/
    IVE_IMAGE_TYPE_S8C1           =  0x1,      /*单通道，每个像素用8bit有符号数表示*/

    IVE_IMAGE_TYPE_YUV420SP       =  0x2,      /*YUV420 SemiPlanar格式*/ 
    IVE_IMAGE_TYPE_YUV422SP       =  0x3,      /*YUV422 SemiPlanar格式*/ 
    IVE_IMAGE_TYPE_YUV420P        =  0x4,      /*YUV420 Planar格式 */ 
    IVE_IMAGE_TYPE_YUV422P        =  0x5,      /*YUV422 planar格式 */ 

    IVE_IMAGE_TYPE_S8C2_PACKAGE   =  0x6,      /*以Package格式存储2通道图像，每个像素有符号8bit*/
    IVE_IMAGE_TYPE_S8C2_PLANAR    =  0x7,       /*以Planar格式存储2通道图像，每个像素有符号8bit*/

    IVE_IMAGE_TYPE_S16C1           =  0x8,      /*单通道，每个像素用16bit有符号数表示*/
    IVE_IMAGE_TYPE_U16C1           =  0x9,       /*单通道，每个像素用16bit无符号数表示*/

    IVE_IMAGE_TYPE_U8C3_PACKAGE   =  0xa,       /*以Package格式存储3通道图像，每个像素无符号8bit*/
    IVE_IMAGE_TYPE_U8C3_PLANAR    =  0xb,       /*以Planar格式存储3通道图像，每个像素无符号8bit*/

    IVE_IMAGE_TYPE_S32C1           =  0xc,      /*单通道，每个像素用32bit有符号数表示*/
    IVE_IMAGE_TYPE_U32C1           =  0xd,      /*单通道，每个像素用32bit无符号数表示*/

    IVE_IMAGE_TYPE_ARGB8888         = 0xe,      
    IVE_IMAGE_TYPE_ARGB4444         = 0xf,
    IVE_IMAGE_TYPE_ARGB1555         = 0x10,

    IVE_IMAGE_TYPE_1BIT         = 0x11,         /*每个像素用1bit表示的图像类型*/
    IVE_IMAGE_TYPE_2BIT         = 0x12,         /*每个像素用2bit表示的图像类型*/
    IVE_IMAGE_TYPE_4BIT         = 0x13,         /*每个像素用4bit表示的图像类型*/

    IVE_IMAGE_TYPE_BUTT 
}IVE_IMAGE_TYPE_E; 


typedef enum tagIveOpModeType               
{ 
    IVE_OP_TYPE_SINGLE                 =  0x0,      /*单个任务*/
    IVE_OP_TYPE_GROUP                  =  0x1,      /*组任务*/

    IVE_OP_TYPE_BUTT 
}IVE_OP_TYPE_MODE_E; 


typedef struct tagIveOpMode
{ 
    IVE_GRP_ID  GrpId;          /*group模式下使用，为创建group时返回的ID*/        
}IVE_OP_MODE_S; 


typedef enum tagIveBigEndLittleEndType
{ 
    IVE_BIGEND_LITTLEEND_TYPE_SHORT         =  0x0,   /*Short类型*/
    IVE_BIGEND_LITTLEEND_TYPE_INT           =  0x1,   /*int类型*/  
    IVE_BIGEND_LITTLEEND_TYPE_BUSWIDTH      =  0x2,   /*总线位宽*/
    
    IVE_BIGEND_LITTLEEND_TYPE_BUT   
}IVE_BIGEND_LITTLEEND_TYPE_E; 


typedef struct tagIveBigEndLittleEndOp
{ 
    IVE_BIGEND_LITTLEEND_TYPE_E enType;              /*源0的数据大小端类型*/
    NI_BOOL                     bUnitIn;             /*源0数据类型内部是否进行大小端转换*/
    NI_BOOL                     bBetweenIn;          /*源0数据类型之间是否进行大小端转换*/    
}IVE_BIGEND_LITTLEEND_OP_S; 


typedef struct tagIveImage
{ 
    IVE_IMAGE_TYPE_E    enImageType;        /*图像类型*/
    IVE_BIGEND_LITTLEEND_OP_S  stBeLeOp;    /*大小端操作模式*/
    IVE_PHY_ADDR        aPhyAddr[3];        /*Physical address of the data*/ 
    NI_VOID             *apVirAddr[3];      /*Virtual address of the data*/
    NI_U16              au16Stride[3];      /*Data stride by byte*/ 
    NI_U16              u16Height;          /*Data height by byte, ForceRange:[1,1080]*/ 
    NI_U16              u16Width;           /*Data width by byte, ForceRange:[16,1920], ForceStep:2*/ 
    NI_U16              u16Reserved; 
}IVE_IMAGE_S; 

typedef IVE_IMAGE_S IVE_SRC_IMAGE_S;
typedef IVE_IMAGE_S IVE_DST_IMAGE_S; 


typedef struct tagIveMemInfo
{ 
    IVE_PHY_ADDR        PhyAddr;        /*Physical address of the data*/ 
    NI_VOID             *pVirAddr;      /*Virtual address of the data*/
    NI_U32              u32Size;        /*内存空间大小*/
}IVE_MEM_INFO_S; 

typedef IVE_MEM_INFO_S IVE_SRC_MEM_S;
typedef IVE_MEM_INFO_S IVE_DST_MEM_S; 

typedef enum tagIveFlterOpMode
{
    IVE_FILTER_OP_MODE_3x3_U8       = 0,  /*1byte输入， 1byte无符号输出*/
    IVE_FILTER_OP_MODE_5x5_U8       = 1,  /*1byte输入， 1byte无符号输出*/
    IVE_FILTER_OP_MODE_7x7_U8       = 2,  /*1byte输入， 1byte无符号输出*/
    IVE_FILTER_OP_MODE_3x3_S32      = 3,  /*1byte输入， 4byte有符号输出*/
    IVE_FILTER_OP_MODE_5x5_S32      = 4,  /*1byte输入， 4byte有符号输出*/
    IVE_FILTER_OP_MODE_BUTT,  
}IAS_FILTER_OP_MODE_E;


typedef enum tagIveNormMode
{
    IVE_NORM_SHIFTR   = 0x0,            /*右移模式*/
    IVE_NORM_DIV      = 0x1,            /*除法模式*/
    IVE_NORM_BUTT, 
}IVE_NORM_MODE_E;


typedef struct tagIveImageRect
{  
    NI_U16 u16LeftTopX;     /* 图像区域左上角横坐标 */
    NI_U16 u16LeftTopY;     /* 图像区域左上角纵坐标 */
    NI_U16 u16Width;        /* 图像区域宽度 */
    NI_U16 u16Height;       /* 图像区域高度 */
} IVE_IMAGE_RECT_S;


typedef struct tagIveImageRect1
{  
    NI_U16 u16LX;     /* 图像区域左上角横坐标 */
    NI_U16 u16LY;     /* 图像区域左上角纵坐标 */
    NI_U16 u16RX;     /* 图像区域右下角横坐标 */
    NI_U16 u16RY;     /* 图像区域右下角纵坐标 */
} IVE_IMAGE_RECT_1_S;


typedef struct tagIveGausNorm
{
    IVE_NORM_MODE_E         enNormMode;         /*归一化模式选择*/
    NI_U32                  u32Norm;            /*归一化值*/
} IVE_NORM_S;


typedef enum tagIveWinSize
{
	IVE_WIN_SIZE_3X3  = 0,  
	IVE_WIN_SIZE_5X5  = 1,   
	IVE_WIN_SIZE_7X7  = 2, 
	IVE_WIN_SIZE_BUTT,
}IVE_WIN_SIZE_E;


typedef enum tagIveDownSampleMode
{
    IVE_DOWM_SAMPLE_MODE_TOP      = 0,         /*用DOWN_WIDTH x DOWN_HEIGHT窗口中左上角像素代替下采样后的值。*/
    IVE_DOWM_SAMPLE_MODE_NZERO    = 1,         /*DOWN_WIDTH x DOWN_HEIGHT窗口中有一个像素值非零，则下采样值为TRUE_VALUE，否则为FALSE_VALUE*/
    IVE_DOWM_SAMPLE_MODE_THR      = 2,         /*DOWN_WIDTH x DOWN_HEIGHT窗口中所有像素累加和大于设定的阈值THRESH_16BIT_0，则下采样值为TRUE_VALUE，否则为FALSE_VALUE。*/
    IVE_DOWM_SAMPLE_MODE_AVERAGE  = 3,         /*DOWN_WIDTH x DOWN_HEIGHT窗口中所有像素累加和的均值作为下采样后的值。*/
    IVE_DOWM_SAMPLE_MODE_MAX      = 4,         /*DOWN_WIDTH x DOWN_HEIGHT窗口中最大值作为下采样后的值。*/

    IVE_DOWM_SAMPLE_MODE_BUTT,
}IVE_DOWM_SAMPLE_MODE_E;


typedef enum tagIveThreshMode
{
    IVE_THRESH_MODE_BINARY                 = 0,
    IVE_THRESH_MODE_TRUNC                  = 1,
    IVE_THRESH_MODE_TO_MINVAL             = 2,
    IVE_THRESH_MODE_MIN_MID_MAX           = 3,
    IVE_THRESH_MODE_ORI_MID_MAX            = 4,
    IVE_THRESH_MODE_MIN_MID_ORI            = 5,
    IVE_THRESH_MODE_MIN_ORI_MAX            = 6,
    IVE_THRESH_MODE_ORI_MID_ORI            = 7,
    IVE_THRESH_MODE_MIN_ORI_MAX_4BYTE     = 8,      
    IVE_THRESH_MODE_H8B_MIN_ORI_MAX     = 9,        /*xs7312新增模式*/

    IVE_THRESH_MODE_BUTT,
}IVE_THRESH_MODE_E;


typedef enum tagIveDmaOpMode
{
    IVE_DMA_MODE_LINE           = 0,        /*线性拷贝模式*/
    IVE_DMA_MODE_RECT           = 1,        /*矩形拷贝模式*/
    IVE_DMA_MODE_MASK           = 2,        /*带屏蔽的拷贝模式*/

    IVE_DMA_MODE_BUTT,
}IVE_DMA_OP_MODE_E;


typedef enum tagIveDmaCpyMode
{
    IVE_DMA_CPY_MODE_BYTE2BYTE             = 0,     /*单字节到单字节*/
    IVE_DMA_CPY_MODE_BYTE2WORD_L           = 1,     /*单字节到双字节,将单字放到双字的低byte*/
    IVE_DMA_CPY_MODE_BYTE2WORD_H           = 2,     /*单字节到双字节,将单字放到双字的高byte*/
    IVE_DMA_CPY_MODE_WORD2BYTE_L           = 3,     /*双字节到单字节，取双字的低byte*/
    IVE_DMA_CPY_MODE_WORD2BYTE_H           = 4,     /*双字节到单字节，取双字的高byte*/

    IVE_DMA_CPY_MODE_BUTT,
}IVE_DMA_CPY_MODE_E;


typedef enum tagIveBoundaryMode
{
	IVE_BOUNDARY_MODE_FIEX_VAL  = 0,   /*固定值*/
	IVE_BOUNDARY_MODE_KEEP_ORI  = 1,   /*边界保持原来值不变*/
	IVE_BOUNDARY_MODE_EXT       = 2,   /*边界扩展计算*/
	IVE_BOUNDARY_MODE_COPY_ORI  = 3,   /*边界圈像素复制源像素*/

	IVE_BOUNDARY_MODE_BUTT,
}IVE_BOUNDARY_MODE_E;


typedef struct tagIveBoundary
{
    IVE_BOUNDARY_MODE_E     enMode;         /*边界模式*/
    NI_U32                  u32Val;         /*边界值*/
} IVE_BOUNDARY_S;

typedef enum tagIveErodeOutputMode
{
	IVE_ERODE_OUTPUT_MODE_BINARIZATION     = 0,   
	IVE_ERODE_OUTPUT_MODE_CENTER           = 1,   
	IVE_ERODE_OUTPUT_MODE_SUM              = 2, 
	IVE_ERODE_OUTPUT_MODE_AVER             = 3, 
	IVE_ERODE_OUTPUT_MODE_CONDITION_AVER   = 4, 
	IVE_ERODE_OUTPUT_MODE_MAX              = 5, 

	IVE_ERODE_OUTPUT_MODE_BUTT,
}IVE_ERODE_OUTPUT_MODE_E;
/*目前并未使用该枚举变量*/


typedef enum tagIveNhErodeOpMode
{
	IVE_NH_ERODE_MODE_NC_SUN_THR      = 0,      /*中心点不进行求和，统一阈值*/
	IVE_NH_ERODE_MODE_SUN_THR         = 1,      /*中心点进行求和，统一阈值*/

	IVE_NH_ERODE_MODE_BUTT,
}IVE_NH_ERODE_OP_MODE_E;


typedef enum tagIveFieldSize
{
	IVE_4FIELD  = 0,   /*4邻域腐蚀，窗口范围固定3x3*/
	IVE_8FIELD  = 1,   /*8邻域腐蚀，窗口范围固定3x3*/

	IVE_FIELD_BUTT,
}IVE_FIELD_E;


typedef enum tagIveWinErodeMode
{
	IVE_WIN_ERODE_MODE_SUM_THR   = 0,       /*和值模式*/
	IVE_WIN_ERODE_MODE_PIEXL_THR = 1,       /*像素阈值模式*/
    
	IVE_WIN_ERODE_MODE_BUTT,
}IVE_WIN_ERODE_MODE_E;


typedef enum tagIveNhDlateFieldSize
{
	IVE_NH_DILATE_MODE_4FIELD  = 0,   /*4邻域腐蚀，窗口范围固定3x3*/
	IVE_NH_DILATE_MODE_8FIELD  = 1,   /*8邻域腐蚀，窗口范围固定3x3*/

	IVE_NH_DILATE_FIELD_MODE_BUTT,
}IVE_NH_DILATE_FIELD_E;


typedef enum tagIveNhDlateOpMode
{
	IVE_NH_DILATE_MODE_NC_SUN_THR      = 0,      /*中心点不进行求和，统一阈值*/
	IVE_NH_DILATE_MODE_SUN_THR         = 1,      /*中心点进行求和，统一阈值*/
	IVE_NH_DILATE_MODE_CONDITION_AVER  = 2,      /*满足条件均值（包含中心点）*/
	IVE_NH_DILATE_MODE_MAX             = 3,      /*取最大值（包含中心点）*/

	IVE_NH_DILATE_MODE_BUTT,
}IVE_NH_DILATE_OP_MODE_E;


typedef enum tagIveSubRefOpMode
{    
	IVE_SUB_REF_MODE_WIN_HOR   = 0,      /*支持窗口大小1~5*/      
	IVE_SUB_REF_MODE_WIN_VER   = 1,      /*支持窗口大小1~3*/      
    IVE_SUB_REF_MODE_WIN_3x3   = 2,      /*支持窗口3x3*/  
    IVE_SUB_REF_MODE_WIN_5x5   = 3,      /*支持窗口5x5*/  
    IVE_SUB_REF_MODE_WIN_7x7   = 4,      /*支持窗口7x7*/  
    
	IVE_SUB_REF_MODE_BUTT,
}IVE_SUB_WIN_REF_OP_MODE_E;


typedef enum tagIveSumOpMode
{
    IVE_SUM_IMG_REGION_U8       = 0X0,       /*图像0和图像1按像素相加（相同区域），结果无符号饱和*/
    IVE_SUM_IMG_FIXED_PIXEL     = 0x1,       /*图像0与FixedValue相加，结果无符号饱和*/
    IVE_SUM_IMG_REGION_S32      = 0x2,       /*图像0和图像1按像素相加（相同区域）,四字节有符号输入并结果有符号输出*/
    /*以下几个模式尚未实现*/
#if 0
    IVE_SUM_IMG_IMAGE           = 0X3,       /*两图像相加*/
    IVE_SUM_IMG_MASK            = 0X4,       /*图像像素和计算，mask*/
    IVE_SUM_IMG_MASK_NOT        = 0x5,       /*图像像素和计算，mask，与模式1相反*/
#endif

    IVE_SUM_MODE_BUTT,
}IVE_SUM_OP_MODE_E;


typedef enum tagIveCmpOpMode
{
    IVE_CMP_IMG_U8             = 0x0,           /*两帧单字节图像对比*/
    IVE_CMP_FIXED_THR          = 0x1,           /*一帧图像与用户设置的阈值比较*/

    IVE_CMP_BUTT,
}IVE_CMP_OP_MODE_E;

typedef enum tagIveSobelOpMode
{  
	IVE_SOBEL_OUT_ABS_H_V_MAX               = 0,   /* 输出U8C1水平垂直的绝对值以及两者中的大者，需要配置右移位数 */     
	IVE_SOBEL_OUT_H_V_ABS_SUM               = 1,   /* 输出S16C1水平垂直的有符号数，以及U8C1两者绝对值之和,需要配置右移位数*/ 
	IVE_SOBEL_OUT_ABS_MAX_U16               = 2,   /* 输出U16水平垂直绝对值之中的大值*/ 
	IVE_SOBEL_OUT_ABS_MAX_U8                = 3,   /* 输出U8水平垂直绝对值之中的大值,需要配置右移位数*/ 
    
	IVE_SOBEL_OUT_BUTT,
}IVE_SOBEL_OP_MODE_E;


typedef enum tagIveSobelDealOpMode
{  
	IVE_SOBEL_DEAL_MAX_ABS                  = 0,   /* 输出U16C1, 求取PH/PV绝度值得大者，PH/PV为双字节有符号数 */     
	IVE_SOBEL_DEAL_SUM_ABS                  = 1,   /* 输出U8C1, 求取PH/PV绝对值的和值，并将结果限制在单字节（配置右移RB_SHIFT）*/ 
   
	IVE_SOBEL_DEAL_MODE_BUTT,
}IVE_SOBEL_DEAL_OP_MODE_E;


typedef enum tagIveRopMode
{  
	IVE_ROP_MODE_LOGIC_AND                  = 0,    /*逻辑与*/ 
	IVE_ROP_MODE_LOGIC_OR                   = 1,    /*逻辑或*/  
  	IVE_ROP_MODE_LOGIC_NOT                  = 2,    /*逻辑非*/
  	IVE_ROP_MODE_BIT_AND                    = 3,    /*按位与*/
  	IVE_ROP_MODE_BIT_OR                     = 4,    /*按位或*/
   	IVE_ROP_MODE_BIT_ANDN                   = 5,    /*按位与非*/
   	IVE_ROP_MODE_BIT_ORN                    = 6,    /*按位或非*/
   	IVE_ROP_MODE_BIT_NAND                   = 7,    /*按位非与*/
   	IVE_ROP_MODE_BIT_NOR                    = 8,    /*按位非或*/  	
   	IVE_ROP_MODE_BIT_XNOR                   = 9,    /*按位同或*/
   	IVE_ROP_MODE_BIT_XOR                    = 10,    /*按位异或*/
   	IVE_ROP_MODE_BIT_NOT                    = 11,    /*按位非*/

	IVE_ROP_MODE_BUTT,
}IVE_ROP_MODE_E;

typedef enum tagIveRopFixedValMode
{  
	IVE_ROP_FIEXED_VAL_NONE     = 0,    /*不与固定值进行与操作*/ 
	IVE_ROP_SRC1_OR_VAL         = 1,    /*SRC1与固定值进行或操作， 仅位操作支持*/ 
	IVE_ROP_SRC1_OR_VAL_OP_SRC2 = 2,    /*SRC1与固定值进行或操作再和SRC2进行， 仅位操作支持*/ 

	IVE_ROP_FIEXED_VAL_MODE_BUTT,
}IVE_ROP_FIEXED_VAL_MODE_E;


typedef enum tagIveClearRefMode
{  
	IVE_CLEAR_REF_MODE_FIXED_VAL            = 0,        /*固定值模式*/
	IVE_CLEAR_REF_MODE_IMAGE                = 1,        /*像素值模式*/

	IVE_CLEAR_REF_MODE_BUTT,
}IVE_CLEAR_REF_MODE_E;

typedef enum tagIveClearRefOp
{  
	IVE_CLEAR_REF_OP_BIT_AND            = 0,    
	IVE_CLEAR_REF_OP_BIT_OR             = 1,   
	IVE_CLEAR_REF_OP_BIT_XOR            = 2, 
	IVE_CLEAR_REF_OP_BIT_XNOR           = 3, 
	IVE_CLEAR_REF_OP_BIT_ANDN           = 4, 
	IVE_CLEAR_REF_OP_BIT_NAND           = 5, 

	IVE_CLEAR_REF_OP_BUTT,
}IVE_CLEAR_REF_OP_E;


typedef enum tagIveValueAdjust
{  
	IVE_VALUE_ADJUST_MODE_0            = 0, /*3源图像操作，源图像0与源图像1对应位置像素点相等，则源图像2对应位置减去配置值SUB_VAL，否则源图像2对应位置加上配置值ADD_VAL*/   
	IVE_VALUE_ADJUST_MODE_1            = 1, /*4源图像操作，如果源图像3对应点像素值等于用户设置的阈值FIX_VALUE0或FIX_VALUE1，则保留源图像2对应位置像素值，否则按模式0进行操作*/     

	IVE_VALUE_ADJUST_MODE_BUTT,
}IVE_VALUE_ADJUST_MODE_E;


typedef enum tagIveNccMode
{  
	IVE_NCC_MODE_0            = 0, /*3源图像操作，源图像0与源图像1对应位置像素点相等，则源图像2对应位置减去配置值SUB_VAL，否则源图像2对应位置加上配置值ADD_VAL*/   
	IVE_NCC_MODE_1            = 1, /*4源图像操作，如果源图像3对应点像素值等于用户设置的阈值FIX_VALUE0或FIX_VALUE1，则保留源图像2对应位置像素值，否则按模式0进行操作*/     

	IVE_NCC_MODE_BUTT,
}IVE_NCC_MODE_E;


typedef enum tagIveImgProjectHvMode
{  
	IVE_IMG_PROJECT_HV_MODE_H            = 0,       /*计算图像水平方向上的灰度投影*/
	IVE_IMG_PROJECT_HV_MODE_V            = 1,       /*计算图像垂直方向上的灰度投影*/
	IVE_IMG_PROJECT_HV_MODE_HV           = 2,       /*同时计算水平和垂直方向上的灰度投影*/

	IVE_IMG_PROJECT_HV_MODE_BUTT,
}IVE_IMG_PROJECT_HV_MODE_E;


typedef enum tagIveYuvConvertMode
{  
	IVE_CONVERT_MODE_UYVY2YUV420             = 0, 
	IVE_CONVERT_MODE_UYVY2YUV422             = 1,
	IVE_CONVERT_MODE_VYUY2YUV420             = 2, 
	IVE_CONVERT_MODE_VYUY2YUV422             = 3,
	IVE_CONVERT_MODE_YUYV2YUV420             = 4, 
	IVE_CONVERT_MODE_YUYV2YUV422             = 5,
	IVE_CONVERT_MODE_YVYU2YUV420             = 6, 
	IVE_CONVERT_MODE_YVYU2YUV422             = 7,

	IVE_CONVERT_MODE_BUTT,
}IVE_YUV_CONVERT_MODE_E;


typedef enum tagIveYv12ToHMode
{  
	IVE_YV12_TO_RGB             = 0,        /*转换为RGB图的模式*/
	IVE_YV12_TO_H               = 1,        /*转换为H色度图的模式*/

	IVE_YU12_TO_H_MODE_BUTT,
}IVE_YU12_TO_H_MODE_E;


typedef enum tagIveImgThrAdapScaleMode
{  
	IVE_IMG_THR_ADAP_SCALE_MODE_0             = 0,      /*Pout0(i)=Pin0(i)≤MAX((Pin1(i)?8)×Pin2(i),MIN_TH) ? FALSE_VALUE∶ TRUE_VALUE*/
	IVE_IMG_THR_ADAP_SCALE_MODE_1             = 1,      /*Pout0(i)=Pin0(i)≤MAX((Pin1(i)?8)×scale,MIN_TH) ? FALSE_VALUE∶ TRUE_VALUE*/

	IVE_IMG_THR_ADAP_SCALE_MODE_BUTT,
}IVE_IMG_THR_ADAP_SCALE_MODE_E;


typedef enum tagIveUpdSimilarMeanMode
{  
	IVE_UPD_SIMILAR_MEAN_MODE_0             = 0,    /*先和 Pin0(i)比较，再将Pin1(i)直接加/减像素Pin2(i)，获取Pout0的值*/
	IVE_UPD_SIMILAR_MEAN_MODE_1             = 1,    /*in0(i)及MAX_SPEED分别比较，再将Pin1(i)加/减像素Pin2(i)与MAX_SPEED的中的较小值，获取Pout0的值*/
	IVE_UPD_SIMILAR_MEAN_MODE_2             = 2,    /*先和 Pin0(i)比较，再将Pin1(i)直接加/减设定值FixedValue，得到Pout0的值*/

	IVE_UPD_SIMILAR_MEAN_MODE_BUTT,
}IVE_UPD_SIMILAR_MEAN_MODE_E;


typedef enum tagIveUpdSimilarMeanRefMode
{  
	IVE_UPD_SIMILAR_MEAN_REG_MODE_0             = 0,    /*先用Pin1(i) ，Pin0(i)， MAX_REF比较，再将Pin2(i)直接加/减设置的FixedValue值，获取最终的Pout0*/
	IVE_UPD_SIMILAR_MEAN_REG_MODE_1             = 1,    /*先用Pin1(i)，Pin0(i)，MAX_REF比较，再将Pin2(i)加/减MAX_SPEED与Pin0(i)移位后的两者中的小值，作为最终的Pout0*/

	IVE_UPD_SIMILAR_MEAN_REG_MODE_BUTT,
}IVE_UPD_SIMILAR_MEAN_REG_MODE_E;


typedef enum tagIveThrRefMode
{  
	IVE_THR_REF_MODE_0             = 0,     /*两级判断设定值选择；使用Pin0(i)，Pin1(i)*/
	IVE_THR_REF_MODE_1             = 1,     /*三级判断设定值选择；使用Pin0(i)，Pin1(i)*/
	IVE_THR_REF_MODE_2             = 2,     /*四级判断设定值选择（最后一级移位LB位）；使用Pin0(i)，Pin1(i)，Pin2(i)*/
	IVE_THR_REF_MODE_3             = 3,     /*四级判断设定值选择使用Pin0(i)，Pin1(i)，Pin2(i)*/
	IVE_THR_REF_MODE_4             = 4,     /*背景更新没有发生光照变化*/
	IVE_THR_REF_MODE_5             = 5,     /*背景更新发生光照变化*/

	IVE_THR_REF_MODE_BUTT,
}IVE_THR_REF_MODE_E;


typedef enum tagIveSetRefMode
{  
	IVE_SET_REF_MODE_EQUAL         = 0, 
	IVE_SET_REF_MODE_NOT_EQUAL     = 1,
	IVE_SET_REF_MODE_GREATER       = 2, 
	IVE_SET_REF_MODE_NOT_GREATER   = 3, 
	IVE_SET_REF_MODE_LESS          = 4, 
	IVE_SET_REF_MODE_NOT_LESS      = 5, 

	IVE_SET_REF_MODE_BUTT,
}IVE_SET_REF_MODE_E;

typedef enum tagIveSubAbsCoefMode
{  
	IVE_SET_SUB_ABS_COEF_MODE_0    = 0,         /*实现像素加权差的绝对值*/
	IVE_SET_SUB_ABS_COEF_MODE_1    = 1,         /*Src0和Src1分别是待混合的RGB分量，Dst0是目标输出分量*/
    IVE_SET_SUB_ABS_COEF_MODE_2    = 2,         /*Src1和Src2分别是待混合RGB分量，Src0是alpha(单字节)，Dst0是目标输出分量*/
    IVE_SET_SUB_ABS_COEF_MODE_3    = 3,         /*Src1和Src2分别是待混合RGB分量，Src0是alpha(双字节)，Dst0是目标输出分量*/
	IVE_SET_SUB_ABS_COEF_MODE_4    = 4,         /*Src1,2,3分别是待混合图像1的R,G,B分量，Src0是ARGB8888格式的图像0*/
	IVE_SET_SUB_ABS_COEF_MODE_5    = 5,         /*Src1是待混合图像1的分量，Src0是图像0分量(例如A0G0，两字节)，Dst是目标输出分量*/

	IVE_SET_SUB_ABS_COEF_BUTT,
}IVE_SUB_ABS_COEF_MODE_E;


typedef enum tagIveLocalSigmaMode
{  
	IVE_LOCAL_SIGMA_MODE_AVER               = 0,        /*仅输出均值*/
    IVE_LOCAL_SIGMA_MODE_SIGMA              = 1,        /*仅输出Sigma值*/
	IVE_LOCAL_SIGMA_MODE_AVER_AND_SIGMA     = 2,        /*同时输出均值和Sigma值*/

	IVE_LOCAL_SIGMA_OUTPUTMODE_BUTT,
}IVE_LOCAL_SIGMA_MODE_E;


typedef enum tagIveAngleMode
{  
	IVE_ANGLE_MODE_ARCTAN               = 0, 
	IVE_ANGLE_MODE_EVOLUTION            = 1,

	IVE_ANGLE_MODE_BUTT,
}IVE_ANGLE_MODE_E;
/*该枚举变量目前并未使用*/

typedef enum tagIveAngleByDxDyMode
{  
	IVE_ANGLE_BY_DXDY_MODE_0       = 0,    /*根据像素值代表的水平、垂直方向的图像点数值差值dx,dy[-255,255],利用dx的反正切求角度*/
	IVE_ANGLE_BY_DXDY_MODE_1       = 1,    /*根据像素值代表的水平、垂直方向的图像点数值差值dx,dy[-255,255],利用dx的开平方求角度*/

	IVE_ANGLE_BY_DXDY_MODE_BUTT,
}IVE_ANGLE_BY_DXDY_MODE_E;


typedef enum tagIvePointFltMode
{  
	IVE_POINT_FLT_MODE_0            = 0,     /*针对二值化图像，用3x3的模板，查表决定是否滤除中心点。单源图像按照行方向滤波*/
	IVE_POINT_FLT_MODE_1            = 1,     /*针对二值化图像，用3x3的模板，查表决定是否滤除中心点，滤除时判断条件增加判断另两幅图像的对应像素值之差的绝对值*/

	IVE_POINT_FLT_MOD_BUTT,
}IVE_POINT_FLT_MODE_E;

typedef enum tagIveIntegralMode
{  
	IVE_INTEGRAL_ORI_PIX            = 0,    /*表示原像素积分*/
	IVE_INTEGRAL_SQUARE_PIX         = 1,    /*像素平方积分*/

	IVE_INTEGRAL_BUTT,
}IVE_INTEGRAL_MODE_E;

typedef enum tagIveThresholdMode
{  
	IVE_THRESHOLD_MODE_0         = 0,   /*图像A像素值大于图像B对应位置像素值*/
	IVE_THRESHOLD_MODE_1         = 1,   /*图像A像素值大于图像B对应位置像素值与SCALE的乘积*/
    IVE_THRESHOLD_MODE_2         = 2,   /*图像A像素值大于配置阈值对应位置像素值*/
    IVE_THRESHOLD_MODE_3         = 3,   /*图像A像素值减去图像B的绝对值大于阈值*/        

	IVE_THRESHOLD_BUTT,
}IVE_THRESHOLD_MODE_E;


typedef struct tagIveDmaRectParam
{
    IVE_IMAGE_RECT_S        stSrcImgRect;
    IVE_IMAGE_RECT_S        stDstImgRect;  
} IVE_DMA_RECT_PARAM_S;


typedef struct tagIveDmaMaksParam
{
    IVE_IMAGE_S         stMask;
    NI_U8               u8Mask;        /* MASK模式下使用，当mask表中的值为该值，则当前像素mask生效 */
} IVE_DMA_MASK_PARAM_S;


/* 定义DMA控制信息 */
typedef struct tagIveDmaCtrl
{
    IVE_DMA_OP_MODE_E        enOpMode;       /*算子模式*/
    IVE_DMA_CPY_MODE_E       enCopyMode;     /*操作模式*/
    union
    {
        IVE_DMA_RECT_PARAM_S stRectParam;    /*UNION: enOpMode=IVE_DMA_MODE_RECT | Rect模式下使用*/
        IVE_DMA_MASK_PARAM_S stMaskParam;    /*UNION: enOpMode=IVE_DMA_MODE_MASK | Mask模式下使用*/
    };
} IVE_DMA_CTRL_S;


/* 定义256级直方图统计控制信息, 支持U8C1 和 U16C1*/
typedef struct tagIveHist
{
    IVE_IMAGE_RECT_S    stRect;
} IVE_HIST_CTRL_S;


/* 定义图像邻域腐蚀控制信息 */
typedef struct tagIveErodeNhCtrl
{
    IVE_NH_ERODE_OP_MODE_E  enOpMode;       /*算子模式*/
    IVE_FIELD_E             enField;        /*领域范围*/
    NI_U8                   u8BoundaryVal;      /*边界值*/
    NI_U16                  u16SumThr;       /* 邻域值和值阈值 */
    NI_U8                   u8TrueVal;      /*正确值*/
    NI_U8                   u8FalseVal;     /*错误值*/
} IVE_ERODE_NH_CTRL_S;


typedef struct tagIveErodeWinCtrl
{
    IVE_WIN_ERODE_MODE_E    enOpMode;        /*算子模式*/
    IVE_WIN_SIZE_E          enWinSize;          /*窗口尺寸*/
    NI_U8                   u8BoundaryVal;      /*边界值*/
    union
    {
        IVE_SRC_IMAGE_S     stPiexlThr;          /* UNION: enOpMode=IVE_WIN_ERODE_MODE_PIEXL_THR | 像素阈值模式下使用，和值大于mask中对应的位置的值则输出原值，否则输出falseVal */
        NI_U16               u16SumThr;           /* UNION: enOpMode=IVE_WIN_ERODE_MODE_SUM_THR | 和值模式下使用,和值大于该阈值则输出原值，否则输出falseVal*/
    };    
    NI_U8                   u8FalseVal;   
} IVE_ERODE_WIN_CTRL_S;


/* 定义图像行腐蚀控制信息 */
typedef struct tagIveErodeHorCtrl
{
    NI_U16                  u16ErodeNum;        /*腐蚀数量*/
    NI_U8                   u8TrueVal;          /*正确值*/
    NI_U8                   u8FalseVal;         /*错误值*/
} IVE_ERODE_HOR_CTRL_S;


/* 定义图像邻域膨胀控制信息 */
typedef struct tagIveDilateNhCtrl
{
    IVE_NH_DILATE_OP_MODE_E  enOpMode;        /*算子模式*/
    IVE_FIELD_E              enField;         /*领域模式*/
    NI_U16                   u16SumThr;       /* 邻域值和值阈值 */
    NI_U8                    u8TrueVal;        /*正确值*/
    NI_U8                    u8FalseVal;        /*错误值*/     
} IVE_DILATE_NH_CTRL_S;


/* 定义图像窗口膨胀控制信息 */
typedef struct tagIveDilateWinCtrl
{
    IVE_WIN_SIZE_E           enWinSize;         /*窗口尺寸*/
    NI_U16                   u16SumThr;          /*和值的阈值*/
    NI_U8                    u8TrueVal;         /*正确值*/
    NI_U8                    u8FalseVal;        /*错误值*/
} IVE_DILATE_WIN_CTRL_S;


/* 定义图像行膨胀控制信息 */
typedef struct tagIveDilateHorCtrl
{
    NI_U16                  u16DilateNum;       /*膨胀数量*/
    NI_U8                   u8TrueVal;          /*正确值*/
    NI_U8                   u8FalseVal;         /*错误值*/
} IVE_DILATE_HOR_CTRL_S;


/* 定义U8减U16控制信息 */
/*typedef struct tagIveSubU16SU8Ctrl
{
    NI_U8   u8Thr;
} IVE_SUB_U16SU8_CTRL_S;*/

typedef struct tagIveSubFixedVal
{
    NI_U8   u8FalseVal;         /*错误值*/
    NI_U8   u8FixedVal;         /*固定值*/
} IVE_SUBFIXEDVAL_CTRL_S;


/* 定义图像减法控制信息 */
typedef struct tagIveSubWinRefCtrl
{
    IVE_SUB_WIN_REF_OP_MODE_E    enOpMode;      /*算子模式*/
    NI_BOOL                  bThrMode;        /*阈值模式，为0表示PREF>THRESH，1表示PREF<=THRESH*/   
    NI_U8                    u8HalfWin;         
    NI_U8                    u8Thr;
    NI_U8                    u8MinDiff;
    NI_U8                    u8FalseVal; 
} IVE_SUB_WIN_REF_CTRL_S;


typedef struct tagIveSumCtrl
{
    IVE_SUM_OP_MODE_E   enMode;         /*算子模式*/
    IVE_IMAGE_RECT_S    stRect;         /*矩形区域*/
    NI_U8               u8FixVal;       /*固定值*/
    //NI_U8               u8Thr;
} IVE_SUM_CTRL_S;


typedef struct tagIveImgCompareStatics
{
    NI_U32 u32NumEq;             /*相等的像素个数*/
    NI_U32 u32NumLarger;         /*大于比较值的的像素个数*/
} IVE_IMG_COMPARE_STATICS_S;


typedef struct tagIveImgCompareCtrl
{
    IVE_CMP_OP_MODE_E       enMode;          /*算子模式*/
    NI_U16                  u16Thr;          /*阈值*/
}IVE_IMG_COMPARE_CTRL_S;

typedef struct tagIveSobelCtrl
{
    IVE_SOBEL_OP_MODE_E    enMode;          /*算子模式*/
    NI_U8                  u8ShiftR;        /*右移值*/
    NI_U16                 u16BoundaryVal;      /*边界值*/
    NI_S8                  s8HgMask[9];         /*水平梯度值*/
	NI_S8                  s8VgMask[9];         /*垂直梯度值*/
} IVE_SOBEL_CTRL_S;


typedef struct tagIveSobelDealCtrl
{
    IVE_SOBEL_DEAL_OP_MODE_E    enMode;        /*算子模式*/
    NI_U8                       u8ShiftR;       /*右移值*/
} IVE_SOBEL_DEAL_CTRL_S;


/*Filter*/
typedef struct tagIveFilterCtrl
{
    IAS_FILTER_OP_MODE_E enOpMode;          /*工作模式*/
    IVE_NORM_S           stNorm;            /*归一化，右移模式支持范围[0:23]bit，除法模式支持[1:4095]*/
    NI_BOOL              bFirstLineOut;     /*第一行是否输出*/
    NI_U32               u32SetVal;         /*周围圈输出置位数据*/
    NI_S8                s8Mask[49];
} IVE_FILTER_CTRL_S;


typedef struct tagIveUpSampleCtrl
{
    NI_U8      u8ScaleW;        /*1~255*/
    NI_U8      u8ScaleH;        /*1~255*/
} IVE_UP_SAMPLE_CTRL_S;


typedef struct tagIveDownSampleCtrl
{
    IVE_DOWM_SAMPLE_MODE_E enOMode;         /*算子模式*/
    NI_U8                  u8ScaleW;        /*横向采样尺度*/
    NI_U8                  u8ScaleH;        /*纵向采样尺度*/
    NI_U16                 u16Thr;          /*IVE_DOWM_SAMPLE_MODE_THR,IVE_DOWM_SAMPLE_MODE_THR有效 */
    NI_U16                 u16TrueValue;    /*IVE_DOWM_SAMPLE_MODE_NZERO, IVE_DOWM_SAMPLE_MODE_THR有效 */
    NI_U16                 u16FalseValue;   /*IVE_DOWM_SAMPLE_MODE_NZERO, IVE_DOWM_SAMPLE_MODE_THR有效 */
    IVE_NORM_S             stNorm;          /*IVE_DOWM_SAMPLE_MODE_AVERAGE 有效*/
} IVE_DOWN_SAMPLE_CTRL_S;


typedef struct tagIveThresh
{
    IVE_THRESH_MODE_E      enMode;          /*算子模式*/
    NI_U16                 u16LowThr;       /*下限值*/
    NI_U16                 u16HighThr;      /*上限值*/
    NI_U16                 u16MinVal;       /*无符号16bit最小值*/
    NI_U16                 u16MaxVal;       /*无符号16bit最大值*/
    NI_U16                 u16MidVal;       /*无符号16bit中间值*/
    NI_S32                 s32MinVal;       /*有符号32bit最小值*/
    NI_S32                 s32MaxVal;       /*有符号32bit最大值*/
    
    /*以下为xs7312新模式需要而增加的变量，xs7320不需要使用*/
    NI_U32                 u32MinVal;       /*无符号32bit最小值*/
    NI_U32                 u32MaxVal;       /*无符号32bit最大值*/
    
} IVE_THRESH_CTRL_S;


typedef struct tagIveRop
{
    IVE_ROP_MODE_E              enMode;      /*算子模式*/
    IVE_ROP_FIEXED_VAL_MODE_E   enFixedVal;  /*仅bit操作支持*/
    NI_U16                      u16FixedVal; /*仅fixedvalue模式选择为1或者2生效*/   
} IVE_ROP_CTRL_S;


typedef struct tagIveClearRef
{
    IVE_CLEAR_REF_MODE_E        enMode;     /*算子模式*/
    IVE_CLEAR_REF_OP_E          enOp;       /*操作模式*/
    NI_U8                       u8FixedVal; /*仅IVE_CLEAR_REF_MODE_FIXED_VAL模式生效*/ 
    NI_U8                       u8SetVal;   /*仅IVE_CLEAR_REF_MODE_FIXED_VAL模式生效*/ 
} IVE_CLEAR_REF_CTRL_S;


typedef struct tagIveValueAdjist
{
    IVE_VALUE_ADJUST_MODE_E     enMode;         /*算子模式*/
    NI_U8                       u8AddVal;       /*增加值*/
    NI_U8                       u8SubVal;       /*减少值*/
    NI_U8                       u8FixedVal0;    /*固定值0*/
    NI_U8                       u8FixedVal1;    /*固定值1*/
} IVE_VALUE_ADJUST_CTRL_S;


typedef struct tagIveNcc
{
    IVE_WIN_SIZE_E  enWinSize;    /*只支持3X3 和5X5*/
    NI_BOOL         bEnMask;      /*mask使能，如果使能 需要传入MASK图像*/
    NI_U16          u16NccThr;      /*Ncc阈值*/
    NI_U16          u8BoundaryVal;  /*边界值*/
    NI_U8           u8TureVal;      /*正确值*/
    NI_U8           u8FalseVal;     /*错误值*/
} IVE_NCC_CTRL_S;


typedef struct tagIveSetImgValue
{
    IVE_WIN_SIZE_E  enWinSize;      /*只支持3X3 和5X5*/
    NI_U8           u8BoundaryVal;  /*边界值*/
    NI_U8           u8TureVal;      /*正确值*/
    NI_U8           u8FalseVal;     /*错误值*/
} IVE_SET_IMG_VALUE_CTRL_S;


typedef struct tagIveVarianceCtrl
{
    NI_BOOL             bEnMask;                    /*mask使能，如果使能 需要传入MASK图像*/
    IVE_IMAGE_RECT_1_S  astRect[IVE_MAX_RECT_NUM];      
    NI_U8               u8RectNum;    
    NI_U8               u8MaskThr;                  /*mask 模式开启时，当mask图像值等于maskthr时，当前像素才会参与计算*/
    NI_U16              u16PiexlThr;                /*当前图像像素值小于该阈值时，当前像素才会参与计算*/
    NI_U16              u16AroundThr;               /*周围像素阈值，周围一圈像素小于该值时，当前像素才会参与计算*/
} IVE_VARIANCE_CTRL_S;

typedef struct tagIveVarianceResult
{
    NI_U32              au32VarRes[IVE_MAX_RECT_NUM];               /*方差均值结果*/
    NI_U32              au32AveRes[IVE_MAX_RECT_NUM];               /*像素均值结果*/
    NI_U8               u8RectNum;    
} IVE_VARIANCE_RESULT_S;

typedef struct tagIveIntegralResult
{
    IVE_INTEGRAL_MODE_E enIntegralMode;         /*算子模式*/
} IVE_INTEGRAL_CTRL_S;


typedef struct tagIveThreshold
{
    IVE_THRESHOLD_MODE_E enThresholdMode;       /*算子模式*/
    NI_U8                u8Scale;               
    NI_U32               u32MinDiff;
    NI_U8                u8TureVal;
    NI_U8                u8FalseVal;    
} IVE_THRESHOLD_CTRL_S;


typedef struct tagIveProjectHv
{
    IVE_IMG_PROJECT_HV_MODE_E   enMode;         /*算子模式*/
    NI_U8                       u8BlockSize;    /*0:整幅图像为一个块， 1:图像划分为2X2 ... 15:图像划分为16X16*/               
} IVE_PROJECT_HV_CTRL_S;


typedef struct tagIveYuvConvertCtrl
{
    IVE_YUV_CONVERT_MODE_E   enMode;          /*算子模式*/
} IVE_YUV_CONVERT_CTRL_S;


typedef struct tagIveYv12ToHCtrl
{
    IVE_YU12_TO_H_MODE_E   enMode;          /*算子模式*/
} IVE_YV12_TO_H_CTRL_S;


typedef struct tagIveImgThrAdapScale
{
    IVE_IMG_THR_ADAP_SCALE_MODE_E   enMode;     /*算子模式*/
    //NI_BOOL                         bStatics; /*统计满足条件的像素个数*/
    NI_U8                           u8Scale;
    NI_U16                          u16MinTh;
    NI_U8                           u8TureVal;
    NI_U8                           u8FalseVal;     
} IVE_IMG_THR_ADAP_SCALE_CTRL_S;


typedef struct tagIveUpdateSimilarMean
{
    IVE_UPD_SIMILAR_MEAN_MODE_E   enMode;       /*算子模式*/
    NI_U16                        u16MaxSpeed;      /*最大速率*/
    NI_U16                        u16MaxVal;    /*最大值*/
    NI_U8                         u8MaxDif;      
} IVE_UPDATE_SIMILAR_MEAN_CTRL_S;


typedef struct tagIveUpdateSimilarMeanRef
{
    IVE_UPD_SIMILAR_MEAN_REG_MODE_E   enMode;       /*算子模式*/
    NI_U8                             u8MaxRef;     /*最大参考值*/
    NI_U8                             u8Shift;      /*偏移值*/
    NI_U16                            u16MaxSpeed;      /*最大速率*/
    NI_U16                            u16FixedVal;      /*固定值*/
} IVE_UPDATE_SIMILAR_MEAN_REF_CTRL_S;


typedef struct tagIveThr2ValCtrl
{
    NI_U8       u8Thr;      /*算子模式*/
    NI_U16      u16Mx256;
    NI_U16      u16Nx256;
    NI_U8       u8TureVal;
} IVE_THR2VAL_CTRL_S;


typedef struct tagIveThrRefCtrl
{
    IVE_THR_REF_MODE_E  enMode;         /*算子模式*/
    NI_U8               au8Thr[3];      /*阈值*/
    NI_U16              au16FixedVal[3];    /*固定值*/
    NI_U16              u16FalseVal;
    NI_U8               u8Shift;     /*IVE_THR_REF_MODE_2模式下有效*/
} IVE_THR_REF_CTRL_S;



typedef struct tagIveAverGrayCtrl
{
   NI_U8  u8MaskThr;            /*Mask图像中像素等于该值才参与计算*/
   NI_U8  u8ScaleW;             /*横向取点间隔[0:15]*/
   NI_U8  u8ScaleH;             /*纵向取点间隔[0:15]*/
} IVE_AVER_GRAY_CTRL_S;


typedef struct tagIveAverGrayResult
{
    NI_U32 u32Num;          /*值为MaskThr的像素个数*/
    NI_U8  au8Pave[3];      /*三帧图像的均值*/
} IVE_AVER_GRAY_RESULT_S;


typedef struct tagIveImgSetRefCtrl
{
    IVE_SET_REF_MODE_E  enMode;         /*算子模式*/
    NI_U8               u8ScaleW;
    NI_U8               u8ScaleH;
    NI_U8               u8ThreshM;
    NI_U8               u8ThreshV;
} IVE_IMG_SET_REF_CTRL_S;

typedef struct tagIveSubAbsCoefAlpha
{
    NI_U8               u8AlphaBit0;
    NI_U8               u8AlphaBit1;
} IVE_SRC_ALPHA_S;

typedef enum tagIveSubAbsCoefAlphaSelect
{
    IVE_ALPHA_SELECT_MODE_0 = 0,         /*global_alpha*/
    IVE_ALPHA_SELECT_MODE_1 = 1,         /*src0_alpha*/
    IVE_ALPHA_SELECT_MODE_2 = 2,         /*src1_alpha*/
    IVE_ALPHA_SELECT_MODE_3 = 3,         /*src0_alpha * global_alpha >>8*/
    IVE_ALPHA_SELECT_MODE_4 = 4,         /*src0_alpha * src1_alpha >>8*/
    
    IVE_ALPHA_SELECT_BUTT,
} IVE_ALPHA_SELECT_MODE_E;

typedef struct tagIveSubAbsCoefCtrl
{
    IVE_SUB_ABS_COEF_MODE_E enMode;
    NI_U8                   u8ShiftL;       /*左移值*/
    NI_U8                   u8ShiftR;       /*右移值*/
    NI_U8                   u8Alpha;        /*alpha值(透明度)*/
    NI_BOOL                 bAlphaEn;       /*像素alpha使能*/
    
    /*xs7312的模式1需要以下变量,xs7320不需要使用*/
    IVE_ALPHA_SELECT_MODE_E enMixAlMode;     /*像素混合过程中选择alpha*/
    IVE_ALPHA_SELECT_MODE_E enDstAlMode;     /*输出alpha选择*/
    IVE_SRC_ALPHA_S         stSrc0Alpha;     /*以像素形式输入的alpha0*/
    IVE_SRC_ALPHA_S         stSrc1Alpha;     /*以像素形式输入的alpha1*/
    NI_U32                  au32BitValue[16];    /*图像0为1BIT,2BIT或4BIT类型时保存相应的像素值，其中1BIT模式使用前两个值，\
                                                    2BIT模式使用前4个值, 4BIT模式使用全部的16个值*/
} IVE_SUB_ABS_COEF_CTRL_S;


typedef struct tagIveLocalSigmaCtrl
{
    IVE_LOCAL_SIGMA_MODE_E  enMode;         /*算子模式*/
    IVE_WIN_SIZE_E          enWinSize;      /*窗口尺寸*/
    NI_BOOL                 bMask;          /*是否使用掩码*/
    NI_U16                  u16BoundaryVal;     /*边界值*/
} IVE_LOCAL_SIGMA_CTRL_S;


typedef struct tagIveLbpCtrl
{
    IVE_FIELD_E             enMode;         /*算子模式*/
    NI_S16                  s16MinDiff;
    NI_U8                   au8ValSet[2];           /*设置值*/
    NI_U8                   u8BoundaryVal;    
} IVE_LBP_CTRL_S;

typedef struct tagIveAngleCtrl
{
    IVE_ANGLE_MODE_E enMode;      
} IVE_ANGLE_CTRL_S;

typedef struct tagIveAngleByDxDyCtrl
{
    IVE_ANGLE_BY_DXDY_MODE_E enMode;      
} IVE_ANGLE_BY_DXDY_CTRL_S;

typedef struct tagIvePointFltCtrl
{
    IVE_POINT_FLT_MODE_E    enMode;         /*算子模式*/
    NI_U8                   u8Thr;          /*阈值*/
    IVE_BOUNDARY_S          stBoundary;
    NI_U32                  u32PointFilterTable[8];     /*点滤波参数*/
} IVE_POINT_FLT_CTRL_S;

typedef enum tagIveErrCode
{    
    ERR_IVE_NONODE = 100,
  
    ERR_IVE_BUTT

} IVE_ERR_CODE_E;


/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define NI_ERR_IVE_ILLEGAL_PARAM     NI_DEF_ERR(NI_ID_IVE, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)
/* using a NI_NULL point */
#define NI_ERR_IVE_NULL_PTR          NI_DEF_ERR(NI_ID_IVE, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)
/* try to enable or initialize system,device or channel, before configing attribute */
#define NI_ERR_IVE_NOT_CONFIG        NI_DEF_ERR(NI_ID_IVE, EN_ERR_LEVEL_ERROR, ERR_NOT_CONFIG)
/* operation is not supported by NOW */
#define NI_ERR_IVE_NOT_SUPPORT       NI_DEF_ERR(NI_ID_IVE, EN_ERR_LEVEL_ERROR, ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change static attribute */
#define NI_ERR_IVE_NOT_PERM          NI_DEF_ERR(NI_ID_IVE, EN_ERR_LEVEL_ERROR, ERR_NOT_PERM)
/* failure caused by malloc memory */
#define NI_ERR_IVE_NOMEM             NI_DEF_ERR(NI_ID_IVE, EN_ERR_LEVEL_ERROR, ERR_NOMEM)
/* failure caused by malloc buffer */
#define NI_ERR_IVE_NOBUF             NI_DEF_ERR(NI_ID_IVE, EN_ERR_LEVEL_ERROR, ERR_NOBUF)
/* no data in buffer */
#define NI_ERR_IVE_BUF_EMPTY         NI_DEF_ERR(NI_ID_IVE, EN_ERR_LEVEL_ERROR, ERR_BUF_EMPTY)
/* no buffer for new data */
#define NI_ERR_IVE_BUF_FULL          NI_DEF_ERR(NI_ID_IVE, EN_ERR_LEVEL_ERROR, ERR_BUF_FULL)
/* not initialize */
#define NI_ERR_IVE_NOT_INIT          NI_DEF_ERR(NI_ID_IVE, EN_ERR_LEVEL_ERROR, ERR_SYS_NOTREADY)

#define NI_ERR_IVE_REQUEST_IRQFAIL   NI_DEF_ERR(NI_ID_IVE, EN_ERR_LEVEL_ERROR, ERR_REQ_IRQ_FAIL)

#define NI_ERR_IVE_TIMEOUT           NI_DEF_ERR(NI_ID_IVE, EN_ERR_LEVEL_ERROR, ERR_TIME_OUT)

#define NI_ERR_IVE_NOT_ENABLE        NI_DEF_ERR(NI_ID_IVE, EN_ERR_LEVEL_ERROR, ERR_NOT_ENABLE)

#define NI_ERR_IVE_NONODE            NI_DEF_ERR(NI_ID_IVE, EN_ERR_LEVEL_ERROR, ERR_IVE_NONODE)




#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of #ifndef __NI_COMM_IVE_H__ */


