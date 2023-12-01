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
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/sysfs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HolmesChang");
MODULE_VERSION("V001P001");
MODULE_DESCRIPTION("Sample008_SYSFS");

// Step01: Creation Of Character Device Number //Registration Of Character Device
#define PCD_DEVNO_NAME	"PCD_DEVNO_NAME"
#define PCD_CLASS_NAME	"PCD_CLASS"
#define PCD_DEVICE_NAME	"PCD_DEVICE"

dev_t pcd_dev_no;

// Step02: Registration Of Character Device To Linux VFS
// Step02_01: Initialization Of Character Device With File Operations
struct cdev pcd_cdev;
struct file_operations pcd_fops;

struct class * pcd_class;
struct device * pcd_device;

static uint32_t pcd_buf_size = 128;

ssize_t pcd_buf_size_show (struct device * dev, struct device_attribute * attr, char * buf) {
	pr_info("Begin\n");
	
	return sprintf(buf, "%d\n", pcd_buf_size);
}

ssize_t pcd_buf_size_store (struct device * dev, struct device_attribute * attr, const char * buf, size_t count) {
	pr_info("Begin\n");
	
	sscanf(buf, "%d", &pcd_buf_size);
	
	return count;
}

DEVICE_ATTR(pcd_buf_size, S_IRUGO | S_IWUSR, pcd_buf_size_show, pcd_buf_size_store);

static int __init PCD_Init (void) {
	int ret;
	
	printk("%s: Device Driver Initialization\n", __func__);
	
	// Step01: Creation Of Character Device Number //Registration Of Character Device
	alloc_chrdev_region(&pcd_dev_no, 0, 1, PCD_DEVNO_NAME);
	
	// Step02: Registration Of Character Device To Linux VFS
	// Step02_01: Initialization Of Character Device With File Operaitons
	cdev_init(&pcd_cdev, &pcd_fops);
	pcd_cdev.owner = THIS_MODULE;
	
	// Step02_02: Adding Character Device To Linux VFS
	cdev_add(&pcd_cdev, pcd_dev_no, 1);
	
	pcd_class = class_create(THIS_MODULE, PCD_CLASS_NAME);
	
	pcd_device = device_create(pcd_class, NULL, pcd_dev_no, NULL, PCD_DEVICE_NAME);
	
	// Step03: Adding Device Attribute To SYSFS
	ret = sysfs_create_file(&pcd_device->kobj, &dev_attr_pcd_buf_size.attr);
	if (ret != 0) {
		pr_err("Creating SYSFS File: NG\n");
	}
	
	return 0;
}

static void __exit PCD_Exit (void) {
	printk("%s: Device Driver Finalization\n", __func__);
	
	device_destroy(pcd_class, pcd_dev_no);
	
	class_destroy(pcd_class);
	
	cdev_del(&pcd_cdev);
	
	unregister_chrdev_region(pcd_dev_no, 1);
}

module_init(PCD_Init);
module_exit(PCD_Exit);
