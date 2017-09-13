#include <linux/config.h>
#include <linux/init.h>
#include <linux/module.h>


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ECEN5013 HW2 Example Kernel Module");
MODULE_AUTHOR("Chase E Stewart");

module_init(example_mod_init);
module_exit(example_mod_exit);

int module_init(void)
{
	printk("Example Module Init");
	return 0;
}

int example_mod_exit(void)
{
	printk("Example Module Exit");
	return 0;
}


