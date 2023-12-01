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
#include <linux/spi/spi.h>
#include <linux/errno.h>
#include <linux/of.h>
#include <linux/of_device.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HolmesChang");
MODULE_VERSION("V001P001");
MODULE_DESCRIPTION("Sample006_SPI_Client_Driver_SPI_Platform_Driver");

#undef		pr_fmt
#define		pr_fmt(fmt) "%s: " fmt,__func__

char msg [] = "Hello World";

struct of_device_id my_spi_driver_of_device_id_table [] = {
	{.compatible = "my_spi_driver"},
	{}
};

struct spi_device_id my_spi_device_id_table [] = {
    {.name = "my_spi_driver"},
    {}
};

static int my_spi_driver_probe (struct spi_device * spi) {
	int ret;
	uint32_t speed;
	
	pr_info("Begin\n");
	
	ret = of_property_read_u32(spi->dev.of_node, "spi-max-frequency", &speed);
	if (ret != 0) {
		pr_err("of_property_read_u32(spi->dev.of_node, \"spi-max-frequency\", &speed): NG\n");
	}
	else {
		pr_info("spi-max-frequency = %u\n", speed);
	}
	
	spi->bits_per_word	= 8;
	spi->mode			= SPI_MODE_0;
	spi->max_speed_hz	= 8000000;
	spi->chip_select	= 1;
	
	ret = spi_setup(spi);
	if (ret) {
		pr_err("spi_setup(spi): NG\n");
        
        return ret;
	}
	
    // Testing of spi_write()
	spi_write(spi, msg, sizeof(msg));
    
    pr_info("End\n");
	
	return 0;
}

static int my_spi_driver_remove (struct spi_device * spi) {
	pr_info("Begin\n");
	
	pr_info("End\n");
	
	return 0;
}

struct spi_driver my_spi_driver = {
	.driver = {
		.owner			= THIS_MODULE,
		.name			= "my_spi_driver",
		.of_match_table	= my_spi_driver_of_device_id_table
	},
    .id_table   = my_spi_device_id_table,
	.probe		= my_spi_driver_probe,
	.remove		= my_spi_driver_remove
};

static int __init Sample006_SPI_Client_Driver_SPI_Platform_Driver_Init (void) {
    int ret;
    
	pr_info("Begin\n");

	ret = spi_register_driver(&my_spi_driver);
    if (ret) {
        pr_err("spi_register_driver(&my_spi_driver): NG\n");
        
        return ret;
    }
    
    pr_info("End\n");

	return 0;
}

static void __exit Sample006_SPI_Client_Driver_SPI_Platform_Driver_Exit (void) {
	pr_info("Begin\n");

	spi_unregister_driver(&my_spi_driver);
    
    pr_info("End\n");
}

module_init(Sample006_SPI_Client_Driver_SPI_Platform_Driver_Init);
module_exit(Sample006_SPI_Client_Driver_SPI_Platform_Driver_Exit);
