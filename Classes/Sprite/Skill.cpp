#include "Skill.h"
#include "../Service/GameService.h"
#include "../GameScene.h"
#include "Boss.h"
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
	return true;
}

void  Skill::skillEffect()
{
	if (_nSkillId == 0)
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfo("images/skills/boss_skill/t_sk_0019.ExportJson");

		for (int i = 0; i < 10; i++)
		{
			int nX = Constants::DESIGN_WIDTH / 2 - 200 - Utils::random(0, 2 * (Constants::DESIGN_WIDTH / 2 - 200));
			int nY = Constants::DESIGN_HEIGHT / 2 - 200 - Utils::random(0, 2 * (Constants::DESIGN_HEIGHT / 2 - 200));
			Vec2 normalPos = Vec2(nX, nY);
			_pSkillArmature = Armature::create("t_sk_0019");
			_pSkillArmature->getAnimation()->play("Animation2");
			this->addChild(_pSkillArmature);
			_pSkillArmature->setPosition(normalPos);

			auto skillAtk = CallFunc::create(CC_CALLBACK_0(Skill::skill1Attack, this, _pSkillArmature));

			_pSkillArmature->runAction(Sequence::create(DelayTime::create(2.0f), skillAtk, NULL));
		}
	}
	else if (_nSkillId == 1)
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfo("images/skills/boss_skill/t_sk_0021.ExportJson");

		int nX = Constants::DESIGN_WIDTH / 2 - 200 - Utils::random(0, 2 * (Constants::DESIGN_WIDTH / 2 - 200));
		int nY = Constants::DESIGN_HEIGHT / 2 - 200 - Utils::random(0, 2 * (Constants::DESIGN_HEIGHT / 2 - 200));
		Vec2 normalPos = Vec2(nX, Constants::DESIGN_HEIGHT / 2 + 100);

		_pSkillArmature = Armature::create("t_sk_0021");
		_pSkillArmature->getAnimation()->play("Animation1");
		_pSkillArmature->setPosition(normalPos);
		this->addChild(_pSkillArmature);

		auto readyAction = CallFunc::create([=](){
			_pSkillArmature->getAnimation()->play("Animation2");
			_pSkillArmature->getAnimation()->gotoAndPause(1);
		});

		auto fireAction = CallFunc::create([=](){
			_pSkillArmature->getAnimation()->play("Animation2");
		});

		auto bombAction = CallFunc::create([=](){
			_pSkillArmature->getAnimation()->play("Animation3");
		});
		auto releaseAction = CallFunc::create([=](){
			_pSkillArmature->removeFromParentAndCleanup(true);
		});

		_pSkillArmature->runAction(Sequence::create(MoveTo::create(1.5f, Vec2(normalPos.x, nY)), readyAction,
			DelayTime::create(2.0f), fireAction,
			DelayTime::create(3.0f), bombAction,
			DelayTime::create(3.0f), releaseAction, NULL));

	}
	else if (_nSkillId == 2)
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfo("images/skills/boss_skill/t_sk_0010.ExportJson");
		ArmatureDataManager::getInstance()->addArmatureFileInfo("images/skills/boss_skill/t_sk_0007.ExportJson");

		
			auto releaseAction = CallFuncN::create([=](Node* pNode){
				pNode->removeFromParentAndCleanup(true);
			});

			auto fireAction = CallFunc::create([=](){
				float fRoation = _pSkillArmature->getRotation();
				Vec2 pos = _pSkillArmature->getPosition();
				_pSkillArmature->removeFromParentAndCleanup(true);
				_pSkillArmature = Armature::create("t_sk_0007");
				_pSkillArmature->getAnimation()->play("Animation1",-1,0);
				_pSkillArmature->setRotation(fRoation);
				_pSkillArmature->setPosition(pos);
				this->addChild(_pSkillArmature);
			});

			Vec2 curPos = GameService::getInstance()->getGameScene()->getBoss()->getPosition();

			Vec2 heroPos = GameService::getInstance()->getGameScene()->getHero()->getPosition();
			heroPos = GameService::getInstance()->getGameScene()->getBackGround()->convertToNodeSpace(heroPos);
			float fAngle = Utils::getPosAngle(heroPos, curPos);

			_pSkillArmature = Armature::create("t_sk_0010");
			_pSkillArmature->getAnimation()->play("Animation1");
			this->addChild(_pSkillArmature);
			this->setPosition(curPos);
			_pSkillArmature->setRotation(-fAngle);
        
            _pSkillArmature->runAction(Sequence::create(DelayTime::create(5.0f), releaseAction, NULL));
		
			this->runAction(Sequence::createWithTwoActions(DelayTime::create(1.5f), fireAction));

	}

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

void Skill::skill2Attack(Armature* pNode)
{

}