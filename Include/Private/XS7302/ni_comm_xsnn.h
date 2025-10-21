/* 
 * ni_comm_xsnn.h -- xsnn模块对外提供的头文件
 *
 * Created by : Chipup Software Team
 * Copyright : (C) 2021-2022 ZheJiang Xinsheng Electronic Technology Co., Ltd. All rights reserved.
 */



#ifndef __NI_COMM_XSNN_H__
#define __NI_COMM_XSNN_H__

#include "ni_errno.h"
#include "ni_common.h"
#include "ni_define.h"
#include "list.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


typedef enum tagXsnnErrCode
{
    ERR_XSNN_HAS_STARTED  = 64,      /* device or channel has been started*/
    ERR_XSNN_NOT_START    = 65,      /* device or channel not start */
    ERR_XSNN_BYPASS_CHN_UNSTART = 66
} XSNN_ERR_CODE_E;
    
/* invlalid device ID */
#define NI_ERR_XSNN_INVALID_HANDLE     NI_DEF_ERR(NI_ID_XSNN, EN_ERR_LEVEL_ERROR, ERR_INVALID_DEVID)
/* invlalid channel ID */
#define NI_ERR_XSNN_INVALID_CHNID     NI_DEF_ERR(NI_ID_XSNN, EN_ERR_LEVEL_ERROR, ERR_INVALID_CHNID)
/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define NI_ERR_XSNN_ILLEGAL_PARAM     NI_DEF_ERR(NI_ID_XSNN, EN_ERR_LEVEL_ERROR, ERR_ILLEGAL_PARAM)
/* channel exists */
#define NI_ERR_XSNN_CHN_EXIST             NI_DEF_ERR(NI_ID_XSNN, EN_ERR_LEVEL_ERROR, ERR_EXIST)
/* channel unexists */
#define NI_ERR_XSNN_CHN_UNEXIST           NI_DEF_ERR(NI_ID_XSNN, EN_ERR_LEVEL_ERROR, ERR_UNEXIST)
/* using a NULL point */
#define NI_ERR_XSNN_NULL_PTR          NI_DEF_ERR(NI_ID_XSNN, EN_ERR_LEVEL_ERROR, ERR_NULL_PTR)
/* try to enable or initialize system,device or channel, before configing attribute */
#define NI_ERR_XSNN_NOT_CONFIG        NI_DEF_ERR(NI_ID_XSNN, EN_ERR_LEVEL_ERROR, ERR_NOT_CONFIG)
/* operation is not supported by NOW */
#define NI_ERR_XSNN_NOT_SUPPORT       NI_DEF_ERR(NI_ID_XSNN, EN_ERR_LEVEL_ERROR, ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change static attribute */
#define NI_ERR_XSNN_NOT_PERM          NI_DEF_ERR(NI_ID_XSNN, EN_ERR_LEVEL_ERROR, ERR_NOT_PERM)
/* failure caused by malloc memory */
#define NI_ERR_XSNN_NOMEM             NI_DEF_ERR(NI_ID_XSNN, EN_ERR_LEVEL_ERROR, ERR_NOMEM)
/* failure caused by malloc buffer */
#define NI_ERR_XSNN_NOBUF             NI_DEF_ERR(NI_ID_XSNN, EN_ERR_LEVEL_ERROR, ERR_NOBUF)
/* no data in buffer */
#define NI_ERR_XSNN_BUF_EMPTY         NI_DEF_ERR(NI_ID_XSNN, EN_ERR_LEVEL_ERROR, ERR_BUF_EMPTY)
/* no buffer for new data */
#define NI_ERR_XSNN_BUF_FULL          NI_DEF_ERR(NI_ID_XSNN, EN_ERR_LEVEL_ERROR, ERR_BUF_FULL)
/* system is not ready,had not initialed or loaded*/
#define NI_ERR_XSNN_SYS_NOTINIT      NI_DEF_ERR(NI_ID_XSNN, EN_ERR_LEVEL_ERROR, ERR_SYS_NOTREADY)
/* failure caused by enc has started encoding */
#define NI_ERR_XSNN_CHN_HAS_STARTED   NI_DEF_ERR(NI_ID_XSNN, EN_ERR_LEVEL_ERROR, ERR_XSNN_HAS_STARTED)
/* failure caused by enc has started encoding */
#define NI_ERR_XSNN_CHN_NOT_START     NI_DEF_ERR(NI_ID_XSNN, EN_ERR_LEVEL_ERROR, ERR_XSNN_NOT_START)
#define NI_ERR_XSNN_BYPASS_CHN_UNSTART NI_DEF_ERR(NI_ID_XSNN, EN_ERR_LEVEL_ERROR, ERR_XSNN_BYPASS_CHN_UNSTART)
#define NI_ERR_XSNN_NOT_DISABLE          NI_DEF_ERR(NI_ID_XSNN, EN_ERR_LEVEL_ERROR, ERR_NOT_DISABLE)


#define XSNN_MAX_DST_ADDR           (4)
#define XSNN_MAX_DEV_NUM            (1)

typedef NI_U32 NI_PHY_ADDR;
typedef NI_U64 NN_HANDLE;
typedef NI_U32 NN_ID_E;

typedef enum tagXsnnWorkStatus
{
    XSNN_WORK_STATUS_IDLE,              /*当前节点空间未被使用*/     
    XSNN_WORK_STATUS_TODO,              /*当前节点空间数据正在被计算或者已经在Todolist里等待被计算*/         
    XSNN_WORK_STATUS_DONE,              /*当前节点空间已经计算成功*/
    XSNN_WORK_STATUS_TIMEOUT,           /*当前节点空间已经计算超时*/
    XSNN_WORK_STATUS_PROCESSING,        /*当前节点空间正在被计算*/
    
    XSNN_WORK_STATUS_BUTT
}XSNN_WORK_STATUS_E;


typedef enum tagXsnnTaskPrio
{
    XSNN_TASK_PRIO_0,                   /* 第一优先级*/
    XSNN_TASK_PRIO_1,                   /* 第二优先级*/
    XSNN_TASK_PRIO_2,                   /* 第三优先级*/
    XSNN_TASK_PRIO_3,                   /* 第四优先级*/
    XSNN_TASK_PRIO_4,                   /* 第五优先级*/
    XSNN_TASK_PRIO_5,                   /* 第六优先级*/
    XSNN_TASK_PRIO_6,                   /* 第七优先级*/
    
    XSNN_TASK_PRIO_BUTT
}XSNN_TASK_PRIO_E;

typedef struct tagXsnnMmzBuf
{
    NI_VOID             *pBuf;
    NI_PHY_ADDR         u32PhyAddr;
    NI_U32              u32BufLen;
    NI_U32              u32DataLen;
} XSNN_MMZ_BUF_S;

typedef struct tagNnForwardByBin
{ 
    NN_ID_E                enNnId;                            /*预留，指定任务运行在哪个core上*/
	NI_U32                 u32NnLayerNum;                     /*cnn layer 层数信息*/
    XSNN_MMZ_BUF_S         stMmzAddrHead;                     /*cnn layer 配置信息存放的地址*/    
    XSNN_MMZ_BUF_S         stMmzAddrCoef;                     /*cnn 权重系数信息存放的地址*/ 
    XSNN_MMZ_BUF_S         stMmzAddrSrc;                      /*cnn 输入图像存放的地址*/    
    XSNN_MMZ_BUF_S         stMmzAddrMidData;                  /*cnn 中间数据存放的地址*/
    XSNN_MMZ_BUF_S         astMmzAddrDst[XSNN_MAX_DST_ADDR];  /*cnn 输出存放的地址*/

    XSNN_TASK_PRIO_E       enPriority;                        /*任务优先级*/
}XSNN_FORWARD_BY_BIN_CTRL_S;

typedef struct tagXsnnWorkResult
{
    XSNN_WORK_STATUS_E    enWorkResult;         /* NN 运算结果 */
    NI_U64                u64RunTime;           /* NN 计算所用时间 */
}XSNN_WORK_RESULT_S;    




#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif  /*  __NI_COMM_XSNN_H__  */

