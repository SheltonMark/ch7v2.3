//create by zhujc 20140318
//主要封装一些不属于独立功能模块的接口，对应的声明请放在MediaCom.h内
#include "TdCommon.h"
#include "toolconfig.h"
#include "video_osd.h"

static pthread_t g_pid = 0;
static DWORD g_flag = 0;

extern INT SystemAsh(const char * fmt,...);

void* _get_memAlloc_proc(void* p)
{
	PRINT_TRACE("qijiannan MediaGetMemAlloc upgrade test\n");
	image_smart_deinit();
	PRINT_TRACE("qijiannan MediaGetMemAlloc upgrade test\n");
	// 销毁YUV数据管理通道
	VPS_GRP VpsGrp = 0;
	VPS_CHN_E VpsChn = VPS_CHN_VIRTIAS0;
	NI_MDK_VPS_DisableChn(VpsGrp, VpsChn);
	PRINT_TRACE("qijiannan MediaGetMemAlloc upgrade test\n");
	CaptureDevice_p pCaptureDevice = &GlobalDevice.CaptureDevice;
	int i = 0;
	int vencChannel = 0;
	int channel = 0;
	for(i = 0; i < pCaptureDevice->EncCount; i++)
	{
		for (vencChannel = 0; vencChannel < pCaptureDevice->EncDevice[i].StreamCount; vencChannel++)
		{
			channel = pCaptureDevice->EncDevice[i].StreamDevice[vencChannel].EncChannel_info.channel;
			SDK_COMM_VENC_Stop(channel);
			if (STREAM_TYPE_THI == vencChannel)
			{
				vencChannel = VPS_CHN_IAS0;
			}
			SDK_COMM_VPS_Stop(0,channel);
		}
	}
	PRINT_TRACE("qijiannan MediaGetMemAlloc upgrade test\n");

	SDK_COMM_VPS_Stop(0, VPS_CHN_VD);
	PRINT_TRACE("qijiannan MediaGetMemAlloc upgrade test\n");
	
	VideoOSD_Destroy();
	PRINT_TRACE("qijiannan MediaGetMemAlloc upgrade test\n");
	VideoOSD_DestroyLogo();
	PRINT_TRACE("qijiannan MediaGetMemAlloc upgrade test\n");
	AudioDestory();
	PRINT_TRACE("qijiannan MediaGetMemAlloc upgrade test\n");
	capture_stop_stream_proc();
	PRINT_TRACE("qijiannan MediaGetMemAlloc upgrade test\n");
	SDK_COMM_ISP_Stop(0);
	PRINT_TRACE("qijiannan MediaGetMemAlloc upgrade test\n");
	SDK_COMM_VI_Stop(0);
	PRINT_TRACE("qijiannan MediaGetMemAlloc upgrade test\n");
	SDK_COMM_SYS_Exit();
	PRINT_TRACE("qijiannan MediaGetMemAlloc upgrade test\n");
	SDK_COMM_SYS_Init();
	PRINT_TRACE("qijiannan MediaGetMemAlloc upgrade test\n");
	SystemAsh("cat /proc/media-mem");
	PRINT_TRACE("qijiannan MediaGetMemAlloc upgrade test\n");


	g_flag = 1;
	
	return NULL;	
}

int MediaGet960HCaps()
{
	return 1;
}

int MediaDeleteMmz()
{
	return 0;
}

int MediaPrepareFreeMem()
{
	g_flag = 0;
	
	if(pthread_create(&g_pid,0,_get_memAlloc_proc,NULL) != 0)
	{
		PRINT_ERROR("MediaPrepareFreeMem pthread_create faield");
		return FAILED;
	}
	
	DWORD count = 0;
	while(g_flag == 0 && count++ < 90)
	{
		sleep(1);
	}
	
	if(g_flag == 0)
	{
		PRINT_ERROR("MediaGetMemAlloc time ount");
		return FAILED;
	}
	
	return SUCCESS;
}

void* MediaGetMmzMem(unsigned int size)
{
	NI_U32 u32PhyAddr = 0;
	unsigned char* pAddr = NULL;
	NI_S32 ret = NI_MDK_SYS_MmzAlloc(&u32PhyAddr,(void**)&pAddr,size,"upgrade","anonymous");
	if(ret != NI_SUCCESS)
	{
		PRINT_ERROR("MmzMalloc NI_MDK_SYS_MmzAlloc size %u ret %d failed",size,ret);
		return NULL;
	}

	return pAddr;
}

//获得一些能力判断的接口
int MediaGetConfigCaps(MediaCaps_t *caps)
{
	return 0;
}

//以下函数仅仅保证编译通过
int SlaveSysSetTime(SYSTEM_TIME time)
{
    return 0;
}
int SlaveShutDown()
{
    return 0;
}

int SlaveReboot()
{
	return 0;
}
int SlaveWDGCreate()
{
	return 0;
}
int SlaveWDGDestroy()
{
	return 0;
}
int SlaveWDGClear()
{
	return 0;
}


