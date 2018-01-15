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
     *  制作并且弹出离开游戏的弹框
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
     *  弹出轻豆不足的提示框  
     */
    static void makeAlertOfDontHaveEnoughQingDou(cocos2d::Node* parentNode);
    
    
    

   
};

#endif /* ALAlertUtil_h */
