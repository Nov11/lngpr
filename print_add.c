#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>

#define SYS_CALL_TAB	0xffffffff80215360
//0xffffffff8020d4f0

int init_module(void)
{
	int i;
	for(i = 0; i < 40; i++){
		printk("num: %d\t%p\t%p\n", i, (void**)SYS_CALL_TAB + i, *((void**)SYS_CALL_TAB + i));
	}
	return 0;
}

void cleanup_module(void)
{
	printk("end\n");
}

MODULE_LICENSE("GPL");

