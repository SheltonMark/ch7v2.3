/**
 * @file sensor_config.c
 * @brief Sensor hardware configuration table implementation
 *
 * This file contains static configuration data for all supported sensors.
 * Data extracted from MediaSetSystemInfor() in td_common.c
 */

#include "sensor_config.h"
#include <stddef.h>

/**
 * @brief Static sensor configuration table
 *
 * All sensor hardware specifications are defined here as read-only data.
 * This replaces the if-else chain in MediaSetSystemInfor() and
 * the sensor switch-case in video_pipeline.c
 */
static const SensorConfig_t g_sensor_configs[] = {
    {
        .sensor_type = SENSOR_GC4653,
        .max_width = 2560,
        .max_height = 1440,
        .max_fps = 30,
        .sen_type = SEN_TYPE_GC4653,
        .vi_mode = TEST_VI_GC4653_MIPI_2560x1440_30FPS,
        .mirror_and_flip = 1,
        .i2c_dev_addr = 0x52,
        .clk_freq_khz = 24000,
        .name = "GC4653"
    },
    {
        .sensor_type = SENSOR_GC4023,
        .max_width = 2560,
        .max_height = 1440,
        .max_fps = 30,
        .sen_type = SEN_TYPE_GC4023,
        .vi_mode = TEST_VI_GC4023_MIPI_2560x1440_30FPS,
        .mirror_and_flip = 0,
        .i2c_dev_addr = 0x52,
        .clk_freq_khz = 27000,
        .name = "GC4023"
    },
    {
        .sensor_type = SENSOR_0S04L10,
        .max_width = 2560,
        .max_height = 1440,
        .max_fps = 30,
        .sen_type = SEN_TYPE_OS04L10,
        .vi_mode = TEST_VI_OS04L10_MIPI_2560x1440_30FPS,
        .mirror_and_flip = 0,
        .i2c_dev_addr = 0x78,
        .clk_freq_khz = 24000,
        .name = "OS04L10"
    },
    {
        .sensor_type = SENSOR_0S04D10,
        .max_width = 2560,
        .max_height = 1440,
        .max_fps = 30,
        .sen_type = SEN_TYPE_OS04D10,
        .vi_mode = TEST_VI_OS04D10_MIPI_2560x1440_30FPS,
        .mirror_and_flip = 0,  /* 0=no default mirror/flip */
        .i2c_dev_addr = 0x78,
        .clk_freq_khz = 24000,
        .name = "OS04D10"
    },
    {
        .sensor_type = SENSOR_GC2083,
        .max_width = 1920,
        .max_height = 1080,
        .max_fps = 30,
        .sen_type = SEN_TYPE_GC2083,
        .vi_mode = TEST_VI_GC2083_MIPI_1080P_30FPS,
        .mirror_and_flip = 0,
        .i2c_dev_addr = 0x6e,
        .clk_freq_khz = 27000,
        .name = "GC2083"
    },
    {
        .sensor_type = SENSOR_SC3335,
        .max_width = 2304,
        .max_height = 1296,
        .max_fps = 30,
        .sen_type = -1,  // Not specified in original code
        .vi_mode = -1,   // Not specified in original code
        .mirror_and_flip = 0,
        .i2c_dev_addr = 0x60,  // Typical SC3335 address
        .clk_freq_khz = 27000,
        .name = "SC3335"
    },
    {
        .sensor_type = SENSOR_SC3336,
        .max_width = 2304,
        .max_height = 1296,
        .max_fps = 30,
        .sen_type = -1,  // Not specified in original code (same as SC3335)
        .vi_mode = -1,   // Not specified in original code (same as SC3335)
        .mirror_and_flip = 0,
        .i2c_dev_addr = 0x60,  // Typical SC3336 address (same as SC3335)
        .clk_freq_khz = 27000,
        .name = "SC3336"
    }
};

static const int g_sensor_count = sizeof(g_sensor_configs) / sizeof(g_sensor_configs[0]);

/**
 * @brief Get sensor configuration by sensor type
 * @param sensor_type Sensor type enum (SENSOR_GC4653, SENSOR_0S04D10, etc.)
 * @return Pointer to sensor config, or NULL if not found
 */
const SensorConfig_t* SensorConfig_Get(ISP_SENSORTYPE_E sensor_type)
{
    for (int i = 0; i < g_sensor_count; i++) {
        if (g_sensor_configs[i].sensor_type == sensor_type) {
            return &g_sensor_configs[i];
        }
    }
    return NULL;  // Sensor not found
}

/**
 * @brief Get all supported sensors
 * @param count Output parameter for number of sensors
 * @return Pointer to sensor config array
 */
const SensorConfig_t* SensorConfig_GetAll(int *count)
{
    if (count) {
        *count = g_sensor_count;
    }
    return g_sensor_configs;
}
