//
//  ALGameHelpUtilForAndroid.cpp
//  LLKGame
//
//  Created by Allen on 2017/12/26.
//
//

#include "ALGameHelpUtilForAndroid.h"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "ALPlayerData.h"
#include "ALUserData.h"
#include "ALUserInfoModel.h"
#include "ALNetControl.h"
#include <jni/JniHelper.h>
#include <jni.h>


/**
 *  设置用户uid的相关操作
 */
JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_ALGameHelpUtil_setLoginUidOfGame(JNIEnv *env,jclass clz,jint uid)
{
    if (uid > 0) {
        ALUserData::loginUid = uid;
        ALUserData::userInfo->setUid(uid);
        CCLOG("设置了登录UID = %d",ALUserData::loginUid);
        CCLOG("%d  ---  %d ",ALPlayerData::isFirstStartGameActivity,ALPlayerData::isNotifyResetScene);
        if (ALPlayerData::isFirstStartGameActivity) {
            ALPlayerData::isFirstStartGameActivity = false;
        }else{
            ALPlayerData::isNotifyResetScene = true;
        }

        
    }
}


/**
 *  分享成功的相关回调
 */
JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_ALGameHelpUtil_gameShareSuccessCallback(JNIEnv *env,jclass clz)
{
    CCLOG("分享成功");
    ALNetControl::requestGetQingDou(20);
}





static ALGameHelpUtilForAndroid* s_instance = nullptr;
ALGameHelpUtilForAndroid* ALGameHelpUtilForAndroid::getInstance()
{
    if (!s_instance) {
        s_instance = new (std::nothrow) ALGameHelpUtilForAndroid();
        CCASSERT(s_instance, "ALGameHelpUtilForAndroid new Failed,not enoug memory");
    }
    return s_instance;
}

/**
 *  游戏主界面内的分享按钮的点击相关操作
 */
void ALGameHelpUtilForAndroid::shareOfGameCardInfo()
{
    JniMethodInfo methodInfo;
    bool isHave = JniHelper::getStaticMethodInfo(methodInfo,"org/cocos2dx/cpp/ALGameHelpUtil","shareOfGameCardInfo","()V");
    if (isHave) {
        CCLOG("游戏主界面内的分享按钮的点击相关操作 方法存在");
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID,methodInfo.methodID);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
}

/**
 *  游戏中 抽卡分享按钮的点击相关操作
 */
void ALGameHelpUtilForAndroid::shareOfGameDrawCard(int cardIndex)
{
    JniMethodInfo methodInfo;
    bool isHave = JniHelper::getStaticMethodInfo(methodInfo,"org/cocos2dx/cpp/ALGameHelpUtil","shareOfGameDrawCard","(I)V");
    if (isHave) {
        CCLOG("游戏中 抽卡分享按钮的点击相关操作 方法存在");
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID,methodInfo.methodID,cardIndex);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
}

/**
 *  退出游戏的相关操作
 */
void ALGameHelpUtilForAndroid::exitGame()
{
    JniMethodInfo methodInfo;
    bool isHave = JniHelper::getStaticMethodInfo(methodInfo,"org/cocos2dx/cpp/ALGameHelpUtil","exitGame","()V");
    if (isHave) {
        CCLOG("退出游戏的相关操作 方法存在");
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID,methodInfo.methodID);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
    
}


#endif

