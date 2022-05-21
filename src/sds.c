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

    // 根据是否有初始化内容，选择适当的内存分配方式
    if (str != NULL) {
        sh = zmalloc(sizeof(sdshdr) + len + 1);
    } else {
        //calloc在动态分配完内存后，自动初始化该内存空间为零
        //而malloc不做初始化，分配到的空间中的数据是随机数据
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
 * 创建并返回一个只保存了空字符串 "" 的 sds
 *
 * return：创建成功返回 sdshdr 相对应的 sds
 *         创建失败返回 NULL
 */
sds sdsnewempty(void)
{
    return sdsnewlen("", 0);
}

/*
 * 释放给定的 sds
 *
 */
void sdsfree(sds buf) {
    if (buf == NULL) {
        return;
    }
    zfree(buf - sizeof(sdshdr));
}

/*
 * 复制给定 sds 的副本
 *
 * return：创建成功返回输入 sds 的副本
 *         创建失败返回 NULL
 */
sds sdsdup(const sds buf)
{
    return sdsnewlen(buf, sdslen(buf));
}

/*
 * 在不释放 SDS 的字符串空间的情况下，
 * 重置 SDS 所保存的字符串为空字符串。
 */
void sdsclear(sds buf)
{
    sdshdr *sh = (void *)(buf - (sizeof(sdshdr)));

    sh->avail = sh->avail + sh->len;
    sh->len = 0;

    sh->buf[0] = '\0';
}

/*
 * 对 sds 中 buf 的长度进行扩展，确保在函数执行之后，
 * buf 至少会有 addlen + 1 长度的空余空间
 * （额外的 1 字节是为 \0 准备的）
 *
 * return：扩展成功返回扩展后的 sds
 *         扩展失败返回 NULL
 */
sds sdsMakeRooomFor(sds buf, int addlen)
{
    int avail;
    int len, newlen;
    sdshdr *sh, *newsh;

    avail = sdsavail(buf);
    
    //sds中可用空间大于需要扩展的空间，则无需扩展，直接返回
    if (avail >= addlen) {
        return buf;
    }

    len = sdslen(buf);
    newlen = len + addlen;

    if (newlen < SDS_MAX_PREALLOC) {
        newlen = newlen * 2;
    } else {
        newlen = newlen + SDS_MAX_PREALLOC;
    }

    sh = (void *)(buf - (sizeof(sdshdr)));

    //realloc 更改已经配置的内存空间，即更改由malloc()函数分配的内存空间的大小(扩大或缩小)
    //realloc 扩大空间，如果原地址空间后续由足够的连续空间，则直接在原地址上扩展，否则在堆中重新寻找一块足够的空间进行分配
    newsh = (sdshdr *)realloc(sh, sizeof(sdshdr) + newlen + 1);

    //扩展空间失败,原sh空间还在
    if (newsh == NULL) {
        return NULL;
    }

    newsh->len = len;
    newsh->avail = newlen - len;

    return newsh->buf;
}

/*
 * 回收 sds 中的空闲空间，
 * 回收不会对 sds 中保存的字符串内容做任何修改。
 *
 * return：内存调整后的 sds
 */
sds sdsRemoveFreeSpace(sds buf)
{
    int len;
    sdshdr *sh;

    len = sdslen(buf);
    sh = (void *)(buf - (sizeof(sdshdr)));

    //realloc缩小空间，则就在原地址空间上做缩减
    sh = (sdshdr *)realloc(sh, sizeof(sdshdr) + len + 1);

    sh->avail = 0;

    return sh->buf;
}

sds sdscatlen(sds buf, sds str, int len)
{
    int curlen;
    sdshdr *sh;

    if(str == NULL || len == 0) {
        return buf;
    }

    curlen = sdslen(buf);

    buf = sdsMakeRooomFor(buf, len);
    if (buf == NULL) {
        return NULL;
    }

    memcpy(buf + curlen, str, len);

    sh = (void *)(buf - (sizeof(sdshdr)));

    sh->len = curlen + len;
    sh->avail = sh->avail - len;

    buf[curlen + len] = '\0';

    return sh->buf;
}

/*
 * 将给定字符串 t 追加到 sds 的末尾
 * 
 * return：追加成功返回新 sds ，失败返回 NULL
 */
sds sdscat(sds buf, const sds str)
{
    return sdscatlen(buf, str, strlen(str));
}
