ifneq ($(KERNELRELEASE),)
obj-m += record.o
obj-m += writelog.o
obj-m += readlog.o
obj-m += ioctl.o
else
all:default stub

default:
	make -C /lib/modules/3.10.0-2015.03.17.fc19.mipsel/build/ M=`pwd` 
stub: setup getpid replay
setup replay: command.h
endif
