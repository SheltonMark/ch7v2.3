/* 
 * sdk_comm.h -- sample公用头文件 
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */


#ifndef _SDK_COMM_H_
#define _SDK_COMM_H_


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */

#include "ni_comm_vi.h"
#include "ni_comm_pisp.h"
#include "ni_comm_vo.h"
#include "ni_comm_vps.h"
#include "ni_comm_osd.h"
#include "ni_comm_smart_enc.h"
#include "ni_comm_venc.h"
#include "ni_comm_pm.h"
#include "mdk_sys.h"
#include "mdk_mbuf.h"
#include "ni_sensor.h"
#include "mkp_nifb.h"
#include "mdk_audio.h"
#include "mdk_codec.h"
#include "mdk_venc.h"
// #include "mdk_rotate.h"
// #include "ni_comm_rotate.h"
#include "mdk_vps.h"
#include "mdk_vi.h"
#include "faac.h"
#include "mdk_aisp.h"
#include "mdk_pisp.h"
#include "mdk_md.h"
#include "ni_ias_od.h"
#include "mdk_osd.h"
#include "faad.h"
// #include "ni_ptz_tracking.h"
// #include "mdk_clock.h"
#include "mdk_aispalg.h"
#include "ni_ias_ad.h"
#include "ni_ias_pet_detect.h"
#include "mdk_pm.h"
#include "PrintGrade.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */


/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */

typedef struct tagTestSensorCfg
{
    SEN_TYPE_E          enSenType;
    
    VI_INTF_TYPE_E      enIntfType;
    HDR_MODE_E          enHdrMode;
    NI_U32              u32DevAddr;     /* 器件地址 */
    NI_U32              u32I2cDevId;    /* iic设备号 */
    NI_U32              u32RstGpioId;   /* 复位引脚 */
    NI_U32              u32ClkFreq;     /* sensor vclk时钟频率,   单位:KHZ */
    NI_U32              u32Width;
    NI_U32              u32Height;
    NI_U32              u32Fps;
    NI_U32              u32LaneNum;     /* MIPI 数据lane数目*/
}TEST_SEN_CONFIG_S;

typedef enum tagTestViMode
{
    TEST_VI_SC2238_DC_720P_30FPS,
    TEST_VI_SC2238_DC_1080P_25FPS,
    TEST_VI_SC200AI_MIPI_720P_30FPS,
    TEST_VI_SC200AI_MIPI_1080P_25FPS,
    TEST_VI_SC200AI_MIPI_1080P_25FPS_HDR,
    TEST_VI_IMX307_MIPI_720P_30FPS,
    TEST_VI_IMX307_MIPI_1080P_7FPS,
    TEST_VI_IMX307_MIPI_1080P_25FPS,
    TEST_VI_IMX307_MIPI_1080P_30FPS,
    TEST_VI_IMX307_MIPI_720P_25FPS_HDR,
    TEST_VI_IMX307_MIPI_1080P_12_5FPS_DHR,
    TEST_VI_IMX307_MIPI_1080P_3FPS_DHR,
    TEST_VI_IMX307_MIPI_1080P_25FPS_HDR,
    TEST_VI_IMX307_MIPI_1080P_30FPS_HDR,
    TEST_VI_IMX415_MIPI_4K_25FPS,
    TEST_VI_IMX415_MIPI_4K_30FPS_HDR, 
    TEST_VI_OS04A10_MIPI_2688x1520_25FPS,
    TEST_VI_OS04A10_MIPI_2688x1520_30FPS,
    TEST_VI_OS04A10_MIPI_2688x1520_25FPS_HDR,
    TEST_VI_OS05A10_MIPI_2688x1944_25FPS,
    TEST_VI_OS05A10_MIPI_2688x1944_20FPS_HDR,
    TEST_VI_OS04C10_MIPI_2688x1520_25FPS,
    TEST_VI_OS04C10_MIPI_2688x1520_25FPS_HDR,
    TEST_VI_OS04L10_MIPI_2560x1440_30FPS,
    TEST_VI_OS04D10_MIPI_2560x1440_30FPS,
    TEST_VI_GC2083_MIPI_1080P_30FPS,
    TEST_VI_SC2239P_MIPI_720P_60FPS,
    TEST_VI_SC2239P_MIPI_1080P_30FPS,
    TEST_VI_GC4023_MIPI_2560x1440_30FPS,
    TEST_VI_GC4653_MIPI_2560x1440_30FPS,
    TEST_VI_COLOR_720P_30FPS,
    TEST_VI_COLOR_1080P_10FPS,
    TEST_VI_NI9931_BT656_720P_30FPS,
    TEST_VI_NI9931_BT1120_1080P_25FPS,

    TEST_VI_MODE_BUTT
} TEST_VI_MODE_E;

typedef struct tagTestViType
{
    NI_CHAR             as8Name[32];
    TEST_VI_MODE_E      enVimode;
}TEST_VI_TYPE_S;

typedef struct tagTestViConfig
{
    VI_WORK_MODE_E      enViWorkMode;
    NI_BOOL             b2Venc;     /* VI 的数据直接通过bind发送venc  */
    VENC_CHN            VeChn;      /* VI 的发送的venc通道号  */
    PIXEL_FORMAT_E      enFmt;      /* VI上内存格式 */
}TEST_VI_CONFIG_S;

typedef struct hiSAMPLE_VI_INFO_S
{
    TEST_VI_MODE_E      enViMode;
    TEST_SEN_CONFIG_S   stSenCfg;
    TEST_VI_CONFIG_S    stViCfg;
} SAMPLE_VI_INFO_S;

typedef struct tagTestIspConfig
{
    NI_U32 u32Width;
    NI_U32 u32Height;
    NI_U32 u32Fps;
}TEST_ISP_CONFIG_S;

TEST_VI_TYPE_S gastViType[TEST_VI_MODE_BUTT];
#define SAMPLE_GET_VI_TYPE() gastViType

extern VI_DEV_ATTR_S gstViDevAttr_SC2238_DC_720p;
extern VI_DEV_ATTR_S gstViDevAttr_SC2238_DC_1080p;
extern VI_DEV_ATTR_S gstViDevAttr_Imx307_Mipi_720p;
extern VI_DEV_ATTR_S gstViDevAttr_Imx307_Mipi_720p_Hdr;
extern VI_DEV_ATTR_S gstViDevAttr_Imx307_Mipi_1080p_12_5fps_Hdr;
extern VI_DEV_ATTR_S gstViDevAttr_Imx307_Mipi_1080p_3fps_Hdr;
extern VI_DEV_ATTR_S gstViDevAttr_Imx307_Mipi_1080p_25fps_Hdr;
extern VI_DEV_ATTR_S gstViDevAttr_Imx307_Mipi_1080p_30fps_Hdr;
extern VI_DEV_ATTR_S gstViDevAttr_Imx307_Mipi_1080p;
extern VI_DEV_ATTR_S gstViDevAttr_Imx307_Mipi_1080p_7fps;
extern VI_DEV_ATTR_S gstViDevAttr_Imx415_Mipi_4k_25fps;
extern VI_DEV_ATTR_S gstViDevAttr_Imx415_Mipi_4k_30fps_Hdr;
extern VI_DEV_ATTR_S gstViDevAttr_NI9931_720p_25fps;
extern VI_DEV_ATTR_S gstViDevAttr_NI9931_1080p_25fps;
extern VI_DEV_ATTR_S gstViDevAttr_SC200AI_720P_30fps;
extern VI_DEV_ATTR_S gstViDevAttr_SC200AI_1080P_25fps;
extern VI_DEV_ATTR_S gstViDevAttr_SC200AI_1080P_25fps_Hdr;
extern VI_DEV_ATTR_S gstViDevAttr_OS04A10_2688x1520_25fps;
extern VI_DEV_ATTR_S gstViDevAttr_OS05A10_2688x1944_25fps;
extern VI_DEV_ATTR_S gstViDevAttr_OS04A10_2688x1520_25fps_Hdr;
extern VI_DEV_ATTR_S gstViDevAttr_OS05A10_2688x1944_25fps_Hdr;
extern VI_DEV_ATTR_S gstViDevAttr_GC4023_2560x1440_30fps;
extern VI_DEV_ATTR_S gstViDevAttr_GC4653_2560x1440_30fps;
extern VI_DEV_ATTR_S gstViDevAttr_OS04L10_2560x1440_30fps;
extern VI_DEV_ATTR_S gstViDevAttr_OS04D10_2560x1440_30fps;
extern VI_DEV_ATTR_S gstViDevAttr_GC2083_1080P_30fps;
extern VI_DEV_ATTR_S gstViDevAttr_SC2239P_Mipi_1080p_30fps;
extern VI_DEV_ATTR_S gstViDevAttr_SC2239P_Mipi_720p_60fps;
extern VI_DEV_ATTR_S gstViDevAttr_OS04C10_2688x1520_25fps;
extern VI_DEV_ATTR_S gstViDevAttr_OS04C10_2688x1520_25fps_Hdr;

extern VI_DEV_ATTR_S gstViDevAttr_COLOR_720p;
extern VI_DEV_ATTR_S gstViDevAttr_COLOR_1080p;

extern VI_MIPI_PARAM_S gstViMipiParam_Imx307_720p;
extern VI_MIPI_PARAM_S gstViMipiParam_Imx307_1080p;
extern VI_MIPI_PARAM_S gstViMipiParam_Imx307_1080p_7fps;
extern VI_MIPI_PARAM_S gstViMipiParam_Imx307_720p_Hdr;
extern VI_MIPI_PARAM_S gstViMipiParam_Imx307_1080p_12_5fps_Hdr;
extern VI_MIPI_PARAM_S gstViMipiParam_Imx307_1080p_3fps_Hdr;
extern VI_MIPI_PARAM_S gstViMipiParam_Imx307_1080p_25fps_Hdr;
extern VI_MIPI_PARAM_S gstViMipiParam_Imx307_1080p_30fps_Hdr;
extern VI_MIPI_PARAM_S gstViMipiParam_Imx415_4k_25fps;
extern VI_MIPI_PARAM_S gstViMipiParam_Imx415_4k_30fps_Hdr;
extern VI_MIPI_PARAM_S gstViMipiParam_SC200AI_720P_30fps;
extern VI_MIPI_PARAM_S gstViMipiParam_SC200AI_1080P_25fps;
extern VI_MIPI_PARAM_S gstViMipiParam_SC200AI_1080P_25fps_Hdr;
extern VI_MIPI_PARAM_S gstViMipiParam_OS04A10_2688x1520_25fps;
extern VI_MIPI_PARAM_S gstViMipiParam_OS05A10_2688x1944_25fps;
extern VI_MIPI_PARAM_S gstViMipiParam_OS04A10_2688x1520_25fps_Hdr;
extern VI_MIPI_PARAM_S gstViMipiParam_OS05A10_2688x1944_25fps_Hdr;
extern VI_MIPI_PARAM_S gstViMipiParam_GC4023_2560x1440_30fps;
extern VI_MIPI_PARAM_S gstViMipiParam_GC4653_2560x1440_30fps;
extern VI_MIPI_PARAM_S gstViMipiParam_OS04L10_2560x1440_30fps;
extern VI_MIPI_PARAM_S gstViMipiParam_OS04D10_2560x1440_30fps;
extern VI_MIPI_PARAM_S gstViMipiParam_GC2083_1080P_30fps;
extern VI_MIPI_PARAM_S gstViMipiParam_SC2239P_1080p_30fps;
extern VI_MIPI_PARAM_S gstViMipiParam_SC2239P_720p_60fps;
extern VI_MIPI_PARAM_S gstViMipiParam_OS04C10_2688x1520_25fps;
extern VI_MIPI_PARAM_S gstViMipiParam_OS04C10_2688x1520_25fps_Hdr;

typedef struct tagTestVoConfig
{    
    NI_BOOL             bColorBar;      /* 数据是否为VO的colorbar */
    VO_INTF_TYPE_E      enIntfType;     /* 显示接口类型，支持或操作，如:VO_INTF_CVBS|VO_INTF_BT1120 */
    VO_INTF_SYNC_E      enIntfSync;    
    NI_U32              u32Width;       //必须和enIntfSync对应的宽高一致，VO_SYNC_1080P25对应的1920/1080
    NI_U32              u32Height;
    VO_OUT_FORMAT_E     enOutFormat;
    YUV_SEQ_E           enYuvSeq;       /* VPS 输出的YUV格式 */
}TEST_VO_CONFIG_S;

typedef struct tagTestVpsConfig
{
    NI_U32          u32InWidth;
    NI_U32          u32InHeight;
    NI_U32          u32InFps;
    NI_U32          u32OutWidth;
    NI_U32          u32OutHeight;
    NI_U32          u32OutFps;
    NI_BOOL         bYuv2Ldc;   /* VPS数据是否经过LDC矫正 */
    PIXEL_FORMAT_E  enOutFormat; /* 数据输出格式 */
    VENC_CHN        enEncBindChn;/* 编码通道绑定通道号 */
    NI_BOOL        bOnLineMode;
    PAYLOAD_TYPE_E  enPayLoad;
}TEST_VPS_CONFIG_S;



typedef struct tagTestCommIspThreadParam
{
    NI_BOOL    bThreadStart;
    ISP_DEV    IspDev;
}TEST_COMM_ISP_THREAD_PARA_S;

typedef struct tagTestVencConfig
{
    NI_U32          u32InWidth;
    NI_U32          u32InHeight;
    NI_U32          u32InFps;
    NI_U32          u32OutFps;
    PIXEL_FORMAT_E  enEncType;
    PAYLOAD_TYPE_E  enPayLoad;
    VENC_RC_MODE_E  enRcMode;
}TEST_VENC_CONFIG_S;

typedef struct tagSrcOsdAttr
{
    OSD_REGION_S stOsdRegion;
    SIZE_S          stSize;  
    NI_U8        u8FgIdx;
    NI_U8        u8BgIdx;
    OSD_ACCESS_FORMAT_E enFormat;
}SRC_OSD_ATTR;


typedef struct tagSampleOsdConfig
{
    POINT_S                 stStartPiont;     
    NI_U32                  u32Layer;          
    NI_BOOL                 bSpecialFrmId;   
    NI_U32                  u32TargetFrameId;
}SAMPLE_OSD_CONFIG_S;

typedef struct tagSampleNifbConfig
{
    NI_BOOL             bShow;
    NIFB_COLOR_FMT_E    enColorFmt;
    NI_U8               u8PixelAlpha;
    NI_U8               u8GlobalAlpha;
    RECT_S              stRect;
}SAMPLE_NIFB_CONFIG_S;

typedef struct tagSampleCfgParam {
    TEST_SEN_CONFIG_S  stSenCfg;
    SAMPLE_VI_INFO_S   stViInfo;
    TEST_ISP_CONFIG_S  stIspCfg;
    TEST_VPS_CONFIG_S  stVpsCfg;
    TEST_VENC_CONFIG_S stVencCfg;
} SAMPLE_CFG_PARAM_S;


/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */

NI_S32 SDK_COMM_SYS_Init();
NI_S32 SDK_COMM_SYS_Exit();

NI_S32 SDK_COMM_SEN_Start(SENSOR_ID SenId, TEST_SEN_CONFIG_S *pstSenCfg);

NI_S32 SDK_COMM_VI_GetViInfo(SENSOR_ID SenId, TEST_VI_MODE_E enViMode, SAMPLE_VI_INFO_S *pstViInfo);
NI_S32 SDK_COMM_VI_GetDevAttr(TEST_VI_MODE_E enViMode, VI_DEV_ATTR_S *pstViDevAttr);
NI_S32 SDK_COMM_VI_Config(VI_DEV ViDev, SAMPLE_VI_INFO_S *pstViInfo);
NI_S32 SDK_COMM_VI_Start(VI_DEV ViDev);
NI_S32 SDK_COMM_VI_Stop(VI_DEV ViDev);

NI_S32 SDK_COMM_ISP_Start(ISP_DEV IspDev, TEST_ISP_CONFIG_S *pstIspConfig, NI_BOOL bRunThread2A, NI_BOOL bHdrMode);
NI_S32 SDK_COMM_ISP_Stop(ISP_DEV IspDev);

NI_S32 SDK_COMM_VO_Start(VPS_GRP VpsGrp,VO_DEV VoDev, TEST_VO_CONFIG_S *pstVoConfig);
NI_S32 SDK_COMM_VO_Stop(VO_DEV VoDev);

NI_S32 SDK_COMM_VPS_Start(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, TEST_VPS_CONFIG_S *pstVpsCfg);
NI_S32 SDK_COMM_VPS_Stop(VPS_GRP VpsGrp, VPS_CHN_E VpsChn);
NI_S32 SDK_COMM_VPS_StartSendFrame(VPS_GRP VpsGrp,NI_U32 u32Width,NI_U32 u32Height,NI_U32 u32InPutFps,NI_CHAR * pFileName,PIXEL_FORMAT_E enPixelFormat,NI_BOOL bSendOnece,NI_BOOL bStepDebug);
NI_S32 SDK_COMM_VPS_StopSendFrame(VPS_GRP VpsGrp);
NI_S32 SDK_COMM_VPS_StartGetFrame(VPS_GRP VpsGrp, VPS_CHN_E VpsChn, NI_BOOL bSaveFile);
NI_S32 SDK_COMM_VPS_StopGetFrame(VPS_GRP VpsGrp, VPS_CHN_E VpsChn);

NI_S32 SDK_COMM_VENC_StartSendFrame(VENC_CHN VeChn,NI_U32 u32Width,NI_U32 u32Height,NI_CHAR * pFileName,PIXEL_FORMAT_E enEncType,NI_BOOL bVencDebug);
NI_S32 SDK_COMM_VENC_StopSendFrame(VENC_CHN VeChn);
NI_S32 SDK_COMM_VENC_StartGetStream(VENC_CHN VeChn, NI_BOOL bSaveFile);
NI_S32 SDK_COMM_VENC_StopGetStream(VENC_CHN VeChn);
NI_S32 SDK_COMM_VENC_Start(VENC_CHN VeChn,TEST_VENC_CONFIG_S *pstVencCfg);
NI_S32 SDK_COMM_VENC_Stop(VENC_CHN VeChn);


NI_S32 SAMPLE_SMART_ENC_StartSendStream(VENC_CHN VeChn, PAYLOAD_TYPE_E enType, VENC_RC_MODE_E enRcMode, NI_CHAR *pcFileName);
NI_S32 SAMPLE_SMART_ENC_StopSendStream(VENC_CHN VeChn);
NI_S32 SAMPLE_SMART_ENC_StartGetStream(VENC_CHN VeChn);
NI_S32 SAMPLE_SMART_ENC_StopGetStream(VENC_CHN VeChn);
NI_S32 SAMPLE_SMART_ENC_Stop(VENC_CHN VeChn);

NI_S32 SDK_COMM_NIFB_Start(NI_S32 s32Layer, SAMPLE_NIFB_CONFIG_S *pstNifbConfig);
NI_S32 SDK_COMM_NIFB_Stop(NI_S32 s32Layer);

NI_S32  SDK_COMM_VD_StartGetData(VD_CHN VdChn, NI_U32 u32Width, NI_U32 u32Height);
NI_S32  SDK_COMM_VD_StopGetData(VD_CHN VdChn);
NI_S32  SDK_COMM_VD_StartGetDrawData(VD_CHN VdChn, NI_U8 u8AomId);
NI_S32  SDK_COMM_VD_StopGetDrawData(VD_CHN VdChn);

NI_S32 SDK_COMM_OSD_Creat(NI_U32 u32OsdHandle, MDK_CHN_S *pstMdkChn, SAMPLE_OSD_CONFIG_S *pstOsdCfg);
NI_S32 SDK_COMM_OSD_Destroy(NI_U32 u32OsdHandle, MDK_CHN_S *pstMdkChn);

NI_S32 SAMPLE_SMART_ENC_StopGetStream(VENC_CHN VeChn);
NI_S32 SAMPLE_SMART_ENC_StopSendStream(VENC_CHN VeChn);
NI_S32 SAMPLE_SMART_ENC_Stop(VENC_CHN VeChn);
NI_S32 SAMPLE_SMART_ENC_StartSendStream(VENC_CHN VeChn, PAYLOAD_TYPE_E enType, VENC_RC_MODE_E enRcMode, NI_CHAR *pcFileName);
NI_S32 SAMPLE_SMART_ENC_StartGetStream(VENC_CHN VeChn);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

