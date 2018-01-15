//
//  ALNetControl.h
//  LLKGame
//
//  Created by Allen on 2017/12/18.
//
//

#ifndef ALNetControl_h
#define ALNetControl_h

#include "cocos2d.h"
#include "ALNetDefine.h"

class ALNetControl
{
public:
    /**
     *  设置网络通知的开关
     */
    static void setupNetworkNotificationSwitch(bool isOpen);
    
    
    /**
     *  检测
     */
    static void checkNetwork();
    
    
    /**
     *  是否连接服务器
     */
    static bool isConnect();
    
    
    /**
     *  连接服务器
     */
    static bool connectServerWithName(const char* name, unsigned short port,int uid);
    
    
    /**
     *  连接服务器通过ip
     */
    static bool connectServerWithIp(const char* ip, unsigned short port,int uid);
    
    /**
     *  关闭连接
     */
    static void closeConnect();
    
    /**
     *  获取用户信息
     */
    static void requestUserInfo();
    
    /**
     *  刷新用户的轻豆
     */
    static void requestUserQingDouCount();
    
    /**
     *  请求用户娘卡
     */
    static void requestUserNiangCard();
    
    /**
     *  发送随机匹配
     */
    static void sendRandomMatch();
    
    /**
     *  单机 发送游戏结果
     */
    static void sendStandAloneGameResult(bool result);
    
    /**
     *  发送抽奖请求
     */
    static void sendDrawNiangCardAward();
    
    /**
     *  发送获取轻豆的方法
     */
    static void requestGetQingDou(int number);
    
    /**
     *  请求在线人数
     */
    static void requestOnlineNumber();
    
    
};

#endif /* ALNetControl_h */
