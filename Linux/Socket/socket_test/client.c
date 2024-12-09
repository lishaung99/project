#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 10000

void start_client() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "你好，服务器！";
    char buffer[1024] = {0};

    // 创建socket文件描述符
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 将IP地址从点分十进制转换为二进制形式
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }

    // 连接到服务器
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    // 发送数据
    if (send(sock, hello, strlen(hello), 0) < 0) {
        perror("send failed");
        exit(EXIT_FAILURE);
    }
    printf("发送的数据: %s\n", hello);

    // 接收响应
    if ((valread = read(sock, buffer, 1024)) < 0) {
        perror("read failed");
        exit(EXIT_FAILURE);
    }
    printf("接收到的响应: %s\n", buffer);

    // 关闭连接
    close(sock);
}

int main() {
    start_client();
    return 0;
}
