#ifndef _NIAL_PRIV_H_
#define _NIAL_PRIV_H_

#define NIAL_VERSION "1.0"
#include "nial_list.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef __LINUX__

#elif defined __YOC__
#include <core/csi_kernel.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#elif defined __DHOS__
#include <kernel/csi_kernel.h>
#include <k_api.h>

#elif defined __RT_THREAD__

#endif



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
