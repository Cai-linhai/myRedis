#include <stdio.h>
#include <string.h>
#include "sds.h"

int main()
{
    sds name1 = sdsnew("cailinhai");
    sdshdr *sh = (void *)(name1 - (sizeof(sdshdr)));
    sds name2 = sdsnew("cailinhai");
    sdshdr *sh = (void *)(name2 - (sizeof(sdshdr)));

    int cmp = sdscmp(name1, name2);
    printf("name1 = name2 is %s.\n", cmp ? "false" : "true");


    sds newname = sdscpy(name1, "Cailinhai is so handsam!");
    sdshdr *newsh = (void *)(newname - (sizeof(sdshdr)));
    printf("newname copy is %s.\n", newname);
    printf("newsh->len is %d, newsh->avail is %d.\n", newsh->len, newsh->avail);

    return 0;
}
