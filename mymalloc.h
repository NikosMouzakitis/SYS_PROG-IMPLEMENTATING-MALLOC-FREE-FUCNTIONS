#ifndef __MYMALLOC_H__
#define __MYMALLOC_H__

#include <stdint.h>

void *mymalloc(size_t size, char *file, int line);
void myfree(void *ptr, char *file, int line);
unsigned int real_size(size_t size);
void traverse(void);

#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)

static char myblock[4096];

	//metadata structure.
struct meta {
	void *		free; // next free space between this segment and next(NULL if not exit)
	struct meta*	nmeta;// next allocated segment(NULL if not existing)
	void *		udata;// actual space for user data.	
}*head, *tmp, *top, *active;

extern unsigned int total_segments;
#endif
