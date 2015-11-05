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
	this->scheduleUpdate();
    return true;
}

void Bubble::update(float dt)
{
	Vec2 bubblePos = this->getPosition();
	bubblePos = GameService::getInstance()->getGameScene()->getBackGround()->convertToNodeSpace(bubblePos);
	vector<Rect> collideRect = GameService::getInstance()->getGameScene()->getCollideRects();
	for (auto& rect : collideRect)
	{
		if (rect.containsPoint(bubblePos))
		{
			GameService::getInstance()->getGameScene()->removeBubble(this);
			this->removeFromParentAndCleanup(true);
			break;
		}
	}
}
