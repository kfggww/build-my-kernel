#include <linux/kernel.h>
#include <linux/module.h>

static int hello_kernel_init(void)
{
  printk(KERN_INFO "Module hello_kernel init, Hello!");
  return 0;
}

static void hello_kernel_exit(void)
{
  printk(KERN_INFO "Module hello_kernel exit, Bye!");
  return;
}

module_init(hello_kernel_init);
module_exit(hello_kernel_exit);

MODULE_AUTHOR("kfggw");
MODULE_LICENSE("GPL v3");
MODULE_DESCRIPTION("Demo to learn Linux module");
MODULE_VERSION("0.01");
