/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NIAL_IOCTL_H_
#define _NIAL_IOCTL_H_

#ifdef __cplusplus
extern "C" {
#endif

#define _NIAL_IOC_NRBITS   8
#define _NIAL_IOC_TYPEBITS 8
#define _NIAL_IOC_SIZEBITS 14
#define _NIAL_IOC_DIRBITS  2

#define _NIAL_IOC_NRMASK   ((1 << _NIAL_IOC_NRBITS) - 1)
#define _NIAL_IOC_TYPEMASK ((1 << _NIAL_IOC_TYPEBITS) - 1)
#define _NIAL_IOC_SIZEMASK ((1 << _NIAL_IOC_SIZEBITS) - 1)
#define _NIAL_IOC_DIRMASK  ((1 << _NIAL_IOC_DIRBITS) - 1)

#define _NIAL_IOC_NRSHIFT   0
#define _NIAL_IOC_TYPESHIFT (_NIAL_IOC_NRSHIFT + _NIAL_IOC_NRBITS)
#define _NIAL_IOC_SIZESHIFT (_NIAL_IOC_TYPESHIFT + _NIAL_IOC_TYPEBITS)
#define _NIAL_IOC_DIRSHIFT  (_NIAL_IOC_SIZESHIFT + _NIAL_IOC_SIZEBITS)

#define _NIAL_IOC_NONE  0U
#define _NIAL_IOC_WRITE 1U
#define _NIAL_IOC_READ  2U

#define _NIAL_IOC(dir, type, nr, size)                                                             \
    (((dir) << _NIAL_IOC_DIRSHIFT) | ((type) << _NIAL_IOC_TYPESHIFT) | ((nr) << _NIAL_IOC_NRSHIFT) \
     | ((size) << _NIAL_IOC_SIZESHIFT))

#define _NIAL_IOC_TYPECHECK(t) (sizeof(t))

#define NIAL_IO(type, nr)             _NIAL_IOC(_NIAL_IOC_NONE, (type), (nr), 0)
#define NIAL_IOR(type, nr, size)      _NIAL_IOC(_NIAL_IOC_READ, (type), (nr), (_NIAL_IOC_TYPECHECK(size)))
#define NIAL_IOW(type, nr, size)      _NIAL_IOC(_NIAL_IOC_WRITE, (type), (nr), (_NIAL_IOC_TYPECHECK(size)))
#define NIAL_IOWR(type, nr, size)     _NIAL_IOC(_NIAL_IOC_READ | _NIAL_IOC_WRITE, (type), (nr), (_NIAL_IOC_TYPECHECK(size)))
#define NIAL_IOR_BAD(type, nr, size)  _NIAL_IOC(_NIAL_IOC_READ, (type), (nr), sizeof(size))
#define NIAL_IOW_BAD(type, nr, size)  _NIAL_IOC(_NIAL_IOC_WRITE, (type), (nr), sizeof(size))
#define NIAL_IOWR_BAD(type, nr, size) _NIAL_IOC(_NIAL_IOC_READ | _NIAL_IOC_WRITE, (type), (nr), sizeof(size))

/* used to decode ioctl numbers.. */
#define _NIAL_IOC_DIR(nr)  (((nr) >> _NIAL_IOC_DIRSHIFT) & _NIAL_IOC_DIRMASK)
#define _NIAL_IOC_TYPE(nr) (((nr) >> _NIAL_IOC_TYPESHIFT) & _NIAL_IOC_TYPEMASK)
#define _NIAL_IOC_NR(nr)   (((nr) >> _NIAL_IOC_NRSHIFT) & _NIAL_IOC_NRMASK)
#define _NIAL_IOC_SIZE(nr) (((nr) >> _NIAL_IOC_SIZESHIFT) & _NIAL_IOC_SIZEMASK)

#define NIAL_IOC_IN        (_NIAL_IOC_WRITE << _NIAL_IOC_DIRSHIFT)
#define NIAL_IOC_OUT       (_NIAL_IOC_READ << _NIAL_IOC_DIRSHIFT)
#define NIAL_IOC_INOUT     ((_NIAL_IOC_WRITE | _NIAL_IOC_READ) << _NIAL_IOC_DIRSHIFT)
#define NIAL_IOCSIZE_MASK  (_NIAL_IOC_SIZEMASK << _NIAL_IOC_SIZESHIFT)
#define NIAL_IOCSIZE_SHIFT (_NIAL_IOC_SIZESHIFT)

#ifdef __cplusplus
}
#endif

#endif /* _NIAL_IOCTL_H_ */
