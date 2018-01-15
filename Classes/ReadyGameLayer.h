//
//  ReadyGameLayer.h
//  LLKGame
//
//  Created by Allen on 2017/12/19.
//
//

#ifndef ReadyGameLayer_h
#define ReadyGameLayer_h

#include "ALBaseLayer.h"

class ReadyGameLayer:public ALBaseLayer
{
public:
    
    typedef std::function<void ()> ALReadGameAnimationCallback;
    
    static ReadyGameLayer* create();
    ReadyGameLayer();
    ~ReadyGameLayer();
    
    void baseInit();
    void initUI();
    
    
    virtual void layerWillAppear() override;
    
    virtual void layerDidDisappear() override;
    
    
    /**
     *  执行进度条加载动画(此动画)
     */
    void startProgressAnimation(const ALReadGameAnimationCallback& startCallback,const ALReadGameAnimationCallback& endCallback);
    
    
    /**
     *   结束进度条并且 开始Ready Go动画
     */
    void endProgressAndStartReadyGoAnimation(const ALReadGameAnimationCallback& endCallback);
    

    /**
     *  直接执行动作 (waitCallback 是在子线程中执行的)
     */
    void startAniamtion(const ALReadGameAnimationCallback& waitCallback,const ALReadGameAnimationCallback& endCallback);
    

    
    
private:
    Node* baseNode;
    cocos2d::Sprite* progressBg;
    cocos2d::ProgressTimer* progress;
    cocos2d::Sprite* progressTxt;
    cocos2d::Sprite* readGoSprite;
    cocos2d::Sprite* txtSprite;
    
    
    ALReadGameAnimationCallback _endCallback;
    
    
    
    void waitThread(const ALReadGameAnimationCallback& waitCallback);
    
    
};

#endif /* ReadyGameLayer_h */
