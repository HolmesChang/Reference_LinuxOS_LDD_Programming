#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/stat.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/errno.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HolmesChang");
MODULE_VERSION("V001P001");
MODULE_DESCRIPTION("Simple002_GPIO_Platform_Device");

#undef		pr_fmt
#define		pr_fmt(fmt) "%s: " fmt,__func__

void gpio_platform_device_release (struct device *dev) {
	pr_info("Begin\n");
	
	pr_info("End\n");
}

static struct platform_device gpio_platform_device_1 = {
    //.name       = "gpio_platform_driver_com",
    .name   = "com",
    .id         = 0,
    .dev        = {
        .release    = gpio_platform_device_release,
    }
};

static int __init gpio_platform_device_init (void) {
	pr_info("Begin\n");
	    
    platform_device_register(&gpio_platform_device_1);
    
    pr_info("End\n");
    
    return 0;
}

static void __exit gpio_platform_device_exit (void) {
	pr_info("Begin\n");
	    
    platform_device_unregister(&gpio_platform_device_1);

	pr_info("End\n");
}

module_init(gpio_platform_device_init);
module_exit(gpio_platform_device_exit);
