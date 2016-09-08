/*******************************************************************
*   testing.c
*   Cameron Brock
*   Programming Assignment 1 calculon
*
*   This program is entirely my own work
*******************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "value.h"
#include "node.h"
#include "stack.h"
#include "queue.h"
#include "tree.h"

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
        if((strcmp(s->top->val->type,"INTEGER") == 0)){
        		if((s->top->val->ival == n->val->ival))
        			 t->testsPassed++;
        }
        else if((strcmp(s->top->val->type,"REAL") == 0)){
        		if((s->top->val->rval == n->val->rval))
                t->testsPassed++;
        }
        else if((strcmp(s->top->val->type,"STRING") == 0) || (strcmp(s->top->val->type,"VARIABLE") == 0)){
                if((s->top->val->rval == n->val->rval))
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
    else{
        if((strcmp(s->top->next->val->type,"INTEGER") == 0)){
                if((s->top->next->val->ival == n->val->ival))
                     t->testsPassed++;
        }
        else if((strcmp(s->top->next->val->type,"REAL") == 0)){
                if((s->top->next->val->rval == n->val->rval))
                t->testsPassed++;
        }
        else if((strcmp(s->top->next->val->type,"STRING") == 0) || (strcmp(s->top->next->val->type,"VARIABLE") == 0)){
                if((s->top->next->val->rval == n->val->rval))
                t->testsPassed++;
        }
        else
            printf("TestStackNext Failed\n");
    }

}
void TestStack(test *t){
    int INT_VAR = 10;
    double REAL_VAR = 1.1;
    char *STRING_VAR = malloc(12 + 1);
    strcpy(STRING_VAR,"Hello World");
    char *VARIABLE_VAR = malloc(19 + 1);
    strcpy(VARIABLE_VAR,"This is a Var Name");
    value *v0,*v1,*v2,*v3;
    v0 = newIntegerValue(INT_VAR);
    TestSetValue(t,v0);
    v1 = newRealValue(REAL_VAR);
    TestSetValue(t,v1);
    v2 = newStringValue(STRING_VAR);
    TestSetValue(t,v2);
    v3 = newVariableValue(VARIABLE_VAR);
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
    n3 = Pop(s);
    TestStackTop(t,n2,s);
    TestStackNext(t,n1,s);
    n2 = Pop(s);
    TestStackTop(t,n1,s);
    TestStackNext(t,n0,s);
    n1 = Pop(s);
    TestStackTop(t,n0,s);
    TestStackNext(t,NULL,s);
    n0 = Pop(s);
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
void TestQueueHead(test *t,node *n,queue *q){
    t->testsRan++;
    if(n == NULL){
        if(q->head == NULL)
            t->testsPassed++;
        else
            printf("TestQueueHead Failed\n");
    }
    else {
        if((strcmp(q->head->val->type,"INTEGER") == 0)){
            if((q->head->val->ival == n->val->ival))
                t->testsPassed++;
        }
        else if((strcmp(q->head->val->type,"REAL") == 0)){
            if((q->head->val->rval == n->val->rval))
                t->testsPassed++;
        }
        else if((strcmp(q->head->val->type,"STRING") == 0) || (strcmp(q->head->val->type,"VARIABLE") == 0)){
            if((q->head->val->rval == n->val->rval))
                t->testsPassed++;
        }
        else
            printf("TestQueueHead Failed\n");
    }
}
void TestQueueHeadNext(test *t,node *n,queue *q){
    t->testsRan++;
    if(n == NULL){
        if(q->head->next == NULL)
            t->testsPassed++;
        else
            printf("TestQueueHeadNext Failed\n");
    }
    else{
        if((strcmp(q->head->next->val->type,"INTEGER") == 0)){
            if((q->head->next->val->ival == n->val->ival))
                t->testsPassed++;
        }
        else if((strcmp(q->head->next->val->type,"REAL") == 0)){
            if((q->head->next->val->rval == n->val->rval))
                t->testsPassed++;
        }
        else if((strcmp(q->head->next->val->type,"STRING") == 0) || (strcmp(q->head->next->val->type,"VARIABLE") == 0)){
            if((q->head->next->val->rval == n->val->rval))
                t->testsPassed++;
        }
        else
            printf("TestQueueHeadNext Failed\n");
    }
}
void TestQueueTail(test *t,node *n,queue *q){
    t->testsRan++;
    if(n == NULL){
        if(q->tail == NULL)
            t->testsPassed++;
        else
            printf("TestQueueTail Failed\n");
    }
    else{
        if((strcmp(q->tail->val->type,"INTEGER") == 0)){
            if((q->tail->val->ival == n->val->ival))
                t->testsPassed++;
        }
        else if((strcmp(q->tail->val->type,"REAL") == 0)){
            if((q->tail->val->rval == n->val->rval))
                t->testsPassed++;
        }
        else if((strcmp(q->tail->val->type,"STRING") == 0) || (strcmp(q->tail->val->type,"VARIABLE") == 0)){
            if((q->tail->val->rval == n->val->rval))
                t->testsPassed++;
        }
        else
            printf("TestQueueTail Failed\n");
    }
}
void TestQueue(test *t){
    int INT_VAR = 10;
    double REAL_VAR = 1.1;
    char *STRING_VAR = malloc(12 + 1);
    strcpy(STRING_VAR,"Hello World");
    char *VARIABLE_VAR = malloc(19 + 1);
    strcpy(VARIABLE_VAR,"This is a Var Name");
    value *v0,*v1,*v2,*v3;
    v0 = newIntegerValue(INT_VAR);
    TestSetValue(t,v0);
    v1 = newRealValue(REAL_VAR);
    TestSetValue(t,v1);
    v2 = newStringValue(STRING_VAR);
    TestSetValue(t,v2);
    v3 = newVariableValue(VARIABLE_VAR);
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
    queue *q;
    q = newQueue();
    TestQueueHead(t,NULL,q);
    TestQueueTail(t,NULL,q);
    Enqueue(q,n0);
    TestQueueHead(t,n0,q);
    TestQueueTail(t,n0,q);
    Enqueue(q,n1);
    TestQueueHead(t,n0,q);
    TestQueueHeadNext(t,n1,q);
    TestQueueTail(t,n1,q);
    Enqueue(q,n2);
    TestQueueHead(t,n0,q);
    TestQueueHeadNext(t,n1,q);
    TestQueueTail(t,n2,q);
    Enqueue(q,n3);
    TestQueueHead(t,n0,q);
    TestQueueHeadNext(t,n1,q);
    TestQueueTail(t,n3,q);
    node *node = Dequeue(q);
    TestQueueHead(t,n1,q);
    TestQueueHeadNext(t,n2,q);
    TestQueueTail(t,n3,q);
    free(node);
    node = Dequeue(q);
    TestQueueHead(t,n2,q);
    TestQueueHeadNext(t,n3,q);
    TestQueueTail(t,n3,q);
    free(node);
    node = Dequeue(q);
    TestQueueHead(t,n3,q);
    TestQueueTail(t,n3,q);
    free(node);
    node = Dequeue(q);
    TestQueueHead(t,NULL,q);
    TestQueueTail(t,NULL,q);
    free(n0);
    free(n1);
    free(n2);
    free(n3);
    free(v0);
    free(v1);
    free(v2->sval);
    free(v2);
    DestroyQueue(q);
    free(q);
}
void TestTree(test *t){
    tree *tr = newTree();
    int INT_VAR = 10;
    double REAL_VAR = 1.1;
    char *STRING_VAR = malloc(12 + 1);
    strcpy(STRING_VAR,"Hello World");
    char *VARIABLE_VAR = malloc(19 + 1);
    strcpy(VARIABLE_VAR,"This is a Var Name");
    value *v0,*v1,*v2,*v3;
    v0 = newIntegerValue(INT_VAR);
    TestSetValue(t,v0);
    v1 = newRealValue(REAL_VAR);
    TestSetValue(t,v1);
    v2 = newStringValue(STRING_VAR);
    TestSetValue(t,v2);
    v3 = newVariableValue(VARIABLE_VAR);
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
    n0->key = "x";
    Insert(tr,n0);
    n0 = NULL;
    t->testsRan++;
    n0 = Search(tr,"x");
    if(n0->val->ival == 10)
        t->testsPassed++;
    n1->key = "y";
    Insert(tr,n1);
    n1 = NULL;
    t->testsRan++;
    n1 = Search(tr,"y");
    if(n1->val->rval == 1.1)
        t->testsPassed++;
    //printTree(tr);
    Delete(tr,"x");
    n0 = NULL;
    t->testsRan++;
    n0 = Search(tr,"x");
    if(n0 == NULL)
        t->testsPassed++;
    Delete(tr,"y");
    n0 = NULL;
    t->testsRan++;
    n0 = Search(tr,"y");
    if(n0 == NULL)
        t->testsPassed++;
    //printf("Deleting 10\n");
    //printTree(tr);
    free(tr);
}
int main(int argc, char **argv) {
    test t;
    t.testsRan = 0;
    t.testsPassed = 0;
    TestStack(&t);
    TestQueue(&t);
    TestTree(&t);
    if(t.testsRan == t.testsPassed)
        printf("All %d Tests Passed\n",t.testsPassed);
    else if(t.testsRan > t.testsPassed)
        printf("%d Tests Failed and %d Tests Passed\n", (t.testsRan - t.testsPassed),t.testsPassed);
    else
        printf("Error, number of Tests Passed %d is greater than Tests Ran %d!\n",t.testsPassed,t.testsRan);
    return 0;
}
