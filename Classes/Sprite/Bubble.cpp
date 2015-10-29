#include "Bubble.h"
#include "../Service/GameService.h"
#include "../GameScene.h"
#include "../common/Constants.h"
bool Bubble::init()
{
    if (!Sprite::init()) {
        return  false;
    }

	_pSprite = Sprite::create("images/hero/Bullet_00040.png");
	this->addChild(_pSprite);

	this->setContentSize(_pSprite->getContentSize());
	_pSprite->setPosition(Vec2(_pSprite->getContentSize().width / 2, _pSprite->getContentSize().height / 2));
    return true;
}

void Bubble::update(float dt)
{
	
}
