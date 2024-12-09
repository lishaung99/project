/*
* 客户端代码
*/
#include "config.h"

/*处理接收服务器消息函数*/
void *recv_message(void *fd)
{
	int32 sockfd = *(int *)fd;
	while(1)
	{
		char buf[MAX_LINE];
		memset(buf , 0 , MAX_LINE);
		int32 n;
		if((n = recv(sockfd , buf , MAX_LINE , 0)) == -1)
		{
			perror("recv error.\n");
			exit(1);
		}//if
		buf[n] = '\0';
		
		//若收到的是exit字符，则代表退出通信
		if(strcmp(buf , "byebye.") == 0)
		{
			printf("Server is closed.\n");
			close(sockfd);
			exit(0);
		}//if

		printf("\nServer: %s\n", buf);
	}//while
}


int32 main(int argc , char **argv)
{
	/*声明套接字和链接服务器地址*/
    int32 sockfd;
	pthread_t recv_tid , send_tid;
    struct sockaddr_in servaddr;

    // /*判断是否为合法输入*/
    // if(argc != 2)
    // {
    //     perror("usage:tcpcli <IPaddress>");
    //     exit(1);
    // }//if

	sockfd = init_socket();
	/*创建子线程处理该客户链接接收消息*/
	if(pthread_create(&recv_tid , NULL , recv_message, &sockfd) == -1)
	{
		perror("pthread create error.\n");
		exit(1);
	}//if	

	/*处理客户端发送消息*/
	char msg[MAX_LINE];
	memset(msg , 0 , MAX_LINE);
	while(fgets(msg , MAX_LINE , stdin) != NULL)	
	{
		if(strcmp(msg , "exit\n") == 0)
		{
			printf("byebye.\n");
			memset(msg , 0 , MAX_LINE);
			strcpy(msg , "byebye.");
			send(sockfd , msg , strlen(msg) , 0);
			close(sockfd);
			exit(0);
		}//if
		if(send(sockfd , msg , strlen(msg) , 0) == -1)
		{
			perror("send error.\n");
			exit(1);
		}//if
	}//while
	close(sockfd);
}