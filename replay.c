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

void sigchild(int sig)
{
	int ret;
	ret = wait(child_pid);
	printf("==========================\n");
	printf("waited child, replay done\n");
}

int main()
{
	int ret;
	if(signal(SIGCHLD, sigchild) == SIG_ERR){
		perror("Error set up SIGCHLD handler\n");
		exit(1);
	}

	ret = open("record", O_RDWR);
	if(ret < 0){
		perror("Error open file record");
		exit(1);
	}

	fd = ret;
	child_pid = fork();
	if(child_pid == 0){
		ret = ioctl(fd, IOCTL_SET_PID_REPLAY, getpid());
		if(ret < 0){
			printf("error set pid in ioctl\n");
			exit(1);
		}

		ret = ioctl(fd, IOCTL_START_REPLAY, 0);
		if(ret < 0){
			printf("Error start repaly\n");
			exit(1);
		}
		printf("start replaying\n");
		printf("===============================\n");
		if(execl("getpid", "getpid", NULL)){
			perror("execl");
			return -1;
		}
	}else{
		ret = sleep(100);
		if(ret != 0){
			perror("sleep return \n");
			printf("child exist\n");
		}
		ret = ioctl(fd, IOCTL_RESET, 0);
		if(ret < 0){
			printf("Error reset from replaying\n");
			exit(1);
		}
	}
	return 0;
}
