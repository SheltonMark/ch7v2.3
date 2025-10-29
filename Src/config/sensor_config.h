/**
 * @file sensor_config.h
 * @brief Sensor hardware configuration table
 *
 * This module provides static configuration data for supported sensors.
 * All sensor specifications are defined here as read-only data.
 */

#ifndef SENSOR_CONFIG_H
#define SENSOR_CONFIG_H

#include "TdCommon.h"


/**
 * @brief Sensor hardware specification (read-only)
 */
typedef struct {
    ISP_SENSORTYPE_E sensor_type;  // SENSOR_GC4653, SENSOR_0S04D10, etc.
    int max_width;                 // Maximum resolution width
    int max_height;                // Maximum resolution height
    int max_fps;                   // Maximum frame rate
    int sen_type;                  // SEN_TYPE_GC4653, etc. (for driver)
    int vi_mode;                   // VI mode enum (platform-specific)
    int mirror_and_flip;           // Mirror and flip flag
    unsigned int i2c_dev_addr;     // I2C device address (7-bit)
    unsigned int clk_freq_khz;     // Sensor clock frequency in kHz
    const char *name;              // Sensor name (for logging)
} SensorConfig_t;

/**
 * @brief Get sensor configuration by sensor type
 * @param sensor_type Sensor type enum (SENSOR_GC4653, SENSOR_0S04D10, etc.)
 * @return Pointer to sensor config, or NULL if not found
 */
const SensorConfig_t* SensorConfig_Get(ISP_SENSORTYPE_E sensor_type);

/**
 * @brief Get all supported sensors
 * @param count Output parameter for number of sensors
 * @return Pointer to sensor config array
 */
const SensorConfig_t* SensorConfig_GetAll(int *count);

#endif /* SENSOR_CONFIG_H */
