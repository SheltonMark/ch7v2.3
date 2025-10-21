/* 
 * sdk_comm_vi.c -- vi sample用例 
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

#include "ni_comm_vi.h"
#include "mdk_vi.h"
#include "mdk_sys.h"
#include "mdk_pm.h"
#include "sdk_comm.h"
#include "ni9932_config.h"

#define SAMPLE_BOARD_ID1    0x6 //XS7302 MIPI样片测试板
#define SAMPLE_BOARD_ID2    0x7 //XS7302 DVP样片测试板
#define SAMPLE_BOARD_ID3    0x2 //XS7312 MIPI样片测试板
#define SAMPLE_BOARD_ID4    0x0 //XS7302 MIPI演示DEMO板

extern int get_frame_rate(float FrameRate);
static NI_S32 as32ViDestChnId[VI_MAX_DEV_NUM] = {0};

TEST_VI_TYPE_S gastViType[TEST_VI_MODE_BUTT] = 
{
    {"IMX307 720p@30fps liner", TEST_VI_IMX307_MIPI_720P_30FPS},
    {"IMX307 1080p@25fps liner", TEST_VI_IMX307_MIPI_1080P_25FPS},
    {"IMX307 720p@25fps hdr", TEST_VI_IMX307_MIPI_720P_25FPS_HDR},
    {"IMX307 1080p@25fps hdr", TEST_VI_IMX307_MIPI_1080P_25FPS_HDR},
    {"SC2238 720p@30fps liner", TEST_VI_SC2238_DC_720P_30FPS},
    {"SC2238 1080p@25fps liner", TEST_VI_SC2238_DC_1080P_25FPS},
    {"SC200AI 720p@30fps liner", TEST_VI_SC200AI_MIPI_720P_30FPS},
    {"SC200AI 1080p@25fps liner", TEST_VI_SC200AI_MIPI_1080P_25FPS},
    {"SC200AI 1080p@25fps hdr", TEST_VI_SC200AI_MIPI_1080P_25FPS_HDR},
    {"SC2239P 1080p@30fps liner", TEST_VI_SC2239P_MIPI_1080P_30FPS},
    {"OS04A10 2688x1520@25fps liner", TEST_VI_OS04A10_MIPI_2688x1520_25FPS},
    {"OS04A10 2688x1520@25fps hdr", TEST_VI_OS04A10_MIPI_2688x1520_25FPS_HDR},
    {"OS05A10 2688x1944@25fps liner", TEST_VI_OS05A10_MIPI_2688x1944_25FPS},
    {"OS05A10 2688x1944@20fps hdr", TEST_VI_OS05A10_MIPI_2688x1944_20FPS_HDR},
    {"OS04C10 2688x1520@25fps liner", TEST_VI_OS04C10_MIPI_2688x1520_25FPS},
    {"OS04C10 2688x1520@25fps hdr", TEST_VI_OS04C10_MIPI_2688x1520_25FPS_HDR},
    {"COLORBAR 720p@30fps liner", TEST_VI_COLOR_720P_30FPS},
    {"COLORBAR 1080p@30fps liner", TEST_VI_COLOR_1080P_10FPS},
    {"NI9931 720p@30fps bt656", TEST_VI_NI9931_BT656_720P_30FPS},
    {"NI9931 1080p@25fps bt1120", TEST_VI_NI9931_BT1120_1080P_25FPS},
};

NI_U32 SDK_COMM_VI_GetBoardID(void)
{
    NI_U32 u32Value = 0;
    NI_U32 u32BoardType = 0;


    /*管脚复用 GPIO_3_12 GPIO3_13 GPIO 3_16*/
    NI_MDK_SYS_ReadReg(0x08001028, &u32Value);
    u32Value &= ~((0xf << 12) | (0xf << 16) | (0xf << 28));
    u32Value |= ((0x1 << 12) | (0x1 << 16) | (0x1 << 28));
    NI_MDK_SYS_WriteReg(0x08001028, u32Value);

    /*设置3个GPIO管脚为输入*/
    NI_MDK_SYS_ReadReg(0x01130004, &u32Value);
    u32Value &= ~((1 << 12) | (1 << 13) | (1 << 16));
    NI_MDK_SYS_WriteReg(0x01130004, u32Value);

    /*读取输入状态*/
    NI_MDK_SYS_ReadReg(0x01130050, &u32Value);
    
    u32BoardType = (((u32Value & (1 << 12)) >> 12) << 2) | (((u32Value & (1 << 13)) >> 13) << 1) | ((u32Value & (1 << 16)) >> 16);

    return u32BoardType;
}

NI_S32 SDK_COMM_VI_SetPinCfg(VI_DEV ViDev, TEST_VI_MODE_E enViMode)
{
    NI_U32 u32Value;
    NI_U32 u32BoardType = 0;

    u32BoardType = SDK_COMM_VI_GetBoardID();
    if (SAMPLE_BOARD_ID3 == u32BoardType)
    {
        /* set i2c0&i2c1&i2c2&i2c3 pinmux */
        NI_MDK_SYS_ReadReg(0x08001000, &u32Value);
        u32Value &= 0x0;
        NI_MDK_SYS_WriteReg(0x08001000, u32Value);

        /* set sensor0&sensor1 vclk pinmux */
        NI_MDK_SYS_ReadReg(0x08001020, &u32Value);
        u32Value &= 0xfffff0f0;
        u32Value |= 0x200;
        NI_MDK_SYS_WriteReg(0x08001020, u32Value);

        /* set sensor reset gpio2_31&gpio2_30 */
        NI_MDK_SYS_ReadReg(0x01060004, &u32Value);
        u32Value &= 0x3fffffff;
        u32Value |= 0xc0000000;
        NI_MDK_SYS_WriteReg(0x01060004, u32Value);
        
        NI_MDK_SYS_ReadReg(0x01060000, &u32Value);
        u32Value &= 0x3fffffff;
        NI_MDK_SYS_WriteReg(0x01060000, u32Value);
        usleep(10);
        u32Value |= 0xc0000000;
        NI_MDK_SYS_WriteReg(0x01060000, u32Value);

        /* set interface pin mux */
        switch(enViMode)
        {
            case TEST_VI_SC200AI_MIPI_720P_30FPS:
            case TEST_VI_SC200AI_MIPI_1080P_25FPS:
            case TEST_VI_SC200AI_MIPI_1080P_25FPS_HDR:
            case TEST_VI_IMX307_MIPI_720P_30FPS:
            case TEST_VI_IMX307_MIPI_1080P_25FPS:
            case TEST_VI_IMX307_MIPI_1080P_30FPS:
            case TEST_VI_IMX307_MIPI_720P_25FPS_HDR:
            case TEST_VI_IMX307_MIPI_1080P_12_5FPS_DHR:
            case TEST_VI_IMX307_MIPI_1080P_3FPS_DHR:
            case TEST_VI_IMX307_MIPI_1080P_25FPS_HDR:
            case TEST_VI_IMX307_MIPI_1080P_30FPS_HDR:
            case TEST_VI_IMX415_MIPI_4K_25FPS:
            case TEST_VI_IMX415_MIPI_4K_30FPS_HDR:
            case TEST_VI_IMX307_MIPI_1080P_7FPS:
            case TEST_VI_OS04A10_MIPI_2688x1520_25FPS:
            case TEST_VI_OS04A10_MIPI_2688x1520_30FPS:
            case TEST_VI_OS04A10_MIPI_2688x1520_25FPS_HDR:
            case TEST_VI_OS05A10_MIPI_2688x1944_25FPS:
            case TEST_VI_OS05A10_MIPI_2688x1944_20FPS_HDR:
            case TEST_VI_OS04C10_MIPI_2688x1520_25FPS:
            case TEST_VI_OS04C10_MIPI_2688x1520_25FPS_HDR:
            case TEST_VI_SC2239P_MIPI_720P_60FPS:
            case TEST_VI_SC2239P_MIPI_1080P_30FPS:
            case TEST_VI_COLOR_720P_30FPS:
            case TEST_VI_COLOR_1080P_10FPS:
            case TEST_VI_GC2083_MIPI_1080P_30FPS:
                /* no pinmux config */
                break;
                
            case TEST_VI_SC2238_DC_720P_30FPS:
            case TEST_VI_SC2238_DC_1080P_25FPS:
                /* set dc pin mux */
                NI_MDK_SYS_ReadReg(0x08001020, &u32Value);
                u32Value &= 0xfff000ff;
                NI_MDK_SYS_WriteReg(0x08001020, u32Value);

                break;
            case TEST_VI_NI9931_BT656_720P_30FPS:
            case TEST_VI_NI9931_BT1120_1080P_25FPS:
                /* set bt-in pin mux */
                NI_MDK_SYS_ReadReg(0x08001020, &u32Value);
                u32Value &= 0x000fffff;
                NI_MDK_SYS_WriteReg(0x08001020, u32Value);

                NI_MDK_SYS_ReadReg(0x08001024, &u32Value);
                u32Value &= 0xff000000;
                NI_MDK_SYS_WriteReg(0x08001024, u32Value);
                break;

            case TEST_VI_MODE_BUTT:
            default:
                printf("not support sesnor mode:%d\n", enViMode);
            return NI_FAILURE;
        }
    }
    else if (SAMPLE_BOARD_ID1 == u32BoardType || SAMPLE_BOARD_ID2 == u32BoardType || SAMPLE_BOARD_ID4 == u32BoardType)
    {
        /* set i2c0 pinmux */
        NI_MDK_SYS_ReadReg(0x08001000, &u32Value);
        u32Value &= 0xffffff00;
        NI_MDK_SYS_WriteReg(0x08001000, u32Value);
        
        /* set sensor vclk pinmux */
        NI_MDK_SYS_ReadReg(0x08001020, &u32Value);
        u32Value &= 0xfffffff0;
        NI_MDK_SYS_WriteReg(0x08001020, u32Value);

        if (SAMPLE_BOARD_ID1 == u32BoardType || SAMPLE_BOARD_ID2 == u32BoardType)
        {
    
        /* sensor reset */
            NI_MDK_SYS_WriteReg(0x080000e0, 0x2);
            usleep(10);
            NI_MDK_SYS_WriteReg(0x080000e0, 0x3);
        }
        else if (SAMPLE_BOARD_ID4 == u32BoardType)
        {

            /* set sensor reset gpio2_27 pinmux */
            NI_MDK_SYS_ReadReg(0x0800101c, &u32Value);
            u32Value &= 0x0fffffff;
            u32Value |= 0x10000000;
            NI_MDK_SYS_WriteReg(0x0800101c, u32Value);
        
            /* set sensor reset gpio2_27 */
            NI_MDK_SYS_ReadReg(0x01060004, &u32Value);
            u32Value &= 0xf7ffffff;
            u32Value |= 0x08000000;
            NI_MDK_SYS_WriteReg(0x01060004, u32Value);
            
            NI_MDK_SYS_ReadReg(0x01060000, &u32Value);
            u32Value &= 0xf7ffffff;
            NI_MDK_SYS_WriteReg(0x01060000, u32Value);
            usleep(10);
            u32Value |= 0x08000000;
            NI_MDK_SYS_WriteReg(0x01060000, u32Value);
        }

        /* set interface pin mux */
        switch(enViMode)
        {
            case TEST_VI_SC200AI_MIPI_720P_30FPS:
            case TEST_VI_SC200AI_MIPI_1080P_25FPS:
            case TEST_VI_SC200AI_MIPI_1080P_25FPS_HDR:
            case TEST_VI_SC2239P_MIPI_720P_60FPS:
            case TEST_VI_SC2239P_MIPI_1080P_30FPS:
            case TEST_VI_IMX307_MIPI_720P_30FPS:
            case TEST_VI_IMX307_MIPI_1080P_25FPS:
            case TEST_VI_IMX307_MIPI_1080P_30FPS:
            case TEST_VI_IMX307_MIPI_720P_25FPS_HDR:
            case TEST_VI_IMX307_MIPI_1080P_12_5FPS_DHR:
            case TEST_VI_IMX307_MIPI_1080P_3FPS_DHR:
            case TEST_VI_IMX307_MIPI_1080P_25FPS_HDR:
            case TEST_VI_IMX307_MIPI_1080P_30FPS_HDR:
            case TEST_VI_IMX415_MIPI_4K_25FPS:
            case TEST_VI_IMX415_MIPI_4K_30FPS_HDR:
            case TEST_VI_IMX307_MIPI_1080P_7FPS:
            case TEST_VI_OS04A10_MIPI_2688x1520_25FPS:
            case TEST_VI_OS04A10_MIPI_2688x1520_30FPS:
            case TEST_VI_OS04A10_MIPI_2688x1520_25FPS_HDR:
            case TEST_VI_OS05A10_MIPI_2688x1944_25FPS:
            case TEST_VI_OS05A10_MIPI_2688x1944_20FPS_HDR:
            case TEST_VI_OS04C10_MIPI_2688x1520_25FPS:
            case TEST_VI_OS04C10_MIPI_2688x1520_25FPS_HDR:
            case TEST_VI_NI9931_BT656_720P_30FPS:
            case TEST_VI_NI9931_BT1120_1080P_25FPS:
            case TEST_VI_COLOR_720P_30FPS:
            case TEST_VI_COLOR_1080P_10FPS:
            case TEST_VI_GC2083_MIPI_1080P_30FPS:
                /* no pinmux config */
                break;

            case TEST_VI_SC2238_DC_720P_30FPS:
            case TEST_VI_SC2238_DC_1080P_25FPS:
                /* set dc pin mux */
                NI_MDK_SYS_ReadReg(0x08001020, &u32Value);
                u32Value &= 0xfffffff0;
                NI_MDK_SYS_WriteReg(0x08001020, u32Value);
        
                NI_MDK_SYS_ReadReg(0x08001028, &u32Value);
                u32Value = 0x22222222;
                NI_MDK_SYS_WriteReg(0x08001028, u32Value);

                NI_MDK_SYS_ReadReg(0x0800102c, &u32Value);
                u32Value &= 0xfffffff0;
                u32Value |= 0x2;
                NI_MDK_SYS_WriteReg(0x0800102c, u32Value);
                break;
        
            default:
                printf("not support sesnor mode:%d\n", enViMode);
            return NI_FAILURE;
        }
    }

    printf("boardType %#x set vi pinmux&clk success!\n", u32BoardType);
    return NI_SUCCESS;
}

NI_S32 SDK_COMM_VI_GetViInfo(SENSOR_ID SenId, TEST_VI_MODE_E enViMode, SAMPLE_VI_INFO_S *pstViInfo)
{
    memset(pstViInfo, 0, sizeof(SAMPLE_VI_INFO_S));

    pstViInfo->enViMode = enViMode;
    switch(enViMode)
    {
        case TEST_VI_SC2238_DC_720P_30FPS:
            pstViInfo->stSenCfg.enSenType = SEN_TYPE_SC2238;
            pstViInfo->stSenCfg.enIntfType = VI_INTF_TYPE_DC;
            pstViInfo->stSenCfg.enHdrMode = HDR_MODE_NONE;
            pstViInfo->stSenCfg.u32Width = 1280;
            pstViInfo->stSenCfg.u32Height = 720;
            pstViInfo->stSenCfg.u32Fps = 30;
            pstViInfo->stSenCfg.u32LaneNum = 0;
            pstViInfo->stViCfg.enViWorkMode = VI_WORK_MODE_NORMAL;
            break;
        case TEST_VI_SC2238_DC_1080P_25FPS:
            pstViInfo->stSenCfg.enSenType = SEN_TYPE_SC2238;
            pstViInfo->stSenCfg.enIntfType = VI_INTF_TYPE_DC;
            pstViInfo->stSenCfg.enHdrMode = HDR_MODE_NONE;
            pstViInfo->stSenCfg.u32Width = 1920;
            pstViInfo->stSenCfg.u32Height = 1080;
            pstViInfo->stSenCfg.u32Fps = 25;
            pstViInfo->stSenCfg.u32LaneNum = 0;
            pstViInfo->stViCfg.enViWorkMode = VI_WORK_MODE_NORMAL;
            break;
        case TEST_VI_IMX307_MIPI_720P_30FPS:
            pstViInfo->stSenCfg.enSenType = SEN_TYPE_IMX307;
            pstViInfo->stSenCfg.enIntfType = VI_INTF_TYPE_MIPI;
            pstViInfo->stSenCfg.enHdrMode = HDR_MODE_NONE;
            pstViInfo->stSenCfg.u32Width = 1280;
            pstViInfo->stSenCfg.u32Height = 720;
            pstViInfo->stSenCfg.u32Fps = 30;
            pstViInfo->stSenCfg.u32LaneNum = 2;
            pstViInfo->stViCfg.enViWorkMode = VI_WORK_MODE_NORMAL;
            break;
        case TEST_VI_IMX307_MIPI_1080P_25FPS:
            pstViInfo->stSenCfg.enSenType = SEN_TYPE_IMX307;
            pstViInfo->stSenCfg.enIntfType = VI_INTF_TYPE_MIPI;
            pstViInfo->stSenCfg.enHdrMode = HDR_MODE_NONE;
            pstViInfo->stSenCfg.u32Width = 1920;
            pstViInfo->stSenCfg.u32Height = 1080;
            pstViInfo->stSenCfg.u32Fps = 25;
            pstViInfo->stSenCfg.u32LaneNum = 2;
            pstViInfo->stViCfg.enViWorkMode = VI_WORK_MODE_NORMAL;
            break;
        case TEST_VI_IMX307_MIPI_1080P_30FPS:
            pstViInfo->stSenCfg.enSenType = SEN_TYPE_IMX307;
            pstViInfo->stSenCfg.enIntfType = VI_INTF_TYPE_MIPI;
            pstViInfo->stSenCfg.enHdrMode = HDR_MODE_NONE;
            pstViInfo->stSenCfg.u32Width = 1920;
            pstViInfo->stSenCfg.u32Height = 1080;
            pstViInfo->stSenCfg.u32Fps = 30;
            pstViInfo->stSenCfg.u32LaneNum = 2;
            pstViInfo->stViCfg.enViWorkMode = VI_WORK_MODE_NORMAL;
            break;
        case TEST_VI_IMX307_MIPI_720P_25FPS_HDR:
            pstViInfo->stSenCfg.enSenType = SEN_TYPE_IMX307;
            pstViInfo->stSenCfg.enIntfType = VI_INTF_TYPE_MIPI;
            pstViInfo->stSenCfg.enHdrMode = HDR_MODE_2To1_LINE;
            pstViInfo->stSenCfg.u32Width = 1280;
            pstViInfo->stSenCfg.u32Height = 720;
            pstViInfo->stSenCfg.u32Fps = 25;
            pstViInfo->stSenCfg.u32LaneNum = 2;
            pstViInfo->stViCfg.enViWorkMode = VI_WORK_MODE_NORMAL;
            break;
        case TEST_VI_IMX307_MIPI_1080P_12_5FPS_DHR:
            pstViInfo->stSenCfg.enSenType = SEN_TYPE_IMX307;
            pstViInfo->stSenCfg.enIntfType = VI_INTF_TYPE_MIPI;
            pstViInfo->stSenCfg.enHdrMode = HDR_MODE_2To1_LINE;
            pstViInfo->stSenCfg.u32Width = 1920;
            pstViInfo->stSenCfg.u32Height = 1080;
            pstViInfo->stSenCfg.u32Fps = 12;
            pstViInfo->stSenCfg.u32LaneNum = 4;
            pstViInfo->stViCfg.enViWorkMode = VI_WORK_MODE_NORMAL;
            break;
        case TEST_VI_IMX307_MIPI_1080P_25FPS_HDR:
            pstViInfo->stSenCfg.enSenType = SEN_TYPE_IMX307;
            pstViInfo->stSenCfg.enIntfType = VI_INTF_TYPE_MIPI;
            pstViInfo->stSenCfg.enHdrMode = HDR_MODE_2To1_LINE;
            pstViInfo->stSenCfg.u32Width = 1920;
            pstViInfo->stSenCfg.u32Height = 1080;
            pstViInfo->stSenCfg.u32Fps = 25;
            pstViInfo->stSenCfg.u32LaneNum = 2;
            pstViInfo->stViCfg.enViWorkMode = VI_WORK_MODE_NORMAL;
            break;
        case TEST_VI_IMX307_MIPI_1080P_30FPS_HDR:
            pstViInfo->stSenCfg.enSenType = SEN_TYPE_IMX307;
            pstViInfo->stSenCfg.enIntfType = VI_INTF_TYPE_MIPI;
            pstViInfo->stSenCfg.enHdrMode = HDR_MODE_2To1_LINE;
            pstViInfo->stSenCfg.u32Width = 1920;
            pstViInfo->stSenCfg.u32Height = 1080;
            pstViInfo->stSenCfg.u32Fps = 30;
            pstViInfo->stSenCfg.u32LaneNum = 2;
            pstViInfo->stViCfg.enViWorkMode = VI_WORK_MODE_NORMAL;
            break;
        case TEST_VI_IMX307_MIPI_1080P_7FPS:
            pstViInfo->stSenCfg.enSenType = SEN_TYPE_IMX307;
            pstViInfo->stSenCfg.enIntfType = VI_INTF_TYPE_MIPI;
            pstViInfo->stSenCfg.enHdrMode = HDR_MODE_NONE;
            pstViInfo->stSenCfg.u32Width = 1920;
            pstViInfo->stSenCfg.u32Height = 1080;
            pstViInfo->stSenCfg.u32Fps = 7;
            pstViInfo->stSenCfg.u32LaneNum = 4;
            pstViInfo->stViCfg.enViWorkMode = VI_WORK_MODE_NORMAL;
            break;
        case TEST_VI_IMX415_MIPI_4K_25FPS:
            pstViInfo->stSenCfg.enSenType = SEN_TYPE_IMX415;
            pstViInfo->stSenCfg.enIntfType = VI_INTF_TYPE_MIPI;
            pstViInfo->stSenCfg.enHdrMode = HDR_MODE_NONE;
            pstViInfo->stSenCfg.u32Width = 3840;
            pstViInfo->stSenCfg.u32Height = 2160;
            pstViInfo->stSenCfg.u32Fps = 25;
            pstViInfo->stSenCfg.u32LaneNum = 4;
            pstViInfo->stViCfg.enViWorkMode = VI_WORK_MODE_NORMAL;
            break;
        case TEST_VI_IMX415_MIPI_4K_30FPS_HDR:
            pstViInfo->stSenCfg.enSenType = SEN_TYPE_IMX415;
            pstViInfo->stSenCfg.enIntfType = VI_INTF_TYPE_MIPI;
            pstViInfo->stSenCfg.enHdrMode = HDR_MODE_2To1_LINE;
            pstViInfo->stSenCfg.u32Width = 3840;
            pstViInfo->stSenCfg.u32Height = 2160;
            pstViInfo->stSenCfg.u32Fps = 30;
            pstViInfo->stSenCfg.u32LaneNum = 4;
            pstViInfo->stViCfg.enViWorkMode = VI_WORK_MODE_NORMAL;
            break;
        case TEST_VI_SC200AI_MIPI_720P_30FPS:
            pstViInfo->stSenCfg.enSenType = SEN_TYPE_SC200AI;
            pstViInfo->stSenCfg.enIntfType = VI_INTF_TYPE_MIPI;
            pstViInfo->stSenCfg.enHdrMode = HDR_MODE_NONE;
            pstViInfo->stSenCfg.u32Width = 1280;
            pstViInfo->stSenCfg.u32Height = 720;
            pstViInfo->stSenCfg.u32Fps = 30;
            pstViInfo->stSenCfg.u32LaneNum = 2;
            pstViInfo->stViCfg.enViWorkMode = VI_WORK_MODE_NORMAL;
            break;
        case TEST_VI_SC200AI_MIPI_1080P_25FPS:
            pstViInfo->stSenCfg.enSenType = SEN_TYPE_SC200AI;
            pstViInfo->stSenCfg.enIntfType = VI_INTF_TYPE_MIPI;
            pstViInfo->stSenCfg.enHdrMode = HDR_MODE_NONE;
            pstViInfo->stSenCfg.u32Width = 1920;
            pstViInfo->stSenCfg.u32Height = 1080;
            pstViInfo->stSenCfg.u32Fps = 25;
            pstViInfo->stSenCfg.u32LaneNum = 2;
            pstViInfo->stViCfg.enViWorkMode = VI_WORK_MODE_NORMAL;
            break;
        case TEST_VI_SC200AI_MIPI_1080P_25FPS_HDR:
            pstViInfo->stSenCfg.enSenType = SEN_TYPE_SC200AI;
            pstViInfo->stSenCfg.enIntfType = VI_INTF_TYPE_MIPI;
            pstViInfo->stSenCfg.enHdrMode = HDR_MODE_2To1_LINE;
            pstViInfo->stSenCfg.u32Width = 1920;
            pstViInfo->stSenCfg.u32Height = 1080;
            pstViInfo->stSenCfg.u32Fps = 25;
            pstViInfo->stSenCfg.u32LaneNum = 2;
            pstViInfo->stViCfg.enViWorkMode = VI_WORK_MODE_NORMAL;
            break;
        case TEST_VI_OS04A10_MIPI_2688x1520_25FPS:
            pstViInfo->stSenCfg.enSenType = SEN_TYPE_OS04A10;
            pstViInfo->stSenCfg.enIntfType = VI_INTF_TYPE_MIPI;
            pstViInfo->stSenCfg.enHdrMode = HDR_MODE_NONE;
            pstViInfo->stSenCfg.u32Width = 2688;
            pstViInfo->stSenCfg.u32Height = 1520;
            pstViInfo->stSenCfg.u32Fps = 25;
            pstViInfo->stSenCfg.u32LaneNum = 2;
            pstViInfo->stViCfg.enViWorkMode = VI_WORK_MODE_NORMAL;
            break;
        case TEST_VI_OS04A10_MIPI_2688x1520_30FPS:
            pstViInfo->stSenCfg.enSenType = SEN_TYPE_OS04A10;
            pstViInfo->stSenCfg.enIntfType = VI_INTF_TYPE_MIPI;
            pstViInfo->stSenCfg.enHdrMode = HDR_MODE_NONE;
            pstViInfo->stSenCfg.u32Width = 2688;
            pstViInfo->stSenCfg.u32Height = 1520;
            pstViInfo->stSenCfg.u32Fps = 30;
            pstViInfo->stSenCfg.u32LaneNum = 2;
            pstViInfo->stViCfg.enViWorkMode = VI_WORK_MODE_NORMAL;
            break;
        case TEST_VI_OS04A10_MIPI_2688x1520_25FPS_HDR:
            pstViInfo->stSenCfg.enSenType = SEN_TYPE_OS04A10;
            pstViInfo->stSenCfg.enIntfType = VI_INTF_TYPE_MIPI;
            pstViInfo->stSenCfg.enHdrMode = HDR_MODE_2To1_LINE;
            pstViInfo->stSenCfg.u32Width = 2688;
            pstViInfo->stSenCfg.u32Height = 1520;
            pstViInfo->stSenCfg.u32Fps = 25;
            pstViInfo->stSenCfg.u32LaneNum = 2;
            pstViInfo->stViCfg.enViWorkMode = VI_WORK_MODE_NORMAL;
            break;
        case TEST_VI_OS05A10_MIPI_2688x1944_25FPS:
            pstViInfo->stSenCfg.enSenType = SEN_TYPE_OS05A10;
            pstViInfo->stSenCfg.enIntfType = VI_INTF_TYPE_MIPI;
            pstViInfo->stSenCfg.enHdrMode = HDR_MODE_NONE;
            pstViInfo->stSenCfg.u32Width = 2688;
            pstViInfo->stSenCfg.u32Height = 1944;
            pstViInfo->stSenCfg.u32Fps = 25;
            pstViInfo->stSenCfg.u32LaneNum = 2;
            pstViInfo->stViCfg.enViWorkMode = VI_WORK_MODE_NORMAL;
            break;
        case TEST_VI_OS05A10_MIPI_2688x1944_20FPS_HDR:
            pstViInfo->stSenCfg.enSenType = SEN_TYPE_OS05A10;
            pstViInfo->stSenCfg.enIntfType = VI_INTF_TYPE_MIPI;
            pstViInfo->stSenCfg.enHdrMode = HDR_MODE_2To1_LINE;
            pstViInfo->stSenCfg.u32Width = 2688;
            pstViInfo->stSenCfg.u32Height = 1944;
            pstViInfo->stSenCfg.u32Fps = 20;
            pstViInfo->stSenCfg.u32LaneNum = 2;
            pstViInfo->stViCfg.enViWorkMode = VI_WORK_MODE_NORMAL;
            break;
        case TEST_VI_OS04C10_MIPI_2688x1520_25FPS:
            pstViInfo->stSenCfg.enSenType = SEN_TYPE_OS04C10;
            pstViInfo->stSenCfg.enIntfType = VI_INTF_TYPE_MIPI;
            pstViInfo->stSenCfg.enHdrMode = HDR_MODE_NONE;
            pstViInfo->stSenCfg.u32Width = 2688;
            pstViInfo->stSenCfg.u32Height = 1520;
            pstViInfo->stSenCfg.u32Fps = 25;
            pstViInfo->stSenCfg.u32LaneNum = 2;
            pstViInfo->stViCfg.enViWorkMode = VI_WORK_MODE_NORMAL;
            break;
        case TEST_VI_OS04C10_MIPI_2688x1520_25FPS_HDR:
            pstViInfo->stSenCfg.enSenType = SEN_TYPE_OS04C10;
            pstViInfo->stSenCfg.enIntfType = VI_INTF_TYPE_MIPI;
            pstViInfo->stSenCfg.enHdrMode = HDR_MODE_2To1_LINE;
            pstViInfo->stSenCfg.u32Width = 2688;
            pstViInfo->stSenCfg.u32Height = 1520;
            pstViInfo->stSenCfg.u32Fps = 25;
            pstViInfo->stSenCfg.u32LaneNum = 2;
            pstViInfo->stViCfg.enViWorkMode = VI_WORK_MODE_NORMAL;
            break;
        case TEST_VI_SC2239P_MIPI_1080P_30FPS:
            pstViInfo->stSenCfg.enSenType = SEN_TYPE_SC2239P;
            pstViInfo->stSenCfg.enIntfType = VI_INTF_TYPE_MIPI;
            pstViInfo->stSenCfg.enHdrMode = HDR_MODE_NONE;
            pstViInfo->stSenCfg.u32Width = 1920;
            pstViInfo->stSenCfg.u32Height = 1080;
            pstViInfo->stSenCfg.u32Fps = 30;
            pstViInfo->stSenCfg.u32LaneNum = 1;
            pstViInfo->stViCfg.enViWorkMode = VI_WORK_MODE_NORMAL;
            break;
        case TEST_VI_SC2239P_MIPI_720P_60FPS:
            pstViInfo->stSenCfg.enSenType = SEN_TYPE_SC2239P;
            pstViInfo->stSenCfg.enIntfType = VI_INTF_TYPE_MIPI;
            pstViInfo->stSenCfg.enHdrMode = HDR_MODE_NONE;
            pstViInfo->stSenCfg.u32Width = 1280;
            pstViInfo->stSenCfg.u32Height = 720;
            pstViInfo->stSenCfg.u32Fps = 60;
            pstViInfo->stSenCfg.u32LaneNum = 1;
            pstViInfo->stViCfg.enViWorkMode = VI_WORK_MODE_NORMAL;
            break;
        case TEST_VI_COLOR_720P_30FPS:
            pstViInfo->stSenCfg.enSenType = SEN_TYPE_BUTT;
            pstViInfo->stSenCfg.enIntfType = VI_INTF_TYPE_DC;
            pstViInfo->stSenCfg.enHdrMode = HDR_MODE_NONE;
            pstViInfo->stSenCfg.u32Width = 1280;
            pstViInfo->stSenCfg.u32Height = 720;
            pstViInfo->stSenCfg.u32Fps = 30;
            pstViInfo->stSenCfg.u32LaneNum = 0;
            pstViInfo->stViCfg.enViWorkMode = VI_WORK_MODE_SYSCLK_COLORBAR;
            break;
        case TEST_VI_COLOR_1080P_10FPS:
            pstViInfo->stSenCfg.enSenType = SEN_TYPE_BUTT;
            pstViInfo->stSenCfg.enIntfType = VI_INTF_TYPE_DC;
            pstViInfo->stSenCfg.enHdrMode = HDR_MODE_NONE;
            pstViInfo->stSenCfg.u32Width = 1920;
            pstViInfo->stSenCfg.u32Height = 1080;
            pstViInfo->stSenCfg.u32Fps = 10;
            pstViInfo->stSenCfg.u32LaneNum = 0;
            pstViInfo->stViCfg.enViWorkMode = VI_WORK_MODE_SYSCLK_COLORBAR;
            break;
        case TEST_VI_NI9931_BT656_720P_30FPS:
            pstViInfo->stSenCfg.enSenType = SEN_TYPE_BUTT;
            pstViInfo->stSenCfg.enIntfType = VI_INTF_TYPE_BT1120;
            pstViInfo->stSenCfg.enHdrMode = HDR_MODE_NONE;
            pstViInfo->stSenCfg.u32Width = 1280;
            pstViInfo->stSenCfg.u32Height = 720;
            pstViInfo->stSenCfg.u32Fps = 30;
            pstViInfo->stSenCfg.u32LaneNum = 0;
            pstViInfo->stViCfg.enViWorkMode = VI_WORK_MODE_NORMAL;
            break;
        case TEST_VI_NI9931_BT1120_1080P_25FPS:
            pstViInfo->stSenCfg.enSenType = SEN_TYPE_BUTT;
            pstViInfo->stSenCfg.enIntfType = VI_INTF_TYPE_BT1120;
            pstViInfo->stSenCfg.enHdrMode = HDR_MODE_NONE;
            pstViInfo->stSenCfg.u32Width = 1920;
            pstViInfo->stSenCfg.u32Height = 1080;
            pstViInfo->stSenCfg.u32Fps = 25;
            pstViInfo->stSenCfg.u32LaneNum = 0;
            pstViInfo->stViCfg.enViWorkMode = VI_WORK_MODE_NORMAL;
            break;
        
        default:
            printf("not support vi mode:%d\n", enViMode);
            return NI_FAILURE;
    }

    switch(pstViInfo->stSenCfg.enSenType)
    {
        case SEN_TYPE_SC2238:
        case SEN_TYPE_SC2239P:
        case SEN_TYPE_SC200AI:
            pstViInfo->stSenCfg.u32DevAddr  = 0x60;
            pstViInfo->stSenCfg.u32I2cDevId = SenId;
            pstViInfo->stSenCfg.u32RstGpioId = SenId;
            pstViInfo->stSenCfg.u32ClkFreq = 27000;
            break;
        case SEN_TYPE_IMX307:
            pstViInfo->stSenCfg.u32DevAddr  = 0x34;
            pstViInfo->stSenCfg.u32I2cDevId = SenId;
            pstViInfo->stSenCfg.u32RstGpioId = SenId;
            pstViInfo->stSenCfg.u32ClkFreq = 37125;
            break;
        case SEN_TYPE_IMX415:
            pstViInfo->stSenCfg.u32DevAddr  = 0x34;
            pstViInfo->stSenCfg.u32I2cDevId = SenId;
            pstViInfo->stSenCfg.u32RstGpioId = SenId;
            pstViInfo->stSenCfg.u32ClkFreq = 27000;
            break;
        case SEN_TYPE_OS04A10:
        case SEN_TYPE_OS05A10:
        case SEN_TYPE_OS04C10:
            pstViInfo->stSenCfg.u32DevAddr  = 0x6c;
            pstViInfo->stSenCfg.u32I2cDevId = SenId;
            pstViInfo->stSenCfg.u32RstGpioId = SenId;
            pstViInfo->stSenCfg.u32ClkFreq = 24000;
            break;
            
        case SEN_TYPE_BUTT:
            pstViInfo->stSenCfg.u32ClkFreq = 37125;
            return NI_SUCCESS;
                
        default:
            printf("not support sensor type:%d\n", pstViInfo->stSenCfg.enSenType);
            return NI_FAILURE;
    }
    
    return NI_SUCCESS;
}

NI_S32 SDK_COMM_VI_GetDevAttr(TEST_VI_MODE_E enViMode, VI_DEV_ATTR_S *pstViDevAttr)
{
    switch(enViMode)
    {
        case TEST_VI_SC2238_DC_720P_30FPS:
            memcpy(pstViDevAttr, &gstViDevAttr_SC2238_DC_720p, sizeof(VI_DEV_ATTR_S));
            break;
        case TEST_VI_SC2238_DC_1080P_25FPS:
            memcpy(pstViDevAttr, &gstViDevAttr_SC2238_DC_1080p, sizeof(VI_DEV_ATTR_S));
            break;
        case TEST_VI_IMX307_MIPI_720P_30FPS:
            memcpy(pstViDevAttr, &gstViDevAttr_Imx307_Mipi_720p, sizeof(VI_DEV_ATTR_S));
            break;
        case TEST_VI_IMX307_MIPI_1080P_25FPS:
        case TEST_VI_IMX307_MIPI_1080P_30FPS:
            memcpy(pstViDevAttr, &gstViDevAttr_Imx307_Mipi_1080p, sizeof(VI_DEV_ATTR_S));
            break;
        case TEST_VI_IMX307_MIPI_720P_25FPS_HDR:
            memcpy(pstViDevAttr, &gstViDevAttr_Imx307_Mipi_720p_Hdr, sizeof(VI_DEV_ATTR_S));
            break;           
        case TEST_VI_IMX307_MIPI_1080P_12_5FPS_DHR:
            memcpy(pstViDevAttr, &gstViDevAttr_Imx307_Mipi_1080p_12_5fps_Hdr, sizeof(VI_DEV_ATTR_S));
            break;     
        case TEST_VI_IMX307_MIPI_1080P_3FPS_DHR:
            memcpy(pstViDevAttr, &gstViDevAttr_Imx307_Mipi_1080p_3fps_Hdr, sizeof(VI_DEV_ATTR_S));
            break;
        case TEST_VI_IMX307_MIPI_1080P_25FPS_HDR:
            memcpy(pstViDevAttr, &gstViDevAttr_Imx307_Mipi_1080p_25fps_Hdr, sizeof(VI_DEV_ATTR_S));
            break;
        case TEST_VI_IMX307_MIPI_1080P_30FPS_HDR:
            memcpy(pstViDevAttr, &gstViDevAttr_Imx307_Mipi_1080p_30fps_Hdr, sizeof(VI_DEV_ATTR_S));
            break;
        case TEST_VI_COLOR_720P_30FPS:
            memcpy(pstViDevAttr, &gstViDevAttr_COLOR_720p, sizeof(VI_DEV_ATTR_S));
            break;
        case TEST_VI_COLOR_1080P_10FPS:
            memcpy(pstViDevAttr, &gstViDevAttr_COLOR_1080p, sizeof(VI_DEV_ATTR_S));
            break;
        case TEST_VI_IMX307_MIPI_1080P_7FPS:
            memcpy(pstViDevAttr, &gstViDevAttr_Imx307_Mipi_1080p_7fps, sizeof(VI_DEV_ATTR_S));
            break;
        case TEST_VI_IMX415_MIPI_4K_25FPS:
            memcpy(pstViDevAttr, &gstViDevAttr_Imx415_Mipi_4k_25fps, sizeof(VI_DEV_ATTR_S));
            break;
        case TEST_VI_IMX415_MIPI_4K_30FPS_HDR:
            memcpy(pstViDevAttr, &gstViDevAttr_Imx415_Mipi_4k_30fps_Hdr, sizeof(VI_DEV_ATTR_S));
            break;
        case TEST_VI_NI9931_BT656_720P_30FPS:
            memcpy(pstViDevAttr, &gstViDevAttr_NI9931_720p_25fps, sizeof(VI_DEV_ATTR_S));
            break;
        case TEST_VI_NI9931_BT1120_1080P_25FPS:
            memcpy(pstViDevAttr, &gstViDevAttr_NI9931_1080p_25fps, sizeof(VI_DEV_ATTR_S));
            break;
        case TEST_VI_SC200AI_MIPI_720P_30FPS:
            memcpy(pstViDevAttr, &gstViDevAttr_SC200AI_720P_30fps, sizeof(VI_DEV_ATTR_S));
            break;
        case TEST_VI_SC200AI_MIPI_1080P_25FPS:
            memcpy(pstViDevAttr, &gstViDevAttr_SC200AI_1080P_25fps, sizeof(VI_DEV_ATTR_S));
            break;
        case TEST_VI_SC200AI_MIPI_1080P_25FPS_HDR:
            memcpy(pstViDevAttr, &gstViDevAttr_SC200AI_1080P_25fps_Hdr, sizeof(VI_DEV_ATTR_S));
            break;
        case TEST_VI_OS04A10_MIPI_2688x1520_25FPS:
        case TEST_VI_OS04A10_MIPI_2688x1520_30FPS:
            memcpy(pstViDevAttr, &gstViDevAttr_OS04A10_2688x1520_25fps, sizeof(VI_DEV_ATTR_S));
            break;
        case TEST_VI_OS05A10_MIPI_2688x1944_25FPS:
            memcpy(pstViDevAttr, &gstViDevAttr_OS05A10_2688x1944_25fps, sizeof(VI_DEV_ATTR_S));
            break;
        case TEST_VI_OS04A10_MIPI_2688x1520_25FPS_HDR:
            memcpy(pstViDevAttr, &gstViDevAttr_OS04A10_2688x1520_25fps_Hdr, sizeof(VI_DEV_ATTR_S));
            break;
        case TEST_VI_OS05A10_MIPI_2688x1944_20FPS_HDR:
            memcpy(pstViDevAttr, &gstViDevAttr_OS05A10_2688x1944_25fps_Hdr, sizeof(VI_DEV_ATTR_S));
            break;
        case TEST_VI_OS04L10_MIPI_2560x1440_30FPS:
            memcpy(pstViDevAttr, &gstViDevAttr_OS04L10_2560x1440_30fps, sizeof(VI_DEV_ATTR_S));
            break;
        case TEST_VI_OS04D10_MIPI_2560x1440_30FPS:
            memcpy(pstViDevAttr, &gstViDevAttr_OS04D10_2560x1440_30fps, sizeof(VI_DEV_ATTR_S));
            break;
        case TEST_VI_GC4023_MIPI_2560x1440_30FPS:
            memcpy(pstViDevAttr, &gstViDevAttr_GC4023_2560x1440_30fps, sizeof(VI_DEV_ATTR_S));
            break;
        case TEST_VI_GC4653_MIPI_2560x1440_30FPS:
            memcpy(pstViDevAttr, &gstViDevAttr_GC4653_2560x1440_30fps, sizeof(VI_DEV_ATTR_S));
            break;
        case TEST_VI_GC2083_MIPI_1080P_30FPS:
            memcpy(pstViDevAttr, &gstViDevAttr_GC2083_1080P_30fps, sizeof(VI_DEV_ATTR_S));
            break;
        case TEST_VI_SC2239P_MIPI_1080P_30FPS:
            memcpy(pstViDevAttr, &gstViDevAttr_SC2239P_Mipi_1080p_30fps, sizeof(VI_DEV_ATTR_S));
            break;
        case TEST_VI_SC2239P_MIPI_720P_60FPS:
            memcpy(pstViDevAttr, &gstViDevAttr_SC2239P_Mipi_720p_60fps, sizeof(VI_DEV_ATTR_S));
            break;
        case TEST_VI_OS04C10_MIPI_2688x1520_25FPS:
            memcpy(pstViDevAttr, &gstViDevAttr_OS04C10_2688x1520_25fps, sizeof(VI_DEV_ATTR_S));
            break;
        case TEST_VI_OS04C10_MIPI_2688x1520_25FPS_HDR:
            memcpy(pstViDevAttr, &gstViDevAttr_OS04C10_2688x1520_25fps_Hdr, sizeof(VI_DEV_ATTR_S));
            break;

        default:
            printf("not support vi mode:%d\n", enViMode);
            return NI_FAILURE;
    }

    return NI_SUCCESS;
}

NI_S32 SAMPEL_COMM_VI_GetMipiPara(TEST_VI_MODE_E enViMode, VI_MIPI_PARAM_S *pstViMipiPara)
{
    switch(enViMode)
    {
        case TEST_VI_IMX307_MIPI_720P_30FPS:
            memcpy(pstViMipiPara, &gstViMipiParam_Imx307_720p, sizeof(VI_MIPI_PARAM_S));
            break;
        case TEST_VI_IMX307_MIPI_1080P_25FPS:
        case TEST_VI_IMX307_MIPI_1080P_30FPS:
            memcpy(pstViMipiPara, &gstViMipiParam_Imx307_1080p, sizeof(VI_MIPI_PARAM_S));
            break;
        case TEST_VI_IMX307_MIPI_720P_25FPS_HDR:
            memcpy(pstViMipiPara, &gstViMipiParam_Imx307_720p_Hdr, sizeof(VI_MIPI_PARAM_S));
            break;
        case TEST_VI_IMX307_MIPI_1080P_12_5FPS_DHR:
            memcpy(pstViMipiPara, &gstViMipiParam_Imx307_1080p_12_5fps_Hdr, sizeof(VI_MIPI_PARAM_S));
            break;
        case TEST_VI_IMX307_MIPI_1080P_3FPS_DHR:
            memcpy(pstViMipiPara, &gstViMipiParam_Imx307_1080p_3fps_Hdr, sizeof(VI_MIPI_PARAM_S));
            break;
        case TEST_VI_IMX307_MIPI_1080P_25FPS_HDR:
            memcpy(pstViMipiPara, &gstViMipiParam_Imx307_1080p_25fps_Hdr, sizeof(VI_MIPI_PARAM_S));
            break;
        case TEST_VI_IMX307_MIPI_1080P_30FPS_HDR:
            memcpy(pstViMipiPara, &gstViMipiParam_Imx307_1080p_30fps_Hdr, sizeof(VI_MIPI_PARAM_S));
            break;
        case TEST_VI_IMX307_MIPI_1080P_7FPS:
            memcpy(pstViMipiPara, &gstViMipiParam_Imx307_1080p_7fps, sizeof(VI_MIPI_PARAM_S));
            break;
        case TEST_VI_IMX415_MIPI_4K_25FPS:
            memcpy(pstViMipiPara, &gstViMipiParam_Imx415_4k_25fps, sizeof(VI_MIPI_PARAM_S));
            break;
        case TEST_VI_IMX415_MIPI_4K_30FPS_HDR:
            memcpy(pstViMipiPara, &gstViMipiParam_Imx415_4k_30fps_Hdr, sizeof(VI_MIPI_PARAM_S));
            break;
        case TEST_VI_SC200AI_MIPI_720P_30FPS:
            memcpy(pstViMipiPara, &gstViMipiParam_SC200AI_720P_30fps, sizeof(VI_MIPI_PARAM_S));
            break;
        case TEST_VI_SC200AI_MIPI_1080P_25FPS:
            memcpy(pstViMipiPara, &gstViMipiParam_SC200AI_1080P_25fps, sizeof(VI_MIPI_PARAM_S));
            break;
        case TEST_VI_SC200AI_MIPI_1080P_25FPS_HDR:
            memcpy(pstViMipiPara, &gstViMipiParam_SC200AI_1080P_25fps_Hdr, sizeof(VI_MIPI_PARAM_S));
            break;
        case TEST_VI_OS04A10_MIPI_2688x1520_25FPS:
        case TEST_VI_OS04A10_MIPI_2688x1520_30FPS:
            memcpy(pstViMipiPara, &gstViMipiParam_OS04A10_2688x1520_25fps, sizeof(VI_MIPI_PARAM_S));
            break;
        case TEST_VI_OS05A10_MIPI_2688x1944_25FPS:
            memcpy(pstViMipiPara, &gstViMipiParam_OS05A10_2688x1944_25fps, sizeof(VI_MIPI_PARAM_S));
            break;
        case TEST_VI_OS04A10_MIPI_2688x1520_25FPS_HDR:
            memcpy(pstViMipiPara, &gstViMipiParam_OS04A10_2688x1520_25fps_Hdr, sizeof(VI_MIPI_PARAM_S));
            break;
        case TEST_VI_OS05A10_MIPI_2688x1944_20FPS_HDR:
            memcpy(pstViMipiPara, &gstViMipiParam_OS05A10_2688x1944_25fps_Hdr, sizeof(VI_MIPI_PARAM_S));
            break;
        case TEST_VI_OS04L10_MIPI_2560x1440_30FPS:
            memcpy(pstViMipiPara, &gstViMipiParam_OS04L10_2560x1440_30fps, sizeof(VI_MIPI_PARAM_S));
            break;
        case TEST_VI_OS04D10_MIPI_2560x1440_30FPS:
            memcpy(pstViMipiPara, &gstViMipiParam_OS04D10_2560x1440_30fps, sizeof(VI_MIPI_PARAM_S));
            break;
        case TEST_VI_GC4023_MIPI_2560x1440_30FPS:
            memcpy(pstViMipiPara, &gstViMipiParam_GC4023_2560x1440_30fps, sizeof(VI_MIPI_PARAM_S));
            break;
        case TEST_VI_GC4653_MIPI_2560x1440_30FPS:
            memcpy(pstViMipiPara, &gstViMipiParam_GC4653_2560x1440_30fps, sizeof(VI_MIPI_PARAM_S));
            break;
        case TEST_VI_GC2083_MIPI_1080P_30FPS:
            memcpy(pstViMipiPara, &gstViMipiParam_GC2083_1080P_30fps, sizeof(VI_MIPI_PARAM_S));
            break;
        case TEST_VI_SC2239P_MIPI_1080P_30FPS:
            memcpy(pstViMipiPara, &gstViMipiParam_SC2239P_1080p_30fps, sizeof(VI_MIPI_PARAM_S));
            break;
        case TEST_VI_SC2239P_MIPI_720P_60FPS:
            memcpy(pstViMipiPara, &gstViMipiParam_SC2239P_720p_60fps, sizeof(VI_MIPI_PARAM_S));
            break;
        case TEST_VI_OS04C10_MIPI_2688x1520_25FPS:
            memcpy(pstViMipiPara, &gstViMipiParam_OS04C10_2688x1520_25fps, sizeof(VI_MIPI_PARAM_S));
            break;
        case TEST_VI_OS04C10_MIPI_2688x1520_25FPS_HDR:
            memcpy(pstViMipiPara, &gstViMipiParam_OS04C10_2688x1520_25fps_Hdr, sizeof(VI_MIPI_PARAM_S));
            break;
        
        case TEST_VI_SC2238_DC_720P_30FPS:
        case TEST_VI_SC2238_DC_1080P_25FPS:
        case TEST_VI_COLOR_720P_30FPS:
        case TEST_VI_COLOR_1080P_10FPS:
        case TEST_VI_NI9931_BT656_720P_30FPS:
        case TEST_VI_NI9931_BT1120_1080P_25FPS:
            break;

        default:
            printf("not support vi mode:%d\n", enViMode);
            return NI_FAILURE;
    }

    return NI_SUCCESS;
}

NI_S32 SDK_COMM_VI_SetClkCfg(VI_DEV ViDev, SAMPLE_VI_INFO_S *pstViInfo)
{
    NI_S32 s32Ret;
    PM_CLOCK_CFG_S stClockCfg;

    s32Ret = NI_MDK_PM_GetClkCfg(&stClockCfg);
    if(NI_SUCCESS != s32Ret )
    {
        printf("NI_MDK_CLOCK_GetClkCfg error!\n");
        return NI_FAILURE;
    }

    stClockCfg.stVi.bSensorPClkEn = NI_TRUE;
    stClockCfg.stVi.abSensorVClkEn[ViDev] = NI_TRUE;
    stClockCfg.stVi.au32SensorVClkFreq[ViDev] = pstViInfo->stSenCfg.u32ClkFreq;

    s32Ret = NI_MDK_PM_SetClkCfg(PM_CLOCK_MASK_SET_VI_CLK, &stClockCfg);
    if(NI_SUCCESS != s32Ret )
    {
        printf("NI_MDK_PM_SetClkCfg error!\n");
        return NI_FAILURE;
    }
    
    printf("ViDev %d set clk %dKHZ success!\n", ViDev, pstViInfo->stSenCfg.u32ClkFreq);
    
    return NI_SUCCESS;
}

NI_S32 SDK_COMM_VI_Config(VI_DEV ViDev, SAMPLE_VI_INFO_S *pstViInfo)
{
    NI_S32 s32Ret = NI_SUCCESS;
    VI_DEV_ATTR_S stDevAttr;
    VI_MIPI_PARAM_S stMipiParam;
    VI_WORK_PARAM_S stWorkParam;
    VI_GET_FRM_PARAM_S stGetFrmParam;
    MDK_CHN_S stSrcChn;
    MDK_CHN_S stDestChn;

    /* set sensor clock config */
    SDK_COMM_VI_SetClkCfg(ViDev, pstViInfo);
    
    s32Ret = NI_MDK_VI_DisableDev(ViDev);
    if (s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("NI_MDK_VI_DisableDev fail");
    }

    /* init sensor config */
    if(pstViInfo->enViMode < TEST_VI_COLOR_720P_30FPS)
    {
        s32Ret = SDK_COMM_SEN_Start(ViDev, &pstViInfo->stSenCfg);
        if (s32Ret != NI_SUCCESS)
        {
            PRINT_ERROR("call SDK_COMM_SEN_Start fail,ret=%#x\n", s32Ret);
            return s32Ret;
        }
    }
    else if (TEST_VI_NI9931_BT656_720P_30FPS == pstViInfo->enViMode)
    {
        NI9931_BT656_720p_25fps_Config();
    }
    else if (TEST_VI_NI9931_BT1120_1080P_25FPS == pstViInfo->enViMode)
    {
        NI9931_BT1120_1080p_25fps_Config();
    }

    if(VI_WORK_MODE_SYSCLK_COLORBAR == pstViInfo->stViCfg.enViWorkMode)
    {
        memset(&stWorkParam,0,sizeof(VI_WORK_PARAM_S));
        stWorkParam.enWorkMode = VI_WORK_MODE_NORMAL;
        s32Ret = NI_MDK_VI_SwitchWorkMode(ViDev, &stWorkParam);
        if (s32Ret != NI_SUCCESS)
        {
            PRINT_ERROR("call NI_MDK_VI_SwitchWorkMode VI_WORK_MODE_NORMAL fail %#x!\n", s32Ret);
            return NI_FAILURE;
        }

        memset(&stWorkParam,0,sizeof(VI_WORK_PARAM_S));
        stWorkParam.bColorBarMove = NI_TRUE;
        stWorkParam.enColorBarMode = VI_COLORBAR_SQUARE;
        stWorkParam.enWorkMode = VI_WORK_MODE_SYSCLK_COLORBAR;
        s32Ret = NI_MDK_VI_SwitchWorkMode(ViDev, &stWorkParam);
        if (s32Ret != NI_SUCCESS)
        {
            PRINT_ERROR("call NI_MDK_VI_SwitchWorkMode VI_WORK_MODE_SYSCLK_COLORBAR fail %#x!\n", s32Ret);
            return NI_FAILURE;
        }
    }
    else
    {    
        memset(&stWorkParam,0,sizeof(VI_WORK_PARAM_S));
        stWorkParam.enWorkMode = pstViInfo->stViCfg.enViWorkMode;
        s32Ret = NI_MDK_VI_SwitchWorkMode(ViDev, &stWorkParam);
        if (s32Ret != NI_SUCCESS)
        {
            PRINT_ERROR("call NI_MDK_VI_SwitchWorkMode fail,ret=%#x\n", s32Ret);
            return s32Ret;
        }
    }

    s32Ret = NI_SEN_SetCallBackReportFps(0, get_frame_rate);
    if (s32Ret != NI_SUCCESS)
    {
        NI_ERROR("call NI_SEN_SetCallBackReportFps fail,ret=%#x\n", s32Ret);
        return s32Ret;
    }

    s32Ret = NI_MDK_VI_GetMipiParam(ViDev, &stMipiParam);
    if (s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("call NI_MDK_VI_GetMipiParam fail,ret=%#x\n", s32Ret);
        return s32Ret;
    }

    SAMPEL_COMM_VI_GetMipiPara(pstViInfo->enViMode,&stMipiParam);
    s32Ret = NI_MDK_VI_SetMipiParam(ViDev,&stMipiParam);
    if (s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("call NI_MDK_VI_SetMipiParam fail,ret=%#x\n", s32Ret);
        return s32Ret;
    }

    s32Ret = NI_MDK_VI_GetDevAttr(ViDev, &stDevAttr);
    if (s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("call NI_MDK_VI_GetDevAttr fail,ret=%#x\n", s32Ret);
        return s32Ret;
    }

    SDK_COMM_VI_GetDevAttr(pstViInfo->enViMode, &stDevAttr);
    
    if (VI_WORK_MODE_NONDIRECT == pstViInfo->stViCfg.enViWorkMode)
	{
        stDevAttr.bRingModeEn = NI_TRUE;
    }
	else
	{
        stDevAttr.bRingModeEn = NI_FALSE;
    }
    
    s32Ret = NI_MDK_VI_SetDevAttr(ViDev, &stDevAttr);
    if (s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("call NI_MDK_VI_SetDevAttr fail,ret=%#x\n", s32Ret);
        return s32Ret;
    }

    if(NI_TRUE == pstViInfo->stViCfg.b2Venc)
    {
        memset(&stWorkParam,0,sizeof(stWorkParam));
        stWorkParam.enWorkMode = VI_WORK_MODE_NONDIRECT;
        s32Ret = NI_MDK_VI_SwitchWorkMode(ViDev, &stWorkParam);
        if (s32Ret != NI_SUCCESS)
        {
            PRINT_ERROR("call NI_MDK_VI_EnableGetFrame fail,ret=%#x\n", s32Ret);
            return s32Ret;
        }

        s32Ret = NI_MDK_VI_GetDevAttr(ViDev, &stDevAttr);
        if (s32Ret != NI_SUCCESS)
        {
            PRINT_ERROR("call NI_MDK_VI_EnableGetFrame fail,ret=%#x\n", s32Ret);
            return s32Ret;
        }
        
        stDevAttr.bCompress = NI_FALSE;
        s32Ret = NI_MDK_VI_SetDevAttr(ViDev, &stDevAttr);
        if (s32Ret != NI_SUCCESS)
        {
            PRINT_ERROR("call NI_MDK_VI_EnableGetFrame fail,ret=%#x\n", s32Ret);
            return s32Ret;
        }
        
        stGetFrmParam.enPixelFormat = pstViInfo->stViCfg.enFmt;
        stGetFrmParam.s32Depth = 4;
        stGetFrmParam.au32Stride[0] = 0;
        stGetFrmParam.au32Stride[1] = 0;
        stGetFrmParam.au32Stride[2] = 0;
        s32Ret = NI_MDK_VI_EnableGetFrame(ViDev,&stGetFrmParam);
        if (s32Ret != NI_SUCCESS)
        {
            PRINT_ERROR("call NI_MDK_VI_EnableGetFrame fail,ret=%#x\n", s32Ret);
            return s32Ret;
        }

        /* 绑定vi通道到venc */
        stSrcChn.enModId = NI_ID_VI;
        stSrcChn.s32ChnId = 0;
        stSrcChn.s32DevId = ViDev;
        
        stDestChn.enModId = NI_ID_VENC;
        stDestChn.s32DevId = 0;
        stDestChn.s32ChnId = pstViInfo->stViCfg.VeChn;
        as32ViDestChnId[ViDev] = pstViInfo->stViCfg.VeChn;
        
        s32Ret = NI_MDK_SYS_Bind(&stSrcChn,&stDestChn);
        if (s32Ret != NI_SUCCESS)
        {
            PRINT_ERROR("call NI_MDK_SYS_Bind fail,ret=%#x\n", s32Ret);
            return s32Ret;
        }
    }

    return NI_SUCCESS;
}

NI_S32 SDK_COMM_VI_Start(VI_DEV ViDev)
{
    NI_MDK_VI_EnableDev(ViDev);
    
    return NI_SUCCESS;
}


NI_S32 SDK_COMM_VI_Stop(VI_DEV ViDev)
{
    MDK_CHN_S stSrcChn;
    MDK_CHN_S stDestChn;

    NI_MDK_VI_DisableDev(ViDev);

    NI_MDK_VI_DisableGetFrame(ViDev);

    /* 绑定vi通道到venc */
    stSrcChn.enModId = NI_ID_VI;
    stSrcChn.s32ChnId = ViDev;
    stSrcChn.s32DevId = 0;
    
    stDestChn.enModId = NI_ID_VENC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = as32ViDestChnId[ViDev];
    
    NI_MDK_SYS_UnBind(&stSrcChn,&stDestChn); 
        
    return NI_SUCCESS;
}



