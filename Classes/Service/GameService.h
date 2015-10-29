#ifndef _SERVICE_GAMESERVICE_
#define _SERVICE_GAMESERVICE_
#include "cocos2d.h"
USING_NS_CC;
#include "../Common/Config.h"
using namespace Config;
using namespace std;
class GameScene;
class GameService
{
 public:
	static GameService* getInstance();
	~GameService();
	
	CC_SYNTHESIZE(GameScene*, _pGameScene, GameScene);
	CC_SYNTHESIZE(OperateModel, _emOperate, CurOperate);
private:
	GameService();
private:
};

#endif