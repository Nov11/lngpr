#include <stdlib.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include "command.h"

int child_pid;
int fd;

void sigch(int sig)
{
	int ret;
	wait(child_pid);
	printf("wait child done\n");
}

int main()
{
	int ret;
	long record_pid = 0;
	//struct sigaction sa;
	//sa.sa_sigaction = sigch;
	//sa.sa_flags = 
	//ret = sigaction(SIG_CHILD
	if(signal(SIGCHLD, sigch) == SIG_ERR){
		perror("error set up handler\n");
		exit(1);
	}
	ret = open("record", O_RDWR);
	if(ret < 0){
		perror("open file");
		printf("open file went wrong\n");
		exit(1);
	}
	fd = ret;
	child_pid = fork();	
	if(child_pid == 0){
		printf("to do comfermation, pid: %d\n", getpid());
		ret = ioctl(fd, IOCTL_SET_PID_RECORD);
		if(ret < 0){
			printf("ioctl error setp pid record \n");
			return -1;
		}
		ret = ioctl(fd, IOCTL_GET_PID_RECORD, &record_pid);
		if(ret < 0){
			printf("error get record pid\n");
			exit(1);
		}
		printf("##in setup proc pid: %d\n", record_pid);
		ret = ioctl(fd, IOCTL_START_RECORD);
		if(ret < 0){
			printf("ioctl failed start recording\n");
			return -1;
		}
		printf("start recording\n");
		ret = execl("getpid", "getpid", NULL);
		if(ret < 0){
			perror("execl");
			return -1;
		}
	}else{
		
//		ret = ioctl(fd, 0x4, child_pid);
//		if(ret < 0){
//			printf("ioctl went wrong\n");	
//			exit(1);
//		}
		ret = sleep(100);
		if(ret != 0){
			perror("sleep return\n");
			printf("child exits\n");
		}
		ret = ioctl(fd, IOCTL_RESET);// getpid());
		if(ret < 0){
			printf("error send stop signal IOCTL_RESET");
			exit(1);                      	
		}
		printf("##setup finished\n");
	}
	return 0;
}	
