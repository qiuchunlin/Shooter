#ifndef _UTILS_ASTAR_
#define _UTILS_ASTAR_
#include "cocos2d.h"
USING_NS_CC;

#define MAP_WIDTH 46
#define MAP_HEIGHT 30

class PathItem :public Ref
{
public:
	PathItem* clone();
	PathItem* _pParentPath = nullptr;//父节点
	PathItem* _pChildPath = nullptr;//子节点
	float _fDisToStart = 0;//到起始点的距离
	int _nX = 0;//地图坐标
	int _nY = 0;
	float _fFValue = 0;
	Vec2 _posInMap = Vec2::ZERO;
};

class AStarInfo
{
public:
	int _nStartX = 0;//开始点
	int _nStartY = 0;

	int _nEndX = 0;//结束点
	int _nEndY = 0;

	AStarInfo* clone();

	void calculatePath(Node* pCurNode);//计算路径

	void clearPath();//清理路径

	Vector<PathItem*> _vOpenList;//开放列表(里面存放相邻节点)
	PathItem* _aSearchArr[MAP_WIDTH][MAP_HEIGHT];//全部需要检测的点
	Vector<PathItem*> _vPathList;//路径列表

	PathItem* getFromSearchList(int x, int y);//根据横纵坐标从检测数组中获取点

private:
	Vec2 getMinPath(Vec2 pos, Node* pCurNode);
	float getDis(PathItem* obj1, PathItem* obj2);//计算两个物体间的距离
	void SearchNodes(PathItem* node, PathItem* adjacent, PathItem* endNode);//把相邻的节点放入开放节点中
	PathItem* getMinPath();
private:
	Vector<PathItem*> _vSearchedList;//检测过的列表
};
#endif