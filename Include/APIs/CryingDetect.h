#ifndef __CRYING_DETECT_H__
#define __CRYING_DETECT_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "TdCommon.h"

typedef struct tagIasOdFileInfo
{
    unsigned char *pu8DataFile;     // 输入视频数据文件
    unsigned char *pu8ODModelFile;  // OD模型文件
    unsigned char  u8PrintFlag;     // 打印标记
} IAS_OD_TEST_FILE_INFO_S;

typedef struct tagIasAdTestCtx
{
    IAS_ALG_CHN     OdChn;
    NI_BOOL         bRunFrame;
    NI_BOOL         bRunGetOdResult;
    NI_BOOL         bRunGetUslessFrame;
    NI_BOOL         bDetectEnable;
    NI_S16          *pWaveData;

   IAS_OD_TEST_FILE_INFO_S stOdTestFileInfo;
}IAS_AD_TEST_CTX_S;

typedef struct CRY_DETECT_PARAM
{
	BYTE Level; 	///< 等级	0 - 100
	int Enable; 	///< 为1表示使能，为0时表示禁用
} CRY_DETECT_PARAM;

typedef struct CRY_DETECT_RESULT
{
    IAS_AD_PROC_TYPE_E enCryStatus;
    unsigned short u16Score;
} CRY_DETECT_RESULT;

/// 初始化哭声检测
///
/// \retval 0  初始化成功
/// \retval <0  初始化失败
int CryingDetectCreate(void);

/// 获取哭声检测结果
///
/// \retval 0  识别到哭声
/// \retval <0  未识别到哭声
int CryingDetectGetResult(int channel, CRY_DETECT_RESULT *pResult);

/// 设置哭声检测参数
///
/// \retval 0  设置成功
/// \retval <0  设置失败
int CryingDetectSetParameter(int channel, CRY_DETECT_PARAM *pParam);

/// 销毁哭声检测
///
/// \retval 0  销毁成功
/// \retval <0  销毁失败
int CryingDetectDestroy(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /*__CRYING_DETECT_H__*/
