#ifndef __PUBLIC_FUNCTION_H
#define __PUBLIC_FUNCTION_H

#include "include.h"
/********************************************************************************************************************/
/*                                                              定义全局函数                                                                                      */
/********************************************************************************************************************/
// initialzation.c
int32 Initialization(void);
int32 InitialSocketClient(void);

// interface.c
int32 mainInterface();
int32 helpInterface();
void chatInterface(char *);

// login.c
int32 loginUser(int32 );

// register.c
int32 registerUser(int32 );

// chat.c
void recvMsg(int32 *);
void enterChat(User *user , int32 sockfd);
#endif
