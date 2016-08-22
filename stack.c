/*******************************************************************
*   stack.c
*   Cameron Brock
*   Programming Assignment 1 calculon
*
*   This program is entirely my own work
*******************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "Fatal.h"

void initStack(stack *s){
    s->top = NULL;
}
stack *newStack(void){
    stack *s;
    if ((s = (stack *)malloc(sizeof(stack))) == 0)
        Fatal("out of memory\n");
    initStack(s);
    return s;
}
int EmptyStack(stack *s){
    if(s->top == NULL)
        return 1;
    else
        return 0;
}
int FullStack(stack *s){
    return 0;
}
void Push(node *n, stack *s){
    node *temp;
    temp = (node *)malloc(sizeof(node));
    temp->val = n->val;
    temp->next = s->top;
    s->top = temp;
}
void Pop(stack *s, node *n){
    node *temp;
    if(!EmptyStack(s)){
        n->val = s->top->val;
        temp = s->top;
        s->top = s->top->next;
        free(temp);
    }
}
void printStack(stack *s){
    node *temp = s->top;
    if(s->top == NULL)
        return;
    printNode(temp);
    while(temp->next != NULL){
        temp = temp->next;
        printNode(temp);
    }
}