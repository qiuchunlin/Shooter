#include "Boss.h"
#include "../Service/GameService.h"
#include "../GameScene.h"
#include "../Utils/Utils.h"
#include "../Common/Constants.h"

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
	_emStatus = Monster_Stand;
	_emType = Type_Boss;

	_SearchInfo = new PathSearchInfo();
	_SearchInfo = GameService::getInstance()->getGameScene()->_pPathInfo;
	_pBackGround = GameService::getInstance()->getGameScene()->getBackGround();

	this->scheduleUpdate();
	this->run();

	Vec2 center = Vec2(Constants::DESIGN_WIDTH / 2, Constants::DESIGN_HEIGHT / 2);
	center = _pBackGround->convertToNodeSpace(center);
	for (int i = 0; i < 3;i++)
	{
		Skill* pSkill = Skill::create(i);
		GameService::getInstance()->getGameScene()->getBackGround()->addChild(pSkill);
		pSkill->setPosition(center);
		_vSkills.pushBack(pSkill);
	}
//	this->runAction(Sequence::createWithTwoActions(DelayTime::create(2.5f), CallFunc::create(CC_CALLBACK_0(Boss::bossAtk, this))));
    return true;
}

void Boss::run()
{
	auto runAction = CallFunc::create(CC_CALLBACK_0(Monster::run, this));
	this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.1f), runAction));

	this->stopActionByTag(100);

	Vec2 heroPos = GameService::getInstance()->getGameScene()->getHero()->getPosition();
	heroPos = _pBackGround->convertToNodeSpace(heroPos);

	float fDis = this->getPosition().getDistance(heroPos);
	if (_emStatus != Monster_Stand || fDis < 300)
	{
		return;
	}

	Vec2 monsterPos = this->getPosition();
	float fAngle = Utils::getPosAngle(heroPos, monsterPos);
	int nDirection = this->getDirection(fAngle);
	if (fAngle > 90 && fAngle < 270)
	{
		_pBossArmature->setScaleX(-1);
	}
	else
	{
		_pBossArmature->setScaleX(1);
	}

	if (_nLastDirection != nDirection)
	{
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
	Monster::update(dt);
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
	int nRandom = Utils::random(0, 2);
//	nRandom = 1;
	if (_vSkills.at(nRandom)->IsInCd())
	{
		return;
	}
	Vec2 heroPos = GameService::getInstance()->getGameScene()->getHero()->getPosition();
	heroPos = _pBackGround->convertToNodeSpace(heroPos);

	Vec2 monsterPos = this->getPosition();
	float fAngle = Utils::getPosAngle(heroPos, monsterPos);
	int nDirection = this->getDirection(fAngle);
	if (fAngle > 90 && fAngle < 270)
	{
		_pBossArmature->setScaleX(-1);
	}
	else
	{
		_pBossArmature->setScaleX(1);
	}

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