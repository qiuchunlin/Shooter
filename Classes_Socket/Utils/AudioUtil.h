#ifndef _UTILS_AUDIOUTIL_
#define _UTILS_AUDIOUTIL_
#include "../Common/ResourceName.h"
class AudioUtil
{
public:
    static void preload();
  

    static void playMusic(const char* file, bool repeat = false);
	static void pauseMusic();
	static void resumeMusic();
	static void stopMusic();
	static void pauseAudioEngineAll();
	static void resumeAudioEngineAll();

	static void playSound(const char* file, bool repeat = false);
	static void stopSound();
   
   
    static void stopAll();
	

	static void enableSound(bool enable);
	static void enableMusic(bool enable);
private:
	
	static bool _bIsLoaded;
	static bool _bIsSoundEnabled;
	static bool _bIsMusicEnabled;

};

#endif
