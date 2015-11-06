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

	Vec2 center = Vec2(Constants::DESIGN_WIDTH / 2, Constants::DESIGN_HEIGHT / 2);
	center = GameService::getInstance()->getGameScene()->getBackGround()->convertToNodeSpace(center);

	Vec2 posGap = center - this->getPosition();
	if (_nSkillId == 0)
	{
		_fCdTime = 8.0f;
		ArmatureDataManager::getInstance()->addArmatureFileInfo("images/skills/boss_skill/t_sk_0019.ExportJson");

		for (int i = 0; i < 10; i++)
		{
			int nX = 200 + Utils::random(0, Constants::DESIGN_WIDTH - 400);
			int nY = 200 + Utils::random(0, Constants::DESIGN_HEIGHT - 400);

			Vec2 targetPos = Vec2(nX, nY);
			targetPos = GameService::getInstance()->getGameScene()->getBackGround()->convertToNodeSpace(targetPos);

			Armature* pSkillArmature = Armature::create("t_sk_0019");
			pSkillArmature->getAnimation()->play("Animation2");
			GameService::getInstance()->getGameScene()->getBackGroundUp()->addChild(pSkillArmature);
			pSkillArmature->setPosition(targetPos);

			auto skillAtk = CallFunc::create(CC_CALLBACK_0(Skill::skill1Attack, this, pSkillArmature));

			pSkillArmature->runAction(Sequence::create(DelayTime::create(2.0f), skillAtk, NULL));
		}
	}
	else if (_nSkillId == 1)
	{
		_fCdTime = 6.0f;
		ArmatureDataManager::getInstance()->addArmatureFileInfo("images/skills/boss_skill/t_sk_0021.ExportJson");

		for (int i = 0; i < 3; i++)
		{
			int nX = 200 + Utils::random(0, Constants::DESIGN_WIDTH - 400);
			int nY = 200 + Utils::random(0, Constants::DESIGN_HEIGHT - 400);

			Vec2 targetPos = Vec2(nX, nY);
			targetPos = GameService::getInstance()->getGameScene()->getBackGround()->convertToNodeSpace(targetPos);

			Armature* pSkillArmature = Armature::create("t_sk_0021");
			pSkillArmature->getAnimation()->play("Animation1");

			this->getParent()->addChild(pSkillArmature);

			Vec2 curPos = GameService::getInstance()->getGameScene()->getBoss()->getPosition();
			pSkillArmature->setPosition(curPos);

			auto readyAction = CallFuncN::create([=](Node* pNode){
				Armature* pArmature = (Armature*)pNode;
				pArmature->getAnimation()->play("Animation2");
				pArmature->getAnimation()->pause();
				_vArmatures.pushBack(pArmature);
			});

			pSkillArmature->runAction(Sequence::create(MoveTo::create(1.5f, targetPos), readyAction, NULL));
		}
	}
	else if (_nSkillId == 2)
	{
		_fCdTime = 10.0f;
		ArmatureDataManager::getInstance()->addArmatureFileInfo("images/skills/boss_skill/t_sk_0010.ExportJson");
		ArmatureDataManager::getInstance()->addArmatureFileInfo("images/skills/boss_skill/t_sk_0007.ExportJson");

		Vec2 curPos = GameService::getInstance()->getGameScene()->getBoss()->getPosition();
		Vec2 heroPos = GameService::getInstance()->getGameScene()->getHero()->getPosition();
		heroPos = GameService::getInstance()->getGameScene()->getBackGround()->convertToNodeSpace(heroPos);
		float fAngle = Utils::getPosAngle(heroPos, curPos);

		Armature* pSkillArmature = Armature::create("t_sk_0010");
		pSkillArmature->getAnimation()->play("Animation1");
		GameService::getInstance()->getGameScene()->getBackGround()->addChild(pSkillArmature);
		pSkillArmature->setPosition(curPos);
		pSkillArmature->setRotation(-fAngle);
        
   
		auto fireAction = CallFuncN::create([=](Node* pNode){

			auto checkAction = CallFuncN::create([=](Node* pNode){
				float fRoation = pNode->getRotation();
				Rect heroRect = GameService::getInstance()->getGameScene()->getHero()->getHeroBox();
				float fAngle = -fRoation * M_PI / 180;
				float fY1 = (heroPos.y - curPos.y) / (heroPos.x - curPos.x) *(heroRect.getMinX() - curPos.x) + curPos.y;
				float fY2 = (heroPos.y - curPos.y) / (heroPos.x - curPos.x) *(heroRect.getMaxX() - curPos.x) + curPos.y;

				if ((fY1 >= heroRect.getMinY() && fY1 <= heroRect.getMaxY()) || (fY2 >= heroRect.getMinY() && fY2 <= heroRect.getMaxY()))
				{
					GameService::getInstance()->getGameScene()->getHero()->hurt(50);
				}
			});

			auto releaseAction = CallFuncN::create([=](Node* pNode){
				pNode->removeFromParentAndCleanup(true);
			});

			Armature* pSkillArmature = (Armature*)pNode;
			float fRoation = pSkillArmature->getRotation();
			Vec2 pos = pSkillArmature->getPosition();
			pSkillArmature->removeFromParentAndCleanup(true);
			pSkillArmature = Armature::create("t_sk_0007");
			pSkillArmature->getAnimation()->play("Animation1", -1, 0);
			pSkillArmature->setRotation(fRoation);
			pSkillArmature->setPosition(pos);
			GameService::getInstance()->getGameScene()->getBackGroundUp()->addChild(pSkillArmature);

			pSkillArmature->runAction(Sequence::create(DelayTime::create(0.5f), checkAction, DelayTime::create(3.0f), releaseAction, NULL));
		});	
		
		pSkillArmature->runAction(Sequence::create(DelayTime::create(1.5f), fireAction,NULL));

	}

}

void Skill::skill1Attack(Armature* pNode)
{
	Vec2 targetPos = pNode->getPosition();
	Vec2 normalPos = Vec2(targetPos.x, Constants::DESIGN_HEIGHT + 100);
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
		Rect bombRect = Rect(bombPos.x - 60 / 2, bombPos.y - 60 / 2, 60, 60);
		if (bombRect.intersectsRect(heroRect))
		{
			GameService::getInstance()->getGameScene()->getHero()->hurt(15);
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
		Rect bombRect = Rect(bombPos.x - 100 / 2, bombPos.y - 100 / 2, 100, 100);
		if (bombRect.intersectsRect(heroRect))
		{
			GameService::getInstance()->getGameScene()->getHero()->hurt(20);
		}
	});

	auto releaseAction = CallFuncN::create([=](Node* pNode){
		pNode->removeFromParentAndCleanup(true);
	});

	pNode->runAction(Sequence::create(fireAction,DelayTime::create(2.5f), bombAction, DelayTime::create(0.25f), checkAction,
		DelayTime::create(3.0f), releaseAction, NULL));
}

void Skill::update(float delta)
{
	if (_fCdTime > 0)
	{
		_fCdTime -= delta;
	}

	if (_nSkillId == 1)
	{
		Rect heroRect = GameService::getInstance()->getGameScene()->getHero()->getHeroBox();		
		for (auto& pArmature : _vArmatures)
		{	
			Vec2 bombPos = pArmature->getPosition();
			Rect bombRect = Rect(bombPos.x - 100/2, bombPos.y - 100/2, 100, 100);
			if (bombRect.intersectsRect(heroRect))
			{
				this->skill2Attack(pArmature);
				_vArmatures.eraseObject(pArmature);
				break;
			}
		}
		
	}
}

bool Skill::IsInCd()
{
	return _fCdTime > 0 ? true:false;
}