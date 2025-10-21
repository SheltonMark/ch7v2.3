/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NIAL_FOPS_H_
#define _NIAL_FOPS_H_

#define O_NIAL_RO    0
#define O_NIAL_WO    1
#define O_NIAL_RDWR  2
#define O_NIAL_CREAT 100

/**
 * 函数名: nial_klib_fopen
 * 描述: 打开指定文件
 * 输入: 参数       描述
 *      filename         待打开文件的文件名(含路径)
 *      flags       打开文件的标志为
 *      mode        若为创建文件，文件的位图
 * 返回: 返回值     描述
 *      file        文件描述符
 */
void *nial_klib_fopen(const char *filename, int flags, int mode);

/**
 * 函数名: nial_klib_fclose
 * 描述: 关闭指定文件
 * 输入: 参数       描述
 *      filp         对应文件描述符
 * 返回: 返回值     描述
 *      无
 */
void nial_klib_fclose(void *filp);

/**
 * 函数名: nial_klib_fwrite
 * 描述: 向指定文件写数据
 * 输入: 参数       描述
 *      buf         需要写入的数据
 *      len         需要写入数据的长度
 *      filp         对应文件描述符
 * 返回: 返回值     描述
 *      >=0          成功写入的数量
 *      <0          错误码
 */
int nial_klib_fwrite(char *buf, int len, void *filp);

/**
 * 函数名: nial_klib_fread
 * 描述: 读取指定文件中的数据到buf
 * 输入: 参数       描述
 *      buf         读出数据的目的地址
 *      len         需要读出数据的长度
 *      filp         对应文件描述符
 * 返回: 返回值     描述
 *      >=0          成功读出的数量
 *      <0          错误码
 */
int nial_klib_fread(char *buf, unsigned int len, void *filp);

/**
 * 函数名: nial_klib_fsetpos
 * 描述: 设置指定文件当前读写位置
 * 输入: 参数       描述
 *      filp         对应文件描述符
 *      fpos        指定文件位置
 * 返回: 返回值     描述
 *      无
 */
void nial_klib_fsetpos(void *filp, long long fpos);

/**
 * 函数名: nial_klib_fgetpos
 * 描述: 获取指定文件当前读写位置
 * 输入: 参数       描述
 *      filp         对应文件描述符
 * 返回: 返回值     描述
 *      fpos        文件当前读写位置
 */
long long nial_klib_fgetpos(void *filp);

/**
 * 函数名: nial_klib_fgetsize
 * 描述: 获取指定文件的大小
 * 输入: 参数       描述
 *      filp         对应文件描述符
 * 返回: 返回值     描述
 *      fsize        文件的大小
 */
long long nial_klib_fgetsize(void *filp);

#endif /* _NIAL_FOPS_H_ */
