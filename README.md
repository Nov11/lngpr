# practice
ugly getpid() syscall record implementation
for loongson
###how to make them run online?###


#r&r:
##Get the address for sys_getpid
<p>This is done by `grep sys_getpid /proc/kallsyms`
Get same result when grep from System.map
</p>

##Locate the right sys_call_table addr 
<p>
As source files compiled by gcc are all 32bit, It will invoke 32bit version system call
The sys_call_table with less address should be the one for 64bit version
and the one I modified is for 32bit
I figured out these by print the address and content of two table, modify address which contains addr for sys_getpid
and test by a program invoke getpid()
</p>
##About the abi
Run `file programname` show no 'N32', I guess it's o32 abi

##__NR_ numbers can't be used as that for x86
<p>They must be minused a certain offset before using as an offset to sys_call_table
Ways to get the right offset may lay in the source files
I found this after I make sure the right address for sys_getpid and change sys_call_table can make user invoke the program I substitude.
One printk makes it clear.But offset for getpid in different table is not same. It surprised me.
</p>
##while sys_call_table is in text section 
<p>There is no error when modify content of sys_call_table
Recalled that on x86, sys_call_table is marked R, which means read only section .
It surprised me, too.
update setup and replay stub
the ioctl_set series need not a parameter
</p>

##cross compile kernel 3.10 from loonginx
<p>64 without o32 n32 can't boot
64 with o32 is alright,but can't load module compiled by gcc shipped with loonginx fedora 19
</p>
##work is done on original fedora 19 from loonginx
It seems that port ugly implementation of online record & replay syscall that only retrieve info from kernel from x86 to mips (loongson) is done.

<p>The questions are still there, how to record and replay:
syscalls change the outside environment
time stamp instructions
asynchronized signals
variable access sequence threads preform
and also inter process communication, semaphore, message queue, shared memory
make the replay online(a little lagged behind the recorded one)
</p>
