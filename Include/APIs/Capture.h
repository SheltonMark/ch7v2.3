//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang Dahua Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:	
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//


#ifndef __CAPTURE_API_H__
#define __CAPTURE_API_H__

//#include "base.h"
#include "System.h"
#include "Memory.h"
#include "sdk_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

	/// \defgroup CaptureAPI API Capture
	/// 多路多码流音视频编码接口，依赖于Memory接口。
	///	\n 调用流程图:
	/// \code
	///    ============================================
	///                   	|                            
	///             CaptureCreate             
	///       +-----------|------------------+         
	///       |     CaptureSetVstd         CaptureGetChannels
	///		  |				|                CaptureGetDspInfo,
	///       |   	VideoSetFormat		VideoGetCaps,
	///    	  |	CaptureSetPacketPolicy	VideoGetExtCaps      
	///       |           	|           	CaptureSetTime   	 
	///       |     CaptureStart     		VideoSetTitle   
	///       | +---------|          		VideoSetCover   
	///       | |  CaptureGetBuffer  		CaptureSetVolume  
	///       | | CaptureForceIFrame 	CaptureSynchronize
	///       | |  CaptureGetVolume          |         
	///       | +---------|                  |         
	///       |      CaptureStop             |         
	///       +-----------|------------------+         
	///           CaptureDestroy                       
	///                   |                            
	///    ============================================
#define MAX_CAPTURE_CHANNEL	32+2

#define ZLAV_HEADER		(0x56414c5a)
#define ZLAV_TAIL		(0x76616c7a)

#define MAX_VENC_VB_POOL	4

#define	DEF_ENC_POWER		  (4 * 352 * 288 * 25 + 25 * 176 * 144)
#define DEF_ENC_POWER_HALF_HD1080	(960*1080*30)
#define DEF_ENC_POWER_HALF_HD1080_15 (960*1080*15)
#define	DEF_ENC_POWER_HD1080	(1920 * 1080 * 30 )
#define	DEF_ENC_POWER_HD1080_15	(1920 * 1080 * 15 )
#define	DEF_ENC_POWER_HD1080_12	(1920 * 1080 * 12 )
#define	DEF_ENC_POWER_QXGA_18	(2048 * 1536 * 15 )
#define	DEF_ENC_POWER_5M_12	(2560 * 1920 * 12 )
#define DEF_ENC_POWER_4M_15	(2688 * 1520 * 15 )
#define	DEF_ENC_POWER_HD720 	(1280 * 720 * 30 )
#define DEF_ENC_POWER_HD720_15 (1280 * 720 * 15 )
#define	DEF_ENC_POWER_D1	(4 * 352 * 288 * 25 )
#define	DEF_ENC_POWER_D1_12FPS	(4 * 352 * 288 * 12 )
#define	DEF_ENC_POWER_D1_15	   (704 * 480 * 15)
#define	DEF_ENC_POWER_D1_30	   (704 * 480 * 30)
#define	DEF_ENC_POWER_960H	(960 * 576 * 25 )
#define	DEF_ENC_POWER_960H_12 (960 * 480 * 12 )
#define	DEF_ENC_POWER_CIF   (1 * 352 * 288 * 25 )
#define	DEF_ENC_POWER_HD1	(2 * 352 * 288 * 25 )
#define	DEF_ENC_POWER_QCIF	(176 * 144 *25)
#define	DEF_ENC_POWER_QCIF_12FPS	(176 * 144 * 12)
#define	DEF_ENC_POWER_3M_18	(1920 * 1536 * 15 )
#define DEF_ENC_POWER_UHD4K_15 (3840 * 2160 * 15)
#define DEF_ENC_POWER_4M_QHD_30 (2560 * 1440 * 30)

#define	DEF_SUPPORT_CHL	(1)

#define TD_EINVAL 		(-10001)
#define NOTSUPPORT  	(-10002)
#define GETRC_ERR		(-10003)
#define SETRC_ERR   	(-10004)
#define GETCHN_ERR		(-10005)

typedef struct CaptureImageQuality
{
	unsigned int ImaxQP;
	unsigned int IminQP;
	unsigned int PmaxQP;
	unsigned int PminQP;
} CaptureImageQuality_t;

typedef enum H264E_REF_TYPE_E {
    BASE_IDRSLICE = 0,                              /* the Idr frame at Base layer*/
    BASE_PSLICE_REFTOIDR,                           /* the P frame at Base layer, referenced by other frames at Base layer and reference to Idr frame*/
    BASE_PSLICE_REFBYBASE,                          /* the P frame at Base layer, referenced by other frames at Base layer*/
    BASE_PSLICE_REFBYENHANCE,                       /* the P frame at Base layer, referenced by other frames at Enhance layer*/
    ENHANCE_PSLICE_REFBYENHANCE,                    /* the P frame at Enhance layer, referenced by other frames at Enhance layer*/
    ENHANCE_PSLICE_NOTFORREF,                       /* the P frame at Enhance layer ,not referenced*/
    ENHANCE_PSLICE_BUTT
} H264E_REF_TYPE_E;

typedef enum AV_SLICE_TYPE_e
{
	AV_SLICE_TYPE_V_H264_I =0, 
	AV_SLICE_TYPE_V_H264_P,
	AV_SLICE_TYPE_V_JPEG,
	AV_SLICE_TYPE_A,
	AV_SLICE_TYPE_V_WATER,
	AV_SLICE_TYPE_V_H265_I, 
	AV_SLICE_TYPE_V_H265_P,
	AV_SLICE_TYPE_BUT,
}AV_SLICE_TYPE_e;

typedef enum  PACK_SubType_e
{
    PACK_GENERAL_PSLICE = 0,               // 为普通P帧
    PACK_BASE_REFTOIDR = 1, //虚拟I帧
    PACK_BASE_PSLICE_REFBYBASE, //b层P帧，被b层的其它P帧参考
    PACK_BASE_PSLICE_REFBYENHANCE,  //b层P帧，被e层P帧参考
    PACK_ENHANCE_PSLICE_REFBYENHANCE,  //e层P帧，被e层的其它P帧参考
    PACK_ENHANCE_PSLICE_NOTFORREF,    //e层P帧，不被参考
    PACK_ENHANCE_PSLICE_BUTT,
}PACK_SubType_e;

typedef struct PRI_STREAM_PACK_s
{
	unsigned char *addr;
	unsigned int offset;
	unsigned int length;
	unsigned int unusedLen;
}PRI_STREAM_PACK_t, *PRI_STREAM_PACK_p;

typedef struct AV_SLICE_HEADER_COM_s
{
	unsigned int avHead;//帧头标志
	
	unsigned char avSliceType;//帧类型 FD
	unsigned char avSubSliceType;//子帧类型 0
	unsigned char avChnNum;//通道号
	unsigned char resv0;//保留//相对帧数
	
	unsigned int avSequence;//序列号
	
	unsigned int avSliceLength;//帧长度 = 帧头 + 码流数据 + 帧尾
	
	unsigned int ClockTimes;//墙上时钟 s 
	
	unsigned short msTimes;//时间戳 ms
	unsigned char expandLength;//扩展长度
	unsigned char CheckSum;//数据头累加和校验
}AV_SLICE_HEADER_COM_t, *AV_SLICE_HEADER_COM_p;

typedef struct Water_Verify_s
{
    unsigned char type;        //0X88 表示校验信息
    unsigned char verify_result[4];      //校验结果 
    unsigned char reserved1;   //保留，暂时没用
    unsigned char reserved2;   //保留，暂时没用
    unsigned char verify_type;  //校验类型， 目前为2： CRC32   
}Water_Verify_t, *Water_Verify_p;

typedef enum Waterverify_Type_e
{
	VERIFY_TYPE_CRCSUM = 0x0,
	VERIFY_TYPE_CRCXOR,
	VERIFY_TYPE_CRC32 
}Waterverify_Type_e;

typedef struct H264_I_SLICE_HEADERS_s
{
	unsigned char PicMask;//图像标志位
	unsigned char PicInterlace;//接交错
	unsigned char PicWidth;//图像宽度 8个像素点单位
	unsigned char PicHeight;//图像高度 8个像素点单位
	
	unsigned char VStreamMask;//码流类型标志
	unsigned char VStreamRev;//保留
	unsigned char VStreamType;//码流类型 MPEG H264 HI_H264 等类型
	unsigned char VFps;//帧率
	Water_Verify_t WaterVerify;	 
}H264_I_SLICE_HEADERS_t, *H264_I_SLICE_HEADERS_p;

typedef struct H264_P_SLICE_HEADERS_s
{
	unsigned char PicMask;//图像标志位
	unsigned char PicInterlace;//接交错
	unsigned char PicWidth;//图像宽度 8个像素点单位
	unsigned char PicHeight;//图像高度 8个像素点单位
	
	unsigned char VStreamMask;//码流类型标志
	unsigned char VStreamRev;//保留
	unsigned char VStreamType;//码流类型 MPEG H264 HI_H264 等类型
	unsigned char VFps;//帧率
	Water_Verify_t WaterVerify;
}H264_P_SLICE_HEADERS_t, *H264_P_SLICE_HEADERS_p;

typedef struct A_SLICE_HEADERS_s
{
	unsigned char AFomatMask;
	unsigned char AenSoundmode;
	unsigned char AenPayloadType;
	unsigned char AenSamplerate;
}A_SLICE_HEADERS_t, *A_SLICE_HEADERS_p;

#define AUDIOPACKAGE  AUDIO_ONE_FRAME_SIZE+28+8+1 //28 = sizeof(AV_SLICE_HEADER_COM_t) ; 8 = sizeof(AV_SLICE_TAILS_t)
#define AUDIOCHN  32+1
#define AUDIOSTEAM  2  
typedef struct A_PACKAGE_Chn
{
	unsigned char PackageBuf[AUDIOPACKAGE];
	unsigned int   PackagLen;
	unsigned char PackagFlag;
	
}A_PACKAGE_Chn_t,*A_PACKAGE_Chn_p;

typedef struct A_PACKAGE_Steam
{
	A_PACKAGE_Chn_t Package_Chn[AUDIOCHN];
}A_PACKAGE_Steam_t,*A_PACKAGE_Steam_p;

typedef struct A_PACKAGE_s
{

	A_PACKAGE_Steam_t Package_Steam[2];
	unsigned char AudioinFlag;
	
}A_PACKAGE_t, *A_PACKAGE_p;


typedef struct J_SLICE_HEADERS_s
{
	unsigned char VStreamMask;//图像标志位
	unsigned char VStreamRev;//保留
	unsigned char VStreamType;//码流类型 MPEG H264 HI_H264 等类型
	unsigned char VFps;//帧率
	
	unsigned char PicMask;//图像标志位
	unsigned char PicInterlace;//接交错
	unsigned char Rev1;//保留
	unsigned char Rev2;//保留
	
	unsigned char PicWidth[2];//图像宽
	unsigned char PicHeight[2];//图像高
	Water_Verify_t WaterVerify;	 
	
}J_SLICE_HEADERS_t, *J_SLICE_HEADERS_p;

typedef struct DEV_IN_SLICE_HEADERS_s
{
	unsigned char PicMask;//图像标志位
	unsigned char PicInterlace;//接交错
	unsigned char PicWidth;//图像宽度 8个像素点单位
	unsigned char PicHeight;//图像高度 8个像素点单位
	
	unsigned char VStreamMask;//码流类型标志
	unsigned char VStreamRev;//保留
	unsigned char VStreamType;//码流类型 MPEG H264 HI_H264 等类型
	unsigned char VFps;//帧率
	Water_Verify_t WaterVerify;	 
	
	unsigned char ViPicMask;//图锟斤拷锟街疚?
	unsigned char ViPicWidth;//图锟斤拷锟?
	unsigned char ViPicHeight;//图锟斤拷锟?
	unsigned char ViFps;//帧锟斤拷
	unsigned char AudioType;
	unsigned char Rev1;//锟斤拷锟斤拷
	unsigned char Rev2;//锟斤拷锟斤拷
	unsigned char Rev3;//锟斤拷锟斤拷
	
}DEV_IN_SLICE_HEADERS_t, *DEV_IN_SLICE_HEADERS_p;

typedef union AV_SLICE_HEADER_u
{
	H264_I_SLICE_HEADERS_t H264ISlice;
	H264_P_SLICE_HEADERS_t H264PSlice;
	A_SLICE_HEADERS_t ASlice;
	J_SLICE_HEADERS_t JpgeSlice;
	DEV_IN_SLICE_HEADERS_t DevInSlice;
	Water_Verify_t WaterVerify;
}AV_SLICE_HEADER_u;

typedef struct AV_SLICE_HEADER_s
{
	AV_SLICE_HEADER_COM_t ComHeader;
	AV_SLICE_HEADER_u ExternHeader;
	AV_SLICE_TYPE_e Type;
}AV_SLICE_HEADER_t, *AV_SLICE_HEADER_p;

typedef struct AV_SLICE_TAILS_s
{
	unsigned int avTail;
	unsigned int avSliceLength; 
}AV_SLICE_TAILS_t,*AV_SLICE_TAILS_p;

typedef struct SLICE_LOCATION_s
{
	char FrameType;					/*!< 帧类型 */// 0 : P 帧  1:  I 帧3 : 音频帧 4:JPEG	  
	char FrameFlag;					/*!< 帧头尾的标识 */
	unsigned short FramePos;					/*!< 帧的起始位置 */
	unsigned int FrameLength;				/*!< 帧的长度，可能跨块 */
	unsigned short DataLength;				/*!< 帧在本块中的长度 */
	char Reserve[2];				/*!< 保留 */
}SLICE_LOCATION_t;

typedef struct SLINCE_DATE_TIME_s					                        
{
	unsigned int second	:6;		//	秒	0-59
	unsigned int  minute :6;		//	分	0-59
	unsigned int  hour :5;		//	时	0-23
	unsigned int  day :5;		//	日	1-31
	unsigned int  month :4;		//	月	1-12
	unsigned int  year :6;		//	年	2000-2063	
}SLINCE_DATE_TIME_t;

typedef enum capture_channel_t {
	CHL_MAIN_T = 0,		///< 主通道	－	主码流1 
	CHL_2END_T = 1,		///< 辅通道	－	出辅码流2 
	CHL_3IRD_T = 2,		///< 辅通道	－	出辅码流3 
	CHL_4RTH_T = 3,		///< 辅通道	－	出辅码流4 
	CHL_JPEG_T = 4,		///< 辅通道	－	出JPEG抓图
	CHL_IJPEG_T =5,		///< 辅通道	－	出IJPEG抓图
	CHL_PJPEG_T =6,	
	CHL_BITMAP_T = 7, //底层来到，做一转换 
	CHL_NET_TALK_T = 8, //网络通道对讲 
	CHL_MJPG_T = 9,//抓图码流使用MJPG编码 

	CHL_FUNCTION_NUM
}capture_channel_t;



/// 捕获开始类型
typedef enum capture_start_t {
	CAPTURE_START_VIDEO = 1,	///< 视频编码开。
	CAPTURE_START_AUDIO = 2		///< 音频编码开。
}capture_start_t;

typedef enum StreamType_e
{
	STREAM_TYPE_FIR = 0,
	STREAM_TYPE_SEC,
	STREAM_TYPE_THI,
	STREAM_TYPE_FOU,
	STREAM_TYPE_FIV,
	STREAM_TYPE_BUT,
}StreamType_e;

#define WATERMARK_MAX_LEN 128

typedef struct WaterData_s
{
	char Head[4];
	unsigned int crc;
	int buffer_length;
	char buffer[WATERMARK_MAX_LEN];
	int data_length;
}WaterData_t, *WaterData_p;

typedef enum exVencType_e
{
	ZERO_CHANNEL,
	IVS_FACE_CHANNEL,
	IVS_BZ_CHANNEL,
	PLAYBACK_SNAP_CHANNEL,
	BUTT_CHANNEL
}exVencType_e;

typedef enum CaptureCapPower_e
{
	CAPTURE_POWER_HD1080_30,
	CAPTURE_POWER_HD720_30,
	CAPTURE_POWER_960H_25,
	CAPTURE_POWER_960H_6,
	CAPTURE_POWER_960H_USER,
	CAPTURE_POWER_D1_25,
	CAPTURE_POWER_D1_6,
	CAPTURE_POWER_D1_USER,
	CAPTURE_POWER_BCIF_25,
	CAPTURE_POWER_BCIF_6,
	CAPTURE_POWER_BCIF_USER,
	CAPTURE_POWER_CIF_25,
	CAPTURE_POWER_CIF_6,
	CAPTURE_POWER_CIF_USER,
	CAPTURE_POWER_QCIF_25,
	CAPTURE_POWER_QCIF_6,
	CAPTURE_POWER_QCIF_USER,
	CAPTURE_POWER_HALF_HD1080,
	CAPTURE_POWER_HD1080_15,
	CAPTURE_POWER_QXGA_18,
	CAPTURE_POWER_5M_12,
	CAPTURE_POWER_HALF_HD1080_15,
	CAPTURE_POWER_HD720_15,
	CAPTURE_POWER_4M_15,
	CAPTURE_POWER_half4M_12,
	CAPTURE_POWER_4M_QHD_12,
	CAPTURE_POWER_3M_18,
	CAPTURE_POWER_UHD4K_15,
	CAPTURE_POWER_4M_QHD_30,
	CAPTURE_POWER_NHD_30,
	CAPTURE_POWER_1296P_30,
	CAPTURE_POWER_QVGA_25,
	CAPTURE_POWER_BUT,
}CaptureCapPower_e;

typedef enum OSD_TYPE
{
	OSD_TYPE_TIME,
	OSD_TYPE_TITLE,
	OSD_TYPE_LABLE1,
	OSD_TYPE_LABLE2,
} OSD_TYPE;

/// 捕获数据格式结构
typedef struct VPS_CHN_OUT_ATTR
{
	unsigned int OutWidth;		// VPS通道输出图像的宽
	unsigned int OutHeight;		// VPS通道输出图像的高
	unsigned int OutFps;		// VPS通道输出的帧率
	unsigned int EnPayLoad;		//VPS通道编码类型
} VPS_CHN_OUT_ATTR;

typedef struct venc_caps_info_s
{
	VIDEO_FORMAT fmtIfo[STREAM_TYPE_BUT];
	BYTE	perframe[STREAM_TYPE_BUT];
	SIZE_S  picsize[STREAM_TYPE_BUT];
	unsigned int EncChipPow[STREAM_TYPE_BUT];
}venc_caps_info_t,*venc_caps_info_p;

typedef struct CaptureFmt_s
{
	BYTE viframe;
	VIDEO_FORMAT fmt[STREAM_TYPE_BUT];
}CaptureFmt_t,*CaptureFmt_p;

typedef struct exVencFuncChn_s
{
	int Id;//逻辑通道，用于对应通道
	exVencType_e Type;//具体使用的功能
	int Venc;//具体使用的通道号
	VIDEO_FORMAT Format;//配置属性
	int m_VbPoolId;//使用的vb块号
	BOOL Enable;//通道状态
	int SrcDevId;//该编码通道使用的设备号
	int SrcChnId;//该编码通道使用的通道号
	int chfd;
}exVencFuncChn_t,*exVencFuncChn_p;

typedef struct VencUserVb_s
{
	BOOL Status;//使能USERVB标示
	int Count;
	PIC_RES_E Size[MAX_VENC_VB_POOL];
	int PoolId[MAX_VENC_VB_POOL];
}VencUserVb_t,*VencUserVb_p;

#define WATERMARK_MAX_LEN 128

typedef struct  CAPTURE_WATERMARK
{
	int  		key;			//0000表示是水印信息是字符类型，01WH表示二值灰度图象，W/H分别是宽度/8和高度/8， 以后其他类型再扩展。目前使用的类型key = 0；
	int			enable;			//水印使能，0 或1
	int 		length;			//0-128　缓冲区的长度
	char		data[WATERMARK_MAX_LEN];	//128字节的buffer指针	
	int 		reserved;		 
}CAPTURE_WATERMARK, *pCAPTURE_WATERMARK;			//水印信息结构

typedef enum CAP_PRIORITY_E
{
	 ENCODE_PRIORITY =0,
	 DECODE_PRIORITY = 1,
}CAP_PRIORITY_E;

typedef struct CaptureStreamInfo_s
{
    int reframe[MAX_CAPTURE_CHANNEL];        //跳帧参考模式
    unsigned int relFrames[MAX_CAPTURE_CHANNEL];     //相对前一个I帧的P帧序号
    unsigned int refIdrFrames[MAX_CAPTURE_CHANNEL];   //相对前一个I帧的虚拟I帧序号
}CaptureStreamInfo_t;

typedef struct StreamDevice_s
{
	int EncChannel;
	int VencStart;
	int AencStart;
	int NetTalkStart;
	int VencFd;
	int AencFd;
	HI_S32 AiFd;
	unsigned int VSequence;
	unsigned int ASequence;
	int VpssGrpIndex;   //编码使用的通道组索引
	int VpssChn;             //编码使用的VPSS通道号
	VIDEO_FORMAT Format;
	capture_channel_t CapChn;
	DWORD	SupportPowerMask;
	VIDEO_CHN_CAP Caps;
	CAPTURE_WATERMARK  WaterMark;
	struct channel_info EncChannel_info;
	pthread_mutex_t	mutex;
	
}StreamDevice_t, *StreamDevice_p;

typedef struct EncDevice_s
{
	int StreamCount;
	StreamDevice_t StreamDevice[STREAM_TYPE_BUT];
	DWORD SupportStream;
	
	//unsigned int	nMaxEncodePower;
	
}EncDevice_t, *EncDevice_p;

typedef struct CaptureDevice_s
{
	pthread_t VencPid[2];
	pthread_t AencPid;
	pthread_t VencCmdProcPid;
	int MaxFd;
	int EncCount;
//	int MaxCapCount;
	int ch4Pjpeg;
	int ch4Ijpeg;
	EncDevice_t EncDevice[MAX_CAPTURE_CHANNEL];

	unsigned char		PlayFlg;// 1 应用不能调用接口调节当前的编码配置
	unsigned int 		TotalCaps;//总能力
	unsigned int 		userCaps;//使用的能力
	unsigned int		playGetCaps;//回放需要的能力
	venc_caps_info_t	capsInfo[MAX_CAPTURE_CHANNEL];//每个通道的能力

	CAP_PRIORITY_E cappriority; //编解码优先

	unsigned char exifBuf[512];
	unsigned short exifLen;
	unsigned int exifmdfoff[5];
	unsigned int maccal;//用MAC计算一个随机数

	int captureRealChn[MAX_CAPTURE_CHANNEL];
	CaptureFmt_t capfmtChn[MAX_CAPTURE_CHANNEL];
	int exCnt;//功能通道数量,目前限制为4个
	exVencFuncChn_t exVencFun[8];
	int 		IvsVencCnt;//总能力
	exVencFuncChn_t 		IvsVenc[4];//使用的能力
	DWORD FontDataDstPhy;
	BYTE *FontDataDst;
	unsigned int FontDataDstLen;
	DWORD RGBResizePhy;
	BYTE *RGBResizeBuf;
	unsigned int RGBResizeLen;
	sem_t Captovdec;
	sem_t Vdectocap;
	MEMORY_BLOCK Block4BMP;	
	SIZE_S PlaySnapSize[MAX_CAPTURE_CHANNEL];
	int VeduCnt;//编码器的个数，手册获取
	VencUserVb_t VencVb;//私有VB结构体
	CaptureStreamInfo_t VencStreamInfo[STREAM_TYPE_BUT];
	pthread_t NOVPSSPid;
	unsigned int 		ChangeBind;//锟角凤拷锟斤拷要锟睫改绑定癸拷系

	unsigned int Rotate;
	unsigned int MirrorAndflip;
//	VIDEO_FRAME_INFO_S jpgFrameInfo;
#if 0	
	int VencEnable;
	int AencEnable;
	int VencStart;
	int AencStart;

	int JpegEnable;
	int JpegStart;	

	
	int D1_Count;
	VIDEO_EXT_STREAM extStream;
	
	
	
	VIDEO_FORMAT CaptureFormat[16 * 3];
	unsigned int	nMaxEncodePower[MAX_VIDEO_IN_CHANNEL];
	
#endif	
}CaptureDevice_t, *CaptureDevice_p;

extern INT SystemAsh(const char *fmt, ...);

/// 得到捕获的通道数目。不包括语音捕获的通道，语音捕获的通道是最大捕获通道号加1。
/// 
/// \return捕获的通道数目。
int CaptureGetChannels(void);

/// 创建捕获设备
/// 
/// \param [in] channel 通道号。
/// \retval 0 创建失败
/// \retval 0 创建成功
int CaptureCreate(int channel);


/// 销毁捕获设备
/// 
/// \param [in] channel 通道号。
/// \retval 0 销毁失败
/// \retval 0 销毁成功
int CaptureDestroy(int channel);

/// 强制编码器产生I帧。 
/// 
/// \param [in] channel 通道号。
/// \param [in] dwType
/// \retval 0  设置成功。
/// \retval 0  设置失败。
int CaptureForceIFrame(int  channel, DWORD dwType);

/// 抓取YUV数据
///
///
/// \param [in]  len 	 picData申请的内存大小
/// \param [out] picData YUV数据
/// \param [out] picSize YUV图片的分辨率
/// \retval 0  抓取成功
/// \retval 0  抓取失败。
int CaptureGetYuvPic(char *picData, unsigned int len, SIZE_S *picSize);

/// 开始编码。在视频制式、捕获选项、图像质量、帧率、码流控制方式、分辨率、编码
/// 模式等参数被改变时，应用程序会停止捕获，然后重新开始捕获。
/// 
/// \param [in] channel 通道号。
/// \param [in] dwType 参数表示启动捕获的选项，是capture_start_t类型的值的组合。
/// \retval 0  开始成功
/// \retval 0  开始失败
int CaptureStart(int  channel, DWORD dwType);


/// 停止编码，当前操作立即返回。
/// 
/// \param [in]  channel 通道号。
/// \param [in]  dwType
/// \retval 0  停止成功
/// \retval 0  停止失败
int CaptureStop(int  channel, DWORD dwType);

void audio_lock_enter();

void audio_lock_leave();

int capture_video_stream_start(void);
int capture_video_stream_stop(void);
int capture_init(void);

void capture_stop_stream_proc();

#ifdef __cplusplus
}
#endif

#endif //__CAPTURE_API_H__

