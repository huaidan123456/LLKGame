//
//  ALBaseLayer.h
//  LLKGame
//
//  Created by Allen on 2017/12/20.
//
//

#ifndef ALBaseLayer_h
#define ALBaseLayer_h

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

class ALBaseLayer:public cocos2d::Layer
{
public:
    ALBaseLayer();
    ~ALBaseLayer();
    
    virtual void setVisible(bool bVisible) override;
    
    virtual void layerWillAppear();
    
    virtual void layerWillDisappear();
    
    virtual void layerDidAppear();
    
    virtual void layerDidDisappear();
    
};
#endif /* ALBaseLayer_h */
