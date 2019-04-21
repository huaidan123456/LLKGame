//
//  ALMusicUtil.cpp
//  LLKGame
//
//  Created by Allen on 2017/12/25.
//
//

#include "ALMusicUtil.h"


#include "cocos2d.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
USING_NS_CC;


#define ALMusicUtil_KEY_ISALLSOUND "ALMusicUtil_KEY_ISALLSOUND"
#define ALMusicUtil_KEY_ISBGMUSIC "ALMusicUtil_KEY_ISBGMUSIC"
#define ALMusicUtil_KEY_ISEFFECT "ALMusicUtil_KEY_ISEFFECT"

ALMusicUtil* mimic = nullptr;//拿着自己的对象

ALMusicUtil* ALMusicUtil::getInstrins()
{
    if(!mimic)
    {
        mimic = new ALMusicUtil();
        mimic->_presentBGSoundId = -1;
        
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1);
        SimpleAudioEngine::getInstance()->setEffectsVolume(1);
        
    }

    return mimic;
    
}


ALMusicUtil::ALMusicUtil(void)
{
    _isSoundAll = UserDefault::getInstance()->getBoolForKey(ALMusicUtil_KEY_ISALLSOUND, true);
    _isEffect = UserDefault::getInstance()->getBoolForKey(ALMusicUtil_KEY_ISEFFECT, true);
    _isBGMusic = UserDefault::getInstance()->getBoolForKey(ALMusicUtil_KEY_ISBGMUSIC, true);
    
}


ALMusicUtil::~ALMusicUtil(void)
{
    
}


#pragma mark -- 关于背景音乐 --

/**
 *  根据ID播放背景音乐
 */
void ALMusicUtil::playBgSound(int musicId)
{
    
    if (musicId < 0) {
        return;
    }
    // 如果存在背景音乐 并且 背景音乐就是要播放的音乐 则继续播放
    if (isBGSound() && _presentBGSoundId == musicId) {
        resumeSound(false);
        return;
    }else{
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1);
        // 如果有播放的背景音乐，就停掉
        if (isBGSound()) {
            SimpleAudioEngine::getInstance()->stopBackgroundMusic();
        }
        _presentBGSoundId = musicId;
        std::string musicName;
        switch (musicId) {
            case ALMusicUtil::GameBgMusic::GameMusic://游戏内的音乐
            {
                musicName = "sound/bg_game.mp3";
                break;
            }
            case ALMusicUtil::GameBgMusic::MainMusic://游戏主界面
            {
                musicName = "sound/bg_main.mp3";
                break;
            }
                
            default:{
                _presentBGSoundId = -1;
                break;
                
            }
                
        }
        
        if (_isSoundAll && _isBGMusic && _presentBGSoundId != -1) {
            SimpleAudioEngine::getInstance()->playBackgroundMusic(musicName.c_str(),true);
        }
        
    }

}

/**
 *  停止背景音乐
 */
void ALMusicUtil::stopBgSound(){
    if (isBGSound()) {
        SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    }
    _presentBGSoundId = -1;
}

/**
 *  暂停背景音乐
 */

void ALMusicUtil::pauseSound()
{
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

/**
 * 继续音乐(是否从新播放音乐)
 */
void ALMusicUtil::resumeSound(bool isReWind)
{
    if(_isSoundAll)
    {
        if (isBGSound()) {
            if(isReWind)
            {
                SimpleAudioEngine::getInstance()->rewindBackgroundMusic();
            }else
            {
                SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
            }
        }
        
    }
    
}

/**
 *  是否有背景音乐正在播放（播放状态  和 暂停状态）
 */
bool ALMusicUtil::isBGSound()
{
    return SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying();
}

/**
 *  获取当前正在播放的背景音乐ID
 */
int ALMusicUtil::getPresentBGSoundId()
{
    if (!isBGSound()) {
        _presentBGSoundId = -1;
    }
    return _presentBGSoundId;
}


/**
 *  获取被暂停的背景音乐
 */


#pragma mark -- 关于音效部分 --
//根据ID播放音效
void ALMusicUtil::playEffic(int effId)
{
    //文件名字   是否循环  基频 立体效果 体积
    if(_isSoundAll && _isEffect)
    {
        SimpleAudioEngine::getInstance()->setEffectsVolume(1);
        std::string soundName;
        bool isShake = false;
        switch (effId) {
            case ALMusicUtil::GameEffic::BtnClickEffic:
            {
                soundName = "sound/e_btnClicked.mp3";
                break;
            }
            case ALMusicUtil::GameEffic::SelectedEffic:
            {
                soundName = "sound/e_selected.mp3";
                break;
            }
            case ALMusicUtil::GameEffic::ClearEffic:
            {
                soundName = "sound/e_clear.mp3";
                break;
            }
            case ALMusicUtil::GameEffic::MatchSuccessEffic:
            {
                soundName = "sound/e_matchSuccess.mp3";
                break;
            }
            case ALMusicUtil::GameEffic::GameWinEffic:
            {
                soundName = "sound/e_win.mp3";
                break;
            }
            case ALMusicUtil::GameEffic::GameFailEffic:
            {
                soundName = "sound/e_fail.mp3";
                break;
            }
            case ALMusicUtil::GameEffic::PageEffic:
            {
                soundName = "sound/e_fanye.mp3";
                break;
            }

            default:
            {
                soundName = "";
                break;
            }
                
        }
        if (soundName.length() > 0) {
            SimpleAudioEngine::getInstance()->playEffect(soundName.c_str(), false, 1, 0, 1);
            
        }
        
        
    }
}



void ALMusicUtil::stopEffect()
{
    SimpleAudioEngine::getInstance()->stopAllEffects();
}



/**
 *  停止和恢复所有音乐,（主要用于应用进入后台停止音乐，恢复前台恢复音乐）
 */
void ALMusicUtil::stopAllMusicAndEffect()
{
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    SimpleAudioEngine::getInstance()->stopAllEffects();
}


void ALMusicUtil::resumeAllMusicAndEffect()
{
    playBgSound(_presentBGSoundId);
}


void ALMusicUtil::endMusicUtil()
{
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    SimpleAudioEngine::getInstance()->stopAllEffects();
    SimpleAudioEngine::getInstance()->end();
}



/**
 *  所有音乐的开关方法
 */
bool ALMusicUtil::getSoundSwitch(){
    return _isSoundAll;
}

void ALMusicUtil::setSoundSwitch(bool _isS)
{
    _isSoundAll = _isS;
    if(_isS==false)
    {
        SimpleAudioEngine::getInstance()->stopBackgroundMusic();
        stopEffect();
    }else
    {
        playBgSound(_presentBGSoundId);
        
    }
    UserDefault::getInstance()->setBoolForKey(ALMusicUtil_KEY_ISALLSOUND, _isSoundAll);
    
}

/**
 *  背景音乐的开关方法
 */
void ALMusicUtil::setBGMusicSwitch(bool _isS)
{
    if (_isBGMusic == _isS) {
        return;
    }
    _isBGMusic = _isS;
    if (_isS == false) {
        SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    }else{
        playBgSound(_presentBGSoundId);
    }
    UserDefault::getInstance()->setBoolForKey(ALMusicUtil_KEY_ISBGMUSIC, _isBGMusic);
}


bool ALMusicUtil::getBGMusicSwitch()
{
    return _isBGMusic;
}


/**
 *  音效的开关方法
 */
void ALMusicUtil::setEffectSwitch(bool _isS)
{
    if (_isEffect == _isS) {
        return;
    }
    
    _isEffect = _isS;
    if (_isS == false) {
        stopEffect();
    }
    UserDefault::getInstance()->setBoolForKey(ALMusicUtil_KEY_ISEFFECT, _isEffect);
}
bool ALMusicUtil::getEffectSwitch()
{
    return _isEffect;
}

