#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
int main(int argc, char* argv[])
{
	int pid;
	int i;
	int repeat;
	struct timeval start, end;
	double sec;
	if(argc != 2){
		printf("need one parameter indicate repeat times\n");
		exit(1);
	}
	if(!(repeat = atoi(argv[1]))){
		printf("parameter must be a integer\n");
		exit(1);
	}
	gettimeofday(&start, NULL);
	for(i = 0; i < repeat; i++){
		pid = getpid();
	}
	gettimeofday(&end, NULL);
	sec = end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec) * 1.0 / 1000000;
	printf("pid: %d repeat %d times, it takes: %f s\n", pid, repeat, sec);
	//printf("In gp.c ----  pid is : %d\n", pid);
	return 0;
}
