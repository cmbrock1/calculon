/*******************************************************************
*   queue.h
*   Cameron Brock
*   Programming Assignment 1 calculon
*
*   This program is entirely my own work
*******************************************************************/
#ifndef QUEUE_H
#define QUEUE_H
#include "node.h"
typedef struct queue{
    node *head;
    node *tail;
}queue;

extern void initQueue(queue *q);
extern int EmptyQueue(queue *q);
extern int FullQueue(queue *q);
extern void Enqueue(queue *q,node *n);
extern node *Dequeue(queue *q);
#endif // QUEUE_H
