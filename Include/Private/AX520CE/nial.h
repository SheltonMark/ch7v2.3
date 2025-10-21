/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NIAL_H_
#define _NIAL_H_

#include "nial/nial_ioctl.h"
#include "nial/nial_list.h"

#ifdef __KERNEL__
#include "nial/nial_type.h"
#include "nial/nial_addr.h"
#include "nial/nial_tty.h"
#include "nial/nial_errno.h"
#include "nial/nial_platform.h"
#include "nial/nial_string.h"
#include "nial/nial_debug.h"
#include "nial/nial_device.h"
#include "nial/nial_workqueue.h"
#include "nial/nial_proc.h"
#include "nial/nial_schedule.h"
#include "nial/nial_semaphore.h"
#include "nial/nial_spinlock.h"
#include "nial/nial_wait.h"
#include "nial/nial_module.h"
#include "nial/nial_fops.h"
#include "nial/nial_interrupt.h"
#include "nial/nial_atomic.h"
#include "nial/nial_barrier.h"
#include "nial/nial_cache.h"
#include "nial/nial_malloc.h"
#include "nial/nial_math.h"
#include "nial/nial_mutex.h"
#include "nial/nial_task.h"
#include "nial/nial_time.h"
#include "nial/nial_timer.h"
#include "nial/nial_seq.h"
#include "nial/nial_hashtable.h"
#include "nial/nial_sysfs.h"
#include "nial/nial_export.h"
#include "nial/nial_spi.h"
#include "nial/nial_i2c.h"
#include "nial/nial_gpio.h"

#include "nial/nial_version.h"
#define NI_NIAL_VERSION "2.0" NIAL_GIT_VERSION
#endif /* __KERNEL__ */

#endif /* _NIAL_H_ */
