#include<stdio.h>
#include "mymalloc.h"
/*
	SYSTEMS PROGRAMMING: IMPLEMENTING MALLOC AND FREE FUNCTIONS.
	malloc() and free() implementation, not the best storage efficiency.
			Systems Programming.
	author: Mouzakitis Nikolaos, 2019.
*/
void halt()
{
	while(1)
		;
}
int main(int argc, char *argv[])
{

	//testing
	//int call = -1;
	char * tmp;	
	for(int i = 0; i < 150; i++) {
	//	printf("\tmalloc\n");
		tmp = malloc(1);
	//	traverse();
	//	printf("\tfree\n");
		free(tmp);
	//	traverse();
	}	
	printf("Memgrid test1\n");
	sleep(1);
	printf("Allocation / deallocation 1 byte... test OK\n");


	char *p[150];
	
	for(int i = 0; i <100; i++)
		p[i] = malloc(1);
	
	for(int i = 0; i <100; i++)
		free(p[i]);

	printf("Memgrid test2\n");
	sleep(1);
	printf("Malloc 100 1 bytes, free 100 1 bytes... test OK\n");




	
	return (0);
}
