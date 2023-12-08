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
#include <linux/i2c.h>
#include <linux/errno.h>

MODULE_LICENSE("GPL");
MODULE_VERSION("V001P001");
MODULE_AUTHOR("HolmesChang");
MODULE_DESCRIPTION("Sample005_I2C_Client_Driver_I2C_Platform_Driver");

#undef	pr_fmt
#define	pr_fmt(fmt) "%s: " fmt,__func__

#define MY_I2C_DRIVER_NAME	"my_i2c_driver"
#define MY_I2C_DEVICE_NAME	MY_I2C_DRIVER_NAME

#define BME280_ADDRESS	(0x77)
#define BME280_REG_ID	(0xD0)
#define BME280_REG_DATA	(0xF7)

static uint8_t obuf [] = {0xF4, 0x55, 0xF5, 0xAA};
static uint8_t ibuf [10];

struct of_device_id my_i2c_of_device_id_table [] = {
	{.compatible = MY_I2C_DRIVER_NAME},
	{}
};

struct i2c_device_id my_i2c_device_id_table [] = {
	{.name = MY_I2C_DEVICE_NAME},
	{}
};

static int my_i2c_driver_probe (struct i2c_client * client, const struct i2c_device_id * id) {
	int ret;
	int i;
	
	pr_info("Begin\n");
	
	if (client == NULL) {
		pr_err("NULL client\n");
	}
	
#if 1	
	ret = i2c_check_functionality(client->adapter, I2C_FUNC_I2C);
	if (!ret) {
		pr_err("i2c_check_functionality(client->adapter, I2C_FUNC_I2C): NG\n");
		
		return -EIO;
	}
	
	// Testing001: Testing Of i2c_master_send()
	ret = i2c_master_send(client, obuf, 4);
	pr_info("i2c_master_send: %d\n", ret);
	
	// Testing002: Testing Of i2c_master_recv()
	for (i=0; i<10; i++) pr_info("ibuf[%02d] = 0x%02X\n", i, ibuf[i]);
	ret = i2c_master_send(client, obuf, 1);
	ret = i2c_master_recv(client, ibuf, 2);
	pr_info("i2c_master_recv: %d\n", ret);
	for (i=0; i<10; i++) pr_info("ibuf[%02d] = 0x%02X\n", i, ibuf[i]);
	
	// Testing003: Testing Of i2c_transfer
	obuf[0] = 0xE0;
	obuf[1] = 0xB6;
	ret = i2c_master_send(client, obuf, 2);
	obuf[0] = 0xF7;
	struct i2c_msg msg_i2c [] = {
		{
			.addr = BME280_ADDRESS,
			.flags = 0,
			.len = 1,
			.buf = obuf
		},
		{
			.addr = BME280_ADDRESS,
			.flags = I2C_M_RD,
			.len = 8,
			.buf = ibuf
		}
	};
	i2c_transfer(client->adapter, msg_i2c, 2);
#endif	
	pr_info("End\n");
	
	return 0;
}

static int my_i2c_driver_remove (struct i2c_driver * client) {
	pr_info("Begin\n");
	
	pr_info("End\n");
	
	return 0;
}

struct i2c_driver my_i2c_driver = {
	.driver		= {
		.owner			= THIS_MODULE,
		.name			= MY_I2C_DRIVER_NAME,
		.of_match_table	= my_i2c_of_device_id_table
	},
	.id_table	= my_i2c_device_id_table,
	.probe		= my_i2c_driver_probe,
	.remove		= my_i2c_driver_remove
};

static int __init Sample005_I2C_Client_Driver_I2C_Platform_Driver_Init (void) {
	int ret;
	
	pr_info("Begin\n");
	
	ret = i2c_add_driver(&my_i2c_driver);
	if (ret < 0) {
		pr_err("i2c_add_driver(&my_i2c_driver): NG\n");
		
		return ret;
	}
	
	pr_info("End\n");
	
	return 0;
}

static void __exit Sample005_I2C_Client_Driver_I2C_Platform_Driver_Exit (void) {
	pr_info("Begin\n");
	
	i2c_del_driver(&my_i2c_driver);
	
	pr_info("End\n");
}

module_init(Sample005_I2C_Client_Driver_I2C_Platform_Driver_Init);
module_exit(Sample005_I2C_Client_Driver_I2C_Platform_Driver_Exit);
