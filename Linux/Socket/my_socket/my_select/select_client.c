#include "config.h"

int max(int a, int b) {
    return a > b ? a : b;
}

/* readline函数实现 */
ssize_t readline(int fd, char *vptr, size_t maxlen) {
    ssize_t n, rc;
    char c, *ptr;

    ptr = vptr;
    for (n = 1; n < maxlen; n++) {
        if ((rc = read(fd, &c, 1)) == 1) {
            *ptr++ = c;
            if (c == '\n')
                break;  /* newline is stored, like fgets() */
        } else if (rc == 0) {
            *ptr = 0;
            return (n - 1);  /* EOF, n - 1 bytes were read */
        } else
            return (-1);  /* error, errno set by read() */
    }

    *ptr = 0;  /* null terminate like fgets() */
    return (n);
}

/* 采用select的客户端消息处理函数 */
void str_select_client(FILE *fp, int sockfd) {
    int select_fd;
    fd_set readfdset;
    char send_buffer[BUFFER_SIZE];
    char recv_buffer[BUFFER_SIZE];
    FD_ZERO(&readfdset);

    while (1) {
        FD_SET(fileno(fp), &readfdset);
        FD_SET(sockfd, &readfdset);
        select_fd = max(fileno(fp), sockfd) + 1;

        if (select(select_fd, &readfdset, NULL, NULL, NULL) == -1) {
            perror("select created error");
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(fileno(fp), &readfdset)) {
            if (fgets(send_buffer, BUFFER_SIZE, fp) == NULL) {
                printf("read nothing~\n");
                close(sockfd);  /* all done */
                return;
            }
            send_buffer[strlen(send_buffer) - 1] = '\0';
            write(sockfd, send_buffer, strlen(send_buffer));
        }

        if (FD_ISSET(sockfd, &readfdset)) {
            if (readline(sockfd, recv_buffer, BUFFER_SIZE) == 0) {
                perror("handleMsg: server terminated prematurely.\n");
                exit(EXIT_FAILURE);
            }

            if (fputs(recv_buffer, stdout) == EOF) {
                perror("fputs error");
                exit(EXIT_FAILURE);
            }
        }
    }
}

/* 功能：客户端启动函数 */
void start_client() {
    int client_socket;
    struct sockaddr_in server_addr;

    // 创建套接字
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("failed to create client socket");
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, IP, &server_addr.sin_addr) < 0) {
        perror("failed to set server address");
        exit(EXIT_FAILURE);
    }

    // 连接服务器
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("failed to connect to server");
        exit(EXIT_FAILURE);
    }

    // 发送 "hello service" 消息
    const char *message = "hello service\n";
    write(client_socket, message, strlen(message));
    printf("debug-Client: %s", message);

    // 启动消息处理函数
    str_select_client(stdin, client_socket);
}

int main(int argc, char *argv[]) {
    start_client();
    return 0;
}