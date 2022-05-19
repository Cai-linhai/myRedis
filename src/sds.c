#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "sds.h"
#include "zmalloc.h"

sds sdsnewlen(const char *str, int strlen)
{
    sdshdr *sh;

    if (str != NULL) {
        sh = zmalloc(sizeof(sdshdr) + strlen + 1);
    } else {
        sh = zcalloc(sizeof(sdshdr) + strlen + 1);
    }

    if (sh == NULL) {
        return NULL;
    }

    sh->len = strlen;
    sh->avail = 0;
    if (str != NULL && strlen > 0) {
        memcmp(sh->buf, str, strlen);
    }
    sh->buf[strlen] = '\0';

    return sh->buf;
}

sds sdsnew(const char *str)
{
    int strlen = 0;
    if (str != NULL) {
        strlen = sizeof(str);
    }
    return sdsnewlen(str, strlen); 
}
