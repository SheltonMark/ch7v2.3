#include "TdCommon.h"

#define IAS_OD_TEST_CALL_BACK 0
#define IAS_OD_TEST_BIG_MODEL_FILE "/var/model/net_model_yolo_model_448x256_tiny_pcd_v2.bin"
#define IAS_OD_TEST_SMALL_MODEL_FILE "/var/model/net_model_yolo_model_256x160_tiny_pcd.bin"

typedef struct tagSendFramePhreadCTx
{
	IAS_ALG_CHN OdChn;
	//IAS_TASK_IMAGE_S stFrame;
	NI_BOOL          bPthreadSendVencRun;
	VPS_CHN_E u32VpsChn;
	VPS_FRAME_INFO_S stVpsFrame;
	NI_U32 u32JpegCnt;
}SENDFRAME_PTHREAD_CTX_S;

pthread_t gpthreadIasPid;
IAS_PTHREAD_CTX_S gstPhreadCtx;

IAS_OD_INIT_PARAM_S gstCreateParam =
{
    .stLicense =
    {
        .pData = NI_NULL,
        .u32Len = 0,
        .pcPath = NI_NULL,
    },
    .stImageSrcSize = {IAS_WIDTH, IAS_HEIGHT},
    .stObjSize = {IAS_WIDTH, 1},
    .stPipeLine = {IAS_PIPELINE_MODE_SP, 1},
    .stAlgCfg =
    {
        .stOdParam =
            {
                .u16ScoreThr = 280,
                .u16MaxObjNum = 32,
            },
        .stOtParam =
            {
                .bPositiveTrack = NI_TRUE,
                .u16MaxTrackNum = 32,
            },
        .stPgParam =
            {
                .bEnablePg = NI_TRUE,
            },
    },
};

NI_VOID *gpvHandle = NI_NULL;
NI_PTZ_TRACKING_INTERFACE_S *gpstInterFace = NI_NULL;

extern Quadrilateral gstMonitorArea[2];
extern PERIMETER_DETECT_PARAM_S gstPerimeterParam;
extern void _perimeter_detect_save_result(IAS_PG_RESULT_S *pstResult, PG_RESULT *pstPgResult);

int AddPgRule(int Channel, unsigned int *Id, IAS_PERIMETER_RULE_S *Param)
{
    int ret = -1;
    unsigned int pu32RuleId = 0;
    IAS_PERIMETER_RULE_S pstRule;

    memset(&pstRule, 0, sizeof(IAS_PERIMETER_RULE_S));

    memcpy(&pstRule, Param, sizeof(IAS_PERIMETER_RULE_S));

    ret = NI_IAS_AddPerimeterRule(Channel, &pstRule, &pu32RuleId);
    if (RETURN_OK != ret)
    {
        PRINT_ERROR("[ERROR]: Channel %d NI_IAS_AddPerimeterRule failed, ret=%x\n", Channel, ret);
        return ret;
    }
    
    PRINT_INFO("pu32RuleId %d\n", pu32RuleId);
    *Id = pu32RuleId;
    gstPerimeterParam.RuleNum[Channel]++;
    PRINT_INFO("gstPerimeterParam.RuleNum[%d] %d\n", Channel, gstPerimeterParam.RuleNum[Channel]);
    return SUCCESS;
}

int DelPgRule(int Channel, IAS_PERIMETER_RULE_S *Param)
{
    int ret = -1;
    IAS_PERIMETER_RULE_S pstRule;

    memset(&pstRule, 0, sizeof(IAS_PERIMETER_RULE_S));
    memcpy(&pstRule, Param, sizeof(IAS_PERIMETER_RULE_S));
  
    ret = NI_IAS_DelPerimeterRule(Channel, &pstRule);
    if (RETURN_OK != ret)
    {
        PRINT_ERROR("[ERROR]: Channel %d NI_IAS_DelPerimeterRule failed, ret=%x\n", Channel, ret);
        return ret;
    }

    gstPerimeterParam.RuleNum[Channel]--;
    PRINT_INFO("gstPerimeterParam.RuleNum[%d] %d\n", Channel, gstPerimeterParam.RuleNum[Channel]);

    return SUCCESS;
}

int ChangePgLevel(int Channel, int Id, int Level)
{
    int ret = -1;
    IAS_PARAMS_S pastArr;
    IAS_ALG_OD_PARAM_LIST_S pstAlgParam;

    memset(&pastArr, 0, sizeof(IAS_PARAMS_S));
    memset(&pstAlgParam, 0, sizeof(IAS_ALG_OD_PARAM_LIST_S));

    pastArr.s32ByteSize = 4;
    pastArr.s32Type = IAS_OD_PARAM_PG_SENSE;
    pastArr.as32Rev[0] = Id;
    pastArr.as32Rev[1] = ceil((double)Level / 10);

    pstAlgParam.u32ParamNum = 1;
    pstAlgParam.pastArr = &pastArr;

    ret = NI_IAS_OD_SetParams(Channel, (NI_VOID *)(&pstAlgParam));
    if (RETURN_OK != ret)
    {
        PRINT_ERROR("[ERROR]:Channel %d NI_IAS_OD_SetParams  failed, ret=%x\n",Channel, ret);
    }

    return ret;
}


int is_intersect(POINTS p1, POINTS p2, POINTS q1, POINTS q2)
{
    return (p1.x <= q2.x && p2.x >= q1.x) && (p1.y <= q2.y && p2.y >= q1.y);
}

int is_inside(POINTS p, Quadrilateral stMonitorArea)
{
    int count = 0;
    for (int i = 0; i < 4; i++)
    {
        POINTS a = stMonitorArea.Points[i];
        POINTS b = stMonitorArea.Points[(i + 1) % 4];
        if ((a.y > p.y) != (b.y > p.y))
        {
            double t = (p.y - a.y) * (b.x - a.x) / (b.y - a.y) + a.x;
            if (t > p.x)
            {
                count++;
            }
        }
    }
    return count % 2 == 1;
}

int rectangle_intersects_quadrilateral(Quadrilateral stResult, Quadrilateral stMonitorArea)
{
    for (int j = 0; j < 4; j++)
    {
        POINTS r1 = stResult.Points[j];
        POINTS r2 = stResult.Points[(j + 1) % 4];

        for (int i = 0; i < 4; i++)
        {
            POINTS p1 = stMonitorArea.Points[i];
            POINTS p2 = stMonitorArea.Points[(i + 1) % 4];

            if (is_intersect(r1, r2, p1, p2))
            {
                return TRUE;
            }
        }
    }

    for (int i = 0; i < 4; i++)
    {
        if (is_inside(stMonitorArea.Points[i], stResult))
        {
            return TRUE;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        if (is_inside(stResult.Points[i], stMonitorArea))
        {
            return TRUE;
        }
    }

    return FALSE;
}

static NI_S32 reg_od_result_process(NI_VOID *pvCbCtx, IAS_OD_RESULT_S *pstResult)
{
    unsigned int i = 0;
    //Quadrilateral Result;
    IAS_ALG_CHN OdChn = 0;
    HumanDetectDevice_p pHumanDetectDevice = &GlobalDevice.HumanDetectDevice;
	
    //由于上层在人形检测关闭时，不再获取结果，这边增加使能判断，防止下次打开人形时，直接产生报警
    pthread_rwlock_wrlock(&pHumanDetectDevice->result_lock);

    if (pstResult->u32ObjNum)
    {
        for (i = 0; i < pstResult->u32ObjNum; i++)
        {
            // Result.Points[0].x = pstResult->pstObj[i].stRect.s32Xl;
            // Result.Points[0].y = pstResult->pstObj[i].stRect.s32Yl;

            // Result.Points[1].x = pstResult->pstObj[i].stRect.s32Xr;
            // Result.Points[1].y = pstResult->pstObj[i].stRect.s32Yl;

            // Result.Points[2].x = pstResult->pstObj[i].stRect.s32Xr;
            // Result.Points[2].y = pstResult->pstObj[i].stRect.s32Yr;

            // Result.Points[3].x = pstResult->pstObj[i].stRect.s32Xl;
            // Result.Points[3].y = pstResult->pstObj[i].stRect.s32Yr;

            // if (!rectangle_intersects_quadrilateral(Result, gstMonitorArea[OdChn]))
            // {
            //     continue;
            // }

            if (pHumanDetectDevice->human_param[OdChn].Enable)
            {
                if (IAS_OBJ_TYPE_HUMAN == pstResult->pstObj[i].enType || IAS_OBJ_TYPE_FACE == pstResult->pstObj[i].enType)
                {
                    // 由于上层获取间隔与算法间隔不一致，易获取到未检出的结果，这边调整为检测到就累加
                    // 上层获取后再清零
                    pHumanDetectDevice->human_result[OdChn].Result++;
                    pHumanDetectDevice->human_result[OdChn].x1 = pstResult->pstObj[i].stRect.s32Xl;
                    pHumanDetectDevice->human_result[OdChn].y1 = pstResult->pstObj[i].stRect.s32Yl;
                    pHumanDetectDevice->human_result[OdChn].x2 = pstResult->pstObj[i].stRect.s32Xr;
                    pHumanDetectDevice->human_result[OdChn].y2 = pstResult->pstObj[i].stRect.s32Yr;
                }
            }
            else
            {
                pHumanDetectDevice->human_result[OdChn].Result = 0;
                pHumanDetectDevice->human_result[OdChn].x1 = 0;
                pHumanDetectDevice->human_result[OdChn].y1 = 0;
                pHumanDetectDevice->human_result[OdChn].x2 = 0;
                pHumanDetectDevice->human_result[OdChn].y2 = 0;
            }

            if (pHumanDetectDevice->vehicle_param[OdChn].Enable)
            {
                if (IAS_OBJ_TYPE_CAR == pstResult->pstObj[i].enType)
                {
                    // 由于上层获取间隔与算法间隔不一致，易获取到未检出的结果，这边调整为检测到就累加
                    // 上层获取后再清零
                    pHumanDetectDevice->vehicle_result[OdChn].Result++;
                    pHumanDetectDevice->vehicle_result[OdChn].x1 = pstResult->pstObj[i].stRect.s32Xl;
                    pHumanDetectDevice->vehicle_result[OdChn].y1 = pstResult->pstObj[i].stRect.s32Yl;
                    pHumanDetectDevice->vehicle_result[OdChn].x2 = pstResult->pstObj[i].stRect.s32Xr;
                    pHumanDetectDevice->vehicle_result[OdChn].y2 = pstResult->pstObj[i].stRect.s32Yr;
                }
            }
            else
            {
                pHumanDetectDevice->vehicle_result[OdChn].Result = 0;
                pHumanDetectDevice->vehicle_result[OdChn].x1 = 0;
                pHumanDetectDevice->vehicle_result[OdChn].y1 = 0;
                pHumanDetectDevice->vehicle_result[OdChn].x2 = 0;
                pHumanDetectDevice->vehicle_result[OdChn].y2 = 0;
            }
        }

    }

    pthread_rwlock_unlock(&pHumanDetectDevice->result_lock);
    /*if(pstResult->u32ObjNum)
    {
        // printf("############################### \n");
        PRINT_ERROR("odch = 0 OBJ num:%d\n", pstResult->u32ObjNum);
        for (i = 0; i < pstResult->u32ObjNum; i++)
        {
            //printf("OBJ num:%d\n", pstResult->u32ObjNum);
            PRINT_ERROR("od result id:%d, type:%d rect:[%d,%d,%d,%d] score:%d\n",
                   pstResult->pstObj[i].stIdInfo.s32ObjId,
                   pstResult->pstObj[i].enType,
                   pstResult->pstObj[i].stRect.s32Xl,
                   pstResult->pstObj[i].stRect.s32Yl,
                   pstResult->pstObj[i].stRect.s32Xr,
                   pstResult->pstObj[i].stRect.s32Yr,
                   pstResult->pstObj[i].u16Score);
        }
        // printf("############################### \n");
    }*/
    return NI_SUCCESS;
}

static NI_S32 _ias_od_get_model_file(IAS_OD_INIT_PARAM_S *pstCreateParam)
{
    NI_S32 s32Ret;
    FILE *fOdFile;
    NI_U32 u32Size;
    NI_U32 u23PhyAddr;
    NI_VOID *pvVirAddr = NI_NULL;

    fOdFile = fopen(IAS_OD_TEST_BIG_MODEL_FILE, "rb");
    if (NI_NULL == fOdFile)
    {
        PRINT_ERROR("open file %s error\n", IAS_OD_TEST_BIG_MODEL_FILE);
        return NI_FAILURE;
    }
    else
    {
        NI_INFO("open model file %s success !\n", IAS_OD_TEST_BIG_MODEL_FILE);
    }

    fseek(fOdFile, 0, SEEK_END);
    u32Size = ftell(fOdFile);
    rewind(fOdFile);
    // printf("==============fun:%s line:%u=============\n", __FUNCTION__, __LINE__);
    // getchar();

    s32Ret = NI_MDK_SYS_MmzAllocCached(&u23PhyAddr, &pvVirAddr, u32Size, "hdmodelfile", NI_NULL);
    if (s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("call NI_MDK_SYS_MmzAlloc error 0x%x", s32Ret);
        fclose(fOdFile);
        return s32Ret;
    }

    // printf("u23PhyAddr 0x%x, pvVirAddr 0x%x u32Size %d\n", u23PhyAddr, pvVirAddr, u32Size);
    // printf("==============fun:%s line:%u=============\n", __FUNCTION__, __LINE__);
    // getchar();

    fread(pvVirAddr, u32Size, 1, fOdFile);

    pstCreateParam->stAlgModel.enType = IAS_OD_MODEL_READ_FROM_MEM;
    pstCreateParam->stAlgModel.stOdModelCfg.stModelCfgOd.pData = pvVirAddr;
    pstCreateParam->stAlgModel.stOdModelCfg.stModelCfgOd.lPhyAddr = u23PhyAddr;
    pstCreateParam->stAlgModel.stOdModelCfg.stModelCfgOd.lLen = u32Size;

    pstCreateParam->stAlgModel.stOdModelCfg.stModelCfgOt.lPhyAddr = 0;
    pstCreateParam->stAlgModel.stOdModelCfg.stModelCfgOt.lLen = 0;
    pstCreateParam->stAlgModel.stOdModelCfg.stModelCfgOt.lLen = 0;

    fclose(fOdFile);
    return NI_SUCCESS;
}

static NI_S32 _ias_od_release_model_file(void)
{
    NI_S32 s32Ret;

    s32Ret = NI_MDK_SYS_MmzFree(gstCreateParam.stAlgModel.stOdModelCfg.stModelCfgOd.lPhyAddr, gstCreateParam.stAlgModel.stOdModelCfg.stModelCfgOd.pData);
    if (s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("call NI_MDK_SYS_MmzFree error 0x%x", s32Ret);
        return s32Ret;
    }

    return NI_SUCCESS;
}

static NI_S32 _ias_od_init(IAS_ALG_CHN OdChn, IAS_OD_INIT_PARAM_S *pstCreateParam)
{
    NI_S32 s32Ret;
    CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
    // pstCreateParam->stLicense.pData = NI_NULL;
    // pstCreateParam->stLicense.u32Len = 0;
    // pstCreateParam->stLicense.pcPath = NI_NULL;
    // pstCreateParam->stImageSrcSize.u16ImageW = 1920;
    // pstCreateParam->stImageSrcSize.u16ImageH = 1080;
    // pstCreateParam->stObjSize.u16MinSize = 10;
    // pstCreateParam->stObjSize.u16MaxSize = 1000;
    s32Ret = _ias_od_get_model_file(pstCreateParam);
    if (s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("call _ias_od_get_model_file error 0x%x\n", s32Ret);
        return s32Ret;
    }

    #if (IAS_OD_TEST_CALL_BACK == 0)
        pstCreateParam->stFunc.pfn_ias_od_result = NI_NULL;
        pstCreateParam->stFunc.pfn_ias_pg_result = NI_NULL;
        pstCreateParam->stFunc.pfn_ias_release_frame = NI_NULL;
    #else
        pstCreateParam->stFunc.pfn_ias_od_result = pfn_ias_od_result;
        pstCreateParam->stFunc.pfn_ias_pg_result = pfn_ias_pg_result;
        pstCreateParam->stFunc.pfn_ias_release_frame = pfn_ias_release_frame;
    #endif

    if (Mode_Rotate_90 == pCaptureDevice->Rotate ||Mode_Rotate_270 == pCaptureDevice->Rotate)
    {
        pstCreateParam->stImageSrcSize.u16ImageW = 320;
        pstCreateParam->stImageSrcSize.u16ImageH = 480;
        pstCreateParam->stObjSize.u16MaxSize = pstCreateParam->stImageSrcSize.u16ImageW;
    }
    else
    {
        pstCreateParam->stImageSrcSize.u16ImageW = 480;
        pstCreateParam->stImageSrcSize.u16ImageH = 320;
        pstCreateParam->stObjSize.u16MaxSize = pstCreateParam->stImageSrcSize.u16ImageW;
    }

    s32Ret = NI_IAS_OD_Init(OdChn, pstCreateParam);
    if (s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("call NI_IAS_OD_Init 0x%x\n", s32Ret);
        return s32Ret;
    }

    SystemAsh("rm -rf %s", IAS_OD_TEST_BIG_MODEL_FILE);
    
    return NI_SUCCESS;
}

NI_VOID *_ias_od_run(void *p)
{
    //NI_S32 i;
    int i = -1;
    NI_S32 OdChn;
    NI_S32 s32Ret = 0;
    unsigned int id = 0;
    Quadrilateral Result;
    unsigned int num = 0;
    IAS_TASK_IMAGE_S stFrame;
    IAS_OD_RESULT_S stOdResult;
    IAS_PERIMETER_RULE_S pstRule;
    VPS_FRAME_INFO_S stVpsFrameInfo;
    VPS_CHN_E u32VpsChn = VPS_CHN_IAS0;
    IAS_PTHREAD_CTX_S *pIasPThread = NI_NULL;

    HumanDetectDevice_p pHumanDetectDevice = &GlobalDevice.HumanDetectDevice;
    PerimeterDetectDevice_p pPerimeterDetectDevice = &GlobalDevice.PerimeterDetectDevice;
    pIasPThread = (IAS_PTHREAD_CTX_S *)p;

    OdChn = pIasPThread->OdChn;

    pthread_t pthread;
    pid_t pid;
    pid_t tid;
    pid = getpid();
    tid = syscall(SYS_gettid);
    pthread = pthread_self();

    PRINT_ERROR("human_detect thread pid %d tid %d thread id 0x%x\n", pid, tid, pthread);

    s32Ret = _ias_od_init(OdChn, &gstCreateParam);
    if (s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("call _ias_od_init error 0x%x\n", s32Ret);
        return NULL;
    }
    IAS_PG_RESULT_S pstResult;

    while (gstPhreadCtx.bPthreadIasRun)
    {
        s32Ret = NI_MDK_VPS_GetFrame(0, u32VpsChn, &stVpsFrameInfo, 1000);
        if (s32Ret != NI_SUCCESS)
        {
            //PRINT_ERROR("call NI_MDK_VPS_GetFrame error !!! 0x%x\n", s32Ret);
            usleep(0);
            continue;
        }
        stFrame.stFrame = stVpsFrameInfo.astVideoFrame[0];
        s32Ret = NI_IAS_OD_Run(OdChn, &stFrame, 1000);
        if (s32Ret != NI_SUCCESS)
        {
            PRINT_ERROR("call NI_IAS_OD_Run 0x%x\n", s32Ret);

            s32Ret = NI_IAS_OD_GetImageToRelease(OdChn, &stFrame, 1000);
            if (s32Ret != NI_SUCCESS)
            {
                PRINT_ERROR("call NI_IAS_OD_GetImageToRelease 0x%x\n", s32Ret);
            }

            s32Ret = NI_MDK_VPS_ReleaseFrame(0, u32VpsChn, &stVpsFrameInfo);
            if (s32Ret != NI_SUCCESS)
            {
                PRINT_ERROR("call NI_MDK_VPS_ReleaseFrame 0x%x\n", s32Ret);
            }
            usleep(0);
            continue;
            // return s32Ret;
        }

        s32Ret = NI_IAS_OD_GetResult(OdChn, &stOdResult, 1000);
        /*printf("============OBJ NUM %d   stOdResult stFrame.au32PhyAddr[0 1 2]:0x%x 0x%x 0x%x stOdResult w h [%u %u]\n", stOdResult.u32ObjNum, \
		stOdResult.stFrame.stFrame.au32PhyAddr[0],\
		stOdResult.stFrame.stFrame.au32PhyAddr[1],\
		stOdResult.stFrame.stFrame.au32PhyAddr[2],\ 	 
		stOdResult.stFrame.stFrame.u32Width,\
		stOdResult.stFrame.stFrame.u32Height);*/
        if (s32Ret != NI_SUCCESS)
        {
            PRINT_ERROR("call NI_IAS_OD_GetResult 0x%x\n", s32Ret);
            s32Ret = NI_IAS_OD_GetImageToRelease(OdChn, &stFrame, 1000);
            if (s32Ret != NI_SUCCESS)
            {
                PRINT_ERROR("call NI_IAS_OD_GetImageToRelease 0x%x\n", s32Ret);
            }

            s32Ret = NI_MDK_VPS_ReleaseFrame(0, u32VpsChn, &stVpsFrameInfo);
            if (s32Ret != NI_SUCCESS)
            {
                PRINT_ERROR("call NI_MDK_VPS_ReleaseFrame 0x%x\n", s32Ret);
            }

            usleep(0);
            continue;
        }

        memset(&Result, 0, sizeof(Quadrilateral));

        /*if (stOdResult.u32ObjNum)
        {
            // printf("############################### \n");
            printf("odch = 0 OBJ num:%d\n", stOdResult.u32ObjNum);
            for (i = 0; i < stOdResult.u32ObjNum; i++)
            {
                //printf("OBJ num:%d\n", pstResult->u32ObjNum);
                printf("od result id:%d, type:%d rect:[%d,%d,%d,%d] score:%d\n",
                       stOdResult.pstObj[i].stIdInfo.s32ObjId,
                       stOdResult.pstObj[i].enType,
                       stOdResult.pstObj[i].stRect.s32Xl,
                       stOdResult.pstObj[i].stRect.s32Yl,
                       stOdResult.pstObj[i].stRect.s32Xr,
                       stOdResult.pstObj[i].stRect.s32Yr,
                       stOdResult.pstObj[i].u16Score);
            }
            // printf("############################### \n");
        }*/
#if 1
        pthread_rwlock_wrlock(&pHumanDetectDevice->result_lock);
        if (stOdResult.u32ObjNum)
        {
            for (i = 0; i < stOdResult.u32ObjNum; i++)
            {
                Result.Points[0].x = stOdResult.pstObj[i].stRect.s32Xl;
                Result.Points[0].y = stOdResult.pstObj[i].stRect.s32Yl;

                Result.Points[1].x = stOdResult.pstObj[i].stRect.s32Xr;
                Result.Points[1].y = stOdResult.pstObj[i].stRect.s32Yl;

                Result.Points[2].x = stOdResult.pstObj[i].stRect.s32Xr;
                Result.Points[2].y = stOdResult.pstObj[i].stRect.s32Yr;

                Result.Points[3].x = stOdResult.pstObj[i].stRect.s32Xl;
                Result.Points[3].y = stOdResult.pstObj[i].stRect.s32Yr;

                if (!rectangle_intersects_quadrilateral(Result, gstMonitorArea[OdChn]))
                {
                    continue;
                }

                if (IAS_OBJ_TYPE_HUMAN == stOdResult.pstObj[i].enType || IAS_OBJ_TYPE_FACE == stOdResult.pstObj[i].enType)
                {
                    if (pHumanDetectDevice->human_param[OdChn].Enable)
                    {
                        // 由于上层获取间隔与算法间隔不一致，易获取到未检出的结果，这边调整为检测到就累加
                        // 上层获取后再清零
                        pHumanDetectDevice->human_result[OdChn].Result++;
                        pHumanDetectDevice->human_result[OdChn].x1 = stOdResult.pstObj[i].stRect.s32Xl;
                        pHumanDetectDevice->human_result[OdChn].y1 = stOdResult.pstObj[i].stRect.s32Yl;
                        pHumanDetectDevice->human_result[OdChn].x2 = stOdResult.pstObj[i].stRect.s32Xr;
                        pHumanDetectDevice->human_result[OdChn].y2 = stOdResult.pstObj[i].stRect.s32Yr;
                    }
                    else
                    {
                        pHumanDetectDevice->human_result[OdChn].Result = 0;
                        pHumanDetectDevice->human_result[OdChn].x1 = 0;
                        pHumanDetectDevice->human_result[OdChn].y1 = 0;
                        pHumanDetectDevice->human_result[OdChn].x2 = 0;
                        pHumanDetectDevice->human_result[OdChn].y2 = 0;
                    }
                }
                else if (IAS_OBJ_TYPE_CAR == stOdResult.pstObj[i].enType)
                {
                    if (pHumanDetectDevice->vehicle_param[OdChn].Enable)
                    {
                        // 由于上层获取间隔与算法间隔不一致，易获取到未检出的结果，这边调整为检测到就累加
                        // 上层获取后再清零
                        pHumanDetectDevice->vehicle_result[OdChn].Result++;
                        pHumanDetectDevice->vehicle_result[OdChn].x1 = stOdResult.pstObj[i].stRect.s32Xl;
                        pHumanDetectDevice->vehicle_result[OdChn].y1 = stOdResult.pstObj[i].stRect.s32Yl;
                        pHumanDetectDevice->vehicle_result[OdChn].x2 = stOdResult.pstObj[i].stRect.s32Xr;
                        pHumanDetectDevice->vehicle_result[OdChn].y2 = stOdResult.pstObj[i].stRect.s32Yr;
                    }
                    else
                    {
                        pHumanDetectDevice->vehicle_result[OdChn].Result = 0;
                        pHumanDetectDevice->vehicle_result[OdChn].x1 = 0;
                        pHumanDetectDevice->vehicle_result[OdChn].y1 = 0;
                        pHumanDetectDevice->vehicle_result[OdChn].x2 = 0;
                        pHumanDetectDevice->vehicle_result[OdChn].y2 = 0;
                    }
                }
            }
        }
        pthread_rwlock_unlock(&pHumanDetectDevice->result_lock);

        for (i = 0; i < gstPerimeterParam.RuleNum[OdChn]; i++)
        {
            s32Ret = NI_IAS_PG_GetResult(OdChn, &pstResult, 1000);
            if (s32Ret != NI_SUCCESS)
            {
                PRINT_INFO("gstPerimeterParam.RuleNum[%d] %d\n", OdChn, gstPerimeterParam.RuleNum[OdChn]);
                PRINT_ERROR("NI_IAS_PG_GetResult faild 0x%x\n", s32Ret);
                usleep(40 * 1000);
                continue;
            }

            if (pstResult.u32ObjNum)
            {
                pthread_rwlock_wrlock(&pPerimeterDetectDevice->result_lock);
                for (num = 0; num < PG_MAX_NUM; num++)
                {
                    if (IAS_PERIMETER_RULE_POLYLINE == pstResult.pstRule->enType)
                    { 
                        if (FALSE == pPerimeterDetectDevice->Result.PolyLineResultExist[OdChn][num]
                            || pstResult.u32RuleId == pPerimeterDetectDevice->Result.PolyLineResult[OdChn][num].Id)
                        {
                            pPerimeterDetectDevice->Result.PolyLineResultExist[OdChn][num] = TRUE;
                            _perimeter_detect_save_result(&pstResult, &pPerimeterDetectDevice->Result.PolyLineResult[OdChn][num]);
                            break;
                        }
                    }
                    else if (IAS_PERIMETER_RULE_POLYGON == pstResult.pstRule->enType)
                    {
                        if (FALSE == pPerimeterDetectDevice->Result.PolyGonResultExist[OdChn][num]
                            || pstResult.u32RuleId == pPerimeterDetectDevice->Result.PolyGonResult[OdChn][num].Id)
                        {
                            pPerimeterDetectDevice->Result.PolyGonResultExist[OdChn][num] = TRUE;
                            _perimeter_detect_save_result(&pstResult, &pPerimeterDetectDevice->Result.PolyGonResult[OdChn][num]);
                            break;
                        }
                    }
                    // PRINT_ERROR("od result id:%d, type:%d rect:[%d,%d,%d,%d] score:%d\n",
                    //             pstResult.pstObj[j].stIdInfo.s32ObjId,
                    //             pstResult.pstObj[j].enType,
                    //             pstResult.pstObj[j].stRect.s32Xl,
                    //             pstResult.pstObj[j].stRect.s32Yl,
                    //             pstResult.pstObj[j].stRect.s32Xr,
                    //             pstResult.pstObj[j].stRect.s32Yr,
                    //             pstResult.pstObj[j].u16Score);
                }
                pthread_rwlock_unlock(&pPerimeterDetectDevice->result_lock);
            }
            NI_IAS_PG_ReleaseResult(OdChn, &pstResult);
        }

#endif
        s32Ret = NI_IAS_OD_ReleaseResult(OdChn, &stOdResult);
        if (s32Ret != NI_SUCCESS)
        {
            PRINT_ERROR("call NI_IAS_OD_ReleaseResult 0x%x\n", s32Ret);
        }

        s32Ret = NI_IAS_OD_GetImageToRelease(OdChn, &stFrame, 1000);
        if (s32Ret != NI_SUCCESS)
        {
            PRINT_ERROR("call NI_IAS_OD_GetImageToRelease 0x%x\n", s32Ret);
        }

        s32Ret = NI_MDK_VPS_ReleaseFrame(0, u32VpsChn, &stVpsFrameInfo);
        if (s32Ret != NI_SUCCESS)
        {
            PRINT_ERROR("call NI_MDK_VPS_ReleaseFrame 0x%x\n", s32Ret);
        }

        pthread_rwlock_wrlock(&gstPerimeterParam.rwlock);
        for(i = 0;i < PG_MAX_NUM; i++)
        {
            memset(&pstRule, 0, sizeof(IAS_PERIMETER_RULE_S));
            memcpy(&pstRule, &gstPerimeterParam.Parm[OdChn][i], sizeof(IAS_PERIMETER_RULE_S));
            if (gstPerimeterParam.Delete[OdChn][i])
            {
                s32Ret = DelPgRule(OdChn, &pstRule);
                if (SUCCESS == s32Ret)
                {
                    PRINT_INFO("DelPgRule OdChn %d i %d\n", OdChn, i);
                    memset(&gstPerimeterParam.Parm[OdChn][i], 0, sizeof(IAS_PERIMETER_RULE_S));
                    gstPerimeterParam.Id[OdChn][i] = 0;
                    gstPerimeterParam.Delete[OdChn][i] = FALSE;
                }
            }
            if (gstPerimeterParam.Add[OdChn][i])
            {
                s32Ret = AddPgRule(OdChn, &id, &pstRule);
                if (SUCCESS == s32Ret)
                {
                    PRINT_INFO("AddPgRule OdChn %d i %d id %d\n", OdChn, i, id);
                    gstPerimeterParam.Add[OdChn][i] = FALSE;
                }
            }
            if (gstPerimeterParam.Change[OdChn][i])
            {
                s32Ret = ChangePgLevel(OdChn, gstPerimeterParam.Id[OdChn][i], gstPerimeterParam.Level[OdChn][i]);
                if (SUCCESS == s32Ret)
                {
                    PRINT_INFO("ChangePgLevel OdChn %d i %d level %d\n", OdChn, i, gstPerimeterParam.Level[OdChn][i]);
                    gstPerimeterParam.Change[OdChn][i] = FALSE;
                }
            }
        }
        pthread_rwlock_unlock(&gstPerimeterParam.rwlock);
        
        usleep(0);
    }

    return NULL;
}

extern int pu32RuleId[2];

NI_S32 _ias_run(IAS_ALG_CHN OdChn)
{
    NI_S32 s32Ret = NI_FAILURE;

    //NI_MDK_SYS_WriteReg(0x080020d8, 0x85631fe4);
    if (NI_FALSE == gstPhreadCtx.bPthreadIasRun)
    {
	    //在创建人形的时候，将跨线和电子围栏的配置清除
	    memset(&gstPerimeterParam, 0, sizeof(PERIMETER_DETECT_PARAM_S));
        pu32RuleId[OdChn] = -1;

        gstPhreadCtx.bPthreadIasRun = NI_TRUE;
        gstPhreadCtx.OdChn = OdChn;

        s32Ret = pthread_create(&gpthreadIasPid, 0, _ias_od_run, (void *)&gstPhreadCtx);
        if (s32Ret != NI_SUCCESS)
        {
            PRINT_ERROR("call pthread_create error 0x%x\n", s32Ret);
            return s32Ret;
        }
    }

    return s32Ret;
}

NI_VOID _ias_thread_stop(NI_VOID)
{
    if (NI_TRUE == gstPhreadCtx.bPthreadIasRun)
    {
        gstPhreadCtx.bPthreadIasRun = NI_FALSE;
        pthread_join(gpthreadIasPid, 0);
    }
}

int human_detect_start(void)
{
    int ret;
    IAS_ALG_CHN OdChn = 0;

    ret = _ias_run(OdChn);
    if(RETURN_OK != ret)
    {
        PRINT_ERROR("human_detect_start create failed!! ret : %x\n",ret);
    }
    return ret;
}

int human_detect_stop(void)
{
    int ret = -1;
    IAS_ALG_CHN OdChn = 0;

    if (NI_TRUE == gstPhreadCtx.bPthreadIasRun)
    {
        gstPhreadCtx.bPthreadIasRun = NI_FALSE;
        pthread_join(gpthreadIasPid, 0);

        ret = NI_IAS_OD_DeInit(OdChn);
        if (RETURN_OK != ret)
        {
            PRINT_ERROR("NI_IAS_OD_DeInit failed!! ret : %x\n", ret);
        }

        ret = _ias_od_release_model_file();
        if (RETURN_OK != ret)
        {
            PRINT_ERROR("_ias_od_release_model_file failed!! ret : %x\n", ret);
        }

    }

    return ret;
}

int HumanTrackInit(void)
{
    NI_S32 s32Ret = -1;
    NI_BOOL bStart = 0;
    NI_PTZ_TRACKING_CREATE_S stCreate;
    NI_PTZ_TRACKING_OD_RESULT_PROCESS_CB_S stOdResultProc;

    memset(&stCreate, 0, sizeof(NI_PTZ_TRACKING_CREATE_S));
    memset(&stOdResultProc, 0, sizeof(NI_PTZ_TRACKING_OD_RESULT_PROCESS_CB_S));

    //NI_MDK_SYS_WriteReg(0x080020d8, 0x9aef9f76);
    //NI_MDK_SYS_WriteReg(0x080020d8, 0x8AC63FC8);

    stCreate.u32Type = 0;
    stCreate.pszCfgFile = NI_NULL;
    
    stCreate.stInitCfg.pszModelFile = IAS_OD_TEST_BIG_MODEL_FILE;
    stCreate.stInitCfg.u16IasWidth = IAS_WIDTH;
    stCreate.stInitCfg.u16IasHeight = IAS_HEIGHT;

    s32Ret = ptz_human_tracking_create(&gpvHandle, &stCreate);

    if (s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("ptz track creat failed\n");
        return NI_FAILURE;
    }

    s32Ret = ptz_human_tracking_get_func(gpvHandle, &gpstInterFace);
    if (s32Ret != NI_SUCCESS)
    {
        ptz_human_tracking_destory(gpvHandle);
        PRINT_ERROR("ptz track get func failed\n");
        return NI_FAILURE;
    }

    stOdResultProc.enFuncId = NI_PTZ_TRACKING_CB_FUNC_ID_OD_RESULT_PROC;
    stOdResultProc.pvCbCtx = NI_NULL;
    stOdResultProc.RegOdResultProcess = reg_od_result_process;
    s32Ret = ptz_human_tracking_reg_func_callback(gpvHandle, &stOdResultProc);
    if (s32Ret != NI_SUCCESS)
    {
        ptz_human_tracking_destory(gpvHandle);
        PRINT_ERROR("ptz track reg func failed\n");
        return NI_FAILURE;
    }

    s32Ret = gpstInterFace->SetStart(gpvHandle, bStart);
    if (s32Ret != NI_SUCCESS)
    {
        NI_ERROR("ptz track stop faild\n");
        return NI_FAILURE;
    }

    SystemAsh("rm -rf %s", IAS_OD_TEST_BIG_MODEL_FILE);
    SystemAsh("rm -rf %s", IAS_OD_TEST_SMALL_MODEL_FILE);

    return NI_SUCCESS;
}

int HumanTrackDeInit(void)
{
    int s32Ret = -1;

    s32Ret = ptz_human_tracking_destory(gpvHandle);
    if (s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("ptz track destroy failed\n");
        return NI_FAILURE;
    }

    return NI_SUCCESS;
}

int HumanTrackRun(unsigned int Enable)
{
    int s32Ret = -1;

    if (0 != Enable && 1 != Enable)
    {
        PRINT_ERROR("HumanTrackRun param error Enable = %d\n", Enable);
        return NI_FAILURE;
    }

    if(NULL == gpvHandle)
    {
        PRINT_ERROR("HumanTrackRun HumanTrack is not create\n");
        return NI_FAILURE;
    }

    s32Ret = gpstInterFace->SetStart(gpvHandle, Enable);

    if (s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("ptz track start/stop:%d failed\n", Enable);
        return NI_FAILURE;
    }

    return NI_SUCCESS;
}

int HumanTrackSetParam(unsigned int MoveKpx)
{
    int s32Ret = -1;
    NI_VOID *pvParam = NI_NULL;
    NI_PTZ_TRACKING_ATTR_S stAttr;

    memset(&stAttr, 0, sizeof(NI_PTZ_TRACKING_ATTR_S));

    if(NULL == gpvHandle)
    {
        PRINT_ERROR("HumanTrack is not create\n");
        return NI_FAILURE;
    }

    stAttr.enType = NI_PTZ_TRACKING_PARAM_TYPE_ATTR;
    pvParam = &stAttr;

    s32Ret = gpstInterFace->GetParam(gpvHandle, pvParam);
    if (s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("ptz track get:%x failed\n", s32Ret);
        return NI_FAILURE;
    }

    stAttr.u32MoveKpx = MoveKpx;

    s32Ret = gpstInterFace->SetParam(gpvHandle, pvParam);
    if (s32Ret != NI_SUCCESS)
    {
        PRINT_ERROR("ptz track set:%d failed\n", s32Ret);
        return NI_FAILURE;
    }

    printf("tolerance %d kpx %d\n", stAttr.u32ObjTolerance, stAttr.u32MoveKpx);

    return NI_SUCCESS;
}

int HumanTrackSetMirror(unsigned int mirror, unsigned int flip)
{
    int s32Ret;
    NI_VOID *pvParam = NI_NULL;
    NI_PTZ_TRACKING_MIRROR_S stMirror;

    memset(&stMirror,0,sizeof(NI_PTZ_TRACKING_MIRROR_S));

    if (0 != mirror && 1 != mirror && 0 != flip && 1 != flip)
    {
        PRINT_ERROR("HumanTrackSetMirror param error mirror = %d flip = %d\n", mirror, flip);
        return NI_FAILURE;
    }

    if (NULL == gpvHandle)
    {
        PRINT_ERROR("HumanTrackSetMirror HumanTrack is not create\n");
        return NI_FAILURE;
    }

    stMirror.enType = NI_PTZ_TRACKING_PARAM_TYPE_MIRROR;
    stMirror.bMirror = mirror;
    stMirror.bflip = flip;
    pvParam = &stMirror;

    s32Ret = gpstInterFace->SetParam(gpvHandle, pvParam);
    if (s32Ret != NI_SUCCESS)
    {
        NI_ERROR("ptz track SetParam:mirror %d flip %d failed\n", mirror, flip);
        return NI_FAILURE;
    }

    return NI_SUCCESS;
}

