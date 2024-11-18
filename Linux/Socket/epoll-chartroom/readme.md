# **ChatRoom**



## **项目简介**



采用I/O复用技术select实现socket通信，采用多线程负责每个客户操作处理，完成Linux下的多客户聊天室！



## **环境**

OS：

Linux ubuntu 6.2.0-37-generic #38~22.04.1-Ubuntu SMP PREEMPT_DYNAMIC Thu Nov  2 18:01:13 UTC 2 x86_64 x86_64 x86_64 GNU/Linux



IDE：vim gcc make



DB：Sqlite 3



Time：2024.11



## 项目功能架构



1. 采用从c/s结构；

2. 给出客户操作主界面（注册、登录、帮助和退出）、登录后主界面（查看在线列表、私聊、群聊、查看聊天记录、退出）；

3. 多客户可同时连接服务器进行自己操作

4. 文件结构

   ```shell
   ├── chatRome.db # 数据库文件
   
      ├── client 客户端代码
   
      │  ├── client #客户端执行文件 ./client
   
      │  ├── include # 头文件
   
      │  │  ├── config.h 
   
      │  │  ├── enum.h
   
      │  │  ├── include.h
   
      │  │  ├── public_function.h
   
      │  │  └── sys_config.h
   
      │  ├── lib
   
      │  ├── Makefile  #make文件
   
      │  └── src  # 程序c文件
   
      │    ├── chat.c        # 客户端实现用户的聊天互动操作；
   
      │    ├── config.c
   
      │    ├── initialzation.c  # epoll客户端初始化
   
      │    ├── interface.c    # 客户端界面文件
   
      │    ├── login.c     # 客户端实现用户登录；
   
      │    ├── main.c
   
      │    └── register.c
   
      ├── readme
   
      └── server # 服务端代码
   
        ├── include # 头文件
   
        │  ├── config.h
   
        │  ├── public_function.h 
   
        │  └── sys_config.h
   
        ├── lib  # 系统事件笔记
   
        │  └── readme.md
   
        ├── Makefile
   
        ├── server # 执行文件 ./server
   
        └── src # 程序c文件
   
          ├── chat.c
   
          ├── config.c
   
          ├── initialzation.c  # epoll服务器端初始化
   
          ├── list.c      # 链表实现文件，用于维护在线用户链表的添加、更新、删除操作；
   
          ├── login.c     # 服务器端实现用户登录；
   
          ├── main.c
   
          └── register.c  # 服务器端实现用户注册
   ```

   


