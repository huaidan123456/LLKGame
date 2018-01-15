package org.cocos2dx.cpp;

import android.util.Log;

/**
 * Created by allen on 2017/12/26.
 */

public class ALGameHelpUtil {
    /**
     *  设置游戏用户的uid，必须在进入游戏前进行设置
     * @param uid
     */
    public static native  void setLoginUidOfGame(int uid);

    /**
     *  游戏分享成功后调用的方法
     *  此方法用于告诉游戏服务器分享成功，发放轻豆
     */
    public static native  void gameShareSuccessCallback();


    /**
     *  游戏主界面中的娘卡详情页面 点击分享按钮的相关操作
     *  用户需要实现此方法，应该在此方法中掉用相应的分享操作
     */
    public static void shareOfGameCardInfo()
    {
        // TODO: 2017/12/26  在此处写相关分享的方法
        Log.e("ALGameHelpUtil","游戏主界面的分享操作");
    }

    /**
     *   游戏内抽卡界面 中分享按钮的相关操作
     *  用户需要实现此方法，应该在此方法中掉用相应的分享操作
     */
    public static void shareOfGameDrawCard(int cardIndex)
    {
        // TODO: 2017/12/26 在此处写相关分享的操作
        Log.e("ALGameHelpUtil","游戏中的分享操作 卡片index = "+cardIndex);
    }

    /**
     *  退出游戏的相关一些操作  (此处用户点击退出游戏时候的一些操作)
     *  用户需要实现此方法，此方法中需要调用切换activity
     */
    public static void exitGame()
    {
        // TODO: 2017/12/26 实现退出游戏的相关操作
        Log.e("ALGameHelpUtil","退出游戏操作");

    }

}
