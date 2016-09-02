/*******************************************************************
*   value.c
*   Cameron Brock
*   Programming Assignment 1 calculon
*
*   This program is entirely my own work
*******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "value.h"

char *INTEGER = "INTEGER";
char *REAL = "REAL";
char *STRING = "STRING";
char *VARIABLE = "VARIABLE";
char *OPERATOR = "OPERATOR";
char *SEMICOLON = "SEMICOLON";

static value *newValue(char *);

/*************** public interface *************/

value *
newIntegerValue(int v)
    {
    value *n = newValue(INTEGER);
    n->ival = v;
    return n;
    }

value *
newRealValue(double v)
    {
    value *n = newValue(REAL);
    n->rval = v;
    return n;
    }

value *
newStringValue(char *v)
    {
    value *n = newValue(STRING);
    n->sval = v;
    return n;
    }

value *
newVariableValue(char *v)
    {
    value *n = newValue(VARIABLE);
    n->sval = v;
    return n;
    }

value *
newOperatorValue(char *v)
    {
    value *n = newValue(OPERATOR);
    n->sval = v;
    return n;
    }
value *
newSemicolonValue(void)
    {
    value *n = newValue(SEMICOLON);
    return n;
    }

void
displayValue(FILE *fp,value *v)
    {
    if (strcmp(v->type,STRING) == 0)
       fprintf(fp,"\"%s\" ",v->sval);
    else if (strcmp(v->type,INTEGER) == 0)
       fprintf(fp,"%d ",v->ival);
    else if (strcmp(v->type,REAL) == 0)
       fprintf(fp,"%f ",v->rval);
    else if (strcmp(v->type,VARIABLE) == 0)
       fprintf(fp,"%s ",v->sval);
    else if (strcmp(v->type,OPERATOR) == 0)
       fprintf(fp,"%s ",v->sval);
    else if (strcmp(v->type,SEMICOLON) == 0)
       fprintf(fp,";\n");
    else
       fprintf(fp,"<UNKNOWN VALUE TYPE>");
    }

/*************** private methods *************/

static value *newValue(char *t)
{
    value *n = malloc(sizeof(value));
    if (n == 0) { fprintf(stderr,"out of memory"); exit(-1); }
    n->type = t;
    return n;
}
