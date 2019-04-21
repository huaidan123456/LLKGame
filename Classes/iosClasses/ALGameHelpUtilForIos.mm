//
//  ALGameHelpUtilForIos.cpp
//  LLKGame
//
//  Created by Allen on 2017/12/26.
//
//

#include "ALGameHelpUtilForIos.h"
#include "ALGameHelpSetupUtilForIos.h"
#include "cocos2d.h"

static ALGameHelpUtilForIos* s_instance = nullptr;
ALGameHelpUtilForIos* ALGameHelpUtilForIos::getInstance()
{
    if (!s_instance) {
        s_instance = new (std::nothrow) ALGameHelpUtilForIos();
        CCASSERT(s_instance, "ALGameHelpUtilForAndroid new Failed,not enoug memory");
    }
    return s_instance;
}



/**
 *  游戏主界面内的分享按钮的点击相关操作
 */
void ALGameHelpUtilForIos::shareOfGameCardInfo()
{
    if ([ALGameHelpSetupUtilForIos getInstance].shareOfMenuCallback) {
        [ALGameHelpSetupUtilForIos getInstance].shareOfMenuCallback();
    }
}

/**
 *  游戏中 抽卡分享按钮的点击相关操作
 */
void ALGameHelpUtilForIos::shareOfGameDrawCard(int cardIndex)
{
    if ([ALGameHelpSetupUtilForIos getInstance].shareOfGameCallback) {
        [ALGameHelpSetupUtilForIos getInstance].shareOfGameCallback(cardIndex);
    }
}

/**
 *  退出游戏的相关操作
 */
void ALGameHelpUtilForIos::exitGame()
{
    if ([ALGameHelpSetupUtilForIos getInstance].exitGameCallback) {
        [ALGameHelpSetupUtilForIos getInstance].exitGameCallback();
    }
}


/**
 *  游戏controller是否创建
 */
bool ALGameHelpUtilForIos::isDoneGameController()
{
    return [[ALGameHelpSetupUtilForIos getInstance] isDoneGameController];
}
