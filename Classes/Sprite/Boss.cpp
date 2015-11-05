#include "Boss.h"
#include "../Service/GameService.h"
#include "../GameScene.h"
#include "../Utils/Utils.h"
#include "../Common/Constants.h"
#include "../Utils/LayoutUtil.h"
bool Boss::init()
{
    if (!Sprite::init()) {
        return  false;
    }

	ArmatureDataManager::getInstance()->addArmatureFileInfo("images/boss/m_0007.ExportJson");
	_pBossArmature = Armature::create("m_0007");
	this->addChild(_pBossArmature);

	_fSpeed = 50;
	_nTotalHp = 100;
	_nCurHp = _nTotalHp;
	_emStatus = Monster_Stand;
	_emType = Type_Boss;

	_pBackGround = GameService::getInstance()->getGameScene()->getBackGround();
	_SearchInfo = new PathSearchInfo();

	Size _mapSize = _pBackGround->getMapSize();
	for (int j = 0; j < _mapSize.height; j++) {
		for (int i = 0; i < _mapSize.width; i++) {
			PathSprite* pathSprite = GameService::getInstance()->getGameScene()->getPathSearchInfo()->m_inspectArray[i][j];
			if (pathSprite != nullptr)
			{
				PathSprite* newPath = new PathSprite();
				newPath->m_x = pathSprite->m_x;
				newPath->m_y = pathSprite->m_y;
				newPath->_pos = pathSprite->_pos;
				_SearchInfo->m_inspectArray[i][j] = newPath;
			}
			else
			{
				_SearchInfo->m_inspectArray[i][j] = nullptr;
			}
		}
	}

	//血条
	auto pHpBg = Sprite::createWithSpriteFrameName("Game_Blood_Bg_2.png");
	this->addChild(pHpBg);
	LayoutUtil::layoutParentCenter(pHpBg, 0, 100);

	_pHpProgress = Sprite::createWithSpriteFrameName("Game_Blood_Pro_2.png");
	_pHpProgress->setAnchorPoint(Vec2::ZERO);
	pHpBg->addChild(_pHpProgress);
	LayoutUtil::layoutParentCenter(_pHpProgress);

	this->scheduleUpdate();
	this->run();

	Vec2 center = Vec2(Constants::DESIGN_WIDTH / 2, Constants::DESIGN_HEIGHT / 2);
	center = _pBackGround->convertToNodeSpace(center);
	for (int i = 0; i < 3;i++)
	{
		Skill* pSkill = Skill::create(i);
		GameService::getInstance()->getGameScene()->getBackGroundUp()->addChild(pSkill);
		pSkill->setPosition(center);
		_vSkills.pushBack(pSkill);
	}
//	this->runAction(Sequence::createWithTwoActions(DelayTime::create(2.5f), CallFunc::create(CC_CALLBACK_0(Boss::bossAtk, this))));
    return true;
}

void Boss::die()
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
	_pBossArmature->setVisible(false);
	_pSprite = Sprite::create();
	this->addChild(_pSprite);
	this->stopAllActions();
	_pSprite->runAction(Sequence::create(pDieAni, FadeOut::create(0.5f), releaseAction, NULL));
}

void Boss::run()
{
	auto runAction = CallFunc::create(CC_CALLBACK_0(Monster::run, this));
	this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.1f), runAction));

	this->stopActionByTag(100);

	Vec2 heroPos = GameService::getInstance()->getGameScene()->getHero()->getPosition();
	heroPos = _pBackGround->convertToNodeSpace(heroPos);

	float fDis = this->getPosition().getDistance(heroPos);
	if (_emStatus != Monster_Stand || fDis < 300 || fDis > 600)
	{
		return;
	}
	Vec2 monsterPos = this->getPosition();
	float fAngle = Utils::getPosAngle(heroPos, monsterPos);
	_fLastAngle = fAngle;
	int nDirection = this->getDirection(fAngle);
	if (fAngle > 90 && fAngle < 270)
	{
		_pBossArmature->setScaleX(-1);
	}
	else
	{
		_pBossArmature->setScaleX(1);
	}

	if (_nLastDirection != nDirection || _nMonsterAction != 1)
	{
		_nMonsterAction = 1;
		_nLastDirection = nDirection;
		_pBossArmature->getAnimation()->stop();
		string sArmtureName = StringUtils::format("m_r0%d", _nLastDirection);
		_pBossArmature->getAnimation()->play(sArmtureName);
	}

	Point startPos = Utils::getPosInMap(this->getPosition(), _pBackGround);
	//设置起始和终点
	_SearchInfo->m_startX = startPos.x;
	_SearchInfo->m_startY = startPos.y;

	////清除之前的路径
	clearPath();

	heroPos = Utils::getPosInMap(heroPos, _pBackGround);
	PathSprite* targetPath = _SearchInfo->getObjFromInspectArray((int)heroPos.x, (int)heroPos.y);
	if (targetPath == nullptr)
	{
		targetPath = new PathSprite();
		targetPath->m_x = heroPos.x;
		targetPath->m_y = heroPos.y;
		targetPath->_pos = _pBackGround->getLayer("background")->getPositionAt(Vec2(heroPos.x, heroPos.y));
		_SearchInfo->m_inspectArray[targetPath->m_x][targetPath->m_y] = targetPath;
	}

	_SearchInfo->m_endX = targetPath->m_x;
	_SearchInfo->m_endY = targetPath->m_y;
	//计算路径
	calculatePath();
	//移动物体
	autoMove();

}

int Boss::getDirection(float angle)
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

void Boss::update(float dt)
{
	if (_fSkillCdTime > 0)
	{
		_fSkillCdTime -= dt;
	}

	Vector<Bubble*> vBubbles = GameService::getInstance()->getGameScene()->getBubbles();
	for (auto& pBubble : vBubbles)
	{
		Vec2 bubblePos = this->getParent()->convertToNodeSpace(pBubble->getPosition());
		Vec2 monsterPos = this->getPosition();
		Rect bubbRect = Rect(bubblePos.x - pBubble->getContentSize().width / 2, bubblePos.y - pBubble->getContentSize().height / 2, pBubble->getContentSize().width, pBubble->getContentSize().height);
		Rect monsterRect = Rect(monsterPos.x - 100 / 2, monsterPos.y - 150 / 2, 100, 150);
		if (monsterRect.intersectsRect(bubbRect))
		{
			GameService::getInstance()->getGameScene()->removeBubble(pBubble);
			pBubble->removeFromParentAndCleanup(true);

			this->hurt(5);

			if (_emStatus != Monster_Die)
			{
				if (_pBossArmature->getActionByTag(10))
				{
					_pBossArmature->stopActionByTag(10);
				}
				auto sequence = Sequence::createWithTwoActions(TintTo::create(0.1f, Color3B::RED), TintTo::create(0.1f, Color3B::WHITE));
				sequence->setTag(10);
				_pBossArmature->runAction(sequence);
			}
			break;
		}
	}


	Vec2 heroPos = GameService::getInstance()->getGameScene()->getHero()->getPosition();
	heroPos = _pBackGround->convertToNodeSpace(heroPos);
	float fDis = this->getPosition().getDistance(heroPos);
	if (fDis <= 300 && _emStatus != Monster_Attack || fDis >=600 && _emStatus != Monster_Attack)
	{
		this->bossAtk();
	}
}

void Boss::attack()
{
	

}

void Boss::bossAtk()
{
	bool bIsCdTime = false;

	if (_fSkillCdTime > 0)
	{
		bIsCdTime = true;
	}
	int nRandom = Utils::random(0, 2);
	if (_vSkills.at(nRandom)->IsInCd())
	{
		bIsCdTime = true;
	}

	if (bIsCdTime)
	{
		int nDirection = this->getDirection(_fLastAngle);
		if (_nLastDirection != nDirection || _nMonsterAction != 2)
		{
			_nMonsterAction = 2;
			_nLastDirection = nDirection;
			_pBossArmature->getAnimation()->stop();
			string sArmtureName = StringUtils::format("m_s0%d", _nLastDirection);
			_pBossArmature->getAnimation()->play(sArmtureName);
		}

		return;
	}

	Vec2 heroPos = GameService::getInstance()->getGameScene()->getHero()->getPosition();
	heroPos = _pBackGround->convertToNodeSpace(heroPos);

	Vec2 monsterPos = this->getPosition();
	float fAngle = Utils::getPosAngle(heroPos, monsterPos);
	_fLastAngle = fAngle;
	int nDirection = this->getDirection(fAngle);
	if (fAngle > 90 && fAngle < 270)
	{
		_pBossArmature->setScaleX(-1);
	}
	else
	{
		_pBossArmature->setScaleX(1);
	}

	_fSkillCdTime = 10;
	if (_nLastDirection != nDirection)
	{
		_nLastDirection = nDirection;
	}

	_pBossArmature->getAnimation()->stop();
	string sArmtureName = StringUtils::format("m_a0%d", _nLastDirection);
	_pBossArmature->getAnimation()->play(sArmtureName, -1, 0);
	_pBossArmature->getAnimation()->pause();

	auto atkAction = CallFunc::create([=](){
		_pBossArmature->getAnimation()->resume();
	});

	auto atkFinish = CallFunc::create([=](){
		_emStatus = Monster_Stand;
	});


	_emStatus = Monster_Attack;

	_vSkills.at(nRandom)->skillEffect();
	

	this->runAction(Sequence::create(DelayTime::create(1.5f), atkAction, DelayTime::create(1.5f), atkFinish, NULL));

	_nLastDirection = 0;
}