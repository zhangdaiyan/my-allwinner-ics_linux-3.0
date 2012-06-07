/*
 * 2011-2012
 * panlong <panlong@allwinnertech.com>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef GPIO_TEST_SW_H_INCLUDED
#define GPIO_TEST_SW_H_INCLUDED

#include <linux/list.h>

#include <linux/spinlock.h>
#include <linux/rwsem.h>
#include <linux/timer.h>


#define GPIO_TEST_BASE 0xf1c20800

#define	PIN_TEST_GPIO_A			0x00
#define	PIN_TEST_GPIO_B			0x01
#define	PIN_TEST_GPIO_C			0x02
#define	PIN_TEST_GPIO_D			0x03
#define	PIN_TEST_GPIO_E			0x04
#define	PIN_TEST_GPIO_F			0x05
#define	PIN_TEST_GPIO_G			0x06

extern struct rw_semaphore gpio_test_sw_list_lock;
extern struct list_head gpio_test_sw_list;

struct gpio_test_sw_platdata {

	unsigned int		flags;
	char				name[16];

};
struct gpio_test_sw_classdev{
	const char	*name;
	int	port;					/*GPIO使用的端口号 1:PA 2:PB .... */
	int port_num;				/*GPIO在当前端口的序号(第几个引脚)  */
	int flags;
	__u32 cfg;
	__u32 data;
	__u32 drv;
	__u32 pull;
	#define SW_TEST_GPIO_SUSPENDED		(1 << 0)
	#define SW_TEST_GPIO_CORE_SUSPENDED		(1 << 16)
	void (*gpio_sw_test_information_get)(unsigned int group , unsigned int num ,
				struct gpio_test_sw_classdev *gpio_sw_cdev);
	void (*gpio_sw_test_information_set)(unsigned int group , unsigned int num ,\
				__u32 cfg, __u32 data,  __u32 drv, __u32 pull, \
				struct gpio_test_sw_classdev *gpio_sw_cdev);
	struct device		*dev;
	struct list_head	 node;

};

extern void gpio_test_sw_classdev_suspend(struct gpio_test_sw_classdev *gpio_sw_cdev);
extern void gpio_test_sw_classdev_resume(struct gpio_test_sw_classdev *gpio_sw_cdev);

extern int gpio_test_sw_classdev_register(struct device *parent,
				struct gpio_test_sw_classdev *gpio_sw_cdev);
extern void gpio_test_sw_classdev_unregister(struct gpio_test_sw_classdev *gpio_sw_cdev);


#endif /* define GPIO_TEST_SW_H_INCLUDED */
