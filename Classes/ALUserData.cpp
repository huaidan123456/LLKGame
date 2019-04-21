//
//  ALUserData.cpp
//  LLKGame
//
//  Created by Allen on 2017/12/18.
//
//

#include "ALUserData.h"
#include "ALUserInfoModel.h"
#include "ALGameFriendInfoModel.h"


int ALUserData::loginUid = 1;

ALUserInfoModel* ALUserData::userInfo = new ALUserInfoModel();

int ALUserData::qingDouCount = 0;

bool ALUserData::niangCard[10] = {false,false,false,false,false,false,false,false,false,false};

cocos2d::Map<int,ALGameFriendInfoModel*> ALUserData::gameFriends; // 好友列表
//** 是否刷新好友列表 *//
bool ALUserData::isRefreshFriendList = false;







/**
 *  重置数据
 */
void ALUserData::resetUserData()
{
    ALUserData::loginUid = 1;
    ALUserData::qingDouCount = 0;
    // 娘卡
    for (int i = 0; i < 10 ; ++i) {
        ALUserData::niangCard[i] = false;
    }
    // 好友列表
    ALUserData::gameFriends.clear();
    ALUserData::isRefreshFriendList = false;

}




