#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>


int major;
int Device_Open;
static int d_open(struct inode* inode, struct file* filp)
{
	if(Device_Open){
		return -EBUSY;
	}
	Device_Open++;
	printk(KERN_INFO"device open\n");
	try_module_get(THIS_MODULE);
	return 0;
}

static int d_release(struct inode* inode, struct file* filp)
{
	Device_Open--;
	printk(KERN_INFO"device release\n");
	module_put(THIS_MODULE);
	return 0;
}

static ssize_t d_read(struct file* filp, char __user* buffer, size_t length, loff_t* offset){
	return 0;
}

static ssize_t d_write(struct file* filp, const char __user* buffer, size_t length, loff_t* offset){
	return 0;
}

	asmlinkage long (*ptr_getpid)(void);
static long d_ioctl(struct file* filp, unsigned int ioctl_num, unsigned long ioctl_param){
	printk(KERN_INFO"## in d_ioctl command : %u param: %lu\n", ioctl_num, ioctl_param);
	ptr_getpid = (void*)0xffffffff8023ec6c;
	
	printk("from module, pid: %ld\n", ptr_getpid());
	return 0;
}

static struct file_operations fops = {
	.open = d_open,
	.release = d_release,
	.read = d_read,
	.write = d_write,
	.unlocked_ioctl = d_ioctl
};

int init_module(void)
{
	int ret = 0;
	printk(KERN_INFO"=========in file ioctl Record & Replay=========\n");
	ret = register_chrdev(0, "r&r", &fops);
	if(ret < 0){
		printk(KERN_INFO "Error regs dev\n");
		return ret;
	}
	printk(KERN_INFO"Major num: %d \n", ret);
	major = ret;
	return 0;
}



void cleanup_module(void)
{
	printk(KERN_INFO"unload module\n");
	unregister_chrdev(major, "r&r");
}



MODULE_LICENSE("GPL");
