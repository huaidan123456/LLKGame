//
//  ALToastUtil.cpp
//  AllenLRS
//
//  Created by Allen on 2017/11/29.
//
//

#include "ALToastUtil.h"
#include "ALGameConfig.h"



/**
 *  显示吐司
 */
void ALToastUtil::makeToast(Node* parentNode,const char* imagePath)
{
    makeToast(parentNode, imagePath, ToastPosition::TCenter);
}

/**
 *  显示吐司  位置
 */
void ALToastUtil::makeToast(Node* parentNode,const char* imagePath,int position)
{
    auto sToast = Sprite::create(imagePath);
    if (NULL != sToast) {
        sToast->setAnchorPoint(Vec2(0.5, 0.5));
        float mY = DF_DesignSize.height/2;
        switch (position) {
            case TCenter:{
                mY = DF_DesignSize.height/2;
                break;
            }
            case TBottom:{
                mY = sToast->getContentSize().height/2 + 50;
                break;
            }
            case TTop:{
                mY = DF_DesignSize.height -(sToast->getContentSize().height/2 + 50);
                break;
            }
            default:
                break;
        }
        
        sToast->setPosition(DF_DesignSize.width/2, mY);
        sToast->setOpacity(0);
        parentNode->addChild(sToast,9999);
        // 开始的回调动画
        CallFunc* startCallback = CallFunc::create([=]{
        });
        CallFunc* endCallback = CallFunc::create([&,sToast]{
            sToast->removeFromParent();
        });
        sToast->runAction(Sequence::create(startCallback,
                                           FadeIn::create(0.5),
                                           DelayTime::create(2),
                                           FadeOut::create(1),
                                           endCallback,
                                           NULL));
    }
}

/**
 * 显示吐司  自己设定位置
 */
void ALToastUtil::makeToastWithBottom(Node* parentNode,const char* imagePath,float bottom)
{
    auto sToast = Sprite::create(imagePath);
    if (NULL != sToast) {
        sToast->setAnchorPoint(Vec2(0.5, 0.5));
        sToast->setPosition(DF_DesignSize.width/2, sToast->getContentSize().height/2 + bottom);
        sToast->setOpacity(0);
        parentNode->addChild(sToast,9999);
        // 开始的回调动画
        CallFunc* startCallback = CallFunc::create([=]{
        });
        CallFunc* endCallback = CallFunc::create([&,sToast]{
            sToast->removeFromParent();
        });
        sToast->runAction(Sequence::create(startCallback,
                                           FadeIn::create(0.5),
                                           DelayTime::create(2),
                                           FadeOut::create(1),
                                           endCallback,
                                           NULL));
    }
}
