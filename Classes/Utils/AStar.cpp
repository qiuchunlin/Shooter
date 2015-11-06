#include "AStar.h"

PathItem* PathItem::clone()
{
	PathItem* newPath = new PathItem();
	newPath->_nX = _nX;
	newPath->_nY = _nY;
	newPath->_posInMap = _posInMap;
	return newPath;
}

AStarInfo* AStarInfo::clone()
{
	AStarInfo* pAStarInfo = new AStarInfo();
	for (int j = 0; j < MAP_HEIGHT; j++) {
		for (int i = 0; i < MAP_WIDTH; i++) {
			PathItem* pathSprite = _aSearchArr[i][j];
			if (pathSprite!=nullptr)
			{
				pAStarInfo->_aSearchArr[i][j] = pathSprite->clone();
			}
			else
			{
				pAStarInfo->_aSearchArr[i][j] = nullptr;
			}
		}
	}
	return pAStarInfo;
}

float AStarInfo::getDis(PathItem* obj1, PathItem* obj2)
{

	float fX = abs(obj2->_nX - obj1->_nX);
	float fY = abs(obj2->_nY - obj1->_nY);

	return fX + fY;
}

void AStarInfo::SearchNodes(PathItem* node, PathItem* sideNode, PathItem* endNode)//�����ڵĽڵ���뿪�Žڵ���
{
	if (sideNode != nullptr)
	{
		float fX = abs(endNode->_nX - sideNode->_nX);
		float fY = abs(endNode->_nY - sideNode->_nY);

		float F, G, H2;
		sideNode->_fDisToStart = node->_fDisToStart + getDis(node, sideNode);//����ۼƵ�·��
		G = sideNode->_fDisToStart;
		H2 = hypot(fX, fX);

		int nFunction = 0;
		if (nFunction == 0)
		{
			//A*�㷨 = Dijkstra�㷨 + �����������
			F = G + H2;
		}
		else if (nFunction == 1)
		{
			//Dijkstra�㷨
			F = G;
		}
		else if (nFunction == 2)
		{
			//�����������
			F = H2;
		}

		sideNode->_fFValue = F;

		sideNode->_pParentPath = node;//���ø��ڵ�

		_vSearchedList.pushBack(sideNode);
		node->_pChildPath = sideNode;//�����ӽڵ�

		_aSearchArr[sideNode->_nX][sideNode->_nY] = nullptr;//�Ѽ����ĵ�Ӽ���б���ɾ��
		_vOpenList.pushBack(sideNode);//���뿪���б�
	}
}

PathItem* AStarInfo::getMinPath()
{
	PathItem* pPath = nullptr;
	if (_vOpenList.size() > 0)
	{
		pPath = _vOpenList.at(0);
		for (auto& pItem : _vOpenList)
		{
			if (pItem->_fFValue < pPath->_fFValue)
			{
				pPath = pItem;
			}
		}
	}
	return pPath;
}

PathItem* AStarInfo::getFromSearchList(int x, int y)//���ݺ�������Ӽ�������л�ȡ��
{
	if (x >= 0 && y >= 0 && x < MAP_WIDTH && y < MAP_HEIGHT) {
		return _aSearchArr[x][y];
	}
	return  nullptr;
}

void AStarInfo::calculatePath(Node* pCurNode)
{
	//�õ���ʼ��Ľڵ�
	PathItem* pStartNode = getFromSearchList(_nStartX, _nStartY);
	//�õ�������Ľڵ�
	PathItem* pEndNode = getFromSearchList(_nEndX, _nEndY);

	if (pStartNode == nullptr)
	{
		Vec2 startPos = this->getMinPath(Vec2(_nStartX, _nStartY),pCurNode);
		_nStartX = startPos.x;
		_nStartY= startPos.y;
		pStartNode = getFromSearchList(_nStartX, _nStartY);
	}

	//��Ϊ�ǿ�ʼ�� �ѵ���ʼ��ľ�����Ϊ0, FֵҲΪ0
	pStartNode->_fDisToStart = 0;
	pStartNode->_fFValue = 0;

	//���Ѿ������ĵ�Ӽ���б���ɾ��
	_aSearchArr[_nStartX][_nStartY] = nullptr;
	//�Ѹõ�����Ѿ���������б���
	_vSearchedList.pushBack(pStartNode);
	//Ȼ����뿪���б�
	_vOpenList.pushBack(pStartNode);

	PathItem* pNode = nullptr;
	while (true)
	{
		//�õ�����ʼ������ĵ�(����ǵ�һ��ִ��, �õ��������)
		pNode = getMinPath();
		if (!pNode)
		{
			//�Ҳ���·��
			break;
		}
		//�Ѽ�����ĵ�ӿ����б���ɾ��
		_vOpenList.eraseObject(pNode);

		int nX = pNode->_nX;
		int nY = pNode->_nY;

		if (nX == _nEndX && nY == _nEndY)
		{
			break;
		}

		//���8����������ڽڵ��Ƿ���Է��뿪���б���
		PathItem* pSideNode = getFromSearchList(nX + 1, nY + 1);
		SearchNodes(pNode, pSideNode, pEndNode);

		pSideNode = getFromSearchList(nX + 1, nY);
		SearchNodes(pNode, pSideNode, pEndNode);

		pSideNode = getFromSearchList(nX + 1, nY - 1);
		SearchNodes(pNode, pSideNode, pEndNode);

		pSideNode = getFromSearchList(nX, nY - 1);
		SearchNodes(pNode, pSideNode, pEndNode);

		pSideNode = getFromSearchList(nX - 1, nY - 1);
		SearchNodes(pNode, pSideNode, pEndNode);

		pSideNode = getFromSearchList(nX - 1, nY);
		SearchNodes(pNode, pSideNode, pEndNode);

		pSideNode = getFromSearchList(nX - 1, nY + 1);
		SearchNodes(pNode, pSideNode, pEndNode);

		pSideNode = getFromSearchList(nX, nY + 1);
		SearchNodes(pNode, pSideNode, pEndNode);

	}

	while (pNode)
	{
		//��·������뵽·���б���
		_vPathList.insert(0, pNode);
		pNode = pNode->_pParentPath;
	}
}

void AStarInfo::clearPath()
{
	for (auto& pItem : _vSearchedList)
	{
		pItem->_fDisToStart = 0;
		pItem->_fFValue = 0;
		pItem->_pParentPath = nullptr;
		pItem->_pChildPath = nullptr;
		
		_aSearchArr[pItem->_nX][pItem->_nY] = pItem;
	}

	_vOpenList.clear();
	_vPathList.clear();
	_vSearchedList.clear();

}


Vec2  AStarInfo::getMinPath(Vec2 pos, Node* pCurNode)
{
	float fMinDis = 10000;
	Vec2 minPos = Vec2::ZERO;
	for (int i = 1; i < 9; i++)
	{
		Vec2 target = pos;
		if (i == 1)
		{
			target.x--;
			target.y--;
		}
		else if (i == 2)
		{
			target.y--;
		}
		else if (i == 3)
		{
			target.x++;
			target.y--;
		}
		else if (i == 4)
		{
			target.x--;
		}
		else if (i == 5)
		{
			target.x++;
		}
		else if (i == 6)
		{
			target.x--;
			target.y++;
		}
		else if (i == 7)
		{
			target.y++;
		}
		else if (i == 8)
		{
			target.x++;
			target.y++;
		}

		PathItem* pPath = getFromSearchList(target.x, target.y);
		if (pPath != nullptr)
		{
			float fDis = pCurNode->getPosition().getDistance(pPath->_posInMap);
			if (fDis < fMinDis)
			{
				fMinDis = fDis;
				minPos = target;
			}
		}
	}

	return minPos;
}