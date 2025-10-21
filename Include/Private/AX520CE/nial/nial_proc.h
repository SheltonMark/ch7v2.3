/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NIAL_PROC_H_
#define _NIAL_PROC_H_

typedef struct nial_proc_dir_entry {
    char                 name[64];
    void                *proc_parent_entry;
    void                *proc_self_entry;
    struct nial_fileops *nial_fops;
    void                *fops;
} nial_proc_entry_t;

/**
 * 函数名: nial_proc_mkdir
 * 描述: 创建proc文件目录
 * 输入: 参数       描述
 *      proc       proc文件目录信息
 * 返回: 返回值     描述
 *       0          成功
 *      -1          失败
 */
int nial_proc_mkdir(nial_proc_entry_t *proc);

/**
 * 函数名: nial_create_proc
 * 描述: 创建proc文件
 * 输入: 参数       描述
 *      proc       proc文件信息
 * 返回: 返回值     描述
 *       0          成功
 *      <0          错误码
 */
int nial_create_proc(nial_proc_entry_t *proc);

/**
 * 函数名: nial_remove_proc
 * 描述: 移除proc文件/文件目录
 * 输入: 参数       描述
 *      proc       proc文件/文件目录信息
 * 返回: 返回值     描述
 *       无
 */
void nial_remove_proc(nial_proc_entry_t *proc);

#endif /* _NIAL_PROC_H_ */
