#ifndef _SPRITE_MONSTER_BOSS_
#define _SPRITE_MONSTER_BOSS_
#include "cocos2d.h"
#include "Monster.h"
USING_NS_CC;
using namespace std;
#include "cocostudio/CocoStudio.h"
#include "Skill.h"
using namespace cocostudio;
class Boss : public Monster
{
public:
	CREATE_FUNC(Boss);
protected:
	bool init();
	void run() override;
	void attack() override;
	void update(float dt);
	int getDirection(float fAngle);
	void bossAtk();
	void die()override;
protected:
	
private:
	Armature* _pBossArmature = nullptr;
	float _fSkillCdTime = 10;
	Vector<Skill*> _vSkills;
	float _fLastAngle = 0;
	int _nMonsterAction = 0;
};
#endif