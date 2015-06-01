#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/syscalls.h>
#include <linux/unistd.h>
#include <asm/uaccess.h>

#define FILE_PATH	"/root/log"
struct file* filp;
static int __init init(void)
{
	mm_segment_t old_fs;
	filp = filp_open(FILE_PATH, O_RDWR | O_APPEND | O_CREAT, 0644);
	if(IS_ERR(filp)){
		printk(KERN_INFO"ERROR filp_open\n");
		return -1;
	}
	old_fs = get_fs();
	set_fs(KERNEL_DS);

	filp->f_op->write(filp, (char*)"qwertyuiop", 11, &filp->f_pos);
   	set_fs(old_fs);
	return 0;
}

static void __exit ex(void)
{
	filp_close(filp, NULL);
}

module_init(init);
module_exit(ex);
MODULE_LICENSE("GPL");
