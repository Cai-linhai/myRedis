#ifndef __SDS_H
#define __SDS_H

#include <sys/types.h>
#include <stdarg.h>

/*
 * 最大预分配长度
 */
#define SDS_MAX_PREALLOC (1024*1024)

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

extern sds sdsnewlen(const char *str, int strlen);
extern sds sdsnew(const char *str);
extern sds sdsnewempty(void);
extern void sdsfree(sds buf);
extern sds sdsdup(const sds buf);
extern void sdsclear(sds buf);
extern sds sdsMakeRoomFor(sds buf, int addlen);
extern sds sdsRemoveFreeSpace(sds buf);
extern sds sdscatlen(sds buf, sds str, int len);
extern sds sdscat(sds buf, const sds str);
extern sds sdscpylen(sds buf, sds str, int cpylen);
extern sds sdscpy(sds buf, const sds str);
extern int sdscmp(sds buf1, sds buf2);


#endif