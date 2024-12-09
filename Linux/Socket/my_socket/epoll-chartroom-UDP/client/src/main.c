#include "config.h"

/****************************************************************
 **	描  述:
 屏蔽signo信号
 **	输  入:
 **	输  出:
 **	返回值:
 *****************************************************************/
void BlockSigno(int32 signo) 
{ /* 阻塞掉某个信号 */
	sigset_t signal_mask;
	sigemptyset(&signal_mask); /* 初始化信号集,并清除signal_mask中的所有信号 */
	sigaddset(&signal_mask, signo); /* 将signo添加到信号集中 */
	sigprocmask(SIG_BLOCK, &signal_mask, NULL); /* 这个进程屏蔽掉signo信号 */
}

/****************************************************************
 **	描  述:
 主函数
 **	输  入: 
 **	输  出: 
 返回值:
 ****************************************************************/
int32 main(int32 argc, char *argv[])
{
	int32 sockfd;
    int32 choice;

 	/*声明消息变量*/
	Message message;
	/*声明消息缓冲区*/
	char buf[MAX_LINE];

	/*UserInfo*/
	User user;
	strcpy(user.userName , "***");
	user.speak = 1;
   
	//屏蔽socket套接字已关闭导致程序崩溃
	BlockSigno(SIGPIPE);

    init_printf("Start\n");
	// 1 初始化
	sockfd = InitialSocketClient();

	while(1)
	{
		/*(4) 显示聊天室主界面*/		
		mainInterface();	
		setbuf(stdin,NULL); //是linux中的C函数，主要用于打开和关闭缓冲机制
		scanf("%d",&choice);
		setbuf(stdin,NULL);
		while(choice != 1 && choice != 2 && choice != 3 && choice !=4)
		{
			printf("未找到命令，请重新输入！\n");
			setbuf(stdin,NULL); //是linux中的C函数，主要用于打开和关闭缓冲机制
			scanf("%d",&choice);
			setbuf(stdin,NULL);
		}//while

		/*清空缓冲区*/		
		switch(choice)
		{		
			case REGISTER:	/*注册请求*/		
				memset(&message , 0 , sizeof(message));
				memset(buf , 0 , MAX_LINE);		
				message.msgType = REGISTER;
				strcpy(message.content , "");
				message.sendAddr = servaddr;
				/*首先向服务器发送注册请求*/		
				memcpy(buf , &message , sizeof(message));	
				send(sockfd , buf , sizeof(buf) , 0);	
				registerUser(sockfd);
				//goto sign;
				break;
			case LOGIN:		/*登陆请求*/
				memset(&message , 0 , sizeof(message));
				memset(buf , 0 , MAX_LINE);
				message.msgType = LOGIN;
				strcpy(message.content , "");
				message.sendAddr = servaddr;
				/*向服务器发送登陆请求*/
				memcpy(buf , &message , sizeof(message));
				send(sockfd , buf , sizeof(buf) , 0);
				loginUser(sockfd);					
				break;	
			case HELP:		/*帮助请求，显示帮助界面*/
				helpInterface(); 
				//goto sign;                                                                
				break;
			case EXIT:
				close(sockfd);
				printf("退出聊天室!\n");
				exit(0);	/*用户退出*/
				break;
			default:
				printf("unknown operation.\n");
				//goto sign;
				break;	
		}//switch
	}

	close(sockfd);
	return 0;
}
