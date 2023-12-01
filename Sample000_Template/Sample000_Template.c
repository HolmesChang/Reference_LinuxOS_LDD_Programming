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
MODULE_VERSION("V001P001");
MODULE_AUTHOR("HolmesChang");
MODULE_DESCRIPTION("Sample000_Template");

#undef		pr_fmt
#define		pr_fmt(fmt) "%s: " fmt,__func__

static uint32_t buf_size = 1024;
module_param(buf_size, uint, S_IRUGO);
MODULE_PARM_DESC(buf_size, "LDD Internal Buffer Size");

static uint32_t buf_offset = 0;
module_param(buf_offset, uint, S_IRUGO);
MODULE_PARM_DESC(buf_offset, "LDD Offset Of Internal Buffer");

static uint8_t * buf = NULL;

static int __init Sample000_Template_Init (void) {
	pr_info("Begin\n");
	
	pr_info("buf_size = %d\n", buf_size);
	pr_info("buf_offset = %d\n", buf_offset);
	
	buf = kzalloc(buf_size, GFP_KERNEL);
	if (buf == NULL) {
		pr_err("kzalloc: NG\n");
		
		return -ENOMEM;
	}
	else {
		uint32_t i;
		
		for (i=0; i<buf_size; i++) {
			buf[i] = ((uint8_t) i);
		}
		
		pr_info("buf[%d] = 0x%02X\n", buf_offset, buf[buf_offset]);
	}
	
	pr_info("End\n");
	
	return 0;
}

static void __exit Sample000_Template_Exit (void) {
	pr_info("Begin\n");
	
	if (buf != NULL) {
		kzfree(buf);
	}
	
	pr_info("End\n");
}

module_init(Sample000_Template_Init);
module_exit(Sample000_Template_Exit);
