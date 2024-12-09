/*******************************************************************************
* 基本配置文件 -- 包含所需头文件
* 用户信息结构体定义
* 在线用户链表定义
********************************************************************************/
#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <memory.h> /*使用memcpy所需的头文件*/

#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
// #include <sys/select.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <pthread.h>

#include <sqlite3.h>
#include <signal.h>
#include "sys_config.h"
#include "public_function.h"
#include "enum.h"
#include "include.h"

/*FD_SETSIZE定义描述符集的大小，定义在sys/types.h中*/
#ifndef FD_SETSIZE
#define FD_SETSIZE 256
#endif

#define IP "127.0.0.1"
#define MAX_LINE  8192
#define PORT  8888
#define LISTENEQ  6000

/*预定义数据库名称*/
#define DB_NAME "../../chatRome.db"

//自定义打印
#define init_printf(format, ...) printf(format, ##__VA_ARGS__)
#ifdef DEBUG
#define debug_printf(format, ...)   printf(format, ##__VA_ARGS__)
#define debug_line()                debug_printf("%d\n",__LINE__)
#else
#define debug_printf(format, ...)
#define debug_line()
#endif


#endif