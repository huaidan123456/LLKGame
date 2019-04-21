//
//  ALBNSocketUitl.cpp
//  LLKGame
//
//  Created by Allen on 2017/12/18.
//
//

#include "ALBNSocketUitl.h"
#include <fcntl.h>
#include <string>
#include "ALNetDefine.h"
#include "ALUserData.h"
#include "ALPlayerData.h"
#include "ALUserInfoModel.h"
#include "ALGameResultInfoModel.h"
#include "ALDrawCardResultInfoModel.h"
#include "ALNotificationNameDefine.h"
#include "ALGameFriendInfoModel.h"



#define SOCKET_ERROR -1
#define INVALID_SOCKET -1



HSocket ALBNSocketUitl::_socketHandle = 0;
bool ALBNSocketUitl::_connectFlag = false;
bool ALBNSocketUitl::_isOpenHeadThread = false;
bool ALBNSocketUitl::_notificationSwitch = true;




/**
 *  连接服务器
 */
bool ALBNSocketUitl::connectServer(const char* m_ip,unsigned short m_port)
{
    if (_connectFlag) {
        return true;
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    
    wVersionRequested = MAKEWORD(1, 1);
    err = WSAStartup(wVersionRequested, &wsaData);
#endif
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    struct sockaddr_in sa;
    struct hostent* hp;
    
    hp = gethostbyname(m_ip);
    if (!hp)
    {
        return 0;
    }

    
    const char *addr = inet_ntoa(*(struct in_addr *)hp->h_addr_list[0]);
    struct in_addr iip;
    iip.s_addr = inet_addr(addr);
    sa.sin_family = AF_INET;// hp->h_addrtype;
    sa.sin_port = htons(m_port);
    sa.sin_addr = iip;
    _socketHandle = socket(AF_INET, SOCK_STREAM, 0);
    
    if (_socketHandle < 0)
    {
        return 0;
    }
    if (::connect(_socketHandle, (sockaddr*)&sa, sizeof(sa)) < 0)
    {
        int closeInt = 0;
#if defined(WIN32)
        closeInt = ::shutdown(_socketHandle, 2);
#else
        closeInt = ::shutdown(_socketHandle, SHUT_RDWR);
#endif
        
        CCLOG("no connect!!!");
        return 0;
    }
    CCLOG("connect  OK!!!!");
    
//    _connectFlag = true;
//    // 接收
//    std::thread sthread(&ALBNSocketUitl::threadReceiveTask);
//    sthread.detach();
//    return true;
    
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
    //连接ip
    char ip[128];
    memset(ip, 0, sizeof(ip));
    strcpy(ip, m_ip);
    
    void* svraddr = nullptr;
    int error = -1, svraddr_len;
    // int tempSocket = -1;
    bool ret = true;
    struct sockaddr_in svraddr_4;
    struct sockaddr_in6 svraddr_6;
    
    //获取网络协议
    struct addrinfo *result;
    error = getaddrinfo(ip, NULL, NULL, &result);
    const struct sockaddr *sa = result->ai_addr;
    socklen_t maxlen = 128;
    switch (sa->sa_family) {
        case AF_INET://ipv4
            if ((_socketHandle = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                perror("socket create failed");
                ret = false;
                break;
            }
            if (inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr),
                          ip, maxlen) == NULL)
            {
                perror(ip);
                ret = false;
                break;
            }
            svraddr_4.sin_family = AF_INET;
            svraddr_4.sin_addr.s_addr = inet_addr(ip);
            svraddr_4.sin_port = htons(m_port);
            svraddr_len = sizeof(svraddr_4);
            svraddr = &svraddr_4;
            break;
        case AF_INET6://ipv6
            if ((_socketHandle = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
                perror("socket create failed");
                ret = false;
                break;
            }
            inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr),
                      ip, maxlen);
            
            printf("socket created ipv6/n");
            
            bzero(&svraddr_6, sizeof(svraddr_6));
            svraddr_6.sin6_family = AF_INET6;
            svraddr_6.sin6_port = htons(m_port);
            if (inet_pton(AF_INET6, ip, &svraddr_6.sin6_addr) < 0) {
                perror(ip);
                ret = false;
                break;
            }
            svraddr_len = sizeof(svraddr_6);
            svraddr = &svraddr_6;
            break;
            
        default:
            printf("Unknown AF\ns");
            ret = false;
    }
    freeaddrinfo(result);
    if (!ret)
    {
        fprintf(stderr, "Cannot Connect the server!n");
        return 0;
    }
    int nret = ::connect(_socketHandle, (struct sockaddr*)svraddr, svraddr_len);
    if (nret<0)
    {
        CCLOG("no connect!!!");
        return 0;
    }
    
    // 设置超时时间
//    struct timeval timeout = {3,0};
//    setsockopt(_socketHandle, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
//    setsockopt(_socketHandle,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout));
    CCLOG("connect  OK!!!!");
    
//    _connectFlag = true;
//    std::thread sthread(&ALBNSocketUitl::threadReceiveTask);
//    sthread.detach();
//    return true;

#endif
    
    _connectFlag = true;
    // 接收
    std::thread sthread(&ALBNSocketUitl::threadReceiveTask);
    sthread.detach();
    // 心跳包
    if (!_isOpenHeadThread) {
        std::thread headThread(&ALBNSocketUitl::threadSendHeartbeat);
        headThread.detach();
    }
    
    return true;
}

bool ALBNSocketUitl::loginServer(const char *ip, unsigned short port, int uid)
{
    if (_connectFlag) {
        return true;
    }
    
    if (connectServer(ip, port)) {
        ALBNSocketUitl::sendString(ALND_NF_CONNECT);
        ALBNSocketUitl::sendInt(uid);
        return true;
    }
    // 发送链接失败通知
    if(_notificationSwitch){
        cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([]{
            cocos2d::NotificationCenter::getInstance()->postNotification(NND_Disconnect);
        });
    }
    
    return false;
}



/**
 *  心跳包
 */
void* ALBNSocketUitl::threadSendHeartbeat()
{
    CCLOG("开启心跳线程 ====");
    _isOpenHeadThread = true;
    while (_connectFlag) {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]{
            struct timeval tv2;
            gettimeofday(&tv2, NULL);
            float intervalTime = (float)(tv2.tv_sec - tv.tv_sec) + ((float)(tv2.tv_usec - tv.tv_usec))/1000000.f;
            if (intervalTime < 0.5f && _connectFlag) {
                ALBNSocketUitl::sendString("<-jump->");
                CCLOG("心跳包发送 ");
            }
        });
        sleep(1);
    }
    
    _isOpenHeadThread = false;
    CCLOG("关闭心跳线程 ====");
    
    return 0;
}


/**
 *  接收消息
 */
void* ALBNSocketUitl::threadReceiveTask()
{
    while (_connectFlag) {
        char* data = receiveStr();
        CCLOG("接收消息!!!!! === %s",data);
        //返回OK。则为连接成功。
        
        if (strcmp(data, "<#OK#>") == 0)//返回一个确认。说明连接上了
        {
            CCLOG("connecting!!!!!");
        }
        // 用户信息
        if (strcmp(data, ALND_NF_USER_INFO) == 0) {
            char* name = receiveStr();
            char* headUrl = receiveStr();
            int level = receiveInt();
            int currentExp = receiveInt();
            int sumExp = receiveInt();
            int qingdouCount = receiveInt();
            
            ALUserData::userInfo->setName(name);
            ALUserData::userInfo->setHeadUrl(headUrl);
            ALUserData::userInfo->setLevel(level);
            ALUserData::userInfo->setCurrentEXP(currentExp);
            ALUserData::userInfo->setSumExp(sumExp);
            ALUserData::userInfo->setQingDou(qingdouCount);
            ALUserData::qingDouCount = qingdouCount;

            CCLOG("获取用户信息");
            CCLOG("名称 =%s",name);
            CCLOG("头像地址  %s",headUrl);

            delete [] name;
            delete [] headUrl;
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([]{
                cocos2d::NotificationCenter::getInstance()->postNotification(NND_RefreshUserInfo,NULL);
            });
            
        }
        // 刷新用户信息
        else if (strcmp(data, ALND_NF_REFRESH_USER_INFO) == 0)
        {
            int level = receiveInt();
            int currentExp = receiveInt();
            int sumExp = receiveInt();
            int qingdouCount = receiveInt();
            
            ALUserData::userInfo->setLevel(level);
            ALUserData::userInfo->setCurrentEXP(currentExp);
            ALUserData::userInfo->setSumExp(sumExp);
            ALUserData::userInfo->setQingDou(qingdouCount);
            ALUserData::qingDouCount = qingdouCount;
            CCLOG("更新用户信息");
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([]{
                cocos2d::NotificationCenter::getInstance()->postNotification(NND_RefreshUserInfo,NULL);
            });
            
        }
        // 刷新用户轻豆数量
        else if (strcmp(data, ALND_NF_UPDATE_USER_QINGDOU) == 0)
        {
            int qingdouCount = receiveInt();
            ALUserData::qingDouCount = qingdouCount;
            ALUserData::userInfo->setQingDou(qingdouCount);
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([]{
                cocos2d::NotificationCenter::getInstance()->postNotification(NND_RefreshUserQingDouCount,NULL);
            });
            
        }
        // 获取自动匹配角色的信息
        else if (strcmp(data, ALND_NF_GET_RANDOM_PLAYER) == 0)
        {
            CCLOG("获取自动匹配的用户信息");
            int uid = receiveInt();
            char* name = receiveStr();
            char* headUrl = receiveStr();
            int level = receiveInt();
            bool isConcern = receiveInt();
            ALPlayerData::difficultyLevel = receiveInt();//难易程度
            CCLOG("获取到的游戏难度 %d",ALPlayerData::difficultyLevel);
            // 接收电脑AI
            for (int i = 0; i < 7; ++i) {
                ALPlayerData::systemAIDifficulty[i] = receiveInt();
            }

            ALPlayerData::opponentInfo->setUid(uid);
            ALPlayerData::opponentInfo->setName(name);
            ALPlayerData::opponentInfo->setHeadUrl(headUrl);
            ALPlayerData::opponentInfo->setLevel(level);
            ALPlayerData::opponentInfo->setConcern(isConcern);
            CCLOG("名称 =%s",name);
            CCLOG("头像地址  %s",headUrl);
            
            delete [] name;
            delete [] headUrl;
            
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([]{
                cocos2d::NotificationCenter::getInstance()->postNotification(NND_GetRandomMatchUserInfo,NULL);
            });
            
            CCLOG("返回匹配信息");
            
        }
        // 获取用户娘卡
        else if (strcmp(data, ALND_NF_REFRESH_USER_NIANGCARD) == 0)
        {
            CCLOG("刷新娘卡");
            for (int i = 0; i < 10 ; ++i) {
                ALUserData::niangCard[i] = false;
            }
            int count =  receiveInt();
            for (int i  = 0; i < count; ++i) {
                int index = receiveInt()-1;
                ALUserData::niangCard[index] = true;
            }
            CCLOG("刷新娘卡  信息接收完毕");
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([]{
                cocos2d::NotificationCenter::getInstance()->postNotification(NND_RefreshUserNiangCard,NULL);
            });
            
        }
        // 获取自动匹配的游戏结果
        else if (strcmp(data, ALND_NF_GET_STANDALONE_GAME_RESULT) == 0)
        {
            bool result = receiveInt();
            int level = receiveInt();
            int currentExp = receiveInt();
            int sumExp = receiveInt();
            int qingdouCount = receiveInt();
            int addExp = receiveInt();
            int addQingDou = receiveInt();
            
            ALUserData::userInfo->setLevel(level);
            ALUserData::userInfo->setCurrentEXP(currentExp);
            ALUserData::userInfo->setSumExp(sumExp);
            ALUserData::userInfo->setQingDou(qingdouCount);
            ALUserData::qingDouCount = qingdouCount;
            
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([result,addExp,addQingDou]{
                ALGameResultInfoModel* model = ALGameResultInfoModel::create();
                model->gameResult = result;
                model->addExp = addExp;
                model->addQingdao = addQingDou;
                model->retain();
                CCLOG("返回游戏结果  %d  %d  %d",result,addExp,addQingDou);
                cocos2d::NotificationCenter::getInstance()->postNotification(NND_GetStandAloneGameResult,model);
            });
            
        }// 接收抽奖奖励
        else if (strcmp(data, ALND_NF_DRAW_NIANING_CARD_AWARY) == 0)
        {
            CCLOG("抽卡回调");
            int isGain = receiveInt();
            int cardIndex = receiveInt();
            int awardIndex = receiveInt();
            
            
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([isGain,cardIndex,awardIndex]{
                auto model =  ALDrawCardResultInfoModel::create();
                model->isGain = isGain;
                model->cardIndex = cardIndex;
                model->awardIndex = awardIndex;
                model->retain();
                CCLOG("bnsocket抽卡详情   %d  %d  %d ",model->isGain,model->cardIndex,model->awardIndex);
                cocos2d::NotificationCenter::getInstance()->postNotification(NND_GetDrawNiangCardResult, model);
            });
            
        }
        // 请求在线人数
        else if (strcmp(data, ALND_NF_REQUEST_ONLINE_NUMBER) == 0)
        {
            ALPlayerData::onLienNumber = receiveInt();
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([]{
                cocos2d::NotificationCenter::getInstance()->postNotification(NND_RefreshOnlineNumber);
            });
            
            CCLOG("在线人数获取完毕");
        }
        // 获取好友信息列表
        else if (strcmp(data, ALND_NF_REFRESH_GAME_FRIENDS_LIST) == 0)
        {
            CCLOG("获取好友信息列表");
            int friendCount = receiveInt();
            CCLOG("好友数量 = %d",friendCount);
            for (int i = 0; i < friendCount; ++i)
            {
                int uid = receiveInt();
                char* name = receiveStr();
                char* headUrl = receiveStr();
                int level = receiveInt();
                int onlineState = receiveInt();
                
                auto model = ALGameFriendInfoModel::create();
                model->setUid(uid);
                model->setName(name);
                model->setHeadUrl(headUrl);
                model->setLevel(level);
                model->setOnlineState(onlineState);
                ALUserData::gameFriends.insert(model->getUid(), model);
                
                delete [] name;
                delete [] headUrl;
            }
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([]{
                cocos2d::NotificationCenter::getInstance()->postNotification(NND_RefreshGameFriendList);
            });
            
        }
        
        // 刷新好友在线情况
        else if (strcmp(data, ALND_NF_REFRESH_GAME_FRIEND_ONLINE) == 0)
        {
            int friendCount = receiveInt();
            CCLOG("数量 %d",friendCount);
            for (int i = 0; i < friendCount; ++i) {
                int fid = receiveInt();
                int onlineState = receiveInt();
                CCLOG("好友 %d   %d",fid,onlineState);
                auto model = ALUserData::gameFriends.at(fid);
                if (model) {
                    model->setOnlineState(onlineState);
                }
            }
            if (friendCount > 0) {
                ALUserData::isRefreshFriendList = true;
            }
            
//            cocos2d::NotificationCenter::getInstance()->postNotification(NND_RefreshGameFriendList);

        }
        //被申请约战（给被约战的客户端发送） 做弹出框提示约战(如果在大厅界面时)
        else if (strcmp(data, ALND_NF_RECEIVE_FIGHT_INVITE) == 0)
        {
            int uid = receiveInt();
            char* name = receiveStr();
            char* headUrl = receiveStr();
            int level = receiveInt();
            
            ALUserInfoModel* model = ALUserInfoModel::create();
            model->setUid(uid);
            model->setName(name);
            model->setHeadUrl(headUrl);
            model->setLevel(level);
            model->retain();

            delete [] name;
            delete [] headUrl;
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([model]{
                cocos2d::NotificationCenter::getInstance()->postNotification(NND_InviteFightGameWithFriend,model);
            });
            
        }
        //同样给申请人返回（表示他已经成功给对方发信息并且自己也要进入等待对方同意的状态）
        else if (strcmp(data, ALND_NF_RECEIVE_INVITE_SUCCESS_TO_WAIT) == 0)
        {
            
        }
        // 对方拒绝约战
        else if (strcmp(data, ALND_NF_RECEIVE_REFUSE_FIGHT_INVITE) == 0)
        {
            CCLOG("拒绝约战");
            int uid = receiveInt();
            
            
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([uid]{
                cocos2d::String* uidStr = cocos2d::String::createWithFormat("%d",uid);
                uidStr->retain();
                CCLOG("%d  拒绝约战",uid);
                cocos2d::NotificationCenter::getInstance()->postNotification(NND_RefuseInviteFlageGame,uidStr);
            });
        }
        
        //对方同意约战  //** 给双方返回游戏难度和房间号 *//
        else if (strcmp(data, ALND_NF_RECEIVE_PK_GAME_INFO) == 0)
        {
            ALPlayerData::difficultyLevel = receiveInt();
            ALPlayerData::roomIdOfPK = receiveInt();
            int uid = receiveInt();
            
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([uid]{
                CCLOG("开始对战  对方id== %d",uid);
                cocos2d::String* uidStr = cocos2d::String::createWithFormat("%d",uid);
                uidStr->retain();
                cocos2d::NotificationCenter::getInstance()->postNotification(NND_AgreeInviteFlageGame,uidStr);
            });
            
        }
        //** 双方准备完毕  游戏开始 *//
        else if (strcmp(data, ALND_NF_FIGHT_PK_START_GAME) == 0)
        {
            ALPlayerData::isReadyOfFriendFightGame = true;
        }
        //** 接收当前游戏消除情况 服务器给客户端返回双方当前分数情况（两个玩家的得分情况）*//
        else if (strcmp(data, ALND_NF_RECEIVE_FIGHT_PK_CURRENT_COUNT) == 0)
        {
            int uid1 = receiveInt();
            int count1 = receiveInt();

            int uid2 = receiveInt();
            int count2 = receiveInt();

            if (ALUserData::userInfo->getUid() == uid1) {
                ALPlayerData::leftCleanCountOfPKGame = count1;
                ALPlayerData::rightCleanCountOfPKGame = count2;
            }else{
                ALPlayerData::leftCleanCountOfPKGame = count2;
                ALPlayerData::rightCleanCountOfPKGame = count1;
            }

//            int uid = receiveInt();
//            int count = receiveInt();
//
//
//
//            if (ALUserData::userInfo->getUid() == uid) {
//                ALPlayerData::leftCleanCountOfPKGame = count;
//            }else{
//                ALPlayerData::rightCleanCountOfPKGame = count;
//            }

        }
        //** 接收到游戏结束 *//
        else if (strcmp(data, ALND_NF_RECEIVE_FIGHT_PK_GAME_OVER) == 0)
        {
            bool result = receiveInt();
            int level = receiveInt();
            int currentExp = receiveInt();
            int sumExp = receiveInt();
            int qingdouCount = receiveInt();
            int addExp = receiveInt();
            int addQingDou = receiveInt();
            
            ALUserData::userInfo->setLevel(level);
            ALUserData::userInfo->setCurrentEXP(currentExp);
            ALUserData::userInfo->setSumExp(sumExp);
            ALUserData::userInfo->setQingDou(qingdouCount);
            ALUserData::qingDouCount = qingdouCount;
            
            
            
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([result,addExp,addQingDou]{
                ALGameResultInfoModel* model = ALGameResultInfoModel::create();
                model->gameResult = result;
                model->addExp = addExp;
                model->addQingdao = addQingDou;
                model->retain();
                cocos2d::NotificationCenter::getInstance()->postNotification(NND_GetFriendFightGameResult,model);
                CCLOG("返回游戏结果  %d  %d  %d",result,addExp,addQingDou);
            });
            
        }
        //** 接收 中途退出游戏 *//
        else if (strcmp(data, ALND_NF_RECEIVE_FIGHT_PK_OPPONENT_LEAVE) == 0)
        {
//            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([]{
                cocos2d::NotificationCenter::getInstance()->postNotification(NND_ReceiveFriendLeaveInTheGame);
//            });
            
        }
        //** 接收 再来一局的邀请 *//
        else if (strcmp(data, ALND_NF_PK_RECEIVE_GAME_AGAIN_INVITE) == 0)
        {
            CCLOG("接收到再来一局的对战");
            int uid = receiveInt();
            char* name = receiveStr();
            char* headUrl = receiveStr();
            int level = receiveInt();
            
            delete [] name;
            delete [] headUrl;
            
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([uid]{
                cocos2d::String* uidStr = cocos2d::String::createWithFormat("%d",uid);
                uidStr->retain();
                cocos2d::NotificationCenter::getInstance()->postNotification(NND_ReceiveFriendFightGameAgainInvitation,uidStr);
            });
            
            
        }//** 再来一局游戏游戏开始 *//
        else if (strcmp(data, ALND_NF_PK_GAME_AGAIN_READY_START) == 0)
        {
            ALPlayerData::difficultyLevel = receiveInt();
            ALPlayerData::roomIdOfPK = receiveInt();
            int uid = receiveInt();
            
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([uid]{
                CCLOG("再来一局  对方id== %d",uid);
                cocos2d::String* uidStr = cocos2d::String::createWithFormat("%d",uid);
                uidStr->retain();
                cocos2d::NotificationCenter::getInstance()->postNotification(NND_ReceiveFriendFightGameAgainReadyStart,uidStr);
            });
        }
        
        //** 接收 游戏结束后  对方离开 *//
        else if (strcmp(data, ALND_NF_PK_OPPOSITE_LEAVE_GAME) == 0)
        {
            int uid = receiveInt();
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([uid]{
                CCLOG("接收 游戏结束后  对方离开 = %d",uid);
                cocos2d::String* uidStr = cocos2d::String::createWithFormat("%d",uid);
                uidStr->retain();
                cocos2d::NotificationCenter::getInstance()->postNotification(NND_ReceiveFriendFightOppositeLeave,uidStr);
            });
        }
        
        // PK 游戏超时
        else if (strcmp(data, ALND_NF_FIGHT_PK_GAME_ROOM_TIME_OUT) == 0)
        {
            int roomId = receiveInt();
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([roomId]{
                cocos2d::String* roomIdStr = cocos2d::String::createWithFormat("%d",roomId);
                roomIdStr->retain();
                cocos2d::NotificationCenter::getInstance()->postNotification(NND_ReceiveFriendFightRoomTimeOut,roomIdStr);
            });
            
        }
            
        // 添加轻豆
        else if (strcmp(data, ALND_NF_ADD_LIGHT_BEAN) == 0)
        {
            CCLOG("添加轻豆");
        }
        
        
        // 重连结果返回给客户端 重连结果 *// //int   是否重连成功 （0 不成功【原因包括房间消失，其他错误等】   1重连成功）
        else if (strcmp(data, ALND_NF_FIGHT_PK_RECONNECT_REQUEST) == 0)
        {
            int code = receiveInt();
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([code]{
                CCLOG("重连结果 code= %d",code);
                cocos2d::String* codeStr = cocos2d::String::createWithFormat("%d",code);
                codeStr->retain();
                cocos2d::NotificationCenter::getInstance()->postNotification(NND_ReceiveFriendFightReconnectResult,codeStr);
            });
        }
        
        delete [] data;
        
    }
    CCLOG("线程关闭啦");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    ::closesocket(_socketHandle);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
    ::close(_socketHandle);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    close(_socketHandle);
    
#endif
    
    _socketHandle = 0;
    
    
    return 0;
}


/**
 *   关闭连接
 */
void ALBNSocketUitl::closeConnect()
{
    
    CCLOG("-----closeConnect()-----");
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//    ::closesocket(_socketHandle);
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
//    if (_connectFlag) {
//        ::close(_socketHandle);
//    }
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    close(_socketHandle);
//
//#endif
//
//     _socketHandle = 0;
    
    // 断开连接发送通知
    if (_connectFlag) {
        if(_notificationSwitch){
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([]{
                cocos2d::NotificationCenter::getInstance()->postNotification(NND_Disconnect);
            });
            
        }
    }
    _connectFlag = false;
    
}








#pragma mark -- 发送 和 接收字符串 --
void ALBNSocketUitl::sendInt(int si)
{
    if(_connectFlag)
    {
        if (hasSendError()) {
            closeConnect();
            return;
        }
        
        send(_socketHandle, (const char*)&si, 4, 0);
    }
}

void ALBNSocketUitl::sendFloat(float sf)
{
    char a[11];
    snprintf(a, sizeof(a), "%f", sf);
    const char *p = a;
    send(_socketHandle, p, 4, 0);
}

void ALBNSocketUitl::sendStr(const char* str, int len)
{
    if(_connectFlag)
    {
        if (hasSendError()) {
            closeConnect();
            return;
        }
        if (len > 0) {
            sendInt(len);
            send(_socketHandle, str, len, 0);
        }
    }
}


//直接发送字符串 自己获取长度
void ALBNSocketUitl::sendString(const char* str)
{
    sendStr(str,strlen(str));
//    CCLOG("发送的字符串  == %s",str);
}

char* ALBNSocketUitl::receiveBytes(int len)
{
    if(_connectFlag)
    {
        if (len <= 0)
        {
            return "close";
        }
        char* result = new char[len];
        int status = 0;
        status = recv(_socketHandle, result, len, 0);
        
        if (status == 0)
        {
            CCLOG("接收到的数据 status == 0");
            if (errno != EINTR)
            {
                closeConnect();
            }
            
            
            return "close";
        }
        else if (status == -1)//<0代表断开连接
        {
            if ((errno != EINTR) && (errno != EWOULDBLOCK))
            {
                closeConnect();
                return "close";
            }
        }
        while (status != len)
        {
            if(!_connectFlag)
            {
                break;
            }
            int index = status;
            int clength = len - index;
            char* b = new char[clength];
            int count = recv(_socketHandle, b, len - status, 0);
            
            if (count == 0)
            {
                if (errno != EINTR)
                {
                    closeConnect();
                }

                break;
            }
            else if (count < 0)//<0代表断开连接
            {
                if ((errno != EINTR) && (errno != EWOULDBLOCK))
                {
                    closeConnect();
                }
                break;
            }
            status = status + count;
            if (count != 0)
            {
                for (int i = 0; i < count; i++)
                {
                    result[index + i] = b[i];
                }
            }
            
            delete [] b;
            b = NULL;
        }

        return result;
    }else{
        return "close";
    }
}

int ALBNSocketUitl::receiveInt()
{
    char* a = receiveBytes(4);
    if (strcmp(a, "close") == 0)
    {
        return 0;
    }
    int ri = 0;
    
    memset(&ri, 0, sizeof(ri));
    memcpy((char*)(&ri), a, 4);
    
    if (strcmp(a, "close") != 0) {
        delete[] a;
    }
//    CCLOG("接收int  == %d",ri);
    return ri;
    
    
}

float ALBNSocketUitl::receiveFloat()
{

    float ri;

    return ri;
}

char* ALBNSocketUitl::receiveStr()
{
    
    int len = receiveInt();
    
    if (len < 0)
    {
        if (errno != EINTR)
        {
            closeConnect();
        }
        return "";
    }
    char* a = receiveBytes(len);
    char* result = new char[len + 1];
    memset(result, 0, len + 1);
    memcpy(result, a, len);
    if (strcmp(a, "close") != 0) {
        delete[] a;
    }
//    CCLOG("接收字符串  == %s",result);
    
    return result;
}






bool ALBNSocketUitl::hasError()
{
#ifdef WIN32
    int err = WSAGetLastError();
    if(err != WSAEWOULDBLOCK) {
        return true;
    }
#else
        int err = errno;
        if(err != EINPROGRESS && err != EAGAIN) {

            CCLOG("有错");
            return true;
        }
#endif
        
        return false;
}



bool ALBNSocketUitl::hasSendError()
{
    if (_socketHandle < 0) {
        return true;
    }
    return false;
}




