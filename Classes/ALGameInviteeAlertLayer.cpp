//
//  ALGameInviteeAlertLayer.cpp
//  LLKGame-mobile
//
//  Created by Allen on 2018/1/9.
//

#include "ALGameInviteeAlertLayer.h"
#include "ALImageView.h"
#include "ALUserInfoModel.h"
#include "ALMusicUtil.h"
#include "ALNotificationNameDefine.h"
#include "ALHelpTools.h"

USING_NS_CC;
using namespace cocos2d::ui;

#define DF_AGAL_invitee_SCHEDULE_CountDownTime  "DF_AGAL_invitee_SCHEDULE_CountDownTime"


ALGameInviteeAlertLayer* ALGameInviteeAlertLayer::create()
{
    ALGameInviteeAlertLayer* layer = new (std::nothrow)ALGameInviteeAlertLayer();
    if (layer && layer->init()) {
        layer->baseInit();
        layer->autorelease();
        return layer;
    }else{
        CC_SAFE_DELETE(layer);
        return nullptr;
    }
}


void ALGameInviteeAlertLayer::makeInviteeAlert(cocos2d::Node *parentNode, ALUserInfoModel *model, const alAlertOperationCallback &callback)
{
    auto alert = ALGameInviteeAlertLayer::create();
    alert->setVisible(false);
    CCLOG("alert  == %p",alert);
    alert->setupDisappearCallback([=]{
        CCLOG("this  alert  == %p",alert);
        alert->removeFromParent();
    });
    alert->setupOperationCallback(callback);
    parentNode->addChild(alert,9900);
    alert->showAlertWithUserInfo(model);
}

ALGameInviteeAlertLayer::ALGameInviteeAlertLayer()
{
    
    _remainingTime = 10;
    _appEnterBackgroundTime = 0;
    _appBackgroundTime = 0;
}

ALGameInviteeAlertLayer::~ALGameInviteeAlertLayer()
{
    unscheduleAllCallbacks();
    NotificationCenter::getInstance()->removeAllObservers(this);
}

void ALGameInviteeAlertLayer::baseInit()
{
    initUI();
    registerNotification();
}


void ALGameInviteeAlertLayer::registerNotification()
{
    NotificationCenter::getInstance()->removeAllObservers(this);
    
    //监听恢复到前台的操作
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ALGameInviteeAlertLayer::appDidEnterBackgroundObserverFunc), NND_AppDidEnterBackground, NULL);
    
    //监听恢复到前台的操作
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ALGameInviteeAlertLayer::appWillEnterForegroundObserverFunc), NND_AppWillEnterForeground, NULL);
}

void ALGameInviteeAlertLayer::initUI()
{
    baseNode = CSLoader::createNode("csbFile/InviteeAlertLayer.csb");
    boxBg = (Sprite*)baseNode->getChildByName("s_box");
    nameText = (Text*)boxBg->getChildByName("txt_name");
    levelText = (Text*)(boxBg->getChildByName("s_levelBg")->getChildByName("txt_level"));

    progress = ProgressTimer::create(Sprite::create("images/alertL_progress1.png"));
    progress->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    progress->setType(ProgressTimer::Type::BAR);
    progress->setMidpoint(Vec2(0,0.5));
    progress->setBarChangeRate(Vec2(1, 0));
    progress->setPercentage(100);
    boxBg->getChildByName("s_progress")->getChildByName("n_progress")->addChild(progress,2);
    
    remainingTimeText = (Text*)boxBg->getChildByName("s_progress")->getChildByName("txt_time");
    
    // 创建头像
    Node* headNode = boxBg->getChildByName("n_head");
    Sprite* mask = Sprite::create("images/alertL_headmask.png");
    mask->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    mask->setPosition(Vec2::ZERO);
    
    headView = (ALImageView*) ALImageView::create("images/alertL_headImage.png");
    headView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    headView->setPosition(Vec2::ZERO);
    headView->ignoreContentAdaptWithSize(false);
    
    auto clip1 = ClippingNode::create(mask);
    clip1->setInverted(false);
    clip1->setContentSize(mask->getContentSize());
    clip1->setAlphaThreshold(0.8f);
    clip1->addChild(headView);
    headNode->addChild(clip1);
    
    auto headKuang = ImageView::create("images/alertL_headKuang.png");
    headKuang->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    headKuang->setPosition(Vec2::ZERO);
    headNode->addChild(headKuang,2);
    
    agreeBtn = (Button*)boxBg->getChildByName("btn_agree");
    agreeBtn->addClickEventListener([=](Ref* btn){
        ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::BtnClickEffic);
        if (_operationCallback) {
            _operationCallback(true,_friendUId);
        }
        hideAlert();
    });
    
    refuseBtn = (Button*)boxBg->getChildByName("btn_refuse");
    refuseBtn->addClickEventListener([=](Ref* btn){
        ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::BtnClickEffic);
        if (_operationCallback) {
            _operationCallback(false,_friendUId);
        }
        hideAlert();
    });
    
    
    
    /**
     *  动画
     */
    showAnimation = CSLoader::createTimeline("csbFile/InviteeAlertLayer.csb");
    showAnimation->setLastFrameCallFunc([&,this]{
        // 执行进入条逐渐减少的动画
        progress->runAction(ProgressTo::create(_remainingTime, 0));
        this->schedule([=](float ft){
            this->updateRemainingTime();
        }, 1, 10, 0, DF_AGAL_invitee_SCHEDULE_CountDownTime);
    });
    
    hideAnimation = CSLoader::createTimeline("csbFile/InviteeAlertLayer.csb");
    hideAnimation->setLastFrameCallFunc([&,this]{
        this->setVisible(false);
    });
    
    baseNode->runAction(hideAnimation);
    baseNode->runAction(showAnimation);

    this->addChild(baseNode);
}


void ALGameInviteeAlertLayer::layerWillAppear()
{
    
}

void ALGameInviteeAlertLayer::layerDidAppear()
{
    showAnimation->play("showAnimation", false);
}

void ALGameInviteeAlertLayer::layerDidDisappear()
{
    this->unschedule(DF_AGAL_invitee_SCHEDULE_CountDownTime);
    progress->stopAllActions();
    progress->setPercentage(100);
    _remainingTime = 10;
    remainingTimeText->setString(StringUtils::format("%d",_remainingTime));
    
    if (_disappearCallback) {
        _disappearCallback();
    }
}

void ALGameInviteeAlertLayer::showAlertWithUserInfo(ALUserInfoModel *model)
{
    _friendUId = model->getUid();
    headView->loadWebImageTexture(model->getHeadUrl(),"images/alertL_headImage.png");
    nameText->setString(model->getName());
    levelText->setString(StringUtils::format("Lv.%d",model->getLevel()));
    this->setVisible(true);
}

void ALGameInviteeAlertLayer::hideAlert()
{
//    hideAnimation->play("hideAnimation", false);
    if (this->isVisible()) {
        hideAnimation->play("hideAnimation", false);
    }else{
        this->setVisible(false);
    }
}


void ALGameInviteeAlertLayer::setupOperationCallback(const alAlertOperationCallback &callback)
{
    _operationCallback = callback;
}

void ALGameInviteeAlertLayer::setupDisappearCallback(const alAlertDisappearCallback& callback)
{
    _disappearCallback = callback;
}



/**
 *  更新剩余时间
 */
void ALGameInviteeAlertLayer::updateRemainingTime()
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
void ALGameInviteeAlertLayer::resetRemainingTimeAction()
{
    if (this->isVisible()) {
        this->unschedule(DF_AGAL_invitee_SCHEDULE_CountDownTime);
        progress->stopAllActions();
        _remainingTime = _remainingTime - _appBackgroundTime;
        if (_remainingTime > 0) {
            // 执行进入条逐渐减少的动画
            progress->setPercentage(_remainingTime*10);
            remainingTimeText->setString(StringUtils::format("%d",_remainingTime));
            progress->runAction(ProgressTo::create(_remainingTime, 0));
            this->schedule([=](float ft){
                this->updateRemainingTime();
            }, 1, _remainingTime, 0, DF_AGAL_invitee_SCHEDULE_CountDownTime);
        }else{
            hideAlert();
        }

    }
    
}




/**
 *  游戏进入后台
 */
void ALGameInviteeAlertLayer::appDidEnterBackgroundObserverFunc(Ref* ref)
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
void ALGameInviteeAlertLayer::appWillEnterForegroundObserverFunc(Ref* ref)
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
