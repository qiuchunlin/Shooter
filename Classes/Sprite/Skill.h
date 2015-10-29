#ifndef _SPRITE_SKILL_
#define _SPRITE_SKILL_
#include "cocos2d.h"
#include "Skill.h"
USING_NS_CC;
using namespace std;
#include "cocostudio/CocoStudio.h"
using namespace cocostudio;
class Skill : public Sprite
{
public:
	static Skill* create(int nSkillId);
protected:
	bool init();
	void skill1Attack(Armature* pNode);
protected:

private:
	int _nSkillId = 0;
	Armature* _pSkillArmature = nullptr;
};
#endif