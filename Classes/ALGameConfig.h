//
//  ALGameConfig.h
//  LLKGame-mobile
//
//  Created by Allen on 2018/1/8.
//

#ifndef ALGameConfig_h
#define ALGameConfig_h

// 设计图尺寸
#define DF_DesignSize Director::getInstance()->getOpenGLView()->getDesignResolutionSize()
// 屏幕分辨率
#define DF_SSIZE Director::getInstance()->getVisibleSize()
#define DF_XCOUNT  7
#define DF_YCOUNT  10
#define DF_ICON_SIZE  87    // icon尺寸
#define DF_ICON_MARGIN 8   //icon 边距
#define DF_ICON_COUNT 10   // iocn 样式总数



#define DF_DESIGN_TYPE_h1280  1   // 设计图的类型 720*1280
#define DF_DESIGN_TYPE_h1560  2   // 设计图的类型 720*1560


class ALGameConfig
{
public:
    //** 设计图的类型 *//
    static int designType;
    
    
};

#endif /* ALGameConfig_h */
