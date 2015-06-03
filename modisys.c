#include <linux/module.h>
#include <linux/kernel.h>




asmlinkage long dumy(void)
{
	printk(KERN_INFO"!!!!!!!called\n");
	return 0;
}

int init_module(void)
{
	void** sys_call_table = (void**)0xffffffff80215360;
	printk("ori addr :%p content:%p\n", sys_call_table + 20, *(sys_call_table + 20));
	*(sys_call_table + 20) = dumy;
	printk("mod addr :%p content:%p\n", sys_call_table + 20, *(sys_call_table + 20));
	return 0;
}

void cleanup_module(void)
{
}

MODULE_LICENSE("GPL");
