//
//  StartGameScene.h
//  LLKGame
//
//  Created by Allen on 2017/12/15.
//
//

#ifndef StartGameScene_h
#define StartGameScene_h


#include "cocos2d.h"
#include "ui/CocosGUI.h"






#define DF_TAG_Sign_StartGameScene  5000

class MatchSuccessLayer;
class ALUserInfoModel;
class ALNiangCardShowLayer;
class ALNiangCardInfoLayer;
class ALGameRulesLayer;
class ALGameFriendsListLayer;

class StartGameScene:public cocos2d::Scene
{
public:
    
    
    static StartGameScene* create();
    static StartGameScene* createWithShowCardInfo(int cardIndex);
    
    StartGameScene();
    ~StartGameScene();
    
    /**
     *  重置
     */
    void resetScene();
    
    
    void setupCardInfoLayerVisible(bool isvisible,int index);
    
    
private:
    Node* startGameLayer;
    
    cocos2d::ui::Button* backBtn;
    
    cocos2d::ui::Button* rulesBtn;

    cocos2d::ui::Button* friendsBtn;
    
    cocos2d::ui::Button* soundSwitchBtn;
    
    cocos2d::ui::Button* startMatchBtn;
    cocos2d::ui::Button* endMatchBtn;
    
    cocos2d::ui::Text* onlineNumberText;

    MatchSuccessLayer* matchSuccessLayer;
    
    ALNiangCardShowLayer* cardShowLayer;
    //** 娘卡详情页 *//
    ALNiangCardInfoLayer* cardInfoLayer;
    ALGameRulesLayer* gameRulesLayer;
    
    //** 好友列表页 *//
    ALGameFriendsListLayer* friendsListLayer;
    
    
    //** 是否正在匹配 *//
    bool _isMatching;
    //** 是否有接收断网通知 *//
    bool _isDisconnectNotify;
    
    //** 进入后台的时间戳 *//
    long _appEnterBackgroundTime;
    //** 防止android锁屏时候两次进入后台 *//
    bool _isEnableOfSetupBackgroundTime;
    //** 匹配超时提示框是否已经显示 *//
    bool _timeOutAlertVisible;
    
    //** 匹配成功超时 *//
    bool _isTimeOutOfMatchSuccess;
    
    
    void baseInit();
    
    void initUI();
    
    void initScheduler();
    
    
    
    /**
     *  注册通知
     */
    void registerNotification();
    
    
    void gameMatch(bool isMatch);
    
    
    void refreshUserInfo();
    
    
    void matchSuccessInfo(ALUserInfoModel* model);
    
    
    /**
     *  进入自动匹配的游戏界面
     */
    void gotoRandomMatchGame(float delay);
    
    /**
     *  更新用户信息
     */
    void refreshUserInfoObserverFunc(Ref* ref);
    
    /**
     *  获取匹配到的用户信息
     */
    void getPlayerInfoWithMatchObserverFunc(Ref* ref);
    
    /**
     *  刷新在线人数
     */
    void refreshOnlineNumberObserverFunc(Ref* ref);
    
    /**
     *  接收到好友的约战邀请
     */
    void receiveFriendFightInvitationObserverFunc(Ref* ref);
    
    /**
     *  好友约战OK  准备进入游戏
     */
    void receiveGoToFriendFightGameObserverFunc(Ref* ref);
    
    
    /**
     *  断开连接的操作
     */
    void disConnectObserverFunc(Ref* ref);
    
    /**
     *  游戏进入后台
     */
    void appDidEnterBackgroundObserverFunc(Ref* ref);
    
    /**
     *  游戏恢复前台
     */
    void appWillEnterForegroundObserverFunc(Ref* ref);
    
    
    
};

#endif /* StartGameScene_h */
