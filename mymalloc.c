#include "mymalloc.h"
#include "color.h"
//#define DEBUG TRUE


static int call = -1;
unsigned int real_size(size_t size)
{
	return ( sizeof(struct meta) + (unsigned int) size);
}

void traverse(void) {

	struct meta *t;
	t = active;

	if(total_segments == 0) {
	
		printf("Nothing allocated yet.\n");

		return;
	}

	printf("TOTAL SEGMENTS:%s %d%s\n",KGRN, total_segments,KWHT);
	while(t->nmeta != NULL) {
	
		printf(" addr: %s %3d %s\n",KGRN, t, KWHT);
		t = t->nmeta;
	}
	printf(" addr: %s %3d %s\n",KGRN, t, KWHT);
}	

void *mymalloc(size_t size, char *file, int line) 
{

	call++;
//	printf("CALLLL:: %d\n", call);	

	if(call == 0) {
		unsigned int real_sz;
		real_sz = real_size(size);
//		printf("%d %s\n", __LINE__, __FILE__);
		//	Always keep the start of the allocatable memory in the
	       	//	top structure, even if allocated or not.
		top = myblock;		
			
//		printf("Call to mymalloc() request: %d\n", (int) size);
	
		// catch the exception	
		if( real_sz > 4096) {
			printf("%sNot enough memory to allocate\n Error line: %d in \"%s\" \n%s",KRED, line, file, KWHT);
			return NULL;	
		}	
	
		head = myblock;	
		head->used = 1;
		head->count = (int)size;
		head->free = (char *)head + sizeof(struct meta) + head->count;
		head->udata = (char *)head + sizeof(struct meta);
		head->nmeta = NULL;
		active = head;
		#ifdef DEBUG		
		printf("DEBUG-----------------:call:%d\n",call);
		printf("HEAD address: %d\n",head);
		printf("UDATA address: %d\n",head->udata);
		printf("NMETA address: %d\n", head->nmeta);
		printf("FREE address: %d\n", head->free);
		printf("Total bytes allocated: %d\n", real_sz);
		#endif
		total_segments++;
		return head->udata;

	} else {
		unsigned int real_sz;

		real_sz = real_size(size);
	
//		printf("Not first time\n");
		
		struct meta *new;

		new = head;	
	
		// First check if it fits in the free space on the top 
		// in the case we got empty space.
		if( (top != active) && ( active - top <= real_sz) ) {
			#ifdef DEBUG
			printf("Fits in the top\n");
			#endif
			struct nmeta *t, *p;
			t = top;
			p = active;

			top->used = 1;
			top->count = size;

			if( (active-top) == real_sz)
				top->free = NULL;
			else
				top->free = (char *)top + sizeof(struct meta) + size;

			top->udata = (char *)head + sizeof(struct meta);
			active = top;
			total_segments++;
			return;
		}

		// get in the last chunk.
		
		struct meta *helper;
		
		while(new->nmeta != NULL) {

			if( new->free != NULL) {
				struct meta *this;
				this = new;	
		
				unsigned int difference = (unsigned char *) new->nmeta - (unsigned char *)new->free;
				
				#ifdef	DEBUG	
				printf("%d difference of next meta to valid free here.\n", difference);
				printf("Call to mymalloc() request: %d\n", (int) size);
				printf("Need: %d bytes\n", real_sz);	
				#endif

				if(difference >= real_sz) {
				
					//helper: now will allocate new space.
					helper = new->free;		
					helper->used = 1;
					helper->count = (int)size;
					helper->udata = (char *)helper + sizeof(struct meta);	
					
					if(new->nmeta != NULL) // will always execute.
						helper->nmeta = new->nmeta;

					if( (difference - real_sz) == 0)
						helper->free = NULL;
					else
						helper->free = (char *)helper + real_sz;
				
					// assign correct the nmeta pointers after first fit insertion.	
					helper->nmeta = new->nmeta;
					new->nmeta = (struct meta *) &helper;	
					#ifdef	DEBUG		
					printf("DEBUG-----------------:call:%d\n",call);
					printf("HEAD address: %d\n",helper);
					printf("UDATA address: %d\n",helper->udata);
					printf("NMETA address: %d\n", helper->nmeta);
					printf("FREE address: %d\n", helper->free);
					printf("Total bytes allocated: %d\n", real_sz);
					#endif
					total_segments++;
					return helper->udata;
				}	
			}		
			new = new->nmeta;
		} 
		//new nmeta is NULL.
		
		int difference = &myblock[4095] - (char *)new->free+1;	
		
		#ifdef	DEBUG	
		printf("new->meta is null\n");
		printf("Call to mymalloc() request: %d\n", (int) size);
		printf("Need: %d bytes\n", real_sz);	
		printf("Available difference: %d\n", difference);	
		#endif
			
		if( real_sz <= difference) {
			helper = new->free;		
			helper->count = (int)size;
			helper->used = 1;
			helper->udata = (char *)helper + sizeof(struct meta);

			if( (difference - real_sz) == 0) {

				helper->free = NULL;
			}
			else
				helper->free = (char *)helper + real_sz;

			helper->nmeta = NULL;
			
			new->free = NULL;
			// get the right address.
			new->nmeta =*(&helper);
			
			#ifdef	DEBUG		
			printf("DEBUG-----------------:call:%d\n",call);
			printf("HEAD address: %d\n",helper);
			printf("UDATA address: %d\n",helper->udata);
			printf("NMETA address: %d\n", helper->nmeta);
			printf("FREE address: %d\n", helper->free);
			printf("Total bytes allocated: %d\n", real_sz);
			#endif

			total_segments++;
			return helper->udata;
		} else {
			printf("%sNot enough memory to allocate\n Error line: %d in \"%s\" \n%s",KRED, line, file, KWHT);
			return NULL;
		}

	}
	
}

void myfree(void *ptr, char *file, int line)
{

	#ifdef	DEBUG
	printf("free address: %d\n", ptr);
	#endif
	struct meta *this, *helper;

	//start by using the active metadata pointer.
	this = active;
	helper = active; // just to be sure.

	while(this->nmeta != NULL) { 
		
		if(this->udata == ptr) {

			#ifdef DEBUG
			printf("valid free call()\n");
			#endif
			
			/*		DEALLOCATION PROCESS
				Different scenarios for our segment:

				a) be the first allocated space.
				b) be somewhere in the middle of other segments.
				c) be the last allocated space in the list.
			*/
				
			if(this == active) { //(a)

				active = this->nmeta;
				head = this->nmeta;
//				printf("New active addr: %d\n", active);	
				this->used = 0;
				this->count = 0;
				
				total_segments--;
				
				return ;

			} else { //(b)
				helper->nmeta = this->nmeta;
				this->used = 0;
				this->count = 0;
				helper->free = this;	
				#ifdef DEBUG	
				printf("Deallocated space\n");
				#endif
				total_segments--;

				return;

			}
		}
		helper = this;
		this = this->nmeta;	
	}

	if(this->udata == ptr) {

		#ifdef DEBUG
		printf("valid free call()\n");
		#endif
		
		this->used = 0;
		this->count = 0;
		this->nmeta  = NULL;

		if(this->nmeta != NULL)
			helper->nmeta = this->nmeta;
		else
			helper->nmeta = NULL;

		total_segments--;
		call = -1;		
		return;
	} else {
		
		printf("%s Invalid call to free\n",KRED);
		printf(" Line: %d in \"%s\"\n", line, file);
		printf("Bypassing call as a security feature\nNo allocation happen%s\n",KWHT);
		return;
	}

}
