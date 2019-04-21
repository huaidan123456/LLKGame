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
#include "ALGameInviteeAlertLayer.h"
#include "ALGameFriendInfoModel.h"
#include "ALWebImageUtil.h"

#include "ALGameConfig.h"




USING_NS_CC;
using namespace cocos2d::ui;

//** 发送自动匹配信息按钮 *//
#define DF_SCHEDULE_NAME_SendMatch "DF_SCHEDULE_NAME_SendMatch"
//** 禁止 取消匹配按钮 *//
#define DF_SCHEDULE_NAME_DisableCancelMatch "DF_SCHEDULE_NAME_DisableCancelMatch"
//** 进入 自动匹配游戏 *//
#define DF_SCHEDULE_NAME_GotoGame "DF_SCHEDULE_NAME_GotoGame"
//** 等待服务器返回数据   (如果 某个时间没有返回，则取消)*//
#define DF_SCHEDULE_SGS_Wait_playerInfoOfMatch "DF_SCHEDULE_SGS_Wait_playerInfoOfMatch"

//** 进入 好友约战的游戏 *//
#define DF_SCHEDULE_NAME_FRIEND_PK_GotoGame "DF_SCHEDULE_NAME_FRIEND_PK_GotoGame"


//** 匹配失败 *//
#define DF_SCHEDULE_NAME_SGS_hint_failMatch "DF_SCHEDULE_NAME_SGS_hint_failMatch"

//** 重连 *//
#define DF_SCHEDULE_NAME_SGS_reconnect "DF_SCHEDULE_NAME_SGS_reconnect"



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
    _isEnableOfSetupBackgroundTime = true;
    
    //** 匹配成功超时 *//
    _isTimeOutOfMatchSuccess = false;
    _timeOutAlertVisible = false;
    
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
    
    if(ALGameConfig::DebugType == GAME_PRODUCTION){
        log("开始连接了   ==  %d",ALUserData::loginUid);
        bool isConnect = ALNetControl::connectServerWithIp(ALND_NF_IP, ALND_NF_PORT, ALUserData::loginUid);
        if (isConnect) {
            // 请求用户信息
            ALNetControl::requestUserInfo();
            ALNetControl::requestGameFriendList();
            ALNetControl::requestOnlineNumber();
            
        }
        
        log("是否连接成功 %d",isConnect);
        
        initScheduler();
    }
    
    // 播放音乐
    ALMusicUtil::getInstrins()->playBgSound(ALMusicUtil::GameBgMusic::MainMusic);
    
    
    
}

void StartGameScene::initScheduler()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
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
#endif
}


void StartGameScene::registerNotification()
{
    NotificationCenter::getInstance()->removeAllObservers(this);
    
    // 断开连接的操作
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(StartGameScene::disConnectObserverFunc), NND_Disconnect, NULL);
    
    //监听用户信息
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(StartGameScene::refreshUserInfoObserverFunc), NND_RefreshUserInfo, NULL);
    
    //监听在线人数
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(StartGameScene::refreshOnlineNumberObserverFunc), NND_RefreshOnlineNumber, NULL);
    
    // 被申请约战
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(StartGameScene::receiveFriendFightInvitationObserverFunc), NND_InviteFightGameWithFriend, NULL);
    
    // 好友对战开始
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(StartGameScene::receiveGoToFriendFightGameObserverFunc), NND_AgreeInviteFlageGame, NULL);
    
    
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
    
    soundSwitchBtn = (Button*)startGameLayer->getChildByName("btn_sound");
    soundSwitchBtn->loadTextureNormal(ALMusicUtil::getInstrins()->getSoundSwitch() ? "images/mms_btn_music_on.png" : "images/mms_btn_music_off.png");
    soundSwitchBtn->addClickEventListener([&,this](Ref* btn){
        ALMusicUtil::getInstrins()->setSoundSwitch(!ALMusicUtil::getInstrins()->getSoundSwitch());
        if (ALMusicUtil::getInstrins()->getSoundSwitch()) {
            ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::BtnClickEffic);
        }
        soundSwitchBtn->loadTextureNormal(ALMusicUtil::getInstrins()->getSoundSwitch() ? "images/mms_btn_music_on.png" : "images/mms_btn_music_off.png");
    });
    
    
    rulesBtn = (Button*)startGameLayer->getChildByName("btn_rules");
    rulesBtn->addClickEventListener([&,this](Ref* btn){
        if (!_isMatching)
        {
            ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::BtnClickEffic);
            gameRulesLayer->setVisible(true);
        }
    });
    
    friendsBtn = (Button*)startGameLayer->getChildByName("btn_friends");
    friendsBtn->setVisible(true);
    friendsBtn->addClickEventListener([&,this](Ref* btn){
        if (!_isMatching)
        {
            ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::BtnClickEffic);
            if(ALGameConfig::DebugType == GAME_PRODUCTION){
                if (!ALNetControl::isConnect()) {
                    _isDisconnectNotify = false;
                    ALAlertUtil::makeAlertOfDisconnect(this, [&,this]{
                        bool isConnect = ALNetControl::connectServerWithIp(ALND_NF_IP, ALND_NF_PORT, ALUserData::loginUid);
                        if (isConnect) {
                            _isDisconnectNotify = true;
                        }
                        // 请求用户信息
                        ALNetControl::requestUserInfo();
                        ALNetControl::requestGameFriendList();
                        ALNetControl::requestOnlineNumber();
                        this->unschedule(DF_SCHEDULE_NAME_SendMatch);
                        this->unschedule(DF_SCHEDULE_NAME_DisableCancelMatch);
                        this->unschedule(DF_SCHEDULE_NAME_GotoGame);
                        this->unschedule(DF_SCHEDULE_SGS_Wait_playerInfoOfMatch);
                        this->gameMatch(false);
                    });
                }else{
                    friendsListLayer->setVisible(true);
                }
            }else{
                
            }
            
        }
        
    });
    
    startMatchBtn = (Button*)startGameLayer->getChildByName("btn_match");
    startMatchBtn->addClickEventListener([&,this](Ref* btn){
        ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::BtnClickEffic);
        if(ALGameConfig::DebugType == GAME_PRODUCTION){
            if (!ALNetControl::isConnect()) {
                _isDisconnectNotify = false;
                ALAlertUtil::makeAlertOfDisconnect(this, [&,this]{
                    bool isConnect = ALNetControl::connectServerWithIp(ALND_NF_IP, ALND_NF_PORT, ALUserData::loginUid);
                    if (isConnect) {
                        _isDisconnectNotify = true;
                    }
                    // 请求用户信息
                    ALNetControl::requestUserInfo();
                    ALNetControl::requestGameFriendList();
                    ALNetControl::requestOnlineNumber();
                    this->unschedule(DF_SCHEDULE_NAME_SendMatch);
                    this->unschedule(DF_SCHEDULE_NAME_DisableCancelMatch);
                    this->unschedule(DF_SCHEDULE_NAME_GotoGame);
                    this->unschedule(DF_SCHEDULE_SGS_Wait_playerInfoOfMatch);
                    this->gameMatch(false);
                });
            }else{
                this->gameMatch(true);
            }
            
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
        if (!_isMatching)
        {
            CCLOG("显示娘卡详情页");
            cardInfoLayer->setupCardSelectedIndex(index,true);
            cardInfoLayer->setVisible(true);
        }
        
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
    
    soundSwitchBtn->loadTextureNormal(ALMusicUtil::getInstrins()->getSoundSwitch() ? "images/mms_btn_music_on.png" : "images/mms_btn_music_off.png");
    _isDisconnectNotify = true;
    startGameLayer->setVisible(true);
    backBtn->setVisible(true);
    rulesBtn->setVisible(true);
    friendsBtn->setVisible(true);
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
    if(ALGameConfig::DebugType == GAME_PRODUCTION){
        log("开始连接了");
        bool isConnect = ALNetControl::connectServerWithIp(ALND_NF_IP, ALND_NF_PORT, ALUserData::loginUid);
        log("是否连接成功 %d",isConnect);
        if (isConnect) {
            // 请求用户信息
            ALNetControl::requestUserInfo();
            ALNetControl::requestGameFriendList();
            ALNetControl::requestOnlineNumber();
        }
    }
    
    
    
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
            ALPlayerData::isResponseInviterNotification = false;
            //            startMatchBtn->loadTextureNormal("images/sgl_btn_matchingBtn.png");
            startMatchBtn->setVisible(false);
            endMatchBtn->setVisible(true);
            endMatchBtn->setEnabled(true);
            
            // 随机种子
            srand((unsigned)time(NULL));
            CCRANDOM_0_1(); // 避免重复
            float delay = (int)(CCRANDOM_0_1()* 10) % 3 + 1; // 1 ~ 3 秒
            log("随机的时间是 %f",delay);
            
            // 禁止取消
            unschedule(DF_SCHEDULE_NAME_DisableCancelMatch);
            scheduleOnce([&,this](float ft){
                endMatchBtn->setEnabled(false);
            }, delay, DF_SCHEDULE_NAME_DisableCancelMatch);
            
            // 发送匹配
            unschedule(DF_SCHEDULE_NAME_SendMatch);
            scheduleOnce([&,this](float ft){
                if(ALGameConfig::DebugType == GAME_PRODUCTION){
                    log("发送获取自动匹配信息");
                    ALNetControl::sendRandomMatch();
                }else{
                    cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([]{
                        cocos2d::NotificationCenter::getInstance()->postNotification(NND_GetRandomMatchUserInfo,NULL);
                    });
                }
                
            }, delay, DF_SCHEDULE_NAME_SendMatch);
            
            // 发送比配失败的信息
            unschedule(DF_SCHEDULE_SGS_Wait_playerInfoOfMatch);
            scheduleOnce([&,this](float ft){
                this->gameMatch(false);
                ALAlertUtil::makeAlertOfMatchFail(this, [&,this]{
                    CCLOG("匹配超时(没有拿到服务器信息)");
                    this->gameMatch(false);
                    matchSuccessLayer->setVisible(false);
                });
            }, delay+10, DF_SCHEDULE_SGS_Wait_playerInfoOfMatch);
            
        }else{
            ALPlayerData::isResponseInviterNotification = true;
            log("取消 匹配");
            //            startMatchBtn->loadTextureNormal("images/sgl_btn_startMatchBtn.png");
            startMatchBtn->setVisible(true);
            endMatchBtn->setVisible(false);
            endMatchBtn->setEnabled(true);
            matchSuccessLayer->setVisible(false);
            unschedule(DF_SCHEDULE_NAME_SendMatch);
            unschedule(DF_SCHEDULE_NAME_DisableCancelMatch);
            unschedule(DF_SCHEDULE_SGS_Wait_playerInfoOfMatch);
            
        }
        
    }
}

void StartGameScene::refreshUserInfo()
{
    // 拿去用户的头像
    ALWebImageUtil::addWebImageToCache(ALUserData::userInfo->getHeadUrl().c_str());
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
            if(ALGameConfig::DebugType == GAME_PRODUCTION){
                if (_isMatching && ALNetControl::isConnect() && !_isTimeOutOfMatchSuccess) {
                    CCLOG("进入游戏");
                    auto gameS = GameScene::createWithIconCount(ALPlayerData::difficultyLevel);
                    //                auto gameS = GamePKScene::createWithIconCount(ALPlayerData::difficultyLevel);
                    Director::getInstance()->replaceScene(gameS);
                }else{
                    this->unschedule(DF_SCHEDULE_NAME_SendMatch);
                    this->unschedule(DF_SCHEDULE_NAME_DisableCancelMatch);
                    this->unschedule(DF_SCHEDULE_NAME_GotoGame);
                    this->unschedule(DF_SCHEDULE_SGS_Wait_playerInfoOfMatch);
                    this->gameMatch(false);
                    matchSuccessLayer->setVisible(false);
                }
            }else{
                CCLOG("进入游戏");
                auto gameS = GameScene::createWithIconCount(ALPlayerData::difficultyLevel);
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
        this->unschedule(DF_SCHEDULE_SGS_Wait_playerInfoOfMatch);
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
 *  接收到好友的约战邀请
 */
void StartGameScene::receiveFriendFightInvitationObserverFunc(Ref* ref)
{
    if (ALPlayerData::isResponseInviterNotification) {
        ALUserInfoModel* model = (ALUserInfoModel*)ref;
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([&,model,this]()->void{
            ALGameInviteeAlertLayer::makeInviteeAlert(this, model, [=](bool isAgree,int uid){
                CCLOG("是否点击了 同意  = %s",(isAgree ? "是" : "否"));
                if (isAgree) {
                    ALNetControl::sendAgreeFightInvite(uid);
                }else{
                    ALNetControl::sendRefuseFightInvite(uid);
                }
            });
            model->release();
        });
    }
}


/**
 *  好友约战OK  准备进入游戏
 */
void StartGameScene::receiveGoToFriendFightGameObserverFunc(Ref* ref)
{
    CCLOG("好友约战OK  准备进入游戏");
    int uid = ((String*)ref)->intValue();
    if (ALUserData::gameFriends.find(uid) != ALUserData::gameFriends.end()) {
        ALGameFriendInfoModel* model = ALUserData::gameFriends.at(uid);
        ALPlayerData::opponentInfo->setUid(model->getUid());
        ALPlayerData::opponentInfo->setName(model->getName().c_str());
        ALPlayerData::opponentInfo->setHeadUrl(model->getHeadUrl().c_str());
        ALPlayerData::opponentInfo->setLevel(model->getLevel());
        
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([&,this]()->void{
            CCLOG("进入游戏");
            friendsListLayer->hideInviterAlertLayerWithState(true);
            this->scheduleOnce([&,this](float ft){
                auto gameS = GamePKScene::createWithIconCount(ALPlayerData::difficultyLevel);
                Director::getInstance()->replaceScene(gameS);
            }, 0.6f, DF_SCHEDULE_NAME_FRIEND_PK_GotoGame);
            
        });
    }
    ref->release();
    
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
                this->unschedule(DF_SCHEDULE_NAME_SendMatch);
                this->unschedule(DF_SCHEDULE_NAME_DisableCancelMatch);
                this->unschedule(DF_SCHEDULE_NAME_GotoGame);
                this->unschedule(DF_SCHEDULE_SGS_Wait_playerInfoOfMatch);
                
                bool isConnect = ALNetControl::connectServerWithIp(ALND_NF_IP, ALND_NF_PORT, ALUserData::loginUid);
                // 请求用户信息
                ALNetControl::requestUserInfo();
                ALNetControl::requestGameFriendList();
                ALNetControl::requestOnlineNumber();
                this->gameMatch(false);
                matchSuccessLayer->setVisible(false);
                // 隐藏好友界面
                friendsListLayer->setVisible(false);
                if (!ALMusicUtil::getInstrins()->isBGSound()) {
                    // 播放音乐
                    ALMusicUtil::getInstrins()->playBgSound(ALMusicUtil::GameBgMusic::MainMusic);
                }
                
                if (isConnect) {
                    _isDisconnectNotify = true;
                }
            });
        }else{
            _isDisconnectNotify = true;
        }
    });
}


/**
 *  游戏进入后台
 */
void StartGameScene::appDidEnterBackgroundObserverFunc(Ref* ref)
{
    if (_isEnableOfSetupBackgroundTime) {
        _appEnterBackgroundTime = ALHelpTools::getCurrentTime();
        _isEnableOfSetupBackgroundTime = false;
    }
    
    _isDisconnectNotify = false;
    CCLOG("StartGameScene 游戏进入后台   == 时间 = %d",_appEnterBackgroundTime);
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([&,this]()->void{
        CCLOG("StartGameScene 后台 主线程");
    });
    
}



/**
 *  游戏恢复前台
 */
void StartGameScene::appWillEnterForegroundObserverFunc(Ref* ref)
{
    CCLOG("StartGameScene  游戏恢复前台   时间  = %d",ALHelpTools::getCurrentTime());
    if (_appEnterBackgroundTime == 0) {
        return;
    }
    int mtime = (int)(ALHelpTools::getCurrentTime() - _appEnterBackgroundTime);
    _appEnterBackgroundTime = 0;
    _isTimeOutOfMatchSuccess = mtime > 6;
    _isEnableOfSetupBackgroundTime = true;
    
    CCLOG("时间差  =  %d",mtime);
    
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([&,mtime,this]()->void{
        CCLOG("StartGameScene 恢复前台 主线程");
        // 判断是否需要重连
        bool isShowAlert = false;
        if (matchSuccessLayer->isVisible() && mtime > 6) {
            CCLOG("匹配成功已经显示");
            this->unschedule(DF_SCHEDULE_NAME_SendMatch);
            this->unschedule(DF_SCHEDULE_NAME_DisableCancelMatch);
            this->unschedule(DF_SCHEDULE_NAME_GotoGame);
            this->unschedule(DF_SCHEDULE_SGS_Wait_playerInfoOfMatch);
            isShowAlert = true;
        }
        this->scheduleOnce([=](float ft){
            bool isConnect = ALNetControl::isConnect();
            if (!isConnect) {
                CCLOG("断开连接");
                this->unschedule(DF_SCHEDULE_NAME_SendMatch);
                this->unschedule(DF_SCHEDULE_NAME_DisableCancelMatch);
                this->unschedule(DF_SCHEDULE_NAME_GotoGame);
                this->unschedule(DF_SCHEDULE_SGS_Wait_playerInfoOfMatch);
                
                isConnect = ALNetControl::connectServerWithIp(ALND_NF_IP, ALND_NF_PORT, ALUserData::loginUid);
                // 请求用户信息
                ALNetControl::requestUserInfo();
                ALNetControl::requestGameFriendList();
                ALNetControl::requestOnlineNumber();
                
                this->gameMatch(false);
                if (isConnect) {
                    _isDisconnectNotify = true;
                }
            }
            if (isConnect && isShowAlert) {
                if (!_timeOutAlertVisible) {
                    ALAlertUtil::makeAlertOfMatchFail(this, [&,this]{
                        CCLOG("匹配超时");
                        this->gameMatch(false);
                        matchSuccessLayer->setVisible(false);
                        if (!ALMusicUtil::getInstrins()->isBGSound()) {
                            // 播放音乐
                            ALMusicUtil::getInstrins()->playBgSound(ALMusicUtil::GameBgMusic::MainMusic);
                        }
                    });
                    
                    _timeOutAlertVisible = true;
                }
                
            }
            
        }, 0.2, DF_SCHEDULE_NAME_SGS_reconnect);
        
    });
    
    
    
    
    //    Director::getInstance()->getScheduler()->performFunctionInCocosThread([&,mtime,this]()->void{
    //        // 匹配成功后的界面
    //        if (matchSuccessLayer->isVisible()) {
    //            if (mtime > 6) {
    //                this->unschedule(DF_SCHEDULE_NAME_SendMatch);
    //                this->unschedule(DF_SCHEDULE_NAME_DisableCancelMatch);
    //                this->unschedule(DF_SCHEDULE_NAME_GotoGame);
    //                this->scheduleOnce([=](float ft){
    //                    if (ALNetControl::isConnect()) {
    //                        ALAlertUtil::makeAlertOfDisconnect(this, [&,this]{
    //                            this->gameMatch(false);
    //                            matchSuccessLayer->setVisible(false);
    //                            if (!ALMusicUtil::getInstrins()->isBGSound()) {
    //                                // 播放音乐
    //                                ALMusicUtil::getInstrins()->playBgSound(ALMusicUtil::GameBgMusic::MainMusic);
    //                            }
    //                        });
    //                    }
    //                }, 0.2, DF_SCHEDULE_NAME_SGS_hint_failMatch);
    //
    //            }
    //        }
    //    });
    
    
}




