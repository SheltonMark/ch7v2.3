#ifndef __NI_NIAL_USER__
#define __NI_NIAL_USER__

#include "ni_type.h"

#define NI_NIAL_USER_VERSION "1.0"


#ifdef __cplusplus
extern "C" {
#endif


#ifndef PROT_READ
#define PROT_READ
#endif
#ifndef PROT_WRITE
#define PROT_WRITE
#endif
#ifndef MAP_SHARED
#define MAP_SHARED
#endif
#ifndef MAP_FAILED
#define MAP_FAILED (NULL)
#endif

#define NI_MAGIC_NUM   0x4D414749

/* 线程名最大长度 */
#define NI_THREAD_NAME_MAX_LEN 32

/* 
 * thread api
 */
typedef enum
{
    NIAL_SCHED_OTHER  = 0,  /* 分时调度策略 */
    NIAL_SCHED_FIFO,        /* 实时调度策略，先到先服务 */
    NIAL_SCHED_RR,          /* 实时调度策略，时间片轮转 */
    NIAL_SCHED_PRI,         /* 实时调度策略，基于优先级 */

    /* -------------- Only for Linux User mode --------------- */
    NIAL_SCHED_DEFAULT,     /* 系统默认属性(调度策略以及优先级) */
    NIAL_SCHED_INHERIT,     /* 继承创建者当前所在线程的属性(调度策略以及优先级) */

    NIAL_SCHED_NUM
} NIAL_ThrSchedPolicy;


typedef struct
{
    /* 线程绑定到指定的CPU */
    NI_U32          cpuBindMask;  /* 如果值为0,表示不绑定CPU */
    NI_U32          reservedWord; /* 保留字段 */
    NI_U64          reserved[15]; /* 保留字段 */
} NIAL_ThrAttr;

typedef struct
{
    NI_S32  (*OpThrRun)(NI_PTR pUsrArgs); /* 线程运行函数 */

    NI_U16  thrPol;      /* 调度策略，其定义见NIAL_ThrSchedPolicy */
    NI_U16  thrPri;      /* 调度优先级 */

    /*
    栈大小，NIAL_THR_STACK_SIZE_DEFAULT表示默认栈大小，如果为默认大小，
    则pStackAddr不起作用
    */
    NI_U32  stackSize;
    NI_PTR  pStackAddr;  /* 栈起始地址，Linux内核态中无效，不使用时请设为NULL */

    NI_PTR  pUsrArgs;    /* 用户自定义参数 */

    NI_CHAR  *pName;      /* 线程名仅在NI_thrCreateEx函数中有效 */

    /* 线程扩展属性，仅在NI_thrCreateEx函数中有效 */
    NI_PTR_T(NIAL_ThrAttr, pAttr); 

    NI_U32 reserved[1];
} NIAL_ThrCreate;

typedef NI_Handle NIAL_ThrHandle;

/*******************************************************************************
 * * 函数名  : NI_ThrCreate
 * * 描  述  : 创建线程。
 * * 输  入  : - pCreate: 创建参数。
 * * 输  出  : - phThr  : 创建成功的线程句柄
 * * 返回值  : NI_SUCCESS: 成功。
 * *           NI_FAILURE: 失败。
 * *****************************************************************************/
extern NI_S32 NI_ThrCreate(NIAL_ThrCreate *pCreate, NIAL_ThrHandle *phThr);

/*******************************************************************************
 * * 函数名  : NI_ThrCreateEx
 * * 描  述  : 创建线程，该函数与NI_ThrCreate增加支持了pName字段用于记录函数名，并
 * *           且对reserved会进行0值判断，用户调用该函数的时候，务必使reserved清零。
 * * 输  入  : - pCreate: 创建参数。
 * * 输  出  : - phThr  : 返回的句柄。
 * * 返回值  : NI_SUCCESS  : 成功。
 * *           NI_FAILURE: 失败。
 * *******************************************************************************/
extern NI_S32 NI_ThrCreateEx(NIAL_ThrCreate *pCreate, NIAL_ThrHandle *phThr);

/*******************************************************************************
* 函数名  : NI_ThrDelete
* 描  述  : 删除线程。
* 输  入  : - hThr: 线程句柄。
* 输  出  : 无。
* 返回值  : NI_SUCCESS  : 成功。
*           NI_FAILURE: 失败。
*******************************************************************************/
extern NI_S32 NI_ThrDelete(NIAL_ThrHandle hThr);

/*******************************************************************************
* 函数名  : NI_ThrDelete
* 描  述  : 删除线程。
* 输  入  : - hThr: 线程句柄。
* 输  出  : 无。
* 返回值  : NI_SUCCESS  : 成功。
*           NI_FAILURE: 失败。
*******************************************************************************/
extern NI_S32 NI_ThrJoin(NIAL_ThrHandle hThr);

/*******************************************************************************
* 函数名  : NI_ThrChangePri
* 描  述  : 修改线程优先级。
* 输  入  : - hThr  : 线程句柄。
            - thrPri: 线程优先级。
* 输  出  : 无。
* 返回值  : NI_SUCCESS  : 成功。
*           NI_FAILURE: 失败。
*******************************************************************************/
extern NI_S32 NI_ThrChangePri(NIAL_ThrHandle hThr, NI_U32 thrPri);

/*******************************************************************************
* 函数名  : NI_ThrExit
* 描  述  : 主动退出线程。
* 输  入  : - pRetVal: 退出后返回的值。
* 输  出  : 无。
* 返回值  : NI_SUCCESS  : 成功。
*           NI_FAILURE: 失败。
*******************************************************************************/
extern NI_S32 NI_ThrExit(NI_PTR pRetVal);

/*******************************************************************************
* 函数名  : NI_ThrGetTid
* 描  述  : 获取线程号。
* 输  入  : 无。
* 输  出  : 无。
* 返回值  : 号。
*******************************************************************************/
extern NI_S32 NI_ThrGetTid(NI_VOID);

/*******************************************************************************
* 函数名  : NI_ThrGetPid
* 描  述  : 获取进程号。
* 输  入  : 无。
* 输  出  : 无。
* 返回值  : 进程号。
*******************************************************************************/
extern NI_S32 NI_ThrGetPid(NI_VOID);

/*******************************************************************************
 * * 函数名  : NI_ThrGetMaxPri
 * * 描  述  : 获取线程最大优先级。
 * * 输  入  : 调度策略
 * * 输  出  : 无
 * * 返回值  : 最大优先级值
 * *******************************************************************************/
extern NI_S32 NI_ThrGetMaxPri(NI_U32 thrPol);

/*******************************************************************************
 * * 函数名  : NI_ThrGetMinPri
 * * 描  述  : 获取线程最小优先级
 * * 输  入  : 调度策略
 * * 输  出  : 无
 * * 返回值  : 最优先级值
 * *******************************************************************************/
extern NI_S32 NI_ThrGetMinPri(NI_U32 thrPol);


/* 
 * semaphore api
 */
typedef NI_Handle NIAL_SemHandle;

/*******************************************************************************
* 函数名  : NI_semCreate
* 描  述  : 该函数负责创建一个信号量
*           该接口不能在中断上下文调用
*
* 输  入  : - val:      信号量的初始值
*
* 输  出  : - phSem:    信号量 句柄指针,当创建成功时输出信号量句柄
* 返回值  :  NI_SUCCESS:   创建成功
*            NI_FAILURE: 创建失败
*******************************************************************************/
extern NI_S32 NI_SemCreate(NI_U32 val, NIAL_SemHandle *phSem);

/*******************************************************************************
* 函数名  : NI_SemWait
* 描  述  : 信号量获取操作,
*           该接口不能在中断上下文调用
*
* 输  入  : - hSem:     信号量句柄,
*           - timeOut:  信号量等待超时时间, 单位毫秒，若等待超时则返回NI_ETIMEOUT
*                       NI_TIMEOUT_NONE 表示不等待信号量立即返回，
*                               此时若能获取到信号量则返回NI_SUCCESS
*                               若不能获取信号量则返回NI_FAILURE
*                       NI_TIMEOUT_FOREVER 表示永远等待信号量
*
* 输  出  : 无
* 返回值  :  NI_SUCCESS: 获取信号量成功
*            NI_FAILURE: 获取信号量失败
*            NI_ETIMEOUT: 获取信号量超时
*******************************************************************************/
extern NI_S32 NI_SemWait(NIAL_SemHandle hSem, NI_U32 timeOut);

/*******************************************************************************
* 函数名  : NI_semPost
* 描  述  : 信号量释放操作
*           该接口可以在中断上下文调用
*
* 输  入  : - hSem: 信号量句柄
*
* 输  出  : 无。
* 返回值  : NI_SUCCESS: 成功
*           NI_FAILURE: 失败
*******************************************************************************/
extern NI_S32 NI_SemPost(NIAL_SemHandle hSem);

/*******************************************************************************
* 函数名  : NI_semGetCnt
* 描  述  : 获取信号量的个数
* 输  入  : - hSem: 信号量句柄
* 输  出  : - 信号量的个数
* 返回值  : NI_SUCCESS  : 成功
*           NI_FAILURE: 失败
*******************************************************************************/
extern NI_S32 NI_SemGetCount(NIAL_SemHandle hSem);

/*******************************************************************************
* 函数名  : NI_semDelete
* 描  述  : 该函数负责销毁一个信号量
*           该接口不能在中断上下文调用
*
* 输  入  : - hSem: 信号量句柄
*
* 输  出  : 无。
* 返回值  : NI_SUCCESS:   成功
*           NI_FAILURE: 失败
*******************************************************************************/
extern NI_S32 NI_SemDelete(NIAL_SemHandle hSem);

/*
 * mutex api
 */
typedef enum
{
    NIAL_MUTEX_NORMAL = 0,      /* 普通锁,不支持递归 */
    NIAL_MUTEX_RECURSIVE        /* 递归锁,支持递归加锁,即同一线程可以对锁加锁多次*/
} NIAL_MutexType;

typedef NI_Handle NIAL_MutexHandle;

/*******************************************************************************
* 函数名  : NI_MutexCreate
* 描  述  : 该函数负责创建一个互斥锁
*           该接口不能在内核态中断上下文调用
*
* 输  入  : - type:    互斥锁类型,参见NIAL_MutexType定义
*
* 输  出  : - phMutex: 互斥锁句柄指针,当创建成功时输出互斥锁句柄
* 返回值  : NI_SUCCESS: 创建成功
*           NI_FAILURE: 创建失败
*******************************************************************************/
extern NI_S32 NI_MutexCreate(NI_U32 type, NIAL_MutexHandle *phMutex);

/*******************************************************************************
* 函数名  : NI_MutexLock
* 描  述  : 互斥锁加锁
*           该接口不能在内核态中断上下文调用
* 输  入  : - hMutex: 互斥锁句柄,
*
* 输  出  : 无
* 返回值  : NI_SUCCESS: 成功
*           NI_FAILURE: 失败
*******************************************************************************/
extern NI_S32 NI_MutexLock(NIAL_MutexHandle hMutex);

/*******************************************************************************
* 函数名  : NI_MutexTryLock
* 描  述  : 试图对互斥锁进行加锁。
* 输  入  : - hMutex: 互斥锁句柄,
*
* 输  出  : 无
* 返回值  : NI_SUCCESS: 成功
*           其他值: 获取锁失败的具体含义
*******************************************************************************/
extern NI_S32 NI_MutexTryLock(NIAL_MutexHandle hMutex);

/*******************************************************************************
* 函数名  : NI_MutexUnlock
* 描  述  : 互斥锁解锁
* 输  入  : - hMutex: 互斥锁句柄,
*
* 输  出  : 无
* 返回值  : NI_SUCCESS: 成功
*           NI_FAILURE: 失败
*******************************************************************************/
extern NI_S32 NI_MutexUnlock(NIAL_MutexHandle hMutex);

/*******************************************************************************
* 函数名  : NI_MutexDelete
* 描  述  : 该函数负责销毁一个互斥锁
*
* 输  入  : - hMutex: 互斥锁句柄
*
* 输  出  : 无。
* 返回值  : NI_SUCCESS: 成功
*           NI_FAILURE: 失败
*******************************************************************************/
extern NI_S32 NI_MutexDelete(NIAL_MutexHandle hMutex);

/*
 * time api
 */
typedef struct
{
    NI_S32 tvSec;     /* 秒 */
    NI_S32 tvUsec;    /* 微秒 */
} NIAL_TimeVal;

typedef struct
{
    NI_S64 tvSec;     /* 秒 */
    NI_S64 tvUsec;    /* 微秒 */
} NIAL_TimeVal64;

typedef struct
{
    /*获取jiffies时间函数*/
    NI_U64 (* OpGetTimeOfJiffies)(NI_VOID);

    /*获取utc时间函数*/
    NI_S32 (* OpGetTimeOfDay)(NIAL_TimeVal *pUtcTime);

    /*获取utc时间函数*/
    NI_S32 (* OpGetTimeOfDay64)(NIAL_TimeVal64 *pUtcTime);

    /*预留*/
    NI_S32 reserved[5];

} NIAL_MctsRegisterFun;

/******************************************************************************* 
*  函数名: NI_RegisterMcts
*  描   述   :注册时间同步模块的获取时间函数
*  输   入   : 
*  输   出   : 
*  返回值: NI_SUCCESS: 成功。
*          NI_FAILURE: 失败。
*******************************************************************************/ 
extern NI_S32 NI_RegisterMcts(NIAL_MctsRegisterFun *pFun);

/*******************************************************************************
* 函数名  : NI_GetTimeOfDay
* 描  述  : 获取当前时间。
*
* 输  入  : - pTimeVal: 时间表述结构，由秒和微妙组成。
*
* 输  出  : 无
* 返回值  : NI_SUCCESS  : 成功。
*           NI_FAILURE: 失败。
*******************************************************************************/
extern NI_S32 NI_GetTimeOfDay(NIAL_TimeVal *pTimeVal);

/*******************************************************************************
* 函数名  : NI_GetTimeOfDay64
* 描  述  : 获取当前时间。
*
* 输  入  : - pTimeVal: 时间表述结构，由秒和微妙组成。
*
* 输  出  : 无
* 返回值  : NI_SUCCESS  : 成功。
*           NI_FAILURE: 失败。
*******************************************************************************/
extern NI_S32 NI_GetTimeOfDay64(NIAL_TimeVal64 *pTimeVal);

/*******************************************************************************
* 函数名  : NI_GetTimeOfJiffies
* 描  述  : 获取系统单调递增的时间。
* 输  入  : 无。
* 输  出  : 无。
* 返回值  : 时间值，单位是毫秒。
*******************************************************************************/
extern NI_U64 NI_GetTimeOfJiffies(NI_VOID);

/*******************************************************************************
* 函数名  : NI_GetCurTimeInMsec
* 描  述  : 获取当前时刻的毫秒数
*
* 输  入  : 无
*
* 输  出  : 无
* 返回值  : 当前系统毫秒数
*******************************************************************************/
extern NI_U64 NI_GetCurTimeInMsec(NI_VOID);

/*******************************************************************************
* 函数名  : NI_MSleep
* 描  述  : 毫秒级延时,延时过程中会休眠
*
* 输  入  : - ms: 毫秒数
*
* 输  出  : 无
* 返回值  : 无
*******************************************************************************/
extern NI_VOID NI_MSleep(NI_U32 ms);

/*******************************************************************************
* 函数名  : NI_USleep
* 描  述  : 毫秒级延时,延时过程中会休眠
*
* 输  入  : - ms: 微秒数
*
* 输  出  : 无
* 返回值  : 无
*******************************************************************************/
extern NI_VOID NI_USleep(NI_U32 us);

/*******************************************************************************
* 函数名  : NI_mdelay
* 描  述  : 毫秒级延时,延时过程中不休眠忙等
*
* 输  入  : - ms: 毫秒数
*
* 输  出  : 无
* 返回值  : 无
*******************************************************************************/
extern NI_VOID NI_MDelay(NI_U32 ms);

/*******************************************************************************
* 函数名  : NI_udelay
* 描  述  : 微秒级延时,延时过程中不休眠忙等
*
* 输  入  : - us: 微秒数
*
* 输  出  : 无
* 返回值  : 无
*******************************************************************************/
extern NI_VOID NI_UDelay(NI_U32 us);

/*******************************************************************************
* 函数名  : NI_TickToMsec
* 描  述  : tick转换至毫秒
*
* 输  入  : - tick
*
* 输  出  : 无
* 返回值  : 毫秒
*******************************************************************************/
extern NI_U64 NI_TickToMsec(NI_U32 tick);

/*******************************************************************************
* 函数名  : NI_MsecToTick
* 描  述  : 毫秒转换至tick
*
* 输  入  : - 毫秒
*
* 输  出  : 无
* 返回值  : tick值
*******************************************************************************/
extern NI_U64 NI_MsecToTick(NI_U32 ms);

/*
 * timer api
 */
typedef NI_Handle NIAL_TimerHandle;

typedef NI_S32 (*NI_TimerCallBack)(NIAL_TimerHandle hTimer, NI_PTR pUserArgs);

typedef enum
{
    NIAL_TIMER_ONCE = 0,    /*Start后只执行一次的定时器*/
	NIAL_TIMER_LOOP         /*Start后循环执行的定时器*/
} NIAL_TimerType;

typedef struct
{
    NI_TimerCallBack userFunc;             /*定时器的回调处理函数*/
    NI_PTR pUserArgs;                       /*传给回调处理函数的用户参数*/

    NI_U32 type;                            /*定时器类型,参见NIAL_TimerType定义*/

    /*定时器超时时间,单位为毫秒*/
    /*类型为NIAL_TIMER_LOOP的定时器expireMs 表示启动后每隔多少毫秒执行一次回调函数*/
    /*类型为NIAL_TIMER_ONCE的定时器expireMs 表示启动后隔多少毫秒执行回调函数*/
    /*对于类型为NIAL_TIMER_LOOP的定时器,*/
    /*expireMs必须大于等于NIAL_TIMER_INTV_MIN, 以避免用户设得过短，导致CPU过高*/
    /*如果expireMs小于NIAL_TIMER_INTV_MIN, 程序将自动设为NIAL_TIMER_INTV_MIN*/
    NI_U32 expireMs;

    NI_U32 reserved[4];                     /*保留*/
} NIAL_TimerInitParams;

extern NI_S32 NI_TimerCreate(NIAL_TimerInitParams *pParams, NIAL_TimerHandle *phTimer);
extern NI_S32 NI_TimerStart(NIAL_TimerHandle hTimer);
extern NI_S32 NI_TimerStop(NIAL_TimerHandle hTimer);
extern NI_S32 NI_TimerDelete(NIAL_TimerHandle hTimer);

/*
 * memory api
 */

/*******************************************************************************
* 函数名  : NI_MemAlloc
* 描  述  : 该函数负责在用户态中申请内存
*
* 输  入  : - size:  要申请的内存大小
*           - align: 用户自定义对齐的字节数, 若为0表示不进行自定义对齐
*                    该参数必须为4的整数倍, 否则函数将返回NULL
*
* 输  出  : 无。
* 返回值  : 非NULL: 申请成功
*           NULL:   申请失败
*******************************************************************************/
extern NI_PTR NI_MemAlloc(NI_U32 size, NI_U32 align);

/*******************************************************************************
* 函数名  : NI_MemCalloc
* 描  述  : 该函数在NI_MemAlloc的基础上,增加对内存清0的动作
*
* 输  入  : - size:  要申请的内存大小
*           - align: 用户自定义对齐的字节数, 若为0表示不进行自定义对齐
*                    该参数必须为4的整数倍, 否则函数将返回NULL
*
* 输  出  : 无。
* 返回值  : 非NULL:  申请成功
*           NULL:    申请失败
*******************************************************************************/
extern NI_PTR NI_MemCalloc(NI_U32 size, NI_U32 align);

/*******************************************************************************
* 函数名  : NI_MemFree
* 描  述  : 该函数负责在用户态中释放内存
*
* 输  入  : - pPtr:    要释放的内存指针
* 输  出  : 无
* 返回值  : NI_SUCCESS: 成功,内存已释放
*           NI_FAILURE: 失败, 内存未释放
*******************************************************************************/
extern NI_S32 NI_MemFree(NI_PTR ptr);


/*
 * message queue api
 */

//消息队列的操作模式
typedef enum
{
    NIAL_MqMODE_RDONLY  = 00, /* 打开消息队列，只接收消息*/
    NIAL_MqMODE_WRONLY  = 01, /* 打开消息队列，只发送消息 */
    NIAL_MqMODE_RDWR    = 02, /* 打开消息队列，接收和发送消息*/
    NIAL_MqMODE_O_CREAT = 0100, /* 创建消息队列 */
    NIAL_MqMODE_O_EXCL  = 0200,    /* 消息队列创建时发现已有同名队列则返回报错, 原子操作 */
    NIAL_MqMODE_O_NONBLOCK = 04000    /* 设置消息队列为非阻塞式 */
} NIAL_MqMode;

typedef NI_Handle NIAL_MqHandle;

extern NI_S32 NI_MqOpen(NI_U32 MsgNum, NIAL_MqHandle *phMq);
extern NI_S32 NI_MqUnlink(NIAL_MqHandle hMq);
extern NI_S32 NI_MqClose(NIAL_MqHandle hMq);
extern NI_S32 NI_MqSend(NIAL_MqHandle hMq, NI_U32 msgLen, NI_U32 us);
extern NI_U32 NI_MqReceive(NIAL_MqHandle hMq, NI_U32 msgLen, NI_U32 us);

/*
 * file api
 */

/* 文件的操作模式*/
typedef enum
{
    NIAL_FILEMODE_RDONLY  = 0, /* 只读, 若文件不存在将失败*/
    NIAL_FILEMODE_WRONLY,      /* 只写,从文件头开始写,原有数据将被清掉, 若文件不存在将创建*/
    NIAL_FILEMODE_RDWR,        /* 读写,从文件头开始写,原有数据将被清掉, 若文件不存在将创建*/
    NIAL_FILEMODE_WRAPPEND,    /* 追加写,从文件尾部追加写, 若文件不存在将创建*/
    NIAL_FILEMODE_RDWR_NOCLR,  /* 从文件读写打开,不清数据,若文件不存在将创建*/
    NIAL_FILEMODE_FEXIST       /* 测试文件是否存在 */
} NIAL_FileMode;

typedef NI_Handle NIAL_FileHandle;

/*文件的Seek类型*/
typedef enum
{
    NIAL_FILESEEK_SET = 0,     /* 从文件头开始Seek*/
    NIAL_FILESEEK_CUR,         /* 从文件当前位置开始Seek*/
    NIAL_FILESEEK_END          /* 从文件尾开始Seek*/
} NIAL_FileSeekType;

extern NI_S32 NI_FileOpen(NI_CHAR *fileName, NIAL_FileMode mode,
							NIAL_FileHandle *phFile);
extern NI_S32 NI_FileRead(NIAL_FileHandle hFile, NI_CHAR *buffer, NI_U32 size);
extern NI_S32 NI_FileWrite(NIAL_FileHandle hFile, NI_CHAR *buffer, NI_U32 size);
extern NI_S32 NI_FileClose(NIAL_FileHandle hFile);
extern NI_S32 NI_FileAccess(NIAL_FileHandle hFile, NIAL_FileMode mode);
extern NI_S32 NI_FileRemove(NIAL_FileHandle hFile);
extern NI_S32 NI_FileSync(NIAL_FileHandle hFile);
extern NI_S32 NI_FileSeek(NIAL_FileHandle hFile, NI_S32 offset, NI_S32 type);
extern NI_S32 NI_FileTell(NIAL_FileHandle hFile, NI_U32 *pOffset);
extern NI_S32 NI_Fgets(NI_CHAR *s, NI_S32 size, NIAL_FileHandle hFile);


/*
 * debug api
 */

#if 0
typedef struct {
	//TODO
    NI_U32 flags;     /* object status flags */
    struct nial_list_head object_list;
    struct nial_list_head gray_list;
    struct rb_node rb_node;
    struct rcu_head rcu;        /* object_list lockless traversal */
    /* object usage count; object freed when use_count == 0 */
    atomic_t use_count;
    NI_U32 pointer;
    size_t size;
    /* pass surplus references to this pointer */
    unsigned long excess_ref;
    /* minimum number of a pointers found before it is considered leak */
    NI_S32 min_count;
    /* the total number of pointers found pointing to this object */
    NI_S32 count;
    /* checksum for detecting modified objects */
    NI_U32 checksum;
    /* memory ranges to be scanned inside an object (empty for all) */
    struct nial_hlist_head area_list;
	unsigned long trace[16];
    NI_U32 trace_len;
    NI_U64 jiffies;      /* creation timestamp */
    NI_Handle task;         /* handle of the current task */
    NI_CHAR comm[16];    /* executable name */

} NIAL_MemLeakObj;

extern NI_VOID memleak_init(NI_VOID);
extern NI_VOID memleak_alloc(const void *ptr, size_t size, int min_count,
       			gfp_t gfp) __ref;
extern NI_VOID memleak_free(const void *ptr) __ref;
extern NI_VOID memleak_free_part(const void *ptr, size_t size) __ref;
extern NI_VOID memleak_update_trace(const void *ptr) __ref;
extern NI_VOID memleak_not_leak(const void *ptr) __ref;
extern NI_VOID memleak_ignore(const void *ptr) __ref;
extern NI_VOID memleak_scan_area(const void *ptr, size_t size, gfp_t gfp) __ref;
extern NI_VOID memleak_no_scan(const void *ptr) __ref;
#endif

#ifdef __cplusplus
}
#endif

#endif
