/*******************************************************************
*   stack.h
*   Cameron Brock
*   Programming Assignment 1 calculon
*
*   This program is entirely my own work
*******************************************************************/
#ifndef STACK_H
#define STACK_H
#include "node.h"
typedef struct stack{
    node *top;
}stack;
extern stack *newStack(void);
extern int EmptyStack(stack *s);
extern int FullStack(stack *s);
extern void Push(node *n, stack *s);
extern node *Pop(stack *s);
extern void printStack(stack *s,FILE *fp);
#endif // STACK_H
