#ifndef _SPRITE_HERO_SKILL_
#define _SPRITE_HERO_SKILL_
#include "cocos2d.h"
USING_NS_CC;
using namespace std;
#include "cocostudio/CocoStudio.h"
using namespace cocostudio;
class HeroSkill : public Sprite
{
public:
	static HeroSkill* create(int nSkillId);
	void skillEffect();
protected:
	bool init();
private:
	int _nSkillId = 0;
};
#endif