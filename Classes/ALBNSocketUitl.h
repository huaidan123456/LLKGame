//
//  ALBNSocketUitl.h
//  LLKGame
//
//  Created by Allen on 2017/12/18.
//
//

#ifndef ALBNSocketUitl_h
#define ALBNSocketUitl_h
#include "cocos2d.h"
#include <sys/types.h>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")
#define HSocket SOCKET
// 对于android平台
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//#include <arpa/inet.h>	// for inet_**
//#include <netdb.h>	// for gethost**
//#include <netinet/in.h>	// for sockaddr_in
//#include <sys/types.h>	// for socket
//#include <sys/socket.h>	// for socket
//#include <unistd.h>
//#include <stdio.h>	     // for printf
//#include <stdlib.h>	// for exit
//#include <string.h>	// for bzero

// for socket
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/select.h>
#define HSocket int
#elif CC_TARGET_PLATFORM==CC_PLATFORM_IOS
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#define HSocket int
#endif



class ALBNSocketUitl
{
public:
    static HSocket _socketHandle;
    //** 是否连接服务器的标志 *//
    static bool _connectFlag;
    
    
    /**
     *  通知的开关
     */
    static bool _notificationSwitch;
    
    
    
    /**
     *  连接服务器
     */
    static bool connectServer(const char* ip,unsigned short port);
    
    /**
     *  登录操作
     */
    static bool loginServer(const char* ip,unsigned short port,int uid);
    
    /**
     *  接收消息
     */
    static void* threadReceiveTask();
    
    /**
     *   关闭连接
     */
    static void closeConnect();
    
    
    
    
public:
#pragma mark -- 发送和接收--
    static void sendInt(int si);
    
    static void sendFloat(float sf);
    
    static void sendString(const char* str);
    
    static void sendStr(const char* str, int len);
    
    static char* receiveBytes(int len);
    
    static int receiveInt();
    
    static float receiveFloat();
    
    static char* receiveStr();
    
    static bool Check();
    static bool hasError();
    
    static bool hasSendError();
};


#endif /* ALBNSocketUitl_h */
