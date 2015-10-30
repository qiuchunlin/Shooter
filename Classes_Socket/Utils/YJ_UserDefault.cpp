#include "YJ_UserDefault.h"
#include "platform/CCCommon.h"
#include "platform/CCFileUtils.h"
#include "tinyxml2/tinyxml2.h"
#include "base/base64.h"
#include "base/ccUtils.h"
#include "Utils.h"

#define USERDEFAULT_ROOT_NAME    "userDefaultRoot"

#define XML_FILE_NAME "YJ_UserDefault.xml"

static tinyxml2::XMLElement* getXMLNodeForKey(const char* pKey, tinyxml2::XMLElement** rootNode, tinyxml2::XMLDocument **doc)
{
	tinyxml2::XMLElement* curNode = nullptr;

	if (!pKey)
	{
		return nullptr;
	}

	do
	{
		tinyxml2::XMLDocument* xmlDoc = new tinyxml2::XMLDocument();
		*doc = xmlDoc;

		std::string xmlBuffer = FileUtils::getInstance()->getStringFromFile(YJ_UserDefault::getInstance()->getXMLFilePath());

		//数据加密
		//xmlBuffer = Utils::decode(xmlBuffer);

		if (xmlBuffer.empty())
		{
			CCLOG("can not read xml file");
			break;
		}
		xmlDoc->Parse(xmlBuffer.c_str(), xmlBuffer.size());

		// get root node
		*rootNode = xmlDoc->RootElement();
		if (nullptr == *rootNode)
		{
			CCLOG("read root node error");
			break;
		}
		// find the node
		curNode = (*rootNode)->FirstChildElement();
		while (nullptr != curNode)
		{
			const char* nodeName = curNode->Value();
			if (!strcmp(nodeName, pKey))
			{
				break;
			}

			curNode = curNode->NextSiblingElement();
		}
	} while (0);

	return curNode;
}

void YJ_UserDefault::setValueForKey(const char* pKey, const char* pValue)
{
	std::string sKey = pKey;
	std::string sValue = pValue;
	std::string path = FileUtils::getInstance()->getWritablePath() + "game.db3";
	
	char ** selectResult;//查询结果
	int row, column;//行、列
	std::string sSql = "";

	int result = -1;
	if (_pdb == nullptr)
	{
		result = sqlite3_open(path.c_str(), &_pdb);
		sqlite3_key(_pdb, _sPassWord, sizeof(_sPassWord));
	}
	else
	{
		result = SQLITE_OK;
	}

	if (result == SQLITE_OK)
	{
		sSql = "select "+sKey+" from User";
		result = sqlite3_get_table(_pdb, sSql.c_str(), &selectResult, &row, &column, NULL);
		bool bIsUpdate = false;
		if (result == SQLITE_OK)
		{
			if (sValue != selectResult[1])
			{
				bIsUpdate = true;
			}
		}
		else if (result == SQLITE_ERROR)
		{
			//创建该列
			sSql = "alter table User add " + sKey + " text";
			result = sqlite3_exec(_pdb, sSql.c_str(), NULL, NULL, NULL);
			bIsUpdate = true;
		}
		if (bIsUpdate)
		{
			sSql = "update User set " + sKey + " = '" + sValue + "' where id = 1";
			result = sqlite3_exec(_pdb, sSql.c_str(), NULL, NULL, NULL);
		}
		
		
	}
//	sqlite3_close(_pdb);
//	_pdb = nullptr;
	
	//XML 存储
	//tinyxml2::XMLElement* rootNode;
	//tinyxml2::XMLDocument* doc;
	//tinyxml2::XMLElement* node;
	//// check the params
	//if (!pKey || !pValue)
	//{
	//	return;
	//}
	//// find the node
	//node = getXMLNodeForKey(pKey, &rootNode, &doc);
	//// if node exist, change the content
	//if (node)
	//{
	//	if (node->FirstChild())
	//	{
	//		node->FirstChild()->SetValue(pValue);
	//	}
	//	else
	//	{
	//		tinyxml2::XMLText* content = doc->NewText(pValue);
	//		node->LinkEndChild(content);
	//	}
	//}
	//else
	//{
	//	if (rootNode)
	//	{
	//		tinyxml2::XMLElement* tmpNode = doc->NewElement(pKey);//new tinyxml2::XMLElement(pKey);
	//		rootNode->LinkEndChild(tmpNode);
	//		tinyxml2::XMLText* content = doc->NewText(pValue);//new tinyxml2::XMLText(pValue);
	//		tmpNode->LinkEndChild(content);
	//	}
	//}

	//// save file and free doc
	//if (doc)
	//{
	//	//数据加密
	//	/*tinyxml2::XMLPrinter printer;
	//	doc->Print(&printer);
	//	string sOutValue = printer.CStr();
	//	sOutValue = Utils::encode(sOutValue);
	//	Utils::writeStringToFile(sOutValue, YJ_UserDefault::getInstance()->getXMLFilePath().c_str());*/
	//	doc->SaveFile(YJ_UserDefault::getInstance()->getXMLFilePath().c_str());
	//	delete doc;
	//}
}

std::string YJ_UserDefault::getValueForKey(const char* pKey, const char* pValue)
{
	std::string sKey = pKey;
	std::string sValue = pValue;
	std::string path = FileUtils::getInstance()->getWritablePath() + "game.db3";

	char ** selectResult;//查询结果
	int row, column;//行、列
	std::string sSql = "";

	int result = -1;
	if (_pdb == nullptr)
	{
		result = sqlite3_open(path.c_str(), &_pdb);
		sqlite3_key(_pdb, _sPassWord, sizeof(_sPassWord));
	}
	else
	{
		result = SQLITE_OK;
	}

	if (result == SQLITE_OK)
	{
		sSql = "select " + sKey + " from User";
		result = sqlite3_get_table(_pdb, sSql.c_str(), &selectResult, &row, &column, NULL);
		if (result == SQLITE_OK)
		{
			sValue = selectResult[1];
		}
	}
	
	return sValue;
}



YJ_UserDefault* YJ_UserDefault::_userDefault = nullptr;
string YJ_UserDefault::_filePath = "";
bool YJ_UserDefault::_isFilePathInitialized = false;
const char*  YJ_UserDefault::_sPassWord = "yujue2015zxcvb";
sqlite3* YJ_UserDefault::_pdb = nullptr;

bool YJ_UserDefault::getBoolForKey(const char* pKey, bool defaultValue)
{
	std::string sDefaultValue = "false";
	if (defaultValue)
	{
		sDefaultValue = "true";
	}
	sDefaultValue = getValueForKey(pKey, sDefaultValue.c_str());

	if (sDefaultValue == "true")
	{
		defaultValue = true;
	}
	else
	{
		defaultValue = false;
	}

	return defaultValue;


	//const char* value = nullptr;
	//tinyxml2::XMLElement* rootNode;
	//tinyxml2::XMLDocument* doc;
	//tinyxml2::XMLElement* node;
	//node = getXMLNodeForKey(pKey, &rootNode, &doc);
	//// find the node
	//if (node && node->FirstChild())
	//{
	//	value = (const char*)(node->FirstChild()->Value());
	//}

	//bool ret = defaultValue;

	//if (value)
	//{
	//	ret = (!strcmp(value, "true"));
	//}

	//if (doc) delete doc;

	//return ret;
}

int YJ_UserDefault::getIntegerForKey(const char* pKey, int defaultValue)
{
	std::string sDefaultValue = Utils::TtoS(defaultValue);
	
	sDefaultValue = getValueForKey(pKey, sDefaultValue.c_str());

	defaultValue = atoi(sDefaultValue.c_str());

	return defaultValue;

	//const char* value = nullptr;
	//tinyxml2::XMLElement* rootNode;
	//tinyxml2::XMLDocument* doc;
	//tinyxml2::XMLElement* node;
	//node = getXMLNodeForKey(pKey, &rootNode, &doc);
	//// find the node
	//if (node && node->FirstChild())
	//{
	//	value = (const char*)(node->FirstChild()->Value());
	//}

	//int ret = defaultValue;

	//if (value)
	//{
	//	ret = atoi(value);
	//}

	//if (doc)
	//{
	//	delete doc;
	//}


	//return ret;
}

float YJ_UserDefault::getFloatForKey(const char* pKey, float defaultValue)
{
	float ret = (float)getDoubleForKey(pKey, (double)defaultValue);

	return ret;
}

double YJ_UserDefault::getDoubleForKey(const char* pKey, double defaultValue)
{
	std::string sDefaultValue = Utils::TtoS(defaultValue);

	sDefaultValue = getValueForKey(pKey, sDefaultValue.c_str());

	defaultValue = atof(sDefaultValue.c_str());

	return defaultValue;

	//const char* value = nullptr;
	//tinyxml2::XMLElement* rootNode;
	//tinyxml2::XMLDocument* doc;
	//tinyxml2::XMLElement* node;
	//node = getXMLNodeForKey(pKey, &rootNode, &doc);
	//// find the node
	//if (node && node->FirstChild())
	//{
	//	value = (const char*)(node->FirstChild()->Value());
	//}

	//double ret = defaultValue;

	//if (value)
	//{
	//	ret = utils::atof(value);
	//}

	//if (doc) delete doc;

	//return ret;
}

string YJ_UserDefault::getStringForKey(const char* pKey, const std::string & defaultValue)
{
	std::string sDefaultValue = defaultValue;

	sDefaultValue = getValueForKey(pKey, sDefaultValue.c_str());

	return sDefaultValue;

	//const char* value = nullptr;
	//tinyxml2::XMLElement* rootNode;
	//tinyxml2::XMLDocument* doc;
	//tinyxml2::XMLElement* node;
	//node = getXMLNodeForKey(pKey, &rootNode, &doc);
	//// find the node
	//if (node && node->FirstChild())
	//{
	//	value = (const char*)(node->FirstChild()->Value());
	//}

	//string ret = defaultValue;

	//if (value)
	//{
	//	ret = string(value);
	//}

	//if (doc) delete doc;

	//return ret;
}

void YJ_UserDefault::setBoolForKey(const char* pKey, bool value)
{
	// save bool value as string

	if (true == value)
	{
		setStringForKey(pKey, "true");
	}
	else
	{
		setStringForKey(pKey, "false");
	}
}

void YJ_UserDefault::setIntegerForKey(const char* pKey, int value)
{
	// check key
	if (!pKey)
	{
		return;
	}

	// format the value
	char tmp[50];
	memset(tmp, 0, 50);
	sprintf(tmp, "%d", value);

	setValueForKey(pKey, tmp);
}

void YJ_UserDefault::setFloatForKey(const char* pKey, float value)
{
	setDoubleForKey(pKey, value);
}

void YJ_UserDefault::setDoubleForKey(const char* pKey, double value)
{
	// check key
	if (!pKey)
	{
		return;
	}

	// format the value
	char tmp[50];
	memset(tmp, 0, 50);
	sprintf(tmp, "%f", value);

	setValueForKey(pKey, tmp);
}

void YJ_UserDefault::setStringForKey(const char* pKey, const std::string & value)
{
	// check key
	if (!pKey)
	{
		return;
	}

	setValueForKey(pKey, value.c_str());
}

YJ_UserDefault* YJ_UserDefault::getInstance()
{
	//initXMLFilePath();

	//// only create xml file one time
	//// the file exists after the program exit
	//if ((!isXMLFileExist()) && (!createXMLFile()))
	//{
	//	return nullptr;
	//}

	if (!_userDefault)
	{
		_userDefault = new (std::nothrow) YJ_UserDefault();
		//初始化数据库
		sqlite3 *pdb = NULL;
		std::string path = FileUtils::getInstance()->getWritablePath() + "game.db3";
		int result = sqlite3_open(path.c_str(), &pdb);
		sqlite3_rekey(pdb, _sPassWord, sizeof(_sPassWord));

		if (result == SQLITE_OK)
		{
			string sSql = "select id from User";
			char ** selectResult;//查询结果
			int row, column;//行、列
			result = sqlite3_get_table(pdb, sSql.c_str(), &selectResult, &row, &column, NULL);

			if (result == SQLITE_ERROR)
			{
				sSql = "create table User(id integer primary key autoincrement)";
				//第一次创建表
				result = sqlite3_exec(pdb, sSql.c_str(), NULL, NULL, NULL);
				//创建用户数据
				sSql = "insert into User  values(1)";
				result = sqlite3_exec(pdb, sSql.c_str(), NULL, NULL, NULL);
			}
		}
		sqlite3_close(pdb);
	}

	return _userDefault;
}

void YJ_UserDefault::destroyInstance()
{
	CC_SAFE_DELETE(_userDefault);
}

bool YJ_UserDefault::isXMLFileExist()
{
	FILE *fp = fopen(_filePath.c_str(), "r");
	bool bRet = false;

	if (fp)
	{
		bRet = true;
		fclose(fp);
	}

	return bRet;
}

void YJ_UserDefault::initXMLFilePath()
{
	if (!_isFilePathInitialized)
	{
		_filePath += FileUtils::getInstance()->getWritablePath() + XML_FILE_NAME;
		_isFilePathInitialized = true;
	}
}

// create new xml file
bool YJ_UserDefault::createXMLFile()
{
	bool bRet = false;
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	if (nullptr == pDoc)
	{
		return false;
	}
	tinyxml2::XMLDeclaration *pDeclaration = pDoc->NewDeclaration(nullptr);
	if (nullptr == pDeclaration)
	{
		return false;
	}
	pDoc->LinkEndChild(pDeclaration);
	tinyxml2::XMLElement *pRootEle = pDoc->NewElement(USERDEFAULT_ROOT_NAME);
	if (nullptr == pRootEle)
	{
		return false;
	}
	pDoc->LinkEndChild(pRootEle);

	//数据加密
	/*tinyxml2::XMLPrinter printer;
	pDoc->Print(&printer);
	string sOutValue = printer.CStr();
	sOutValue = Utils::encode(sOutValue);
	Utils::writeStringToFile(sOutValue, _filePath.c_str());

	bRet = tinyxml2::XML_SUCCESS;*/

	bRet = tinyxml2::XML_SUCCESS == pDoc->SaveFile(_filePath.c_str());

	if (pDoc)
	{
		delete pDoc;
	}

	return bRet;
}

const string& YJ_UserDefault::getXMLFilePath()
{
	return _filePath;
}

//#endif // (CC_TARGET_PLATFORM != CC_PLATFORM_IOS && CC_PLATFORM != CC_PLATFORM_ANDROID)
