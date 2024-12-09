#include "config.h"
int32 init_socket(void){
	/*声明套接字和链接服务器地址*/
    int sockfd;
	struct sockaddr_in servaddr;
	/*(1) 创建套接字*/
    if((sockfd = socket(AF_INET , SOCK_STREAM , 0)) == -1)
    {
        perror("socket error");
        exit(1);
    }//if

    /*(2) 设置链接服务器地址结构*/
    bzero(&servaddr , sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    if(inet_pton(AF_INET , IP , &servaddr.sin_addr) < 0)
    {
        printf("inet_pton error for %s\n",IP);
        exit(1);
    }//if

    /*(3) 发送链接服务器请求*/
    if( connect(sockfd , (struct sockaddr *)&servaddr , sizeof(servaddr)) < 0)
    {
        perror("connect error");
        exit(1);
    }//if	
	return sockfd;
}