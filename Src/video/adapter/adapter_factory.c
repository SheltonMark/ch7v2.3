/**
 * @file adapter_factory.c
 * @brief Platform adapter factory implementation
 *
 * This file implements the factory pattern to select the correct
 * platform adapter at compile time based on PLATFORM_AX macro.
 */

#include "platform_adapter.h"
#include <stdio.h>
#include <string.h>

/* Static adapter instance */
static PlatformAdapter g_adapter;
static int g_adapter_initialized = 0;

/* Platform-specific initialization functions */
#ifdef PLATFORM_AX
extern int AX_InitAdapter(PlatformAdapter* adapter);
#endif

/**
 * @brief Get the platform adapter instance
 */
PlatformAdapter* GetPlatformAdapter(void)
{
    if (!g_adapter_initialized) {
        memset(&g_adapter, 0, sizeof(PlatformAdapter));

        if (InitPlatformAdapter(&g_adapter) != 0) {
            PRINT_ERROR("Failed to initialize platform adapter\n");
            return NULL;
        }

        g_adapter_initialized = 1;
        PRINT_INFO("Platform adapter initialized successfully\n");
    }

    return &g_adapter;
}

/**
 * @brief Initialize platform adapter based on compile-time platform selection
 */
int InitPlatformAdapter(PlatformAdapter* adapter)
{
    if (!adapter) {
        PRINT_ERROR("Adapter pointer is NULL\n");
        return -1;
    }

#ifdef PLATFORM_AX
    PRINT_INFO("Initializing AX platform adapter\n");
    return AX_InitAdapter(adapter);
#else
    PRINT_ERROR("No platform defined! Please define PLATFORM_AX in Makefile\n");
    return -1;
#endif
}
