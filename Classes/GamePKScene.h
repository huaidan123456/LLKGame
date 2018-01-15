//
//  GamePKScene.h
//  LLKGame
//
//  Created by Allen on 2017/12/15.
//
//

#ifndef GamePKScene_h
#define GamePKScene_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "IconSprite.h"
#include "ALGameConfig.h"


class ALImageView;
class ReadyGameLayer;
class ALGameResultInfoModel;
class GameOverLayer;
class ALDrawNiangCardLayer;

class GamePKScene:public cocos2d::Scene
{
public:
    static GamePKScene* createWithIconCount(int count);
    
    GamePKScene(int iconCount = 6);
    ~GamePKScene();
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    
    
    
private:
    Node* gameLayer;
    
    cocos2d::ui::Button* backBtn;
    
    cocos2d::Sprite* progressLeft;
    cocos2d::Sprite* progressRight;
    
    Node* leftHeadNode;
    ALImageView* leftHeadImg;
    
    Node* rightHeadNode;
    ALImageView* rightHeadImg;

    cocos2d::ui::Text* leftNameText;
    cocos2d::ui::Text* rightNameText;
    
    cocos2d::ui::Text* leftLevelText;
    cocos2d::ui::Text* rightLevelText;
    
    //** 准备界面 *//
    ReadyGameLayer* readyLayer;
    //** 游戏结束后的结算界面 *//
    GameOverLayer* gameOverLayer;
    //** 抽到卡片的界面 *//
    ALDrawNiangCardLayer* drawCardLayer;
    
    //** 第一个icon的 x坐标 *//
    float df_startPointX;
    float df_startPointY;
    
    //** 横的icon个数 *//
    const int _xCount = DF_XCOUNT;
    //** 纵向icon个数 *//
    const int _yCount = DF_YCOUNT;
    //** 定义显示icon的个数 *//
    const int _iconCount;
    
    //** 左边清除的次数 *//
    int _leftClearIconCount = 0;
    //** 右边清除的次数 *//
    int _rightClearIconCount = 0;
    
    ///** 是否可以开始游戏 *//
    bool _isStartGame = false;
    //** 游戏是否结束 *//
    bool _gameOver = false;
    //** 是否是暂停状态 *//
    bool _isPause = false;
    
    //** 进入后台的时间戳 *//
    long _appEnterBackgroundTime;
    //** 是否关闭断网通知 *//
    bool _closeDisconnectNotfication;
    
    //** dataMap 主要是用于存储 iconType *//
    int _dataMap[DF_XCOUNT][DF_YCOUNT];
    //** 被选中的icon坐标 *//
    std::vector<cocos2d::Vec2> _selectedVector;
    //** 自动选择的icon坐标 *//
    std::vector<cocos2d::Vec2> _autoLinkVector;
    //** 需要连线的pointVector *//
    std::vector<cocos2d::Vec2> _pathVector;
    
    
    cocos2d::Vector<IconSprite*> _iconVector;
    
    //** 游戏结果 *//
    ALGameResultInfoModel* _gameResultInfo;
    
    
    
    void baseInit();
    
    /**
     *  注册通知
     */
    void registerNotification();
    /**
     *
     */
    void initScheduler();
    
    void initUI();
    /**
     *  初始化map
     */
    void initDataMap();
    
    /**
     *  绘制地图
     */
    void drawMap();
    
    /**
     * 改变data位置
     */
    void changeDataMap();
    
    
    /**
     *  刷新iconMap
     */
    void refreshIconMap();
    
    /**
     *  添加被选中的坐标
     */
    void addSelectedMapPosit(int x,int y);
    
    /**
     *  判断是否可以连接
     */
    bool isLink(cocos2d::Vec2 v1, cocos2d::Vec2 v2);
    
    /**
     *  判断两点的直线连接
     */
    bool isLinkStraight(cocos2d::Vec2 v1, cocos2d::Vec2 v2);
    
    /**
     *  x轴方向的扩展
     */
    void extendPointsX(cocos2d::Vec2 v,std::vector<cocos2d::Vec2> &vector);
    
    /**
     *  y轴方向的扩展
     */
    void extendPointsY(cocos2d::Vec2 v,std::vector<cocos2d::Vec2> &vector);
    
    /**
     *  判断是否是死局
     */
    bool isDeadStatus();
    
    /**
     *  画连线
     */
    void drawLine();
    
    /**
     *  消除icon方法
     */
    void clearIcon(cocos2d::Vec2 v1, cocos2d::Vec2 v2, cocos2d::DrawNode* drawNode);
    
    /**
     *  自动消除
     */
    void autoClear();
    
    /**
     *  检测是否出现死局，如果出现显示提示，并调整位置
     */
    void checkMapStatus();
    
    /**
     *  更新进度条
     */
    void updateProgress();

    /**
     *  准备开始界面
     */
    void readyGame();
    
    
    /**
     *   开始游戏
     */
    void startGame();
    
    /**
     *  暂停游戏
     */
    void pauseGame();
    
    /**
     *  恢复游戏
     */
    void resumeGame();
    
    /**
     *  重新开始
     */
    void restartGame();
    
    
    
    /**
     *  游戏结束
     */
    void GameOver();
    
    /**
     *  展示游戏结果
     */
    void showGameResult();
    
    
    
    /**
     *  将数组坐标转化成屏幕坐标
     */
    cocos2d::Vec2 indexToScenePosition(int x,int y);
    
    /**
     *  将屏幕坐标转化成数组坐标
     */
    cocos2d::Vec2 scenePositionToIndex(float px,float py);
    
    /**
     *  将数组坐标转化成需要画线的坐标
     */
    cocos2d::Vec2 indexToDrawPosition(int x,int y);
    
    /**
     *  将map数组中的x,y转化成vector中的index值
     */
    int indexToVectorIndex(int x, int y);
    
    
    
#pragma mark --监听--
    /**
     *  游戏结果的监听回调
     */
    void getGameResultObserverFunc(Ref* ref);
    
    /**
     *  抽奖结果的监听回调
     */
    void getDrawNiangCardResultObserverFunc(Ref* ref);
    
    /**
     *  刷新用户轻豆数量
     */
    void refreshUserQingDouCountObserverFunc(Ref* ref);
    
    /**
     *  断开连接的操作
     */
    void disConnectObserverFunc(Ref* ref);
    
    /**
     *  游戏进入后台
     */
    void appDidEnterBackgroundObserverFunc(Ref* ref);
    
    /**
     *  游戏恢复到前台
     */
    void appWillEnterForegroundObserverFunc(Ref* ref);
};

#endif /* GamePKScene_h */
