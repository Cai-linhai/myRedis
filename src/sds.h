#ifndef __SDS_H
#define __SDS_H

#include <sys/types.h>
#include <stdarg.h>

typedef char * sds;

typedef struct sdshdr_t {
    //buf中已使用的字节数
    int len;
    //buf中还空余的字节数
    int avail;
    //储存字符数据
    char buf[];
}sdshdr;

static inline int sdslen(const sds str)
{
    //获取sds->buf所对应的sds的起始地址
    sdshdr *sh = (void *)(str - (sizeof(sdshdr)));
    return sh->len;
}

static inline int sdsavail(const sds str)
{
    sdshdr *sh = (void *)(str - (sizeof(sdshdr)));
    return sh->avail;
}

sds sdsnewlen(const char *str, int strlen);
sds sdsnew(const char *str);


#endif