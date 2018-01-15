//
//  ALNiangCardShowLayer.cpp
//  LLKGame
//
//  Created by Allen on 2017/12/21.
//
//

#include "ALNiangCardShowLayer.h"
#include "ALGameConfig.h"
#include "ALMusicUtil.h"


USING_NS_CC;
using namespace cocos2d::ui;

ALNiangCardShowLayer* ALNiangCardShowLayer::create()
{
    ALNiangCardShowLayer* layer = new (std::nothrow)ALNiangCardShowLayer();
    if (layer && layer->init()) {
        layer->baseInit();
        layer->autorelease();
        return layer;
    }else{
        CC_SAFE_DELETE(layer);
        return nullptr;
    }
}


ALNiangCardShowLayer::ALNiangCardShowLayer()
{
    _currentIndex = 4;
    oldPoint = Point(0, 0);
    newPoint = Point(0, 0);
    isPlayEffic = false;
    isMove = false;
    speed = 0;
    speedAdd = 2;
    jiange = 50;
    mScale = 0.5;  //1 - 640 / DF_DesignSize.height;
    yuandian = DF_DesignSize.width/2 - jiange * 4;
    yuandianY = DF_DesignSize.height * 0.47;
    touchStartY =  yuandianY-20;
    touchEndY = yuandianY+20 + mScale*640;
    
    clickedBeginPoint = Point(0,0);
    clickedEndPoint = Point(0,0);
}


ALNiangCardShowLayer::~ALNiangCardShowLayer()
{
    
}



void ALNiangCardShowLayer::baseInit()
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
        
        if (i == 4)
        {
            centerPoint_x = yuandian + (i * jiange);
        }
        if (i == miCard.size() - 2)
        {
            a1 = miCard.at(i)->getPositionX()- centerPoint_x;
        }
        if (i == miCard.size() - 1)
        {
            a2 = miCard.at(i)->getPositionX()- centerPoint_x;
            a1 = a2 - 100;
        }
    }
    
    initCardPosition();
    
    

//    //1.注册监听事件对象
//    auto listener = EventListenerTouchAllAtOnce::create();
//    //多点触摸
//    //2.定义监听对象的回调方法
//    listener->onTouchesBegan = CC_CALLBACK_2(ALNiangCardShowLayer::onTouchesBegan, this);
//    listener->onTouchesMoved = CC_CALLBACK_2(ALNiangCardShowLayer::onTouchesMoved, this);
//    listener->onTouchesEnded = CC_CALLBACK_2(ALNiangCardShowLayer::onTouchesEnded, this);
//    listener->onTouchesCancelled = CC_CALLBACK_2(ALNiangCardShowLayer::onTouchesCancelled, this);
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    //1.注册监听事件对象
    auto listener = EventListenerTouchOneByOne::create();
    //2.定义监听对象的回调方法
    listener->onTouchBegan = CC_CALLBACK_2(ALNiangCardShowLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(ALNiangCardShowLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(ALNiangCardShowLayer::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(ALNiangCardShowLayer::onTouchCancelled, this);
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


void ALNiangCardShowLayer::initCardPosition()
{
    
    int showIndex = 0;
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
            _currentIndex = i;
        }
        
        panduan(mic);
    }
    if (speed > 0)
    {
        speed -= speedAdd;
        if (speed <= 0)
        {
            speed = 0;
            isMove = false;
        }
    }
    else if (speed < 0)
    {
        speed += speedAdd;
        if (speed >= 0)
        {
            speed = 0;
            isMove = false;
        }
    }
    
}

void ALNiangCardShowLayer::updateMove()
{
    
    int showIndex = 0;
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
            _currentIndex = i;
        }
        
        panduan(mic);
    }
    if (speed > 0)
    {
        speed -= speedAdd;
        if (speed <= 0)
        {
            speed = 0;
            isMove = false;
            isPlayEffic = false;
        }
    }
    else if (speed < 0)
    {
        speed += speedAdd;
        if (speed >= 0)
        {
            speed = 0;
            isMove = false;
            isPlayEffic = false;
        }
    }
    
//    if (!isPlayEffic) {
//        ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::PageEffic);
//        isPlayEffic = true;
//    }
    
}


void ALNiangCardShowLayer::panduan(Sprite* misss)
{
    if (misss->getPositionX() > centerPoint_x + a1)
    {
        if (misss->getPositionX() > centerPoint_x + a2)
        {
            misss->setOpacity(0);
        }
        else
        {
            misss->setOpacity((centerPoint_x + a2 - misss->getPositionX()) * 255 / 100);
        }
        
    }
    else if (misss->getPositionX() < centerPoint_x - a1)
    {
        if (misss->getPositionX() < centerPoint_x - a2)
        {
            misss->setOpacity(0);
        }
        else
        {
            misss->setOpacity((misss->getPositionX() - (centerPoint_x - a2)) * 255 / 100);
        }
    }
    else
    {
        misss->setOpacity(255);
    }
    misss->setZOrder(centerPoint_x - abs(centerPoint_x - misss->getPositionX()));
    
    
    pianzhuan(misss);
}

void ALNiangCardShowLayer::pianzhuan(Sprite* misss)
{
    
    misss->setRotation((misss->getPositionX() - centerPoint_x) / 15);
    misss->setPositionY(yuandianY - abs(misss->getPositionX() - centerPoint_x)/3);
}


bool ALNiangCardShowLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    auto location = touch->getLocation();
    
    if (location.y > touchEndY || location.y < touchStartY) {
        return false;
    }
    
    oldPoint = Point(location);
    clickedBeginPoint = Point(location);
    
    return true;
}

void ALNiangCardShowLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
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

void ALNiangCardShowLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    auto location = touch->getLocation();
    clickedEndPoint = Point(location);
    if (abs(clickedEndPoint.x - clickedBeginPoint.x) < 30) {
        CCLOG("点击啦");
        if (_clickedCallback) {
            _clickedCallback(_currentIndex);
        }
    }
    
    isMove = true;
}

void ALNiangCardShowLayer::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    
}



void ALNiangCardShowLayer::setCardPosition(float asd)
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


void ALNiangCardShowLayer::setupClickedCallback(const ALClickedCallback& callback)
{
    _clickedCallback = callback;
}
