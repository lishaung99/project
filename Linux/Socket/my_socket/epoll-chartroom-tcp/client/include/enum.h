#ifndef _ENUM_H_
#define _ENUM_H_

/*标志*/
enum Flag{
	YES,	/*代表被禁言*/
	NO		/*代表没有被禁言*/
};

/*定义服务器--客户端 消息传送类型*/
enum MessageType{	
	REGISTER = 1,	/*注册请求*/		
	LOGIN,		/*登陆请求*/
	HELP,		/*帮助请求*/
	EXIT,				/*退出请求*/
	VIEW_USER_LIST,		/*查看在线列表*/
	GROUP_CHAT,		/*群聊请求*/
	PERSONAL_CHAT,		/*私聊请求*/
	VIEW_RECORDS,		/*查看聊天记录请求*/
	RESULT,				/*结果消息类型*/
	UNKONWN				/*未知请求类型*/
};

/*定义操作结果 */
enum StateRet{
    EXCEED, //已达服务器链接上限
    SUCCESS, //成功
	FAILED,  //失败
    DUPLICATEID, //重复的用户名
	INVALID,	//不合法的用户名
    ID_NOT_EXIST, //账号不存在
    WRONGPWD, //密码错误
	ALREADY_ONLINE,		//已经在线
	ID_NOT_ONLINE,	//账号不在线
	ALL_NOT_ONLINE, 	//无人在线
	MESSAGE_SELF   //消息对象不能选择自己
};

#endif