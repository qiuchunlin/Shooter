#include "Joystick.h"
#include "ServerLayer.h"
#include "Service/GameService.h"
bool Joystick::init()
{
	if (!Node::create())
		return false;

	_lastPos = Vec2::ZERO;

	_frame = Sprite::create("map/map_0009.png");
	addChild(_frame);

	_stick = Sprite::create("map/7_Gold.png");
	_stick->setPosition(_frame->getContentSize().width / 2, _frame->getContentSize().height / 2);
	_frame->addChild(_stick);

	_controlDiameter = _frame->getContentSize().width - _stick->getContentSize().width;

	_touchEventListener = EventListenerTouchOneByOne::create();
	_touchEventListener->onTouchBegan = CC_CALLBACK_2(Joystick::onTouchBegan, this);
	_touchEventListener->onTouchMoved = CC_CALLBACK_2(Joystick::onTouchMoved, this);
	_touchEventListener->onTouchEnded = CC_CALLBACK_2(Joystick::onTouchEnded, this);
	_touchEventListener->onTouchCancelled = CC_CALLBACK_2(Joystick::onTouchCancelled, this);
	_touchEventListener->setSwallowTouches(true);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_touchEventListener, this);

	this->scheduleUpdate();

	return true;
}


bool Joystick::onTouchBegan(Touch* touch, Event* event)
{
	this->setPosition(touch->getLocation());
	//if (this->getBoundingBox().containsPoint(touch->getLocation()))
	{
		handleTouchChange(touch);
		return true;
	}

	return false;
}

void Joystick::onTouchMoved(Touch* touch, Event* event)
{
	//if (this->getBoundingBox().containsPoint(touch->getLocation()))
		handleTouchChange(touch);
}

void Joystick::onTouchEnded(Touch* touch, Event* event)
{
	static Vec2 origin = Vec2(_frame->getContentSize().width / 2, _frame->getContentSize().height / 2);

	_stick->setPosition(origin);


	_lastPos = Vec2::ZERO;
}

void Joystick::onTouchCancelled(Touch* touch, Event* event)
{
	onTouchEnded(touch, event);
}

void Joystick::handleTouchChange(Touch* touch)
{
	static Vec2 origin = Vec2(_frame->getContentSize().width / 2, _frame->getContentSize().height / 2);
	static float bigR = _frame->getContentSize().width / 2;
	static float smallR = _stick->getContentSize().width / 2;

	
	Vec2 hit = this->_frame->convertToNodeSpaceAR(touch->getLocation());
	if (hit.getDistance(Vec2::ZERO) + smallR > bigR)
	{
		float x = (bigR - smallR) / sqrt(1 + hit.y * hit.y / hit.x / hit.x);
		float y = abs(hit.y / hit.x * x);

		if (hit.x > 0)
		{
			if (hit.y > 0)
			{
				hit.x = x;
				hit.y = y;
			}
			else
			{
				hit.x = x;
				hit.y = -y;
			}
		}
		else
		{
			if (hit.y > 0)
			{
				hit.x = -x;
				hit.y = y;
			}
			else
			{
				hit.x = -x;
				hit.y = -y;
			}
		}
	}

	_stick->setPosition(hit + origin);

	_lastPos = hit;
}

void Joystick::update(float delta)
{
	float bigR = _frame->getContentSize().width / 2;
	Vec2 centerPos = Vec2(bigR, bigR);
	Vec2 stickPos = _stick->getPosition();
	float fY = stickPos.y - centerPos.y;
	float fX = stickPos.x - centerPos.x;

	float fAngle = 0;
	if (fX < 0 && fY > 0)
	{
		fAngle = atanf(fY / fX) * 180.0f / M_PI + 180;
	}
	else if (fX < 0 && fY < 0)
	{
		fAngle = atanf(fY / fX) * 180.0f / M_PI + 180;
	}
	else if (fX > 0 && fY < 0)
	{
		fAngle = atanf(fY / fX) * 180.0f / M_PI + 360;
	}
	else if (fX >0 && fY > 0)
	{
		fAngle = atanf(fY / fX) * 180.0f / M_PI;
	}
	if (fX == 0 && fY > 0)
	{
		fAngle = 90;
	}
	else if (fX == 0 && fY < 0)
	{
		fAngle = 270;
	}

	if (fX != 0 || fY !=0)
	{
		GameService::getInstance()->getServerLayer()->run(fAngle);

		CCLOG("%f", fAngle);
	}
	else
	{
		GameService::getInstance()->getServerLayer()->Stand();
	}
}