#include "TdCommon.h"

#define IAS_PD_MODEL_FILE "/var/model/net_model_yolo_model_480x320_ptd.bin"
#define IAS_PD_OC_FILE "/var/model/net_model_yolo_model_112x112_ptc.bin"

pthread_t pdthreadIasPid;
IAS_MODEL_CFG_S PdModelCfg;
IAS_MODEL_CFG_S PdOcModelCfg;
IAS_PTHREAD_CTX_S PdPhreadCtx;
int oc_file_exits = FALSE;

PetDetectDevice_t PetDetectDevice;

static int _ias_pd_get_model_file()
{
    int s32Ret = -1;
    FILE *fOdFile = NULL;
    unsigned int u32Size = 0;
    unsigned int u23PhyAddr = 0;
    void *pvVirAddr = NI_NULL;

    fOdFile = fopen(IAS_PD_MODEL_FILE, "rb");
    if (NI_NULL == fOdFile)
    {
        PRINT_ERROR("open file %s error\n", IAS_PD_MODEL_FILE);
        return FAILED;
    }
    else
    {
        PRINT_INFO("open model file %s success !\n", IAS_PD_MODEL_FILE);
    }

    fseek(fOdFile, 0, SEEK_END);
    u32Size = ftell(fOdFile);
    rewind(fOdFile);

    s32Ret = NI_MDK_SYS_MmzAllocCached(&u23PhyAddr, &pvVirAddr, u32Size, "pdmodelfile", NI_NULL);
    if (RETURN_OK != s32Ret)
    {
        PRINT_ERROR("call NI_MDK_SYS_MmzAllocCached error 0x%x", s32Ret);
        fclose(fOdFile);
        return s32Ret;
    }

    fread(pvVirAddr, u32Size, 1, fOdFile);
    PdModelCfg.pData = pvVirAddr;
    PdModelCfg.lPhyAddr = u23PhyAddr;
    PdModelCfg.lLen = u32Size;
    fclose(fOdFile);

    SystemAsh("rm -rf %s", IAS_PD_MODEL_FILE);

    s32Ret = access(IAS_PD_OC_FILE, F_OK);
    if (RETURN_OK == s32Ret)
    {
        fOdFile = fopen(IAS_PD_OC_FILE, "rb");
        if (NULL == fOdFile)
        {
            PRINT_ERROR("open file %s error\n", IAS_PD_OC_FILE);
            return FAILED;
        }
        else
        {
            PRINT_INFO("open model file %s success !\n", IAS_PD_OC_FILE);
        }

        fseek(fOdFile, 0, SEEK_END);
        u32Size = ftell(fOdFile);
        rewind(fOdFile);

        s32Ret = NI_MDK_SYS_MmzAllocCached(&u23PhyAddr, &pvVirAddr, u32Size, "pdocmodelfile", NI_NULL);
        if (s32Ret != SUCCESS)
        {
            PRINT_ERROR("call NI_MDK_SYS_MmzAlloc error 0x%x", s32Ret);
            fclose(fOdFile);
            return s32Ret;
        }

        fread(pvVirAddr, u32Size, 1, fOdFile);
        PdOcModelCfg.pData = pvVirAddr;
        PdOcModelCfg.lPhyAddr = u23PhyAddr;
        PdOcModelCfg.lLen = u32Size;
        fclose(fOdFile);
        oc_file_exits = TRUE;

        SystemAsh("rm -rf %s", IAS_PD_OC_FILE);
    }

    return SUCCESS;
}

static int _ias_pd_release_model_file(void)
{
    int s32Ret = -1;

    s32Ret = NI_MDK_SYS_MmzFree(PdModelCfg.lPhyAddr, PdModelCfg.pData);
    if (RETURN_OK != s32Ret)
    {
        PRINT_ERROR("call NI_MDK_SYS_MmzFree error 0x%x", s32Ret);
        return s32Ret;
    }

    if (oc_file_exits)
    {
        s32Ret = NI_MDK_SYS_MmzFree(PdOcModelCfg.lPhyAddr, PdOcModelCfg.pData);
        if (s32Ret != SUCCESS)
        {
            PRINT_ERROR("call NI_MDK_SYS_MmzFree error 0x%x", s32Ret);
            return s32Ret;
        }
    }

    return SUCCESS;
}

void *_ias_pd_run(void *p)
{
    int OdChn;
    int s32Ret = 0;
    VPS_GRP VpsGrp = 0;
    IAS_TASK_IMAGE_S stFrame;
    IAS_PTD_RESULT_S stOdResult;
    IAS_PTC_RESULT_S stOcResult;
    VPS_FRAME_INFO_S stVpsFrameInfo;
    VPS_CHN_E u32VpsChn = VPS_CHN_VIRTIAS0;
    IAS_PTHREAD_CTX_S *pIasPThread = NI_NULL;

    memset(&stFrame, 0, sizeof(IAS_TASK_IMAGE_S));
    memset(&stOdResult, 0, sizeof(IAS_PTD_RESULT_S));
    memset(&stOcResult, 0, sizeof(IAS_PTC_RESULT_S));
    memset(&stVpsFrameInfo, 0, sizeof(VPS_FRAME_INFO_S));
    
    pIasPThread = (IAS_PTHREAD_CTX_S *)p;

    OdChn = pIasPThread->OdChn;

    pthread_t pthread;
    pid_t pid;
    pid_t tid;
    pid = getpid();
    tid = syscall(SYS_gettid);
    pthread = pthread_self();

    PRINT_ERROR("pet_detect thread pid %d tid %d thread id 0x%x\n", pid, tid, pthread);

    while (PdPhreadCtx.bPthreadIasRun)
    {

        s32Ret = NI_MDK_VPS_GetFrame(VpsGrp, u32VpsChn, &stVpsFrameInfo, 20000);
        if (RETURN_OK != s32Ret)
        {
            PRINT_ERROR("call NI_MDK_VPS_GetFrame error !!! 0x%x\n", s32Ret);
            continue;
        }

        stFrame.stFrame = stVpsFrameInfo.astVideoFrame[0];
        s32Ret = NI_IAS_PTD_Run(OdChn, &stFrame, 20000);
        if (RETURN_OK != s32Ret)
        {
            PRINT_ERROR("call NI_IAS_PTD_Run 0x%x\n", s32Ret);
            
            s32Ret = NI_MDK_VPS_ReleaseFrame(VpsGrp, u32VpsChn, &stVpsFrameInfo);
            if (RETURN_OK != s32Ret)
            {
                PRINT_ERROR("call NI_MDK_VPS_ReleaseFrame 0x%x\n", s32Ret);
            }
            continue;
        }

        s32Ret = NI_IAS_PTD_GetRectResult(OdChn, &stOdResult, 20000);
        if (RETURN_OK != s32Ret)
        {
            s32Ret = NI_MDK_VPS_ReleaseFrame(0, u32VpsChn, &stVpsFrameInfo);
            if (RETURN_OK != s32Ret)
            {
                PRINT_ERROR("call NI_MDK_VPS_ReleaseFrame 0x%x\n", s32Ret);
            }

            s32Ret = NI_IAS_PTD_GetImageToRelease(OdChn, &stFrame, 20000);
            if (RETURN_OK != s32Ret)
            {
                PRINT_ERROR("call NI_IAS_PTD_GetImageToRelease 0x%x\n", s32Ret);
            }

            PRINT_ERROR("call NI_IAS_PTD_GetRectResult 0x%x\n", s32Ret);
            continue;
        }

        s32Ret = NI_IAS_PTD_ReleaseRectResult(OdChn, &stOdResult);
        if (RETURN_OK != s32Ret)
        {
            PRINT_ERROR("call NI_IAS_PTD_ReleaseRectResult 0x%x\n", s32Ret);
        }

        s32Ret = NI_IAS_PTC_GetCaptureResult(OdChn, &stOcResult, 20000);
        if (RETURN_OK != s32Ret)
        {
            s32Ret = NI_IAS_PTD_GetImageToRelease(OdChn, &stFrame, 20000);
            if (RETURN_OK != s32Ret)
            {
                PRINT_ERROR("call NI_IAS_PTD_GetImageToRelease 0x%x\n", s32Ret);
            }

            s32Ret = NI_MDK_VPS_ReleaseFrame(0, u32VpsChn, &stVpsFrameInfo);
            if (RETURN_OK != s32Ret)
            {
                PRINT_ERROR("call NI_IAS_PTD_ReleaseRectResult 0x%x\n", s32Ret);
            }

            PRINT_ERROR("call NI_IAS_PTC_GetCaptureResult 0x%x\n", s32Ret);
            continue;
        }
        // 由于上层在宠物检测关闭时，不再获取结果，这边增加使能判断，防止下次打开宠物时，直接产生报警
        pthread_mutex_lock(&PetDetectDevice.Result.result_lock);
        if (stOcResult.u32ObjNum)
        {
            if (PetDetectDevice.param.Enable)
            {
                // printf("############################### \n");
                // printf("OBJ num:%d\n", stOdResult.u32ObjNum);
                for (int i = 0; i < stOcResult.u32ObjNum; i++)
                {
                    if (IAS_OBJ_TYPE_PET == stOcResult.pstObj->enType)
                    {
                        // 由于上层获取间隔与算法间隔不一致，易获取到未检出的结果，这边调整为检测到就累加
                        // 上层获取后再清零
                        PetDetectDevice.Result.Result++;
                        PetDetectDevice.Result.x1 = stOcResult.pstObj[i].stRect.s32Xl;
                        PetDetectDevice.Result.y1 = stOcResult.pstObj[i].stRect.s32Yl;
                        PetDetectDevice.Result.x2 = stOcResult.pstObj[i].stRect.s32Xr;
                        PetDetectDevice.Result.y2 = stOcResult.pstObj[i].stRect.s32Yr;
                    }
                    // printf("od result id:%d, type:%d rect:[%d,%d,%d,%d] score %d\n",
                    //        stOcResult.pstObj[i].stIdInfo.s32ObjId,
                    //        stOcResult.pstObj[i].enType,
                    //        stOcResult.pstObj[i].stRect.s32Xl,
                    //        stOcResult.pstObj[i].stRect.s32Yl,
                    //        stOcResult.pstObj[i].stRect.s32Xr,
                    //        stOcResult.pstObj[i].stRect.s32Yr,
                    //        stOcResult.pstObj[i].stPtcInfo.s32PtcScore);
                }
                // printf("############################### \n");
            }
            else
            {
                PetDetectDevice.Result.Result = 0;
                PetDetectDevice.Result.x1 = 0;
                PetDetectDevice.Result.y1 = 0;
                PetDetectDevice.Result.x2 = 0;
                PetDetectDevice.Result.y2 = 0;
            }
        }
        pthread_mutex_unlock(&PetDetectDevice.Result.result_lock);

        s32Ret = NI_IAS_PTC_ReleaseCaptureResult(OdChn, &stOcResult);
        if (RETURN_OK != s32Ret)
        {
            PRINT_ERROR("call NI_IAS_PTC_ReleaseCaptureResult 0x%x\n", s32Ret);
        }

        s32Ret = NI_IAS_PTD_GetImageToRelease(OdChn, &stFrame, 20000);
        if (RETURN_OK != s32Ret)
        {
            PRINT_ERROR("call NI_IAS_PTD_GetImageToRelease 0x%x\n", s32Ret);
        }

        s32Ret = NI_MDK_VPS_ReleaseFrame(VpsGrp, u32VpsChn, &stVpsFrameInfo);
        if (RETURN_OK != s32Ret)
        {
            PRINT_ERROR("call NI_MDK_VPS_ReleaseFrame 0x%x\n", s32Ret);
        }
    }
    return NULL;
}

int _pet_ias_run(IAS_ALG_CHN OdChn)
{
    int ret = -1;
    IAS_PTD_INIT_PARAM_S stCreateParam;

    memset(&stCreateParam, 0, sizeof(IAS_PTD_INIT_PARAM_S));
    memset(&PetDetectDevice, 0, sizeof(PetDetectDevice_t));

    ret = _ias_pd_get_model_file();
    if (RETURN_OK != ret)
    {
        PRINT_ERROR("_pet_ias_run _ias_pd_get_model_file error 0x%x\n", ret);
        return ret;
    }

    stCreateParam.stAlgModel.enType = IAS_OD_MODEL_READ_FROM_MEM;
    stCreateParam.stAlgModel.stPtdModelCfg.stModelCfgPtd.pData = PdModelCfg.pData;
    stCreateParam.stAlgModel.stPtdModelCfg.stModelCfgPtd.lPhyAddr = PdModelCfg.lPhyAddr;
    stCreateParam.stAlgModel.stPtdModelCfg.stModelCfgPtd.lLen = PdModelCfg.lLen;

    stCreateParam.stAlgCfg.stPttParam.bPositiveTrack = NI_TRUE;
    stCreateParam.stAlgCfg.stPttParam.u16MaxTrackNum = 50;

    /*由于静止过滤效果不佳，故关闭此功能*/
    /*开启宠物静止过滤，等级为3，OS内存占用增加160KB*/
    // stCreateParam.stAlgCfg.stPtfParam.bEnablePtf = NI_TRUE;
    // stCreateParam.stAlgCfg.stPtfParam.enProcType = IAS_PTC_PTF_PROC_LEVEL_3;

    stCreateParam.stImageSrcSize.u16ImageW = IAS_WIDTH;
    stCreateParam.stImageSrcSize.u16ImageH = IAS_HEIGHT;

    stCreateParam.stObjSize.u16MinSize = 1;
    stCreateParam.stObjSize.u16MaxSize = IAS_WIDTH;

    stCreateParam.stPipeLine.enPipeMode = IAS_PIPELINE_MODE_SP;
    stCreateParam.stPipeLine.u32BufNum = 1;

    stCreateParam.stAlgCfg.stPtdParam.u16ScoreThr = 300;
    stCreateParam.stAlgCfg.stPtdParam.u16MaxObjNum = 32;

    stCreateParam.stProcCfg.enProcMode = IAS_PROC_MODE_MULTI_TASK;

    if (oc_file_exits)
    {
        stCreateParam.stAlgModel.stPtdModelCfg.stModelCfgPtc.pData = PdOcModelCfg.pData;
        stCreateParam.stAlgModel.stPtdModelCfg.stModelCfgPtc.lPhyAddr = PdOcModelCfg.lPhyAddr;
        stCreateParam.stAlgModel.stPtdModelCfg.stModelCfgPtc.lLen = PdOcModelCfg.lLen;

        stCreateParam.stAlgCfg.stPtcParam.bEnablePtc = TRUE;
        stCreateParam.stAlgCfg.stPtcParam.u16ScoreThr = 300;
    }

    stCreateParam.stAlgCfg.stPtcParam.bEnableAccu = NI_TRUE;
    stCreateParam.stAlgCfg.stPtcParam.u16MaxNum = 15;
    ret = NI_IAS_PTD_Init(OdChn, &stCreateParam);
    if (RETURN_OK != ret)
    {
        PRINT_ERROR("_pet_ias_run NI_IAS_PTD_Init failed!! ret : %x\n", ret);
        return ret;
    }

    PdPhreadCtx.OdChn = OdChn;
    PdPhreadCtx.bPthreadIasRun = TRUE;

    ret = pthread_create(&pdthreadIasPid, 0, _ias_pd_run, (void *)&PdPhreadCtx);
    if (RETURN_OK != ret)
    {
        PRINT_ERROR("_pet_ias_run pthread_create failed\n");
        return ret;
    }

    return ret;
}

/// 创建宠物检测设备
///
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int PetDetectCreate(void)
{
    int ret = -1;
    IAS_ALG_CHN OdChn = 0;

    ret = _pet_ias_run(OdChn);
    if (RETURN_OK != ret)
    {
        PRINT_ERROR("pet_detect_start create failed!! ret : %x\n", ret);
    }

    return ret;
}

/// 销毁宠物检测设备
///
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int PetDetectDestroy(void)
{
    int ret;
    IAS_ALG_CHN OdChn = 0;

    if (NI_TRUE == PdPhreadCtx.bPthreadIasRun)
    {
        PdPhreadCtx.bPthreadIasRun = NI_FALSE;
        pthread_join(pdthreadIasPid, 0);
    }

    ret = NI_IAS_PTD_DeInit(OdChn);
    if (RETURN_OK != ret)
    {
        PRINT_ERROR("NI_IAS_PTD_DeInit failed!! ret : %x\n", ret);
    }

    ret = _ias_pd_release_model_file();
    if (RETURN_OK != ret)
    {
        PRINT_ERROR("_ias_pd_release_model_file failed!! ret : %x\n", ret);
    }

    return ret;
}

/// 设置宠物检测的参数。立即返回。
///
/// \param [in] channel 通道号。
/// \param [in] pParam 指向动态检测参数结构PET_DETECT_PARAM的指针。
/// \retval 0  设置成功
/// \retval <0  设置失败
int PetDetectSetParameter(int channel, PET_DETECT_PARAM *pParam)
{
    int ret = -1;
    IAS_ALG_CHN OdChn = 0;
    IAS_PTD_PARAM_S pstAlgParam;

    memset(&pstAlgParam, 0, sizeof(IAS_PTD_PARAM_S));

    PetDetectDevice.param.Enable = pParam->Enable;

    ret = NI_IAS_PTD_GetParams(OdChn, &pstAlgParam);
    if (RETURN_OK != ret)
    {
        PRINT_ERROR("[ERROR]: NI_IAS_PTD_GetParams failed, ret=%x\n", ret);
		return ret;
    }

    pstAlgParam.stPtdParam.u16ScoreThr = 350 - pParam->Level;

    ret = NI_IAS_PTD_SetFullyParams(OdChn, &pstAlgParam);
    if (RETURN_OK != ret)
    {
        PRINT_ERROR("[ERROR]: NI_IAS_OD_SetParams failed, ret=%x\n", ret);
		return ret;
    }

    return ret;
}

/// 宠物检测捕获的详细结果。
///
/// \param [in] channel 通道号。
/// \param [in] pResult 指向动态检测参数结构PET_DETECT_RESULT的指针。
/// \retval 0  检测到移动宠物
/// \retval <0  未检测到移动宠物
int PetDetectGetResult(int channel, PET_DETECT_RESULT *pResult)
{
    int ret = -1;

    if (PetDetectDevice.param.Enable)
    {
        pthread_mutex_lock(&PetDetectDevice.Result.result_lock);
        pResult->result = PetDetectDevice.Result.Result;
        pResult->left = PetDetectDevice.Result.x1;
        pResult->top = PetDetectDevice.Result.y1;
        pResult->right = PetDetectDevice.Result.x2;
        pResult->bottom = PetDetectDevice.Result.y2;

        // 上层获取检测结果后清零
        PetDetectDevice.Result.Result = 0;
        PetDetectDevice.Result.x1 = 0;
        PetDetectDevice.Result.y1 = 0;
        PetDetectDevice.Result.x2 = 0;
        PetDetectDevice.Result.y2 = 0;
        pthread_mutex_unlock(&PetDetectDevice.Result.result_lock);
    }
    else
    {
        pResult->result = 0;
        pResult->left = 0;
        pResult->top = 0;
        pResult->right = 0;
        pResult->bottom = 0;
    }

    if (pResult->result)
    {
        ret = 0;
    }

    return ret;
}
