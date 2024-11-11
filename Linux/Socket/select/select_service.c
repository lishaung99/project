//  socket——select 服务端 server.c
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

#define PORT 10000
#define BUFFER_SIZE 12048

#define LISTENQ 20 // 监听队列长度

void start_service(){
    int socket_service; // 服务端套接字
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen;
    char buffer[BUFFER_SIZE];

    // 创建套接字
    if((socket_service = socket(AF_INET, SOCK_STREAM, 0)) == -1){ // AF_INET:ipv4 SOCK_STREAM:TCP
        perror("listenfd socket error");
        exit(EXIT_FAILURE);
    }
    
    // 绑定地址和端口
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(socket_service, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 监听
    if(listen(socket_service, LISTENQ) == -1){
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    // 设置select参数
    fd_set readset , allset; // readset:监听读事件 allset:监听所有事件
    int i, maxi, maxfd, socket_client, sockfd;
	int nready , client[FD_SETSIZE];
	ssize_t n, ret;
	maxfd = socket_service;
    maxi = -1; // client数组下标
    for(int i = 0; i < FD_SETSIZE; ++i)
	{
		client[i] = -1;
	}
    // 清空集合
	FD_ZERO(&allset);

	FD_SET(socket_service, &allset);
    while(1){ // 监听所有事件
        readset = allset;
        nready = select(maxfd + 1, &readset, NULL, NULL, NULL);
        
        if(FD_ISSET(maxfd, &readset)){// 监听服务端套接字
            clientAddrLen = sizeof(clientAddr);
            if((socket_client = accept(socket_service, (struct sockaddr *)&clientAddr, &clientAddrLen)) == -1){
                perror("accept failed");
                exit(EXIT_FAILURE);
            }
            printf("new client connected %s:%d\n",inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
            // 将新连接的套接字加入到集合中
            for(i = 0; i < FD_SETSIZE; ++i){
                if(client[i] < 0){
                    client[i] = socket_client;
                    break;
                }
            }
            if(i == FD_SETSIZE){
                printf("too many clients\n");
                exit(EXIT_FAILURE);
            }
            FD_SET(socket_client, &allset);
            if(socket_client > maxfd){
                maxfd = socket_client; // 更新最大值
            }
            if(i > maxi){
                maxi = i; // 更新最大下标
            }
            if(--nready <= 0){
                continue;
            }
        }

        for(int i = 0; i <= maxi; ++i){
            if((sockfd = client[i]) < 0){
                continue;
            }
            if(FD_ISSET(sockfd, &readset)){ // 监听客户端套接字
                if((n = read(sockfd, buffer, BUFFER_SIZE)) == 0){ // 客户端关闭
                    close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                }else{
                    printf("clint[%d] send message: %s\n", i , buffer);
                    if((ret = write(sockfd, buffer, n)) == -1){
                        perror("write error");
                        exit(EXIT_FAILURE);
                    }
                }

                if(--nready <= 0){
                    break;
                }

            }
        }  
    }
}
int main(int argc, char *argv[]){
    start_service();
    return 0;
}