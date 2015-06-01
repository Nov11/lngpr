#include <sys/ioctl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h> 
#include <stdlib.h>

#include <fcntl.h>


int main()
{
	int ret;
	int fd = open("test", O_RDWR);
	if(fd < 0){
		perror("open");
		exit(1);
	}
	ret = ioctl(fd, 5);
	if(ret){
		perror("ioctl");
		printf("ioctl ret != 0\n");
	
		exit(1);
	}
	printf("Succ!\n");
	return 0;
}
