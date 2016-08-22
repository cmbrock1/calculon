/*******************************************************************
*   value.h
*   Cameron Brock
*   Programming Assignment 1 calculon
*
*   This program is entirely my own work
*******************************************************************/
#ifndef VALUE_H
#define VALUE_H
typedef struct value{
    int type;
    int ival;
    double rval;
    char *sval;
    //char *varval;
}value;

extern int INTEGER;
extern int REAL;
extern int STRING;
extern int VARIABLE;

extern value *newValueI(int);
extern value *newValueR(double);
extern value *newValueS(char *);
extern value *newValueV(char *);
extern void printValue(value *v);
#endif // VALUE_H
