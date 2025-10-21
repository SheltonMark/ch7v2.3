/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NI_MACRO_H_
#define _NI_MACRO_H_

#include "ni_type.h"

/* 设置真假 */
#define NI_SetTrue(val)  ((val) = NI_TRUE)
#define NI_SetFalse(val) ((val) = NI_FALSE)

/* 检测value值是否是2的N次方 */
#define NI_CheckIs2n(value) (((value) == 0) ? NI_FALSE : ((value) & ((value)-1)) ? NI_FALSE \
                                                                                 : NI_TRUE)

/* 数据存储度量单位 */
#define NI_KB (1024)
#define NI_MB (NI_KB * NI_KB)
#define NI_GB (NI_KB * NI_MB)

#ifndef NI_INT_MAX
#define NI_INT_MAX ((~0U) >> 1)
#endif

/* 数值操作*/
#define NI_Max(a, b)         ((a) > (b) ? (a) : (b))
#define NI_Min(a, b)         ((a) < (b) ? (a) : (b))
#define NI_Max3(a, b, c)     NI_Max(NI_Max(a, b), c)
#define NI_Min3(a, b, c)     NI_Min(NI_Min(a, b), c)
#define NI_Clip(a, min, max) NI_Min(NI_Max(a, min), max)
#define NI_Div(a, b)         (((b) != 0) ? ((a) / (b)) : ((((b) == 0) && ((a) != 0)) ? (NI_INT_MAX) : 0))
#define NI_RoundDiv(a, b)    ((a) == 0 ? 0 : NI_Div(((a) > 0 ? (a) + ((b) >> 1) : (a) - ((b) >> 1)), (b))) /* a 除以 b, 四舍五入取整*/
#define NI_Abs(a)            ((a) >= 0 ? (a) : (-(a)))
#define NI_Swap(type, a, b)       \
    do {                          \
        type _swapTmp = b;        \
        b             = a;        \
        a             = _swapTmp; \
    } while (0)

/* 内存操作*/
#ifndef __KERNEL__
#define NI_Clear(ptr)                 memset((ptr), 0, sizeof(*(ptr)))
#define NI_ClearSize(ptr, size)       memset((ptr), 0, (size))
#define NI_MemCmp(dst, src)           memcmp((dst), (src), sizeof(*(src)))
#define NI_MemCmpSize(dst, src, size) memcmp((dst), (src), (size))
#else
#define NI_Clear(ptr)                 nial_memset((ptr), 0, sizeof(*(ptr)))
#define NI_ClearSize(ptr, size)       nial_memset((ptr), 0, (size))
#define NI_MemCmp(dst, src)           nial_memcmp((dst), (src), sizeof(*(src)))
#define NI_MemCmpSize(dst, src, size) nial_memcmp((dst), (src), (size))
#endif

/* 对齐操作*/
#define NI_Align4(_x)          (((_x) + 0x03) & ~0x03)
#define NI_Align8(_x)          (((_x) + 0x07) & ~0x07)
#define NI_Align16(_x)         (((_x) + 0x0f) & ~0x0f)
#define NI_Align32(_x)         (((_x) + 0x1f) & ~0x1f)
#define NI_Align64(_x)         (((_x) + 0x3f) & ~0x3f)
#define NI_Align(value, align) ((((value) + ((align)-1)) / (align)) * (align))
#define NI_Ceil(value, align)  NI_Align(value, align)
#define NI_Floor(value, align) (((value) / (align)) * (align))

/* 获取数组成员数量 */
#define NI_ArraySize(array) (sizeof(array) / sizeof((array)[0]))

/* 获取结构体成员的地址偏移量 */
#ifdef __compiler_offsetof
#define NI_OffsetOf(type, member) __compiler_offsetof(type, member)
#else
#define NI_OffsetOf(type, member) ((size_t) & ((type *)0)->member)
#endif

/* 通过结构体成员获取结构体首地址 */
#define NI_ContainerOf(ptr, type, member)                     \
    ({                                                        \
        const typeof(((type *)0)->member) *__mptr = (ptr);    \
        (type *)((char *)__mptr - NI_OffsetOf(type, member)); \
    })

#define NI_MakeWord(low, high) \
    ((unsigned short)(((unsigned char)(low)) | ((unsigned short)((unsigned char)(high))) << 8))

#define BITMSK(bit) (int)(1 << (bit))
#define LOWORD(l)   ((unsigned short)(l))
#define HIWORD(l)   ((unsigned short)(((unsigned int)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)   ((unsigned char)(w))
#define HIBYTE(w)   ((unsigned char)(((unsigned short)(w) >> 8) & 0xFF))

#define NI_RegReadl(virtAddr)         (*((volatile unsigned int *)(virtAddr)))
#define NI_RegWritel(virtAddr, value) (*((volatile unsigned int *)(virtAddr)) = (value))

#define NI_RegRead(RegAddr)          (NI_RegReadl(KSEG1ADDR(RegAddr)))
#define NI_RegWrite(RegAddr, RegVal) (NI_RegWritel(KSEG1ADDR(RegAddr), RegVal))

/* 获取当前系统时间us */
#ifdef __KERNEL__
#define GetCurTimeUsec() ({nial_timeval_t stTime;NI_U64 u64CurTimeUsec;\
    nial_gettimeofday(&stTime);\
    u64CurTimeUsec = (NI_U64)stTime.tv_sec * 1000000 + (NI_U64)stTime.tv_usec;\
    u64CurTimeUsec; })
#else
#define GetCurTimeUsec() ({struct timeval stTime;NI_U64 u64CurTimeUsec;\
    gettimeofday(&stTime, NI_NULL);\
    u64CurTimeUsec = (NI_U64)stTime.tv_sec * 1000000 + (NI_U64)stTime.tv_usec;\
    u64CurTimeUsec; })
#endif

#endif /*  _NI_MACRO_H_  */
