#include <linux/kernel.h>
#include <linux/module.h>

static int count = 10;
static char *name = "kfggw";

module_param(count, int, S_IRUGO|S_IWUSR);
module_param(name, charp, S_IRUGO|S_IWUSR);

static int hello_init(void)
{
  printk(KERN_ALERT "Module hello init, Hello!");
  while(count--) {
    printk(KERN_ALERT "count: %d, name: %s\n", count, name);
  }
  return 0;
}

static void hello_exit(void)
{
  printk(KERN_ALERT "Module hello exit, Bye!");
  return;
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("kfggw");
MODULE_LICENSE("GPL v3");
MODULE_DESCRIPTION("Demo to learn Linux module");
MODULE_VERSION("0.01");
