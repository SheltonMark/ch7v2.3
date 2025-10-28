/**
 * @file video_config.c
 * @brief Video Global Configuration and Infrastructure Implementation
 */

#include "video_config.h"
#include "TdCommon.h"
#include <string.h>

/* ========================================================================== */
/*                         Private Global Variables                           */
/* ========================================================================== */

/**
 * @brief Current global frame rate (private)
 * @note Access via VideoConfig_GetFrameRate() / VideoConfig_SetFrameRate()
 */
static unsigned int g_frame_rate = 30;

/**
 * @brief Sensor frame rate (private)
 * @note Access via VideoConfig_GetSensorFps() / VideoConfig_SetSensorFps()
 */
static unsigned int g_sensor_fps = 30;

/**
 * @brief OSD mutex for synchronizing OSD/Logo operations (private)
 * @note Access via VideoConfig_LockOsd() / VideoConfig_UnlockOsd()
 */
static pthread_mutex_t g_osd_lock = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief Font size table (private)
 * @note Index 0=large(64), 1=medium(32), 2=small(16)
 */
static const int g_font_size_table[] = {64, 32, 16};

/**
 * @brief Capture quality table for main stream (private)
 * @note Level 0=lowest quality, 5=highest quality
 */
static const CaptureImageQuality_t g_capture_qt_table[6] = {
	{.ImaxQP = 50, .IminQP = 40, .PmaxQP = 50, .PminQP = 42},  // Level 0
	{.ImaxQP = 50, .IminQP = 36, .PmaxQP = 50, .PminQP = 38},  // Level 1
	{.ImaxQP = 44, .IminQP = 32, .PmaxQP = 48, .PminQP = 34},  // Level 2
	{.ImaxQP = 44, .IminQP = 28, .PmaxQP = 48, .PminQP = 30},  // Level 3
	{.ImaxQP = 44, .IminQP = 20, .PmaxQP = 48, .PminQP = 20},  // Level 4
	{.ImaxQP = 44, .IminQP = 10, .PmaxQP = 48, .PminQP = 10},  // Level 5
};

/**
 * @brief Capture quality table for sub stream (private)
 * @note Sub stream uses different QP ranges
 */
static const CaptureImageQuality_t g_substream_qt_table[6] = {
	{.ImaxQP = 50, .IminQP = 20, .PmaxQP = 50, .PminQP = 20},  // Level 0
	{.ImaxQP = 50, .IminQP = 18, .PmaxQP = 50, .PminQP = 18},  // Level 1
	{.ImaxQP = 50, .IminQP = 16, .PmaxQP = 50, .PminQP = 16},  // Level 2
	{.ImaxQP = 50, .IminQP = 14, .PmaxQP = 50, .PminQP = 14},  // Level 3
	{.ImaxQP = 50, .IminQP = 12, .PmaxQP = 50, .PminQP = 12},  // Level 4
	{.ImaxQP = 50, .IminQP = 10, .PmaxQP = 50, .PminQP = 10},  // Level 5
};

/**
 * @brief Image size lookup table (private)
 * @note [0][x] = PAL sizes, [1][x] = NTSC sizes
 */
static const SIZE_S g_image_size[2][VIDEO_SIZE_NR] = {
	{ // PAL
		{704, 576},    // VIDEO_SIZE_D1
		{704, 288},    // VIDEO_SIZE_HD1
		{352, 576},    // VIDEO_SIZE_BCIF
		{352, 288},    // VIDEO_SIZE_CIF
		{176, 144},    // VIDEO_SIZE_QCIF
		{640, 480},    // VIDEO_SIZE_VGA
		{320, 240},    // VIDEO_SIZE_QVGA
		{480, 480},    // VIDEO_SIZE_SVCD
		{160, 128},    // VIDEO_SIZE_QQVGA
		{800, 592},    // VIDEO_SIZE_SVGA
		{1024, 768},   // VIDEO_SIZE_XVGA
		{1280, 800},   // VIDEO_SIZE_WXGA
		{1280, 1024},  // VIDEO_SIZE_SXGA
		{1600, 1024},  // VIDEO_SIZE_WSXGA
		{1600, 1200},  // VIDEO_SIZE_UXGA
		{1920, 1200},  // VIDEO_SIZE_WUXGA
		{240, 192},    // VIDEO_SIZE_LTF
		{1280, 720},   // VIDEO_SIZE_720P
		{1920, 1080},  // VIDEO_SIZE_1080P
		{1280, 960},   // VIDEO_SIZE_HD960
		{960, 576},    // VIDEO_SIZE_960H
		{1280, 1024},  // VIDEO_SIZE_HD1024
		{960, 1080},   // VIDEO_SIZE_HALF1080
		{2048, 1536},  // VIDEO_SIZE_QXGA
		{3840, 2160},  // VIDEO_SIZE_UHD4K
		{2688, 1520},  // VIDEO_SIZE_4M
		{2880, 1620},  // VIDEO_SIZE_5M
		{1280, 1440},  // VIDEO_SIZE_HALF4M
		{4000, 3000},  // VIDEO_SIZE_12M
		{2560, 1440},  // VIDEO_SIZE_4MQHD
		{1920, 1536},  // VIDEO_SIZE_3M
		{1440, 900},   // VIDEO_SIZE_1440_900
		{640, 368},    // VIDEO_SIZE_NHD
		{2304, 1296},  // VIDEO_SIZE_1296P
		{960, 544},    // VIDEO_SIZE_540P
	},
	{ // NTSC
		{704, 480},    // VIDEO_SIZE_D1
		{704, 240},    // VIDEO_SIZE_HD1
		{352, 480},    // VIDEO_SIZE_BCIF
		{352, 240},    // VIDEO_SIZE_CIF
		{176, 120},    // VIDEO_SIZE_QCIF
		{640, 480},    // VIDEO_SIZE_VGA
		{320, 240},    // VIDEO_SIZE_QVGA
		{480, 480},    // VIDEO_SIZE_SVCD
		{160, 128},    // VIDEO_SIZE_QQVGA
		{800, 592},    // VIDEO_SIZE_SVGA
		{1024, 768},   // VIDEO_SIZE_XVGA
		{1280, 800},   // VIDEO_SIZE_WXGA
		{1280, 1024},  // VIDEO_SIZE_SXGA
		{1600, 1024},  // VIDEO_SIZE_WSXGA
		{1600, 1200},  // VIDEO_SIZE_UXGA
		{1920, 1200},  // VIDEO_SIZE_WUXGA
		{240, 192},    // VIDEO_SIZE_LTF
		{1280, 720},   // VIDEO_SIZE_720P
		{1920, 1080},  // VIDEO_SIZE_1080P
		{1280, 960},   // VIDEO_SIZE_HD960
		{960, 480},    // VIDEO_SIZE_960H
		{1280, 1024},  // VIDEO_SIZE_HD1024
		{960, 1080},   // VIDEO_SIZE_HALF1080
		{2048, 1536},  // VIDEO_SIZE_QXGA
		{3840, 2160},  // VIDEO_SIZE_UHD4K
		{2688, 1520},  // VIDEO_SIZE_4M
		{2880, 1620},  // VIDEO_SIZE_5M
		{1280, 1440},  // VIDEO_SIZE_HALF4M
		{4000, 3000},  // VIDEO_SIZE_12M
		{2560, 1440},  // VIDEO_SIZE_4MQHD
		{1920, 1536},  // VIDEO_SIZE_3M
		{1440, 900},   // VIDEO_SIZE_1440_900
		{640, 368},    // VIDEO_SIZE_NHD
		{2304, 1296},  // VIDEO_SIZE_1296P
		{960, 544},    // VIDEO_SIZE_540P
	}
};

/* ========================================================================== */
/*                         Frame Rate Management                              */
/* ========================================================================== */

unsigned int VideoConfig_GetFrameRate(void)
{
	return g_frame_rate;
}

void VideoConfig_SetFrameRate(unsigned int fps)
{
	g_frame_rate = fps;
	PRINT_INFO("Frame rate updated: %u fps\n", fps);
}

unsigned int VideoConfig_GetSensorFps(void)
{
	return g_sensor_fps;
}

void VideoConfig_SetSensorFps(unsigned int fps)
{
	g_sensor_fps = fps;
	PRINT_INFO("Sensor fps updated: %u fps\n", fps);
}

/* ========================================================================== */
/*                      Static Configuration Tables                           */
/* ========================================================================== */

const SIZE_S* VideoConfig_GetImageSize(int vstd, video_size_t size)
{
	if (vstd < 0 || vstd > 1) {
		PRINT_ERROR("Invalid video standard: %d (must be 0=PAL or 1=NTSC)\n", vstd);
		return NULL;
	}

	if (size < 0 || size >= VIDEO_SIZE_NR) {
		PRINT_ERROR("Invalid video size: %d (must be 0-%d)\n", size, VIDEO_SIZE_NR - 1);
		return NULL;
	}

	return &g_image_size[vstd][size];
}

const CaptureImageQuality_t* VideoConfig_GetQualityTable(int level)
{
	if (level < 0 || level > 5) {
		PRINT_ERROR("Invalid quality level: %d (must be 0-5)\n", level);
		return NULL;
	}

	return &g_capture_qt_table[level];
}

const CaptureImageQuality_t* VideoConfig_GetSubstreamQualityTable(int level)
{
	if (level < 0 || level > 5) {
		PRINT_ERROR("Invalid substream quality level: %d (must be 0-5)\n", level);
		return NULL;
	}

	return &g_substream_qt_table[level];
}

int VideoConfig_GetFontSize(int index)
{
	if (index < 0 || index >= 3) {
		PRINT_ERROR("Invalid font size index: %d (must be 0-2)\n", index);
		return 0;
	}

	return g_font_size_table[index];
}

/* ========================================================================== */
/*                       OSD Synchronization                                  */
/* ========================================================================== */

void VideoConfig_LockOsd(void)
{
	int ret = pthread_mutex_lock(&g_osd_lock);
	if (ret != 0) {
		PRINT_ERROR("Failed to lock osd_lock: %d\n", ret);
	}
}

void VideoConfig_UnlockOsd(void)
{
	int ret = pthread_mutex_unlock(&g_osd_lock);
	if (ret != 0) {
		PRINT_ERROR("Failed to unlock osd_lock: %d\n", ret);
	}
}

pthread_mutex_t* VideoConfig_GetOsdLock(void)
{
	return &g_osd_lock;
}

/* ========================================================================== */
/*                       Module Initialization                                */
/* ========================================================================== */

int VideoConfig_Init(void)
{
	int ret;

	// Initialize osd_lock mutex
	ret = pthread_mutex_init(&g_osd_lock, NULL);
	if (ret != 0) {
		PRINT_ERROR("Failed to initialize osd_lock: %d\n", ret);
		return -1;
	}

	// Set default values
	g_frame_rate = 30;
	g_sensor_fps = 30;

	PRINT_INFO("Video config module initialized (frame_rate=%u, sensor_fps=%u)\n",
	           g_frame_rate, g_sensor_fps);

	return 0;
}

void VideoConfig_Deinit(void)
{
	pthread_mutex_destroy(&g_osd_lock);
	PRINT_INFO("Video config module deinitialized\n");
}
