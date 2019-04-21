//
//  ALNiangCardInfoLayer.h
//  LLKGame
//
//  Created by Allen on 2017/12/23.
//
//

#ifndef ALNiangCardInfoLayer_h
#define ALNiangCardInfoLayer_h

#include "ALBaseLayer.h"
#include "cocostudio/CocoStudio.h"


class ALNiangCardInfoShowLayer;
class ALNiangCardInfoLayer:public ALBaseLayer
{
public:
    static ALNiangCardInfoLayer* create();
    ALNiangCardInfoLayer();
    ~ALNiangCardInfoLayer();
    
    /**
     *  注册通知
     */
    void registerNotification();
    void initUI();
    
    
    virtual void layerWillAppear() override;
    
    virtual void layerDidAppear() override;
    
    virtual void layerDidDisappear() override;
    
    /**
     *  设置卡片的选中
     *  index 卡片数组中的index
     *  isClicked 是否是点击小卡（点击小卡 true 滑动是false）
     */
    void setupCardSelectedIndex(int index,bool isClicked);

    
private:
    Node* baseNode;
    cocos2d::ui::ScrollView* scrollview;
    cocos2d::Sprite* cardName;
    
    ALNiangCardInfoShowLayer* cardLayer;
    
    
    
    //** 上面大卡片的 *//
//    cocos2d::Vector<cocos2d::Sprite*> _cardVector;
    //** 下面小卡片 *//
    cocos2d::Vector<cocos2d::ui::Button*> _cardBtnVector;
    
    
    //** 当前显示的index *//
    int _currentIndex;
    
    
    /**
     *  刷新娘卡是否拥有
     */
    void refreshNiangCard();
    /**
     *  刷新娘卡
     */
    void refreshNiangCardObserverFunc(Ref* ref);
    
    
    void adjustScrollviewContentOffY(int index);
};

#endif /* ALNiangCardInfoLayer_h */
