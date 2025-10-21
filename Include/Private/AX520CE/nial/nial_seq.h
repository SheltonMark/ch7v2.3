/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NIAL_SEQ_H_
#define _NIAL_SEQ_H_

#include "nial_type.h"

/**
 * 函数名: nial_single_open
 * 描述: 打开可重入文件
 * 输入: 参数       描述
 *      file       seq文件描述符
 *      show          显示文件内容函数
 *      data        私有数据
 * 返回: 返回值     描述
 *       0           成功
 *      <0       错误码
 */
int nial_single_open(void *file, int (*show)(void *m, void *p), void *data);

/**
 * 函数名: nial_seq_read
 * 描述: 读取seq序列化文件的内容
 * 输入: 参数       描述
 *      file      seq文件描述符
 *      size        可读的最大字节数
 *      ppos        文件的读取位置
 * 输出: 参数       描述
 *      buf         存储读取到的内容
 * 返回: 返回值     描述
 *       >=0     完成的读取数量
 *      <0       错误码
 */
int nial_seq_read(void *file, char *buf, nial_size_t size, nial_loff_t *ppos);

/**
 * 函数名: nial_seq_lseek
 * 描述: 对seq序列化文件进行偏移
 * 输入: 参数       描述
 *      file       seq文件描述符
 *      offset        偏移量
 *      whence        相对位置/绝对位置
 * 返回: 返回值     描述
 *       >=0     偏移位置
 *      <0       错误码
 */
long long nial_seq_lseek(void *file, nial_loff_t offset, int whence);

/**
 * 函数名: nial_seq_printf
 * 描述: 输出内容到seq序列化文件
 * 输入: 参数       描述
 *      m       seq文件描述符
 *      f        输出内容
 *      ...        可变参数
 * 返回: 返回值     描述
 *       无
 */
void nial_seq_printf(void *m, const char *f, ...);

/**
 * 函数名: nial_single_release
 * 描述: 释放seq序列化文件
 * 输入: 参数       描述
 *      inode       文件索引
 *      file        seq文件描述符
 * 返回: 返回值     描述
 *       0          成功
 *       <0         错误码
 */
int nial_single_release(void *inode, void *file);

#endif /* _NIAL_SEQ_H_ */
