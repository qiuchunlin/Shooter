#include "Skill.h"
#include "../Service/GameService.h"
#include "../GameScene.h"
#include "../Utils/Utils.h"
#include "../Common/Constants.h"


Skill*  Skill::create(int nSkillId)
{
	Skill *pRet = new(std::nothrow) Skill();
	pRet->_nSkillId = nSkillId;
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


bool Skill::init()
{
	if (!Sprite::init()) {
		return  false;
	}

	if (_nSkillId == 0)
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfo("images/skills/boss_skill/t_sk_0019.ExportJson");

		for (int i = 0; i < 10;i++)
		{
			int nX = Constants::DESIGN_WIDTH/2 - 100 - Utils::random(0, 2*(Constants::DESIGN_WIDTH/2 - 100));
			int nY = Constants::DESIGN_HEIGHT/2 - 100 - Utils::random(0, 2 * (Constants::DESIGN_HEIGHT/2 - 100));
			Vec2 normalPos = Vec2(nX, nY);
			_pSkillArmature = Armature::create("t_sk_0019");
			_pSkillArmature->getAnimation()->play("Animation2");
			this->addChild(_pSkillArmature);
			_pSkillArmature->setPosition(normalPos);

			auto skillAtk = CallFunc::create(CC_CALLBACK_0(Skill::skill1Attack, this,_pSkillArmature));

			_pSkillArmature->runAction(Sequence::create(DelayTime::create(2.0f), skillAtk, NULL));
		}
		

	}

	return true;
}

void Skill::skill1Attack(Armature* pNode)
{
	Vec2 targetPos = pNode->getPosition();
	Vec2 normalPos = Vec2(targetPos.x, Constants::DESIGN_HEIGHT + 100);
	normalPos = GameService::getInstance()->getGameScene()->getBackGround()->convertToNodeSpace(normalPos);
	normalPos.x = targetPos.x;
	pNode->setPosition(normalPos);
	float fDis = normalPos.getDistance(targetPos);

	pNode->getAnimation()->stop();
	pNode->getAnimation()->play("Animation1");
	pNode->setRotation(-90);


	auto bombAction = CallFunc::create([=](){
		pNode->setRotation(0);
		pNode->getAnimation()->stop();
		pNode->getAnimation()->play("Animation3");
	});

	auto releaseAction = CallFunc::create([=](){
		pNode->removeFromParentAndCleanup(true);
	});

	pNode->runAction(Sequence::create(EaseOut::create(MoveTo::create(fDis / 500, targetPos), 0.5f), bombAction,DelayTime::create(2.5f), releaseAction, NULL));
}