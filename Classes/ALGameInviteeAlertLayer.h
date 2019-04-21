//
//  ALGameInviteeAlertLayer.h
//  LLKGame-mobile
//
//  Created by Allen on 2018/1/9.
//

#ifndef ALGameInviteeAlertLayer_h
#define ALGameInviteeAlertLayer_h

#include "ALBaseLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"


class ALImageView;
class ALUserInfoModel;
class ALGameInviteeAlertLayer:public ALBaseLayer
{
public:
    typedef std::function<void (bool isAgree,int uid)> alAlertOperationCallback;
    typedef std::function<void ()> alAlertDisappearCallback;
    
    
    static ALGameInviteeAlertLayer* create();
    /**
     *  弹出 被邀请的Alert
     */
    static void makeInviteeAlert(cocos2d::Node* parentNode,ALUserInfoModel* model,const alAlertOperationCallback& callback);
    
    ALGameInviteeAlertLayer();
    ~ALGameInviteeAlertLayer();
    
    void baseInit();
    
    void initUI();
    
    void registerNotification();
    
    virtual void layerWillAppear() override;
    
    virtual void layerDidAppear() override;
    
    virtual void layerDidDisappear() override;
    
    void showAlertWithUserInfo(ALUserInfoModel* model);
    void hideAlert();
    
    void setupOperationCallback(const alAlertOperationCallback& callback);
    
    void setupDisappearCallback(const alAlertDisappearCallback& callback);
    
    
private:
    Node* baseNode;
    cocos2d::Sprite* boxBg;
    //** 头像node *//
    ALImageView* headView;
    //** 名称 *//
    cocos2d::ui::Text* nameText;
    //** 等级 *//
    cocos2d::ui::Text* levelText;
    //** 进度条 *//
    cocos2d::ProgressTimer* progress;
    //** 进度条上的时间 *//
    cocos2d::ui::Text* remainingTimeText;
    //** 接收按钮 *//
    cocos2d::ui::Button* agreeBtn;
    //** 拒绝按钮 *//
    cocos2d::ui::Button* refuseBtn;
    
    //** 显示动画 *//
    cocostudio::timeline::ActionTimeline* showAnimation;
    //** 隐藏动画 *//
    cocostudio::timeline::ActionTimeline* hideAnimation;
    
    
    //** 剩余时间 *//
    int _remainingTime;
    //** 进入后台的时间戳 *//
    long _appEnterBackgroundTime;
    //** 在后台停留的时间 *//
    int _appBackgroundTime;
    //** 操作回调 *//
    alAlertOperationCallback _operationCallback;
    //** 隐藏回调 *//
    alAlertDisappearCallback _disappearCallback;
    
    
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

#endif /* ALGameInviteeAlertLayer_h */
