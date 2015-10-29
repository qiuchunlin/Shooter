#ifndef __SuperMario__GameTouch__
#define __SuperMario__GameTouch__

#include "cocos2d.h"
using namespace cocos2d;
class Joystick : public Node
{
public:
	static Joystick* create(bool _bIsLeft);
	CC_SYNTHESIZE(bool, _bIsTouched, IsTouched);

protected:
	virtual bool init();
	void update(float delta);

private:
	Sprite* _stick;
	Sprite* _frame;
	Vec2 _lastPos;
	bool _bIsLeft = false;
	EventListenerTouchOneByOne* _touchEventListener;


	bool onTouchBegan(Touch*, Event*);
	void onTouchMoved(Touch*, Event*);
	void onTouchEnded(Touch*, Event*);
	void onTouchCancelled(Touch*, Event*);

	void handleTouchChange(Touch*);
};
#endif /* defined(__SuperMario__GameTouch__) */
