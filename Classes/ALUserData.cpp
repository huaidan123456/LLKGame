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




