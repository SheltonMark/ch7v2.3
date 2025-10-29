/**
 * @file product_config.c
 * @brief Product configuration management implementation
 *
 * Loads configuration from /etc/product.conf using simple key=value format.
 * Falls back to built-in defaults if file is not available.
 *
 * Configuration file search order:
 * 1. ./product.conf (current directory, for development)
 * 2. /etc/product.conf (production deployment)
 * 3. Built-in defaults (if no config file found)
 */

#include "product_config.h"
#include "PrintGrade.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Configuration file paths - checked in order */
#define PRODUCT_CONFIG_FILE_DEV  "./product.conf"      /* Development: current directory */
#define PRODUCT_CONFIG_FILE_PROD "/etc/product.conf"   /* Production: system config */
#define MAX_LINE_LEN 256

/* Compile-time default product variant (can be overridden by -DDEFAULT_HW_VARIANT) */
#ifndef DEFAULT_HW_VARIANT
#define DEFAULT_HW_VARIANT IPC_CH7V20_HW
#endif

/**
 * @brief Built-in default configuration
 *
 * The hw_variant can be set at compile time using -DDEFAULT_HW_VARIANT=xxx
 * Example: make CFLAGS="-DDEFAULT_HW_VARIANT=IPC_CP7V20_HW"
 */
static ProductConfig_t g_product_config = {
    .hw_variant = DEFAULT_HW_VARIANT,  // Compile-time configurable default
    .sensor_type = SENSOR_0S04D10,     // Default sensor (from td_common.c:248)
    .main_max_width = 2560,
    .main_max_height = 1440,
    .sub_max_width = 640,
    .sub_max_height = 368,
    .rotation_mode = 0,                // No rotation
    .enable_audio = 1,                 // Audio enabled
    .enable_motion_detect = 1,         // Motion detection enabled
    .enable_human_detect = 1           // Human detection enabled
};

static int g_config_initialized = 0;

/**
 * @brief Trim whitespace from string
 */
static char* trim_whitespace(char *str)
{
    char *end;

    // Trim leading space
    while (*str == ' ' || *str == '\t') str++;

    if (*str == 0) return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) {
        end--;
    }
    end[1] = '\0';

    return str;
}

/**
 * @brief Parse sensor type from string
 */
static ISP_SENSORTYPE_E parse_sensor_type(const char *value)
{
    if (strcmp(value, "GC4653") == 0) return SENSOR_GC4653;
    if (strcmp(value, "GC4023") == 0) return SENSOR_GC4023;
    if (strcmp(value, "OS04L10") == 0) return SENSOR_0S04L10;
    if (strcmp(value, "OS04D10") == 0) return SENSOR_0S04D10;
    if (strcmp(value, "GC2083") == 0) return SENSOR_GC2083;
    if (strcmp(value, "SC3335") == 0) return SENSOR_SC3335;
    if (strcmp(value, "SC3336") == 0) return SENSOR_SC3336;

    PRINT_WARN("Unknown sensor type: %s, using default\n", value);
    return SENSOR_0S04D10;  // Default
}

/**
 * @brief Parse hardware variant from string
 */
static PinmuxVariant_e parse_hw_variant(const char *value)
{
    if (strcmp(value, "ch7v20") == 0) return IPC_CH7V20_HW;
    if (strcmp(value, "cp3v31") == 0) return IPC_CP3V31_HW;
    if (strcmp(value, "cp7v20") == 0) return IPC_CP7V20_HW;

    PRINT_WARN("Unknown hw_variant: %s, using default (ch7v20)\n", value);
    return IPC_CH7V20_HW;  // Default
}

/**
 * @brief Load configuration from file
 */
static int load_config_file(const char *filepath)
{
    FILE *fp = fopen(filepath, "r");
    if (!fp) {
        PRINT_WARN("Cannot open %s, using defaults\n", filepath);
        return -1;
    }

    char line[MAX_LINE_LEN];
    int line_num = 0;

    while (fgets(line, sizeof(line), fp)) {
        line_num++;

        // Skip comments and empty lines
        char *trimmed = trim_whitespace(line);
        if (trimmed[0] == '#' || trimmed[0] == '\0') {
            continue;
        }

        // Parse key=value
        char *equals = strchr(trimmed, '=');
        if (!equals) {
            PRINT_WARN("Invalid line %d in %s: %s\n", line_num, filepath, trimmed);
            continue;
        }

        *equals = '\0';
        char *key = trim_whitespace(trimmed);
        char *value = trim_whitespace(equals + 1);

        // Parse configuration items
        if (strcmp(key, "hw_variant") == 0) {
            g_product_config.hw_variant = parse_hw_variant(value);
        } else if (strcmp(key, "sensor_type") == 0) {
            g_product_config.sensor_type = parse_sensor_type(value);
        } else if (strcmp(key, "main_max_width") == 0) {
            g_product_config.main_max_width = atoi(value);
        } else if (strcmp(key, "main_max_height") == 0) {
            g_product_config.main_max_height = atoi(value);
        } else if (strcmp(key, "sub_max_width") == 0) {
            g_product_config.sub_max_width = atoi(value);
        } else if (strcmp(key, "sub_max_height") == 0) {
            g_product_config.sub_max_height = atoi(value);
        } else if (strcmp(key, "rotation_mode") == 0) {
            g_product_config.rotation_mode = atoi(value);
        } else if (strcmp(key, "enable_audio") == 0) {
            g_product_config.enable_audio = atoi(value);
        } else if (strcmp(key, "enable_motion_detect") == 0) {
            g_product_config.enable_motion_detect = atoi(value);
        } else if (strcmp(key, "enable_human_detect") == 0) {
            g_product_config.enable_human_detect = atoi(value);
        } else {
            PRINT_WARN("Unknown config key at line %d: %s\n", line_num, key);
        }
    }

    fclose(fp);
    PRINT_TRACE("Product config loaded from %s\n", filepath);
    return 0;
}

/**
 * @brief Initialize product configuration
 *
 * Configuration priority (highest to lowest):
 * 1. ./product.conf (development environment)
 * 2. /etc/product.conf (production deployment)
 * 3. Built-in defaults (compile-time DEFAULT_HW_VARIANT)
 */
int ProductConfig_Init(void)
{
    if (g_config_initialized) {
        PRINT_WARN("ProductConfig already initialized\n");
        return 0;
    }

    /* Try development config first (for easier testing) */
    if (load_config_file(PRODUCT_CONFIG_FILE_DEV) == 0) {
        PRINT_INFO("Product config loaded from %s (development)\n", PRODUCT_CONFIG_FILE_DEV);
    }
    /* Try production config */
    else if (load_config_file(PRODUCT_CONFIG_FILE_PROD) == 0) {
        PRINT_INFO("Product config loaded from %s (production)\n", PRODUCT_CONFIG_FILE_PROD);
    }
    /* Use built-in defaults */
    else {
        PRINT_INFO("Using built-in default: hw_variant=%d\n", g_product_config.hw_variant);
    }

    g_config_initialized = 1;
    return 0;
}

/**
 * @brief Get current product configuration
 */
const ProductConfig_t* ProductConfig_Get(void)
{
    if (!g_config_initialized) {
        PRINT_WARN("ProductConfig not initialized, using defaults\n");
        ProductConfig_Init();
    }
    return &g_product_config;
}

/**
 * @brief Reload product configuration from file
 */
int ProductConfig_Reload(void)
{
    if (!g_config_initialized) {
        return ProductConfig_Init();
    }

    /* Try both config file locations */
    if (load_config_file(PRODUCT_CONFIG_FILE_DEV) == 0) {
        PRINT_INFO("Product config reloaded from %s\n", PRODUCT_CONFIG_FILE_DEV);
        return 0;
    }
    if (load_config_file(PRODUCT_CONFIG_FILE_PROD) == 0) {
        PRINT_INFO("Product config reloaded from %s\n", PRODUCT_CONFIG_FILE_PROD);
        return 0;
    }

    PRINT_WARN("Config reload failed, keeping current configuration\n");
    return -1;
}

/**
 * @brief Clean up product configuration
 */
void ProductConfig_Deinit(void)
{
    g_config_initialized = 0;
    // Reset to defaults (optional)
}
