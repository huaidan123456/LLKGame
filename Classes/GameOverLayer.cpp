//
//  GameOverLayer.cpp
//  LLKGame
//
//  Created by Allen on 2017/12/21.
//
//

#include "GameOverLayer.h"
#include "ALImageView.h"
#include "ALGameResultInfoModel.h"
#include "ALUserInfoModel.h"
#include "ALMusicUtil.h"
#include "ALUserData.h"
#include "ALPlayerData.h"
#include "ALUserInfoModel.h"


USING_NS_CC;
using namespace cocos2d::ui;

GameOverLayer* GameOverLayer::create()
{
    GameOverLayer* layer = new (std::nothrow)GameOverLayer();
    if (layer && layer->init()) {
        layer->initUI();
        layer->autorelease();
        return layer;
    }else{
        CC_SAFE_DELETE(layer);
        return nullptr;
    }
}


GameOverLayer::GameOverLayer()
{
    //** 对手的id *//
    _opponentUid = -1;
    //** 增加的经验 *//
    _addExp = 0;
    //** 增加的轻豆 *//
    _addQingDou = 0;
    //** 胜负 *//
    _gameResult = false;
    //** 再来一局按钮状态 *//
    int _gameAgainState = 0;
}

GameOverLayer::~GameOverLayer()
{
    
}


void GameOverLayer::initUI()
{
    baseNode = CSLoader::createNode("csbFile/GameOverLayer.csb");
    auto box = baseNode->getChildByName("i_box");
    resultSign = (Sprite*)baseNode->getChildByName("s_top_resultSign");
    snameText = (Text*)box->getChildByName("txt_name");
    levelText = (Text*)box->getChildByName("txt_level");
    addExpText = (Text*)box->getChildByName("txt_addExp");
    qingdouText = (Text*)box->getChildByName("txt_qingdou");
    addQdText = (Text*)box->getChildByName("txt_addQD");
    fnameText = (Text*)box->getChildByName("txt_fname");
    
    drawCardBtn = (Button*)box->getChildByName("btn_drawCard");
    drawCardBtn->addClickEventListener([&,this](Ref* ref){
        ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::BtnClickEffic);
        if (_drawCardCallback) {
            _drawCardCallback();
        }
    });
    
    concernBtn = (Button*)box->getChildByName("btn_concern");
    concernBtn->addClickEventListener([&,this](Ref* ref){
        ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::BtnClickEffic);
//        if (!_isConcern) {
//            _isConcern = true;
//            ((Button*)ref)->setEnabled(false);
//        }
        this->setupConcernState(true);
        if (_concernCallback) {
            _concernCallback(_opponentUid);
        }
    });
    
    backBtn = (Button*)baseNode->getChildByName("btn_btmBack");
    backBtn->addClickEventListener([&,this](Ref* ref){
        ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::BtnClickEffic);
        if (_backCallback) {
            _backCallback();
        }
    });
    
    againBtn = (Button*)baseNode->getChildByName("btn_btmAgain");
    againBtn->addClickEventListener([&,this](Ref* ref){
        ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::BtnClickEffic);
        if (_gameAgainCallback) {
            _gameAgainCallback();
        }
    });
    
    auto sheadNode = box->getChildByName("n_head");
    auto fheadNode = box->getChildByName("n_fhead");
    
    /**
     *  创建头像
     */
    // 自己的头像
    Sprite* mask1 = Sprite::create("images/gol_headMask_self.png");
    mask1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    mask1->setPosition(Vec2::ZERO);
    
    sheadView = (ALImageView*) ALImageView::create("images/gol_definedHead_s.png");
    sheadView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sheadView->setPosition(Vec2::ZERO);
    sheadView->ignoreContentAdaptWithSize(false);
    
    auto clip1 = ClippingNode::create(mask1);
    clip1->setInverted(false);
    clip1->setContentSize(mask1->getContentSize());
    clip1->setAlphaThreshold(0.8f);
    clip1->addChild(sheadView);
    sheadNode->addChild(clip1);
    
    auto headKuang1 = ImageView::create("images/gol_headKuang_self.png");
    headKuang1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    headKuang1->setPosition(Vec2::ZERO);
    sheadNode->addChild(headKuang1,2);
    
    // 加载头像
//    sheadView->loadWebImageTexture(ALUserData::userInfo->getHeadUrl());
    
    // 别人的头像
    Sprite* mask2 = Sprite::create("images/gol_headKuang_op.png");
    mask2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    mask2->setPosition(Vec2::ZERO);
    
    fheadView = (ALImageView*) ALImageView::create("images/gol_definedHead_o.png");
    fheadView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    fheadView->setPosition(Vec2::ZERO);
    fheadView->ignoreContentAdaptWithSize(false);
    
    auto clip2 = ClippingNode::create(mask2);
    clip2->setInverted(false);
    clip2->setContentSize(mask2->getContentSize());
    clip2->setAlphaThreshold(0.8f);
    clip2->addChild(fheadView);
    fheadNode->addChild(clip2);
    
    auto headKuang2 = ImageView::create("images/gol_headMask_op.png");
    headKuang2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    headKuang2->setPosition(Vec2::ZERO);
    fheadNode->addChild(headKuang2,2);
    
    // 加载头像
//    fheadView->loadWebImageTexture(ALPlayerData::opponentInfo->getHeadUrl());
    
    // 初始化进度条
    auto progressBg = (Sprite*)box->getChildByName("s_progress0");
    progress = ProgressTimer::create(Sprite::create("images/gol_progress1.png"));
    progress->setPosition(progressBg->getPosition());
    progress->setType(ProgressTimer::Type::BAR);
    progress->setMidpoint(Vec2(0,0.5));
    progress->setBarChangeRate(Vec2(1, 0));
    progress->setPercentage(0);
    box->addChild(progress,2);
    
    
    
    // 显示动作
    showAnimation = CSLoader::createTimeline("csbFile/GameOverLayer.csb");
    showAnimation->setLastFrameCallFunc([&,this]{
//        drawCardBtn->setEnabled(true);
//        backBtn->setEnabled(true);
//        againBtn->setEnabled(true);
        
        if (_showCompleteCallback) {
            _showCompleteCallback();
        }
    });
    baseNode->runAction(showAnimation);
    
    
    
    this->addChild(baseNode);
}

void GameOverLayer::layerWillAppear()
{
    
}

void GameOverLayer::layerDidAppear()
{
//    drawCardBtn->setEnabled(false);
//    backBtn->setEnabled(false);
//    againBtn->setEnabled(false);
    showAnimation->play("showAnimation", false);
}

void GameOverLayer::layerDidDisappear()
{
    //** 对手的id *//
    _opponentUid = -1;
    //** 增加的经验 *//
    _addExp = 0;
    //** 增加的轻豆 *//
    _addQingDou = 0;
    //** 胜负 *//
    _gameResult = false;
    setupGameAgainState(GameAgainState::againState);
}

void GameOverLayer::showWithInfo(ALUserInfoModel* selfInfo,ALUserInfoModel* opponentInfo,ALGameResultInfoModel* resultInfo)
{
    
    _gameResult = resultInfo->gameResult;
    _addExp = resultInfo->addExp;
    _addQingDou = resultInfo->addQingdao;
    _opponentUid = opponentInfo->getUid();
    
    // 设置结果
    resultSign->setTexture((resultInfo->gameResult ? "images/gol_result_win.png" : "images/gol_result_fail.png"));
    
    // 设置个人信息
    sheadView->loadWebImageTexture(selfInfo->getHeadUrl(),"images/gol_definedHead_s.png");
    snameText->setString(selfInfo->getName());
    levelText->setString(StringUtils::format("lv.%d",selfInfo->getLevel()));
    qingdouText->setString(StringUtils::format("轻豆:%d",selfInfo->getQingDouCount()));
    
    
    // 对方的的信息
    fheadView->loadWebImageTexture(opponentInfo->getHeadUrl(),"images/gol_definedHead_o.png");
    fnameText->setString(opponentInfo->getName());
    
    // 加经验  加轻豆
    addExpText->setString(StringUtils::format("+%d",resultInfo->addExp));
    addQdText->setString(StringUtils::format("+%d",resultInfo->addQingdao));
    
    // 设置进度条的初始值
    float percent = ((float)selfInfo->getCurrentExp()/(float)selfInfo->getSumExp())*100.f;
    progress->setPercentage(percent);
    
    
    this->setVisible(true);
    
    if (resultInfo->gameResult) {
        ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::GameWinEffic);
    }else{
        ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::GameFailEffic);
    }
}


/**
 *  刷新用户的轻豆
 */
void GameOverLayer::setupQingDouCount(int count)
{
    qingdouText->setString(StringUtils::format("轻豆:%d",count));
}


/**
 *  设置关注状态
 */
void GameOverLayer::setupConcernState(bool isConcerState)
{
    _isConcern = isConcerState;
    concernBtn->setEnabled(!_isConcern);
}




void GameOverLayer::setupGameAgainState(int state)
{
    if (_gameAgainState != state) {
        _gameAgainState = state;
        switch (_gameAgainState) {
                //再来一局
            case againState:
            {
                againBtn->loadTextureNormal("images/gol_btn_btm_gameAgain.png");
                againBtn->loadTextureDisabled("images/gol_btn_btm_gameAgain.png");
                againBtn->setEnabled(true);
                break;
            }
                // 等待对方接受
            case waitState:
            {
                againBtn->loadTextureDisabled("images/gol_btn_btm_wait.png");
                againBtn->setEnabled(false);
                break;
            }
                // 对方想再战 点击接受
            case invitedState:
            {
                againBtn->loadTextureNormal("images/gol_btn_btm_invitedg.png");
                againBtn->setEnabled(true);
                break;
            }
                // 对方已离开
            case levelState:
            {
                againBtn->loadTextureDisabled("images/gol_btn_btm_leave.png");
                againBtn->setEnabled(false);
                break;
            }
                
            default:
            {
                againBtn->loadTextureNormal("images/gol_btn_btm_gameAgain.png");
                againBtn->setEnabled(true);
                break;
            }
        }
    }
}

int GameOverLayer::getGameAgainBtnState()
{
    return _gameAgainState;
}

void GameOverLayer::setupDrawCardCallback(const alGameOverBtnClickCallback &callback)
{
    _drawCardCallback = callback;
}

void GameOverLayer::setupBackCallback(const alGameOverBtnClickCallback &callback)
{
    _backCallback = callback;
}

void GameOverLayer::setupGameAgainCallback(const alGameOverBtnClickCallback &callback)
{
    _gameAgainCallback = callback;
}

void GameOverLayer::setupGameOverShowCompleteCallback(const alGameOverShowCompleteCallback &callback)
{
    _showCompleteCallback = callback;
}

void GameOverLayer::setupConcernCallback(const alGameOverConcernCallback& callback)
{
    _concernCallback = callback;
}
