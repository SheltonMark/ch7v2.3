/**
 * @file video_osd.c
 * @brief Video overlay (OSD) implementation using platform adapter
 */

#include "video_osd.h"
#include "platform_adapter.h"
#include "TdCommon.h"
#include "sdk_comm.h"

/**
 * @brief Initialize video overlay (OSD)
 */
int VideoOverlay_Init(void)
{
    PlatformAdapter* adapter = GetPlatformAdapter();
    if (!adapter || !adapter->osd_create) {
        PRINT_ERROR("OSD adapter not available\n");
        return -1;
    }

    PRINT_INFO("===== Starting VideoOverlay_Init =====\n");

    /* OSD configuration */
    MDK_CHN_S stMdkChn;
    SAMPLE_OSD_CONFIG_S stOsdCfg;
    NI_U32 u32OsdHandle = 0;

    stMdkChn.enModId = NI_ID_VPS;
    stMdkChn.s32DevId = 0;
    stMdkChn.s32ChnId = 0;  /* Attach to first VPS channel */

    stOsdCfg.stStartPiont.s32X = 0;
    stOsdCfg.stStartPiont.s32Y = 0;
    stOsdCfg.u32Layer = 0;
    stOsdCfg.bSpecialFrmId = NI_FALSE;
    stOsdCfg.u32TargetFrameId = 0;

    /* Create OSD via adapter */
    if (adapter->osd_create(u32OsdHandle, &stMdkChn, &stOsdCfg) != 0) {
        PRINT_WARN("OSD create failed (non-fatal)\n");
        return -1;  /* OSD failure is non-fatal */
    }

    PRINT_INFO("===== VideoOverlay_Init SUCCESS =====\n");
    return 0;
}

/**
 * @brief Destroy video overlay (OSD)
 */
int VideoOverlay_Destroy(void)
{
    PlatformAdapter* adapter = GetPlatformAdapter();
    if (!adapter || !adapter->osd_destroy) {
        PRINT_ERROR("OSD adapter not available\n");
        return -1;
    }

    NI_U32 u32OsdHandle = 0;

    if (adapter->osd_destroy(u32OsdHandle) != 0) {
        PRINT_WARN("OSD destroy failed\n");
        return -1;
    }

    PRINT_INFO("VideoOverlay_Destroy SUCCESS\n");
    return 0;
}
