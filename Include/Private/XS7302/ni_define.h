/******************************************************************************

  Copyright (C), 2014-2020, ZheJiang Chipup Technology CO.,LTD.

 ******************************************************************************
  File Name     : ni_define.h
  Version       : Initial Draft
  Author        : Lv Zhuqing
  Created       : 2014.3.13
  Last Modified :
  Description   : 描述各种宏定义
  Function List :
  DHstory       :
  1.Date        : 2014/3/13
    Author      : 24497
    Modification: Create
******************************************************************************/
#ifndef __NI_DEFINE_H__
#define __NI_DEFINE_H__
    
    
#ifdef __cplusplus
#if __cplusplus
    extern "C"{
#endif
#endif /* __cplusplus */


#define XS7312_V100  0x73120100
#define XS7302_V100  0x73020100
#define XS7320_V100  0x73200100
#define XS7310_V100  0x73100100


#ifndef XSCHIP
    #define XSCHIP XS7312_V100
#endif

#if XSCHIP == XS7312_V100
    #define CHIP_NAME    "XS7312"
#else
    #error XSCHIP define may be error
#endif

#define GPIO_BANK_NUM               (5)
#define GPIO_MAX_NUM                (160)
#define GPIO_MAX_NUM_OF_PER_BANK	(32)
#define GPIO_MAX_INT_NUM            (160)
#define SPI_MAX_NUM                 (2)
#define DHC_SSI_DEF_BAUDRATE        (4)
#define DHC_SSI_FIFO_DEPTH          (16)

#define ADC_CHANNEL_NUM             (4)

#define PWM_MAX_DEV                 (6)
#define PWM_CONFIG_APB_FREQ         (50000000) 

#define I2C_SPEED                   (400*1000)
#define I2C_DUTYRATIO               (46)

#define MDK_VER_PRIX "_MDK_V"

#define ISP_YHIST_STA_IRQ           (32)
#define ISP_YHIST_FRESH_IRQ         (33)
#define ISP_AWES_IRQ                (34)
#define ISP_AF_IRQ                  (57)
#define ISP_IRQ                     (36)

#define VI_IRQ                      (53)

/*********************VENC 部分规格定义*******************************/
#define VENC_MAX_CHN_NUM            (4)

#define VENC_MAX_WIDTH              (4096)
#define VENC_MAX_HEIGHT             (4096)
#define VENC_MIN_H26X_WIDTH         (128)
#define VENC_MIN_H26X_HEIGHT        (128)
#define VENC_MIN_JPEG_WIDTH         (32)
#define VENC_MIN_JPEG_HEIGHT        (32)

#define VENC_MAX_ROI_NUM            (8)

#define VENC_MIN_USER_DATA_SIZE     (16)
#define VENC_MAX_USER_DATA_SIZE     (256-35+16) /* From Vendor */

#define AENC_MAX_CHN_NUM                (4)
#define AENC_GEN_CHN_NUM                (2)
#define ADEC_MAX_CHN_NUM                (2)

/*********************VDEC 部分规格定义*******************************/
#define VDEC_MAX_CHN_NUM            (4)
#define VDEC_MAX_OUTPUT_PER_CHN     (1) /* 每一路解码能输出几路视频 */

#define VDEC_MAX_WIDTH              (4096)
#define VDEC_MAX_HEIGHT             (2304)
#define VDEC_MIN_WIDTH              (48)
#define VDEC_MIN_HEIGHT             (48)

/* *********************VD 部分规格定义*******************************/
/*最大支持的动检的通道数*/
#define VD_MAX_CHN_NUM         (1)

/*每个通道最大支持动检的区域*/
#define VD_MAX_DETECT_REGION   (4)

/* ****************VPS 部分规格定义***********************************/
#define VPS_MAX_GRP_NUM         (1)
#define VPS_GRP_MAX_CHN_NUM     (8)
#define VPS_GRP_ENC_NUM         (2)
#define VPS_GRP_IAS_NUM         (1)
#define VPS_GRP_VIR_IAS_NUM     (2)

/*支持MASK 的最大通道数目 */
#define VPS_SUPPORT_MASK_CHN_NUM   (4)

/*支持OSD 的最大通道数目 */
#define VPS_SUPPORT_OSD_CHN_NUM    (4)

#define VPS_MIN_INPUT_WIDTH     (176)
#define VPS_MIN_INPUT_HEIGHT    (144)
#define VPS_MAX_INPUT_WIDTH     (2688)
#define VPS_MAX_INPUT_HEIGHT    (1944)
#define VPS_MIN_OUTPUT_WIDTH     (32)
#define VPS_MIN_OUTPUT_HEIGHT    (32)
#define VPS_MAX_INPUT_FPS       (120)
#define VPS_MIN_INPUT_FPS       (1)

#define VPS_OSD_PALETTE_NUM     (16)
#define VPS_OSD_LAYER_NUM       (3)
#define VPS_OSD_NUM_PER_LAYER   (16)
#define VPS_OSD_MAX_NUM         (VPS_OSD_LAYER_NUM * VPS_OSD_NUM_PER_LAYER)
#define VPS_OSD_ALPHA_LEVEL     (32)

/* 叠YUV源时，限制获取内存的最大值，宽128*高128*1.5 */
#define VPS_MAX_OSD_YUV_SIZE    (24576)
#define VPS_MAX_OSD_DOT_MATRIX_SIZE    (8847360)

#define VPS_MAX_CHN_MASK_NUM    (8)

/* 定义vps一个group中支持mask的通道个数 */
#define VPS_MASK_MAX_CHN_NUM    (4)

#define VPS_MASK_MAX_WIDTH      VPS_MAX_INPUT_WIDTH
#define VPS_MASK_MAX_HEIGHT     VPS_MAX_INPUT_HEIGHT

#define VPS_DISP_MAX_BANK       (4)

#define VPS_MAX_BANK_NUM        (4)

#define VPS_ENC_IMG_MAX_WIDTH            (2688)
#define VPS_ENC_IMG_MAX_HEIGH            (1944)

#define VPS_DISP_IMG_MAX_WIDTH            (2688)
#define VPS_DISP_IMG_MAX_HEIGH            (1944)

#define VPS_IAS_IMG_MAX_WIDTH            (2688)
#define VPS_IAS_IMG_MAX_HEIGH            (1944)

#define VPS_IAS_SEC_IMG_MAX_WIDTH        (1920)
#define VPS_IAS_SEC_IMG_MAX_HEIGHT        (1080)

#define VPS_IAS_MAX_EXTRACT_FRAME0       (32) 

/* *******************************OSD***********************************************/
#define OSD_MAX_HANDLE_NUM         (1000)
#define OSD_MAX_SUPPORT_MOD_NUM    (1)
#define OSD_EX_MAX_SUPPORT_MOD_NUM (2)
#define OSD_MAX_GRP_NUM            (2)
/*vps中只有ENC0，ENC1，ENC2，DISP，IAS1五个通道支持，IAS1==5，故取值为6*/
#define OSD_MAX_CHN_NUM            (6)
#define OSD_EX_MAX_CHN_NUM         (16)
#define OSD_EX_VPS_MAX_CHN_NUM     (3)
#define OSD_MAX_PALETTE_NUM        (16)
#define OSD_MAX_LAYER_NUM          (3)
/* OSD_EX软件概念上只有一层，由硬件实现16层分级概念 */
#define OSD_EX_MAX_LAYER_NUM       (1)
#define OSD_PER_LAYER_NUM          (16)
/* OSD_EX最大层优先级序号 */
#define OSD_EX_MAX_LAYER_PRI       (16)
#define OSD_YUV_PER_LAYER_NUM      (4)
#define OSD_ALPHA_LEVEL            (32)
#define OSD_MAX_LATTICE_LAYER_NUM  (2)
//tenda_start
#define TD_OSD_MAX_NUM             (4)
//tenda_end
/* OSD_EX 帧信息队列最大队列深度 */
#define OSD_EX_MAX_NODE_NUM        (32)

/*********************************VI************************************************/
#define VI_MAX_DEV_NUM              (4)

#define VI_MAX_DATA_LINE            (16)

#define VI_MAX_HDR_NUM              (2)
#define VI_GET_FRAME_MAX_DEPTH      (20)
#define VI_ENC_MAX_BANK_NUM         (4)
#define VI_ENC_CHANEL_NUM           (3)
#define VI_DEC_CHANEL_NUM           (2)
#define VI_DEC_SEL_NUM              (2)

#define VI_SEND_RAW_FRAME_MAX_NUM   (256)
#define VI_MAX_WIDTH                (4096)

#define VI_MAX_HEIGHT               (2160)

#define VI_MAX_INTF_NUM             (2)

/*********************************ISP************************************************/
/* 定义ISP最大dev数 */
#define ISP_MAX_DEV_NUM             (1)
/* 定义SPC最大静态坏点个数 */
#define ISP_SPC_MAX_DP_COUNT        (1024)
/* 定义AWES统计模块最大分块个数 */
#define ISP_AWES_MAX_ZONE_NUM       (2048)
/* 定义AWES统计模块最大分块个数 */
#define ISP_RAW_HIST_MAX_ZONE_NUM   (2048)
/* 定义DPC在系统中的处理单元个数 */
#define ISP_DPC_MAX_ID_NUM          (1)
/* 定义GIC在系统中的处理单元个数 */
#define ISP_GIC_MAX_ID_NUM          (1)
/* 定义BLC在系统中的处理单元个数 */
#define ISP_BLC_MAX_ID_NUM          (3)
/* 定义DGC在系统中的处理单元个数 */
#define ISP_DGC_MAX_ID_NUM          (3)
/* 定义WB在系统中的处理单元个数 */
#define ISP_WB_MAX_ID_NUM          (3)
/* 定义LSCC在系统中的处理单元个数 */
#define ISP_LSC_MAX_ID_NUM          (1)
/* 定义AWES统计数据缓存水线，该值不可设太大 */
#define IPS_AWES_STATISTICS_LINE    (1)
/* 定义YHIST统计数据缓存水线，该值不可设太大 */
#define IPS_YHIST_STATISTICS_LINE   (1)

/*定义isp计算内存大小*/
#define ISP_CALCE_MEM_SIZE          (8192*4)

/* 定义DLC分块个数 */
#define ISP_DLC_BLOCK_NUM           (25)

#define ISP_SPC_MAX_TABLE_LEN        (1024)
#define ISP_LSC_MAX_TABLE_LEN        (168)
#define ISP_LWDR_FI_MAX_TABLE_LEN    (256)
#define ISP_LWDR_ALPHA_MAX_TABLE_LEN (256)
#define ISP_GAMMA_MAX_TABLE_LEN      (256)
#define ISP_CSCCDS_MAX_TABLE_LEN     (256)


/* yhist调节对比度的,长度256，不在让用户拼奇偶表，内部做掉*/
#define ISP_YHIST_MAX_TABLE_LEN     (256)
#define ISP_YHIST_STAT_DATA_NUM     (256)
#define ISP_RAWHIST_STAT_DATA_NUM   (256)
#define ISP_AF_STAT_DATA_NUM        (1024)

#define ISP_LWDR_MAX_FI_TABLE_LEN       (256)
#define ISP_LWDR_MAX_ALPHA_TABLE_LEN    (256)

/* ISP 支持的最大宽分辨率宽高 */
#define ISP_MAX_WIDTH                 (2688) 
#define ISP_MAX_HEIGHT                (1944)

/*最多3帧数据，长、短、融合*/
#define ISP_AWES_MAX_FRM            (3)
#define ISP_RAWHIST_MAX_FRM         (3)

/* 定义ISP EE_NRY2D算法空域低频滤波系数个数 */
#define ISP_EE_NRY2D_SPLF_COEF_NUM  (6)

/* 定义ISP WLTEE算法空域低频滤波系数个数 */
#define ISP_WLTEE_SPLF_COEF_NUM  (6)

/*********************************VO************************************************/
/* we has two disp device */
#define VO_MAX_DEV_NUM              (1)
#define VO_MAX_CHN_NUM              (1)
#define VO_MAX_WIDTH                (3072)
#define VO_MAX_HEIGHT               (1728)
#define VO_MAX_LAYER_NUM            (1)

#define NIFB_MAX_LAYER_NUM          (1)
#define NIFB_MAX_LAYER_WIDTH_G0     (1920)
#define NIFB_MAX_LAYER_HEIGHT_G0    (1080)
#define NIFB_MIN_LAYER_WIDTH_G0     (32)
#define NIFB_MIN_LAYER_HEIGHT_G0    (32)


/*********************************mbuf**********************************************/
#define MBUF_MAX_POOLS              (128)
#define MBUF_MAX_COMM_POOLS         (16)
#define MBUF_MAX_PRIV_POOLS         (MBUF_MAX_POOLS - MBUF_MAX_COMM_POOLS)


/* ddr 名字长度 */
//#define MAX_DDR_NAME_LEN         (16)

/*********************************ISP_SEN_SYNC************************************************/
#define SEN_MAX_REGS_NUM        (32)    /*根据平台区分最大发送能力*/
#define SEN_MAX_I2C_GROUP_NUM   (16)     /*根据平台区分最大发送能力*/
#define SEN_MAX_I2C_REGS_NUM    (6)     /*根据平台区分最大发送能力*/
#define SEN_I2C_MAX_DELAY       (2)     /*最大延迟时间*/
#define SEN_I2C_SEND_NUM_MAX    (128)     /*最大延迟时间*/

/*********************************AISP************************************************/
#define AISP_MAX_DB_NUM	(16)

/*********************************DVP************************************************/
/* 定义DVP支持的图像最大宽度 */
#define DVP_MAX_WIDTH              (2688)

/* 定义DVP支持的图像最大高度 */
#define DVP_MAX_HEIGHT             (2688)

/* 定义DVP支持的图像最小宽度 */
#define DVP_MIN_WIDTH              (32)

/* 定义DVP支持的图像最小高度 */
#define DVP_MIN_HEIGHT             (32)

/* 定义DVP图像跨距stride对齐要求 */
#define DVP_STRIDE_ALIGN           (16)

/* 定义DVP图像Y/U/V起始地址对齐要求 */
#define DVP_PHYADDR_ALIGN          (16)

/* 定义DVP点阵数据最大支持的调色板颜色数 */
#define DVP_MAX_PALETTE_NUM        (16)

/* 定义DVP启动一次硬件链表最大支持的命令个数 */
#define DVP_MAX_CMD_NUM            (32)

/* 定义DVP一次硬件命令操作最大支持的遮挡块个数 */
#define DVP_MAX_MASK_NUM           (8)

/* 定义DVP一次硬件命令操作最大支持的YUV OSD个数 */
#define DVP_MAX_YUV_OSD_NUM        (8)

/* 定义DVP OSD 最大透明度 */
#define DVP_MAX_OSD_ALPHA_LEVEL    (32)

/* 定义DVP硬件支持的OSD层数，2层点阵，1层yuv图像 */
#define DVP_MAX_OSD_LAYER_NUM      (3)

/* 定义DVP硬件支持的字符点阵OSD层数 */
#define DVP_MAX_LATTICE_OSD_LAYER_NUM    (2)

/* 定义DVP硬件每层点阵OSD最大支持的osd个数 */
#define DVP_MAX_LATTICE_OSD_PER_PERLAY_NUM    (4)

/* 定义DVP一次硬件命令操作最大支持的字符点阵OSD个数 */
#define DVP_MAX_LATTICE_OSD_NUM    (DVP_MAX_LATTICE_OSD_LAYER_NUM * DVP_MAX_LATTICE_OSD_PER_PERLAY_NUM)

/*********************************LDC************************************************/
#define LDC_MAX_DEV_NUM        (2)
#define LDC_FRAME_MAX_DEPTH        (10)

/*********************************IVE************************************************/
#define IVE_MAX_STRIDE     (65535)
#define IVE_MAX_WIDTH      (1920)
#define IVE_MAX_HEIGHT     (1080)

/********************************CLOCK***********************************************/
/* 定义芯片支持的时钟PLL个数 */
#define CLOCK_MAX_PLL_NUM           (6)

/* 定义芯片支持的sensor vclk 个数 */
#define CLOCK_SENSOR_VCLK_NUM       (2)

/* 定义芯片支持的DDR PORT口 */
#define CLOCK_DDR_PORT_NUM          (6)

/* 定义CLOCK 时钟约束,其中频率定义为(-1)表示平台不支持该项频率配置 */
#define CLOCK_DDR_PLL_MAX_FREQ      (533000)
#define CLOCK_AHB_MIN_FREQ          (32258)
#define CLOCK_AHB_MAX_FREQ          (200000)
#define CLOCK_APB_MAX_FREQ          (50000)
#define CLOCK_VCLK_MIN_FREQ         (15873)
#define CLOCK_VCLK_MAX_FREQ         (100000)
#define CLOCK_VCLK_DEFAULT_FREQ     (37125)
#define CLOCK_VIU_MIN_FREQ          (15873)
#define CLOCK_VIU_MAX_FREQ          (333333)
#define CLOCK_VIU_DEFAULT_FREQ      (333333)
#define CLOCK_ISP_MIN_FREQ          (32258)
#define CLOCK_ISP_MAX_FREQ          (237600)
#define CLOCK_ISP_DEFAULT_FREQ      (237600)
#define CLOCK_VPS_MIN_FREQ          (32258)
#define CLOCK_VPS_MAX_FREQ          (250000)
#define CLOCK_VPS_DEFAULT_FREQ      (250000)
#define CLOCK_IVE_MIN_FREQ          (32258)
#define CLOCK_IVE_MAX_FREQ          (500000)
#define CLOCK_IVE_DEFAULT_FREQ      (500000)
#define CLOCK_BT1120_MIN_FREQ       (18857)
#define CLOCK_BT1120_MAX_FREQ       (74250)
#define CLOCK_BT1120_DEFAULT_FREQ   (74250)
#define CLOCK_LCD_MAIN_MIN_FREQ     (18857)
#define CLOCK_LCD_SYS_MIN_FREQ      (2693)
#define CLOCK_LCD_MAX_FREQ          (108000)
#define CLOCK_LCD_DEFAULT_FREQ      (108000)
#define CLOCK_JPEG_MIN_FREQ         (1000)
#define CLOCK_JPEG_MAX_FREQ         (200000)
#define CLOCK_JPEG_DEFAULT_FREQ     (200000)
#define CLOCK_DDR1_MIN_FREQ         (142857)
#define CLOCK_DDR1_MAX_FREQ         (250000)
#define CLOCK_DDR1_DEFAULT_FREQ     (250000)
#define CLOCK_DDR2_MIN_FREQ         (142857)
#define CLOCK_DDR2_MAX_FREQ         (500000)
#define CLOCK_DDR2_DEFAULT_FREQ     (500000)
#define CLOCK_DDR3_MIN_FREQ         (142857)
#define CLOCK_DDR3_MAX_FREQ         (250000)
#define CLOCK_DDR3_DEFAULT_FREQ     (250000)
#define CLOCK_DDR4_MIN_FREQ         (142857)
#define CLOCK_DDR4_MAX_FREQ         (334000)
#define CLOCK_DDR4_DEFAULT_FREQ     (333333)
#define CLOCK_SDIO_MAX_FREQ         (50000)
#define CLOCK_IAS_PLL_MAX_FREQ      (594000)
#define CLOCK_IAS_PLL_DEFAULT_FREQ  (200000)
#define CLOCK_ENC_PLL_MIN_FREQ      (1000)
#define CLOCK_ENC_PLL_MAX_FREQ      (594000)
#define CLOCK_ENC_PLL_DEFAULT_FREQ  (594000)
#define CLOCK_AUDIO_MIN_FREQ        (38322)
#define CLOCK_AUDIO_MAX_FREQ        (396000)
#define CLOCK_AUDIO_DEFAULT_FREQ    (297000)
#define CLOCK_CPU_PLL_MAX_FREQ      (-1)
#define CLOCK_APB0_MAX_FREQ         (-1)
#define CLOCK_CPUV_MIN_FREQ         (-1)
#define CLOCK_CPUV_MAX_FREQ         (-1)
#define CLOCK_CPUV_DEFAULT_FREQ     (-1)
#define CLOCK_ZSP_MIN_FREQ          (-1)
#define CLOCK_ZSP_MAX_FREQ          (-1)
#define CLOCK_ZSP_DEFAULT_FREQ      (-1)
#define CLOCK_MIPI_MIN_FREQ         (-1)
#define CLOCK_MIPI_MAX_FREQ         (-1)
#define CLOCK_MIPI_DEFAULT_FREQ     (-1)




#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* End of #ifndef __NI_DEFINE_H__ */

