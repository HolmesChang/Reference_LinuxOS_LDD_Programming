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

#undef		pr_fmt
#define		pr_fmt(fmt) "%s: " fmt,__func__

#define MY_DEV_NO_NAME	"MY_DEV_NO_NAME"
#define MY_CLASS_NAME	"MY_CLASS"
#define MY_DEVICE_NAME	"MY_DEVICE"

static uint32_t pcd_buf_size = 128;

/*
	Declaration Of Device Number
*/
dev_t pcd_dev_no;

/*
	Declaration AND Initialization Of Variable AND Function For:
		Character Device File In /dev VFS
		Class In /sys VFS
*/
int my_cdev_fops_open (struct inode * inode, struct file * filp) {
	pr_info("Begin\n");
	
	pr_info("End\n");
	
	return 0;
}

int my_cdev_fops_release (struct inode * inode, struct file * filp) {
	pr_info("Begin\n");
	
	pr_info("End\n");
	
	return 0;
}

ssize_t my_cdev_fops_write (struct file * filp, const char __user * ubuf, size_t count, loff_t * fpos) {
	pr_info("Begin\n");
	
	pr_info("End\n");
	
	return count;
}

ssize_t my_cdev_fops_read (struct file * filp, char __user * ubuf, size_t count, loff_t * fpos) {
	pr_info("Begin\n");
	
	pr_info("End\n");
	
	return 0;
}

struct cdev my_cdev;
struct file_operations my_cdev_fops = {
	.owner		= THIS_MODULE,
	.open		= my_cdev_fops_open,
	.release	= my_cdev_fops_release,
	.write		= my_cdev_fops_write,
	.read		= my_cdev_fops_read
};

struct class * my_class;
struct device * my_device;

/*
	Variable AND Function For /sys VFS
*/
ssize_t pcd_buf_size_show (struct device * dev, struct device_attribute * attr, char * buf) {
	pr_info("Begin\n");
	
	pr_info("End\n");
	
	return sprintf(buf, "%d\n", pcd_buf_size);
}

ssize_t pcd_buf_size_store (struct device * dev, struct device_attribute * attr, const char * buf, size_t count) {
	pr_info("Begin\n");
	
	sscanf(buf, "%d", &pcd_buf_size);
	
	pr_info("End\n");
	
	return count;
}

DEVICE_ATTR(pcd_buf_size, S_IRUGO | S_IWUSR, pcd_buf_size_show, pcd_buf_size_store);

static int __init Sample008_SYSFS_Init (void) {
	int ret;
	
	pr_info("Begin\n");
	
	ret = alloc_chrdev_region(&pcd_dev_no, 0, 1, MY_DEV_NO_NAME);
	if (ret) {
		pr_err("alloc_chrdev_region(&pcd_dev_no, 0, 1, MY_DEV_NO_NAME): NG");
		
		return ret;
	}
	
	cdev_init(&my_cdev, &my_cdev_fops);
	my_cdev.owner = THIS_MODULE;
	
	ret = cdev_add(&my_cdev, pcd_dev_no, 1);
	if (ret) {
		pr_err("cdev_add(&my_cdev, pcd_dev_no, 1): NG");
		
		goto err_cdev_add;
	}
	
	my_class = class_create(THIS_MODULE, MY_CLASS_NAME);
	if (IS_ERR(my_class)) {
		pr_err("class_create(THIS_MODULE, MY_CLASS_NAME): NG\n");
		
		ret = PTR_ERR(my_class);
		
		goto err_class_create;
	}
	
	my_device = device_create(my_class, NULL, pcd_dev_no, NULL, MY_DEVICE_NAME);
	if (IS_ERR(my_device)) {
		pr_err("device_create(my_class, NULL, pcd_dev_no, NULL, MY_DEVICE_NAME): NG\n");
		
		ret = PTR_ERR(my_device);
		
		goto err_device_create;
	}
	
	ret = sysfs_create_file(&my_device->kobj, &dev_attr_pcd_buf_size.attr);
	if (ret != 0) {
		pr_err("Creating SYSFS File: NG\n");
	}
	
	pr_info("End\n");
	
	return 0;
	
err_device_create:
	class_destroy(my_class);
err_class_create:
	cdev_del(&my_cdev);
err_cdev_add:
	unregister_chrdev_region(pcd_dev_no, 1);
	
	return ret;
}

static void __exit Sample008_SYSFS_Exit (void) {
	pr_info("Begin\n");
	
	device_destroy(my_class, pcd_dev_no);
	
	class_destroy(my_class);
	
	cdev_del(&my_cdev);
	
	unregister_chrdev_region(pcd_dev_no, 1);
	
	pr_info("End\n");
}

module_init(Sample008_SYSFS_Init);
module_exit(Sample008_SYSFS_Exit);
