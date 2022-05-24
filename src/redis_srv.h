#ifndef __REDIS_SRV_H
#define __REDIS_SRV_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <inttypes.h>
#include <pthread.h>
#include <syslog.h>
#include <netinet/in.h>
#include <lua.h>
#include <signal.h>
#include "adlist.h"
#include "dict.h"

/* Error codes */
#define REDIS_OK                0
#define REDIS_ERR               -1

#define REDIS_RUN_ID_SIZE       40
#define REDIS_CONFIG_HZ         60
#define REDIS_CONFIG_PORT       6379

/*
 * Redis 对象
 */
typedef struct redisObject {

    // 类型
    unsigned type:4;

    // 编码
    unsigned encoding:4;

    // 指向实际值的指针
    void *ptr;

} robj;


typedef struct redisDb {

    // 数据库键空间，保存着数据库中的所有键值对
    dict *dict;                 /* The keyspace for this DB */

    // 键的过期时间，字典的键为键，字典的值为过期事件 UNIX 时间戳
    dict *expires;              /* Timeout of keys with a timeout set */

    // 正处于阻塞状态的键
    dict *blocking_keys;        /* Keys with clients waiting for data (BLPOP) */

    // 可以解除阻塞的键
    dict *ready_keys;           /* Blocked keys that received a PUSH */

    // 正在被 WATCH 命令监视的键
    dict *watched_keys;         /* WATCHED keys for MULTI/EXEC CAS */

    struct evictionPoolEntry *eviction_pool;    /* Eviction pool of keys */

    // 数据库号码
    int id;                     /* Database ID */

    // 数据库的键的平均 TTL ，统计信息
    long long avg_ttl;          /* Average TTL, just for stats */
} redisDb;

/*
 * redis服务器状态（配置）
 *
 */
typedef struct redisServer{

    //服务器运行id
    char runid[REDIS_RUN_ID_SIZE + 1];

    //配置文件的绝对路径
    char *configfile;

    //服务器运行频率
    int hz;

    //服务器运行架构
    int arch_bits;

    //服务运行端口号
    int port;

    //记录客户端链表
    list *client;

} redisServer;


#endif