#include <stdio.h>
#include <string.h>
#include "sds.h"

int main()
{
    sds name = sdsnew("cailinhai");
    sdshdr *sh = (sdshdr *)(name - (sizeof(sdshdr)));

    sds newname = sdscat(name, " is so handsam!");
    sdshdr *newsh =  (sdshdr *)(newname - (sizeof(sdshdr)));
    
    printf("name is %s.\n", name);
    printf("name len is %d, avail is %d.\n", sh->len, sh->avail);
    printf("newname is %s.\n", newname);
    printf("newname len is %d, avail is %d.\n", newsh->len, newsh->avail);

    return 0;
}
