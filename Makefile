CFLAGS = -ansi -pedantic -Wall
CC = gcc
LIBNAME = libscreen
TEST_EXE = test


build: libscreen.o
	ar r $(LIBNAME).a libscreen.o

build_test: test.o libscreen.o
	$(CC) -o $(TEST_EXE) $^

libscreen.o: libscreen.c libscreen.h
	$(CC) $(CFLAGS) -o $@ -c $< 

test.o: test.c libscreen.h
	$(CC) $(CFLAGS) -o $@ -c $< 

run_test: build_test
	./$(TEST_EXE)

clean:
	rm -f *.o *.a $(TEST_EXE)