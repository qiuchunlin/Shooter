#ifndef _UTILS_LOCALIZATIONMGR_
#define _UTILS_LOCALIZATIONMGR_
#include "cocos2d.h"
USING_NS_CC;

class LocalizationMgr
{
public:
	~LocalizationMgr();

	static LocalizationMgr* getInstance();

	void loadDataToDictionary();

	const char* getLocalizedString(const char* key);

private:
	LocalizationMgr();
	__Dictionary* _pDict;
	
};
#endif