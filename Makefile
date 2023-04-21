CFLAGS = -ansi -pedantic -Wall -DDEBUG
CC = gcc


build: libscreen.o
	ar r libscreen.a libscreen.o

test: build_test
	./build_test

build_test: test.o libscreen.o
	$(CC) -o $@ $^

libscreen.o: libscreen.c libscreen.h
	$(CC) $(CFLAGS) -o $@ -c $< 

test.o: test.c libscreen.h
	$(CC) $(CFLAGS) -o $@ -c $< 

clean:
	rm -f *.o *.a build_test