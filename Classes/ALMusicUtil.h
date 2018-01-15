//
//  ALMusicUtil.h
//  LLKGame
//
//  Created by Allen on 2017/12/25.
//
//

#ifndef ALMusicUtil_h
#define ALMusicUtil_h

class ALMusicUtil
{
public:
    //背景音乐
    enum GameBgMusic
    {
        GameMusic = 0, //游戏内的音乐
        MainMusic,
    };
    
    // 音效
    enum GameEffic
    {
        BtnClickEffic = 0, //按钮点击音效
        SelectedEffic,   // 选中方块的音效
        ClearEffic,     // 消除的音效
        MatchSuccessEffic, // 匹配成功的音效
        GameWinEffic,    //游戏胜利的音效
        GameFailEffic,   // 游戏失败的音效
        PageEffic,      // 翻页音效
        
    };
    
    
    static ALMusicUtil* getInstrins();
    ALMusicUtil();
    ~ALMusicUtil();
    
    
    void playBgSound(int musicId);
    void stopBgSound();
    void pauseSound();//停止背景音乐
    void resumeSound(bool isReWind);//继续音乐(是否从头开始播放）
    bool isBGSound();//是否有背景音乐正在播放（播放状态  和 暂停状态）
    int getPresentBGSoundId();  // 获取当前需要播放的背景音乐ID
    
    //根据ID播放音效
    void playEffic(int effId);
    void stopEffect();//停止所有音效
    
    /**
     *  停止和恢复所有音乐,（主要用于应用进入后台停止音乐，恢复前台恢复音乐）
     */
    void stopAllMusicAndEffect();
    void resumeAllMusicAndEffect();
    
    void endMusicUtil();
    
    /**
     *  所有音乐的开关方法
     */
    void setSoundSwitch(bool _isS);
    bool getSoundSwitch();
    
    
    /**
     *  背景音乐的开关方法
     */
    void setBGMusicSwitch(bool _isS);
    bool getBGMusicSwitch();
    
    
    /**
     *  音效的开关方法
     */
    void setEffectSwitch(bool _isS);
    bool getEffectSwitch();
    
    
    
private:
    /** 所有声音的开关 */
    bool _isSoundAll;
    /** 背景音乐的开关 */
    bool _isEffect;
    /** 音效的开关 */
    bool _isBGMusic;
    
    int  _presentBGSoundId;  // 当前需要播放的背景音乐ID，包括暂停的背景音乐(-1代表没有)
    
};



#endif /* ALMusicUtil_h */
