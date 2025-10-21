/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef __MDK_MBUF_H__
#define __MDK_MBUF_H__

#include "ni_comm_mbuf.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @description: 设置媒体缓存池配置，含公共缓存池配置。本接口必须在NI_MDK_MBUF_Init接口前调用。调用Init后不允许再设置。
 *               若用户不需要使用公共缓存池的功能，只需将第一个MBUF_CONF_S.astCommPool[0]公共缓存池的块数置为0即可。
 * @param {MBUF_CONF_S} *pstMbufConf: 输入，缓存池配置信息
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_MBUF_SetConf(const MBUF_CONF_S *pstMbufConf);

/**
 * @description: 获取已设置的缓存池配置信息。必须先调用NI_MDK_MBUF_SetConf接口设置后才能获取，否则返回失败。
 * @param {MBUF_CONF_S} *pstMbufConf: 输出，缓存池配置信息
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_MBUF_GetConf(MBUF_CONF_S *pstMbufConf);

/**
 * @description:
 * 初始化媒体缓存池模块，并按照NI_MDK_MBUF_SetConf接口中的配置创建公共缓存池。调用本接口前需先调用NI_MDK_MBUF_SetConf接口。
 *               重复调用本接口将返回成功，但实际只有第一次调用是真正的初始化缓存池。
 * @param {MBUF_CONF_S} *pstMbufConf: 输入，缓存池配置信息
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_MBUF_Init(void);

/**
 * @description:
 * 去初始化缓存池模块，将所有私有和公共缓存池资源销毁(强制销毁)。需确保没有任何人在使用缓存池功能时，才能调用本接口，否则会出现未定义的错误。
 *               重复调用本接口将返回成功。
 * @return {*}：0：成功，非0：失败
 */
NI_S32 NI_MDK_MBUF_Exit(void);

/**
 * @description:
 * 创建一个缓存池，缓存池的大小为u32BlkCnt*u32BlkSize，会自动向上对齐到4KB，不用时需要调用NI_MDK_MBUF_DestroyPool接口销毁。
 * @param {NI_U32} u32BlkSize:
 * 输入，缓存块大小，单位:byte，缓存池的总大小应控制在mmz驱动管理的内存范围内，超出MMZ的可用空闲内存将返回失败。
 * @param {NI_U32} u32BlkCnt: 输入，缓存块个数
 * @param {NI_CHAR} *pcMmzName: 输入，mmz分区名，默认传入NULL
 * @return {*}: MBUF_INVALID_POOLID:表创建失败，非MBUF_INVALID_POOLID: 表示创建成功的缓存池ID
 */
MBUF_POOL NI_MDK_MBUF_CreatePool(NI_U32 u32BlkSize, NI_U32 u32BlkCnt, const NI_CHAR *pcMmzName);

/**
 * @description: 销毁缓存池，调用者需确保没有任何人正在使用待销毁的缓存池，否则将可能出现未定义的错误。
 * @param {MBUF_POOL} PoolId: 输入，待销毁的缓存池id号
 * @return {*}: 0：成功，非0:失败
 */
NI_S32 NI_MDK_MBUF_DestroyPool(MBUF_POOL PoolId);

/**
 * @description: 获取缓存池信息
 * @param {MBUF_POOL} PoolId: 输入，有效的缓存池ID
 * @param {MBUF_POOL_INFO_S} *pstPoolInfo: 输出，返回缓存池信息
 * @return {*}: 0：成功，pstPoolInfo中返回有效的信息，非0:失败
 */
NI_S32 NI_MDK_MBUF_GetPoolInfo(MBUF_POOL PoolId, MBUF_POOL_INFO_S *pstPoolInfo);

/**
 * @description:
 * 从公共或私有缓存池中获取一块指定大小的空闲缓存块,调用本接口获取到的缓存块，在不用时，需要调用NI_MDK_MBUF_ReleaseBlock接口释放。
 * @param {MBUF_POOL} PoolId: 输入，缓存池ID号，若为MBUF_INVALID_POOLID则表从公共缓存池中获取
 * @param {NI_U32} u32BlkSize:
 * 输入，期望获取到的缓存大小。当PoolId为指定缓存池时，该参数无效，当PoolId为MBUF_INVALID_POOLID时，需要配置该参数，从公共缓存池组中获取大于等于u32BlkSize的空闲缓存块。
 * @param {NI_CHAR}
 * *pcMmzName：输入，当从公共缓存池中获取时，指定从哪片ddr的公共池子中获取;当从指定缓存池中获取时，该参数无效。
 * @return {*}：MBUF_INVALID_BLKID：获取失败，非MBUF_INVALID_BLKID：返回获取到的id值
 */
MBUF_BLK NI_MDK_MBUF_GetBlock(MBUF_POOL PoolId, NI_U32 u32BlkSize, const NI_CHAR *pcMmzName);

/**
 * @description: 从公共或私有缓存池中获取一块指定大小的空闲缓存块,
 * 调用本接口获取到的缓存块，在不用时，需要调用NI_MDK_MBUF_ReleaseBlock接口释放。
 * @param {MBUF_POOL} PoolId: 输入，缓存池ID号，若为MBUF_INVALID_POOLID则表从公共缓存池中获取
 * @param {NI_U32} u32BlkSize:
 * 输入，期望获取到的缓存大小。当PoolId为指定缓存池时，该参数无效，当PoolId为MBUF_INVALID_POOLID时，需要配置该参数，从公共缓存池组中获取大于等于u32BlkSize的空闲缓存块。
 * @param {NI_CHAR} *pcMmzName:
 * 输入，当从公共缓存池中获取时，指定从哪片ddr的公共池子中获取;当从指定缓存池中获取时，该参数无效。
 * @param {MBUF_BLK_S} *pstMbufBlk: 输出，返回获取到的空闲缓存块信息。
 * @return {*}：0：成功，非0:失败
 */
NI_S32 NI_MDK_MBUF_GetBlock2(MBUF_POOL PoolId, NI_U32 u32BlkSize, const NI_CHAR *pcMmzName, MBUF_BLK_S *pstMbufBlk);

/**
 * @description: 释放缓存块。调用本接口会对缓存块的占用计数做减1操作，等效于NI_MDK_MBUF_UserSub接口。
 * @param {MBUF_BLK} BlkId: 待释放的缓存块ID
 * @return {*}: 0：成功，非0:失败
 */
NI_S32 NI_MDK_MBUF_ReleaseBlock(MBUF_BLK BlkId);

/**
 * @description:
 * 对缓存块占用计数做加1操作。一个缓存块可以被多次占用，不用时，要配对地调用NI_MDK_MBUF_UserSub接口对占用计数做减1操作。
 *               对一个空闲的缓存块做加操作将返回失败。
 * @param {MBUF_BLK} BlkId: 输入，有效的缓存块ID
 * @return {*}：0：成功，非0：失败
 */
NI_S32 NI_MDK_MBUF_UserAdd(MBUF_BLK BlkId);

/**
 * @description:
 * 对缓存块的占用计数做减1操作。当占用计数减为0时，将变成空闲块，以待下一次GetBlock时被分配使用。等效于NI_MDK_MBUF_ReleaseBlock接口。
 *               UserAdd/UserSub, GetBlock/ReleaseBlock接口应配套使用，遵循“谁申请谁释放”的原则。
 * @param {MBUF_BLK} BlkId: 输入，有效的缓存块ID
 * @return {*}: 0：成功，非0：失败
 */
NI_S32 NI_MDK_MBUF_UserSub(MBUF_BLK BlkId);

/**
 * @description: 通过缓存块id号得到物理地址。
 * @param {MBUF_BLK} BlkId: 输入，有效的缓存块ID号
 * @return {*}: 0：失败，非0：返回缓存块物理地址
 */
NI_U32 NI_MDK_MBUF_BlkId2PhysAddr(MBUF_BLK BlkId);

/**
 * @description: 通过缓存块ID，得到所在缓存池ID。
 * @param {MBUF_BLK} BlkId: 输入, 有效的缓存块ID
 * @return {*}: MBUF_INVALID_POOLID:失败，非MBUF_INVALID_POOLID: 缓存块所在的缓存池ID
 */
MBUF_POOL NI_MDK_MBUF_BlkId2PoolId(MBUF_BLK BlkId);

/**
 * @description:
 * 映射缓存池虚拟地址。创建的缓存池默认是未映射虚拟地址的，如果CPU要访问，可调用本接口映射虚拟地址，之后调用NI_MDK_MBUF_GetBlkVirAddr
 *               或者NI_MDK_MBUF_GetBlock2接口可得到用户态虚拟地址。重复映射同一个缓存池将返回成功，实际仅映射1次。
 * @param {MBUF_POOL} PoolId: 输入，缓存池ID
 * @return {*}: 0：映射成功，非0：映射失败
 */
NI_S32 NI_MDK_MBUF_MmapPool(MBUF_POOL PoolId);

/**
 * @description:
 * 释放已映射的缓存池虚拟地址。用户需确保没有任何人再使用缓存池虚拟地址的情况下，才能调用本接口释放虚拟地址，否则将可能发生未定义的错误。
 *               重复调用本接口将返回成功。
 * @param {MBUF_POOL} PoolId: 输入，缓存池ID
 * @return {*}: 0：成功，非0:失败
 */
NI_S32 NI_MDK_MBUF_MunmapPool(MBUF_POOL PoolId);

/**
 * @description: 通过缓存池ID和该缓存池内的任意物理地址，得到对应的已映射的虚拟地址
 * @param {MBUF_POOL} PoolId:
 * 输入，缓存池ID，该缓存池需是已经映射过虚拟地址的，否则会返回失败，可用调用NI_MDK_MBUF_MmapPool接口映射缓存池用户态虚拟地址。
 * @param {NI_U32} u32PhyAddr: 输入，缓存池范围内的物理地址
 * @param {NI_VOID} **ppVirAddr: 输出，该物理地址对应的虚拟地址
 * @return {*}: 0：成功，非0:失败
 */
NI_S32 NI_MDK_MBUF_GetBlkVirAddr(MBUF_POOL PoolId, NI_U32 u32PhyAddr, NI_VOID **ppVirAddr);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
