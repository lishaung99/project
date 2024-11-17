#ifndef __INCLUDES_H
#define __INCLUDES_H
#include <netinet/in.h>
#include "sys_config.h"

#define STRUCT_PACKED __attribute__((packed))

/*定义服务器 -- 客户端 消息传送结构体*/
typedef struct _Message{
	char content[2048];		/*针对聊天类型的消息，填充该字段*/
	int32 msgType;	/*消息类型 即为MessageType中的值*/
	int32 msgRet;		/*针对操作结果类型的消息，填充该字段*/
	struct sockaddr_in sendAddr; /*发送者IP*/
	struct sockaddr_in recvAddr;
	char sendName[20]; /*发送者名称*/
	char recvName[20]; /*接收者名称*/
	char msgTime[20];  /*消息发送时间*/
}STRUCT_PACKED Message;

//用户信息结构体
typedef struct _User{
	char userName[20]; 		//用户名
	char password[20];
	struct sockaddr_in userAddr;	//用户IP地址，选择IPV4
	int32 sockfd;			//当前用户套接字描述符
	int32 speak;			//是否禁言标志
	char registerTime[20];	//记录用户注册时间	
}STRUCT_PACKED User;

/*定义用户链表结构体*/
typedef struct _ListNode{
	User user;
	struct _ListNode *next;
}STRUCT_PACKED ListNode;

/*定义在线用户链表*/
extern ListNode *userList;
extern struct sockaddr_in servaddr;

extern char *stateMsg(int32 stateRet);
extern void copyUser(User *user1 , User *user2);

#endif