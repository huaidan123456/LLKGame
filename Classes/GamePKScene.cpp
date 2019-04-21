//
//  GamePKScene.cpp
//  LLKGame
//
//  Created by Allen on 2017/12/15.
//
//

#include "GamePKScene.h"
#include "cocostudio/CocoStudio.h"
#include "ALImageView.h"
#include "StartGameScene.h"
#include "ALPlayerData.h"
#include "ALUserData.h"
#include "ReadyGameLayer.h"
#include "ALNetControl.h"
#include "ALGameResultInfoModel.h"
#include "ALNotificationNameDefine.h"
#include "GameOverLayer.h"
#include "ALDrawNiangCardLayer.h"
#include "ALDrawCardResultInfoModel.h"
#include "ALAlertUtil.h"
#include "ALMusicUtil.h"
#include "ALUserInfoModel.h"
#include "ALGameFriendInfoModel.h"
#include "ALToastUtil.h"
#include "ALHelpTools.h"
#include "ALNetworkHelper.h"
#include <sys/timeb.h>




USING_NS_CC;
using namespace cocos2d::ui;

//#define DF_START_POINT_X 75          // 第二个icon的Y坐标
//#define DF_START_POINT_Y 970         // 第一个icon的Y坐标
#define DF_ZORDER_PK_ICON 10            // icon的z坐标
#define DF_ZORDER_PK_DRAWNODE 9         // 画笔的z坐标
#define DF_ZORDER_PK_BOTTOMICON 8       // 底部背景的z坐标

#define DF_SCHEDULE_PK_reconnect "DF_SCHEDULE_PK_reconnect"   //  重连操作
#define DF_SCHEDULE_PK_game "DF_SCHEDULE_PK_game"  // 游戏
#define DF_SCHEDULE_PK_checkFriendOnLine "DF_SCHEDULE_PK_checkFriendOnLine"  // 检查好友的在线情况

#define DF_SCHEDULE_PK_backGame "DF_SCHEDULE_PK_backGame"
#define DF_SCHEDULT_PK_Friend_Leaved_Delay "DF_SCHEDULT_PK_Friend_Leaved_Delay"





GamePKScene* GamePKScene::createWithIconCount(int count)
{
    count = MIN(count, DF_ICON_COUNT);
    
    GamePKScene *scene = new (std::nothrow) GamePKScene(count);
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

GamePKScene::GamePKScene(int iconCount):_iconCount(iconCount)
{
    CCLOG("GamePKScene 的构造");
    df_startPointX = 75;
    df_startPointY = ALGameConfig::designType == DF_DESIGN_TYPE_h1280 ? 970 : 1100;
    _closeDisconnectNotfication = true;
    _isShowAlertOfLeaveInTheGame = false;
    _appEnterBackgroundTime = 0;
    _leftClearIconCount = 0;
    _rightClearIconCount = 0;
    _gameOver = false;
    _isStartGame = false;
    _isPause = false;
    _gameResultInfo = new ALGameResultInfoModel();
    
    
    ALPlayerData::isReadyOfFriendFightGame = false;
    ALPlayerData::leftCleanCountOfPKGame = 0;
    ALPlayerData::rightCleanCountOfPKGame = 0;
}


GamePKScene::~GamePKScene()
{
    delete _gameResultInfo;
    _gameResultInfo = NULL;
    unscheduleAllCallbacks();
    NotificationCenter::getInstance()->removeAllObservers(this);
    ALPlayerData::resetPlayerData();
    log("GamePKScene 析构");
}


void GamePKScene::baseInit()
{
    this->initUI();
    // 绘制icon
    this->initDataMap();
    this->changeDataMap();
    while (isDeadStatus()) {
        this->changeDataMap();
    }
    this->drawMap();
    registerNotification();
    initScheduler();
    
    
    readyLayer->setVisible(true);
    readyLayer->startAniamtion([=]{
        ALNetControl::readyOfFriendFightGame();
    },[=]()->bool{
        return ALPlayerData::isReadyOfFriendFightGame;
    }, [=]{
        this->startGame();
        ALPlayerData::isReadyOfFriendFightGame = false;
    });
}


void GamePKScene::registerNotification()
{
    NotificationCenter::getInstance()->removeAllObservers(this);
    
    //监听游戏结果
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GamePKScene::getGameResultObserverFunc), NND_GetFriendFightGameResult, NULL);
    
    //监听抽奖结果
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GamePKScene::getDrawNiangCardResultObserverFunc), NND_GetDrawNiangCardResult, NULL);
    
    // 刷新轻豆
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GamePKScene::refreshUserQingDouCountObserverFunc), NND_RefreshUserQingDouCount, NULL);
    
    /**
     *  接收到对方再来一局的邀请
     */
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GamePKScene::receiveFriendInvitationOfGameAgainObserverFunc), NND_ReceiveFriendFightGameAgainInvitation, NULL);
    
    /**
     *  接收到对方在游戏结束后 离开
     */
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GamePKScene::receiveFriendLeaveAfterGameOverObserverFunc), NND_ReceiveFriendFightOppositeLeave, NULL);
    
    /**
     *  再来一局游戏 开始
     */
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GamePKScene::receiveGameAgainReadyStartOverObserverFunc), NND_ReceiveFriendFightGameAgainReadyStart, NULL);
    
    
    /**
     *  对方中途退出游戏
     */
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GamePKScene::receiveFriendLeaveInTheGameObserverFunc), NND_ReceiveFriendLeaveInTheGame, NULL);
    
    /**
     *  重连到房间的数据（是否重连成功）
     */
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GamePKScene::receiveFriendFightGameReconnectResultObserverFunc), NND_ReceiveFriendFightReconnectResult, NULL);
    
    
    // 注册游戏超时
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GamePKScene::receiveFightRoomTimeOutObserverFunc), NND_ReceiveFriendFightRoomTimeOut, NULL);
    
    // 断开连接
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GamePKScene::disConnectObserverFunc), NND_Disconnect, NULL);
    
    
    
    //监听进入到后台的操作
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GamePKScene::appDidEnterBackgroundObserverFunc), NND_AppDidEnterBackground, NULL);
    
    //监听恢复到前台的操作
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GamePKScene::appWillEnterForegroundObserverFunc), NND_AppWillEnterForeground, NULL);
}


void GamePKScene::initScheduler()
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



/**
 *   开始游戏
 */
void GamePKScene::startGame()
{
    if (! _isStartGame) {
        _isStartGame = true;
        
        // 开始背景音乐
        ALMusicUtil::getInstrins()->playBgSound(ALMusicUtil::GameBgMusic::GameMusic);
        unschedule(DF_SCHEDULE_PK_game);
        
        /**
         *  设置游戏的监听
         */
        schedule([=](float ft){
            bool isUpate = false;
            if (ALPlayerData::leftCleanCountOfPKGame > _leftClearIconCount
                && ALPlayerData::leftCleanCountOfPKGame <= (_xCount * _yCount *0.5))
            {
                _leftClearIconCount = ALPlayerData::leftCleanCountOfPKGame;
                isUpate = true;
            }
            
            if (ALPlayerData::rightCleanCountOfPKGame > _rightClearIconCount
                && ALPlayerData::rightCleanCountOfPKGame <= (_xCount * _yCount *0.5))
            {
                _rightClearIconCount = ALPlayerData::rightCleanCountOfPKGame;
                isUpate = true;
            }
            
            if (isUpate)
            {
                updateProgress();
            }
            
        }, 0.2, DF_SCHEDULE_PK_game);
    }
}

/**
 *  暂停游戏
 */
void GamePKScene::pauseGame()
{
    _isPause = true;
}


/**
 *  继续游戏
 */
void GamePKScene::resumeGame()
{
    _isPause = false;
}




void GamePKScene::initUI()
{
    // 测试
    /*
     // 重新玩儿
     auto restartBtn = Button::create("images/icon_normal_1.png");
     restartBtn->setPosition(Vec2(650, 1230));
     restartBtn->setTitleText("重玩");
     restartBtn->setTitleColor(Color3B::RED);
     restartBtn->setTitleFontSize(32);
     restartBtn->addClickEventListener([&,this](Ref*btn){
     this->restartGame();
     });
     this->addChild(restartBtn,999);
     */
    
    //    // 加轻豆
    //    auto restartBtn = Button::create("images/icon_normal_1.png");
    //    restartBtn->setPosition(Vec2(650, 1230));
    //    restartBtn->setTitleText("加轻豆");
    //    restartBtn->setTitleColor(Color3B::RED);
    //    restartBtn->setTitleFontSize(32);
    //    restartBtn->addClickEventListener([&,this](Ref*btn){
    //        ALNetControl::requestGetQingDou(2000);
    //    });
    //    this->addChild(restartBtn,999);
    //
             //自动消除
//            auto autoBtn = Button::create("images/icon_normal_1.png");
//            autoBtn->setPosition(Vec2(400, 1230));
//            autoBtn->setTitleText("自动");
//            autoBtn->setTitleColor(Color3B::RED);
//            autoBtn->setTitleFontSize(32);
//            autoBtn->addClickEventListener([&,this](Ref*btn){
//                this->autoClear();
//            });
//            this->addChild(autoBtn,999);
    
    // ---------------------------------------
    
    gameLayer = CSLoader::createNode("csbFile/GameLayer.csb");
    backBtn = (Button*)gameLayer->getChildByName("btn_back");
    backBtn->addClickEventListener([&,this](Ref*btn){
        ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::BtnClickEffic);
        ALAlertUtil::makeAlertOfExitGame(this, [&,this]{
            if (!_isLeaveGame) {
                ALNetControl::sendLeaveRoomOfFriendFight();
                Director::getInstance()->replaceScene(StartGameScene::create());
                _isLeaveGame = true;
                this->unschedule("schedule_leaveGame");
                this->scheduleOnce([=](float ft){
                    _isLeaveGame = false;
                }, 1.f, "schedule_leaveGame");
            }
            
        });
    });
    
    soundSwitchBtn = (Button*)gameLayer->getChildByName("btn_sound");
    soundSwitchBtn->loadTextureNormal(ALMusicUtil::getInstrins()->getSoundSwitch() ? "images/mms_btn_music_on.png" : "images/mms_btn_music_off.png");
    soundSwitchBtn->addClickEventListener([&,this](Ref* btn){
        ALMusicUtil::getInstrins()->setSoundSwitch(!ALMusicUtil::getInstrins()->getSoundSwitch());
        if (ALMusicUtil::getInstrins()->getSoundSwitch()) {
            ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::BtnClickEffic);
        }
        soundSwitchBtn->loadTextureNormal(ALMusicUtil::getInstrins()->getSoundSwitch() ? "images/mms_btn_music_on.png" : "images/mms_btn_music_off.png");
    });
    
    
    progressLeft = (Sprite*)gameLayer->getChildByName("s_progressLeft_1");
    progressLeft->setTextureRect(Rect(0, 0, 0, progressLeft->getContentSize().height));
    progressRight = (Sprite*)gameLayer->getChildByName("s_progressRight_1");
    progressRight->setTextureRect(Rect(0, 0, 0, progressRight->getContentSize().height));
    this->addChild(gameLayer);
    
    
    /**
     *  设置头像
     */
    leftHeadNode = Node::create();
    leftHeadNode->setPosition(progressLeft->getPositionX(), progressLeft->getPositionY());
    this->addChild(leftHeadNode,3);
    
    rightHeadNode = Node::create();
    rightHeadNode->setPosition(progressRight->getPositionX(),progressRight->getPositionY());
    this->addChild(rightHeadNode,2);
    
    // 左边头像
    // 遮盖层
    Sprite* lmask = Sprite::create("images/head_mask.png");
    lmask->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lmask->setPosition(Vec2::ZERO);
    leftHeadImg = (ALImageView*) ALImageView::create("images/definedHead.png");
    leftHeadImg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    leftHeadImg->setPosition(Vec2::ZERO);
    leftHeadImg->ignoreContentAdaptWithSize(false);
    
    auto leftClip = ClippingNode::create(lmask);
    leftClip->setInverted(false);
    leftClip->setContentSize(lmask->getContentSize());
    leftClip->setAlphaThreshold(0.8f);
    leftClip->addChild(leftHeadImg);
    leftHeadNode->addChild(leftClip);
    
    auto leftHeadKuang = ImageView::create("images/head_kuang.png");
    leftHeadKuang->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    leftHeadKuang->setPosition(Vec2::ZERO);
    leftHeadNode->addChild(leftHeadKuang,2);
    
    // 加载头像
    leftHeadImg->loadWebImageTexture(ALUserData::userInfo->getHeadUrl());
    
    // 右边头像
    Sprite* rmask = Sprite::create("images/head_mask.png");
    rmask->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    rmask->setPosition(Vec2::ZERO);
    
    rightHeadImg = (ALImageView*)ALImageView::create("images/definedHead.png");
    rightHeadImg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    rightHeadImg->setPosition(Vec2::ZERO);
    rightHeadImg->ignoreContentAdaptWithSize(false);
    
    auto rightClip = ClippingNode::create(rmask);
    rightClip->setInverted(false);
    rightClip->setContentSize(rmask->getContentSize());
    rightClip->setAlphaThreshold(0.8f);
    rightClip->addChild(rightHeadImg);
    rightHeadNode->addChild(rightClip);
    
    auto rightHeadKuang = ImageView::create("images/head_kuang.png");
    rightHeadKuang->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    rightHeadKuang->setPosition(Vec2::ZERO);
    rightHeadNode->addChild(rightHeadKuang,2);
    // 加载头像
    rightHeadImg->loadWebImageTexture(ALPlayerData::opponentInfo->getHeadUrl());
    
    
    // 设置名称
    leftNameText = (Text*)gameLayer->getChildByName("txt_name_left");
    leftNameText->setString(ALUserData::userInfo->getName());
    
    
    rightNameText = (Text*)gameLayer->getChildByName("txt_name_right");
    rightNameText->setString(ALPlayerData::opponentInfo->getName());
    
    // 设置等级
    auto leftLevelBg = (ImageView*)gameLayer->getChildByName("i_level_left");
    auto rightLevelBg = (ImageView*)gameLayer->getChildByName("i_level_right");
    
    leftLevelText = (Text*)leftLevelBg->getChildByName("txt_level_left");
    leftLevelText->setString(StringUtils::format("Lv.%d",ALUserData::userInfo->getLevel()));
    rightLevelText = (Text*)rightLevelBg->getChildByName("txt_level_right");
    rightLevelText->setString(StringUtils::format("Lv.%d",ALPlayerData::opponentInfo->getLevel()));
    
    
    // 添加触摸监听
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(GamePKScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GamePKScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(GamePKScene::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(GamePKScene::onTouchCancelled, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    
    // 准备界面
    readyLayer = ReadyGameLayer::create();
    readyLayer->setVisible(false);
    this->addChild(readyLayer,99);
    
    // 游戏结束界面
    gameOverLayer = GameOverLayer::create();
    gameOverLayer->setupConcernState(true);
    gameOverLayer->setVisible(false);
    gameOverLayer->setupGameAgainCallback([&,this]{
        // 再来一局
        if (gameOverLayer->getGameAgainBtnState() == GameOverLayer::GameAgainState::againState) {
            gameOverLayer->setupGameAgainState(GameOverLayer::GameAgainState::waitState);
            ALNetControl::sendGameAgainOfFriendFight(ALPlayerData::opponentInfo->getUid());
        }
        // 同意再来一局
        else if (gameOverLayer->getGameAgainBtnState() == GameOverLayer::GameAgainState::invitedState){
            ALNetControl::sendAgreeGameAgainOfFriendFight(ALPlayerData::opponentInfo->getUid());
        }
        
    });
    gameOverLayer->setupBackCallback([&,this]{
        // 返回的回调
        if (!_isLeaveGame) {
            ALNetControl::sendLeaveFriendFightGameOfGameOver(ALPlayerData::opponentInfo->getUid());
            Director::getInstance()->replaceScene(StartGameScene::create());
            _isLeaveGame = true;
            this->unschedule("schedule_leaveGame");
            this->scheduleOnce([=](float ft){
                _isLeaveGame = false;
            }, 1.f, "schedule_leaveGame");
        }
        
    });
    gameOverLayer->setupDrawCardCallback([&,this]{
        // 抽卡回调
        if (ALUserData::qingDouCount >= 50) {
            ALNetControl::sendDrawNiangCardAward();
        }else{
            ALToastUtil::makeToast(this, "images/al_toast_qdBuGou.png");
            
        }
        
    });
    // 游戏结束
    gameOverLayer->setupGameOverShowCompleteCallback([&,this]{
        this->unschedule(DF_SCHEDULE_PK_checkFriendOnLine);
        this->schedule([=](float ft){
            // 查看好友的状态
            if (gameOverLayer->isVisible() && gameOverLayer->getGameAgainBtnState() != GameOverLayer::GameAgainState::levelState)
            {
                // 判断好友是否在线来判断好友是否离开
                if (ALUserData::gameFriends.find(ALPlayerData::opponentInfo->getUid()) != ALUserData::gameFriends.end())
                {
                    ALGameFriendInfoModel* model = ALUserData::gameFriends.at(ALPlayerData::opponentInfo->getUid());
                    if (model->getOnlineState() == ALGameFriendInfoModel::ONLINE_STATE::OFFLINE)
                    {
                        gameOverLayer->setupGameAgainState(GameOverLayer::GameAgainState::levelState);
                    }
                }else{
                    gameOverLayer->setupGameAgainState(GameOverLayer::GameAgainState::levelState);
                }
            }
        }, 1.f,DF_SCHEDULE_PK_checkFriendOnLine);
    });
    
    this->addChild(gameOverLayer,20);
    
    //抽卡展示页面
    drawCardLayer = ALDrawNiangCardLayer::create();
    drawCardLayer->setupLookCardInfoCallbakc([=](int cardIndex){
        if (!_isLeaveGame) {
            ALNetControl::sendLeaveFriendFightGameOfGameOver(ALPlayerData::opponentInfo->getUid());
            Director::getInstance()->replaceScene(StartGameScene::createWithShowCardInfo(cardIndex-1));
            _isLeaveGame = true;
            this->unschedule("schedule_leaveGame");
            this->scheduleOnce([=](float ft){
                _isLeaveGame = false;
            }, 1.f, "schedule_leaveGame");
        }
    });
    drawCardLayer->setVisible(false);
    this->addChild(drawCardLayer,22);
    
}

void GamePKScene::initDataMap()
{
    // 10中类型的icon
    int typeArray[DF_ICON_COUNT]= {0};
    for (int i = 0; i < DF_ICON_COUNT; ++i) {
        typeArray[i] = i+1;
    }
    struct timeb timeSeed;
    ftime(&timeSeed);
    srand(timeSeed.time * 1000 + timeSeed.millitm);
    //    srand((unsigned int)time(NULL));
    int tempIndex;
    for (int i = 0; i < DF_ICON_COUNT; ++i) {
        tempIndex = (int)(CCRANDOM_0_1()*(DF_ICON_COUNT-1-i));
        int temp = typeArray[tempIndex];
        typeArray[tempIndex] = typeArray[DF_ICON_COUNT-1-i];
        typeArray[DF_ICON_COUNT-1-i] = temp;
    }
    
    
    // iconType
    int iconTypeIndex = 0;
    // 是否成对
    bool isPair = false;
    
    for (int x = 0; x < _xCount; ++x) {
        for (int y = 0; y < _yCount; ++y) {
            _dataMap[x][y] = typeArray[iconTypeIndex];
            if (isPair) {
                ++iconTypeIndex;
                iconTypeIndex = iconTypeIndex < _iconCount ? iconTypeIndex : 0;
            }
            isPair = !isPair;
        }
    }
    
}


void GamePKScene::drawMap()
{
    for (int x = 0; x < _xCount; ++x) {
        for (int y = 0; y < _yCount; ++y) {
            auto icon = IconSprite::createWithType(_dataMap[x][y]);
            auto position = indexToScenePosition(x, y);
            icon->setPosition(position);
            this->addChild(icon,DF_ZORDER_PK_ICON);
            _iconVector.pushBack(icon);
            
            auto iconBg = Sprite::create("images/icon_bottom_bg_k.png");
            iconBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            iconBg->setPosition(position);
            this->addChild(iconBg,DF_ZORDER_PK_BOTTOMICON);
        }
    }
}


void GamePKScene::changeDataMap()
{
    struct timeb timeSeed;
    ftime(&timeSeed);
    srand(timeSeed.time * 1000 + timeSeed.millitm);
    //    srand((unsigned int)time(NULL));
    int tempX,tempY,tempM;
    
    // 遍历地图数组，随机换位置
    for (int x = 0; x < _xCount; ++x) {
        for (int y = 0; y < _yCount; ++y) {
            tempX = (int)(CCRANDOM_0_1() * _xCount);
            tempY = (int)(CCRANDOM_0_1() * _yCount);
            if (tempX >= 0 && tempX < _xCount && tempY >= 0 && tempY < _yCount) {
                tempM = _dataMap[x][y];
                _dataMap[x][y] = _dataMap[tempX][tempY];
                _dataMap[tempX][tempY] = tempM;
            }
        }
    }
    // 将选中的数组清空
    _selectedVector.clear();
}


void GamePKScene::refreshIconMap()
{
    // 遍历地图数组，随机换位置
    for (int x = 0; x < _xCount; ++x) {
        for (int y = 0; y < _yCount; ++y) {
            auto icon = _iconVector.at(indexToVectorIndex(x, y));
            icon->changeIconTypeWithAnimation(_dataMap[x][y]);
        }
    }
}

void GamePKScene::addSelectedMapPosit(int x, int y)
{
    if (_gameOver) {
        return;
    }
    if (_selectedVector.size() == 1) {
        Vec2 point1(_selectedVector.front());
        Vec2 point2(x,y);
        if (point1.equals(point2)) {
            log("点击同一个icon");
        }else{
            log("两个坐标 [%d,%d] , [%d,%d]",(int)_selectedVector.front().x,(int)_selectedVector.front().y,x,y);
            auto icon2 = _iconVector.at(indexToVectorIndex((int)point2.x, (int)point2.y));
            icon2->setupSelected(true);
            
            _selectedVector.clear();
            if (isLink(point1, point2)) {
                log("可以连接");
                drawLine();
                // TODO: todo:划线 销毁操作
            }else{
                log("不可以连接");
                _selectedVector.push_back(point2);
                auto icon1 = _iconVector.at(indexToVectorIndex((int)point1.x, (int)point1.y));
                icon1->setupSelected(false);
            }
        }
    }else{
        _selectedVector.push_back(Vec2(x, y));
        auto icon = _iconVector.at(indexToVectorIndex(x, y));
        icon->setupSelected(true);
    }
}


bool GamePKScene::isLink(cocos2d::Vec2 v1, cocos2d::Vec2 v2)
{
    if (v1.equals(v2)) {
        return false;
    }
    
    // 清除路径点击
    _pathVector.clear();
    
    // 判断两点对应的iconType值是否相等
    if (_dataMap[(int)v1.x][(int)v1.y] == _dataMap[(int)v2.x][(int)v2.y])
    {
        // 判断直线
        if (isLinkStraight(v1, v2)) {
            _pathVector.push_back(v1);
            _pathVector.push_back(v2);
            log("=== 是直线连接");
            return true;
        }
        
        // 有一个拐角的时候，对角线可以直连
        auto point = Vec2(v1.x,v2.y);
        if (_dataMap[(int)point.x][(int)point.y] == 0) {
            if (isLinkStraight(v1, point) && isLinkStraight(point, v2)) {
                _pathVector.push_back(v1);
                _pathVector.push_back(point);
                _pathVector.push_back(v2);
                log("=== 有一个拐角");
                return true;
            }
        }
        
        // 有一个拐角的时候，对角线可以直连
        point = Vec2(v2.x,v1.y);
        if (_dataMap[(int)point.x][(int)point.y] == 0) {
            if (isLinkStraight(v1, point) && isLinkStraight(point, v2)) {
                _pathVector.push_back(v1);
                _pathVector.push_back(point);
                _pathVector.push_back(v2);
                log("=== 有一个拐角");
                return true;
            }
        }
        
        // X轴的扩展连线，判断时候有可以连接的
        // 如果在X轴上就表示 p1左右的通路点，Y轴上就表示 p1上下的通路点
        std::vector<Vec2>p1Empty;
        std::vector<Vec2>p2Empty;
        
        // 判断 X扩展
        extendPointsX(v1, p1Empty);
        extendPointsX(v2, p2Empty);
        for (auto pt1 : p1Empty) {
            for (auto pt2 : p2Empty) {
                if (pt1.x == pt2.x) {
                    if (pt1.x == -1 || pt1.x == _xCount) {
                        _pathVector.push_back(v1);
                        _pathVector.push_back(pt1);
                        _pathVector.push_back(pt2);
                        _pathVector.push_back(v2);
                        log("=== 有两个拐角 = x轴");
                        return true;
                    }else{
                        if (isLinkStraight(pt1, pt2)) {
                            _pathVector.push_back(v1);
                            _pathVector.push_back(pt1);
                            _pathVector.push_back(pt2);
                            _pathVector.push_back(v2);
                            log("=== 有两个拐角 = x轴");
                            return true;
                        }
                    }
                }
            }
        }
        
        // 判断 Y扩展
        extendPointsY(v1, p1Empty);
        extendPointsY(v2, p2Empty);
        for (auto pt1 : p1Empty) {
            for (auto pt2 : p2Empty) {
                if (pt1.y == pt2.y) {
                    if (pt1.y == -1 || pt1.y == _yCount) {
                        _pathVector.push_back(v1);
                        _pathVector.push_back(pt1);
                        _pathVector.push_back(pt2);
                        _pathVector.push_back(v2);
                        log("=== 有两个拐角 = y轴");
                        return true;
                    }else{
                        if (isLinkStraight(pt1, pt2)) {
                            _pathVector.push_back(v1);
                            _pathVector.push_back(pt1);
                            _pathVector.push_back(pt2);
                            _pathVector.push_back(v2);
                            log("=== 有两个拐角 = y轴");
                            return true;
                        }
                    }
                }
            }
        }
        
        
        return false;
    }else{
        return false;
    }
}


bool GamePKScene::isLinkStraight(cocos2d::Vec2 v1, cocos2d::Vec2 v2)
{
    // 判断是否x坐标一致，如果一直扫描中间是否是通路
    if (v1.x == v2.x) {
        int minY = MIN(v1.y, v2.y);
        int maxY = MAX(v1.y, v2.y);
        bool flag = true;
        for (int y = minY +1; y < maxY; ++y) {
            if (_dataMap[(int)v1.x][y] != 0) {
                flag = false;
                break;
            }
        }
        if (flag) {
            return true;
        }
    }
    
    // 判断是否y坐标一致，如果一直扫描中间是否是通路
    if (v1.y == v2.y) {
        int minX = MIN(v1.x, v2.x);
        int maxX = MAX(v1.x, v2.x);
        bool flag = true;
        for (int x = minX+1; x < maxX; ++x) {
            if (_dataMap[x][(int)v1.y] != 0) {
                flag = false;
                break;
            }
        }
        if (flag) {
            return true;
        }
    }
    
    return false;
}


void GamePKScene::extendPointsX(cocos2d::Vec2 v, std::vector<cocos2d::Vec2> &vector)
{
    // vector 引用传入
    vector.clear();
    
    if (v.x == _xCount -1) {
        vector.push_back(Vec2(_xCount, v.y));
    }else{
        for (int x = (int)v.x+1; x < _xCount; ++x) {
            if(_dataMap[x][(int)v.y] != 0){
                break;
            }
            vector.push_back(Vec2(x,(int)v.y));
            if (x == _xCount-1) {
                vector.push_back(Vec2(_xCount, v.y));
            }
        }
    }
    
    if (v.x == 0) {
        vector.push_back(Vec2(-1, v.y));
        
    }else{
        for (int x = (int)v.x-1; x >= 0; --x) {
            if (_dataMap[x][(int)v.y] != 0) {
                break;
            }
            vector.push_back(Vec2(x,(int)v.y));
            if (x == 0) {
                vector.push_back(Vec2(-1, v.y));
            }
        }
    }
    
}


void GamePKScene::extendPointsY(cocos2d::Vec2 v, std::vector<cocos2d::Vec2> &vector)
{
    vector.clear();
    
    if (v.y == _yCount - 1) {
        vector.push_back(Vec2(v.x,_yCount));
    }else{
        for (int y = (int)v.y+1; y < _yCount; ++y) {
            if (_dataMap[(int)v.x][y] != 0) {
                break;
            }
            vector.push_back(Vec2(v.x, y));
            if (y == _yCount -1) {
                vector.push_back(Vec2(v.x,_yCount));
            }
        }
    }
    
    if (v.y == 0) {
        vector.push_back(Vec2(v.x,-1));
    }else{
        for (int y = (int)v.y -1; y >= 0 ; --y) {
            if (_dataMap[(int)v.x][y] != 0) {
                break;
            }
            vector.push_back(Vec2(v.x, y));
            if (y == 0) {
                vector.push_back(Vec2(v.x, -1));
            }
        }
    }
}


bool GamePKScene::isDeadStatus()
{
    if (_autoLinkVector.size() == 2) {
        return false;
    }else{
        bool isAllClear = true;
        for (int y = 0; y < _yCount; ++y) {
            for (int x = 0; x < _xCount; ++x) {
                if (_dataMap[x][y] != 0) {
                    isAllClear = false;
                    for (int j = y; j < _yCount; ++j) {
                        if (j == y) {
                            for (int i = x + 1; i < _xCount; ++i) {
                                if (_dataMap[i][j] == _dataMap[x][y] && isLink(Vec2(x,y), Vec2(i,j))) {
                                    _autoLinkVector.push_back(Vec2(x,y));
                                    _autoLinkVector.push_back(Vec2(i,j));
                                    return false;
                                }
                            }
                        }else{
                            for (int i = 0; i < _xCount; ++i) {
                                if (_dataMap[i][j] == _dataMap[x][y] && isLink(Vec2(x,y), Vec2(i,j))) {
                                    _autoLinkVector.push_back(Vec2(x,y));
                                    _autoLinkVector.push_back(Vec2(i,j));
                                    return false;
                                }
                            }
                        }
                    }
                }
            }
        }
        if (isAllClear) {
            GameOver();
            log("@@@@@@@@@@ GAME-OVER @@@@@@@@@");
            return false;
        }
        return true;
    }
    
}


void GamePKScene::drawLine()
{
    // 画线
    auto draw = DrawNode::create();
    this->addChild(draw,DF_ZORDER_PK_DRAWNODE);
    
    if (_pathVector.size() >= 2) {
        Vec2* vecs = new Vec2[_pathVector.size()];
        for (int i = 0; i < _pathVector.size(); ++i) {
            vecs[i] = indexToDrawPosition(_pathVector.at(i).x, _pathVector.at(i).y);
        }
        
        // 画出线段
        for (int i = 0 ; i < _pathVector.size(); ++i) {
            if (i > 0) {
                draw->drawSegment(vecs[i-1], vecs[i], 6, Color4F(249.f/255.f,254.f/255.f,14.f/255.f,1));
            }
        }
        // 删除数组
        delete [] vecs;
        vecs = nullptr;
        
        // 获取icon的坐标
        Vec2 p1 = _pathVector.front();
        Vec2 p2 = _pathVector.back();
        
        clearIcon(p1, p2,draw);
        
    }
    
}



void GamePKScene::clearIcon(cocos2d::Vec2 v1, cocos2d::Vec2 v2, cocos2d::DrawNode* drawNode)
{
    
    //    _leftClearIconCount++;
    //    updateProgress();
    //    ALPlayerData::leftCleanCountOfPKGame++;
    // TODO: todo:发送服务器
    ALNetControl::sendCleanIconOfFriendFight();
    
    _dataMap[(int)v1.x][(int)v1.y] = 0;
    _dataMap[(int)v2.x][(int)v2.y] = 0;
    
    auto icon1 = _iconVector.at(indexToVectorIndex(v1.x, v1.y));
    icon1->destroyIcon([&,drawNode,this]{
    });
    auto icon2 = _iconVector.at(indexToVectorIndex(v2.x, v2.y));
    icon2->destroyIcon([&,drawNode,this]{
        drawNode->clear();
        drawNode->removeFromParent();
    });
    ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::ClearEffic);
    
    
    // 检查是否出现死局
    if (_autoLinkVector.size() == 2) {
        auto aV1 = _autoLinkVector.front();
        auto aV2 = _autoLinkVector.back();
        if (aV1.equals(v1) || aV1.equals(v2) || aV2.equals(v1) || aV2.equals(v2)) {
            _autoLinkVector.clear();
            checkMapStatus();
        }
    }
}


void GamePKScene::autoClear()
{
    if (_autoLinkVector.size() == 2) {
        if (isLink(_autoLinkVector.front(), _autoLinkVector.back())) {
            drawLine();
        }else{
            log("############# 自动消除的时候，提示的两个连线出现了不能连接情况（检测死局的时候出现问题） ###############");
        }
    }else{
        log("############# 自动消除的时候，没有可以消除的Icon（检测死局的时候出现问题） ###############");
    }
}


void GamePKScene::checkMapStatus()
{
    if (isDeadStatus()) {
        // TODO: todo:这里需要做一些提示作用；
        
        changeDataMap();
        while (isDeadStatus()) {
            changeDataMap();
        }
        refreshIconMap();
    }
}


void GamePKScene::updateProgress()
{
    // 移动进度条
    float lrate = ((float)_leftClearIconCount) / ((float)(_xCount * _yCount * 0.5));
    float lwidth = progressLeft->getTexture()->getPixelsWide() * lrate;
    progressLeft->setTextureRect(Rect(0, 0, lwidth, progressLeft->getContentSize().height));
    // 移动头像
    leftHeadNode->setPosition(progressLeft->getPositionX()+lwidth, progressLeft->getPositionY());
    
    
    
    float rrate = ((float)_rightClearIconCount) / ((float)(_xCount * _yCount * 0.5));
    float rwidth = progressRight->getTexture()->getPixelsWide() * rrate;
    progressRight->setTextureRect(Rect(progressRight->getTexture()->getPixelsWide()-rwidth, 0, rwidth, progressRight->getContentSize().height));
    
    rightHeadNode->setPosition(progressRight->getPositionX()-rwidth,progressRight->getPositionY());
}



void GamePKScene::restartGame()
{
    this->unschedule(DF_SCHEDULE_PK_game);
    this->unschedule(DF_SCHEDULE_PK_checkFriendOnLine);
    _gameOver = false;
    _isStartGame = false;
    _isPause = false;
    _selectedVector.clear();
    _autoLinkVector.clear();
    _pathVector.clear();
    ALPlayerData::leftCleanCountOfPKGame = 0;
    ALPlayerData::rightCleanCountOfPKGame = 0;
    _leftClearIconCount = 0;
    _rightClearIconCount = 0;
    this->initDataMap();
    this->changeDataMap();
    while (isDeadStatus()) {
        this->changeDataMap();
    }
    refreshIconMap();
    updateProgress();
    
    gameOverLayer->setVisible(false);
    
    readyLayer->setVisible(true);
    readyLayer->startAniamtion([=]{
        ALNetControl::readyOfFriendFightGame();
    },[=]()->bool{
        return ALPlayerData::isReadyOfFriendFightGame;
    }, [=]{
        this->startGame();
        ALPlayerData::isReadyOfFriendFightGame = false;
    });
    
}

void GamePKScene::GameOver()
{
    _gameOver = true;
}

void GamePKScene::showGameResult()
{
    // 停止
    this->unschedule(DF_SCHEDULE_PK_game);
    
    
    log("展示游戏结果页面");
    // 停止背景音乐
    ALMusicUtil::getInstrins()->stopBgSound();
    _gameResultInfo->printDescription();
    // TODO: todo:展示游戏结果页面
    gameOverLayer->showWithInfo(ALUserData::userInfo, ALPlayerData::opponentInfo, _gameResultInfo);
}






#pragma mark --touch--
bool GamePKScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    auto point = touch->getLocation();
    CCLOG("Location point x=%f, y=%f", point.x, point.y);
    return true;
}

void GamePKScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    
}

void GamePKScene::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event)
{
    
}

void GamePKScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    float px = touch->getLocation().x;
    
    float py = touch->getLocation().y;
    //    log("点击屏幕坐标 (%f,%f)",px,py);
    // 点击区域的限制
    if (py <= df_startPointY + 70) {
        // 获取_dataMap 坐标
        auto point = scenePositionToIndex(px, py);
        log("转化成 Map坐标 (%f,%f)",point.x,point.y);
        
        if (point.x >= 0 && point.y >= 0) {
            int x = point.x;
            int y = point.y;
            if (_dataMap[x][y] > 0) {
                addSelectedMapPosit(x, y);
            }
        }
    }
    
}





#pragma mark -- 私有方法 --


Vec2 GamePKScene::indexToScenePosition(int x, int y)
{
    float px = df_startPointX + x * (DF_ICON_SIZE + DF_ICON_MARGIN);
    float py = df_startPointY - y * (DF_ICON_SIZE + DF_ICON_MARGIN);
    
    return Vec2(px, py);
}


Vec2 GamePKScene::scenePositionToIndex(float px, float py)
{
    // ceil(x)返回的是大于x的最小整数
    int x = ceil(((px - DF_ICON_SIZE/2) - df_startPointX)/(DF_ICON_SIZE + DF_ICON_MARGIN));
    int y = ceil((df_startPointY - (py + DF_ICON_SIZE/2))/(DF_ICON_SIZE + DF_ICON_MARGIN));
    
    if (x < 0 || x >= _xCount) {
        x = -1;
    }
    if (y < 0 || y >= _yCount) {
        y = -1;
    }
    
    return Vec2(x, y);
}


Vec2 GamePKScene::indexToDrawPosition(int x,int y)
{
    float px = df_startPointX + x * (DF_ICON_SIZE + DF_ICON_MARGIN);
    float py = df_startPointY - y * (DF_ICON_SIZE + DF_ICON_MARGIN);
    
    if (x == -1) {
        px = px + DF_ICON_SIZE/2;
    }else if (x == _xCount) {
        px = px - DF_ICON_SIZE/2;
    }
    
    if (y == -1) {
        py = py - DF_ICON_SIZE/2;
    }else if (y == _yCount){
        py = py + DF_ICON_SIZE/2;
    }
    
    return Vec2(px,py);
}



int GamePKScene::indexToVectorIndex(int x, int y)
{
    return (x*_yCount+y);
}


#pragma mark --接收通知的回调--
void GamePKScene::getGameResultObserverFunc(Ref* ref)
{
    ALGameResultInfoModel* model = (ALGameResultInfoModel*)ref;
    model->printDescription();
    _gameResultInfo->gameResult = model->gameResult;
    _gameResultInfo->addExp = model->addExp;
    _gameResultInfo->addQingdao = model->addQingdao;
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([&,this]()->void{
        this->showGameResult();
    });
}


void GamePKScene::getDrawNiangCardResultObserverFunc(cocos2d::Ref *ref)
{
    ALDrawCardResultInfoModel* model = (ALDrawCardResultInfoModel*)ref;
    model->retain();
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([&,model,this]()->void{
        drawCardLayer->showWithInfo(model);
    });
}

/**
 *  刷新用户轻豆数量
 */
void GamePKScene::refreshUserQingDouCountObserverFunc(Ref* ref)
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([&,this]()->void{
        gameOverLayer->setupQingDouCount(ALUserData::qingDouCount);
    });
}

/**
 *  接收到对方再来一局的邀请
 */
void GamePKScene::receiveFriendInvitationOfGameAgainObserverFunc(Ref* ref)
{
    int uid = ((String*)ref)->intValue();
    if (uid == ALPlayerData::opponentInfo->getUid()) {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([&,this]()->void{
            CCLOG("再来一局");
            if (gameOverLayer->isVisible() && gameOverLayer->getGameAgainBtnState() != GameOverLayer::GameAgainState::levelState) {
                // 设置再来一局
                gameOverLayer->setupGameAgainState(GameOverLayer::GameAgainState::invitedState);
            }
        });
    }
}

/**
 *  接收到对方在游戏结束后 离开
 */
void GamePKScene::receiveFriendLeaveAfterGameOverObserverFunc(Ref* ref)
{
    int uid = ((String*)ref)->intValue();
    if (uid == ALPlayerData::opponentInfo->getUid()) {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([&,this]()->void{
            if (gameOverLayer->isVisible()) {
                // 设置对方已经离开
                gameOverLayer->setupGameAgainState(GameOverLayer::GameAgainState::levelState);
            }
        });
    }
}

/**
 *  再来一局游戏 开始
 */
void GamePKScene::receiveGameAgainReadyStartOverObserverFunc(Ref* ref)
{
    int uid = ((String*)ref)->intValue();
    if (uid == ALPlayerData::opponentInfo->getUid())
    {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([&,this]()->void{
            this->restartGame();
        });
    }
}

/**
 *  对方中途退出游戏
 */
void GamePKScene::receiveFriendLeaveInTheGameObserverFunc(Ref* ref)
{
    
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([&,this]()->void{
        CCLOG("对方中途退出游戏");
        if (!_isShowAlertOfLeaveInTheGame) {
            _isShowAlertOfLeaveInTheGame = true;
            ALAlertUtil::makeAlertOfGameInterruption(this, [=]{
                this->unschedule(DF_SCHEDULT_PK_Friend_Leaved_Delay);
                Director::getInstance()->replaceScene(StartGameScene::create());
            });
            // 3 秒退出游戏 返回大厅
            this->unschedule(DF_SCHEDULT_PK_Friend_Leaved_Delay);
            this->scheduleOnce([=](float ft){
                Director::getInstance()->replaceScene(StartGameScene::create());
            }, 3.5f, DF_SCHEDULT_PK_Friend_Leaved_Delay);
        }
        
    });
}

/**
 *  接收到游戏超时消息
 */
void GamePKScene::receiveFightRoomTimeOutObserverFunc(Ref* ref)
{
    CCLOG("接收到游戏超时消息");
    int roomId = ((String*)ref)->intValue();
    if (roomId == ALPlayerData::roomIdOfPK) {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([&,this]()->void{
            ALAlertUtil::makeAlertOfFriendFightRoomTimeOut(this, [=]{
                Director::getInstance()->replaceScene(StartGameScene::create());
            });
            // 3 秒退出游戏 返回大厅
            this->unschedule(DF_SCHEDULT_PK_Friend_Leaved_Delay);
            this->scheduleOnce([=](float ft){
                Director::getInstance()->replaceScene(StartGameScene::create());
            }, 3.5f, DF_SCHEDULT_PK_Friend_Leaved_Delay);
        });
    }
}





/**
 *  断开连接的操作
 */
void GamePKScene::disConnectObserverFunc(Ref* ref)
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([&,this]()->void{
        if (!_closeDisconnectNotfication) {
            this->pauseGame();
            ALAlertUtil::makeAlertOfDisconnect(this, [&,this]{
                Director::getInstance()->replaceScene(StartGameScene::create());
            });
            // 3 秒退出游戏 返回大厅
            this->unschedule(DF_SCHEDULT_PK_Friend_Leaved_Delay);
            this->scheduleOnce([=](float ft){
                Director::getInstance()->replaceScene(StartGameScene::create());
            }, 3.5f, DF_SCHEDULT_PK_Friend_Leaved_Delay);
            
        }else{
            _closeDisconnectNotfication = false;
            bool isConnect = ALNetControl::connectServerWithIp(ALND_NF_IP, ALND_NF_PORT, ALUserData::loginUid);
            if (isConnect) {
                _closeDisconnectNotfication = true;
                // 重新连接回游戏
                ALNetControl::reconnectOfFriendFight(ALUserData::loginUid, ALPlayerData::roomIdOfPK);
            }
        }
    });
}


/**
 *  游戏进入后台
 */
void GamePKScene::appDidEnterBackgroundObserverFunc(Ref* ref)
{
    _appEnterBackgroundTime = ALHelpTools::getCurrentTime();
    CCLOG("GamePKScene  游戏进入后台  时间戳  = %ld",_appEnterBackgroundTime);
    this->pauseGame();
    //    _closeDisconnectNotfication = true;
}

/**
 *  游戏恢复前台
 */
void GamePKScene::appWillEnterForegroundObserverFunc(Ref* ref)
{
    CCLOG("GamePKScene  游戏恢复前台 ");
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([&,this]()->void{
        
        this->scheduleOnce([=](float ft){
            CCLOG("是否连接   = %d",ALNetControl::isConnect());
            if (ALNetControl::isConnect()) {
                if (!gameOverLayer->isVisible()) {
                    ALNetControl::backGameAfterEnterforeground(ALPlayerData::roomIdOfPK);
                }
            }
        }, 0.1, DF_SCHEDULE_PK_backGame);
        
    });
}

/**
 *  重连到房间的数据（是否重连成功）
 */
void GamePKScene::receiveFriendFightGameReconnectResultObserverFunc(Ref* ref)
{
    int code = ((String*)ref)->intValue();
    CCLOG("GamePKScene  重连到房间的数据 code = %d ",code);
    // 失败
    if (code == 0) {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([&,this]()->void{
            if (!gameOverLayer->isVisible()) {
                ALAlertUtil::makeAlertOfFriendFightGameOver(this, [=]{
                    Director::getInstance()->replaceScene(StartGameScene::create());
                });
                // 3 秒退出游戏 返回大厅
                this->unschedule(DF_SCHEDULT_PK_Friend_Leaved_Delay);
                this->scheduleOnce([=](float ft){
                    Director::getInstance()->replaceScene(StartGameScene::create());
                }, 3.5f, DF_SCHEDULT_PK_Friend_Leaved_Delay);
            }
        });
    }
    // 成功
    else if (code == 1){
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([&,this]()->void{
            if (gameOverLayer->isVisible()) {
                gameOverLayer->setupGameAgainState(GameOverLayer::GameAgainState::levelState);
            }
            
        });
    }
}




