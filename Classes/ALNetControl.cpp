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

/**
 *  请求好友列表
 */
void ALNetControl::requestGameFriendList()
{
    ALBNSocketUitl::sendString(ALND_NF_REFRESH_GAME_FRIENDS_LIST);
}

/**
 *  申请约战
 */
void ALNetControl::requestFightWithFriend(int friendId)
{
    ALBNSocketUitl::sendString(ALND_NF_REQUEST_FIGHT_WITH_FRIEND);
    ALBNSocketUitl::sendInt(friendId);
}

/**
 *  同意约战
 */
void ALNetControl::sendAgreeFightInvite(int inviterId)
{
    ALBNSocketUitl::sendString(ALND_NF_SEND_AGREE_FIGHT_WITH_FRIEND);
    ALBNSocketUitl::sendInt(inviterId);
}

/**
 *  拒绝约战
 */
void ALNetControl::sendRefuseFightInvite(int inviterId)
{
    ALBNSocketUitl::sendString(ALND_NF_RECEIVE_REFUSE_FIGHT_INVITE);
    ALBNSocketUitl::sendInt(inviterId);
}


/**
 *  进入房间并且准备完毕
 */
void ALNetControl::readyOfFriendFightGame()
{
    ALBNSocketUitl::sendString(ALND_NF_FIGHT_PK_READY_FINISH);
}


/**
 *  好友对战时候 消除一个向服务器发送消除
 */
void ALNetControl::sendCleanIconOfFriendFight()
{
    ALBNSocketUitl::sendString(ALND_NF_SEND_FIGHT_PK_CLEAN_ICON);
}

/**
 *  好友对战的时候  离开本局
 */
void ALNetControl::sendLeaveRoomOfFriendFight()
{
    ALBNSocketUitl::sendString(ALND_NF_SEND_QUIT_PK_GAME);
}

/**
 *  好友对战结束后 发送再来一局
 */
void ALNetControl::sendGameAgainOfFriendFight(int friendUid)
{
    ALBNSocketUitl::sendString(ALND_NF_PK_REQUEST_FIGHT_GAME_AGAIN);
    ALBNSocketUitl::sendInt(friendUid);
}

/**
 *  发送同意对方的再来一局
 */
void ALNetControl::sendAgreeGameAgainOfFriendFight(int inviterId)
{
    ALBNSocketUitl::sendString(ALND_NF_PK_AGREE_GAME_AGAIN_INVITE);
    ALBNSocketUitl::sendInt(inviterId);
}

/**
 *  发送 对战结束  离开游戏
 */
void ALNetControl::sendLeaveFriendFightGameOfGameOver(int friendUid)
{
    ALBNSocketUitl::sendString(ALND_NF_PK_OPPOSITE_LEAVE_GAME);
    ALBNSocketUitl::sendInt(friendUid);
}


/**
 *  好友对战中 断线重连机制
 */
void ALNetControl::reconnectOfFriendFight(int uid,int roomId)
{
    ALBNSocketUitl::sendString(ALND_NF_FIGHT_PK_RECONNECT_REQUEST);
    ALBNSocketUitl::sendInt(uid);
    ALBNSocketUitl::sendInt(roomId);
}


/**
 *  好友对战的游戏中  从后台回到前台给服务器发送到字段
 */
void ALNetControl::backGameAfterEnterforeground(int roomId)
{
    ALBNSocketUitl::sendString(ALND_NF_FIGHT_PK_RETURN_GAME_WILL_ENTERFOREGROUND);
    ALBNSocketUitl::sendInt(roomId);
}

/**
 *  发送随机匹配
 */
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

