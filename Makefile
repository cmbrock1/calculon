#   Makefile
#   Cameron Brock
#   Programming Assignment 1 calculon
#
#   This program is entirely my own work
CC=gcc
CFLAGS=-Wall -std=c99 -c -g
TESTOBJECTS=testing.o value.o stack.o node.o Fatal.o queue.o
CALCOBJECTS=calculon.o scanner.o queue.o value.o stack.o node.o Fatal.o -lm
BINARYS=calculon testing
all: calculon

calculon: ${CALCOBJECTS}
	${CC} -Wall -std=c99 -o calculon ${CALCOBJECTS}

test: ${TESTOBJECTS}
	${CC} -Wall -std=c99 -o testing ${TESTOBJECTS}; ./testing;

calculon.o: calculon.c
	${CC} ${CFLAGS} calculon.c

testing.o: testing.c value.h node.h stack.h
	${CC} ${CFLAGS} testing.c

value.o: value.c value.h Fatal.o
	${CC} ${CFLAGS} value.c

stack.o: stack.c stack.h Fatal.o
	${CC} ${CFLAGS} stack.c

node.o: node.c node.h Fatal.o
	${CC} ${CFLAGS} node.c

Fatal.o: Fatal.c Fatal.h
	${CC} ${CFLAGS} Fatal.c

queue.o: queue.c queue.h
	${CC} ${CFLAGS} queue.c

scanner.o: scanner.c scanner.h
	${CC} ${CFLAGS} scanner.c

clean:
	rm *o ${BINARYS}
