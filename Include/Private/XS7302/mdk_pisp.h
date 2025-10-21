/* 
 * mdk_pisp.h -- pisp模块对外提供的API头文件
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */


#ifndef _MDK_PISP_H_
#define _MDK_PISP_H_
    
/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "ni_comm_pisp.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @description: 设置PISP设备公共属性，如输入图像有效分辨率、帧率等公共参数。输入图像有效宽度最大支持2688，有效高度最大支持1944；本接口支持多次调用。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_PUB_ATTR_S} *pstPubAttr: 输入，公共属性
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_GetPubAttr(ISP_DEV IspDev, PISP_PUB_ATTR_S *pstPubAttr);

/**
 * @description: 获取PISP设备公共属性。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_PUB_ATTR_S} *pstPubAttr: 输出，公共属性
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_SetPubAttr(ISP_DEV IspDev, const PISP_PUB_ATTR_S *pstPubAttr);

/**
 * @description: 初始化PISP设备。初始化前需要先调用NI_MDK_PISP_SetPubAttr完成PISP设备公共属性配置；重复调用NI_MDK_PISP_Init，返回错误码NI_ERR_ISP_HAS_INIT。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_Init(ISP_DEV IspDev);

/**
 * @description: 去初始化PISP设备。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_Exit(ISP_DEV IspDev);

/**
 * @description: 获取PISP设备BLC模块的参数。获取BLC参数时，需指定待获取的BLC ID。若指定的BLC ID大于芯片BLC处理单元个数时，将返回BLC ID为1的BLC单元参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_BLC_PARAM_S} *pstBlcParam: 输出，BLC参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_GetBlcParam(ISP_DEV IspDev, PISP_BLC_PARAM_S *pstBlcParam);

/**
 * @description: 设置PISP设备BLC模块的参数。BLC ID与硬件结构和是否开启HDR宽动态有关：
                 线性模式下，只配置u32BlcId=2时的黑电平值，其他u32BlcId中的黑电平值请设置为默认值0x0；
                 宽动态模式下，u32BlcId=1表示长帧，u32BlcId=2表示短帧，其他u32BlcId中的黑电平值请配置为默认值0x0。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_BLC_PARAM_S} *pstBlcParam: 输入，BLC参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_SetBlcParam(ISP_DEV IspDev, const PISP_BLC_PARAM_S *pstBlcParam);

/**
 * @description: 获取PISP设备DGC模块的参数。获取DGC参数时，需指定待获取的DGC ID。若指定的DGC ID大于芯片DGC处理单元个数时，将返回DGC ID为1的DGC单元参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_DGC_PARAM_S} *pstDgcParam: 输出，DGC参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_GetDgcParam(ISP_DEV IspDev, PISP_DGC_PARAM_S *pstDgcParam);

/**
 * @description: 设置PISP设备DGC的参数。DGC ID与硬件结构和是否开启HDR宽动态有关：
                 线性模式下，只配置u32DgcId=0时的增益值，其他u32DgcId中的增益值请设置为默认值0x100；
                 宽动态模式下，u32DgcId=1表示长帧，u32DgcId=0表示短帧，其他u32DgcId中的增益值请配置为默认值0x100。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_DGC_PARAM_S} *pstDgcParam: 输入，DGC参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_SetDgcParam(ISP_DEV IspDev, const PISP_DGC_PARAM_S *pstDgcParam);

/**
 * @description: 获取PISP设备WB的参数。获取WB参数时，需指定待获取的WB ID。若指定的WB ID大于芯片WB处理单元个数时，将返回WB ID为1的WB单元参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_WB_PARAM_S} *pstWbParam: 输出，WB参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_GetWbParam(ISP_DEV IspDev, PISP_WB_PARAM_S *pstWbParam);

/**
 * @description: 设置PISP设备WB的参数。WB ID与硬件结构和是否开启HDR宽动态有关：
                 线性模式下，只配置u8WbId =1时的增益值，其他u8WbId中的增益值请设置为默认值0x100；
                 宽动态模式下，u8WbId =0表示长帧，u8WbId =1表示短帧，其他u8WbId中的增益值请配置为默认值0x100。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_WB_PARAM_S} *pstWbParam: 输入，WB参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_SetWbParam(ISP_DEV IspDev, const PISP_WB_PARAM_S *pstWbParam);

/**
 * @description: 获取PISP设备LWDR的参数。获取LWDR表时，需对u16AlphaTabLen，u16FiTabLen赋值，对pu16AlphaTab，pu16FiTab分配内存后，再获取。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_LWDR_PARAM_S} *pstLwdrParam: 输出，LWDR参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_GetLwdrParam(ISP_DEV IspDev, PISP_LWDR_PARAM_S *pstLwdrParam);

/**
 * @description: 设置ISP设备LWDR的参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_LWDR_PARAM_S} *pstLwdrParam: 输入，LWDR参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_SetLwdrParam(ISP_DEV IspDev, const PISP_LWDR_PARAM_S *pstLwdrParam);

/**
 * @description: 获取PISP设备GAMMA的参数。获取gamma表时，需对u32TabLen赋值，对pu8GammaTab分配内存后，再获取。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_GAMMA_PARAM_S} *pstGammaParam: 输出，gamma参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_GetGammaParam(ISP_DEV IspDev, PISP_GAMMA_PARAM_S *pstGammaParam);

/**
 * @description: 设置PISP设备GAMMA的参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_GAMMA_PARAM_S} *pstGammaParam: 输入，gamma参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_SetGammaParam(ISP_DEV IspDev, const PISP_GAMMA_PARAM_S *pstGammaParam);

/**
 * @description: 获取PISP设备DPC的参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_DPC_PARAM_S} *pstDpcParam: 输出，DPC参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_GetDpcParam(ISP_DEV IspDev, PISP_DPC_PARAM_S *pstDpcParam);

/**
 * @description: 设置PISP设备DPC的参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_DPC_PARAM_S} *pstDpcParam: 输入，DPC参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_SetDpcParam(ISP_DEV IspDev, const PISP_DPC_PARAM_S *pstDpcParam);   

/**
 * @description: 获取PISP设备GIC的参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_GIC_PARAM_S} *pstGicParam: 输出，GIC参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_GetGicParam(ISP_DEV IspDev, PISP_GIC_PARAM_S *pstGicParam);

/**
 * @description: 设置PISP设备GIC的参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_GIC_PARAM_S} *pstGicParam: 输入，GIC参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_SetGicParam(ISP_DEV IspDev, const PISP_GIC_PARAM_S *pstGicParam);           

/**
 * @description: 获取PISP设备CCM的参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_CCM_PARAM_S} *pstCcmParam: 输出，CCM参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_GetCcmParam(ISP_DEV IspDev, PISP_CCM_PARAM_S *pstCcmParam);

/**
 * @description: 设置PISP设备CCM的参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_CCM_PARAM_S} *pstCcmParam: 输入，CCM参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_SetCcmParam(ISP_DEV IspDev, const PISP_CCM_PARAM_S *pstCcmParam);

/**
 * @description: 获取PISP设备CNR的参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_CNR_PARAM_S} *pstCnrParam: 输出，CNR参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_GetCnrParam(ISP_DEV IspDev, PISP_CNR_PARAM_S *pstCnrParam);

/**
 * @description: 设置PISP设备CNR的参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_CNR_PARAM_S} *pstCnrParam: 输入，CNR参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_SetCnrParam(ISP_DEV IspDev, const PISP_CNR_PARAM_S *pstCnrParam);

/**
 * @description: 获取PISP设备CSC_CDS的参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_CSC_CDS_PARAM_S} *pstCscCdsParam: 输出，CSC_CDS参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_GetCscCdsParam(ISP_DEV IspDev, PISP_CSC_CDS_PARAM_S *pstCscCdsParam);        

/**
 * @description: 设置PISP设备CSC_CDS的参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_CSC_CDS_PARAM_S} *pstCscCdsParam: 输入，CSC_CDS参数。
 * @return {*}: 0：成功，非0：失败
 */                       
NI_S32 NI_MDK_PISP_SetCscCdsParam(ISP_DEV IspDev, const PISP_CSC_CDS_PARAM_S *pstCscCdsParam);                                          

/**
 * @description: 获取PISP设备CFA的参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_CFA_PARAM_S} *pstCfaParam: 输出，CFA参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_GetCfaParam(ISP_DEV IspDev, PISP_CFA_PARAM_S *pstCfaParam);

/**
 * @description: 设置PISP设备CFA的参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_CFA_PARAM_S} *pstCfaParam: 输入，CFA参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_SetCfaParam(ISP_DEV IspDev, const PISP_CFA_PARAM_S *pstCfaParam);

/**
 * @description: 获取PISP设备DRCD的参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_DRCD_PARAM_S} *pstDrcdParam: 输出，DRCD参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_GetDrcdParam(ISP_DEV IspDev, PISP_DRCD_PARAM_S *pstDrcdParam);

/**
 * @description: 设置PISP设备DRCD的参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_DRCD_PARAM_S} *pstDrcdParam: 输入，DRCD参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_SetDrcdParam(ISP_DEV IspDev, const PISP_DRCD_PARAM_S *pstDrcdParam);

/**
 * @description: 获取PISP设备LSC的参数。获取LSC表时，需对u16TabLen赋值，对au16LscTabR，au16LscTabG，au16LscTabB分配内存后，再获取。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_LSC_PARAM_S} *pstLscParam: 输出，LSC参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_GetLscParam(ISP_DEV IspDev, PISP_LSC_PARAM_S *pstLscParam);

/**
 * @description: 设置PISP设备LSC的参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_LSC_PARAM_S} *pstLscParam: 输入，LSC参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_SetLscParam(ISP_DEV IspDev, const PISP_LSC_PARAM_S *pstLscParam);

/**
 * @description: 获取PISP设备3D的参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_NRY3D_PARAM_S} *pstNry3dParam: 输出，LC3D参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_GetNry3dParam(ISP_DEV IspDev, PISP_NRY3D_PARAM_S *pstNry3dParam);

/**
 * @description: 设置PISP设备3D的参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_NRY3D_PARAM_S} *pstNry3dParam: 输入，LC3D参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_SetNry3dParam(ISP_DEV IspDev, const PISP_NRY3D_PARAM_S *pstNry3dParam);

/**
 * @description: 获取PISP设备DOW的参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_DOW_PARAM_S} *pstDowParam: 输出，DOW参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_GetDowParam(ISP_DEV IspDev, PISP_DOW_PARAM_S *pstDowParam);

/**
 * @description: 设置PISP设备DOW的参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_DOW_PARAM_S} *pstDowParam: 输入，DOW参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_SetDowParam(ISP_DEV IspDev, const PISP_DOW_PARAM_S *pstDowParam);

/**
 * @description: 获取PISP设备DOW的状态信息。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_DOW_STATE_INFO_S} *pstDowStateInfo: 输出，DOW状态信息。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_GetDowStateInfo(ISP_DEV IspDev, PISP_DOW_STATE_INFO_S *pstDowStateInfo);

/**
 * @description: 获取PISP设备EE的参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_EE_PARAM_S} *pstEeParam: 输出，EE参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_GetEeParam(ISP_DEV IspDev, PISP_EE_PARAM_S *pstEeParam);

/**
 * @description: 设置PISP设备EE的参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_EE_PARAM_S} *pstEeParam: 输入，EE参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_SetEeParam(ISP_DEV IspDev, const PISP_EE_PARAM_S *pstEeParam);

/**
 * @description: 获取PISP设备AF的参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_AF_PARAM_S} *pstAfParam: 输出，AF参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_GetAfParam(ISP_DEV IspDev, PISP_AF_PARAM_S *pstAfParam);

/**
 * @description: 设置PISP设备AF的参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_AF_PARAM_S} *pstAfParam: 输入，AF参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_SetAfParam(ISP_DEV IspDev, const PISP_AF_PARAM_S *pstAfParam);

/**
 * @description: 获取PISP设备Af数据。当 u32Timeout 设为-1 时，表示阻塞模式，程序一直等待，直到获取到数据才返回。
                 u32Timeout 大于 0 时，表示非阻塞模式，参数的单位是毫秒，指超时时间，在此时间内如果没有获取到图像，则超时返回。
                 该接口需与释放接口 NI_MDK_PISP_ReleaseAfData 配对使用。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_AF_STATISTICS_S} *pstAfData: 输出，AF数据。
 * @param {NI_U32} u32MilliSec: 输入，超时等待时间[ms]。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_GetAfData(ISP_DEV IspDev, PISP_AF_STATISTICS_S *pstAfData, NI_U32 u32MilliSec);

/**
 * @description: 释放PISP设备AF数据。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_AF_STATISTICS_S} *pstAfData: 输入，AF数据。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_ReleaseAfData(ISP_DEV IspDev, PISP_AF_STATISTICS_S *pstAfData);

/**
 * @description: 获取PISP设备YHIST_CONTRAST的参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_YHIST_CONTRAST_S} *pstContrast: 输出，YHIST_CONTRAST参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_GetYhistContrast(ISP_DEV IspDev, PISP_YHIST_CONTRAST_S *pstContrast);

/**
 * @description: 设置PISP设备YHIST_CONTRAST的参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_YHIST_CONTRAST_S} *pstContrast: 输入，YHIST_CONTRAST参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_SetYhistContrast(ISP_DEV IspDev, const PISP_YHIST_CONTRAST_S *pstContrast);

/**
 * @description: 获取PISP设备Yhist的参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_YHIST_PARAM_S} *pstYhistParam: 输出，Yhist参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_GetYhistParam(ISP_DEV IspDev, PISP_YHIST_PARAM_S *pstYhistParam);

/**
 * @description: 设置PISP设备Yhist的参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_YHIST_PARAM_S} *pstYhistParam: 输入，Yhist参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_SetYhistParam(ISP_DEV IspDev, const PISP_YHIST_PARAM_S *pstYhistParam);

/**
 * @description: 获取PISP设备Yhist数据。当 u32Timeout 设为-1 时，表示阻塞模式，程序一直等待，直到获取到数据才返回。
                 u32Timeout 大于 0 时，表示非阻塞模式，参数的单位是毫秒，指超时时间，在此时间内如果没有获取到图像，则超时返回。
                 该接口需与释放接口 NI_MDK_PISP_ReleaseYhistData  配对使用。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_YHIST_STATISTICS_S} *pstYhistData: 输出，Yhist数据。
 * @param {NI_U32} u32MilliSec: 输入，超时等待时间[ms]。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_GetYhistData(ISP_DEV IspDev, PISP_YHIST_STATISTICS_S *pstYhistData, NI_U32 u32MilliSec);

/**
 * @description: 释放PISP设备Yhist数据。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_YHIST_STATISTICS_S} *pstYhistData): 输入，Yhist数据。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_ReleaseYhistData(ISP_DEV IspDev, PISP_YHIST_STATISTICS_S *pstYhistData);

/**
 * @description: 获取PISP设备Awes的参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_AWES_PARAM_S} *pstAwesParam: 输出，Awes参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_GetAwesParam(ISP_DEV IspDev, PISP_AWES_PARAM_S *pstAwesParam);

/**
 * @description: 设置PISP设备Awes的参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_AWES_PARAM_S} *pstAwesParam: 输入，Awes参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_SetAwesParam(ISP_DEV IspDev, const PISP_AWES_PARAM_S *pstAwesParam);

/**
 * @description: 获取PISP设备Awes数据。当 u32Timeout 设为-1 时，表示阻塞模式，程序一直等待，直到获取到数据才返回。
                 u32Timeout 大于 0 时，表示非阻塞模式，参数的单位是毫秒，指超时时间，在此时间内如果没有获取到图像，则超时返回。
                 该接口需与释放接口 NI_MDK_PISP_ReleaseAwesData 配对使用。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_AWES_STATISTICS_S} *pstAwesData: 输出，Awes数据。
 * @param {NI_U32} u32MilliSec: 输入，超时等待时间[ms]。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_GetAwesData(ISP_DEV IspDev, PISP_AWES_STATISTICS_S *pstAwesData, NI_U32 u32MilliSec);

/**
 * @description: 释放PISP设备Awes数据。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_AWES_STATISTICS_S} *pstAwesData): 输入，Awes数据。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_ReleaseAwesData(ISP_DEV IspDev, PISP_AWES_STATISTICS_S *pstAwesData);

/**
 * @description: 获取PISP设备RawHist的参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_RAWHIST_PARAM_S} *pstRawHistParam: 输出，RawHist参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_GetRawHistParam(ISP_DEV IspDev, PISP_RAWHIST_PARAM_S *pstRawHistParam);

/**
 * @description: 设置PISP设备RawHist的参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_RAWHIST_PARAM_S} *pstRawHistParam: 输入，RawHist参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_SetRawHistParam(ISP_DEV IspDev, const PISP_RAWHIST_PARAM_S *pstRawHistParam);

/**
 * @description: 获取PISP设备RawHist数据。当 u32Timeout 设为-1 时，表示阻塞模式，程序一直等待，直到获取到数据才返回。
                 u32Timeout 大于 0 时，表示非阻塞模式，参数的单位是毫秒，指超时时间，在此时间内如果没有获取到图像，则超时返回。
                 该接口需与释放接口 NI_MDK_PISP_ReleaseRawHistData 配对使用。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_RAWHIST_STATISTICS_S} *pstRawHistData: 输出，RawHist数据。
 * @param {NI_U32} u32MilliSec: 输入，超时等待时间[ms]。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_GetRawHistData(ISP_DEV IspDev, PISP_RAWHIST_STATISTICS_S *pstRawHistData, NI_U32 u32MilliSec);

/**
 * @description: 释放PISP设备RawHist数据。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_RAWHIST_STATISTICS_S} *pstRawHistData: 输入，RawHist数据。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_ReleaseRawHistData(ISP_DEV IspDev, PISP_RAWHIST_STATISTICS_S *pstRawHistData);

/**
 * @description: 获取PISP设备宽动态模式。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_HDR_MODE_S} *pstHdrMode: 输出，宽动态模式。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_GetHdrMode(ISP_DEV IspDev, PISP_HDR_MODE_S *pstHdrMode);

/**
 * @description: 设置PISP设备宽动态模式。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_HDR_MODE_S} *pstHdrMode: 输入，宽动态模式。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_SetHdrMode(ISP_DEV IspDev, const PISP_HDR_MODE_S *pstHdrMode);

/**
 * @description: 获取PISP设备模式参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_MOD_PARAM_S} *pstModParam: 输出，模式参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_GetModParam(ISP_DEV IspDev, PISP_MOD_PARAM_S *pstModParam);

/**
 * @description: 设置PISP设备模式参数。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @param {PISP_MOD_PARAM_S} *pstModParam: 输入，模式参数。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_SetModParam(ISP_DEV IspDev,const PISP_MOD_PARAM_S *pstModParam);

/**
 * @description: 设置BT1120模式。
 * @param {ISP_DEV} IspDev: 输入，isp设备号。取值：0
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_PISP_SetBt1120Mode(ISP_DEV IspDev);

#ifdef __cplusplus
}
#endif


#endif



