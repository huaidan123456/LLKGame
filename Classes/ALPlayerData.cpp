//
//  ALPlayerData.cpp
//  LLKGame
//
//  Created by Allen on 2017/12/18.
//
//

#include "ALPlayerData.h"
#include "ALUserInfoModel.h"


//** 当前在线人数 *//
int ALPlayerData::onLienNumber = 1;
//** 是否能够接受到约战邀请通知 *//
bool ALPlayerData::isResponseInviterNotification = true;
//** 对手的用户信息 *//
ALUserInfoModel* ALPlayerData::opponentInfo = new ALUserInfoModel();


//** 游戏难度 *//
int ALPlayerData::difficultyLevel = 6;
//** 当前对战游戏的房间Id *//
int ALPlayerData::roomIdOfPK = 0;
//** 电脑ai难度数组 *//
int ALPlayerData::systemAIDifficulty[7] = {85,65,55,40,60,75,85};


//** 好友约战中  左侧消除数量 右侧消除数量*//
int ALPlayerData::leftCleanCountOfPKGame = 0;
int ALPlayerData::rightCleanCountOfPKGame = 0;

//** 好友对战游戏中 是否可以开始比赛 *//
bool ALPlayerData::isReadyOfFriendFightGame = false;




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
    ALPlayerData::roomIdOfPK = 0;
    ALPlayerData::isResponseInviterNotification = true;
    ALPlayerData::isReadyOfFriendFightGame = false;
    
    // 重置敌人的数据
    ALPlayerData::opponentInfo->resetModel();
}

