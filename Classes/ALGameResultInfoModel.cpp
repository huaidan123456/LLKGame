//
//  ALGameResultInfoModel.cpp
//  LLKGame
//
//  Created by Allen on 2017/12/20.
//
//

#include "ALGameResultInfoModel.h"


ALGameResultInfoModel* ALGameResultInfoModel::create()
{
    ALGameResultInfoModel* model = new (std::nothrow) ALGameResultInfoModel();
    if (model) {
        model->autorelease();
    }else{
        CC_SAFE_DELETE(model);
    }
    return model;
}

ALGameResultInfoModel::ALGameResultInfoModel()
{
    
}

ALGameResultInfoModel::~ALGameResultInfoModel()
{
    gameResult = false;
    addExp = 0;
    addQingdao = 0;
}


void ALGameResultInfoModel::printDescription()
{
    CCLOG("游戏结果：%s, 获得的经验：%d, 获得的青豆：%d",(gameResult ? "胜利" : "失败"),addExp,addQingdao);
}
