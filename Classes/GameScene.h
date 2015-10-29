#ifndef __GAME_BASE_H__
#define __GAME_BASE_H__

#include "cocos2d.h"
USING_NS_CC;

#include "Sprite/Monster.h"
#include "Sprite/Hero.h"
#include "Sprite/Bubble.h"



class Joystick;
class GameScene : public Layer
{
public:
	static cocos2d::Scene* createScene();

	CREATE_FUNC(GameScene);
	CC_SYNTHESIZE(Hero*, _pHero, Hero);
	CC_SYNTHESIZE(TMXTiledMap*, _pBackGround, BackGround);
	CC_SYNTHESIZE(Vector<Bubble*>, _vBubbles, Bubbles);
	CC_SYNTHESIZE(Vector<Monster*>, _vMonsters, Monsters);
	CC_SYNTHESIZE(vector<Rect>, collideRects, CollideRects);
	CC_SYNTHESIZE(Joystick*, _pLeftJoystick, LeftJoystick);
	CC_SYNTHESIZE(Joystick*, _pRightJoystick, RightJoystick);
	
	void addBubble(Bubble* pBubble);
	void removeBubble(Bubble* pBubble);
	void killMonster(Monster* pMonster);
	PathSearchInfo* _pPathInfo;
	void updateMonster();
protected:
	void update(float delta);
	void initUI();
	void initTouch();
	void initHero();

	virtual bool init();
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
	
private:

};

#endif 
