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
	PathItem* _pParentPath = nullptr;//���ڵ�
	PathItem* _pChildPath = nullptr;//�ӽڵ�
	float _fDisToStart = 0;//����ʼ��ľ���
	int _nX = 0;//��ͼ����
	int _nY = 0;
	float _fFValue = 0;
	Vec2 _posInMap = Vec2::ZERO;
};

class AStarInfo
{
public:
	int _nStartX = 0;//��ʼ��
	int _nStartY = 0;

	int _nEndX = 0;//������
	int _nEndY = 0;

	AStarInfo* clone();

	void calculatePath(Node* pCurNode);//����·��

	void clearPath();//����·��

	Vector<PathItem*> _vOpenList;//�����б�(���������ڽڵ�)
	PathItem* _aSearchArr[MAP_WIDTH][MAP_HEIGHT];//ȫ����Ҫ���ĵ�
	Vector<PathItem*> _vPathList;//·���б�

	PathItem* getFromSearchList(int x, int y);//���ݺ�������Ӽ�������л�ȡ��

private:
	Vec2 getMinPath(Vec2 pos, Node* pCurNode);
	float getDis(PathItem* obj1, PathItem* obj2);//�������������ľ���
	void SearchNodes(PathItem* node, PathItem* adjacent, PathItem* endNode);//�����ڵĽڵ���뿪�Žڵ���
	PathItem* getMinPath();
private:
	Vector<PathItem*> _vSearchedList;//�������б�
};
#endif