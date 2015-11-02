#ifndef _COMMON_CONFIG_CONFIG_
#define _COMMON_CONFIG_CONFIG_
#include "cocos2d.h"
using namespace std;
USING_NS_CC;
namespace Config{
	enum OperateModel
	{
		Operate_None,//������
		Operate_Left,//��ҡ��
		Operate_Right,//��ҡ��
		Operate_Both,//˫ҡ��
	};

	enum MonsterStatus
	{
		Monster_Run,
		Monster_Stand,
		Monster_Attack,
		Monster_Die,
	};

	enum MonsterType
	{
		Type_Boss,
		Type_Dog,
	};
	
}
#endif
