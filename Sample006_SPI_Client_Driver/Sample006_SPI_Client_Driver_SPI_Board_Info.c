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

MODULE_LICENSE("GPL");
MODULE_VERSION("V001P001");
MODULE_AUTHOR("HolmesChang");
MODULE_DESCRIPTION("Sample006_SPI_Client_Driver_SPI_Board_Info");

struct spi_master * my_spi_master;
static struct spi_device * my_spi_device;
struct spi_board_info my_spi_board_info = {
    .modalias     = "my_spi_driver",
    .bus_num      = 0,
    .chip_select  = 1,
    .mode         = SPI_MODE_0,
    .max_speed_hz = 8000000
};

struct spi_message msg_spi;
struct spi_transfer xfer_spi [2];

static uint8_t obuf [] = {0xE0, 0x25, 0x08, 0x64, 0xA5, 0x30};
static uint8_t ibuf [30];

static int __init Sample006_SPI_Client_Driver_SPI_Board_Info_Init (void) {
    int ret;
    
    pr_info("Begin\n");
    
    my_spi_master = spi_busnum_to_master(my_spi_board_info.bus_num);
    if(my_spi_master == NULL) {
        pr_err("spi_busnum_to_master(my_spi_board_info.bus_num): NG\n");
        
        return -ENODEV;
    }
    
    my_spi_device = spi_new_device(my_spi_master, &my_spi_board_info);
    if(my_spi_device == NULL) {
        pr_err("spi_new_device(my_spi_master, &my_spi_board_info): NG\n");
        
        return -ENODEV;
    }
    
    my_spi_device->bits_per_word = 8;
    
    ret = spi_setup(my_spi_device);
    if(ret)
    {
        pr_err("spi_setup(my_spi_device): NG\n");
        
        spi_unregister_device(my_spi_device);
        
        return -ENODEV;
    }
    
    // Testing Of spi_write()
    ret = spi_write(my_spi_device, obuf, sizeof(obuf));
    if (ret) {
        pr_err("spi_write(my_spi_device, obuf, sizeof(obuf)): NG\n");
        
        return ret;
    }
    
    // Testing Of spi_read()
    ret = spi_read(my_spi_device, ibuf, 10);
    if (ret) {
        pr_err("spi_read(my_spi_device, ibuf, 10): NG\n");
        
        return ret;
    }
    
    // Testing Of spi_write_then_read()
    ret = spi_write_then_read(my_spi_device, obuf, sizeof(obuf), ibuf, 10);
    if (ret) {
        pr_err("spi_write_then_read(my_spi_device, obuf, sizeof(obuf), ibuf, 10): NG\n");
        
        return ret;
    }
    
    // Testing Of spi_sync()
    spi_message_init(&msg_spi);
    memset(xfer_spi, 0, sizeof(xfer_spi));
    
    memset(&xfer_spi[0], 0, sizeof(xfer_spi[0]));
    xfer_spi[0].tx_buf = obuf;
    xfer_spi[0].len = sizeof(obuf);
    spi_message_add_tail(&xfer_spi[0], &msg_spi);
    
    memset(&xfer_spi[1], 0, sizeof(xfer_spi[1]));
    xfer_spi[1].rx_buf = ibuf;
    xfer_spi[1].len = 10;
    spi_message_add_tail(&xfer_spi[1], &msg_spi);
    
    ret = spi_sync(my_spi_device, &msg_spi);
    if (ret) {
        pr_err("spi_sync(my_spi_device, &msg_spi): NG\n");
        
        return ret;
    }
    
    // Testing Of spi_sync_transfer()
    memset(&xfer_spi[0], 0, sizeof(xfer_spi[0]));
    xfer_spi[0].tx_buf = obuf;
    xfer_spi[0].len = sizeof(obuf);
    
    ret = spi_sync_transfer(my_spi_device, &xfer_spi[0], 1);
    if (ret) {
        pr_err("spi_sync_transfer(my_spi_device, &xfer_spi[0], 1): NG\n");
        
        return ret;
    }
    
    pr_info("End\n");
    
    return 0;
}

static void __exit Sample006_SPI_Client_Driver_SPI_Board_Info_Exit (void) { 
    pr_info("Begin\n");
    
    if (my_spi_device) {
        spi_unregister_device( my_spi_device );
    }
    
    pr_info("End\n");
}

module_init(Sample006_SPI_Client_Driver_SPI_Board_Info_Init);
module_exit(Sample006_SPI_Client_Driver_SPI_Board_Info_Exit);
