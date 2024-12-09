头文件及其用途

<stdio.h>
用途：标准输入输出库。
提供：printf, scanf, fopen, fclose 等标准输入输出函数。

<stdlib.h>
用途：标准库函数。
提供：malloc, free, exit, atexit 等内存管理和程序控制函数。

<string.h>
用途：字符串操作函数。
提供：strcpy, strncpy, strlen, strcmp, strcat 等字符串操作函数。

<errno.h>
用途：错误号定义。
提供：errno 变量和相关的错误号常量，用于错误处理

<unistd.h>
用途：Unix 标准函数。
提供：read, write, close, pipe, fork, exec 等系统调用函数。

<time.h>
用途：时间日期函数。
提供：time, clock, difftime, mktime, strftime 等时间日期操作函数。

<sys/socket.h>
用途：套接字编程。
提供：socket, bind, listen, accept, connect, send, recv 等套接字相关函数。

<sys/select.h>
用途：select 函数。
提供：select 函数和相关的 fd_set 结构体。

<sys/types.h>
用途：基本系统数据类型。
提供：pid_t, uid_t, gid_t, size_t, ssize_t 等基本数据类型。

<netinet/in.h>
用途：互联网协议定义。
提供：struct sockaddr_in, inet_addr, ntohl, htons 等互联网协议相关的结构体和函数。

<arpa/inet.h>
用途：IP 地址转换。
提供：inet_addr, inet_ntoa, inet_pton, inet_ntop 等IP地址转换函数。

<netdb.h>
用途：网络数据库操作。
提供：gethostbyname, gethostbyaddr, getservbyname, getservbyport 等网络数据库查询函数。

<fcntl.h>
用途：文件控制选项。
提供：open, fcntl, O_RDONLY, O_WRONLY, O_RDWR 等文件控制函数和常量。