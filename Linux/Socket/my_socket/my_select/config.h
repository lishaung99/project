#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define IP "127.0.0.1"
#define PORT 10000
#define BUFFER_SIZE 2048

#define LISTENQ 5 // 监听队列长度
