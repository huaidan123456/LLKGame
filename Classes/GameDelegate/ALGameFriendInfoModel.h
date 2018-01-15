//
//  ALGameFriendInfoModel.h
//  LLKGame-mobile
//
//  Created by Allen on 2018/1/11.
//

#ifndef ALGameFriendInfoModel_h
#define ALGameFriendInfoModel_h

#include "ALUserInfoModel.h"

class ALGameFriendInfoModel:public ALUserInfoModel
{
public:
    enum ONLINE_STATE
    {
        OFFLINE = 0,
        ONLINE,
        ONGAME
    };
    
    
    static ALGameFriendInfoModel* create();
    ALGameFriendInfoModel();
    ~ALGameFriendInfoModel();
    
    int getOnlineState();
    void setOnlineState(int state);
    
public:
    int _onlineState;
};

#endif /* ALGameFriendInfoModel_h */
