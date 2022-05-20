#include <stdio.h>
#include <string.h>
#include "sds.h"

int main()
{
    sds name = sdsnew("cailinhai");
    sdshdr *sh =  (void *)(name - (sizeof(sdshdr)));

    printf("len = %d.\n", sdslen(name));
    printf("sizeof sdshdr %d.\n", sizeof(sdshdr));
    printf("sds->buf address %p.\n", name);
    printf("sds address %p.\n", sh);


    return 0;
}