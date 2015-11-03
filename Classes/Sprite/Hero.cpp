#include "Hero.h"
#include "../Common/Constants.h"
#include "../Service/GameService.h"
#include "../GameScene.h"
#include "Bubble.h"
#include "../Utils/Utils.h"
#include "../Utils/LayoutUtil.h"
bool Hero::init()
{
    if (!Sprite::init()) {
        return  false;
    }

	_nFireCount = 1;
	_bIsProtect = false;

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("images/hero/m_00050.plist", "images/hero/m_00050.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("images/hero/t_fire_00020.plist", "images/hero/t_fire_00020.png");


	_pSprite = Sprite::createWithSpriteFrameName("m_r04_0005_0000.png");
	_pSprite->setPosition(Vec2(_pSprite->getContentSize().width / 2, _pSprite->getContentSize().height / 2));
	this->addChild(_pSprite);
	this->setContentSize(_pSprite->getContentSize());


	//ÑªÌõ
	auto pHpBg = Sprite::createWithSpriteFrameName("Game_Blood_Bg_2.png");
	this->addChild(pHpBg);
	LayoutUtil::layoutParentCenter(pHpBg, 0, 30);

	_pHpProgress = Sprite::createWithSpriteFrameName("Game_Blood_Pro_2.png");
	_pHpProgress->setAnchorPoint(Vec2::ZERO);
	_pHpProgress->setColor(Color3B::GREEN);
	pHpBg->addChild(_pHpProgress);
	LayoutUtil::layoutParentCenter(_pHpProgress);


	this->scheduleUpdate();

    return true;
}

void Hero::update(float dt)
{
	/*if (_fMinGapTime > 0)
	{
		_fMinGapTime -= dt;
	}*/
}

void Hero::handFire()
{
	/*if (_fMinGapTime > 0)
	{
		return;
	}
	_fMinGapTime = 0.5f;*/
	this->stopAllActions();
	auto handFire = CallFuncN::create(CC_CALLBACK_0(Hero::fire, this, -1));
	this->runAction(RepeatForever::create(Sequence::create(handFire, DelayTime::create(0.5f), NULL)));
}

void Hero::autoFire()
{
	/*if (_fMinGapTime > 0)
	{
		return;
	}
	_fMinGapTime = 0.5f;*/
	this->stopAllActions();
	auto autoFire = CallFunc::create(CC_CALLBACK_0(Hero::findMonsterFire, this));
	this->runAction(RepeatForever::create(Sequence::create(autoFire, DelayTime::create(0.5f), NULL)));
}

void Hero::findMonsterFire()
{
	Vec2 heroPos = this->getPosition();
	heroPos = GameService::getInstance()->getGameScene()->getBackGround()->convertToNodeSpace(heroPos);
	Monster* pNearMonster = nullptr;
	float fMinDis = 0;
	auto vMonsters = GameService::getInstance()->getGameScene()->getMonsters();
	if (vMonsters.size() > 0)
	{
		pNearMonster = vMonsters.at(0);
		fMinDis = heroPos.getDistance(pNearMonster->getPosition());
	}

	for (auto& pMonster : vMonsters)
	{
		float fDis = heroPos.getDistance(pMonster->getPosition());
		if (fDis < fMinDis)
		{
			fMinDis = fDis;
			pNearMonster = pMonster;
		}
	}

	if (pNearMonster != nullptr)
	{
		Vec2 monsterPos = pNearMonster->getPosition();
		_runTargetPos = monsterPos;
		_fAngle = Utils::getPosAngle(monsterPos, heroPos);
	}
	else
	{
		_runTargetPos = Vec2::ZERO;
	}
	fire(_fAngle);
}

void Hero::run(float angle, bool bIsDirection)
{
	float fAngle = angle * M_PI / 180.0f;
	float fSpeed =3.0f;

	Vec2 curPos = this->getPosition();
	float fXGap = fSpeed * cosf(fAngle);
	float fYGap = fSpeed * sinf(fAngle);


	Vec2 targetPos = Vec2(curPos.x + fXGap, curPos.y + fYGap);

	if (GameService::getInstance()->getCurOperate() == Operate_Right)
	{
		return;
	}
	if (GameService::getInstance()->getCurOperate() == Operate_Left
		|| (GameService::getInstance()->getCurOperate() == Operate_Both && !bIsDirection))
	{
		if (GameService::getInstance()->getCurOperate() == Operate_Left)
		{
			if (_runTargetPos != Vec2::ZERO)
			{
				angle = _fAngle;
				targetPos = _runTargetPos;
			}
			else
			{
				_fAngle = angle;
			}
		}
		
		if (angle > 90 && angle < 270)
		{
			_pSprite->setFlippedX(true);
		}
		else
		{
			_pSprite->setFlippedX(false);
		}

		int nCurDirection = this->getDirection(angle);
		if (_nLastDirection != nCurDirection)
		{
			if (GameService::getInstance()->getCurOperate() == Operate_Left)
			{
				this->autoFire();
			}
			else if (GameService::getInstance()->getCurOperate() == Operate_Both)
			{
				this->handFire();
			}
			_nLastDirection = nCurDirection;
			_emStatus = RUN;
			_pSprite->stopAllActions();
			auto runAnimation = Animation::create();
			for (int i = 0; i < 7; ++i)
			{
				string fileName = StringUtils::format("m_r0%d_0005_000%d.png", nCurDirection, i);
				runAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(fileName));
			}
			runAnimation->setDelayPerUnit(0.1f);
			Animate* runAnim = Animate::create(runAnimation);
			_pSprite->runAction(RepeatForever::create(runAnim));
		}
		if (GameService::getInstance()->getCurOperate() == Operate_Both && !bIsDirection)
		{
			return;
		}
	}
	
	float fSideWidth = 50;
	bool bIsMoveHero = false;
	bool bIsBgXSide = false;
	bool bIsBgYSide = false;

	TMXTiledMap* pBackGround = GameService::getInstance()->getGameScene()->getBackGround();

	Vec2 bgPos = pBackGround->getPosition();

	float bgWidth = pBackGround->getContentSize().width - Constants::DESIGN_WIDTH;
	float bgHeight = pBackGround->getContentSize().height - Constants::DESIGN_HEIGHT ;

	Vec2 normalBgPos = bgPos;

	Vec2 heroPos = this->getPosition();
	Vec2 normalHeroPos = heroPos;

	bgPos.x -= fXGap;
	bgPos.y -= fYGap;

	if (bgPos.x > 0 || bgPos.x < -bgWidth)
	{
		bgPos.x = normalBgPos.x;
		bIsBgXSide = true;
		heroPos.x += fXGap;
	}

	if (bgPos.y > 0 ||  bgPos.y < -bgHeight)
	{
		bgPos.y = normalBgPos.y;
		bIsBgYSide = true;
		heroPos.y += fYGap;
	}

	if (fabsf(heroPos.x - Constants::DESIGN_WIDTH/2) > 5)
	{
		if (!bIsBgXSide)
		{
			bIsMoveHero = true;
			heroPos.x += fXGap;
			bgPos.x = normalBgPos.x;
		}
	}

	if (fabsf( heroPos.y - Constants::DESIGN_HEIGHT / 2) > 5)
	{
		if (!bIsBgYSide)
		{
			bIsMoveHero = true;
			heroPos.y += fYGap;
			bgPos.y = normalBgPos.y;
		}
	}

	if (bIsBgYSide || bIsBgXSide)
	{
		bIsMoveHero = true;
	}
	

	if (bIsMoveHero)
	{
		if (heroPos.x < fSideWidth)
		{
			heroPos.x = fSideWidth;
		}

		if (heroPos.y < fSideWidth)
		{
			heroPos.y = fSideWidth;
		}

		if (heroPos.x > Constants::DESIGN_WIDTH - fSideWidth)
		{
			heroPos.x = Constants::DESIGN_WIDTH - fSideWidth;
		}

		if (heroPos.y > Constants::DESIGN_HEIGHT - fSideWidth)
		{
			heroPos.y = Constants::DESIGN_HEIGHT - fSideWidth;
		}

		this->setPosition(heroPos);
	}

	pBackGround->setPosition(bgPos);

	Vec2 heroCheckPos = pBackGround->convertToNodeSpace(this->getPosition());
//	heroCheckPos.y -= _pSprite->getContentSize().height / 5;
	Vec2 newBgPos = Vec2::ZERO;
	Vec2 newHeroPos = Vec2::ZERO;

	vector<Rect> collideRect = GameService::getInstance()->getGameScene()->getCollideRects();
//	for (auto& rect : collideRect)
	{
		if (IsTouched(heroCheckPos))
		{
			newBgPos.x = normalBgPos.x;
			newBgPos.y = bgPos.y;

			newHeroPos.x = normalHeroPos.x;
			newHeroPos.y = heroPos.y;

			this->setPosition(newHeroPos);
			pBackGround->setPosition(newBgPos);

			heroCheckPos = pBackGround->convertToNodeSpace(this->getPosition());
//			heroCheckPos.y -= _pSprite->getContentSize().height / 5;

			if (IsTouched(heroCheckPos))
			{
				newBgPos.x = bgPos.x;
				newBgPos.y = normalBgPos.y;

				newHeroPos.x = heroPos.x;
				newHeroPos.y = normalHeroPos.y;

				this->setPosition(newHeroPos);
				pBackGround->setPosition(newBgPos);

				heroCheckPos = pBackGround->convertToNodeSpace(this->getPosition());
//				heroCheckPos.y -= _pSprite->getContentSize().height / 5;

				if (IsTouched(heroCheckPos))
				{
					pBackGround->setPosition(normalBgPos);
					this->setPosition(normalHeroPos);
				}

			}
		}
	}
}

bool Hero::IsTouched(Vec2 pos)
{
	vector<Rect> collideRect = GameService::getInstance()->getGameScene()->getCollideRects();
	for (auto& rect : collideRect)
	{
		if (rect.containsPoint(pos))
		{
			return true;
		}
	}
	return false;
}

int Hero::getDirection(float angle)
{
	int nCurDirection = 0;
	if ((angle >= 360 - 90 && angle <= 360 - 60) || (angle > 360 - 120 && angle <= 360 - 90))
	{
		nCurDirection = 1;
	}
	else if ((angle > 360 - 60 && angle <= 360 - 30) || (angle > 360 - 150 && angle <= 360 - 120))
	{
		nCurDirection = 2;
	}
	else if ((angle > 360 - 30 && angle <= 360) || (angle > 360 - 180 && angle <= 360 - 150))
	{
		nCurDirection = 3;
	}
	else if ((angle >= 0 && angle < 30) || (angle >= 180 - 30 && angle <= 180))
	{
		nCurDirection = 4;
	}
	else if ((angle >= 30 && angle < 60) || (angle >= 180 - 60 && angle < 180 - 30))
	{
		nCurDirection = 5;
	}
	else if ((angle >= 60 && angle <= 90) || (angle > 180 - 90 && angle < 180 - 60))
	{
		nCurDirection = 6;
	}

	return nCurDirection;
}

void Hero::stand()
{
	if (_emStatus != STAND)
	{
		this->stopAllActions();
		if (_pFire!=nullptr)
		{
			_pFire->removeFromParentAndCleanup(true);
			_pFire = nullptr;
		}
		
		if (_nLastDirection == 0)
		{
			_nLastDirection = 2;
		}		
		_emStatus = STAND;
		_pSprite->stopAllActions();
		auto standAnimation = Animation::create();
		for (int i = 0; i < 7; ++i)
		{
			string fileName = StringUtils::format("m_s0%d_0005_000%d.png", _nLastDirection, i);
			standAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(fileName));
		}

		standAnimation->setDelayPerUnit(0.2f);
		Animate* standAnim = Animate::create(standAnimation);
		_pSprite->runAction(RepeatForever::create(standAnim));

		_nLastDirection = 0;
	}
}

void Hero::fire(float targetAngle)
{
//	return;
	int nDirection = _nLastDirection;
	if (targetAngle != -1)
	{
		nDirection = this->getDirection(targetAngle);
		
	}

	auto fireBubble = CallFunc::create([=](){
		float fAngle = 0;
		Vec2 firePos = _pSprite->getPosition();
		Vec2 bubblePos = this->getPosition();
		switch (nDirection)
		{
		case 1:
			fAngle = 270;
			firePos.y -= 60;
			bubblePos.y -= 60;
			break;
		case 2:
			fAngle = 345;
			if (_pSprite->isFlippedX())
			{
				fAngle = 180 - fAngle;
				firePos.x -= 50;
				firePos.y -= 25;
				bubblePos.x -= 50;
				bubblePos.y -= 25;
			}
			else
			{
				firePos.x += 50;
				firePos.y -= 20;
				bubblePos.x += 50;
				bubblePos.y -= 20;
			}
			break;
		case 3:
			fAngle = 360;
			if (_pSprite->isFlippedX())
			{
				fAngle = 180;
				firePos.x -= 60;
				firePos.y -= 5;
				bubblePos.x -= 60;
				bubblePos.y -= 5;
			}
			else
			{
				firePos.x += 60;
				bubblePos.x += 60;
			}
			break;
		case 4:
			fAngle = 15;
			if (_pSprite->isFlippedX())
			{
				fAngle = 180 - fAngle;
				firePos.x -= 55;
				firePos.y += 10;
				bubblePos.x -= 55;
				bubblePos.y += 10;
			}
			else
			{
				firePos.x += 55;
				firePos.y += 15;
				bubblePos.x += 55;
				bubblePos.y += 15;
			}

			break;
		case 5:
			fAngle = 40;
			if (_pSprite->isFlippedX())
			{
				fAngle = 180 - fAngle;
				firePos.x -= 45;
				firePos.y += 25;
				bubblePos.x -= 45;
				bubblePos.y += 25;
			}
			else
			{
				firePos.x += 45;
				firePos.y += 30;
				bubblePos.x += 45;
				bubblePos.y += 30;
			}
			break;
		case 6:
			firePos.y += 60;
			bubblePos.y += 60;
			fAngle = 90;
			break;
		default:
			return;
			break;
		}

        auto releaseAction = CallFunc::create([=](){
			_pFire->removeFromParentAndCleanup(true);
			_pFire = nullptr;
		});

		if (_pFire != nullptr)
		{
			_pFire->stopAllActions();
			_pFire->removeFromParentAndCleanup(true);
		}
		_pFire = Sprite::createWithSpriteFrameName("t_fire_0002_0001.png");
		Animation* fireAnimation = Animation::create();
		for (int i = 1; i < 5; ++i)
		{
			string fileName = StringUtils::format("t_fire_0002_000%d.png", i);
			fireAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(fileName));
		}
		fireAnimation->setDelayPerUnit(0.1f);
		_pFire->runAction(Sequence::create(Animate::create(fireAnimation), FadeOut::create(0.2f), releaseAction, NULL));
		_pFire->setPosition(firePos);
		_pFire->setFlippedY(true);
		this->addChild(_pFire);
		_pFire->setRotation(90 - fAngle);
		_pFire->setScale(0.5f);

		for (int i = 0; i<_nFireCount; i++) {
            Bubble* pBubble = Bubble::create();
            this->getParent()->addChild(pBubble,Constants::ZORDER_MONSTER);
            GameService::getInstance()->getGameScene()->addBubble(pBubble);
            
            pBubble->setPosition(bubblePos);
            float bubbleAngle = fAngle;
            if (i == 1) {
                bubbleAngle += 15;
            }
            else if(i == 2){
                bubbleAngle -= 15;
            }
            
            pBubble->setRotation(-bubbleAngle);
            
            bubbleAngle = bubbleAngle * M_PI / 180.0f;
            float fX = 1200 * cosf(bubbleAngle);
            float fY = 1200 * sinf(bubbleAngle);
            
            auto releaseBubble = CallFuncN::create([](Node* pNode){
                Bubble* pBubble = (Bubble*)pNode;
                GameService::getInstance()->getGameScene()->removeBubble(pBubble);
                
                pNode->removeFromParentAndCleanup(true);
            });
            
            pBubble->runAction(Sequence::create(MoveBy::create(2.0f, Vec2(fX, fY)), releaseBubble,NULL));
        }
        

	});

	for (int i = 0; i < 3; i++)
	{
		this->runAction(Sequence::createWithTwoActions(DelayTime::create(i*0.1f), fireBubble));
	}

}

void  Hero::hurt(float fAtk)
{
	if (_bIsProtect)
	{
		return;
	}
	_nCurHp -= fAtk;
	float fPercent = (float)_nCurHp / (float)_nTotalHp;
	if (fPercent < 0)
	{
		fPercent = 0;
	}	
	_pHpProgress->setScaleX(fPercent);
	if (_nCurHp <= 0)
	{
		//game over
	}
}

Rect Hero::getHeroBox()
{
	Vec2 pos = this->getPosition();
	pos = GameService::getInstance()->getGameScene()->getBackGround()->convertToNodeSpace(pos);
	Rect heroRect = Rect(pos.x - _pSprite->getContentSize().width / 2, pos.y - _pSprite->getContentSize().height/2, _pSprite->getContentSize().width, _pSprite->getContentSize().height);
	return heroRect;
}