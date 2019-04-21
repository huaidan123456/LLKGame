//
//  ALBaseLayer.cpp
//  LLKGame
//
//  Created by Allen on 2017/12/20.
//
//

#include "ALBaseLayer.h"


ALBaseLayer::ALBaseLayer()
{
    
}


ALBaseLayer::~ALBaseLayer()
{
    
}

void ALBaseLayer::layerWillAppear()
{
    
}

void ALBaseLayer::layerDidAppear()
{
    
}

void ALBaseLayer::layerWillDisappear()
{
    
}

void ALBaseLayer::layerDidDisappear()
{
    
}


#pragma mark -- setVisible 重写--
void ALBaseLayer::setVisible(bool bVisible)
{
    if (_visible == bVisible) {
        return;
    }
    if (bVisible) {
        layerWillAppear();
    }
    if (!bVisible) {
        layerWillDisappear();
    }
    Layer::setVisible(bVisible);
    
    if (bVisible) {
        layerDidAppear();
    }
    if (!bVisible) {
        layerDidDisappear();
    }
    
}
