#ifndef __GAME_BASE_H__
#define __GAME_BASE_H__

#include "cocos2d.h"
USING_NS_CC;

enum DataType
{
	RUN,
	STAND,
	POSITION
};

struct GameData
{
	int dataSize;
	DataType dataType;	
	Vec2 position;
};

class GameBase : public Layer
{
public:
	virtual bool init();
	void onEnter();
	void onExit();

	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);

protected:
	void initUI();
	void initTouch();
	void initEnemy();
	virtual void initNetwork() = 0;

	EventListenerTouchOneByOne* _touchListener;
	Animate *_runAnim, *_standAnim;
	Sprite* _enemy;
	Label* _lblInfo;
	float ENEMY_MOVE_SPEED;
};

#endif 
