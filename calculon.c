/*******************************************************************
*   calculon.c
*   Cameron Brock
*   Programming Assignment 1 calculon
*
*   This program is entirely my own work
*******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#include "scanner.h"
#include "queue.h"
#include "stack.h"
#include "value.h"
#include "tree.h"
#include "Fatal.h"

/* options */
/* -v print Author's Name and exit */
bool SETVAR = false;
bool EndOfFILE = false;
bool OptionD = false;

int ProcessOptions(int,char **);
queue *ProcessInputExpression(FILE *);
queue *InfixToPostfix(queue *);
int getWeight(node *);
value *EvalPostfix(tree *,queue *);
value *Combine(tree *,value *,value *,value *);

int main(int argc,char **argv){
    int argIndex;
    value *answer = NULL;
    tree *vars = newTree();
    argIndex = ProcessOptions(argc,argv);
    queue *infix = newQueue();
    queue *postfix = newQueue();
    FILE *fp = NULL;
    if (argIndex == argc){
        fp = stdin;
    }
    else {
        fp = fopen(argv[argIndex], "r");
    }
    while(!EndOfFILE){
        infix = ProcessInputExpression(fp);
        if(infix != NULL){
            //printQueue(infix,stdout);
            postfix = InfixToPostfix(infix);
            //printQueue(postfix,stdout);
            answer = EvalPostfix(vars,postfix);
        }
    }
    if(OptionD == true)
        printQueue(postfix,stdout);
    else if(answer != NULL){
        displayValue(stdout,answer);
        printf("\n");
    }
    //free(infix);
    //free(postfix);
    //free(vars);
    if(fp != NULL)
        fclose(fp);
    return 0;
}

/* only -oXXX  or -o XXX options */

int
ProcessOptions(int argc, char **argv)
{
    int argIndex;
    int argUsed;
    int separateArg;
    //char *arg;

    argIndex = 1;

    while (argIndex < argc && *argv[argIndex] == '-')
        {
        /* check if stdin, represented by "-" is an argument */
        /* if so, the end of options has been reached */
        if (argv[argIndex][1] == '\0') return argIndex;

        separateArg = 0;
        argUsed = 0;

        if (argv[argIndex][2] == '\0')
            {
            //arg = argv[argIndex+1];
            separateArg = 1;
            }
        //else
            //arg = argv[argIndex]+2;

        switch (argv[argIndex][1])
            {
            /*
             * when option has an argument, do this
             *
             *     examples are -m4096 or -m 4096
             *
             *     case 'm':
             *         MemorySize = atol(arg);
             *         argUsed = 1;
             *         break;
             *
             *
             * when option does not have an argument, do this
             *
             *     example is -a
             *
             *     case 'a':
             *         PrintActions = 1;
             *         break;
             */

            case 'v':
                printf("Cameron Brock\n");
                argUsed = 1;
                exit(0);
                break;
            case 'd':
                OptionD = true;
                break;
            default:
                Fatal("option %s not understood\n",argv[argIndex]);
            }

        if (separateArg && argUsed)
            ++argIndex;

        ++argIndex;
        }

    return argIndex;
}

static value *readValue(FILE *fp){
    value *v = NULL;
    if (stringPending(fp))
        v = newStringValue(readString(fp));
    else
        {
        char *token = readToken(fp);
        if (token == 0){
            EndOfFILE = true;
            return NULL;
        }
        else if (*token == ';')
                v = newSemicolonValue();
        else if (strchr(token,'.') != 0) // dot found!
            v = newRealValue(atof(token));
        else if (((*token == '-') && (isdigit(token[1]))) || isdigit(*token))
            v = newIntegerValue(atoi(token));
        else if (*token == '+' || *token == '-' || *token == '*' ||
                 *token == '/' || *token == '%' || *token == '^' ||
                 *token == '=' || *token == '(' || *token == ')')
            v = newOperatorValue(token);
        else if (strcmp(token,"var") == 0)
            v = newVariableValue(token);
        else
            v = newVariableValue(token);
        }
    return v;
}

node *addInputToNode(FILE *fp){
    value *v = readValue(fp);
    node *n = newNode(v);
    return n;
}

queue *ProcessInputExpression(FILE *fp){
    queue *q = newQueue();
    node *n = addInputToNode(fp);
    if(n->val != NULL){
        if(strcmp(n->val->type,VARIABLE) == 0 && strcmp(n->val->sval,"var") == 0){
            SETVAR = true;
            n = addInputToNode(fp);
        }
        while(strcmp(n->val->type,SEMICOLON) != 0){
            Enqueue(q,n);
            n = addInputToNode(fp);
        }
        return q;
    }
    else
        return NULL;
}

queue *InfixToPostfix(queue *infix){
    int weight = 0;
    stack *s = newStack();
    queue *postfix = newQueue();
    node *temp = infix->head;
    while(temp != NULL){
        if(strcmp(temp->val->type,OPERATOR) == 0){
            if(*temp->val->sval == '('){
                Push(temp,s);
                temp = temp->next;
                continue;
            }
            else if(*temp->val->sval == ')'){
                while(!EmptyStack(s) && *s->top->val->sval != '('){
                    if(*s->top->val->sval != '(' && *s->top->val->sval != ')')
                        Enqueue(postfix,s->top);
                    Pop(s);
                }
                if(!EmptyStack(s))
                    Pop(s);
                temp = temp->next;
                continue;
            }
            else{
                weight = getWeight(temp);
                if(EmptyStack(s))
                    Push(temp,s);
                else {
                    while(!EmptyStack(s) && *s->top->val->sval != '(' &&
                           weight <= getWeight(s->top)){
                        Enqueue(postfix,s->top);
                        Pop(s);
                    }
                    Push(temp,s);
                }
            }
        }
        else
            Enqueue(postfix,temp);
        temp = temp->next;
    }
    while(!EmptyStack(s)){
        Enqueue(postfix,s->top);
        Pop(s);
    }
    free(s);
    return postfix;
}

int getWeight(node *n) {
    char ch;
    if(strcmp(n->val->type,OPERATOR) != 0)
        return 0;
    else
        ch = *n->val->sval;
    switch (ch) {
        case '^': return 7;
        case '%': return 6;
        case '/': return 5;
        case '*': return 4;
        case '-': return 3;
        case '+': return 2;
        case '=': return 1;
        default : return 0;
    }
}

value *EvalPostfix(tree *vars,queue *postfix){
    stack *s = newStack();
    node *temp = postfix->head;
    node *nval = NULL;
    if( postfix->head != NULL && postfix->head == postfix->tail){
        if(strcmp(postfix->head->val->type,VARIABLE) == 0){
            nval = Search(vars,postfix->head->val->sval);
            return nval->val;
        }
        else
            return postfix->head->val;
    }
    while(temp != NULL){
        if(strcmp(temp->val->type,OPERATOR) != 0)
            Push(temp,s);
        else {
            value *arg2 = s->top->val;
            Pop(s);
            value *arg1 = s->top->val;
            //if(*temp-val->sval == '=' && strcmp(arg1,VARIABLE) != 0){
            //    Pop(s);
            //    arg1 = s->top->val;
            //}
            Pop(s);
            nval = newNode(Combine(vars,temp->val,arg1,arg2));
            Push(nval,s);
        }
        temp = temp->next;
    }
    if(nval != NULL)
        return nval->val;
    else return NULL;
}
int isPlus(value *op){
    if(*op->sval == '+')
        return true;
    else
        return false;
}
int isMinus(value *op){
    if(*op->sval == '-')
        return true;
    else
        return false;
}
int isMult(value *op){
    if(*op->sval == '*')
        return true;
    else
        return false;
}
int isDivide(value *op){
    if(*op->sval == '/')
        return true;
    else
        return false;
}
int isMod(value *op){
    if(*op->sval == '%')
        return true;
    else
        return false;
}
int isExp(value *op){
    if(*op->sval == '^')
        return true;
    else
        return false;
}
int isEqual(value *op){
    if(*op->sval == '=')
        return true;
    else
        return false;
}
int isInt(value *arg1){
    if(strcmp(arg1->type,INTEGER) == 0)
        return true;
    else
        return false;
}
int isReal(value *arg1){
    if(strcmp(arg1->type,REAL) == 0)
        return true;
    else
        return false;
}
int isString(value *arg1){
    if(strcmp(arg1->type,STRING) == 0)
        return true;
    else
        return false;
}
int isVar(value *arg1){
    if(strcmp(arg1->type,VARIABLE) == 0)
        return true;
    else
        return false;
}
void *VariableNotDeclared(char *str){
    fprintf(stderr,"variable %s was not declared.\n",str);
    exit(1);
    return NULL;
}
value *Combine(tree *t,value *op,value *arg1,value *arg2){
    // Int arg1 and Int arg2
    if(isPlus(op) && isInt(arg1) && isInt(arg2))
        return newIntegerValue(arg1->ival + arg2->ival);
    else if(isMinus(op) && isInt(arg1) && isInt(arg2))
        return newIntegerValue(arg1->ival - arg2->ival);
    else if(isMult(op) && isInt(arg1) && isInt(arg2))
        return newIntegerValue(arg1->ival * arg2->ival);
    else if(isDivide(op) && isInt(arg1) && isInt(arg2))
        return newIntegerValue(arg1->ival / arg2->ival);
    else if(isMod(op) && isInt(arg1) && isInt(arg2))
        return newIntegerValue(arg1->ival % arg2->ival);
    else if(isExp(op) && isInt(arg1) && isInt(arg2))
        return newIntegerValue(pow(arg1->ival,arg2->ival));
    // Real arg1 and Int arg2
    else if(isPlus(op) && isReal(arg1) && isInt(arg2))
        return newRealValue(arg1->rval + arg2->ival);
    else if(isMinus(op) && isReal(arg1) && isInt(arg2))
        return newRealValue(arg1->rval - arg2->ival);
    else if(isMult(op) && isReal(arg1) && isInt(arg2))
        return newRealValue(arg1->rval * arg2->ival);
    else if(isDivide(op) && isReal(arg1) && isInt(arg2))
        return newRealValue(arg1->rval / arg2->ival);
    else if(isMod(op) && isReal(arg1) && isInt(arg2))
        return newRealValue(fmod(arg1->rval,arg2->ival));
    else if(isExp(op) && isReal(arg1) && isInt(arg2))
        return newRealValue(pow(arg1->rval,arg2->ival));
    // Int arg1 and Real arg2
    else if(isPlus(op) && isInt(arg1) && isReal(arg2))
        return newRealValue(arg1->ival + arg2->rval);
    else if(isMinus(op) && isInt(arg1) && isReal(arg2))
        return newRealValue(arg1->ival - arg2->rval);
    else if(isMult(op) && isInt(arg1) && isReal(arg2))
        return newRealValue(arg1->ival * arg2->rval);
    else if(isDivide(op) && isInt(arg1) && isReal(arg2))
        return newRealValue(arg1->ival / arg2->rval);
    else if(isMod(op) && isInt(arg1) && isReal(arg2))
        return newRealValue(fmod(arg1->ival,arg2->rval));
    else if(isExp(op) && isInt(arg1) && isReal(arg2))
        return newRealValue(pow(arg1->ival,arg2->rval));
    // Real arg1 and Real arg2
    else if(isPlus(op) && isReal(arg1) && isReal(arg2))
        return newRealValue(arg1->rval + arg2->rval);
    else if(isMinus(op) && isReal(arg1) && isReal(arg2))
        return newRealValue(arg1->rval - arg2->rval);
    else if(isMult(op) && isReal(arg1) && isReal(arg2))
        return newRealValue(arg1->rval * arg2->rval);
    else if(isDivide(op) && isReal(arg1) && isReal(arg2))
        return newRealValue(arg1->rval / arg2->rval);
    else if(isMod(op) && isReal(arg1) && isReal(arg2))
        return newRealValue(fmod(arg1->rval,arg2->rval));
    else if(isExp(op) && isReal(arg1) && isReal(arg2))
        return newRealValue(pow(arg1->rval,arg2->rval));
    // String arg1 and Int arg2
    else if(isPlus(op) && isString(arg1) && isInt(arg2))
        return newIntegerValue(atoi(arg1->sval) + arg2->ival);
    else if(isMinus(op) && isString(arg1) && isInt(arg2))
        return newIntegerValue(atoi(arg1->sval) - arg2->ival);
    else if(isMult(op) && isString(arg1) && isInt(arg2))
        return newIntegerValue(atoi(arg1->sval) * arg2->ival);
    else if(isDivide(op) && isString(arg1) && isInt(arg2))
        return newIntegerValue(atoi(arg1->sval) / arg2->ival);
    else if(isMod(op) && isString(arg1) && isInt(arg2))
        return newIntegerValue(atoi(arg1->sval) % arg2->ival);
    else if(isExp(op) && isString(arg1) && isInt(arg2))
        return newIntegerValue(pow(atoi(arg1->sval),arg2->ival));
    //  Int arg1 and String arg2
    else if(isPlus(op) && isInt(arg1) && isString(arg2))
        return newIntegerValue(arg1->ival + atoi(arg2->sval));
    else if(isMinus(op) && isInt(arg1) && isString(arg2))
        return newIntegerValue(arg1->ival - atoi(arg2->sval));
    else if(isMult(op) && isInt(arg1) && isString(arg2))
        return newIntegerValue(arg1->ival * atoi(arg2->sval));
    else if(isDivide(op) && isInt(arg1) && isString(arg2))
        return newIntegerValue(arg1->ival / atoi(arg2->sval));
    else if(isMod(op) && isInt(arg1) && isString(arg2))
        return newIntegerValue(arg1->ival % atoi(arg2->sval));
    else if(isExp(op) && isInt(arg1) && isString(arg2))
        return newIntegerValue(pow(arg1->ival,atoi(arg2->sval)));
    // String arg1 and Real arg2
    else if(isPlus(op) && isString(arg1) && isReal(arg2))
        return newRealValue(atof(arg1->sval) + arg2->rval);
    else if(isMinus(op) && isString(arg1) && isReal(arg2))
        return newRealValue(atof(arg1->sval) - arg2->rval);
    else if(isMult(op) && isString(arg1) && isReal(arg2))
        return newRealValue(atof(arg1->sval) * arg2->rval);
    else if(isDivide(op) && isString(arg1) && isReal(arg2))
        return newRealValue(atof(arg1->sval) / arg2->rval);
    else if(isMod(op) && isString(arg1) && isReal(arg2))
        return newRealValue(fmod(atof(arg1->sval),arg2->rval));
    else if(isExp(op) && isString(arg1) && isReal(arg2))
        return newRealValue(pow(atof(arg1->sval),arg2->rval));
    //  Real arg1 and String arg2
    else if(isPlus(op) && isReal(arg1) && isString(arg2))
        return newRealValue(arg1->rval + atof(arg2->sval));
    else if(isMinus(op) && isReal(arg1) && isString(arg2))
        return newRealValue(arg1->rval - atof(arg2->sval));
    else if(isMult(op) && isReal(arg1) && isString(arg2))
        return newRealValue(arg1->rval * atof(arg2->sval));
    else if(isDivide(op) && isReal(arg1) && isString(arg2))
        return newRealValue(arg1->rval / atof(arg2->sval));
    else if(isMod(op) && isReal(arg1) && isString(arg2))
        return newRealValue(fmod(arg1->rval,atof(arg2->sval)));
    else if(isExp(op) && isReal(arg1) && isString(arg2))
        return newRealValue(pow(arg1->rval,atof(arg2->sval)));
    // String arg1 and String arg2
    else if(isPlus(op) && isString(arg1) && isString(arg2)){
        char *concat = malloc(sizeof(char) * (strlen(arg1->sval) + strlen(arg2->sval) + 1));
        if (concat == 0) Fatal("out of memory\n");
        sprintf(concat,"%s%s",arg1->sval,arg2->sval);
        return newStringValue(concat);
    }
    else if(isEqual(op) && isVar(arg1) && SETVAR == true){
        node *n = newNode(arg2);
        n->key = arg1->sval;
        //printf("Key is %s\n",arg1->sval);
        if(Search(t,arg1->sval) != NULL)
            Delete(t,arg1->sval);
        Insert(t,n);
        //printf("Value of %s is: ",n->key);
        //printf("%d\n",Search(t,n->key)->val->ival);
        return n->val;
    }
    else if(isPlus(op) && isVar(arg1) && isInt(arg2)){
        node *n = Search(t,arg1->sval);
        if (n == NULL)
            return VariableNotDeclared(arg1->sval);
        if(strcmp(n->val->type,INTEGER) == 0)
            return newIntegerValue(n->val->ival + arg2->ival);
        else if(strcmp(n->val->type,REAL) == 0)
            return newRealValue(n->val->rval + arg2->ival);
        else if(strcmp(n->val->type,STRING) == 0)
            return newIntegerValue(atoi(n->val->sval) + arg2->ival);
        else
            return NULL;
    }
    else if(isMinus(op) && isVar(arg1) && isInt(arg2)){
        node *n = Search(t,arg1->sval);
        if (n == NULL)
            return VariableNotDeclared(arg1->sval);
        if(strcmp(n->val->type,INTEGER) == 0)
            return newIntegerValue(n->val->ival - arg2->ival);
        else if(strcmp(n->val->type,REAL) == 0)
            return newRealValue(n->val->rval - arg2->ival);
        else if(strcmp(n->val->type,STRING) == 0)
            return newIntegerValue(atoi(n->val->sval) - arg2->ival);
        else
            return NULL;
    }
    else if(isMult(op) && isVar(arg1) && isInt(arg2)){
        node *n = Search(t,arg1->sval);
        if (n == NULL)
            return VariableNotDeclared(arg1->sval);
        if(strcmp(n->val->type,INTEGER) == 0)
            return newIntegerValue(n->val->ival * arg2->ival);
        else if(strcmp(n->val->type,REAL) == 0)
            return newRealValue(n->val->rval * arg2->ival);
        else if(strcmp(n->val->type,STRING) == 0)
            return newIntegerValue(atoi(n->val->sval) * arg2->ival);
        else
            return NULL;
    }
    else if(isDivide(op) && isVar(arg1) && isInt(arg2)){
        node *n = Search(t,arg1->sval);
        if (n == NULL)
            return VariableNotDeclared(arg1->sval);
        if(strcmp(n->val->type,INTEGER) == 0)
            return newIntegerValue(n->val->ival / arg2->ival);
        else if(strcmp(n->val->type,REAL) == 0)
            return newRealValue(n->val->rval / arg2->ival);
        else if(strcmp(n->val->type,STRING) == 0)
            return newIntegerValue(atoi(n->val->sval) / arg2->ival);
        else
            return NULL;
    }
    else if(isExp(op) && isVar(arg1) && isInt(arg2)){
        node *n = Search(t,arg1->sval);
        if (n == NULL)
            return VariableNotDeclared(arg1->sval);
        if(strcmp(n->val->type,INTEGER) == 0)
            return newIntegerValue(pow(n->val->ival,arg2->ival));
        else if(strcmp(n->val->type,REAL) == 0)
            return newRealValue(pow(n->val->rval,arg2->ival));
        else if(strcmp(n->val->type,STRING) == 0)
            return newIntegerValue(pow(atoi(n->val->sval),arg2->ival));
        else
            return NULL;
    }
    else if(isMod(op) && isVar(arg1) && isInt(arg2)){
        node *n = Search(t,arg1->sval);
        if (n == NULL)
            return VariableNotDeclared(arg1->sval);
        if(strcmp(n->val->type,INTEGER) == 0)
            return newIntegerValue(n->val->ival % arg2->ival);
        else if(strcmp(n->val->type,REAL) == 0)
            return newRealValue(fmod(n->val->rval,arg2->ival));
        else if(strcmp(n->val->type,STRING) == 0)
            return newIntegerValue(atoi(n->val->sval) % arg2->ival);
        else
            return NULL;
    }
    else if(isPlus(op) && isVar(arg1) && isReal(arg2)){
        node *n = Search(t,arg1->sval);
        if (n == NULL)
            return VariableNotDeclared(arg1->sval);
        if(strcmp(n->val->type,INTEGER) == 0)
            return newRealValue(n->val->ival + arg2->rval);
        else if(strcmp(n->val->type,REAL) == 0)
            return newRealValue(n->val->rval + arg2->rval);
        else if(strcmp(n->val->type,STRING) == 0)
            return newRealValue(atof(n->val->sval) + arg2->rval);
        else
            return NULL;
    }
    else if(isMinus(op) && isVar(arg1) && isReal(arg2)){
        node *n = Search(t,arg1->sval);
        if (n == NULL)
            return VariableNotDeclared(arg1->sval);
        if(strcmp(n->val->type,INTEGER) == 0)
            return newRealValue(n->val->ival - arg2->rval);
        else if(strcmp(n->val->type,REAL) == 0)
            return newRealValue(n->val->rval - arg2->rval);
        else if(strcmp(n->val->type,STRING) == 0)
            return newRealValue(atof(n->val->sval) - arg2->rval);
        else
            return NULL;
    }
    else if(isMult(op) && isVar(arg1) && isReal(arg2)){
        node *n = Search(t,arg1->sval);
        if (n == NULL)
            return VariableNotDeclared(arg1->sval);
        if(strcmp(n->val->type,INTEGER) == 0)
            return newIntegerValue(n->val->ival / arg2->rval);
        else if(strcmp(n->val->type,REAL) == 0)
            return newRealValue(n->val->rval / arg2->rval);
        else if(strcmp(n->val->type,STRING) == 0)
            return newIntegerValue(atof(n->val->sval) / arg2->rval);
        else
            return NULL;
    }
    else if(isDivide(op) && isVar(arg1) && isReal(arg2)){
        node *n = Search(t,arg1->sval);
        if (n == NULL)
            return VariableNotDeclared(arg1->sval);
        if(strcmp(n->val->type,INTEGER) == 0)
            return newIntegerValue(n->val->ival / arg2->rval);
        else if(strcmp(n->val->type,REAL) == 0)
            return newRealValue(n->val->rval / arg2->rval);
        else if(strcmp(n->val->type,STRING) == 0)
            return newIntegerValue(atof(n->val->sval) / arg2->rval);
        else
            return NULL;
    }
    else if(isExp(op) && isVar(arg1) && isReal(arg2)){
        node *n = Search(t,arg1->sval);
        if (n == NULL)
            return VariableNotDeclared(arg1->sval);
        if(strcmp(n->val->type,INTEGER) == 0)
            return newIntegerValue(pow(n->val->ival,arg2->rval));
        else if(strcmp(n->val->type,REAL) == 0)
            return newRealValue(pow(n->val->rval,arg2->rval));
        else if(strcmp(n->val->type,STRING) == 0)
            return newIntegerValue(pow(atof(n->val->sval),arg2->rval));
        else
            return NULL;
    }
    else if(isMod(op) && isVar(arg1) && isReal(arg2)){
        node *n = Search(t,arg1->sval);
        if (n == NULL)
            return VariableNotDeclared(arg1->sval);
        if(strcmp(n->val->type,INTEGER) == 0)
            return newRealValue(fmod(n->val->ival,arg2->rval));
        else if(strcmp(n->val->type,REAL) == 0)
            return newRealValue(fmod(n->val->rval,arg2->rval));
        else if(strcmp(n->val->type,STRING) == 0)
            return newIntegerValue(fmod(atof(n->val->sval),arg2->rval));
        else
            return NULL;
    }
    else if(isPlus(op) && isInt(arg1) && isVar(arg2)){
        node *n = Search(t,arg2->sval);
        if (n == NULL)
            return VariableNotDeclared(arg2->sval);
        if(strcmp(n->val->type,INTEGER) == 0)
            return newIntegerValue(arg1->ival + n->val->ival);
        else if(strcmp(n->val->type,REAL) == 0)
            return newRealValue(arg1->rval + n->val->rval);
        else if(strcmp(n->val->type,STRING) == 0)
            return newIntegerValue(arg1->ival + atoi(n->val->sval));
        else
            return NULL;
    }
    else if(isMinus(op) && isInt(arg1) && isVar(arg2)){
        node *n = Search(t,arg2->sval);
        if (n == NULL)
            return VariableNotDeclared(arg2->sval);
        if(strcmp(n->val->type,INTEGER) == 0)
            return newIntegerValue(arg1->ival - n->val->ival);
        else if(strcmp(n->val->type,REAL) == 0)
            return newRealValue(arg1->ival - n->val->rval);
        else if(strcmp(n->val->type,STRING) == 0)
            return newIntegerValue(arg1->ival - atoi(n->val->sval));
        else
            return NULL;
    }
    else if(isMult(op) && isInt(arg1) && isVar(arg2)){
        node *n = Search(t,arg2->sval);
        if (n == NULL)
            return VariableNotDeclared(arg2->sval);
        if(strcmp(n->val->type,INTEGER) == 0)
            return newIntegerValue(arg1->ival / n->val->ival);
        else if(strcmp(n->val->type,REAL) == 0)
            return newRealValue(arg1->ival / n->val->rval);
        else if(strcmp(n->val->type,STRING) == 0)
            return newIntegerValue(arg1->ival / atoi(n->val->sval));
        else
            return NULL;
    }
    else if(isDivide(op) && isInt(arg1) && isVar(arg2)){
        node *n = Search(t,arg2->sval);
        if (n == NULL)
            return VariableNotDeclared(arg2->sval);
        if(strcmp(n->val->type,INTEGER) == 0)
            return newIntegerValue(arg1->ival / n->val->ival);
        else if(strcmp(n->val->type,REAL) == 0)
            return newRealValue(arg1->ival / n->val->rval);
        else if(strcmp(n->val->type,STRING) == 0)
            return newIntegerValue(arg1->ival / atoi(n->val->sval));
        else
            return NULL;
    }
    else if(isExp(op) && isInt(arg1) && isVar(arg2)){
        node *n = Search(t,arg2->sval);
        if (n == NULL)
            return VariableNotDeclared(arg2->sval);
        if(strcmp(n->val->type,INTEGER) == 0)
            return newIntegerValue(pow(arg1->ival,n->val->ival));
        else if(strcmp(n->val->type,REAL) == 0)
            return newRealValue(pow(arg1->ival,n->val->rval));
        else if(strcmp(n->val->type,STRING) == 0)
            return newIntegerValue(pow(arg1->ival,atoi(n->val->sval)));
        else
            return NULL;
    }
    else if(isMod(op) && isInt(arg1) && isVar(arg2)){
        node *n = Search(t,arg2->sval);
        if (n == NULL)
            return VariableNotDeclared(arg2->sval);
        if(strcmp(n->val->type,INTEGER) == 0)
            return newIntegerValue(arg1->ival % n->val->ival);
        else if(strcmp(n->val->type,REAL) == 0)
            return newRealValue(fmod(arg1->ival,n->val->rval));
        else if(strcmp(n->val->type,STRING) == 0)
            return newIntegerValue(arg1->ival % atoi(n->val->sval));
        else
            return NULL;
    }
    else if(isPlus(op) && isReal(arg1) && isVar(arg2)){
        node *n = Search(t,arg2->sval);
        if (n == NULL)
            return VariableNotDeclared(arg2->sval);
        if(strcmp(n->val->type,INTEGER) == 0)
            return newRealValue(arg1->rval + n->val->ival);
        else if(strcmp(n->val->type,REAL) == 0)
            return newRealValue(arg1->rval + n->val->rval);
        else if(strcmp(n->val->type,STRING) == 0)
            return newRealValue(arg1->rval + atoi(n->val->sval));
        else
            return NULL;
    }
    else if(isMinus(op) && isReal(arg1) && isVar(arg2)){
        node *n = Search(t,arg2->sval);
        if (n == NULL)
            return VariableNotDeclared(arg2->sval);
        if(strcmp(n->val->type,INTEGER) == 0)
            return newRealValue(arg1->rval - n->val->ival);
        else if(strcmp(n->val->type,REAL) == 0)
            return newRealValue(arg1->rval - n->val->rval);
        else if(strcmp(n->val->type,STRING) == 0)
            return newRealValue(arg1->rval - atoi(n->val->sval));
        else
            return NULL;
    }
    else if(isMult(op) && isReal(arg1) && isVar(arg2)){
        node *n = Search(t,arg2->sval);
        if (n == NULL)
            return VariableNotDeclared(arg2->sval);
        if(strcmp(n->val->type,INTEGER) == 0)
            return newRealValue(arg1->rval / n->val->ival);
        else if(strcmp(n->val->type,REAL) == 0)
            return newRealValue(arg1->rval / n->val->rval);
        else if(strcmp(n->val->type,STRING) == 0)
            return newRealValue(arg1->rval / atoi(n->val->sval));
        else
            return NULL;
    }
    else if(isDivide(op) && isReal(arg1) && isVar(arg2)){
        node *n = Search(t,arg2->sval);
        if (n == NULL)
            return VariableNotDeclared(arg2->sval);
        if(strcmp(n->val->type,INTEGER) == 0)
            return newRealValue(arg1->rval / n->val->ival);
        else if(strcmp(n->val->type,REAL) == 0)
            return newRealValue(arg1->rval / n->val->rval);
        else if(strcmp(n->val->type,STRING) == 0)
            return newRealValue(arg1->rval / atoi(n->val->sval));
        else
            return NULL;
    }
    else if(isExp(op) && isReal(arg1) && isVar(arg2)){
        node *n = Search(t,arg2->sval);
        if (n == NULL)
            return VariableNotDeclared(arg2->sval);
        if(strcmp(n->val->type,INTEGER) == 0)
            return newRealValue(pow(arg1->rval,n->val->ival));
        else if(strcmp(n->val->type,REAL) == 0)
            return newRealValue(pow(arg1->rval,n->val->rval));
        else if(strcmp(n->val->type,STRING) == 0)
            return newRealValue(pow(arg1->rval,atof(n->val->sval)));
        else
            return NULL;
    }
    else if(isMod(op) && isReal(arg1) && isVar(arg2)){
        node *n = Search(t,arg2->sval);
        if (n == NULL)
            return VariableNotDeclared(arg2->sval);
        else if(strcmp(n->val->type,INTEGER) == 0)
            return newRealValue(fmod(arg1->rval,n->val->ival));
        else if(strcmp(n->val->type,REAL) == 0)
            return newRealValue(fmod(arg1->rval,n->val->rval));
        else if(strcmp(n->val->type,STRING) == 0)
            return newRealValue(fmod(arg1->rval,atoi(n->val->sval)));
        else
            return NULL;
    }
    else if(isPlus(op) && isVar(arg1) && isVar(arg2)){
        node *n1 = Search(t,arg1->sval);
        node *n2 = Search(t,arg2->sval);
        if (n1 == NULL)
            return VariableNotDeclared(arg1->sval);
        else if (n1 == NULL)
            return VariableNotDeclared(arg2->sval);
        else if((strcmp(n1->val->type,INTEGER) == 0) && (strcmp(n2->val->type,INTEGER) == 0))
            return newIntegerValue(n1->val->ival + n2->val->ival);
        else if((strcmp(n1->val->type,REAL) == 0) && (strcmp(n2->val->type,INTEGER) == 0))
            return newRealValue(arg1->rval + n2->val->ival);
        else if((strcmp(n1->val->type,STRING) == 0) && (strcmp(n2->val->type,INTEGER) == 0))
            return newIntegerValue(atoi(n1->val->sval) + n2->val->ival);
        else if((strcmp(n1->val->type,INTEGER) == 0) && (strcmp(n2->val->type,REAL) == 0))
            return newIntegerValue(n1->val->rval + n2->val->rval);
        else if((strcmp(n1->val->type,REAL) == 0) && (strcmp(n2->val->type,REAL) == 0))
            return newRealValue(arg1->rval + n2->val->rval);
        else if((strcmp(n1->val->type,STRING) == 0) && (strcmp(n2->val->type,REAL) == 0))
            return newIntegerValue(atoi(n1->val->sval) + n2->val->rval);
        else if((strcmp(n1->val->type,INTEGER) == 0) && (strcmp(n2->val->type,STRING) == 0))
            return newIntegerValue(n1->val->rval + atoi(n2->val->sval));
        else if((strcmp(n1->val->type,REAL) == 0) && (strcmp(n2->val->type,STRING) == 0))
            return newRealValue(arg1->rval + atof(n2->val->sval));
        else if((strcmp(n1->val->type,STRING) == 0) && (strcmp(n2->val->type,STRING) == 0)){
            char *concat = malloc(sizeof(char) * (strlen(arg1->sval) + strlen(arg2->sval) + 1));
            if (concat == 0) Fatal("out of memory\n");
            sprintf(concat,"%s%s",arg1->sval,arg2->sval);
            return newStringValue(concat);
        }
        else
            return NULL;
    }
    else if(isMinus(op) && isVar(arg1) && isVar(arg2)){
        node *n1 = Search(t,arg1->sval);
        node *n2 = Search(t,arg2->sval);
        if (n1 == NULL)
            return VariableNotDeclared(arg1->sval);
        else if (n1 == NULL)
            return VariableNotDeclared(arg2->sval);
        else if((strcmp(n1->val->type,INTEGER) == 0) && (strcmp(n2->val->type,INTEGER) == 0))
            return newIntegerValue(n1->val->ival - n2->val->ival);
        else if((strcmp(n1->val->type,REAL) == 0) && (strcmp(n2->val->type,INTEGER) == 0))
            return newRealValue(arg1->rval - n2->val->ival);
        else if((strcmp(n1->val->type,STRING) == 0) && (strcmp(n2->val->type,INTEGER) == 0))
            return newIntegerValue(atoi(n1->val->sval) - n2->val->ival);
        else if((strcmp(n1->val->type,INTEGER) == 0) && (strcmp(n2->val->type,REAL) == 0))
            return newIntegerValue(n1->val->rval - n2->val->rval);
        else if((strcmp(n1->val->type,REAL) == 0) && (strcmp(n2->val->type,REAL) == 0))
            return newRealValue(arg1->rval - n2->val->rval);
        else if((strcmp(n1->val->type,STRING) == 0) && (strcmp(n2->val->type,REAL) == 0))
            return newIntegerValue(atoi(n1->val->sval) - n2->val->rval);
        else if((strcmp(n1->val->type,INTEGER) == 0) && (strcmp(n2->val->type,STRING) == 0))
            return newIntegerValue(n1->val->rval - atoi(n2->val->sval));
        else if((strcmp(n1->val->type,REAL) == 0) && (strcmp(n2->val->type,STRING) == 0))
            return newRealValue(arg1->rval - atof(n2->val->sval));
        else
            return NULL;
    }
    else if(isMult(op) && isVar(arg1) && isVar(arg2)){
        node *n1 = Search(t,arg1->sval);
        node *n2 = Search(t,arg2->sval);
        if (n1 == NULL)
            return VariableNotDeclared(arg1->sval);
        else if (n1 == NULL)
            return VariableNotDeclared(arg2->sval);
        else if((strcmp(n1->val->type,INTEGER) == 0) && (strcmp(n2->val->type,INTEGER) == 0))
            return newIntegerValue(n1->val->ival * n2->val->ival);
        else if((strcmp(n1->val->type,REAL) == 0) && (strcmp(n2->val->type,INTEGER) == 0))
            return newRealValue(arg1->rval * n2->val->ival);
        else if((strcmp(n1->val->type,STRING) == 0) && (strcmp(n2->val->type,INTEGER) == 0))
            return newIntegerValue(atoi(n1->val->sval) * n2->val->ival);
        else if((strcmp(n1->val->type,INTEGER) == 0) && (strcmp(n2->val->type,REAL) == 0))
            return newIntegerValue(n1->val->rval * n2->val->rval);
        else if((strcmp(n1->val->type,REAL) == 0) && (strcmp(n2->val->type,REAL) == 0))
            return newRealValue(arg1->rval * n2->val->rval);
        else if((strcmp(n1->val->type,STRING) == 0) && (strcmp(n2->val->type,REAL) == 0))
            return newIntegerValue(atoi(n1->val->sval) * n2->val->rval);
        else if((strcmp(n1->val->type,INTEGER) == 0) && (strcmp(n2->val->type,STRING) == 0))
            return newIntegerValue(n1->val->rval * atoi(n2->val->sval));
        else if((strcmp(n1->val->type,REAL) == 0) && (strcmp(n2->val->type,STRING) == 0))
            return newRealValue(arg1->rval * atof(n2->val->sval));
    else
        return NULL;
    }
    else if(isDivide(op) && isVar(arg1) && isVar(arg2)){
        node *n1 = Search(t,arg1->sval);
        node *n2 = Search(t,arg2->sval);
        if (n1 == NULL)
            return VariableNotDeclared(arg1->sval);
        else if (n1 == NULL)
            return VariableNotDeclared(arg2->sval);
        else if((strcmp(n1->val->type,INTEGER) == 0) && (strcmp(n2->val->type,INTEGER) == 0))
            return newIntegerValue(n1->val->ival / n2->val->ival);
        else if((strcmp(n1->val->type,REAL) == 0) && (strcmp(n2->val->type,INTEGER) == 0))
            return newRealValue(arg1->rval * n2->val->ival);
        else if((strcmp(n1->val->type,STRING) == 0) && (strcmp(n2->val->type,INTEGER) == 0))
            return newIntegerValue(atoi(n1->val->sval) / n2->val->ival);
        else if((strcmp(n1->val->type,INTEGER) == 0) && (strcmp(n2->val->type,REAL) == 0))
            return newIntegerValue(n1->val->rval / n2->val->rval);
        else if((strcmp(n1->val->type,REAL) == 0) && (strcmp(n2->val->type,REAL) == 0))
            return newRealValue(arg1->rval / n2->val->rval);
        else if((strcmp(n1->val->type,STRING) == 0) && (strcmp(n2->val->type,REAL) == 0))
            return newIntegerValue(atoi(n1->val->sval) / n2->val->rval);
        else if((strcmp(n1->val->type,INTEGER) == 0) && (strcmp(n2->val->type,STRING) == 0))
            return newIntegerValue(n1->val->rval / atoi(n2->val->sval));
        else if((strcmp(n1->val->type,REAL) == 0) && (strcmp(n2->val->type,STRING) == 0))
            return newRealValue(arg1->rval / atof(n2->val->sval));
    else
        return NULL;
    }
    else if(isExp(op) && isVar(arg1) && isVar(arg2)){
        node *n1 = Search(t,arg1->sval);
        node *n2 = Search(t,arg2->sval);
        if (n1 == NULL)
            return VariableNotDeclared(arg1->sval);
        else if (n1 == NULL)
            return VariableNotDeclared(arg2->sval);
        else if((strcmp(n1->val->type,INTEGER) == 0) && (strcmp(n2->val->type,INTEGER) == 0))
            return newIntegerValue(pow(n1->val->ival,n2->val->ival));
        else if((strcmp(n1->val->type,REAL) == 0) && (strcmp(n2->val->type,INTEGER) == 0))
            return newRealValue(pow(arg1->rval,n2->val->ival));
        else if((strcmp(n1->val->type,STRING) == 0) && (strcmp(n2->val->type,INTEGER) == 0))
            return newIntegerValue(pow(atoi(n1->val->sval),n2->val->ival));
        else if((strcmp(n1->val->type,INTEGER) == 0) && (strcmp(n2->val->type,REAL) == 0))
            return newIntegerValue(pow(n1->val->rval,n2->val->rval));
        else if((strcmp(n1->val->type,REAL) == 0) && (strcmp(n2->val->type,REAL) == 0))
            return newRealValue(pow(arg1->rval,n2->val->rval));
        else if((strcmp(n1->val->type,STRING) == 0) && (strcmp(n2->val->type,REAL) == 0))
            return newIntegerValue(pow(atoi(n1->val->sval),n2->val->rval));
        else if((strcmp(n1->val->type,INTEGER) == 0) && (strcmp(n2->val->type,STRING) == 0))
            return newIntegerValue(pow(n1->val->rval,atoi(n2->val->sval)));
        else if((strcmp(n1->val->type,REAL) == 0) && (strcmp(n2->val->type,STRING) == 0))
            return newRealValue(pow(arg1->rval,atof(n2->val->sval)));
    else
        return NULL;
    }
    else if(isMod(op) && isVar(arg1) && isVar(arg2)){
        node *n1 = Search(t,arg1->sval);
        node *n2 = Search(t,arg2->sval);
        if (n1 == NULL)
            return VariableNotDeclared(arg1->sval);
        else if (n1 == NULL)
            return VariableNotDeclared(arg2->sval);
        else if((strcmp(n1->val->type,INTEGER) == 0) && (strcmp(n2->val->type,INTEGER) == 0))
            return newIntegerValue(n1->val->ival % n2->val->ival);
        else if((strcmp(n1->val->type,REAL) == 0) && (strcmp(n2->val->type,INTEGER) == 0))
            return newRealValue(fmod(arg1->rval,n2->val->ival));
        else if((strcmp(n1->val->type,STRING) == 0) && (strcmp(n2->val->type,INTEGER) == 0))
            return newIntegerValue(atoi(n1->val->sval) % n2->val->ival);
        else if((strcmp(n1->val->type,INTEGER) == 0) && (strcmp(n2->val->type,REAL) == 0))
            return newRealValue(fmod(n1->val->ival,n2->val->rval));
        else if((strcmp(n1->val->type,REAL) == 0) && (strcmp(n2->val->type,REAL) == 0))
            return newRealValue(fmod(arg1->rval,n2->val->rval));
        else if((strcmp(n1->val->type,STRING) == 0) && (strcmp(n2->val->type,REAL) == 0))
            return newRealValue(fmod(atof(n1->val->sval),n2->val->rval));
        else if((strcmp(n1->val->type,INTEGER) == 0) && (strcmp(n2->val->type,STRING) == 0))
            return newIntegerValue(n1->val->ival % atoi(n2->val->sval));
        else if((strcmp(n1->val->type,REAL) == 0) && (strcmp(n2->val->type,STRING) == 0))
            return newRealValue(fmod(arg1->rval,atof(n2->val->sval)));
    else
        return NULL;
    }
    else {
        fprintf(stderr,"Error the operation ");
        displayValue(stderr,arg1);
        displayValue(stderr,op);
        displayValue(stderr,arg2);
        fprintf(stderr,"is not supported\n");
        exit(1);
        return NULL;
    }
}
