//
//  ALGameFriendInfoModel.cpp
//  LLKGame-mobile
//
//  Created by Allen on 2018/1/11.
//

#include "ALGameFriendInfoModel.h"

ALGameFriendInfoModel* ALGameFriendInfoModel::create()
{
    ALGameFriendInfoModel* model = new (std::nothrow) ALGameFriendInfoModel();
    if (model) {
        model->autorelease();
    }else{
        CC_SAFE_DELETE(model);
    }
    return model;
}
ALGameFriendInfoModel::ALGameFriendInfoModel()
{
    _onlineState = ONLINE_STATE::OFFLINE;
}
ALGameFriendInfoModel::~ALGameFriendInfoModel()
{
    
}


int ALGameFriendInfoModel::getOnlineState()
{
    return _onlineState;
}


void ALGameFriendInfoModel::setOnlineState(int state)
{
    _onlineState = state;
}
