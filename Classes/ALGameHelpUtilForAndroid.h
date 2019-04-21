//
//  ALGameHelpUtilForAndroid.h
//  LLKGame
//
//  Created by Allen on 2017/12/26.
//
//

#ifndef ALGameHelpUtilForAndroid_h
#define ALGameHelpUtilForAndroid_h

#include "cocos2d.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include "cocos2d.h"
#include <string>
using namespace std;
USING_NS_CC;

#ifdef __cplusplus
extern "C" {
#endif
    
    /**
     *  设置用户uid的相关操作
     */
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_ALGameHelpUtil_setLoginUidOfGame(JNIEnv *env,jclass clz,jint uid);
    
    /**
     *  分享成功的相关回调
     */
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_ALGameHelpUtil_gameShareSuccessCallback(JNIEnv *env,jclass clz);
    
    
#ifdef __cplusplus
}
#endif

class ALGameHelpUtilForAndroid
{
public:
    /**
     *  获得单利
     */
    static ALGameHelpUtilForAndroid* getInstance();
    
    /**
     *  游戏主界面内的分享按钮的点击相关操作
     */
    void shareOfGameCardInfo();
    
    /**
     *  游戏中 抽卡分享按钮的点击相关操作
     */
    void shareOfGameDrawCard(int cardIndex);
    
    /**
     *  退出游戏的相关操作
     */
    void exitGame();
    
};



#endif
#endif /* ALGameHelpUtilForAndroid_h */
