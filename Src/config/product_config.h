/**
 * @file product_config.h
 * @brief Product configuration management
 *
 * This module loads product-specific configuration from /etc/product.conf
 * with fallback to built-in defaults. Provides runtime configuration data.
 */

#ifndef PRODUCT_CONFIG_H
#define PRODUCT_CONFIG_H

#include "TdCommon.h"

/**
 * @brief Hardware pinmux variant enumeration
 *
 * Different hardware variants have different pin configurations (I2C pins, GPIO, etc.)
 * Note: This is distinct from System.h's ProductType_e which identifies product models.
 */
typedef enum {
    IPC_CH7V20_HW = 0,       // CH7 v2.0 hardware pinmux configuration
    IPC_CP3V31_HW,           // CP3 v3.1 hardware pinmux configuration
    IPC_CP7V20_HW,           // CP7 v2.0 hardware pinmux configuration
    // Add more hardware variants as needed
} PinmuxVariant_e;

/**
 * @brief Product configuration structure
 *
 * Contains runtime product configuration loaded from file or defaults.
 */
typedef struct {
    PinmuxVariant_e hw_variant;  // Hardware pinmux variant
    ISP_SENSORTYPE_E sensor_type;       // Detected sensor type
    int main_max_width;                  // Main stream max width
    int main_max_height;                 // Main stream max height
    int sub_max_width;                   // Sub stream max width
    int sub_max_height;                  // Sub stream max height
    int rotation_mode;                   // Rotation: 0=0°, 1=90°, 2=180°, 3=270°
    int enable_audio;                    // Enable audio input
    int enable_motion_detect;            // Enable motion detection
    int enable_human_detect;             // Enable human detection
} ProductConfig_t;

/**
 * @brief Initialize product configuration
 *
 * Loads configuration from /etc/product.conf if available,
 * otherwise uses built-in defaults.
 *
 * @return 0 on success, -1 on error
 */
int ProductConfig_Init(void);

/**
 * @brief Get current product configuration
 * @return Pointer to product config (read-only)
 */
const ProductConfig_t* ProductConfig_Get(void);

/**
 * @brief Reload product configuration from file
 * @return 0 on success, -1 on error
 */
int ProductConfig_Reload(void);

/**
 * @brief Clean up product configuration
 */
void ProductConfig_Deinit(void);

#endif /* PRODUCT_CONFIG_H */
