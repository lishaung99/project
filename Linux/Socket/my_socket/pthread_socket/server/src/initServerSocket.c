#include "config.h"

int32 InitServerSockrt(void){
    	//声明套接字
	int32 listenfd , connfd;
	socklen_t clilen;


	//定义地址结构
	struct sockaddr_in servaddr;
	
	/*(1) 创建套接字*/
	if((listenfd = socket(AF_INET , SOCK_STREAM , 0)) == -1)
	{
		perror("socket error.\n");
		exit(1);
	}//if

	/*(2) 初始化地址结构*/
	bzero(&servaddr , sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	/*(3) 绑定套接字和端口*/
	if(bind(listenfd , (struct sockaddr *)&servaddr , sizeof(servaddr)) < 0)
	{
		perror("bind error.\n");
		exit(1);
	}//if

	/*(4) 监听*/
	if(listen(listenfd , LISTENQ) < 0)
	{
		perror("listen error.\n");
		exit(1);
	}//if
    return listenfd;
}