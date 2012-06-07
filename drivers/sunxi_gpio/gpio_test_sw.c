/*
 * 2011-2012
 * panlong <panlong@allwinnertech.com>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <asm/io.h>
#include "gpio_test_sw.h"
#include <mach/memory.h>
#if 1
#define GPIO_SW_DEBUG(fmt...) printk(fmt)
#else
#define GPIO_SW_DEBUG(fmt...) do{} while (0)
#endif

struct gpio_test_sw {
	struct gpio_test_sw_classdev		cdev;
	struct gpio_test_sw_platdata		*pdata;
};

static inline struct gpio_test_sw *pdev_to_gpio(struct platform_device *dev)
{
	return platform_get_drvdata(dev);
}

static inline struct gpio_test_sw *to_gpio(struct gpio_test_sw_classdev *gpio_sw_cdev)
{
	return container_of(gpio_sw_cdev, struct gpio_test_sw, cdev);
}

static __u32 gpio_read_cfg(unsigned int group , unsigned int num)
{
	unsigned int	cfg,cfg_num;
	__u32	group_pin_cfg,single_pin_cfg;
	GPIO_SW_DEBUG("enter gpio_read_cfg \n,GPIO_TEST_BASE is %x \n",GPIO_TEST_BASE);
	cfg				= num / 8;
	cfg_num			= num % 8;
	GPIO_SW_DEBUG("before reading  \n");
	group_pin_cfg	= __raw_readl(GPIO_TEST_BASE + group * 0x24 + cfg * 4 );
	GPIO_SW_DEBUG("after reading  \n");
	single_pin_cfg	= (group_pin_cfg >> ( cfg_num * 4 )) & 0x0f;
	return single_pin_cfg;
}

static void gpio_set_cfg(unsigned int group , unsigned int num , __u32 single_pin_cfg)
{
	unsigned int	cfg,cfg_num;
	unsigned long	group_pin_cfg;
	cfg             = num / 8;
	cfg_num         = num % 8;
	group_pin_cfg   = __raw_readl(GPIO_TEST_BASE + group * 0x24 + cfg * 4 );
	GPIO_SW_DEBUG("origin group_pin_cfg =  %x \n",group_pin_cfg);
	group_pin_cfg	|=  ((single_pin_cfg & 0x07) << ( cfg_num * 4 ));
	GPIO_SW_DEBUG("new group_pin_cfg =  %x \n",group_pin_cfg);
	__raw_writel(group_pin_cfg,GPIO_TEST_BASE + group * 0x24 + cfg * 4 );
}

static __u32 gpio_read_data(unsigned int group , unsigned int num)
{
	__u32	group_pin_data,single_pin_data;
	group_pin_data	= __raw_readl(GPIO_TEST_BASE + group * 0x24 + 0x10 );
	single_pin_data	= (group_pin_data >> num)&0x1;
	return single_pin_data;
}

static void gpio_set_data(unsigned int group , unsigned int num , __u32 single_pin_data)
{
	unsigned long	group_pin_data;
	group_pin_data  = __raw_readl(GPIO_TEST_BASE + group * 0x24 + 0x10 );
	GPIO_SW_DEBUG("origin group_pin_data =  %x \n",group_pin_data);
	group_pin_data	|= ((single_pin_data & 0x1) << num);
	GPIO_SW_DEBUG("new group_pin_data =  %x \n",group_pin_data);
	__raw_writel(group_pin_data,GPIO_TEST_BASE + group * 0x24 + 0x10 );
}

static __u32 gpio_read_drv(unsigned int group , unsigned int num)
{
	unsigned int	group_drv,drv_num;
	__u32	group_pin_drv,single_pin_drv;
	group_drv		= num / 16;
	drv_num			= num % 16;
	group_pin_drv	= __raw_readl(GPIO_TEST_BASE + group * 0x24 + 0x14 + group_drv * 4 );
	single_pin_drv	= (group_pin_drv >> (drv_num * 2)) &0x2;
	return single_pin_drv;
}

static void gpio_set_drv(unsigned int group , unsigned int num , __u32 single_pin_drv)
{
	unsigned int	group_drv,drv_num;
	unsigned long	group_pin_drv;
	group_drv       = num / 16;
	drv_num         = num % 16;
	group_pin_drv   = __raw_readl(GPIO_TEST_BASE + group * 0x24 + 0x14 + group_drv * 4 );
	GPIO_SW_DEBUG("origin group_pin_drv =  %x \n",group_pin_drv);
	group_pin_drv	|= ((single_pin_drv &0x2) << (drv_num * 2));
	GPIO_SW_DEBUG("new group_pin_drv =  %x \n",group_pin_drv);
	__raw_writel(group_pin_drv,GPIO_TEST_BASE + group * 0x24 + 0x14 + group_drv * 4 );

}

static __u32 gpio_read_pull(unsigned int group , unsigned int num)
{
	unsigned int group_pull,pull_num;
	__u32 group_pin_pull,single_pin_pull;
	group_pull		= num / 16;
	pull_num		= num % 16;
	group_pin_pull	= __raw_readl(GPIO_TEST_BASE + group * 0x24 + 0x1c + group_pull * 4 );
	single_pin_pull	= (group_pin_pull >> (pull_num * 2)) &0x2;
	return single_pin_pull;
}

static void gpio_set_pull(unsigned int group , unsigned int num , __u32 single_pin_pull)
{
	unsigned int	group_pull,pull_num;
	unsigned long	group_pin_pull;
	group_pull      = num / 16;
	pull_num        = num % 16;
	group_pin_pull  = __raw_readl(GPIO_TEST_BASE + group * 0x24 + 0x1c + group_pull * 4 );
	GPIO_SW_DEBUG("origin group_pin_pull =  %x \n",group_pin_pull);
	group_pin_pull   |= ((single_pin_pull &0x2) << (pull_num * 2));
	GPIO_SW_DEBUG("new group_pin_pull =  %x \n",group_pin_pull);
	__raw_writel(group_pin_pull,GPIO_TEST_BASE + group * 0x24 + 0x1c + group_pull * 4 );
}

static void gpio_read_all(unsigned int group , unsigned int num,
			struct gpio_test_sw_classdev *gpio_sw_cdev)
{
	__u32 cfg, data, drv, pull;
	GPIO_SW_DEBUG("enter gpio_read_all\n");
	
	cfg		= gpio_read_cfg(group,num);
	data	= gpio_read_data(group,num);
	drv		= gpio_read_drv(group,num);
	pull	= gpio_read_pull(group,num);
	
	gpio_sw_cdev->cfg	= cfg;
	gpio_sw_cdev->data	= data;
	gpio_sw_cdev->drv	= drv;
	gpio_sw_cdev->pull	= pull;
	
	printk("cfg is %d \n",cfg);
	printk("data is %d \n",data);
	printk("drv is %d \n",drv);
	printk("pull is %d \n",pull);
}
static void gpio_set_all(unsigned int group , unsigned int num, \
			__u32 cfg, __u32 data,  __u32 drv, __u32 pull, \
			struct gpio_test_sw_classdev *gpio_sw_cdev)
{
	GPIO_SW_DEBUG("cfg is %d \n",cfg);
	gpio_set_cfg(group,num,cfg);
	GPIO_SW_DEBUG("data is %d \n",data);
	gpio_set_data(group,num,data);
	GPIO_SW_DEBUG("drv is %d \n",drv);
	gpio_set_drv(group,num,drv);
	GPIO_SW_DEBUG("pull is %d \n",pull);
	gpio_set_pull(group,num,pull);
}

static int gpio_test_sw_probe(struct platform_device *dev)
{
	struct gpio_test_sw *gpio;
	struct gpio_test_sw_platdata *pdata = dev->dev.platform_data;
	int ret;
	
	gpio = kzalloc(sizeof(struct gpio_test_sw), GFP_KERNEL);
	GPIO_SW_DEBUG("kzalloc ok !\n");

	if (gpio == NULL) {
		dev_err(&dev->dev, "No memory for device\n");
		return -ENOMEM;
	}

	platform_set_drvdata(dev, gpio);
	GPIO_SW_DEBUG("platform_set_drvdata ok !\n");
	gpio->pdata = pdata;
	gpio->cdev.gpio_sw_test_information_get = gpio_read_all;
	gpio->cdev.gpio_sw_test_information_set = gpio_set_all;
	gpio->cdev.name="gpio_info";

	ret = gpio_test_sw_classdev_register(&dev->dev, &gpio->cdev);
	GPIO_SW_DEBUG("gpio_test_sw_classdev_register ok !\n");
	if (ret < 0) {
		dev_err(&dev->dev, "gpio_test_sw_classdev_register failed\n");
		kfree(gpio);
		return ret;
	}
	GPIO_SW_DEBUG("gpio_test_sw_classdev_register good !\n");
	return 0;
}

static int gpio_test_sw_remove(struct platform_device *dev)
{
	struct gpio_test_sw *gpio = pdev_to_gpio(dev);

	gpio_test_sw_classdev_unregister(&gpio->cdev);

	GPIO_SW_DEBUG("gpio_test_sw_classdev_unregister ok !\n");

	kfree(gpio);

	GPIO_SW_DEBUG("kfree ok !\n");
	return 0;
}

static void gpio_test_sw_release (struct device *dev)
{
	GPIO_SW_DEBUG("gpio_sw_release good !\n");
}

static struct platform_driver gpio_test_sw_driver = {
	.probe		= gpio_test_sw_probe,
	.remove		= gpio_test_sw_remove,

	.driver		= {
		.name		= "gpio_test_sw",
		.owner		= THIS_MODULE,
	},
};

static struct platform_device gpio_test_devs = {
	.name			= "gpio_test_sw",
	.dev ={
		.release	= gpio_test_sw_release,
	},
};

static int __init gpio_test_sw_init(void)
{
	int ret;
	platform_device_register(&gpio_test_devs);

	ret = platform_driver_register(&gpio_test_sw_driver);
	if (ret == 0)
		goto INIT_END;
	platform_device_del(&gpio_test_devs);
INIT_END:
	GPIO_SW_DEBUG("gpio_init finish ! \n");
	return 0;
}

static void __exit gpio_test_sw_exit(void)
{
	platform_driver_unregister(&gpio_test_sw_driver);
	platform_device_unregister(&gpio_test_devs);
	GPIO_SW_DEBUG("gpio_exit finish !  \n");
}
module_init(gpio_test_sw_init);
module_exit(gpio_test_sw_exit);

MODULE_AUTHOR("panlong <panlong@allwinnertech.com>");
MODULE_DESCRIPTION("SW GPIO driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:gpio_sw");
