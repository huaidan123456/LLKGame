//
//  ALGameHelpSetupUtilForIos.m
//  LLKGame
//
//  Created by Allen on 2017/12/26.
//
//

#import "ALGameHelpSetupUtilForIos.h"
#import "ALUserData.h"
#import "ALNetControl.h"
#import "RootViewController.h"
#import "platform/ios/CCEAGLView-ios.h"
#import "cocos2d.h"
#import "CCDirectorCaller-ios.h"
#import "StartGameScene.h"
#import "ALUserInfoModel.h"
#import <AVFoundation/AVFoundation.h>
#import "ALGameConfig.h"
#import "ALGameAppDelegate.h"



@implementation ALGameHelpSetupUtilForIos

+ (instancetype)getInstance
{
    static ALGameHelpSetupUtilForIos *instance;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[ALGameHelpSetupUtilForIos alloc]init];
    });
    return instance;
}



- (void)setLoginUidOfGame:(int)uid
{
    ALUserData::loginUid = uid;
    ALUserData::userInfo->setUid(uid);
}


- (void)gameShareSuccess
{
    ALNetControl::requestGetQingDou(20);
}


static RootViewController* _controller;
- (UIViewController*)getGameController
{
    if (!_controller) {
        [[AVAudioSession sharedInstance] setCategory: AVAudioSessionCategorySoloAmbient error: nil];
        [[AVAudioSession sharedInstance] setActive:true error:nil];
        
        
        _controller = [[RootViewController alloc] initWithNibName:nil bundle:nil];
        _controller.wantsFullScreenLayout = YES;
        cocos2d::GLViewImpl::convertAttrs();
        
        // Init the CCEAGLView
        CCEAGLView *eaglView = [CCEAGLView viewWithFrame: [[UIScreen mainScreen] bounds]
                                             pixelFormat: (NSString*)cocos2d::GLViewImpl::_pixelFormat
                                             depthFormat: cocos2d::GLViewImpl::_depthFormat
                                      preserveBackbuffer: NO
                                              sharegroup: nil
                                           multiSampling: NO
                                         numberOfSamples: 0 ];
        // Enable or disable multiple touches
        [eaglView setMultipleTouchEnabled:NO];
        _controller.view = eaglView;
        
        cocos2d::GLView *glview = cocos2d::GLViewImpl::createWithEAGLView(eaglView);
        cocos2d::Director::getInstance()->setOpenGLView(glview);

        //
//        if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
//
//        }else if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
//
//        }
        cocos2d::Size frameSize = glview->getFrameSize();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        if (frameSize.width == 1125 && frameSize.height == 2436) {
            glview->setDesignResolutionSize(720,1560, ResolutionPolicy::SHOW_ALL);
            std::vector<std::string> searchPath;
            searchPath.push_back("IphoneX");
            cocos2d::FileUtils::getInstance()->setSearchPaths(searchPath);
            cocos2d::Director::getInstance()->setContentScaleFactor(1);
            ALGameConfig::designType = DF_DESIGN_TYPE_h1560;
            
        }else{
            glview->setDesignResolutionSize(720,1280, ResolutionPolicy::SHOW_ALL);
            ALGameConfig::designType = DF_DESIGN_TYPE_h1280;
        }
#else
        glview->setDesignResolutionSize(720,1280, ResolutionPolicy::NO_BORDER);
        ALGameConfig::designType = DF_DESIGN_TYPE_h1280;
#endif

        cocos2d::Director::getInstance()->setContentScaleFactor(1);
        
        
        cocos2d::Director::getInstance()->setAnimationInterval(1.0 / 60);
        
        cocos2d::Application::getInstance()->run();
        auto sence = StartGameScene::create();
        cocos2d::Director::getInstance()->runWithScene(sence);
        
        
//        [[CCDirectorCaller sharedDirectorCaller] startMainLoop];
    }
    return _controller;
}

- (void)pauseGame
{

    cocos2d::Director::getInstance()->pause();
    [[AVAudioSession sharedInstance] setCategory: AVAudioSessionCategoryAmbient error: nil];
    [[AVAudioSession sharedInstance] setActive:false error:nil];
    
    
//    cocos2d::Director::getInstance()->end();
//    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.1 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
//        [_controller release];
//        _controller = nil;
//    });
    
}

/**
 *  运行游戏
 */
- (void)runGame
{
    if (cocos2d::Director::getInstance()->isPaused()) {
        [[AVAudioSession sharedInstance] setCategory: AVAudioSessionCategorySoloAmbient error: nil];
        [[AVAudioSession sharedInstance] setActive:true error:nil];
        cocos2d::Director::getInstance()->resume();
        if (cocos2d::Director::getInstance()->getRunningScene()->getTag() == DF_TAG_Sign_StartGameScene)
        {
            auto scene = (StartGameScene*)cocos2d::Director::getInstance()->getRunningScene();
            scene->resetScene();
        }else{
            cocos2d::Director::getInstance()->replaceScene(StartGameScene::create());
        }
        
    }
    
}

/**
 *  游戏controller是否创建
 */
- (bool)isDoneGameController
{
    return _controller ? true : false;
}

@end
