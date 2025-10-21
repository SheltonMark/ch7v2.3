/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NIAL_DEVICE_H_
#define _NIAL_DEVICE_H_

#include "nial_module.h"
#include "nial_wait.h"
#include "nial_type.h"
#include "nial_list.h"

#define DYNAMIC_MAJOR 0
#define DYNAMIC_MINOR 0

/* These are specified by iBCS2 */
#define NIAL_POLLIN   0x0001
#define NIAL_POLLPRI  0x0002
#define NIAL_POLLOUT  0x0004
#define NIAL_POLLERR  0x0008
#define NIAL_POLLHUP  0x0010
#define NIAL_POLLNVAL 0x0020

#define nial_module_driver(nial_driver, nial_register, nial_unregister, priority, ...) \
    static int __init nial_driver##_init(void)                                         \
    {                                                                                  \
        return nial_register(&(nial_driver));                                          \
    }                                                                                  \
    nial_module_init(nial_driver##_init, priority);                                    \
    static void __exit nial_driver##_exit(void)                                        \
    {                                                                                  \
        nial_unregister(&(nial_driver));                                               \
    }                                                                                  \
    nial_module_exit(nial_driver##_exit);

/**
 * 宏名: nial_module_driver
 * 描述: 完成驱动的注册-注销函数声明
 * 输入: 参数       描述
 *      platform_driver         对应platform驱动结构体
 *      priority                驱动优先级，按字符序排，可并列
 */
#define nial_module_platform_driver(platform_driver, priority) \
    nial_module_driver(platform_driver, nial_platform_driver_register, nial_platform_driver_unregister, priority)

struct nial_fileops {
    int (*open)(void *inode, void *file);
    int (*read)(void *file, char *, nial_size_t, long long *);
    int (*write)(void *file, const char *, nial_size_t, long long *);
    unsigned int (*poll)(void *file, void *);
    long long (*llseek)(void *file, long long, int);
    int (*release)(void *inode, void *file);
    long (*unlocked_ioctl)(void *file, unsigned int, unsigned long);
    int (*mmap)(void *file, void *vma);
    // unmap
    int (*fasync)(int, void *file, int);
};

struct nial_dev {
    char                 name[48];   // 创建设备名称
    void                *dev;        // 设备结构体
    int                  major;      // 主设备号
    int                  minor;      // 次设备号
    void                *cdev;       // 字符设备
    unsigned int         devno;      //
    struct nial_fileops *nial_fops;  // 文件操作函数集
    void                *fops;       // 文件操作函数指针

    struct nial_list_head list;  //
};

/**
 * 函数名: nial_dev_register
 * 描述: 注册驱动设备
 * 输入: 参数       描述
 *      nial_dev    驱动设备
 * 返回: 返回值     描述
 *      0         成功
 *      <0        对应错误码
 */
int nial_dev_register(struct nial_dev *nial_dev);

/**
 * 函数名: nial_dev_unregister
 * 描述: 注销驱动设备
 * 输入: 参数       描述
 *      nial_dev    驱动设备
 * 返回: 返回值     描述
 *      无
 */
void nial_dev_unregister(struct nial_dev *nial_dev);

/**
 * 函数名: nial_set_file_private
 * 描述: 设置文件操作的私有数据
 * 输入: 参数       描述
 *      nial_file         对应文件
 *      data        私有数据
 * 返回: 返回值     描述
 *      0           成功
 *      <0          失败
 */
int nial_set_file_private(void *nial_file, void *data);

/**
 * 函数名: nial_get_file_private
 * 描述: 获取文件操作的私有数据
 * 输入: 参数       描述
 *      nial_file         对应文件
 * 返回: 返回值     描述
 *      无
 */
void *nial_get_file_private(void *nial_file);

/**
 * 函数名: nial_file_nonblock
 * 描述: 设置文件属性为非阻塞
 * 输入: 参数       描述
 *      nial_file         对应文件
 * 返回: 返回值     描述
 *      0           失败
 *      非0         成功
 */
int nial_file_nonblock(void *nial_file);

/**
 * 函数名: nial_get_file_name
 * 描述: 获取文件名
 * 输入: 参数       描述
 *      nial_file         对应文件
 * 返回: 返回值     描述
 *      name         文件名
 */
char *nial_get_file_name(void *file);

/**
 * 函数名: nial_poll_wait
 * 描述: 用于轮询等待设备状态变化
 * 输入: 参数       描述
 *      filep         需等待文件的文件描述符
 *      table           轮询表
 *      nial_wait        等待队列
 * 返回: 返回值     描述
 *      val         打印的数量
 */
void nial_poll_wait(void *filep, void *table, nial_wait_t *nial_wait);

#endif /* _NIAL_DEVICE_H_ */
