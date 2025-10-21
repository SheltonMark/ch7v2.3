/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NIAL_TTY_H_
#define _NIAL_TTY_H_

struct nial_tty {
    void *current_tty;
    short type;
    short subtype;
};

#define NIAL_TTY_DRIVER_TYPE_SERIAL 0x0003

/**
 * 函数名: nial_get_current_tty
 * 描述: 获取当前tty
 * 输入: 参数       描述
 *      nial_tty    tty设备
 * 返回: 返回值     描述
 *       0          成功
 *      -1          失败
 */
int nial_get_current_tty(struct nial_tty *nial_tty);

/**
 * 函数名: nial_tty_put_char
 * 描述: tty输出字符
 * 输入: 参数       描述
 *      nial_tty    tty设备
 *      ch          需要输出的字符
 * 返回: 返回值     描述
 *       0          成功
 *      <0          失败
 */
int nial_tty_put_char(struct nial_tty *nial_tty, unsigned char ch);

/**
 * 函数名: nial_tty_flush_chars
 * 描述: 刷新tty
 * 输入: 参数       描述
 *      nial_tty    tty设备
 * 返回: 返回值     描述
 *       0          成功
 *      -1          失败
 */
int nial_tty_flush_chars(struct nial_tty *nial_tty);

#endif /* _NIAL_TTY_H_ */
