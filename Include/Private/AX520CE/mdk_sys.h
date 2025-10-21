/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _MDK_SYS_H_
#define _MDK_SYS_H_

/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "ni_type.h"
#include "ni_comm_sys.h"
#include "ni_common.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @description:  设置媒体缓存池，本接口用于指定公共缓存池的信息。
 * @param {MDK_SYS_CONF_S} *pstSysConf：输入，媒体缓存池属性。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_SYS_SetConf(const MDK_SYS_CONF_S *pstSysConf);

/**
 * @description:  获取媒体缓存池的配置信息。
 * @param {MDK_SYS_CONF_S} *pstSysConf：输出，媒体缓存池属性。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_SYS_GetConf(MDK_SYS_CONF_S *pstSysConf);

/**
 * @description:  初始化 MDK 平台系统，调用本接口会初始化 VI、 ISP、 VD、 VENC 等模块。
 * @param 无。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_SYS_Init(void);

/**
 * @description:  去初始化 MDK 平台系统，调用本接口会去初始化 VI、 ISP、 VD、 VENC 等模块。
 * @param 无。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_SYS_Exit(void);

/**
 * @description:  数据源到数据接收者的绑定。
 * @param {MDK_CHN_S} *pstSrcChn：输入，源通道。
 * @param {MDK_CHN_S} *pstDestChn：输入，目的通道。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_SYS_Bind(MDK_CHN_S *pstSrcChn, MDK_CHN_S *pstDestChn);

/**
 * @description:  数据源到数据接收者的解绑定。
 * @param {MDK_CHN_S} *pstSrcChn：输入，源通道。
 * @param {MDK_CHN_S} *pstDestChn：输入，目的通道。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_SYS_UnBind(MDK_CHN_S *pstSrcChn, MDK_CHN_S *pstDestChn);

/**
 * @description:  通过数据接收者获取与之绑定的数据源。
 * @param {MDK_CHN_S} *pstDestChn：输入，目的通道。
 * @param {MDK_CHN_S} *pstSrcChn：输出，源通道。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_SYS_GetBindbyDest(MDK_CHN_S *pstDestChn, MDK_CHN_S *pstSrcChn);

/**
 * @description:  向 MMZ 申请一块连续的物理内存（不带cache）。
 * @param {NI_U32} *pu32PhyAddr：输出，申请内存的起始物理地址指针。
 * @param {NI_VOID} **ppVirAddr：输出，申请内存的起始虚拟地址指针。
 * @param {NI_U32} u32Size：输入，申请内存的大小。
 * @param {NI_CHAR} *pszMmbName：输入，给申请的物理内存块命名，如不命名则配为 NULL。
 * @param {NI_CHAR} *pszMmzName：输入，要从哪个 MMZ 分区中申请内存。如MMZ分区中有"anonymous"分区，则本项参数可配为
 * NULL。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_SYS_MmzAlloc(NI_U32 *pu32PhyAddr, NI_VOID **ppVirAddr, NI_U32 u32Size, const NI_CHAR *pszMmbName,
                           const NI_CHAR *pszMmzName);

/**
 * @description:  向 MMZ 申请一块连续的物理内存（带cache）。
 * @param {NI_U32} *pu32PhyAddr：输出，申请内存的起始物理地址指针。
 * @param {NI_VOID} **ppVirAddr：输出，申请内存的起始虚拟地址指针。
 * @param {NI_U32} u32Size：输入，申请内存的大小。
 * @param {NI_CHAR} *pszMmbName：输入，给申请的物理内存块命名，如不命名则配为 NULL。
 * @param {NI_CHAR} *pszMmzName：输入，要从哪个 MMZ 分区中申请内存。如MMZ分区中有"anonymous"分区，则本项参数可配为
 * NULL。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_SYS_MmzAllocCached(NI_U32 *pu32PhyAddr, NI_VOID **ppVirAddr, NI_U32 u32Size, const NI_CHAR *pszMmbName,
                                 const NI_CHAR *pszMmzName);

/**
 * @description:  向 MMZ 释放一块连续的物理内存（带cache）。
 * @param {NI_U32} u32PhyAddr：输入，申请内存的起始物理地址。
 * @param {NI_VOID} *pVirAddr：输入，申请内存的起始虚拟地址。
 * @param {NI_U32} u32Size：输入，申请内存的大小。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_SYS_MmzFlushCache(NI_U32 u32PhyAddr, NI_VOID *pVirAddr, NI_U32 u32Size);

/**
 * @description:  向 MMZ 释放一块连续的物理内存（不带cache）。
 * @param {NI_U32} u32PhyAddr：输入，申请内存的起始物理地址。
 * @param {NI_VOID} *pVirAddr：输入，申请内存的起始虚拟地址。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_SYS_MmzFree(NI_U32 u32PhyAddr, NI_VOID *pVirAddr);

/**
 * @description:  映射一块物理内存，得到用户态虚拟地址（不带cache）。
 * @param {NI_U32} u32PhyAddr：输入，内存的物理地址。
 * @param {NI_U32} u32Size：输入，内存的大小。
 * @return {*}: 0：成功，非0：失败
 */
NI_VOID *NI_MDK_SYS_Mmap(NI_U32 u32PhyAddr, NI_U32 u32Size);

/**
 * @description:  映射一块物理内存，得到用户态虚拟地址（带cache）。
 * @param {NI_U32} u32PhyAddr：输入，内存的物理地址。
 * @param {NI_U32} u32Size：输入，内存的大小。
 * @return {*}: 0：成功，非0：失败
 */
NI_VOID *NI_MDK_SYS_MmapCached(NI_U32 u32PhyAddr, NI_U32 u32Size);

/**
 * @description:  解映射一块物理内存，释放用户态虚拟地址。
 * @param {NI_U32} *pVirAddr：输入，内存的虚拟地址。
 * @param {NI_U32} u32Size：输入，内存的大小。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_SYS_Munmap(NI_VOID *pVirAddr, NI_U32 u32Size);

/**
 * @description:  设置寄存器的值。
 * @param {NI_U32} u32RegAddr：输入，寄存器地址。
 * @param {NI_U32} u32Value：输入，寄存器值。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_SYS_WriteReg(NI_U32 u32RegAddr, NI_U32 u32Value);

/**
 * @description:  获取寄存器的值。
 * @param {NI_U32} u32RegAddr：输入，寄存器地址。
 * @param {NI_U32} u32Value：输出，寄存器值。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_SYS_ReadReg(NI_U32 u32RegAddr, NI_U32 *pu32Value);

/**
 * @description:  设置寄存器的值，寄存器的值为 2 个字节。
 * @param {NI_U32} u32RegAddr：输入，寄存器地址。
 * @param {NI_U16} u32Value：输入，寄存器值。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_SYS_Write16Reg(NI_U32 u32RegAddr, NI_U16 u16Value);

/**
 * @description:  获取寄存器的值，寄存器的值为 2 个字节。
 * @param {NI_U32} u32RegAddr：输入，寄存器地址。
 * @param {NI_U16} *pu16Value：输出，寄存器值。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_SYS_Read16Reg(NI_U32 u32RegAddr, NI_U16 *pu16Value);

/**
 * @description:  将信息写入 log 中。
 * @param {NI_S32} s32Level：输入，打印等级。
 * @param {MOD_ID_E} enModId：输入，模块id。
 * @param {NI_BOOL} bCompare：输入，s32Level 是否与log预设等级比较。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_SYS_LogWrite(NI_S32 s32Level, MOD_ID_E enModId, NI_BOOL bCompare, const char *fmt, ...);

/**
 * @description:  获取芯片硬件信息。
 * @param {SYS_CHIP_HWINFO_S} *pstChipHwInfo：输出，硬件信息。
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_SYS_GetChipHwInfo(SYS_CHIP_HWINFO_S *pstChipHwInfo);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
