//
//  ALDrawCardResultInfoModel.cpp
//  LLKGame
//
//  Created by Allen on 2017/12/22.
//
//

#include "ALDrawCardResultInfoModel.h"


ALDrawCardResultInfoModel* ALDrawCardResultInfoModel::create()
{
    ALDrawCardResultInfoModel* model = new (std::nothrow) ALDrawCardResultInfoModel();
    if (model) {
        model->autorelease();
    }else{
        CC_SAFE_DELETE(model);
    }
    return model;
}

ALDrawCardResultInfoModel::ALDrawCardResultInfoModel()
{
    
}

ALDrawCardResultInfoModel::~ALDrawCardResultInfoModel()
{
    isGain = false;
    cardIndex = 1;
    awardIndex = 0;
}
