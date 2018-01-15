//
//  ALWebImageUtil.hpp
//  LLKGame-mobile
//
//  Created by Allen on 2018/1/4.
//

#ifndef ALWebImageUtil_h
#define ALWebImageUtil_h

#include "cocos2d.h"

USING_NS_CC;

class ALWebImageUtil
{
public:
    typedef std::function<void (bool isSuccess)> ALCallBack;
    
    static void loadWebImageTexture(const char* url ,const ALCallBack& callback, const char* defultImage = "images/definedHead.png");
};

#endif /* ALWebImageUtil_h */
