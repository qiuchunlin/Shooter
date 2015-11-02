#include "HeroSkill.h"
#include "../Service/GameService.h"
#include "../GameScene.h"
#include "../Utils/Utils.h"
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
	if (_nSkillId == 1)
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfo("images/skills/bomb_skill/t_sk_0012.ExportJson");
        _pSkillArmature = Armature::create("t_sk_0012");
        _pSkillArmature->getAnimation()->play("Animation1");
        this->addChild(_pSkillArmature);
        
        auto fireAction = CallFunc::create([&](){
            _pSkillArmature->getAnimation()->play("Animation2");
        });
        
        auto releaseAction = CallFunc::create([=](){
            this->removeFromParentAndCleanup(true);
        });
        
        this->runAction(Sequence::create(DelayTime::create(1.5f),fireAction,DelayTime::create(3.5f),releaseAction,NULL));
	}
    else if (_nSkillId == 2)
    {
        ArmatureDataManager::getInstance()->addArmatureFileInfo("images/skills/protect_skill/t_sk_0022.ExportJson");
        _pSkillArmature = Armature::create("t_sk_0022");
        _pSkillArmature->getAnimation()->play("Animation1");
        this->addChild(_pSkillArmature);
        
        auto releaseAction = CallFunc::create([=](){
            this->removeFromParentAndCleanup(true);
        });
        
        this->runAction(Sequence::create(DelayTime::create(10.0f),releaseAction,NULL));
    }

}