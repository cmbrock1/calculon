/*******************************************************************
*   value.h
*   Cameron Brock
*   Programming Assignment 1 calculon
*
*   This program is entirely my own work
*******************************************************************/
#ifndef VALUE_H
#define VALUE_H
#include <stdio.h>
typedef struct value{
    char* type;
    int ival;
    double rval;
    char *sval;
}value;

extern char *INTEGER;
extern char *REAL;
extern char *STRING;
extern char *VARIABLE;
extern char *OPERATOR;
extern char *SEMICOLON;

extern value *newIntegerValue(int);
extern value *newRealValue(double);
extern value *newStringValue(char *);
extern value *newVariableValue(char *);
extern value *newOperatorValue(char *);
extern value *newSemicolonValue(void);
extern void displayValue(FILE *,value *);
#endif // VALUE_H
