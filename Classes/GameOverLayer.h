//
//  GameOverLayer.h
//  LLKGame
//
//  Created by Allen on 2017/12/21.
//
//

#ifndef GameOverLayer_h
#define GameOverLayer_h

#include "ALBaseLayer.h"
#include "cocostudio/CocoStudio.h"


class ALImageView;
class ALUserInfoModel;
class ALGameResultInfoModel;

class GameOverLayer:public ALBaseLayer
{
public:
    typedef std::function<void ()> alGameOverBtnClickCallback;
    typedef std::function<void ()> alGameOverShowCompleteCallback;
    
    enum GameAgainState
    {
        againState = 0, // 再来一次
        waitState,     // 邀请后的等待状态
        invitedState, // 被邀请的状态（对方想再战 点击接受）
        levelState, // 对方已经离开的状态
        
        
    };
    
    static GameOverLayer* create();
    GameOverLayer();
    ~GameOverLayer();
    

    void initUI();
    
    
    virtual void layerWillAppear() override;
    
    virtual void layerDidAppear() override;
    
    virtual void layerDidDisappear() override;
    
    
    /**
     *  显示游戏结果
     */
    void showWithInfo(ALUserInfoModel* selfInfo,ALUserInfoModel* opponentInfo,ALGameResultInfoModel* resultInfo);
    
 
    
    /**
     *  设置再来一次按钮的状态
     */
    void setupGameAgainState(int state);
    /**
     *  获取再来一局按钮的状态
     */
    int getGameAgainBtnState();
    
    /**
     *  刷新用户的轻豆
     */
    void setupQingDouCount(int count);
    
    
    /**
     *  设置按钮的回调
     */
    void setupDrawCardCallback(const alGameOverBtnClickCallback& callback);
    void setupBackCallback(const alGameOverBtnClickCallback& callback);
    void setupGameAgainCallback(const alGameOverBtnClickCallback& callback);
    
    /**
     *  设置游戏结束的显示回调
     */
    void setupGameOverShowCompleteCallback(const alGameOverShowCompleteCallback& callback);
    
    
    
    
private:
    Node* baseNode;
    //** 结果标识 *//
    cocos2d::Sprite* resultSign;
    //** 自己的名字 *//
    cocos2d::ui::Text* snameText;
    //** 等级 *//
    cocos2d::ui::Text* levelText;
    //** 加的经验 *//
    cocos2d::ui::Text* addExpText;
    //** 轻豆 *//
    cocos2d::ui::Text* qingdouText;
    //** 加的轻豆 *//
    cocos2d::ui::Text* addQdText;
    //** 对方的名字 *//
    cocos2d::ui::Text* fnameText;
    //** 抽卡按钮 *//
    cocos2d::ui::Button* drawCardBtn;
    //** 关注按钮 *//
    cocos2d::ui::Button* concernBtn;
    //** 返回按钮 *//
    cocos2d::ui::Button* backBtn;
    //** 自来一局按钮 *//
    cocos2d::ui::Button* againBtn;
    //** 自己的头像 *//
    ALImageView* sheadView;
    //** 对方的头像 *//
    ALImageView* fheadView;
    
    //** 进度条 *//
    cocos2d::ProgressTimer* progress;
    
    
    //** 动画 *//
    cocostudio::timeline::ActionTimeline* showAnimation;
    
    
    //** 对手的id *//
    int _opponentUid = -1;
    //** 增加的经验 *//
    int _addExp = 0;
    //** 增加的轻豆 *//
    int _addQingDou = 0;
    //** 胜负 *//
    bool _gameResult = false;
    //** 是否关注了对方 *//
    bool _isConcern = false;
    
    //** 再来一局按钮状态 *//
    int _gameAgainState = 0;
    
    
    //** 抽奖的回调 *//
    alGameOverBtnClickCallback _drawCardCallback;
    //** 返回回调 *//
    alGameOverBtnClickCallback _backCallback;
    //** 再来一局回调 *//
    alGameOverBtnClickCallback _gameAgainCallback;
    
    //** gameOver 界面显示成功的回调 *//
    alGameOverShowCompleteCallback _showCompleteCallback;
    
};

#endif /* GameOverLayer_h */
