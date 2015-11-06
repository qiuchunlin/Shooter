#ifndef _SPRITE_MONSTER_MONSTER_
#define _SPRITE_MONSTER_MONSTER_
#include "cocos2d.h"
#include "../Common/Config.h"
#include "../Utils/AStar.h"
USING_NS_CC;
using namespace std;
using namespace Config;

class Monster : public Sprite
{
public:
	CREATE_FUNC(Monster);

	CC_SYNTHESIZE(Sprite*, _pSprite, Sprite);
	CC_SYNTHESIZE(MonsterStatus, _emStatus, Status);
	CC_SYNTHESIZE(MonsterType, _emType, MonsterType);
	virtual void run();

	void hurt(float fAtk);
protected:
	bool init();
	
	virtual void attack();
	virtual void die();
	void update(float dt);

	void runAway();

protected:

	void autoMove();//人物走动

	int getDirection(float angle);

	Vec2 getRandomTarget(Vec2 target);

protected:

	int m_playerMoveStep;//人物当前的行程的索引

	AStarInfo* _AStarInfo;

	Sprite* _pHpProgress;
	
	int _nLastDirection = 0;

	int _nTotalHp = 20;

	int _nCurHp = _nTotalHp;

	TMXTiledMap* _pBackGround;

	float _fSpeed = 150;
	int _nTarPosIndex = 1;
};
#endif