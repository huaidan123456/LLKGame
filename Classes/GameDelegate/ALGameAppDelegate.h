//
//  ALGameAppDelegate.h
//  LLKGame
//
//  Created by Allen on 2017/12/26.
//
//

#ifndef ALGameAppDelegate_h
#define ALGameAppDelegate_h

#include "cocos2d.h"


class ALGameAppDelegate:private cocos2d::Application
{
public:
    ALGameAppDelegate();
    virtual ~ALGameAppDelegate();
    
    
    virtual void initGLContextAttrs();
    
    /**
     @brief    Implement Director and Scene init code here.
     @return true    Initialize success, app continue.
     @return false   Initialize failed, app terminate.
     */
    virtual bool applicationDidFinishLaunching();
    
    /**
     @brief  The function be called when the application enter background
     @param  the pointer of the application
     */
    virtual void applicationDidEnterBackground();
    
    /**
     @brief  The function be called when the application enter foreground
     @param  the pointer of the application
     */
    virtual void applicationWillEnterForeground();
    
};

#endif /* ALGameAppDelegate_h */
