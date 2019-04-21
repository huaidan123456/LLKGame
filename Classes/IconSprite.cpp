//
//  IconSprite.cpp
//  LLKGame
//
//  Created by Allen on 2017/12/12.
//
//

#include "IconSprite.h"
#include "ui/CocosGUI.h"
#include "ALMusicUtil.h"

USING_NS_CC;

IconSprite* IconSprite::createWithType(int type)
{
    IconSprite * ret = new (std::nothrow) IconSprite(type);
    if (ret && ret->init())
    {
        ret->initUI();
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;

}


IconSprite::IconSprite(int type):_iconType(type)
{
    _isSelected = false;
    _isDestroy = false;
}


IconSprite::~IconSprite()
{
//    log("IconSprite 析构");
}

void IconSprite::changeIconTypeWithAnimation(int type)
{
    setupSelected(false);
    
    CallFunc* scaleCompleteCallback = CallFunc::create([&,type,this]{
        this->setupIconType(type);
    });
    _boomSprite->stopAllActions();
    if (_destroyFinishCallback) {
        setupIconType(IconType_0);
        _destroyFinishCallback();
        _destroyFinishCallback = nullptr;
    }
    _normalSprite->stopAllActions();
    _normalSprite->runAction(Sequence::create(ScaleTo::create(0.2,0.1),Hide::create(),scaleCompleteCallback,Show::create(),ScaleTo::create(0.2,1), NULL));
}


void IconSprite::setupIconType(int type)
{
    type = (type >= 0 && type <= 10) ? type : 0;
    if (_iconType != type) {
        _iconType = type;
        retsetIcon();
    }
}

int IconSprite::getIconType()
{
    return _iconType;
}

void IconSprite::setupSelected(bool isSelected)
{
    if (isSelected) {
        ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::SelectedEffic);
    }
    if (_isSelected != isSelected) {
        _isSelected = isSelected;
        runIconSelectionAnimation();
    }
}

bool IconSprite::getSelectionState()
{
    return _isSelected;
}


void IconSprite::destroyIcon(const IconDestroyFinishCallback& callback)
{

    _destroyFinishCallback = callback;
    float mDelay = 0.4;
    _normalSprite->setVisible(false);
    _selectedSprite->setVisible(false);
    _selectedKuang->setVisible(false);
    _destroySprite->setVisible(true);
    _boomSprite->setVisible(true);
    _boomSprite->runAction(Sequence::create(ScaleTo::create(0.1, 1.2),ScaleTo::create(0.1, 0.8),ScaleTo::create(0.1, 1.2),ScaleTo::create(0.1, 1),CallFunc::create([&,this]{
        setupIconType(IconType_0);
        if (_destroyFinishCallback) {
            _destroyFinishCallback();
            _destroyFinishCallback = nullptr;
        }
    }) ,NULL));
}



#pragma mark -- 私有方法 --
void IconSprite::initUI()
{
//    _bgIconSprite = Sprite::create("images/icon_bottom_bg_k.png");
//    _bgIconSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    _bgIconSprite->setPosition(Vec2::ZERO);
//    this->addChild(_bgIconSprite,1);
    
    _selectedKuang = Sprite::create("images/icon_selected_kuang.png");
    _selectedKuang->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _selectedKuang->setPosition(Vec2::ZERO);
    _selectedKuang->setVisible(false);
    this->addChild(_selectedKuang,2);
    
    _boomSprite = Sprite::create("images/icon_boom_animationBg.png");
    _boomSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _boomSprite->setPosition(Vec2::ZERO);
    _boomSprite->setVisible(false);
    this->addChild(_boomSprite,3);
    
    _normalSprite = Sprite::create();
    _normalSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _normalSprite->setPosition(Vec2::ZERO);
    this->addChild(_normalSprite,4);
    
    
    _selectedSprite = Sprite::create();
    _selectedSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _selectedSprite->setPosition(Vec2::ZERO);
    _selectedSprite->setVisible(false);
    this->addChild(_selectedSprite,5);
    

    _destroySprite = Sprite::create();
    _destroySprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _destroySprite->setPosition(Vec2::ZERO);
    _destroySprite->setVisible(false);
    this->addChild(_destroySprite,6);
    
    
    
    retsetIcon();

}


void IconSprite::retsetIcon()
{
    switch (_iconType) {
        case IconType_0:
        {
            _normalSprite->setTexture(nullptr);
            _normalSprite->setTextureRect(Rect::ZERO);
            
            _selectedSprite->setTexture(nullptr);
            _selectedSprite->setTextureRect(Rect::ZERO);
            
            _destroySprite->setTexture(nullptr);
            _destroySprite->setTextureRect(Rect::ZERO);
            
            break;
        }
        case IconType_1:
        case IconType_2:
        case IconType_3:
        case IconType_4:
        case IconType_5:
        case IconType_6:
        case IconType_7:
        case IconType_8:
        case IconType_9:
        case IconType_10:
        {
            _normalSprite->setTexture(StringUtils::format("images/icon_normal_%d.png",_iconType));
            
            _selectedSprite->setTexture(StringUtils::format("images/icon_selected_%d.png",_iconType));
            
            _destroySprite->setTexture(StringUtils::format("images/icon_destroy_%d.png",_iconType));
            break;
        }
  
        default:
            break;
    }
    
    _normalSprite->setVisible(true);
    _selectedSprite->setVisible(false);
    _selectedKuang->setVisible(false);
    _destroySprite->setVisible(false);
    _boomSprite->setVisible(false);
    
    setupSelected(false);
}


void IconSprite::runIconSelectionAnimation()
{
    if (_isSelected) {
        log("执行被选中动画");
        _normalSprite->setVisible(false);
        _selectedSprite->setVisible(true);
        _selectedKuang->setVisible(true);
    }else{
        log("执行取消选中的动画");
        _normalSprite->setVisible(true);
        _selectedSprite->setVisible(false);
        _selectedKuang->setVisible(false);
    }
}
