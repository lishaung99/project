/*
 * config.h 包含该tcp/ip套接字编程所需要的基本头文件，与server.c client.c位于同一目录下
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "sys_config.h"
#include "public_fuction.h"
#include <signal.h>


#define MAX_LINE 2048
#define PORT 6001
#define BACKLOG 10
#define LISTENQ 6666
#define MAX_CONNECT 20
// #define IP "127.0.0.1"

