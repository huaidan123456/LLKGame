//
//  ALNetDefine.h
//  LLKGame
//
//  Created by Allen on 2017/12/18.
//
//

#ifndef ALNetDefine_h
#define ALNetDefine_h


#define ALND_NF_IP "39.106.171.36"    //"192.168.0.101"
#define ALND_NF_HOST_NAME "api.qingdian.cn"
#define ALND_NF_PORT 59422

//** 连接服务器 *//
#define ALND_NF_CONNECT "<#CONNECT#>"
//** 获取用户信息 *//
#define ALND_NF_USER_INFO "<#GET_USER_INFO#>"
//** 刷新角色信息 *//
#define ALND_NF_REFRESH_USER_INFO "<#REFRESH_INFO#>"
//** 获取自动匹配的用户信息 *//
#define ALND_NF_GET_RANDOM_PLAYER "<#GET_RANDOM_PLAYER#>"
//** 更新用户的轻豆 *//
#define ALND_NF_UPDATE_USER_QINGDOU "<#REFRESH_BEAN#>"
//** 刷新用户娘卡 *//
#define ALND_NF_REFRESH_USER_NIANGCARD "<#NIANG_CARD#>"

//** 申请在线人数 *//
#define ALND_NF_REQUEST_ONLINE_NUMBER "<#QUERY_ONLINE#>"
//** 申请好友列表 *//
#define ALND_NF_REFRESH_GAME_FRIENDS_LIST "<#U_FRIENDS#>"
//** 刷新好友在线情况 *//
#define ALND_NF_REFRESH_GAME_FRIEND_ONLINE "<#F_ONLINE#>"
//** 申请约战 *//
#define ALND_NF_REQUEST_FIGHT_WITH_FRIEND "<#FIGHT#>"

//被申请约战（给被约战的客户端发送） 做弹出框提示约战(如果在大厅界面时)
#define ALND_NF_RECEIVE_FIGHT_INVITE "<#FIGHT_APPLY#>"
//给申请人返回（表示他已经成功给对方发信息并且自己也要进入等待对方同意的状态）
#define ALND_NF_RECEIVE_INVITE_SUCCESS_TO_WAIT "<#FIGHT_APPLY_WAIT#>"
//给申请人返回  对方拒绝约战
#define ALND_NF_RECEIVE_REFUSE_FIGHT_INVITE "<#FIGHT_UN_AGREE#>"
// 同意约战
#define ALND_NF_SEND_AGREE_FIGHT_WITH_FRIEND "<#FIGHT_AGREE#>"
//** 给双方返回游戏难度和房间号 （表示被约战人同意 约战  并且可以准备游戏了）*//
#define ALND_NF_RECEIVE_PK_GAME_INFO "<#FIGHT_BEGIN#>"
//** 进入房间并且准备完毕的操作 *//
#define ALND_NF_FIGHT_PK_READY_FINISH "<#READY_TO_FINISH#>"
//** 双方都准备完毕，开始的操作 *//
#define ALND_NF_FIGHT_PK_START_GAME "<#GAME_START#>"
//** 消除 给服务器发送消除一对 *//
#define ALND_NF_SEND_FIGHT_PK_CLEAN_ICON "<#ADD_POINT#>"
//** 接收当前游戏消除情况 服务器给客户端返回双方当前分数情况（两个玩家的得分情况）*//
#define ALND_NF_RECEIVE_FIGHT_PK_CURRENT_COUNT "<#NOW_COUNT#>"
//** 接收到游戏结束 *//
#define ALND_NF_RECEIVE_FIGHT_PK_GAME_OVER "<#BATTLE_RESULT#>"

//** 对方中途退出 *//
#define ALND_NF_RECEIVE_FIGHT_PK_OPPONENT_LEAVE "<#GAME_OVER2#>"

//** 退出当前玩的对局(中途退出) *//
#define ALND_NF_SEND_QUIT_PK_GAME "<#QUIT_GAME#>"

//** 返回游戏主菜单（游戏结束 返回） *//
#define ALND_NF_PK_SEND_BACK "<#QUIT_TO_MENU#>"
//** 对方离开游戏 *//
#define ALND_NF_PK_OPPOSITE_LEAVE_GAME "<#QUIT_TO_MENU#>"

//** 邀请对方再来一局 *//
#define ALND_NF_PK_REQUEST_FIGHT_GAME_AGAIN "<#FIGHT_IN_GAME#>"
//** 被邀请方接收到再来一局的邀请 *//
#define ALND_NF_PK_RECEIVE_GAME_AGAIN_INVITE "<#FIGHT_APPLY_INGAME#>"
//** 被邀请方 发送 同意再来一局 *//
#define ALND_NF_PK_AGREE_GAME_AGAIN_INVITE "<#FIGHT_AGREE_INGAME#>"
//** 再来一局游戏游戏开始 *//
#define ALND_NF_PK_GAME_AGAIN_READY_START "<#FIGHT_BEGIN_INGAME#>"

//** 断线重连 *//
#define ALND_NF_FIGHT_PK_RECONNECT_REQUEST "<#RECONNECT_PK#>"
//** 返回给客户端 重连结果 *// //int   是否重连成功 （0 不成功【原因包括房间消失，其他错误等】   1重连成功）
#define ALND_NF_FIGHT_PK_RECONNECT_RESULT "<#RECONNECT_PK#>"

//** 好友对战的游戏中  从后台回到前台给服务器发送到字段 *//
#define ALND_NF_FIGHT_PK_RETURN_GAME_WILL_ENTERFOREGROUND "<#RETURN_GAME#>"

//** 好友PK 超时 *//
#define ALND_NF_FIGHT_PK_GAME_ROOM_TIME_OUT "<#ROOM_TIME_OUT#>"

//**  *//


//** 发送单机游戏 是否胜利 *//
#define ALND_NF_SEND_STANDALONE_GAME_RESULT "<#JIANGLI#>"
//** 游戏结果 *//
#define ALND_NF_GET_STANDALONE_GAME_RESULT "<#RESULT#>"
//** 发送 和接收 抽娘卡 *//
#define ALND_NF_DRAW_NIANING_CARD_AWARY "<#EXTRACT_CARD#>"
//** 加轻豆的方法 *//
#define ALND_NF_ADD_LIGHT_BEAN "<#ADD_LIGHT_BEAN#>"


#endif /* ALNetDefine_h */
