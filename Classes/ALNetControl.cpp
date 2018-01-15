//
//  ALNetControl.cpp
//  LLKGame
//
//  Created by Allen on 2017/12/18.
//
//

#include "ALNetControl.h"
#include "ALBNSocketUitl.h"


/**
 *  设置网络通知的开关
 */
void ALNetControl::setupNetworkNotificationSwitch(bool isOpen)
{
    ALBNSocketUitl::_notificationSwitch = isOpen;
}

void ALNetControl::checkNetwork()
{
    ALBNSocketUitl::sendString(ALND_NF_REQUEST_ONLINE_NUMBER);
}


bool ALNetControl::isConnect()
{
    return ALBNSocketUitl::_connectFlag;
}

bool ALNetControl::connectServerWithName(const char *name, unsigned short port, int uid)
{
    // 获取域名
    hostent *mhost = gethostbyname(name);
    char ip_str[32];
    if (mhost) {
        switch (mhost->h_addrtype) {
            case AF_INET:
            case AF_INET6:
            {
                char **pptr;
                
                pptr = mhost->h_addr_list;
                for (;*pptr != NULL; pptr++) {
                    printf(" address:%s/n", inet_ntop(mhost->h_addrtype, *pptr, ip_str, sizeof(ip_str)));
                }
                break;
            }
   
            default:
                break;
        }
    }

    return connectServerWithIp(ip_str, port, uid);
    
}


bool ALNetControl::connectServerWithIp(const char *ip, unsigned short port,int uid)
{
   return ALBNSocketUitl::loginServer(ip, port, uid);
}

void ALNetControl::closeConnect()
{
    ALBNSocketUitl::closeConnect();
}


void ALNetControl::requestUserInfo()
{
    ALBNSocketUitl::sendString(ALND_NF_USER_INFO);
}

/**
 *  刷新用户的轻豆
 */
void  ALNetControl::requestUserQingDouCount()
{
    ALBNSocketUitl::sendString(ALND_NF_UPDATE_USER_QINGDOU);
}


void ALNetControl::requestUserNiangCard()
{
    ALBNSocketUitl::sendString(ALND_NF_REFRESH_USER_NIANGCARD);
}



void ALNetControl::sendRandomMatch()
{
    ALBNSocketUitl::sendString(ALND_NF_GET_RANDOM_PLAYER);
}

/**
 *  单机 发送游戏结果
 */
void ALNetControl::sendStandAloneGameResult(bool result)
{
    ALBNSocketUitl::sendString(ALND_NF_SEND_STANDALONE_GAME_RESULT);
    ALBNSocketUitl::sendInt(result);
}


/**
 *  发送抽奖请求
 */
void ALNetControl::sendDrawNiangCardAward()
{
    ALBNSocketUitl::sendString(ALND_NF_DRAW_NIANING_CARD_AWARY);
}

/**
 *  添加轻豆
 */
void ALNetControl::requestGetQingDou(int number)
{
    ALBNSocketUitl::sendString(ALND_NF_ADD_LIGHT_BEAN);
    ALBNSocketUitl::sendInt(number);
}


/**
 *  请求在线人数
 */
void ALNetControl::requestOnlineNumber()
{
    ALBNSocketUitl::sendString(ALND_NF_REQUEST_ONLINE_NUMBER);
}

