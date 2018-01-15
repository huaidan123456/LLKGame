//
//  ALGameAppDelegate.cpp
//  LLKGame
//
//  Created by Allen on 2017/12/26.
//
//

#include "ALGameAppDelegate.h"
#include "ALNotificationNameDefine.h"
#include "ALMusicUtil.h"
#include "ALGameHelpUtilForIos.h"


USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(720,1280);
static cocos2d::Size smallResolutionSize = cocos2d::Size(720,1280);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(720,1280);
static cocos2d::Size largeResolutionSize = cocos2d::Size(720,1280);


ALGameAppDelegate::ALGameAppDelegate()
{
    
}


ALGameAppDelegate::~ALGameAppDelegate()
{
    
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void ALGameAppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    
    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages,
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool ALGameAppDelegate::applicationDidFinishLaunching() {
    CCLOG("ASDFASDFASDFASDFASDF");

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void ALGameAppDelegate::applicationDidEnterBackground() {
    
    if (ALGameHelpUtilForIos::getInstance()->isDoneGameController()) {
        CCLOG("applicationDidEnterBackground");
        Director::getInstance()->stopAnimation();
        //    ALMusicUtil::getInstrins()->stopAllMusicAndEffect();
        NotificationCenter::getInstance()->postNotification(NND_AppDidEnterBackground);
    }
    
}

// this function will be called when the app is active again
void ALGameAppDelegate::applicationWillEnterForeground() {
    if (ALGameHelpUtilForIos::getInstance()->isDoneGameController()){
        CCLOG("applicationWillEnterForeground");
        Director::getInstance()->startAnimation();
        //    ALMusicUtil::getInstrins()->resumeAllMusicAndEffect();
        NotificationCenter::getInstance()->postNotification(NND_AppWillEnterForeground);
    }
}



