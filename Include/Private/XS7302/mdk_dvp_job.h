/* 
 * mdk_dvp_jop.h -- dvp_jop模块对外提供的API头文件
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */

#ifndef _MDK_DVP_JOB_H_
#define _MDK_DVP_JOB_H_


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */


#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */


/*******************************************************************************
 * 函数名  : NI_MDK_DVP_BeginJob
 * 描  述  :  开始一个DVP任务，1个任务可以包含1个或多个DVP操作，成功开始任务后通过NI_MDK_DVP_EndJob结束并提交该任务。同一个任务中的操作是顺序执行的。
 * 输  入  :  参数        描述
 *         :   NI_VOID         请输入NI_VOID的描述
 * 输  出  : 无
 * 返回值  : 非0: 成功
 *           NI_ERR_DVP_INVALID_HANDLE: 失败
 *******************************************************************************/
DVP_HANDLE NI_MDK_DVP_BeginJob(NI_VOID);

/*******************************************************************************
 * 函数名  : NI_MDK_DVP_EndJob
 * 描  述  : 提交已创建的DVP任务
 * 输  入  :  参数        描述
 *         :   DvpHandle              任务句柄，任务提交后该任务句柄不允许再次提交。
 *         :   u32TimeOutMSec         0：非阻塞模式，本函数调用会立刻返回，而不关心任务中的命令是否已经完成。
                                      非0：阻塞模式，等待任务完成的超时时间，单位毫秒。当超时返回或中断返回时，已提交的操作仍会在未来的某个时刻完成。
 * 输  出  : 无
 * 返回值  : 0: 成功
 *           非0: 失败
 *******************************************************************************/
NI_S32 NI_MDK_DVP_EndJob(DVP_HANDLE DvpHandle, NI_U32 u32TimeOutMSec);


/*******************************************************************************
 * 函数名  : NI_MDK_DVP_WaitJobDone
 * 描  述  : 非阻塞模式下调用本接口等待任务完成
 * 输  入  :  参数        描述
 *         :   DvpHandle              任务句柄，不允许等待一个未提交的任务。
 * 输  出  : 无
 * 返回值  : 0: 成功
 *           非0: 失败
 *******************************************************************************/
NI_S32 NI_MDK_DVP_WaitJobDone(DVP_HANDLE DvpHandle);


/*******************************************************************************
 * 函数名  : NI_MDK_DVP_CancelJob
 * 描  述  : 取消DVP任务及已经成功加入该任务中的操作。已提交的任务无法取消。
 * 输  入  :  参数        描述
 *         :   DvpHandle         任务句柄
 * 输  出  : 无
 * 返回值  : 0: 成功
 *           非0: 失败
 *******************************************************************************/
NI_S32 NI_MDK_DVP_CancelJob(DVP_HANDLE DvpHandle);


/*******************************************************************************
 * 函数名  : NI_MDK_DVP_ScaleImage
 * 描  述  : 向任务中添加缩放操作。将基地址为pstSrc的图像以区域pstSrcRect指定的尺寸缩放至pstDstRect的尺寸，
 将结果写到以pstDst为目的地址、pstDstRect为输出区域的内存中。
 * 输  入  :  参数        描述
 *         :   DvpHandle          任务句柄
 *         :   pstSrc             源图像
 *         :   pstSrcRect         源图像操作区域
 *         :   pstDst             目标图像
 *         :   pstDstRect         目标图像操作区域
 * 输  出  : 无
 * 返回值  : 0: 成功
 *           非0: 失败
 *******************************************************************************/
NI_S32 NI_MDK_DVP_ScaleImage(DVP_HANDLE DvpHandle, DVP_IMAGE_S *pstSrc, RECT_S *pstSrcRect, DVP_IMAGE_S *pstDst, RECT_S *pstDstRect);


/*******************************************************************************
 * 函数名  : NI_MDK_DVP_FillRect
 * 描  述  : 向任务中添加填充色块操作
 * 输  入  :  参数        描述
 *         :   DvpHandle            任务句柄
 *         :   pstDst               目标图像
 *         :   pstDstRect           目标图像的操作区域
 *         :   stFillColor          填充的颜色，YUV色彩空间
 * 输  出  : 无
 * 返回值  : 0: 成功
 *           非0: 失败
 *******************************************************************************/
NI_S32 NI_MDK_DVP_FillRect(DVP_HANDLE DvpHandle, DVP_IMAGE_S *pstDst, RECT_S *pstDstRect, COLOR_YUV_S *pstFillColor);


/*******************************************************************************
 * 函数名  : NI_MDK_DVP_StrokeRect
 * 描  述  : 向任务中添加绘制矩形框的操作
 * 输  入  :  参数        描述
 *         :   DvpHandle            任务句柄
 *         :   pstDst               目标图像
 *         :   pstDstRect           目标图像的操作区域
 *         :   u32FillColor         矩形框颜色
 *         :   u32LineWidth         线宽
 * 输  出  : 无
 * 返回值  : 0: 成功
 *           非0: 失败
 *******************************************************************************/
NI_S32 NI_MDK_DVP_StrokeRect(DVP_HANDLE DvpHandle, DVP_IMAGE_S *pstDst, RECT_S *pstDstRect, DVP_STROKE_STYLE_S *pstStyle);


/*******************************************************************************
 * 函数名  : NI_MDK_DVP_FillOsd
 * 描  述  : 向任务中添加叠加osd的操作
 * 输  入  :  参数        描述
 *         :   DvpHandle         任务句柄
 *         :   pstDst            目标图像
 *         :   pstOsd            待叠加的osd图像
 * 输  出  : 无
 * 返回值  : 0: 成功
 *           非0: 失败
 *******************************************************************************/
NI_S32 NI_MDK_DVP_FillOsd(DVP_HANDLE DvpHandle, DVP_IMAGE_S *pstDst, DVP_OSD_S *pstOsd);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif


