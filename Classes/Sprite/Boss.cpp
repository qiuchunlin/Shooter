#include "Boss.h"
#include "../Service/GameService.h"
#include "../GameScene.h"
#include "../Utils/Utils.h"
#include "Skill.h"
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

	auto runatcion = CallFunc::create([=](){
		_SearchInfo = new PathSearchInfo();
		_SearchInfo = GameService::getInstance()->getGameScene()->_pPathInfo;
		_pBackGround = GameService::getInstance()->getGameScene()->getBackGround();
		this->run();
	});

	this->runAction(Sequence::createWithTwoActions(DelayTime::create(1.5f), runatcion));

	this->runAction(Sequence::createWithTwoActions(DelayTime::create(2.5f), CallFunc::create(CC_CALLBACK_0(Boss::bossAtk, this))));
    return true;
}

void Boss::run()
{
	_bIsRun = true;
	auto runAction = CallFunc::create(CC_CALLBACK_0(Monster::run, this));
	this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.1f), runAction));

	this->stopActionByTag(100);

	Vec2 heroPos = GameService::getInstance()->getGameScene()->getHero()->getPosition();

	auto nodePosition = _pBackGround->convertToNodeSpace(heroPos);

	Vec2 monsterPos = this->getPosition();
	float fAngle = Utils::getPosAngle(nodePosition, monsterPos);
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

	nodePosition = Utils::getPosInMap(nodePosition, _pBackGround);
	PathSprite* targetPath = _SearchInfo->getObjFromInspectArray((int)nodePosition.x, (int)nodePosition.y);
	if (targetPath == nullptr)
	{
		targetPath = new PathSprite();
		targetPath->m_x = nodePosition.x;
		targetPath->m_y = nodePosition.y;
		targetPath->_pos = _pBackGround->getLayer("background")->getPositionAt(Vec2(nodePosition.x, nodePosition.y));
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

void Boss::attack()
{
	

}

void Boss::bossAtk()
{
	Vec2 center = Vec2(Constants::DESIGN_WIDTH / 2, Constants::DESIGN_HEIGHT / 2);
	center = _pBackGround->convertToNodeSpace(center);
	Skill* pSkill = Skill::create(2);
	GameService::getInstance()->getGameScene()->getBackGround()->addChild(pSkill);
	pSkill->setPosition(center);
	pSkill->skillEffect();


}
