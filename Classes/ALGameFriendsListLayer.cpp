//
//  ALGameFriendsListLayer.cpp
//  LLKGame-mobile
//
//  Created by Allen on 2018/1/11.
//

#include "ALGameFriendsListLayer.h"
#include "ALGameFriendInfoModel.h"
#include "cocostudio/CocoStudio.h"
#include "ALMusicUtil.h"
#include "ALUserData.h"
#include "ALPlayerData.h"
#include "ALNotificationNameDefine.h"
#include "ALGameInviteeAlertLayer.h"
#include "ALGameInviterAlertLayer.h"
#include "ALNetControl.h"


USING_NS_CC;
using namespace cocos2d::ui;

#define DF_ALGFLL_SCHEDULE_NAME_updateFriends "DF_ALGFLL_SCHEDULE_NAME_updateFriends"


ALGameFriendsListLayer* ALGameFriendsListLayer::create()
{
    ALGameFriendsListLayer* layer = new (std::nothrow)ALGameFriendsListLayer();
    if (layer && layer->init()) {
        layer->baseInit();
        layer->autorelease();
        return layer;
    }else{
        CC_SAFE_DELETE(layer);
        return nullptr;
    }
}

ALGameFriendsListLayer::ALGameFriendsListLayer()
{
    _isRefreshFriendList = false;

//    for (int i = 1; i < 12; ++i) {
//        auto model = ALGameFriendInfoModel::create();
//        model->setUid(i);
//        model->setLevel(i);
//        int state = CCRANDOM_0_1()*3;
//        model->setOnlineState(state);
//        model->setName(StringUtils::format("连连看A-%d",i).c_str());
//        ALUserData::gameFriends.insert(model->getUid(),model);
//    }
}


ALGameFriendsListLayer::~ALGameFriendsListLayer()
{
    CCLOG("~ALGameFriendsListLayer");
    unscheduleAllCallbacks();
    NotificationCenter::getInstance()->removeAllObservers(this);
}


void ALGameFriendsListLayer::baseInit()
{
    initYueCallback();
    initUI();
    registerNotification();
}


void ALGameFriendsListLayer::registerNotification()
{
    NotificationCenter::getInstance()->removeAllObservers(this);
    
    //更新好友状态
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ALGameFriendsListLayer::refreshGameFriendListObserverFunc), NND_RefreshGameFriendList, NULL);
    
    // 接收到对方拒绝约战
     NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ALGameFriendsListLayer::receiveFriendRefuseFightObserverFunc), NND_RefuseInviteFlageGame, NULL);
}


void ALGameFriendsListLayer::initUI()
{
    baseNode = CSLoader::createNode("csbFile/ALFriendsListLayer.csb");
    noFriendSign = (Sprite*)baseNode->getChildByName("s_noFriendSign");
    backBtn = (Button*)baseNode->getChildByName("btn_back");
    backBtn->addClickEventListener([=](Ref* btn){
        ALMusicUtil::getInstrins()->playEffic(ALMusicUtil::GameEffic::BtnClickEffic);
        this->setVisible(false);
    });
    
    listView = (ListView*)baseNode->getChildByName("listView");
    listView->setScrollBarOpacity(0);
    this->addChild(baseNode);
    
    inviterAlertLayer = ALGameInviterAlertLayer::create();
    inviterAlertLayer->setupDisappearCallback([=]{
        ALPlayerData::isResponseInviterNotification = true;
    });
    inviterAlertLayer->setVisible(false);
    this->addChild(inviterAlertLayer,100);
    
}


void ALGameFriendsListLayer::initYueCallback()
{
    _yueCallback = [=](int friendId){
        ALNetControl::requestFightWithFriend(friendId);
        CCLOG("friendid  = %d",friendId);
        inviterAlertLayer->showAlertWithUserInfo(ALUserData::gameFriends.at(friendId));
        ALPlayerData::isResponseInviterNotification = false;
    };
}


void ALGameFriendsListLayer::layerWillAppear()
{
    unschedule(DF_ALGFLL_SCHEDULE_NAME_updateFriends);
//    if (_isRefreshFriendList) {
//        refreshFriendList();
//    }
    if (ALUserData::isRefreshFriendList) {
        refreshFriendList();
        ALUserData::isRefreshFriendList = false;
    }
    
    schedule([=](float ft){
        if (ALUserData::isRefreshFriendList) {
            refreshFriendList();
            ALUserData::isRefreshFriendList = false;
        }
        
    }, 2.f, DF_ALGFLL_SCHEDULE_NAME_updateFriends);
    
}

void ALGameFriendsListLayer::layerDidDisappear()
{
    unschedule(DF_ALGFLL_SCHEDULE_NAME_updateFriends);
}



void ALGameFriendsListLayer::refreshFriendList()
{
    _isRefreshFriendList = false;
    struct alFriednsSort {
        // true 代表 t1 在 t2前面
        bool operator()(const std::pair<int, ALGameFriendInfoModel*>&it1,std::pair<int, ALGameFriendInfoModel*>&it2){
            if (it1.second->getOnlineState() == it2.second->getOnlineState()) {
                return it1.second->getLevel() > it2.second->getLevel();
            }else{
                if (it1.second->getOnlineState() == ALGameFriendInfoModel::ONLINE_STATE::OFFLINE) {
                    return false;
                }else if (it2.second->getOnlineState() == ALGameFriendInfoModel::ONLINE_STATE::OFFLINE){
                    return true;
                }else{
                    return it1.second->getOnlineState() < it2.second->getOnlineState();
                }
            }
        }
    };
    // 将map 变成Vector 并且排序
    std::vector<std::pair<int, ALGameFriendInfoModel*>>tempVector(ALUserData::gameFriends.begin(),ALUserData::gameFriends.end());
    _friendVector.assign(tempVector.begin(), tempVector.end());
    if (_friendVector.size() == 0) {
        listView->removeAllItems();
        _cellVector.clear();
        
        // 显示暂无好友
        noFriendSign->setVisible(true);
    }else{
        noFriendSign->setVisible(false);
        // 排序
        std::sort(_friendVector.begin(), _friendVector.end(), alFriednsSort());
        if (_friendVector.size() >= _cellVector.size()) {
            // 好友增加 或者 不变
            for (int i = 0; i < _friendVector.size(); i++) {
                if (i < _cellVector.size()) {
                    _cellVector.at(i)->setupCellWithModel(_friendVector.at(i).second);
                }else{
                    auto cell = ALGameFriendCellNode::createWithModel(_friendVector.at(i).second);
                    cell->setupYueBtnClickedCallback(_yueCallback);
                    listView->pushBackCustomItem(cell);
                    _cellVector.pushBack(cell);
                }
            }
        }else{
            // 好友减少
            for (int i = 0; i < _friendVector.size();i++) {
                if (i < _friendVector.size()) {
                    _cellVector.at(i)->setupCellWithModel(_friendVector.at(i).second);
                }else{
                    listView->removeLastItem();
                    _cellVector.popBack();
                }
            }
        }
    }
}



/**
 *  更新好友状态
 */
void ALGameFriendsListLayer::refreshGameFriendListObserverFunc(Ref* ref)
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([&,this]()->void{
//        if (this->isVisible()) {
//            this->refreshFriendList();
//        }else{
//            _isRefreshFriendList = true;
//        }

        this->refreshFriendList();
    });
    
}


void ALGameFriendsListLayer::hideInviterAlertLayerWithState(bool isReceive)
{
    inviterAlertLayer->setupSignState((isReceive ? ALGameInviterAlertLayer::INVITE_STATE::AGREE_SIGN : ALGameInviterAlertLayer::INVITE_STATE::REFUSE_SIGN));
    inviterAlertLayer->hideAlertWithDelay(0.3f);
}


/**
 *  接收到对方拒绝约战
 */
void ALGameFriendsListLayer::receiveFriendRefuseFightObserverFunc(Ref* ref)
{
    String* uidStr = (String*)ref;
    int uid = uidStr->intValue();
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([&,uid,this]()->void{
        if (inviterAlertLayer->isVisible() && uid == inviterAlertLayer->getFriendUid())
        {
            inviterAlertLayer->setupSignState(ALGameInviterAlertLayer::INVITE_STATE::REFUSE_SIGN);
            inviterAlertLayer->hideAlertWithDelay(1.f);
        }
    });
}
