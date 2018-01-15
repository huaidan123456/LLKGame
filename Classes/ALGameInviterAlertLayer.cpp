//
//  ALGameInviterAlertLayer.cpp
//  LLKGame-mobile
//
//  Created by Allen on 2018/1/9.
//

#include "ALGameInviterAlertLayer.h"
#include "ALImageView.h"
#include "ALUserInfoModel.h"

USING_NS_CC;
using namespace cocos2d::ui;

#define DF_AGAL_SCHEDULE_CountDownTime  "DF_AGAL_SCHEDULE_CountDownTime"

ALGameInviterAlertLayer* ALGameInviterAlertLayer::create()
{
    ALGameInviterAlertLayer* layer = new (std::nothrow)ALGameInviterAlertLayer();
    if (layer && layer->init()) {
        layer->initUI();
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
}

ALGameInviterAlertLayer::~ALGameInviterAlertLayer()
{
    
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
    this->unschedule(DF_AGAL_SCHEDULE_CountDownTime);
    setupSignState(INVITE_STATE::WAIT_SIGN);
    progress->setPercentage(100);
    _remainingTime = 10;
    remainingTimeText->setString(StringUtils::format("%d",_remainingTime));
    
}

void ALGameInviterAlertLayer::showAlertWithUserInfo(ALUserInfoModel *model)
{
    headView->loadWebImageTexture(model->getHeadUrl(),"images/alertL_headImage.png");
    nameText->setString(model->getName());
    levelText->setString(StringUtils::format("Lv.%d",model->getLevel()));
    setupSignState(INVITE_STATE::WAIT_SIGN);
    this->setVisible(true);
}




#pragma mark -- 私有方法 --

void ALGameInviterAlertLayer::setupSignState(int state)
{
    if (_signState != state) {
        _signState = state;
        switch (_signState) {
            case INVITE_STATE::REFUSE_SIGN:
            {
                signSprite->setTexture("images/alertL_sign_refuse.png");
                break;
            }
            case INVITE_STATE::WAIT_SIGN:
            {
                signSprite->setTexture("images/alertL_sign_wait.png");
                break;
            }
            case INVITE_STATE::AGREE_SIGN:
            {
                signSprite->setTexture("images/alertL_sign_agree.png");
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
        hideAnimation->play("hideAnimation", false);
    }
}



