//
//  ALAlertUtil.cpp
//  LLKGame
//
//  Created by Allen on 2017/12/22.
//
//

#include "ALAlertUtil.h"
#include "ALGameConfig.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ALMusicUtil.h"

using namespace cocos2d::ui;
USING_NS_CC;


void ALAlertUtil::makeAlertOfExitGame(cocos2d::Node *parentNode, const alAlertOkCallback &callback)
{
    // 创建遮盖层
    auto background = ImageView::create("images/background_BTM_50.png");
    background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    background->setTouchEnabled(true);
    background->setPosition(Vec2(DF_DesignSize.width/2, DF_DesignSize.height/2));
    // 创建BOX
    auto box = Sprite::create("images/alalert_box_exitGame.png");
    box->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    box->setPosition(Vec2(DF_DesignSize.width/2, DF_DesignSize.height/2-20));
    // 取消按钮
    auto cancelBtn = Button::create("images/alalert_btn_cancel.png");
    cancelBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    cancelBtn->setPosition(Vec2(box->getContentSize().width*0.25, cancelBtn->getContentSize().height*0.5+30));
    cancelBtn->addClickEventListener([&,callback](Ref*btn){
        ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::BtnClickEffic);
        auto mbox = ((Button*)btn)->getParent();
        auto mbg = mbox->getParent();
        mbox->runAction(Sequence::create(ScaleTo::create(0.3, 0.1),Hide::create(),CallFunc::create([&,mbg]{
            mbg->removeFromParent();
        }), NULL));
    });
    // 确定按钮
    auto okBtn = Button::create("images/alalert_btn_ok.png");
    okBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    okBtn->setPosition(Vec2(box->getContentSize().width*0.75, okBtn->getContentSize().height*0.5+30));
    okBtn->addClickEventListener([&,callback](Ref*btn){
        ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::BtnClickEffic);
        auto mbox = ((Button*)btn)->getParent();
        auto mbg = mbox->getParent();
        mbox->runAction(Sequence::create(ScaleTo::create(0.3, 0.1),Hide::create(),CallFunc::create([&,mbg,callback]{
            mbg->removeFromParent();
            if (callback) {
                callback();
            }
        }), NULL));
        
    });
    
    background->addChild(box);
    box->addChild(cancelBtn);
    box->addChild(okBtn);
    parentNode->addChild(background,9990);
    
    box->setScale(0.1);
    box->runAction(Sequence::create(ScaleTo::create(0.3, 1), NULL));
}


void ALAlertUtil::makeAlertOfDisconnect(cocos2d::Node *parentNode, const alAlertOkCallback &callback)
{
    // 创建遮盖层
    auto background = ImageView::create("images/background_BTM_50.png");
    background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    background->setTouchEnabled(true);
    background->setPosition(Vec2(DF_DesignSize.width/2, DF_DesignSize.height/2));
    // 创建BOX
    auto box = Sprite::create("images/alalert_box_disconnect.png");
    box->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    box->setPosition(Vec2(DF_DesignSize.width/2, DF_DesignSize.height/2-20));
    // 确定按钮
    auto okBtn = Button::create("images/alalert_btn_ok.png");
    okBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    okBtn->setPosition(Vec2(box->getContentSize().width/2, okBtn->getContentSize().height*0.5+30));
    okBtn->addClickEventListener([&,callback](Ref*btn){
        ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::BtnClickEffic);
        auto mbox = ((Button*)btn)->getParent();
        auto mbg = mbox->getParent();
        mbox->runAction(Sequence::create(ScaleTo::create(0.3, 0.1),Hide::create(),CallFunc::create([&,mbg,callback]{
            mbg->removeFromParent();
        }),CallFunc::create([&,mbg,callback]{
            if (callback) {
                callback();
            }
        }), NULL));
        
    });
    
    background->addChild(box);
    box->addChild(okBtn);
    parentNode->addChild(background,9999);
    
    box->setScale(0.1);
    box->runAction(Sequence::create(ScaleTo::create(0.3, 1), NULL));
}


/**
 *  弹出游戏内连接超时提示框
 */
void ALAlertUtil::makeAlertOfConnectTimeOut(cocos2d::Node* parentNode,const alAlertOkCallback& callback)
{
    // 创建遮盖层
    auto background = ImageView::create("images/background_BTM_50.png");
    background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    background->setTouchEnabled(true);
    background->setPosition(Vec2(DF_DesignSize.width/2, DF_DesignSize.height/2));
    // 创建BOX
    auto box = Sprite::create("images/alalert_box_connectTimeOut.png");
    box->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    box->setPosition(Vec2(DF_DesignSize.width/2, DF_DesignSize.height/2-20));
    // 确定按钮
    auto okBtn = Button::create("images/alalert_btn_ok.png");
    okBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    okBtn->setPosition(Vec2(box->getContentSize().width/2, okBtn->getContentSize().height*0.5+30));
    okBtn->addClickEventListener([&,callback](Ref*btn){
        ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::BtnClickEffic);
        auto mbox = ((Button*)btn)->getParent();
        auto mbg = mbox->getParent();
        mbox->runAction(Sequence::create(ScaleTo::create(0.3, 0.1),Hide::create(),CallFunc::create([&,mbg,callback]{
            mbg->removeFromParent();
        }),CallFunc::create([&,mbg,callback]{
            if (callback) {
                callback();
            }
        }), NULL));
        
    });
    
    background->addChild(box);
    box->addChild(okBtn);
    parentNode->addChild(background,9999);
    
    box->setScale(0.1);
    box->runAction(Sequence::create(ScaleTo::create(0.3, 1), NULL));
}



/**
 *  弹出轻豆不足的提示框  alalert_box_qingdouBuGou.png
 */
void ALAlertUtil::makeAlertOfDontHaveEnoughQingDou(cocos2d::Node* parentNode)
{
    // 创建遮盖层
    auto background = ImageView::create("images/background_BTM_50.png");
    background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    background->setTouchEnabled(true);
    background->setPosition(Vec2(DF_DesignSize.width/2, DF_DesignSize.height/2));
    // 创建BOX
    auto box = Sprite::create("images/alalert_box_qingdouBuGou.png");
    box->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    box->setPosition(Vec2(DF_DesignSize.width/2, DF_DesignSize.height/2-20));
    // 确定按钮
    auto okBtn = Button::create("images/alalert_btn_ok.png");
    okBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    okBtn->setPosition(Vec2(box->getContentSize().width/2, okBtn->getContentSize().height*0.5+30));
    okBtn->addClickEventListener([&](Ref*btn){
        ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::BtnClickEffic);
        auto mbox = ((Button*)btn)->getParent();
        auto mbg = mbox->getParent();
        mbox->runAction(Sequence::create(ScaleTo::create(0.3, 0.1),Hide::create(),CallFunc::create([&,mbg]{
            mbg->removeFromParent();
        }), NULL));
        
    });
    
    background->addChild(box);
    box->addChild(okBtn);
    parentNode->addChild(background,9999);
    
    box->setScale(0.1);
    box->runAction(Sequence::create(ScaleTo::create(0.3, 1), NULL));
}








