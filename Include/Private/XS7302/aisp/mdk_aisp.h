/*******************************************************************************
 * Copyright (C) 2020-2025 ZheJiang Chipup Technology CO.,LTD.
 *
 * 文件名 : mdk_aisp.h
 *
 * 作  者 : Chipup Software Team
 * 版  本 : V1.0.0
 * 日  期 : 2022-02-23 Create
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
 * 日    期: 2022-02-23
 * 修订版本:
 * 作    者:
 * 修订备注:
 *******************************************************************************/
#ifndef __MDK_AISP_H__
#define __MDK_AISP_H__


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "ni_type.h"
#include "ni_define.h"
#include "ni_comm_aisp.h"


#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */


/*******************************************************************************
 * 函数名:    NI_MDK_AISP_Init
 * 描  述:    AISP初始化
 * 输  入:   参数          描述
 *       :   IspDev       Isp设备号
         :   pstInitParam 初始化参数
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
         :   pstAeResult  Ae曝光结果
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
         :   pstVideoFmtInfo    视频制式
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
         :   pstVideoFmtInfo    视频制式
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
         :   pstGama       gamma等级参数
         :   
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
         :   pstGama       gamma等级参数
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
         :   pstColor      颜色属性
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
         :   pstColor      颜色属性
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
         :   pstBackLight  背光属性
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
         :   pstBackLight  背光属性
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
         :   pstDayNight   日夜属性
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
         :   pstDayNight   日夜属性
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
         :   pstSmartLight 补光灯参数
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_SetSmartLight(ISP_DEV IspDev, AISP_APP_LIGHT_S *pstSmartLight);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_SetSmartLight
 * 描  述:    获取当前补光灯参数
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
         :   pstSmartLight 补光灯参数
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
         :   pstNr         降噪参数
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
         :   pstNr         降噪参数
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
         :   pstEe         锐化参数
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
         :   pstEe         锐化参数
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
         :   pstExp        曝光参数
         :   
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
         :   pstExp        曝光参数
         :   
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
         :   pstWb        白平衡参数
         :   
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
         :   pstWb        白平衡参数
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
         :   penStyle      图像风格
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
         :   penStyle      图像风格
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_GetStyle(ISP_DEV IspDev, AISP_IMG_STYLE_E *penStyle);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_GetCfgInfo
 * 描  述:    获取配置文件首地址
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
         :   pstCfgInfo   配置文件首地址
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_GetCfgInfo(ISP_DEV IspDev, AISP_APP_CFG_S *pstCfgInfo);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_SetCfgInfo
 * 描  述:    该功能不支持
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
         :   pstCfgInfo
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_SetCfgInfo(ISP_DEV IspDev, AISP_APP_CFG_S *pstCfgInfo);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_GetVersion
 * 描  述:    获取版本信息
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
         :   pstVersion    版本信息
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_GetVersion(ISP_DEV IspDev, AISP_VERSION_S *pstVersion);

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
 * 函数名:    NI_MDK_AISP_CopyCfgToMem
 * 描  述:    将当前正在使用的配置数据拷贝到内存，返回当前正在使用的内存地址。
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
         :   bEnable       拷贝使能
         :   pu32CfgBuf    拷贝后的内存地址
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_CopyCfgToMem(ISP_DEV IspDev, NI_BOOL bEnable, NI_U32 *pu32CfgBuf);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_SelectCfgInfo
 * 描  述:    选择使用配置的方式--cvi项目用于内存切换,选用flash配置还是内存中配置,IPC项目用于标识内存中的数据允许被修改。
 * 输  入:   参数          描述
 *       :   IspDev        Isp设备号
         :   enCfgSelect   配置选择
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_SelectCfgInfo(ISP_DEV IspDev, AISP_CFG_SELECT_E enCfgSelect);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_SetModRunState
 * 描  述:    设置单模块运行使能--调试接口，用于控制模块的运行
 * 输  入:   参数          描述
 *       :   pszModName   模块名
         :   pbRunEn      使能开关
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_SetModRunState(ISP_DEV IspDev, NI_CHAR *pszModName, NI_BOOL *pbRunEn);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_SetModRunState
 * 描  述:    获取单模块运行使能状态--调试接口，用于获取模块使能状态
 * 输  入:   参数          描述
 *       :   pszModName   模块名
         :   pbRunEn      使能开关
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_GetModRunState(ISP_DEV IspDev, NI_CHAR *pszModName, NI_BOOL *pbRunEn);


#if XSCHIP == XS7312_V100
NI_S32 NI_MDK_AISP_AwesInit(ISP_DEV IspDev, AISP_MAWES_INIT_PARAM_S *pstAwes);
NI_S32 NI_MDK_AISP_AwesDeInit(ISP_DEV IspDev);
NI_S32 NI_MDK_AISP_AwesGetData(ISP_DEV IspDev, AISP_MAWES_STATISTICS_S *pstAwes, NI_U32 u32TimeOutMs);
NI_S32 NI_MDK_AISP_AwesReleaseData(ISP_DEV IspDev, AISP_MAWES_STATISTICS_S *pstAwes);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_DnGetResult
 * 描  述:    获取日夜状态信息
 * 输  入:   参数          描述
 *       :   
         :   
         :   
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_DnGetResult(ISP_DEV IspDev, AISP_DN_RESULT_S *pstDnRes);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_SetAeAdjust
 * 描  述:    AE调整曝光开关
 * 输  入:   参数          描述
 *       :   
         :   
         :   
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_SetAeAdjust(ISP_DEV IspDev, NI_BOOL bAeAdjust);

/* 仅在AE曝光模式为Auto时有效 */
/*******************************************************************************
 * 函数名:    NI_MDK_AISP_SetAutoFps
 * 描  述:    设置自动降帧控制参数, 仅在AE曝光模式为Auto时有效
 * 输  入:   参数          描述
 *       :   
         :   
         :   
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_SetAutoFps(ISP_DEV IspDev, AISP_AFPS_CTRL_S *pstAFpsCtrl);

/*******************************************************************************
 * 函数名:    NI_MDK_AISP_GetAutoFps
 * 描  述:    获取自动降帧控制参数, 仅在AE曝光模式为Auto时有效
 * 输  入:   参数          描述
 *       :   
         :   
         :   
 * 输  出:   无
 * 返回值:   NI_SUCCESS: 成功
 *          NI_FAILURE: 失败
 *******************************************************************************/
NI_S32 NI_MDK_AISP_GetAutoFps(ISP_DEV IspDev, AISP_AFPS_CTRL_S *pstAFpsCtrl);

#endif


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of #ifndef __NI_AISP_H__ */

