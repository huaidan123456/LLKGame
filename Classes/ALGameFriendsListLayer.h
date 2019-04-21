//
//  ALGameFriendsListLayer.h
//  LLKGame-mobile
//
//  Created by Allen on 2018/1/11.
//

#ifndef ALGameFriendsListLayer_h
#define ALGameFriendsListLayer_h

#include "ALBaseLayer.h"
#include "ALGameFriendCellNode.h"


class ALGameFriendInfoModel;
class ALGameInviterAlertLayer;
class ALGameFriendsListLayer:public ALBaseLayer
{
public:
    typedef std::function<void (int friendId)> ALYueCallback;
    
    static ALGameFriendsListLayer* create();
    
    ALGameFriendsListLayer();
    ~ALGameFriendsListLayer();
    
    void baseInit();
    
    /**
     *  注册通知
     */
    void registerNotification();
    
    void initUI();
    
    void initYueCallback();
    
    void refreshFriendList();
    
    virtual void layerWillAppear() override;
    
    virtual void layerDidDisappear() override;
    
    
    void hideInviterAlertLayerWithState(bool isReceive);
    
private:
    Node* baseNode;
    //** 没有好友的标记 *//
    cocos2d::Sprite* noFriendSign;
    //** listView *//
    cocos2d::ui::ListView* listView;
    //** 返回按钮 *//
    cocos2d::ui::Button* backBtn;
    //** 邀请好友约战的layer *//
    ALGameInviterAlertLayer* inviterAlertLayer;
    
    //** model vector *//
    std::vector<std::pair<int,ALGameFriendInfoModel*>> _friendVector;
    //** cell  vector *//
//    std::vector<ALGameFriendCellNode*> _cellVector;
    cocos2d::Vector<ALGameFriendCellNode*> _cellVector;
    
    ALYueCallback _yueCallback;
    
    
    //** 是否要刷新好友表 *//
    bool _isRefreshFriendList;
    
    
    
    
    
    
    /**
     *  更新好友状态
     */
    void refreshGameFriendListObserverFunc(Ref* ref);
    
    
    /**
     *  接收到对方拒绝约战
     */
    void receiveFriendRefuseFightObserverFunc(Ref* ref);
    
    
};

#endif /* ALGameFriendsListLayer_h */
