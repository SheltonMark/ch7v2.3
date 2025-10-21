/* 
 * mdk_ive.h -- ive模块对外提供的API头文件
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */

#ifndef __MDK_IVE_H__
#define __MDK_IVE_H__

/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "ni_type.h"
#include "ni_comm_ive.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */

/**
 * @description: 请求一个组用于挂载任务节点，一次性分配的ID数最多16个，每次一组ID计算完成以后ID号会回收。
 * @param {IVE_GRP_ID} *IVE_GRP_ID: 输出，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_RequestGroup(IVE_GRP_ID *pIveGrpId);

/**
 * @description: 将一组任务提交给硬件计算，可选择阻塞模式或者非阻塞模式，一组的IVE_GRP_ID是一致的，
                都会在NI_MDK_IVE_SubmitGroup之后被硬件进行计算，因为各种原因申请到但是不使用了的组
                一定要调用NI_MDK_IVE_SubmitGroup，这样才能把组号还回给IVE。
 * @param {IVE_GRP_ID} IVE_GRP_ID: 输入，系统分配的组号
 * @param {NI_BOOL} bBlock: 输入，阻塞模式选择，True:阻塞模式，程序阻塞在这里，直到本次提交的所有算子计算完毕
                            False:非阻塞模式，程序正常往下执行，配合IVE_CAL_Query算子得到算子计算完成与否。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_SubmitGroup(IVE_GRP_ID IveGrpId, NI_BOOL bBlock);

/**
 * @description: 该函数根据组号查询提交的一组任务的完成状态。
 * @param {IVE_GRP_ID} IVE_GRP_ID: 输入，系统分配的组号
 * @param {NI_BOOL} *pbFinish: 输出，NI_TRUE：任务已经完成,NI_FALSE：任务没有完成。
 * @param {NI_BOOL} bBlock: 输入，阻塞模式选择，True:阻塞模式，程序阻塞在这里，直到本次提交的所有算子计算完毕
                            False:非阻塞模式，程序正常往下执行，配合IVE_CAL_Query算子得到算子计算完成与否
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_Query(IVE_GRP_ID IveGrpId, NI_BOOL *pbFinish, NI_BOOL bBlock); 

/**
 * @description:创建数据搬移任务，可实现线性拷贝，矩形拷贝，带屏蔽功能的拷贝，几种拷贝区别参见用户手册。
                算子支持的输入输出图像最小宽度为16，且满足2对齐，最小高度为2。
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc: 输入，源数据指针。
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，输出数据指针。
 * @param {IVE_DMA_CTRL_S} *pstDmaCtrl: 输入，Dma控制参数指针.
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_Dma(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc,
    IVE_DST_IMAGE_S *pstDst, IVE_DMA_CTRL_S *pstDmaCtrl, IVE_GRP_ID IveGrpId);

/**
 * @description:统计一帧中的像素值分别落在0~255之间的个数。输入：单字节或双字节单分量像素值,输出：位宽为32bit的256级直方图统计值
                该算子支持的输入输出图像最小宽度为16，且满足2对齐，最小高度为1。
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc: 输入，源数据指针。
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，输出数据指针。
 * @param {IVE_HIST_CTRL_S} *pstHistCtrl: 输入，定义HIST算子控制信息。
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_Hist(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc,
    IVE_DST_MEM_S *pstDst, IVE_HIST_CTRL_S *pstHistCtrl, IVE_GRP_ID IveGrpId);


/**
 * @description:实现形态学图像腐蚀功能，图像腐蚀属于二值图像形态学范畴
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc: 输入，源数据指针。
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，输出数据指针。
 * @param {IVE_ERODE_NH_CTRL_S} *pstErodeCtrl: 输入，定义邻域腐蚀控制信息。
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_ErodeNh(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc,
    IVE_DST_IMAGE_S *pstDst, IVE_ERODE_NH_CTRL_S *pstErodeCtrl, IVE_GRP_ID IveGrpId);

/**
 * @description:实现形态学图像腐蚀功能，用于实现窗口腐蚀，
                模式0腐蚀计算过程：求取3x3中黑色位置点像素和值SUM，如果SUM值大于等于设置的阈值，
                将此对应位置点输出值置为P(m,n)，否则置为FALSE_VALUE。
                模式1腐蚀计算过程：求取3x3中黑色位置点像素和值SUM，如果SUM值大于等于对应位置的像素阈值，
                则将此对应位置点输出值置为P(m,n)，否则置为FALSE_VALUE
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc: 输入，源数据指针。
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，输出数据指针。
 * @param {IVE_ERODE_WIN_CTRL_S} *pstErodeCtrl: 输入，定义窗口腐蚀控制信息。
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_ErodeWin(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc,
    IVE_DST_IMAGE_S *pstDst, IVE_ERODE_WIN_CTRL_S *pstErodeCtrl, IVE_GRP_ID IveGrpId);

/**
 * @description:水平腐蚀，该算子支持的输入输出图像最小宽度为16，且满足2对齐，最小高度为7，
               该算子应该满足图像宽度u16Width + u16ErodeNum / 2 <= 2047。流程如下：
               先确定连续非零像素的起始点和结束点，如果非零像素个数小于ERODE_NUM个，
               则将此些像素直接腐蚀（置零），否则将此些像素最左边（ERODE_NUM+1）/2个
               和最右边ERODE_NUM/2个直接腐蚀（置零），之后继续在当前像素行中寻找，直至当前行像素完成。
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc: 输入，源数据指针。
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，输出数据指针。
 * @param {IVE_ERODE_HOR_CTRL_S} *pstErodeCtrl: 输入，定义水平腐蚀控制信息。
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_ErodeHor(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc,
    IVE_DST_IMAGE_S *pstDst, IVE_ERODE_HOR_CTRL_S *pstErodeCtrl, IVE_GRP_ID IveGrpId);

/**
 * @description:水平腐蚀，该算子支持的输入输出图像最小宽度为16，且满足2对齐，最小高度为7，
               该算子应该满足图像宽度u16Width + u16ErodeNum / 2 <= 2047。流程如下：
               先确定连续非零像素的起始点和结束点，如果非零像素个数小于ERODE_NUM个，
               则将此些像素直接腐蚀（置零），否则将此些像素最左边（ERODE_NUM+1）/2个
               和最右边ERODE_NUM/2个直接腐蚀（置零），之后继续在当前像素行中寻找，直至当前行像素完成。(详情参考用户手册)
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc: 输入，源数据指针。
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，输出数据指针。
 * @param {IVE_ERODE_HOR_CTRL_S} *pstErodeCtrl: 输入，定义水平腐蚀控制信息。
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_DilateNh(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc,
    IVE_DST_IMAGE_S *pstDst, IVE_DILATE_NH_CTRL_S *pstDilateCtrl, IVE_GRP_ID IveGrpId);

/**
 * @description:用于实现邻域膨胀。该算子支持的输入输出图像最小宽度为24，且满足2对齐，最小高度为7
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc: 输入，源数据指针。
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，输出数据指针。
 * @param {IVE_DILATE_WIN_CTRL_S} *pstDilateCtrl: 输入，定义邻域膨胀控制信息。
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_DilateWin(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc,
    IVE_DST_IMAGE_S *pstDst, IVE_DILATE_WIN_CTRL_S *pstDilateCtrl, IVE_GRP_ID IveGrpId);

/**
 * @description:用于实现水平膨胀。该算子支持的输入输出图像最小宽度为24，且满足2对齐，最小高度为7
                该算子应该满足图像宽度u16Width + u16DilateNum / 2 <= 2047。(详情参考用户手册)
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc: 输入，源数据指针。
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，输出数据指针。
 * @param {IVE_DILATE_HOR_CTRL_S} *pstDilateCtrl: 输入，定义水平膨胀控制信息。
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_DilateHor(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc,
    IVE_DST_IMAGE_S *pstDst, IVE_DILATE_HOR_CTRL_S *pstDilateCtrl, IVE_GRP_ID IveGrpId);

/**
 * @description:求两帧图像的差值，结果为无符号类型数据，该算子支持的输入输出图像最小宽度为16，且满足2对齐，最小高度为1
                Pout(x,y)=Pin1(x,y)>Pin2(xy)  ? (Pin1(x,y)-Pin2(xy)) ∶FALSE_VALUE.
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc0: 输入，源数据指针0。
 * @param {IVE_SRC_IMAGE_S} *pstSrc1: 输入，源数据指针1。
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，输出数据指针。
 * @param {NI_U8} u8FalseVal: 输入，错误值。
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_Sub(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc0, IVE_SRC_IMAGE_S *pstSrc1,
    IVE_DST_IMAGE_S *pstDst, NI_U8 u8FalseVal, IVE_GRP_ID IveGrpId);


/**
 * @description:实现计算图像与FixedValue的差，结果无符号饱和，支持U8C1
                Pout(x,y)=Pin1(x,y)>VAL ? (Pin1(x,y)-FixedValue) ∶FALSE_VALUE
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc: 输入，源数据指针。
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，输出数据指针。
 * @param {IVE_SUBFIXEDVAL_CTRL_S} *pstSubCtrl: 输入，固定值减法控制信息。
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_SubFixedVal(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc,
    IVE_DST_IMAGE_S *pstDst, IVE_SUBFIXEDVAL_CTRL_S *pstSubCtrl, IVE_GRP_ID IveGrpId);


/**
 * @description:该算子实现两帧图像差的绝对值（图像1为双字节，图像2为单字节），并统计大于阈值的像素数目，
                该算子支持的输入输出图像最小宽度为16，且满足2对齐，最小高度为1
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc0: 输入，源数据指针0。
 * @param {IVE_SRC_IMAGE_S} *pstSrc1: 输入，源数据指针1。
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，输出数据指针。
 * @param {NI_U8} u8Thr: 输入，设定的阈值。
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_SubU16SU8(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc0, IVE_SRC_IMAGE_S *pstSrc1, 
    IVE_DST_IMAGE_S *pstDst, NI_U8 u8Thr, IVE_GRP_ID IveGrpId);

/**
 * @description:该算子实现计算含参考图像的两帧图像差的绝对值（参考图像有效时计算差值，参考图像无效时差值为零）
                该算子支持的输入输出图像最小宽度为16，且满足2对齐，最小高度为1。
                Pout(x,y)=PMASK(x,y)  != 0  ?  ABS(Pin1(x,y)- Pin2(x,y)) ∶FALSE_VALUE
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc0: 输入，源数据指针0。
 * @param {IVE_SRC_IMAGE_S} *pstSrc1: 输入，源数据指针1。
 * @param {IVE_SRC_IMAGE_S} *pstSrcRef: 输入，参考图像源数据指针。。
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，输出数据指针。
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_SubRef(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc0, IVE_SRC_IMAGE_S *pstSrc1, 
    IVE_SRC_IMAGE_S *pstSrcRef, IVE_DST_IMAGE_S *pstDst, IVE_GRP_ID IveGrpId);

/**
 * @description:实现计算窗口差，该算子支持的输入输出图像最小宽度为16，且满足2对齐，最小高度为7。
                模式0计算水平方向窗口差的绝对值的最小值，模式1计算垂直方向窗口差的绝对值最小值，
                模式2计算3X3窗口差的绝对值最小值，先判断是否同时满足：
                ① Pref(x,y) > THRESH  
                ②ABS(Pin1(x,y)-Pin2(x,y)) >= MIN_DIFF这两个条件
                模式3、模式4与模式2类似（详情参考用户手册）
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc0: 输入，源数据指针0。
 * @param {IVE_SRC_IMAGE_S} *pstSrc1: 输入，源数据指针1。
 * @param {IVE_SRC_IMAGE_S} *pstSrcRef: 输入，参考图像源数据指针。
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，输出数据指针。
 * @param {IVE_SUB_WIN_REF_CTRL_S} *pstWinRefCtrl: 输入，计算窗口查控制信息。
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_SubWinRef(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc0, IVE_SRC_IMAGE_S *pstSrc1, 
    IVE_SRC_IMAGE_S *pstSrcRef, IVE_DST_IMAGE_S *pstDst, IVE_SUB_WIN_REF_CTRL_S *pstWinRefCtrl, IVE_GRP_ID IveGrpId);

/**
 * @description:实现图像像素和计算功能,该算子支持的输入输出图像最小宽度为16，最小高度为1,
                四字节模式的数据源需保证每个像素值的第31位和第30位是相同的，都表示符号位
                （即有效位数只有31位）（详情参考用户手册）
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc0: 输入，源数据指针0。
 * @param {IVE_SRC_IMAGE_S} *pstSrc1: 输入，源数据指针1。
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，输出数据指针。
 * @param {IVE_SUM_CTRL_S} *pstSumCtrl: 输入，像素和计算控制信息。
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_Sum(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc0, IVE_SRC_IMAGE_S *pstSrc1, 
    IVE_SRC_IMAGE_S *pstDst, IVE_SUM_CTRL_S *pstSumCtrl, IVE_GRP_ID IveGrpId);

/**
 * @description:实现两幅图像对应位置像素数据相乘操作，输入图像为单字节数据，输出为双字节数据
                支持的输入输出图像最小宽度为16，且满足2对齐，最小高度为1
                Pout0(i) = Pin0(i)×Pin1(i)（详情参考用户手册）
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc0: 输入，源数据指针0。
 * @param {IVE_SRC_IMAGE_S} *pstSrc1: 输入，源数据指针1。
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，输出数据指针。
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_Mul(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc0, IVE_SRC_IMAGE_S *pstSrc1, 
    IVE_DST_IMAGE_S *pstDst, IVE_GRP_ID IveGrpId);

/**
 * @description:实现图像乘以用户配置的固定值FixedValue（单字节）
                该算子支持的输入输出图像最小宽度为16，且满足2对齐，最小高度为1
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc0: 输入，源数据指针0。
 * @param {NI_U8} u8FixedVal: 输入，固定值数值。
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_MulFixedVal(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, NI_U8 u8FixedVal, 
    IVE_DST_IMAGE_S *pstDst, IVE_GRP_ID IveGrpId);

#if 0
/*支持U8C1,双源图像比较并输出统计值*/
NI_S32 NI_MDK_IVE_IMG_Compare(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc0, IVE_SRC_IMAGE_S *pstSrc1, 
    IVE_IMG_COMPARE_STATICS_S *pstResult, IVE_IMG_COMPARE_CTRL_S *pstCmpCtrl, IVE_GRP_ID IveGrpId);

/*支持U8C1,图像每个像素和固定值比较并输出统计值*/
NI_S32 NI_MDK_IVE_IMG_CompareVal(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, NI_U16 u16FixedVal, 
    IVE_IMG_COMPARE_STATICS_S *pstResult, IVE_GRP_ID IveGrpId);
#endif

/**
 * @description:现3x3水平和垂直方向SOBEL梯度计算功能，支持的输入输出图像最小宽度为20，且满足2对齐，最小高度为4
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc: 输入，源数据指针0。
 * @param {IVE_DST_IMAGE_S} *pstDstH: 输出，水平梯度值。
 * @param {IVE_DST_IMAGE_S} *pstDstV: 输出，垂直梯度值。
 * @param {IVE_DST_IMAGE_S} *pstDstAbsMax: 输出，垂直梯度值的两者绝对值的最大值
 * @param {IVE_SOBEL_CTRL_S} *pstSobelCtrl: 输入，Sobel算子控制信息
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_Sobel(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_DST_IMAGE_S *pstDstH, IVE_DST_IMAGE_S *pstDstV, 
    IVE_DST_IMAGE_S * pstDstAbsMax, IVE_SOBEL_CTRL_S *pstSobelCtrl, IVE_GRP_ID IveGrpId);

/**
 * @description:SOBEL算子结果的其他处理方式(后处理)比如：
                ①求取PH/PV绝度值得大值,②求取PH/PV绝对值的和值，并将结果限制在单字节（配置右移RB_SHIFT）
                算子支持的输入输出图像最小宽度为16，且满足2对齐，最小高度为1。
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstPh: 输入，水平梯度值。
 * @param {IVE_SRC_IMAGE_S} *pstPv: 输出，垂直梯度值。
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，垂直梯度值。
 * @param {IVE_SOBEL_DEAL_CTRL_S} *pstSobelDealCtrl: 输入，sobel算子结果的后处理控制信息
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_SobelDeal(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstPh, IVE_SRC_IMAGE_S *pstPv,
    IVE_DST_IMAGE_S *pstDst, IVE_SOBEL_DEAL_CTRL_S *pstSobelDealCtrl, IVE_GRP_ID IveGrpId);

/**
 * @description:实现高斯滤波，算子支持的输入输出图像最小宽度为20，且满足2对齐，最小高度为7
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc: 输入，源数据。
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，目的数据。
 * @param {IVE_FILTER_CTRL_S} *pstDst: 输入，高斯滤波控制信息指针
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_Filter(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc,
    IVE_DST_IMAGE_S *pstDst, IVE_FILTER_CTRL_S *pstFilterCtrl, IVE_GRP_ID IveGrpId);

/**
 * @description:实现图像上采样,支持横向和纵向的上采样,支持的输入输出图像最小宽度为16，且满足2对齐，最小高度为1.
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc: 输入，源数据。
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，目的数据。
 * @param {IVE_UP_SAMPLE_CTRL_S} *pstUpSampleCtrl: 输入，上采样控制信息指针
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_UpSample(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc,
    IVE_DST_IMAGE_S *pstDst, IVE_UP_SAMPLE_CTRL_S *pstUpSampleCtrl, IVE_GRP_ID IveGrpId);


/**
 * @description:实现图像下采样,支持的输入输出图像最小宽度为16，且满足2对齐，最小高度为1，
                算子的采样步长（即u8ScaleW和u8ScaleH）需分别能被图像的宽度和高度整除，u8ScaleW和u8ScaleH的取值大于0。
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc: 输入，源数据。
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，目的数据。
 * @param {IVE_DOWN_SAMPLE_CTRL_S} *pstDownSampleCtrl: 输入，下采样控制信息指针
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_DownSample(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc,
    IVE_DST_IMAGE_S *pstDst, IVE_DOWN_SAMPLE_CTRL_S *pstDownSampleCtrl, IVE_GRP_ID IveGrpId);

/**
 * @description:实现图像图像阈值化,支持的输入输出图像最小宽度为16，且满足2对齐，最小高度为1。
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc: 输入，源数据。
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，目的数据。
 * @param {IVE_THRESH_CTRL_S} *pstThreshCtrl: 输入，下采样控制信息指针
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_Thresh(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc,
    IVE_DST_IMAGE_S *pstDst, IVE_THRESH_CTRL_S *pstThreshCtrl, IVE_GRP_ID IveGrpId);

/**
 * @description:实现图像数据“逻辑与”、“逻辑或”、“逻辑非”、“按位与”、按位非与”、
                “按位非或”、“按位同或”、“按位异或”、“按位非”操作。
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc0: 输入，源数据0。
 * @param {IVE_SRC_IMAGE_S} *pstSrc1: 输入，源数据1。
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，目的数据。
 * @param {IVE_ROP_CTRL_S} *pstRopCtrl: 输入，逻辑控制信息指针
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_Rop(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc0, IVE_SRC_IMAGE_S *pstSrc1,
    IVE_DST_IMAGE_S *pstDst, IVE_ROP_CTRL_S *pstRopCtrl, IVE_GRP_ID IveGrpId);

/**
 * @description:实现目的图像数据按索引清零功能。该将读取的图像1(源图像SRC0) PMASK1(i)值
                与设定的FixedValue1(或图像2 (源图像SRC1)PMASK2(i))进行各种逻辑操作
                (按位与、按位或、按位异或、按位同或、按位与非、按位或非)，如果结果非零，
                则将目标地址中相应数据置零，否则保持目标地址中的原始结果不变。
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc0: 输入，源数据0。
 * @param {IVE_SRC_IMAGE_S} *pstSrc1: 输入，源数据1。
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，目的数据。
 * @param {IVE_CLEAR_REF_CTRL_S} *pstClearRefCtrl: 输入，按索引清零功能信息指针
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_ClearRef(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc0, IVE_SRC_IMAGE_S *pstSrc1,
    IVE_DST_IMAGE_S *pstDst, IVE_CLEAR_REF_CTRL_S *pstClearRefCtrl, IVE_GRP_ID IveGrpId);

/**
 * @description:实现目的图像按照如下方式实现数据调整，
                ① 3源图像操作，源图像0与源图像1对应位置像素点相等，则源图像2对应位置减去配置值SUB_VAL，比如：
                Pout(i)=Pin0(i)==Pin1(i)?MAX(0,Pin2(i)-SUB_VAL) ：MIN(255,Pin2(i)+ADD_VAL)
                否则源图像2对应位置加上配置值ADD_VAL实现方式。
                ② 4源图像操作，如果源图像3对应点像素值等于用户设置的阈值FIX_VALUE0或FIX_VALUE1，
                则保留源图像2对应位置像素值，否则按模式0进行操作
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc0: 输入，源数据0。
 * @param {IVE_SRC_IMAGE_S} *pstSrc1: 输入，源数据1。
 * @param {IVE_SRC_IMAGE_S} *pstSrc2: 输入，源数据2。
 * @param {IVE_SRC_IMAGE_S} *pstSrc3: 输入，源数据3。
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，目的数据。
 * @param {IVE_VALUE_ADJUST_CTRL_S} *pstAdjustCtrl: 输入，数据调整能信息指针
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_ValueAdjust(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc0, IVE_SRC_IMAGE_S *pstSrc1,
    IVE_SRC_IMAGE_S *pstSrc2, IVE_SRC_IMAGE_S *pstSrc3, IVE_DST_IMAGE_S *pstDst, IVE_VALUE_ADJUST_CTRL_S *pstAdjustCtrl, IVE_GRP_ID IveGrpId);

/**
 * @description:实现求取两幅图像的归一化互相关函数（NCC）功能（算法原理参见用户手册）
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc0: 输入，源数据0。
 * @param {IVE_SRC_IMAGE_S} *pstSrc1: 输入，源数据1。
 * @param {IVE_SRC_IMAGE_S} *pstMask: 输入，源MASK图像(mask使能后传入改值)
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，目的数据。
 * @param {IVE_NCC_CTRL_S} *pstNccCtrl: 输入，归一化互相关函数控制信息指针
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_Ncc(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc0, IVE_SRC_IMAGE_S *pstSrc1, 
    IVE_SRC_IMAGE_S *pstMask, IVE_DST_IMAGE_S *pstDst, IVE_NCC_CTRL_S *pstNccCtrl, IVE_GRP_ID IveGrpId);

/**
 * @description:根据源图像1（SRC0）、源图像2（SRC1）的取值情况进行相应目标地址值的设定（算法原理参见用户手册）
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc0: 输入，源数据0。
 * @param {IVE_SRC_IMAGE_S} *pstSrc1: 输入，源数据1。
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，目的数据。
 * @param {IVE_SET_IMG_VALUE_CTRL_S} *pstSetImgValCtrl: 输入，控制信息指针，目前只支持3X3与5X5.
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_SetImgValue(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc0, IVE_SRC_IMAGE_S *pstSrc1, 
    IVE_DST_IMAGE_S *pstDst, IVE_SET_IMG_VALUE_CTRL_S *pstSetImgValCtrl, IVE_GRP_ID IveGrpId);

#if 0
/*支持U8C1 源图像的方差计算*/
NI_S32 NI_MDK_IVE_Variance(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_SRC_IMAGE_S *pstMask, IVE_VARIANCE_RESULT_S *pstVarResult, 
    IVE_VARIANCE_CTRL_S *pstVarCtrl, NI_BOOL bBlock);
#endif

/**
 * @description:计算源图像0和源图像1对应像素比值，
                Ratio(i) = Pin0(i) * 4096 / (Pin1(i)+1)
                Pin0对应SRC0，Pin1对应SRC1，Ratio(i)对应DST0的值。
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc0: 输入，源数据0。
 * @param {IVE_SRC_IMAGE_S} *pstSrc1: 输入，源数据1。
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，目的数据。
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_ImgRatio(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc0, IVE_SRC_IMAGE_S *pstSrc1, 
    IVE_DST_IMAGE_S *pstDst, IVE_GRP_ID IveGrpId);

/**
 * @description:实现计算检测区域内图像积分图功能，或者像素平方的积分功能
                该算子支持的输入输出图像最小宽度为16，且满足2对齐，最小高度为1。（算法原理参见用户手册）
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc0: 输入，源数据。
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，目的数据。
 * @param {IVE_INTEGRAL_CTRL_S} *pstIntegralCtrl: 输入，控制信息指针。
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_Integral(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_DST_IMAGE_S *pstDst, 
    IVE_INTEGRAL_CTRL_S *pstIntegralCtrl, IVE_GRP_ID IveGrpId);

/**
 * @description:按条件比较两帧图像A和B对应位置像素值，若满足条件则置目标图像数据为TRUE_VALUE，并统计满足条件的像素个数；
                如不满足条件则置目标图像数据为FALSE_VALUE，该算子支持的输入输出图像最小宽度为16，且满足2对齐，最小高度为1
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc0: 输入，源数据0。
 * @param {IVE_SRC_IMAGE_S} *pstSrc1: 输入，源数据1。
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，目的数据。
 * @param {IVE_THRESHOLD_CTRL_S} *pstThresholdCtrl: 输入，控制信息指针。
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_Threshold(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc0, IVE_SRC_IMAGE_S *pstSrc1,
    IVE_DST_IMAGE_S *pstDst, IVE_THRESHOLD_CTRL_S *pstThresholdCtrl, IVE_GRP_ID IveGrpId);

/**
 * @description:图像水平/垂直方向上的灰度投影的功能，支持的输入输出图像最小宽度为16，且满足2对齐，最小高度为16
                算子的图像宽高应该能够被u8BlockSize + 1整除，u8BlockSize取值范围为[0, 15]，
                U8BlockSize的取值：
                0: 整幅图像为一个块， 
                1: 图像划分为2X2 
                ... 
                15: 图像划分为16X16
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc: 输入，源数据。
 * @param {IVE_DST_MEM_S} *pstH: 输出，水平投影。
 * @param {IVE_DST_MEM_S} *pstV: 输出，垂直投影。
 * @param {IVE_PROJECT_HV_CTRL_S} *pstProjectHvCtrl: 输入，控制信息指针。
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_ProjectHv(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_DST_MEM_S *pstH, 
    IVE_DST_MEM_S *pstV, IVE_PROJECT_HV_CTRL_S *pstProjectHvCtrl, IVE_GRP_ID IveGrpId);

/**
 * @description:本算子实现将图像数据从UYVY、VYUY、YUYV、YVYU格式转成YUV420/422格式,
                该算子支持的输入输出图像最小宽度为16，且满足2对齐，最小高度为2，且满足2对齐
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc: 输入，源数据0。
 * @param {IVE_DST_IMAGE_S} *pstDstY: 输入，目的数据。
 * @param {IVE_DST_IMAGE_S} *pstDstU: 输出，目的数据。
 * @param {IVE_DST_IMAGE_S} *pstDstV: 输出，目的数据。
 * @param {IVE_YUV_CONVERT_CTRL_S} *pstYuvConvertCtrl: 输入，控制信息指针。
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_YuvConvert(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_DST_IMAGE_S *pstDstY, 
        IVE_DST_IMAGE_S *pstDstU, IVE_DST_IMAGE_S *pstDstV, IVE_YUV_CONVERT_CTRL_S *pstYuvConvertCtrl, IVE_GRP_ID IveGrpId);

/**
 * @description:实现将YUV420图像转换成RGB或色度H图像.算子支持的输入输出图像最小宽度为20，且满足2对齐，最小高度为2，且满足2对齐
                1.算子通过pstSrc来传入完整的YUV420图像数据，其中U和V平面的物理地址和stride值分别通过pstSrc指向的aPhyAddr和au16Stride
                数组的下标为1和2的值来传入，而宽高则会在驱动程序内部自动将Y平面图像的宽高除以二得到U和V平面图像的宽高.
                2.?	在输出RGB图像的模式中，输出的三个平面数据是通过pstDst结构体一并输出，R，G，B的数据在地址上是连续的，
                两个平面之间地址的偏移值为输出图像的Stride x Height
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc: 输入，源数据0。
 * @param {IVE_DST_IMAGE_S} *pstDstY: 输出，目的数据。
 * @param {IVE_YV12_TO_H_CTRL_S} *pstYu12ToHCtrl: 输入，控制信息指针。
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_Yv12ToH(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_DST_IMAGE_S *pstDst, IVE_YV12_TO_H_CTRL_S *pstYu12ToHCtrl, IVE_GRP_ID IveGrpId);

/**
 * @description:完成目标图像二值判断操作，并反馈置1像素个数,该算子支持的输入输出图像最小宽度为16，且满足2对齐，最小高度为1
                模式0：Pin1与Pin2相乘Pout0(i)=Pin0(i)≤MAX((Pin1(i)>>8)×Pin2(i),MIN_TH) ? FALSE_VALUE∶ TRUE_VALUE
                模式1：Pin1与常数scale相乘Pout0(i)=Pin0(i)≤MAX((Pin1(i)>>8)×scale,MIN_TH) ? FALSE_VALUE∶ TRUE_VALUE
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc0: 输入，源数据0。
 * @param {IVE_SRC_IMAGE_S} *pstSrc1: 输入，源数据1。
 * @param {IVE_SRC_IMAGE_S} *pstSrc2: 输入，源数据2。
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，目的数据。
 * @param {IVE_IMG_THR_ADAP_SCALE_CTRL_S} *pstImgThrAdapScaleCtrl: 输入，控制信息指针。
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_ImgThrAdapScale(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc0, IVE_SRC_IMAGE_S *pstSrc1,
    IVE_SRC_IMAGE_S *pstSrc2, IVE_DST_IMAGE_S *pstDst, IVE_IMG_THR_ADAP_SCALE_CTRL_S *pstImgThrAdapScaleCtrl, IVE_GRP_ID IveGrpId);

/**
 * @description:实现像素均值更新功能(算法原理参见用户手册)
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc0: 输入，源数据0。
 * @param {IVE_SRC_IMAGE_S} *pstSrc1: 输入，源数据1。
 * @param {IVE_SRC_IMAGE_S} *pstSrc2: 输入，源数据2。
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，目的数据。
 * @param {IVE_UPDATE_SIMILAR_MEAN_CTRL_S} *pstUpdSimilarMeanCtrl: 输入，控制信息指针。
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_UpdSimilarMean(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc0, IVE_SRC_IMAGE_S *pstSrc1,
    IVE_SRC_IMAGE_S *pstSrc2, IVE_DST_IMAGE_S *pstDst, IVE_UPDATE_SIMILAR_MEAN_CTRL_S *pstUpdSimilarMeanCtrl, IVE_GRP_ID IveGrpId);

/**
 * @description:实现像素均值的参考更新(算法原理参见用户手册)
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc0: 输入，源数据0。
 * @param {IVE_SRC_IMAGE_S} *pstSrc1: 输入，源数据1。
 * @param {IVE_SRC_IMAGE_S} *pstSrc2: 输入，源数据2。
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，目的数据。
 * @param {IVE_UPDATE_SIMILAR_MEAN_REF_CTRL_S} *pstUpdSimilarMeanRefCtrl: 输入，控制信息指针。
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_UpdSimilarMeanRef(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc0, IVE_SRC_IMAGE_S *pstSrc1,
    IVE_SRC_IMAGE_S *pstSrc2, IVE_DST_IMAGE_S *pstDst, IVE_UPDATE_SIMILAR_MEAN_REF_CTRL_S *pstUpdSimilarMeanRefCtrl, IVE_GRP_ID IveGrpId);

/**
 * @description:本算子根据阈值大小进行设定值的选择操作功能，根据阈值选择设定值，其中设定值二根据加权计算获取
                该算子支持的输入输出图像最小宽度为16，且满足2对齐，最小高度为1.
                V_TM =(M_X_256 ×Pin0(i)+N_X_256)>>8
                PDST(i) = (Pin0(i) ≤ TH) ? TRUE_VALUE∶ V_TM
                Pin0对应输入源SRC0
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc: 输入，源数据0。
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，目的数据。
 * @param {IVE_THR2VAL_CTRL_S} *pstThr2ValCtrl: 输入，控制信息指针。
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_Thr2Val(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_DST_IMAGE_S *pstDst, 
    IVE_THR2VAL_CTRL_S *pstThr2ValCtrl, IVE_GRP_ID IveGrpId);

/**
 * @description:根据参考像素数据与阈值大小进行设定值的选择操作,该算子支持的输入输出图像最小宽度为16，且满足2对齐，最小高度为1
                比如模式0：Pout0(i)= (Pin0(i)≤ TH0 ||Pin1(i)≥ TH1)? FALSE_VALUE: FixedValue
                模式1：Pout0(i) = (Pin1(i)≤TH0|| Pin0(i)≥TH2) ? FALSE_VALUE∶ (Pin1(i)≥TH1?FixedValue0∶ Pin0(i))
                模式2：Pout0(i)=Pin0(i)≥TH0 ?FixedValue0∶ Pin1(i)≥TH1? FixedValue1∶ Pin2(i)≥TH2? FixedValue2∶MAX(Pin1(i)<<LB,Pin0(i))
                模式3：Pout0(i)=Pin0(i)≥TH0? FixedValue0∶ Pin1(i)≥TH1 ? FixedValue1∶ Pin2(i)≥TH2 ? FixedValue2∶MAX(Pin1(i),Pin0(i))
                模式4：Pout0(i)=((Pin1(i)==0)  ||(Pin0(i)==0xff))?0∶ (Pin2(i)!=0)? 0x100∶(TH0==1)? Pin0(i) ∶MAX(Pin1(i)<<LB,Pin0(i))
                模式5：Pout0(i)=((Pin1(i)==0)  ||(Pin0(i)==0xff))?0∶ (Pin1(i)>MAX{20,TH1*5})? 0x20∶(Pin2(i)!=0)? 0x100∶(TH0==1)? Pin0(i) ∶MAX(Pin1(i)<<LB,Pin0(i))
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc0: 输入，源数据0。
 * @param {IVE_SRC_IMAGE_S} *pstSrc1: 输入，源数据0。
 * @param {IVE_SRC_IMAGE_S} *pstSrc2: 输入，源数据0。
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，目的数据。
 * @param {IVE_THR_REF_CTRL_S} *pstThrRefCtrl: 输入，控制信息指针。
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
/*支持U8C1 根据参考像素数据与阈值大小进行设定值的选择*/
NI_S32 NI_MDK_IVE_ThrRef(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc0, IVE_SRC_IMAGE_S *pstSrc1, IVE_SRC_IMAGE_S *pstSrc2, 
    IVE_DST_IMAGE_S *pstDst, IVE_THR_REF_CTRL_S *pstThrRefCtrl, IVE_GRP_ID IveGrpId);

#if 0
/*计算三帧图像均值*/
NI_S32 NI_MDK_IVE_AverGray(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstMask, IVE_SRC_IMAGE_S *pstSrc1, IVE_SRC_IMAGE_S *pstSrc2, IVE_SRC_IMAGE_S *pstSrc3,
    IVE_AVER_GRAY_RESULT_S *pstAverGrayResult, IVE_AVER_GRAY_CTRL_S *pstAverGrayCtrl, NI_BOOL bBlock);
#endif

/**
 * @description:根据索引表值完成目标图像的像素填充功能，其中一个索引填充4个目标像素,该算子支持的输入输出图像最小宽度为16，最小高度为8(算法原理参见用户手册)
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc: 输入，源数据0。
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，目的数据。
 * @param {IVE_IMG_SET_REF_CTRL_S} *pstSetRefCtrl: 输入，控制信息指针。
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_SetRef(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_DST_IMAGE_S *pstDst, 
    IVE_IMG_SET_REF_CTRL_S *pstSetRefCtrl, IVE_GRP_ID IveGrpId);

/**
 * @description:算子实现了①像素加权差的绝对值操作;②像素加权差的求和操作(算法原理参见用户手册)
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc0: 输入，源数据0。
 * @param {IVE_SRC_IMAGE_S} *pstSrc1: 输入，源数据1。
 * @param {IVE_SRC_IMAGE_S} *pstSrc2: 输入，源数据2。
 * @param {IVE_SRC_IMAGE_S} *pstSrc0: 输入，pstSrc3。
 * @param {IVE_DST_IMAGE_S} *pstDst0: 输出，目的数据0。
 * @param {IVE_DST_IMAGE_S} *pstDst1: 输出，目的数据1。
 * @param {IVE_DST_IMAGE_S} *pstDst2: 输出，目的数据1。
 * @param {IVE_SUB_ABS_COEF_CTRL_S} *pstSubAbsCoefCtrl: 输入，控制信息指针。
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_SubAbsCoef(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc0, IVE_SRC_IMAGE_S *pstSrc1, IVE_SRC_IMAGE_S *pstSrc2, IVE_SRC_IMAGE_S *pstSrc3,
    IVE_DST_IMAGE_S *pstDst0, IVE_DST_IMAGE_S *pstDst1, IVE_DST_IMAGE_S *pstDst2, IVE_SUB_ABS_COEF_CTRL_S *pstSubAbsCoefCtrl, IVE_GRP_ID IveGrpId);

/**
 * @description:算子实现LOCAL_SIGMA计算功能(算法原理参见用户手册)
                该算子支持的输入输出图像最小宽度为34，且满足2对齐，最小高度为7
                该算子在输入源为双字节模式下的最大宽度为960
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc: 输入，源数据。
 * @param {IVE_SRC_IMAGE_S} *pstMask: 输入，输入Mask指针。
 * @param {IVE_DST_IMAGE_S} *pstDstSigma: 输出，输出Sigma指针。
 * @param {IVE_DST_IMAGE_S} *pstDstAver: 输出，输出Aver指针。
 * @param {IVE_LOCAL_SIGMA_CTRL_S} *pstLocalSigmaCtrl: 输入，控制信息指针。
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_LocalSigma(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_SRC_IMAGE_S *pstMask,
    IVE_DST_IMAGE_S *pstDstSigma, IVE_DST_IMAGE_S *pstDstAver, IVE_LOCAL_SIGMA_CTRL_S *pstLocalSigmaCtrl, IVE_GRP_ID IveGrpId);
    

/**
 * @description:算子产生LBP图功能。算子支持的输入输出图像最小宽度为16，且满足2对齐，最小高度为4(算法原理参见用户手册)
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc: 输入，源数据。 
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，输出目的指针。
 * @param {IVE_LBP_CTRL_S} *pstLbpCtrl: 输入，控制信息指针。 
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_Lbp(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc, IVE_DST_IMAGE_S *pstDst,
    IVE_LBP_CTRL_S *pstLbpCtrl, IVE_GRP_ID IveGrpId);

#if 0
NI_S32 NI_MDK_IVE_Angle(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstDx, IVE_SRC_IMAGE_S *pstDy, IVE_DST_IMAGE_S *pstDst,
    IVE_ANGLE_CTRL_S *pstCtrl, IVE_GRP_ID IveGrpId);
#endif

/**
 * @description:根据像素值代表的水平、垂直方向的图像点数值差值dx,dy[-255,255],利用dx的反正切求角度。
                代表的水平、垂直方向的图像点数值差值dx,dy[-255,255],利用dx的开平方求角度。
                该算子支持的输入输出图像最小宽度为16，且满足2对齐，最小高度为1(算法原理参见用户手册)
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstDx: 输入，源数据。
 * @param {IVE_SRC_IMAGE_S} *pstDy: 输入，源数据。 
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，输出目的数据指针。
 * @param {IVE_ANGLE_BY_DXDY_CTRL_S} *pstAngleByDxDyCtrl: 输入，控制信息指针。 
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_AngleByDxDy(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstDx, IVE_SRC_IMAGE_S *pstDy, IVE_DST_IMAGE_S *pstDst,
    IVE_ANGLE_BY_DXDY_CTRL_S *pstAngleByDxDyCtrl, IVE_GRP_ID IveGrpId);

/**
 * @description:实现基于查表方式的点滤波功能(算法原理参见用户手册)
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc0: 输入，源数据0。
 * @param {IVE_SRC_IMAGE_S} *pstSrc1: 输入，源数据1。 
 * @param {IVE_SRC_IMAGE_S} *pstSrc2: 输入，源数据2。 
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，输出目的指针。
 * @param {IVE_ANGLE_BY_DXDY_CTRL_S} *pstAngleByDxDyCtrl: 输入，控制信息指针。
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_PointFlt(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc0, IVE_SRC_IMAGE_S *pstSrc1, IVE_SRC_IMAGE_S *pstSrc2, IVE_DST_IMAGE_S *pstDst,
    IVE_POINT_FLT_CTRL_S *pstPointFltCtrl, IVE_GRP_ID IveGrpId);

/**
 * @description:根据索引位置拷贝源图像的像素点到目标图像(算法原理参见用户手册)
 * @param {IVE_HANDLE} *pIveHandle: 输入，任务的句柄
 * @param {IVE_SRC_IMAGE_S} *pstSrc0: 输入，源数据0。
 * @param {IVE_SRC_IMAGE_S} *pstSrc1: 输入，源数据1。 
 * @param {IVE_SRC_IMAGE_S} *pstSrc2: 输入，源数据2。 
 * @param {IVE_DST_IMAGE_S} *pstDst: 输出，输出目的指针。
 * @param {IVE_GRP_ID} IveGrpId: 输入，系统分配的组号
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_IVE_ImgCopy(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc0, IVE_SRC_IMAGE_S *pstSrc1, IVE_SRC_IMAGE_S *pstSrc2, IVE_DST_IMAGE_S *pstDst, IVE_GRP_ID IveGrpId);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of #ifndef __MDK_IVE_H__ */


