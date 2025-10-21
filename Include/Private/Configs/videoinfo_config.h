#ifndef VEDIO_INFO_CONFIG_H_
#define VEDIO_INFO_CONFIG_H_

#define VIDEO_INFO_BATH "/opt/sav/Config/video_info"   //默认配置信息文件保存路径


typedef struct VideoInfoValue_s
{
	unsigned int Wd1_value;   //WD1 路数
	unsigned int D1_value;     //	D1路数
	unsigned int Cif_value;    //CIF 路数
	unsigned int Dec_value;  //解码路数
	unsigned int Resv[64];

}VideoInfoValue_t,*VideoInfoValue_p;


typedef struct VideoInfoConfig_s
{
	int Info_num;    //产品拥有的配置类型的个数
	VideoInfoValue_t  videoInfo[NAME_MAX_LENGTH];  //视频配置信息结构体数组
}VideoInfoConfig_t,*VideoInfoConfig_p;

/*******************************************************/
/*             获取产品的视频配置结构体              */
/*******************************************************/
CONST VideoInfoConfig_t*GetVideoInfo_Flbase(VOID);

/*******************************************************/
/*             获取默认视频信息并保存                   */
/*******************************************************/
//获取默认配置信息并保存到文件/opt/sav/Config/video_info中
CONST VideoInfoConfig_t*GetDefaultVideoInfo_Flbase(VOID);

//增加前端摄像机支持的制式
/*
VIDEO_IN_MODE配置开始
NET
960H
HALF_720P
1080P
VIDEO_IN_MODE配置结束
*/
typedef struct VideoInCamModeConfig {
 DWORD mix_mode; 
 
}VideoInCamModeCfg_t,*VideoInCamModeCfg_p;
CONST VideoInCamModeCfg_t *GetVideoInCamMode_Flbase(void);


#endif 
