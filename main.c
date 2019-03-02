#include<stdio.h>
#include "mymalloc.c"
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
	char *a, *b, *c, *d;
	printf("\tMalloc CALL\n");
	malloc(4096 - sizeof(struct meta));	
	traverse();	
	halt();
	printf("\tMalloc CALL\n");
	a = malloc(10);
	traverse();	
	printf("\tMalloc CALL\n");
	b = malloc(20);
	traverse();	
	printf("\tMalloc CALL\n");
	c = malloc(10);
	traverse();	
	printf("\tMalloc CALL\n");
	d = malloc(10);
	traverse();	
	printf("\tFREE CALL\n");
	free(b);
	traverse();	
	printf("\tINVALID FREE CALL\n");
	free(NULL);	
	//b = malloc(10);
	printf("\tMalloc CALL\n");
	b = malloc(40);
	
	//printf(" %d \n %d \n %d\n %d\n",a , b , c, d);

	traverse();	
	printf("\tFREE CALL\n");
	free(a);	
	traverse();
	printf("\tMalloc CALL\n");
	malloc(10);
	traverse();
	
	printf("\tMalloc CALL\n");
	malloc(4096 - sizeof(struct meta));	
	traverse();	
	return (0);
}
