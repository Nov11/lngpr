#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/stat.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/highmem.h>
#include <asm/unistd.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/syscalls.h>
#include <linux/string.h>
#include "command.h"

//#define IOCTL_RESET		0x0
//#define IOCTL_SET_PID	0x1
//#define IOCTL_GET_PID	0x2
//#define IOCTL_START		0x3
//#define IOCTL_STOP		0x4
//#define SYS_CALL_TABLE	0xffffffff8161d3c0

#define SYS_TAB_TWO	0xffffffff8020d4f0
//this is for 64 abi not for sure!!!
//#define SYS_TAB_ONE	0xffffffff8020dcb8
//0xffffffff8020d4f0 grep from sysmap
// t sys_call_table
#define SYS_TAB_ONE	0xffffffff80215360
//above is for O32abi not for sure!!
//#define SYS_TAB_TWO	0xffffffff802162d8
//0xffffffff80215360 grep from sysmap
//t sys_call_table
MODULE_LICENSE("GPL");
char* program = "getpid";
module_param(program, charp, S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(program, "program to be record");

int sys_call_tab_chged;
int major;
int record_pid;
int replay_pid;
int Device_Open = 0;
int to_record;
int to_replay;
char* file_name = "/home/loongson/lngpr/rrlog";
void** systable[2] = {(void**)SYS_TAB_ONE, (void**)SYS_TAB_TWO};
int cnt = 1;

//tact 64abi only
//5000 is the offset specified in 
//arch/mips/include/uapi/asm/unistd.h for kernel 3.10
//the original __NR_getpid is (__NR_linux + 38)
//since for mips the offset I need not include __NR_linux
//I take that off here
int getpid_offset = 20;//get this by experi  not know why
//__NR_getpid - 5000; 

//asmlinkage int (*ori_getpid)(void) = *(systable[0][getpid_offset]);

static int device_open(struct inode* inode, struct file* filp)
{
	if(Device_Open){
		return -EBUSY;
	}
	Device_Open++;

	printk(KERN_INFO"device_open\n");
	try_module_get(THIS_MODULE);
	return 0;
}

static int device_release(struct inode* inode, struct file* filp)
{
	Device_Open--;
	printk(KERN_INFO"device_release\n");
	module_put(THIS_MODULE);
	return 0;
}

static ssize_t device_read(struct file* file, char __user* buffer, size_t length, loff_t* offset)
{
	return 0;
}

static ssize_t device_write(struct file* file, const char __user* buffer, size_t length, loff_t* offset)
{
	return 0;
}

int make_rw(unsigned long address)
{
//	unsigned int level;
//	pte_t* pte = lookup_address(address, &level);
//	if(pte->pte & ~_PAGE_RW){
//		pte->pte |= _PAGE_RW;
//	}
	return 0;
}

int make_ro(unsigned long address)
{
	//unsigned int level;
	//pte_t* pte = lookup_address(address, &level);
	//pte->pte &= ~_PAGE_RW;
	return 0;
}

void write_log(void)
{
	struct file* filp;
	mm_segment_t old_fs;
	char buf[20];
	int ret;

	filp = filp_open(file_name, O_RDWR | O_CREAT, 0644);
	if(IS_ERR(filp)){
		printk("ERROR in filp_open\n");
		return ;
	}
	ret = snprintf(buf, 20, "%d", record_pid);
	printk(KERN_INFO"buf: %s ret: %d\n", buf, ret);
	old_fs = get_fs();
	set_fs(KERNEL_DS);
	filp->f_op->write(filp, //"abcdefg", sizeof("abcdefg"), &filp->f_pos);
	 buf, ret + 1, &filp->f_pos);
	printk(KERN_INFO"wrote pid\n");
	set_fs(old_fs);
	filp_close(filp, NULL);
}

int read_log(void)
{
	//return 777;
	mm_segment_t old_fs;
	char buf[20];
	long saved_pid;
	struct file* filp;
	int ret;
	//char* ptr;
	filp = filp_open(file_name, O_RDWR, 0644);
	if(IS_ERR(filp)){
		printk(KERN_INFO"Error in filpopen (replay\n");
		return -1;
	}
	old_fs = get_fs();
	set_fs(KERNEL_DS);

	ret = filp->f_op->read(filp, buf, sizeof(buf), &filp->f_pos);
	if(ret < 0){
		printk(KERN_INFO"Error in read file(replay\n");
		return -1;
	}
	set_fs(old_fs);

	//ptr = buf + ret;
	//if((saved_pid = simple_strtol(buf, &ptr, 10))){
	ret = kstrtol(buf, 10, &saved_pid);
	if(ret != 0){
		printk(KERN_INFO"Error in converting form char* to long error num: %d\n", ret);
		return -1;
	}
	
	filp_close(filp, NULL);
	//convert long to int intentionaliy
	return (int)saved_pid;
}
asmlinkage int (*real_getpid[2])(void);
asmlinkage int rr_getpid(void) 
{
	int ret;
	if(to_record && current->pid == record_pid){
	printk(KERN_INFO"inside(old) record_getpid\n");
	write_log();
	//printk(KERN_INFO"$$TODO fake logging done\n");
	printk(KERN_INFO"about to call original getpid()\n");
	return  real_getpid[0]();
	}else if(to_replay && current->pid == replay_pid){
		printk(KERN_INFO"inside replay getpid\n");
		ret = read_log();
		printk(KERN_INFO"read pid: %d from file\n", ret);
		return ret;
	}
	//none of above
	//printk(KERN_INFO"11111\n");
	return real_getpid[0]();
}

static  int change_sys_call_table(void)
{
	//change entry for get pid
	//void** systable;
	int i;
	//tact 64abi only ,for the moment
	printk(KERN_INFO"$$$$chagne_sys_call_table$$$$\n");	
	if(sys_call_tab_chged != 0){
		printk(KERN_INFO"getpid changed before, won't subst\n");
		return -1;
	}
	//leave O32 abi for now
	for(i = 0; i < cnt; i++){
	//	printk(KERN_INFO"ori __NR_getpid: %daddr: %p\taddr2: %p\n", __NR_getpid, (systable[i] + getpid_offset), ((void**)systable[i] + getpid_offset));

	printk(KERN_INFO"ori addr:%p\tcontent:%p\n", systable[i] + getpid_offset, *(systable[i] + getpid_offset));
	}


	for(i = 0; i < cnt; i++){
	make_rw((unsigned long)systable[i]);
    //systable = (void**)SYS_CALL_TABLE;
	real_getpid[i] = *((void**)systable[i] + getpid_offset);
	*((void**)systable[i]  + getpid_offset) = rr_getpid;
	make_ro((unsigned long)systable[i]);
	}	
	sys_call_tab_chged = 1;
	for(i = 0; i < cnt; i++){
		printk(KERN_INFO"stored ori: %p rr_getpid: %p contentafter modify:%p\n", real_getpid[i], rr_getpid, *(systable[i] + getpid_offset));
	}

	printk(KERN_INFO"####change_sys_call_table####\n");
	return 0;
}

static int restore_sys_call_table(void)
{
	//restore sys table
	int i;
	printk("$$$$restore_sys_call_table$$$$\n");
	if(sys_call_tab_chged != 1){
		printk(KERN_INFO"getpid not chged");
		return -1;
	}

	for(i = 0; i < cnt; i++){
        printk(KERN_INFO"content before restor getpid:%p\n",
        	   	((void**)systable[i])[getpid_offset]);
	}
	for(i = 0; i < cnt; i++){
	make_rw((unsigned long)systable[i]);
	((void**)systable[i])[getpid_offset] = real_getpid[i];
	make_ro((unsigned long)systable[i]);
	}	
	sys_call_tab_chged = 0;
	for(i = 0; i < cnt; i++){
	printk(KERN_INFO"restor getpid now:%p\n",
		   	((void**)systable[i])[getpid_offset]);
	}
	printk(KERN_INFO"####restore_sys_call_table####\n");
	return 0;
}

static long device_ioctl(/*struct inode* inode,*/ struct file* file, unsigned int ioctl_num, unsigned long ioctl_param)
{
	int input;
	int ret;
	printk(KERN_INFO"lalal in ioctl num: %u parm: %ld\n", _IOC_NR(ioctl_num), ioctl_param);
//	return 0;
	switch(ioctl_num)
	{
		case IOCTL_SET_PID_RECORD:
			input = current->pid;
			printk(KERN_INFO"!!!!ioctl set pid(record) : %d\n", input);
			record_pid = input;
			to_record = 1;
			to_replay = 0;
			break;
		case IOCTL_GET_PID_RECORD:
			printk(KERN_INFO"ioctl get pid(record) : %d\n", record_pid);
			copy_to_user((long*)ioctl_param, &record_pid, sizeof(record_pid));
			break;
		case IOCTL_START_RECORD:
			printk(KERN_INFO"ioctl start\n");
			ret = change_sys_call_table();
			if(ret != 0){
				printk(KERN_INFO"error change sys call table\n");
				return -1;
			}
			break;
		case IOCTL_STOP_RECORD:
			printk(KERN_INFO"ioctl stop\n");
			record_pid = 0;
			break;
		case IOCTL_RESET:
			printk(KERN_INFO"R&R reset\n");
			restore_sys_call_table();
			to_record = 0;
			to_replay = 0;
			record_pid = 0;
			replay_pid = 0;
			break;
		case IOCTL_SET_PID_REPLAY:
			printk(KERN_INFO"@@@@set pid to replay: %d\n", current->pid);
			replay_pid = current->pid; 
			to_record = 0;
			to_replay = 1;
			break;
		case IOCTL_START_REPLAY:
			printk(KERN_INFO"START REPLAY\n");
			change_sys_call_table();
			break;
		case IOCTL_STOP_REPLAY:
			break;

	}
	return 0;
}

struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	//.ioctl = device_ioctl,
	.unlocked_ioctl = device_ioctl,
	.open = device_open,
	.release = device_release
};

static int init_record(void)
{
	int ret = 0;
	printk(KERN_INFO"hellow===========\n");
	printk(KERN_INFO"record program %s\n", program);
	ret = register_chrdev(0, "r&r", &fops);
	if(ret < 0){
		printk(KERN_INFO "error regs dev\n");
		return ret;
	}
	printk(KERN_INFO"major : %d\n", ret);
	major = ret;
	return 0;
}

static void exit_record(void)
{
	printk(KERN_INFO"exit!\n");
	unregister_chrdev(major, "r&r");
	
}

module_init(init_record);
module_exit(exit_record);
