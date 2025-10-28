//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang Dahua Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:	
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#ifndef __VIDEO_API_H__
#define __VIDEO_API_H__

#include "TdCommon.h"
#include "System.h"
#include "sdk_comm.h"

#define MAX_VIDEO_IN_CHANNEL        32+2 //16

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup VideoAPI API Video
/// 视频设置API。主要是视频输入参数，输出参数的设置，还有一些辅助的设置功能。
///	\n 调用流程图:
/// \code
///    =======================================================
///                |                            
///	      *Video2GetOutChannels       
///          VideoGetCaps            
///                |                            
///           VideoCreate                      
///      +---------|--------------+----------------------+     
///      |         |(In)          |(Out)                 |(Others)
///      |   VideoSetColor   VideoSwitch         VideoWriteRegister         
///      |   VideoSetCover VideoSetBkColor        VideoReadRegister
///      |   VideoSetVstd   VideoSetTVMargin        VideoMatrix
///      |         |        VideoSetTVColor      VideoSwitchTVMonitor
///      |         |       VideoSetTVAntiDither VideoSwitchVGAMonitor
///      |         |      VideoSetOutputAdapter          |
///      |         |      VideoSetOutputAdapter          |
///      |         |              |                      |
///      +---------|--------------+----------------------+          
///            VideoDestory   
///                |                            
///    =======================================================
/// \endcode
/// @{
#define MAX_SYS_CHANNEL 32
/// 通道类型
#define MAX_ENC_CHIP_NR (32)

/// 视频制式类型
enum video_standard_t {
	VIDEO_STANDARD_PAL = 0,	///< PAL制式。
	VIDEO_STANDARD_NTSC,	///< NTSC制式。
	VIDEO_STANDARD_SECAM	///< SECAM制式。
};

typedef struct VIDEO_RECT
{
	WORD	left;			///< 左上角x坐标。
	WORD	top;			///< 左上角y坐标。
	WORD	right;			///< 右下角x坐标。
	WORD	bottom;			///< 右下角y坐标。
}VIDEO_RECT;

/// 视频覆盖参数
typedef struct VIDEO_COVER_PARAM
{
	/// 覆盖的矩形区域，采用相对坐标体系。
	VIDEO_RECT	rect;

	/// 被覆盖区域显示的颜色。16进制表示为0x00BBGGRR，最高字节保留。
	DWORD		color;

	/// 区域操作的类型， 取video_cover_t类型的值。窗口和全屏操作应该是独立的， 
	/// 即窗口内的操作不影响全屏，全屏内的操作也不影响窗口。 清除区域时，其他
	/// 参数被忽略。
	int			enable;
}VIDEO_COVER_PARAM;

/// 视频压缩格式类型
typedef enum video_comp_t {
	VIDEO_COMP_DIVX_MPEG4,	///< DIVX MPEG4。
	VIDEO_COMP_MS_MPEG4,		///< MS MPEG4。
	VIDEO_COMP_MPEG2,			///< MPEG2。
	VIDEO_COMP_MPEG1,			///< MPEG1。
	VIDEO_COMP_H263,			///< H.263
	VIDEO_COMP_MJPG,			///< MJPG
	VIDEO_COMP_FCC_MPEG4,		///< FCC MPEG4
	VIDEO_COMP_H264,			///< H.264
	VIDEO_COMP_JPEG,			///< JPEG
	VIDEO_COMP_H265,			///< H.265
	VIDEO_COMP_H264_IMPROVE,	///< H.264+
	VIDEO_COMP_H265_IMPROVE,  	///< H.265+ 
	VIDEO_COMP_NR				///< 枚举的压缩标准数目。
}video_comp_t;

/// H.264编码Profile
typedef enum video_h264profile_t{
	VIDEO_264PROFILE_BASELINE = 0,	///< Baseline Profile(BP)
	VIDEO_264PROFILE_MAIN,			///< Main Profile(MP)
	VIDEO_264PROFILE_HIGH,			///< High Profile(HP)
	VIDEO_264PROFILE_EXTENDED,		///< Extended Profile(XP)
	VIDEO_264PROFILE_NR				///< Profile个数
}video_h264profile_t;

/// H.265编码Profile
typedef enum video_h265profile_t{
	VIDEO_265PROFILE_MAIN = 0,		///< Main profile
	VIDEO_265PROFILE_MAIN_10BIT,		///< Main 10 profile
	VIDEO_265PROFILE_MAIN_STILL_PIC,	///< Main Still Picture profile
	VIDEO_PROFILE_NR					///< Profile个数
}video_h265profile_t;

/// 码流控制模式类型
typedef enum video_brc_t {
	VIDEO_BRC_CBR,			///< 固定码流。
	VIDEO_BRC_VBR,			///< 可变码流。
	VIDEO_BRC_MBR,			///< 混合码流。
	VIDEO_BRC_NR				///< 枚举的码流控制模式数目。
}video_brc_t;

/// 分辨率类型
/// \note更新下面这个结构时，要同步更新Record.cpp里验证编码能力
typedef enum video_size_t 
{
	VIDEO_SIZE_D1,  ///< 704*576(PAL) 704*480(NTSC)
	VIDEO_SIZE_HD1,  ///< 704*288(PAL) 704*240(NTSC)
	VIDEO_SIZE_BCIF,	///< 352*576(PAL) 352*480(NTSC)
	VIDEO_SIZE_CIF,  ///< 352*288(PAL) 352*240(NTSC)
	VIDEO_SIZE_QCIF,	///< 176*144(PAL) 176*120(NTSC)
	VIDEO_SIZE_VGA,  ///< 640*480(PAL) 640*480(NTSC)
	VIDEO_SIZE_QVGA,	///< 320*240(PAL) 320*240(NTSC)
	VIDEO_SIZE_SVCD,	///< 480*480(PAL) 480*480(NTSC)
	VIDEO_SIZE_QQVGA,  ///< 160*128(PAL) 160*128(NTSC)
	VIDEO_SIZE_SVGA = 9,	//800*592
	VIDEO_SIZE_XVGA,	//1024*768
	VIDEO_SIZE_WXGA,	//1280*800
	VIDEO_SIZE_SXGA,	//1280*1024
	VIDEO_SIZE_WSXGA,  //1600*1024
	VIDEO_SIZE_UXGA,	//1600*1200
	VIDEO_SIZE_WUXGA,  //1920*1200
	VIDEO_SIZE_LTF,  //240*192
	VIDEO_SIZE_720,  //1280*720
	VIDEO_SIZE_1080,	//1920*1080
	VIDEO_SIZE_HD960,  //1280*960
	VIDEO_SIZE_960H,	 //960 * 576 (PAL)	960 * 480(NTSC) 20
	VIDEO_SIZE_HD1024, //1280*1024 
	VIDEO_SIZE_HALF1080,//960*1080
	VIDEO_SIZE_QXGA,//2048*1536
	VIDEO_SIZE_UHD4K,//3840*2160
	VIDEO_SIZE_4M,//2688*1520
	VIDEO_SIZE_5M,//2560*1920
	VIDEO_SIZE_HALF4M,//1280*1440
	VIDEO_SIZE_12M,//4000*3000
	VIDEO_SIZE_4M_QHD,//2560*1440
	VIDEO_SIZE_3M,//1920*1536
	VIDEO_SIZE_1440X900,//1440*900
	VIDEO_SIZE_NHD,//640*360
	VIDEO_SIZE_1296P,//2304*1296
	VIDEO_SIZE_540P, //960*540
	VIDEO_SIZE_NR   ///< 枚举的图形大小种类的数目。
}video_size_t;

typedef enum AdChnFormat_e 
{ 
	IP_FORMAT = 0,
	NEXTCHIP_960H =1,
	NEXTCHIP_720P =2,
	NEXTCHIP_1080P =3,
	TECHPOINT_960H =4,
	TECHPOINT_720P =5,
	TECHPOINT_HALF_1080P =6,
	TECHPOINT_1080P =7,
	TECHWELL_960H =8,
	TECHPOINT_3M =9,
	TECHPOINT_5M =10,           //2592x1944@12.5 for TVI
	NEXTCHIP_HALF_1080P =11, 
	TECHPOINT_4M =12, //2688x1520@15 for TVI
	NEXTCHIP_3M =13,
	TECHPOINT_3M_1920 = 14,
	TECHPOINT_3M_2304 = 15,
	FORMAT_AUTO = 16,
	TECHPOINT_HALF_4M = 17,     //1280x1440@30 for TVI/HDA/HDC
	TECHPOINT_HALF_QHD = 18,    //1280x1440@15 for TVI/HDA/HDC
	TECHPOINT_4M_QHD = 19,   //2560x1440@30 for TVI/HDA/HDC
	TECHPOINT_4M_QHD15 = 20, //2560x1440@15 for TVI/HDA/HDC
	TECHPOINT_5M20 = 21,     //2592x1944@20 for TVI/HDA
	Format_BUTT
}AdChnFormat_e;

//通道关联枚举 
//用于提供给应用产品对应的关联模式 
//singlechn_ass :单通道关联 
//doublechn_ass:双通道关联 
typedef enum VideoChnAssociationMode_s 
{ 
	SINGLE_ASS = 0, 
	DOUBLE_ASS, 	
	FORTH_ASS,
	ASSOCIATION_MODE_BUT
}VideoChnAssociationMode_e; 

//提供所有可选的通道模式掩码以及通道关联模式 
typedef struct VideoChnRules_s 
{ 
	DWORD caps; 
	VideoChnAssociationMode_e chnsyn; 
}VideoChnRules_t,*VideoChnRules_p;

typedef struct VideoInInfo_s
{
	int ViDev;
	int ViChn;//vi??????
	int AdChn;//AD?????
	DWORD Chntype;//???????

#ifdef DELETE_FOR_BUILT
	SAMPLE_VI_DEV_TYPE_E ViDevMode;
	SAMPLE_VI_CHN_MODE_E ViChnMode;
#endif

	int ClkEdge;//?????
	int ViMonorAttr;// VI???
//	use_ad_program_e AdProg;// ???AD??
	//PIC_SIZE_E InSize;//vi?????
	unsigned int work_type;
	int AdChnMode;
	VIDEO_NORM_E enNorm;
	int ViSrcFps;
	unsigned char std;
	int ViDstFps;//????????
	int EncFps;
}VideoInInfo_t,*VideoInInfo_p;

typedef struct VideoInDevice_s
{
	int videoInChannel;
	SIZE_S vi_pic;
	int isp_setformat;
    TEST_VI_MODE_E ViMode;
	VideoInInfo_t VDInfo[MAX_VIDEO_IN_CHANNEL];
}VideoInDevice_t, *VideoInDevice_p;

typedef struct SensorDevice_t
{
	SEN_TYPE_E SenType;
	char Name[16];
	int MaxWidth;
	int MaxHeight;
	int MaxFPS;
	int ISP_Format;
}SensorDevice_t,*SensorDevice_p;

typedef struct channel_info
{
    int channel;
    int enable;
    int width;
    int height;
    int max_width;
    int max_height;
    int yuv_type;
    unsigned char frame_count;
    unsigned char frame_time;
    int bps;
    int enc_type;
    int rc_type;
    unsigned short breath_on;
	unsigned int qt_level;
	VENC_PROFILE_E profile;
} channel_info;

typedef enum ImagMode_e
{
	Mode_Rotate_0 = 0,
	Mode_Rotate_90,
	Mode_Rotate_180,
	Mode_Rotate_270,
}ImagMode_e;

///视频编码跳帧模式
typedef enum VideoRefMode
{
	videoNoRefMode			= 0,			///< 非跳帧模式
	videoNomalRefMode		= 1<<0,			///<  普通模式
	videoAdvanceRefMode		= 1<<1,			///<  高级模式
}VideoRefMode;

/// 视频编码普通跳帧模式参数
typedef struct VideoRefNomalModeOpt
{
	///<  支持的跳帧倍数
	// Mode1 : 支持1,2倍跳帧
	// Mode2 : 支持1,2,4倍跳帧
	// Mode3 : 支持1,2,4,6倍跳帧
	// Mode4 : 支持1,2,4,6,8倍跳帧
	unsigned int refFrame;			
}VideoRefNomalModeOpt;

/// 视频编码跳帧属性,h264和h265编码类型支持128字节
typedef struct VideoRefOptions
{
	VideoRefMode				mode;					///< 跳帧模式
	int							resercved[7];			///< 保留位
	union
	{
		VideoRefNomalModeOpt		nomalModeOpt; 
		int							resercved[24];		///< 保留位
	} options;
}VideoRefOptions;

/// 视频数据格式结构
typedef struct VIDEO_FORMAT
{
	BYTE	Compression;		///< 压缩标准，取video_comp_t类型的值。 
	BYTE	BitRateControl; 	///< 码流控制模式，取video_brc_t类型的值。 
	BYTE	ImageSize; 			///< 图像大小，取video_size_t类型的值。 
	BYTE	ImageQuality;		///< 图像质量，取值1-6，值越大，图像质量越好,只有在可变码流下才起作用。
	BYTE	FramesPerSecond;	///< 每秒钟的帧数。一般PAL制1-25帧/秒，NTSC制1-30帧/秒。 
	BYTE	AVOption;			///< 音视频选项，最低位表示视频，其次位表示音频 
	WORD	BitRate;			///< 参考码流值，Kbps为单位
	BYTE	profile;			///< h264、h265编码类型,取video_h264profile_t和video_h265profile_t类型的值。
	VideoRefOptions RefOptions; 
} VIDEO_FORMAT;

/// 标题叠加参数结构
typedef struct VIDEO_TITLE_PARAM
{
	/// 标题序号，最多支持的标题数从CaptureGetCaps可以取得。序号0对应的是
	/// 时间标题。时间标题点阵由底层内部产生，width height raster参数被忽略。
	int		index;

	/// 是否显示。0-不显示，其他参数忽略，1-显示。
	int		enable;

	/// 标题左上角x坐标，取8的整数倍，采用相对坐标体系(CIF)。
	WORD	x;		

	/// 标题左上角y坐标，采用相对坐标体系(CIF)。
	WORD	y;
	
	/// 标题宽度，取8的整数倍。					
	WORD	width;		

	/// 标题高度。
	WORD	height;		

	/// 前景颜色，16进制表示为0xAABBGGRR，A(Alpha)表示透明度。
	/// text color, rgba 8:8:8:8	
	DWORD	fg_color;
	
	/// 背景颜色，16进制表示为0xAABBGGRR，A(Alpha)表示透明度。
	/// background color, rgba 8:8:8:8	
	DWORD	bg_color;

	/// 标题单色点阵数据。0表示该点无效，1表示该点有效。每个字节包含8个象素的
	/// 数据，最左边的象素在最高位，紧接的下一个字节对应右边8个象素，直到一行
	/// 结束，接下来是下一行象素的点阵数据。
	BYTE	*raster;

	///< 是否设置字体黑白自动切换，0-不自动，1-自动
	int		AutoBlackWhite;
}VIDEO_TITLE_PARAM;

/// 设备特性结构
typedef struct VIDEO_CHN_CAP
{
	/// 支持的压缩标准的掩码，位序号对应枚举类型video_comp_t的每一个值。
	/// 置1表示支持该枚举值对应的特性，置0表示不支持。
	DWORD Compression;

	/// 支持的码流控制模式的掩码，位序号对应枚举类型video_brc_t的每一个值。
	/// 置1表示支持该枚举值对应的特性，置0表示不支持。
	DWORD BitRateControl;

	/// 支持的图像大小类型的掩码，位序号对应枚举类型video_size_t的每一个值。
	/// 置1表示支持该枚举值对应的特性，置0表示不支持。
	DWORD ImageSize;

	/// 支持的媒体类型的掩码，与枚举类型capture_start_t的每一个值相与，得到对应
	/// 的特性。置1表示支持该枚举值对应的特性，置0表示不支持。
	DWORD Media;

	/// 区域覆盖的支持的块数。
	int CoverBlocks;

//////////////////////////////////////////////////////////////////////////////////////
//修改
	/// 支持的图像大小类型的掩码，位序号对应枚举类型video_size_t的每一个值。
	/// 置1表示支持该枚举值对应的特性，置0表示不支持。
	DWORD JpegImageSize;

	/// 支持最多的标题叠加的个数。
	int	TitleCount;

	/// 是否支持区域覆盖 0-不支持，1-支持实时的区域覆盖， 即设置区域时
	/// 速度较快，可以实时调用， 2-支持非实时的区域覆盖，即设置区域时速度较慢，
	/// 只能在用户确定区域后调用。  
	int	RegionCover;
	///水印
	DWORD WaterMask;		/*!< 用位来表示，１位，字符，２位，图片 */
} VIDEO_CHN_CAP;

typedef struct VIDEO_CAPS
{
	VIDEO_CHN_CAP ChnCap[MAX_SYS_CHANNEL];
} VIDEO_CAPS;

/// 设备支持的最大分辨率与帧率
typedef struct VIDEO_MAX_SIZE
{
	int maxWidth;
	int maxHeight;
	int maxFPS;
}VIDEO_MAX_SIZE, *pVIDEO_MAX_SIZE;

typedef struct QP_FORMAT
{
	unsigned int BitRate;			
	unsigned int ImaxQP;
	unsigned int IminQP;
	unsigned int PmaxQP;
	unsigned int PminQP;
} QP_FORMAT;

#if 0
typedef struct AVBR_Attr
{
	int InitQp;
	int IminQP;
	int ImaxQP;
	int PminQP;
	int PmaxQP;
	int MaxratePercent;
	int IFrmMaxBits;
	int IPQPDelta;
	int IBitProp;
	int PBitProp;
	int FluctuateLevel;
	int StillratePercent;
	int Maxstillqp;
}AVBR_Attr;

int VideoSetAVBRAttr(int iChannel, AVBR_Attr* pAVBR);
#endif

/// 辅助码流支持特性结构
typedef struct VIDEO_EXT_STREAM
{
	DWORD ExtraStream;						///< 用channel_t的位来表示支持的功能。
	DWORD CaptureSizeMask[VIDEO_SIZE_NR];	///< 每一个值表示对应分辨率支持的辅助码流。（目前只能用于辅码流）
}VIDEO_EXT_STREAM, *pVIDEO_EXT_STREAM;	

/// 捕获编码能力结构
typedef struct VIDEO_DSPINFO 
{
	unsigned int	nMaxEncodePower;	///< DSP 支持的最高编码能力。
	unsigned short	nMaxSupportChannel;	///< DSP 支持最多输入视频通道数。
	unsigned short	bChannelMaxSetSync;	///< DSP 每通道的最大编码设置是否同步 0-不同步, 1 -同步。
	unsigned short	nExpandChannel;		///< DSP 支持的扩展通道数，主要是多路回放使用，目前只是一个
	unsigned short  rev;
}VIDEO_DSPINFO, *PVIDEO_DSPINFO;

typedef struct VIDEO_ENCCHIPCAPABILITY
{
	int EncChipNR;
	VIDEO_DSPINFO EncChipInfo[MAX_ENC_CHIP_NR];
}VIDEO_ENCCHIPCAPABILITY,*PVIDEO_ENCCHIPCAPABILITY;

int VideoGetEncChipCapability(VIDEO_ENCCHIPCAPABILITY *pEncChipCapability);

/// 得到捕获双码流支持的特性。参数和CaptureGetCaps一样，但得到得是双码流支持得特性，
/// 而不是普通编码特性。 
/// 
/// \param [out] pCaps 指向捕获特性结构CAPTURE_CAPS的指针。
/// \retval 0  获取成功。
/// \retval 0  获取失败。
int VideoGetExtCaps(VIDEO_EXT_STREAM *pStream);

/// 得到视频支持的特性。
/// 
/// \param [out] pCaps 指向视频特性结构VIDEO_CAPS的指针。
/// \retval 0  获取成功。
/// \retval 0  获取失败。
int VideoGetCaps(VIDEO_CAPS * pCaps);

/// 得到设备支持的最大分辨率与帧率
/// 
/// \param [out] pCaps 指向视频特性结构VIDEO_MAX_SIZE的指针。
/// \retval 0  获取成功。
/// \retval 0  获取失败。
int VideoGetMaxSize(VIDEO_MAX_SIZE *pCaps);

/// 创建视频设备
/// 
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int VideoCreate(int index);

/// 销毁视频设备
/// 
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功  
int VideoDestroy(int index);

/// 创建音频编码通道
/// 仅SD卡格式化使用，创建后需要重新设置编码参数
/// \retval 0  开始成功
/// \retval 0  开始失败
int VideoVencCreate(void);

/// 销毁音频编码通道
/// 仅SD卡格式化使用，销毁后需要重新创建
/// \retval 0  开始成功
/// \retval 0  开始失败
int VideoVencDestroy(void);

/// 设置视频数据压缩的格式。
/// 
/// \param [in] channel 通道号。
/// \param [in] dwType
/// \param [in] pFormat 指向视频格式结构VIDEO_FORMAT的指针。
/// \retval 0  设置成功
/// \retval 0  设置失败
int VideoSetFormat(int channel, DWORD dwType, VIDEO_FORMAT * pFormat);

/// 设置OSD字体大小。
///
/// \param [in] channel 通道号。
/// \param [in] size 字体大小
/// \retval 0  设置成功。
int VideoSetTitleSize(int channel,unsigned int size);

/// 设置标题叠加。
/// 
/// \param [in] channel 通道号。
/// \param [in] pParam 指向标题叠加结构VIDEO_TITLE_PARAM的指针。
/// \retval 0  设置成功。
/// \retval 0  设置失败。
int VideoSetTitle(int channel, VIDEO_TITLE_PARAM *pParam);

/// 设置logo叠加。
/// 
/// \param [in] channel 通道号。
/// \param [in] pParam 指向标题叠加结构VIDEO_TITLE_PARAM的指针。
/// \retval 0  设置成功。
/// \retval !0  设置失败。
int VideoSetLogo(int channel, VIDEO_TITLE_PARAM *pParam);

/// 设置覆盖区域。
/// 
/// \param [in] channel 通道号。
/// \param [in] pParam 指向视频覆盖结构VIDEO_COVER_PARAM的指针。
/// \retval 0  设置成功。
/// \retval 0  设置失败。
int VideoSetCover(int channel,int index, VIDEO_COVER_PARAM *pParam);

/// 获取上一帧的qp。
/// 
/// \param [in] channel 通道号。
/// \param [in] dwType
/// \param [in] qp 返回上一帧的qp。
/// \retval 0  设置成功
/// \retval 0  设置失败
int VideoGetLastQP(int channel, DWORD dwType, unsigned int * lastqp);

/// 设置单一通道的属性。
/// 
/// \param [in] channel 通道号。
/// \param [in] dwType
/// \param [in] QP_FORMAT 设置的参数。
/// \retval 0  设置成功
/// \retval 0  设置失败
int VideoSetQP(int channel, DWORD dwType, QP_FORMAT *pFormat);

/// 设置画面旋转。
/// 
/// \param [in] channel 通道号。
/// \param [in] Rotate 旋转角度。
/// \retval 0  设置成功。
/// \retval 0  设置失败。
int VideoSetRotate(int channel,ImagMode_e Rotate);

/// 设置画面镜像与翻转。
/// 
/// \param [in] channel 通道号。
/// \param [in] mirror 是否镜像，0:不镜像 1:镜像。
/// \param [in] flip 是否翻转，0:不翻转 1:翻转。
/// \retval 0  设置成功。
/// \retval 0  设置失败。
int VideoSetMirrorAndflip(int channel,int mirror,int flip);

/// 设置时间和格式。应用程序需要定时调用此函数进行同步时间。
/// 
/// \param [in] channel 通道号。
/// \param [in] pTime 指向系统时间结构SYSTEM_TIME的指针。
/// \param [in] datefmt 日期格式，取date_fmt枚举值。
/// \param [in] datespc 日期分割符，取date_spec枚举值。
/// \param [in] timefmt 时间格式，取time_fmt枚举值。
/// \retval 0  设置成功。
/// \retval 0  设置失败。
int VideoSetTime(int channel, const SYSTEM_TIME * pTime, int datefmt,int datespc,int timefmt,int WeekEnable);

/// 设置OSD语言。
/// 
/// \param [in] language 语言，0:英文 1:中文。
/// \retval 0  设置成功。
/// \retval 0  设置失败。
int VideoSetOSDLanguage(int language);

/// 设置视频制式。
/// 
/// \param [in] channel 通道号。
/// \param [in] dwStandard 视频制式，取video_standard_t类型的值。
/// \retval 0  设置成功
/// \retval <0  设置失败
int VideoSetVstd(int channel,DWORD dwStandard);	

/// /* 初始化调色板内容 */
///
/// \param [in] pstOsdPalette 调色板信息
/// \retval 0  设置成功。
int SampleOsdPaletteInit(OSD_PALETTE_S *pstOsdPalette);

/// 大小端点阵转换
///
/// \param [in] pOsdData 点阵数据
/// \param [in] enPixelFormat 点阵数据格式
/// \param [in] u32DataSize 点阵数据大小
/// \retval 0  设置成功。
void OSD_TEST_ConvertToLittleEndian2(NI_VOID *pOsdData, OSD_PIXEL_FORMAT_E enPixelFormat, NI_U32 u32DataSize);

/// 开启编码自适应。
/// \retval 0  成功
/// \retval <0  失败
int video_venc_adaptive_start();

/// 关闭编码自适应。
/// \retval 0  成功
/// \retval <0  失败
int video_venc_adaptive_stop();

/// 获取帧率
/// \retval 0  开始成功
/// \retval 0  开始失败
int get_frame_rate(float FrameRate);


#ifdef __cplusplus
}
#endif

#endif //__VIDEO_API_H__