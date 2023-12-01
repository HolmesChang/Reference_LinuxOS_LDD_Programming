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
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/errno.h>

MODULE_LICENSE("GPL");
MODULE_VERSION("V001P001");
MODULE_AUTHOR("HolmesChang");
MODULE_DESCRIPTION("Simple003_EXTINT");

#undef		pr_fmt
#define		pr_fmt(fmt) "%s: " fmt,__func__

#define BUZZER_GPIO_NO		(16)
#define KEY1_GPIO_NO		(24)
#define KEY2_GPIO_NO		(23)

#define KEY1_EXTINT_NAME	"KEY1_EXTINT"
#define KEY2_EXTINT_NAME	"KEY2_EXTINT"
#define KEY1_DEVICE_NAME	"KEY1_DEVICE"
#define KEY2_DEVICE_NAME	"KEY2_DEVICE"

static int key1_irq_no;
static int key2_irq_no;
static unsigned long irq_flag;

static irqreturn_t key_irq (int irq, void * data) {
	pr_info("Begin\n");
	
	local_irq_save(irq_flag);
	
	pr_info("%s\n", ((char *) data));
	
	if (irq == key1_irq_no) {
		pr_info("Key1 Pressed\n");
		
		gpio_set_value(BUZZER_GPIO_NO, 0);
	}
	
	if (irq == key2_irq_no) {
		pr_info("Key2 Pressed\n");
		
		gpio_set_value(BUZZER_GPIO_NO, 1);
	}
	
	local_irq_restore(irq_flag);

	pr_info("End\n");
	
	return IRQ_HANDLED;
}

static int __init Sample003_EXTINT_Init (void) {
	int ret;
	
	pr_info("Begin\n");
	
	if(gpio_request(BUZZER_GPIO_NO, "BUZZER")) return -EACCES;
	if(gpio_request(KEY1_GPIO_NO, "KEY1")) return -EACCES;
	if(gpio_request(KEY2_GPIO_NO, "KEY2")) return -EACCES;
	
	ret = gpio_direction_output(BUZZER_GPIO_NO, 1);
	if (ret) {
		pr_err("gpio_direction_output(BUZZER_GPIO_NO, 1): NG\n");
		
		return ret;
	}
	
	gpio_set_value(BUZZER_GPIO_NO, 0);
	
	ret = gpio_direction_input(KEY1_GPIO_NO);
	if (ret) {
		pr_err("gpio_direction_input(KEY1_GPIO_NO): NG\n");
		
		return ret;
	}
	key1_irq_no = gpio_to_irq(KEY1_GPIO_NO);
	if (key1_irq_no < 0) {
		pr_err("gpio_to_irq(KEY1_GPIO_NO): NG\n");
		
		return -EINVAL;
	}
	ret = request_irq(key1_irq_no, key_irq, IRQF_TRIGGER_FALLING, KEY1_EXTINT_NAME, KEY1_DEVICE_NAME);
	if (ret) {
		pr_err("request_irq(key1_irq_no, key_irq, IRQF_TRIGGER_FALLING, KEY1_EXTINT_NAME, KEY1_DEVICE_NAME): NG\n");
		
		return ret;
	}
	
	ret = gpio_direction_input(KEY2_GPIO_NO);
	if (ret) {
		pr_err("gpio_direction_input(KEY2_GPIO_NO): NG\n");
		
		return ret;
	}
	key2_irq_no = gpio_to_irq(KEY2_GPIO_NO);
	if (key2_irq_no < 0) {
		pr_err("gpio_to_irq(KEY2_GPIO_NO): NG\n");
		
		return -EINVAL;
	}
	ret = request_irq(key2_irq_no, key_irq, IRQF_TRIGGER_FALLING, KEY2_EXTINT_NAME, KEY2_DEVICE_NAME);
	if (ret) {
		pr_err("request_irq(key2_irq_no, key_irq, IRQF_TRIGGER_FALLING, KEY2_EXTINT_NAME, KEY2_DEVICE_NAME): NG\n");
		
		return ret;
	}
	
	pr_info("End\n");
	
	return 0;
}

static void __exit Sample003_EXTINT_Exit (void) {
	pr_info("Begin\n");
	
	gpio_free(BUZZER_GPIO_NO);
	free_irq(key1_irq_no, KEY1_DEVICE_NAME);
	gpio_free(KEY1_GPIO_NO);
	free_irq(key2_irq_no, KEY2_DEVICE_NAME);
	gpio_free(KEY2_GPIO_NO);
	
	pr_info("End\n");
}

module_init(Sample003_EXTINT_Init);
module_exit(Sample003_EXTINT_Exit);
