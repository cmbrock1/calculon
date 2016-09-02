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
#include "Fatal.h"

/* options */
/* -v print Author's Name and exit */
bool SETVAR = false;
bool EndOfFILE = false;

int ProcessOptions(int,char **);
queue *ProcessInputExpression(FILE *);
queue *InfixToPostfix(queue *);
int getWeight(node *);
value *EvalPostfix(queue *);
value *Combine(value *,value *,value *);
int isPlus(value *);
int isInt(value *);

int main(int argc,char **argv){
    int argIndex;
    value *answer;
    argIndex = ProcessOptions(argc,argv);
    queue *infix = newQueue();
    queue *postfix = newQueue();
    if (argIndex == argc){
        while(!EndOfFILE){
            infix = ProcessInputExpression(stdin);
            if(infix != NULL){
                printQueue(infix,stdout);
                postfix = InfixToPostfix(infix);
                answer = EvalPostfix(postfix);
            }
        }
    }
    else {
        FILE *fp;
        fp = fopen(argv[argIndex], "r");
        while(!EndOfFILE){
            infix = ProcessInputExpression(fp);
            if(infix != NULL){
                printQueue(infix,stdout);
                postfix = InfixToPostfix(infix);
                answer = EvalPostfix(postfix);
            }
        }
        fclose(fp);
    }
    printQueue(postfix,stdout);
    if(answer != NULL){
        displayValue(stdout,answer);
        printf("\n");
    }
    free(infix);
    free(postfix);
    //printf("press any key to continue...\n");
    //getchar();
    return 0;
}

/* only -oXXX  or -o XXX options */

int
ProcessOptions(int argc, char **argv)
{
    int argIndex;
    int argUsed;
    int separateArg;
    char *arg;

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
            arg = argv[argIndex+1];
            separateArg = 1;
            }
        else
            arg = argv[argIndex]+2;

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
    value *v;
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
        else if (*token == '-' || isdigit(*token))
            v = newIntegerValue(atoi(token));
        else if (*token == '+' || *token == '-' || *token == '*' ||
                *token == '/'|| *token == '=')
            v = newOperatorValue(token);
        else if (strcmp(token,"var") == 0)
            v = newVariableValue(token);
        else
            Fatal("The token %s is not a value\n",token);
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
    int weight;
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
        ch = '~';
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

value *EvalPostfix(queue *postfix){
    stack *s = newStack();
    node *temp = postfix->head;
    node *nval = NULL;
    while(temp != NULL){
        if(strcmp(temp->val->type,OPERATOR) != 0)
            Push(temp,s);
        else {
            value *arg1 = s->top->val;
            Pop(s);
            value *arg2 = s->top->val;
            nval = newNode(Combine(temp->val,arg2,arg1));
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
value *Combine(value *op,value *arg1,value *arg2){
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
    else
        return NULL;
}
