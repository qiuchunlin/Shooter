#ifndef _UTILS_UTILS_
#define _UTILS_UTILS_
#include "cocos2d.h"
USING_NS_CC;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h" 
#endif
using namespace std;

class Utils
{
public:
    template<typename T>
    static string TtoS(T t)
    {
        ostringstream os;
        
        os << t;
        return os.str();
    }
	
    static int random(int startIncluded, int endIncluded);
    static bool randomPercent(float fSuccessPercent);
    static SpriteFrame* createFrame(const string& fileName);
    
    static string encode(const string & inValue);
    static string decode(const string & inValue);

	static int getUtf8Len(char firstByte);
	static int getUtf8StrLen(const string & sString);
	static string splitUtf8StrByCount(const string & sString ,int nCount);
	static string chageUtf8Str(const string & sString, const char* changeItem, const string& fChangeValue);

	static int getTimeGap(time_t time1, time_t time2);
	static time_t time_maker(int date);
	static int current_time(time_t& currenttime);

	static string getStringFromFile(const string& filePath);
	static void writeStringToFile(const string& srtContent, const string& filePath);
	static Data getDataFromString(const string& srtContent);

	static void childrenFadeIn(Node * pNode, float fFadeTime, float fDelayTime = 0);
	static void childrenFadeOut(Node * pNode, float fFadeTime, float fDelayTime = 0);
	static void stopAllActions(Node * pNode);

	static string getShowTime(float fTime);

	static Vec2 getPosInMap(Vec2 pos, TMXTiledMap* pMap);

	static float getPosAngle(Vec2 targetPos,Vec2 curPos);

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	static string jstringToChar(JNIEnv* env,jstring jString);
#endif
	
};
#endif 
