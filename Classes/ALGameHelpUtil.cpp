//
//  ALGameHelpUtil.cpp
//  LLKGame
//
//  Created by Allen on 2017/12/26.
//
//

#include "ALGameHelpUtil.h"
#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "ALGameHelpUtilForAndroid.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "ALGameHelpUtilForIos.h"
#endif

static ALGameHelpUtil* s_instance = nullptr;
ALGameHelpUtil* ALGameHelpUtil::getInstance()
{
    if (!s_instance) {
        s_instance = new (std::nothrow) ALGameHelpUtil();
        CCASSERT(s_instance, "ALGameHelpUtil new Failed,not enoug memory");
    }
    return s_instance;
}


/**
 *  游戏主界面内的分享按钮的点击相关操作
 */
void ALGameHelpUtil::shareOfGameCardInfo()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ALGameHelpUtilForAndroid::getInstance()->shareOfGameCardInfo();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ALGameHelpUtilForIos::getInstance()->shareOfGameCardInfo();
#endif
}

/**
 *  游戏中 抽卡分享按钮的点击相关操作
 */
void ALGameHelpUtil::shareOfGameDrawCard(int cardIndex)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ALGameHelpUtilForAndroid::getInstance()->shareOfGameDrawCard(cardIndex);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ALGameHelpUtilForIos::getInstance()->shareOfGameDrawCard(cardIndex);
#endif
}

/**
 *  退出游戏的相关操作
 */
void ALGameHelpUtil::exitGame()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ALGameHelpUtilForAndroid::getInstance()->exitGame();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ALGameHelpUtilForIos::getInstance()->exitGame();
#endif
}
