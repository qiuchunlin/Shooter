#include "GameService.h"
USING_NS_CC;

GameService::GameService()
{
	
}
GameService::~GameService()
{
}


GameService* GameService::getInstance()
{
	static GameService instance;

	return &instance;
}