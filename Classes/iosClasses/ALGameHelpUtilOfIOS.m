//
//  ALGameHelpUtilOfIOS.m
//  LLKGame
//
//  Created by Allen on 2017/12/26.
//
//

#import "ALGameHelpUtilOfIOS.h"
#import "ALGameHelpSetupUtilForIos.h"


@implementation ALGameHelpUtilOfIOS


+ (instancetype)getInstance
{
    static ALGameHelpUtilOfIOS *instance;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[ALGameHelpUtilOfIOS alloc]init];
    });
    return instance;
}

/**
 *  设置进入游戏的登录Uid
 *  在进入游戏请需要设置
 */
- (void)setupLoginUidOfGame:(int)uid
{
    [[ALGameHelpSetupUtilForIos getInstance] setLoginUidOfGame:uid];
}

/**
 *  设置分享成功后需要掉用的方法
 */
- (void)shareSuccessToGiveReward
{
    [[ALGameHelpSetupUtilForIos getInstance] gameShareSuccess];
}






/**
 *  游戏主界面内的分享按钮的点击相关操作
 */
- (void)setupShareOfGameCardInfoCallback:(void (^)())block
{
    [[ALGameHelpSetupUtilForIos getInstance] setShareOfMenuCallback:block];
}

/**
 *  游戏中 抽卡分享按钮的点击相关操作
 */
- (void)setupShareOfGameDrawCardCallback:(void (^)(int))block
{
    [[ALGameHelpSetupUtilForIos getInstance] setShareOfGameCallback:block];
}

/**
 *  退出游戏的相关操作
 */
- (void)setupExitGameCallback:(void (^)())block
{
    [[ALGameHelpSetupUtilForIos getInstance] setExitGameCallback:block];
}

/**
 *  获取游戏Controller
 *  在获取到游戏Controller，获取到controller的同时，游戏已经在运行
 */
- (UIViewController*)getGameController
{
    return [[ALGameHelpSetupUtilForIos getInstance] getGameController];
}

- (void)pauseGame
{
    [[ALGameHelpSetupUtilForIos getInstance] pauseGame];
}

- (void)runGame
{
    [[ALGameHelpSetupUtilForIos getInstance] runGame];
}

/**
 *  游戏controller是否创建
 */
- (bool)isDoneGameController
{
    return [[ALGameHelpSetupUtilForIos getInstance] isDoneGameController];
}



@end

