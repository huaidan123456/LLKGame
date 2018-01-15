//
//  ALGameHelpUtilForIos.h
//  LLKGame
//
//  Created by Allen on 2017/12/26.
//
//

#ifndef ALGameHelpUtilForIos_h
#define ALGameHelpUtilForIos_h

#include <stdio.h>


class ALGameHelpUtilForIos
{
public:
    /**
     *  获得单利
     */
    static ALGameHelpUtilForIos* getInstance();
    
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
    
    
    /**
     *  游戏controller是否创建
     */
    bool isDoneGameController();
    
    
    
};

#endif /* ALGameHelpUtilForIos_h */
