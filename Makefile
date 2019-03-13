CC = gcc -g
CFLAGS = -O1 -Wall
STATIC_CFLAGS = -O1 -Wall -static # to static used with $(CC).
SOURCES  := mymalloc.c  
OBJECTS  := $(SOURCES:.c=*.o)
  
all: mymalloc.o a.out 
  
obj: $(SOURCES) $(INCLUDES)
	@$(CC) $(CFLAGS) -c $(SOURCES)&
  
libmalloc.a: mymalloc.o 
	ar rcs libmalloc.a mymalloc.o
 
a.out: memgrid.c libmalloc.a
	$(CC) $(STATIC_CFLAGS) -o a.out memgrid.c libmalloc.a
  
clean:
	rm *.o *.a a.out

