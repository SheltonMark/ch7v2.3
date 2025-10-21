/* 
 * mdk_vo.h -- vo模块对外提供的API头文件
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */

#ifndef _MDK_VO_H_
#define _MDK_VO_H_


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "ni_type.h"
#include "ni_comm_vo.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

/**
 * @description:  设置 VO 设备属性。
 * @param {VO_DEV} VoDev：输入，设备号，取值范围[0,VO_MAX_DEV_NUM）。
 * @param {VO_DEV_ATTR_S} *pstDevAttr：输入，设备属性信息。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VO_SetDevAttr(VO_DEV VoDev, const VO_DEV_ATTR_S *pstDevAttr);

/**
 * @description:  获取VO设备属性。
 * @param {VO_DEV} VoDev：输入，设备号，取值范围[0,VO_MAX_DEV_NUM）。
 * @param {VO_DEV_ATTR_S} *pstDevAttr：输出，设备属性信息。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VO_GetDevAttr(VO_DEV VoDev, VO_DEV_ATTR_S *pstDevAttr);

/**
 * @description:  使能VO设备。
 * @param {VO_DEV} VoDev：输入，设备号，取值范围[0,VO_MAX_DEV_NUM）。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VO_Enable(VO_DEV VoDev);

/**
 * @description:  去使能VO设备。
 * @param {VO_DEV} VoDev：输入，设备号，取值范围[0,VO_MAX_DEV_NUM）。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VO_Disable(VO_DEV VoDev);

/**
 * @description:  VO 绑定视频层与设备。
 * @param {VO_LAYER} VoLayer：输入，视频层，取值范围[0,VO_MAX_LAYER_NUM）。
 * @param {VO_DEV} VoDev：输入，设备号，取值范围[0,VO_MAX_DEV_NUM）。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VO_BindVideoLayer(VO_LAYER VoLayer, VO_DEV VoDev);

/**
 * @description:  VO 解绑定视频层与设备。
 * @param {VO_LAYER} VoLayer：输入，视频层，取值范围[0,VO_MAX_LAYER_NUM）。
 * @param {VO_DEV} VoDev：输入，设备号，取值范围[0,VO_MAX_DEV_NUM）。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VO_UnBindVideoLayer(VO_LAYER VoLayer, VO_DEV VoDev);

/**
 * @description:  设置 VO 设备 BT1120 高级接口。
 * @param {VO_DEV} VoDev：输入，设备号，取值范围[0,VO_MAX_DEV_NUM）。
 * @param {VO_BT1120_INTF_PARAM_S} *pstBt1120Param：输入，bt1120接口参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VO_SetBt1120Param(VO_DEV VoDev, const VO_BT1120_INTF_PARAM_S *pstBt1120Param);

/**
 * @description:  获取 VO 设备 BT1120 接口参数高级接口。
 * @param {VO_DEV} VoDev：输入，设备号，取值范围[0,VO_MAX_DEV_NUM）。
 * @param {VO_BT1120_INTF_PARAM_S} *pstBt1120Param：输出，bt1120接口参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VO_GetBt1120Param(VO_DEV VoDev, VO_BT1120_INTF_PARAM_S *pstBt1120Param);

/**
 * @description:  设置 VO 设备 LCD 接口属性。
 * @param {VO_DEV} VoDev：输入，设备号，取值范围[0,VO_MAX_DEV_NUM）。
 * @param {VO_LCD_INTF_PARAM_S} *pstLcdParam：输入，LCD属性。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VO_SetLcdParam(VO_DEV VoDev, const VO_LCD_INTF_PARAM_S *pstLcdParam);

/**
 * @description:  获取 VO 设备 LCD 接口属性。
 * @param {VO_DEV} VoDev：输入，设备号，取值范围[0,VO_MAX_DEV_NUM）。
 * @param {VO_LCD_INTF_PARAM_S} *pstLcdParam：输出，LCD属性。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VO_GetLcdParam(VO_DEV VoDev, VO_LCD_INTF_PARAM_S *pstLcdParam);

/**
 * @description:  设置 VO 设备 MIPI 接口。
 * @param {VO_DEV} VoDev：输入，设备号，取值范围[0,VO_MAX_DEV_NUM）。
 * @param {VO_MIPI_INTF_PARAM_S} *pstMipiParam：输入，MIPI接口属性。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VO_SetMipiParam(VO_DEV VoDev, const VO_MIPI_INTF_PARAM_S *pstMipiParam);

/**
 * @description:  获取 VO 设备 MIPI 接口。
 * @param {VO_DEV} VoDev：输入，设备号，取值范围[0,VO_MAX_DEV_NUM）。
 * @param {VO_MIPI_INTF_PARAM_S} *pstMipiParam：输出，MIPI接口属性。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VO_GetMipiParam(VO_DEV VoDev, VO_MIPI_INTF_PARAM_S *pstMipiParam);

/**
 * @description:  设置 VO 设备 MIPI TX 命令接口，主要用于配置 MIPI 屏的初始化参数。
 * @param {VO_DEV} VoDev：输入，设备号，取值范围[0,VO_MAX_DEV_NUM）。
 * @param {VO_MIPI_TX_CMD_INFO_S} *pstMipiCmdInfo：输入，Tx 命令。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VO_SetMipiTxCmd(const VO_MIPI_TX_CMD_INFO_S *pstMipiCmdInfo);

/**
 * @description:  设置 VO 设备 CVBS 高级接口。
 * @param {VO_DEV} VoDev：输入，设备号，取值范围[0,VO_MAX_DEV_NUM）。
 * @param {VO_CVBS_FORMAT_E} enCvbsFormat：输入，CVBS接口制式。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VO_ConfigCvbsFormat(VO_DEV VoDev, VO_CVBS_FORMAT_E enCvbsFormat);

/**
 * @description:  设置视频层参数。
 * @param {VO_LAYER} VoLayer：输入，设备号，取值范围[0,VO_MAX_LAYER_NUM）。
 * @param {VO_VIDEO_LAYER_PARAM_S} *pstVideoLayerParam：输入，视频层参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VO_SetVideoLayerParam(VO_LAYER VoLayer, const VO_VIDEO_LAYER_PARAM_S *pstVideoLayerParam);

/**
 * @description:  获取视频层参数。
 * @param {VO_LAYER} VoLayer：输入，设备号，取值范围[0,VO_MAX_LAYER_NUM）。
 * @param {VO_VIDEO_LAYER_PARAM_S} *pstVideoLayerParam：输出，视频层参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VO_GetVideoLayerParam(VO_LAYER VoLayer, VO_VIDEO_LAYER_PARAM_S *pstVideoLayerParam);

/**
 * @description:  使能视频层
 * @param {VO_LAYER} VoLayer：输入，设备号，取值范围[0,VO_MAX_LAYER_NUM）。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VO_EnableVideoLayer(VO_LAYER VoLayer);

/**
 * @description:  去使能视频层
 * @param {VO_LAYER} VoLayer：输入，设备号，取值范围[0,VO_MAX_LAYER_NUM）。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VO_DisableVideoLayer(VO_LAYER VoLayer);

/**
 * @description:  设置通道属性
 * @param {VO_LAYER} VoLayer：输入，设备号，取值范围[0,VO_MAX_LAYER_NUM）。
 * @param {VO_CHN} VoChn：输入，通道号，取值范围[0,VO_MAX_CHN_NUM）。
 * @param {VO_CHN_ATTR_S} *pstChnAttr：输入，通道属性。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VO_SetChnAttr(VO_LAYER VoLayer, VO_CHN VoChn, const VO_CHN_ATTR_S *pstChnAttr);

/**
 * @description:  获取通道属性
 * @param {VO_LAYER} VoLayer：输入，设备号，取值范围[0,VO_MAX_LAYER_NUM）。
 * @param {VO_CHN} VoChn：输入，通道号，取值范围[0,VO_MAX_CHN_NUM）。
 * @param {VO_CHN_ATTR_S} *pstChnAttr：输出，通道属性。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VO_GetChnAttr(VO_LAYER VoLayer, VO_CHN VoChn, VO_CHN_ATTR_S *pstChnAttr);

/**
 * @description:  使能通道
 * @param {VO_LAYER} VoLayer：输入，设备号，取值范围[0,VO_MAX_LAYER_NUM）。
 * @param {VO_CHN} VoChn：输入，通道号，取值范围[0,VO_MAX_CHN_NUM）。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VO_EnableChn(VO_LAYER VoLayer, VO_CHN VoChn);

/**
 * @description:  去使能通道
 * @param {VO_LAYER} VoLayer：输入，设备号，取值范围[0,VO_MAX_LAYER_NUM）。
 * @param {VO_CHN} VoChn：输入，通道号，取值范围[0,VO_MAX_CHN_NUM）。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VO_DisableChn(VO_LAYER VoLayer, VO_CHN VoChn);

/**
 * @description:  设置视频层颜色填充区域透明度（高级接口）
 * @param {VO_LAYER} VoLayer：输入，设备号，取值范围[0,VO_MAX_LAYER_NUM）。
 * @param {NI_U8} u8Alpha：输入，透明度。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VO_SetPaddingAlpha(VO_LAYER VoLayer, NI_U8 u8Alpha);

/**
 * @description:  获取视频层颜色填充区域透明度（高级接口）
 * @param {VO_LAYER} VoLayer：输入，设备号，取值范围[0,VO_MAX_LAYER_NUM）。
 * @param {NI_U8} *pu8Alpha：输出，透明度。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VO_GetPaddingAlpha(VO_LAYER VoLayer, NI_U8 *pu8Alpha);

/**
 * @description:  设置 RGB 与 YUV 转换参数（高级接口）
 * @param {VO_LAYER} VoLayer：输入，设备号，取值范围[0,VO_MAX_LAYER_NUM）。
 * @param {VO_CSC_PARAM_S} *pstCscParam：输入，转换参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VO_SetCscParam(VO_LAYER VoLayer, const VO_CSC_PARAM_S *pstCscParam);

/**
 * @description:  获取 RGB 与 YUV 转换参数（高级接口）
 * @param {VO_LAYER} VoLayer：输入，设备号，取值范围[0,VO_MAX_LAYER_NUM）。
 * @param {VO_CSC_PARAM_S} *pstCscParam：输出，转换参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VO_GetCscParam(VO_LAYER VoLayer, VO_CSC_PARAM_S *pstCscParam);

/**
 * @description:  选择显示的视频源（高级接口）
 * @param {VO_LAYER} VoLayer：输入，设备号，取值范围[0,VO_MAX_LAYER_NUM）。
 * @param {VO_CHN} VoChn：输入，通道号，取值范围[0,VO_MAX_CHN_NUM）。
 * @param {VO_DISP_SRC_S} *pstDispSrc：输入，视频源参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VO_SelectDispSrc(VO_LAYER VoLayer, VO_CHN VoChn, VO_DISP_SRC_S *pstDispSrc);

/**
 * @description:  设置通道垂直翻转（高级接口）
 * @param {VO_LAYER} VoLayer：输入，设备号，取值范围[0,VO_MAX_LAYER_NUM）。
 * @param {VO_CHN} VoChn：输入，通道号，取值范围[0,VO_MAX_CHN_NUM）。
 * @param {NI_BOOL} bFlip：输入，翻转开关。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VO_SetChnFlip(VO_LAYER VoLayer, VO_CHN VoChn, NI_BOOL bFlip);

/**
 * @description:  获取通道垂直翻转信息（高级接口）
 * @param {VO_LAYER} VoLayer：输入，设备号，取值范围[0,VO_MAX_LAYER_NUM）。
 * @param {VO_CHN} VoChn：输入，通道号，取值范围[0,VO_MAX_CHN_NUM）。
 * @param {NI_BOOL} *pbFlip：输入，翻转开关。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VO_GetChnFlip(VO_LAYER VoLayer, VO_CHN VoChn, NI_BOOL *pbFlip);

/**
 * @description:  设置通道透明度（高级接口）
 * @param {VO_LAYER} VoLayer：输入，设备号，取值范围[0,VO_MAX_LAYER_NUM）。
 * @param {VO_CHN} VoChn：输入，通道号，取值范围[0,VO_MAX_CHN_NUM）。
 * @param {NI_U8} u8Alpha：输入，透明度。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VO_SetChnAlpha(VO_LAYER VoLayer, VO_CHN VoChn, NI_U8 u8Alpha);

/**
 * @description:  获取通道透明度（高级接口）
 * @param {VO_LAYER} VoLayer：输入，设备号，取值范围[0,VO_MAX_LAYER_NUM）。
 * @param {VO_CHN} VoChn：输入，通道号，取值范围[0,VO_MAX_CHN_NUM）。
 * @param {NI_U8} *pu8Alpha：输出，透明度。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VO_GetChnAlpha(VO_LAYER VoLayer, VO_CHN VoChn, NI_U8 *pu8Alpha);

/**
 * @description:  用户送帧接口（高级接口）
 * @param {VO_LAYER} VoLayer：输入，设备号，取值范围[0,VO_MAX_LAYER_NUM）。
 * @param {VO_CHN} VoChn：输入，通道号，取值范围[0,VO_MAX_CHN_NUM）。
 * @param {VIDEO_FRAME_S} *pstVideoFrame：输入，视频帧信息。
 * @param {NI_U32} u32TimeOutMs：输入，超时时间，单位ms。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_VO_SendFrame(VO_LAYER VoLayer, VO_CHN VoChn, const VIDEO_FRAME_S *pstVideoFrame, NI_U32 u32TimeOutMs);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

