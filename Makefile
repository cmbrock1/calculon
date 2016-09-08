#   Makefile
#   Cameron Brock
#   Programming Assignment 1 calculon
#
#   This program is entirely my own work
CC=gcc
CFLAGS=-Wall -g -std=c99 -c
TESTOBJECTS=testing.o value.o stack.o node.o Fatal.o queue.o tree.o
CALCOBJECTS=calculon.o scanner.o queue.o value.o stack.o node.o Fatal.o tree.o
BINARYS=calculon testing

all: testing calculon

calculon: ${CALCOBJECTS}
	${CC} -Wall -std=c99 -g -o calculon ${CALCOBJECTS} -lm

testing: ${TESTOBJECTS}
	${CC} -Wall -std=c99 -g -o testing ${TESTOBJECTS}; ./testing;

test: calculon expr
	./calculon -v
	./calculon expr

calculon.o: calculon.c scanner.h queue.h stack.h value.h Fatal.h
	${CC} ${CFLAGS} calculon.c

testing.o: testing.c value.h node.h stack.h queue.h tree.h
	${CC} ${CFLAGS} testing.c

value.o: value.c value.h Fatal.h
	${CC} ${CFLAGS} value.c

stack.o: stack.c stack.h node.h Fatal.h
	${CC} ${CFLAGS} stack.c

node.o: node.c node.h Fatal.o
	${CC} ${CFLAGS} node.c

Fatal.o: Fatal.c Fatal.h
	${CC} ${CFLAGS} Fatal.c

queue.o: queue.c queue.h node.h Fatal.h
	${CC} ${CFLAGS} queue.c

tree.o: tree.c tree.h node.h Fatal.h
	${CC} ${CFLAGS} tree.c

scanner.o: scanner.c scanner.h
	${CC} ${CFLAGS} scanner.c

clean:
	rm ${CALCOBJECTS} testing.o ${BINARYS}
	rm -rf *.dSYM
