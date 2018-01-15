//
//  ALGameInviterAlertLayer.h
//  LLKGame-mobile
//
//  Created by Allen on 2018/1/9.
//

/**
 *  游戏邀请Alert
 */

#ifndef ALGameInviterAlertLayer_h
#define ALGameInviterAlertLayer_h

#include "ALBaseLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

class ALImageView;
class ALUserInfoModel;
class ALGameInviterAlertLayer:public ALBaseLayer
{
public:
    enum INVITE_STATE
    {
        REFUSE_SIGN = -1,  // 拒绝标签
        WAIT_SIGN = 0,     // 等待标签
        AGREE_SIGN = 1,    // 同意标签
    };
    
    static ALGameInviterAlertLayer* create();
    
    ALGameInviterAlertLayer();
    ~ALGameInviterAlertLayer();
    
    void initUI();
    
    
    virtual void layerWillAppear() override;
    
    virtual void layerDidAppear() override;
    
    virtual void layerDidDisappear() override;

    void showAlertWithUserInfo(ALUserInfoModel* model);
    
    /**
     *  设置标记状态  0 为等待状态  1 为接收   -1 为拒绝
     */
    void setupSignState(int state);
    
    
private:
    Node* baseNode;
    cocos2d::Sprite* boxBg;
    //** 头像node *//
    ALImageView* headView;
    //** 名称 *//
    cocos2d::ui::Text* nameText;
    //** 等级 *//
    cocos2d::ui::Text* levelText;
    //** 文字提示 *//
    cocos2d::Sprite* hintSprite;
    //** 进度条 *//
    cocos2d::ProgressTimer* progress;
    //** 进度条上的时间 *//
    cocos2d::ui::Text* remainingTimeText;
    //** 标志 *//
    cocos2d::Sprite* signSprite;

    //** 显示动画 *//
    cocostudio::timeline::ActionTimeline* showAnimation;
    //** 隐藏动画 *//
    cocostudio::timeline::ActionTimeline* hideAnimation;
    
    int _signState;
    //** 剩余时间 *//
    int _remainingTime;
    
    /**
     *  更新剩余时间
     */
    void updateRemainingTime();
};

#endif /* ALGameInviterAlertLayer_h */
