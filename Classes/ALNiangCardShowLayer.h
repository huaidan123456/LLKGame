//
//  ALNiangCardShowLayer.h
//  LLKGame
//
//  Created by Allen on 2017/12/21.
//
//
/**
 *
 */

#ifndef ALNiangCardShowLayer_h
#define ALNiangCardShowLayer_h

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

class ALNiangCardShowLayer:public cocos2d::Layer
{
public:
    typedef std::function<void (int index)> ALClickedCallback;
    
    static ALNiangCardShowLayer* create();
    
    ALNiangCardShowLayer();
    ~ALNiangCardShowLayer();
    
    void baseInit();
    
    
    
    
    cocos2d::Vector<cocos2d::Sprite*> miCard;
    
    int centerPoint_x;//中心Y坐标
    cocos2d::Point oldPoint;
    cocos2d::Point newPoint;
    
    cocos2d::Point clickedBeginPoint;
    cocos2d::Point clickedEndPoint;
    
    // 触摸范围
    float touchStartY;  // 触摸开始的Y
    float touchEndY;    // 触摸结束的Y
    
    bool isPlayEffic;   // 是否播放音效
    bool isMove;
    float speed;
    float speedAdd;
    
    int _currentIndex;
    
    int a1;     // 开始渐隐的距离
    int a2;     // 边界距离
    float mScale;
    int jiange;
    int yuandian;
    int yuandianY;
    void setCardPosition(float asd);
    void initCardPosition();   // 初始化卡片位置
    void updateMove();
    void panduan(cocos2d::Sprite* misss);
    void pianzhuan(cocos2d::Sprite* misss);//计算偏转
    
    
    
    void setupClickedCallback(const ALClickedCallback& callback);
    
    
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);

    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);

    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);

    virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    
    
private:
    
    ALClickedCallback _clickedCallback;
    
};

#endif /* ALNiangCardShowLayer_h */
