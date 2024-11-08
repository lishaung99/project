/*
 * @Description: 
 * @Author: lishuang
 * @Date: 2024-11-08 17:42:23
 * @FilePath: \undefinedd:\LearningPro\git\Linux\Socket\TCP\Client.c
 * @LastEditTime: 2024-11-08 17:43:11
 * @LastEditors: lishuang
 */
// my_client.c  TCP client
#include <stdio.h>          // 包含标准输入输出库
#include <stdlib.h>         // 包含标准库函数，如 exit
#include <string.h>         // 包含字符串处理函数，如 memset 和 strcpy
#include <unistd.h>         // 包含 Unix 标准函数，如 close
#include <arpa/inet.h>      // 包含网络地址转换函数，如 inet_pton
#include <sys/socket.h>     // 包含套接字相关函数

#define PORT 10000          // 定义服务器端口号
#define IP "127.0.0.1"      // 定义服务器 IP 地址
#define BUFFER_SIZE 1024    // 定义缓冲区大小

void start_client() {
    int clientsock;
    struct sockaddr_in serveraddr;
    char buffer[BUFFER_SIZE];

    // 创建客户端套接字
    clientsock = socket(AF_INET, SOCK_STREAM, 0);
    if (clientsock == -1) {
        perror("fail to create client socket");
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, IP, &(serveraddr.sin_addr)) <= 0) {
        perror("Failed to set server IP");
        exit(EXIT_FAILURE);
    }

    // 连接服务器
    if (connect(clientsock, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1) {
        perror("fail to connect server");
        exit(EXIT_FAILURE);
    }
    printf("Connected to server %s:%d\n", IP, PORT);

    // 发送数据
    strcpy(buffer, "hello server");
    if (send(clientsock, buffer, strlen(buffer), 0) == -1) {
        perror("fail to send data");
        exit(EXIT_FAILURE);
    }
    printf("send data success  %s  \n", buffer);

    // 接收数据
    memset(buffer, 0, sizeof(buffer));
    if (recv(clientsock, buffer, BUFFER_SIZE, 0) == -1) {
        perror("fail to recv data");
        exit(EXIT_FAILURE);
    }
    printf("Server response: %s\n", buffer);

    // 关闭套接字
    close(clientsock);
}

int main() {
    start_client();
    return 0;
}