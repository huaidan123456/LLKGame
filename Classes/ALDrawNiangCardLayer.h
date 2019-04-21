//
//  ALDrawNiangCardLayer.h
//  LLKGame
//
//  Created by Allen on 2017/12/22.
//
//

#ifndef ALDrawNiangCardLayer_h
#define ALDrawNiangCardLayer_h

#include "ALBaseLayer.h"
#include "cocostudio/CocoStudio.h"


class ALDrawCardResultInfoModel;
class ALDrawNiangCardLayer:public ALBaseLayer
{
public:
    //** 查看详情的回调 *//
    typedef std::function<void (int cardIndex)> alDrawCardResultLookCardInfoCallback;
    
    
    static ALDrawNiangCardLayer* create();
    ALDrawNiangCardLayer();
    ~ALDrawNiangCardLayer();
    
    void initUI();
    
    virtual void layerWillAppear() override;
    
    virtual void layerDidAppear() override;
    
    virtual void layerDidDisappear() override;
    
    /**
     *  展示是否中奖
     */
    void showWithInfo(ALDrawCardResultInfoModel* model);
    
    /**
     *  设置查看卡片详情的回调
     */
    void setupLookCardInfoCallbakc(const alDrawCardResultLookCardInfoCallback& callback);
    
    
private:
    Node* baseNode;
    
    //** 抽中的NODE*//
    Node* doAwardNode;
    //** 未抽中的Node *//
    Node* noAwardNode;
    
    /////=============中奖
    //** 娘卡卡片 *//
    cocos2d::Sprite* cardSprite;
    //** 获得卡片文字图片 *//
    cocos2d::Sprite* getCardtxtS;
    //** 抽中卡片的名称 *//
    cocos2d::Sprite* cardNameS;
    //** 说明 *//
    cocos2d::Sprite* explaintxtS;
    //** 查看详情按妞 *//
    cocos2d::ui::Button* lookBtn;
    
    ////===============未中奖
    //** 轻豆 画面 *//
    cocos2d::Sprite* qingDouSprite;
    //** 加轻豆文字 *//
    cocos2d::Sprite* addQDtxtS;
    //** 抱歉文字 *//
    cocos2d::Sprite* sorrytxtS;
    //** 确定按妞 *//
    cocos2d::ui::Button* okBtn;
    
    
    //** 分享按钮 *//
    cocos2d::ui::Button* shareBtn;
    
    
    //** 获取到的卡片  0表示没有获取*//
    int _cardIndex;
    //** 查看卡片详情的回调 *//
    alDrawCardResultLookCardInfoCallback _lookCardInfoCallback;
    
    
};

#endif /* ALDrawNiangCardLayer_h */
