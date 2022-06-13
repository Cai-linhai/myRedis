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

    Server.fd_cnt = 0;
}

int listenClient(int port, int *fd, int *count)
{

}

void Server_init(void)
{
    //初始化客户端
    Server.client = listCreate();

    //监听客户端
    if (Server.port != 0) {
        if (listenClient(Server.port, Server.fd, &Server.fd_cnt) == REDIS_ERR) {
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