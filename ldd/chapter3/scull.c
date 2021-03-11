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
  long max_size;
  void *data_block;
  struct cdev cdev;
};

int scull_open(struct inode *inode, struct file *filp)
{
  struct scull_dev *scull_device;
  scull_device = container_of(inode->i_cdev, struct scull_dev, cdev);
  filp->private_data = scull_device;
  printk(KERN_ALERT "my_scull_device has been opened");
  return 0;
}

int scull_release(struct inode *inode, struct file *filp)
{
  printk(KERN_ALERT "my_scull_device has been released");
  return 0;
}

ssize_t scull_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
  struct scull_dev* scull_device = filp->private_data;

  // the end
  if(*f_pos >= scull_device->max_size)
    return 0;

  long rest = scull_device->max_size - *f_pos;
  count = count <= rest ? count : rest;
  unsigned long err = copy_to_user(buf, scull_device->data_block + *f_pos, count);
  
  (*f_pos) += count;
  return count;
}


ssize_t scull_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
  struct scull_dev *scull_device = filp->private_data;
  // the end
  if(*f_pos >= scull_device->max_size)
    return 0;

  long rest = scull_device->max_size - *f_pos;
  count = count <= rest ? count : rest;
  unsigned long err = copy_from_user(scull_device->data_block + *f_pos, buf, count);
  
  (*f_pos) += count;
  return count;
}

struct scull_dev *my_scull_device;
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
  // alloc device id
  dev_t dev_id;
  int err = alloc_chrdev_region(&dev_id, 0, 1, "scull");
  if(err < 0) {
    printk(KERN_ALERT "alloc_chrdev_region Failed");
    return err;
  }
  major = MAJOR(dev_id);
  minor = MINOR(dev_id);

  // alloc scull_device
  my_scull_device = kmalloc(sizeof(struct scull_dev), GFP_KERNEL);
  cdev_init(&my_scull_device->cdev, &scull_fops);
  my_scull_device->cdev.owner = THIS_MODULE;

  // alloc data_block space
  my_scull_device->max_size = 4096;
  my_scull_device->data_block = kmalloc(4096, GFP_KERNEL);
  
  // add scull_device->cdev to kernel
  cdev_add(&my_scull_device->cdev, dev_id, 1);
  printk(KERN_ALERT "my_scull_device has been added to kernel");
  return 0;
}

static void scull_exit_module(void)
{
  // release dev id
  dev_t dev_id = MKDEV(major, minor);
  unregister_chrdev_region(dev_id, 1);

  // delete cdev
  cdev_del(&my_scull_device->cdev);
  printk(KERN_ALERT "my_scull_deivce has been removed from kernel");
}


module_init(scull_init_module);
module_exit(scull_exit_module);

MODULE_LICENSE("GPL v3");
