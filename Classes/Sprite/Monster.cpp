#include "Monster.h"
#include "../Service/GameService.h"
#include "../GameScene.h"
#include "../Utils/Utils.h"
#include "../Utils/LayoutUtil.h"
#include "../Common/Constants.h"

bool Monster::init()
{
    if (!Sprite::init()) {
        return  false;
    }
	_emStatus = Monster_Stand;
	_emType = Type_Dog;

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("images/monster/m_00080.plist", "images/monster/m_00080.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("images/hero/t_die_00020.plist", "images/hero/t_die_00020.png");
	

	_pBackGround = GameService::getInstance()->getGameScene()->getBackGround();
	_AStarInfo = GameService::getInstance()->getGameScene()->getAStarInfo()->clone();

	_pSprite = Sprite::createWithSpriteFrameName("m_s05_0008_0000.png");
	this->addChild(_pSprite);
	this->setContentSize(Size(60, 60));
	_pSprite->setPosition(Vec2(30, 30));

	Animation* standAnimation = Animation::create();
	for (int i = 0; i < 7; ++i)
	{
		string fileName = StringUtils::format("m_s05_0008_000%d.png", i);
		standAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(fileName));
	}
	standAnimation->setDelayPerUnit(0.1f);
	_pSprite->runAction(RepeatForever::create(Animate::create(standAnimation)));

	
	//血条
	auto pHpBg = Sprite::createWithSpriteFrameName("Game_Blood_Bg_2.png");
	this->addChild(pHpBg);
	LayoutUtil::layoutParentCenter(pHpBg,0, 30);

	_pHpProgress = Sprite::createWithSpriteFrameName("Game_Blood_Pro_2.png");
	_pHpProgress->setAnchorPoint(Vec2::ZERO);
	pHpBg->addChild(_pHpProgress);
	LayoutUtil::layoutParentCenter(_pHpProgress);

	this->scheduleUpdate();
    return true;
}

void Monster::run()
{
	if (_emStatus == Monster_Stand)
	{
		_emStatus = Monster_Run;
	}
	auto runAction = CallFunc::create(CC_CALLBACK_0(Monster::run, this));
	this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.1f), runAction));

	this->stopActionByTag(100);

	Vec2 heroPos = GameService::getInstance()->getGameScene()->getHero()->getHeroCheckPos();

	auto nodePosition = _pBackGround->convertToNodeSpace(heroPos);

	Vec2 monsterPos = this->getPosition();
	float fAngle = Utils::getPosAngle(nodePosition, monsterPos);
	int nDirection = this->getDirection(fAngle);
	if (fAngle > 90 && fAngle < 270)
	{
		_pSprite->setFlippedX(true);
	}
	else
	{
		_pSprite->setFlippedX(false);
	}

	if (_nLastDirection != nDirection)
	{
		_nLastDirection = nDirection;
		_pSprite->stopAllActions();
		Animation* standAnimation = Animation::create();
		for (int i = 0; i < 6; ++i)
		{
			string fileName = StringUtils::format("m_r0%d_0008_000%d.png", _nLastDirection, i);
			standAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(fileName));
		}
		standAnimation->setDelayPerUnit(0.1f);
		_pSprite->runAction(RepeatForever::create(Animate::create(standAnimation)));
	}



	Point startPos = Utils::getPosInMap(this->getPosition(), _pBackGround);

	_AStarInfo->_nStartX = startPos.x;
	_AStarInfo->_nStartY = startPos.y;
	_AStarInfo->clearPath();
	m_playerMoveStep = 0;

	nodePosition = Utils::getPosInMap(nodePosition, _pBackGround);
	PathItem* targetPath = _AStarInfo->getFromSearchList((int)nodePosition.x, (int)nodePosition.y);
	
	//获取触摸点, 设置为终点
	_AStarInfo->_nEndX = targetPath->_nX;
	_AStarInfo->_nEndY = targetPath->_nY;

	_AStarInfo->calculatePath(this);

	//移动物体
	autoMove();
}

int Monster::getDirection(float angle)
{
	int nCurDirection = 0;
	if ((angle >= 360 - 90 && angle <= 360 - 30) || (angle > 210 && angle <= 360 - 90))
	{
		nCurDirection = 1;
	}
	else if ((angle > 360 - 30 || angle <= 10) || (angle > 150 && angle <= 210))
	{
		nCurDirection = 3;
	}
	else if ((angle > 10 && angle <= 90) || (angle > 90 && angle <= 150))
	{
		nCurDirection = 5;
	}

	return nCurDirection;
}

void Monster::attack()
{
	_emStatus = Monster_Attack;
	this->stopAllActions();
	Animation* atkAnimation = Animation::create();
	for (int i = 1; i < 7; ++i)
	{
		string fileName = StringUtils::format("m_a0%d_0008_000%d.png", _nLastDirection, i);
		atkAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(fileName));
	}
	atkAnimation->setDelayPerUnit(0.1f);

	Animate* pAtkAni = Animate::create(atkAnimation);
	_pSprite->stopAllActions();
	_pSprite->runAction(Sequence::create(pAtkAni, CallFunc::create(CC_CALLBACK_0(Monster::runAway,this)), NULL));
	GameService::getInstance()->getGameScene()->getHero()->hurt(2);
}

void Monster::die()
{
	_emStatus = Monster_Die;
	GameService::getInstance()->getGameScene()->killMonster(this);

	Animation* dieAnimation = Animation::create();
	for (int i = 1; i < 10; ++i)
	{
		string fileName = StringUtils::format("t_die_0002_000%d.png", i);
		dieAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(fileName));
	}
	dieAnimation->setDelayPerUnit(0.1f);

	Animate* pDieAni = Animate::create(dieAnimation);

	auto releaseAction = CallFunc::create([&](){
		this->removeFromParentAndCleanup(true);
	});
	_pSprite->stopAllActions();
	this->stopAllActions();
	_pSprite->runAction(Sequence::create(pDieAni, FadeOut::create(0.5f), releaseAction, NULL));
}

void Monster::update(float dt)
{
	Vector<Bubble*> vBubbles = GameService::getInstance()->getGameScene()->getBubbles();
	for (auto& pBubble : vBubbles)
	{
		Vec2 bubblePos = this->getParent()->convertToNodeSpace(pBubble->getPosition());
		Vec2 monsterPos = this->getPosition();
		Rect bubbRect = Rect(bubblePos.x - pBubble->getContentSize().width / 2, bubblePos.y - pBubble->getContentSize().height/2, pBubble->getContentSize().width, pBubble->getContentSize().height);
		Rect monsterRect = Rect(monsterPos.x - this->getContentSize().width / 2, monsterPos.y - this->getContentSize().height / 2, this->getContentSize().width, this->getContentSize().height);
		if (monsterRect.intersectsRect(bubbRect))
		{
			GameService::getInstance()->getGameScene()->removeBubble(pBubble);
			pBubble->removeFromParentAndCleanup(true);
			
			this->hurt(5);
			break;
		}
	}
}

void Monster::hurt(float fAtk)
{
	if (_emStatus!= Monster_Die)
	{
		if (_emType != Type_Boss)
		{
			if (_pSprite->getActionByTag(10))
			{
				_pSprite->stopActionByTag(10);
			}
			auto sequence = Sequence::createWithTwoActions(TintTo::create(0.1f, Color3B::RED), TintTo::create(0.1f, Color3B::WHITE));
			sequence->setTag(10);
			_pSprite->runAction(sequence);
		}
		
		_nCurHp -= fAtk;
		float fPerfent = (float)_nCurHp / (float)_nTotalHp;
		_pHpProgress->setScaleX(fPerfent);
		if (_nCurHp <= 0)
		{
			_pHpProgress->getParent()->setVisible(false);
			this->unscheduleUpdate();
			this->die();
		}
	}
	
}

void Monster::runAway()
{
	Vec2 heroPos = GameService::getInstance()->getGameScene()->getHero()->getHeroCheckPos();
	heroPos = _pBackGround->convertToNodeSpace(heroPos);
	
	heroPos = Utils::getPosInMap(heroPos, _pBackGround);
	Vec2 targetPos = this->getRandomTarget(heroPos);
	_nTarPosIndex = Utils::random(1, 8);

	PathItem* pTargetPath = nullptr;
	while (pTargetPath == nullptr)
	{
		targetPos = this->getRandomTarget(heroPos);
		pTargetPath = _AStarInfo->getFromSearchList(targetPos.x, targetPos.y);
		if (pTargetPath == nullptr)
		{
			for (auto& pOpenPath : _AStarInfo->_vOpenList)
			{
				if (pOpenPath->_nX == targetPos.x && pOpenPath->_nY == targetPos.y)
				{
					pTargetPath = pOpenPath;
					break;
				}
			}
		}

	}

	targetPos = _pBackGround->getLayer("background")->getPositionAt(targetPos);

	Vec2 monsterPos = this->getPosition();
	float fAngle = Utils::getPosAngle(targetPos, monsterPos);
	int nDirection = this->getDirection(fAngle);
	if (fAngle > 90 && fAngle < 270)
	{
		_pSprite->setFlippedX(true);
	}
	else
	{
		_pSprite->setFlippedX(false);
	}

	_nLastDirection = nDirection;
	_pSprite->stopAllActions();
	Animation* standAnimation = Animation::create();
	for (int i = 0; i < 6; ++i)
	{
		string fileName = StringUtils::format("m_r0%d_0008_000%d.png", _nLastDirection, i);
		standAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(fileName));
	}
	standAnimation->setDelayPerUnit(0.1f);
	_pSprite->runAction(RepeatForever::create(Animate::create(standAnimation)));
	
	_nLastDirection = 0;
	float fDis = this->getPosition().getDistance(targetPos);
	auto pMoveAction = Sequence::create(MoveTo::create(fDis / _fSpeed, targetPos), CallFunc::create(CC_CALLBACK_0(Monster::run, this)), NULL);
	this->runAction(pMoveAction);
}

void Monster::autoMove()
{
	m_playerMoveStep++;

	if (m_playerMoveStep >= _AStarInfo->_vPathList.size()) {
		this->attack();
		return;
	}

	Vec2 tarPos = _AStarInfo->_vPathList.at(m_playerMoveStep)->_posInMap;
	int nRandom = 1;
	if (Utils::randomPercent(50))
	{
		nRandom = -1;
	}

	tarPos.x += nRandom * Utils::random(0,10);
	nRandom = 1;
	if (Utils::randomPercent(50))
	{
		nRandom = -1;
	}
	tarPos.y += nRandom * Utils::random(0, 10);


	float fDis = this->getPosition().getDistance(tarPos);

	auto pMoveAction = Sequence::create(MoveTo::create(fDis / _fSpeed, tarPos), CallFunc::create(CC_CALLBACK_0(Monster::autoMove, this)), NULL);
	pMoveAction->setTag(100);
	////根据路径列表移动人物
	this->runAction(pMoveAction);
	

}

Vec2 Monster::getRandomTarget(Vec2 target)
{
	_nTarPosIndex++;

	if (_nTarPosIndex > 8)
	{
		_nTarPosIndex = 1;
	}

	int nDirection = _nTarPosIndex;

	if (nDirection == 1)
	{
		target.x--;
		target.y--;
	}
	else if (nDirection == 2)
	{
		target.y--;
	}
	else if (nDirection == 3)
	{
		target.x++;
		target.y--;
	}
	else if (nDirection == 4)
	{
		target.x--;
	}
	else if (nDirection == 5)
	{
		target.x++;
	}
	else if (nDirection == 6)
	{
		target.x--;
		target.y++;
	}
	else if (nDirection == 7)
	{
		target.y++;
	}
	else if (nDirection == 8)
	{
		target.x++;
		target.y++;
	}

	return  target;
}