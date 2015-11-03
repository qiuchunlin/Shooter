#include "HeroSkill.h"
#include "../Service/GameService.h"
#include "../GameScene.h"
#include "../Utils/Utils.h"
#include "../Utils/LayoutUtil.h"
#include "../Common/Constants.h"


HeroSkill*  HeroSkill::create(int nSkillId)
{
	HeroSkill *pRet = new(std::nothrow) HeroSkill();
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


bool HeroSkill::init()
{
	if (!Sprite::init()) {
		return  false;
	}
	return true;
}

void  HeroSkill::skillEffect()
{
	auto releaseAction = CallFuncN::create([](Node* pNode){
		pNode->removeFromParentAndCleanup(true);
	});

	if (_nSkillId == 0)
	{
		GameService::getInstance()->getGameScene()->getHero()->setFireCount(3);
		auto skillFinish = CallFunc::create([](){
			GameService::getInstance()->getGameScene()->getHero()->setFireCount(1);
		});
		this->runAction(Sequence::createWithTwoActions(DelayTime::create(8.0f), skillFinish));
	}
	else if (_nSkillId == 1)
	{
		Vec2 center = Vec2(Constants::DESIGN_WIDTH / 2, Constants::DESIGN_HEIGHT / 2);
		center = GameService::getInstance()->getGameScene()->getBackGround()->convertToNodeSpace(center);

		ArmatureDataManager::getInstance()->addArmatureFileInfo("images/skills/bomb_skill/t_sk_0012.ExportJson");
		Armature* pSkillArmature = Armature::create("t_sk_0012");
        pSkillArmature->getAnimation()->play("Animation1");
        GameService::getInstance()->getGameScene()->getBackGround()->addChild(pSkillArmature);
		pSkillArmature->setPosition(center);
        
        auto fireAction = CallFuncN::create([](Node* pNode){
			Armature* pArmature = (Armature*)pNode;
			pArmature->getAnimation()->play("Animation2");

        });

		auto checkAction = CallFuncN::create([](Node* pNode){
			Rect rect = pNode->getBoundingBox();
			for (auto& pMonster : GameService::getInstance()->getGameScene()->getMonsters())
			{
				if (rect.intersectsRect(pMonster->getBoundingBox()))
				{
					pMonster->hurt(10);
				}
			}
		});
        
		pSkillArmature->runAction(Sequence::create(DelayTime::create(1.0f), fireAction, DelayTime::create(1.0f), checkAction, DelayTime::create(2.0f), releaseAction, NULL));
	}
    else if (_nSkillId == 2)
    {
		auto fiishAction = CallFuncN::create([](Node* pNode){
			GameService::getInstance()->getGameScene()->getHero()->setIsProtect(false);
			pNode->removeFromParentAndCleanup(true);
		});

        ArmatureDataManager::getInstance()->addArmatureFileInfo("images/skills/protect_skill/t_sk_0022.ExportJson");
		Armature* pSkillArmature = Armature::create("t_sk_0022");
        pSkillArmature->getAnimation()->play("Animation1");
        
		GameService::getInstance()->getGameScene()->getHero()->addChild(pSkillArmature);
		LayoutUtil::layoutParentCenter(pSkillArmature, 0, 15);
        
		pSkillArmature->runAction(Sequence::create(DelayTime::create(10.0f), fiishAction, NULL));

		GameService::getInstance()->getGameScene()->getHero()->setIsProtect(true);

    }

	

}