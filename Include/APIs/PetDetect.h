#ifndef __PET_DETECT_H__
#define __PET_DETECT_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "TdCommon.h"

/// 宠物检测参数
typedef struct PET_DETECT_PARAM
{
	BYTE Level; 	///< 等级	0 - 100
	int Enable; 	///< 为1表示使能，为0时表示禁用
} PET_DETECT_PARAM;

/// 宠物检测结果
typedef struct PET_DETECT_RESULT
{
	int result;		///< 为1表示检测到宠物，为0表示未检测到宠物
	WORD left;		///< 检测结果左边界
	WORD top;	 	///< 检测结果上边界
	WORD right;		///< 检测结果右边界
	WORD bottom; 	///< 检测结果下边界
} PET_DETECT_RESULT;

typedef struct pet_result_s
{
	unsigned int Result;
 	unsigned int x1;
 	unsigned int y1;
 	unsigned int x2;
 	unsigned int y2;

	pthread_mutex_t result_lock;
} pet_result_t;

typedef struct pet_param_s
{
	unsigned int sensiblity; /* 灵敏度 1--100 */
	unsigned int Enable;
	unsigned int change;
} pet_param_t;

typedef struct PetDetectDevice_s
{
	pet_result_t Result;
	pet_param_t param;

} PetDetectDevice_t;

/// 创建宠物检测设备
///
/// \param 无
/// \retval <0 创建失败
/// \retval 0 创建成功
int PetDetectCreate(void);

/// 销毁宠物检测设备
///
/// \param 无
/// \retval <0 销毁失败
/// \retval 0 销毁成功
int PetDetectDestroy(void);

/// 设置宠物检测的参数。立即返回。
///
/// \param [in] channel 通道号。
/// \param [in] pParam 指向动态检测参数结构PET_DETECT_PARAM的指针。
/// \retval 0  设置成功
/// \retval <0  设置失败
int PetDetectSetParameter(int channel, PET_DETECT_PARAM *pParam);

/// 宠物检测捕获的详细结果。
///
/// \param [in] channel 通道号。
/// \param [in] pResult 指向动态检测参数结构PET_DETECT_RESULT的指针。
/// \retval 0  检测到移动宠物
/// \retval <0  未检测到移动宠物
int PetDetectGetResult(int channel, PET_DETECT_RESULT *pResult);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /*__PET_DETECT_H__*/
