#include <stdio.h>
#include <stdlib.h>
#include "Fatal.h"
void Fatal(char *msg,...) {
    fprintf(stderr, msg,  __LINE__);
    exit(1);
}
