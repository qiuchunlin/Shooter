#ifndef _SPRITE_HERO_HERO_
#define _SPRITE_HERO_HERO_
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

enum HeroStatus
{
	RUN,
	STAND,
	ATK,
	POSITION
};


class Hero : public Sprite
{
public:
	CREATE_FUNC(Hero);

	CC_SYNTHESIZE(Sprite*, _pSprite, Sprite);

	void run(float angle,bool bIsDirection);
	void stand();
	void handFire();
	
	void autoFire();

private:
	void update(float dt);
	void findMonsterFire();
	void fire(float fAngle);
    bool init();
	int getDirection(float fAngle);
	bool isTouched(Vec2 pos);
private:
	int _nLastDirection = 0;
	HeroStatus _emStatus;
	float _fAngle = 0;
	Sprite* _pFire = nullptr;
	Vec2 _runTargetPos = Vec2::ZERO;
	float _fMinGapTime = 0;

};
#endif