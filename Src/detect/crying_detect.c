#include "TdCommon.h"
#include "ring_buffer.h"

#define IAS_AD_MODEL_FILE "/var/model/net_model_ad_model_256x256_ad_8k.bin"

pthread_t stOdResultThread;

static IAS_AD_INIT_PARAM_S adgstCreateParam = {0};
static IAS_AD_TEST_CTX_S adCtxParam = {0};
static IAS_AD_RESULT_S stResult= {0};
RING_BUFF AD_PCM_BUFF;

int ias_ad_write_pcm(unsigned char *pcm_buff, unsigned int pcm_length)
{
    int ret = -1;

    if (DISABLE == adCtxParam.bRunGetOdResult)
    {
        return RETURN_OK;
    }

    ret = write_ring_buff(&AD_PCM_BUFF, pcm_buff, pcm_length);
    if (ret != RETURN_OK)
    {
        PRINT_ERROR("ias_ad_write_pcm write_ring_buff failed");
    }

    return ret;
}

int ias_ad_read_pcm(unsigned char *pcm_buff, unsigned int pcm_length)
{
    int ret = -1;

    ret = read_ring_buff(&AD_PCM_BUFF, pcm_buff, pcm_length);
    if (ret != RETURN_OK)
    {
        PRINT_ERROR("ias_ad_read_pcm read_ring_buff failed");
    }

    return ret;
}

/* ========================================================================== 
* 函数名 : _ias_ad_get_model_file
* 描  述 : 获取模型文件
* 输  入 : 
*          input    -i    pstCreateParam 参数结构体
* 输  出 : 无
* 返  回 : 状态码 
========================================================================== */
static int _ias_ad_get_model_file(IAS_AD_INIT_PARAM_S *pstCreateParam)
{
    int iRet;
    FILE *fOdFile;
    unsigned int u32Size;
    unsigned int u23PhyAddr;
    void *pvVirAddr = NULL;

    fOdFile = fopen(IAS_AD_MODEL_FILE, "rb");
    if (NULL == fOdFile)
    {
        PRINT_INFO("open file %s error\n", IAS_AD_MODEL_FILE);
        return NI_FAILURE;
    }
    else
    {
        PRINT_INFO("open model file %s success !\n", IAS_AD_MODEL_FILE);
    }

    fseek(fOdFile, 0, SEEK_END);
    u32Size = ftell(fOdFile);
    rewind(fOdFile);

    iRet = NI_MDK_SYS_MmzAllocCached(&u23PhyAddr, 
                                       &pvVirAddr, 
                                        u32Size, 
                                        "cdmodelfile", 
                                        NULL);
    if(iRet != 0)
    {
        PRINT_ERROR("call NI_MDK_SYS_MmzAlloc error 0x%x", iRet);
        fclose(fOdFile);
        return iRet;
    }

    //printf("u23PhyAddr 0x%x, pvVirAddr 0x%x u32Size %d\n", u23PhyAddr, pvVirAddr, u32Size);

    fread(pvVirAddr, u32Size, 1, fOdFile);

    pstCreateParam->stAlgModel.enType                             = IAS_AD_MODEL_READ_FROM_MEM;
    pstCreateParam->stAlgModel.stAdModelCfg.stModelCfgAd.pData    = pvVirAddr;
    pstCreateParam->stAlgModel.stAdModelCfg.stModelCfgAd.lPhyAddr = u23PhyAddr;
    pstCreateParam->stAlgModel.stAdModelCfg.stModelCfgAd.lLen     = u32Size;
    fclose(fOdFile);
    return 0;
}

/* ========================================================================== 
* 函数名 : _ias_ad_get_model_file
* 描  述 : 释放模型文件
* 输  入 : 
*          input    -i    pstCreateParam 参数结构体
* 输  出 : 无
* 返  回 : 状态码 
========================================================================== */
static int _ias_ad_release_model_file(IAS_AD_INIT_PARAM_S *pstCreateParam)
{
    int iRet;

    iRet = NI_MDK_SYS_MmzFree(pstCreateParam->stAlgModel.stAdModelCfg.stModelCfgAd.lPhyAddr, 
                                pstCreateParam->stAlgModel.stAdModelCfg.stModelCfgAd.pData);
    if(iRet != 0)
    {
        PRINT_ERROR("call NI_MDK_SYS_MmzFree error 0x%x", iRet);
        return iRet;
    }

    return 0;
}

/* ========================================================================== 
* 函数名 : pfn_ias_ad_result
* 描  述 : 检测结果
* 输  入 : 
*          input    -i    OdChn     通道数
*          input    -i    pstResult 检测结果
* 输  出 : 无
* 返  回 : 状态码 
========================================================================== */
// static int pfn_ias_ad_result(IAS_ALG_CHN      OdChn, 
//                                 IAS_AD_RESULT_S *pstResult)
// {
//     //IAS_AD_TEST_CTX_Sprintf("run pfn_ias_face_rect_result\n");
//     printf("crystatus %d score %d\n", pstResult->stResult.enCryStatus, pstResult->stResult.u16Score);

//     return 0;
// }

/* ========================================================================== 
* 函数名 : _ias_ad_get_result_process
* 描  述 : 获取检测结果程序
* 输  入 : 
*          input    -i    p     参数指针
* 输  出 : 无
* 返  回 : 状态码 
========================================================================== */
static void *_ias_ad_get_result_process()
{
    pthread_t pthread;
    pid_t pid;
    pid_t tid;
    pid = getpid();
    tid = syscall(SYS_gettid);
    pthread = pthread_self();
    PRINT_ERROR("crying_detect thread pid %d tid %d thread id 0x%x\n", pid, tid, pthread);

    int iRet = -1;
    unsigned int u32FrameId = 0;
    unsigned char *buff = NULL;
    unsigned int size = 640;
    IAS_TASK_WAVE_S stFrame;
    memset(&stFrame, 0 , sizeof(IAS_TASK_WAVE_S));
    memset(&stResult, 0 , sizeof(IAS_AD_RESULT_S));
    if(buff == NULL)
    {  
        buff = malloc(size);
    }
    memset(buff, 0 , size);

    while(adCtxParam.bRunGetOdResult)
    {
        if((AD_PCM_BUFF.tail - AD_PCM_BUFF.head) < size)
        {
            usleep(40000);
            continue;
        }

        iRet = ias_ad_read_pcm(buff, size);
        if(iRet != RETURN_OK)
        {
            continue;
        }

        stFrame.u32DataNum  = size >> 1;
        stFrame.u32DataType = IAS_AD_DATA_U16;
        stFrame.u32Freq     = 8000;
        stFrame.pVData[0]   = (void *)buff;
        stFrame.u32FrameId  =  u32FrameId++;
        iRet = NI_IAS_AD_Run(adCtxParam.OdChn, &stFrame, 20000);
        if(iRet != RETURN_OK)
        {
            PRINT_ERROR("call NI_IAS_AD_Run 0x%x\n", iRet);
			continue;
        }

        iRet = NI_IAS_AD_GetResult(adCtxParam.OdChn, &stResult, 20000);
        if(iRet != RETURN_OK)
        {
            PRINT_ERROR("call NI_IAS_AD_GetResult 0x%x\n", iRet);
            continue;
        }

        iRet = NI_IAS_AD_ReleaseResult(adCtxParam.OdChn, &stResult);
        if(iRet != RETURN_OK)
        {
            PRINT_ERROR("call NI_IAS_AD_ReleaseResult 0x%x\n", iRet);
            continue;
        }
    }

    free(buff);
    pthread_exit(0);
}

/* ========================================================================== 
* 函数名 : _ias_ad_init
* 描  述 : IAS检测初始化
* 输  入 : 
*          input    -i    OdChn          通道数
*          input    -i    pstCreateParam 参数结构体
* 输  出 : 无
* 返  回 : 状态码 
========================================================================== */
static int _ias_ad_init(IAS_ALG_CHN OdChn, IAS_AD_INIT_PARAM_S *pstCreateParam)
{
    int iRet;
    iRet = _ias_ad_get_model_file(pstCreateParam);
    if (iRet != 0)
    {
        PRINT_ERROR("call _ias_ad_get_model_file error 0x%x\n", iRet);
        return iRet;
    }
    pstCreateParam->stFunc.pfn_ias_ad_result = NULL;

    iRet = NI_IAS_AD_Init(OdChn, pstCreateParam);
    if (0 != iRet)
    {
        PRINT_ERROR("call NI_IAS_AD_Init 0x%x\n", iRet);
        return iRet;
    }

    return 0;
}

/* ========================================================================== 
* 函数名 : _ias_ad_deInit
* 描  述 : IAS检测反初始化
* 输  入 : 
*          input    -i    OdChn          通道数
*          input    -i    pstCreateParam 参数结构体
* 输  出 : 无
* 返  回 : 状态码 
========================================================================== */
static int _ias_ad_deInit(IAS_ALG_CHN          OdChn, 
                          IAS_AD_INIT_PARAM_S *pstCreateParam)
{
    int iRet;
    
    iRet = NI_IAS_AD_DeInit(OdChn);
    if (0 != iRet)
    {
        PRINT_ERROR("call NI_IAS_AD_DeInit 0x%x\n", iRet);
    }

    iRet = _ias_ad_release_model_file(pstCreateParam);
    if (0 != iRet)
    {
        PRINT_ERROR("_ias_ad_release_model_file 0x%x\n", iRet);
    }

    return 0;
}

int CryingDetectCreate(void)
{
    int iRet = 0;
    int OdChn = 2;      //OD通道号写死为2，后续统一优化

    memset(&adCtxParam, 0 , sizeof(IAS_AD_TEST_CTX_S));
    memset(&adgstCreateParam, 0 , sizeof(IAS_AD_INIT_PARAM_S));
    adgstCreateParam.stPipeLine.enPipeMode = IAS_PIPELINE_MODE_SP;
    adgstCreateParam.stPipeLine.u32BufNum = 1;
    adgstCreateParam.stPipeLine.bSerial = TRUE;
    adgstCreateParam.stAlgCfg.stAdParam.u16ScoreThr = 500;
    adgstCreateParam.stAlgCfg.stAdAlarmParam.enAlarmType = IAS_AD_ALARM_CONTINUED_TIME;
    adgstCreateParam.stAlgCfg.stAdAlarmParam.stAdAlarmTimeParam.u32AlarmContinueTime = 6000;
    adgstCreateParam.stAlgCfg.u32SampleFreq = 8000;
    adgstCreateParam.stAlgCfg.enAlarmModeType = IAS_AD_ALARM_ACCURACY_MODE;
    adgstCreateParam.stAlgCfg.u32Gain = 0;
    adgstCreateParam.stProcCfg.enProcMode = IAS_PROC_MODE_MULTI_TASK;

    iRet = _ias_ad_init(OdChn, &adgstCreateParam);
    if (iRet != 0)
    {
        PRINT_ERROR("call _ias_ad_init error 0x%x\n", iRet);
        return iRet;
    }

    memset(&AD_PCM_BUFF, 0, sizeof(RING_BUFF));
    AD_PCM_BUFF.head = 0;
    AD_PCM_BUFF.tail = 0;
    AD_PCM_BUFF.tatol_len = AUDIO_PCM_BUFFER_SIZE;
    if (NULL == AD_PCM_BUFF.buf)
    {
        AD_PCM_BUFF.buf = malloc(AD_PCM_BUFF.tatol_len);
        if (NULL == AD_PCM_BUFF.buf)
        {
            PRINT_ERROR("AD_PCM_BUFF.buf malloc failed!\n");
            return -1;
        }
    }

    adCtxParam.OdChn = OdChn;
    adCtxParam.bRunGetOdResult = TRUE;
    adCtxParam.bDetectEnable = FALSE;
    pthread_create(&stOdResultThread, 0, _ias_ad_get_result_process, NULL);

    SystemAsh("rm -rf %s", IAS_AD_MODEL_FILE);

    return 0;
}

int CryingDetectGetResult(int channel, CRY_DETECT_RESULT *pResult)
{
    pResult->enCryStatus = stResult.stResult.enCryStatus;
    pResult->u16Score = stResult.stResult.u16Score;

    if(stResult.stResult.enCryStatus == 1 && stResult.stResult.u16Score >= 999 && adCtxParam.bDetectEnable == TRUE)
    //if(stResult.stResult.u16Score >= 999 && adCtxParam.bDetectEnable == TRUE)//为缩短哭声检测时间，改为只看得分判断是否检测到哭声
    {
        return 0;
    }
    return -1;
}

int CryingDetectSetParameter(int channel, CRY_DETECT_PARAM *pParam)
{
    int iRet = 0;
    int OdChn = 2;      //OD通道号写死为2，后续统一优化
    IAS_PARAMS_S pastArr;
    IAS_ALG_AD_PARAM_LIST_S adParamList;
    memset(&pastArr, 0, sizeof(IAS_PARAMS_S));
    memset(&adParamList, 0, sizeof(IAS_ALG_AD_PARAM_LIST_S));

    adCtxParam.bDetectEnable = pParam->Enable;

    pastArr.s32ByteSize = 4;
    pastArr.s32Type = IAS_AD_PARAM_AD_SCORE_THRE;
    pastArr.as32Rev[0] = 550 - pParam->Level;
    adParamList.u32ParamNum = 1;
    adParamList.pastArr     = &pastArr;

    iRet = NI_IAS_AD_SetParams(OdChn, (NI_VOID *)(&adParamList));
    if (RETURN_OK != iRet)
    {
        PRINT_ERROR("[ERROR]: NI_IAS_AD_SetParams failed, ret = %x\n", iRet);
    }

    return iRet;
}

int CryingDetectDestroy(void)
{
    int iRet;
    int OdChn = 2;      //OD通道号写死为2，后续统一优化
    IAS_AD_INIT_PARAM_S *pstCreateParam = &adgstCreateParam;

    adCtxParam.bRunGetOdResult = FALSE;
    pthread_join(stOdResultThread, 0);

    free(AD_PCM_BUFF.buf);

    iRet = _ias_ad_deInit(OdChn, pstCreateParam);
    if (0 != iRet)
    {
        PRINT_ERROR("call NI_IAS_AD_DeInit 0x%x\n", iRet);
    }

    return 0;
}