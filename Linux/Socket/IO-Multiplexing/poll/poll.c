#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <poll.h>

int main() {
    // struct pollfd结构体来定义要监听的文件描述符和事件类型
    struct pollfd fds[1];
    int timeout = 5000; // 5秒
    
    fds[0].fd = STDIN_FILENO; // 监听标准输入 POLLIN：0，输入 ；POLLIN：1，输出；POLLIN：2，输出错误信息
    fds[0].events = POLLIN; // 监听可读事件
    
    int result = poll(fds, 1, timeout);
    
    if (result == -1) {
        perror("poll error");
        exit(EXIT_FAILURE);
    } else if (result == 0) {
        printf("Timeout\n");
    } else {
        if (fds[0].revents & POLLIN) {
           printf("Data available on stdin\n");
        }
    }
    
    return 0;
}
