/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NIAL_ERRNO_H_
#define _NIAL_ERRNO_H_

#define E_NIAL_OK   0  /* success */
#define E_NIAL_FAIL -1 /* fail */

#define E_NIAL_PERM   1  /* Operation not permitted */
#define E_NIAL_NOENT  2  /* No such file or directory */
#define E_NIAL_SRCH   3  /* No such process */
#define E_NIAL_INTR   4  /* Interrupted system call */
#define E_NIAL_IO     5  /* I/O error */
#define E_NIAL_NXIO   6  /* No such device or address */
#define E_NIAL_2BIG   7  /* Argument list too long */
#define E_NIAL_NOEXEC 8  /* Exec format error */
#define E_NIAL_BADF   9  /* Bad file number */
#define E_NIAL_CHILD  10 /* No child processes */
#define E_NIAL_AGAIN  11 /* Try again */
#define E_NIAL_NOMEM  12 /* Out of memory */
#define E_NIAL_ACCES  13 /* Permission denied */
#define E_NIAL_FAULT  14 /* Bad address */
#define E_NIAL_NOTBLK 15 /* Block device required */
#define E_NIAL_BUSY   16 /* Device or resource busy */
#define E_NIAL_EXIST  17 /* File exists */
#define E_NIAL_XDEV   18 /* Cross-device link */
#define E_NIAL_NODEV  19 /* No such device */
#define E_NIAL_NOTDIR 20 /* Not a directory */
#define E_NIAL_ISDIR  21 /* Is a directory */
#define E_NIAL_INVAL  22 /* Invalid argument */
#define E_NIAL_NFILE  23 /* File table overflow */
#define E_NIAL_MFILE  24 /* Too many open files */
#define E_NIAL_NOTTY  25 /* Not a typewriter */
#define E_NIAL_TXTBSY 26 /* Text file busy */
#define E_NIAL_FBIG   27 /* File too large */
#define E_NIAL_NOSPC  28 /* No space left on device */
#define E_NIAL_SPIPE  29 /* Illegal seek */
#define E_NIAL_ROFS   30 /* Read-only file system */
#define E_NIAL_MLINK  31 /* Too many links */
#define E_NIAL_PIPE   32 /* Broken pipe */
#define E_NIAL_DOM    33 /* Math argument out of domain of func */
#define E_NIAL_RANGE  34 /* Math result not representable */

#define E_NIAL_NOIOCTLCMD 515 /* No ioctl command */
#define E_NIAL_RESTARTSYS 512

#endif /* _NIAL_ERRNO_H_ */
