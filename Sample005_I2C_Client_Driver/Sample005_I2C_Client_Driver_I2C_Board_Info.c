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
MODULE_DESCRIPTION("Sample005_I2C_Client_Driver_I2C_Board_Info");

#undef	pr_fmt
#define	pr_fmt(fmt) "%s: " fmt,__func__

#define BME280_ADDRESS	(0x77)
#define BME280_REG_ID	(0xD0)
#define BME280_REG_DATA	(0xF7)

struct i2c_adapter * my_i2c_adapter	= NULL;
struct i2c_client * my_i2c_client	= NULL;
struct i2c_board_info my_i2c_board_info = {
	.type		= "my_i2c_device",
	.addr		= BME280_ADDRESS,
};

uint8_t obuf [] = {BME280_REG_DATA};
uint8_t ibuf [10] = {0};

static int __init Sample005_I2C_Client_Driver_I2C_Board_Info_Init (void) {
	int ret;
	int i;
	
	pr_info("Begin\n");
	
	my_i2c_adapter = i2c_get_adapter(1);
	if (my_i2c_adapter == NULL) {
		pr_err("i2c_get_adapter(1): NG\n");
		
		return -EINVAL;
	}
	
	ret = i2c_check_functionality(my_i2c_adapter, I2C_M_NOSTART);
	if (ret) pr_info("I2C_M_NOSTART: OK\n");
	else pr_info("I2C_M_NOSTART: NG\n");
	
	my_i2c_client = i2c_new_device(my_i2c_adapter, &my_i2c_board_info);
	if (my_i2c_client == NULL) {
		pr_err("i2c_new_device(my_i2c_adapter, &my_i2c_board_info): NG\n");
		
		i2c_put_adapter(my_i2c_adapter);
		
		return -EINVAL;
	}
	
	// Testing001: Testing Of i2c_master_send()
	//ret = i2c_master_send(my_i2c_client, obuf, 1);
	
	// Testing002: Testing Of i2c_master_recv()
	//ret = i2c_master_recv(my_i2c_client, ibuf, 8);
	
	// Testing003: Testing Of i2c_transfer
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
	i2c_transfer(my_i2c_client->adapter, msg_i2c, 2);
	
	pr_info("End\n");
	
	return 0;
}

static void __exit Sample005_I2C_Client_Driver_I2C_Board_Info_Exit (void) {
	pr_info("Begin\n");
	
	if (my_i2c_client) {
		i2c_unregister_device(my_i2c_client);
	}
	
	if (my_i2c_adapter) {
		i2c_put_adapter(my_i2c_adapter);
	}
	
	pr_info("End\n");
}

module_init(Sample005_I2C_Client_Driver_I2C_Board_Info_Init);
module_exit(Sample005_I2C_Client_Driver_I2C_Board_Info_Exit);
