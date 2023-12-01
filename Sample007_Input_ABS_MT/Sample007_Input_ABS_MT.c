#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/errno.h>

MODULE_LICENSE("GPL");
MODULE_VERSION("V001P001");
MODULE_AUTHOR("HolmesChang");
MODULE_DESCRIPTION("Simple007_Input_ABS_MT");

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

#define INPUT_DEVICE1_NAME	"KEY1"
#define INPUT_DEVICE2_NAME	"KEY2"

struct input_dev * input;
uint8_t value;

static irqreturn_t key_irq (int irq, void * data) {
	local_irq_save(irq_flag);
	
	pr_info("%s\n", ((char *) data));
	
	if (irq == key1_irq_no) {
		pr_info("Key1 Pressed\n");
		
		if (value == 1) value = 0;
		else value = 1;
		
		input_event(input, EV_KEY, BTN_0, value);
		
		input_sync(input);
	}
	
	if (irq == key2_irq_no) {
		pr_info("Key2 Pressed\n");
	}
	
	local_irq_restore(irq_flag);
	
	return IRQ_HANDLED;
	//return 0;
}

static int key_open (struct input_dev * input) {
	pr_info("\n");
	
	return 0;
}

static void key_close (struct input_dev * input) {
	pr_info("\n");
}

static int __init sample007_extint_init (void) {
	int ret;
	
	pr_info("Sample007_EXTINT\n");
	
	pr_info("GPIO Initialization: Begin\n");
	
	if(gpio_request(BUZZER_GPIO_NO, "BUZZER")) return -EACCES;
	if(gpio_request(KEY1_GPIO_NO, "KEY1")) return -EACCES;
	if(gpio_request(KEY2_GPIO_NO, "KEY2")) return -EACCES;
	
	gpio_direction_output(BUZZER_GPIO_NO, 1);
	gpio_set_value(BUZZER_GPIO_NO, 0);
	
	gpio_direction_input(KEY1_GPIO_NO);
	key1_irq_no = gpio_to_irq(KEY1_GPIO_NO);
	request_irq(key1_irq_no, key_irq, IRQF_TRIGGER_FALLING, KEY1_EXTINT_NAME, KEY1_DEVICE_NAME);
	
	gpio_direction_input(KEY2_GPIO_NO);
	key2_irq_no = gpio_to_irq(KEY2_GPIO_NO);
	request_irq(key2_irq_no, key_irq, IRQF_TRIGGER_FALLING, KEY2_EXTINT_NAME, KEY2_DEVICE_NAME);
	
	pr_info("GPIO Initialization: End\n");
	
	pr_info("Input Device Initialization: Begin\n");
	
	value = 1;
	
	input = input_allocate_device();
	if (input == NULL) {
		pr_err("Allocation Input Device: NG\n");
		
		return -ENOMEM;
	}
	
	input->name			= INPUT_DEVICE1_NAME;
	//input->phys = "INPUT_DEVICE1_PHYS";
	input->open			= key_open;
	input->close		= key_close;
	set_bit(EV_KEY, input->evbit);
	set_bit(BTN_0, input->keybit);
	//set_bit(EV_SYN, input->evbit);
	//input->id.bustype	= BUS_HOST;
	input->id.vendor	= 0x0001;
	input->id.product	= 0x0002;
	input->id.version	= 0x0003;
	//input_set_capability(input, EV_KEY, BTN_0);
	
	ret = input_register_device(input);
	if (ret != 0) {
		pr_err("Input Device Registeration: NG\n");
		
		return ret;
	}
	
	pr_info("Input Device Initialization: End\n");
	
	return 0;
}

static void __exit sample007_extint_exit (void) {
	pr_info("Sample007_EXTINT\n");
	
	input_unregister_device(input);
	
	gpio_free(BUZZER_GPIO_NO);
	free_irq(key1_irq_no, KEY1_DEVICE_NAME);
	gpio_free(KEY1_GPIO_NO);
	free_irq(key2_irq_no, KEY2_DEVICE_NAME);
	gpio_free(KEY2_GPIO_NO);
}

module_init(sample007_extint_init);
module_exit(sample007_extint_exit);
