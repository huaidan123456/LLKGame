//
//  IconSprite.h
//  LLKGame
//
//  Created by Allen on 2017/12/12.
//
//

#ifndef IconSprite_h
#define IconSprite_h
#include "cocos2d.h"

class IconSprite:public cocos2d::Node
{
public:
    typedef std::function<void()> IconDestroyFinishCallback;
    
    enum IconType
    {
        IconType_0 = 0,
        IconType_1 = 1,
        IconType_2,
        IconType_3,
        IconType_4,
        IconType_5,
        IconType_6,
        IconType_7,
        IconType_8,
        IconType_9,
        IconType_10
    };
    
    
    static IconSprite* createWithType(int type);
    
    IconSprite(int type = 0);
    ~IconSprite();
    
    void changeIconTypeWithAnimation(int type);
    
    void setupIconType(int type);
    
    int getIconType();
    
    void setupSelected(bool isSelected);
    
    bool getSelectionState();
    
    void destroyIcon(const IconDestroyFinishCallback& callback);
    
private:
    //** 背景框的Sprite *//
    cocos2d::Sprite* _bgIconSprite;
    //** 正常状态下 *//
    cocos2d::Sprite* _normalSprite;
    cocos2d::Sprite* _selectedSprite;
    cocos2d::Sprite* _selectedKuang;
    //** 摧毁状态的图片 *//
    cocos2d::Sprite* _destroySprite;
    cocos2d::Sprite* _boomSprite;
    
    
    
    //** icon类型 *//
    int _iconType;
    //** 是否是选择状态 *//
    bool _isSelected;
    //** 是否已经销毁 *//
    bool _isDestroy;
    
    
    
    /**
     *  初始化UI
     */
    void initUI();
    
    /**
     *  重置icon
     */
    void retsetIcon();
    
    /**
     *  选择动画
     */
    void runIconSelectionAnimation();

    
};

#endif /* IconSprite_h */
