#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>

struct scull_qset {
  struct scull_qset *next;
  void **data;
};

struct scull_dev {
  int quantum;
  int qset;
  struct scull_qset *data;
  struct cdev cdev;
};

int scull_open(struct inode *inode, struct file *filp)
{
	struct scull_dev *dev; /* device information */

	dev = container_of(inode->i_cdev, struct scull_dev, cdev);
	filp->private_data = dev; /* for other methods */
	printk(KERN_ALERT "open has been called");
      	return 0;          /* success */
}

int scull_release(struct inode *inode, struct file *filp)
{
	return 0;
}

ssize_t scull_read(struct file *filp, char __user *buf, size_t count,
                loff_t *f_pos)
{
  printk(KERN_ALERT "read has been called");
  return count;
}


ssize_t scull_write(struct file *filp, const char __user *buf, size_t count,
                loff_t *f_pos)
{
  printk(KERN_ALERT "write has been called");
  return count;
}

struct scull_dev *scull_device;
int major;
int minor;

struct file_operations scull_fops = {
	.owner =    THIS_MODULE,
	.read =     scull_read,
	.write =    scull_write,
	.open =     scull_open,
	.release =  scull_release,
};

static int scull_init_module(void)
{
  // 申请设备号
  dev_t dev = 0;
  int result = alloc_chrdev_region(&dev, 0, 1, "scull");
  if(result < 0) {
    printk(KERN_ALERT "获取Major失败!");
    return result;
  }

  major = MAJOR(dev);
  minor = MINOR(dev);

  // 创建scull设备
  scull_device = kmalloc(sizeof(struct scull_dev), GFP_KERNEL);
  if(scull_device == 0) {
    result = -ENOMEM;
    return result;
  }

  // 初始化scull设备
  cdev_init(&scull_device->cdev, &scull_fops);
  scull_device->cdev.owner = THIS_MODULE;
  scull_device->cdev.ops = &scull_fops;
  cdev_add(&scull_device->cdev, dev, 1);
  printk(KERN_ALERT "scull_init");
  return 0;
}

static void scull_cleanup_module(void)
{
  dev_t devno = MKDEV(major, minor);
  if(scull_device != 0) {
    cdev_del(&scull_device->cdev);
    kfree(scull_device);
  }
  printk(KERN_ALERT "scull_cleanup");
  unregister_chrdev_region(devno, 1);
}


module_init(scull_init_module);
module_exit(scull_cleanup_module);

MODULE_LICENSE("GPL v3");
