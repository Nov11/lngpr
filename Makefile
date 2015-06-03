ifneq ($(KERNELRELEASE),)
obj-m += record.o
obj-m += writelog.o
obj-m += readlog.o
obj-m += ioctl.o
else
all:default stub

default:
	make -C /lib/modules/3.10.0-2015.03.17.fc19.mipsel/build/ M=`pwd` 
	#make  -C ../linuxsrc M=`pwd`
.PHONY:clean

clean:
	make -C /lib/modules/3.10.0-2015.03.17.fc19.mipsel/build/ M=`pwd` clean
	#make -C ../linuxsrc M=`pwd` clean
	rm setup replay getpid
stub: setup getpid replay
setup replay: command.h
getpid:
	gcc -o getpid gp.c
endif
