/******************************************************************************

  Copyright (C), 2014-2020, ZheJiang Chipup Technology CO.,LTD.

 ******************************************************************************
  File Name     : ni_macro.h
  Version       : Initial Draft
  Author        : 
  Created       : 2014.3.13
  Last Modified :
  Description   : define the format of error code
  Function List :
  DHstory       :
  1.Date        : 2014/3/13
    Author      :
    Modification: Create
******************************************************************************/

#ifndef _NI_MACRO_H_
#define _NI_MACRO_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 超时参数 */
#define NI_TIMEOUT_NONE         (0)    /* 不等待，立即返回。*/ 
#define NI_TIMEOUT_FOREVER      (~0U)  /* 一直等待直到返回 */

/* 检测value值是否是2的N次方 */
#define NI_checkIs2n(value)  ( ((value) == 0) ? NI_FALSE\
                               : ((value) & ((value) - 1))  \
                                  ? NI_FALSE : NI_TRUE)

/* 数据存储度量单位 */
#define NI_KB (1024)
#define NI_MB (NI_KB * NI_KB)
#define NI_GB (NI_KB * NI_MB)

/* 数值操作*/
#define NI_max(a, b)            ( (a) > (b) ? (a) : (b) )
#define NI_min(a, b)            ( (a) < (b) ? (a) : (b) )
#define NI_max3(a,b,c)          NI_max(NI_max(a,b),c)
#define NI_min3(a,b,c)          NI_min(NI_min(a,b),c)
#define NI_clip(a, min, max)    NI_min(NI_max(a, min), max)
#define NI_Div(a, b)            (((b) != 0) ? ((a)/(b)) : 0)
#define NI_RoundDiv(a,b)        (NI_Div(((a)>0 ? (a) + ((b)>>1) : (a) - ((b)>>1)),(b)))  /* a 除以 b, 四舍五入取整*/
#define NI_abs(a)               ((a) >= 0 ? (a) : (-(a)))
#define NI_swap(type,a,b)       do{type SWAP_tmp= b; b= a; a= SWAP_tmp;}while(0)

/* 内存操作*/
#define NI_clear(ptr)		           memset((ptr), 0, sizeof(*(ptr)))	
#define NI_clearSize(ptr, size)        memset((ptr), 0, (size))
#define NI_memCmp(dst, src)            memcmp((dst), (src), sizeof(*(src)))
#define NI_memCmpSize(dst, src, size)  memcmp((dst), (src), (size))


/* 对齐操作*/
#define NI_ALIGN4(_x)              (((_x)+0x03)&~0x03)
#define NI_ALIGN8(_x)              (((_x)+0x07)&~0x07)
#define NI_ALIGN16(_x)             (((_x)+0x0f)&~0x0f)
#define NI_ALIGN32(_x)             (((_x)+0x1f)&~0x1f)
#define NI_ALIGN64(_x)             (((_x)+0x3f)&~0x3f)
#define NI_align(value, align)   ((( (value) + ( (align) - 1 ) ) \
                                     / (align) ) * (align) )
#define NI_ceil(value, align)    NI_align(value, align) 
#define NI_floor(value, align)   (( (value) / (align) ) * (align) )

/* 设置真假 */
#define NI_setTrue(val)        ((val) = NI_TRUE)
#define NI_setFalse(val)       ((val) = NI_FALSE)      

/* 获取数组成员数量 */
#define NI_arraySize(array)    (sizeof(array)/sizeof((array)[0]))


/* 获取结构体成员的地址偏移量 */
#ifdef __compiler_offsetof
#define NI_offsetof(TYPE,MEMBER) __compiler_offsetof(TYPE,MEMBER)
#else
#define NI_offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif


/* 通过结构体成员获取结构体首地址 */
#define NI_containerOf(ptr, type, member) ({			\
        const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
        (type *)( (char *)__mptr - NI_offsetof(type,member) );})
        
#define BITMSK(bit)				(int)(1 << (bit))
#define MAKEWORD(a, b)      ((unsigned short)(((unsigned char)(a)) | ((unsigned short)((unsigned char)(b))) << 8))

#define LOWORD(l)           ((unsigned short)(l))
#define HIWORD(l)           ((unsigned short)(((unsigned int)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)           ((unsigned char)(w))
#define HIBYTE(w)           ((unsigned char)(((unsigned short)(w) >> 8) & 0xFF))

#define NI_regReadl(virtAddr) (*((volatile NI_U32*)(virtAddr)))
#define NI_regWritel(virtAddr, value) \
        (*((volatile NI_U32 *)(virtAddr)) = (value))

#define NI_RegRead(RegAddr)             (NI_regReadl(KSEG1ADDR(RegAddr)))
#define NI_RegWrite(RegAddr, RegVal)    (NI_regWritel(KSEG1ADDR(RegAddr), RegVal))

#ifdef __cplusplus
}
#endif

#endif  /*  _NI_MACRO_H_  */

