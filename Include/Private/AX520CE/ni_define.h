/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef __NI_DEFINE_H__
#define __NI_DEFINE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define AX520_V100 0x5200100

#define XS7312_V100 0x73120100
#define XS7156_V100 0x71560100

#ifndef XSCHIP
#define XSCHIP AX520_V100
#endif

#if AX520_V100 == XSCHIP
#define CHIP_NAME "AX520"
#else
#error XSCHIP define may be error
#endif

#define MDK_VER_PRIX "_MDK_V"

/**********************************SOC************************************************/
#define CHIP_CPU_NUM             (2)
#define GPIO_BANK_NUM            (2)
#define GPIO_MAX_NUM             (44)
#define GPIO_MAX_NUM_OF_PER_BANK (32)
#define GPIO_MAX_INT_NUM         (44)
#define ADC_CHANNEL_NUM          (2)
#define PWM_MAX_DEV              (13)
#define CHIP_REG_CPU_BUFF        (0x09000040)

/*********************************VI************************************************/
#define VI_MAX_DEV_NUM            (2)
#define VI_MAX_DATA_LINE          (16)
#define VI_MAX_HDR_NUM            (2)
#define VI_GET_FRAME_MAX_DEPTH    (20)
#define VI_ENC_MAX_BANK_NUM       (4)
#define VI_ENC_CHANEL_NUM         (4)
#define VI_DEC_CHANEL_NUM         (2)
#define VI_DEC_SEL_NUM            (2)
#define VI_SEND_RAW_FRAME_MAX_NUM (256)
#define VI_MAX_WIDTH              (3072)
#define VI_MAX_HEIGHT             (2160)
#define VI_MAX_INTF_NUM           (2)

/*********************************ISP************************************************/
#define ISP_MAX_DEV_NUM              (2)
#define ISP_AWES_MAX_ZONE_NUM        (2048)
#define ISP_RAW_HIST_MAX_ZONE_NUM    (2048)
#define ISP_GIC_MAX_ID_NUM           (1)
#define ISP_BLC_MAX_ID_NUM           (3)
#define ISP_DGC_MAX_ID_NUM           (3)
#define ISP_WB_MAX_ID_NUM            (3)
#define ISP_LSC_MAX_ID_NUM           (1)
#define ISP_LSC_MAX_TABLE_LEN        (192)
#define ISP_LWDR_FI_MAX_TABLE_LEN    (256)
#define ISP_LWDR_ALPHA_MAX_TABLE_LEN (256)
#define ISP_GAMMA_MAX_TABLE_LEN      (256)
#define ISP_CE_MAX_TABLE_LEN         (256)
#define ISP_YHIST_MAX_TABLE_LEN      (256)
#define ISP_YHIST_STAT_DATA_NUM      (256)
#define ISP_RAWHIST_STAT_DATA_NUM    (256)
#define ISP_AF_STAT_DATA_NUM         (1024)
#define ISP_MAX_WIDTH                (3072)
#define ISP_MAX_HEIGHT               (1944)
#define ISP_AWES_MAX_FRM             (3)
#define ISP_RAWHIST_MAX_FRM          (3)

/* ********************************VPS***********************************************/
#define VPS_MAX_GRP_NUM          (2)
#define VPS_GRP_MAX_CHN_NUM      (10)
#define VPS_GRP_ENC_NUM          (3)
#define VPS_GRP_VIR_ENC_NUM      (1)
#define VPS_GRP_IAS_NUM          (1)
#define VPS_GRP_VIR_IAS_NUM      (2)
#define VPS_GRP_VD_NUM           (1)
#define VPS_VD_MAX_DETECT_REGION (4)
#define VPS_MIN_INPUT_WIDTH      (176)
#define VPS_MIN_INPUT_HEIGHT     (144)
#define VPS_MAX_INPUT_WIDTH      (3072)
#define VPS_MAX_INPUT_HEIGHT     (1944)
#define VPS_MIN_OUTPUT_WIDTH     (32)
#define VPS_MIN_OUTPUT_HEIGHT    (32)
#define VPS_MAX_INPUT_FPS        (120)
#define VPS_MIN_INPUT_FPS        (1)
#define VPS_OSD_PALETTE_NUM      (16)
#define VPS_OSD_LAYER_NUM        (3)
#define VPS_OSD_NUM_PER_LAYER    (16)
#define VPS_MAX_CHN_MASK_NUM     (8)
#define VPS_MAX_BANK_NUM         (4)
#define VPS_ENC0_IMG_MAX_WIDTH   (3072)
#define VPS_ENC0_IMG_MAX_HEIGHT  (1944)
#define VPS_ENC1_IMG_MAX_WIDTH   (800)
#define VPS_ENC1_IMG_MAX_HEIGHT  (576)
#define VPS_ENC2_IMG_MAX_WIDTH   (1920)
#define VPS_ENC2_IMG_MAX_HEIGHT  (1080)
#define VPS_IAS_IMG_MAX_WIDTH    (1024)
#define VPS_IAS_IMG_MAX_HEIGHT   (1024)
#define VPS_VD_IMG_MAX_WIDTH     (1280)
#define VPS_VD_IMG_MAX_HEIGHT    (720)

/********************************VENC************************************************/
#define VENC_MAX_CHN_NUM        (8)
#define VENC_MAX_H265_WIDTH     (3072) /* AX513CE (2304) */
#define VENC_MAX_H265_HEIGHT    (3072) /* AX513CE (2304) */
#define VENC_MIN_H265_WIDTH     (16)
#define VENC_MIN_H265_HEIGHT    (16)
#define VENC_MAX_H264_WIDTH     (3072) /* AX513CE (2304) */
#define VENC_MAX_H264_HEIGHT    (3072) /* AX513CE (2304) */
#define VENC_MIN_H264_WIDTH     (16)
#define VENC_MIN_H264_HEIGHT    (16)
#define VENC_MAX_JPEG_WIDTH     (3072) /* AX513CE (2304) */
#define VENC_MAX_JPEG_HEIGHT    (3072) /* AX513CE (2304) */
#define VENC_MIN_JPEG_WIDTH     (16)
#define VENC_MIN_JPEG_HEIGHT    (16)
#define VENC_MAX_ROI_NUM        (8)
#define VENC_MIN_USER_DATA_SIZE (16)
#define VENC_MAX_USER_DATA_SIZE (256 - 35 + 16) /* From Vendor */
#define JPEG_MIN_HEAD_SIZE      (608)           /* 最小608byte码流头 */

/*******************************AUDIO************************************************/
#define AENC_MAX_CHN_NUM (4)
#define AENC_GEN_CHN_NUM (2)
#define ADEC_MAX_CHN_NUM (2)
#define AGC_MAX_CHN_NUM  (AENC_MAX_CHN_NUM + ADEC_MAX_CHN_NUM)
#define ANS_MAX_CHN_NUM  (AGC_MAX_CHN_NUM)
#define AEC_MAX_CHN_NUM  (1)

/*********************************OSD************************************************/
#define OSD_MAX_HANDLE_NUM         (128)
#define OSD_MAX_SUPPORT_MOD_NUM    (1)
#define OSD_EX_MAX_SUPPORT_MOD_NUM (2)
#define OSD_MAX_GRP_NUM            (2)
#define OSD_MAX_CHN_NUM            (3)
#define OSD_EX_MAX_CHN_NUM         (10) /* OSD_EX中最多八个通道支持，其中vps支持10个通道，venc支持8个通道 */
#define OSD_MAX_PALETTE_NUM        (16)
#define OSD_MAX_LAYER_NUM          (3)
#define OSD_EX_MAX_LAYER_NUM       (1)
#define OSD_PER_LAYER_NUM          (16)
#define OSD_EX_MAX_LAYER_PRI       (16) /* OSD_EX最大层优先级序号 */
#define OSD_YUV_PER_LAYER_NUM      (4)
#define OSD_ALPHA_LEVEL            (32)
#define OSD_MAX_LATTICE_LAYER_NUM  (2)
#define OSD_EX_MAX_NODE_NUM        (32) /* OSD_EX 帧信息队列最大队列深度 */
//tenda_start
#define TD_OSD_MAX_NUM             (4)
/* 定义vps一个group中支持mask的通道个数 */
#define VPS_MASK_MAX_CHN_NUM    (4)
/*最大支持的动检的通道数*/
#define VD_MAX_CHN_NUM         (1)
//tenda_end

/*********************************MBUF***********************************************/
#define MBUF_MAX_POOLS      (128)
#define MBUF_MAX_COMM_POOLS (16)
#define MBUF_MAX_PRIV_POOLS (MBUF_MAX_POOLS - MBUF_MAX_COMM_POOLS)

/*********************************mbuf**********************************************/
#define MBUF_MAX_POOLS      (128)
#define MBUF_MAX_COMM_POOLS (16)
#define MBUF_MAX_PRIV_POOLS (MBUF_MAX_POOLS - MBUF_MAX_COMM_POOLS)

/*********************************ISP_SEN_SYNC**************************************/
#define SEN_MAX_REGS_NUM      (32)
#define SEN_MAX_I2C_GROUP_NUM (16)
#define SEN_MAX_I2C_REGS_NUM  (6)
#define SEN_I2C_MAX_DELAY     (2)
#define SEN_I2C_SEND_NUM_MAX  (128)

/*********************************AISP************************************************/
#define AISP_MAX_DB_NUM (16)

/*********************************DVP************************************************/
#define DVP_MAX_WIDTH                  (3072)
#define DVP_MAX_HEIGHT                 (3072)
#define DVP_MIN_WIDTH                  (16)
#define DVP_MIN_HEIGHT                 (16)
#define DVP_STRIDE_ALIGN               (8)
#define DVP_PHYADDR_ALIGN              (8)
#define DVP_MAX_PALETTE_NUM            (16)
#define DVP_MAX_CMD_NUM                (32)
#define DVP_MAX_MASK_NUM               (8)
#define DVP_MAX_YUV_OSD_NUM            (8)
#define DVP_MAX_OSD_ALPHA_LEVEL        (32)
#define DVP_MAX_OSD_LAYER_NUM          (1)
#define DVP_MAX_LATTICE_OSD_LAYER_NUM  (1)
#define DVP_MAX_LATTICE_OSD_PERLAY_NUM (4)
#define DVP_MAX_LATTICE_OSD_NUM        (DVP_MAX_LATTICE_OSD_LAYER_NUM * DVP_MAX_LATTICE_OSD_PERLAY_NUM)
#define DVP_MAX_SCALE_LEVEL_Y          (3)
#define DVP_MAX_SCALE_LEVEL_UV         (1)

/********************************CLOCK***********************************************/
#define CLOCK_MAX_PLL_NUM         (6)
#define CLOCK_SENSOR_VCLK_NUM     (2)
#define CLOCK_DDR_PORT_NUM        (6)
#define CLOCK_SDIO_MAX_NUM        (2)
#define CLOCK_DDR_PLL_MAX_FREQ    (466000)
#define CLOCK_AHB_MIN_FREQ        (32258)
#define CLOCK_AHB_MAX_FREQ        (200000)
#define CLOCK_APB_MAX_FREQ        (50000)
#define CLOCK_VCLK_MIN_FREQ       (15873)
#define CLOCK_VCLK_MAX_FREQ       (100000)
#define CLOCK_VCLK_DEFAULT_FREQ   (37125)
#define CLOCK_VIU_MIN_FREQ        (15873)
#define CLOCK_VIU_MAX_FREQ        (396000)
#define CLOCK_VIU_DEFAULT_FREQ    (396000)
#define CLOCK_ISP_MIN_FREQ        (32258)
#define CLOCK_ISP_MAX_FREQ        (200000) /* AX513CE (100000) */
#define CLOCK_ISP_DEFAULT_FREQ    (200000) /* AX513CE (100000) */
#define CLOCK_VPS_MIN_FREQ        (32258)
#define CLOCK_VPS_MAX_FREQ        (396000)
#define CLOCK_VPS_DEFAULT_FREQ    (396000)
#define CLOCK_EDP_MIN_FREQ        (32258)
#define CLOCK_EDP_MAX_FREQ        (396000)
#define CLOCK_EDP_DEFAULT_FREQ    (396000)
#define CLOCK_DVP_MIN_FREQ        (37125)
#define CLOCK_DVP_MAX_FREQ        (594000)
#define CLOCK_DVP_DEFAULT_FREQ    (100000)
#define CLOCK_IAS_MIN_FREQ        (32258)
#define CLOCK_IAS_MAX_FREQ        (594000)
#define CLOCK_IAS_DEFAULT_FREQ    (200000)
#define CLOCK_IVE_MIN_FREQ        (-1)
#define CLOCK_IVE_MAX_FREQ        (-1)
#define CLOCK_IVE_DEFAULT_FREQ    (-1)
#define CLOCK_BT1120_MIN_FREQ     (-1)
#define CLOCK_BT1120_MAX_FREQ     (-1)
#define CLOCK_BT1120_DEFAULT_FREQ (-1)
#define CLOCK_LCD_MAIN_MIN_FREQ   (-1)
#define CLOCK_LCD_SYS_MIN_FREQ    (-1)
#define CLOCK_LCD_MAX_FREQ        (-1)
#define CLOCK_LCD_DEFAULT_FREQ    (-1)
#define CLOCK_JPEG_MIN_FREQ       (38322)
#define CLOCK_JPEG_MAX_FREQ       (500000)
#define CLOCK_JPEG_DEFAULT_FREQ   (50000)

#define CLOCK_DDR2P1_MIN_FREQ     (142857)
#define CLOCK_DDR2P1_MAX_FREQ     (333000)
#define CLOCK_DDR2P1_DEFAULT_FREQ (266000)
#define CLOCK_DDR2P2_MIN_FREQ     (142857)
#define CLOCK_DDR2P2_MAX_FREQ     (333000)
#define CLOCK_DDR2P2_DEFAULT_FREQ (266000)
#define CLOCK_DDR2P3_MIN_FREQ     (142857)
#define CLOCK_DDR2P3_MAX_FREQ     (333000)
#define CLOCK_DDR2P3_DEFAULT_FREQ (266000)
#define CLOCK_DDR2P4_MIN_FREQ     (-1)
#define CLOCK_DDR2P4_MAX_FREQ     (-1)
#define CLOCK_DDR2P4_DEFAULT_FREQ (-1)

#define CLOCK_DDR3P1_MIN_FREQ     (142857)
#define CLOCK_DDR3P1_MAX_FREQ     (466000)
#define CLOCK_DDR3P1_DEFAULT_FREQ (396000)
#define CLOCK_DDR3P2_MIN_FREQ     (142857)
#define CLOCK_DDR3P2_MAX_FREQ     (466000)
#define CLOCK_DDR3P2_DEFAULT_FREQ (396000)
#define CLOCK_DDR3P3_MIN_FREQ     (142857)
#define CLOCK_DDR3P3_MAX_FREQ     (466000)
#define CLOCK_DDR3P3_DEFAULT_FREQ (396000)
#define CLOCK_DDR3P4_MIN_FREQ     (-1)
#define CLOCK_DDR3P4_MAX_FREQ     (-1)
#define CLOCK_DDR3P4_DEFAULT_FREQ (-1)

#define CLOCK_SDIO_MAX_FREQ        (100000)
#define CLOCK_IAS_PLL_MAX_FREQ     (594000)
#define CLOCK_IAS_PLL_DEFAULT_FREQ (200000)
#define CLOCK_ENC_PLL_MIN_FREQ     (1000)
#define CLOCK_ENC_PLL_MAX_FREQ     (832000) /* AX513CE (588000) */
#define CLOCK_ENC_PLL_DEFAULT_FREQ (832000) /* AX513CE (588000) */
#define CLOCK_AUDIO_MIN_FREQ       (38322)
#define CLOCK_AUDIO_MAX_FREQ       (396000)
#define CLOCK_AUDIO_DEFAULT_FREQ   (38322)
#define CLOCK_CPU_PLL_MAX_FREQ     (1000000)
#define CLOCK_APB0_MAX_FREQ        (-1)
#define CLOCK_CPUV_MIN_FREQ        (79200)
#define CLOCK_CPUV_MAX_FREQ        (594000)
#define CLOCK_CPUV_DEFAULT_FREQ    (594000)
#define CLOCK_ZSP_MIN_FREQ         (-1)
#define CLOCK_ZSP_MAX_FREQ         (-1)
#define CLOCK_ZSP_DEFAULT_FREQ     (-1)
#define CLOCK_MIPI_MIN_FREQ        (-1)
#define CLOCK_MIPI_MAX_FREQ        (-1)
#define CLOCK_MIPI_DEFAULT_FREQ    (-1)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* End of #ifndef __NI_DEFINE_H__ */
