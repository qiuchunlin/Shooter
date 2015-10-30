#include "AudioUtil.h"
#include "SimpleAudioEngine.h"
#include "AudioEngine.h"
//#include "../Service/UserService.h"
#include "cocos2d.h"
USING_NS_CC;

using namespace std;
using namespace CocosDenshion;
using namespace experimental;

bool AudioUtil::_bIsLoaded = false;
bool AudioUtil::_bIsMusicEnabled = true;
bool AudioUtil::_bIsSoundEnabled = true;

void AudioUtil::preload()
{
	if(!_bIsLoaded)
	{
//		_bIsMusicEnabled = UserService::getInstance()->getIsMisicOn();
//		_bIsSoundEnabled = UserService::getInstance()->getIsSoundOn();
        
		
		auto audioEngine = SimpleAudioEngine::getInstance();
	

    }

	_bIsLoaded = true;
}

void AudioUtil::playMusic(const char *file, bool repeat)
{
    if(_bIsMusicEnabled)
    {
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.8f);//volume ranges in [0,1]
        SimpleAudioEngine::getInstance()->playBackgroundMusic(file, repeat);
    }
}


void AudioUtil::pauseMusic()
{
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void AudioUtil::resumeMusic()
{
	if(_bIsMusicEnabled)
	{
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	}
}

void AudioUtil::stopMusic()
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
}

void AudioUtil::playSound(const char *file, bool repeat)
{
    if(_bIsSoundEnabled)
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		SimpleAudioEngine::getInstance()->playEffect(file, repeat);
#else
		AudioEngine::play2d(file, repeat, 1.0f);//播放音效文件
#endif
    }
}

void AudioUtil::stopSound()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	SimpleAudioEngine::getInstance()->stopAllEffects();
#else
	AudioEngine::stopAll();
#endif
	
}


void AudioUtil::stopAll()
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	SimpleAudioEngine::getInstance()->stopAllEffects();
#else
	AudioEngine::stopAll();
#endif
}


void AudioUtil::enableMusic(bool enable) {
	_bIsMusicEnabled = enable;
//	UserService::getInstance()->setIsMisicOn(enable);
	if (!enable)
	{
		AudioUtil::stopMusic();
	}
}


void AudioUtil::enableSound(bool enable) {
	_bIsSoundEnabled = enable;
//	UserService::getInstance()->setIsSoundOn(enable);
}

void AudioUtil::pauseAudioEngineAll()
{
	AudioEngine::pauseAll();
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	SimpleAudioEngine::getInstance()->pauseAllEffects();
#else
	AudioEngine::pauseAll();
#endif
}

void AudioUtil::resumeAudioEngineAll()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	SimpleAudioEngine::getInstance()->resumeAllEffects();
#else
	AudioEngine::resumeAll();
#endif
}