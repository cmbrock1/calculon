/*******************************************************************
*   node.c
*   Cameron Brock
*   Programming Assignment 1 calculon
*
*   This program is entirely my own work
*******************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "node.h"
#include "Fatal.h"
void initNode(node *n){
    n->next = NULL;
    n->left = NULL;
    n->right = NULL;
}
node *newNode(value *v){
    node *n;
    if ((n = malloc(sizeof(node))) == 0)
        Fatal("out of memory\n");
    initNode(n);
    n->val = v;
    return n;
}
void printNode(node *n,FILE *fp){
    displayValue(fp,n->val);
}
