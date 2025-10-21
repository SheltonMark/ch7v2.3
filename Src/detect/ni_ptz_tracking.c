/*******************************************************************************
 * Copyright (C) 2020-2030 ZheJiang Chipup Technology CO.,LTD.
 *
 * 文件名 : ptz_tracking.c
 *
 * 作  者 : Chipup Software Team
 * 版  本 : V1.0.0
 * 日  期 : 2022年09月09日 Create
 *
 * 描  述 : 
 *
 *       1. 硬件说明
 *          无。
 *
 *       2. 程序结构说明。
 *          无。
 *
 *       3. 使用说明。
 *          无。
 *
 *       4. 局限性说明。
 *          无。
 *
 *       5. 其他说明。
 *          无。
 *
 * 修订历史: 
 * 日    期: 2022-09-09
 * 修订版本:
 * 作    者: Chipup Software Team
 * 修订备注:
 *******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <semaphore.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/syscall.h>

#include <list.h>

#include "ni_errno.h"
#include "ni_macro.h"

#include "ni_comm_vps.h"
#include "mdk_vps.h"

#include "ni_ias_od.h"

#include "PtzTracking.h"
#include "PtzOption.h"
#include "ni_ptz_tracking.h"

#include "motor.h"
#include "TdCommon.h"

#define PTZ_TRACKING_PTZ_EN 0

typedef struct tagPtzTrackingPtzResetButt
{
    NI_PTZ_TRACKING_PTZ_CMD_TYPE_E enType;
}PTZ_TRACKING_PTZ_CMD_BUTT_S;

typedef struct tagPtzTrackingParamButt
{
    NI_PTZ_TRACKING_PARAM_TYPE_E enType;
}PTZ_TRACKING_PARAM_BUTT_S;

typedef struct tagPtzTrackingRegFuncCbButt
{
    NI_PTZ_TRACKING_CB_FUNC_ID_E    enFuncId;
} PTZ_TRACKING_REG_FUNC_CB_BUTT_S;


typedef struct tagPtzTrackingMmz
{
    NI_U32          u32PhyAddr;
    NI_VOID         *pvVirAddr;
    NI_U32          u32BufLen;
} PTZ_TRACKING_MMZ_S;

typedef struct tagPtzTrackingFuncCallBack
{
    NI_PTZ_TRACKING_OD_RESULT_PROCESS_CB_S stOdResultProcessCb;
} PTZ_TRACKING_FUNC_CALL_BACK_S;

typedef struct stPtzTrackingOptPos
{
    NI_U32  u32PosX;                                /*  水平坐标，取值范围0~8192 */
    NI_U32  u32PosY;                                /*  竖直坐标，取值范围0~8192 */
}PTZ_TRACKING_OPT_POS_S;

typedef struct stPtzTrackingOldResult
{
     NI_S32       s32OldIndex; 
     NI_S32       s32OldRectSize; 
     NI_U32       s32OldObjNum;
     NI_S32       s32OldMaxId;
     NI_S32       s32OldObjId;
     NI_S32       s32NoObjCount;
     NI_S32       s32OldOffset;
     NI_S32       s32ConstObjCount;
     NI_S32       s32OldxCenter;
     NI_S32       s32Xl;
     NI_S32       s32Xr;
     NI_S32       s32Yl;
     NI_S32       s32Yr;
     NI_S32       s32Count;
     NI_S32       s32VrialID;
    
}PTZ_TRACKING_OLDRESULT_S;

typedef union stPtzTrackingUsrCmd
{
    PTZ_TRACKING_PTZ_CMD_BUTT_S stPtzCmd;
    NI_PTZ_TRACKING_PTZ_POS_S   stPtzPosCmd;
    NI_PTZ_TRACKING_PTZ_RESET_S stRstCmd;
    NI_U32 au32Rev[8];
}PTZ_TRACKING_USR_CMD_U;

typedef struct stPtzTrackingUsrParamAttr
{
    NI_U32                       u32ObjTolerance;
    NI_U32                       u32MoveKpx;
    NI_BOOL                      bStart;
    NI_BOOL                      bMirror;
    NI_BOOL                      bflip;
}PTZ_TRACKING_USR_PARAM_ATTR_S;

typedef struct stPtzTrackingUsrParam
{
    PTZ_TRACKING_USR_PARAM_ATTR_S   stPtzAdjAttr;
    NI_BOOL                         bSetThreadRun;
}PTZ_TRACKING_USR_PARAM_S;

typedef struct stPtzTrackingLargeScore
{
    NI_S32                          s32Xl;
    NI_S32                          s32Xr;
    NI_S32                          s32Yl;
    NI_S32                          s32Yr;
    NI_S32                          s32ObjId;
}PTZ_TRACKING_LARGESCORE_S;

typedef struct tagPtzTrackingCtx
{
    NI_U32                          u32CheckCode;
    PTZ_TRACKING_MMZ_S              stGblMmz;
    PTZ_TRACKING_MMZ_S              stOdModelMmz;
    PTZ_TRACKING_MMZ_S              stOcModelMmz;
    NI_PTZ_TRACKING_INTERFACE_S     stInterFace;

    PTZ_TRACKING_FUNC_CALL_BACK_S   stFuncCallBack;

    NI_VOID                         *pvPtzOptHandle;
    PTZ_OPT_INTERFACE_S             *pstPtzOptInterFace;

    pthread_t                       stGetFrameThread;
    pthread_attr_t                  stThreadGetFrameAttr;

    pthread_t                       stOdGetResultThread;
    pthread_attr_t                  stThreadOdGetResultAttr;

    pthread_t                       stUsrCmdThread;
    pthread_attr_t                  stThreadUsrCmd;

    NI_U32                          u32ObjTolerance;
    NI_U32                          u32MoveKpx;

    NI_BOOL                         bObjValid;
    NI_U32                          u32StaticCount;
    IAS_OD_OBJ_RESULT_S             stObjTracked;
    NI_U32                          u32CurObjMaxId;

    NI_U32                          u32NoObjCount;

    NI_BOOL                         bMotorGoBack;
    PTZ_TRACKING_OPT_POS_S          stMotorOrignPos;

    NI_BOOL                         bThreadRun;
    NI_BOOL                         bThreadCreate;
    NI_BOOL                         bPtzRun;
    NI_BOOL                         bMirror;
    NI_BOOL                         bflip;
    NI_BOOL                         bUsrCmdHasRun;

    NI_BOOL                         bUsrCmdPos;

    NI_U32                          u32ProcessingFrame;

    PTZ_TRACKING_OLDRESULT_S        stOldResult;

    PTZ_TRACKING_LARGESCORE_S       pstLargeScore[20];

    NI_BOOL                         bDelayEn;
    NI_BOOL                         bUsrCmdSync;
    PTZ_TRACKING_USR_CMD_U          unUsrCmd;

    NI_BOOL                         bUsrParamSync;
    PTZ_TRACKING_USR_PARAM_S        stUsrParam;

    PTZ_TRACKING_CFG_PHRASE_S       stCfgPhrase;
}PTZ_TRACKING_CTX;


// typedef struct tagIasOdHumanResult
// {
//     IAS_ALG_HANDLE                  Handle;            /* 句柄信息 */
//     IAS_TASK_IMAGE_S                stFrame;           /* 结果对应的图 */
//     IAS_OD_OBJ_RESULT_S             pstObj[10];           /* 目标结果存放地址， 由算法库分配内存， 用户拿到后需要将信息拷贝走 */
//     NI_U32                          u32ObjNum;         /* 目标数量 */       
// } IAS_OD_RESULT_S_HUMAN;


static pthread_mutex_t gstLock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t gstApiLock = PTHREAD_MUTEX_INITIALIZER;
static sem_t gstSemFrameProc;
static sem_t gstSemResultProc;
// static sem_t gstSemSleepProc;
pthread_cond_t gstCondSleepProc = PTHREAD_COND_INITIALIZER;
static PTZ_TRACKING_CTX *gpstDevCtx = NI_NULL;
static int ResetState = FALSE;
static char ReviseGoBackPoint = FALSE;
static int xGoBackPoint = -1;
static int yGoBackPoint = -1;
static int HdOcExits = FALSE;
VPS_FRAME_INFO_S gstFrameInfo;
#if 0
#define PRINT_ERROR(level, fmt, args...) \
do{\
    if (level <= 3)\
    {\
        NI_PRINT(RED "ERROR (%s|%d): " fmt NONE_C, __func__, __LINE__, ## args); \
    }\
    else if (level <= 6)\
    {\
        NI_PRINT(GREEN "INFO (%s|%d): " fmt NONE_C, __func__, __LINE__, ## args); \
    }\
    else if (level >= 7)\
    {\
        NI_PRINT(PURPLE "DEBUG (%s|%d): " fmt NONE_C, __func__, __LINE__, ## args); \
    }\
}while(0)
#endif

#define LOCK_INIT(lock)    pthread_mutex_init((lock), NULL);
#define LOCK_DEINIT(lock)  pthread_mutex_destroy((lock));
#define LOCK(lock)         pthread_mutex_lock((lock));//NI_INFO("lock 0x%x\n", lock);
#define UNLOCK(lock)       pthread_mutex_unlock((lock));//NI_INFO("unlock 0x%x\n", lock)

static NI_VOID _ptz_tracking_cal_get_abs_time_wait(NI_U32 u32Ms, struct timespec *pstAbstime)
{
    struct timeval tv;
    NI_U64 u64Absmsec;
    
    gettimeofday(&tv, NULL);
    u64Absmsec = tv.tv_sec * 1000ll + tv.tv_usec / 1000ll;
    u64Absmsec += u32Ms;

    pstAbstime->tv_sec = u64Absmsec / 1000ll;
    pstAbstime->tv_nsec = u64Absmsec % 1000ll * 1000000ll;
}

static NI_VOID _ptz_tracking_cal_wake_up(sem_t *pstSem)
{
    sem_post(pstSem);
}

static NI_S32 _ptz_tracking_cal_wait_event_timeout(sem_t *pstSem, NI_U32 u32TimeOut)
{
    struct timespec stTs;

    _ptz_tracking_cal_get_abs_time_wait(u32TimeOut, &stTs);
    return sem_timedwait(pstSem, &stTs);
}

static NI_S32 _ptz_tracking_cal_cond_signal(pthread_cond_t *pstCont)
{
    return pthread_cond_signal(pstCont);  
}

// static NI_S32 PtzTrackingCalCondBroadcast(pthread_cond_t *pstCont)
// {
//     return pthread_cond_broadcast(pstCont);  
// }

static NI_S32 _ptz_tracking_cal_cond_wait_event_timeout(pthread_cond_t *pstCont, pthread_mutex_t *pstMutex, NI_U32 u32TimeOut)
{
    struct timespec stTs;

    _ptz_tracking_cal_get_abs_time_wait(u32TimeOut, &stTs);
    return pthread_cond_timedwait(pstCont, pstMutex, &stTs);  
}

static NI_S32 _ptz_tracking_cal_gbl_mmz_alloc(PTZ_TRACKING_CTX **ppstCtx, NI_PTZ_TRACKING_CREATE_S *pstCreate)
{
    NI_S32 s32Ret;
    PTZ_TRACKING_MMZ_S stMmz;
    PTZ_TRACKING_CTX *pstCtx = NI_NULL;
    stMmz.u32BufLen = NI_Align(sizeof(PTZ_TRACKING_CTX), 16);

    s32Ret = NI_MDK_SYS_MmzAlloc(&stMmz.u32PhyAddr, &stMmz.pvVirAddr, stMmz.u32BufLen, "ptztrack", NI_NULL);
    if(s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("ptz track mmz alloc faild\n");
        return NI_FAILURE;
    }

    //PRINT_ERROR("gbl vir 0x%x phy 0x%x %d\n", stMmz.pvVirAddr, stMmz.u32PhyAddr, stMmz.u32BufLen);

    memset(stMmz.pvVirAddr, 0, stMmz.u32BufLen);

    pstCtx = (PTZ_TRACKING_CTX *)stMmz.pvVirAddr;

    pstCtx->stGblMmz.pvVirAddr = stMmz.pvVirAddr;
    pstCtx->stGblMmz.u32PhyAddr = stMmz.u32PhyAddr;
    pstCtx->stGblMmz.u32BufLen = stMmz.u32BufLen;

    pstCtx->u32CheckCode = 0xabcddcba;
    *ppstCtx = pstCtx;

    return NI_SUCCESS;
}

static NI_S32 _ptz_tracking_cal_gbl_mmz_free(PTZ_TRACKING_MMZ_S *pstMmz)
{
    NI_S32 s32Ret;

    s32Ret = NI_MDK_SYS_MmzFree(pstMmz->u32PhyAddr, pstMmz->pvVirAddr);
    if(s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("ptz track mmz free faild\n");
        return NI_FAILURE;
    }

    return NI_SUCCESS;
}

static NI_S32 _ptz_tracking_cal_init_motor(PTZ_TRACKING_CTX *pstCtx)
{
    NI_S32 s32Ret;
    //PTZ_OPT_CREATE_S stCreate;
    //PTZ_OPT_POS_S stOptPos;

#if PTZ_TRACKING_PTZ_EN
    stCreate.u32Type = 0;
    stCreate.pszCfgFile = NI_NULL;
    s32Ret = PTZ_OPT_Create(&pstCtx->pvPtzOptHandle, &stCreate);
    if(s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("creat ptz opt faild\n");
        return s32Ret;
    }

    s32Ret = PTZ_OPT_GetFunc(pstCtx->pvPtzOptHandle, &pstCtx->pstPtzOptInterFace);
    if(s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("get ptz opt interface faild\n");
        return s32Ret;
    }

    stOptPos.enPtzCmd = PTZ_OPT_CMD_LOCATION;
    s32Ret = pstCtx->pstPtzOptInterFace->UserPtzCtrlCmd(pstCtx->pvPtzOptHandle, &stOptPos);

    pstCtx->stMotorOrignPos.u32PosX = stOptPos.u32PosX;
    pstCtx->stMotorOrignPos.u32PosY = stOptPos.u32PosY;

#else
    //PtzReset();
    ProductType_e Type;
    PTZ_OPT_PtzPrm stTzPrm;

    memset(&stTzPrm, 0, sizeof(stTzPrm));
    Type = SystemGetProductType(); 
    stTzPrm.ptzCmd = VIDEOIN_PTZ_LOCATION;
    s32Ret = CtrltPtz(&stTzPrm);
    if(s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("read coord file faild failed\n");
        //初始化云台默认复位点，人形跟踪默认使能时，由于无法读取坐标文件，会导致复位点为0，0
        if (IPC_CP3 == Type || IPC_RP3 == Type)
        {
            stTzPrm.ptzLocation.xCoord = 4192;
            stTzPrm.ptzLocation.yCoord = 6200;
        }
        else
        {
            stTzPrm.ptzLocation.xCoord = 4096;
            stTzPrm.ptzLocation.yCoord = 0;
        }
        PRINT_ERROR("Use Default Coord xCoord:%d yCoord%d\n", stTzPrm.ptzLocation.xCoord, stTzPrm.ptzLocation.yCoord);
    }
    
    pstCtx->stMotorOrignPos.u32PosX = stTzPrm.ptzLocation.xCoord;
    pstCtx->stMotorOrignPos.u32PosY = stTzPrm.ptzLocation.yCoord;
#endif

    return NI_SUCCESS;
}

static NI_S32 _ptz_tracking_cal_deinit_motor(PTZ_TRACKING_CTX *pstCtx)
{
    //NI_S32 s32Ret;

#if PTZ_TRACKING_PTZ_EN
    s32Ret = PTZ_OPT_Destory(pstCtx->pvPtzOptHandle);
    if(s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("creat ptz opt faild\n");
        return s32Ret;
    }
#endif
    return NI_SUCCESS;
}

#if 0
static NI_S32 _ptz_tracking_cal_init_vps_ias_chn(PTZ_TRACKING_CTX *pstCtx)
{
    NI_S32 s32Ret;
    VPS_CHN_ATTR_S stChnAttr;

    s32Ret = NI_MDK_VPS_DisableChn(0, 4);
    // if(s32Ret != NI_SUCCESS)
    // {
    //     PRINT_ERROR("disable vps ias chn faild\n");
    //     return s32Ret;
    // }

    s32Ret = NI_MDK_VPS_DestroyChn(0, 4);
    // if(s32Ret != NI_SUCCESS)
    // {
    //     PRINT_ERROR("destory vps ias chn faild\n");
    //     return s32Ret;
    // }

    memset(&stChnAttr, 0, sizeof(stChnAttr));

    stChnAttr.bCropEn = 0;
    stChnAttr.u32OutWidth = pstCtx->stCfgPhrase.u16IasWidth;
    stChnAttr.u32OutHeight = pstCtx->stCfgPhrase.u16IasHeight;
    stChnAttr.enAttrType = VPS_CHN_ATTR_TYPE_IAS;
    stChnAttr.stIasAttr.u32BuffNum = 4;
    stChnAttr.stIasAttr.enOutFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    stChnAttr.stIasAttr.u32OutFps = 25;
    s32Ret = NI_MDK_VPS_CreateChn(0, 4, &stChnAttr);
    if(s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("creat vps ias chn faild\n");
        return s32Ret;
    }

    s32Ret = NI_MDK_VPS_EnableChn(0, 4);
    if(s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("enable vps ias chn faild\n");
        return s32Ret;
    }

    return NI_SUCCESS;
}
#endif

#if 0
static NI_S32 _ptz_tracking_cal_deinit_vps_ias_chn()
{
    NI_S32 s32Ret;

    s32Ret = NI_MDK_VPS_DisableChn(0, 4);
    if(s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("disable vps ias chn faild\n");
        return s32Ret;
    }

    s32Ret = NI_MDK_VPS_DestroyChn(0, 4);
    if(s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("destory vps ias chn faild\n");
        return s32Ret;
    }

    return NI_SUCCESS;
}
#endif

static NI_S32 _ptz_tracking_cal_get_model_file(PTZ_TRACKING_CTX *pstCtx)
{
    NI_S32 s32Ret;
    FILE *fOdFile;
    NI_U32 u32Size;
    NI_U32 u23PhyAddr;
    NI_VOID *pvVirAddr = NI_NULL;

    fOdFile = fopen(pstCtx->stCfgPhrase.aszModelFile, "rb");
    if (NI_NULL == fOdFile)
    {
        PRINT_ERROR("open file %s error\n", pstCtx->stCfgPhrase.aszModelFile);
        return NI_FAILURE;
    }
    //else
    //{
    //    PRINT_ERROR("open model file %s success !\n", pstCtx->stCfgPhrase.aszModelFile);
    //}

    fseek(fOdFile, 0, SEEK_END);
    u32Size = ftell(fOdFile);
    rewind(fOdFile);

    s32Ret = NI_MDK_SYS_MmzAlloc(&u23PhyAddr, &pvVirAddr, u32Size, "odmodelfile", NI_NULL);
    if(s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("od model mmz alloc faild\n");
        fclose(fOdFile);
        return s32Ret;
    }

    fread(pvVirAddr, u32Size, 1, fOdFile);
    fclose(fOdFile);

    pstCtx->stOdModelMmz.pvVirAddr = pvVirAddr;
    pstCtx->stOdModelMmz.u32PhyAddr = u23PhyAddr;
    pstCtx->stOdModelMmz.u32BufLen = u32Size;

    s32Ret = access(IAS_HD_OC_FILE, F_OK);
    if (RETURN_OK == s32Ret)
    {
        fOdFile = fopen(IAS_HD_OC_FILE, "rb");
        if (NULL == fOdFile)
        {
            PRINT_ERROR("open file %s error\n", IAS_HD_OC_FILE);
            return FAILED;
        }
        else
        {
            PRINT_INFO("open model file %s success !\n", IAS_HD_OC_FILE);
        }

        fseek(fOdFile, 0, SEEK_END);
        u32Size = ftell(fOdFile);
        rewind(fOdFile);

        s32Ret = NI_MDK_SYS_MmzAllocCached(&u23PhyAddr, &pvVirAddr, u32Size, "hdocmodelfile", NI_NULL);
        if (s32Ret != SUCCESS)
        {
            PRINT_ERROR("call NI_MDK_SYS_MmzAlloc error 0x%x", s32Ret);
            fclose(fOdFile);
            return s32Ret;
        }

        fread(pvVirAddr, u32Size, 1, fOdFile);
        pstCtx->stOcModelMmz.pvVirAddr = pvVirAddr;
        pstCtx->stOcModelMmz.u32PhyAddr = u23PhyAddr;
        pstCtx->stOcModelMmz.u32BufLen = u32Size;
        fclose(fOdFile);
        HdOcExits = TRUE;

        SystemAsh("rm -rf %s", IAS_HD_OC_FILE);
    }

    return NI_SUCCESS;
}

static NI_S32 _ptz_tracking_cal_release_model_file(PTZ_TRACKING_CTX *pstCtx)
{
    NI_S32 s32Ret;

    s32Ret = NI_MDK_SYS_MmzFree(pstCtx->stOdModelMmz.u32PhyAddr, pstCtx->stOdModelMmz.pvVirAddr);
    if(s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("od model mmz free faild\n", s32Ret);
        return s32Ret;
    }

    if(HdOcExits)
    {
        s32Ret = NI_MDK_SYS_MmzFree(pstCtx->stOcModelMmz.u32PhyAddr, pstCtx->stOcModelMmz.pvVirAddr);
        if (s32Ret != NI_SUCCESS)
        {
            PRINT_ERROR("od model mmz free faild\n", s32Ret);
            return s32Ret;
        }
        HdOcExits = FALSE;
    }
    return NI_SUCCESS;
}

static NI_S32 _ptz_tracking_cal_creat_thread(pthread_t *pstThread, pthread_attr_t *pstThreadAttr, NI_VOID *(*func)(NI_VOID *), NI_VOID *args, NI_S32 s32Policy, NI_S32 s32Prio)
{
    struct sched_param stSchedParam;
    NI_S32 s32Ret = NI_SUCCESS;

    stSchedParam.sched_priority = s32Prio;

    pthread_attr_init(pstThreadAttr);
    pthread_attr_setschedpolicy(pstThreadAttr, s32Policy);
    pthread_attr_setschedparam(pstThreadAttr, &stSchedParam);
    pthread_attr_setinheritsched(pstThreadAttr, PTHREAD_EXPLICIT_SCHED);

    if(s32Prio != -1)
    {
        s32Ret = pthread_create(pstThread, pstThreadAttr, func, args);
    }
    else
    {
        s32Ret = pthread_create(pstThread, 0, func, args);
    }

    return s32Ret;
}

void _perimeter_detect_save_result(IAS_PG_RESULT_S *pstResult, PG_RESULT *pstPgResult)
{
    OBJ_TYPE_E enType = OBJ_TYPE_BUTT;

    for (int i = 0; i < pstResult->u32ObjNum; i++)
    {
        if (IAS_OBJ_TYPE_HUMAN == pstResult->pstObj[i].enType || IAS_OBJ_TYPE_FACE == pstResult->pstObj[i].enType)
        {
            enType = OBJ_TYPE_HUMAN;
        }
        else if (IAS_OBJ_TYPE_CAR == pstResult->pstObj[i].enType)
        {
            enType = OBJ_TYPE_CAR;
        }
        else
        {
            continue;
        }

        if ((0 != pstPgResult->Result) &&(enType != pstPgResult->Type))
        {
            pstPgResult->Type = OBJ_TYPE_BOTH;
        }
        else
        {
            pstPgResult->Type = enType;
        }

        pstPgResult->Id = pstResult->u32RuleId;
        pstPgResult->Result++;
    }
}

static NI_S32 _ptz_tracking_cal_calc_offset(IAS_OD_RESULT_S_HUMAN *HunmanResult, NI_S32 s32Index, NI_F32 *pf32OffsetX, NI_F32 *pf32OffsetY, PTZ_TRACKING_OLDRESULT_S *pstOldResult)
{
    NI_F32 f32Offset;

    f32Offset = (HunmanResult->pstObj[s32Index].stRect.s32Xl + HunmanResult->pstObj[s32Index].stRect.s32Xr) / 2.0;
    f32Offset = (f32Offset / HunmanResult->stFrame.stFrame.u32Width - 0.5) * 4;

    if((0 == pstOldResult->s32OldObjNum)&&(HunmanResult->u32ObjNum != 0)&&(f32Offset > 0.5*HunmanResult->stFrame.stFrame.u32Width))
    {
        f32Offset = 0;
    }
    if(fabs(f32Offset) >= 2)
    {
        f32Offset = 0;
    }
    *pf32OffsetX = f32Offset;

    f32Offset = (HunmanResult->pstObj[s32Index].stRect.s32Yl + HunmanResult->pstObj[s32Index].stRect.s32Yr) / 2.0;
    f32Offset = (f32Offset / HunmanResult->stFrame.stFrame.u32Height - 0.5) * 4;

    if((0 == pstOldResult->s32OldObjNum)&&(HunmanResult->u32ObjNum != 0)&&(f32Offset > 0.5*HunmanResult->stFrame.stFrame.u32Height))
    {
        f32Offset = 0;
    }
    if(fabs(f32Offset) >= 2)
    {
        f32Offset = 0;
    }
    *pf32OffsetY = f32Offset;

    return NI_SUCCESS;
}

#if 0
/* 1111 的功能验证版本 */
static NI_S32 _ptz_tracking_cal_calc_offset2(IAS_OD_RESULT_S *pstResult, NI_S32 s32Index, NI_F32 *pf32OffsetX, NI_F32 *pf32OffsetY)
{
    NI_F32 f32Offset;

    f32Offset = (pstResult->pstObj[s32Index].stRect.s32Xl + pstResult->pstObj[s32Index].stRect.s32Xr) / 2.0;
    f32Offset = (f32Offset / pstResult->stFrame.stFrame.u32Width - 0.5) * 2;
    *pf32OffsetX = f32Offset;

    f32Offset = (pstResult->pstObj[s32Index].stRect.s32Yl + pstResult->pstObj[s32Index].stRect.s32Yr) / 2.0;
    f32Offset = (f32Offset / pstResult->stFrame.stFrame.u32Height - 0.5) * 2;
    *pf32OffsetY = f32Offset;

    return NI_SUCCESS;
}

static NI_S32 _ptz_tracking_cal_od_result_filter2(PTZ_TRACKING_CTX *pstCtx, IAS_OD_RESULT_S *pstResult, NI_S32 *ps32Index)
{
    NI_U32 i;
    NI_U32 u32RectSize, u32MaxRectSize;
    NI_S32 s32Index;
    IAS_RECT_S stRect;
    NI_BOOL bNotMove = NI_FALSE;
    NI_F32 f32OffsetX;
    NI_F32 f32OffsetY;
    NI_F32 f32Offset;
    NI_U32 u32LastMaxObjId;

    s32Index = -1;
    u32MaxRectSize = 0;

    u32LastMaxObjId = pstCtx->u32CurObjMaxId;

    /*先计算当前画面中占比面积最大的人形索引*/
    for(i = 0; i < pstResult->u32ObjNum; i++)
    {
        if(IAS_OBJ_TYPE_HUMAN == pstResult->pstObj[i].enType)
        {
            u32RectSize = (pstResult->pstObj[i].stRect.s32Xr - pstResult->pstObj[i].stRect.s32Xl);
            u32RectSize *= (pstResult->pstObj[i].stRect.s32Yr - pstResult->pstObj[i].stRect.s32Yl);

            pstCtx->u32CurObjMaxId = NI_Max(pstCtx->u32CurObjMaxId, pstResult->pstObj[i].stIdInfo.s32ObjId);

            if(u32RectSize > u32MaxRectSize)
            {
                s32Index = i;
                u32MaxRectSize = u32RectSize;
            }
        }
    }

    if(-1 == s32Index)
    {
        pstCtx->u32NoObjCount++;
    }
    else
    {
        pstCtx->u32NoObjCount = 0;
    }

    /* 不为初始目标 */
    if(NI_TRUE == pstCtx->bObjValid)
    {
        if(s32Index != -1)
        {
            /* 新增目标 */
            if(pstResult->pstObj[s32Index].stIdInfo.s32ObjId > u32LastMaxObjId)
            {
                /*查找上一个目标是否仍然存在*/
                for(i = 0; i < pstResult->u32ObjNum; i++)
                {
                    if(IAS_OBJ_TYPE_HUMAN == pstResult->pstObj[i].enType)
                    {
                        if(pstCtx->stObjTracked.stIdInfo.s32ObjId == pstResult->pstObj[i].stIdInfo.s32ObjId)
                        {
                            /*比较两个目标占比，决定跟踪哪一个*/
                            u32RectSize = (pstResult->pstObj[i].stRect.s32Xr - pstResult->pstObj[i].stRect.s32Xl);
                            u32RectSize *= (pstResult->pstObj[i].stRect.s32Yr - pstResult->pstObj[i].stRect.s32Yl);

                            if((u32MaxRectSize - u32RectSize) * 100 / u32RectSize < 20)
                            {
                                // NI_INFO("new obj last id:%d new id %d new size < old size * 10\n", u32LastMaxObjId, pstResult->pstObj[s32Index].stIdInfo.s32ObjId);
                                s32Index = i;
                            }
                            else
                            {
                                NI_INFO("new obj last id:%d new id %d\n", u32LastMaxObjId, pstResult->pstObj[s32Index].stIdInfo.s32ObjId);
                            }

                            break;
                        }
                    }
                }

                pstCtx->u32StaticCount = 0;
            }
            /* 不为新增目标 */
            else
            {
                for(i = 0; i < pstResult->u32ObjNum; i++)
                {
                    /*查找上一个目标是否仍然存在*/
                    if(IAS_OBJ_TYPE_HUMAN == pstResult->pstObj[i].enType)
                    {
                        if(pstCtx->stObjTracked.stIdInfo.s32ObjId == pstResult->pstObj[i].stIdInfo.s32ObjId)
                        {
                            // NI_INFO("find old id:%d\n", pstResult->pstObj[i].stIdInfo.s32ObjId);
                            s32Index = i;
                            break;
                        }
                    }
                }

                /* 跟踪的目标仍然存在 */
                if(i != pstResult->u32ObjNum)
                {
                    // NI_INFO("find old id:%d\n", pstResult->pstObj[s32Index].stIdInfo.s32ObjId);

                    f32Offset = (pstResult->pstObj[s32Index].stRect.s32Xl + pstResult->pstObj[s32Index].stRect.s32Xr) / 2.0;
                    f32Offset = (f32Offset / pstResult->stFrame.stFrame.u32Width - 0.5) * 2;
                    f32OffsetX = f32Offset;

                    f32Offset = (pstResult->pstObj[s32Index].stRect.s32Yl + pstResult->pstObj[s32Index].stRect.s32Yr) / 2.0;
                    f32Offset = (f32Offset / pstResult->stFrame.stFrame.u32Height - 0.5) * 2;
                    f32OffsetY = f32Offset;

                    if((NI_U32)(NI_Abs(f32OffsetX * 1000)) < pstCtx->u32ObjTolerance)
                    {
                        // NI_INFO("old id:%d offset < tolerance\n", pstResult->pstObj[s32Index].stIdInfo.s32ObjId);

                        /*判断相同目标是否移动*/
                        stRect.s32Xl = NI_Min(pstCtx->stObjTracked.stRect.s32Xl, pstResult->pstObj[s32Index].stRect.s32Xl);
                        stRect.s32Xr = NI_Max(pstCtx->stObjTracked.stRect.s32Xr, pstResult->pstObj[s32Index].stRect.s32Xr);
                        stRect.s32Yl = NI_Min(pstCtx->stObjTracked.stRect.s32Yl, pstResult->pstObj[s32Index].stRect.s32Yl);
                        stRect.s32Yr = NI_Max(pstCtx->stObjTracked.stRect.s32Yr, pstResult->pstObj[s32Index].stRect.s32Yr);
                        u32MaxRectSize = (stRect.s32Xr - stRect.s32Xl) * (stRect.s32Yr - stRect.s32Yl);

                        stRect.s32Xl = NI_Max(pstCtx->stObjTracked.stRect.s32Xl, pstResult->pstObj[s32Index].stRect.s32Xl);
                        stRect.s32Xr = NI_Min(pstCtx->stObjTracked.stRect.s32Xr, pstResult->pstObj[s32Index].stRect.s32Xr);
                        stRect.s32Yl = NI_Max(pstCtx->stObjTracked.stRect.s32Yl, pstResult->pstObj[s32Index].stRect.s32Yl);
                        stRect.s32Yr = NI_Min(pstCtx->stObjTracked.stRect.s32Yr, pstResult->pstObj[s32Index].stRect.s32Yr);
                        u32RectSize = (stRect.s32Xr - stRect.s32Xl) * (stRect.s32Yr - stRect.s32Yl);

                        if(u32RectSize * 100 / u32MaxRectSize > 60)
                        {
                            // NI_INFO("old id:%d not move\n", pstResult->pstObj[s32Index].stIdInfo.s32ObjId);
                            bNotMove = NI_TRUE;
                            pstCtx->stObjTracked = pstResult->pstObj[s32Index];
                            pstCtx->bObjValid = NI_TRUE;
                            s32Index = -1;
                        }
                        else
                        {
                            pstCtx->u32StaticCount = 0;
                        }
                    }
                    else
                    {
                        pstCtx->u32StaticCount = 0;
                    }
                }
                else
                {
                    pstCtx->u32StaticCount = 0;
                }
            }
        }
    }
    else
    {
        pstCtx->u32StaticCount = 0;
    }

    if(s32Index != -1)
    {
        pstCtx->stObjTracked = pstResult->pstObj[s32Index];
        pstCtx->bObjValid = NI_TRUE;
    }
    else
    {
        if(NI_FALSE == bNotMove)
        {
            pstCtx->bObjValid = NI_FALSE;
        }
    }

    *ps32Index = s32Index;

    return NI_SUCCESS;
}
#endif
static NI_S32 _ptz_tracking_cal_od_result_filter3(PTZ_TRACKING_CTX *pstCtx, IAS_OD_RESULT_S_HUMAN *HunmanResult, NI_S32 *ps32Index)
{
    NI_U32 i;
    NI_S32 s32RectSize, s32MaxRectSize;
    NI_S32 s32MaxSizeIndex;
    NI_S32 s32MaxId;
   
    s32MaxId = 0;
    s32RectSize = 0;
    s32MaxSizeIndex = 0;
    s32MaxRectSize = 0;
   
    /*遍历每一个目标，得到当前帧目标里最大ID号*/
    for(i= 0; i < HunmanResult->u32ObjNum; i++)
    {
        if(s32MaxId < HunmanResult->pstObj[i].stIdInfo.s32ObjId)
        {
            s32MaxId = HunmanResult->pstObj[i].stIdInfo.s32ObjId;
        }
    }

    /*追踪分为4种情况*/
    /*前一帧没有目标，当前帧有目标*/
    if((0 == pstCtx->stOldResult.s32OldRectSize)&&(HunmanResult->u32ObjNum != 0))
    {
        /*遍历当前帧所有目标，寻找面积占比最大的目标*/
        for(i= 0; i < HunmanResult->u32ObjNum; i++)
        {
            s32RectSize = (HunmanResult->pstObj[i].stRect.s32Xr - HunmanResult->pstObj[i].stRect.s32Xl);
            s32RectSize *= (HunmanResult->pstObj[i].stRect.s32Yr - HunmanResult->pstObj[i].stRect.s32Yl);

            if(s32RectSize >= s32MaxRectSize)
            {
                s32MaxSizeIndex = i;
                s32MaxRectSize = s32RectSize;
            }      
        }

        *ps32Index = s32MaxSizeIndex;
    }
    else if((0 == HunmanResult->u32ObjNum)&&(0 == pstCtx->stOldResult.s32OldRectSize))
    {
        /*前一帧没有目标，当前帧也没有目标*/
        *ps32Index = -1;
        s32MaxRectSize = 0; 
    }
    else if((0 != pstCtx->stOldResult.s32OldRectSize)&&(HunmanResult->u32ObjNum != 0))
    {
        /*前一帧有目标，当前帧也有目标*/
        NI_S32 u = 0;
        NI_S32 m = 0;
        //NI_S32 h = 0;

        s32MaxRectSize = 0;
    
        for(i = 0; i < HunmanResult->u32ObjNum; i++)
        {
            /*遍历当前帧目标，查找上一帧追踪目标*/
            if(pstCtx->stOldResult.s32OldObjId == HunmanResult->pstObj[i].stIdInfo.s32ObjId)
            {
                u++;
                NI_U32 w;
                NI_S32 p;

                /*上一帧筛选目标在当前帧存在*/
                for(p = 0; p < HunmanResult->u32ObjNum; p++)
                {
                    if(pstCtx->stOldResult.s32VrialID == HunmanResult->pstObj[p].stIdInfo.s32ObjId)
                    {
                        m++;
                        if(pstCtx->stOldResult.s32Count < 2)
                        {
                            *ps32Index = i;
                            s32MaxRectSize = pstCtx->stOldResult.s32OldRectSize;
                            pstCtx->stOldResult.s32Count++;
                        }
                        else
                        {
                            pstCtx->stOldResult.s32Count = 0;
                            *ps32Index = p;
                            s32MaxRectSize = HunmanResult->pstObj[p].stRect.s32Xr - HunmanResult->pstObj[p].stRect.s32Xl;
                            s32MaxRectSize *= HunmanResult->pstObj[p].stRect.s32Yr - HunmanResult->pstObj[p].stRect.s32Yl;
                        }
                        break;
                    }
                    pstCtx->stOldResult.s32Count = 0;
                }
                if(0 == m)
                {
                    if (pstCtx->stOldResult.s32OldMaxId < s32MaxId)
                    {
                        /*判断当前帧有新目标加入*/
                        for(w = pstCtx->stOldResult.s32OldMaxId; w < s32MaxId; w++)
                        {
                            /*遍历当前目标寻找新目标在当前帧所对应的目标索引*/
                            NI_U32 k;
                            for(k = 0; k < HunmanResult->u32ObjNum; k++)
                            {
                                if(w == HunmanResult->pstObj[k].stIdInfo.s32ObjId)
                                {
                                    /*寻找面积占比最大的目标*/
                                    s32RectSize = (HunmanResult->pstObj[k].stRect.s32Xr - HunmanResult->pstObj[k].stRect.s32Xl);
                                    s32RectSize *= (HunmanResult->pstObj[k].stRect.s32Yr - HunmanResult->pstObj[k].stRect.s32Yl);

                                    if(s32RectSize > s32MaxRectSize)
                                    {
                                        s32MaxSizeIndex = k;
                                        s32MaxRectSize = s32RectSize;
                                    }      
                                }    
                            }         
                        }

                        /*比较上一帧追踪目标的面积和新加入面积占比最大目标的面积*/
                        if(1.1*pstCtx->stOldResult.s32OldRectSize < s32MaxRectSize)
                        {
                            *ps32Index = s32MaxSizeIndex;
                            pstCtx->stOldResult.s32VrialID = HunmanResult->pstObj[*ps32Index].stIdInfo.s32ObjId;
                            *ps32Index = i;
                            s32MaxRectSize = pstCtx->stOldResult.s32OldRectSize; 
                        }
                        else
                        {
                            *ps32Index = i;
                            s32MaxRectSize = pstCtx->stOldResult.s32OldRectSize;      
                        }
                    } 
                    else
                    {
                        int z;
                        for(z = 0; z < HunmanResult->u32ObjNum; z++)
                        {
                            s32RectSize = (HunmanResult->pstObj[z].stRect.s32Xr - HunmanResult->pstObj[z].stRect.s32Xl);
                            s32RectSize *= (HunmanResult->pstObj[z].stRect.s32Yr - HunmanResult->pstObj[z].stRect.s32Yl);

                            if(s32RectSize >= s32MaxRectSize)
                            {
                                s32MaxSizeIndex = z;
                                s32MaxRectSize = s32RectSize;
                            }      
                        }
                        if(s32MaxSizeIndex == i)
                        {
                            *ps32Index = i;
                            s32MaxRectSize = pstCtx->stOldResult.s32OldRectSize;
                        }
                        else
                        {
                            *ps32Index = s32MaxSizeIndex;
                            pstCtx->stOldResult.s32VrialID = HunmanResult->pstObj[*ps32Index].stIdInfo.s32ObjId;
                            *ps32Index = i;
                            s32MaxRectSize = pstCtx->stOldResult.s32OldRectSize; 
                        }
                    }
                }
                break; 
            }        
        } 
        if(0 == u)
        {
            /*上一帧目标不存在*/
            for(i = 0; i < HunmanResult->u32ObjNum; i++)
            {
                if(pstCtx->stOldResult.s32VrialID == HunmanResult->pstObj[i].stIdInfo.s32ObjId)
                {
                    m++;
                    if(pstCtx->stOldResult.s32Count < 2)
                    {
                        *ps32Index = -1;
                        s32MaxRectSize = pstCtx->stOldResult.s32OldRectSize;
                        pstCtx->stOldResult.s32Count++;
                    }
                    else
                    {
                        pstCtx->stOldResult.s32Count = 0;
                        *ps32Index = i;
                        s32MaxRectSize = HunmanResult->pstObj[i].stRect.s32Xr - HunmanResult->pstObj[i].stRect.s32Xl;
                        s32MaxRectSize *= HunmanResult->pstObj[i].stRect.s32Yr - HunmanResult->pstObj[i].stRect.s32Yl;
                    }
                    break;
                }
                pstCtx->stOldResult.s32Count = 0;
            }
            if(0 == m)
            {
                NI_S32 s32t = 0;
                
                if(0 == s32t)
                {
                    //usleep(100);
                    s32MaxRectSize = 0;
                    for(i= 0; i < HunmanResult->u32ObjNum; i++)
                    {
                        /*寻找面积占比最大的目标*/
                        s32RectSize = (HunmanResult->pstObj[i].stRect.s32Xr - HunmanResult->pstObj[i].stRect.s32Xl);
                        s32RectSize *= (HunmanResult->pstObj[i].stRect.s32Yr - HunmanResult->pstObj[i].stRect.s32Yl);

                        if(s32RectSize > s32MaxRectSize)
                        {
                            s32MaxSizeIndex = i;
                            s32MaxRectSize = s32RectSize;
                        } 
                    }
                    *ps32Index = s32MaxSizeIndex;
                }
            }      
        }             
    }
    else
    {
        /*当上一帧有目标，当前帧没有目标*/
        *ps32Index = -1;
        s32MaxRectSize = 0;
    }
     
    pstCtx->stOldResult.s32OldRectSize = s32MaxRectSize;

    if(-1 != *ps32Index)
    {
        pstCtx->stOldResult.s32OldObjId = HunmanResult->pstObj[*ps32Index].stIdInfo.s32ObjId;
        pstCtx->stOldResult.s32Xl = HunmanResult->pstObj[*ps32Index].stRect.s32Xl;
        pstCtx->stOldResult.s32Xr = HunmanResult->pstObj[*ps32Index].stRect.s32Xr;
        pstCtx->stOldResult.s32Yl = HunmanResult->pstObj[*ps32Index].stRect.s32Yl;
        pstCtx->stOldResult.s32Yr = HunmanResult->pstObj[*ps32Index].stRect.s32Yr;
    }

    pstCtx->stOldResult.s32OldObjNum = HunmanResult->u32ObjNum;
    pstCtx->stOldResult.s32OldIndex = *ps32Index;
    pstCtx->stOldResult.s32OldMaxId = s32MaxId;

    return NI_SUCCESS;
}

#if 0
/* 1111 的功能验证版本 */
static NI_S32 _ptz_tracking_cal_od_result_filter(PTZ_TRACKING_CTX *pstCtx, IAS_OD_RESULT_S *pstResult, NI_S32 *ps32Index)
{
    NI_U32 i;
    NI_U32 u32RectSize, u32MaxRectSize;
    NI_S32 s32Index;
    IAS_RECT_S stRect;
    NI_BOOL bNotMove = NI_FALSE;
    NI_F32 f32OffsetX;
    NI_F32 f32OffsetY;
    NI_F32 f32Offset;

    s32Index = -1;
    u32MaxRectSize = 0;

    /*先计算当前画面中占比面积最大的人形索引*/
    for(i = 0; i < pstResult->u32ObjNum; i++)
    {
        if(IAS_OBJ_TYPE_HUMAN == pstResult->pstObj[i].enType)
        {
            u32RectSize = (pstResult->pstObj[i].stRect.s32Xr - pstResult->pstObj[i].stRect.s32Xl);
            u32RectSize *= (pstResult->pstObj[i].stRect.s32Yr - pstResult->pstObj[i].stRect.s32Yl);

            if(u32RectSize > u32MaxRectSize)
            {
                s32Index = i;
                u32MaxRectSize = u32RectSize;
            }
        }
    }

    if(s32Index != -1)
    {
        // NI_INFO("max obj index:%d id:%d\n", s32Index, pstResult->pstObj[s32Index].stIdInfo.s32ObjId);
    }

    LOCK(&gstLock);

    if(-1 == s32Index)
    {
        pstCtx->u32NoObjCount++;
    }
    else
    {
        pstCtx->u32NoObjCount = 0;
    }

    /* 判断是否为初始目标 */
    if(NI_TRUE == pstCtx->bObjValid)
    {
        if(s32Index != -1)
        {
            if(pstCtx->stObjTracked.stIdInfo.s32ObjId != pstResult->pstObj[s32Index].stIdInfo.s32ObjId)
            {
                // NI_INFO("last id:%d cur id:%d\n", pstCtx->stObjTracked.stIdInfo.s32ObjId, pstResult->pstObj[s32Index].stIdInfo.s32ObjId);

                /*不是同一个目标*/
                for(i = 0; i < pstResult->u32ObjNum; i++)
                {
                    /*查找上一个目标是否仍然存在*/
                    if(IAS_OBJ_TYPE_HUMAN == pstResult->pstObj[i].enType)
                    {
                        if(pstCtx->stObjTracked.stIdInfo.s32ObjId == pstResult->pstObj[i].stIdInfo.s32ObjId)
                        {
                            /*比较两个目标占比，决定跟踪哪一个*/
                            u32RectSize = (pstResult->pstObj[i].stRect.s32Xr - pstResult->pstObj[i].stRect.s32Xl);
                            u32RectSize *= (pstResult->pstObj[i].stRect.s32Yr - pstResult->pstObj[i].stRect.s32Yl);

                            if((u32MaxRectSize - u32RectSize) * 100 / u32RectSize < 100)
                            {
                                // NI_INFO("last size:%d cur size:%d\n", u32RectSize, u32MaxRectSize);

                                s32Index = i;
                            }
                            break;
                        }
                    }
                }
            }

            /*同一个目标*/
            if(pstCtx->stObjTracked.stIdInfo.s32ObjId == pstResult->pstObj[s32Index].stIdInfo.s32ObjId)
            {
                f32Offset = (pstResult->pstObj[s32Index].stRect.s32Xl + pstResult->pstObj[s32Index].stRect.s32Xr) / 2.0;
                f32Offset = (f32Offset / pstResult->stFrame.stFrame.u32Width - 0.5) * 2;
                f32OffsetX = f32Offset;

                f32Offset = (pstResult->pstObj[s32Index].stRect.s32Yl + pstResult->pstObj[s32Index].stRect.s32Yr) / 2.0;
                f32Offset = (f32Offset / pstResult->stFrame.stFrame.u32Height - 0.5) * 2;
                f32OffsetY = f32Offset;
                
                if((NI_U32)(NI_Abs(f32OffsetX * 1000)) < pstCtx->u32ObjTolerance)
                {
                    /*判断相同目标是否移动*/
                    stRect.s32Xl = NI_Min(pstCtx->stObjTracked.stRect.s32Xl, pstResult->pstObj[s32Index].stRect.s32Xl);
                    stRect.s32Xr = NI_Max(pstCtx->stObjTracked.stRect.s32Xr, pstResult->pstObj[s32Index].stRect.s32Xr);
                    stRect.s32Yl = NI_Min(pstCtx->stObjTracked.stRect.s32Yl, pstResult->pstObj[s32Index].stRect.s32Yl);
                    stRect.s32Yr = NI_Max(pstCtx->stObjTracked.stRect.s32Yr, pstResult->pstObj[s32Index].stRect.s32Yr);
                    u32MaxRectSize = (stRect.s32Xr - stRect.s32Xl) * (stRect.s32Yr - stRect.s32Yl);

                    stRect.s32Xl = NI_Max(pstCtx->stObjTracked.stRect.s32Xl, pstResult->pstObj[s32Index].stRect.s32Xl);
                    stRect.s32Xr = NI_Min(pstCtx->stObjTracked.stRect.s32Xr, pstResult->pstObj[s32Index].stRect.s32Xr);
                    stRect.s32Yl = NI_Max(pstCtx->stObjTracked.stRect.s32Yl, pstResult->pstObj[s32Index].stRect.s32Yl);
                    stRect.s32Yr = NI_Min(pstCtx->stObjTracked.stRect.s32Yr, pstResult->pstObj[s32Index].stRect.s32Yr);
                    u32RectSize = (stRect.s32Xr - stRect.s32Xl) * (stRect.s32Yr - stRect.s32Yl);

                    if(u32RectSize * 100 / u32MaxRectSize > 60)
                    {
                        // NI_INFO("the same obj but not move last size:%d cur size:%d count %d\n", u32MaxRectSize, u32RectSize, pstCtx->u32StaticCount);
                        bNotMove = NI_TRUE;
                        pstCtx->stObjTracked = pstResult->pstObj[s32Index];
                        pstCtx->bObjValid = NI_TRUE;
                        s32Index = -1;
                    }
                    else
                    {
                        pstCtx->u32StaticCount = 0;
                    }
                }
                else
                {
                    pstCtx->u32StaticCount = 0;
                }
            }
            else
            {
                pstCtx->u32StaticCount = 0;
            }
        }
    }
    else
    {
        pstCtx->u32StaticCount = 0;
    }

    if(s32Index != -1)
    {
        pstCtx->stObjTracked = pstResult->pstObj[s32Index];
        pstCtx->bObjValid = NI_TRUE;
    }
    else
    {
        if(NI_FALSE == bNotMove)
        {
            pstCtx->bObjValid = NI_FALSE;
        }
    }

    UNLOCK(&gstLock);

    *ps32Index = s32Index;

    return NI_SUCCESS;
}
#endif

static NI_S32 _ptz_tracking_cal_motor_ctrl(PTZ_TRACKING_CTX *pstCtx, NI_F32 f32Offset, NI_F32 f32OffsetY, NI_U32 u32Director)
{
    NI_S32 s32Ret;
    NI_F32 f32MotroStep;
    NI_U32 u32ObjTolerance;
    int u32ObjToleranceY = 400;
    int u32MoveKpxY = 200;
    int xmove = FALSE;
    int ymove = FALSE;
    u32ObjTolerance = pstCtx->u32ObjTolerance;

    if((NI_U32)(NI_Abs(f32Offset * 500)) > u32ObjTolerance)
    {
        xmove = TRUE;
    }
    if((NI_U32)(NI_Abs(f32OffsetY * 500)) > u32ObjToleranceY)
    {
        ymove = TRUE;
    }
    if(xmove == FALSE && ymove == FALSE)
    {
        return NI_SUCCESS;
    }

    
#if PTZ_TRACKING_PTZ_EN
    stOptPos.enPtzCmd = PTZ_OPT_CMD_LOCATION;
    s32Ret = pstCtx->pstPtzOptInterFace->UserPtzCtrlCmd(pstCtx->pvPtzOptHandle, &stOptPos);
    if (s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("ptz ctrl cmd get pos faild\n");
        return s32Ret;
    }

    // printf("0000: [%d, %d]\n", stOptPos.u32PosX, stOptPos.u32PosY, u32MoveKpx, u32ObjTolerance);

    if(1 == u32Director)
    {
        stOptMove.enPtzCmd = PTZ_OPT_CMD_MOVE_CACHE;

        f32MotroStep = f32Offset * u32MoveKpx;

        if(pstCtx->bMirror)
        {
           if(f32MotroStep < 0)
            {
                stOptMove.u32PosX = stOptPos.u32PosX - (NI_U32)(NI_Abs(f32MotroStep));
            }
            else
            {
                if(stOptPos.u32PosX < (NI_U32)(NI_Abs(f32MotroStep)))
                {
                    stOptMove.u32PosX = 0;
                }
                else
                {
                    stOptMove.u32PosX = stOptPos.u32PosX + (NI_U32)(NI_Abs(f32MotroStep));
                }
            } 
        }
        else
        {
            if(f32MotroStep < 0)
            {
                stOptMove.u32PosX = stOptPos.u32PosX + (NI_U32)(NI_Abs(f32MotroStep));
            }
            else
            {
                if(stOptPos.u32PosX < (NI_U32)(NI_Abs(f32MotroStep)))
                {
                    stOptMove.u32PosX = 0;
                }
                else
                {
                    stOptMove.u32PosX = stOptPos.u32PosX - (NI_U32)(NI_Abs(f32MotroStep));
                }
            }
        }
        
        stOptMove.u32PosY = stOptPos.u32PosY;
        CheckValue(&stOptMove.u32PosX);
        CheckValue(&stOptMove.u32PosY);
        stOptMove.u32Speed = 100;

        //printf("1111: step:%d, [%d, %d]\n", (NI_U32)(NI_Abs(f32MotroStep)), stOptMove.u32PosX, stOptMove.u32PosY);

        s32Ret = pstCtx->pstPtzOptInterFace->UserPtzCtrlCmd(pstCtx->pvPtzOptHandle, &stOptMove);
    }
    else
    {

    }

    stOptPos.enPtzCmd = PTZ_OPT_CMD_LOCATION;
    s32Ret = pstCtx->pstPtzOptInterFace->UserPtzCtrlCmd(pstCtx->pvPtzOptHandle, &stOptPos);
    if (s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("ptz ctrl cmd get pos faild\n");
        return s32Ret;
    }

     //printf("2222: [%d, %d]\n", stOptPos.u32PosX, stOptPos.u32PosY);

    if (s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("ptz ctrl cmd move faild\n");
        return s32Ret;
    }
#else
    PTZ_OPT_PtzPrm stTzPrm;
    NI_S32 s32CoordX, s32CoordY;
    unsigned int MoveKpx = pstCtx->u32MoveKpx;
    static ProductType_e DeviceType = -1;
    if(DeviceType == -1)
	{
		DeviceType = SystemGetProductType();
    }
    memset(&stTzPrm, 0, sizeof(stTzPrm));

    if(1 == u32Director)
    {
        stTzPrm.ptzCmd = VIDEOIN_PTZ_LOCATION;
        s32Ret = CtrltPtz(&stTzPrm);
        if (s32Ret != NI_SUCCESS)
        {
            PRINT_ERROR("ptz ctrl cmd get coord faild\n");
            return s32Ret;
        }
        
        s32CoordX = stTzPrm.ptzLocation.xCoord;
        s32CoordY = stTzPrm.ptzLocation.yCoord;
        memset(&stTzPrm, 0, sizeof(stTzPrm));
        if(xmove == TRUE)
        {
            if(DeviceType == IPC_CH7 || DeviceType == IPC_RH7)
            {
                if(fabs(f32Offset) > 0 && fabs(f32Offset) <= 1)
                {
                    MoveKpx = 220;
                }
                if(fabs(f32Offset) > 1 && fabs(f32Offset) <= 1.5)
                {
                    MoveKpx = 200;
                }
                if(fabs(f32Offset) > 1.5)
                {
                    MoveKpx = 180;
                }
            }
            else
            {
                if(fabs(f32Offset) > 0 && fabs(f32Offset) <= 1)
                {
                    MoveKpx = 220;
                }
                if(fabs(f32Offset) > 1 && fabs(f32Offset) <= 1.5)
                {
                    MoveKpx = 200;
                }
                if(fabs(f32Offset) > 1.5)
                {
                    MoveKpx = 170;
                }
            }
            f32MotroStep = f32Offset * MoveKpx;
            //printf("1111: [%d, %d] f32Offset:%f Step:%d\n", s32CoordX, s32CoordY, f32Offset, (NI_U32)(NI_Abs(f32MotroStep)));
            if(pstCtx->bMirror)
            {
                if(f32MotroStep > 0)
                {
                    stTzPrm.ptzMove.xCoord = s32CoordX + (NI_U32)(NI_Abs(f32MotroStep));
                }
                else
                {
                    stTzPrm.ptzMove.xCoord = s32CoordX - (NI_U32)(NI_Abs(f32MotroStep));
                }
            }
            else
            {
                if(f32MotroStep > 0)
                {
                    stTzPrm.ptzMove.xCoord = s32CoordX - (NI_U32)(NI_Abs(f32MotroStep));
                }
                else
                {
                    stTzPrm.ptzMove.xCoord = s32CoordX + (NI_U32)(NI_Abs(f32MotroStep));
                }
            }
        }
        else
        {
            stTzPrm.ptzMove.xCoord = s32CoordX;
        }
        //printf("1111: [%d  -> %d] f32OffsetX:%f Step:%d\n", s32CoordX, stTzPrm.ptzMove.xCoord, f32Offset, (NI_U32)(NI_Abs(f32MotroStep)));
        if(ymove == TRUE)   
        {
            MoveKpx = u32MoveKpxY;
            if(DeviceType == IPC_CH7 || DeviceType == IPC_RH7)
            {
                if(fabs(f32OffsetY) > 0 && fabs(f32OffsetY) <= 0.9)
                {
                    MoveKpx = 150;
                }
                if(fabs(f32OffsetY) > 0.9 && fabs(f32OffsetY) <= 1.4)
                {
                    MoveKpx = 180;
                }
                if(fabs(f32OffsetY) > 1.4)
                {
                    MoveKpx = 160;
                }
            }
            else
            {
                if(fabs(f32OffsetY) > 0 && fabs(f32OffsetY) <= 0.9)
                {
                    MoveKpx = 150;
                }
                if(fabs(f32OffsetY) > 0.9 && fabs(f32OffsetY) <= 1.4)
                {
                    MoveKpx = 180;
                }
                if(fabs(f32OffsetY) > 1.4)
                {
                    MoveKpx = 160;
                }
            }

            f32MotroStep = f32OffsetY * MoveKpx;
            //printf("2222: [%d, %d] f32OffsetY:%f Step:%d\n", s32CoordX, s32CoordY, f32OffsetY, (NI_U32)(NI_Abs(f32MotroStep)));
            if(pstCtx->bflip)
            {
                if(f32MotroStep > 0)
                {
                    stTzPrm.ptzMove.yCoord = s32CoordY - (NI_U32)(NI_Abs(f32MotroStep));
                }
                else
                {
                    stTzPrm.ptzMove.yCoord = s32CoordY + (NI_U32)(NI_Abs(f32MotroStep));
                }
            }
            else
            {
                if(f32MotroStep > 0)
                {
                    stTzPrm.ptzMove.yCoord = s32CoordY + (NI_U32)(NI_Abs(f32MotroStep));
                }
                else
                {
                    stTzPrm.ptzMove.yCoord = s32CoordY - (NI_U32)(NI_Abs(f32MotroStep));
                }
            }
        }
        else
        {
            stTzPrm.ptzMove.yCoord = s32CoordY;
        }
        //printf("2222: [%d  -> %d] step:%f %d\n", s32CoordY, stTzPrm.ptzMove.yCoord, s32CoordY, (NI_U32)(NI_Abs(f32MotroStep)));
        //printf("2222: step:%d, [%d, %d]\n", (NI_U32)(NI_Abs(f32MotroStep)), stTzPrm.ptzMove.xCoord, stTzPrm.ptzMove.yCoord);

        stTzPrm.ptzCmd = VIDEOIN_PTZ_MOVE;//移动视角
        CheckValue(&stTzPrm.ptzMove.xCoord);
        CheckValue(&stTzPrm.ptzMove.yCoord);

        //printf("3333: step:%d, [%d, %d]\n", (NI_U32)(NI_Abs(f32MotroStep)), stTzPrm.ptzMove.xCoord, stTzPrm.ptzMove.yCoord);
        //SetPTZSpeed(2000);
        s32Ret = CtrltPtz(&stTzPrm);
        if (s32Ret != NI_SUCCESS)
        {
            PRINT_ERROR("ptz ctrl cmd move faild\n");
            return s32Ret;
        }
    }
#endif

    return NI_SUCCESS;
}

#if 0
/*1111 的流程验证版本*/
static NI_S32 _ptz_tracking_cal_motor_go_back(PTZ_TRACKING_CTX *pstCtx)
{
    NI_S32 s32Ret;
    PTZ_OPT_POS_S stOptPos;
    PTZ_OPT_MOVE_S stOptMove;

    // NI_INFO("go back pos [%d, %d]\n", pstCtx->stMotorOrignPos.u32PosX, pstCtx->stMotorOrignPos.u32PosY);

#if PTZ_TRACKING_PTZ_EN
    stOptPos.enPtzCmd = PTZ_OPT_CMD_LOCATION;
    s32Ret = pstCtx->pstPtzOptInterFace->UserPtzCtrlCmd(pstCtx->pvPtzOptHandle, &stOptPos);
    if (s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("ptz ctrl cmd get pos faild\n");
        return s32Ret;
    }

    if((stOptPos.u32PosX != pstCtx->stMotorOrignPos.u32PosX) || (stOptPos.u32PosY != pstCtx->stMotorOrignPos.u32PosY))
    {
        stOptMove.enPtzCmd = PTZ_OPT_CMD_MOVE_CACHE;
        stOptMove.u32PosX = pstCtx->stMotorOrignPos.u32PosX;
        stOptMove.u32PosY = pstCtx->stMotorOrignPos.u32PosY;
        stOptMove.u32Speed = 100;

        // printf("0000: go back pos [%d, %d]\n", pstCtx->stMotorOrignPos.u32PosX, pstCtx->stMotorOrignPos.u32PosY);

        s32Ret = pstCtx->pstPtzOptInterFace->UserPtzCtrlCmd(pstCtx->pvPtzOptHandle, &stOptMove);
        if(s32Ret != NI_SUCCESS)
        {
            PRINT_ERROR("ptz ctrl cmd move faild\n");
        }
    }
#else
    PTZ_OPT_PtzPrm stTzPrm;

    memset(&stTzPrm, 0, sizeof(stTzPrm));

    stTzPrm.ptzCmd = VIDEOIN_PTZ_MOVE;
    stTzPrm.ptzMove.xCoord = pstCtx->stMotorOrignPos.u32PosX;
    stTzPrm.ptzMove.yCoord = pstCtx->stMotorOrignPos.u32PosY;
    stTzPrm.ptzMove.Speed = 100;
    CtrltPtz(&stTzPrm);

    printf("0000: go back pos [%d, %d]\n", pstCtx->stMotorOrignPos.u32PosX, pstCtx->stMotorOrignPos.u32PosY);
#endif

    return NI_SUCCESS;
}
#endif

static NI_S32 _ptz_tracking_cal_motor_set_pos(PTZ_TRACKING_CTX *pstCtx, NI_PTZ_TRACKING_PTZ_POS_S *pstPos, NI_BOOL bSave)
{
    NI_S32 s32Ret;
    //PTZ_OPT_POS_S stOptPos;
    //PTZ_OPT_MOVE_S stOptMove;

#if PTZ_TRACKING_PTZ_EN
    stOptPos.enPtzCmd = PTZ_OPT_CMD_LOCATION;
    s32Ret = pstCtx->pstPtzOptInterFace->UserPtzCtrlCmd(pstCtx->pvPtzOptHandle, &stOptPos);
    if (s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("ptz ctrl cmd get pos faild\n");
        return s32Ret;
    }

    if((stOptPos.u32PosX != pstPos->u32PosX) || (stOptPos.u32PosY != pstPos->u32PosY))
    {
        if(bSave)
        {
            stOptMove.enPtzCmd = PTZ_OPT_CMD_MOVE;
        }
        else
        {
            stOptMove.enPtzCmd = PTZ_OPT_CMD_MOVE_CACHE;
        }
        
        stOptMove.u32PosX = NI_clip(pstPos->u32PosX, 0, 8192);
        stOptMove.u32PosY = NI_clip(pstPos->u32PosY, 0, 8190);
        stOptMove.u32Speed = pstPos->u32Speed;

        //printf("0000: user set pos [%d, %d]\n", stOptMove.u32PosX, stOptMove.u32PosY);

        s32Ret = pstCtx->pstPtzOptInterFace->UserPtzCtrlCmd(pstCtx->pvPtzOptHandle, &stOptMove);
        if(s32Ret != NI_SUCCESS)
        {
            PRINT_ERROR("ptz ctrl cmd move faild\n");
        }
    }

    stOptPos.enPtzCmd = PTZ_OPT_CMD_LOCATION;
    s32Ret = pstCtx->pstPtzOptInterFace->UserPtzCtrlCmd(pstCtx->pvPtzOptHandle, &stOptPos);
    if (s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("ptz ctrl cmd get pos faild\n");
        return s32Ret;
    }

    // printf("1111: user set pos [%d, %d]\n", stOptPos.u32PosX, stOptPos.u32PosY);

    pstCtx->stMotorOrignPos.u32PosX = stOptPos.u32PosX;
    pstCtx->stMotorOrignPos.u32PosY = stOptPos.u32PosY;
#else
    PTZ_OPT_PtzPrm stTzPrm;
    memset(&stTzPrm, 0, sizeof(stTzPrm));
    
    if(pstPos->u32PosX == -1 && pstPos->u32PosY == -1)
    {
        stTzPrm.ptzCmd = VIDEOIN_PTZ_LOCATION;
        s32Ret = CtrltPtz(&stTzPrm);
        if(s32Ret != NI_SUCCESS)
        {
            return s32Ret;
        }
        pstPos->u32PosX = stTzPrm.ptzMove.xCoord;
        pstPos->u32PosY = stTzPrm.ptzMove.yCoord;
        memset(&stTzPrm, 0, sizeof(stTzPrm));
    }
    
    stTzPrm.ptzCmd = VIDEOIN_PTZ_MOVE;
    CheckValue((int *)&pstPos->u32PosX);
    CheckValue((int *)&pstPos->u32PosY);
    stTzPrm.ptzMove.xCoord = pstPos->u32PosX;
    stTzPrm.ptzMove.yCoord = pstPos->u32PosY;
    stTzPrm.ptzMove.Speed = pstPos->u32Speed;
    //SetPTZSpeed(2000);

    s32Ret = CtrltPtz(&stTzPrm);
    if (s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("ptz ctrl cmd move faild\n");
        return s32Ret;
    }
    pstCtx->stMotorOrignPos.u32PosX = stTzPrm.ptzMove.xCoord;
    pstCtx->stMotorOrignPos.u32PosY = stTzPrm.ptzMove.yCoord;
#endif

    return NI_SUCCESS;
}


static NI_VOID *_ptz_tracking_cal_get_frame_process(NI_VOID *p)
{
    PTZ_TRACKING_CTX *pstCtx = NI_NULL;
    NI_BOOL bSync;
    NI_S32 s32Ret;
    NI_U32 u32FrameId = 0;
    pthread_t pthread;
    pid_t pid;
    pid_t tid;

    pstCtx = (PTZ_TRACKING_CTX *)p;

    prctl(PR_SET_NAME, "getframe");

    pid = getpid();
    tid = syscall(SYS_gettid);
    pthread = pthread_self();

    PRINT_ERROR("get frame thread pid %d tid %d thread id 0x%x\n", pid, tid, pthread);
	//PRINT_ERROR("_ptz_tracking_cal_get_frame_process tid %d",syscall(SYS_gettid));
    while(pstCtx->bThreadCreate)
    {       
        s32Ret = _ptz_tracking_cal_wait_event_timeout(&gstSemFrameProc, 1000);
        if(s32Ret != NI_SUCCESS)
        {
            continue;
        }

        LOCK(&gstLock);

        bSync = pstCtx->bUsrCmdSync;
 
        if(bSync)
        {
            switch(pstCtx->unUsrCmd.stPtzCmd.enType)
            {
                case NI_PTZ_TRACKING_PTZ_CMD_SET_POS:
                {
                    s32Ret = _ptz_tracking_cal_motor_set_pos(pstCtx, &pstCtx->unUsrCmd.stPtzPosCmd, NI_TRUE);
                    if(s32Ret != NI_SUCCESS)
                    {
                        PRINT_ERROR("set motor pos faild\n");
                    }
                    break;
                }
                default:
                {
                    PRINT_ERROR("user set ptz cmd error\n");
                }
            }

            pstCtx->bUsrCmdSync = NI_FALSE;
            pstCtx->bDelayEn = NI_TRUE;
        }

        UNLOCK(&gstLock);
        if(bSync)
        {
            _ptz_tracking_cal_wake_up(&gstSemFrameProc);
            continue;
        }

        LOCK(&gstLock);
        if(NI_TRUE == pstCtx->bDelayEn)
        {
            // _ptz_tracking_cal_wait_event_timeout(&gstSemSleepProc, 30000);
            _ptz_tracking_cal_cond_wait_event_timeout(&gstCondSleepProc, &gstLock, 30000);
            pstCtx->bDelayEn = NI_FALSE;
        }

        bSync = pstCtx->bUsrParamSync;

        if(bSync)
        {
            pstCtx->bThreadRun = pstCtx->stUsrParam.bSetThreadRun;
            pstCtx->u32MoveKpx = pstCtx->stUsrParam.stPtzAdjAttr.u32MoveKpx;
            pstCtx->u32ObjTolerance = pstCtx->stUsrParam.stPtzAdjAttr.u32ObjTolerance;
            pstCtx->bPtzRun = pstCtx->stUsrParam.stPtzAdjAttr.bStart;
            pstCtx->bMirror = pstCtx->stUsrParam.stPtzAdjAttr.bMirror;
            pstCtx->bflip = pstCtx->stUsrParam.stPtzAdjAttr.bflip;
            pstCtx->bUsrParamSync = NI_FALSE;
        }

        UNLOCK(&gstLock);

        if(pstCtx->bThreadRun && ResetState == FALSE)
        {
            IAS_TASK_IMAGE_S stFrame;

            s32Ret = NI_MDK_VPS_GetFrame(0, 4, &gstFrameInfo, 1000);
            if (s32Ret != NI_SUCCESS)
            {
                PRINT_ERROR("get vps ias frame faild 0x%x\n", s32Ret);
                usleep(40000);
                _ptz_tracking_cal_wake_up(&gstSemFrameProc);
                continue;
            }

            u32FrameId++;
            stFrame.lPrivateData = u32FrameId;
            stFrame.stFrame = gstFrameInfo.astVideoFrame[0];
            
            s32Ret = NI_IAS_OD_Run(0, &stFrame, 1000);
            if (s32Ret != NI_SUCCESS)
            {
                s32Ret = NI_IAS_OD_GetImageToRelease(0, &stFrame, 1000);
                if (s32Ret != NI_SUCCESS)
                {
                    PRINT_ERROR("NI_IAS_OD_GetImageToRelease faild 0x%x\n", s32Ret);
                }

                s32Ret = NI_MDK_VPS_ReleaseFrame(0, 4, &gstFrameInfo);
                if (s32Ret != NI_SUCCESS)
                {
                    PRINT_ERROR("NI_MDK_VPS_ReleaseFrame faild 0x%x\n", s32Ret);
                }

                PRINT_ERROR("run ias od faild 0x%x\n", s32Ret);
                usleep(40000);
                _ptz_tracking_cal_wake_up(&gstSemFrameProc);
                continue;
            }

            LOCK(&gstLock);
            pstCtx->u32ProcessingFrame++;
            UNLOCK(&gstLock);

            _ptz_tracking_cal_wake_up(&gstSemResultProc);
        }
        else
        {
            usleep(40000);
            _ptz_tracking_cal_wake_up(&gstSemFrameProc);
        }
    }

    return 0;
}

#if 0
/* wzx 的第一个版本 */
static NI_S32 _ptz_tracking_cal_od_result_process2(PTZ_TRACKING_CTX *pstCtx, IAS_OD_RESULT_S *pstResult)
{
    NI_S32 s32Ret;
    NI_S32 s32Index;
    NI_F32 f32OffsetX;
    NI_S32 s32xCenter;
    NI_PTZ_TRACKING_PTZ_POS_S stPos;

    LOCK(&gstLock);

    if(0 == pstResult->u32ObjNum)
    {
        pstCtx->stOldResult.i++;
        if(pstCtx->stOldResult.i > 500)
        {
            stPos.enType = NI_PTZ_TRACKING_PTZ_CMD_SET_POS;
            stPos.u32PosX = 4096;
            stPos.u32PosY = 6090;
            _ptz_tracking_cal_motor_set_pos(pstCtx, &stPos, NI_TRUE);
        }
    } 
    else
    {
        pstCtx->stOldResult.i = 0;
        // _ptz_tracking_cal_od_result_filter(pstCtx, &stResult, &s32Index);
        _ptz_tracking_cal_od_result_filter3(pstCtx, pstResult, &s32Index);

        if(s32Index != -1)
        {
            if(NI_FALSE == pstCtx->bMotorGoBack)
            {
                _ptz_tracking_cal_calc_offset(pstResult, s32Index, &f32OffsetX, &(pstCtx->stOldResult));

                s32Ret = _ptz_tracking_cal_motor_ctrl(pstCtx, f32OffsetX, 1);
                if(s32Ret != NI_SUCCESS)
                {
                    PRINT_ERROR("ptz tracking motrol ctrl faild\n");
                }

                s32xCenter = ((pstResult->pstObj[s32Index].stRect.s32Xl)+(pstResult->pstObj[s32Index].stRect.s32Xr)) / 2;

                if(pstCtx->stOldResult.s32OldxCenter == s32xCenter)
                {
                    pstCtx->stOldResult.W++;
                    if(pstCtx->stOldResult.W > 500)
                    {
                        stPos.enType = NI_PTZ_TRACKING_PTZ_CMD_SET_POS;
                        stPos.u32PosX = 4096;
                        stPos.u32PosY = 4096;
                        _ptz_tracking_cal_motor_set_pos(pstCtx, &stPos, NI_TRUE);
                    }
                }
                else
                {
                    pstCtx->stOldResult.W = 0;
                }
            }
            else
            {
                pstCtx->bMotorGoBack = NI_FALSE;
            }
        }
    }

    UNLOCK(&gstLock);

    return NI_SUCCESS;
}
#endif

static NI_S32 _ptz_tracking_cal_od_result_process2(PTZ_TRACKING_CTX *pstCtx, IAS_OD_RESULT_S_HUMAN *HunmanResult)
{
    NI_S32 s32Ret;
    NI_S32 s32Index;
    NI_F32 f32OffsetX;
    NI_F32 f32OffsetY;
    //NI_S32 s32xCenter;
    NI_PTZ_TRACKING_PTZ_POS_S stPos;
    static char GoBackState = FALSE;
    static struct timeval timestart, timeend;
    LOCK(&gstLock);

    if(0 == HunmanResult->u32ObjNum)
    {
        gettimeofday(&timeend, NULL);
        if((timeend.tv_sec * 1000 * 1000 + timeend.tv_usec) - (timestart.tv_sec * 1000 * 1000 + timestart.tv_usec) > (5 * 1000 * 1000) && (timestart.tv_sec != 0)) //大于5秒后归位
        {
            if(GoBackState == FALSE)
            {
                ResetState = TRUE;
                if(ReviseGoBackPoint == TRUE)
                {
                    pstCtx->stMotorOrignPos.u32PosX = xGoBackPoint;
                    pstCtx->stMotorOrignPos.u32PosY = yGoBackPoint;
                    ReviseGoBackPoint = FALSE;
                }
                stPos.enType = NI_PTZ_TRACKING_PTZ_CMD_SET_POS;
                stPos.u32PosX = pstCtx->stMotorOrignPos.u32PosX;
                stPos.u32PosY = pstCtx->stMotorOrignPos.u32PosY;
                _ptz_tracking_cal_motor_set_pos(pstCtx, &stPos, NI_TRUE);
                usleep(500000);
                ResetState = FALSE;
                GoBackState = TRUE;
            }
        }       
    } 
    else
    {
        GoBackState = FALSE;
        gettimeofday(&timestart, NULL);
        // _ptz_tracking_cal_od_result_filter(pstCtx, &stResult, &s32Index);
        _ptz_tracking_cal_od_result_filter3(pstCtx, HunmanResult, &s32Index);

        if(NI_FALSE == pstCtx->bMotorGoBack)
        {
            _ptz_tracking_cal_calc_offset(HunmanResult, s32Index, &f32OffsetX, &f32OffsetY, &(pstCtx->stOldResult));

            s32Ret = _ptz_tracking_cal_motor_ctrl(pstCtx, f32OffsetX, f32OffsetY, 1);
            if(s32Ret != NI_SUCCESS)
            {
                PRINT_ERROR("ptz tracking motrol ctrl faild\n");
            }
        }
        else
        {
            pstCtx->bMotorGoBack = NI_FALSE;
        }
        
    }

    UNLOCK(&gstLock);


    return NI_SUCCESS;
}

static NI_VOID *_ptz_tracking_cal_get_result_process2(NI_VOID *p)
{
    PTZ_TRACKING_CTX *pstCtx = NI_NULL;
    IAS_OD_RESULT_S stResult;
    IAS_TASK_IMAGE_S stFrame;
    NI_VOID *pvOdProcessCbCtx = NI_NULL;
    NI_S32 s32Ret;

    IAS_OD_RESULT_S_HUMAN HunmanResult;
    
    pthread_t pthread;
    pid_t pid;
    pid_t tid;
    //NI_S32 i;
    //NI_S32  s32Xcenter;
    //NI_S32  s32Ycenter;
    pstCtx = (PTZ_TRACKING_CTX *)p;

    prctl(PR_SET_NAME, "getresult");

    pid = getpid();
    tid = syscall(SYS_gettid);
    pthread = pthread_self();

    PRINT_ERROR("get od result thread pid %d tid %d thread id 0x%x\n", pid, tid, pthread);

    while(1)
    {
        sem_wait(&gstSemResultProc);

        LOCK(&gstLock);
        if(0 == pstCtx->u32ProcessingFrame)
        {
           UNLOCK(&gstLock);
           break;
        }
        UNLOCK(&gstLock);

        s32Ret = NI_IAS_OD_GetResult(0, &stResult, 1000);
        if (s32Ret != NI_SUCCESS)
        {
            PRINT_ERROR("get od result faild 0x%x\n", s32Ret);

            s32Ret = NI_IAS_OD_GetImageToRelease(0, &stFrame, 1000);
            if (s32Ret != NI_SUCCESS)
            {
                PRINT_ERROR("get release frame faild 0x%x\n", s32Ret);
            }

            s32Ret = NI_MDK_VPS_ReleaseFrame(0, 4, &gstFrameInfo);
            if (s32Ret != NI_SUCCESS)
            {
                PRINT_ERROR("release vps ias frame faild 0x%x\n", s32Ret);
            }

            LOCK(&gstLock);
            pstCtx->u32ProcessingFrame--;
            UNLOCK(&gstLock);

            usleep(100 * 1000);
            _ptz_tracking_cal_wake_up(&gstSemFrameProc);
            continue;
        }

        if(pstCtx->bPtzRun)
        {
            int i, j = 0;
            unsigned int humannum = stResult.u32ObjNum;
            memset(&HunmanResult,0,sizeof(IAS_OD_RESULT_S_HUMAN));

            for(i = 0; i < stResult.u32ObjNum; i++)
            {
                if(stResult.pstObj[i].enType != IAS_OBJ_TYPE_HUMAN && stResult.pstObj[i].enType != IAS_OBJ_TYPE_FACE)
                {
                    humannum--;
                    continue;
                }
                memcpy(&HunmanResult.pstObj[j],&stResult.pstObj[i],sizeof(IAS_OD_OBJ_RESULT_S));
                j++;  
            }
            HunmanResult.Handle = stResult.Handle;
            HunmanResult.stFrame = stResult.stFrame;
            HunmanResult.u32ObjNum = humannum;
            s32Ret = _ptz_tracking_cal_od_result_process2(pstCtx, &HunmanResult);
            // s32Ret = _ptz_tracking_cal_od_result_process_od(pstCtx, &stResult);
            if(s32Ret != NI_SUCCESS)
            {
                PRINT_ERROR("od result process faild 0x%x\n", s32Ret);
            }
        }
        if(pstCtx->bPtzRun == 0)
        {
            PTZ_OPT_PtzPrm stTzPrm;

            memset(&stTzPrm, 0, sizeof(stTzPrm));

            stTzPrm.ptzCmd = VIDEOIN_PTZ_LOCATION;
            s32Ret = CtrltPtz(&stTzPrm);
            if(s32Ret == NI_SUCCESS)
            {
                pstCtx->stMotorOrignPos.u32PosX = stTzPrm.ptzLocation.xCoord;
                pstCtx->stMotorOrignPos.u32PosY = stTzPrm.ptzLocation.yCoord;
            }
        }

        LOCK(&gstLock);
        if(pstCtx->stFuncCallBack.stOdResultProcessCb.RegOdResultProcess != NI_NULL)
        {
            pvOdProcessCbCtx = pstCtx->stFuncCallBack.stOdResultProcessCb.pvCbCtx;
            NI_S32 (*reg_od_result_process)(NI_VOID *, IAS_OD_RESULT_S *);
            reg_od_result_process = pstCtx->stFuncCallBack.stOdResultProcessCb.RegOdResultProcess;
            UNLOCK(&gstLock);

            s32Ret = reg_od_result_process(pvOdProcessCbCtx, &stResult);
            if (s32Ret != NI_SUCCESS)
            {
                PRINT_ERROR("call od result proc func callback faild 0x%x\n", s32Ret);
            }
            LOCK(&gstLock);
        }
        UNLOCK(&gstLock);

        s32Ret = NI_IAS_OD_ReleaseResult(0, &stResult);
        if (s32Ret != NI_SUCCESS)
        {
            PRINT_ERROR("release od result faild 0x%x\n", s32Ret);
            usleep(10000);
        }

        s32Ret = NI_IAS_OD_GetImageToRelease(0, &stFrame, 1000);
        if (s32Ret != NI_SUCCESS)
        {
            PRINT_ERROR("get release frame faild 0x%x\n", s32Ret);
        }

        s32Ret = NI_MDK_VPS_ReleaseFrame(0, 4, &gstFrameInfo);
        if (s32Ret != NI_SUCCESS)
        {
            PRINT_ERROR("release vps ias frame faild 0x%x\n", s32Ret);
        }

        LOCK(&gstLock);
        pstCtx->u32ProcessingFrame--;
        UNLOCK(&gstLock);

        _ptz_tracking_cal_wake_up(&gstSemFrameProc);
    }

    return 0;
}

#if 0
/*1111 的流程验证版本*/
static NI_S32 _ptz_tracking_cal_od_result_process(PTZ_TRACKING_CTX *pstCtx, IAS_OD_RESULT_S *pstResult)
{
    NI_S32 s32Ret;
    NI_S32 s32Index;
    NI_F32 f32OffsetX;
    NI_F32 f32OffsetY;

    // printf("*********************************\n");

    LOCK(&gstLock);

    // _ptz_tracking_cal_od_result_filter(pstCtx, &stResult, &s32Index);
    _ptz_tracking_cal_od_result_filter2(pstCtx, pstResult, &s32Index);

    if(s32Index != -1)
    {
        if(NI_FALSE == pstCtx->bMotorGoBack)
        {
            _ptz_tracking_cal_calc_offset2(pstResult, s32Index, &f32OffsetX, &f32OffsetY);

            s32Ret = _ptz_tracking_cal_motor_ctrl(pstCtx, f32OffsetX, 1);
            if(s32Ret != NI_SUCCESS)
            {
                PRINT_ERROR("ptz tracking motrol ctrl faild\n");
            }
        }
        else
        {
            pstCtx->bMotorGoBack = NI_FALSE;
        }
    }
    else
    {
        pstCtx->u32StaticCount++;
        // NI_INFO("no obj cnt:%d static cnt:%d\n", pstCtx->u32NoObjCount, pstCtx->u32StaticCount);
        if((pstCtx->u32NoObjCount >= 125) || (pstCtx->u32StaticCount >= 125))
        {
            pstCtx->bMotorGoBack = NI_TRUE;
            s32Ret = _ptz_tracking_cal_motor_go_back(pstCtx);
            if(s32Ret != NI_SUCCESS)
            {
                PRINT_ERROR("ptz tracking motrol go back faild\n");
            }

            _ptz_tracking_cal_cond_wait_event_timeout(&gstCondSleepProc, &gstLock, 5000);
            // UNLOCK(&gstLock);
            // usleep(5000000);
            // LOCK(&gstLock);

            pstCtx->u32NoObjCount = 0;
            pstCtx->u32StaticCount = 0;
        }
    }

    UNLOCK(&gstLock);

    return NI_SUCCESS;
}
#endif

#if 0
NI_S32 _ptz_tracking_cal_od_result_process_od(NI_VOID *pvHandle, IAS_OD_RESULT_S *pstResult)
{
    NI_S32 s32Ret;
    NI_S32 s32Index;
    NI_F32 f32OffsetX;
    NI_F32 f32OffsetY;
    PTZ_TRACKING_CTX *pstCtx = NI_NULL;

    // printf("*********************************\n");
    pstCtx = (PTZ_TRACKING_CTX *)pvHandle;

    LOCK(&gstLock);

    // _ptz_tracking_cal_od_result_filter(pstCtx, &stResult, &s32Index);
    _ptz_tracking_cal_od_result_filter2(pstCtx, pstResult, &s32Index);

    if(s32Index != -1)
    {
        if(NI_FALSE == pstCtx->bMotorGoBack)
        {
            _ptz_tracking_cal_calc_offset2(pstResult, s32Index, &f32OffsetX, &f32OffsetY);

            if(s32Index != -1)
            {
                printf("od result id:%d, type:%d rect:[%d,%d,%d,%d] score:%d\n", \
                    pstResult->pstObj[s32Index].stIdInfo.s32ObjId, \
                    pstResult->pstObj[s32Index].enType, \
                    pstResult->pstObj[s32Index].stRect.s32Xl, \
                    pstResult->pstObj[s32Index].stRect.s32Yl, \
                    pstResult->pstObj[s32Index].stRect.s32Xr, \
                    pstResult->pstObj[s32Index].stRect.s32Yr, \
                    pstResult->pstObj[s32Index].u16Score);
            }

            s32Ret = _ptz_tracking_cal_motor_ctrl(pstCtx, f32OffsetX, 1);
            if(s32Ret != NI_SUCCESS)
            {
                PRINT_ERROR("ptz tracking motrol ctrl faild\n");
            }
        }
        else
        {
            pstCtx->bMotorGoBack = NI_FALSE;
        }
    }
    else
    {
        pstCtx->u32StaticCount++;
        // NI_INFO("no obj cnt:%d static cnt:%d\n", pstCtx->u32NoObjCount, pstCtx->u32StaticCount);
        if((pstCtx->u32NoObjCount >= 125) || (pstCtx->u32StaticCount >= 125))
        {
            pstCtx->bMotorGoBack = NI_TRUE;
            s32Ret = _ptz_tracking_cal_motor_go_back(pstCtx);
            if(s32Ret != NI_SUCCESS)
            {
                PRINT_ERROR("ptz tracking motrol go back faild\n");
            }

            _ptz_tracking_cal_cond_wait_event_timeout(&gstCondSleepProc, &gstLock, 5000);
            // UNLOCK(&gstLock);
            // usleep(5000000);
            // LOCK(&gstLock);

            pstCtx->u32NoObjCount = 0;
            pstCtx->u32StaticCount = 0;
        }
    }

    UNLOCK(&gstLock);

    return NI_SUCCESS;
}
#endif

#if 0
static NI_VOID *_ptz_tracking_cal_get_result_process(NI_VOID *p)
{
    NI_S32 s32Ret;
    PTZ_TRACKING_CTX *pstCtx = NI_NULL;
    IAS_OD_RESULT_S stResult;
    IAS_TASK_IMAGE_S stFrame;
    VPS_FRAME_INFO_S stFrameInfo;
    pthread_t pthread;
    pid_t pid;
    pid_t tid;

    pstCtx = (PTZ_TRACKING_CTX *)p;

    prctl(PR_SET_NAME, "getresult");

    //printf("ywf ywf ywf ywf ywf ywf\n");

    pid = getpid();
    tid = syscall(SYS_gettid);
    pthread = pthread_self();

    PRINT_ERROR("get od result thread pid %d tid %d thread id 0x%x\n", pid, tid, pthread);

    while(1)
    {
        sem_wait(&gstSemResultProc);

        LOCK(&gstLock);
        if(0 == pstCtx->u32ProcessingFrame)
        {
            UNLOCK(&gstLock);
            break;
        }
        UNLOCK(&gstLock);

        s32Ret = NI_IAS_OD_GetResult(0, &stResult, 5000);
        if (s32Ret != NI_SUCCESS)
        {
            PRINT_ERROR("get od result faild 0x%x\n", s32Ret);
            usleep(10000);
            _ptz_tracking_cal_wake_up(&gstSemResultProc);
            continue;
        }

        s32Ret = _ptz_tracking_cal_od_result_process(pstCtx, &stResult);
        if(s32Ret != NI_SUCCESS)
        {
            PRINT_ERROR("od result process faild 0x%x\n", s32Ret);
        }

        LOCK(&gstLock);
        if(pstCtx->stFuncCallBack.stOdResultProcessCb.RegOdResultProcess != NI_NULL)
        {
            NI_VOID *pvOdProcessCbCtx = NI_NULL;
            NI_S32 (*reg_od_result_process)(NI_VOID *, IAS_OD_RESULT_S *);

            pvOdProcessCbCtx = pstCtx->stFuncCallBack.stOdResultProcessCb.pvCbCtx;
            reg_od_result_process = pstCtx->stFuncCallBack.stOdResultProcessCb.RegOdResultProcess;
            UNLOCK(&gstLock);

            // s32Ret = pstCtx->stFuncCallBack.stOdResultProcessCb.RegOdResultProcess(pvOdProcessCbCtx, &stResult);
            s32Ret = reg_od_result_process(pvOdProcessCbCtx, &stResult);
            if (s32Ret != NI_SUCCESS)
            {
                PRINT_ERROR("call od result proc func callback faild 0x%x\n", s32Ret);
            }
            LOCK(&gstLock);
        }
        UNLOCK(&gstLock);

        s32Ret = NI_IAS_OD_ReleaseResult(0, &stResult);
        if (s32Ret != NI_SUCCESS)
        {
            PRINT_ERROR("release od result faild 0x%x\n", s32Ret);
        }

        s32Ret = NI_IAS_OD_GetImageToRelease(0, &stFrame, 1000);
        if (s32Ret != NI_SUCCESS)
        {
            PRINT_ERROR("get release frame faild 0x%x\n", s32Ret);
        }

        stFrameInfo.astVideoFrame[0] = stFrame.stFrame;
        s32Ret = NI_MDK_VPS_ReleaseFrame(0, 4, &stFrameInfo);
        if (s32Ret != NI_SUCCESS)
        {
            PRINT_ERROR("release vps ias frame faild 0x%x\n", s32Ret);
        }

        LOCK(&gstLock);
        pstCtx->u32ProcessingFrame--;
        UNLOCK(&gstLock);

        _ptz_tracking_cal_wake_up(&gstSemFrameProc);
    }

    return 0;
}
#endif

static NI_S32 _ptz_tracking_cal_init_ias_od(PTZ_TRACKING_CTX *pstCtx)
{
    NI_S32 s32Ret;
    IAS_OD_INIT_PARAM_S stCreateParam;

    memset(&stCreateParam, 0, sizeof(stCreateParam));

    s32Ret = _ptz_tracking_cal_get_model_file(pstCtx);//获取模型文件
    if (s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("get ias model faild\n");
        return s32Ret;
    }

    stCreateParam.stAlgModel.enType = IAS_OD_MODEL_READ_FROM_MEM;
    stCreateParam.stAlgModel.stOdModelCfg.stModelCfgOd.pData = pstCtx->stOdModelMmz.pvVirAddr;
    stCreateParam.stAlgModel.stOdModelCfg.stModelCfgOd.lPhyAddr = pstCtx->stOdModelMmz.u32PhyAddr;
    stCreateParam.stAlgModel.stOdModelCfg.stModelCfgOd.lLen = pstCtx->stOdModelMmz.u32BufLen;

    if(HdOcExits)
    {
        stCreateParam.stAlgModel.stOdModelCfg.stModelCfgOc.pData = pstCtx->stOcModelMmz.pvVirAddr;
        stCreateParam.stAlgModel.stOdModelCfg.stModelCfgOc.lPhyAddr = pstCtx->stOcModelMmz.u32PhyAddr;
        stCreateParam.stAlgModel.stOdModelCfg.stModelCfgOc.lLen = pstCtx->stOcModelMmz.u32BufLen;

        stCreateParam.stAlgCfg.stOcParam.bEnableOc = NI_TRUE;
        stCreateParam.stAlgCfg.stOcParam.u16ScoreThr = 800;
    }

    stCreateParam.stImageSrcSize.u16ImageW = pstCtx->stCfgPhrase.u16IasWidth;
    stCreateParam.stImageSrcSize.u16ImageH = pstCtx->stCfgPhrase.u16IasHeight;

    stCreateParam.stObjSize.u16MinSize = 1;
    stCreateParam.stObjSize.u16MaxSize = pstCtx->stCfgPhrase.u16IasWidth;

    stCreateParam.stPipeLine.enPipeMode = IAS_PIPELINE_MODE_SP;
    stCreateParam.stPipeLine.u32BufNum = 1;

    stCreateParam.stAlgCfg.stOdParam.u16ScoreThr = 280;
    stCreateParam.stAlgCfg.stOdParam.u16MaxObjNum = 32;

    stCreateParam.stAlgCfg.stOtParam.bPositiveTrack = NI_TRUE;
    stCreateParam.stAlgCfg.stOtParam.u16MaxTrackNum = 32;

    stCreateParam.stProcCfg.enProcMode = IAS_PROC_MODE_MULTI_TASK;
    /*由于车型静止过滤会造成设备在智能夜视时震荡，所以关闭此功能*/
    // stCreateParam.stAlgCfg.stOfParam.bEnableOf = NI_TRUE;
    // stCreateParam.stAlgCfg.stOfParam.enProcType = IAS_OD_OF_PROC_LEVEL_3;
    // stCreateParam.stAlgCfg.stOfParam.enFilterType = IAS_OD_OF_FILTER_NO_HUMAN;

    s32Ret = NI_IAS_OD_Init(0, &stCreateParam);
    if (s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("ias od init faild\n");
        goto release_model_file;
    }

    s32Ret = _ptz_tracking_cal_creat_thread(&pstCtx->stGetFrameThread, \
       &pstCtx->stThreadGetFrameAttr, _ptz_tracking_cal_get_frame_process, (NI_VOID*)pstCtx->stGblMmz.pvVirAddr, SCHED_RR, 99);//获取人形检测结果
    if (s32Ret != NI_SUCCESS)
    {
       PRINT_ERROR("creat get frame thread faild\n");
       goto od_deinit;
    }

    s32Ret = _ptz_tracking_cal_creat_thread(&pstCtx->stOdGetResultThread, \
        &pstCtx->stThreadOdGetResultAttr, _ptz_tracking_cal_get_result_process2, (NI_VOID*)pstCtx->stGblMmz.pvVirAddr, SCHED_RR, 99);//实现人形跟踪
    if (s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("creat get od result thread faild\n");
        goto od_deinit;
    }

    return NI_SUCCESS;

od_deinit:
    NI_IAS_OD_DeInit(0);
release_model_file:
    _ptz_tracking_cal_release_model_file(pstCtx);

    return s32Ret;
}

static NI_S32 _ptz_tracking_cal_deinit_ias_od(PTZ_TRACKING_CTX *pstCtx)
{
    NI_S32 s32Ret, s32Ret1 = NI_SUCCESS;

    // _ptz_tracking_cal_wake_up(&gstSemSleepProc);
    _ptz_tracking_cal_cond_signal(&gstCondSleepProc);
    pthread_join(pstCtx->stGetFrameThread, 0);

    _ptz_tracking_cal_wake_up(&gstSemResultProc);
    pthread_join(pstCtx->stOdGetResultThread, 0);

    usleep(1000);
    
    s32Ret = NI_IAS_OD_DeInit(0);
    if (s32Ret != NI_SUCCESS)
    {
        s32Ret1 = s32Ret;
        PRINT_ERROR("ias od deinit faild\n");
    }

    s32Ret = _ptz_tracking_cal_release_model_file(pstCtx);
    if (s32Ret != NI_SUCCESS)
    {
        // s32Ret1 = s32Ret;
        PRINT_ERROR("release ias od model faild\n");
    }

    return s32Ret1;
}

static NI_S32 _ptz_tracking_cal_start_attr(NI_VOID *pvHandle, NI_BOOL bStart)
{
    PTZ_TRACKING_CTX *pstCtx = NI_NULL;
    pstCtx = (PTZ_TRACKING_CTX *)pvHandle;
    LOCK(&gstLock);
    pstCtx->stUsrParam.stPtzAdjAttr.bStart = bStart;
    pstCtx->bUsrParamSync = NI_TRUE;
    UNLOCK(&gstLock);

    return NI_SUCCESS;
}


static NI_S32 _ptz_tracking_cal_run(NI_VOID *pvHandle)
{
    PTZ_TRACKING_CTX *pstCtx = NI_NULL;

    LOCK(&gstApiLock);

    if(NI_NULL == gpstDevCtx)
    {
        UNLOCK(&gstApiLock);
        PRINT_ERROR("ptz tracking has destory\n");
        return NI_FAILURE;
    }

    pstCtx = (PTZ_TRACKING_CTX *)pvHandle;

    if(pstCtx->stGblMmz.pvVirAddr != pvHandle)
    {
        UNLOCK(&gstApiLock);
        PRINT_ERROR("haldle error\n");
        return NI_FAILURE;
    }

    LOCK(&gstLock);
    pstCtx->stUsrParam.bSetThreadRun = NI_TRUE;
    pstCtx->bUsrParamSync = NI_TRUE;
    UNLOCK(&gstLock);

    UNLOCK(&gstApiLock);

    NI_INFO("start run\n");

    return NI_SUCCESS;
}

static NI_S32 _ptz_tracking_cal_stop(NI_VOID *pvHandle)
{
   PTZ_TRACKING_CTX *pstCtx = NI_NULL;

    LOCK(&gstApiLock);

    if(NI_NULL == gpstDevCtx)
    {
        UNLOCK(&gstApiLock);
        PRINT_ERROR("ptz tracking has destory\n");
        return NI_FAILURE;
    }

    pstCtx = (PTZ_TRACKING_CTX *)pvHandle;

    if(pstCtx->stGblMmz.pvVirAddr != pvHandle)
    {
        UNLOCK(&gstApiLock);
        PRINT_ERROR("haldle error\n");
        return NI_FAILURE;
    }

    LOCK(&gstLock);
    pstCtx->stUsrParam.bSetThreadRun = NI_FALSE;
    pstCtx->bUsrParamSync = NI_TRUE;
    UNLOCK(&gstLock);

    UNLOCK(&gstApiLock);

    NI_INFO("stop run\n");

    return NI_SUCCESS;
}

static NI_S32 _ptz_tracking_cal_ptz_ctrl_cmd(NI_VOID *pvHandle, NI_VOID *pvCmd)
{
    PTZ_TRACKING_CTX *pstCtx = NI_NULL;
    PTZ_TRACKING_PTZ_CMD_BUTT_S *pstUsrCmd = NI_NULL;

    LOCK(&gstApiLock);

    if(NI_NULL == gpstDevCtx)
    {
        UNLOCK(&gstApiLock);
        PRINT_ERROR("ptz tracking has destory\n");
        return NI_FAILURE;
    }

    pstCtx = (PTZ_TRACKING_CTX *)pvHandle;

    if(pstCtx->stGblMmz.pvVirAddr != pvHandle)
    {
        UNLOCK(&gstApiLock);
        PRINT_ERROR("haldle error\n");
        return NI_FAILURE;
    }

    pstUsrCmd = (PTZ_TRACKING_PTZ_CMD_BUTT_S *)pvCmd;

    LOCK(&gstLock);

    switch(pstUsrCmd->enType)
    {
        case NI_PTZ_TRACKING_PTZ_CMD_SET_POS:
        {
            memcpy(&pstCtx->unUsrCmd.stPtzPosCmd, pvCmd, sizeof(NI_PTZ_TRACKING_PTZ_POS_S));
            break;
        }
        case NI_PTZ_TRACKING_PTZ_CMD_RESET:
        {
            pstCtx->unUsrCmd.stRstCmd.enType = NI_PTZ_TRACKING_PTZ_CMD_RESET;
            break;
        }
        default:
        {
            UNLOCK(&gstLock);
            UNLOCK(&gstApiLock);
            PRINT_ERROR("user set ptz cmd error\n");
            return NI_FAILURE;
        }
    }

    pstCtx->bUsrCmdSync = NI_TRUE;

    _ptz_tracking_cal_cond_signal(&gstCondSleepProc);

    UNLOCK(&gstLock);

    UNLOCK(&gstApiLock);

    return NI_SUCCESS;
}

static NI_S32 _ptz_tracking_cal_attr(PTZ_TRACKING_CTX *pstCtx, NI_PTZ_TRACKING_ATTR_S *pstAttr, NI_BOOL bSet)
{
    LOCK(&gstLock);

    if(bSet)
    {
        pstCtx->stUsrParam.stPtzAdjAttr.u32MoveKpx = NI_Max(pstAttr->u32MoveKpx, 16);
        pstCtx->stUsrParam.stPtzAdjAttr.u32ObjTolerance = NI_Max(pstAttr->u32ObjTolerance, 16000 / pstCtx->u32MoveKpx + 1);
    }
    else
    {
        pstAttr->u32ObjTolerance = pstCtx->u32ObjTolerance;
        pstAttr->u32MoveKpx = pstCtx->u32MoveKpx;
    }

    pstCtx->bUsrParamSync = NI_TRUE;

    UNLOCK(&gstLock);

    return NI_SUCCESS;
}

static NI_S32 _ptz_tracking_cal_mirror(PTZ_TRACKING_CTX *pstCtx, NI_PTZ_TRACKING_MIRROR_S *pstMirror, NI_BOOL bSet)
{
    LOCK(&gstLock);

    if(bSet)
    {
        pstCtx->stUsrParam.stPtzAdjAttr.bMirror = pstMirror->bMirror;
        pstCtx->stUsrParam.stPtzAdjAttr.bflip = pstMirror->bflip;
        //pstCtx->bMirror = pstMirror->bMirror;
        pstCtx->bUsrParamSync = NI_TRUE;
    }
    else
    {
        pstMirror->bMirror = pstCtx->stUsrParam.stPtzAdjAttr.bMirror;
        pstMirror->bflip = pstCtx->stUsrParam.stPtzAdjAttr.bflip;
        //pstMirror->bMirror = pstCtx->bMirror;
        pstCtx->bUsrParamSync = NI_TRUE;
    }

    UNLOCK(&gstLock);
   
    return NI_SUCCESS;
}

static NI_S32 _ptz_tracking_cal_set_param(NI_VOID *pvHandle, NI_VOID *pvParam)
{
    NI_S32 s32Ret;
    PTZ_TRACKING_CTX *pstCtx = NI_NULL;
    PTZ_TRACKING_PARAM_BUTT_S *pstUsrCmd = NI_NULL;

    LOCK(&gstApiLock);

    if(NI_NULL == gpstDevCtx)
    {
        UNLOCK(&gstApiLock);
        PRINT_ERROR("ptz tracking has destory\n");
        return NI_FAILURE;
    }

    pstCtx = (PTZ_TRACKING_CTX *)pvHandle;

    if(pstCtx->stGblMmz.pvVirAddr != pvHandle)
    {
        UNLOCK(&gstApiLock);
        PRINT_ERROR("haldle error\n");
        return NI_FAILURE;
    }

    pstUsrCmd = (PTZ_TRACKING_PARAM_BUTT_S *)pvParam;


    switch(pstUsrCmd->enType)
    {
        case NI_PTZ_TRACKING_PARAM_TYPE_ATTR:
        {
            s32Ret = _ptz_tracking_cal_attr(pstCtx, pvParam, 1);
            break;
        }
        case NI_PTZ_TRACKING_PARAM_TYPE_MIRROR:
        {
            s32Ret = _ptz_tracking_cal_mirror(pstCtx, pvParam, 1);
            break;
        }
        default:
        {
            s32Ret = NI_FAILURE;
            PRINT_ERROR("user set param cmd erro\n");
            break;
        }
    }

    if(s32Ret != NI_SUCCESS)
    {
        UNLOCK(&gstApiLock);
        PRINT_ERROR("user set param error\n");
        return NI_FAILURE;
    }

    UNLOCK(&gstApiLock);

    return NI_SUCCESS;
}

static NI_S32 _ptz_tracking_cal_get_param(NI_VOID *pvHandle, NI_VOID *pvParam)
{
    NI_S32 s32Ret;
    PTZ_TRACKING_CTX *pstCtx = NI_NULL;
    PTZ_TRACKING_PARAM_BUTT_S *pstUsrCmd = NI_NULL;

    LOCK(&gstApiLock);

    if(NI_NULL == gpstDevCtx)
    {
        UNLOCK(&gstApiLock);
        PRINT_ERROR("ptz tracking has destory\n");
        return NI_FAILURE;
    }

    pstCtx = (PTZ_TRACKING_CTX *)pvHandle;

    if(pstCtx->stGblMmz.pvVirAddr != pvHandle)
    {
        UNLOCK(&gstApiLock);
        PRINT_ERROR("haldle error\n");
        return NI_FAILURE;
    }

    pstUsrCmd = (PTZ_TRACKING_PARAM_BUTT_S *)pvParam;

    switch(pstUsrCmd->enType)
    {
        case NI_PTZ_TRACKING_PARAM_TYPE_ATTR:
        {
            s32Ret = _ptz_tracking_cal_attr(pstCtx, pvParam, 0);
            break;
        }
        case NI_PTZ_TRACKING_PARAM_TYPE_MIRROR:
        {
            s32Ret = _ptz_tracking_cal_mirror(pstCtx, pvParam, 0);
            break;
        }
        default:
        {
            s32Ret = NI_FAILURE;
            break;
        }
    }

    if(s32Ret != NI_SUCCESS)
    {
        UNLOCK(&gstApiLock);
        PRINT_ERROR("user get param cmd error faild\n");
        return NI_FAILURE;
    }

    UNLOCK(&gstApiLock);

    return NI_SUCCESS;
}

static NI_S32 _ptz_tracking_cal_gbl_ctx_init(PTZ_TRACKING_CTX *pstCtx, NI_PTZ_TRACKING_CREATE_S *pstCreate)
{
    pstCtx->u32MoveKpx = 240;
    pstCtx->u32ObjTolerance = 360;
    pstCtx->bThreadRun = NI_TRUE;
    pstCtx->bThreadCreate = NI_TRUE;
    pstCtx->bPtzRun = NI_FALSE;
    pstCtx->bMirror = NI_FALSE;
    pstCtx->stUsrParam.bSetThreadRun = NI_TRUE;
    pstCtx->stUsrParam.stPtzAdjAttr.u32MoveKpx = 240;
    pstCtx->stUsrParam.stPtzAdjAttr.u32ObjTolerance = 360;
    pstCtx->stUsrParam.stPtzAdjAttr.bStart = NI_FALSE;
    pstCtx->stUsrParam.stPtzAdjAttr.bMirror = NI_FALSE;
    pstCtx->stUsrParam.stPtzAdjAttr.bflip = NI_FALSE;

    pstCtx->stInterFace.Run = _ptz_tracking_cal_run;
    pstCtx->stInterFace.Stop = _ptz_tracking_cal_stop;
    pstCtx->stInterFace.SetStart = _ptz_tracking_cal_start_attr;
    pstCtx->stInterFace.UserPtzCtrlCmd = _ptz_tracking_cal_ptz_ctrl_cmd;
    pstCtx->stInterFace.SetParam = _ptz_tracking_cal_set_param;
    pstCtx->stInterFace.GetParam = _ptz_tracking_cal_get_param;

    if(1 == pstCreate->u32Type)
    {
        if(NI_NULL == pstCreate->pszCfgFile)
        {
            //PtzParamIoInit(PTZ_TRACKING_DEFAULT_CFG_FILE, &pstCtx->stCfgPhrase);
        }
        else
        {
            //PtzParamIoInit(pstCreate->pszCfgFile, &pstCtx->stCfgPhrase);
        }
    }
    else if(0 == pstCreate->u32Type)
    {
        pstCtx->stCfgPhrase.u16IasWidth = pstCreate->stInitCfg.u16IasWidth;
        pstCtx->stCfgPhrase.u16IasHeight = pstCreate->stInitCfg.u16IasHeight;
        strncpy(pstCtx->stCfgPhrase.aszModelFile, pstCreate->stInitCfg.pszModelFile, sizeof(pstCtx->stCfgPhrase.aszModelFile) - 1);
        // strncpy(pstCtx->stCfgPhrase.aszCoordinateFile, pstCreate->stInitCfg.pszCoordinateFile, sizeof(pstCtx->stCfgPhrase.aszCoordinateFile) - 1);
    }

    return NI_SUCCESS;
}

NI_S32 ptz_human_tracking_create(NI_VOID **pvHandle, NI_PTZ_TRACKING_CREATE_S *pstCreate)
{
    NI_S32 s32Ret;
    PTZ_TRACKING_CTX *pstCtx = NI_NULL;

    //PRINT_ERROR("build time: %s %s\n", __DATE__, __TIME__);
    //PRINT_ERROR("PTZ TRACKING VER: %s\n", PTZ_VERSION);
    //PRINT_ERROR("PTZ TRACKING GIT: %s\n", PTZ_GIT_VERSION);

    LOCK(&gstApiLock);
    if(gpstDevCtx != NI_NULL)
    {
        *pvHandle = gpstDevCtx;
        UNLOCK(&gstApiLock);
        PRINT_ERROR("ptz tracking has created\n");
        return NI_FAILURE;
    }

    sem_init(&gstSemFrameProc, 0 ,1);
    sem_init(&gstSemResultProc, 0 ,0);
    // sem_init(&gstSemSleepProc, 0 ,0);

    s32Ret = _ptz_tracking_cal_gbl_mmz_alloc(&pstCtx, pstCreate);//分配内存
    if(s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("gbl mmz alloc faild\n");
        return s32Ret;
    }

    s32Ret = _ptz_tracking_cal_gbl_ctx_init(pstCtx, pstCreate);//初始化参数，注册回调
    if(s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("gbl ctx init faild\n");
        goto gbl_mmz_free;
    }

    s32Ret = _ptz_tracking_cal_init_motor(pstCtx);//读坐标文件，获取当前位置坐标
    if(s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("motor init faild\n");
        goto gbl_mmz_free;
    }

    //s32Ret = _ptz_tracking_cal_init_vps_ias_chn(pstCtx);//创建智能通道
    //if(s32Ret != NI_SUCCESS)
    //{
    //    PRINT_ERROR("vps ias chn init faild\n");
    //    goto deinit_motor;
    //}
    s32Ret = _ptz_tracking_cal_init_ias_od(pstCtx);//初始化人形跟踪
    if(s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("ias od init faild\n");
        goto gbl_mmz_free;
    }

    gpstDevCtx = pstCtx;
    *pvHandle = pstCtx;

    UNLOCK(&gstApiLock);

    return NI_SUCCESS;

// deinit_ias_od_chn:
//     _ptz_tracking_cal_deinit_ias_od(pstCtx);
//deinit_vps_ias_chn:
//    _ptz_tracking_cal_deinit_vps_ias_chn();
//deinit_motor:
//    _ptz_tracking_cal_deinit_motor(pstCtx);
gbl_mmz_free:
    _ptz_tracking_cal_gbl_mmz_free(&pstCtx->stGblMmz);

    UNLOCK(&gstApiLock);

    return s32Ret;
}

NI_S32 ptz_human_tracking_get_func(NI_VOID *pvHandle, NI_PTZ_TRACKING_INTERFACE_S **pstInterFace)
{
    PTZ_TRACKING_CTX *pstCtx = NI_NULL;

    LOCK(&gstApiLock);

    if(NI_NULL == gpstDevCtx)
    {
        UNLOCK(&gstApiLock);
        PRINT_ERROR("ptz tracking has not created\n");
        return NI_FAILURE;
    }

    pstCtx = (PTZ_TRACKING_CTX *)pvHandle;

    if(pstCtx->stGblMmz.pvVirAddr != pvHandle)
    {
        UNLOCK(&gstApiLock);
        PRINT_ERROR("haldle error\n");
        return NI_FAILURE;
    }

    *pstInterFace = &pstCtx->stInterFace;

    UNLOCK(&gstApiLock);

    return NI_SUCCESS;
}

NI_S32 ptz_human_tracking_reg_func_callback(NI_VOID *pvHandle, NI_VOID *pvCallBack)
{
    PTZ_TRACKING_CTX *pstCtx = NI_NULL;
    PTZ_TRACKING_REG_FUNC_CB_BUTT_S *pstUsrCmd = NI_NULL;

    LOCK(&gstApiLock);

    if(NI_NULL == gpstDevCtx)
    {
        UNLOCK(&gstApiLock);
        PRINT_ERROR("ptz tracking has not created\n");
        return NI_FAILURE;
    }

    pstCtx = (PTZ_TRACKING_CTX *)pvHandle;

    if(pstCtx->stGblMmz.pvVirAddr != pvHandle)
    {
        UNLOCK(&gstApiLock);
        PRINT_ERROR("haldle error\n");
        return NI_FAILURE;
    }

    pstUsrCmd = (PTZ_TRACKING_REG_FUNC_CB_BUTT_S *)pvCallBack;

    switch(pstUsrCmd->enFuncId)
    {
        case NI_PTZ_TRACKING_CB_FUNC_ID_OD_RESULT_PROC:
        {
            LOCK(&gstLock);
            memcpy(&pstCtx->stFuncCallBack.stOdResultProcessCb, pvCallBack, sizeof(NI_PTZ_TRACKING_OD_RESULT_PROCESS_CB_S));
            UNLOCK(&gstLock);
            break;
        }
        default:
        {
            UNLOCK(&gstApiLock);
            PRINT_ERROR("func id not support\n");
            return NI_FAILURE;
        }
    }

    UNLOCK(&gstApiLock);

    return NI_SUCCESS;
}


NI_S32 ptz_human_tracking_destory(NI_VOID *pvHandle)
{
    NI_S32 s32Ret, s32Ret1 = NI_SUCCESS;
    PTZ_TRACKING_CTX *pstCtx = NI_NULL;

    LOCK(&gstApiLock);
    if(NI_NULL == gpstDevCtx)
    {
        UNLOCK(&gstApiLock);
        PRINT_ERROR("ptz tracking has not created\n");
        return NI_FAILURE;
    }

    gpstDevCtx = NI_NULL;

    pstCtx = (PTZ_TRACKING_CTX *)pvHandle;

    if(pstCtx->stGblMmz.pvVirAddr != pvHandle)
    {
        UNLOCK(&gstApiLock);
        PRINT_ERROR("haldle error\n");
        return NI_FAILURE;
    }

    LOCK(&gstLock);
    // pstCtx->bThreadRun = NI_FALSE;
    pstCtx->bThreadCreate = NI_FALSE;
    UNLOCK(&gstLock);

    s32Ret = _ptz_tracking_cal_deinit_ias_od(pstCtx);
    if(s32Ret != NI_SUCCESS)
    {
        s32Ret1 = s32Ret;
        PRINT_ERROR("ias od deinit faild\n");
    }

    //s32Ret = _ptz_tracking_cal_deinit_vps_ias_chn();
    //if(s32Ret != NI_SUCCESS)
    //{
    //    s32Ret1 = s32Ret;
    //    PRINT_ERROR("vps ias chn deinit faild\n");
    //}

    s32Ret = _ptz_tracking_cal_deinit_motor(pstCtx);
    if(s32Ret != NI_SUCCESS)
    {
        s32Ret1 = s32Ret;
        PRINT_ERROR("motor deinit faild\n");
    }

    s32Ret = _ptz_tracking_cal_gbl_mmz_free(&pstCtx->stGblMmz);
    if(s32Ret != NI_SUCCESS)
    {
        s32Ret1 = s32Ret;
        PRINT_ERROR("gbl mmz free faild\n");
    }

    sem_destroy(&gstSemFrameProc);
    sem_destroy(&gstSemResultProc);

    UNLOCK(&gstApiLock);

    return s32Ret1;
}

int HumanTrackGoBackPoint(int xCoord, int yCoord)
{
    ReviseGoBackPoint = TRUE;
    xGoBackPoint = xCoord;
    yGoBackPoint = yCoord;
    return 0;
}