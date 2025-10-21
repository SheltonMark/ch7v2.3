/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NIAL_PLATFORM_H_
#define _NIAL_PLATFORM_H_

#include <linux/module.h>

#define NIAL_IORESOURCE_MEM 0x00000200
#define NIAL_IORESOURCE_IRQ 0x00000400
#define NIAL_IORESOURCE_REG 0x00000300
#define NIAL_IORESOURCE_IRQ 0x00000400
#define NIAL_IORESOURCE_DMA 0x00000800
#define NIAL_IORESOURCE_BUS 0x00001000

struct nial_of_device_id {
    char        name[32];
    char        type[32];
    char        compatible[128];
    const void *data;
};

struct nial_device_driver {
    const char                     *name;
    const struct nial_of_device_id *of_match_table;
};

typedef struct nial_pm_message {
    int event;
} nial_pm_message_t;

struct nial_platform_drv {
    int (*probe)(void *);
    int (*remove)(void *);
    int (*suspend)(void *, nial_pm_message_t state);
    int (*resume)(void *);
    void (*shutdown)(void *);

    void                     *platform_driver;
    struct nial_device_driver driver;
};

struct nial_resource {
    unsigned long start;
    unsigned long size;
};

/**
 * 函数名: nial_platform_driver_register
 * 描述: 注册platform设备驱动
 * 输入: 参数       描述
 *      drv       驱动信息
 * 返回: 返回值     描述
 *       0          成功
 *      <0          错误码
 */
int nial_platform_driver_register(struct nial_platform_drv *drv);

/**
 * 函数名: nial_platform_driver_unregister
 * 描述: 注销platform设备驱动
 * 输入: 参数       描述
 *      drv       驱动信息
 * 返回: 返回值     描述
 *       无
 */
void nial_platform_driver_unregister(struct nial_platform_drv *drv);

/**
 * 函数名: nial_platform_get_resource
 * 描述: 获取设备的I/O资源
 * 输入: 参数       描述
 *      dev       platform设备信息
 *      num        资源索引号
 * 输出: 参数       描述
 *      mem         资源地址
 * 返回: 返回值     描述
 *       0          成功
 *      <0          错误码
 */
int nial_platform_get_resource(void *dev, unsigned int num, struct nial_resource *mem);

/**
 * 函数名: nial_platform_get_irq
 * 描述: 获取设备的I/O资源
 * 输入: 参数       描述
 *      dev       platform设备信息
 *      num        资源索引号
 * 返回: 返回值     描述
 *       0          成功
 *      <0          错误码
 */
int nial_platform_get_irq(void *dev, unsigned int num);

/**
 * 函数名: nial_platform_get_irq_byname
 * 描述: 获取设备的I/O资源
 * 输入: 参数       描述
 *      dev       platform设备信息
 *      name      中断名称
 * 返回: 返回值     描述
 *       0          成功
 *      <0          错误码
 */
int nial_platform_get_irq_byname(void *dev, const char *name);

/**
 * 函数名: nial_platform_set_drvdata
 * 描述: 设置platform device私有数据
 * 输入: 参数       描述
 *      dev       platform设备信息
 *      data      私有数据指针
 * 返回: 返回值     描述
 *       无
 */
void nial_platform_set_drvdata(void *dev, void *data);

/**
 * 函数名: nial_platform_get_drvdata
 * 描述: 设置platform device私有数据
 * 输入: 参数       描述
 *      dev       platform设备信息
 * 返回: 返回值     描述
 *       返回私有数据指针
 */
void *nial_platform_get_drvdata(void *dev);

/**
 * 函数名: nial_platform_get_device
 * 描述: 获取驱动对应的设备对象
 * 输入: 参数       描述
 *      dev       platform设备信息
 * 返回: 返回值     描述
 *       无
 */
void *nial_platform_get_device(void *dev);

/**
 * 函数名: nial_of_property_read_u32_array
 * 描述: 获取设备对象设备树中的32位数组属性的值
 * 输入: 参数       描述
 *      dev       platform设备信息
 *      propname    属性名称
 *      sz          该属性数组中成员的个数
 * 输出: 参数       描述
 *      out_value   存放获取到的属性数组
 * 返回: 返回值     描述
 *       0          成功
 *       <0         错误码
 */
int               nial_of_property_read_u32_array(void *dev, const char *propname, u32 *out_values, u32 sz);
static inline int nial_of_property_read_u32(void *dev, const char *propname, u32 *out_value)
{
    return nial_of_property_read_u32_array(dev, propname, out_value, 1);
}

#endif /* _NIAL_PLATFORM_H_ */
