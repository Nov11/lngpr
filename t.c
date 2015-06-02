#include <linux/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>

int main()
{
	int magic  = 66;
	int i;
	int ret;
	int fd;
	int cnt = 0;
	ret = open("record",O_RDWR);
	if(ret < 0){
		perror("open file test");
		exit(1);
	}
	fd = ret;
	for(i = 0; i <= 0xff; i++){
		ret = ioctl(fd, _IOW(magic, i, int));
		if(ret == 0){
			cnt++;
			printf("valid nr: %d\n", i);
		}
	}
	printf("usageble _IO(magic, )  nr count: %d\n", cnt);
	return 0;
}
