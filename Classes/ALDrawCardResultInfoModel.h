//
//  ALDrawCardResultInfoModel.h
//  LLKGame
//
//  Created by Allen on 2017/12/22.
//
//

#ifndef ALDrawCardResultInfoModel_h
#define ALDrawCardResultInfoModel_h

#include "cocos2d.h"

class ALDrawCardResultInfoModel:public cocos2d::Ref
{
public:
    static ALDrawCardResultInfoModel* create();
    ALDrawCardResultInfoModel();
    ~ALDrawCardResultInfoModel();
    
    //** 是否抽中卡片 *//
    bool isGain;
    //** 抽中的卡片号 *//
    int cardIndex;
    //** 获得奖励的代号 （0 表示未获得） *//
    int awardIndex;
};

#endif /* ALDrawCardResultInfoModel_h */
