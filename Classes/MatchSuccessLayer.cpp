//
//  MatchSuccessLayer.cpp
//  LLKGame
//
//  Created by Allen on 2017/12/15.
//
//

#include "MatchSuccessLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ALImageView.h"
#include "ALUserInfoModel.h"
#include "ALMusicUtil.h"


USING_NS_CC;
using namespace cocos2d::ui;


MatchSuccessLayer* MatchSuccessLayer::create()
{
    MatchSuccessLayer* layer = new (std::nothrow)MatchSuccessLayer();
    if (layer && layer->init()) {
        layer->initUI();
        layer->autorelease();
        return layer;
    }else{
        CC_SAFE_DELETE(layer);
        return nullptr;
    }
}


MatchSuccessLayer::MatchSuccessLayer()
{
    
}

MatchSuccessLayer::~MatchSuccessLayer()
{
    log("MatchSuccessLayer");
}


void MatchSuccessLayer::initUI()
{
    baseNode = CSLoader::createNode("csbFile/MatchSuccessLayer.csb");
    headNode_1 = baseNode->getChildByName("n_headNode_1");
    headNode_2 = baseNode->getChildByName("n_headNode_2");
    
    // 添加头像（上）
    Sprite* mask1 = Sprite::create("images/msl_headMask_pps.png");
    mask1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    mask1->setPosition(Vec2::ZERO);
    
    headView_1 = (ALImageView*) ALImageView::create("images/msl_definedHead_pps.png");
    headView_1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    headView_1->setPosition(Vec2::ZERO);
    headView_1->ignoreContentAdaptWithSize(false);
    
    auto clip1 = ClippingNode::create(mask1);
    clip1->setInverted(false);
    clip1->setContentSize(mask1->getContentSize());
    clip1->setAlphaThreshold(0.8f);
    clip1->addChild(headView_1);
    headNode_1->addChild(clip1);
    
    auto headKuang1 = ImageView::create("images/msl_headKuang_pps.png");
    headKuang1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    headKuang1->setPosition(Vec2::ZERO);
    headNode_1->addChild(headKuang1,2);
    
    // 添加头像（下）
    Sprite* mask2 = Sprite::create("images/msl_headMask_pps.png");
    mask2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    mask2->setPosition(Vec2::ZERO);
    
    headView_2 = (ALImageView*) ALImageView::create("images/msl_definedHead_pps.png");
    headView_2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    headView_2->setPosition(Vec2::ZERO);
    headView_2->ignoreContentAdaptWithSize(false);
    
    auto clip2 = ClippingNode::create(mask2);
    clip2->setInverted(false);
    clip2->setContentSize(mask2->getContentSize());
    clip2->setAlphaThreshold(0.8f);
    clip2->addChild(headView_2);
    headNode_2->addChild(clip2);
    
    auto headKuang2 = ImageView::create("images/msl_headKuang_pps.png");
    headKuang2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    headKuang2->setPosition(Vec2::ZERO);
    headNode_2->addChild(headKuang2,2);
    
    // 名字
    nameText_1 = (Text*)baseNode->getChildByName("txt_name_1");
    nameText_2 = (Text*)baseNode->getChildByName("txt_name_2");
    

    this->addChild(baseNode);
}

void MatchSuccessLayer::layerWillAppear()
{
    
}

void MatchSuccessLayer::layerDidDisappear()
{
    headView_1->loadTexture("images/msl_definedHead_pps.png");
    headView_2->loadTexture("images/msl_definedHead_pps.png");
}



void MatchSuccessLayer::setupMatchUserInfos(ALUserInfoModel *model1, ALUserInfoModel *model2)
{
//    headView_1->loadWebImageTexture(model1->getHeadUrl(),"images/msl_definedHead_pps.png");
//    headView_2->loadWebImageTexture(model2->getHeadUrl(),"images/msl_definedHead_pps.png");
    headView_1->loadWebImageTextureWithLoad(model1->getHeadUrl(), "images/al_webimage_loading.png","images/msl_definedHead_pps.png");
    headView_2->loadWebImageTextureWithLoad(model2->getHeadUrl(), "images/al_webimage_loading.png","images/msl_definedHead_pps.png");
    

    nameText_1->setString(model1->getName());
    nameText_2->setString(model2->getName());
}


void MatchSuccessLayer::showLayerWithMatchSuccess(const ALMatchSuccessCallBack &callback)
{
    ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::MatchSuccessEffic);
    this->setVisible(true);
    if (callback) {
        callback();
    }
}




