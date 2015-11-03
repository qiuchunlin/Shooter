#ifndef _SPRITE_MONSTER_MONSTER_
#define _SPRITE_MONSTER_MONSTER_
#include "cocos2d.h"
#include "../Common/Config.h"
USING_NS_CC;
using namespace std;
using namespace Config;

#define MAP_WIDTH 46//Ҫ��tmx�е�map��
#define MAP_HEIGHT 30
class PathSprite:public Ref
{

public:
	PathSprite* m_parent = nullptr;//���ڵ�
	PathSprite* m_child = nullptr;//�ӽڵ�
	float m_costToSource = 0;//����ʼ��ľ���
	int m_x = 0;//��ͼ����
	int m_y = 0;
	float m_FValue = 0;
	bool _bIsTemp = false;
	Vec2 _pos = Vec2::ZERO;
};
class PathSearchInfo//Ѱ·��(��Ҫ����Ѱ·�Ĳ������߼�)
{
public:

	 int m_startX;//��ʼ��
	 int m_startY;

	 int m_endX = 0;//������
	 int m_endY = 0;

	 Vector<PathSprite*> m_openList;//�����б�(���������ڽڵ�)
	 PathSprite* m_inspectArray[MAP_WIDTH][MAP_HEIGHT];//ȫ����Ҫ���ĵ�
	 Vector<PathSprite*> m_pathList;//·���б�
	 Vector<PathSprite*> m_haveInspectList;//�������б�
	 Vector<PathSprite*> m_monsterPathList;//·���б�

	
	 float calculateTwoObjDistance(PathSprite* obj1, PathSprite* obj2)//�������������ľ���
	{

		float _x = abs(obj2->m_x - obj1->m_x);
		float _y = abs(obj2->m_y - obj1->m_y);

		return _x + _y;
	}
	 void inspectTheAdjacentNodes(PathSprite* node, PathSprite* adjacent, PathSprite* endNode, PathSearchInfo*& pathSearchInfo)//�����ڵĽڵ���뿪�Žڵ���
	{
		if (adjacent!=nullptr)
		{
			float _x = abs(endNode->m_x - adjacent->m_x);
			float _y = abs(endNode->m_y - adjacent->m_y);

			float F, G, H2;
			adjacent->m_costToSource = node->m_costToSource + calculateTwoObjDistance(node, adjacent);//����ۼƵ�·��
			G = adjacent->m_costToSource;

			//�����㷨, �о�H2����
			H2 = hypot(_x, _y);
//			H3 = max(_x, _y);

#if 1 //A*�㷨 = Dijkstra�㷨 + �����������
//			F = G + H2;
#endif
#if 0//Dijkstra�㷨
			F = G;
#endif
#if 1//�����������
			F = H2;
#endif
			adjacent->m_FValue = F;

			adjacent->m_parent = node;//���ø��ڵ�

			m_haveInspectList.pushBack(adjacent);
			node->m_child = adjacent;//�����ӽڵ�

			pathSearchInfo->m_inspectArray[adjacent->m_x][adjacent->m_y] = nullptr;//�Ѽ����ĵ�Ӽ���б���ɾ��
			pathSearchInfo->m_openList.pushBack(adjacent);//���뿪���б�
		}
	}
	 PathSprite* getMinPathFormOpenList()//�ӿ��Žڵ��л�ȡFֵ��Сֵ�ĵ�
	{
		 PathSprite* pPath = nullptr;
		 if (m_openList.size() > 0)
		 {
			 pPath = m_openList.at(0);
			 for (auto& pItem : m_openList)
			 {
				 if (pItem->m_FValue < pPath->m_FValue)
				 {
					 pPath = pItem;
				 }
			 }
		 }
		 return pPath;
	}
	 PathSprite* getObjFromInspectArray(int x, int y)//���ݺ�������Ӽ�������л�ȡ��
	{
		 if (x >= 0 && y >= 0 && x < MAP_WIDTH && y < MAP_HEIGHT) {
			return m_inspectArray[x][y];
		}
		return  nullptr;
	}
	 void removeObjFromOpenList(PathSprite* sprite)//�ӿ����б����Ƴ�����
	{
		 if (sprite) {
			 m_openList.eraseObject(sprite);
		 }
	}
};

class Monster : public Sprite
{
public:
	CREATE_FUNC(Monster);

	CC_SYNTHESIZE(Sprite*, _pSprite, Sprite);
	CC_SYNTHESIZE(MonsterStatus, _emStatus, Status);
	CC_SYNTHESIZE(MonsterType, _emType, MonsterType);
	virtual void run();

	void hurt(float fAtk);
protected:
	bool init();
	
	virtual void attack();
	virtual void die();
	void update(float dt);

	void runAway();

protected:

	void calculatePath();//����·��

	void clearPath();//����·��

	void autoMove();//�����߶�

	int getDirection(float angle);

	Vec2 getRandomTarget(Vec2 target);

	Vec2 getMinTile(Vec2 pos);

	

protected:

	int m_playerMoveStep;//���ﵱǰ���г̵�����

	PathSearchInfo* _SearchInfo;

	Sprite* _pHpProgress;
	
	int _nLastDirection = 0;

	int _nTotalHp = 20;

	int _nCurHp = _nTotalHp;

	TMXTiledMap* _pBackGround;

	float _fSpeed = 150;
	int _nTarPosIndex = 1;
};
#endif