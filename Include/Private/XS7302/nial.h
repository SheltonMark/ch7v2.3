#ifndef __NI_NIAL__
#define __NI_NIAL__

#define NI_NIAL_VERSION "1.0"

#include "nial_list.h"

#define nial_gfp_kernel 0
#define nial_gfp_atomic 1
extern int hil_mmb_flush_dcache_byaddr_safe(void *kvirt, unsigned long phys_addr, unsigned long length);
extern void *nial_vmalloc(unsigned long size);
extern void  nial_vfree(const void *addr);
extern void *nial_kmalloc(unsigned long size, unsigned int nial_gfp_flag);
extern void  nial_kfree(const void * addr);

typedef struct {
	void* owner;
	void* open;
	void* read;
	void* write;
	void* unlocked_ioctl;
}nial_file_ops_t;

//atomic api
typedef struct {
    void *atomic;
}nial_atomic_t;

#define NIAL_ATOMIC_INIT(i)  { (i) }
extern int  nial_atomic_init(nial_atomic_t *atomic);
extern void nial_atomic_destory(nial_atomic_t *atomic);
extern int  nial_atomic_read(nial_atomic_t *v);
extern void nial_atomic_set(nial_atomic_t *v, int i);
extern int  nial_atomic_inc_return(nial_atomic_t *v);
extern int  nial_atomic_dec_return(nial_atomic_t *v);




//semaphore api
#define EINTR 4
typedef struct nial_semaphore{
    void * sem;
}nial_semaphore_t;
extern int nial_sema_init(nial_semaphore_t *sem, int val);
extern int nial_down(nial_semaphore_t *sem);
extern int nial_down_interruptible(nial_semaphore_t *sem);
extern int nial_down_trylock(nial_semaphore_t *sem);
extern int nial_down_timeout(nial_semaphore_t *sem, long ms);
extern void nial_up(nial_semaphore_t *sem);
extern void nial_sema_destory(nial_semaphore_t *sem); //notice:must be called when kmod exit, other wise will lead to memory leak;

//mutex api
typedef struct nial_mutex{
    void * mutex;
}nial_mutex_t;
extern int nial_mutex_init(nial_mutex_t *mutex);
extern int nial_mutex_lock(nial_mutex_t *mutex);
extern int nial_mutex_lock_interruptible(nial_mutex_t *mutex);
extern int nial_mutex_trylock(nial_mutex_t *mutex);
extern void nial_mutex_unlock(nial_mutex_t *mutex);
extern void nial_mutex_destory(nial_mutex_t *mutex); //notice:must be called when kmod exit, other wise will lead to memory leak;

//spin lock api
typedef struct nial_spinlock{
    void * lock;
}nial_spinlock_t;
extern int nial_spin_lock_init(nial_spinlock_t *lock);
extern void nial_spin_lock(nial_spinlock_t *lock);
extern int nial_spin_trylock(nial_spinlock_t *lock);
extern void nial_spin_unlock(nial_spinlock_t *lock);
extern void nial_spin_lock_irqsave(nial_spinlock_t *lock, unsigned long *flags);
extern int nial_spin_trylock_irqsave(nial_spinlock_t *lock, unsigned long *flags);
extern void nial_spin_unlock_irqrestore(nial_spinlock_t *lock, unsigned long *flags);
extern void nial_spin_lock_destory(nial_spinlock_t *lock); //notice:must be called when kmod exit, other wise will lead to memory leak;

//wait api
typedef int (*nial_wait_cond_func_t)(void *param);

typedef struct nial_wait{
   void *wait;
}nial_wait_t;

#define ERESTARTSYS 512

extern unsigned long nial_msecs_to_jiffies(const unsigned int m);
extern int nial_wait_init(nial_wait_t * wait);
extern int nial_wait_interruptible(nial_wait_t * wait, nial_wait_cond_func_t func, void* param);
extern int nial_wait_uninterruptible(nial_wait_t * wait, nial_wait_cond_func_t func, void* param);
extern int nial_wait_timeout_interruptible(nial_wait_t * wait, nial_wait_cond_func_t func, void* param, unsigned long ms);
extern int nial_wait_timeout_uninterruptible(nial_wait_t * wait, nial_wait_cond_func_t func, void* param, unsigned long ms);

#define nial_wait_event_interruptible(wait, func, param)    \
({                                  \
    int __ret = 0;                          \
                                          \
    for (;;){                          \
        if(func(param)){                       \
            __ret = 0;                  \
            break;                    \
        }\
        __ret = nial_wait_timeout_interruptible(wait, (func), param, 100);   \
        if(__ret < 0)                 \
            break;           \
    }                                    \
    __ret;                                   \
})


#define nial_wait_event_uninterruptible(wait, func, param)    \
({                                  \
    int __ret = 0;                          \
                                          \
    for (;;){                          \
        if(func(param)){                       \
            __ret = 0;                  \
            break;                    \
        }\
        __ret = nial_wait_uninterruptible(wait, (func), param);   \
        if(__ret < 0)                 \
            break;           \
    }                                    \
    __ret;                                   \
})


#define nial_wait_event_timeout_interruptible(wait, func, param, timeout)             \
({                                  \
    int __ret = timeout;                          \
                                        \
    if ((func(param)) && !timeout) \
    { \
    __ret = 1; \
    } \
                                          \
    for (;;) {                          \
        if (func(param))                       \
        {\
            __ret = nial_msecs_to_jiffies(__ret);     \
            break;                    \
        }\
        __ret = nial_wait_timeout_interruptible(wait, (func), param, __ret);   \
        if(!__ret || __ret == -ERESTARTSYS)   \
            break;                    \
    }                                   \
    __ret;                                   \
})

#define nial_wait_event_timeout_uninterruptible(wait, func, param, timeout)             \
({                                  \
    int __ret = timeout;                          \
                                        \
    if ((func(param)) && !timeout) \
    { \
    __ret = 1; \
    } \
                                          \
    for (;;) {                          \
        if (func(param))                       \
        {\
            __ret = nial_msecs_to_jiffies(__ret);     \
            break;                    \
        }\
        __ret = nial_wait_timeout_uninterruptible(wait, (func), param, __ret);   \
        if(!__ret || __ret == -ERESTARTSYS)   \
            break;                    \
    }                                   \
    __ret;                                   \
})


extern void nial_wakeup(nial_wait_t * wait); //same as wake_up_all
extern void nial_wait_destory(nial_wait_t * wait);


//workqueue api
typedef struct nial_work_struct {
    void *work;
    void (*func)(struct nial_work_struct *work);
} nial_work_struct_t;
typedef void (*nial_work_func_t)(struct nial_work_struct *work);

extern int nial_init_work(struct nial_work_struct *work, nial_work_func_t func);

#define NIAL_INIT_WORK(_work, _func) \
do {  \
    nial_init_work((_work), (_func));           \
} while (0)

extern int nial_schedule_work(struct nial_work_struct *work);
extern void nial_destroy_work(struct nial_work_struct *work);



//shedule
extern void nial_yield(void);

//interrupt api
enum nial_irqreturn {
    NIAL_IRQ_NONE = (0 << 0),
    NIAL_IRQ_HANDLED = (1 << 0),
    NIAL_IRQ_WAKE_THREAD = (1 << 1),
};

typedef int (*nial_irq_handler_t)(int, void *);
extern int nial_request_irq_share(unsigned int irq, nial_irq_handler_t handler, nial_irq_handler_t thread_fn,const char *name, void *dev);
extern void nial_free_irq(unsigned int irq, void *dev);
extern int nial_in_interrupt(void);
extern int nial_request_irq(unsigned int irq, nial_irq_handler_t handler, unsigned long flags,const char *name, void *dev);

#define NIAL_DIS_IRQ_CNT 2
typedef void (*nial_gic_handle_t)(unsigned int, unsigned int, void*);
extern int nial_register_gic_handle(unsigned int index, unsigned int irq, nial_gic_handle_t handle,const char *name, void *dev);
extern int nial_unregister_gic_handle(unsigned int index,unsigned int irq, void* dev);


//task api
typedef struct nial_task{
    void *task_struct;
}nial_task_t;
typedef int (*threadfn_t)(void *data);
extern nial_task_t *nial_kthread_create(threadfn_t thread, void *data, char *name, int prio, unsigned int stack_size);
extern void nial_kthread_destory(nial_task_t *task, unsigned int stop_flag);
extern void nial_kthread_bind(nial_task_t *task, unsigned int cpu);

//string api
extern char * nial_strcpy(char *s1,const char *s2);
extern char * nial_strncpy(char *s1,const char *s2, int size);
extern int nial_strlcpy(char *s1, const char *s2, int size);
extern char * nial_strcat(char *s1, const char *s2);
extern char * nial_strncat(char *s1, const char *s2, int size);
extern int nial_strlcat(char *s1, const char *s2, int size);
extern int nial_strcmp(const char *s1,const char *s2);
extern int nial_strncmp(const char *s1,const char *s2,int size);
extern int nial_strnicmp(const char *s1, const char *s2, int size);
extern int nial_strcasecmp(const char *s1, const char *s2);
extern int nial_strncasecmp(const char *s1, const char *s2, int n);
extern char * nial_strchr(const char *s,int n);
extern char * nial_strnchr(const char *s , int count, int c);
extern char * nial_strrchr(const char *s,int c);
extern char * nial_skip_spaces(const char *s);
extern char *nial_strim(char *s);
extern char *nial_strstrip(char *str);
extern char * nial_strstr(const char *s1, const char *s2);
extern char * nial_strnstr(const char *s1, const char *s2, int n);
extern int nial_strlen(const char *s);
extern int nial_strnlen(const char *s,int size);
extern char * nial_strpbrk(const char *s1,const char *s2);
extern char * nial_strsep(char **s,const char *ct);
extern int nial_strspn(const char *s,const char *accept);
extern int nial_strcspn(const char *s,const char *reject);
extern void * nial_memset(void *str,int c,int count);
extern void * nial_memcpy(void *s1,const void *s2,int count);
extern void * nial_memmove(void *s1,const void *s2,int count);
extern void * nial_memscan(void *addr, int c, int size);
extern int nial_memcmp(const void *cs,const void *ct,int count);
extern void * nial_memchr(const void *s,int c,int n);
extern void * nial_memchr_inv(const void *s, int c, int n);


extern unsigned long long nial_strtoull(const char *cp, char **endp, unsigned int base);
extern unsigned long nial_strtoul(const char *cp, char **endp, unsigned int base);
extern long nial_strtol(const char *cp, char **endp, unsigned int base);
extern long long nial_strtoll(const char *cp, char **endp, unsigned int base);
extern int nial_snprintf(char *buf, int size, const char *fmt, ...);
extern int nial_scnprintf(char *buf, int size, const char *fmt, ...);
extern int nial_sprintf(char *buf, const char *fmt, ...);
extern int nial_sscanf(const char *buf, const char *fmt, ...);

//addr translate
extern void *nial_ioremap(unsigned long phys_addr, unsigned long size);
extern void *nial_ioremap_wc(unsigned long phys_addr, unsigned long size);
extern void *nial_ioremap_nocache(unsigned long phys_addr, unsigned long size);
extern void *nial_ioremap_cached(unsigned long phys_addr, unsigned long size);
extern void nial_iounmap(void *addr);
extern void *nial_arch_ioaddress(void *addr);
extern void *nial_ioaddress(int addr);
#define NIAL_IO_ADDRESS(x) nial_ioaddress(x)
void* nial_map_logical_kernel_cached(unsigned long phyAddr, unsigned long bytes);
void* nial_map_logical_kernel_noncached(unsigned long phyAddr, unsigned long bytes);
void* nial_map_logical_kernel_writecombine(unsigned long phyAddr, unsigned long bytes);
int nial_unmap_logical_kernel(void* pVaddr);


#define nial_readl(x) (*((volatile unsigned int *)(x)))
#define nial_writel(v, x) (*((volatile unsigned int *)(x)) = (v))

extern unsigned long nial_copy_from_user(void *to, const void *from, unsigned long n);
extern unsigned long nial_copy_to_user(void *to, const void *from, unsigned long n);

#define NIAL_VERIFY_READ  0
#define NIAL_VERIFY_WRITE 1
extern int nial_access_ok(int type, const void *addr, unsigned long size);

//cache api
extern void nial_flush_cache_all(void);
extern void nial_cpuc_flush_dcache_area(void *addr, int size);
extern void nial_dma_map_area(void *addr, int size, int dir);

extern void nial_flush_dcache_area(void *kvirt, unsigned long phys_addr, unsigned long length);
extern int nial_flush_dcache_all(void);


//math
extern unsigned long long nial_div_u64(unsigned long long dividend, unsigned int divisor);
extern long long nial_div_s64(long long dividend, int divisor);
extern unsigned long long nial_div64_u64(unsigned long long  dividend, unsigned long long  divisor);
extern long long nial_div64_s64(long long dividend, long long divisor);
extern unsigned long long nial_div_u64_rem(unsigned long long  dividend, unsigned int divisor);
extern long long nial_div_s64_rem(long long dividend, int divisor);
extern unsigned long long nial_div64_u64_rem(unsigned long long dividend, unsigned long long divisor);
extern unsigned int nial_random(void);


#define nial_max(x, y) ({                            \
        __typeof__(x) _max1 = (x);                  \
        __typeof__(y) _max2 = (y);                  \
        (void) (&_max1 == &_max2);              \
        _max1 > _max2 ? _max1 : _max2; })

#define nial_min(x, y) ({                \
    __typeof__(x) _min1 = (x);          \
     __typeof__(y) _min2 = (y);          \
     (void) (&_min1 == &_min2);      \
     _min1 < _min2 ? _min1 : _min2; })

#define nial_abs(x) ({                       \
     long ret;                   \
     if (sizeof(x) == sizeof(long)) {        \
         long __x = (x);             \
         ret = (__x < 0) ? -__x : __x;       \
     } else {                    \
         int __x = (x);              \
         ret = (__x < 0) ? -__x : __x;       \
     }                       \
     ret;                        \
})


//barrier
extern void nial_mb(void);
extern void nial_rmb(void);
extern void nial_wmb(void);
extern void nial_smp_mb(void);
extern void nial_smp_rmb(void);
extern void nial_smp_wmb(void);
extern void nial_isb(void);
extern void nial_dsb(void);
extern void nial_dmb(void);

//debug
extern int nial_printk(const char *fmt, ...);
extern void nial_panic(const char *fmt, const char * file, const char * fun, int line, const char *);
#define NIAL_BUG() do { } while (1)

#define NIAL_ASSERT(expr)               \
do{                                   \
    if (!(expr)) {                    \
        nial_printk("\nASSERT failed at:\n"\
               "  >Condition: %s\n",  \
                #expr);\
        NIAL_BUG();\
    } \
}while(0)

#define NIAL_BUG_ON(expr)  \
do{\
    if(expr){\
        nial_printk("BUG: failure at %s:%d/%s()!\n", __FILE__, __LINE__, __func__); \
        NIAL_BUG();\
    }\
}while(0)

//proc
#if 1
typedef struct nial_proc_dir_entry{
     char name[50];

     void *proc_dir_entry;

     /*linux4.16有效，linux3.0.8无效参数*/
     void* fops;
}nial_proc_entry_t;

extern nial_proc_entry_t *nial_create_proc(nial_proc_entry_t *proc);

#else
typedef struct nial_proc_dir_entry{
     char name[50];
     void *proc_dir_entry;
     int (*open)(struct nial_proc_dir_entry *entry);
     int (*read)(struct nial_proc_dir_entry *entry);
     int (*write)(struct nial_proc_dir_entry *entry, const char *buf, int count, long long *);
     void *private;
     void *seqfile;
     struct nial_list_head node;
}nial_proc_entry_t;
extern nial_proc_entry_t *nial_create_proc_entry(const char *name, nial_proc_entry_t *parent);
extern nial_proc_entry_t *nial_proc_mkdir(const char *name, nial_proc_entry_t *parent);
extern void nial_remove_proc_entry(const char *name, nial_proc_entry_t *parent);
extern int nial_seq_printf(nial_proc_entry_t *entry, const char *fmt, ...);
#endif

//device api
#ifndef _IOC_TYPECHECK
#include "nial_ioctl.h"
#endif


typedef struct nial_dev{
    char name[48];
//#ifdef __LINUX_KERNEL__ 
#if 1
    void *dev;
    int minor;
    struct nial_fileops *fops;
    struct nial_pmops *nial_pmops;
#else
   /* char compatible[128];
    fdtcall_t fn;
    void* data;*/
#endif
}nial_dev_t;

/*typedef struct nial_dev{
    char name[48];
    void *dev;
	void *cdev;
	void *devclass;
	unsigned int major;
	unsigned int minorcnt;
    struct nial_fileops *fops;
    struct nial_pmops *nial_pmops;
}nial_dev_t;*/

typedef struct nial_vm{
    void *vm;
}nial_vm_t;

#define NIAL_POLLIN      0x0001
#define NIAL_POLLPRI     0x0002
#define NIAL_POLLOUT     0x0004
#define NIAL_POLLERR     0x0008
#define NIAL_POLLHUP     0x0010
#define NIAL_POLLNVAL    0x0020
#define NIAL_POLLRDNORM  0x0040
#define NIAL_POLLRDBAND  0x0080
#define NIAL_POLLWRNORM  0x0100


typedef struct nial_poll{
    void *poll_table;
    void *data;
}nial_poll_t;

typedef struct nial_fileops{
    int (*open)(void *private_data);
    int (*read)(char *buf, int size, long *offset, void *private_data);
    int (*write)(const char *buf, int size, long *offset, void *private_data);
    long (*llseek)(long offset, int whence, void *private_data);
    int (*release)(void *private_data);
    long (*unlocked_ioctl) (unsigned int cmd, unsigned long arg, void *private_data);
    unsigned int (*poll) (nial_poll_t *nial_poll, void *private_data);
    int (*mmap) (nial_vm_t *vm, unsigned long start, unsigned long end, unsigned long vm_pgoff, void *private_data);
#ifdef CONFIG_COMPAT
    long (*compat_ioctl) (unsigned int cmd, unsigned long arg, void *private_data);
#endif
}nial_fileops_t;

typedef struct nial_pmops{
    int (*pm_prepare)(nial_dev_t *dev);
    void (*pm_complete)(nial_dev_t *dev);
    int (*pm_suspend)(nial_dev_t *dev);
    int (*pm_resume)(nial_dev_t *dev);
    int (*pm_freeze)(nial_dev_t *dev);
    int (*pm_thaw)(nial_dev_t *dev);
    int (*pm_poweroff)(nial_dev_t *dev);
    int (*pm_restore)(nial_dev_t *dev);
    int (*pm_suspend_late)(nial_dev_t *dev);
    int (*pm_resume_early)(nial_dev_t *dev);
    int (*pm_freeze_late)(nial_dev_t *dev);
    int (*pm_thaw_early)(nial_dev_t *dev);
    int (*pm_poweroff_late)(nial_dev_t *dev);
    int (*pm_restore_early)(nial_dev_t *dev);
    int (*pm_suspend_noirq)(nial_dev_t *dev);
    int (*pm_resume_noirq)(nial_dev_t *dev);
    int (*pm_freeze_noirq)(nial_dev_t *dev);
    int (*pm_thaw_noirq)(nial_dev_t *dev);
    int (*pm_poweroff_noirq)(nial_dev_t *dev);
    int (*pm_restore_noirq)(nial_dev_t *dev);
}nial_pmops_t;

#define NIAL_SEEK_SET 0
#define NIAL_SEEK_CUR 1
#define NIAL_SEEK_END 2

//#define PAGE_SHIFT 12

extern nial_dev_t *nial_createdev(char *name);
extern int nial_destroydev(nial_dev_t *pdev);
extern int nial_registerdevice(nial_dev_t *pdev);
extern void nial_deregisterdevice(nial_dev_t *pdev);
extern void nial_poll_wait(nial_poll_t *table, nial_wait_t * wait);
extern void nial_pgprot_noncached(nial_vm_t *vm);
extern void nial_pgprot_cached(nial_vm_t *vm);
extern void nial_pgprot_writecombine(nial_vm_t *vm);
extern void nial_pgprot_stronglyordered(nial_vm_t *vm);
extern int nial_remap_pfn_range(nial_vm_t *vm, unsigned long addr, unsigned long pfn, unsigned long size);
extern int nial_io_remap_pfn_range(nial_vm_t *vm, unsigned long addr, unsigned long pfn, unsigned long size);

//timer
typedef struct nial_timer{
    void *timer;
    void (*function)(void *);
    unsigned long data;
}nial_timer_t;

typedef struct nial_timeval{
      long tv_sec;
      long tv_usec;
}nial_timeval_t;

typedef struct nial_rtc_time {
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
    int tm_isdst;
} nial_rtc_time_t;

extern int nial_timer_init(nial_timer_t *timer);
extern int nial_set_timer(nial_timer_t *timer, unsigned long interval);//ms
extern int nial_del_timer(nial_timer_t *timer);
extern int nial_timer_destory(nial_timer_t *timer);

extern unsigned long nial_msleep(unsigned int msecs);
extern void nial_udelay(unsigned int usecs);
extern void nial_mdelay(unsigned int msecs);

extern unsigned int nial_get_tickcount(void);
extern unsigned long long nial_sched_clock(void);
extern void nial_gettimeofday(nial_timeval_t *tv);
extern void nial_rtc_time_to_tm(unsigned long time, nial_rtc_time_t *tm);
extern void nial_rtc_tm_to_time(nial_rtc_time_t *tm ,unsigned long *time);
extern int nial_rtc_valid_tm(struct nial_rtc_time *tm);
extern void nial_getjiffies(unsigned long long *pjiffies);

// file ops

#define NIAL_O_ACCMODE       00000003
#define NIAL_O_RDONLY        00000000
#define NIAL_O_WRONLY        00000001
#define NIAL_O_RDWR          00000002
#define NIAL_O_CREAT         00000100
extern void *nial_klib_fopen(const char *filename, int flags, int mode);
extern void nial_klib_fclose(void *filp);
extern int nial_klib_fwrite(char *buf, int len, void *filp);
extern int nial_klib_fread(char *buf, unsigned int len, void *filp);




// reboot
struct nial_notifier_block {
    int (*notifier_call)(struct nial_notifier_block *nb, unsigned long action, void *data);
    void *notifier_block;
};
typedef int (*nial_notifier_fn_t)(struct nial_notifier_block *nb, unsigned long action, void *data);

extern int nial_register_reboot_notifier(struct nial_notifier_block *nb);
extern int nial_unregister_reboot_notifier(struct nial_notifier_block *nb);

#include <stdarg.h>

#ifndef _NIAL_VA_LIST

#define _NIAL_VA_LIST
#define nial_va_list             va_list
#define nial_va_arg(ap, T)       va_arg(ap,T)
#define nial_va_end(ap)          va_end(ap)
#define nial_va_start(ap, A)     va_start(ap,A)

#endif  /* va_arg */


#define NULL_STRING "NULL"

extern void nial_vprintk(const char *fmt, nial_va_list args);
extern int nial_vsnprintf(char *str, int size, const char *fmt, nial_va_list args);

#ifdef CONFIG_HISI_SNAPSHOT_BOOT

#ifndef NIAL_UMH_WAIT_PROC
#define NIAL_UMH_WAIT_PROC   2       /* wait for the process to complete */
#endif

extern int nial_call_usermodehelper_force(char *path, char **argv, char **envp, int wait);
#endif

// platform
unsigned int nial_platform_get_resource_start(void *resource, unsigned int num);
unsigned int nial_platform_get_resource_size(void *resource, unsigned int num);
char * nial_platform_get_resource_name(void *resource, unsigned int num);

int nial_platform_driver_register(void *drv);
void nial_platform_driver_unregister(void *drv);
void* nial_platform_get_resource_byname(void *dev, unsigned int type,
    const char *name);
void* nial_platform_get_resource(void *dev, unsigned int type,
    unsigned int num);
int nial_platform_get_irq(void *dev, unsigned int num);
int nial_platform_get_irq_byname(void *dev, const char *name);


#define nial_module_driver(nial_driver, nial_register, nial_unregister, ...) \
static int __init nial_driver##_init(void) \
{ \
        return nial_register(&(nial_driver)); \
} \
module_init(nial_driver##_init); \
static void __exit nial_driver##_exit(void) \
{ \
        nial_unregister(&(nial_driver)); \
} \
module_exit(nial_driver##_exit);

#define nial_module_platform_driver(platform_driver) \
        nial_module_driver(platform_driver, nial_platform_driver_register, \
                        nial_platform_driver_unregister)

#endif
