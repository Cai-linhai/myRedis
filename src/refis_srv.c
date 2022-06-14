#include "redis_srv.h"

#include <time.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/uio.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <sys/resource.h>
#include <sys/utsname.h>
#include <locale.h>

/* 服务器状态配置全局变量 */
redisServer Server;

void getRunid(char *ptr, int size)
{
    char *charset = "0123456789abcdef";

    for(int i = 0; i < size; i++) {
        ptr[i] ^= rand();
        ptr[i] = charset[ptr[i] & 0x0F];
    }
    ptr[size] = '\0';
}

void ServerConfig_init(void)
{
    getRunid(Server.runid, REDIS_RUN_ID_SIZE);

    Server.configfile = NULL;

    Server.hz = REDIS_CONFIG_HZ;

    Server.port = REDIS_CONFIG_PORT;

    Server.addr = NULL;

    Server.fd_cnt = 0;

    Server.tcp_backlog = REDIS_TCP_BACKLOG;
}

int listenClient(char *addr, int port, int *fd, int *count, int backlog)
{
    struct addrinfo hints;
    struct addrinfo *result;
    struct addrinfo *p;
    char _port[6];
    int ret, s;

    snprintf(_port, 6, "%d", port);
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_flags = AI_PASSIVE;

    ret = getaddrinfo(addr, port, &hints, &result);
    if (ret != 0) {
        return REDIS_ERR;
    }

    for (p = result; p != NULL; p = p->ai_next) {
        s = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (s == -1) {
            continue;
        }

        ret = bind(s, p->ai_addr, p->ai_addr_len);
        if (ret == -1) {
            close(s);
            return REDIS_ERR;
        }

        ret = listen(s, backlog);
        if (ret == -1) {
            close(s);
            return REDIS_ERR;
        }
 
        fd[*count] = s;
        (*count)++;
    }

    if (p == NULL) {
        return REDIS_ERR;
    }

    freeaddrinfo(result);

    return REDIS_OK;
}

void Server_init(void)
{   
    int ret;

    //初始化客户端
    Server.client = listCreate();

    //监听客户端
    if (Server.port != 0) {
        ret = listenClient(Server.addr, Server.port, Server.fd, &Server.fd_cnt, Server.tcp_backlog);
        if ( ret == REDIS_ERR) {
            exit(1);
        }
    }

    
}

int main(int argc, char **argv)
{
    //初始化服务器默认配置
    ServerConfig_init();

    //检查是否使用指定的配置文件
    if (argc > 2) {
        fprintf(stderr,"Please enter redis config.\n");
    }

    Server_init();

    return;
}