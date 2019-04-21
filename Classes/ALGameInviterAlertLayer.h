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
    typedef std::function<void ()> alAlertDisappearCallback;
    enum INVITE_STATE
    {
        REFUSE_SIGN = -1,  // 拒绝标签
        WAIT_SIGN = 0,     // 等待标签
        AGREE_SIGN = 1,    // 同意标签
    };
    
    static ALGameInviterAlertLayer* create();
    
    ALGameInviterAlertLayer();
    ~ALGameInviterAlertLayer();
    
    void baseInit();
    
    void initUI();
    
    void registerNotification();
    
    
    virtual void layerWillAppear() override;
    
    virtual void layerDidAppear() override;
    
    virtual void layerDidDisappear() override;

    void showAlertWithUserInfo(ALUserInfoModel* model);
    
    void hideAlert();
    
    void hideAlertWithDelay(float delay);
    
    /**
     *  设置标记状态  0 为等待状态  1 为接收   -1 为拒绝
     */
    void setupSignState(int state);
    
    void setupDisappearCallback(const alAlertDisappearCallback& callback);
    
    /**
     *  获取被邀请好友的的uid
     */
    int getFriendUid();
    
    
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
    
    //** 隐藏回调 *//
    alAlertDisappearCallback _disappearCallback;
    
    int _signState;
    //** 剩余时间 *//
    int _remainingTime;
    
    //** 进入后台的时间戳 *//
    long _appEnterBackgroundTime;
    //** 在后台停留的时间 *//
    int _appBackgroundTime;
    
    
    //** 好友uid *//
    int _friendUId;
    
    /**
     *  更新剩余时间
     */
    void updateRemainingTime();
    
    /**
     *  alert 重新设置剩余时间
     */
    void resetRemainingTimeAction();
    
    
    
    
    /**
     *  游戏进入后台
     */
    void appDidEnterBackgroundObserverFunc(Ref* ref);
    
    /**
     *  游戏恢复前台
     */
    void appWillEnterForegroundObserverFunc(Ref* ref);
};

#endif /* ALGameInviterAlertLayer_h */
