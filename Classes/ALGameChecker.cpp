//
//  ALGameChecker.cpp
//  LLKGame-mobile
//
//  Created by Allen on 2018/1/10.
//

#include "ALGameChecker.h"
#include "ALNetworkHelper.h"
#include "ALBNSocketUitl.h"


#define DF_ALGameChecker_SCHEDULE_Check_network "DF_ALGameChecker_SCHEDULE_Check_network"


ALGameChecker* checker = nullptr;

ALGameChecker* ALGameChecker::getInstrins()
{
    if(!checker)
    {
        checker = new ALGameChecker();
    }
    
    return checker;
    
}


void ALGameChecker::startNetworkChecker()
{
    schedule([=](float ft){
        if (!ALNetworkHelper::checkCurrentNetworkState()) {
            CCLOG("检查到 无网络状态");
            ALBNSocketUitl::closeConnect();
        }else{
            CCLOG("检查到 有网络状态");
        }
        
    }, 1, DF_ALGameChecker_SCHEDULE_Check_network);
}


void ALGameChecker::stopNetworkChecker()
{
    unschedule(DF_ALGameChecker_SCHEDULE_Check_network);
}
