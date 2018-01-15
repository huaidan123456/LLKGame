//
//  ALGameHelpSetupUtilForIos.h
//  LLKGame
//
//  Created by Allen on 2017/12/26.
//
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

typedef void(^ALShareCallback)();
typedef void(^ALCardShareback)(int cardIndex);
typedef void(^ALExitCallback)();
@interface ALGameHelpSetupUtilForIos : NSObject

@property(nonatomic,copy) ALShareCallback shareOfMenuCallback;
@property(nonatomic,copy) ALCardShareback shareOfGameCallback;
@property(nonatomic,copy) ALExitCallback exitGameCallback;


+ (instancetype)getInstance;
/**
 *  设置
 */
- (void)setLoginUidOfGame:(int)uid;

/**
 *  分享成功后需要调用的方法
 */
- (void)gameShareSuccess;


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
