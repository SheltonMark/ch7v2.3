/**
 * @file video_pipeline.c
 * @brief Video pipeline implementation using platform adapter
 *
 * This file implements the high-level video pipeline initialization,
 * replacing the original video_in_init() function.
 */

#include "video_pipeline.h"
#include "platform_adapter.h"
#include "TdCommon.h"
#include "sdk_comm.h"
#include "Capture.h"
#include "Isp.h"

/* External GlobalDevice */
extern GlobalDevice_t GlobalDevice;

/**
 * @brief Initialize complete video pipeline
 */
int VideoPipeline_Init(void)
{
    PlatformAdapter* adapter = GetPlatformAdapter();
    if (!adapter) {
        PRINT_ERROR("Get adapter failed\n");
        return -1;
    }

    SensorDevice_p pSensorDevice = &GlobalDevice.SensorDevice;
    VideoInDevice_p pVideoInDevice = &GlobalDevice.VideoInDevice;
    CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;

    PRINT_INFO("===== Starting VideoPipeline_Init =====\n");

    /* ===== Step 1: Construct VI configuration =====  */
    SAMPLE_VI_INFO_S stViInfo = {0};
    stViInfo.enViMode = pVideoInDevice->ViMode;
    stViInfo.stSenCfg.enSenType = pSensorDevice->SenType;
    stViInfo.stSenCfg.enIntfType = VI_INTF_TYPE_MIPI;
    stViInfo.stSenCfg.enHdrMode = HDR_MODE_NONE;
    stViInfo.stSenCfg.u32Width = pVideoInDevice->vi_pic.u32Width;
    stViInfo.stSenCfg.u32Height = pVideoInDevice->vi_pic.u32Height;
    stViInfo.stSenCfg.u32Fps = pVideoInDevice->VDInfo[0].ViDstFps;
    stViInfo.stSenCfg.u32LaneNum = 2;
    stViInfo.stViCfg.enViWorkMode = VI_WORK_MODE_NORMAL;
    stViInfo.stSenCfg.u32I2cDevId = 0;
    stViInfo.stSenCfg.u32RstGpioId = SENSOR_RESET;

    /* Set sensor-specific parameters */
    switch (stViInfo.stSenCfg.enSenType)
    {
        case SEN_TYPE_GC4023:
            stViInfo.stSenCfg.u32DevAddr = 0x52;
            stViInfo.stSenCfg.u32ClkFreq = 27000;
            break;
        case SEN_TYPE_OS04L10:
        case SEN_TYPE_OS04D10:
            stViInfo.stSenCfg.u32DevAddr = 0x78;
            stViInfo.stSenCfg.u32ClkFreq = 24000;
            break;
        case SEN_TYPE_GC2083:
            stViInfo.stSenCfg.u32DevAddr = 0x6e;
            stViInfo.stSenCfg.u32ClkFreq = 27000;
            break;
        case SEN_TYPE_GC4653:
            stViInfo.stSenCfg.u32DevAddr = 0x52;
            stViInfo.stSenCfg.u32ClkFreq = 24000;
            break;
        default:
            PRINT_ERROR("Unsupported sensor type: %d\n", stViInfo.stSenCfg.enSenType);
            return -1;
    }

    PRINT_INFO("VI Config: %dx%d %dfps sensor=%d\n",
               stViInfo.stSenCfg.u32Width,
               stViInfo.stSenCfg.u32Height,
               stViInfo.stSenCfg.u32Fps,
               stViInfo.stSenCfg.enSenType);

    /* ===== Step 2: Initialize VI via adapter ===== */
    if (adapter->vi_init && adapter->vi_init() != 0) {
        PRINT_ERROR("VI init failed\n");
        return -1;
    }

    if (adapter->vi_create_dev(0, &stViInfo) != 0) {
        PRINT_ERROR("VI create dev failed\n");
        return -1;
    }

    if (adapter->vi_start_dev(0) != 0) {
        PRINT_ERROR("VI start dev failed\n");
        return -1;
    }

    PRINT_INFO("VI initialized and started\n");

    /* ===== Step 3: Initialize ISP (keep isp.c unchanged) ===== */
    if (isp_init(stViInfo) != 0) {
        PRINT_ERROR("ISP init failed\n");
        return -1;
    }

    PRINT_INFO("ISP initialized\n");

    /* ===== Step 4-6: Create video encoding pipeline for each channel =====
     * For each channel, we:
     *   1. Create VPS channel (scaling/processing)
     *   2. Bind VI→VPS (video input to scaler)
     *   3. Create VENC channel (encoder)
     *
     * Note: VPS→VENC binding is done automatically by SDK_COMM_VPS_Start()
     * inside AX_VPS_CreateEncodeChannel(), so only VI→VPS needs manual binding.
     */
    int EncCnt = pCaptureDevice->EncCount;
    for (int i = 0; i < EncCnt; i++) {
        int StreamCount = pCaptureDevice->EncDevice[i].StreamCount;

        for (int venc_channel = 0; venc_channel < StreamCount; venc_channel++) {
            struct channel_info* info = &pCaptureDevice->EncDevice[i].StreamDevice[venc_channel].EncChannel_info;

            /* All channels use VpsGrp=0, only VpsChn differs (AX520CE limitation) */
            int VpsGrp = 0;
            int VpsChn = venc_channel;

            /* Create VPS group once for all channels (platform-specific) */
            if (venc_channel == 0 && adapter->vps_create_grp) {
                int ret = adapter->vps_create_grp(VpsGrp, NULL);
                if (ret != 0) {
                    PRINT_WARN("VPS create group failed (may be optional on some platforms)\n");
                }
            }

            /* Step 4.1: Create VPS channel for this encoder */
            VPS_CONFIG_S vpsConfig = {0};
            vpsConfig.u32InWidth = pSensorDevice->MaxWidth;
            vpsConfig.u32InHeight = pSensorDevice->MaxHeight;
            vpsConfig.u32OutWidth = info->max_width;
            vpsConfig.u32OutHeight = info->max_height;
            vpsConfig.enInPixFmt = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
            vpsConfig.enOutPixFmt = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
            vpsConfig.u32FrameRate = info->frame_count;

            if (adapter->vps_create_encode_chn(VpsGrp, VpsChn, &vpsConfig) != 0) {
                PRINT_ERROR("VPS create encode channel failed, grp=%d chn=%d\n", VpsGrp, VpsChn);
                return -1;
            }
            PRINT_INFO("✓ VPS channel created: grp=%d chn=%d %dx%d\n",
                       VpsGrp, VpsChn, vpsConfig.u32OutWidth, vpsConfig.u32OutHeight);

            /* Step 5.1: Bind VI→VPS (must be before VENC creation) */
            MDK_CHN_S srcChn = {NI_ID_VI, 0, 0};
            MDK_CHN_S destChn = {NI_ID_VPS, VpsGrp, VpsChn};

            if (adapter->sys_bind(&srcChn, &destChn) != 0) {
                PRINT_ERROR("Bind VI->VPS failed, chn=%d\n", venc_channel);
                return -1;
            }
            PRINT_INFO("✓ VI->VPS bound: VI[0] -> VPS[%d,%d]\n", VpsGrp, VpsChn);

            /* Step 6.1: Create VENC channel */
            VENC_CHN_ATTR_S vencAttr = {0};
            vencAttr.stVeAttr.enType = info->enc_type;
            vencAttr.stVeAttr.enProfile = 0;  /* Main profile */
            vencAttr.stVeAttr.stInputPicAttr.u32PicWidth = info->max_width;
            vencAttr.stVeAttr.stInputPicAttr.u32PicHeight = info->max_height;
            vencAttr.stVeAttr.stInputPicAttr.au32Stride[0] = info->max_width;
            vencAttr.stVeAttr.stInputPicAttr.au32Stride[1] = info->max_width;
            vencAttr.stVeAttr.stInputPicAttr.au32Stride[2] = 0;
            vencAttr.stRcAttr.enRcMode = VENC_RC_MODE_CBR;
            vencAttr.stRcAttr.u32FrmRateNum = info->frame_count;
            vencAttr.stRcAttr.u32FrmRateDenom = 1;
            vencAttr.stGopAttr.stGopNomalAttr.u32Gop = info->frame_count * 2;
            vencAttr.stRcAttr.stAttrCbr.u32StatTime = 1;
            vencAttr.stRcAttr.stAttrCbr.u32TargetBitRate = info->bps;

            if (adapter->venc_create(venc_channel, &vencAttr) != 0) {
                PRINT_ERROR("VENC create failed, chn=%d\n", venc_channel);
                return -1;
            }

            if (adapter->venc_start(venc_channel) != 0) {
                PRINT_ERROR("VENC start failed, chn=%d\n", venc_channel);
                return -1;
            }
            PRINT_INFO("✓ VENC channel created: chn=%d %dx%d %dkbps %dfps type=%d\n",
                       venc_channel, info->max_width, info->max_height,
                       info->bps, info->frame_count, info->enc_type);
        }
    }

    /* ===== Step 7: Create VD channel for motion detection (optional) ===== */
    if (adapter->vps_create_vd_chn) {
        VPS_CHN_ATTR_S vdAttr = {0};
        vdAttr.bCropEn = NI_FALSE;
        vdAttr.stCropRect.s32X = 0;
        vdAttr.stCropRect.s32Y = 0;
        vdAttr.stCropRect.u32Width = pSensorDevice->MaxWidth;
        vdAttr.stCropRect.u32Height = pSensorDevice->MaxHeight;
        vdAttr.enAttrType = VPS_CHN_ATTR_TYPE_VD;
        vdAttr.u32OutWidth = 704;
        vdAttr.u32OutHeight = 576;
        vdAttr.stVdAttr.enFilterWin = VPS_FILTER_WIN_TYPE_LEVEL_3;
        vdAttr.stVdAttr.u32BuffNum = 3;
        vdAttr.stVdAttr.u32OutFps = pVideoInDevice->VDInfo[0].ViDstFps;
        vdAttr.stVdAttr.u32Stride = 704;

        int ret = adapter->vps_create_vd_chn(0, (VPS_VD_CHN_ATTR_S*)&vdAttr);
        if (ret != 0) {
            PRINT_WARN("VD channel create failed (non-fatal) ret=%d\n", ret);
        } else {
            PRINT_INFO("VD channel created successfully\n");
        }
    }

    PRINT_INFO("===== VideoPipeline_Init SUCCESS =====\n");
    return 0;
}

/**
 * @brief Destroy video pipeline
 */
int VideoPipeline_Destroy(void)
{
    PlatformAdapter* adapter = GetPlatformAdapter();
    if (!adapter) {
        return -1;
    }

    CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
    int EncCnt = pCaptureDevice->EncCount;

    PRINT_INFO("===== Starting VideoPipeline_Destroy =====\n");

    /* Reverse order: unbind first, then destroy channels */

    /* 1. Unbind VPS→VENC */
    for (int i = 0; i < EncCnt; i++) {
        int StreamCount = pCaptureDevice->EncDevice[i].StreamCount;

        for (int venc_channel = 0; venc_channel < StreamCount; venc_channel++) {
            MDK_CHN_S srcChn = {NI_ID_VPS, 0, venc_channel};  /* VpsGrp=0 */
            MDK_CHN_S destChn = {NI_ID_VENC, 0, venc_channel};
            adapter->sys_unbind(&srcChn, &destChn);
        }
    }

    /* 2. Stop and destroy VENC */
    for (int i = 0; i < EncCnt; i++) {
        int StreamCount = pCaptureDevice->EncDevice[i].StreamCount;

        for (int venc_channel = 0; venc_channel < StreamCount; venc_channel++) {
            adapter->venc_stop(venc_channel);
            adapter->venc_destroy(venc_channel);
        }
    }

    /* 3. Unbind VI→VPS */
    for (int i = 0; i < EncCnt; i++) {
        int StreamCount = pCaptureDevice->EncDevice[i].StreamCount;

        for (int venc_channel = 0; venc_channel < StreamCount; venc_channel++) {
            MDK_CHN_S srcChn = {NI_ID_VI, 0, 0};
            MDK_CHN_S destChn = {NI_ID_VPS, 0, venc_channel};  /* VpsGrp=0 */
            adapter->sys_unbind(&srcChn, &destChn);
        }
    }

    /* 4. Destroy VPS channels */
    for (int i = 0; i < EncCnt; i++) {
        int StreamCount = pCaptureDevice->EncDevice[i].StreamCount;

        for (int venc_channel = 0; venc_channel < StreamCount; venc_channel++) {
            adapter->vps_disable_chn(0, venc_channel);  /* VpsGrp=0, VpsChn=venc_channel */
            adapter->vps_destroy_chn(0, venc_channel);
        }
    }

    /* 5. Stop VI */
    adapter->vi_stop_dev(0);

    PRINT_INFO("===== VideoPipeline_Destroy SUCCESS =====\n");
    return 0;
}
