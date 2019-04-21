//
//  ALGameFriendCellNode.cpp
//  LLKGame-mobile
//
//  Created by Allen on 2018/1/11.
//

#include "ALGameFriendCellNode.h"
#include "ALGameFriendInfoModel.h"
#include "ALImageView.h"
#include "cocostudio/CocoStudio.h"
#include "ALHelpTools.h"


ALGameFriendCellNode* ALGameFriendCellNode::createWithModel(ALGameFriendInfoModel *model)
{
    ALGameFriendCellNode * ret = new (std::nothrow) ALGameFriendCellNode();
    
    if (ret && ret->init())
    {
        ret->initUI();
        ret->setupCellWithModel(model);
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


ALGameFriendCellNode::ALGameFriendCellNode()
{
    _friendId = -1;
}


ALGameFriendCellNode::~ALGameFriendCellNode()
{
    CCLOG("~ALGameFriendCellNode 析构  id== %d",_friendId);
}


void ALGameFriendCellNode::initUI()
{
    baseNode = CSLoader::createNode("csbFile/ALFriendCellNode.csb");
    box = (Sprite*)baseNode->getChildByName("s_box");
    
    nameText = (Text*)box->getChildByName("txt_name");
    levelSprite = (Sprite*)box->getChildByName("s_level");
    levelText = (Text*)levelSprite->getChildByName("txt_level");
    stateSprite = (Sprite*)box->getChildByName("s_state");
    yueBtn = (Button*)box->getChildByName("btn_yue");
    yueBtn->addClickEventListener([=](Ref* btn){
        if (_yueCallback) {
            _yueCallback(_friendId);
        }
    });
    
    // 创建头像
    Node* headNode = box->getChildByName("n_headNode");
    Sprite* mask = Sprite::create("images/afll_cell_headMask.png");
    mask->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    mask->setPosition(Vec2::ZERO);
    
    headView = (ALImageView*) ALImageView::create("images/afll_cell_headImage.png");
    headView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    headView->setPosition(Vec2::ZERO);
    headView->ignoreContentAdaptWithSize(false);
    
    auto clip1 = ClippingNode::create(mask);
    clip1->setInverted(false);
    clip1->setContentSize(mask->getContentSize());
    clip1->setAlphaThreshold(0.8f);
    clip1->addChild(headView);
    headNode->addChild(clip1);
    
    headKuang = Sprite::create("images/afll_cell_headKuang.png");
    headKuang->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    headKuang->setPosition(Vec2::ZERO);
    headNode->addChild(headKuang,2);
    
    
    this->setContentSize(box->getContentSize());
    this->addChild(baseNode);
}


void ALGameFriendCellNode::setupCellWithModel(ALGameFriendInfoModel *model)
{
    _friendId = model->getUid();
    _onlineState = model->getOnlineState();
    
    nameText->setString(model->getName());
    levelText->setString(StringUtils::format("Lv.%d",model->getLevel()));
    headView->loadWebImageTexture(model->getHeadUrl(),"images/afll_cell_headImage.png");
    
    switch (_onlineState) {
            // 在线
        case ALGameFriendInfoModel::ONLINE_STATE::ONLINE:
        {
            yueBtn->setVisible(true);
            stateSprite->setTexture("images/afll_cell_txtState_online.png");
            ALHelpTools::removeNodeGray(box);
            ALHelpTools::removeNodeGray(headView);
            ALHelpTools::removeNodeGray(headKuang);
            ALHelpTools::removeNodeGray(levelSprite);
            headView->setColor(Color3B::WHITE);
            nameText->setTextColor(Color4B::WHITE);
            nameText->enableShadow(Color4B(120, 52, 200, 255) ,Size(1,1));
            levelText->setTextColor(Color4B(Color3B(103,80,24)));
            
            
            break;
        }
            // 游戏中
        case ALGameFriendInfoModel::ONLINE_STATE::ONGAME:
        {
            yueBtn->setVisible(false);
            stateSprite->setTexture("images/afll_cell_txtState_pk.png");
            ALHelpTools::removeNodeGray(box);
            ALHelpTools::removeNodeGray(headKuang);
            ALHelpTools::removeNodeGray(nameText);
            ALHelpTools::removeNodeGray(levelSprite);
            ALHelpTools::removeNodeGray(levelText);
            headView->setColor(Color3B::WHITE);
            nameText->setTextColor(Color4B::WHITE);
            nameText->enableShadow(Color4B(120, 52, 200, 255) ,Size(1,1));
            levelText->setTextColor(Color4B(Color3B(103,80,24)));
            break;
        }
            // 离线
        case ALGameFriendInfoModel::ONLINE_STATE::OFFLINE:
        {
            yueBtn->setVisible(false);
            stateSprite->setTexture("images/afll_cell_txtState_offline.png");
            ALHelpTools::setupNodeGray(box);
            ALHelpTools::setupNodeGray(headKuang);
            ALHelpTools::setupNodeGray(nameText);
            ALHelpTools::setupNodeGray(levelSprite);
            ALHelpTools::setupNodeGray(levelText);
            headView->setColor(Color3B(90, 90, 90));
            nameText->setTextColor(Color4B(Color3B(238,233,222)));
            nameText->enableShadow(Color4B(20, 20, 20, 255) ,Size(1,1));
            levelText->setTextColor(Color4B(Color3B(40,20,35)));
            
            break;
        }
    }
    
}



int ALGameFriendCellNode::getFriendId()
{
    return _friendId;
}

void ALGameFriendCellNode::setupYueBtnClickedCallback(const ALYueBtnClickedCallback &callback)
{
    _yueCallback = callback;
}


