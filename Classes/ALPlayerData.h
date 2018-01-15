//
//  ALPlayerData.h
//  LLKGame
//
//  Created by Allen on 2017/12/18.
//
//

#ifndef ALPlayerData_h
#define ALPlayerData_h

class ALUserInfoModel;
class ALPlayerData
{
public:
    //** 当前在线人数 *//
    static int onLienNumber;
    //** 游戏难度 *//
    static int difficultyLevel;
    
    //** 对手的用户信息 *//
    static ALUserInfoModel* opponentInfo;
    
    //** 电脑ai难度数组 *//
    static int systemAIDifficulty[7];
    
    //** 好友约战中  左侧消除数量 *//
    static int leftCleanCountOfPKGame;
    
    //** 好友约战中  右侧消除数量 *//
    static int rightCleanCountOfPKGame;
    
    
    
    
    
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // andorid第一次启动游戏Activity
    static bool isFirstStartGameActivity;
    
    // android 是否要通知重新设置界面
    static bool isNotifyResetScene;
#endif
    
    
    
    /**
     *  重置数据
     */
    static void resetPlayerData();
    
};

#endif /* ALPlayerData_h */
