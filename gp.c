#include <unistd.h>
#include <stdio.h>

int main()
{
	int pid = getpid();
	printf("In gp.c ----  pid is : %d\n", pid);
	return 0;
}
