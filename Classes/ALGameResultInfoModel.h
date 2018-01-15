//
//  ALGameResultInfoModel.h
//  LLKGame
//
//  Created by Allen on 2017/12/20.
//
//

#ifndef ALGameResultInfoModel_h
#define ALGameResultInfoModel_h

#include "cocos2d.h"

class ALGameResultInfoModel:public cocos2d::Ref
{
public:
    static ALGameResultInfoModel* create();
    ALGameResultInfoModel();
    ~ALGameResultInfoModel();
    
    //** 游戏结果 *//
    bool gameResult;
    //** 增加的经验 *//
    int addExp;
    //** 增加的青豆 *//
    int addQingdao;
    
    
    /**
     *  打印描述
     */
    void printDescription();
    
};

#endif /* ALGameResultInfoModel_h */
