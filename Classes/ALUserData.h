//
//  ALUserData.h
//  LLKGame
//
//  Created by Allen on 2017/12/18.
//
//

#ifndef ALUserData_h
#define ALUserData_h

#include <string>
#include <map>
#include "cocos2d.h"


class ALUserInfoModel;
class ALGameFriendInfoModel;
class ALUserData
{
public:
    //** 用户登录uid *//
    static int loginUid;
    
    //** 用户基本信息 *//
    static ALUserInfoModel* userInfo;
    
    //** 轻豆的数量 *//
    static int qingDouCount;
    
    //** 用户娘卡 *//
    static bool niangCard[10];
    
    //** 好友列表 *//
    static cocos2d::Map<int,ALGameFriendInfoModel*> gameFriends;
    
    
};

#endif /* ALUserData_h */
