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
     *  请求好友列表
     */
    static void requestGameFriendList();
    /**
     *  申请约战
     */
    static void requestFightWithFriend(int friendId);
    
    /**
     *  同意约战
     */
    static void sendAgreeFightInvite(int inviterId);
    
    /**
     *  拒绝约战
     */
    static void sendRefuseFightInvite(int inviterId);
    
    /**
     *  进入房间并且准备完毕
     */
    static void readyOfFriendFightGame();
    
    /**
     *  好友对战时候 没消除一个向服务器发送消除
     */
    static void sendCleanIconOfFriendFight();
    
    /**
     *  好友对战的时候  离开本局(中途离开)
     */
    static void sendLeaveRoomOfFriendFight();
    
    /**
     *  好友对战结束后 发送再来一局
     */
    static void sendGameAgainOfFriendFight(int friendUid);
    
    /**
     *  发送同意对方的再来一局
     */
    static void sendAgreeGameAgainOfFriendFight(int inviterId);
    
    /**
     *  发送 对战结束  离开游戏
     */
    static void sendLeaveFriendFightGameOfGameOver(int friendUid);
    
    /**
     *  好友对战中 断线重连机制
     */
    static void reconnectOfFriendFight(int uid,int roomId);
    
    /**
     *  好友对战的游戏中  从后台回到前台给服务器发送到字段
     */
    static void backGameAfterEnterforeground(int roomId);

    
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
