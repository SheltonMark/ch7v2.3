#include "TdCommon.h"

int MD_CD_check(void)
{
	int ret;
	VD_CHN VdChn = 0;
	VPS_GRP VpsGrp = 0;
	MD_RESULT_S stResult;
	VIDEO_FRAME_S stFrame;
	VPS_FRAME_INFO_S stVideoFrame;
	MD_CD_Check_p check = &GlobalDevice.md_cd_check;
	VideoInDevice_p pVideoInDevice = &GlobalDevice.VideoInDevice;
	int delay = pVideoInDevice->VDInfo[0].ViDstFps;

	pthread_t pthread;
	pid_t pid;
	pid_t tid;
	pid = getpid();
	tid = syscall(SYS_gettid);
	pthread = pthread_self();

	PRINT_ERROR("MD_CD_check thread pid %d tid %d thread id 0x%x\n", pid, tid, pthread);

	while(check->md_cd_is_running)
	{
		if(check->md_cd_state != 0)
		{
			memset(&stFrame, 0, sizeof(VIDEO_FRAME_S));
			memset(&stVideoFrame, 0, sizeof(VPS_FRAME_INFO_S));

			ret = NI_MDK_VPS_GetFrame(VpsGrp, VPS_CHN_VD, &stVideoFrame, 1000);
			if (RETURN_OK != ret)
			{
				pthread_mutex_lock(&check->MD_CD_lock);
				check->cd_result = 0;
				memset(check->md_data, 0, stResult.stMdData.astAomResult[0].u32Size + 1);
				pthread_mutex_unlock(&check->MD_CD_lock);
				usleep(delay * 2 * 1000);
				continue;
			}

			stFrame = stVideoFrame.astVideoFrame[0];

			ret = NI_MDK_MD_DetectFrame(VdChn, &stFrame, &stResult, 1000);
			if (RETURN_OK != ret)
			{
				pthread_mutex_lock(&check->MD_CD_lock);
				check->cd_result = 0;
				memset(check->md_data, 0, stResult.stMdData.astAomResult[0].u32Size + 1);
				pthread_mutex_unlock(&check->MD_CD_lock);
				ret = NI_MDK_VPS_ReleaseFrame(VpsGrp, VPS_CHN_VD, &stVideoFrame);
				if (RETURN_OK != ret)
				{
					PRINT_ERROR("NI_MDK_VPS_ReleaseFrame failed with: %d\n", ret);
				}
				usleep(delay * 2 * 1000);
				continue;
			}

			pthread_mutex_lock(&check->MD_CD_lock);

			memset(check->md_data, 0, stResult.stMdData.astAomResult[0].u32Size + 1);

			if (stResult.bMdDataExist && stResult.stMdData.bAomResultValid)
			{
				memcpy(check->md_data, (void *)stResult.stMdData.astAomResult[0].pVirAddr, stResult.stMdData.astAomResult[0].u32Size);
			}

			if (stResult.bBdDataExist)
			{
				check->cd_result = stResult.stBdData.bBdDetect;
			}
			else
			{
				check->cd_result = 0;
			}

			pthread_mutex_unlock(&check->MD_CD_lock);

			ret = NI_MDK_VPS_ReleaseFrame(VpsGrp, VPS_CHN_VD, &stVideoFrame);
			if (RETURN_OK != ret)
			{
				PRINT_ERROR("NI_MDK_VPS_ReleaseFrame failed with: %d\n", ret);
			}

			usleep(delay * 2 * 1000);
		}
		else
		{
			usleep((delay + 1000) * 1000);
		}
	}
	check->request_to_stop = 1;
	return 0;
}

