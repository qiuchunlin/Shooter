#ifndef _SPRITE_BUBBLE_
#define _SPRITE_BUBBLE_
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

class Bubble : public Sprite
{
public:
	CREATE_FUNC(Bubble);

	CC_SYNTHESIZE(Sprite*, _pSprite, Sprite);
	
protected:
	bool init();
	void update(float dt);
protected:
	
};
#endif