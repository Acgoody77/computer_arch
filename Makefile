#Austin Goodman make file for cache simulator
CC = gcc

#define output executable
output: cachesim.o
	$(CC) cachesim.o -o cachesim

#define the output and compilation of code
cachesim.o: cachesim.c cachesim.h
	$(CC) -c cachesim.c

#define clean function to force remove any .out files
clean:
	rm -f *.o
