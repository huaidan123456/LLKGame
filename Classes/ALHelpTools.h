//
//  ALHelpTools.h
//  LLKGame-mobile
//
//  Created by Allen on 2018/1/8.
//

#ifndef ALHelpTools_h
#define ALHelpTools_h

#include <stdio.h>
#include "cocos2d.h"
class ALHelpTools
{
public:
    /**
     *  获取当前的时间戳 (以 秒 为单位)
     */
    static long getCurrentTime();
    
    /**
     *  设置Node的灰度
     */
    static void setupNodeGray(cocos2d::Node* node);
    
    /**
     *  移除node的灰色
     */
    static void removeNodeGray(cocos2d::Node* node);
};

#endif /* ALHelpTools_h */
