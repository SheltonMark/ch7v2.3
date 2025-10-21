//
//  "$Id$"
//
//  Copyright (c)1992-2007, ZheJiang Zhiling Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:	
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#ifndef __BLIND_DETECT_API_H__
#define __BLIND_DETECT_API_H__

#include "TdCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup BlindDetectAPI API Blind Detect
/// 全屏范围按灵敏度的遮挡检测接口。
///	\n 调用流程图:
/// \code
///    ===========================
///                |
///        *BlindDetectGetCaps
///                |
///         BlindDetectCreate
///    +-----------|
///    |   BlindDetectGetState
///    |   BlindDetectSetParameter
///    +-----------|
///       BlindDetectDestory
///                |
///    ===========================
/// \endcode
/// @{

/// 黑屏检测设备特性结构
typedef struct BLIND_DETECT_CAPS
{
	DWORD Enabled;			///< 置1表示支持黑屏检测，置0表示不支持黑屏检测。
}BLIND_DETECT_CAPS;

/// 黑屏检测参数
typedef struct BLIND_DETECT_PARAM
{
	BYTE	Level;            	///< 等级	0 - 100
	int		Enable;				///< 为1表示使能，为0时表示禁用
}BLIND_DETECT_PARAM;

/**
 *  @brief 遮挡侦测配置结构体
 */
typedef struct CDConfig_t
{
	unsigned int enable;
	unsigned int threshold;	 /*!< 灵敏度 [0~100] */
	unsigned int framedelay; /*!< 帧间隔，与动检一致 */
} CDConfig_t;

/// 创建黑屏检测设备
/// 
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int BlindDetectCreate(void);


/// 销毁黑屏检测设备
/// 
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int BlindDetectDestory(void);


/// 执行黑屏检测。立即返回。
/// 
/// \param [out] pData 指向一个DWORD值的指针，DWORD值是各个报警输入通道电平状态的
///       掩码。低通道在低位，高通道在高位。高电平置1，低电平置0，不存在的通道置0。
/// \retval 0  检测成功
/// \retval <0  检测失败
int BlindDetectGetState(DWORD* pData);

/// 黑屏检测捕获的详细结果。
/// 
/// \retval 0  设置成功
/// \retval <0  设置失败
int BlindDetectGetResult(int channel, int * result);


/// 设置黑屏检测捕获的参数。
/// 
/// \param [in] channel 通道号。
/// \param [in] pParam 指向黑屏检测参数结构BLIND_DETECT_ PARAM的指针。
/// \retval 0  设置成功
/// \retval <0  设置失败
int BlindDetectSetParameter(int channel, BLIND_DETECT_PARAM *pParam);


/// 得到黑屏支持的特性。
/// 
/// \param [out] pCaps 指向黑屏特性结构BLIND_DETECT _CAPS的指针。
/// \retval 0  获取成功。
/// \retval <0  获取失败。
int BlindDetectGetCaps(BLIND_DETECT_CAPS * pCaps);

/// @} end of group

#ifdef __cplusplus
}
#endif

#endif //__BLIND_DETECT_API_H__

