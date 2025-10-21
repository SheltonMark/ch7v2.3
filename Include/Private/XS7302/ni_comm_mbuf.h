/* 
 * ni_comm_mbuf.h -- mbuf模块对外提供的头文件
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */

#ifndef _NI_COMM_MBUF_H_
#define _NI_COMM_MBUF_H_


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */
#include "ni_type.h"
#include "ni_common.h"
#include "ni_errno.h"
#include "ni_define.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */


/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */
/* invlalid video buffer pool ID */
#define NI_ERR_MBUF_INVALID_POOLID      NI_DEF_ERR(NI_ID_MBUF, EN_ERR_LEVEL_ERROR, ERR_INVALID_DEVID)
/* invlalid video buffer block ID */
#define NI_ERR_MBUF_INVALID_BLKID       NI_DEF_ERR(NI_ID_MBUF, EN_ERR_LEVEL_ERROR, ERR_INVALID_DEVID)
/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define NI_ERR_MBUF_ILLEGAL_PARAM       NI_DEF_ERR(NI_ID_MBUF, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)
/* using a NULL point */
#define NI_ERR_MBUF_NULL_PTR            NI_DEF_ERR(NI_ID_MBUF, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)
/* try to enable or initialize system,device or channel, before configing attribute */
#define NI_ERR_MBUF_NOT_CONFIG          NI_DEF_ERR(NI_ID_MBUF, EN_ERR_LEVEL_ERROR, ERR_NOT_CONFIG)
/* operation is not supported by NOW */
#define NI_ERR_MBUF_NOT_SUPPORT         NI_DEF_ERR(NI_ID_MBUF, EN_ERR_LEVEL_ERROR, ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change static attribute */
#define NI_ERR_MBUF_NOT_PERM            NI_DEF_ERR(NI_ID_MBUF, EN_ERR_LEVEL_ERROR, ERR_NOT_PERM)
/* failure caused by malloc memory */
#define NI_ERR_MBUF_NOMEM               NI_DEF_ERR(NI_ID_MBUF, EN_ERR_LEVEL_ERROR, ERR_NOMEM)
/* failure caused by malloc buffer */
#define NI_ERR_MBUF_NOBUF               NI_DEF_ERR(NI_ID_MBUF, EN_ERR_LEVEL_ERROR, ERR_NOBUF)
/* no data in buffer */
#define NI_ERR_MBUF_EMPTY               NI_DEF_ERR(NI_ID_MBUF, EN_ERR_LEVEL_ERROR, ERR_BUF_EMPTY)
/* no buffer for new data */
#define NI_ERR_MBUF_FULL                NI_DEF_ERR(NI_ID_MBUF, EN_ERR_LEVEL_ERROR, ERR_BUF_FULL)
/* not initialize */
#define NI_ERR_MBUF_NOT_INIT            NI_DEF_ERR(NI_ID_MBUF, EN_ERR_LEVEL_ERROR, ERR_SYS_NOTREADY)
/* has initialize */
#define NI_ERR_MBUF_HAS_INITED          NI_DEF_ERR(NI_ID_MBUF, EN_ERR_LEVEL_ERROR, ERR_EXIST)
/* not create */
#define NI_ERR_MBUF_NOT_CREATE          NI_DEF_ERR(NI_ID_MBUF, EN_ERR_LEVEL_ERROR, ERR_UNEXIST)

typedef NI_U32 MBUF_POOL;
typedef NI_U32 MBUF_BLK;

#define MBUF_INVALID_POOLID         (-1UL)
#define MBUF_INVALID_BLKID          (-1UL)


/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */

typedef enum tagMbufUserId
{
	MBUF_UID_VI 		= 0,
	MBUF_UID_VO 		= 1,
	MBUF_UID_VPS		= 2,
	MBUF_UID_VENC 		= 3,
	MBUF_UID_VDEC 		= 4,
	MBUF_UID_VD 		= 5,
	MBUF_UID_H264E		= 6,
	MBUF_UID_JPEGE		= 7,
	MBUF_UID_MDK		= 8,
	MBUF_UID_AI			= 9,
	MBUF_UID_AENC 		= 10,
	MBUF_UID_USER 		= 11,
	MBUF_UID_H265E		= 12,
	MBUF_UID_SCE		= 13,
	MBUF_UID_ISP		= 15,
	MBUF_UID_LDC		= 16,	
	MBUF_UID_FW         = 17,   /* Firmware */
	
    MBUF_UID_BUTT    
} MBUF_UID_E;

typedef struct tagMbufPoolCfg
{
    NI_U32 u32BlkSize;
    NI_U32 u32BlkCnt;
    NI_CHAR acMmzName[MAX_DDR_NAME_LEN];
}MBUF_POOL_CFG_S;

typedef struct tagMBufConfig
{
    NI_U32          u32MaxPoolCnt;     /* max count of pools, (0,MBUF_MAX_POOLS]  */    
    MBUF_POOL_CFG_S astCommPool[MBUF_MAX_COMM_POOLS];
} MBUF_CONF_S;

typedef struct tagMbufPoolInfo
{
    NI_U32 u32PoolId;
    NI_U32 bIsCommPool;
    NI_U32 u32PhyAddr;
    NI_U32 u32VirAddr;
    NI_U32 u32BlkCnt;
    NI_U32 u32BlkSize;
    NI_U32 u32FreeBlkCnt;    
}MBUF_POOL_INFO_S;

typedef struct tagMbufBLK
{
	NI_U32 u32BlkId;		/* 缓存块ID号 */
    NI_U32 u32PhyAddr;		/* 缓存块物理地址 */
    NI_U32 u32VirAddr;		/* 缓存块虚拟地址，如果缓存池未映射虚拟地址，该参数将返回NULL，用户可根据应用场景需要，调用NI_MDK_MBUF_MmapPool接口映射后，可以获取到虚拟地址 */
    NI_U32 u32BlkSize;		/* 缓存块大小 */
	NI_U32 u32PoolId;		/* 缓存块所属缓存池ID号 */
}MBUF_BLK_S;

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

