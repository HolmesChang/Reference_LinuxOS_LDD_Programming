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
#include <linux/gpio.h>
#include <linux/errno.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HolmesChang");
MODULE_VERSION("V001P001");
MODULE_DESCRIPTION("Simple002_GPIO_Platform_Driver");

#undef		pr_fmt
#define		pr_fmt(fmt) "%s: " fmt,__func__

#define GPIO_DEVNO_NAME		"GPIO_DEVNO_NAME"
#define GPIO_CLASS_NAME		"GPIO_CLASS"
#define GPIO_DEVICE_NAME	"GPIO_DEVICE"

#define COM				(22)
#define	LED1			(5)
#define	LED2			(6)
#define	LED3			(13)
#define	LED4			(26)
#define	LED5			(12)

#define DEVICE_BUF_SIZE	(8)

static char gpio_status [DEVICE_BUF_SIZE];

dev_t gpio_dev_no;

int gpio_cdev_open (struct inode * ino, struct file * filp) {
	int i;
	
	pr_info("Begin\n");
		
	for (i=0; i<6; i++) {
		gpio_status[i] = '0';
	}
	gpio_status[6] = '\n';
	gpio_status[7] = '\0';
	
	pr_info("End\n");
		
	return 0;
}

int gpio_cdev_release (struct inode * ino, struct file * filp) {
	pr_info("Begin\n");
	
	pr_info("End\n");
	
	return 0;
}

ssize_t gpio_cdev_write (
	struct file * filp,
	const char __user * buf_from,
	size_t count,
	loff_t * fpos) {
	int ret;
	
	pr_info("Begin\n");
	
	if (*fpos >= DEVICE_BUF_SIZE) return -EINVAL;
	
	if (*fpos + count >= DEVICE_BUF_SIZE) count = DEVICE_BUF_SIZE - *fpos;
	
	ret = copy_from_user(gpio_status, buf_from, count);
	if (ret != 0) return -EFAULT;
	
	*fpos += count;
	
	if (gpio_status[0] == '1')			gpio_set_value(COM, 1);
	else if (gpio_status[0] == '0')		gpio_set_value(COM, 0);
	
	if (gpio_status[1] == '1')			gpio_set_value(LED1, 1);
	else if (gpio_status[1] == '0')		gpio_set_value(LED1, 0);
	
	if (gpio_status[2] == '1')			gpio_set_value(LED2, 1);
	else if (gpio_status[2] == '0')		gpio_set_value(LED2, 0);
	
	if (gpio_status[3] == '1')			gpio_set_value(LED3, 1);
	else if (gpio_status[3] == '0')		gpio_set_value(LED3, 0);
	
	if (gpio_status[4] == '1')			gpio_set_value(LED4, 1);
	else if (gpio_status[4] == '0')		gpio_set_value(LED4, 0);
	
	if (gpio_status[5] == '1')			gpio_set_value(LED5, 1);
	else if (gpio_status[5] == '0')		gpio_set_value(LED5, 0);
	
	pr_info("End\n");
	
	return count;
}

ssize_t gpio_cdev_read (
	struct file * filp,
	char __user * buf_to,
	size_t count,
	loff_t * fpos) {
	int ret;
	
	pr_info("Begin\n");
		
	if (*fpos >= DEVICE_BUF_SIZE) return 0;
	
	if ((*fpos + count) > DEVICE_BUF_SIZE) count = DEVICE_BUF_SIZE - *fpos;
	
	printk("%s: After adjusting count, fpos = %llu, count = %d\n", __func__, *fpos, count);
	
	if (gpio_get_value(COM)) gpio_status[0] |= '1'; //(0x01 << 1);
	if (gpio_get_value(LED1)) gpio_status[1] |= '1'; //(0x01 << 2);
	if (gpio_get_value(LED2)) gpio_status[2] |= '1'; //(0x01 << 3);
	if (gpio_get_value(LED3)) gpio_status[3] |= '1'; //(0x01 << 4);
	if (gpio_get_value(LED4)) gpio_status[4] |= '1'; //(0x01 << 5);
	if (gpio_get_value(LED5)) gpio_status[5] |= '1'; //(0x01 << 6);
	
	ret = copy_to_user(buf_to, &gpio_status, count);
	if (ret != 0) return -EIO;
	
	*fpos += count;
	
	pr_info("End\n");
	
	return count;
}

struct cdev gpio_cdev;
struct file_operations gpio_cdev_fops = {
	.owner		= THIS_MODULE,
	.open		= gpio_cdev_open,
	.release	= gpio_cdev_release,
	.write		= gpio_cdev_write,
	.read		= gpio_cdev_read
};

struct class * gpio_class;
struct device * gpio_device;

int gpio_platform_driver_probe (struct platform_device * pdev) {
	pr_info("Begin\n");
	    
    if(gpio_request(COM, "COM")) return -EACCES;
	if(gpio_request(LED1, "LED1")) return -EACCES;
	if(gpio_request(LED2, "LED2")) return -EACCES;
	if(gpio_request(LED3, "LED3")) return -EACCES;
	if(gpio_request(LED4, "LED4")) return -EACCES;
	if(gpio_request(LED5, "LED5")) return -EACCES;
	
	gpio_direction_output(COM, 1);
	gpio_set_value(COM, 1);
	
	gpio_direction_output(LED1, 1);
	gpio_set_value(LED1, 1);
	
	gpio_direction_output(LED2, 1);
	gpio_set_value(LED2, 0);
	
	gpio_direction_output(LED3, 1);
	gpio_set_value(LED3, 1);
	
	gpio_direction_output(LED4, 1);
	gpio_set_value(LED4, 0);
	
	gpio_direction_output(LED5, 1);
	gpio_set_value(LED5, 1);
    
	alloc_chrdev_region(&gpio_dev_no, 0, 1, GPIO_DEVNO_NAME);
	
	cdev_init(&gpio_cdev, &gpio_cdev_fops);
	gpio_cdev.owner = THIS_MODULE;
	
	cdev_add(&gpio_cdev, gpio_dev_no, 1);
	
	gpio_class = class_create(THIS_MODULE, GPIO_CLASS_NAME);
	
	gpio_device = device_create(gpio_class, NULL, gpio_dev_no, NULL, GPIO_DEVICE_NAME);

	pr_info("End\n");
	    
    return 0;
}

int gpio_platform_driver_remove (struct platform_device * pdev) {
    pr_info("Begin\n");
    
    gpio_free(COM);
	gpio_free(LED1);
	gpio_free(LED2);
	gpio_free(LED3);
	gpio_free(LED4);
	gpio_free(LED5);
	
	device_destroy(gpio_class, gpio_dev_no);
	
	class_destroy(gpio_class);
	
	cdev_del(&gpio_cdev);
	
	unregister_chrdev_region(gpio_dev_no, 1);
   	
   	pr_info("End\n");
    
    return 0;
}

struct platform_device_id gpio_platform_device_id_table [] = {
    [0] = {
        //.name   = "gpio_platform_driver_com",
        .name   = "com",
        .driver_data = 0
    },
    [1] = {
        //.name   = "gpio_platform_driver_led1",
        .name   = "led1",
        .driver_data = 1
    },
    [2] = {
        //.name   = "gpio_platform_driver_led2",
        .name   = "led2",
        .driver_data = 2
    },
    [3] = {
        //.name   = "gpio_platform_driver_led3",
        .name   = "led3",
        .driver_data = 3
    },
    [4] = {
        //.name   = "gpio_platform_driver_led4",
        .name   = "led4",
        .driver_data = 4
    },
    [5] = {
        //.name   = "gpio_platform_driver_led5",
        .name   = "led5",
        .driver_data = 5
    },
    {}
};

struct of_device_id gpio_of_device_id_table [] = {
    {.compatible = "of_COM"},
    {.compatible = "of_LED1"},
    {.compatible = "of_LED2"},
    {.compatible = "of_LED3"},
    {.compatible = "of_LED4"},
    {.compatible = "of_LED5"},
    {}
};

struct platform_driver gpio_platform_driver = {
    .driver     = {
        .name   = "NAME_GPIO_PLATFORM_DRIVER",
        .of_match_table = of_match_ptr(gpio_of_device_id_table)
    },
    .id_table   = gpio_platform_device_id_table,
    .probe      = gpio_platform_driver_probe,
    .remove     = gpio_platform_driver_remove
};

static int __init Sample002_GPIO_Platform_Driver_Init (void) {
	pr_info("Begin\n");
		
	platform_driver_register(&gpio_platform_driver);
    
	pr_info("End\n");
	    
	return 0;
}

static void __exit Sample002_GPIO_Platform_Driver_Exit (void) {
	pr_info("Begin\n");
		
	platform_driver_unregister(&gpio_platform_driver);
	
	pr_info("End\n");
}

module_init(Sample002_GPIO_Platform_Driver_Init);
module_exit(Sample002_GPIO_Platform_Driver_Exit);
