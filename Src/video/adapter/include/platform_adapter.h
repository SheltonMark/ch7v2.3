/**
 * @file platform_adapter.h
 * @brief Platform adapter interface for video pipeline abstraction
 *
 * This header defines the unified interface for different hardware platforms.
 * Each platform (AX, HI, etc.) implements these function pointers.
 */

#ifndef __PLATFORM_ADAPTER_H__
#define __PLATFORM_ADAPTER_H__

#include "sdk_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations for platform-specific types */
typedef struct _VI_CONFIG_S VI_CONFIG_S;
typedef struct _VENC_CONFIG_S VENC_CONFIG_S;

/* VPS configuration structure */
typedef struct _VPS_CONFIG_S {
    NI_U32 u32InWidth;
    NI_U32 u32InHeight;
    NI_U32 u32OutWidth;
    NI_U32 u32OutHeight;
    PIXEL_FORMAT_E enInPixFmt;
    PIXEL_FORMAT_E enOutPixFmt;
    NI_U32 u32FrameRate;  /* Frame rate */
} VPS_CONFIG_S;

/* Use SDK's VPS_VD_CHN_ATTR_S from ni_comm_vps.h - don't redefine */

/**
 * @brief Platform adapter structure containing function pointers
 *
 * All SDK calls are abstracted through this interface.
 */
typedef struct PlatformAdapter {
    /* ===== VI (Video Input) Operations ===== */
    int (*vi_init)(void);
    int (*vi_create_dev)(int ViDev, SAMPLE_VI_INFO_S* pstViInfo);
    int (*vi_start_dev)(int ViDev);
    int (*vi_stop_dev)(int ViDev);
    int (*vi_destroy_dev)(int ViDev);
    int (*vi_get_dev_attr)(int ViDev, VI_DEV_ATTR_S* pAttr);  /* Get VI device attributes */
    int (*vi_set_dev_attr)(int ViDev, VI_DEV_ATTR_S* pAttr);  /* Set VI device attributes */
    int (*vi_disable_dev)(int ViDev);  /* Disable VI device */
    int (*vi_enable_dev)(int ViDev);  /* Enable VI device */

    /* ===== Sensor Operations ===== */
    int (*sensor_set_mirror)(int SenId, int bMirror);  /* Set sensor mirror */
    int (*sensor_set_flip)(int SenId, int bFlip);  /* Set sensor flip */

    /* ===== VPS (Video Processing/Scaling) Operations ===== */
    int (*vps_create_grp)(int VpsGrp, void* pGrpAttr);
    int (*vps_destroy_grp)(int VpsGrp);
    int (*vps_create_chn)(int VpsGrp, int VpsChn, VPS_CONFIG_S* pVpsAttr);
    int (*vps_destroy_chn)(int VpsGrp, int VpsChn);
    int (*vps_enable_chn)(int VpsGrp, int VpsChn);
    int (*vps_disable_chn)(int VpsGrp, int VpsChn);
    int (*vps_create_vd_chn)(int VpsGrp, VPS_VD_CHN_ATTR_S* pVdAttr);  /* For VD (motion detection) channel */
    int (*vps_create_encode_chn)(int VpsGrp, int VpsChn, VPS_CONFIG_S* pConfig);  /* For encode channels - preferred */
    int (*vps_get_chn_attr)(int VpsGrp, int VpsChn, VPS_CHN_ATTR_S* pAttr);  /* Get VPS channel attributes */
    int (*vps_set_chn_attr)(int VpsGrp, int VpsChn, VPS_CHN_ATTR_S* pAttr);  /* Set VPS channel attributes */

    /* ===== VENC (Video Encoding) Operations ===== */
    /* Basic operations (Phase 1) */
    int (*venc_create)(int VencChn, VENC_CHN_ATTR_S* pAttr);
    int (*venc_destroy)(int VencChn);
    int (*venc_start)(int VencChn);
    int (*venc_stop)(int VencChn);
    int (*venc_get_stream)(int VencChn, VENC_STREAM_S* pStream, int timeout);
    int (*venc_release_stream)(int VencChn, VENC_STREAM_S* pStream);

    /* Dynamic parameter operations (Phase 2) */
    int (*venc_set_chn_attr)(int VencChn, VENC_CHN_ATTR_S* pAttr);
    int (*venc_get_chn_attr)(int VencChn, VENC_CHN_ATTR_S* pAttr);
    int (*venc_get_rc_param)(int VencChn, VENC_RC_PARAM_S* pParam);
    int (*venc_set_rc_param)(int VencChn, VENC_RC_PARAM_S* pParam);
    int (*venc_request_idr)(int VencChn);
    int (*venc_set_rotate)(int VencChn, int enRotation);  /* Rotation angle: 0/90/180/270 */

    /* Additional VENC operations (Phase 3) */
    int (*venc_set_stream_check)(int VencChn, int checkMode);  /* Stream check mode */
    int (*venc_set_out_frame_rate)(int VencChn, int frameRate);  /* Output frame rate */

    /* ===== SYS (System Binding) Operations ===== */
    int (*sys_bind)(MDK_CHN_S* pSrcChn, MDK_CHN_S* pDestChn);
    int (*sys_unbind)(MDK_CHN_S* pSrcChn, MDK_CHN_S* pDestChn);

    /* ===== OSD (On-Screen Display) Operations ===== */
    int (*osd_create)(NI_U32 u32Handle, MDK_CHN_S* pChn, SAMPLE_OSD_CONFIG_S* pCfg);
    int (*osd_destroy)(NI_U32 u32Handle);
    int (*osd_update)(NI_U32 u32Handle, void* pData, int len);

} PlatformAdapter;

/**
 * @brief Get the platform adapter instance
 * @return Pointer to the platform adapter, or NULL if not initialized
 */
PlatformAdapter* GetPlatformAdapter(void);

/**
 * @brief Initialize platform adapter (called by adapter_factory.c)
 * @param adapter Pointer to adapter structure to initialize
 * @return 0 on success, -1 on failure
 */
int InitPlatformAdapter(PlatformAdapter* adapter);

#ifdef __cplusplus
}
#endif

#endif /* __PLATFORM_ADAPTER_H__ */
