//
//  ReadyGameLayer.cpp
//  LLKGame
//
//  Created by Allen on 2017/12/19.
//
//

#include "ReadyGameLayer.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace cocos2d::ui;

ReadyGameLayer* ReadyGameLayer::create()
{
    ReadyGameLayer* layer = new (std::nothrow)ReadyGameLayer();
    if (layer && layer->init()) {
        layer->baseInit();
        layer->autorelease();
        return layer;
    }else{
        CC_SAFE_DELETE(layer);
        return nullptr;
    }
}


ReadyGameLayer::ReadyGameLayer()
{
    
}

ReadyGameLayer::~ReadyGameLayer()
{
    
}

void ReadyGameLayer::baseInit()
{
    initUI();
}


void ReadyGameLayer::initUI()
{
    baseNode = CSLoader::createNode("csbFile/ReadyGoGameLayer.csb");
    progressBg = (Sprite*)baseNode->getChildByName("s_progress0");
    
    progress = ProgressTimer::create(Sprite::create("images/rggl_progress_1.png"));
    progress->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    progress->setType(ProgressTimer::Type::BAR);
    progress->setMidpoint(Vec2(0,0.5));
    progress->setBarChangeRate(Vec2(1, 0));
    progress->setPercentage(0);
    baseNode->getChildByName("n_progress1")->addChild(progress,2);
    
    readGoSprite = (Sprite*)baseNode->getChildByName("s_ready");
    readGoSprite->setVisible(false);
    
    txtSprite = (Sprite*)baseNode->getChildByName("s_txt");
    txtSprite->setVisible(true);
    
    progressTxt = (Sprite*)baseNode->getChildByName("s_progressTxt");
    
    
    this->addChild(baseNode);
}

void ReadyGameLayer::layerWillAppear()
{
    CCRANDOM_0_1();
    int index = ceilf(CCRANDOM_0_1()*3.f) ;
    CCLOG("index = %d",index);
    txtSprite->setTexture(StringUtils::format("images/rggl_ss_txt_%d.png",index));
}

void ReadyGameLayer::layerDidDisappear()
{
    progress->setPercentage(0);
    readGoSprite->setTexture("images/rggl_sign_ready.png");
    readGoSprite->setVisible(false);
}


void ReadyGameLayer::startProgressAnimation(const ALReadGameAnimationCallback& startCallback,const ALReadGameAnimationCallback& endCallback)
{
    if (startCallback) {
        startCallback();
    }
    progress->runAction(Sequence::create(ProgressTo::create(0.8, 80),CallFunc::create([&,endCallback]{
        if(endCallback){
            endCallback();
        }
    }),NULL));
}


void ReadyGameLayer::endProgressAndStartReadyGoAnimation(const ALReadGameAnimationCallback& endCallback)
{
    progress->stopAllActions();
    progress->runAction(Sequence::create(ProgressTo::create(0.5, 100),CallFunc::create([=]{
        progress->setVisible(false);
        progressBg->setVisible(false);
        progressTxt->setVisible(false);
        
        
    }),TargetedAction::create(readGoSprite, Sequence::create(Show::create(),FadeIn::create(0.3),DelayTime::create(0.4),FadeOut::create(0.3),CallFunc::create([&,this]{
        readGoSprite->setTexture("images/rggl_sign_go.png");
    }),FadeIn::create(0.3),DelayTime::create(0.4),FadeOut::create(0.3),Hide::create(),CallFunc::create([&,endCallback,this]{
        this->setVisible(false);
        if (endCallback) {
            endCallback();
        }
    }), NULL)), NULL));
}


void ReadyGameLayer::startAniamtion(const ALReadGameAnimationCallback &waitCallback, const ALReadGameAnimationCallback &endCallback)
{
    _endCallback = endCallback;
    progress->setVisible(true);
    progressBg->setVisible(true);
    progressTxt->setVisible(true);
    progress->runAction(Sequence::create(ProgressTo::create(0.8, 80),CallFunc::create([&,waitCallback]{
        if (waitCallback) {
            std::thread t1(&ReadyGameLayer::waitThread,this,waitCallback);
            t1.detach();
        }else{
            this->endProgressAndStartReadyGoAnimation(_endCallback);
        }
    }), NULL));

}


void ReadyGameLayer::waitThread(const ALReadGameAnimationCallback &waitCallback)
{
    if (waitCallback) {
        waitCallback();
    }
    this->endProgressAndStartReadyGoAnimation(_endCallback);
}

