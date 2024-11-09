/*
 * @Description: 
 * @Author: lishuang
 * @Date: 2024-11-08 17:52:45
 * @FilePath: \undefinedd:\LearningPro\project\Linux\Socket\TCP\config.h
 * @LastEditTime: 2024-11-08 17:52:59
 * @LastEditors: lishuang
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define IP "127.0.0.1"  // 定义服务器 IP 地址为本地回环地址
#define PORT 10000     // 定义服务器端口号
#define BUFFER_SIZE 1024       // 定义缓冲区大小