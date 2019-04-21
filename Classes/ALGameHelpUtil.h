//
//  ALGameHelpUtil.h
//  LLKGame
//
//  Created by Allen on 2017/12/26.
//
//

#ifndef ALGameHelpUtil_h
#define ALGameHelpUtil_h

class ALGameHelpUtil
{
public:
    /**
     *  获得单利
     */
    static ALGameHelpUtil* getInstance();
    
    /**
     *  游戏主界面内的分享按钮的点击相关操作
     */
    void shareOfGameCardInfo();
    
    /**
     *  游戏中 抽卡分享按钮的点击相关操作
     *  卡片的index值（1-10）
     */
    void shareOfGameDrawCard(int cardIndex);
    
    /**
     *  退出游戏的相关操作
     */
    void exitGame();
};

#endif /* ALGameHelpUtil_h */
