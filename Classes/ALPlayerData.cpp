//
//  ALPlayerData.cpp
//  LLKGame
//
//  Created by Allen on 2017/12/18.
//
//

#include "ALPlayerData.h"
#include "ALUserInfoModel.h"



int ALPlayerData::onLienNumber = 1;

ALUserInfoModel* ALPlayerData::opponentInfo = new ALUserInfoModel();

int ALPlayerData::difficultyLevel = 6;

int ALPlayerData::systemAIDifficulty[7] = {85,65,55,40,60,75,85};



int ALPlayerData::leftCleanCountOfPKGame = 0;
int ALPlayerData::rightCleanCountOfPKGame = 0;




#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
// andorid第一次启动游戏Activity
bool ALPlayerData::isFirstStartGameActivity = true;

// android 是否要通知重新设置界面
bool ALPlayerData::isNotifyResetScene = false;
#endif






void ALPlayerData::resetPlayerData()
{
    ALPlayerData::difficultyLevel = 6;
    ALPlayerData::leftCleanCountOfPKGame = 0;
    ALPlayerData::rightCleanCountOfPKGame = 0;
}

