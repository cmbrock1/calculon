all: calculon

calculon: calculon.o value.o stack.o node.o Fatal.o queue.o
	g++ -Wall -std=c99 -v -o calculon calculon.o value.o stack.o node.o Fatal.o queue.o

calculon.o: calculon.c value.h node.h stack.h
	gcc -Wall -std=c99 -c -g calculon.c

value.o: value.c value.h Fatal.o
	gcc -Wall -std=c99 -c -g value.c

stack.o: stack.c stack.h Fatal.o
	gcc -Wall -std=c99 -c -g stack.c

node.o: node.c node.h Fatal.o
	gcc -Wall -std=c99 -c -g node.c

Fatal.o: Fatal.c Fatal.h
	gcc -Wall -std=c99 -c -g Fatal.c

queue.o: queue.c queue.h
	gcc -Wall -std=c99 -c -g queue.c

clean:
	rm *o calculon
