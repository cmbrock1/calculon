#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "value.h"
#include "node.h"
#include "stack.h"
#include "queue.h"
typedef struct test{
    int testsPassed;
    int testsRan;
}test;
void TestSetValue(test *t,value *v){
    t->testsRan++;
    if((v->type == INTEGER) && (v->ival == 10))
        t->testsPassed++;
    else if((v->type == REAL) && (v->rval == 1.1))
        t->testsPassed++;
    else if((v->type == STRING) && (strcmp(v->sval,"Hello World") == 0))
        t->testsPassed++;
    else if((v->type == VARIABLE) && (strcmp(v->sval,"This is a Var Name") == 0))
        t->testsPassed++;
    else
        printf("TestSetValue Failed\n");
}
void TestSetNode(test *t,node *n){
    t->testsRan++;
    if((n->val->type == INTEGER) && (n->val->ival == 10))
        t->testsPassed++;
    else if((n->val->type == REAL) && (n->val->rval == 1.1))
        t->testsPassed++;
    else if((n->val->type == STRING) && (strcmp(n->val->sval,"Hello World") == 0))
        t->testsPassed++;
    else if((n->val->type == VARIABLE) && (strcmp(n->val->sval,"This is a Var Name") == 0))
        t->testsPassed++;
    else
        printf("TestSetNode Failed\n");
}
void TestStackTop(test *t,node *n,stack *s){
    t->testsRan++;
    if(n == NULL){
        if(s->top == NULL)
            t->testsPassed++;
        else
            printf("TestStackTop Failed\n");
    }
    else{
        if((s->top->val->ival == n->val->ival) &&
           (s->top->val->rval == n->val->rval) &&
           (strcmp(s->top->val->sval,n->val->sval) == 0)){
            t->testsPassed++;
        }
        else
            printf("TestStackTop Failed\n");
    }
}
void TestStackNext(test *t,node *n,stack *s){
    t->testsRan++;
    if(n == NULL){
        if(s->top->next == NULL)
            t->testsPassed++;
        else
            printf("TestStackNext Failed\n");
    }
    else {
        if((s->top->next->val->ival == n->val->ival) &&
           (s->top->next->val->rval == n->val->rval) &&
           (strcmp(s->top->next->val->sval,n->val->sval) == 0)){
            t->testsPassed++;
        }
        else
            printf("TestStackNext Failed\n");
    }

}
void TestStack(test *t){
    value *v0,*v1,*v2,*v3;
    v0 = newValueI(10);
    TestSetValue(t,v0);
    v1 = newValueR(1.1);
    TestSetValue(t,v1);
    v2 = newValueS("Hello World");
    TestSetValue(t,v2);
    v3 = newValueV("This is a Var Name");
    TestSetValue(t,v3);
    node *n0,*n1,*n2,*n3;
    n0 = newNode(v0);
    TestSetNode(t,n0);
    n1 = newNode(v1);
    TestSetNode(t,n1);
    n2 = newNode(v2);
    TestSetNode(t,n2);
    n3 = newNode(v3);
    TestSetNode(t,n3);
    stack *s = newStack();
    TestStackTop(t,NULL,s);
    Push(n0,s);
    TestStackTop(t,n0,s);
    TestStackNext(t,NULL,s);
    Push(n1,s);
    TestStackTop(t,n1,s);
    TestStackNext(t,n0,s);
    Push(n2,s);
    TestStackTop(t,n2,s);
    TestStackNext(t,n1,s);
    Push(n3,s);
    TestStackTop(t,n3,s);
    TestStackNext(t,n2,s);
    Pop(s,n3);
    TestStackTop(t,n2,s);
    TestStackNext(t,n1,s);
    Pop(s,n2);
    TestStackTop(t,n1,s);
    TestStackNext(t,n0,s);
    Pop(s,n1);
    TestStackTop(t,n0,s);
    TestStackNext(t,NULL,s);
    Pop(s,n0);
    TestStackTop(t,NULL,s);
    free(n0);
    free(n1);
    free(n2);
    free(n3);
    free(v0);
    free(v1);
    free(v2->sval);
    free(v2);
    free(s);
    // printf("Success\n");
}
void TestQueue(test *t){
    value *v0,*v1,*v2,*v3;
    v0 = newValueI(10);
    TestSetValue(t,v0);
    v1 = newValueR(1.1);
    TestSetValue(t,v1);
    v2 = newValueS("Hello World");
    TestSetValue(t,v2);
    v3 = newValueV("This is a Var Name");
    TestSetValue(t,v3);
    node *n0,*n1,*n2,*n3;
    n0 = newNode(v0);
    TestSetNode(t,n0);
    n1 = newNode(v1);
    TestSetNode(t,n1);
    n2 = newNode(v2);
    TestSetNode(t,n2);
    n3 = newNode(v3);
    TestSetNode(t,n3);
    free(n0);
    free(n1);
    free(n2);
    free(n3);
    free(v0);
    free(v1);
    free(v2->sval);
    free(v2);
}
int main(int argc, char **argv) {
    test t;
    TestStack(&t);
    TestQueue(&t);
    if(t.testsRan == t.testsPassed)
        printf("All %d Tests Passed\n",t.testsPassed);
    else if(t.testsRan > t.testsPassed)
        printf("%d Tests Failed and %d Tests Passed\n", (t.testsRan - t.testsPassed),t.testsPassed);
    else
        printf("Error number of Tests Passed is greater than Tests Ran!\n");
    return 0;
}
