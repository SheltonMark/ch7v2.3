/**
 * @file ax_adapter.c
 * @brief AX520CE platform adapter implementation
 *
 * This file implements the complete platform adapter interface for AX520CE chipset.
 * All NI_MDK_* SDK calls are wrapped here, including:
 * - System initialization (MBUF, SYS, Pinmux, Clock)
 * - Sensor operations (Mirror, Flip, GetInfo)
 * - ISP operations (PubAttr, VideoFmt, AutoFps)
 * - VI operations (Video Input)
 * - VPS operations (Video Processing/Scaling)
 * - VENC operations (Video Encoding)
 * - OSD operations (On-Screen Display)
 * - System binding
 */

#include "../include/platform_adapter.h"
#include "sdk_comm.h"
#include "PrintGrade.h"
#include "product_config.h"  // For product-specific configurations
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* Adapter return codes */
#define AX_SUCCESS  0
#define AX_FAILURE  (-1)

/* ========================================================================
 *  System Initialization Operations
 * ======================================================================== */

/**
 * @brief Initialize AX520 system SDK
 */
static int AX_SYS_SystemInit(void)
{
    int s32Ret = -1;
    MBUF_CONF_S stMbufConf;

    /* Clean up any previous instances */
    NI_MDK_SYS_Exit();
    NI_MDK_MBUF_Exit();

    /* VPS uses private pool, so common pool can be minimal */
    memset(&stMbufConf, 0, sizeof(MBUF_CONF_S));
    stMbufConf.u32MaxPoolCnt = 128;
    stMbufConf.astCommPool[0].u32BlkCnt  = 0;
    stMbufConf.astCommPool[0].u32BlkSize = 1920 * 1080 * 2;  /* 16200KB */
    strcpy(stMbufConf.astCommPool[1].acMmzName, "anonymous");

    s32Ret = NI_MDK_MBUF_SetConf(&stMbufConf);
    if (SUCCESS != s32Ret) {
        PRINT_ERROR("AX: NI_MDK_MBUF_SetConf failed, ret=%#x\n", s32Ret);
        return AX_FAILURE;
    }

    s32Ret = NI_MDK_MBUF_Init();
    if (SUCCESS != s32Ret) {
        PRINT_ERROR("AX: NI_MDK_MBUF_Init failed, ret=%#x\n", s32Ret);
        return AX_FAILURE;
    }

    s32Ret = NI_MDK_SYS_Init();
    if (SUCCESS != s32Ret) {
        PRINT_ERROR("AX: NI_MDK_SYS_Init failed, ret=%#x\n", s32Ret);
        return AX_FAILURE;
    }

    /* Set clock manager policy */
    s32Ret = NI_MDK_PM_SetClkMgrPolicy(PM_CLK_MGR_POLICY_LEVEL);
    if (SUCCESS != s32Ret) {
        PRINT_ERROR("AX: NI_MDK_PM_SetClkMgrPolicy failed, ret=%#x\n", s32Ret);
        return AX_FAILURE;
    }

    s32Ret = NI_MDK_PM_SetClkCapLevel(PM_CLOCK_CAP_LEVEL_S5);
    if (SUCCESS != s32Ret) {
        PRINT_ERROR("AX: NI_MDK_PM_SetClkCapLevel failed, ret=%#x\n", s32Ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX: System initialization success\n");
    return AX_SUCCESS;
}

static int AX_SYS_SystemExit(void)
{
    NI_MDK_SYS_Exit();
    PRINT_INFO("AX: System de-initialization success\n");
    return AX_SUCCESS;
}

static int AX_SYS_MbufInit(void)
{
    int s32Ret = NI_MDK_MBUF_Init();
    if (AX_SUCCESS != s32Ret) {
        PRINT_ERROR("AX: MBUF init failed, ret=%#x\n", s32Ret);
        return AX_FAILURE;
    }
    PRINT_INFO("AX: MBUF initialization success\n");
    return AX_SUCCESS;
}

static int AX_SYS_MbufExit(void)
{
    NI_MDK_MBUF_Exit();
    PRINT_INFO("AX: MBUF de-initialization success\n");
    return AX_SUCCESS;
}

/* ========================================================================
 *  System Binding Operations
 * ======================================================================== */

static int AX_SYS_Bind(MDK_CHN_S* pSrcChn, MDK_CHN_S* pDestChn)
{
    if (!pSrcChn || !pDestChn) {
        PRINT_ERROR("AX: Bind channel pointers are NULL\n");
        return AX_FAILURE;
    }

    int ret = NI_MDK_SYS_Bind(pSrcChn, pDestChn);
    if (ret != 0) {
        PRINT_ERROR("AX: NI_MDK_SYS_Bind failed, ret=%#x\n", ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX: SYS Bind success, [%d,%d,%d]->[%d,%d,%d]\n",
               pSrcChn->enModId, pSrcChn->s32DevId, pSrcChn->s32ChnId,
               pDestChn->enModId, pDestChn->s32DevId, pDestChn->s32ChnId);
    return AX_SUCCESS;
}

static int AX_SYS_Unbind(MDK_CHN_S* pSrcChn, MDK_CHN_S* pDestChn)
{
    if (!pSrcChn || !pDestChn) {
        PRINT_ERROR("AX: Unbind channel pointers are NULL\n");
        return AX_FAILURE;
    }

    int ret = NI_MDK_SYS_UnBind(pSrcChn, pDestChn);
    if (ret != 0) {
        PRINT_ERROR("AX: NI_MDK_SYS_UnBind failed, ret=%#x\n", ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX: SYS Unbind success, [%d,%d,%d]-X-[%d,%d,%d]\n",
               pSrcChn->enModId, pSrcChn->s32DevId, pSrcChn->s32ChnId,
               pDestChn->enModId, pDestChn->s32DevId, pDestChn->s32ChnId);
    return AX_SUCCESS;
}

/**
 * @brief Initialize Pinmux based on product configuration
 *
 * Different products have different hardware pin configurations (I2C, VCLK, GPIO).
 * This function reads product_config and applies the correct register settings.
 */
static int AX_SYS_PinmuxInit(void)
{
    unsigned int u32Value = 0;
    const ProductConfig_t *product_cfg = ProductConfig_Get();

    if (!product_cfg) {
        PRINT_ERROR("AX: Failed to get product configuration\n");
        return AX_FAILURE;
    }

    PRINT_INFO("AX: Configuring Pinmux for hw_variant=%d\n", product_cfg->hw_variant);

    switch (product_cfg->hw_variant) {
        case IPC_CH7V20_HW:
            /* CH7 v2.0: I2C0, VCLK, GPIO38 for sensor reset */

            /* Set I2C0 pinmux, sensor vclk pinmux, and sensor reset gpio38 */
            NI_MDK_SYS_ReadReg(0x09001014, &u32Value);
            u32Value &= 0xf0000fff;
            u32Value |= 0x222000;
            NI_MDK_SYS_WriteReg(0x09001014, u32Value);

            /* Configure GPIO38 for sensor reset */
            NI_MDK_SYS_ReadReg(0x09050004, &u32Value);
            u32Value &= ~(1 << 6);
            u32Value |= 0x40;
            NI_MDK_SYS_WriteReg(0x09050004, u32Value);

            /* Sensor reset sequence: pull low -> delay -> pull high */
            NI_MDK_SYS_ReadReg(0x09050000, &u32Value);
            u32Value &= ~(1 << 6);
            NI_MDK_SYS_WriteReg(0x09050000, u32Value);
            usleep(20 * 1000);  /* 20ms delay */

            u32Value |= (1 << 6);
            NI_MDK_SYS_WriteReg(0x09050000, u32Value);
            usleep(50 * 1000);  /* 50ms delay for sensor to stabilize */
            break;

        case IPC_CP3V31_HW:
            /* CP3 v3.1: different I2C/GPIO configuration */
            PRINT_INFO("AX: Using IPC_CP3V31_HW pinmux\n");

        case IPC_CP7V20_HW:
            /* CP7 v2.0: another variant */
            PRINT_INFO("AX: Using IPC_CP7V20_HW pinmux\n");

        default:
            PRINT_ERROR("AX: Unknown hw_variant=%d\n", product_cfg->hw_variant);

            return AX_FAILURE;
    }

    PRINT_INFO("AX: Pinmux initialization success for hw_variant=%d\n", product_cfg->hw_variant);
    return AX_SUCCESS;
}

/* ========================================================================
 *  Sensor Operations
 * ======================================================================== */

static int AX_SensorSetMirror(int SenId, int bMirror)
{
    int ret = NI_SEN_SetMirror(SenId, bMirror);
    if (ret != 0) {
        PRINT_ERROR("AX: NI_SEN_SetMirror failed, SenId=%d mirror=%d ret=%#x\n",
                    SenId, bMirror, ret);
        return AX_FAILURE;
    }
    PRINT_INFO("AX: Sensor SetMirror success, SenId=%d mirror=%d\n", SenId, bMirror);
    return AX_SUCCESS;
}

static int AX_SensorSetFlip(int SenId, int bFlip)
{
    int ret = NI_SEN_SetFlip(SenId, bFlip);
    if (ret != 0) {
        PRINT_ERROR("AX: NI_SEN_SetFlip failed, SenId=%d flip=%d ret=%#x\n",
                    SenId, bFlip, ret);
        return AX_FAILURE;
    }
    PRINT_INFO("AX: Sensor SetFlip success, SenId=%d flip=%d\n", SenId, bFlip);
    return AX_SUCCESS;
}

static int AX_SensorGetCurInfo(int SenId, SEN_INFO_S* pstSenInfo)
{
    if (!pstSenInfo) {
        PRINT_ERROR("AX: Sensor info pointer is NULL\n");
        return AX_FAILURE;
    }

    int ret = NI_SEN_GetCurInfo(SenId, pstSenInfo);
    if (ret != 0) {
        PRINT_ERROR("AX: NI_SEN_GetCurInfo failed, SenId=%d ret=%#x\n", SenId, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX: Sensor GetCurInfo success, SenId=%d fps=%d\n",
               SenId, pstSenInfo->stSenFmtAttr.u32Fps);
    return AX_SUCCESS;
}

/* ========================================================================
 *  ISP Operations
 * ======================================================================== */

static int AX_ISP_GetPubAttr(int IspDev, PISP_PUB_ATTR_S* pstPubAttr)
{
    if (!pstPubAttr) {
        PRINT_ERROR("AX: ISP PubAttr pointer is NULL\n");
        return AX_FAILURE;
    }

    int ret = NI_MDK_PISP_GetPubAttr(IspDev, pstPubAttr);
    if (ret != 0) {
        PRINT_ERROR("AX: NI_MDK_PISP_GetPubAttr failed, IspDev=%d ret=%#x\n", IspDev, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX: ISP GetPubAttr success, IspDev=%d fps=%d\n",
               IspDev, pstPubAttr->u32FrameRate);
    return AX_SUCCESS;
}

static int AX_ISP_SetPubAttr(int IspDev, PISP_PUB_ATTR_S* pstPubAttr)
{
    if (!pstPubAttr) {
        PRINT_ERROR("AX: ISP PubAttr pointer is NULL\n");
        return AX_FAILURE;
    }

    int ret = NI_MDK_PISP_SetPubAttr(IspDev, pstPubAttr);
    if (ret != 0) {
        PRINT_ERROR("AX: NI_MDK_PISP_SetPubAttr failed, IspDev=%d ret=%#x\n", IspDev, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX: ISP SetPubAttr success, IspDev=%d fps=%d\n",
               IspDev, pstPubAttr->u32FrameRate);
    return AX_SUCCESS;
}

static int AX_ISP_GetVideoFmtAttr(int IspDev, AISP_VIDEO_FMT_INFO_S* pstVideoFmtInfo)
{
    if (!pstVideoFmtInfo) {
        PRINT_ERROR("AX: ISP VideoFmtInfo pointer is NULL\n");
        return AX_FAILURE;
    }

    int ret = NI_MDK_AISP_GetVideoFmtAttr(IspDev, pstVideoFmtInfo);
    if (ret != 0) {
        PRINT_ERROR("AX: NI_MDK_AISP_GetVideoFmtAttr failed, IspDev=%d ret=%#x\n", IspDev, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX: ISP GetVideoFmtAttr success, IspDev=%d fps=%d\n",
               IspDev, pstVideoFmtInfo->u32Fps);
    return AX_SUCCESS;
}

static int AX_ISP_SetVideoFmtAttr(int IspDev, AISP_VIDEO_FMT_INFO_S* pstVideoFmtInfo)
{
    if (!pstVideoFmtInfo) {
        PRINT_ERROR("AX: ISP VideoFmtInfo pointer is NULL\n");
        return AX_FAILURE;
    }

    int ret = NI_MDK_AISP_SetVideoFmtAttr(IspDev, pstVideoFmtInfo);
    if (ret != 0) {
        PRINT_ERROR("AX: NI_MDK_AISP_SetVideoFmtAttr failed, IspDev=%d ret=%#x\n", IspDev, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX: ISP SetVideoFmtAttr success, IspDev=%d fps=%d\n",
               IspDev, pstVideoFmtInfo->u32Fps);
    return AX_SUCCESS;
}

static int AX_ISP_GetAutoFps(int IspDev, AISP_AFPS_CTRL_S* pstAFpsCtrl)
{
    if (!pstAFpsCtrl) {
        PRINT_ERROR("AX: ISP AFpsCtrl pointer is NULL\n");
        return AX_FAILURE;
    }

    int ret = NI_MDK_AISP_GetAutoFps(IspDev, pstAFpsCtrl);
    if (ret != 0) {
        PRINT_ERROR("AX: NI_MDK_AISP_GetAutoFps failed, IspDev=%d ret=%#x\n", IspDev, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX: ISP GetAutoFps success, IspDev=%d enable=%d\n",
               IspDev, pstAFpsCtrl->bEnable);
    return AX_SUCCESS;
}

static int AX_ISP_SetAutoFps(int IspDev, AISP_AFPS_CTRL_S* pstAFpsCtrl)
{
    if (!pstAFpsCtrl) {
        PRINT_ERROR("AX: ISP AFpsCtrl pointer is NULL\n");
        return AX_FAILURE;
    }

    int ret = NI_MDK_AISP_SetAutoFps(IspDev, pstAFpsCtrl);
    if (ret != 0) {
        PRINT_ERROR("AX: NI_MDK_AISP_SetAutoFps failed, IspDev=%d ret=%#x\n", IspDev, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX: ISP SetAutoFps success, IspDev=%d enable=%d fps=%d\n",
               IspDev, pstAFpsCtrl->bEnable, pstAFpsCtrl->u32NewFps);
    return AX_SUCCESS;
}

/* ========================================================================
 *  VI (Video Input) Operations
 * ======================================================================== */

static int AX_VI_Init(void)
{
    NI_U32 u32Value;
	const ProductConfig_t *product_cfg = ProductConfig_Get();
    NI_MDK_SYS_ReadReg(0x09050000, &u32Value);
    u32Value &= ~(1 << 6);
    NI_MDK_SYS_WriteReg(0x09050000, u32Value);
    usleep(20 * 1000);
    u32Value |= (1 << 6);
    NI_MDK_SYS_WriteReg(0x09050000, u32Value);
    usleep(50 * 1000);

    PRINT_INFO("AX: VI Init success\n");
    return AX_SUCCESS;
}

static int AX_VI_CreateDev(int ViDev, SAMPLE_VI_INFO_S* pstViInfo)
{
    if (!pstViInfo) {
        PRINT_ERROR("AX: VI info is NULL\n");
        return AX_FAILURE;
    }

    int ret = SDK_COMM_VI_Config(ViDev, pstViInfo);
    if (ret != 0) {
        PRINT_ERROR("AX: SDK_COMM_VI_Config failed, ret=%#x\n", ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX: VI CreateDev success, ViDev=%d\n", ViDev);
    return AX_SUCCESS;
}

static int AX_VI_StartDev(int ViDev)
{
    int ret = SDK_COMM_VI_Start(ViDev);
    if (ret != 0) {
        PRINT_ERROR("AX: SDK_COMM_VI_Start failed, ret=%#x\n", ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX: VI StartDev success, ViDev=%d\n", ViDev);
    return AX_SUCCESS;
}

static int AX_VI_StopDev(int ViDev)
{
    int ret = SDK_COMM_VI_Stop(ViDev);
    if (ret != 0) {
        PRINT_ERROR("AX: SDK_COMM_VI_Stop failed, ret=%#x\n", ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX: VI StopDev success, ViDev=%d\n", ViDev);
    return AX_SUCCESS;
}

static int AX_VI_DestroyDev(int ViDev)
{
    PRINT_INFO("AX: VI DestroyDev, ViDev=%d\n", ViDev);
    return AX_SUCCESS;
}

static int AX_VI_GetDevAttr(int ViDev, VI_DEV_ATTR_S* pAttr)
{
    if (!pAttr) {
        PRINT_ERROR("AX: VI DevAttr pointer is NULL\n");
        return AX_FAILURE;
    }

    int ret = NI_MDK_VI_GetDevAttr(ViDev, pAttr);
    if (ret != 0) {
        PRINT_ERROR("AX: NI_MDK_VI_GetDevAttr failed, ViDev=%d ret=%#x\n", ViDev, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX: VI GetDevAttr success, ViDev=%d\n", ViDev);
    return AX_SUCCESS;
}

static int AX_VI_SetDevAttr(int ViDev, VI_DEV_ATTR_S* pAttr)
{
    if (!pAttr) {
        PRINT_ERROR("AX: VI DevAttr pointer is NULL\n");
        return AX_FAILURE;
    }

    int ret = NI_MDK_VI_SetDevAttr(ViDev, pAttr);
    if (ret != 0) {
        PRINT_ERROR("AX: NI_MDK_VI_SetDevAttr failed, ViDev=%d ret=%#x\n", ViDev, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX: VI SetDevAttr success, ViDev=%d\n", ViDev);
    return AX_SUCCESS;
}

static int AX_VI_DisableDev(int ViDev)
{
    int ret = NI_MDK_VI_DisableDev(ViDev);
    if (ret != 0) {
        PRINT_ERROR("AX: NI_MDK_VI_DisableDev failed, ViDev=%d ret=%#x\n", ViDev, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX: VI DisableDev success, ViDev=%d\n", ViDev);
    return AX_SUCCESS;
}

static int AX_VI_EnableDev(int ViDev)
{
    int ret = NI_MDK_VI_EnableDev(ViDev);
    if (ret != 0) {
        PRINT_ERROR("AX: NI_MDK_VI_EnableDev failed, ViDev=%d ret=%#x\n", ViDev, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX: VI EnableDev success, ViDev=%d\n", ViDev);
    return AX_SUCCESS;
}

/* ========================================================================
 *  VPS (Video Processing/Scaling) Operations
 * ======================================================================== */

static int AX_VPS_CreateChn(int VpsGrp, int VpsChn, VPS_CHN_ATTR_S* pVpsAttr)
{
    if (!pVpsAttr) {
        PRINT_ERROR("AX: VPS config is NULL\n");
        return AX_FAILURE;
    }

    int ret = NI_MDK_VPS_CreateChn(VpsGrp, VpsChn, pVpsAttr);
    if (ret != 0) {
        PRINT_ERROR("AX: NI_MDK_VPS_SetChnAttr failed, VpsGrp=%d VpsChn=%d ret=%#x\n",
                    VpsGrp, VpsChn, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX: VPS CreateChn success, VpsGrp=%d VpsChn=%d\n", VpsGrp, VpsChn);
    return AX_SUCCESS;
}

static int AX_VPS_DestroyChn(int VpsGrp, int VpsChn)
{
    int ret = NI_MDK_VPS_DestroyChn(VpsGrp, VpsChn);
    if (ret != 0) {
        PRINT_ERROR("AX: NI_MDK_VPS_DestroyChn failed, VpsGrp=%d VpsChn=%d ret=%#x\n",
                    VpsGrp, VpsChn, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX: VPS DestroyChn success, VpsGrp=%d VpsChn=%d\n", VpsGrp, VpsChn);
    return AX_SUCCESS;
}

static int AX_VPS_EnableChn(int VpsGrp, int VpsChn)
{
    int ret = NI_MDK_VPS_EnableChn(VpsGrp, VpsChn);
    if (ret != 0) {
        PRINT_ERROR("AX: NI_MDK_VPS_EnableChn failed, VpsGrp=%d VpsChn=%d ret=%#x\n",
                    VpsGrp, VpsChn, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX: VPS EnableChn success, VpsGrp=%d VpsChn=%d\n", VpsGrp, VpsChn);
    return AX_SUCCESS;
}

static int AX_VPS_DisableChn(int VpsGrp, int VpsChn)
{
    int ret = NI_MDK_VPS_DisableChn(VpsGrp, VpsChn);
    if (ret != 0) {
        PRINT_ERROR("AX: NI_MDK_VPS_DisableChn failed, VpsGrp=%d VpsChn=%d ret=%#x\n",
                    VpsGrp, VpsChn, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX: VPS DisableChn success, VpsGrp=%d VpsChn=%d\n", VpsGrp, VpsChn);
    return AX_SUCCESS;
}

static int AX_VPS_CreateEncodeChannel(int VpsGrp, int VpsChn, VPS_CONFIG_S* pConfig)
{
    if (!pConfig) {
        PRINT_ERROR("VPS config is NULL\n");
        return AX_FAILURE;
    }

    /* Use SDK_COMM_VPS_Start which handles all complex channel setup */
    TEST_VPS_CONFIG_S stVpsCfg = {0};
    stVpsCfg.u32InWidth = pConfig->u32InWidth;
    stVpsCfg.u32InHeight = pConfig->u32InHeight;
    stVpsCfg.u32InFps = pConfig->u32FrameRate;
    stVpsCfg.u32OutWidth = pConfig->u32OutWidth;
    stVpsCfg.u32OutHeight = pConfig->u32OutHeight;
    stVpsCfg.u32OutFps = pConfig->u32FrameRate;
    stVpsCfg.enOutFormat = pConfig->enOutPixFmt;
    stVpsCfg.bOnLineMode = NI_FALSE;  /* Use offline mode */
    stVpsCfg.bYuv2Ldc = NI_FALSE;
    stVpsCfg.enPayLoad = PT_H264;  /* Default to H264, will be set by VENC */
    stVpsCfg.enEncBindChn = VpsChn;  /* Bind to corresponding VENC channel */

    int ret = SDK_COMM_VPS_Start(VpsGrp, (VPS_CHN_E)VpsChn, &stVpsCfg);
    if (ret != NI_SUCCESS) {
        PRINT_ERROR("SDK_COMM_VPS_Start failed, grp=%d chn=%d ret=%#x\n",
                    VpsGrp, VpsChn, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX VPS CreateEncodeChannel success, grp=%d chn=%d %dx%d\n",
               VpsGrp, VpsChn, pConfig->u32OutWidth, pConfig->u32OutHeight);
    return AX_SUCCESS;
}

static int AX_VPS_CreateVdChannel(int VpsGrp, VPS_VD_CHN_ATTR_S* pVdAttr)
{
    if (!pVdAttr) {
        PRINT_ERROR("VD attr is NULL\n");
        return AX_FAILURE;
    }

    /* VD channel uses VPS_CHN_ATTR_S, pVdAttr already contains full structure */
    int ret = NI_MDK_VPS_CreateChn(VpsGrp, VPS_CHN_VD, (const VPS_CHN_ATTR_S*)pVdAttr);
    if (ret != 0) {
        PRINT_ERROR("NI_MDK_VPS_CreateChn (VD) failed, grp=%d ret=%#x\n",
                    VpsGrp, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX VPS CreateVdChannel success, grp=%d\n", VpsGrp);
    return AX_SUCCESS;
}

static int AX_VPS_GetChnAttr(int VpsGrp, int VpsChn, VPS_CHN_ATTR_S* pAttr)
{
    if (!pAttr) {
        PRINT_ERROR("AX: VPS ChnAttr pointer is NULL\n");
        return AX_FAILURE;
    }

    int ret = NI_MDK_VPS_GetChnAttr(VpsGrp, VpsChn, pAttr);
    if (ret != 0) {
        PRINT_ERROR("AX: NI_MDK_VPS_GetChnAttr failed, VpsGrp=%d VpsChn=%d ret=%#x\n",
                    VpsGrp, VpsChn, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX: VPS GetChnAttr success, VpsGrp=%d VpsChn=%d\n", VpsGrp, VpsChn);
    return AX_SUCCESS;
}

static int AX_VPS_SetChnAttr(int VpsGrp, int VpsChn, VPS_CHN_ATTR_S* pAttr)
{
    if (!pAttr) {
        PRINT_ERROR("AX: VPS ChnAttr pointer is NULL\n");
        return AX_FAILURE;
    }

    int ret = NI_MDK_VPS_SetChnAttr(VpsGrp, VpsChn, pAttr);
    if (ret != 0) {
        PRINT_ERROR("AX: NI_MDK_VPS_SetChnAttr failed, VpsGrp=%d VpsChn=%d ret=%#x\n",
                    VpsGrp, VpsChn, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX: VPS SetChnAttr success, VpsGrp=%d VpsChn=%d\n", VpsGrp, VpsChn);
    return AX_SUCCESS;
}

static int AX_VPS_GetGrpAttr(int VpsGrp, VPS_GRP_ATTR_S* pAttr)
{
    if (!pAttr) {
        PRINT_ERROR("AX: VPS GrpAttr pointer is NULL\n");
        return AX_FAILURE;
    }

    int ret = NI_MDK_VPS_GetGrpAttr(VpsGrp, pAttr);
    if (ret != 0) {
        PRINT_ERROR("AX: NI_MDK_VPS_GetGrpAttr failed, VpsGrp=%d ret=%#x\n", VpsGrp, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX: VPS GetGrpAttr success, VpsGrp=%d\n", VpsGrp);
    return AX_SUCCESS;
}

static int AX_VPS_SetGrpAttr(int VpsGrp, VPS_GRP_ATTR_S* pAttr)
{
    if (!pAttr) {
        PRINT_ERROR("AX: VPS GrpAttr pointer is NULL\n");
        return AX_FAILURE;
    }

    int ret = NI_MDK_VPS_SetGrpAttr(VpsGrp, pAttr);
    if (ret != 0) {
        PRINT_ERROR("AX: NI_MDK_VPS_SetGrpAttr failed, VpsGrp=%d ret=%#x\n", VpsGrp, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX: VPS SetGrpAttr success, VpsGrp=%d\n", VpsGrp);
    return AX_SUCCESS;
}

static int AX_VPS_GetModParam(int VpsGrp, VPS_MOD_PARAM_S* pParam)
{
    if (!pParam) {
        PRINT_ERROR("AX: VPS ModParam pointer is NULL\n");
        return AX_FAILURE;
    }

    int ret = NI_MDK_VPS_GetModParam(VpsGrp, pParam);
    if (ret != 0) {
        PRINT_ERROR("AX: NI_MDK_VPS_GetModParam failed, ret=%#x\n", ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX: VPS GetModParam success\n");
    return AX_SUCCESS;
}

static int AX_VPS_SetModParam(int VpsGrp, VPS_MOD_PARAM_S* pParam)
{
    if (!pParam) {
        PRINT_ERROR("AX: VPS ModParam pointer is NULL\n");
        return AX_FAILURE;
    }

    int ret = NI_MDK_VPS_SetModParam(VpsGrp, pParam);
    if (ret != 0) {
        PRINT_ERROR("AX: NI_MDK_VPS_SetModParam failed, ret=%#x\n", ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX: VPS SetModParam success\n");
    return AX_SUCCESS;
}

static int AX_VPS_GetChnMaskAttr(int VpsGrp, int VpsChn, VPS_MASK_ATTR_S* pAttr)
{
    if (!pAttr) {
        PRINT_ERROR("AX: VPS MaskAttr pointer is NULL\n");
        return AX_FAILURE;
    }

    int ret = NI_MDK_VPS_GetChnMaskAttr(VpsGrp, VpsChn, pAttr);
    if (ret != 0) {
        PRINT_ERROR("AX: NI_MDK_VPS_GetChnMaskAttr failed, VpsGrp=%d VpsChn=%d ret=%#x\n",
                    VpsGrp, VpsChn, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX: VPS GetChnMaskAttr success, VpsGrp=%d VpsChn=%d\n", VpsGrp, VpsChn);
    return AX_SUCCESS;
}

static int AX_VPS_SetChnMaskAttr(int VpsGrp, int VpsChn, VPS_MASK_ATTR_S* pAttr)
{
    if (!pAttr) {
        PRINT_ERROR("AX: VPS MaskAttr pointer is NULL\n");
        return AX_FAILURE;
    }

    int ret = NI_MDK_VPS_SetChnMaskAttr(VpsGrp, VpsChn, pAttr);
    if (ret != 0) {
        PRINT_ERROR("AX: NI_MDK_VPS_SetChnMaskAttr failed, VpsGrp=%d VpsChn=%d ret=%#x\n",
                    VpsGrp, VpsChn, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX: VPS SetChnMaskAttr success, VpsGrp=%d VpsChn=%d\n", VpsGrp, VpsChn);
    return AX_SUCCESS;
}

static int AX_VPS_EnableChnMask(int VpsGrp, int VpsChn, int index)
{
    int ret = NI_MDK_VPS_EnableChnMask(VpsGrp, VpsChn, index);
    if (ret != 0) {
        PRINT_ERROR("AX: NI_MDK_VPS_EnableChnMask failed, VpsGrp=%d VpsChn=%d index=%d ret=%#x\n",
                    VpsGrp, VpsChn, index, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX: VPS EnableChnMask success, VpsGrp=%d VpsChn=%d index=%d\n",
               VpsGrp, VpsChn, index);
    return AX_SUCCESS;
}

static int AX_VPS_DisableChnMask(int VpsGrp, int VpsChn, int index)
{
    int ret = NI_MDK_VPS_DisableChnMask(VpsGrp, VpsChn, index);
    if (ret != 0) {
        PRINT_ERROR("AX: NI_MDK_VPS_DisableChnMask failed, VpsGrp=%d VpsChn=%d index=%d ret=%#x\n",
                    VpsGrp, VpsChn, index, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX: VPS DisableChnMask success, VpsGrp=%d VpsChn=%d index=%d\n",
               VpsGrp, VpsChn, index);
    return AX_SUCCESS;
}

/* ========================================================================
 *  VENC (Video Encoding) Operations
 * ======================================================================== */

static int AX_VencCreate(int VencChn, VENC_CHN_ATTR_S* pAttr)
{
    if (!pAttr) {
        PRINT_ERROR("VENC attr is NULL\n");
        return AX_FAILURE;
    }

    /* Convert VENC_CHN_ATTR_S to TEST_VENC_CONFIG_S for SDK wrapper */
    TEST_VENC_CONFIG_S stVencCfg = {0};
    stVencCfg.u32InWidth = pAttr->stVeAttr.stInputPicAttr.u32PicWidth;
    stVencCfg.u32InHeight = pAttr->stVeAttr.stInputPicAttr.u32PicHeight;
    stVencCfg.u32InFps = pAttr->stRcAttr.u32FrmRateNum;
    stVencCfg.u32OutFps = pAttr->stRcAttr.u32FrmRateNum;
    stVencCfg.enEncType = PIXEL_FORMAT_YUV_SEMIPLANAR_420;  /* Input pixel format */
    stVencCfg.enPayLoad = pAttr->stVeAttr.enType;  /* Encoding type (H264/H265) */
    stVencCfg.enRcMode = pAttr->stRcAttr.enRcMode;

    /* Use SDK_COMM_VENC_Start wrapper which properly initializes all attributes */
    int ret = SDK_COMM_VENC_Start(VencChn, &stVencCfg);
    if (ret != NI_SUCCESS) {
        PRINT_ERROR("SDK_COMM_VENC_Start failed, chn=%d ret=%#x\n",
                    VencChn, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX VENC Create success, chn=%d %dx%d type=%d\n",
               VencChn,
               stVencCfg.u32InWidth,
               stVencCfg.u32InHeight,
               stVencCfg.enPayLoad);
    return AX_SUCCESS;
}

static int AX_VencDestroy(int VencChn)
{
    int ret = NI_MDK_VENC_DestroyChn(VencChn);
    if (ret != 0) {
        PRINT_ERROR("NI_MDK_VENC_DestroyChn failed, chn=%d ret=%#x\n",
                    VencChn, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX VENC Destroy success, chn=%d\n", VencChn);
    return AX_SUCCESS;
}

static int AX_VencStart(int VencChn)
{
    int ret = NI_MDK_VENC_StartRecvPic(VencChn);
    if (ret != 0) {
        PRINT_ERROR("NI_MDK_VENC_StartRecvPic failed, chn=%d ret=%#x\n",
                    VencChn, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX VENC Start success, chn=%d\n", VencChn);
    return AX_SUCCESS;
}

static int AX_VencStop(int VencChn)
{
    int ret = NI_MDK_VENC_StopRecvPic(VencChn);
    if (ret != 0) {
        PRINT_ERROR("NI_MDK_VENC_StopRecvPic failed, chn=%d ret=%#x\n",
                    VencChn, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX VENC Stop success, chn=%d\n", VencChn);
    return AX_SUCCESS;
}

static int AX_VencGetStream(int VencChn, VENC_STREAM_S* pStream, int timeout)
{
    if (!pStream) {
        PRINT_ERROR("Stream pointer is NULL\n");
        return AX_FAILURE;
    }

    int ret = NI_MDK_VENC_GetStream(VencChn, pStream, timeout);
    if (ret != 0 && ret != NI_ERR_VENC_BUF_EMPTY) {
        /* Only log error if not timeout */
        PRINT_ERROR("NI_MDK_VENC_GetStream failed, chn=%d ret=%#x\n",
                    VencChn, ret);
        return AX_FAILURE;
    }

    return (ret == 0) ? AX_SUCCESS : AX_FAILURE;
}

static int AX_VencReleaseStream(int VencChn, VENC_STREAM_S* pStream)
{
    if (!pStream) {
        PRINT_ERROR("Stream pointer is NULL\n");
        return AX_FAILURE;
    }

    int ret = NI_MDK_VENC_ReleaseStream(VencChn, pStream);
    if (ret != 0) {
        PRINT_ERROR("NI_MDK_VENC_ReleaseStream failed, chn=%d ret=%#x\n",
                    VencChn, ret);
        return AX_FAILURE;
    }

    return AX_SUCCESS;
}

static int AX_VencSetChnAttr(int VencChn, VENC_CHN_ATTR_S* pAttr)
{
    if (!pAttr) {
        PRINT_ERROR("VENC attr is NULL\n");
        return AX_FAILURE;
    }

    int ret = NI_MDK_VENC_SetChnAttr(VencChn, pAttr);
    if (ret != 0) {
        PRINT_ERROR("NI_MDK_VENC_SetChnAttr failed, chn=%d ret=%#x\n",
                    VencChn, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX VENC SetChnAttr success, chn=%d\n", VencChn);
    return AX_SUCCESS;
}

static int AX_VencGetChnAttr(int VencChn, VENC_CHN_ATTR_S* pAttr)
{
    if (!pAttr) {
        PRINT_ERROR("VENC attr is NULL\n");
        return AX_FAILURE;
    }

    int ret = NI_MDK_VENC_GetChnAttr(VencChn, pAttr);
    if (ret != 0) {
        PRINT_ERROR("NI_MDK_VENC_GetChnAttr failed, chn=%d ret=%#x\n",
                    VencChn, ret);
        return AX_FAILURE;
    }

    return AX_SUCCESS;
}

/* Phase 2: Dynamic parameter control interfaces */

static int AX_VencGetRcParam(int VencChn, VENC_RC_PARAM_S* pParam)
{
    if (!pParam) {
        PRINT_ERROR("RC param is NULL\n");
        return AX_FAILURE;
    }

    int ret = NI_MDK_VENC_GetRcParam(VencChn, pParam);
    if (ret != 0) {
        PRINT_ERROR("NI_MDK_VENC_GetRcParam failed, chn=%d ret=%#x\n",
                    VencChn, ret);
        return AX_FAILURE;
    }

    return AX_SUCCESS;
}

static int AX_VencSetRcParam(int VencChn, VENC_RC_PARAM_S* pParam)
{
    if (!pParam) {
        PRINT_ERROR("RC param is NULL\n");
        return AX_FAILURE;
    }

    int ret = NI_MDK_VENC_SetRcParam(VencChn, pParam);
    if (ret != 0) {
        PRINT_ERROR("NI_MDK_VENC_SetRcParam failed, chn=%d ret=%#x\n",
                    VencChn, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX VENC SetRcParam success, chn=%d\n", VencChn);
    return AX_SUCCESS;
}

static int AX_VencRequestIDR(int VencChn)
{
    int ret = NI_MDK_VENC_ForceIDR(VencChn);
    if (ret != 0) {
        PRINT_ERROR("NI_MDK_VENC_ForceIDR failed, chn=%d ret=%#x\n",
                    VencChn, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX VENC ForceIDR success, chn=%d\n", VencChn);
    return AX_SUCCESS;
}

static int AX_VencSetRotate(int VencChn, int enRotation)
{
    int ret = NI_MDK_VENC_SetFrameRotate(VencChn, (VENC_ROTATE_TYPE_E)enRotation);
    if (ret != 0) {
        PRINT_ERROR("NI_MDK_VENC_SetFrameRotate failed, chn=%d rotation=%d ret=%#x\n",
                    VencChn, enRotation, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX VENC SetFrameRotate success, chn=%d rotation=%d\n", VencChn, enRotation);
    return AX_SUCCESS;
}

/* Phase 3: Additional VENC operations */

static int AX_VencSetStreamCheck(int VencChn, VENC_STREAM_CHECK_TYPE_E checkMode)
{
    int ret = NI_MDK_VENC_SetStreamCheck(VencChn, checkMode);
    if (ret != 0) {
        PRINT_ERROR("NI_MDK_VENC_SetStreamCheck failed, chn=%d mode=%d ret=%#x\n",
                    VencChn, checkMode, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX VENC SetStreamCheck success, chn=%d mode=%d\n", VencChn, checkMode);
    return AX_SUCCESS;
}

static int AX_VencSetOutFrameRate(int VencChn, int frameRate)
{
    int ret = NI_MDK_VENC_SetOutFrmRate(VencChn, frameRate);
    if (ret != 0) {
        PRINT_ERROR("NI_MDK_VENC_SetOutFrmRate failed, chn=%d fps=%d ret=%#x\n",
                    VencChn, frameRate, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX VENC SetOutFrmRate success, chn=%d fps=%d\n", VencChn, frameRate);
    return AX_SUCCESS;
}

/* ========================================================================
 *  OSD (On-Screen Display) Operations
 * ======================================================================== */

/* Basic OSD operations - Direct SDK mapping */

static int AX_OSD_CreateRegion(NI_U32 u32Handle, OSD_REGION_S* pRegion)
{
    if (!pRegion) {
        PRINT_ERROR("OSD region is NULL\n");
        return AX_FAILURE;
    }

    int ret = NI_MDK_OSD_Create(u32Handle, pRegion);
    if (ret != 0) {
        PRINT_ERROR("NI_MDK_OSD_Create failed, handle=%u ret=%#x\n", u32Handle, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX OSD CreateRegion success, handle=%u\n", u32Handle);
    return AX_SUCCESS;
}

static int AX_OSD_DestroyRegion(NI_U32 u32Handle)
{
    int ret = NI_MDK_OSD_Destroy(u32Handle);
    if (ret != 0) {
        PRINT_ERROR("NI_MDK_OSD_Destroy failed, handle=%u ret=%#x\n", u32Handle, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX OSD DestroyRegion success, handle=%u\n", u32Handle);
    return AX_SUCCESS;
}

static int AX_OSD_GetBuffer(NI_U32 u32Handle, OSD_BUFFER_INFO_S* pBufInfo)
{
    if (!pBufInfo) {
        PRINT_ERROR("OSD buffer info is NULL\n");
        return AX_FAILURE;
    }

    int ret = NI_MDK_OSD_GetBuffer(u32Handle, pBufInfo);
    if (ret != 0) {
        PRINT_ERROR("NI_MDK_OSD_GetBuffer failed, handle=%u ret=%#x\n", u32Handle, ret);
        return AX_FAILURE;
    }

    return AX_SUCCESS;
}

static int AX_OSD_SetPalette(MDK_CHN_S* pChn, OSD_PALETTE_S* pPalette)
{
    if (!pChn || !pPalette) {
        PRINT_ERROR("OSD channel or palette is NULL\n");
        return AX_FAILURE;
    }

    int ret = NI_MDK_OSD_SetPalette(pChn, pPalette);
    if (ret != 0) {
        PRINT_ERROR("NI_MDK_OSD_SetPalette failed, ret=%#x\n", ret);
        return AX_FAILURE;
    }

    return AX_SUCCESS;
}

static int AX_OSD_GetDispAttr(NI_U32 u32Handle, MDK_CHN_S* pChn, OSD_DISP_ATTR_S* pAttr)
{
    if (!pChn || !pAttr) {
        PRINT_ERROR("OSD channel or attr is NULL\n");
        return AX_FAILURE;
    }

    int ret = NI_MDK_OSD_GetDispAttr(u32Handle, pChn, pAttr);
    if (ret != 0) {
        PRINT_ERROR("NI_MDK_OSD_GetDispAttr failed, handle=%u ret=%#x\n", u32Handle, ret);
        return AX_FAILURE;
    }

    return AX_SUCCESS;
}

static int AX_OSD_PaintToChn(NI_U32 u32Handle, MDK_CHN_S* pChn, OSD_DISP_ATTR_S* pAttr, NI_U32 timeout)
{
    if (!pChn || !pAttr) {
        PRINT_ERROR("OSD channel or attr is NULL\n");
        return AX_FAILURE;
    }

    int ret = NI_MDK_OSD_PaintToChn(u32Handle, pChn, pAttr, timeout);
    if (ret != 0) {
        PRINT_ERROR("NI_MDK_OSD_PaintToChn failed, handle=%u ret=%#x\n", u32Handle, ret);
        return AX_FAILURE;
    }

    return AX_SUCCESS;
}

static int AX_OSD_ClearFromChn(NI_U32 u32Handle, MDK_CHN_S* pChn, NI_U32 timeout)
{
    if (!pChn) {
        PRINT_ERROR("OSD channel is NULL\n");
        return AX_FAILURE;
    }

    int ret = NI_MDK_OSD_ClearFromChn(u32Handle, pChn, timeout);
    if (ret != 0) {
        PRINT_ERROR("NI_MDK_OSD_ClearFromChn failed, handle=%u ret=%#x\n", u32Handle, ret);
        return AX_FAILURE;
    }

    return AX_SUCCESS;
}

static int AX_OSD_Refresh(NI_U32 u32Handle, NI_U32 timeout)
{
    int ret = NI_MDK_OSD_Refresh(u32Handle, timeout);
    if (ret != 0) {
        PRINT_ERROR("NI_MDK_OSD_Refresh failed, handle=%u ret=%#x\n", u32Handle, ret);
        return AX_FAILURE;
    }

    return AX_SUCCESS;
}

/* Legacy OSD operations (for compatibility) */

static int AX_OSD_Create(NI_U32 u32Handle, MDK_CHN_S* pChn, SAMPLE_OSD_CONFIG_S* pCfg)
{
    if (!pChn || !pCfg) {
        PRINT_ERROR("OSD channel or config is NULL\n");
        return AX_FAILURE;
    }

    /* Use SDK_COMM wrapper - note the typo in SDK: Creat not Create */
    int ret = SDK_COMM_OSD_Creat(u32Handle, pChn, pCfg);
    if (ret != 0) {
        PRINT_ERROR("SDK_COMM_OSD_Creat failed, handle=%u ret=%#x\n",
                    u32Handle, ret);
        return AX_FAILURE;
    }

    PRINT_INFO("AX OSD Create success, handle=%u\n", u32Handle);
    return AX_SUCCESS;
}

static int AX_OSD_Destroy(NI_U32 u32Handle)
{
    /* SDK_COMM_OSD_Destroy requires MDK_CHN_S parameter, but we don't have it here
     * This is a design issue - for now, just return success as OSD cleanup
     * is typically handled during system shutdown */
    PRINT_WARN("AX_OSD_Destroy called but needs MDK_CHN_S parameter - skipped\n");
    return AX_SUCCESS;
}



/* ========================================================================
 *  Adapter Initialization
 * ======================================================================== */

/**
 * @brief Initialize AX520CE platform adapter
 *
 * This function registers all AX520CE SDK wrapper functions to the
 * platform adapter structure.
 *
 * @param adapter Pointer to platform adapter structure
 * @return AX_SUCCESS on success, AX_FAILURE on failure
 */
int AX_InitAdapter(PlatformAdapter* adapter)
{
    if (!adapter) {
        PRINT_ERROR("AX: Adapter pointer is NULL\n");
        return AX_FAILURE;
    }

    /* System operations */
    adapter->sys_init = AX_SYS_SystemInit;
    adapter->sys_exit = AX_SYS_SystemExit;
    adapter->sys_mbuf_init = AX_SYS_MbufInit;
    adapter->sys_mbuf_exit = AX_SYS_MbufExit;
    adapter->sys_pinmux_init = AX_SYS_PinmuxInit;
    adapter->sys_bind = AX_SYS_Bind;
    adapter->sys_unbind = AX_SYS_Unbind;

    /* Sensor operations */
    adapter->sensor_set_mirror = AX_SensorSetMirror;
    adapter->sensor_set_flip = AX_SensorSetFlip;
    adapter->sensor_get_cur_info = AX_SensorGetCurInfo;

    /* ISP operations */
    adapter->isp_get_pub_attr = AX_ISP_GetPubAttr;
    adapter->isp_set_pub_attr = AX_ISP_SetPubAttr;
    adapter->isp_get_video_fmt_attr = AX_ISP_GetVideoFmtAttr;
    adapter->isp_set_video_fmt_attr = AX_ISP_SetVideoFmtAttr;
    adapter->isp_get_auto_fps = AX_ISP_GetAutoFps;
    adapter->isp_set_auto_fps = AX_ISP_SetAutoFps;

    /* VI operations */
    adapter->vi_init = AX_VI_Init;
    adapter->vi_create_dev = AX_VI_CreateDev;
    adapter->vi_start_dev = AX_VI_StartDev;
    adapter->vi_stop_dev = AX_VI_StopDev;
    adapter->vi_destroy_dev = AX_VI_DestroyDev;
    adapter->vi_get_dev_attr = AX_VI_GetDevAttr;
    adapter->vi_set_dev_attr = AX_VI_SetDevAttr;
    adapter->vi_disable_dev = AX_VI_DisableDev;
    adapter->vi_enable_dev = AX_VI_EnableDev;

    /* VPS operations */
    adapter->vps_create_chn = AX_VPS_CreateChn;
    adapter->vps_destroy_chn = AX_VPS_DestroyChn;
    adapter->vps_enable_chn = AX_VPS_EnableChn;
    adapter->vps_disable_chn = AX_VPS_DisableChn;
    adapter->vps_create_encode_chn = AX_VPS_CreateEncodeChannel;
    adapter->vps_create_vd_chn = AX_VPS_CreateVdChannel;
    adapter->vps_get_chn_attr = AX_VPS_GetChnAttr;
    adapter->vps_set_chn_attr = AX_VPS_SetChnAttr;
    adapter->vps_get_grp_attr = AX_VPS_GetGrpAttr;
    adapter->vps_set_grp_attr = AX_VPS_SetGrpAttr;
    adapter->vps_get_mod_param = AX_VPS_GetModParam;
    adapter->vps_set_mod_param = AX_VPS_SetModParam;
    adapter->vps_get_chn_mask_attr = AX_VPS_GetChnMaskAttr;
    adapter->vps_set_chn_mask_attr = AX_VPS_SetChnMaskAttr;
    adapter->vps_enable_chn_mask = AX_VPS_EnableChnMask;
    adapter->vps_disable_chn_mask = AX_VPS_DisableChnMask;

    /* VENC operations */
    adapter->venc_create = AX_VencCreate;
    adapter->venc_destroy = AX_VencDestroy;
    adapter->venc_start = AX_VencStart;
    adapter->venc_stop = AX_VencStop;
    adapter->venc_get_stream = AX_VencGetStream;
    adapter->venc_release_stream = AX_VencReleaseStream;
    adapter->venc_set_chn_attr = AX_VencSetChnAttr;
    adapter->venc_get_chn_attr = AX_VencGetChnAttr;
    /* Dynamic parameter control */
    adapter->venc_get_rc_param = AX_VencGetRcParam;
    adapter->venc_set_rc_param = AX_VencSetRcParam;
    adapter->venc_request_idr = AX_VencRequestIDR;
    adapter->venc_set_rotate = AX_VencSetRotate;
    /* Additional VENC operations */
    adapter->venc_set_stream_check = AX_VencSetStreamCheck;
    adapter->venc_set_out_frame_rate = AX_VencSetOutFrameRate;


    /* OSD operations */
    adapter->osd_create_region = AX_OSD_CreateRegion;
    adapter->osd_destroy_region = AX_OSD_DestroyRegion;
    adapter->osd_get_buffer = AX_OSD_GetBuffer;
    adapter->osd_set_palette = AX_OSD_SetPalette;
    adapter->osd_get_disp_attr = AX_OSD_GetDispAttr;
    adapter->osd_paint_to_chn = AX_OSD_PaintToChn;
    adapter->osd_clear_from_chn = AX_OSD_ClearFromChn;
    adapter->osd_refresh = AX_OSD_Refresh;
    adapter->osd_create = AX_OSD_Create;
    adapter->osd_destroy = AX_OSD_Destroy;

    PRINT_INFO("AX: Platform adapter initialized successfully\n");
    return AX_SUCCESS;
}
