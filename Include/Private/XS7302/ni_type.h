/******************************************************************************

  Copyright (C), 2014-2020, ZheJiang Chipup Technology CO.,LTD.

 ******************************************************************************
  File Name     : ni_type.h
  Version       : Initial Draft
  Author        : Lv Zhuqing<lv_zhuqing@chipup.com>
  Created       : 2014.3.13
  Last Modified :
  Description   : The common data type defination
  Function List :
  DHstory       :
  1.Date        : 2014/3/13
    Author      : 24497
    Modification: Create
******************************************************************************/

#ifndef __NI_TYPE_H__
#define __NI_TYPE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*----------------------------------------------*
 * The common data type, will be used in the whole project.*
 *----------------------------------------------*/

typedef unsigned char           NI_U8;
typedef unsigned short          NI_U16;
typedef unsigned int            NI_U32;

typedef signed char             NI_S8;
typedef short                   NI_S16;
typedef int                     NI_S32;

#ifndef _M_IX86
    typedef unsigned long long  NI_U64;
    typedef long long           NI_S64;
#else
    typedef __int64             NI_U64;
    typedef __int64             NI_S64;
#endif


typedef char                    NI_CHAR;
typedef void                    NI_VOID;

typedef float                   NI_F32;
typedef double                  NI_F64;

typedef void *                  NI_PTR;
typedef void *                  NI_Handle;

/* 变量属性register关键词 */
#define NI_REGISTER  register

/*----------------------------------------------*
 * const defination                             *
 *----------------------------------------------*/
typedef enum {
    NI_FALSE = 0,
    NI_TRUE  = 1,
} NI_BOOL;

typedef enum {
    NI_ELEC_VL = 0,
    NI_ELEC_VH = 1,
} NI_ELEC_LEVEL;

#ifndef NULL
    #define NULL    0L
#endif

#define NI_NULL     0L
#define NI_SUCCESS  0
#define NI_FAILURE  (-1)
#define NI_ETIMEOUT  (-2)   /* 等待超时 */        
#define NI_EINTR     (-3)   /* 被信号中断 */      

#ifndef  PTR_FXN_T
#if (__SIZEOF_POINTER__ == 4)
#define NI_PTR_T(type, field) type* field;void *  __attribute__((__unused__)) pad_##field
#define NI_FXN_T(type, field, args ...) type (*field)(args);void *  __attribute__((__unused__)) pad_##field
#elif (__SIZEOF_POINTER__ == 8)
#define NI_PTR_T(type, field) type* field
#define NI_FXN_T(type, field, args ...) type (*field)(args)
#else
#error "__SIZEOF_POINTER__ = ?"
#endif
#endif

#ifndef MODULE_DEVICE_TABLE
#define __stringify_1(x...) #x
#define __stringify(x...)   __stringify_1(x)

#define MODULE_DEVICE_TABLE(type, name)					\
extern const typeof(name) __mod_##type##__##name##_device_table		\
  __attribute__ ((unused, alias(__stringify(name))))
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __NI_TYPE_H__ */


