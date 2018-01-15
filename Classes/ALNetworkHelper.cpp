//
//  ALNetworkHelper.cpp
//  LLKGame-mobile
//
//  Created by Allen on 2018/1/10.
//

#include "ALNetworkHelper.h"
#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "ALNetworkHelperForIos.h"
#endif


bool ALNetworkHelper::checkCurrentNetworkState()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return ALNetworkHelperForIos::checkCurrentNetworkState();
#endif
    
    return false;
}
