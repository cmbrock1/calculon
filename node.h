/*******************************************************************
*   node.h
*   Cameron Brock
*   Programming Assignment 1 calculon
*
*   This program is entirely my own work
*******************************************************************/
#ifndef NODE_H
#define NODE_H
#include "value.h"
typedef struct node{
    value *val;
    struct node *next;
}node;

extern node *newNode(value *v);
extern void printNode(node *n);

#endif // NODE_H
