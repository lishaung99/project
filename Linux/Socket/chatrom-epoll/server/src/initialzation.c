#include "config.h"

/* 定义全局变量 -- 在线用户链表 */
ListNode *userList = NULL;

/****************************************************************
 **描  述:
   初始化服务端socket服务
 **输  入:
 **输  出:
 **返回值:
*****************************************************************/
int32 InitialSocketServer(void)
{
    int32 listenfd, connfd, sockfd;
    /* 套接字选项 */
    int opt = 1;
    /* 声明服务器地址和客户地址结构 */
    struct sockaddr_in servaddr;

   /* (1) 创建套接字 */
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket error.\n");
        exit(1);
    } // if

    /* (2) 初始化地址结构 */
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    /* (3) 绑定套接字和端口 */
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind error.\n");
        exit(1);
    } // if

    /* (4) 监听 */
    if (listen(listenfd, LISTENEQ) < 0)
    {
        perror("listen error.\n");
        exit(1);
    } // if

    return listenfd;
}

/* 处理客户请求的线程 */
void *handleRequest(int *fd)
{
    int sockfd, ret, n;
    /* 声明消息变量 */
    Message message;
    /* 声明消息缓冲区 */
    char buf[MAX_LINE];

    sockfd = *fd;

    memset(buf, 0, MAX_LINE);
    memset(&message, 0, sizeof(message));

    // 接收用户发送的消息
    n = recv(sockfd, buf, sizeof(buf), 0);
    if (n <= 0)
    {
        // 如果连接关闭或出现错误
        fflush(stdout);
        close(sockfd);
        *fd = -1;
        printf("来自%s的退出请求！\n", inet_ntoa(message.sendAddr.sin_addr));
        return NULL;
    }
    else
    {
        // 打印接收到的消息长度和消息类型
        printf("收到来自客户端的消息，长度: %d\n", n);
        memcpy(&message, buf, sizeof(message));

        // 打印接收到的消息类型
        printf("消息类型: %d\n", message.msgType);

        // 检查消息类型是否有效
        if (message.msgType != REGISTER && message.msgType != LOGIN)
        {
            printf("未知操作，msgType = %d\n", message.msgType);
            return NULL;  // 直接返回，避免继续处理无效消息
        }

        /* 为每个客户端操作链接创建一个线程 */
        switch (message.msgType)
        {
        case REGISTER:
        {
            printf("来自%s的注册请求！\n", inet_ntoa(message.sendAddr.sin_addr));
            ret = registerUser(&message, sockfd);
            memset(&message, 0, sizeof(message));
            message.msgType = RESULT;
            message.msgRet = ret;
            strcpy(message.content, stateMsg(ret));
            memset(buf, 0, MAX_LINE);
            memcpy(buf, &message, sizeof(message));
            /* 发送操作结果消息 */
            send(sockfd, buf, sizeof(buf), 0);
            printf("注册：%s\n", stateMsg(ret));
            close(sockfd);
            *fd = -1;
            return NULL;
        }
        case LOGIN:
        {
            printf("来自%s的登陆请求！\n", inet_ntoa(message.sendAddr.sin_addr));
            ret = loginUser(&message, sockfd);
            memset(&message, 0, sizeof(message));
            message.msgType = RESULT;
            message.msgRet = ret;
            strcpy(message.content, stateMsg(ret));
            memset(buf, 0, MAX_LINE);
            memcpy(buf, &message, sizeof(message));
            /* 发送操作结果消息 */
            send(sockfd, buf, sizeof(buf), 0);
            printf("登录：%s\n", stateMsg(ret));
            /* 进入服务器处理聊天界面 */
            enterChat(&sockfd);
            break;
        }
        default:
            // 未知操作
            printf("未知操作，msgType = %d\n", message.msgType);
            break;
        }
    }

    close(sockfd);
    *fd = -1;
    return NULL;
}