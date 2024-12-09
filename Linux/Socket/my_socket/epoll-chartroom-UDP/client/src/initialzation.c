/*******************************************************************************
* 客户端程序代码server.c
*
********************************************************************************/

#include "config.h"
struct sockaddr_in servaddr;
/****************************************************************
 **	描  述:
        重连服务器函数
 **	输  入:
 **	输  出:
 **	返回值:
*****************************************************************/
int32 server_reconnection(void)
{
    int32 ret;
    int32 sockfd;

    // 重连参数
    int32 count = 5;
    int32 sec = 30;

	 /*(1) 创建套接字*/
	if((sockfd = socket(AF_INET , SOCK_STREAM , 0)) == -1)
	{
		perror("socket error");
		exit(1);
	}
	
	/*(2) 设置链接服务器地址结构*/
	bzero(&servaddr , sizeof(servaddr));		/*清空地址结构*/
	servaddr.sin_family = AF_INET;				/*使用IPV4通信域*/
	servaddr.sin_port = htons(PORT);			/*端口号转换为网络字节序*/
	//servaddr.sin_addr = *((struct in_addr *)host->h_addr);		/*可接受任意地址*/
    servaddr.sin_addr.s_addr = inet_addr(IP);

    // 循环重连直到连接成功
    while (1)
    {
        if (count <= 0)
        {
            sleep(sec*10);
            count = 5; //重载重连参数
        }
        init_printf("第%d次重连\n", count);
        count--;
        // 等待30s再重新连接
        sleep(sec);
        ret = (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)));
        if (-1 == ret)
        {
            continue;
        }
        else
        {
            break;
        }
        sleep(0);
    }

    return sockfd;
}

/****************************************************************
 **描  述:
   初始化客户端socket服务
 **输  入:
 **输  出:
 **返回值:socket套接字
*****************************************************************/
int32 InitialSocketClient(void)
{
    int32 ret, sockfd;

	 /*(1) 创建套接字*/
	if((sockfd = socket(AF_INET , SOCK_STREAM , 0)) == -1)
	{
		perror("socket error");
		exit(1);
	}
	
	/*(2) 设置链接服务器地址结构*/
	bzero(&servaddr , sizeof(servaddr));		/*清空地址结构*/
	servaddr.sin_family = AF_INET;				/*使用IPV4通信域*/
	servaddr.sin_port = htons(PORT);			/*端口号转换为网络字节序*/
	//servaddr.sin_addr = *((struct in_addr *)host->h_addr);		/*可接受任意地址*/
    servaddr.sin_addr.s_addr = inet_addr(IP);

	// if(inet_pton(AF_INET , IP , &servaddr.sin_addr) < 0)
	// {
	// 	printf("inet_pton error for %s\n",IP);
	// 	exit(1);
	// }
    
    init_printf("开始和上传服务器进行连接！\n");
    ret = (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)));
    if (-1 == ret)
    {
        init_printf("服务器暂时不能连接,正在等待...\n");
        sockfd = server_reconnection();
        debug_printf("number2--- %d\n", sockfd);
    }

    return sockfd;
}

/****************************************************************
 **	描  述:
        初始化系统
 **	输  入:
 **	输  出:
 **	返回值:
*****************************************************************/
int32 Initialization(void)
{
    // 1 初始化内存

    // 2 设定默认值

    // 3 初始化fifo

    // 4 初始化设备

    /*...................*/
    
    // xx 打印开机信息

    return 1;
}