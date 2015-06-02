#include <stdio.h>
int abc()
{
	return 11;
}
int main()
{
	void* ptr = abc;
	void** pptr = &ptr;
	printf("void* : %d\n", sizeof(void*));
	printf("ptr: %p ptr + 1: %p\n", ptr, ptr+ 1);
	printf("pptr: %p pptr + 1: %p\n", pptr, pptr + 1);
	return 0;
}
