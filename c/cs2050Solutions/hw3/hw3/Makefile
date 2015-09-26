all: bst

CFLAGS= -g -Wall

bst: bst.o driver.o
	gcc bst.o driver.o ${CFLAGS} -o bst

bst.o: bst.c bst.h
	gcc bst.c bst.h ${CFLAGS} -c
driver.o: driver.c
	gcc driver.c ${CFLAGS} -c

clean:
	rm *.o bst
