#include "AppDelegate.h"
#include "StartGameScene.h"
#include "ALMusicUtil.h"
#include "ALNotificationNameDefine.h"
#include "ALPlayerData.h"
#include "ALGameConfig.h"

#include <string.h>

using std::string;

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(720, 1280);
static cocos2d::Size smallResolutionSize = cocos2d::Size(1125, 2436);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(720, 1280);
static cocos2d::Size largeResolutionSize = cocos2d::Size(640, 960);

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("LLKGame", Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("LLKGame");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    // Set the design resolution
//    if (screenBounds.size.height * scale_screen ==2436 && screenBounds.size.width *scale_screen ==1125) {
//        glview->setDesignResolutionSize(750,1624, ResolutionPolicy::SHOW_ALL);
//        std::vector<std::string> searchPath;
//        searchPath.push_back("IphoneX");
//        cocos2d::FileUtils::getInstance()->setSearchPaths(searchPath);
//
//    }else{
//        glview->setDesignResolutionSize(720,1280, ResolutionPolicy::NO_BORDER);
//    }
    Size frameSize = glview->getFrameSize();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (frameSize.width == 1125 && frameSize.height == 2436) {
        glview->setDesignResolutionSize(720,1560, ResolutionPolicy::SHOW_ALL);
        std::vector<std::string> searchPath;
        searchPath.push_back("IphoneX");
        cocos2d::FileUtils::getInstance()->setSearchPaths(searchPath);
        director->setContentScaleFactor(1);
        ALGameConfig::designType = DF_DESIGN_TYPE_h1560;
        
    }else{
        glview->setDesignResolutionSize(720,1280, ResolutionPolicy::NO_BORDER);
        ALGameConfig::designType = DF_DESIGN_TYPE_h1280;
    }
#else
    glview->setDesignResolutionSize(720,1280, ResolutionPolicy::NO_BORDER);
    ALGameConfig::designType = DF_DESIGN_TYPE_h1280;
#endif


    register_all_packages();

    // create a scene. it's an autorelease object
//    auto scene = GameScene::createWithIconCount(10);
    auto scene =  StartGameScene::create();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    // 进入后台
    CCLOG("进入后台");
//    ALMusicUtil::getInstrins()->stopAllMusicAndEffect();
    NotificationCenter::getInstance()->postNotification(NND_AppDidEnterBackground);
    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    
    //恢复前台
    CCLOG("恢复前台");
//    ALMusicUtil::getInstrins()->resumeAllMusicAndEffect();
    NotificationCenter::getInstance()->postNotification(NND_AppWillEnterForeground);


    // android 是否要重构界面
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (ALPlayerData::isNotifyResetScene) {
        CCLOG("android 要要重构界面");
        if (Director::getInstance()->getRunningScene()->getTag() == DF_TAG_Sign_StartGameScene)
        {
            auto scene = (StartGameScene*)Director::getInstance()->getRunningScene();
            scene->resetScene();
        }
        ALPlayerData::isNotifyResetScene = false;
    }
#endif
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
