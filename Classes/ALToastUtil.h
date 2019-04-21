//
//  ALToastUtil.h
//  AllenLRS
//
//  Created by Allen on 2017/11/29.
//
//

#ifndef ALToastUtil_h
#define ALToastUtil_h

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

class ALToastUtil
{
public:
    enum ToastPosition
    {
        TCenter = 0 , // 中间
        TBottom ,
        TTop,
    };
    
    /**
     *  显示吐司
     */
    static void makeToast(Node* parentNode,const char* imagePath);
    
    
    /**
     *  显示吐司 通过位置type
     */
    static void makeToast(Node* parentNode,const char* imagePath,int position);
    
    /**
     * 显示吐司  自己设定位置
     */
    static void makeToastWithBottom(Node* parentNode,const char* imagePath,float bottom);
    

   
};

#endif /* ALToastUtil_h */
