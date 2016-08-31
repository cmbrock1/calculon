#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include "scanner.h"
#include "queue.h"
#include "value.h"
#include "Fatal.h"

/* options */
/* -v print Author's Name and exit */

int ProcessOptions(int,char **);
queue *ProcessInput(FILE *fp);

int main(int argc,char **argv){
    int argIndex;

    argIndex = ProcessOptions(argc,argv);

    if (argIndex == argc){
        queue *q = newQueue();
        q = ProcessInput(stdin);
        printQueue(q,stdin);
        free(q);
    }
    else {
        //ReadFromFile();
        FILE *fp;
        fp = fopen(argv[argIndex], "r");
        queue *q = newQueue();
        q = ProcessInput(fp);
        printQueue(q,stdin);
        free(q);
        fclose(fp);
    }
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
        if (strchr(token,'.') != 0) // dot found!
            v = newRealValue(atof(token));
        else if (*token == '-' || isdigit(*token))
            v = newIntegerValue(atoi(token));
        else if (*token == ';')
            v = newSemicolonValue();
        else if (*token == 0)
            v = 0;
        else
            Fatal("The token %s is not a value\n",token);
        }
    return v;
}

node *addInputToNode(FILE *fp){
    value *v = readValue(fp);
    printf("This is the value: ");
    displayValue(stdout,v);
    node *n = newNode(v);
    return n;
}

queue *ProcessInput(FILE *fp){
    queue *q = newQueue();
    node *n = addInputToNode(fp);
    printNode(n,stdin);
    while(n->val != 0){
        if(n->val->type != SEMICOLON)
            Enqueue(q,n);
        n = addInputToNode(fp);
    }
    return q;
}
