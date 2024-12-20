/*******************************************************************************
* 服务器处理用户基本操作处理实现文件
*
********************************************************************************/

#include "config.h"

/*********************************************
函数名：registerUser
功能：用户注册函数实现
参数：msg--用户发送的注册消息 sockfd--套接字描述符
返回值：成功登陆返回SUCCESS 否则返回异常类型
**********************************************/
int registerUser(Message *msg , int sockfd)
{
	int ret;
	/*声明用户需要的注册信息*/
	User user;
	char buf[MAX_LINE];
	
	/*声明数据库变量*/
	sqlite3 *db;
	sqlite3_stmt *stmt; // 预编译 SQL 语句的指针。通过这个指针可以执行 SQL 语句并处理查询结果。
	const char *tail;

	/*声明sql语句存储变量*/
	char sql[128];

	/*当前系统时间*/
	time_t timeNow;

	/*存储操作结果消息*/
	Message message;

	/*接收用户注册信息*/
	recv(sockfd , buf , sizeof(user) , 0);
	memset(&user , 0 , sizeof(user));
	memcpy(&user , buf , sizeof(buf));
	user.userAddr = (*msg).sendAddr;
	user.sockfd = sockfd;
	
	if(strlen(user.userName) > 20)
	{	
		return INVALID;
	}//if

	/*（1）打开数据库*/
	ret = sqlite3_open(DB_NAME, &db);
	if(ret != SQLITE_OK)
	{
		printf("unable open database.\n");
		return FAILED;
	}//if
	printf("Opened database successfully.\n");

	/*（2）检查要注册用户名是否已存在？*/
	memset(sql , 0 , sizeof(sql)); // 初始化sql语句
	sprintf(sql , "select * from User where userName='%s';",(user.userName));

	ret = sqlite3_prepare(db , sql , strlen(sql) , &stmt , &tail);	// 预编译 （数据库，sql语句， 语句长度， 预编译语句指针， 剩余语句指针）
	if(ret != SQLITE_OK)
	{
		ret = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		printf("database select fail!\n");
		return FAILED;
	}//if
	/*执行*/
	ret = sqlite3_step(stmt);
	//如果有数据则返回SQLITE_ROW，当到达末尾返回SQLITE_DONE
	 while (ret == SQLITE_ROW)// 用户已存在
	 {
	     ret = sqlite3_step(stmt); // 继续查找，移动到下一行
		 sqlite3_finalize(stmt); // 销毁句柄，释放资源
		 sqlite3_close(db);//关闭数据库
		 return DUPLICATEID; // 用户已存在
	 }
	/*销毁句柄，关闭数据库*/
	sqlite3_finalize(stmt);

	/*执行插入操作*/
	memset(sql , 0 , sizeof(sql));
	time(&timeNow);
	sprintf(sql , "insert into User(userName , password , userAddr , sockfd , speak , registerTime)\
			values('%s','%s','%s',%d, %d , '%s');",user.userName , user.password , 
			inet_ntoa(user.userAddr.sin_addr),user.sockfd , YES, asctime(gmtime(&timeNow)));

	ret = sqlite3_prepare(db , sql , strlen(sql) , &stmt , &tail);	
	if(ret != SQLITE_OK)
	{
		ret = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return FAILED;
	}//if
	
	/*顺利注册*/
	ret = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	sqlite3_close(db);
	/*注册成功*/	
	return SUCCESS;
}
