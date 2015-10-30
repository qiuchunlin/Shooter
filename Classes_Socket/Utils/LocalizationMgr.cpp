#include "LocalizationMgr.h"

LocalizationMgr::LocalizationMgr()
{

}

LocalizationMgr::~LocalizationMgr()
{
	CC_SAFE_RELEASE_NULL(_pDict);
}

void LocalizationMgr::loadDataToDictionary()
{
	if (Application::getInstance()->getCurrentLanguage() == LanguageType::CHINESE)
	{
		_pDict = Dictionary::createWithContentsOfFile("fonts/language_zh.plist");
	}
	else
	{
		_pDict = Dictionary::createWithContentsOfFile("fonts/language_en.plist");
	}
	
	_pDict->retain();
}

const char* LocalizationMgr::getLocalizedString(const char* key)
{
	const __String* pStr = _pDict->valueForKey(key);
	if (pStr)
	{
		return pStr->getCString();
	}
	else
	{
		pStr = __String::createWithFormat("%s", "undefined");
		return pStr->getCString();
	}
}

LocalizationMgr* LocalizationMgr::getInstance()
{
	static LocalizationMgr s_Instance;
	return &s_Instance;
}

