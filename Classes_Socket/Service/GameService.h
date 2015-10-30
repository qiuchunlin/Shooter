#ifndef _SERVICE_GAMESERVICE_
#define _SERVICE_GAMESERVICE_
#include "cocos2d.h"
USING_NS_CC;
using namespace std;
class ServerLayer;
class GameService
{
 public:
	static GameService* getInstance();
	~GameService();
	
	CC_SYNTHESIZE(ServerLayer*, _pServerLayer, ServerLayer);
private:
	GameService();
private:
};

#endif