ifneq ($(KERNELRELEASE),)
obj-m += record.o
obj-m += writelog.o
obj-m += readlog.o
obj-m += ioctl.o
obj-m += print_add.o
obj-m += modisys.o
else
all:default stub

default:
	make -C /lib/modules/3.10.0-2015.03.17.fc19.mipsel/build/ M=`pwd` 
	#make  -C ../linuxsrc M=`pwd`
.PHONY:clean

clean:
	make -C /lib/modules/3.10.0-2015.03.17.fc19.mipsel/build/ M=`pwd` clean
	#make -C ../linuxsrc M=`pwd` clean
	rm setup replay getpid getpidn setupn replayn
stub: setup getpid replay getpidn
setup replay: command.h
getpid:gp.c
	gcc -o getpid gp.c
getpidn:gpn.c setupn.c replayn.c
	gcc -o getpidn gpn.c
	gcc -o setupn setupn.c
	gcc -o replayn replayn.c
endif
