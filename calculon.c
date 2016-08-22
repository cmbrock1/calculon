#include <stdlib.h>
#include <stdio.h>
#include "value.h"
#include "node.h"
#include "stack.h"

void TestStack(void){
    value *v0,*v1,*v2;
    v0 = newValueI(10);
    v1 = newValueR(1.1);
    v2 = newValueS("Hello World");
    node *n0 = newNode(v0);
    node *n1 = newNode(v1);
    node *n2 = newNode(v2);
    stack *s = newStack();
    Push(n0,s);
    printf("push n0 on stack\n");
    printStack(s);
    Push(n1,s);
    printf("push n1 on stack\n");
    printStack(s);
    Push(n2,s);
    printf("push n2 on stack\n");
    printStack(s);
    Pop(s,n2);
    printf("pop n2 off stack\n");
    printStack(s);
    Pop(s,n1);
    printf("pop n1 off stack\n");
    printStack(s);
    Pop(s,n0);
    printf("pop n0 off stack\n");
    printStack(s);
    free(n0);
    free(n1);
    free(v0);
    free(v1);
    free(v2->sval);
    free(v2);
    free(s);
    printf("Success\n");
}
int main(int argc, char **argv) {
    TestStack();
    return 0;
}
