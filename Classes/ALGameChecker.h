//
//  ALGameChecker.h
//  LLKGame-mobile
//
//  Created by Allen on 2018/1/10.
//

#ifndef ALGameChecker_h
#define ALGameChecker_h

#include "cocos2d.h"


class ALGameChecker:cocos2d::Node
{
public:
    static ALGameChecker* getInstrins();
    
    
    void startNetworkChecker();
    void stopNetworkChecker();
    
};

#endif /* ALGameChecker_h */
