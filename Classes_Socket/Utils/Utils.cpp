#include "Utils.h"
#include "BASE64/CheckMd5.h"
#include "BASE64/CBase64.h"
#include <random>
#include "LocalizationMgr.h"

using namespace std;

int Utils::random(int startIncluded, int endIncluded)
{
    random_device rd;
    default_random_engine e(rd());
    uniform_int_distribution<int> uniform_dist(startIncluded, endIncluded);
    
    return uniform_dist(e);
}

bool Utils::randomPercent(float fSuccessPercent)
{
    int nSuccess = fSuccessPercent*100;
    int nTotal = 10000;
    if(Utils::random(1, nTotal) <= nSuccess){
        return true;
    }
    return  false;
}

SpriteFrame* Utils::createFrame(const string& fileName)
{
    SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
    SpriteFrame* frame = frameCache->getSpriteFrameByName(fileName);
    if (frame == nullptr) {
        Texture2D* texture = Director::getInstance()->getTextureCache()->addImage(fileName);
        Rect rect = Rect::ZERO;
        rect.size = texture->getContentSize();
        frame = CCSpriteFrame::createWithTexture(texture, rect);
        frameCache->addSpriteFrame(frame, fileName);
    }
    return frame;
}

string Utils::encode(const string & inValue)
{
    MD5		md5;
    string value = md5.MD5String(inValue.c_str());
    value = value + "," + inValue;
    
    return CBase64::encode64(value);
}

string Utils::decode(const string & inValue)
{
    MD5		md5;
    if (inValue!="")
    {
        string value=CBase64::decode64(inValue);
        
        int flag=value.find(",");
        string md5Str=value.substr(0,flag);
        string valueStr=value.substr(flag+1);
        
        if (md5Str==md5.MD5String(valueStr.c_str()))
        {
            return  valueStr;
        }
    }
    return "";
}

int Utils::getUtf8Len(char firstByte)
{
	const unsigned char kFirstBitMask = 128; // 1000000 
//	const unsigned char kSecondBitMask = 64; // 0100000
	const unsigned char kThirdBitMask = 32; // 0010000 
	const unsigned char kFourthBitMask = 16; // 0001000 
//	const unsigned char kFifthBitMask = 8; // 0000100

	std::string::difference_type offset = 1;
	if (firstByte & kFirstBitMask) // This means the first byte has a value greater than 127, and so is beyond the ASCII range.   
	{
		if (firstByte & kThirdBitMask) // This means that the first byte has a value greater than 224, and so it must be at least a three-octet code Vec2.     
		{
			if (firstByte & kFourthBitMask) // This means that the first byte has a value greater than 240, and so it must be a four-octet code Vec2.        
				offset = 4;
			else
				offset = 3;
		}
		else
		{
			offset = 2;
		}
	}
	return offset;
}

int Utils::getUtf8StrLen(const string & sString)
{
	int nCount = 0;
	unsigned int nIndex = 0;
	while (nIndex < sString.size())
	{
		nIndex += Utils::getUtf8Len(sString.at(nIndex));
		nCount++;
	}
	return nCount;
}

string Utils::splitUtf8StrByCount(const string & sString, int nCount)
{
	string newString = "";
	
	int nCurCount = 0;
	unsigned int nIndex = 0;
	unsigned int nFirstIndex = 0;
	while (nIndex < sString.size())
	{
		nIndex += Utils::getUtf8Len(sString.at(nIndex));
		nCurCount++;
		if (nCurCount >= nCount)
		{
			nCurCount = 0;
			newString.append(sString.substr(nFirstIndex, nIndex - nFirstIndex));
			newString.append("\n");
			nFirstIndex = nIndex;
		}
	}
	if (nFirstIndex < sString.size())
	{
		newString.append(sString.substr(nFirstIndex, sString.size()));
	}
	return newString;
}

string Utils::chageUtf8Str(const string & sString, const char* changeItem, const string& fChangeValue)
{
	string newString = "";
	unsigned int nIndex = 0;
	unsigned int nFirstIndex = 0;
	while (nIndex < sString.size())
	{
		nIndex += Utils::getUtf8Len(sString.at(nIndex));
		
		if (sString.substr(nFirstIndex, nIndex - nFirstIndex) == changeItem)
		{
			newString.append(fChangeValue);
		}
		else
		{
			newString.append(sString.substr(nFirstIndex, nIndex - nFirstIndex));
		}
		nFirstIndex = nIndex;
	}
	return newString;
}

int Utils::getTimeGap(time_t timeHead, time_t timeBottom)
{
    struct tm *tm;
    tm = localtime(&timeHead);
    
    int year1=tm->tm_year + 1900;
    int month1=tm->tm_mon + 1;
    int day1=tm->tm_mday;
    
    tm = localtime(&timeBottom);
    
    int year2=tm->tm_year + 1900;
    int month2=tm->tm_mon + 1;
    int day2=tm->tm_mday;
    
    int nd, nm, ny; //new_day, new_month, new_year
    int od, om, oy; //old_day, oldmonth, old_year
    
    nm = (month2 + 9) % 12;
    ny = year2 - nm/10;
    nd = 365*ny + ny/4 - ny/100 + ny/400 + (nm*306 + 5)/10 + (day2 - 1);
    
    om = (month1 + 9) % 12;
    oy = year1 - om/10;
    od = 365*oy + oy/4 - oy/100 + oy/400 + (om*306 + 5)/10 + (day1 - 1);
    
    return od - nd;
}

time_t Utils::time_maker(int date)
{
	struct tm t;
	t.tm_year = date / 10000 - 1900;
	t.tm_mon = (date - (t.tm_year + 1900) * 10000) / 100 - 1;
	t.tm_mday = date - (t.tm_year + 1900) * 10000 - (t.tm_mon + 1) * 100;
	t.tm_hour = 0;
	t.tm_min = 0;
	t.tm_sec = 1;
	t.tm_isdst = 0;
	time_t t_of_day = mktime(&t);
	return t_of_day;
}

int Utils::current_time(time_t& currenttime)
{
	struct tm * timeinfo;
	timeinfo = localtime(&currenttime);
	int year = timeinfo->tm_year + 1900;
	int month = timeinfo->tm_mon + 1;
	int day = timeinfo->tm_mday;
	int date_current = year * 10000 + month * 100 + day;
	return date_current;
}

string Utils::getStringFromFile(const string& filePath)
{
	//读文件
	FILE *fp = fopen(filePath.c_str(), "r");//根据路径打开文件

	char *pchBuf = NULL;//将要取得的字符串
	int  nLen = 0;//将要取得的字符串长度
	fseek(fp, 0, SEEK_END);//文件指针移到文件尾
	nLen = ftell(fp); //得到当前指针位置,即是文件的长度
	rewind(fp);   //文件指针恢复到文件头位置
	//动态申请空间,为保存字符串结尾标志\0,多申请一个字符的空间
	pchBuf = (char*)malloc(sizeof(char)*nLen + 1);
	if (!pchBuf)
	{
		exit(0);
	}
	//读取文件内容//读取的长度和源文件长度有可能有出入，这里自动调整 nLen
	nLen = fread(pchBuf, sizeof(char), nLen, fp);
	pchBuf[nLen] = '\0';//添加字符串结尾标志

	string detailStr = pchBuf;
	fclose(fp); //关闭文件
	free(pchBuf);//释放空间

	return detailStr;
}

void Utils::writeStringToFile(const string& srtContent, const string& filePath)
{
	string path = FileUtils::getInstance()->fullPathForFilename(filePath);
	if (path=="")
	{
		path = filePath;
	}
	FILE *fp = fopen(path.c_str(), "w");
	fputs(srtContent.c_str(), fp);
	fclose(fp);
}

Data Utils::getDataFromString(const string& srtContent)
{
	char *buf = new char[strlen(srtContent.c_str()) + 1];
	strcpy(buf, srtContent.c_str());
	Data retData;
	retData.fastSet((unsigned char*)buf, srtContent.size());
	return retData;
}


void Utils::childrenFadeIn(Node * pNode, float fFadeTime, float fDelayTime)
{
	string sName = pNode->getName();
	if (sName != "NoFadeIn")
	{
		pNode->setOpacity(0);
		pNode->runAction(Sequence::create(DelayTime::create(fDelayTime), FadeTo::create(fFadeTime, 255), NULL));
	}
	Vector<Node*> vChildren = pNode->getChildren();
	for (auto &pNode : vChildren)
	{
		Utils::childrenFadeIn(pNode, fFadeTime, fDelayTime);
	}
}

void Utils::childrenFadeOut(Node * pNode, float fFadeTime, float fDelayTime)
{
	pNode->runAction(Sequence::create(DelayTime::create(fDelayTime), FadeTo::create(fFadeTime, 0), NULL));
	Vector<Node*> vChildren = pNode->getChildren();
	for (auto &pNode : vChildren)
	{
		Utils::childrenFadeOut(pNode, fFadeTime, fDelayTime);
	}
}

void Utils::stopAllActions(Node * pNode)
{
	pNode->stopAllActions();
	Vector<Node*> vChildren = pNode->getChildren();
	for (auto &pNode : vChildren)
	{
		Utils::stopAllActions(pNode);
	}
}

string Utils::getShowTime(float fTime)
{
	string sDes = "";
	int nDay = fTime/(24*3600);
	int nHour = (fTime - nDay * 24 * 3600)/ 3600;
	int nMiu = (fTime - nDay * 24 * 3600 - nHour * 3600) / 60;
	if (nDay > 0)
	{
		sDes += Utils::TtoS(nDay) + LocalizationMgr::getInstance()->getLocalizedString("GAME_DAY");
	}

	if (nHour > 0 )
	{
		sDes += Utils::TtoS(nHour) + LocalizationMgr::getInstance()->getLocalizedString("GAME_HOUR");
	}

	if ((nDay > 0 || nHour > 0) && nMiu == 0)
	{
	}
	else
	{
		sDes += Utils::TtoS(nMiu) + LocalizationMgr::getInstance()->getLocalizedString("GAME_MIU");
	}
	

	return sDes;
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
string Utils::jstringToChar(JNIEnv* env, jstring jString)
{
	string sString = "";
	char* rtn = NULL;
	jclass clsstring = env->FindClass("java/lang/String");
	jstring strencode = env->NewStringUTF("utf-8");
	jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
	jbyteArray barr = (jbyteArray)env->CallObjectMethod(jString, mid, strencode);
	jsize alen = env->GetArrayLength(barr);
	jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
	if (alen > 0)
	{
		rtn = (char*)malloc(alen + 1);

		memcpy(rtn, ba, alen);
		rtn[alen] = 0;
	}
	env->ReleaseByteArrayElements(barr, ba, 0);

	if (rtn!=NULL)
	{
		sString = rtn;
	}
	return sString;
}
#endif