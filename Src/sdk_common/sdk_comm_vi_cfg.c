/* 
 * sdk_comm_vi_cfg.c -- vi 配置文件 
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */

#include "ni_comm_vi.h"


VI_MIPI_PARAM_S gstViMipiParam_Imx307_720p = 
{
    MIPI_DATA_RAW12,                /* .enDataType */
    1,                              /* .u32LaneNum */
    288,                                /* .u32Freq */
    {/* .stDataTiming */
        HDR_MODE_NONE,                  /* .enHdrMode */
        MIPI_HDR_DATA_IDENTIFY_SYNCCODE,  /* .enHdrDataIdentify */
        4,                              /* .u32SyncCodeNum */
        {{0x241,0x241,0x241,0x241},{0x242,0x242,0x242,0x242},{0x244,0x244,0x244,0x244}}, /* .au16SyncCode */
        {0,0,0},                        /* .au32DataTypeCode */
        4,                              /* .u32FsetBitIndex */
        0,                              /* .u32FsetVal */
    },
    {0,1,2,3}
};

VI_MIPI_PARAM_S gstViMipiParam_Imx307_720p_Hdr = 
{
    MIPI_DATA_RAW12,                /* .enDataType */
    1,                              /* .u32LaneNum */
    594,                                /* .u32Freq */
    {/* .stDataTiming */
        HDR_MODE_2To1_LINE,                  /* .enDowMode */
        MIPI_HDR_DATA_IDENTIFY_SYNCCODE,  /* .enHdrDataIdentify */
        4,                              /* .u32SyncCodeNum */
        {{0x241,0x241,0x241,0x241},{0x242,0x242,0x242,0x242},{0x244,0x244,0x244,0x244}}, /* .au16SyncCode */
        {0,0,0},                        /* .au32DataTypeCode */
        4,                              /* .u32FsetBitIndex */
        0,                              /* .u32FsetVal */
    },
    {0,1,2,3}
};


VI_MIPI_PARAM_S gstViMipiParam_Imx307_1080p_12_5fps_Hdr = 
{
    MIPI_DATA_RAW12,                /* .enDataType */
    3,                              /* .u32LaneNum */
    450,                                /* .u32Freq */
    {/* .stDataTiming */
        HDR_MODE_2To1_LINE,                  /* .enDowMode */
        MIPI_HDR_DATA_IDENTIFY_SYNCCODE,  /* .enHdrDataIdentify */
        0,                              /* .u32SyncCodeNum */
        {{577,577,577,577},{578,578,578,578},{580,580,580,580}}, /* .au16SyncCode */
        {0,0,0},                        /* .au32DataTypeCode */
        4,                              /* .u32FsetBitIndex */
        0,                              /* .u32FsetVal */
    },
    {0,1,2,3}
};


VI_MIPI_PARAM_S gstViMipiParam_Imx307_1080p_3fps_Hdr = 
{
    MIPI_DATA_RAW12,                /* .enDataType */
    1,                              /* .u32LaneNum */
    594,                                /* .u32Freq */
    {/* .stDataTiming */
        HDR_MODE_2To1_LINE,                  /* .enDowMode */
        MIPI_HDR_DATA_IDENTIFY_SYNCCODE,  /* .enHdrDataIdentify */
        4,                              /* .u32SyncCodeNum */
        {{0x241,0x241,0x241,0x241},{0x242,0x242,0x242,0x242},{0x244,0x244,0x244,0x244}}, /* .au16SyncCode */
        {0,0,0},                        /* .au32DataTypeCode */
        4,                              /* .u32FsetBitIndex */
        0,                              /* .u32FsetVal */
    },
    {0,1,2,3}
};
    
VI_MIPI_PARAM_S gstViMipiParam_Imx307_1080p_25fps_Hdr = 
{
    MIPI_DATA_RAW12,                /* .enDataType */
    1,                              /* .u32LaneNum */
    900,                                /* .u32Freq */
    {/* .stDataTiming */
        HDR_MODE_2To1_LINE,                  /* .enDowMode */
        MIPI_HDR_DATA_IDENTIFY_SYNCCODE,  /* .enHdrDataIdentify */
        0,                              /* .u32SyncCodeNum */
        {{577,577,577,577},{578,578,578,578},{580,580,580,580}}, /* .au16SyncCode */
        {0,0,0},                        /* .au32DataTypeCode */
        4,                              /* .u32FsetBitIndex */
        0,                              /* .u32FsetVal */
    },
    {0,1,2,3}
};
        
VI_MIPI_PARAM_S gstViMipiParam_Imx307_1080p_30fps_Hdr = 
{
    MIPI_DATA_RAW10,                /* .enDataType */
    1,                              /* .u32LaneNum */
    450,                                /* .u32Freq */
    {/* .stDataTiming */
        HDR_MODE_2To1_LINE,                  /* .enDowMode */
        MIPI_HDR_DATA_IDENTIFY_SYNCCODE,  /* .enHdrDataIdentify */
        0,                              /* .u32SyncCodeNum */
        {{577,577,577,577},{578,578,578,578},{580,580,580,580}}, /* .au16SyncCode */
        {0,0,0},                        /* .au32DataTypeCode */
        4,                              /* .u32FsetBitIndex */
        0,                              /* .u32FsetVal */
    },
    {0,1,2,3}
};


VI_MIPI_PARAM_S gstViMipiParam_Imx307_1080p = 
{
    MIPI_DATA_RAW12,                /* .enDataType */
    1,                              /* .u32LaneNum */
    445,                                /* .u32Freq */
    {/* .stDataTiming */
        HDR_MODE_NONE,                  /* .enDowMode */
        MIPI_HDR_DATA_IDENTIFY_SYNCCODE,  /* .enHdrDataIdentify */
        4,                              /* .u32SyncCodeNum */
        {{0x241,0x241,0x241,0x241},{0x242,0x242,0x242,0x242},{0x244,0x244,0x244,0x244}}, /* .au16SyncCode */
        {0,0,0},                        /* .au32DataTypeCode */
        4,                              /* .u32FsetBitIndex */
        0,                              /* .u32FsetVal */
    },
    {0,1,2,3}
};

VI_MIPI_PARAM_S gstViMipiParam_Imx307_1080p_7fps = 
{
    MIPI_DATA_RAW12,                /* .enDataType */
    3,                              /* .u32LaneNum */
    223,                                /* .u32Freq */
    {/* .stDataTiming */
        HDR_MODE_NONE,                  /* .enDowMode */
        MIPI_HDR_DATA_IDENTIFY_SYNCCODE,  /* .enHdrDataIdentify */
        4,                              /* .u32SyncCodeNum */
        {{0x241,0x241,0x241,0x241},{0x242,0x242,0x242,0x242},{0x244,0x244,0x244,0x244}}, /* .au16SyncCode */
        {0,0,0},                        /* .au32DataTypeCode */
        4,                              /* .u32FsetBitIndex */
        0,                              /* .u32FsetVal */
    },
    {0,1,2,3}
};

VI_MIPI_PARAM_S gstViMipiParam_Imx415_4k_25fps = 
{
    MIPI_DATA_RAW12,                /* .enDataType */
    3,                              /* .u32LaneNum */
    1782,                                /* .u32Freq */
    {/* .stDataTiming */
        HDR_MODE_NONE,                  /* .enDowMode */
        MIPI_HDR_DATA_IDENTIFY_VIRTCHN,  /* .enHdrDataIdentify */
        4,                              /* .u32SyncCodeNum */
        {{0x241,0x241,0x241,0x241},{0x242,0x242,0x242,0x242},{0x244,0x244,0x244,0x244}}, /* .au16SyncCode */
        {0,0,0},                        /* .au32DataTypeCode */
        4,                              /* .u32FsetBitIndex */
        0,                              /* .u32FsetVal */
    },
    {0,1,2,3}
};

VI_MIPI_PARAM_S gstViMipiParam_Imx415_4k_30fps_Hdr = 
{
    MIPI_DATA_RAW12,                /* .enDataType */
    3,                              /* .u32LaneNum */
    1782,                                /* .u32Freq */
    {/* .stDataTiming */
        HDR_MODE_2To1_LINE,                  /* .enDowMode */
        MIPI_HDR_DATA_IDENTIFY_VIRTCHN,  /* .enHdrDataIdentify */
        4,                              /* .u32SyncCodeNum */
        {{0x241,0x241,0x241,0x241},{0x242,0x242,0x242,0x242},{0x244,0x244,0x244,0x244}}, /* .au16SyncCode */
        {0,0,0},                        /* .au32DataTypeCode */
        4,                              /* .u32FsetBitIndex */
        0,                              /* .u32FsetVal */
    },
    {1,0,2,3}
};

VI_MIPI_PARAM_S gstViMipiParam_SC200AI_720P_30fps = 
{
    MIPI_DATA_RAW10,                /* .enDataType */
    1,                              /* .u32LaneNum */
    371,                                /* .u32Freq */
    {/* .stDataTiming */
        HDR_MODE_NONE,                  /* .enDowMode */
        MIPI_HDR_DATA_IDENTIFY_VIRTCHN,  /* .enHdrDataIdentify */
        4,                              /* .u32SyncCodeNum */
        {{0x241,0x241,0x241,0x241},{0x242,0x242,0x242,0x242},{0x244,0x244,0x244,0x244}}, /* .au16SyncCode */
        {0,0,0},                        /* .au32DataTypeCode */
        4,                              /* .u32FsetBitIndex */
        0,                              /* .u32FsetVal */
    },
    {0,1,2,3}
};

VI_MIPI_PARAM_S gstViMipiParam_SC200AI_1080P_25fps = 
{
    MIPI_DATA_RAW10,                /* .enDataType */
    1,                              /* .u32LaneNum */
    371,                                /* .u32Freq */
    {/* .stDataTiming */
        HDR_MODE_NONE,                  /* .enDowMode */
        MIPI_HDR_DATA_IDENTIFY_VIRTCHN,  /* .enHdrDataIdentify */
        4,                              /* .u32SyncCodeNum */
        {{0x241,0x241,0x241,0x241},{0x242,0x242,0x242,0x242},{0x244,0x244,0x244,0x244}}, /* .au16SyncCode */
        {0,0,0},                        /* .au32DataTypeCode */
        4,                              /* .u32FsetBitIndex */
        0,                              /* .u32FsetVal */
    },
    {0,1,2,3}
};

VI_MIPI_PARAM_S gstViMipiParam_SC200AI_720P_30fps_Hdr = 
{
    MIPI_DATA_RAW10,                /* .enDataType */
    1,                              /* .u32LaneNum */
    371,                                /* .u32Freq */
    {/* .stDataTiming */
        HDR_MODE_2To1_LINE,                  /* .enDowMode */
        MIPI_HDR_DATA_IDENTIFY_VIRTCHN,  /* .enHdrDataIdentify */
        4,                              /* .u32SyncCodeNum */
        {{0x241,0x241,0x241,0x241},{0x242,0x242,0x242,0x242},{0x244,0x244,0x244,0x244}}, /* .au16SyncCode */
        {0,0,0},                        /* .au32DataTypeCode */
        4,                              /* .u32FsetBitIndex */
        0,                              /* .u32FsetVal */
    },
    {0,1,2,3}
};

VI_MIPI_PARAM_S gstViMipiParam_SC200AI_1080P_25fps_Hdr = 
{
    MIPI_DATA_RAW10,                /* .enDataType */
    1,                              /* .u32LaneNum */
    371,                                /* .u32Freq */
    {/* .stDataTiming */
        HDR_MODE_2To1_LINE,                  /* .enDowMode */
        MIPI_HDR_DATA_IDENTIFY_VIRTCHN,  /* .enHdrDataIdentify */
        4,                              /* .u32SyncCodeNum */
        {{0x241,0x241,0x241,0x241},{0x242,0x242,0x242,0x242},{0x244,0x244,0x244,0x244}}, /* .au16SyncCode */
        {0,0,0},                        /* .au32DataTypeCode */
        4,                              /* .u32FsetBitIndex */
        0,                              /* .u32FsetVal */
    },
    {0,1,2,3}
};


VI_MIPI_PARAM_S gstViMipiParam_OS04A10_2688x1520_25fps = 
{
    MIPI_DATA_RAW10,                /* .enDataType */
    1,                              /* .u32LaneNum */
    720,                                /* .u32Freq */
    {/* .stDataTiming */
        HDR_MODE_NONE,                  /* .enDowMode */
        MIPI_HDR_DATA_IDENTIFY_VIRTCHN,  /* .enHdrDataIdentify */
        4,                              /* .u32SyncCodeNum */
        {{0x241,0x241,0x241,0x241},{0x242,0x242,0x242,0x242},{0x244,0x244,0x244,0x244}}, /* .au16SyncCode */
        {0,0,0},                        /* .au32DataTypeCode */
        4,                              /* .u32FsetBitIndex */
        0,                              /* .u32FsetVal */
    },
    {0,1,2,3}
};

VI_MIPI_PARAM_S gstViMipiParam_OS04A10_2688x1520_25fps_Hdr = 
{
    MIPI_DATA_RAW10,                /* .enDataType */
    1,                              /* .u32LaneNum */
    1440,                                /* .u32Freq */
    {/* .stDataTiming */
        HDR_MODE_2To1_LINE,                  /* .enDowMode */
        MIPI_HDR_DATA_IDENTIFY_VIRTCHN,  /* .enHdrDataIdentify */
        4,                              /* .u32SyncCodeNum */
        {{0x241,0x241,0x241,0x241},{0x242,0x242,0x242,0x242},{0x244,0x244,0x244,0x244}}, /* .au16SyncCode */
        {0,0,0},                        /* .au32DataTypeCode */
        4,                              /* .u32FsetBitIndex */
        0,                              /* .u32FsetVal */
    },
    {1,0,2,3}
};

VI_MIPI_PARAM_S gstViMipiParam_OS05A10_2688x1944_25fps = 
{
    MIPI_DATA_RAW10,                /* .enDataType */
    1,                              /* .u32LaneNum */
    1504,                                /* .u32Freq */
    {/* .stDataTiming */
        HDR_MODE_NONE,                  /* .enDowMode */
        MIPI_HDR_DATA_IDENTIFY_VIRTCHN,  /* .enHdrDataIdentify */
        4,                              /* .u32SyncCodeNum */
        {{0x241,0x241,0x241,0x241},{0x242,0x242,0x242,0x242},{0x244,0x244,0x244,0x244}}, /* .au16SyncCode */
        {0,0,0},                        /* .au32DataTypeCode */
        4,                              /* .u32FsetBitIndex */
        0,                              /* .u32FsetVal */
    },
    {0,1,2,3}
};

VI_MIPI_PARAM_S gstViMipiParam_OS05A10_2688x1944_25fps_Hdr = 
{
    MIPI_DATA_RAW10,                /* .enDataType */
    1,                              /* .u32LaneNum */
    1504,                                /* .u32Freq */
    {/* .stDataTiming */
        HDR_MODE_2To1_LINE,                  /* .enDowMode */
        MIPI_HDR_DATA_IDENTIFY_VIRTCHN,  /* .enHdrDataIdentify */
        4,                              /* .u32SyncCodeNum */
        {{0x241,0x241,0x241,0x241},{0x242,0x242,0x242,0x242},{0x244,0x244,0x244,0x244}}, /* .au16SyncCode */
        {0,0,0},                        /* .au32DataTypeCode */
        4,                              /* .u32FsetBitIndex */
        0,                              /* .u32FsetVal */
    },
    {1,0,2,3}
};
    
VI_MIPI_PARAM_S gstViMipiParam_SC2239P_720p_60fps = 
{
    MIPI_DATA_RAW10,                /* .enDataType */
    0,                              /* .u32LaneNum */
    742,                                /* .u32Freq */
    {/* .stDataTiming */
        HDR_MODE_NONE,                  /* .enDowMode */
        MIPI_HDR_DATA_IDENTIFY_VIRTCHN,  /* .enHdrDataIdentify */
        4,                              /* .u32SyncCodeNum */
        {{0x241,0x241,0x241,0x241},{0x242,0x242,0x242,0x242},{0x244,0x244,0x244,0x244}}, /* .au16SyncCode */
        {0,0,0},                        /* .au32DataTypeCode */
        4,                              /* .u32FsetBitIndex */
        0,                              /* .u32FsetVal */
    },
    {0,1,2,3}
};

VI_MIPI_PARAM_S gstViMipiParam_SC2239P_1080p_30fps = 
{
    MIPI_DATA_RAW10,                /* .enDataType */
    0,                              /* .u32LaneNum */
    742,                                /* .u32Freq */
    {/* .stDataTiming */
        HDR_MODE_NONE,                  /* .enDowMode */
        MIPI_HDR_DATA_IDENTIFY_VIRTCHN,  /* .enHdrDataIdentify */
        4,                              /* .u32SyncCodeNum */
        {{0x241,0x241,0x241,0x241},{0x242,0x242,0x242,0x242},{0x244,0x244,0x244,0x244}}, /* .au16SyncCode */
        {0,0,0},                        /* .au32DataTypeCode */
        4,                              /* .u32FsetBitIndex */
        0,                              /* .u32FsetVal */
    },
    {0,1,2,3}
};

VI_MIPI_PARAM_S gstViMipiParam_GC4023_2560x1440_30fps =
{
    MIPI_DATA_RAW10, /* .enDataType */
    1,               /* .u32LaneNum */
    864,             /* .u32Freq */
    {
         /* .stDataTiming */
         HDR_MODE_NONE,                                                                              /* .enDowMode */
         MIPI_HDR_DATA_IDENTIFY_VIRTCHN,                                                             /* .enHdrDataIdentify */
        4,                                                                                          /* .u32SyncCodeNum */
        {{0x241, 0x241, 0x241, 0x241}, {0x242, 0x242, 0x242, 0x242}, {0x244, 0x244, 0x244, 0x244}}, /* .au16SyncCode */
        {0, 0, 0},                                                                                  /* .au32DataTypeCode */
        4,                                                                                          /* .u32FsetBitIndex */
        0,                                                                                          /* .u32FsetVal */
    },
    {0, 1, 2, 3}
};

VI_MIPI_PARAM_S gstViMipiParam_GC4653_2560x1440_30fps =
{
    MIPI_DATA_RAW10, /* .enDataType */
    1,               /* .u32LaneNum */
    648,             /* .u32Freq */
    {
        /* .stDataTiming */
        HDR_MODE_NONE,                                                                              /* .enDowMode */
        MIPI_HDR_DATA_IDENTIFY_VIRTCHN,                                                             /* .enHdrDataIdentify */
        4,                                                                                          /* .u32SyncCodeNum */
        {{0x241, 0x241, 0x241, 0x241}, {0x242, 0x242, 0x242, 0x242}, {0x244, 0x244, 0x244, 0x244}}, /* .au16SyncCode */
        {0, 0, 0},                                                                                  /* .au32DataTypeCode */
        4,                                                                                          /* .u32FsetBitIndex */
        0,                                                                                          /* .u32FsetVal */
    },
    {0, 1, 2, 3}
};

VI_MIPI_PARAM_S gstViMipiParam_OS04L10_2560x1440_30fps =
{
    MIPI_DATA_RAW10, /* .enDataType */
    1,               /* .u32LaneNum */
    720,             /* .u32Freq */
    {
        /* .stDataTiming */
        HDR_MODE_NONE,                                                                              /* .enDowMode */
        MIPI_HDR_DATA_IDENTIFY_VIRTCHN,                                                             /* .enHdrDataIdentify */
        4,                                                                                          /* .u32SyncCodeNum */
        {{0x241, 0x241, 0x241, 0x241}, {0x242, 0x242, 0x242, 0x242}, {0x244, 0x244, 0x244, 0x244}}, /* .au16SyncCode */
        {0, 0, 0},                                                                                  /* .au32DataTypeCode */
        4,                                                                                          /* .u32FsetBitIndex */
        0,                                                                                          /* .u32FsetVal */
    },
    {0, 1, 2, 3}
};

VI_MIPI_PARAM_S gstViMipiParam_OS04D10_2560x1440_30fps =
{
    MIPI_DATA_RAW10, /* .enDataType */
    1,               /* .u32LaneNum */
    720,             /* .u32Freq */
    {
        /* .stDataTiming */
        HDR_MODE_NONE,                                                                              /* .enDowMode */
        MIPI_HDR_DATA_IDENTIFY_VIRTCHN,                                                             /* .enHdrDataIdentify */
        4,                                                                                          /* .u32SyncCodeNum */
        {{0x241, 0x241, 0x241, 0x241}, {0x242, 0x242, 0x242, 0x242}, {0x244, 0x244, 0x244, 0x244}}, /* .au16SyncCode */
        {0, 0, 0},                                                                                  /* .au32DataTypeCode */
        4,                                                                                          /* .u32FsetBitIndex */
        0,                                                                                          /* .u32FsetVal */
    },
    {0, 1, 2, 3}
};

VI_MIPI_PARAM_S gstViMipiParam_GC2083_1080P_30fps = 
{
    MIPI_DATA_RAW10,  /* .enDataType */
    1,                /* .u32LaneNum */
    594,              /* .u32Freq */
    {
        /* .stDataTiming */
        HDR_MODE_NONE,                                                                              /* .enDowMode */
        MIPI_HDR_DATA_IDENTIFY_VIRTCHN,                                                             /* .enHdrDataIdentify */
        4,                                                                                          /* .u32SyncCodeNum */
        {{0x241, 0x241, 0x241, 0x241}, {0x242, 0x242, 0x242, 0x242},{0x244, 0x244, 0x244, 0x244}},            /* .au16SyncCode */
        {0, 0, 0},                                                                                    /* .au32DataTypeCode */
        4,                                                                                          /* .u32FsetBitIndex */
        0,                                                                                          /* .u32FsetVal */
    },
    {0,1,2,3}
};

VI_MIPI_PARAM_S gstViMipiParam_OS04C10_2688x1520_25fps = 
{
    MIPI_DATA_RAW10,                /* .enDataType */
    1,                              /* .u32LaneNum */
    1456,                                /* .u32Freq */
    {/* .stDataTiming */
        HDR_MODE_NONE,                  /* .enDowMode */
        MIPI_HDR_DATA_IDENTIFY_VIRTCHN,  /* .enHdrDataIdentify */
        4,                              /* .u32SyncCodeNum */
        {{0x241,0x241,0x241,0x241},{0x242,0x242,0x242,0x242},{0x244,0x244,0x244,0x244}}, /* .au16SyncCode */
        {0,0,0},                        /* .au32DataTypeCode */
        4,                              /* .u32FsetBitIndex */
        0,                              /* .u32FsetVal */
    },
    {0,1,2,3}
};

VI_MIPI_PARAM_S gstViMipiParam_OS04C10_2688x1520_25fps_Hdr = 
{
    MIPI_DATA_RAW10,                /* .enDataType */
    1,                              /* .u32LaneNum */
    1488,                                /* .u32Freq */
    {/* .stDataTiming */
        HDR_MODE_2To1_LINE,                  /* .enDowMode */
        MIPI_HDR_DATA_IDENTIFY_VIRTCHN,  /* .enHdrDataIdentify */
        4,                              /* .u32SyncCodeNum */
        {{0x241,0x241,0x241,0x241},{0x242,0x242,0x242,0x242},{0x244,0x244,0x244,0x244}}, /* .au16SyncCode */
        {0,0,0},                        /* .au32DataTypeCode */
        4,                              /* .u32FsetBitIndex */
        0,                              /* .u32FsetVal */
    },
    {1,0,2,3}
};

VI_DEV_ATTR_S gstViDevAttr_SC2238_DC_720p = 
{
    VI_INTF_TYPE_DC, //enIntfType
    0xfff,  //u32DataSelMask
    NI_FALSE,
    BAYER_SEQ_RGGB,
    YUV_SEQ_UYVY,
    {0,0,1280,720},
    {1280, 720},
    HDR_MODE_NONE,
    {
        VI_SYNC_EXTERNAL, 
        {
            4,
            {
                {VI_SYNC_VALID_START,   0,0,0,0},
                {VI_SYNC_VALID_END,     0,0,0,0},
                {VI_SYNC_INVALID_START, 0,0,0,0},
                {VI_SYNC_INVALID_END,   0,0,0,0}
            }
        },
        VI_HSYNC_VALID_HIGH,
        VI_VSYNC_VALID_LOW,
        NI_FALSE,
    },
    {
        VI_DOWN_PICK_CONTINUE,
        0,
    },
    NI_FALSE,
};

VI_DEV_ATTR_S gstViDevAttr_SC2238_DC_1080p = 
{
    VI_INTF_TYPE_DC,
    0xfff,
    NI_FALSE,
    BAYER_SEQ_RGGB,
    YUV_SEQ_UYVY,
    {0,0,1920,1080},
    {1920, 1080},
    HDR_MODE_NONE,
    {
        VI_SYNC_EXTERNAL, 
        {
            4,
            {
                {VI_SYNC_VALID_START,   0,0,0,0},
                {VI_SYNC_VALID_END,     0,0,0,0},
                {VI_SYNC_INVALID_START, 0,0,0,0},
                {VI_SYNC_INVALID_END,   0,0,0,0}
            }
        },
        VI_HSYNC_VALID_HIGH,
        VI_VSYNC_VALID_LOW,
        NI_FALSE,
    },
    {
        VI_DOWN_PICK_CONTINUE,
        0,
    },
    NI_FALSE,
};

VI_DEV_ATTR_S gstViDevAttr_COLOR_720p = 
{
    VI_INTF_TYPE_DC,
    0x3fff,
    NI_FALSE,
    BAYER_SEQ_RGGB,
    YUV_SEQ_UYVY,
    {0,0,1280,720},
    {4444, 750}, //100M
    HDR_MODE_NONE,
    {
        VI_SYNC_EXTERNAL, 
        {
            4,
            {
                {VI_SYNC_VALID_START,   0,0,0,0},
                {VI_SYNC_VALID_END,     0,0,0,0},
                {VI_SYNC_INVALID_START, 0,0,0,0},
                {VI_SYNC_INVALID_END,   0,0,0,0}
            }
        },
        VI_HSYNC_VALID_HIGH,
        VI_VSYNC_VALID_LOW,
        NI_FALSE,
    },
    {
        VI_DOWN_PICK_CONTINUE,
        0,
    },
    NI_FALSE,

};

VI_DEV_ATTR_S gstViDevAttr_COLOR_1080p = 
{
    VI_INTF_TYPE_DC,
    0x3fff,
    NI_FALSE,
    BAYER_SEQ_RGGB,
    YUV_SEQ_UYVY,
    {0,0,1920,1080},
    {8888, 1125}, //100M
    HDR_MODE_NONE,
    {
        VI_SYNC_EXTERNAL, 
        {
            4,
            {
                {VI_SYNC_VALID_START,   0,0,0,0},
                {VI_SYNC_VALID_END,     0,0,0,0},
                {VI_SYNC_INVALID_START, 0,0,0,0},
                {VI_SYNC_INVALID_END,   0,0,0,0}
            }
        },
        VI_HSYNC_VALID_HIGH,
        VI_VSYNC_VALID_LOW,
        NI_FALSE,
    },
    {
        VI_DOWN_PICK_CONTINUE,
        0,
    },
    NI_FALSE,

};



VI_DEV_ATTR_S gstViDevAttr_Imx307_Mipi_720p = 
{
    VI_INTF_TYPE_MIPI,
    0x3ff,
    NI_FALSE,
    BAYER_SEQ_RGGB,
    YUV_SEQ_UYVY,
    {0,0,1280,720},
    {1280, 720},
    HDR_MODE_NONE,
    {
        VI_SYNC_EXTERNAL, 
        {
            4,
            {
                {VI_SYNC_VALID_START,   0,0,0,0},
                {VI_SYNC_VALID_END,     0,0,0,0},
                {VI_SYNC_INVALID_START, 0,0,0,0},
                {VI_SYNC_INVALID_END,   0,0,0,0}
            }
        },
        VI_HSYNC_VALID_HIGH,
        VI_VSYNC_VALID_LOW,
        NI_FALSE,
    },
    {
        VI_DOWN_PICK_CONTINUE,
        0,
    },
    NI_FALSE,
};



VI_DEV_ATTR_S gstViDevAttr_Imx307_Mipi_720p_Hdr = 
{
    VI_INTF_TYPE_MIPI,
    0x3ff,
    NI_FALSE,
    BAYER_SEQ_RGGB,
    YUV_SEQ_UYVY,
    {0,0,1280,720},
    {1280, 720},
    HDR_MODE_2To1_LINE,
    {
        VI_SYNC_EXTERNAL, 
        {
            4,
            {
                {VI_SYNC_VALID_START,   0,0,0,0},
                {VI_SYNC_VALID_END,     0,0,0,0},
                {VI_SYNC_INVALID_START, 0,0,0,0},
                {VI_SYNC_INVALID_END,   0,0,0,0}
            }
        },
        VI_HSYNC_VALID_HIGH,
        VI_VSYNC_VALID_LOW,
        NI_FALSE,
    },
    {
        VI_DOWN_PICK_CONTINUE,
        0,
    },
    NI_FALSE,
};

VI_DEV_ATTR_S gstViDevAttr_Imx307_Mipi_1080p = 
{
    VI_INTF_TYPE_MIPI,
    0x3ff,
    NI_FALSE,
    BAYER_SEQ_RGGB,
    YUV_SEQ_UYVY,
    {0,0,1920,1080},
    {1920, 1080},
    HDR_MODE_NONE,
    {
        VI_SYNC_EXTERNAL, 
        {
            4,
            {
                {VI_SYNC_VALID_START,   0,0,0,0},
                {VI_SYNC_VALID_END,     0,0,0,0},
                {VI_SYNC_INVALID_START, 0,0,0,0},
                {VI_SYNC_INVALID_END,   0,0,0,0}
            }
        },
        VI_HSYNC_VALID_HIGH,
        VI_VSYNC_VALID_LOW,
        NI_FALSE,
    },
    {
        VI_DOWN_PICK_CONTINUE,
        0,
    },
    NI_FALSE,
};

VI_DEV_ATTR_S gstViDevAttr_Imx307_Mipi_1080p_12_5fps_Hdr = 
{
    VI_INTF_TYPE_MIPI,
    4095,
    NI_FALSE,
    BAYER_SEQ_RGGB,
    YUV_SEQ_UYVY,
    {0,0,1920,1080},
    {1920, 1080},
    HDR_MODE_2To1_LINE,
    {
        VI_SYNC_EXTERNAL, 
        {
            4,
            {
                {VI_SYNC_VALID_START,   0xffff,0,0,32896},
                {VI_SYNC_VALID_END,     0xffff,0,0,40349},
                {VI_SYNC_INVALID_START, 0xffff,0,0,43947},
                {VI_SYNC_INVALID_END,   0xffff,0,0,46774}
            }
        },
        VI_HSYNC_VALID_LOW,
        VI_VSYNC_VALID_LOW,
        NI_FALSE,
    },
    {
        VI_DOWN_PICK_CONTINUE,
        0,
    },
    NI_FALSE,
};



VI_DEV_ATTR_S gstViDevAttr_Imx307_Mipi_1080p_3fps_Hdr = 
{
    VI_INTF_TYPE_MIPI,
    0x3ff,
    NI_FALSE,
    BAYER_SEQ_RGGB,
    YUV_SEQ_UYVY,
    {0,0,1920,1080},
    {1920, 1080},
    HDR_MODE_2To1_LINE,
    {
        VI_SYNC_EXTERNAL, 
        {
            4,
            {
                {VI_SYNC_VALID_START,   0,0,0,0},
                {VI_SYNC_VALID_END,     0,0,0,0},
                {VI_SYNC_INVALID_START, 0,0,0,0},
                {VI_SYNC_INVALID_END,   0,0,0,0}
            }
        },
        VI_HSYNC_VALID_HIGH,
        VI_VSYNC_VALID_LOW,
        NI_FALSE,
    },
    {
        VI_DOWN_PICK_CONTINUE,
        0,
    },
    NI_FALSE,
};

VI_DEV_ATTR_S gstViDevAttr_Imx307_Mipi_1080p_25fps_Hdr = 
{
    VI_INTF_TYPE_MIPI,
    4095,
    NI_FALSE,
    BAYER_SEQ_RGGB,
    YUV_SEQ_UYVY,
    {0,0,1920,1080},
    {1920, 1080},
    HDR_MODE_2To1_LINE,
    {
        VI_SYNC_EXTERNAL, 
        {
            4,
            {
                {VI_SYNC_VALID_START,   0xffff,0,0,32896},
                {VI_SYNC_VALID_END,     0xffff,0,0,40349},
                {VI_SYNC_INVALID_START, 0xffff,0,0,43947},
                {VI_SYNC_INVALID_END,   0xffff,0,0,46774}
            }
        },
        VI_HSYNC_VALID_LOW,
        VI_VSYNC_VALID_LOW,
        NI_FALSE,
    },
    {
        VI_DOWN_PICK_CONTINUE,
        0,
    },
    NI_FALSE,
};

VI_DEV_ATTR_S gstViDevAttr_Imx307_Mipi_1080p_30fps_Hdr = 
{
    VI_INTF_TYPE_MIPI,
    4095,
    NI_FALSE,
    BAYER_SEQ_RGGB,
    YUV_SEQ_UYVY,
    {0,0,1920,1080},
    {1920, 1080},
    HDR_MODE_2To1_LINE,
    {
        VI_SYNC_EXTERNAL, 
        {
            4,
            {
                {VI_SYNC_VALID_START,   0xffff,0,0,32896},
                {VI_SYNC_VALID_END,     0xffff,0,0,40349},
                {VI_SYNC_INVALID_START, 0xffff,0,0,43947},
                {VI_SYNC_INVALID_END,   0xffff,0,0,46774}
            }
        },
        VI_HSYNC_VALID_LOW,
        VI_VSYNC_VALID_LOW,
        NI_FALSE,
    },
    {
        VI_DOWN_PICK_CONTINUE,
        0,
    },
    NI_FALSE,
};



VI_DEV_ATTR_S gstViDevAttr_Imx307_Mipi_1080p_7fps = 
{
    VI_INTF_TYPE_MIPI,
    0x3ff,
    NI_FALSE,
    BAYER_SEQ_RGGB,
    YUV_SEQ_UYVY,
    {0,0,1920,1080},
    {1920, 1080},
    HDR_MODE_NONE,
    {
        VI_SYNC_EXTERNAL, 
        {
            4,
            {
                {VI_SYNC_VALID_START,   0,0,0,0},
                {VI_SYNC_VALID_END,     0,0,0,0},
                {VI_SYNC_INVALID_START, 0,0,0,0},
                {VI_SYNC_INVALID_END,   0,0,0,0}
            }
        },
        VI_HSYNC_VALID_HIGH,
        VI_VSYNC_VALID_LOW,
        NI_FALSE,
    },
    {
        VI_DOWN_PICK_CONTINUE,
        0,
    },
    NI_FALSE,
};

VI_DEV_ATTR_S gstViDevAttr_Imx415_Mipi_4k_25fps = 
{
    VI_INTF_TYPE_MIPI,
    0x3ff,
    NI_FALSE,
    BAYER_SEQ_RGGB,
    YUV_SEQ_UYVY,
    {0,0,3840,2160},
    {3840, 2160},
    HDR_MODE_NONE,
    {
        VI_SYNC_EXTERNAL, 
        {
            4,
            {
                {VI_SYNC_VALID_START,   0,0,0,0},
                {VI_SYNC_VALID_END,     0,0,0,0},
                {VI_SYNC_INVALID_START, 0,0,0,0},
                {VI_SYNC_INVALID_END,   0,0,0,0}
            }
        },
        VI_HSYNC_VALID_HIGH,
        VI_VSYNC_VALID_LOW,
        NI_FALSE,
    },
    {
        VI_DOWN_PICK_CONTINUE,
        0,
    },
    NI_FALSE,
};

VI_DEV_ATTR_S gstViDevAttr_Imx415_Mipi_4k_30fps_Hdr = 
{
    VI_INTF_TYPE_MIPI,
    0x3ff,
    NI_FALSE,
    BAYER_SEQ_RGGB,
    YUV_SEQ_UYVY,
    {0,0,3840,2160},
    {3840, 2160},
    HDR_MODE_2To1_LINE,
    {
        VI_SYNC_EXTERNAL, 
        {
            4,
            {
                {VI_SYNC_VALID_START,   0,0,0,0},
                {VI_SYNC_VALID_END,     0,0,0,0},
                {VI_SYNC_INVALID_START, 0,0,0,0},
                {VI_SYNC_INVALID_END,   0,0,0,0}
            }
        },
        VI_HSYNC_VALID_HIGH,
        VI_VSYNC_VALID_LOW,
        NI_FALSE,
    },
    {
        VI_DOWN_PICK_CONTINUE,
        0,
    },
    NI_FALSE,
};

VI_DEV_ATTR_S gstViDevAttr_NI9931_720p_25fps = 
{
    VI_INTF_TYPE_BT1120,
    0x00ff,
    NI_TRUE,
    BAYER_SEQ_RGGB,
    YUV_SEQ_UYVY,
    {0,0,1280,720},
    {1280, 720},
    HDR_MODE_NONE,
    {
        VI_SYNC_EXTERNAL, 
        {
            4,
            {
                {VI_SYNC_VALID_START,   0,0,0,0},
                {VI_SYNC_VALID_END,     0,0,0,0},
                {VI_SYNC_INVALID_START, 0,0,0,0},
                {VI_SYNC_INVALID_END,   0,0,0,0}
            }
        },
        VI_HSYNC_VALID_HIGH,
        VI_VSYNC_VALID_LOW,
        NI_TRUE,
    },
    {
        VI_DOWN_PICK_CONTINUE,
        0,
    },
    NI_FALSE,
};

VI_DEV_ATTR_S gstViDevAttr_NI9931_1080p_25fps = 
{
    VI_INTF_TYPE_BT1120,
    0xffff,
    NI_TRUE,
    BAYER_SEQ_RGGB,
    YUV_SEQ_UYVY,
    {0,0,1920,1080},
    {1920, 1080},
    HDR_MODE_NONE,
    {
        VI_SYNC_EXTERNAL, 
        {
            4,
            {
                {VI_SYNC_VALID_START,   0,0,0,0},
                {VI_SYNC_VALID_END,     0,0,0,0},
                {VI_SYNC_INVALID_START, 0,0,0,0},
                {VI_SYNC_INVALID_END,   0,0,0,0}
            }
        },
        VI_HSYNC_VALID_HIGH,
        VI_VSYNC_VALID_LOW,
        NI_TRUE,
    },
    {
        VI_DOWN_PICK_CONTINUE,
        0,
    },
    NI_FALSE,
};


VI_DEV_ATTR_S gstViDevAttr_SC200AI_720P_30fps = 
{
    VI_INTF_TYPE_MIPI,
    0x3ff,
    NI_FALSE,
    BAYER_SEQ_BGGR,
    YUV_SEQ_UYVY,
    {0,0,1280,720},
    {1280, 720},
    HDR_MODE_NONE,
    {
        VI_SYNC_EXTERNAL, 
        {
            4,
            {
                {VI_SYNC_VALID_START,   0,0,0,0},
                {VI_SYNC_VALID_END,     0,0,0,0},
                {VI_SYNC_INVALID_START, 0,0,0,0},
                {VI_SYNC_INVALID_END,   0,0,0,0}
            }
        },
        VI_HSYNC_VALID_HIGH,
        VI_VSYNC_VALID_LOW,
        NI_FALSE,
    },
    {
        VI_DOWN_PICK_CONTINUE,
        0,
    },
    NI_FALSE,
};

VI_DEV_ATTR_S gstViDevAttr_SC200AI_1080P_25fps = 
{
    VI_INTF_TYPE_MIPI,
    0x3ff,
    NI_FALSE,
    BAYER_SEQ_BGGR,
    YUV_SEQ_UYVY,
    {0,0,1920,1080},
    {1920, 1080},
    HDR_MODE_NONE,
    {
        VI_SYNC_EXTERNAL, 
        {
            4,
            {
                {VI_SYNC_VALID_START,   0,0,0,0},
                {VI_SYNC_VALID_END,     0,0,0,0},
                {VI_SYNC_INVALID_START, 0,0,0,0},
                {VI_SYNC_INVALID_END,   0,0,0,0}
            }
        },
        VI_HSYNC_VALID_HIGH,
        VI_VSYNC_VALID_LOW,
        NI_FALSE,
    },
    {
        VI_DOWN_PICK_CONTINUE,
        0,
    },
    NI_FALSE,
};

VI_DEV_ATTR_S gstViDevAttr_SC200AI_720P_30fps_Hdr = 
{
    VI_INTF_TYPE_MIPI,
    0x3ff,
    NI_FALSE,
    BAYER_SEQ_BGGR,
    YUV_SEQ_UYVY,
    {0,0,1280,720},
    {1280, 720},
    HDR_MODE_2To1_LINE,
    {
        VI_SYNC_EXTERNAL, 
        {
            4,
            {
                {VI_SYNC_VALID_START,   0,0,0,0},
                {VI_SYNC_VALID_END,     0,0,0,0},
                {VI_SYNC_INVALID_START, 0,0,0,0},
                {VI_SYNC_INVALID_END,   0,0,0,0}
            }
        },
        VI_HSYNC_VALID_HIGH,
        VI_VSYNC_VALID_LOW,
        NI_FALSE,
    },
    {
        VI_DOWN_PICK_CONTINUE,
        0,
    },
    NI_FALSE,
};

VI_DEV_ATTR_S gstViDevAttr_SC200AI_1080P_25fps_Hdr = 
{
    VI_INTF_TYPE_MIPI,
    0x3ff,
    NI_FALSE,
    BAYER_SEQ_BGGR,
    YUV_SEQ_UYVY,
    {0,0,1920,1080},
    {1920, 1080},
    HDR_MODE_2To1_LINE,
    {
        VI_SYNC_EXTERNAL, 
        {
            4,
            {
                {VI_SYNC_VALID_START,   0,0,0,0},
                {VI_SYNC_VALID_END,     0,0,0,0},
                {VI_SYNC_INVALID_START, 0,0,0,0},
                {VI_SYNC_INVALID_END,   0,0,0,0}
            }
        },
        VI_HSYNC_VALID_HIGH,
        VI_VSYNC_VALID_LOW,
        NI_FALSE,
    },
    {
        VI_DOWN_PICK_CONTINUE,
        0,
    },
    NI_FALSE,
};

VI_DEV_ATTR_S gstViDevAttr_OS04A10_2688x1520_25fps = 
{
    VI_INTF_TYPE_MIPI,
    0x3ff,
    NI_FALSE,
    BAYER_SEQ_BGGR,
    YUV_SEQ_UYVY,
    {0,0,2688,1520},
    {2688, 1520},
    HDR_MODE_NONE,
    {
        VI_SYNC_EXTERNAL, 
        {
            4,
            {
                {VI_SYNC_VALID_START,   0,0,0,0},
                {VI_SYNC_VALID_END,     0,0,0,0},
                {VI_SYNC_INVALID_START, 0,0,0,0},
                {VI_SYNC_INVALID_END,   0,0,0,0}
            }
        },
        VI_HSYNC_VALID_HIGH,
        VI_VSYNC_VALID_LOW,
        NI_FALSE,
    },
    {
        VI_DOWN_PICK_CONTINUE,
        0,
    },
    NI_FALSE,
};

VI_DEV_ATTR_S gstViDevAttr_OS04A10_2688x1520_25fps_Hdr = 
{
    VI_INTF_TYPE_MIPI,
    0x3ff,
    NI_FALSE,
    BAYER_SEQ_BGGR,
    YUV_SEQ_UYVY,
    {0,0,2688,1520},
    {2688, 1520},
    HDR_MODE_2To1_LINE,
    {
        VI_SYNC_EXTERNAL, 
        {
            4,
            {
                {VI_SYNC_VALID_START,   0,0,0,0},
                {VI_SYNC_VALID_END,     0,0,0,0},
                {VI_SYNC_INVALID_START, 0,0,0,0},
                {VI_SYNC_INVALID_END,   0,0,0,0}
            }
        },
        VI_HSYNC_VALID_HIGH,
        VI_VSYNC_VALID_LOW,
        NI_FALSE,
    },
    {
        VI_DOWN_PICK_CONTINUE,
        0,
    },
    NI_FALSE,
};

VI_DEV_ATTR_S gstViDevAttr_OS05A10_2688x1944_25fps = 
{
    VI_INTF_TYPE_MIPI,
    0x3ff,
    NI_FALSE,
    BAYER_SEQ_BGGR,
    YUV_SEQ_UYVY,
    {0,0,2688,1944},
    {2688, 1944},
    HDR_MODE_NONE,
    {
        VI_SYNC_EXTERNAL, 
        {
            4,
            {
                {VI_SYNC_VALID_START,   0,0,0,0},
                {VI_SYNC_VALID_END,     0,0,0,0},
                {VI_SYNC_INVALID_START, 0,0,0,0},
                {VI_SYNC_INVALID_END,   0,0,0,0}
            }
        },
        VI_HSYNC_VALID_HIGH,
        VI_VSYNC_VALID_LOW,
        NI_FALSE,
    },
    {
        VI_DOWN_PICK_CONTINUE,
        0,
    },
    NI_FALSE,
};

VI_DEV_ATTR_S gstViDevAttr_OS05A10_2688x1944_25fps_Hdr = 
{
    VI_INTF_TYPE_MIPI,
    0x3ff,
    NI_FALSE,
    BAYER_SEQ_BGGR,
    YUV_SEQ_UYVY,
    {0,0,2688,1944},
    {2688, 1944},
    HDR_MODE_2To1_LINE,
    {
        VI_SYNC_EXTERNAL, 
        {
            4,
            {
                {VI_SYNC_VALID_START,   0,0,0,0},
                {VI_SYNC_VALID_END,     0,0,0,0},
                {VI_SYNC_INVALID_START, 0,0,0,0},
                {VI_SYNC_INVALID_END,   0,0,0,0}
            }
        },
        VI_HSYNC_VALID_HIGH,
        VI_VSYNC_VALID_LOW,
        NI_FALSE,
    },
    {
        VI_DOWN_PICK_CONTINUE,
        0,
    },
    NI_FALSE,
};

VI_DEV_ATTR_S gstViDevAttr_SC2239P_Mipi_720p_60fps = 
{
    VI_INTF_TYPE_MIPI,
    0x3ff,
    NI_FALSE,
    BAYER_SEQ_BGGR,
    YUV_SEQ_UYVY,
    {0,0,1280,720},
    {1280, 720},
    HDR_MODE_NONE,
    {
        VI_SYNC_EXTERNAL, 
        {
            4,
            {
                {VI_SYNC_VALID_START,   0,0,0,0},
                {VI_SYNC_VALID_END,     0,0,0,0},
                {VI_SYNC_INVALID_START, 0,0,0,0},
                {VI_SYNC_INVALID_END,   0,0,0,0}
            }
        },
        VI_HSYNC_VALID_HIGH,
        VI_VSYNC_VALID_LOW,
        NI_FALSE,
    },
    {
        VI_DOWN_PICK_CONTINUE,
        0,
    },
    NI_FALSE,
};

VI_DEV_ATTR_S gstViDevAttr_SC2239P_Mipi_1080p_30fps = 
{
    VI_INTF_TYPE_MIPI,
    0x3ff,
    NI_FALSE,
    BAYER_SEQ_BGGR,
    YUV_SEQ_UYVY,
    {0,0,1920,1080},
    {1920, 1080},
    HDR_MODE_NONE,
    {
        VI_SYNC_EXTERNAL, 
        {
            4,
            {
                {VI_SYNC_VALID_START,   0,0,0,0},
                {VI_SYNC_VALID_END,     0,0,0,0},
                {VI_SYNC_INVALID_START, 0,0,0,0},
                {VI_SYNC_INVALID_END,   0,0,0,0}
            }
        },
        VI_HSYNC_VALID_HIGH,
        VI_VSYNC_VALID_LOW,
        NI_FALSE,
    },
    {
        VI_DOWN_PICK_CONTINUE,
        0,
    },
    NI_FALSE,
};

VI_DEV_ATTR_S gstViDevAttr_GC4023_2560x1440_30fps =
{
    VI_INTF_TYPE_MIPI,
    0x3ff,
    NI_FALSE,
    BAYER_SEQ_RGGB,
    YUV_SEQ_UYVY,
    {0, 0, 2560, 1440},
    {2560, 1440},
    HDR_MODE_NONE,
    {
        VI_SYNC_EXTERNAL,
        {
            4,
            {
                {VI_SYNC_VALID_START,   0,0,0,0},
                {VI_SYNC_VALID_END,     0,0,0,0},
                {VI_SYNC_INVALID_START, 0,0,0,0},
                {VI_SYNC_INVALID_END,   0,0,0,0}
            }
        },
        VI_HSYNC_VALID_HIGH,
        VI_VSYNC_VALID_LOW,
        NI_FALSE,
    },
    {
        VI_DOWN_PICK_CONTINUE,
        0,
    },
    NI_FALSE,
};

VI_DEV_ATTR_S gstViDevAttr_GC4653_2560x1440_30fps =
{
    VI_INTF_TYPE_MIPI,
    0x3ff,
    NI_FALSE,
    BAYER_SEQ_GRBG,
    YUV_SEQ_UYVY,
    {0, 0, 2560, 1440},
    {2560, 1440},
    HDR_MODE_NONE,
    {
        VI_SYNC_EXTERNAL,
        {
            4,
            {
                {VI_SYNC_VALID_START,   0,0,0,0},
                {VI_SYNC_VALID_END,     0,0,0,0},
                {VI_SYNC_INVALID_START, 0,0,0,0},
                {VI_SYNC_INVALID_END,   0,0,0,0}
            }
        },
        VI_HSYNC_VALID_HIGH,
        VI_VSYNC_VALID_LOW,
        NI_FALSE,
    },
    {
        VI_DOWN_PICK_CONTINUE,
        0,
    },
    NI_FALSE,
};

VI_DEV_ATTR_S gstViDevAttr_OS04L10_2560x1440_30fps =
{
    VI_INTF_TYPE_MIPI,
    0x3ff,
    NI_FALSE,
    BAYER_SEQ_BGGR,
    YUV_SEQ_UYVY,
    {0, 0, 2560, 1440},
    {2560, 1440},
    HDR_MODE_NONE,
    {
        VI_SYNC_EXTERNAL,
        {
            4,
            {
                {VI_SYNC_VALID_START,   0,0,0,0},
                {VI_SYNC_VALID_END,     0,0,0,0},
                {VI_SYNC_INVALID_START, 0,0,0,0},
                {VI_SYNC_INVALID_END,   0,0,0,0}
            }
        },
        VI_HSYNC_VALID_HIGH,
        VI_VSYNC_VALID_LOW,
        NI_FALSE,
    },
    {
        VI_DOWN_PICK_CONTINUE,
        0,
    },
    NI_FALSE,
};

VI_DEV_ATTR_S gstViDevAttr_OS04D10_2560x1440_30fps =
{
    VI_INTF_TYPE_MIPI,
    0x3ff,
    NI_FALSE,
    BAYER_SEQ_RGGB,
    YUV_SEQ_UYVY,
    {0,0,2560,1440},
    {2560, 1440},
    HDR_MODE_NONE,
    {
        VI_SYNC_EXTERNAL,
        {
            4,
            {
                {VI_SYNC_VALID_START,   0,0,0,0},
                {VI_SYNC_VALID_END,     0,0,0,0},
                {VI_SYNC_INVALID_START, 0,0,0,0},
                {VI_SYNC_INVALID_END,   0,0,0,0}
            }
        },
        VI_HSYNC_VALID_HIGH,
        VI_VSYNC_VALID_LOW,
        NI_FALSE,
    },
    {
        VI_DOWN_PICK_CONTINUE,
        0,
    },
    NI_FALSE,
};

VI_DEV_ATTR_S gstViDevAttr_GC2083_1080P_30fps = 
{
    VI_INTF_TYPE_MIPI,
    0x3ff,
    NI_FALSE,
    BAYER_SEQ_RGGB,
    YUV_SEQ_UYVY,
    {0, 0, 1920, 1080},
    {1920, 1080},
    HDR_MODE_NONE,
    {
        VI_SYNC_EXTERNAL, 
        {
            4,
            {
                {VI_SYNC_VALID_START,   0,0,0,0},
                {VI_SYNC_VALID_END,     0,0,0,0},
                {VI_SYNC_INVALID_START, 0,0,0,0},
                {VI_SYNC_INVALID_END,   0,0,0,0}
            }
        },
        VI_HSYNC_VALID_HIGH,
        VI_VSYNC_VALID_LOW,
        NI_FALSE,
    },
    {
        VI_DOWN_PICK_CONTINUE,
        0,
    },
    NI_FALSE,
};	

VI_DEV_ATTR_S gstViDevAttr_OS04C10_2688x1520_25fps = 
{
    VI_INTF_TYPE_MIPI,
    0x3ff,
    NI_FALSE,
    BAYER_SEQ_GBRG,
    YUV_SEQ_UYVY,
    {0,0,2688,1520},
    {2688, 1520},
    HDR_MODE_NONE,
    {
        VI_SYNC_EXTERNAL, 
        {
            4,
            {
                {VI_SYNC_VALID_START,   0,0,0,0},
                {VI_SYNC_VALID_END,     0,0,0,0},
                {VI_SYNC_INVALID_START, 0,0,0,0},
                {VI_SYNC_INVALID_END,   0,0,0,0}
            }
        },
        VI_HSYNC_VALID_HIGH,
        VI_VSYNC_VALID_LOW,
        NI_FALSE,
    },
    {
        VI_DOWN_PICK_CONTINUE,
        0,
    },
    NI_FALSE,
};

VI_DEV_ATTR_S gstViDevAttr_OS04C10_2688x1520_25fps_Hdr = 
{
    VI_INTF_TYPE_MIPI,
    0x3ff,
    NI_FALSE,
    BAYER_SEQ_GBRG,
    YUV_SEQ_UYVY,
    {0,0,2688,1520},
    {2688, 1520},
    HDR_MODE_2To1_LINE,
    {
        VI_SYNC_EXTERNAL, 
        {
            4,
            {
                {VI_SYNC_VALID_START,   0,0,0,0},
                {VI_SYNC_VALID_END,     0,0,0,0},
                {VI_SYNC_INVALID_START, 0,0,0,0},
                {VI_SYNC_INVALID_END,   0,0,0,0}
            }
        },
        VI_HSYNC_VALID_HIGH,
        VI_VSYNC_VALID_LOW,
        NI_FALSE,
    },
    {
        VI_DOWN_PICK_CONTINUE,
        0,
    },
    NI_FALSE,
};

