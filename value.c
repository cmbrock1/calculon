/*******************************************************************
*   value.c
*   Cameron Brock
*   Programming Assignment 1 calculon
*
*   This program is entirely my own work
*******************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "value.h"
#include "Fatal.h"

//extern void Fatal(char *,...);
static value *newValue(int,int);
//static value *newValueStr(int t,int strlength);

/**** Public Interface ****/
int INTEGER = 0;
int REAL = 1;
int STRING = 2;
int VARIABLE = 3;

value *newValueI(int i){
    value *v = newValue(INTEGER,1);
    v->ival = i;
    return v;
}

value *newValueR(double r){
    value *v = newValue(REAL,1);
    v->rval = r;
    return v;
}

value *newValueS(char *s){
    value *v = newValue(STRING,strlen(s));
    strcpy(v->sval,s);
    return v;
}

value *newValueV(char *var){
    value *v = newValue(VARIABLE,strlen(var));
    strcpy(v->sval,var);
    return v;
}

void printValue(value *v){
    if(v->type == 0)
        printf("value->INTEGER = %d\n", v->ival);
    else if(v->type == 1)
        printf("value->REAL = %f\n", v->rval);
    else if(v->type == 2)
        printf("value->STRING = %s\n", v->sval);
    else
        printf("value->VARIABLE = %s\n", v->sval);
}

/**** Private Interface ****/

// static value *newValue(int t){
//     value *v;
//     if ((v = malloc(sizeof(value))) == 0){
//             Fatal("out of memory\n");
//     }
//     v->type = t;
//     v->ival = 0;
//     v->rval = 0;
//     v->sval = 0;
//     return v;
// }
static value *newValue(int t,int strlength){
    value *v;
    if ((v = (value *)malloc(sizeof(value) + strlength + 1)) == 0){
        Fatal("out of memory\n");
    }
    v->type = t;
    v->ival = 0;
    v->rval = 0;
    if ((v->sval = (char *)malloc(strlength + 1)) == 0){
        Fatal("out of memory\n");
    }
    strcpy(v->sval,"");
    return v;
}
