//
//  ALGameRulesLayer.h
//  LLKGame
//
//  Created by Allen on 2017/12/25.
//
//

#ifndef ALGameRulesLayer_h
#define ALGameRulesLayer_h

#include "ALBaseLayer.h"
#include "cocostudio/CocoStudio.h"

class ALGameRulesLayer:public ALBaseLayer
{
public:
    static ALGameRulesLayer* create();
    
    ALGameRulesLayer();
    ~ALGameRulesLayer();
    
    void initUI();
    
    
private:
    Node* baseNode;
};

#endif /* ALGameRulesLayer_h */
