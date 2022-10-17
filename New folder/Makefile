CC = gcc
CFLAGS := -Wall -Werror -fPIC

all: lib

lib: liblsd.so
	cp liblsd.so ../lsd

liblsd.so: lsd.o
	$(CC) -shared -o liblsd.so lsd.o

lsd.o: lsd.c lsd.h
	$(CC) $(CFLAGS) -c -O3  lsd.c

clean:
	rm *.o *.so ../lsd/*.so
