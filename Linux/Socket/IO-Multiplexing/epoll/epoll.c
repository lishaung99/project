#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>

int main() {
    // 使用epoll_create1函数创建一个Epoll实例
    int epoll_fd = epoll_create1(0);
    // struct epoll_event结构体来设置要监听的文件描述符和事件类型
    struct epoll_event event;
    struct epoll_event events[10];
    int timeout = 5000; // 5秒
    
    event.events = EPOLLIN;
    event.data.fd = STDIN_FILENO;
    // epoll_ctl函数将文件描述符添加到Epoll实例中
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &event);
    
    // epoll_wait函数来等待事件发生
    int num_events = epoll_wait(epoll_fd, events, 10, timeout);
    
    if (num_events == -1) {
        perror("epoll_wait error");
        exit(EXIT_FAILURE);
    } else if (num_events == 0) {
        printf("Timeout\n");
    } else {
        for (int i = 0; i < num_events; i++) {
            if (events[i].events & EPOLLIN) {
                printf("Data available on file descriptor %d\n", events[i].data.fd);
            }
        }
    }
    
    close(epoll_fd);
    
    return 0;
}
