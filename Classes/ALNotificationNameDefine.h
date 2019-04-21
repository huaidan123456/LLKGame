//
//  ALNotificationNameDefine.h
//  AllenLRS
//
//  Created by Allen on 2017/8/10.
//
//

#ifndef ALNotificationNameDefine_h
#define ALNotificationNameDefine_h


/** 进入后台 */
#define NND_AppDidEnterBackground "NND_AppDidEnterBackground"

/** 重新恢复至前台 */
#define NND_AppWillEnterForeground "NND_AppWillEnterForeground"

//** 重新设置界面的通知 *//
#define NND_AppResetScene "NND_AppResetScene"

//** 断开连接 *//
#define NND_Disconnect "NND_Disconnect"


//** 刷新用户娘卡 *//
#define NND_RefreshOnlineNumber "NND_RefreshOnlineNumber"



//** 刷新用户信息 *//
#define NND_RefreshUserInfo "NND_RefreshUserInfo"

//** 刷新用户的轻豆数量 *//
#define NND_RefreshUserQingDouCount "NND_RefreshUserQingDouCount"

//** 刷新用户娘卡 *//
#define NND_RefreshUserNiangCard "NND_RefreshUserNiangCard"

//** 刷新好友列表 *//
#define NND_RefreshGameFriendList "NND_RefreshGameFriendList"

//** 被申请约战 *//
#define NND_InviteFightGameWithFriend "NND_InviteFightGameWithFriend"

//** 对方拒绝约战 *//
#define NND_RefuseInviteFlageGame "NND_RefuseInviteFlageGame"

//** 同意约战并且给出PK房间号 *//
#define NND_AgreeInviteFlageGame "NND_AgreeInviteFlageGame"

//** 好友对战的游戏结果 *//
#define NND_GetFriendFightGameResult "NND_GetFriendFightGameResult"

//** 好友对战  中途退出游戏 *//
#define NND_ReceiveFriendLeaveInTheGame "NND_ReceiveFriendLeaveInTheGame"

//** 好友对战  接收的再来一局的邀请 *//
#define NND_ReceiveFriendFightGameAgainInvitation "NND_ReceiveFriendFightGameAgainInvitation"

//** 好友对战  同意再来一局  再来一局游戏开始 *//
#define NND_ReceiveFriendFightGameAgainReadyStart "NND_ReceiveFriendFightGameAgainReadyStart"

//** 好友对战  在游戏结束的时候离开了房间 *//
#define NND_ReceiveFriendFightOppositeLeave "NND_ReceiveFriendFightOppositeLeave"

//** 好友对战  重连结果 *//
#define NND_ReceiveFriendFightReconnectResult "NND_ReceiveFriendFightReconnectResult"

//** 好友PK 游戏超时 *//
#define NND_ReceiveFriendFightRoomTimeOut "NND_ReceiveFriendFightRoomTimeOut"



//** 获取到了随机匹配的玩家信息 *//
#define NND_GetRandomMatchUserInfo "NND_GetRandomMatchUserInfo"

//** 单机游戏结果 *//
#define NND_GetStandAloneGameResult "NND_GetStandAloneGameResult"

//** 抽娘卡结果 *//
#define NND_GetDrawNiangCardResult "NND_GetDrawNiangCardResult"


//** 连连看好友的邀请 *//
#define NND_ReceiveFriendInvite "NND_ReceiveFriendInvite"



#endif /* ALNotificationNameDefine_h */
