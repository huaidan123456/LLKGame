//
//  ALNiangCardInfoShowLayer.h
//  LLKGame
//
//  Created by Allen on 2017/12/23.
//
//

#ifndef ALNiangCardInfoShowLayer_h
#define ALNiangCardInfoShowLayer_h

#include "ALBaseLayer.h"
#include "cocostudio/CocoStudio.h"

class ALNiangCardInfoShowLayer:public ALBaseLayer
{
public:
    typedef std::function<void (int index)> ALChangeIndexCallback;
    static ALNiangCardInfoShowLayer* create();
    
    ALNiangCardInfoShowLayer();
    ~ALNiangCardInfoShowLayer();
    
    void baseInit();
    
    virtual void layerWillAppear() override;
    
    
    virtual void layerDidDisappear() override;
    
    
    cocos2d::Vector<cocos2d::Sprite*> miCard;
    
    int centerPoint_x;//中心Y坐标
    cocos2d::Point oldPoint;
    cocos2d::Point newPoint;
    
    cocos2d::Point clickedBeginPoint;
    cocos2d::Point clickedEndPoint;
    
    // 触摸范围
    float touchStartY;  // 触摸开始的Y
    float touchEndY;    // 触摸结束的Y
    float mScale;
    
    
    bool isMove;
    float speed;
    float speedAdd;
    int currentIndex;
    bool isAutoMove;
    int indexOfMove;
    
    int a1;     // 开始渐隐的距离
    int a2;     // 边界距离
    int a3;     // 透明度从 255 到 0 需要移动的距离
    int jiange;
    int yuandian;
    int yuandianY;
    void setCardPosition(float asd);
    void updateMove();
    void panduan(cocos2d::Sprite* misss);
    void pianzhuan(cocos2d::Sprite* misss);//计算偏转
    
    void moveCard(int index);
    
    
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    
    virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    
    
    void setupChangeIndexCallback(const ALChangeIndexCallback& callback);
    
    
    // TODO: 需要设置卡片要移动到上面位置的方法
    // 比较重要的是 卡片开始的时候要移动到第一个到最中间
    void setupCardMoveToIndex(int index);
    
    void setupTouchEnable(bool isTouch);
    
    
private:
    ALChangeIndexCallback _chageIndexCallback;
    
    
    bool _isTouch;
    
};

#endif /* ALNiangCardInfoShowLayer_h */
