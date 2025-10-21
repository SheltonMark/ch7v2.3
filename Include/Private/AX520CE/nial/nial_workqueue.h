/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NIAL_WORKQUEUE_H_
#define _NIAL_WORKQUEUE_H_

typedef struct nial_work_struct {
    void *work;
} nial_work_struct_t;

typedef struct nial_workqueue_struct {
    void *workqueue;
} nial_workqueue_struct_t;

typedef void (*nial_work_func_t)(void *work);

enum {
    NIAL_WQ_MEM_RECLAIM = 1 << 3,
    NIAL_WQ_HIGHPRI     = 1 << 4,

    NIAL_WQ_DFL_ACTIVE = 256,
};

/**
 * 函数名: nial_alloc_workqueue
 * 描述: 分配工作队列
 * 输入: 参数       描述
 *      fmt          工作队列名称
 *      flags       工作队列标志
 *      max_active  工作队列数量
 *      nial_wq     工作队列
 * 返回: 返回值     描述
 *       0          成功
 *      -1          失败
 */
int nial_alloc_workqueue(const char *fmt, unsigned int flags, int max_active, nial_workqueue_struct_t *nial_wq);

/**
 * 函数名: nial_destroy_workqueue
 * 描述: 销毁工作队列
 * 输入: 参数       描述
 *      nial_wq     工作队列
 * 返回: 返回值     描述
 *       无
 */
void nial_destroy_workqueue(nial_workqueue_struct_t *nial_wq);

/**
 * 函数名: nial_flush_workqueue
 * 描述: 刷新工作队列，确认其中的工作项完成
 * 输入: 参数       描述
 *      nial_wq     工作队列
 * 返回: 返回值     描述
 *       无
 */
void nial_flush_workqueue(nial_workqueue_struct_t *nial_wq);

/**
 * 函数名: nial_drain_workqueue
 * 描述: 清空工作队列
 * 输入: 参数       描述
 *      nial_wq     工作项
 * 返回: 返回值     描述
 *       无
 */
void nial_drain_workqueue(nial_workqueue_struct_t *work);

/**
 * 函数名: nial_queue_work
 * 描述: 调度工作队列中的工作项
 * 输入: 参数       描述
 *      nial_wq     工作队列
 *      nial_w      工作项
 * 返回: 返回值     描述
 *       无
 */
void nial_queue_work(nial_workqueue_struct_t *nial_wq, nial_work_struct_t *nial_w);

/**
 * 函数名: nial_init_work
 * 描述: 初始化工作队列中的工作项
 * 输入: 参数       描述
 *      work        工作项
 *      func        工作项对应函数
 *      data        私有数据
 * 返回: 返回值     描述
 *       0          成功
 *      -1          失败
 */
int nial_init_work(struct nial_work_struct *work, nial_work_func_t func, void *data);

/**
 * 函数名: nial_destroy_work
 * 描述: 销毁工作项
 * 输入: 参数       描述
 *      nial_wq     工作项
 * 返回: 返回值     描述
 *       无
 */
void nial_destroy_work(struct nial_work_struct *work);

/**
 * 函数名: nial_work_get_private_data
 * 描述: 获取工作项的私有数据
 * 输入: 参数       描述
 *      data     私有数据项
 * 返回: 返回值     描述
 *       ptrdata    私有数据地址
 */
void *nial_work_get_private_data(void *data);

#endif /* _NIAL_WORKQUEUE_H_ */
