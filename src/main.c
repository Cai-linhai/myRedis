#include <stdio.h>
#include <string.h>
#include "sds.h"

int main()
{
    sds name = sdsnew("cailinhai");

    printf("len = %d.\n", sdslen(name));

    return 0;
}