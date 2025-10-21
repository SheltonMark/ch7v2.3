/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NIAL_TYPE_H_
#define _NIAL_TYPE_H_

// #ifdef __LINUX_KERNEL__
typedef long long    nial_loff_t;
typedef int          nial_ssize_t;
typedef unsigned int nial_size_t;
// #endif

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;
#endif

#define NIAL_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#endif /* _NIAL_TYPE_H_ */
