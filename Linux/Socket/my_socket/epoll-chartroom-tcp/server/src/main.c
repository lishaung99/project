#include "config.h"

/****************************************************************
 **	描  述:
 屏蔽signo信号
 **	输  入:
 **	输  出:
 **	返回值:
 *****************************************************************/
void BlockSigno(int signo) 
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
int32 main(int argc, char *argv[])
{
	int32 listenfd, connfd, epollfd;
    socklen_t clilen;
    struct epoll_event ev, events[FD_SETSIZE];
    struct sockaddr_in cliaddr;
    pthread_t pid;

	//屏蔽socket套接字已关闭导致程序崩溃
	BlockSigno(SIGPIPE);

    init_printf("Start\n");
	// 1 初始化
	listenfd = InitialSocketServer();

    /* (5) 创建epoll实例 */
    epollfd = epoll_create(200); // 创建epoll实例(并发数量)
    if (epollfd == -1)
    {
        perror("epoll_create error.\n");
        exit(EXIT_FAILURE);
    }

    /* (6) 将监听套接字加入epoll实例 */
    ev.events = EPOLLIN;
    ev.data.fd = listenfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev) == -1)
    {
        perror("epoll_ctl: listenfd");
        exit(EXIT_FAILURE);
    }

   /* (7) 接收客户端连接请求 */
    while (1)
    {
        int nready = epoll_wait(epollfd, events, FD_SETSIZE, -1); // 等待事件
        if (nready < 0)
        {
            perror("epoll_wait error.\n");
            exit(EXIT_FAILURE);
        }
        else if(nready == 0)
        {
			debug_printf("timing out!\n");
			continue;            
        }
        else
        {
            for (int32 i = 0; i < nready; i++)
            {
                if (events[i].events & EPOLLIN)// 检测到有数据可读
                {
                    // 处理新的客户端连接
                    if(events[i].data.fd == listenfd)
                    {
                        clilen = sizeof(cliaddr);
                        if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen)) < 0)
                        {
                            perror("accept error.\n");
                            exit(1);
                        }

                        printf("server: got connection from %s\n", inet_ntoa(cliaddr.sin_addr));

                        // 将新的客户端连接加入epoll监听
                        ev.events = EPOLLIN; // 水平触发
                        ev.data.fd = connfd;
                        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &ev) == -1)
                        {
                            perror("epoll_ctl: connfd");
                            exit(EXIT_FAILURE);
                        }

                        if (events[i].events & EPOLLIN)
                        {
                            pthread_create(&pid, NULL, (void *)handleRequest, (void *)&connfd);
                            // 等待线程结束，回收线程资源
                            pthread_join(pid, NULL);  // 确保线程资源被回收
                        }
                    }
                    else
                    {
                        // 处理已有连接的消息                       
                    }
                }

            }
        }
        
    }

    free(events);
	close(listenfd);
	return 0;
}
