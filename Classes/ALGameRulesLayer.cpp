//
//  ALGameRulesLayer.cpp
//  LLKGame
//
//  Created by Allen on 2017/12/25.
//
//

#include "ALGameRulesLayer.h"
#include "ALMusicUtil.h"

USING_NS_CC;
using namespace cocos2d::ui;


ALGameRulesLayer* ALGameRulesLayer::create()
{
    ALGameRulesLayer* layer = new (std::nothrow)ALGameRulesLayer();
    if (layer && layer->init()) {
        layer->initUI();
        layer->autorelease();
        return layer;
    }else{
        CC_SAFE_DELETE(layer);
        return nullptr;
    }
}

ALGameRulesLayer::ALGameRulesLayer()
{
    
}


ALGameRulesLayer::~ALGameRulesLayer()
{
    
}


void ALGameRulesLayer::initUI()
{
    baseNode = CSLoader::createNode("csbFile/GameRulesLayer.csb");
    auto backBtn = (Button*)baseNode->getChildByName("btn_back");
    backBtn->addClickEventListener([&,this](Ref* btn){
        ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::BtnClickEffic);
        this->setVisible(false);
    });
    
    
    this->addChild(baseNode);
}
