/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NIAL_MATH_H_
#define _NIAL_MATH_H_

/**
 * 函数名: nial_div_u64
 * 描述: 无符号64位除以无符号32位
 * 输入: 参数       描述
 *      dividend  被除数
 *      divisor   除数
 * 返回: 返回值     描述
 *      quot      商(64位)
 */
unsigned long long nial_div_u64(unsigned long long dividend, unsigned int divisor);

/**
 * 函数名: nial_div_s64
 * 描述: 有符号64位除以有符号32位
 * 输入: 参数       描述
 *      dividend  被除数
 *      divisor   除数
 * 返回: 返回值     描述
 *      quot      商(64位)
 */
long long nial_div_s64(long long dividend, int divisor);

/**
 * 函数名: nial_div64_u64
 * 描述: 无符号64位除法
 * 输入: 参数       描述
 *      dividend  被除数
 *      divisor   除数
 * 返回: 返回值     描述
 *      quot      商(64位)
 */
unsigned long long nial_div64_u64(unsigned long long dividend, unsigned long long divisor);

/**
 * 函数名: nial_div64_s64
 * 描述: 有符号64位除法
 * 输入: 参数       描述
 *      dividend  被除数
 *      divisor   除数
 * 返回: 返回值     描述
 *      quot      商(64位)
 */
long long nial_div64_s64(long long dividend, long long divisor);

/**
 * 函数名: nial_div_u64_rem
 * 描述: 无符号64位对无符号32位求余
 * 输入: 参数       描述
 *      dividend  被除数
 *      divisor   除数
 * 返回: 返回值     描述
 *      rem       余数(64位)
 */
unsigned long long nial_div_u64_rem(unsigned long long dividend, unsigned int divisor);

/**
 * 函数名: nial_div_s64_rem
 * 描述: 有符号64位对有符号32位求余
 * 输入: 参数       描述
 *      dividend  被除数
 *      divisor   除数
 * 返回: 返回值     描述
 *      rem       余数(64位)
 */
long long nial_div_s64_rem(long long dividend, int divisor);

/**
 * 函数名: nial_div_u64_rem
 * 描述: 无符号64位除法求余
 * 输入: 参数       描述
 *      dividend  被除数
 *      divisor   除数
 * 返回: 返回值     描述
 *      rem       余数(64位)
 */
unsigned long long nial_div64_u64_rem(unsigned long long dividend, unsigned long long divisor);

/**
 * 函数名: nial_random
 * 描述: 产生随机数
 * 输入: 参数       描述
 *      无
 * 返回: 返回值     描述
 *      无
 */
unsigned int nial_random(void);

#define nial_max(x, y)                 \
    ({                                 \
        __typeof__(x) _max1 = (x);     \
        __typeof__(y) _max2 = (y);     \
        (void)(&_max1 == &_max2);      \
        _max1 > _max2 ? _max1 : _max2; \
    })

#define nial_min(x, y)                 \
    ({                                 \
        __typeof__(x) _min1 = (x);     \
        __typeof__(y) _min2 = (y);     \
        (void)(&_min1 == &_min2);      \
        _min1 < _min2 ? _min1 : _min2; \
    })

#define nial_abs(x)                            \
    ({                                         \
        long ret;                              \
        if (sizeof(x) == sizeof(long)) {       \
            long __x = (x);                    \
            ret      = (__x < 0) ? -__x : __x; \
        } else {                               \
            int __x = (x);                     \
            ret     = (__x < 0) ? -__x : __x;  \
        }                                      \
        ret;                                   \
    })

#endif /* _NIAL_MATH_H_ */
