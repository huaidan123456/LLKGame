//
//  ALGameInviterAlertLayer.cpp
//  LLKGame-mobile
//
//  Created by Allen on 2018/1/9.
//

#include "ALGameInviterAlertLayer.h"
#include "ALImageView.h"
#include "ALUserInfoModel.h"
#include "ALHelpTools.h"
#include "ALNotificationNameDefine.h"

USING_NS_CC;
using namespace cocos2d::ui;

#define DF_AGAL_SCHEDULE_CountDownTime  "DF_AGAL_SCHEDULE_CountDownTime"

#define DF_AGAL_SCHEDULE_RefuseToHide  "DF_AGAL_SCHEDULE_RefuseToHide"

ALGameInviterAlertLayer* ALGameInviterAlertLayer::create()
{
    ALGameInviterAlertLayer* layer = new (std::nothrow)ALGameInviterAlertLayer();
    if (layer && layer->init()) {
        layer->baseInit();
        layer->autorelease();
        return layer;
    }else{
        CC_SAFE_DELETE(layer);
        return nullptr;
    }
}

ALGameInviterAlertLayer::ALGameInviterAlertLayer()
{
    _remainingTime = 10;
    _appBackgroundTime = 0;
    _appEnterBackgroundTime = 0;
}

ALGameInviterAlertLayer::~ALGameInviterAlertLayer()
{
    unscheduleAllCallbacks();
    NotificationCenter::getInstance()->removeAllObservers(this);
}

void ALGameInviterAlertLayer::baseInit()
{
    initUI();
    registerNotification();
    
}


void ALGameInviterAlertLayer::registerNotification()
{
    NotificationCenter::getInstance()->removeAllObservers(this);
    
    //监听恢复到前台的操作
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ALGameInviterAlertLayer::appDidEnterBackgroundObserverFunc), NND_AppDidEnterBackground, NULL);
    
    //监听恢复到前台的操作
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ALGameInviterAlertLayer::appWillEnterForegroundObserverFunc), NND_AppWillEnterForeground, NULL);
}



void ALGameInviterAlertLayer::initUI()
{
    baseNode = CSLoader::createNode("csbFile/InviterAlertLayer.csb");
    boxBg = (Sprite*)baseNode->getChildByName("s_box");
    nameText = (Text*)boxBg->getChildByName("txt_name");
    levelText = (Text*)(boxBg->getChildByName("s_levelBg")->getChildByName("txt_level"));
    hintSprite = (Sprite*)boxBg->getChildByName("s_txt_hint");
    
    progress = ProgressTimer::create(Sprite::create("images/alertL_progress1.png"));
    progress->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    progress->setType(ProgressTimer::Type::BAR);
    progress->setMidpoint(Vec2(0,0.5));
    progress->setBarChangeRate(Vec2(1, 0));
    progress->setPercentage(100);
    boxBg->getChildByName("s_progress")->getChildByName("n_progress")->addChild(progress,2);
    
    remainingTimeText = (Text*)boxBg->getChildByName("s_progress")->getChildByName("txt_time");
    
    signSprite = (Sprite*)boxBg->getChildByName("s_sign");
    // 创建头像
    Node* headNode = boxBg->getChildByName("n_head");
    Sprite* mask1 = Sprite::create("images/alertL_headmask.png");
    mask1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    mask1->setPosition(Vec2::ZERO);
    
    headView = (ALImageView*) ALImageView::create("images/alertL_headImage.png");
    headView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    headView->setPosition(Vec2::ZERO);
    headView->ignoreContentAdaptWithSize(false);
    
    auto clip1 = ClippingNode::create(mask1);
    clip1->setInverted(false);
    clip1->setContentSize(mask1->getContentSize());
    clip1->setAlphaThreshold(0.8f);
    clip1->addChild(headView);
    headNode->addChild(clip1);
    
    auto headKuang1 = ImageView::create("images/alertL_headKuang.png");
    headKuang1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    headKuang1->setPosition(Vec2::ZERO);
    headNode->addChild(headKuang1,2);
    
    showAnimation = CSLoader::createTimeline("csbFile/InviterAlertLayer.csb");
    showAnimation->setLastFrameCallFunc([&,this]{
        // 执行进入条逐渐减少的动画
        progress->runAction(ProgressTo::create(_remainingTime, 0));
        this->schedule([=](float ft){
            this->updateRemainingTime();
        }, 1, 10, 0, DF_AGAL_SCHEDULE_CountDownTime);
 
    });
    
    hideAnimation = CSLoader::createTimeline("csbFile/InviterAlertLayer.csb");
    hideAnimation->setLastFrameCallFunc([&,this]{
        this->setVisible(false);
    });
    
    baseNode->runAction(hideAnimation);
    baseNode->runAction(showAnimation);
    
    
    this->addChild(baseNode);
    
    /**
     *  设置状态
     */
    setupSignState(INVITE_STATE::WAIT_SIGN);
}


void ALGameInviterAlertLayer::layerWillAppear()
{
    
}

void ALGameInviterAlertLayer::layerDidAppear()
{
    showAnimation->play("showAnimation", false);
}

void ALGameInviterAlertLayer::layerDidDisappear()
{
    this->unschedule(DF_AGAL_SCHEDULE_RefuseToHide);
    this->unschedule(DF_AGAL_SCHEDULE_CountDownTime);
    setupSignState(INVITE_STATE::WAIT_SIGN);
    progress->stopAllActions();
    progress->setPercentage(100);
    _remainingTime = 10;
    remainingTimeText->setString(StringUtils::format("%d",_remainingTime));
    
    if (_disappearCallback) {
        _disappearCallback();
    }
    
}

void ALGameInviterAlertLayer::showAlertWithUserInfo(ALUserInfoModel *model)
{
    _friendUId = model->getUid();
    
    headView->loadWebImageTexture(model->getHeadUrl(),"images/alertL_headImage.png");
    nameText->setString(model->getName());
    levelText->setString(StringUtils::format("Lv.%d",model->getLevel()));
    setupSignState(INVITE_STATE::WAIT_SIGN);
    this->setVisible(true);
}

void ALGameInviterAlertLayer::hideAlert()
{
    if (this->isVisible()) {
        hideAnimation->play("hideAnimation", false);
    }else{
        this->setVisible(false);
    }
}

void ALGameInviterAlertLayer::hideAlertWithDelay(float delay)
{
    unschedule(DF_AGAL_SCHEDULE_RefuseToHide);
    scheduleOnce([=](float ft){
        this->hideAlert();
    }, delay, DF_AGAL_SCHEDULE_RefuseToHide);
}



void ALGameInviterAlertLayer::setupDisappearCallback(const alAlertDisappearCallback& callback)
{
    _disappearCallback = callback;
}


/**
 *  获取被邀请好友的的uid
 */
int ALGameInviterAlertLayer::getFriendUid()
{
    return _friendUId;
}



#pragma mark -- 私有方法 --

void ALGameInviterAlertLayer::setupSignState(int state)
{
    if (_signState != state) {
        _signState = state;
        hintSprite->setVisible(true);
        switch (_signState) {
            case INVITE_STATE::REFUSE_SIGN:
            {
                signSprite->setTexture("images/alertL_sign_refuse.png");
                hintSprite->setTexture("images/alertL_txt_refuse.png");
                break;
            }
            case INVITE_STATE::WAIT_SIGN:
            {
                signSprite->setTexture("images/alertL_sign_wait.png");
                hintSprite->setTexture("images/alertL_txt_wait.png");
                break;
            }
            case INVITE_STATE::AGREE_SIGN:
            {
                signSprite->setTexture("images/alertL_sign_agree.png");
                hintSprite->setVisible(false);
                break;
            }
        }
        
    }
}


/**
 *  更新剩余时间
 */
void ALGameInviterAlertLayer::updateRemainingTime()
{
    if (_remainingTime > 0) {
        _remainingTime--;
        remainingTimeText->setString(StringUtils::format("%d",_remainingTime));
    }else{
        hideAlert();
    }
}


/**
 *  alert 重新设置剩余时间
 */
void ALGameInviterAlertLayer::resetRemainingTimeAction()
{
    if (this->isVisible()) {
        this->unschedule(DF_AGAL_SCHEDULE_RefuseToHide);
        this->unschedule(DF_AGAL_SCHEDULE_CountDownTime);
        progress->stopAllActions();
        // 先判断当前的状态 （如果是等待状态，则判断等待时间，如果是拒绝状态或者同意，则立即消失）
        if (_signState == WAIT_SIGN) {
            _remainingTime = _remainingTime - _appBackgroundTime;
            if (_remainingTime > 0) {
                // 执行进入条逐渐减少的动画
                progress->setPercentage(_remainingTime*10);
                remainingTimeText->setString(StringUtils::format("%d",_remainingTime));
                progress->runAction(ProgressTo::create(_remainingTime, 0));
                this->schedule([=](float ft){
                    this->updateRemainingTime();
                }, 1, _remainingTime, 0, DF_AGAL_SCHEDULE_CountDownTime);
            }else{
                hideAlert();
            }
        }else{
            hideAlert();
        }

    }

}







/**
 *  游戏进入后台
 */
void ALGameInviterAlertLayer::appDidEnterBackgroundObserverFunc(Ref* ref)
{
    if (!(this->isVisible())) {
        return;
    }
    
    
    if(_appEnterBackgroundTime == 0)
    {
        _appEnterBackgroundTime = ALHelpTools::getCurrentTime();
    }
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([&,this]()->void{
        if (showAnimation->isPlaying()) {
        }
        
        if (hideAnimation->isPlaying())
        {
            hideAnimation->pause();
            this->setVisible(false);
        }
    });
}

/**
 *  游戏恢复前台
 */
void ALGameInviterAlertLayer::appWillEnterForegroundObserverFunc(Ref* ref)
{
    if (!(this->isVisible())) {
        return;
    }
    
    if (_appEnterBackgroundTime == 0) {
        return;
    }
    _appBackgroundTime = (int)(ALHelpTools::getCurrentTime() - _appEnterBackgroundTime);
    _appEnterBackgroundTime = 0;
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([&,this]()->void{
        this->resetRemainingTimeAction();
        _appBackgroundTime = 0;
    });
}



