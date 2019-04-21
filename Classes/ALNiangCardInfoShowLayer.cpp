//
//  ALNiangCardInfoShowLayer.cpp
//  LLKGame
//
//  Created by Allen on 2017/12/23.
//
//

#include "ALNiangCardInfoShowLayer.h"
#include "ALGameConfig.h"


USING_NS_CC;
using namespace cocos2d::ui;



ALNiangCardInfoShowLayer* ALNiangCardInfoShowLayer::create()
{
    ALNiangCardInfoShowLayer* layer = new (std::nothrow)ALNiangCardInfoShowLayer();
    if (layer && layer->init()) {
        layer->baseInit();
        layer->autorelease();
        return layer;
    }else{
        CC_SAFE_DELETE(layer);
        return nullptr;
    }
}


ALNiangCardInfoShowLayer::ALNiangCardInfoShowLayer()
{
    _isTouch = true;
    oldPoint = Point(0, 0);
    newPoint = Point(0, 0);
    isMove = false;
    isAutoMove = false;
    speed = 0;
    speedAdd = 2;
    jiange = 120;
    centerPoint_x = DF_DesignSize.width/2;
    yuandian = centerPoint_x-4*jiange;//237.5
    yuandianY = ALGameConfig::designType == DF_DESIGN_TYPE_h1280 ? 680 : 830;
    
    
    a1 = 80;
    a3 = jiange;
    mScale = 0.65;
    touchStartY =  yuandianY-20;
    touchEndY = yuandianY+20 + mScale*640;
    
    clickedBeginPoint = Point(0,0);
    clickedEndPoint = Point(0,0);
}


ALNiangCardInfoShowLayer::~ALNiangCardInfoShowLayer()
{
    unscheduleAllCallbacks();
}



void ALNiangCardInfoShowLayer::baseInit()
{
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    
    miCard.pushBack(Sprite::create("images/sg_niang_card_1.png"));
    miCard.pushBack(Sprite::create("images/sg_niang_card_2.png"));
    miCard.pushBack(Sprite::create("images/sg_niang_card_3.png"));
    miCard.pushBack(Sprite::create("images/sg_niang_card_4.png"));
    miCard.pushBack(Sprite::create("images/sg_niang_card_5.png"));
    miCard.pushBack(Sprite::create("images/sg_niang_card_6.png"));
    miCard.pushBack(Sprite::create("images/sg_niang_card_7.png"));
    miCard.pushBack(Sprite::create("images/sg_niang_card_8.png"));
    miCard.pushBack(Sprite::create("images/sg_niang_card_9.png"));
    miCard.pushBack(Sprite::create("images/sg_niang_card_10.png"));
    
    
    for (int i = 0; i < miCard.size(); i++)
    {
        Sprite* miS = miCard.at(i);
        miS->setAnchorPoint(Vec2(0.5,0));
        miS->setScale(mScale);
        this->addChild(miS);
        miS->setPosition(Vec2(yuandian + (i * jiange), yuandianY));

        if (i == miCard.size() - 1)
        {
            a2 = miCard.at(i)->getPositionX()- centerPoint_x;
        }
    }
    
    updateMove();
    
    
    
    
    
    //    //1.注册监听事件对象
    //    auto listener = EventListenerTouchAllAtOnce::create();
    //    //多点触摸
    //    //2.定义监听对象的回调方法
    //    listener->onTouchesBegan = CC_CALLBACK_2(ALNiangCardInfoShowLayer::onTouchesBegan, this);
    //    listener->onTouchesMoved = CC_CALLBACK_2(ALNiangCardInfoShowLayer::onTouchesMoved, this);
    //    listener->onTouchesEnded = CC_CALLBACK_2(ALNiangCardInfoShowLayer::onTouchesEnded, this);
    //    listener->onTouchesCancelled = CC_CALLBACK_2(ALNiangCardInfoShowLayer::onTouchesCancelled, this);
    //    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    //1.注册监听事件对象
    auto listener = EventListenerTouchOneByOne::create();
    //2.定义监听对象的回调方法
    listener->onTouchBegan = CC_CALLBACK_2(ALNiangCardInfoShowLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(ALNiangCardInfoShowLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(ALNiangCardInfoShowLayer::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(ALNiangCardInfoShowLayer::onTouchCancelled, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    
    
    
    // 启动滑动事件 定时器
    schedule([&,this](float dt)->void
             {
                 if (isMove)
                 {
                     this->updateMove();
                 }
                 
             }, 0.02f, "updateCard");
    
}

void ALNiangCardInfoShowLayer::layerWillAppear()
{
    
}


void ALNiangCardInfoShowLayer::layerDidDisappear()
{
    
}

void ALNiangCardInfoShowLayer::updateMove()
{
 
    int showIndex = currentIndex;
    float maxZOrder = 0;
    for (int i = 0; i < miCard.size(); i++)
    {
        auto mic = miCard.at(i);
        mic->setPosition(Vec2(mic->getPositionX()+speed, mic->getPositionY()));

        if (speed > 0)
        {
            if (mic->getPositionX() > (centerPoint_x + a2))
            {
                if (i + 1 < miCard.size())
                {
                    mic->setPositionX(miCard.at(i + 1)->getPositionX() - jiange);
                }
                else
                {
                    mic->setPositionX(miCard.at(0)->getPositionX() - jiange);
                }
                
            }
        }
        else if (speed < 0)
        {
            if (mic->getPositionX() < (centerPoint_x - a2))
            {
                if (i - 1 >= 0)
                {
                    mic->setPositionX(miCard.at(i - 1)->getPositionX() + jiange);
                }
                else
                {
                    mic->setPositionX(miCard.at(miCard.size() - 1)->getPositionX() + jiange);
                }
            }
        }
        if (centerPoint_x - abs(centerPoint_x - mic->getPositionX()) > maxZOrder)
        {
            maxZOrder = centerPoint_x - abs(centerPoint_x - mic->getPositionX());
            showIndex = i;
        }
        
        panduan(mic);
    }
    // 此处为手动滑动
    if (!isAutoMove) {
        if (_chageIndexCallback) {
            _chageIndexCallback(showIndex);
        }
        auto mic = miCard.at(showIndex);
        if (speed >= 0)
        {
            speed -= speedAdd;
        }
        else if (speed < 0)
        {
            speed += speedAdd;
        }
        
        if ((1.5 * (speedAdd) * speed * speed) < abs(centerPoint_x - mic->getPositionX())) {
            currentIndex = showIndex;
            if (miCard.at(showIndex)->getPositionX() > centerPoint_x)
            {
                speed = -4;
            }
            else
            {
                speed = 4;
            }
            
            indexOfMove = showIndex;
            isAutoMove = true;
            isMove = true;
        }
        
    }// 如果是自动滑动
    else{
        if ( speed > 0 && miCard.at(indexOfMove)->getPositionX() > centerPoint_x)
        {
            speed = 0;
            isAutoMove = false;
            isMove = false;
        }
        else if (speed < 0 && miCard.at(indexOfMove)->getPositionX() < centerPoint_x)
        {
            speed = 0;
            isAutoMove = false;
            isMove = false;
        }
        
    }
    
    
    
}


void ALNiangCardInfoShowLayer::panduan(Sprite* misss)
{
    if (misss->getPositionX() > centerPoint_x + a1)
    {
        float opacity = MAX(0, ((centerPoint_x + a1 + a3) - misss->getPositionX())* 255 / a3);
        misss->setOpacity(opacity);
    }
    else if (misss->getPositionX() < centerPoint_x - a1)
    {
        float opacity = MAX(0, (misss->getPositionX() - (centerPoint_x - a1 - a3))* 255 / a3);

        misss->setOpacity(opacity);
    }
    else
    {
        misss->setOpacity(255);
    }
    misss->setZOrder(centerPoint_x - abs(centerPoint_x - misss->getPositionX()));
    
    
    pianzhuan(misss);
}

void ALNiangCardInfoShowLayer::pianzhuan(Sprite* misss)
{
    
//    misss->setRotation((misss->getPositionX() - centerPoint_x) / 15);
//    misss->setPositionY(yuandianY - abs(misss->getPositionX() - centerPoint_x)/3);
}

void ALNiangCardInfoShowLayer::moveCard(int index)
{
    // 将index限定在0到9之间
    index = MAX(0, index);
    index = MIN(index, 9);
    isMove = true;
    
    if (miCard.at(index)->getPositionX() > centerPoint_x)
    {
        speed = -10;
    }
    else if (miCard.at(index)->getPositionX() < centerPoint_x)
    {
        speed = 10;
    }
    else
    {
        speed = 0;
        isMove = false;
    }
}



void ALNiangCardInfoShowLayer::setCardPosition(float asd)
{
    for (int i = 0; i < miCard.size(); i++)
    {
        auto mic = miCard.at(i);
        mic->setPosition(Vec2(mic->getPositionX() + asd, mic->getPositionY()));
        if (asd > 0)
        {
            if (mic->getPositionX() > (centerPoint_x + a2))
            {
                if (i + 1 < miCard.size())
                {
                    mic->setPositionX(miCard.at(i + 1)->getPositionX() - jiange);
                }
                else
                {
                    mic->setPositionX(miCard.at(0)->getPositionX() - jiange);
                }
                
            }
        }
        else
        {
            if (mic->getPositionX() < (centerPoint_x - a2))
            {
                if (i - 1 >= 0)
                {
                    mic->setPositionX(miCard.at(i - 1)->getPositionX() + jiange);
                }
                else
                {
                    mic->setPositionX(miCard.at(miCard.size() - 1)->getPositionX() + jiange);
                }
            }
        }
        
        panduan(mic);
    }
    
}


void ALNiangCardInfoShowLayer::setupChangeIndexCallback(const ALChangeIndexCallback& callback)
{
    _chageIndexCallback = callback;
}

void ALNiangCardInfoShowLayer::setupTouchEnable(bool isTouch)
{
    _isTouch = isTouch;
}

void ALNiangCardInfoShowLayer::setupCardMoveToIndex(int index)
{
    currentIndex = index;
    if (miCard.at(index)->getPositionX() > centerPoint_x)
    {
        speed = -20;
    }
    else
    {
        speed = 20;
    }
    
    indexOfMove = index;
    isAutoMove = true;
    isMove = true;
}



#pragma mark --touch--

bool ALNiangCardInfoShowLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    if (!_isTouch) {
        return false;
    }
    
    
    auto location = touch->getLocation();
    
    if (location.y > touchEndY || location.y < touchStartY) {
        return false;
    }
    
    oldPoint = Point(location);
    clickedBeginPoint = Point(location);
    
    return true;
}

void ALNiangCardInfoShowLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    
    CCLOG("touch move");
    auto location = touch->getLocation();
    
    newPoint = Point(location);
    int kk = newPoint.x - oldPoint.x;
    speed = newPoint.x - oldPoint.x;
    if (speed > 30)
    {
        speed = 30;
    }
    if (speed < -30)
    {
        speed = -30;
    }
    setCardPosition(speed);
    
    
    oldPoint = newPoint;
}

void ALNiangCardInfoShowLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    auto location = touch->getLocation();
    clickedEndPoint = Point(location);
    if (abs(clickedEndPoint.x - clickedBeginPoint.x) < 30) {
        CCLOG("点击啦");
    }
    
    isMove = true;
}

void ALNiangCardInfoShowLayer::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    
}
