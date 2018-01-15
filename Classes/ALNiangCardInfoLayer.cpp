//
//  ALNiangCardInfoLayer.cpp
//  LLKGame
//
//  Created by Allen on 2017/12/23.
//
//

#include "ALNiangCardInfoLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ALNiangCardInfoShowLayer.h"
#include "ALNotificationNameDefine.h"
#include "ALUserData.h"
#include "ALMusicUtil.h"
#include "ALGameHelpUtil.h"






USING_NS_CC;
using namespace cocos2d::ui;


#define DF_AL_CONST_margin 100  // 大卡片的间距
#define DF_AL_CONST_centerX 360 // 中间卡片的X坐标
#define DF_AL_CONST_centerY 920 // 中间卡片的Y坐标


#define DF_AL_CONST_minCard_margin 10 //小卡的间距

ALNiangCardInfoLayer* ALNiangCardInfoLayer::create()
{
    ALNiangCardInfoLayer* layer = new (std::nothrow)ALNiangCardInfoLayer();
    if (layer && layer->init()) {
        layer->initUI();
        layer->registerNotification();
        layer->autorelease();
        return layer;
    }else{
        CC_SAFE_DELETE(layer);
        return nullptr;
    }
    
}



ALNiangCardInfoLayer::ALNiangCardInfoLayer()
{
    _currentIndex = -1;
}

ALNiangCardInfoLayer::~ALNiangCardInfoLayer()
{
    NotificationCenter::getInstance()->removeAllObservers(this);
}

void ALNiangCardInfoLayer::registerNotification()
{
    // 刷新娘卡
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ALNiangCardInfoLayer::refreshNiangCardObserverFunc), NND_RefreshUserNiangCard, NULL);
}
void ALNiangCardInfoLayer::initUI()
{
    baseNode = CSLoader::createNode("csbFile/ALNiangCardInfoLayer.csb");
    // 返回按钮
    auto backBtn = (Button*)baseNode->getChildByName("btn_back");
    backBtn->addClickEventListener([&,this](Ref* ref){
        ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::BtnClickEffic);
        this->setVisible(false);
    });
    
    // 分享按钮
    auto shareBtn = (Button*)baseNode->getChildByName("btn_share");
    shareBtn->addClickEventListener([&,this](Ref* ref){
        ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::BtnClickEffic);

        ALGameHelpUtil::getInstance()->shareOfGameCardInfo();
        
    });
    
    // 我知道了
    auto makeQDBtn= (Button*)baseNode->getChildByName("btn_QD");
    makeQDBtn->addClickEventListener([&,this](Ref* ref){
        ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::BtnClickEffic);
        this->setVisible(false);
    });
    
    cardName = (Sprite*)baseNode->getChildByName("s_cardName");
    
    
    scrollview = (ScrollView*)baseNode->getChildByName("ScrollView");
    scrollview->setScrollBarOpacity(0);
    for (int i = 1; i <= 10; ++i) {
        // 添加小卡
        auto cardBtn = (Button*)scrollview->getChildByName(StringUtils::format("btn_card_%d",i));
        cardBtn->setTag(1000+i);
        cardBtn->addClickEventListener([&,this](Ref* btn){
            ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::BtnClickEffic);
            int index = ((Button*)btn)->getTag()-1000-1;
            this->setupCardSelectedIndex(index,true);
        });
        _cardBtnVector.pushBack(cardBtn);
    }
    
    // 创建卡片
    cardLayer = ALNiangCardInfoShowLayer::create();
    cardLayer->setupChangeIndexCallback([&,this](int index){
        CCLOG("滑动卡片的 index值为 %d",index);
//        if (_currentIndex != index) {
//            auto preBtn = _cardBtnVector.at(_currentIndex);
//            auto curBtn = _cardBtnVector.at(index);
//            // 隐藏框
//            preBtn->getChildByName("i_selected")->setVisible(false);
//            preBtn->setEnabled(true);
//            
//            // 显示框
//            curBtn->getChildByName("i_selected")->setVisible(true);
//            curBtn->setEnabled(false);
//            _currentIndex = index;
//            adjustScrollviewContentOffY(_currentIndex);
//        }

        this->setupCardSelectedIndex(index,false);
    });
    baseNode->addChild(cardLayer,1);
    
    setupCardSelectedIndex(0,true);
    
    this->addChild(baseNode);
    

}

void ALNiangCardInfoLayer::layerWillAppear()
{
    this->refreshNiangCard();
    
}

void ALNiangCardInfoLayer::layerDidAppear()
{
    cardLayer->setupTouchEnable(true);

}

void ALNiangCardInfoLayer::layerDidDisappear()
{
    cardLayer->setupTouchEnable(false);
}


void ALNiangCardInfoLayer::setupCardSelectedIndex(int index,bool isClicked)
{
    index = MAX(0, index);
    index = MIN(index, 9);
    if (_currentIndex < 0) {
        _currentIndex = index;
        auto btn = _cardBtnVector.at(_currentIndex);
        btn->getChildByName("i_selected")->setVisible(true);
        btn->setEnabled(false);
        cardName->setTexture(StringUtils::format("images/ncil_txt_cardName_%d.png",index+1));
        adjustScrollviewContentOffY(_currentIndex);
        if (isClicked) {
            cardLayer->setupCardMoveToIndex(_currentIndex);
        }
        
        return;
    }
    
    if (_currentIndex != index) {
        auto preBtn = _cardBtnVector.at(_currentIndex);
        auto curBtn = _cardBtnVector.at(index);
        // 隐藏框
        preBtn->getChildByName("i_selected")->setVisible(false);
        preBtn->setEnabled(true);
        
        // 显示框
        curBtn->getChildByName("i_selected")->setVisible(true);
        curBtn->setEnabled(false);
        _currentIndex = index;
        cardName->setTexture(StringUtils::format("images/ncil_txt_cardName_%d.png",index+1));
        adjustScrollviewContentOffY(_currentIndex);
        if (isClicked) {
            cardLayer->setupCardMoveToIndex(_currentIndex);
        }
        
    }

}

/**
 *  刷新娘卡是否拥有
 */
void ALNiangCardInfoLayer::refreshNiangCard()
{
    for (int i = 0; i < _cardBtnVector.size() ; i++) {
        auto btn = _cardBtnVector.at(i);
        btn->getChildByName("i_zz")->setVisible(!ALUserData::niangCard[i]);
    }
}


/**
 *  刷新娘卡
 */
void ALNiangCardInfoLayer::refreshNiangCardObserverFunc(Ref* ref)
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([&,this]()->void{
        this->refreshNiangCard();
    });
}


/**
 *  调整Scrollview
 */
void ALNiangCardInfoLayer::adjustScrollviewContentOffY(int index)
{
    //** scrollview positionX *//
    float scrollviewOffX = scrollview->getInnerContainer()->getPositionX();
    //** 获取当前点击的按钮的X位置 *//
    float btnPositionX = _cardBtnVector.at(index)->getPositionX();
    //** scrollview高度  和 内容高度的差 *//
    float minX = scrollview->getInnerContainerSize().width -scrollview->getContentSize().width;
    
    // 第一个按钮
    if (index == 0) {
        scrollview->scrollToPercentHorizontal(0, 0.5, false);
    }
    else if(index == _cardBtnVector.size() - 1){
        scrollview->scrollToPercentHorizontal(100, 0.5, false);
    }
    else{
        float beforeBtnPositionX = _cardBtnVector.at(index-1)->getPositionX();
        float nextBtnPositionX = _cardBtnVector.at(index+1)->getPositionX();
        float bottomH= DF_AL_CONST_minCard_margin + _cardBtnVector.at(index+1)->getContentSize().width/2;
        if (scrollview->getContentSize().width - (nextBtnPositionX + bottomH + scrollviewOffX) < 0)
        {
            float percent = 100.f * ((nextBtnPositionX + bottomH  - scrollview->getContentSize().width)/ minX);
            scrollview->scrollToPercentHorizontal(percent, 0.5, false);
            return;
        }
        
        if (beforeBtnPositionX - bottomH + scrollviewOffX < 0) {
            float percent = 100.f * ((beforeBtnPositionX - bottomH)/minX);
            scrollview->scrollToPercentHorizontal(percent, 0.5, false);
            return;
        }
        
    }
}
