/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef __MDK_AISP_H__
#define __MDK_AISP_H__

#include "ni_type.h"
#include "ni_comm_aisp.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_Init
 * 描  述:    AISP初始化
 * 输  入:   参数          描述
 *       :   IspDev       Isp设备号
 *       :   pstInitParam 初始化参数
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_Init(ISP_DEV IspDev, AISP_INIT_PARAM_S *pstInitParam);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_DeInit
 * 描  述:    AISP去初始化
 * 输  入:   参数          描述
 *       :   IspDev       Isp设备号
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_DeInit(ISP_DEV IspDev);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_Run
 * 描  述:    AISP运行,用户需要开启线程连续运行。
 * 输  入:   参数          描述
 *       :   IspDev       Isp设备号
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_Run(ISP_DEV IspDev);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_GetAeResult
 * 描  述:    获取当前曝光结果
 * 输  入:   参数          描述
 *       :   IspDev       Isp设备号
 *       :   pstAeResult  Ae曝光结果
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_GetAeResult(ISP_DEV IspDev, AISP_AE_RESULT_S *pstAeResult);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_SetVideoFmtAttr
 * 描  述:    设置视频制式
 * 输  入:   参数               描述
 *       :   IspDev             Isp设备号
 *       :   pstVideoFmtInfo    视频制式
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_SetVideoFmtAttr(ISP_DEV IspDev, AISP_VIDEO_FMT_INFO_S *pstVideoFmtInfo);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_GetVideoFmtAttr
 * 描  述:    获取当前视频制式信息
 * 输  入:   参数                描述
 *       :   IspDev             Isp设备号
 *       :   pstVideoFmtInfo    视频制式
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_GetVideoFmtAttr(ISP_DEV IspDev, AISP_VIDEO_FMT_INFO_S *pstVideoFmtInfo);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_SetGammaAttr
 * 描  述:    设置gama等级
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
 *       :   pstGama       gamma等级参数
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_SetGammaAttr(ISP_DEV IspDev, AISP_APP_GAMMA_S *pstGama);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_GetGammaAttr
 * 描  述:    获取gamma等级
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
 *       :   pstGama       gamma等级参数
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_GetGammaAttr(ISP_DEV IspDev, AISP_APP_GAMMA_S *pstGama);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_SetColor
 * 描  述:    设置视频颜色参数
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
 *       :   pstColor      颜色属性
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_SetColor(ISP_DEV IspDev, AISP_APP_COLOR_S *pstColor);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_GetColor
 * 描  述:    获取视频颜色参数
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
 *       :   pstColor      颜色属性
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_GetColor(ISP_DEV IspDev, AISP_APP_COLOR_S *pstColor);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_SetBackLight
 * 描  述:    设置背光模式
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
 *       :   pstBackLight  背光属性
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_SetBackLight(ISP_DEV IspDev, AISP_APP_BACKLIGHT_S *pstBackLight);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_GetBackLight
 * 描  述:    获取背光属性
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
 *       :   pstBackLight  背光属性
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_GetBackLight(ISP_DEV IspDev, AISP_APP_BACKLIGHT_S *pstBackLight);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_SetDayNight
 * 描  述:    设置日夜切换属性
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
 *       :   pstDayNight   日夜属性
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_SetDayNight(ISP_DEV IspDev, AISP_APP_DAY_NIGHT_S *pstDayNight);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_GetDayNight
 * 描  述:    获取当前日夜配置属性参数
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
 *       :   pstDayNight   日夜属性
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_GetDayNight(ISP_DEV IspDev, AISP_APP_DAY_NIGHT_S *pstDayNight);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_SetSmartLight
 * 描  述:    设置补光灯参数
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
 *       :   pstSmartLight 补光灯参数
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_SetSmartLight(ISP_DEV IspDev, AISP_APP_LIGHT_S *pstSmartLight);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_GetSmartLight
 * 描  述:    获取当前补光灯参数
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
 *       :   pstSmartLight 补光灯参数
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_GetSmartLight(ISP_DEV IspDev, AISP_APP_LIGHT_S *pstSmartLight);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_SetNrAttr
 * 描  述:    设置降噪属性
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
 *       :   pstNr         降噪参数
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_SetNrAttr(ISP_DEV IspDev, AISP_APP_NR_S *pstNr);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_SetNrAttr
 * 描  述:    获取降噪属性
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
 *       :   pstNr         降噪参数
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_GetNrAttr(ISP_DEV IspDev, AISP_APP_NR_S *pstNr);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_SetEeAttr
 * 描  述:    设置锐化属性
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
 *       :   pstEe         锐化参数
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_SetEeAttr(ISP_DEV IspDev, AISP_APP_EE_S *pstEe);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_GetEeAttr
 * 描  述:    获取锐化属性
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
 *       :   pstEe         锐化参数
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_GetEeAttr(ISP_DEV IspDev, AISP_APP_EE_S *pstEe);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_SetExpAttr
 * 描  述:    设置曝光参数
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
 *       :   pstExp        曝光参数
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_SetExpAttr(ISP_DEV IspDev, AISP_APP_EXP_S *pstExp);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_GetExpAttr
 * 描  述:    获取曝光参数
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
 *       :   pstExp        曝光参数
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_GetExpAttr(ISP_DEV IspDev, AISP_APP_EXP_S *pstExp);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_SetWbAttr
 * 描  述:    设置白平衡参数
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
 *       :   pstWb        白平衡参数
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_SetWbAttr(ISP_DEV IspDev, AISP_APP_WB_S *pstWb);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_GetWbAttr
 * 描  述:    获取白平衡参数
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
 *       :   pstWb        白平衡参数
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_GetWbAttr(ISP_DEV IspDev, AISP_APP_WB_S *pstWb);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_SetStyle
 * 描  述:    设置图像风格
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
 *       :   penStyle      图像风格
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_SetStyle(ISP_DEV IspDev, AISP_IMG_STYLE_E *penStyle);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_GetStyle
 * 描  述:    获取图像风格
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
 *       :   penStyle      图像风格
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_GetStyle(ISP_DEV IspDev, AISP_IMG_STYLE_E *penStyle);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_SetAutoFps
 * 描  述:    设置自动降帧控制参数, 仅在AE曝光模式为Auto时有效
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
 *       :   pstAFpsCtrl   帧率控制参数
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_SetAutoFps(ISP_DEV IspDev, AISP_AFPS_CTRL_S *pstAFpsCtrl);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_GetAutoFps
 * 描  述:    获取自动降帧控制参数, 仅在AE曝光模式为Auto时有效
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
 *       :   pstAFpsCtrl   帧率控制参数
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_GetAutoFps(ISP_DEV IspDev, AISP_AFPS_CTRL_S *pstAFpsCtrl);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_GetVersion
 * 描  述:    获取版本信息
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
 *       :   pstVersion    版本信息
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_GetVersion(ISP_DEV IspDev, AISP_VERSION_S *pstVersion);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_SetDefogAttr
 * 描  述:    设置图像去雾参数
 * 输  入:   参数          描述
 *       :   s32IspDev        Isp设备号
 *       :   pstDefog         去雾参数
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_SetDefogAttr(ISP_DEV IspDev, AISP_APP_DEFOG_S *pstDefog);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_GetDefog
 * 描  述:    获取图像去雾参数
 * 输  入:   参数          描述
 *       :   s32IspDev          Isp设备号
 *       :   pstDefog           去雾参数
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_GetDefogAttr(ISP_DEV IspDev, AISP_APP_DEFOG_S *pstDefog);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_SetModRunState
 * 描  述:    设置单模块运行使能--调试接口，用于控制模块的运行
 * 输  入:   参数          描述
 *       :   pszModName   模块名
 *       :   pbRunEn      使能开关
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_SetModRunState(ISP_DEV IspDev, AISP_MOD_RUN_U *punModeRun);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_SetModRunState
 * 描  述:    获取单模块运行使能状态--调试接口，用于获取模块使能状态
 * 输  入:   参数          描述
 *       :   pszModName   模块名
 *       :   pbRunEn      使能开关
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_GetModRunState(ISP_DEV IspDev, AISP_MOD_RUN_U *punModeRun);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_ForceSync
 * 描  述:    强制参数同步刷新
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_ForceSync(ISP_DEV IspDev);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_GetDnResult
 * 描  述:    获取当前日夜状态信息
 * 输  入:   参数          描述
 *       :   s32IspDev    Isp设备号
 *       :   pstDnRes     日夜状态
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_GetDnResult(NI_S32 IspDev, AISP_DN_RESULT_S *pstDnRes);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_SetAeAdjust
 * 描  述:    AE调整曝光开关
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
 *       :   bAeAdjust     Ae调整曝光使能开关
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_SetAeAdjust(ISP_DEV IspDev, NI_BOOL bAeAdjust);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_SetFastAeExp
 * 描  述:    快启Exp参数设置
 * 输  入:   参数           描述
 *       :   IspDev        Isp设备号
 *       :   pstFastAeExp  Exp参数
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_SetFastAeExp(ISP_DEV IspDev, AISP_APP_FAST_AE_EXP_S *pstFastAeExp);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_GetFastAeExp
 * 描  述:    快启Exp参数获取
 * 输  入:   参数          描述
 *       :   IspDev       Isp设备号
 *       :   pstFastAeExp     Exp参数
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_GetFastAeExp(ISP_DEV IspDev, AISP_APP_FAST_AE_EXP_S *pstFastAeExp);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_AwesInit
 * 描  述:    Awes初始化
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
 *       :   pstAwes       Awes初始化参数
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_AwesInit(ISP_DEV IspDev, AISP_MAWES_INIT_PARAM_S *pstAwes);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_AwesDeInit
 * 描  述:    Awes反初始化
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_AwesDeInit(ISP_DEV IspDev);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_AwesGetData
 * 描  述:    获取Awes数据
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
 *       :   pstAwes       Awes数据
 *       :   u32TimeOutMs  等待超时时间
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_AwesGetData(ISP_DEV IspDev, AISP_MAWES_STATISTICS_S *pstAwes, NI_U32 u32TimeOutMs);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_AwesReleaseData
 * 描  述:    释放Awes数据
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
 *       :   pstAwes       Awes数据
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_AwesReleaseData(ISP_DEV IspDev, AISP_MAWES_STATISTICS_S *pstAwes);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_GetAispData
 * 描  述:    主从核同步时，从核获取相关参数
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
 *       :   pu32Addr      数据地址
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_GetAispData(ISP_DEV IspDev, NI_U32 *pu32Addr);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_SetAispData
 * 描  述:    主从核同步时，主核设置相关参数
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
 *       :   pu32Addr      数据地址
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_SetAispData(ISP_DEV IspDev, NI_U32 *pu32Addr);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
