#include "config.h"

void start_client(){

  int client_socket;
  struct sockaddr_in server_addr; // 服务器地址
  char buffer[BUFFER_SIZE]; // 缓冲区

  // 创建套接字
  client_socket = socket(AF_INET, SOCK_DGRAM, 0); // SOCK_DGRAM:UDP协议
  if(client_socket == -1){
    perror("failed to creat client Socket");
    exit(EXIT_FAILURE);
  }

  // 设置服务端
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  if(inet_pton(AF_INET, IP, &(server_addr.sin_addr)) == -1){
    perror("failed to set server address");
    exit(EXIT_FAILURE);
  }

  // 发送数据
  strcpy(buffer, "Hello, Server!");
  int status = sendto(client_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if(status == -1){
    perror("failed to send data");
    exit(EXIT_FAILURE);
  }

  // 接受数据
  memset(&buffer, 0, sizeof(buffer));
  if(recvfrom(client_socket, buffer, BUFFER_SIZE, 0, NULL, NULL) == -1){
    perror("failed to receive data");
    exit(EXIT_FAILURE);
  }
  printf("Received from server: %s\n", buffer);

  // 关闭套接字
  close(client_socket);
}
int main(){
  start_client();
  return 0;
}
