/*******************************************************************
*   tree.h
*   Cameron Brock
*   Programming Assignment 1 calculon
*
*   This program is entirely my own work
*******************************************************************/
#ifndef TREE_H
#define TREE_H
#include <stdbool.h>
#include "tree.h"
#include "node.h"
typedef struct tree{
    node *root;
}tree;

extern tree *newTree();
extern bool EmptyTree(tree *t);
extern node *Search(tree *t,char *key);
extern bool Insert(tree *t,node *newNode);
extern bool Delete(tree *t,char *key);
extern void printTree(tree *t);
#endif // TREE_H
