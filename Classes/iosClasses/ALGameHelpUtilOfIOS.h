//
//  ALGameHelpUtilOfIOS.h
//  LLKGame
//
//  Created by Allen on 2017/12/26.
//
//

#import <Foundation/Foundation.h>



@interface ALGameHelpUtilOfIOS : NSObject


+ (instancetype)getInstance;

/**
 *  设置进入游戏的登录Uid
 *  在进入游戏请需要设置
 */
- (void)setupLoginUidOfGame:(int)uid;

/**
 *  设置分享成功后需要掉用的方法
 */
- (void)shareSuccessToGiveReward;

/**
 *  游戏主界面内的分享按钮的点击相关操作
 */
- (void)setupShareOfGameCardInfoCallback:(void(^)())block;

/**
 *  游戏中 抽卡分享按钮的点击相关操作
 */
- (void)setupShareOfGameDrawCardCallback:(void(^)(int cardIndex))block;

/**
 *  退出游戏按钮的相关操作
 */
- (void)setupExitGameCallback:(void(^)())block;

/**
 *  获取游戏Controller
 *  在获取到游戏Controller，获取到controller的同时，游戏已经在运行
 */
- (UIViewController*)getGameController;

/**
 *  暂停游戏
 */
- (void)pauseGame;


/**
 *  运行游戏
 */
- (void)runGame;

/**
 *  游戏controller是否创建
 */
- (bool)isDoneGameController;



@end
