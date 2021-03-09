#include <linux/kernel.h>
#include <linux/module.h>

MODULE_AUTHOR("kfggw");
MODULE_LICENSE("GPL v3");
MODULE_DESCRIPTION("Demo to learn Linux module");
MODULE_VERSION("0.01");

static int hello_init(void)
{
  printk(KERN_ALERT "Module hello init, Hello!");
  return 0;
}

static void hello_exit(void)
{
  printk(KERN_ALERT "Module hello exit, Bye!");
  return;
}

module_init(hello_init);
module_exit(hello_exit);
