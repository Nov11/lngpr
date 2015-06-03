#include <sys/ioctl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h> 
#include <stdlib.h>
#include <linux/ioctl.h>
#include <fcntl.h>


int main(int argc, char* argv[])
{
	int ret;
	int i;	
	int j;
	int count = 0;
	int cnt;
	char* file_name = "test";
	if(argc == 2){
		file_name = argv[1];
	}
	if(argc > 2){
		printf("can only take one param\n");
		exit(1);
	}
		
	int fd = open(file_name, O_RDWR);
	if(fd < 0){
		perror("open");
		exit(1);
	}
	for(j = 0x0; j <= 0xff; j++){
		cnt = 0;
		for(i = 0; i <=  0xff; i++){
			ret = ioctl(fd, _IO(j, i));
			//ret = ioctl(fd, _IOW(j, i, int));
			//if(ret != 0){
			//	perror("ioctl");
			//	printf("ioctl ret != 0\n");
			//
			//	exit(1);
			//}
			if(ret == 0){
				count++;
				cnt++;
				printf("magic: %d nr: %d Succ!\n",j,  i);
			}
			
		}
		if(cnt != 0)
		printf("for magic : %d usageble nr cnt: %d\n", j, cnt);
	}
	printf("prog ends here, usable command count : %d\n", count);
	return 0;
}
