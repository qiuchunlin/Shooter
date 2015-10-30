#ifndef _UTILS_YJ_USERDEFAULT_
#define _UTILS_YJ_USERDEFAULT_

#include "cocos2d.h"
#include "wxsqlite/src/sqlite3.h"
USING_NS_CC;
using namespace std;

class YJ_UserDefault
{
public:
	bool    getBoolForKey(const char* pKey, bool defaultValue);
	int     getIntegerForKey(const char* pKey, int defaultValue);
	float    getFloatForKey(const char* pKey, float defaultValue);
	double  getDoubleForKey(const char* pKey, double defaultValue);
	string getStringForKey(const char* pKey, const std::string & defaultValue);

	void    setBoolForKey(const char* pKey, bool value);
	void    setIntegerForKey(const char* pKey, int value);
	void    setFloatForKey(const char* pKey, float value);
	void    setDoubleForKey(const char* pKey, double value);
	void    setStringForKey(const char* pKey, const std::string & value);

	static YJ_UserDefault* getInstance();
	static void destroyInstance();

	static const std::string& getXMLFilePath();
	static bool isXMLFileExist();

private:
	static bool createXMLFile();
	static void initXMLFilePath();

	static std::string getValueForKey(const char* pKey, const char* pValue);
	static void setValueForKey(const char* pKey, const char* pValue);

	static YJ_UserDefault* _userDefault;
	static std::string _filePath;
	static bool _isFilePathInitialized;
	static sqlite3* _pdb ;
	static const char* _sPassWord;
};
#endif 
