#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <string.h>
#include <types.h>


// #define GPIO_DEVICE_NAME	"motor_ctrl"  
// #define GPIO_IOCTL_MAGIC	'm'  
// #define GPIO_IOCTL_RIGHT	_IO(GPIO_IOCTL_MAGIC, 0x0)  
// #define GPIO_IOCTL_LEFT	_IO(GPIO_IOCTL_MAGIC, 0x1)  
// #define GPIO_IOCTL_UP	_IO(GPIO_IOCTL_MAGIC, 0x2)  
// #define GPIO_IOCTL_DOWN	_IO(GPIO_IOCTL_MAGIC, 0x3)  


///< 电子云台控制命令
typedef enum VideoInPtzCmd
{
	VIDEOIN_PTZ_GETCAPS,					///< 获取电子云台控制能力 PTZ_OPT_STRUCT.ptzCaps
	VIDEOIN_PTZ_ZOOM,						///< 配置电子云台放大倍数	PTZ_OPT_STRUCT.zoom  
	VIDEOIN_PTZ_FOCUS,						///< 配置电子云台焦距 PTZ_OPT_STRUCT.focal
	VIDEOIN_PTZ_MOVE,						///< 移动电子云台视角
	VIDEOIN_PTZ_STOP,						///< 停止移动电子云台视角
	VIDEOIN_PTZ_RESET,						///< 电子云台进行复位
	VIDEOIN_PTZ_COVER,						///< 配置物理遮蔽电子云台
	VIDEOIN_PTZ_LOCATION,					///< 获取电子云台当前绝对位置
	VIDEOIN_PTZ_SPEED,						///< 获取电子云台速度范围 PTZ_OPT_STRUCT.ptzSpeed
}PTZ_OPT_PtzCmd;

///< 电子云台功能
typedef struct VideoInPtzCaps
{
	unsigned int capsMask;					///< 支持的电子云台控制命令掩码，定义见 PTZ_OPT_PtzCmd
}PTZ_OPT_PtzCaps;

///< 电子云台总的移动范围: x轴: 0~8192, y轴: 0~8192
typedef struct VideoInPtzMove
{
	int xCoord;								///< 水平坐标，取值范围0~8192
	int yCoord;								///< 竖直坐标，取值范围0~8192
	int Speed;								///< 速度，取值范围
	int Blocking;							///< 云台转完返回的标志位
}PTZ_OPT_PtzMove;

///< 电子云台总的移动范围: x轴: 0~8192, y轴: 0~8192
typedef struct VideoInPtzStat
{
	int xCoord;								///< 水平坐标，取值范围0~8192
	int yCoord;								///< 竖直坐标，取值范围0~8192
	int PtzWorking;						///< 云台转动的标志位
	int XLimitFlag;							///< 云台X轴到达限位的标志位
	int YLimitFlag;							///< 云台Y轴到达限位的标志位
}PTZ_OPT_PtzStat;

///< 电子云台移动速度: 最大速度: MaxSpeed,最小速度: MinSpeed
typedef struct VieoInPtzSpeed
{
	int MaxSpeed;							///< 最大速度
	int MinSpeed;							///< 最小速度
}PTZ_OPT_PtzSpeed;

//! 云台操作命令结构
typedef struct 
{
	int ptzCmd;								///< 电子云台控制命令, 定义见PTZ_OPT_PtzCmd

	union
	{
		PTZ_OPT_PtzCaps ptzCaps;			///< VIDEOIN_PTZ_GETCAPS命令返回值，
		float zoom;							///< VIDEOIN_PTZ_ZOOM命令参数，配置放大倍数，0~1
		float focal;						///< VIDEOIN_PTZ_FOCUS命令参数，配置焦距，0~1
		PTZ_OPT_PtzMove ptzMove;			///< VIDEOIN_PTZ_MOVE命令参数
		PTZ_OPT_PtzStat ptzLocation;		///< VIDEOIN_PTZ_LOCATION命令参数
		PTZ_OPT_PtzSpeed ptzSpeed;			///< VIDEOIN_PTZ_SPEED命令参数
		int ptzCoverEnable;				///< VIDEOIN_PTZ_COVER命令参数
		int reserved[64];
	};

	int reserve[31];						///< 保留
}PTZ_OPT_PtzPrm;

extern int CtrltPtz(PTZ_OPT_PtzPrm *ptzPrm);

extern int SetPTZSpeed(unsigned int level);

extern void CheckValue(int *value);