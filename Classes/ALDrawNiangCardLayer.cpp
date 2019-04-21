//
//  ALDrawNiangCardLayer.cpp
//  LLKGame
//
//  Created by Allen on 2017/12/22.
//
//

#include "ALDrawNiangCardLayer.h"
#include "ALDrawCardResultInfoModel.h"
//#include "StartGameScene.h"
#include "ALMusicUtil.h"
#include "ALGameHelpUtil.h"


USING_NS_CC;
using namespace cocos2d::ui;

ALDrawNiangCardLayer* ALDrawNiangCardLayer::create()
{
    ALDrawNiangCardLayer* layer = new (std::nothrow)ALDrawNiangCardLayer();
    if (layer && layer->init()) {
        layer->initUI();
        layer->autorelease();
        return layer;
    }else{
        CC_SAFE_DELETE(layer);
        return nullptr;
    }
}


ALDrawNiangCardLayer::ALDrawNiangCardLayer()
{
    _cardIndex = 0;
}


ALDrawNiangCardLayer::~ALDrawNiangCardLayer()
{
    
}


void ALDrawNiangCardLayer::initUI()
{
    baseNode = CSLoader::createNode("csbFile/DrawNiangCardLayer.csb");
    
    
    // 抽中
    doAwardNode = baseNode->getChildByName("n_doDraw");
    cardSprite = (Sprite*) doAwardNode->getChildByName("s_card");
    getCardtxtS = (Sprite*) doAwardNode->getChildByName("s_txt_getCard");
    cardNameS = (Sprite*) doAwardNode->getChildByName("s_txt_cardName");
    explaintxtS = (Sprite*) doAwardNode->getChildByName("s_txt_explain");
    lookBtn = (Button*) doAwardNode->getChildByName("btn_look");
    lookBtn->addClickEventListener([&,this](Ref* ref){
        ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::BtnClickEffic);
        this->setVisible(false);
        if (_lookCardInfoCallback) {
            _lookCardInfoCallback(_cardIndex);
        }
//        Director::getInstance()->replaceScene(StartGameScene::createWithShowCardInfo(_cardIndex-1));
    });

    
    // 未抽中
    noAwardNode = baseNode->getChildByName("n_noDraw");
    qingDouSprite = (Sprite*) noAwardNode->getChildByName("s_qingDouCard");
    addQDtxtS = (Sprite*) noAwardNode->getChildByName("s_txt_addQD");
    sorrytxtS = (Sprite*) noAwardNode->getChildByName("s_txt_sorry");
    okBtn = (Button*) noAwardNode->getChildByName("btn_ok");
    okBtn->addClickEventListener([&,this](Ref* ref){
        ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::BtnClickEffic);
        this->setVisible(false);
    });

    
    shareBtn = (Button*) baseNode->getChildByName("btn_share");
    shareBtn->addClickEventListener([&,this](Ref* ref){
        ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::BtnClickEffic);
        CCLOG("分享按钮");
        ALGameHelpUtil::getInstance()->shareOfGameDrawCard(_cardIndex);
        
    });
    
    doAwardNode->setVisible(false);
    noAwardNode->setVisible(false);
    
    this->addChild(baseNode);
}


void ALDrawNiangCardLayer::layerWillAppear()
{
    
}

void ALDrawNiangCardLayer::layerDidAppear()
{
    
}

void ALDrawNiangCardLayer::layerDidDisappear()
{
    doAwardNode->setVisible(false);
    noAwardNode->setVisible(false);
}


void ALDrawNiangCardLayer::showWithInfo(ALDrawCardResultInfoModel* model)
{
     CCLOG("bnsocket抽卡详情   %d  %d  %d ",model->isGain,model->cardIndex,model->awardIndex);
    
    // 中奖
    if (model->isGain) {
        // 显示分享按钮
        shareBtn->setVisible(true);
        
        _cardIndex = model->cardIndex;
        doAwardNode->setVisible(true);
        noAwardNode->setVisible(false);
        cardSprite->setTexture(StringUtils::format("images/sg_niang_card_%d.png",model->cardIndex));
        cardNameS->setTexture(StringUtils::format("images/dncl_txt_niangCard_name_%d.png",model->cardIndex));
        if (model->awardIndex == 0) {
            explaintxtS->setVisible(false);
            getCardtxtS->setPositionY(-230);
            cardNameS->setPositionY(-230);
        }else{
            explaintxtS->setVisible(true);
            explaintxtS->setTexture(StringUtils::format("images/sg_txt_wz_gather_%d.png",model->awardIndex));
            getCardtxtS->setPositionY(-200);
            cardNameS->setPositionY(-200);
        }
    }// 未中奖
    else{
        // 隐藏分享按钮
        shareBtn->setVisible(false);
        
        _cardIndex = 0;
        doAwardNode->setVisible(false);
        noAwardNode->setVisible(true);
    }
    
    this->setVisible(true);
    
}

/**
 *  设置查看卡片详情的回调
 */
void ALDrawNiangCardLayer::setupLookCardInfoCallbakc(const alDrawCardResultLookCardInfoCallback& callback)
{
    _lookCardInfoCallback = callback;
}



