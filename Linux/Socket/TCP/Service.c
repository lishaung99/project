/*
 * @Description: 
 * @Author: lishuang
 * @Date: 2024-11-08 17:46:00
 * @FilePath: \undefinedd:\LearningPro\project\Linux\Socket\TCP\Service.c
 * @LastEditTime: 2024-11-08 17:53:34
 * @LastEditors: lishuang
 */
// socket TCP连接 服务器端
#include "config.h"
void start_service(){
	
	int socket_service, socket_client;
	struct sockaddr_in serviceAddr;
	struct sockaddr_in clientAddr;
	socklen_t client_len;
	char buffer[BUFFER_SIZE];
	// 创建套接字
	socket_service = socket(AF_INET, SOCK_STREAM, 0);// AF_INET=ipv4 SOCK_STRAM=tcp套接字
	if(socket_service == -1){
		perror("failed to creart service");
	}
	//设置服务器地址
    memset(&serviceAddr, 0, sizeof(serviceAddr)); // 清空 serverAddr 结构体，使它的所有字节都设置为 0。
	serviceAddr.sin_family = AF_INET;
	serviceAddr.sin_port = htons(PORT);
	serviceAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    // printf("server started in %d port \n", PORT); 

	// 绑定套接字和端口
    int status = bind(socket_service, (struct sockaddr *)&serviceAddr, sizeof(serviceAddr));
    if(status == -1){
        perror("failed to bind service");
        exit(EXIT_FAILURE);
    }
    // 监听连接请求
    if(listen(socket_service, 5) == -1){
        perror("failed to listen");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d...\n", PORT);

    // 接受连接请求
    client_len = sizeof(clientAddr);
    socket_client = accept(socket_service, (struct sockaddr *)&clientAddr, &client_len);
    if(socket_client == -1){
        perror("failed to accept");
        exit(EXIT_FAILURE);
    }
    // 接收数据
    memset(buffer, 0, sizeof(buffer)); // 这一行代码的目的是清空 buffer 数组，将其内容全部设为零
    if(recv(socket_client, buffer, BUFFER_SIZE, 0) == -1){
        perror("failed to recv");
        exit(EXIT_FAILURE);
    }
    printf("recv: %s\n", buffer);

    // 发送数据
    strcpy(buffer, "hello client");
    if(send(socket_client, buffer, BUFFER_SIZE, 0) == -1){
        perror("failed to send");
        exit(EXIT_FAILURE);
    }

    //关闭套接字
    close(socket_client);
    close(socket_service);
	
}
int main(){
    start_service();
    return 0;
}