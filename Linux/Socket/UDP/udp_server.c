// UDP 服务端 
#include "config.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/socket.h>
// #include <arpa/inet.h>
// #include <unistd.h>

// #define IP "127.0.0.1"  // 定义服务器 IP 地址为本地回环地址
// #define PORT 10000     // 定义服务器端口号
// #define BUFFER_SIZE 1024       // 定义缓冲区大小
void start_service(){
  int socket_service, socket_client;
  struct sockaddr_in serverAddr, clientAddr;
  char buffer[1024];
  socklen_t clientAddlen;

  // 创建套接字
  socket_service = socket(AF_INET, SOCK_DGRAM, 0); //SOCK_DGRAM 表示使用 UDP 协议 SOCK_STREAM 表示使用 TCP 协议
  if (socket_service == -1) {
    perror("failed to create socket");
    exit(EXIT_FAILURE);
  }
  // 绑定服务器
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(PORT);
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  if(bind(socket_service, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1){
    perror("failed to bind socket");
    exit(EXIT_FAILURE);
  }
  printf("server start %s:%d\n", IP, PORT);

  // 接收客户端消息
  memset(buffer, 0, sizeof(buffer));
  clientAddlen = sizeof(clientAddr); // 接收客户端消息
  if(recvfrom(socket_service, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&clientAddr, &clientAddlen) == -1){
    perror("failed to receive message");
    exit(EXIT_FAILURE);
  }
  printf("Client message: %s\n", buffer);

  // 发送消息给客户端
  strcpy(buffer, "Hello, Client!");
  if(sendto(socket_service, buffer, strlen(buffer), 0, (struct sockaddr *)&clientAddr, clientAddlen) == -1){
    perror("failed to send data");
    exit(EXIT_FAILURE);
  }

  close(socket_service);
}
int main(){
  start_service();
  return 0;
}