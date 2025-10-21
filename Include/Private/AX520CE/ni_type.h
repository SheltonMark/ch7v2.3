/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef __NI_TYPE_H__
#define __NI_TYPE_H__

#ifndef __KERNEL__
#include <stdint.h>   // added in C99
#include <stdbool.h>  // added in C99
#include <limits.h>
#endif

#ifndef NULL
#define NULL 0L
#endif
#define NI_NULL 0L

/**
 * Define the basic data type, the types will be used in the whole project.
 */

typedef enum {
    NI_FALSE = 0,
    NI_TRUE  = 1,
} NI_BOOL;

typedef char           NI_CHAR;
typedef unsigned char  NI_UCHAR;
typedef signed char    NI_S8;
typedef unsigned char  NI_U8;
typedef signed short   NI_S16;
typedef unsigned short NI_U16;
typedef int            NI_S32;
typedef unsigned int   NI_U32;

#ifdef _WIN32  // windows os
typedef __int64          NI_S64;
typedef unsigned __int64 NI_U64;
#else
typedef long long          NI_S64;
typedef unsigned long long NI_U64;
#endif

typedef void          NI_VOID;
typedef long          NI_LONG;
typedef unsigned long NI_ULONG;
typedef float         NI_F32;
typedef double        NI_F64;

typedef unsigned long  NI_SIZE_T;
typedef unsigned long  NI_PHYS_ADDR_T;
typedef unsigned long* NI_VIRT_ADDR_T;

typedef void* NI_HANDLE;

typedef NI_BOOL  ni_bool;
typedef NI_CHAR  ni_char;
typedef NI_UCHAR ni_uchar;
typedef NI_S8    ni_s8;
typedef NI_U8    ni_u8;
typedef NI_S16   ni_s16;
typedef NI_U16   ni_u16;
typedef NI_S32   ni_s32;
typedef NI_U32   ni_u32;
typedef NI_S64   ni_s64;
typedef NI_U64   ni_u64;

typedef NI_VOID  ni_void;
typedef NI_LONG  ni_long;
typedef NI_ULONG ni_ulong;
typedef NI_F32   ni_f32;
typedef NI_F64   ni_f64;

typedef NI_SIZE_T      ni_size_t;
typedef NI_PHYS_ADDR_T ni_phys_addr_t;
typedef NI_VIRT_ADDR_T ni_virt_addr_t;
typedef NI_HANDLE      ni_handle;

#endif /* __NI_TYPE_H__ */
