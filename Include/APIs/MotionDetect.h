
#ifndef __MOTION_DETECT_API_H__
#define __MOTION_DETECT_API_H__
//#include "base.h"
#include "TdCommon.h"


#define PG_MAX_NUM 10


#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup MotionDetectAPI API Motion Detect
/// 按区域和灵敏度的动态检测接口。
///	\n 调用流程图:
/// \code
///    ============================
///                  |
///         *MotionDetectGetCaps
///                  |
///          MotionDetectCreate
///     +------------|
///     |   MotionDetectGetState
///     | MotionDetectSetParameter
///     |   MotionDetectGetResult
///     |   MotionDetectShowHint
///     +------------|
///         MotionDetectDestory
///                  |
///    ============================
/// \endcode
/// @{

/// 动态检测参数
typedef struct MOTION_DETECT_PARAM
{
	/// 动态检测的敏感度档次
	int		iLevel;

	/// 动态检测的区域，数据的每一位对应一个矩形区域块，置1表示需要在该区域块动
	/// 态检测，置0表示不需要在该区域块动态检测。每一行用一个DWORD表示，最左边的
	/// 块对应最低位，从上到下的行对应的下标从0到17。支持的行列数之外对应的数据
	/// 位无效。
	DWORD	win[18];

	/// 灵敏度的具体值
	BYTE	sensitiveness;

	/// 帧间隔
	BYTE	fieldinterval;

	/// 使能开关，是否使能，为1表示使能，为0时表示禁用，其他参数被忽略。
	int		enable;
}MOTION_DETECT_PARAM;

/// 动态检测结果
typedef struct MOTION_DETECT_RESULT
{
	/// 有动态检测发生的区域，数据的每一位对应一个矩形区域块，置1表示在该区域块
	/// 动态检测成功，置0表示该区域块动态检测失败。每一行用一个DWORD表示，最左边
	/// 的块在最高位，从上到下的行对应的下标从0到17。支持的行列数之外对应的数据
	/// 位无效。
	DWORD	win[18];
}MOTION_DETECT_RESULT;

typedef struct MD_CONFIG
{
	unsigned int enable;
	unsigned int threshold;  /*!< 灵敏度 [0~100] */
	unsigned int framedelay; /*!< 帧间隔，即每隔几帧计算一次 */
} MD_CONFIG;

/// 人形检测参数
typedef struct HUMAN_DETECT_PARAM
{
	BYTE	Level;            	///< 等级	0 - 100
	int		Enable;				///< 为1表示使能，为0时表示禁用
}HUMAN_DETECT_PARAM;

/// 车型检测参数
typedef struct VEHICLE_DETECT_PARAM
{
	BYTE Level; 				///< 等级	0 - 100
	int Enable; 				///< 为1表示使能，为0时表示禁用
} VEHICLE_DETECT_PARAM;

/// 跨线/区域参数
typedef struct PG_DETECT_PARAM
{
	int Id; 	///< 对应跨线/区域的ID
	int Level;	///< 等级	0 - 100
} PG_DETECT_PARAM;

/// 人形检测结果
typedef struct HUMAN_DETECT_RESULT
{
	int result;			///< 为1表示检测到人形，为0表示未检测到人形
	WORD left;			///< 检测结果左边界
	WORD top;			///< 检测结果上边界
	WORD right;			///< 检测结果右边界
	WORD bottom;		///< 检测结果下边界
}HUMAN_DETECT_RESULT;

/// 车型检测结果
typedef struct VEHICLE_DETECT_RESULT
{
	int result;			///< 为1表示检测到车型，为0表示未检测到车型
	WORD left;			///< 检测结果左边界
	WORD top;	 		///< 检测结果上边界
	WORD right;	 		///< 检测结果右边界
	WORD bottom; 		///< 检测结果下边界
} VEHICLE_DETECT_RESULT;

typedef struct MD_TIME_s                                            
{
    double  second;     //  秒   0-59
    unsigned int  minute;       //  分   0-59
    unsigned int  hour;     //  时   0-23
    unsigned int  day;      //  日   1-31
    unsigned int  month;        //  月   1-12
    unsigned int  year;     //  年   2000-2063   
}MD_TIME_t;

/// 视频动态检测特性结构
typedef struct MOTION_DETECT_CAPS
{
	DWORD Enabled;			///< 置1表示支持动态检测，置0表示不支持动态检测。
	DWORD GridLines;		///< 动态检测的区域需要划分成的列数。
	DWORD GridRows;			///< 动态检测的区域需要划分成的行数。
	BYTE  Result;			///< 是否能得到每块区域的检测结果。
	BYTE  Hint;				///< 是否能在检测成功时给予提示。
}MOTION_DETECT_CAPS;

typedef struct 
{
	int x;
	int y;
}POINTS;

//电子围栏坐标
typedef struct 
{
	POINTS Points[4];
} Quadrilateral;

/* 周界类型 */
typedef enum PerimeterDetectType
{
    PERIMETER_POLYLINE = 0,     /*绊线*/         
    PERIMETER_POLYGON = 1,      /*区域*/             
    PERIMETER_BUTT,                
} PG_TYPE_E;


/* 周界方向设定 */
typedef enum PerimeterDetectDirection
{
    DIRECTION_IN = 0,        /*进方向*/            
    DIRECTION_OUT = 1,       /*出方向*/     
    DIRECTION_BI = 2,        /*双向*/    
    DIRECTION_BUTT,                
}  PG_DIRECTION_E;

/* 线规则信息 */
typedef struct PerimeterDetectPolyline
{
    PG_DIRECTION_E       Direction;           /*报警方向*/
    unsigned int         PointNum;            /*折线的断点数量*/
	POINTS 			     Point[PG_MAX_NUM];
} PG_POLYLINE_S;

/* 区域规则信息 */
typedef struct PerimeterDetectPolygon
{
    PG_DIRECTION_E      Direction;           /*报警方向*/
    unsigned int        PointNum;
    POINTS              Point[PG_MAX_NUM];
} PG_POLYGON_S;

/*周界规则*/
typedef struct PerimeterDetect
{
    PG_TYPE_E   Type;
    union 
    {
        PG_POLYLINE_S   Polyline;   
        PG_POLYGON_S    Polygon;
    };
} PERIMETER_DETECT_S;

/*周界规则*/
typedef struct PerimeterDetectParam
{
	IAS_PERIMETER_RULE_S Parm[2][PG_MAX_NUM];
	unsigned int Id[2][PG_MAX_NUM];
	unsigned int RuleNum[2];
	unsigned int Level[2][PG_MAX_NUM];
	BOOL Add[2][PG_MAX_NUM];
	BOOL Delete[2][PG_MAX_NUM];
	BOOL Change[2][PG_MAX_NUM];
	pthread_rwlock_t rwlock;
} PERIMETER_DETECT_PARAM_S;

typedef enum ObjectType
{
    OBJ_TYPE_HUMAN       = 0,
    OBJ_TYPE_CAR         = 1,
	OBJ_TYPE_BOTH        = 2,
    OBJ_TYPE_BUTT,
} OBJ_TYPE_E;

typedef struct PgResult
{
	unsigned int    Id;
	OBJ_TYPE_E 		Type;
	unsigned int  	Result;
} PG_RESULT;

typedef struct PerimeterDetectResult
{
	PG_TYPE_E 	Type;
	union
	{
		PG_RESULT PolyLineResult[PG_MAX_NUM];
		PG_RESULT PolyGonResult[PG_MAX_NUM];
	};
} PERIMETER_DETECT_RESULT_S;

/// 获取动检和遮挡的初始化参数
///
/// \param [in] pstVdChnAttr 动检和遮挡的初始化参数
void vd_attr_init(MD_CHN_ATTR_S *pstVdChnAttr);

/// 创建动态检测设备
/// 
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int MotionDetectCreate(void);


/// 销毁动态检测设备
/// 
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int MotionDetectDestory(void);


/// 执行动态检测。立即返回。
/// 
/// \param [out] pData 指向一个DWORD值的指针，DWORD值是各个报警输入通道电平状态
///        的掩码。低通道在低位，高通道在高位。高电平置1，低电平置0，不存在的通
///        道置0。
/// \retval 0  检测成功
/// \retval <0  检测失败
int MotionDetectGetState(DWORD* pData);


/// 动态检测捕获的详细结果。
/// 
/// \param [in] channel 通道号。
/// \param [in] pResult 指向动态检测参数结构MOTION_DETECT_RESULT的指针。
/// \retval 0  检测到移动
/// \retval <0  未检测到移动
int MotionDetectGetResult(int channel, MOTION_DETECT_RESULT *pResult);


/// 设置是否在动态检测成功的区域给予提示。
/// 
/// \param [in] channel 通道号。
/// \param [in] enable 为1表示提示，为0时表示不提示。
/// \retval 0  设置成功
/// \retval <0  设置失败
int MotionDetectShowHint(int channel, int enable);


/// 执行动态检测。立即返回。
/// 
/// \param [in] channel 通道号。
/// \param [in] pParam 指向动态检测参数结构MOTION_DETECT_PARAM的指针。
/// \retval 0  设置成功
/// \retval <0  设置失败
int MotionDetectSetParameter(int channel, MOTION_DETECT_PARAM *pParam);


/// 得到动态检测支持的特性。
/// 
/// \param [out] pCaps 指向动态检测特性结构MOTION_DETECT_CAPS的指针。
/// \retval 0  获取成功。
/// \retval <0  获取失败。
int MotionDetectGetCaps(MOTION_DETECT_CAPS * pCaps);

/// 创建人形检测设备
/// 
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int HumanDetectCreate(void);

/// 销毁人形检测设备
/// 
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int HumanDetectDestory(void);

/// 设置人形检测的参数。立即返回。
/// 
/// \param [in] channel 通道号。
/// \param [in] pParam 指向动态检测参数结构HUMAN_DETECT_PARAM的指针。
/// \retval 0  设置成功
/// \retval <0  设置失败
int HumanDetectSetParameter(int channel, HUMAN_DETECT_PARAM *pParam);

/// 人形检测捕获的详细结果。
/// 
/// \param [in] channel 通道号。
/// \param [in] pResult 指向动态检测参数结构HUMAN_DETECT_RESULT的指针。
/// \retval 0  检测到移动人形
/// \retval <0  未检测到移动人形
int HumanDetectGetResult(int channel, HUMAN_DETECT_RESULT *pResult);

/// 设置人形跟踪的开关。
///
/// \param [in] channel 通道号。
/// \param [in] Enable 开关
/// \param [in] MoveKpx 转动角度
/// \retval 0  设置成功
/// \retval <0  设置失败
int HumanTrackSetParameter(int channel, unsigned int Enable, unsigned int MoveKpx);

/// 设置人形跟踪的复位点。
///
/// \param [in] xCoord x坐标
/// \param [in] yCoord y坐标
/// \retval 0  设置成功
/// \retval <0  设置失败
int HumanTrackGoBackPoint(int xCoord, int yCoord);

/// 人形检测捕获的详细结果。
///
/// \param [in] channel 通道号。
/// \param [in] pResult 指向动态检测参数结构HUMAN_DETECT_RESULT的指针。
/// \retval 0  检测到移动人形
/// \retval <0  未检测到移动人形
int HumanDetectGetResult(int channel, HUMAN_DETECT_RESULT *pResult);

/// 创建车型检测设备
///
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int VehicleDetectCreate(void);

/// 销毁车型检测设备
///
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int VehicleDetectDestory(void);

/// 设置车型检测的参数。立即返回。
///
/// \param [in] channel 通道号。
/// \param [in] pParam 指向动态检测参数结构HUMAN_DETECT_PARAM的指针。
/// \retval 0  设置成功
/// \retval <0  设置失败
int VehicleDetectSetParameter(int channel, VEHICLE_DETECT_PARAM *pParam);

/// 车型检测捕获的详细结果。
///
/// \param [in] channel 通道号。
/// \param [in] pResult 指向动态检测参数结构VEHICLE_DETECT_RESULT的指针。
/// \retval 0  检测到车型
/// \retval <0  未检测到车型
int VehicleDetectGetResult(int channel, VEHICLE_DETECT_RESULT *pResult);

/// 添加周界规则。
///
/// \param [in] channel 通道号。
/// \param [in] pRules 指向周界规则PERIMETER_DETECT_S的指针。
/// \retval 0  添加成功
/// \retval <0  添加失败
int PerimeterDetectAdd(int channel, PERIMETER_DETECT_S *pRules);

/// 添加周界规则。
///
/// \param [in] channel 通道号。
/// \param [in] pParam 指向周界规则PG_DETECT_PARAM的指针。
/// \retval 0  添加成功
/// \retval <0  添加失败
int PerimeterDetectSetParameter(int channel, PG_DETECT_PARAM *pParam);

/// 删除周界规则。
///
/// \param [in] channel 通道号。
/// \param [in] pRules 指向周界规则PERIMETER_DETECT_S的指针。
/// \retval 0  删除成功
/// \retval <0  删除失败
int PerimeterDetectDelete(int channel, unsigned int Id);

/// 获取周界结果。
///
/// \param [in] channel 通道号。
/// \param [in] pRules 指向周界结果PERIMETER_DETECT_RESULT_S的指针。
/// \retval 0  成功
int PerimeterDetectGetResult(int channel, PERIMETER_DETECT_RESULT_S *pResult);


/// @} end of group

#ifdef __cplusplus
}
#endif



#endif //__MOTION_DETECT_API_H__

