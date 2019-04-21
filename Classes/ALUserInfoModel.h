//
//  ALUserInfoModel.h
//  LLKGame
//
//  Created by Allen on 2017/12/15.
//
//

#ifndef ALUserInfoModel_h
#define ALUserInfoModel_h

#include "cocos2d.h"

class ALUserInfoModel:public cocos2d::Ref
{
public:
    static ALUserInfoModel* create();
    
    ALUserInfoModel();
    ~ALUserInfoModel();
    
    void setupInfoWithModel(ALUserInfoModel* model);
    
    int getUid();
    void setUid(int uid);
    
    std::string getName();
    void setName(const char* name);
    
    std::string getHeadUrl();
    void setHeadUrl(const char* headUrl);
    
    int getLevel();
    void setLevel(int level);
    
    int getCurrentExp();
    void setCurrentEXP(int exp);
    
    int getSumExp();
    void setSumExp(int exp);
    
    int getQingDouCount();
    void setQingDou(int count);
    
    bool isConcern();
    void setConcern(bool isConcern);
    
    
    /**
     *  重置
     */
    void resetModel();
    
    
private:
    int _uid;
    //** 名称 *//
    std::string _uname;
    //** 头像地址 *//
    std::string _headUrl;
    //** 等级 *//
    int _level;
    //** 当前等级的经验 *//
    int _currentExp;
    //** 当前等级的总经验 *//
    int _sumExp;
    //** 当前用户的轻豆数量 *//
    int _qingDouCount;
    //** 关注状态 （此属性用于账户玩家是否关注该用户） *//
    bool _isConcern;
    
    
    
};

#endif /* ALUserInfoModel_h */
