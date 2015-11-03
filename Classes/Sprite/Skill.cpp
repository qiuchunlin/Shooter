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
	this->scheduleUpdate();
	return true;
}

void  Skill::skillEffect()
{
	if (_fCdTime > 0)
	{
		return;
	}

	_nSkillId = 2;

	Vec2 center = Vec2(Constants::DESIGN_WIDTH / 2, Constants::DESIGN_HEIGHT / 2);
	center = GameService::getInstance()->getGameScene()->getBackGround()->convertToNodeSpace(center);

	Vec2 posGap = center - this->getPosition();
	if (_nSkillId == 0)
	{
		_fCdTime = 8.0f;
		ArmatureDataManager::getInstance()->addArmatureFileInfo("images/skills/boss_skill/t_sk_0019.ExportJson");

		for (int i = 0; i < 10; i++)
		{
			int nX = Constants::DESIGN_WIDTH / 2 - 200 - Utils::random(0, 2 * (Constants::DESIGN_WIDTH / 2 - 200));
			int nY = Constants::DESIGN_HEIGHT / 2 - 200 - Utils::random(0, 2 * (Constants::DESIGN_HEIGHT / 2 - 200));
			Vec2 normalPos = Vec2(nX, nY);
			normalPos += posGap;
			Armature* pSkillArmature = Armature::create("t_sk_0019");
			pSkillArmature->getAnimation()->play("Animation2");
			this->addChild(pSkillArmature);
			pSkillArmature->setPosition(normalPos);

			auto skillAtk = CallFunc::create(CC_CALLBACK_0(Skill::skill1Attack, this, pSkillArmature));

			pSkillArmature->runAction(Sequence::create(DelayTime::create(2.0f), skillAtk, NULL));
		}
	}
	else if (_nSkillId == 1)
	{
		_fCdTime = 6.0f;
		ArmatureDataManager::getInstance()->addArmatureFileInfo("images/skills/boss_skill/t_sk_0021.ExportJson");

		int nX = Constants::DESIGN_WIDTH / 2 - 200 - Utils::random(0, 2 * (Constants::DESIGN_WIDTH / 2 - 200));
		int nY = Constants::DESIGN_HEIGHT / 2 - 200 - Utils::random(0, 2 * (Constants::DESIGN_HEIGHT / 2 - 200));
		Vec2 normalPos = Vec2(nX, Constants::DESIGN_HEIGHT / 2 + 100);

		normalPos += posGap;
		Armature* pSkillArmature = Armature::create("t_sk_0021");
		pSkillArmature->getAnimation()->play("Animation1");
		pSkillArmature->setPosition(normalPos);
		this->addChild(pSkillArmature);

		auto readyAction = CallFuncN::create([=](Node* pNode){
			Armature* pArmature = (Armature*)pNode;
			pArmature->getAnimation()->play("Animation2");
			pArmature->getAnimation()->pause();
		});

		auto fireAction = CallFuncN::create([=](Node* pNode){
			Armature* pArmature = (Armature*)pNode;
			pArmature->getAnimation()->resume();
		});

		auto bombAction = CallFuncN::create([=](Node* pNode){
			Armature* pArmature = (Armature*)pNode;
			pArmature->getAnimation()->play("Animation3");
		});

		auto checkAction = CallFuncN::create([=](Node* pNode){
			Rect heroRect = GameService::getInstance()->getGameScene()->getHero()->getHeroBox();
			Vec2 bombPos = pNode->getPosition();
			bombPos.x += Constants::DESIGN_WIDTH / 2;
			bombPos.y += Constants::DESIGN_HEIGHT / 2;
			bombPos = GameService::getInstance()->getGameScene()->getBackGround()->convertToNodeSpace(bombPos);
			Rect bombRect = Rect(bombPos.x - 250, bombPos.y - 250, 250, 250);
			if (bombRect.intersectsRect(heroRect))
			{
				GameService::getInstance()->getGameScene()->getHero()->hurt(20);
			}
		});

		
		

		auto releaseAction = CallFuncN::create([=](Node* pNode){
			pNode->removeFromParentAndCleanup(true);
		});

		pSkillArmature->runAction(Sequence::create(MoveTo::create(1.5f, Vec2(normalPos.x, nY)), readyAction,
			DelayTime::create(2.0f), fireAction,
			DelayTime::create(3.0f), bombAction, DelayTime::create(0.25f), checkAction,
			DelayTime::create(3.0f), releaseAction, NULL));

	}
	else if (_nSkillId == 2)
	{
		_fCdTime = 10.0f;
		ArmatureDataManager::getInstance()->addArmatureFileInfo("images/skills/boss_skill/t_sk_0010.ExportJson");
		ArmatureDataManager::getInstance()->addArmatureFileInfo("images/skills/boss_skill/t_sk_0007.ExportJson");

		
			auto releaseAction = CallFuncN::create([=](Node* pNode){
				pNode->removeFromParentAndCleanup(true);
			});

			Vec2 curPos = GameService::getInstance()->getGameScene()->getBoss()->getPosition();
			Vec2 heroPos = GameService::getInstance()->getGameScene()->getHero()->getPosition();
			heroPos = GameService::getInstance()->getGameScene()->getBackGround()->convertToNodeSpace(heroPos);
			float fAngle = Utils::getPosAngle(heroPos, curPos);

			Armature* pSkillArmature = Armature::create("t_sk_0010");
			pSkillArmature->getAnimation()->play("Animation1");
			this->addChild(pSkillArmature);
			this->setPosition(curPos);
			pSkillArmature->setRotation(-fAngle);
        
            pSkillArmature->runAction(Sequence::create(DelayTime::create(5.0f), releaseAction, NULL));

			auto fireAction = CallFuncN::create([=](Node* pNode){
				Armature* pSkillArmature = (Armature*)pNode;
				float fRoation = pSkillArmature->getRotation();
				Vec2 pos = pSkillArmature->getPosition();
				pSkillArmature->removeFromParentAndCleanup(true);
				pSkillArmature = Armature::create("t_sk_0007");
				pSkillArmature->getAnimation()->play("Animation1", -1, 0);
				pSkillArmature->setRotation(fRoation);
				pSkillArmature->setPosition(pos);
				this->addChild(pSkillArmature);

				float fAngle = -fRoation * M_PI/180;

				CCLOG("angle%f", -fRoation);
			});
		
			pSkillArmature->runAction(Sequence::createWithTwoActions(DelayTime::create(1.5f), fireAction));

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

	auto checkAction = CallFuncN::create([=](Node* pNode){
		Rect heroRect = GameService::getInstance()->getGameScene()->getHero()->getHeroBox();
		Vec2 bombPos = pNode->getPosition();
		bombPos.x += Constants::DESIGN_WIDTH / 2;
		bombPos.y += Constants::DESIGN_HEIGHT / 2;
		bombPos = GameService::getInstance()->getGameScene()->getBackGround()->convertToNodeSpace(bombPos);
		Rect bombRect = Rect(bombPos.x - 100 / 2, bombPos.y - 100 / 2, 100, 100);
		if (bombRect.intersectsRect(heroRect))
		{
			GameService::getInstance()->getGameScene()->getHero()->hurt(10);
		}
	});

	auto releaseAction = CallFunc::create([=](){
		pNode->removeFromParentAndCleanup(true);
	});

	pNode->runAction(Sequence::create(EaseOut::create(MoveTo::create(fDis / 500, targetPos), 0.5f), bombAction,
		checkAction, DelayTime::create(2.5f), releaseAction, NULL));
}

void Skill::skill2Attack(Armature* pNode)
{

}

void Skill::update(float delta)
{
	if (_fCdTime > 0)
	{
		_fCdTime -= delta;
	}
}

bool Skill::IsInCd()
{
	return _fCdTime > 0 ? true:false;
}