//
//  ALAlertUtil.h
//  LLKGame
//
//  Created by Allen on 2017/12/22.
//
//

#ifndef ALAlertUtil_h
#define ALAlertUtil_h

#include <stdio.h>
#include "cocos2d.h"

class ALAlertUtil
{
public:
    typedef std::function<void()> alAlertOkCallback;
    typedef std::function<void(bool isOk)> alAlertOkCallback2;
    /**
     *  弹出并且弹出离开游戏的弹框
     */
    static void makeAlertOfExitGame(cocos2d::Node* parentNode,const alAlertOkCallback& callback);
    
 
    /**
     *  弹出断开连接的提示框
     */
    static void makeAlertOfDisconnect(cocos2d::Node* parentNode,const alAlertOkCallback& callback);
    
    
    /**
     *  弹出游戏内连接超时提示框
     */
    static void makeAlertOfConnectTimeOut(cocos2d::Node* parentNode,const alAlertOkCallback& callback);
    
    /**
     *  弹出匹配超时提示框
     */
    static void makeAlertOfMatchFail(cocos2d::Node* parentNode,const alAlertOkCallback& callback);
    
    
    /**
     *  游戏中断的提示框
     */
    static void makeAlertOfGameInterruption(cocos2d::Node* parentNode,const alAlertOkCallback& callback);
    
    /**
     *  游戏已经结束的提示框  alalert_box_gameFightOverAndLeavel.png
     */
    static void makeAlertOfFriendFightGameOver(cocos2d::Node* parentNode,const alAlertOkCallback& callback);
    
    /**
     *  好友PK 游戏超时  
     */
    static void makeAlertOfFriendFightRoomTimeOut(cocos2d::Node* parentNode,const alAlertOkCallback& callback);
    
    /**
     *  弹出轻豆不足的提示框  
     */
    static void makeAlertOfDontHaveEnoughQingDou(cocos2d::Node* parentNode);
    
    
    

    
    
    
    
    
    
    
    
    
    
private:
    static void makeAlertWithTitleName(const char* pathName,cocos2d::Node* parentNode,const alAlertOkCallback& callback);
   
};

#endif /* ALAlertUtil_h */
