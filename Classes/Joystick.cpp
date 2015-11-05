#include "Joystick.h"
#include "GameScene.h"
#include "Service/GameService.h"
#include "Utils/Utils.h"

Joystick* Joystick::create(bool bIsLeft)
{ 
	Joystick *pRet = new(std::nothrow) Joystick();
	pRet->_bIsLeft = bIsLeft;
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool Joystick::init()
{
	if (!Node::create())
		return false;

	_lastPos = Vec2::ZERO;
	_bIsTouched = false;

	_frame = Sprite::create("images/map/rocker_0002_01.png");
	addChild(_frame);

	_stick = Sprite::create("images/map/rocker_0001_01.png");
	_stick->setPosition(_frame->getContentSize().width / 2, _frame->getContentSize().height / 2);
	_frame->addChild(_stick);

	_touchEventListener = EventListenerTouchOneByOne::create();
	_touchEventListener->onTouchBegan = CC_CALLBACK_2(Joystick::onTouchBegan, this);
	_touchEventListener->onTouchMoved = CC_CALLBACK_2(Joystick::onTouchMoved, this);
	_touchEventListener->onTouchEnded = CC_CALLBACK_2(Joystick::onTouchEnded, this);
	_touchEventListener->onTouchCancelled = CC_CALLBACK_2(Joystick::onTouchCancelled, this);
	_touchEventListener->setSwallowTouches(true);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_touchEventListener, this);

	this->setContentSize(_frame->getContentSize());
	
	this->scheduleUpdate();

	return true;
}


bool Joystick::onTouchBegan(Touch* touch, Event* event)
{
	Rect checkRect = Rect(this->getPosition().x - this->getContentSize().width / 2, this->getPosition().y - this->getContentSize().height/2, this->getContentSize().width, this->getContentSize().height);
	if (checkRect.containsPoint(touch->getLocation()))
	{
		if (_bIsLeft && !GameService::getInstance()->getGameScene()->getRightJoystick()->getIsTouched())
		{
			GameService::getInstance()->setCurOperate(Operate_Left);
		}
		else if (_bIsLeft && GameService::getInstance()->getGameScene()->getRightJoystick()->getIsTouched())
		{
			GameService::getInstance()->setCurOperate(Operate_Both);
		}
		else if (!_bIsLeft && !GameService::getInstance()->getGameScene()->getLeftJoystick()->getIsTouched())
		{
			GameService::getInstance()->setCurOperate(Operate_Right);
		}
		else if (!_bIsLeft && GameService::getInstance()->getGameScene()->getLeftJoystick()->getIsTouched())
		{
			GameService::getInstance()->setCurOperate(Operate_Both);
		}
		handleTouchChange(touch);

		if (GameService::getInstance()->getCurOperate() == Operate_Left)
		{
			GameService::getInstance()->getGameScene()->getHero()->autoFire();
		}
		else if (GameService::getInstance()->getCurOperate() == Operate_Right || GameService::getInstance()->getCurOperate() == Operate_Both)
		{
			GameService::getInstance()->getGameScene()->getHero()->handFire();
		}
		_bIsTouched = true;
		return true;
	}

	return false;
}

void Joystick::onTouchMoved(Touch* touch, Event* event)
{
	handleTouchChange(touch);
}

void Joystick::onTouchEnded(Touch* touch, Event* event)
{
	static Vec2 origin = Vec2(_frame->getContentSize().width / 2, _frame->getContentSize().height / 2);

	if (_bIsLeft && !GameService::getInstance()->getGameScene()->getRightJoystick()->getIsTouched())
	{
		GameService::getInstance()->setCurOperate(Operate_None);
	}
	else if (_bIsLeft && GameService::getInstance()->getGameScene()->getRightJoystick()->getIsTouched())
	{
		GameService::getInstance()->setCurOperate(Operate_Right);
	}
	else if (!_bIsLeft && !GameService::getInstance()->getGameScene()->getLeftJoystick()->getIsTouched())
	{
		GameService::getInstance()->setCurOperate(Operate_None);
	}
	else if (!_bIsLeft && GameService::getInstance()->getGameScene()->getLeftJoystick()->getIsTouched())
	{
		GameService::getInstance()->setCurOperate(Operate_Left);
	}
	handleTouchChange(touch);

	if (GameService::getInstance()->getCurOperate() == Operate_Left)
	{
		GameService::getInstance()->getGameScene()->getHero()->autoFire();
	}
	else if (GameService::getInstance()->getCurOperate() == Operate_Right)
	{
		GameService::getInstance()->getGameScene()->getHero()->handFire();
	}
	else if (GameService::getInstance()->getCurOperate() == Operate_None)
	{
		GameService::getInstance()->getGameScene()->getHero()->stand();
	}

	_stick->setPosition(origin);
	_lastPos = Vec2::ZERO;
	_bIsTouched = false;
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
		if (hit.x == 0)
		{
			y = bigR - smallR;
		}

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

	Vec2 targetPos = hit + origin;
	_stick->setPosition(targetPos);

	_lastPos = hit;
}

void Joystick::update(float delta)
{
	float bigR = _frame->getContentSize().width / 2;
	Vec2 centerPos = Vec2(bigR, bigR);
	Vec2 stickPos = _stick->getPosition();
	float fY = stickPos.y - centerPos.y;
	float fX = stickPos.x - centerPos.x;

	float fAngle = Utils::getPosAngle(stickPos, centerPos);

	if (fX != 0 || fY !=0)
	{
		if (GameService::getInstance()->getGameScene()->getHero()!=nullptr)
		{
			GameService::getInstance()->getGameScene()->getHero()->run(fAngle, _bIsLeft);
		}
	}
}