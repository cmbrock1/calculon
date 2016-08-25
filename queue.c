/*******************************************************************
*   queue.c
*   Cameron Brock
*   Programming Assignment 1 calculon
*
*   This program is entirely my own work
*******************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "queue.h"
#include "node.h"
#include "Fatal.h"
void initQueue(queue *q){
    q->head = q->tail = NULL;
}
queue *newQueue(void){
    queue *q;
    if ((q = (queue *)malloc(sizeof(queue))) == 0)
        Fatal("out of memory\n");
    initQueue(q);
    return q;
}
int EmptyQueue(queue *q){
    if(q->head == NULL)
        return true;
    else
        return false;
}
int FullQueue(queue *q){
    return false;
}
void Enqueue(queue *q,node *n){
    node *temp = newNode(n->val);
    if(q->head == NULL)
        q->head = q->tail = temp;
    else {
        q->tail->next = temp;
        q->tail = temp;
    }
}
node *Dequeue(queue *q){
    if(EmptyQueue(q))
        return NULL;
    else {
        node *temp = newNode(q->head->val);
        q->head = q->head->next;
        if(q->head == NULL)
            q->tail = NULL;
        return temp;
    }
}
void DestroyQueue(queue *q){
    if(!EmptyQueue(q)){
        node *temp = newNode(q->head->val);
        while(temp != NULL){
            temp = q->head->next;
            free(q->head);
            q->head = temp;
        }
    }
}
void printQueue(queue *q,FILE *fp){
    node *temp = q->head;
    if(q->head == NULL)
        return;
    printNode(temp,fp);
    while(temp->next != NULL){
        temp = temp->next;
        printNode(temp,fp);
    }
}
