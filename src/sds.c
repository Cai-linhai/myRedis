#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "sds.h"
#include "zmalloc.h"

/*
 * 根据指定字符串，和指定字符串长度，创建一个新的的sds结构
 * 
 * str：指定字符串
 * len：指定字符串长度
 * 
 * return：创建成功，返回sds->buf
 *         创建失败，返回NULL
 */
sds sdsnewlen(const char *str, int len)
{
    sdshdr *sh;

    if (str != NULL) {
        sh = zmalloc(sizeof(sdshdr) + len + 1);
    } else {
        sh = zcalloc(sizeof(sdshdr) + len + 1);
    }

    if (sh == NULL) {
        return NULL;
    }

    sh->len = len;
    sh->avail = 0;
    if (str != NULL && len > 0) {
        memcpy(sh->buf, str, len);
    }
    sh->buf[len] = '\0';

    return sh->buf;
}

/*
 * 根据指定字符串，创建对应的sds结构
 * 
 * str：指定字符串，如果为NULL，则创建空白sds
 * 
 * return：创建成功，返回sds->buf
 *         创建失败，返回NULL
 */
sds sdsnew(const char *str)
{
    int len = 0;
    if (str != NULL) {
        len = strlen(str);
    }
    return sdsnewlen(str, len); 
}

/*
 * 释放给定的 sds
 *
 */
void sdsfree(sds s) {
    if (s == NULL) {
        return;
    }
    zfree(s-sizeof(sdshdr));
}
