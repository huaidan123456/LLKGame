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


#define SOCKET_ERROR -1
#define INVALID_SOCKET -1



HSocket ALBNSocketUitl::_socketHandle = 0;
bool ALBNSocketUitl::_connectFlag = false;
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
    _connectFlag = true;

//    new std::thread(&ALBNSocketUitl::threadReceiveTask);
    std::thread sthread(&ALBNSocketUitl::threadReceiveTask);
    sthread.detach();
    return true;
    
    
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
    
    _connectFlag = true;
//    new std::thread(&ALBNSocketUitl::threadReceiveTask);
    std::thread sthread(&ALBNSocketUitl::threadReceiveTask);
    sthread.detach();
    return true;

#endif
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
        cocos2d::NotificationCenter::getInstance()->postNotification(NND_Disconnect);
    }
    
    return false;
}


/**
 *  接收消息
 */
void* ALBNSocketUitl::threadReceiveTask()
{
    while (_connectFlag) {
//        CCLOG("threadReceiveTask()!!!!!");
        char* data = receiveStr();
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
            
            cocos2d::NotificationCenter::getInstance()->postNotification(NND_RefreshUserInfo,NULL);
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
            cocos2d::NotificationCenter::getInstance()->postNotification(NND_RefreshUserInfo,NULL);
        }
        // 刷新用户信息
        else if (strcmp(data, ALND_NF_UPDATE_USER_QINGDOU) == 0)
        {
            int qingdouCount = receiveInt();
            ALUserData::qingDouCount = qingdouCount;
            ALUserData::userInfo->setQingDou(qingdouCount);
            cocos2d::NotificationCenter::getInstance()->postNotification(NND_RefreshUserQingDouCount,NULL);
        }
        // 获取自动匹配角色的信息
        else if (strcmp(data, ALND_NF_GET_RANDOM_PLAYER) == 0)
        {
            int uid = receiveInt();
            char* name = receiveStr();
            char* headUrl = receiveStr();
            int level = receiveInt();
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
            CCLOG("名称 =%s",name);
            CCLOG("头像地址  %s",headUrl);
            
            delete [] name;
            delete [] headUrl;
            
            cocos2d::NotificationCenter::getInstance()->postNotification(NND_GetRandomMatchUserInfo,NULL);
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
            
            cocos2d::NotificationCenter::getInstance()->postNotification(NND_RefreshUserNiangCard,NULL);
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
            
            ALGameResultInfoModel* model = ALGameResultInfoModel::create();
            model->gameResult = result;
            model->addExp = addExp;
            model->addQingdao = addQingDou;
//            model->retain();
            cocos2d::NotificationCenter::getInstance()->postNotification(NND_GetStandAloneGameResult,model);
            CCLOG("返回游戏结果  %d  %d  %d",result,addExp,addQingDou);
        }// 接收抽奖奖励
        else if (strcmp(data, ALND_NF_DRAW_NIANING_CARD_AWARY) == 0)
        {
            CCLOG("抽卡回调");
            auto model =  ALDrawCardResultInfoModel::create();
            model->isGain = receiveInt();
            model->cardIndex = receiveInt();
            model->awardIndex = receiveInt();
//            model->retain();
            
            CCLOG("bnsocket抽卡详情   %d  %d  %d ",model->isGain,model->cardIndex,model->awardIndex);
            
            cocos2d::NotificationCenter::getInstance()->postNotification(NND_GetDrawNiangCardResult, model);
        }
        // 请求在线人数
        else if (strcmp(data, ALND_NF_REQUEST_ONLINE_NUMBER) == 0)
        {
            ALPlayerData::onLienNumber = receiveInt();
            cocos2d::NotificationCenter::getInstance()->postNotification(NND_RefreshOnlineNumber);
        }
        // 添加轻豆
        else if (strcmp(data, ALND_NF_ADD_LIGHT_BEAN) == 0)
        {
            
        }
        
        
        delete [] data;
        
    }
    CCLOG("线程关闭啦");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    ::closesocket(_socketHandle);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
    if (_connectFlag) {
        ::close(_socketHandle);
    }
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
            cocos2d::NotificationCenter::getInstance()->postNotification(NND_Disconnect);
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




