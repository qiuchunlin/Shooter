#include "GameBase.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocostudio::timeline;

bool GameBase::init()
{
	if ( !Layer::init() )
	{
		return false;
	}

	this->initUI();
	this->initTouch();
	this->initEnemy();
	this->initNetwork();

	return true;
}

void GameBase::initUI()
{
	auto size = Director::getInstance()->getVisibleSize();
	_lblInfo = Label::createWithSystemFont("This is GameBase", "Arial", 24);
	this->addChild(_lblInfo);
	_lblInfo->setPosition(Vec2(size.width * 0.5f, size.height * 0.8f));
}

void GameBase::initTouch()
{
	_touchListener = EventListenerTouchOneByOne::create();
	_touchListener->onTouchBegan = CC_CALLBACK_2(GameBase::onTouchBegan, this);
	_touchListener->onTouchMoved = CC_CALLBACK_2(GameBase::onTouchMoved, this);
	_touchListener->onTouchEnded = CC_CALLBACK_2(GameBase::onTouchEnded, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_touchListener, this);
}

void GameBase::initEnemy()
{
	ENEMY_MOVE_SPEED = 100.f;
	auto runAnimation = Animation::create();
	for (int i=0; i<5; ++i)
	{
		auto fileName = StringUtils::format("enemy/run%d.png", i);
		runAnimation->addSpriteFrameWithFile(fileName);
	}
	runAnimation->setDelayPerUnit(0.1f);
	_runAnim = Animate::create(runAnimation);
	_runAnim->retain();

	auto standAnimation = Animation::create();
	for (int i=0; i<3; ++i)
	{
		auto fileName = StringUtils::format("enemy/stand%d.png", i);
		standAnimation->addSpriteFrameWithFile(fileName);
	}
	standAnimation->setDelayPerUnit(0.2f);
	_standAnim = Animate::create(standAnimation);
	_standAnim->retain();

	_enemy = Sprite::create();
	this->addChild(_enemy);
	_enemy->runAction(RepeatForever::create(_standAnim));
}

void GameBase::onEnter()
{
	Layer::onEnter();
}

void GameBase::onExit()
{
	Director::getInstance()->getEventDispatcher()->removeEventListener(_touchListener);
	Layer::onExit();
}

bool GameBase::onTouchBegan(Touch *touch, Event *unused_event)
{
	return true;
}

void GameBase::onTouchMoved(Touch *touch, Event *unused_event)
{

}

void GameBase::onTouchEnded(Touch *touch, Event *unused_event)
{

}