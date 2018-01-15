//
//  StartGameScene.cpp
//  LLKGame
//
//  Created by Allen on 2017/12/15.
//
//

#include "StartGameScene.h"
#include "MatchSuccessLayer.h"
#include "cocostudio/CocoStudio.h"
#include "GameScene.h"
#include "GamePKScene.h"
#include "ALNetControl.h"
#include "ALUserInfoModel.h"
#include "ALUserData.h"
#include "ALPlayerData.h"
#include "ALNiangCardShowLayer.h"
#include "ALNotificationNameDefine.h"
#include "ALAlertUtil.h"
#include "ALNiangCardInfoLayer.h"
#include "ALGameRulesLayer.h"
#include "ALGameFriendsListLayer.h"
#include "ALMusicUtil.h"
#include "ALGameHelpUtil.h"
#include "ALPlayerData.h"
#include "network/HttpClient.h"
#include "ALNetworkHelper.h"
#include "ALHelpTools.h"





USING_NS_CC;
using namespace cocos2d::ui;

#define DF_SCHEDULE_NAME_SendMatch "DF_SCHEDULE_NAME_SendMatch"
#define DF_SCHEDULE_NAME_DisableCancelMatch "DF_SCHEDULE_NAME_DisableCancelMatch"
#define DF_SCHEDULE_NAME_GotoGame "DF_SCHEDULE_NAME_GotoGame"

#define DF_SCHEDULE_NAME_SGS_hint_failMatch "DF_SCHEDULE_NAME_SGS_hint_failMatch"



StartGameScene* StartGameScene::create()
{
    StartGameScene *scene = new (std::nothrow) StartGameScene();
    if (scene && scene->init())
    {
        scene->baseInit();
        scene->autorelease();
        return scene;
    }
    else
    {
        CC_SAFE_DELETE(scene);
        return nullptr;
    }
}


StartGameScene* StartGameScene::createWithShowCardInfo(int cardIndex)
{
    StartGameScene *scene = new (std::nothrow) StartGameScene();
    if (scene && scene->init())
    {
        scene->baseInit();
        scene->setupCardInfoLayerVisible(true,cardIndex);
        
        scene->autorelease();
        return scene;
    }
    else
    {
        CC_SAFE_DELETE(scene);
        return nullptr;
    }
}





StartGameScene::StartGameScene()
{

    this->setTag(DF_TAG_Sign_StartGameScene);
    _isMatching = false;
    _isDisconnectNotify = true;
    //** 进入后台的时间戳 *//
    _appEnterBackgroundTime = 0;
    //** 匹配成功超时 *//
    _isTimeOutOfMatchSuccess = false;
    
}


StartGameScene::~StartGameScene()
{
    log("~StartGameScene 析构");
    unscheduleAllCallbacks();
    NotificationCenter::getInstance()->removeAllObservers(this);
    ALMusicUtil::getInstrins()->stopAllMusicAndEffect();
}


void StartGameScene::baseInit()
{
    initUI();
    
    registerNotification();

    
    log("开始连接了");
    bool isConnect = ALNetControl::connectServerWithIp(ALND_NF_IP, ALND_NF_PORT, ALUserData::loginUid);
    // 请求用户信息
    ALNetControl::requestUserInfo();
    ALNetControl::requestOnlineNumber();
    log("是否连接成功 %d",isConnect);
    
    // 播放音乐
    ALMusicUtil::getInstrins()->playBgSound(ALMusicUtil::GameBgMusic::MainMusic);
    
    initScheduler();
    
}

void StartGameScene::initScheduler()
{
    unschedule(DF_ALNetworkHelper_SCHEDULE_Check_network);
    /**
     *  实时监测网路状况
     */
    schedule([=](float ft){
        if (!ALNetworkHelper::checkCurrentNetworkState()) {
            if (ALNetControl::isConnect()) {
                ALNetControl::closeConnect();
            }
        }
    }, 1, DF_ALNetworkHelper_SCHEDULE_Check_network);
}


void StartGameScene::registerNotification()
{
    // 断开连接的操作
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(StartGameScene::disConnectObserverFunc), NND_Disconnect, NULL);
    
    //监听用户信息
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(StartGameScene::refreshUserInfoObserverFunc), NND_RefreshUserInfo, NULL);
    
    //监听在线人数
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(StartGameScene::refreshOnlineNumberObserverFunc), NND_RefreshOnlineNumber, NULL);
    
    //监听自动匹配的信息
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(StartGameScene::getPlayerInfoWithMatchObserverFunc), NND_GetRandomMatchUserInfo, NULL);
    
    //监听恢复到前台的操作
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(StartGameScene::appDidEnterBackgroundObserverFunc), NND_AppDidEnterBackground, NULL);
    
    //监听恢复到前台的操作
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(StartGameScene::appWillEnterForegroundObserverFunc), NND_AppWillEnterForeground, NULL);

    
}


void StartGameScene::initUI()
{
    startGameLayer = CSLoader::createNode("csbFile/StartGameLayer.csb");
    
    backBtn = (Button*)startGameLayer->getChildByName("btn_back");
    backBtn->addClickEventListener([&,this](Ref* btn){
        ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::BtnClickEffic);
        
        ALMusicUtil::getInstrins()->endMusicUtil();
        unscheduleAllCallbacks();
        NotificationCenter::getInstance()->removeAllObservers(this);
        ALNetControl::closeConnect();
        ALGameHelpUtil::getInstance()->exitGame();
    });
    
    rulesBtn = (Button*)startGameLayer->getChildByName("btn_rules");
    rulesBtn->addClickEventListener([&,this](Ref* btn){
        ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::BtnClickEffic);
        gameRulesLayer->setVisible(true);
    });
    
    friendsBtn = (Button*)startGameLayer->getChildByName("btn_friends");
    friendsBtn->addClickEventListener([&,this](Ref* btn){
        ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::BtnClickEffic);
        friendsListLayer->setVisible(true);
        
    });
    
    startMatchBtn = (Button*)startGameLayer->getChildByName("btn_match");
    startMatchBtn->addClickEventListener([&,this](Ref* btn){
        ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::BtnClickEffic);
        if (!ALNetControl::isConnect()) {
            ALAlertUtil::makeAlertOfDisconnect(this, [&,this]{
                bool isConnect = ALNetControl::connectServerWithIp(ALND_NF_IP, ALND_NF_PORT, ALUserData::loginUid);
                if (isConnect) {
                    _isDisconnectNotify = true;
                }
                // 请求用户信息
                ALNetControl::requestUserInfo();
                this->unschedule(DF_SCHEDULE_NAME_SendMatch);
                this->unschedule(DF_SCHEDULE_NAME_DisableCancelMatch);
                this->unschedule(DF_SCHEDULE_NAME_GotoGame);
                this->gameMatch(false);
            });
        }else{
            this->gameMatch(true);
        }
        
    });
    
    endMatchBtn = (Button*)startGameLayer->getChildByName("btn_nomatch");
    endMatchBtn->addClickEventListener([&,this](Ref* btn){
        ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::BtnClickEffic);
        this->gameMatch(false);
    });
    endMatchBtn->setVisible(false);
    
    onlineNumberText = (Text*)startGameLayer->getChildByName("txt_onlineNumber");
    
    
    this->addChild(startGameLayer);
    
    
    /**
     *  娘卡界面
     */
    cardShowLayer = ALNiangCardShowLayer::create();
    cardShowLayer->setupClickedCallback([&,this](int index){
        CCLOG("显示娘卡详情页");
        cardInfoLayer->setupCardSelectedIndex(index,true);
        cardInfoLayer->setVisible(true);
    });
    this->addChild(cardShowLayer);
    
    
    /**
     *  匹配成功界面
     */
    matchSuccessLayer = MatchSuccessLayer::create();
    matchSuccessLayer->setVisible(false);
    this->addChild(matchSuccessLayer,10);
    
    /**
     *  娘卡详情页
     */
    cardInfoLayer = ALNiangCardInfoLayer::create();
    cardInfoLayer->setVisible(false);
    this->addChild(cardInfoLayer,9);
    
    /**
     *  规则
     */
    gameRulesLayer = ALGameRulesLayer::create();
    gameRulesLayer->setVisible(false);
    this->addChild(gameRulesLayer,8);
    
    /**
     *  好友列表页面
     */
    friendsListLayer = ALGameFriendsListLayer::create();
    friendsListLayer->setVisible(false);
    this->addChild(friendsListLayer,8);
    
}


void StartGameScene::resetScene()
{
    CCLOG("resetScene");
    unscheduleAllCallbacks();
    NotificationCenter::getInstance()->removeAllObservers(this);
    
    _isDisconnectNotify = true;
    startGameLayer->setVisible(true);
    backBtn->setVisible(true);
    rulesBtn->setVisible(true);
    friendsBtn->setVisible(false);
    onlineNumberText->setVisible(true);
    cardShowLayer->setVisible(true);
    matchSuccessLayer->setVisible(false);
    cardInfoLayer->setVisible(false);
    gameRulesLayer->setVisible(false);
    gameMatch(false);
    registerNotification();
    initScheduler();
    
    /**
     *  重新连接网络
     */
    log("开始连接了");
    bool isConnect = ALNetControl::connectServerWithIp(ALND_NF_IP, ALND_NF_PORT, ALUserData::loginUid);
    // 请求用户信息
    ALNetControl::requestUserInfo();
    ALNetControl::requestOnlineNumber();
    log("是否连接成功 %d",isConnect);
    
    // 播放音乐
    ALMusicUtil::getInstrins()->playBgSound(ALMusicUtil::GameBgMusic::MainMusic);

}


void StartGameScene::setupCardInfoLayerVisible(bool isvisible,int index)
{
    if (isvisible) {
        cardInfoLayer->setupCardSelectedIndex(index,true);
    }
    cardInfoLayer->setVisible(isvisible);
}



void StartGameScene::gameMatch(bool isMatch)
{
    if (_isMatching != isMatch) {
        _isMatching = isMatch;
        
        if (_isMatching) {
//            startMatchBtn->loadTextureNormal("images/sgl_btn_matchingBtn.png");
            startMatchBtn->setVisible(false);
            endMatchBtn->setVisible(true);
            endMatchBtn->setEnabled(true);
            
            // 随机种子
            srand((unsigned)time(NULL));
            CCRANDOM_0_1(); // 避免重复
            float delay = (int)(CCRANDOM_0_1()* 10) % 6 + 1; // 1 ~ 6 秒
            log("随机的时间是 %f",delay);
            
            // 禁止取消
            scheduleOnce([&,this](float ft){
                endMatchBtn->setEnabled(false);
            }, delay-0.5, DF_SCHEDULE_NAME_DisableCancelMatch);
            
            // 发送匹配
            scheduleOnce([&,this](float ft){
                ALNetControl::sendRandomMatch();
            }, delay, DF_SCHEDULE_NAME_SendMatch);
        }else{
            log("取消 匹配");
//            startMatchBtn->loadTextureNormal("images/sgl_btn_startMatchBtn.png");
            startMatchBtn->setVisible(true);
            endMatchBtn->setVisible(false);
            endMatchBtn->setEnabled(true);
            unschedule(DF_SCHEDULE_NAME_SendMatch);
            unschedule(DF_SCHEDULE_NAME_DisableCancelMatch);
            endMatchBtn->setEnabled(true);
            
        }
        
    }
}

void StartGameScene::refreshUserInfo()
{
    
}


void StartGameScene::matchSuccessInfo(ALUserInfoModel *model)
{
    matchSuccessLayer->setupMatchUserInfos(ALUserData::userInfo, model);
    if (_isMatching) {
        matchSuccessLayer->showLayerWithMatchSuccess([&,this]{
            _isTimeOutOfMatchSuccess = false;
            this->gotoRandomMatchGame(5);
            endMatchBtn->setEnabled(true);
        });
    }else{
        endMatchBtn->setEnabled(true);
    }
    
}

/**
 *  进入自动匹配的游戏界面
 */
void StartGameScene::gotoRandomMatchGame(float delay)
{
    if (delay <= 0) {
        auto gameS = GameScene::createWithIconCount(ALPlayerData::difficultyLevel);
        Director::getInstance()->replaceScene(gameS);
    }else{
        this->scheduleOnce([&,this](float ft){
            if (ALNetControl::isConnect() && !_isTimeOutOfMatchSuccess) {
                CCLOG("进入游戏");
                auto gameS = GameScene::createWithIconCount(ALPlayerData::difficultyLevel);
//                auto gameS = GamePKScene::createWithIconCount(ALPlayerData::difficultyLevel);
                Director::getInstance()->replaceScene(gameS);
            }
        }, delay, DF_SCHEDULE_NAME_GotoGame);
    }
}




/**
 *  更新用户信息
 */
void StartGameScene::refreshUserInfoObserverFunc(Ref* ref)
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([&,this]()->void{
        this->refreshUserInfo();
    });
}

/**
 *  获取匹配到的用户信息
 */
void StartGameScene::getPlayerInfoWithMatchObserverFunc(Ref* ref)
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([&,this]()->void{
        ALMusicUtil::getInstrins()->stopBgSound();
        this->matchSuccessInfo(ALPlayerData::opponentInfo);
    });
}

/**
 *  刷新在线人数
 */
void StartGameScene::refreshOnlineNumberObserverFunc(Ref* ref)
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([&,this]()->void{
        onlineNumberText->setString(StringUtils::format("%d 人正在玩",ALPlayerData::onLienNumber));
    });
}

/**
 *  断开连接的操作
 */
void StartGameScene::disConnectObserverFunc(Ref* ref)
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([&,this]()->void{
        CCLOG("断开连接");
        if (_isDisconnectNotify) {
            _isDisconnectNotify = false;
            ALAlertUtil::makeAlertOfDisconnect(this, [&,this]{
                bool isConnect = ALNetControl::connectServerWithIp(ALND_NF_IP, ALND_NF_PORT, ALUserData::loginUid);
                // 请求用户信息
                ALNetControl::requestUserInfo();
                this->unschedule(DF_SCHEDULE_NAME_SendMatch);
                this->unschedule(DF_SCHEDULE_NAME_DisableCancelMatch);
                this->unschedule(DF_SCHEDULE_NAME_GotoGame);
                this->gameMatch(false);
                matchSuccessLayer->setVisible(false);
                if (!ALMusicUtil::getInstrins()->isBGSound()) {
                    // 播放音乐
                    ALMusicUtil::getInstrins()->playBgSound(ALMusicUtil::GameBgMusic::MainMusic);
                }
                
                if (isConnect) {
                    _isDisconnectNotify = true;
                }
            });
        } 
    });
}


/**
 *  游戏进入后台
 */
void StartGameScene::appDidEnterBackgroundObserverFunc(Ref* ref)
{
    CCLOG("StartGameScene 游戏进入后台");
    _appEnterBackgroundTime = ALHelpTools::getCurrentTime();
//    _isDisconnectNotify = false;
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([&,this]()->void{
        
    });
}



/**
 *  游戏恢复前台
 */
void StartGameScene::appWillEnterForegroundObserverFunc(Ref* ref)
{
    CCLOG("StartGameScene  游戏恢复前台");
    // 匹配成功后的界面
    if (matchSuccessLayer->isVisible()) {
        int mtime = (int)(ALHelpTools::getCurrentTime() - _appEnterBackgroundTime);
        _isTimeOutOfMatchSuccess = mtime > 6;
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([&,mtime,this]()->void{
            if (mtime > 6) {
                this->unschedule(DF_SCHEDULE_NAME_SendMatch);
                this->unschedule(DF_SCHEDULE_NAME_DisableCancelMatch);
                this->unschedule(DF_SCHEDULE_NAME_GotoGame);
                this->scheduleOnce([=](float ft){
                    if (ALNetControl::isConnect()) {
                        ALAlertUtil::makeAlertOfDisconnect(this, [&,this]{
                            this->gameMatch(false);
                            matchSuccessLayer->setVisible(false);
                            if (!ALMusicUtil::getInstrins()->isBGSound()) {
                                // 播放音乐
                                ALMusicUtil::getInstrins()->playBgSound(ALMusicUtil::GameBgMusic::MainMusic);
                            }
                        });
                    }
                }, 0.2, DF_SCHEDULE_NAME_SGS_hint_failMatch);
                
            }
        });
    }
    
    
    
    
}




