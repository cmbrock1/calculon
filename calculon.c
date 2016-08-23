#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "scanner.h"

/* options */
/* -v print Author's Name and exit */

int ProcessOptions(int,char **);
void Fatal(char *,...);
void ReadFromUser(void);
void ReadFromFile(char *filename);

int main(int argc,char **argv){
    int argIndex;

    //if (argc == 1) Fatal("%d arguments!\n",argc-1);

    argIndex = ProcessOptions(argc,argv);

    if (argIndex == argc)
        ReadFromUser();
    else {
        //ReadFromFile();
        FILE *fp;
        fp = fopen(argv[argIndex], "r");
        fclose(fp);
    }

    return 0;
}

void
Fatal(char *fmt, ...)
    {
    va_list ap;

    fprintf(stderr,"An error occured: ");
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    exit(-1);
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

void ReadFromUser(){
    char token[500];
    strcpy(token,readToken(stdin));
    
    printf("token = %s\n",token);
    while((token != ";") || (token != EOF)){
        strcpy(token,readToken(stdin));
    }
}
