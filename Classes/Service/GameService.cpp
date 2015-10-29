#include "GameService.h"
USING_NS_CC;

GameService::GameService()
{
	_emOperate = Operate_Left;
}

GameService::~GameService()
{
}


GameService* GameService::getInstance()
{
	static GameService instance;

	return &instance;
}