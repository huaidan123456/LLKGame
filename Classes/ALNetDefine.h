//
//  ALNetDefine.h
//  LLKGame
//
//  Created by Allen on 2017/12/18.
//
//

#ifndef ALNetDefine_h
#define ALNetDefine_h


#define ALND_NF_IP  "39.106.171.36"    //"192.168.0.83"
#define ALND_NF_HOST_NAME "api.qingdian.cn"
#define ALND_NF_PORT 59421

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


//** 发送单机游戏 是否胜利 *//
#define ALND_NF_SEND_STANDALONE_GAME_RESULT "<#JIANGLI#>"
//** 游戏结果 *//
#define ALND_NF_GET_STANDALONE_GAME_RESULT "<#RESULT#>"
//** 发送 和接收 抽娘卡 *//
#define ALND_NF_DRAW_NIANING_CARD_AWARY "<#EXTRACT_CARD#>"
//** 加轻豆的方法 *//
#define ALND_NF_ADD_LIGHT_BEAN "<#ADD_LIGHT_BEAN#>"


#endif /* ALNetDefine_h */
