//
//  ALGameFriendCellNode.h
//  LLKGame-mobile
//
//  Created by Allen on 2018/1/11.
//

#ifndef ALGameFriendCellNode_h
#define ALGameFriendCellNode_h

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"



USING_NS_CC;
using namespace cocos2d::ui;

class ALGameFriendInfoModel;
class ALImageView;

class ALGameFriendCellNode:public Widget
{
public:
    typedef std::function<void (int friendId)> ALYueBtnClickedCallback;
    
   
    
    
    
    
    static ALGameFriendCellNode* createWithModel(ALGameFriendInfoModel* model);
    
    ALGameFriendCellNode();
    ~ALGameFriendCellNode();
    
    
    void initUI();
    /**
     *  设置model
     */
    void setupCellWithModel(ALGameFriendInfoModel* model);
    
    int getFriendId();
    
    /**
     *  设置约按钮的点击事件
     */
    void setupYueBtnClickedCallback(const ALYueBtnClickedCallback& callback);
    
    
private:
    Node* baseNode;
    
    Sprite* box;
    //** 头像 *//
    ALImageView* headView;
    Sprite* headKuang;
    //** 名称 *//
    Text* nameText;
    //** 等级背景 *//
    Sprite* levelSprite;
    //** 等级 *//
    Text* levelText;
    //** 约 按钮 *//
    Button* yueBtn;
    //** 状态sprite *//
    Sprite* stateSprite;
    
    
    
    //** 当前好友的id *//
    int _friendId;
    //** 好友的状态 *//
    int _onlineState;
    
    //** 约按钮的点击事件 *//
    ALYueBtnClickedCallback _yueCallback;
    
    
    
    
    
};

#endif /* ALGameFriendCellNode_h */
