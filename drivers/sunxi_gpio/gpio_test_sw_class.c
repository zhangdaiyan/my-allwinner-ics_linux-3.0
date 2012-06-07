/*
 * 2011-2012
 * panlong <panlong@allwinnertech.com>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/device.h>
#include <linux/sysdev.h>
#include <linux/timer.h>
#include <linux/err.h>
#include <linux/ctype.h>
#include "gpio_test_sw.h"
static struct class *gpio_test_sw_class;
#if 1
#define gpio_test_sw_DEBUG(fmt...) printk(fmt)
#else
#define gpio_test_sw_DEBUG(fmt...) do{} while (0)
#endif

static ssize_t info_gpio_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct gpio_test_sw_classdev *gpio_test_sw_cdev = dev_get_drvdata(dev);
	return sprintf(buf, "cfg is %d\n data is %d\n drv is %d\n pull is %d\n", 
			gpio_test_sw_cdev->cfg,gpio_test_sw_cdev->data,
			gpio_test_sw_cdev->drv,gpio_test_sw_cdev->pull);
}


static ssize_t info_gpio_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	unsigned int gpio_area,gpio_num;
	struct gpio_test_sw_classdev *gpio_test_sw_cdev = dev_get_drvdata(dev);
	ssize_t ret = -EINVAL;
	char *after;
	const char *start;
	
	char group;
	size_t count;
	start=buf;
	group=*(start+1);
	if((group >= 'a') && (group <= 'i'))
		gpio_area = group - 'a';
	else if((group >= 'A') && (group <= 'I'))
		gpio_area = group - 'A';
	else{
		gpio_area = 10;
		printk("the area is rang a to g !");
		return ret;
	}
	start=buf+2;
	gpio_num = simple_strtoul(start, &after, 10);
	count = after - buf;
	gpio_test_sw_DEBUG("gpio_area is %d \n gpio_num is %d",gpio_area,gpio_num);
	if (isspace(*after))
		count++;
	gpio_test_sw_DEBUG("count is %d \n size is %d\n",count,size);
	if (count == size) {
		ret = count;
		gpio_test_sw_cdev->gpio_sw_test_information_get(gpio_area, gpio_num,gpio_test_sw_cdev);
	}
	return ret;
}
/*
static ssize_t info_gpio_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	unsigned int gpio_area,gpio_num;
	struct gpio_test_sw_classdev *gpio_test_sw_cdev = dev_get_drvdata(dev);
	ssize_t ret = -EINVAL;
	char *after;
	const char *start;
	const char num[2];
	char group;
	size_t count;
	__u32 cfg, data, drv, pull;
	start=buf;
	group=*(start+1);
	if((group >= 'a') && (group <= 'i'))
		gpio_area = group - 'a';gpio_area = group - 'a';
	else if((group >= 'A') && (group <= 'I'))
		gpio_area = group - 'A';
	else{
		gpio_area = 10;
		printk("the area is rang a to g !");
		return ret;
	}
	
	gpio_num	= *(start+2) * 10 + *(start+3);
	cfg			= *(start+4);
	data		= *(start+5);
	drv			= *(start+6);
	pull		= *(start+7);

}

*/

void gpio_test_sw_classdev_suspend(struct gpio_test_sw_classdev *gpio_test_sw_cdev)
{
	gpio_test_sw_cdev->flags |= SW_TEST_GPIO_SUSPENDED;
	printk("gpio_test_sw_classdev_suspend OK!");
}
EXPORT_SYMBOL_GPL(gpio_test_sw_classdev_suspend);

void gpio_test_sw_classdev_resume(struct gpio_test_sw_classdev *gpio_test_sw_cdev)
{
	printk("gpio_test_sw_classdev_resume OK!");
	gpio_test_sw_cdev->flags &= ~SW_TEST_GPIO_SUSPENDED;
}
EXPORT_SYMBOL_GPL(gpio_test_sw_classdev_resume);

static int gpio_test_sw_suspend(struct device *dev, pm_message_t state)
{
	struct gpio_test_sw_classdev *gpio_test_sw_cdev = dev_get_drvdata(dev);

	if (gpio_test_sw_cdev->flags & SW_TEST_GPIO_CORE_SUSPENDED)
		gpio_test_sw_classdev_suspend(gpio_test_sw_cdev);

	return 0;
}

static int gpio_test_sw_resume(struct device *dev)
{
	struct gpio_test_sw_classdev *gpio_test_sw_cdev = dev_get_drvdata(dev);

	if (gpio_test_sw_cdev->flags & SW_TEST_GPIO_CORE_SUSPENDED)
		gpio_test_sw_classdev_resume(gpio_test_sw_cdev);

	return 0;
}



static struct device_attribute gpio_test_sw_class_attrs[] = {
	__ATTR(info_gpio, 0660, info_gpio_show,info_gpio_store),
	//__ATTR(set_gpio, 0660, info_gpio_show,set_gpio_store),
	__ATTR_NULL,
};



int gpio_test_sw_classdev_register(struct device *parent, struct gpio_test_sw_classdev *gpio_test_sw_cdev)
{
	gpio_test_sw_cdev->dev = device_create(gpio_test_sw_class, parent, 0, gpio_test_sw_cdev,
				      "%s", gpio_test_sw_cdev->name);
	if (IS_ERR(gpio_test_sw_cdev->dev))
		return PTR_ERR(gpio_test_sw_cdev->dev);
	down_write(&gpio_test_sw_list_lock);
	list_add_tail(&gpio_test_sw_cdev->node, &gpio_test_sw_list);
	up_write(&gpio_test_sw_list_lock);
	printk(KERN_DEBUG "Registered gpio_test_sw device: %s\n",
			gpio_test_sw_cdev->name);

	return 0;
}
EXPORT_SYMBOL_GPL(gpio_test_sw_classdev_register);

void gpio_test_sw_classdev_unregister(struct gpio_test_sw_classdev *gpio_test_sw_cdev)
{


	device_unregister(gpio_test_sw_cdev->dev);
	down_write(&gpio_test_sw_list_lock);
	list_del(&gpio_test_sw_cdev->node);
	up_write(&gpio_test_sw_list_lock);
}
EXPORT_SYMBOL_GPL(gpio_test_sw_classdev_unregister);

static int __init gpio_test_sw_init(void)
{
	gpio_test_sw_class = class_create(THIS_MODULE, "gpio_test_sw");
	if (IS_ERR(gpio_test_sw_class))
		return PTR_ERR(gpio_test_sw_class);
	gpio_test_sw_class->suspend = gpio_test_sw_suspend;
	gpio_test_sw_class->resume = gpio_test_sw_resume;
	gpio_test_sw_class->dev_attrs = gpio_test_sw_class_attrs;
	return 0;
}

static void __exit gpio_test_sw_exit(void)
{
	class_destroy(gpio_test_sw_class);
}

subsys_initcall(gpio_test_sw_init);
module_exit(gpio_test_sw_exit);

MODULE_AUTHOR("panlong <panlong@allwinnertech.com>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("sw_gpio Class Interface");







