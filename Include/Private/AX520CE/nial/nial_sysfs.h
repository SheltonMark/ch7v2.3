/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NIAL_SYSFS_H_
#define _NIAL_SYSFS_H_

#define ATTR_MAX_NUM (6)

typedef struct nial_sysfs_attr {
    const char  *name;
    unsigned int mode;
    nial_ssize_t (*show)(void *, void *, char *);
    nial_ssize_t (*store)(void *, void *, const char *, nial_size_t);
} nial_sysfs_attr_t;

typedef struct nial_sysfs_dev_attr {
    const char       *name;
    nial_sysfs_attr_t attr[ATTR_MAX_NUM];
    int               num;
} nial_sysfs_dev_attr_t;

typedef struct nial_sysfs_dir_entry {
    nial_sysfs_dev_attr_t dev_attr;
    void                 *dev;
    void                 *sysfs;
} nial_sysfs_entry_t;

/**
 * 函数名: nial_sysfs_create
 * 描述: 创建sysfs文件目录
 * 输入: 参数       描述
 *      sysfs       proc文件目录信息
 * 返回: 返回值     描述
 *       0          成功
 *      NULL          失败
 */
int nial_sysfs_create(nial_sysfs_entry_t *sysfs);

/**
 * 函数名: nial_sysfs_destroy
 * 描述: 销毁sysfs文件
 * 输入: 参数       描述
 *      sysfs       sysfs文件信息
 */
void nial_sysfs_destroy(nial_sysfs_entry_t *sysfs);

#endif /* _NIAL_SYSFS_H_ */
