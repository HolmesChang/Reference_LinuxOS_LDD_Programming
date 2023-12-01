#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/stat.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/errno.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HolmesChang");
MODULE_VERSION("V001P001");
MODULE_DESCRIPTION("Simple001_PCD");

#undef      pr_fmt
#define     pr_fmt(fmt) "%s: " fmt,__func__

#define PCD_DEVNO_NAME      "PCD_DEVNO_NAME"
#define PCD_CLASS_NAME      "PCD_CLASS"
#define PCD_DEVICE_NAME     "PCD_DEVICE"

#define BUF_SIZE_DEFAULT    (1024)
static uint32_t buf_size = BUF_SIZE_DEFAULT;
module_param(buf_size, uint, S_IRUGO);
MODULE_PARM_DESC(buf_size, "LDD Buffer Size");

static uint8_t * buf;

dev_t pcd_dev_no;

int pcd_open (struct inode * inode, struct file * filp) {
    pr_info("Begin\n");
    
    pr_info("End\n");
    
    return 0;
}

int pcd_release (struct inode * inode, struct file * filp) {
    pr_info("Begin\n");
    
    pr_info("End\n");
    
    return 0;
}

ssize_t pcd_read (struct file * filp, char __user * ubuf, size_t count, loff_t * fpos) {
    pr_info("Begin\n");
    
    pr_info("Old count = %d\n", count);
    pr_info("Old fpos = %d\n", *fpos);
    
    if (*fpos >= buf_size) {
    	return 0;
    }
    
    if ((*fpos + count) > buf_size) {
    	count = buf_size - *fpos;
    }
        
    if (copy_to_user(ubuf, (buf + *fpos), count) != 0) {
    	return -EIO;
    } 
    
    *fpos += count;

    pr_info("New count = %d\n", count);
	pr_info("Old fpos = %d\n", *fpos);

    pr_info("End\n");
        
    return count;
}

ssize_t pcd_write (struct file * filp, const char __user * ubuf, size_t count, loff_t * fpos) {
    pr_info("Begin\n");
    
    pr_info("Old count = %d\n", count);
    pr_info("Old fpos = %d\n", *fpos);
    
    if (*fpos >= buf_size) {
    	return -EINVAL;
    }
    
    if ((*fpos + count) > buf_size) {
    	count = buf_size - *fpos;
    }
    
    if (copy_from_user((buf + *fpos), ubuf, count) != 0) {
    	return -EFAULT;
    }
    
    *fpos += count;
    
    pr_info("New count = %d\n", count);
    pr_info("New fpos = %d\n", *fpos);
    
    pr_info("End\n");
    
    return count;
}

loff_t pcd_llseek (struct file * filp, loff_t offset, int whence) {
	loff_t newpos;
	
	pr_info("Begin\n");
	
	switch (whence) {
		case (SEEK_SET): {
			newpos = offset;
			
			break;
		}
		case (SEEK_CUR): {
			newpos = filp->f_pos + offset;
			
			break;
		}
		case (SEEK_END): {
			newpos = buf_size + offset;
			
			break;
		}
		default: {
			return -EINVAL;
		}
	}
	
	if (newpos < 0) {
		return -EINVAL;
	}
	
	filp->f_pos = newpos;
	
	pr_info("End\n");
	
	return newpos;
}

struct cdev pcd_cdev;
struct file_operations pcd_fops = {
    .open       = pcd_open,
    .release    = pcd_release,
    .write      = pcd_write,
    .read       = pcd_read,
    .llseek		= pcd_llseek
};

struct class * pcd_class;
struct device * pcd_device;

static int __init Sample001_PCD_Init (void) {
    pr_info("Begin\n");
    
    pr_info("buf_size = %d\n", buf_size);
    
    buf = kzalloc(buf_size, GFP_KERNEL);
    if (buf == NULL) {
        pr_err("%i: buf_size = kzalloc(buf_size, GFP_KERNEL): NG\n", __LINE__);
        
        return -ENOMEM;
    }
    
    alloc_chrdev_region(&pcd_dev_no, 0, 1, PCD_DEVNO_NAME);
    
    cdev_init(&pcd_cdev, &pcd_fops);
    pcd_cdev.owner = THIS_MODULE;
    
    cdev_add(&pcd_cdev, pcd_dev_no, 1);
    
    pcd_class = class_create(THIS_MODULE, PCD_CLASS_NAME);
    
    pcd_device = device_create(pcd_class, NULL, pcd_dev_no, NULL, PCD_DEVICE_NAME);
    
    pr_info("End\n");
    
    return 0;
}

static void __exit Sample001_PCD_Exit (void) {
    pr_info("Begin\n");
    
    if (buf != NULL) {
        kzfree(buf);
    }
    
    device_destroy(pcd_class, pcd_dev_no);
    
    class_destroy(pcd_class);
    
    cdev_del(&pcd_cdev);
    
    unregister_chrdev_region(pcd_dev_no, 1);
    
    pr_info("End\n");
}

module_init(Sample001_PCD_Init);
module_exit(Sample001_PCD_Exit);
