#ifndef _SPRITE_MONSTER_MONSTER_
#define _SPRITE_MONSTER_MONSTER_
#include "cocos2d.h"
#include "../Common/Config.h"
USING_NS_CC;
using namespace std;
using namespace Config;

#define MAP_WIDTH 46//要比tmx中的map大
#define MAP_HEIGHT 30
class PathSprite:public Ref
{

public:
	PathSprite* m_parent = nullptr;//父节点
	PathSprite* m_child = nullptr;//子节点
	float m_costToSource = 0;//到起始点的距离
	int m_x = 0;//地图坐标
	int m_y = 0;
	float m_FValue = 0;
	bool _bIsTemp = false;
	Vec2 _pos = Vec2::ZERO;
};
class PathSearchInfo//寻路类(主要负责寻路的参数和逻辑)
{
public:

	 int m_startX;//开始点
	 int m_startY;

	 int m_endX = 0;//结束点
	 int m_endY = 0;

	 Vector<PathSprite*> m_openList;//开放列表(里面存放相邻节点)
	 PathSprite* m_inspectArray[MAP_WIDTH][MAP_HEIGHT];//全部需要检测的点
	 Vector<PathSprite*> m_pathList;//路径列表
	 Vector<PathSprite*> m_haveInspectList;//检测过的列表
	 Vector<PathSprite*> m_monsterPathList;//路径列表

	
	 float calculateTwoObjDistance(PathSprite* obj1, PathSprite* obj2)//计算两个物体间的距离
	{

		float _x = abs(obj2->m_x - obj1->m_x);
		float _y = abs(obj2->m_y - obj1->m_y);

		return _x + _y;
	}
	 void inspectTheAdjacentNodes(PathSprite* node, PathSprite* adjacent, PathSprite* endNode, PathSearchInfo*& pathSearchInfo)//把相邻的节点放入开放节点中
	{
		if (adjacent!=nullptr)
		{
			float _x = abs(endNode->m_x - adjacent->m_x);
			float _y = abs(endNode->m_y - adjacent->m_y);

			float F, G, H2;
			adjacent->m_costToSource = node->m_costToSource + calculateTwoObjDistance(node, adjacent);//获得累计的路程
			G = adjacent->m_costToSource;

			//三种算法, 感觉H2不错
			H2 = hypot(_x, _y);
//			H3 = max(_x, _y);

#if 1 //A*算法 = Dijkstra算法 + 最佳优先搜索
//			F = G + H2;
#endif
#if 0//Dijkstra算法
			F = G;
#endif
#if 1//最佳优先搜索
			F = H2;
#endif
			adjacent->m_FValue = F;

			adjacent->m_parent = node;//设置父节点

			m_haveInspectList.pushBack(adjacent);
			node->m_child = adjacent;//设置子节点

			pathSearchInfo->m_inspectArray[adjacent->m_x][adjacent->m_y] = nullptr;//把检测过的点从检测列表中删除
			pathSearchInfo->m_openList.pushBack(adjacent);//加入开放列表
		}
	}
	 PathSprite* getMinPathFormOpenList()//从开放节点中获取F值最小值的点
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
	 PathSprite* getObjFromInspectArray(int x, int y)//根据横纵坐标从检测数组中获取点
	{
		 if (x >= 0 && y >= 0 && x < MAP_WIDTH && y < MAP_HEIGHT) {
			return m_inspectArray[x][y];
		}
		return  nullptr;
	}
	 void removeObjFromOpenList(PathSprite* sprite)//从开放列表中移除对象
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

	void calculatePath();//计算路径

	void clearPath();//清理路径

	void autoMove();//人物走动

	int getDirection(float angle);

	Vec2 getRandomTarget(Vec2 target);

	Vec2 getMinTile(Vec2 pos);

	

protected:

	int m_playerMoveStep;//人物当前的行程的索引

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