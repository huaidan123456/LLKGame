//
//  ALUserInfoModel.cpp
//  LLKGame
//
//  Created by Allen on 2017/12/15.
//
//

#include "ALUserInfoModel.h"


ALUserInfoModel* ALUserInfoModel::create()
{
    ALUserInfoModel* model = new (std::nothrow) ALUserInfoModel();
    if (model) {
        model->autorelease();
    }else{
        CC_SAFE_DELETE(model);
    }
    return model;
}


ALUserInfoModel::ALUserInfoModel()
{
    _uid = 0;
    _uname = "";
    _headUrl = "";
    _level = 1;
    _currentExp = 0;
    _sumExp = 0;
    _qingDouCount = 0;
    _isConcern = false;
}

ALUserInfoModel::~ALUserInfoModel()
{
    CCLOG("~ALUserInfoModel()");
}


void ALUserInfoModel::setupInfoWithModel(ALUserInfoModel *model)
{
    _uid = model->getUid();
    _uname = model->getName().c_str();
    _headUrl = model->getHeadUrl().c_str();
    _level = model->getCurrentExp();
    _currentExp = model->getCurrentExp();
    _sumExp = model->getSumExp();
    _qingDouCount = model->getQingDouCount();
}


int ALUserInfoModel::getUid()
{
    return _uid;
}

void ALUserInfoModel::setUid(int uid)
{
    _uid = uid;
}


std::string ALUserInfoModel::getName()
{
    return _uname;
}

void ALUserInfoModel::setName(const char* name)
{
    _uname = name;
}


std::string ALUserInfoModel::getHeadUrl()
{
    return _headUrl;
}

void ALUserInfoModel::setHeadUrl(const char* headUrl)
{
    _headUrl = headUrl;
}


int ALUserInfoModel::getLevel()
{
    return _level;
}

void ALUserInfoModel::setLevel(int level)
{
    _level = level;
}


int ALUserInfoModel::getCurrentExp()
{
    return _currentExp;
}

void ALUserInfoModel::setCurrentEXP(int exp)
{
    _currentExp = exp;
}

int ALUserInfoModel::getSumExp()
{
    return _sumExp;
}
void ALUserInfoModel::setSumExp(int exp)
{
    _sumExp = exp;
}


int ALUserInfoModel::getQingDouCount()
{
    return _qingDouCount;
}
void ALUserInfoModel::setQingDou(int count)
{
    _qingDouCount = count;
}


bool ALUserInfoModel::isConcern()
{
    return _isConcern;
}
void ALUserInfoModel::setConcern(bool isConcern)
{
    _isConcern = isConcern;
}


/**
 *  重置
 */
void ALUserInfoModel::resetModel()
{
    _uid = 0;
    _uname = "";
    _headUrl = "";
    _level = 1;
    _currentExp = 0;
    _sumExp = 0;
    _qingDouCount = 0;
    _isConcern = false;
}
