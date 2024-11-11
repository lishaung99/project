#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>

int main(){
    // 定义文件描述符集合
    fd_set readfds;

    // 清空文件描述符集合
    FD_ZERO(&readfds);

    // 添加标准输入文件描述符到集合
    FD_SET(STDIN_FILENO, &readfds);

    // 设置超时时间
    struct  timeval timeout;
    timeout.tv_sec = 20;
    timeout.tv_usec = 0;

    // 监听文件描述符集合
    int res = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);
    if (res == -1)
    {
        perror("failed select");
        exit(EXIT_FAILURE);
    }else if(res == 0){
        printf("time out\n");
    }else{
        if(FD_ISSET(STDIN_FILENO, &readfds)){
           printf("Data available on stdin\n");
        }
    }
    return 0;
    
}