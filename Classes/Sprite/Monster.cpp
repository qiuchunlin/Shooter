#include "Monster.h"
#include "../Service/GameService.h"
#include "../GameScene.h"
#include "../Utils/Utils.h"
#include "../Utils/LayoutUtil.h"
#include "../Common/Constants.h"

bool Monster::init()
{
    if (!Sprite::init()) {
        return  false;
    }
	_emStatus = Monster_Stand;
	_emType = Type_Dog;

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("images/monster/m_00080.plist", "images/monster/m_00080.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("images/hero/t_die_00020.plist", "images/hero/t_die_00020.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("images/monster/Game_Monster_UI.plist", "images/monster/Game_Monster_UI.png");
	

	_pBackGround = GameService::getInstance()->getGameScene()->getBackGround();
	_SearchInfo = new PathSearchInfo();
	TMXLayer* _road = _pBackGround->getLayer("background");//行走路径的地图
	Size _mapSize = _pBackGround->getMapSize();
	for (int j = 0; j < _mapSize.height; j++) {
		for (int i = 0; i < _mapSize.width; i++) {
			Vec2 pos = _road->getPositionAt(Vec2(i, j));
			Rect tileRect = Rect(pos.x, pos.y, _pBackGround->getTileSize().width, _pBackGround->getTileSize().height);

			PathSprite* _pathSprite = new PathSprite();
			_pathSprite->m_x = i;
			_pathSprite->m_y = j;
			_pathSprite->_pos = Vec2(tileRect.getMidX(), tileRect.getMidY());
			_SearchInfo->m_inspectArray[i][j] = _pathSprite;//把地图中所有的点一一对应放入检测列表中

			if (i == 3 && j == 8)
			{
				int a = 0;
			}


			for (auto& rect : GameService::getInstance()->getGameScene()->getCollideRects())
			{
				if (Utils::IsContainsRect(rect, tileRect))
				{
					_SearchInfo->m_inspectArray[i][j] = nullptr;
					_pathSprite->release();
					break;
				}
			}
		}
	}
	

	_pSprite = Sprite::createWithSpriteFrameName("m_s05_0008_0000.png");
	this->addChild(_pSprite);
	this->setContentSize(Size(60, 60));
	_pSprite->setPosition(Vec2(30, 30));

	Animation* standAnimation = Animation::create();
	for (int i = 0; i < 7; ++i)
	{
		string fileName = StringUtils::format("m_s05_0008_000%d.png", i);
		standAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(fileName));
	}
	standAnimation->setDelayPerUnit(0.1f);
	_pSprite->runAction(RepeatForever::create(Animate::create(standAnimation)));

	
	//血条
	auto pHpBg = Sprite::createWithSpriteFrameName("Game_Blood_Bg_2.png");
	this->addChild(pHpBg);
	LayoutUtil::layoutParentCenter(pHpBg,0, 30);

	_pHpProgress = Sprite::createWithSpriteFrameName("Game_Blood_Pro_2.png");
	_pHpProgress->setAnchorPoint(Vec2::ZERO);
	pHpBg->addChild(_pHpProgress);
	LayoutUtil::layoutParentCenter(_pHpProgress);

	this->scheduleUpdate();
    return true;
}

void Monster::run()
{
	if (_emStatus == Monster_Stand)
	{
		_emStatus = Monster_Run;
	}
	auto runAction = CallFunc::create(CC_CALLBACK_0(Monster::run, this));
	this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.1f), runAction));

	this->stopActionByTag(100);

	Vec2 heroPos = GameService::getInstance()->getGameScene()->getHero()->getPosition();

	auto nodePosition = _pBackGround->convertToNodeSpace(heroPos);

	Vec2 monsterPos = this->getPosition();
	float fAngle = Utils::getPosAngle(nodePosition, monsterPos);
	int nDirection = this->getDirection(fAngle);
	if (fAngle > 90 && fAngle < 270)
	{
		_pSprite->setFlippedX(true);
	}
	else
	{
		_pSprite->setFlippedX(false);
	}

	if (_nLastDirection != nDirection)
	{
		_nLastDirection = nDirection;
		_pSprite->stopAllActions();
		Animation* standAnimation = Animation::create();
		for (int i = 0; i < 6; ++i)
		{
			string fileName = StringUtils::format("m_r0%d_0008_000%d.png", _nLastDirection, i);
			standAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(fileName));
		}
		standAnimation->setDelayPerUnit(0.1f);
		_pSprite->runAction(RepeatForever::create(Animate::create(standAnimation)));
	}



	Point startPos = Utils::getPosInMap(this->getPosition(), _pBackGround);
	//设置起始和终点
	_SearchInfo->m_startX = startPos.x;
	_SearchInfo->m_startY = startPos.y;

	////清除之前的路径
	clearPath();

	nodePosition = Utils::getPosInMap(nodePosition, _pBackGround);
	PathSprite* targetPath =_SearchInfo->getObjFromInspectArray((int)nodePosition.x, (int)nodePosition.y);
	
	//获取触摸点, 设置为终点
	_SearchInfo->m_endX = targetPath->m_x;
	_SearchInfo->m_endY = targetPath->m_y;
	//计算路径
	calculatePath();

	//移动物体
	autoMove();
}

int Monster::getDirection(float angle)
{
	int nCurDirection = 0;
	if ((angle >= 360 - 90 && angle <= 360 - 30) || (angle > 210 && angle <= 360 - 90))
	{
		nCurDirection = 1;
	}
	else if ((angle > 360 - 30 || angle <= 10) || (angle > 150 && angle <= 210))
	{
		nCurDirection = 3;
	}
	else if ((angle > 10 && angle <= 90) || (angle > 90 && angle <= 150))
	{
		nCurDirection = 5;
	}

	return nCurDirection;
}

void Monster::attack()
{
	_emStatus = Monster_Attack;
	this->stopAllActions();
	Animation* atkAnimation = Animation::create();
	for (int i = 1; i < 7; ++i)
	{
		string fileName = StringUtils::format("m_a0%d_0008_000%d.png", _nLastDirection, i);
		atkAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(fileName));
	}
	atkAnimation->setDelayPerUnit(0.1f);

	Animate* pAtkAni = Animate::create(atkAnimation);
	_pSprite->stopAllActions();
	_pSprite->runAction(Sequence::create(pAtkAni, CallFunc::create(CC_CALLBACK_0(Monster::runAway,this)), NULL));
	GameService::getInstance()->getGameScene()->getHero()->hurt(2);
}

void Monster::die()
{
	_emStatus = Monster_Die;
	GameService::getInstance()->getGameScene()->killMonster(this);

	Animation* dieAnimation = Animation::create();
	for (int i = 1; i < 10; ++i)
	{
		string fileName = StringUtils::format("t_die_0002_000%d.png", i);
		dieAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(fileName));
	}
	dieAnimation->setDelayPerUnit(0.1f);

	Animate* pDieAni = Animate::create(dieAnimation);

	auto releaseAction = CallFunc::create([&](){
		this->removeFromParentAndCleanup(true);
	});
	_pSprite->stopAllActions();
	this->stopAllActions();
	_pSprite->runAction(Sequence::create(pDieAni, FadeOut::create(0.5f), releaseAction, NULL));
}

void Monster::update(float dt)
{
	auto releaseAction = CallFuncN::create([](Node* pNode){
		pNode->removeFromParentAndCleanup(true);
	});
	Vector<Bubble*> vBubbles = GameService::getInstance()->getGameScene()->getBubbles();
	for (auto& pBubble : vBubbles)
	{
		Vec2 bubblePos = this->getParent()->convertToNodeSpace(pBubble->getPosition());
		Vec2 monsterPos = this->getPosition();
		Rect bubbRect = Rect(bubblePos.x - pBubble->getContentSize().width / 2, bubblePos.y - pBubble->getContentSize().height/2, pBubble->getContentSize().width, pBubble->getContentSize().height);
		Rect monsterRect = Rect(monsterPos.x - this->getContentSize().width / 2, monsterPos.y - this->getContentSize().height / 2, this->getContentSize().width, this->getContentSize().height);
		if (monsterRect.intersectsRect(bubbRect))
		{
			GameService::getInstance()->getGameScene()->removeBubble(pBubble);
			pBubble->runAction(Sequence::createWithTwoActions(DelayTime::create(0.2f), releaseAction));
			
			this->hurt(1);
			break;
		}
	}
}

void Monster::hurt(float fAtk)
{
	_nCurHp -= fAtk;
	float fPerfent = (float)_nCurHp / (float)_nTotalHp;
	_pHpProgress->setScaleX(fPerfent);
	if (_nCurHp <= 0)
	{
		_pHpProgress->getParent()->setVisible(false);
		this->unscheduleUpdate();
		this->die();
	}
}


void Monster::calculatePath()
{
	

	//得到开始点的节点
	PathSprite* _startNode = _SearchInfo->m_inspectArray[_SearchInfo->m_startX][_SearchInfo->m_startY];
	//得到结束点的节点
	PathSprite* _endNode =  _SearchInfo->m_inspectArray[ _SearchInfo->m_endX][ _SearchInfo->m_endY];
	
	if (_startNode == nullptr)
	{
		Vec2 startPos = this->getMinTile(Vec2(_SearchInfo->m_startX, _SearchInfo->m_startY));
		_SearchInfo->m_startX = startPos.x;
		_SearchInfo->m_startY = startPos.y;
		_startNode = _SearchInfo->m_inspectArray[_SearchInfo->m_startX][_SearchInfo->m_startY];
	}

	//因为是开始点 把到起始点的距离设为0, F值也为0
	_startNode->m_costToSource = 0;
	_startNode->m_FValue = 0;

	//把已经检测过的点从检测列表中删除
	 _SearchInfo->m_inspectArray[ _SearchInfo->m_startX][ _SearchInfo->m_startY] = nullptr;
	//把该点放入已经检测过点的列表中
	 _SearchInfo->m_haveInspectList.pushBack(_startNode);
	//然后加入开放列表
	 _SearchInfo->m_openList.pushBack(_startNode);

	 /*for (auto& pMonster : GameService::getInstance()->getGameScene()->getMonsters())
	 {
		 if (pMonster != this)
		 {
			 Vec2 pos = Utils::getPosInMap(pMonster->getPosition(), _pBackGround);
			 PathSprite* pPath = _SearchInfo->m_inspectArray[(int)pos.x][(int)pos.y];
			 if (pPath != nullptr)
			 {
				 _SearchInfo->m_inspectArray[(int)pos.x][(int)pos.y] = nullptr;
				 _SearchInfo->m_monsterPathList.pushBack(pPath);
			 }
		 }
	 }*/

	PathSprite* _node = nullptr;
	while (true)
	{
		//得到离起始点最近的点(如果是第一次执行, 得到的是起点)
		_node =  _SearchInfo->getMinPathFormOpenList();
		if (!_node)
		{
			//找不到路径
			break;
		}
		//把计算过的点从开放列表中删除
		 _SearchInfo->removeObjFromOpenList(_node);
		int _x = _node->m_x;
		int _y = _node->m_y;

		if (_x ==  _SearchInfo->m_endX && _y ==  _SearchInfo->m_endY)
		{
			break;
		}

		//检测8个方向的相邻节点是否可以放入开放列表中
		PathSprite* _adjacent =  _SearchInfo->getObjFromInspectArray(_x + 1, _y + 1);
		_SearchInfo->inspectTheAdjacentNodes(_node, _adjacent, _endNode, _SearchInfo);

		_adjacent =  _SearchInfo->getObjFromInspectArray(_x + 1, _y);
		_SearchInfo->inspectTheAdjacentNodes(_node, _adjacent, _endNode, _SearchInfo);

		_adjacent =  _SearchInfo->getObjFromInspectArray(_x + 1, _y - 1);
		_SearchInfo->inspectTheAdjacentNodes(_node, _adjacent, _endNode, _SearchInfo);

		_adjacent =  _SearchInfo->getObjFromInspectArray(_x, _y - 1);
		_SearchInfo->inspectTheAdjacentNodes(_node, _adjacent, _endNode, _SearchInfo);

		_adjacent =  _SearchInfo->getObjFromInspectArray(_x - 1, _y - 1);
		_SearchInfo->inspectTheAdjacentNodes(_node, _adjacent, _endNode, _SearchInfo);

		_adjacent =  _SearchInfo->getObjFromInspectArray(_x - 1, _y);
		_SearchInfo->inspectTheAdjacentNodes(_node, _adjacent, _endNode, _SearchInfo);

		_adjacent =  _SearchInfo->getObjFromInspectArray(_x - 1, _y + 1);
		_SearchInfo->inspectTheAdjacentNodes(_node, _adjacent, _endNode, _SearchInfo);

		_adjacent =  _SearchInfo->getObjFromInspectArray(_x, _y + 1);
		_SearchInfo->inspectTheAdjacentNodes(_node, _adjacent, _endNode, _SearchInfo);

	}

	while (_node)
	{
		//把路径点加入到路径列表中
		_SearchInfo->m_pathList.insert(0, _node);
		_node = _node->m_parent;
	}

	/*for (auto& pPath : _SearchInfo->m_monsterPathList)
	{
		_SearchInfo->m_inspectArray[pPath->m_x][pPath->m_y] = pPath;
	}
	_SearchInfo->m_monsterPathList.clear();*/
}

void Monster::clearPath()
{
	for (auto& pItem : _SearchInfo->m_haveInspectList)
	{
		pItem->m_costToSource = 0;
		pItem->m_FValue = 0;
		pItem->m_parent = NULL;
		pItem->m_child = NULL;
		if (!pItem->_bIsTemp)
		{
			_SearchInfo->m_inspectArray[pItem->m_x][pItem->m_y] = pItem;
		}
	}
	
	//把移除了障碍物的地图放入检测列表中
	 _SearchInfo->m_openList.clear();
	 _SearchInfo->m_pathList.clear();
	 _SearchInfo->m_haveInspectList.clear();

	m_playerMoveStep = 0;
}

void Monster::runAway()
{
	Vec2 heroPos = GameService::getInstance()->getGameScene()->getHero()->getPosition();
	heroPos = _pBackGround->convertToNodeSpace(heroPos);
	
	heroPos = Utils::getPosInMap(heroPos, _pBackGround);
	Vec2 targetPos = this->getRandomTarget(heroPos);
	_nTarPosIndex = Utils::random(1, 8);
	PathSprite* pTargetPath = nullptr;
	while (pTargetPath == nullptr)
	{
		targetPos = this->getRandomTarget(heroPos);
		pTargetPath = _SearchInfo->getObjFromInspectArray(targetPos.x, targetPos.y);
		if (pTargetPath == nullptr)
		{
			for (auto& pOpenPath : _SearchInfo->m_openList)
			{
				if (pOpenPath->m_x == targetPos.x && pOpenPath->m_y == targetPos.y)
				{
					pTargetPath = pOpenPath;
					break;
				}
			}			
		}

	}
	targetPos = _pBackGround->getLayer("background")->getPositionAt(targetPos);

	Vec2 monsterPos = this->getPosition();
	float fAngle = Utils::getPosAngle(targetPos, monsterPos);
	int nDirection = this->getDirection(fAngle);
	if (fAngle > 90 && fAngle < 270)
	{
		_pSprite->setFlippedX(true);
	}
	else
	{
		_pSprite->setFlippedX(false);
	}

	_nLastDirection = nDirection;
	_pSprite->stopAllActions();
	Animation* standAnimation = Animation::create();
	for (int i = 0; i < 6; ++i)
	{
		string fileName = StringUtils::format("m_r0%d_0008_000%d.png", _nLastDirection, i);
		standAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(fileName));
	}
	standAnimation->setDelayPerUnit(0.1f);
	_pSprite->runAction(RepeatForever::create(Animate::create(standAnimation)));
	
	_nLastDirection = 0;
	float fDis = this->getPosition().getDistance(targetPos);
	auto pMoveAction = Sequence::create(MoveTo::create(fDis / _fSpeed, targetPos), CallFunc::create(CC_CALLBACK_0(Monster::run, this)), NULL);
	this->runAction(pMoveAction);
}

void Monster::autoMove()
{
	m_playerMoveStep++;

	if (m_playerMoveStep >= _SearchInfo->m_pathList.size()) {
		this->attack();
		return;
	}

	Vec2 tarPos = _SearchInfo->m_pathList.at(m_playerMoveStep)->_pos;
	int nRandom = 1;
	if (Utils::randomPercent(50))
	{
		nRandom = -1;
	}

	tarPos.x += nRandom * Utils::random(0,10);
	nRandom = 1;
	if (Utils::randomPercent(50))
	{
		nRandom = -1;
	}
	tarPos.y += nRandom * Utils::random(0, 10);

	/*float fSideWidth = 50;
	if (tarPos.x < fSideWidth )
	{
		tarPos.x = fSideWidth;
	}
	if (tarPos.x > Constants::DESIGN_WIDTH - fSideWidth)
	{
		tarPos.x = Constants::DESIGN_WIDTH - fSideWidth;
	}

	if (tarPos.y < fSideWidth)
	{
		tarPos.y = fSideWidth;
	}
	if (tarPos.x > Constants::DESIGN_WIDTH - fSideWidth)
	{
		tarPos.x = Constants::DESIGN_WIDTH - fSideWidth;
	}*/

	float fDis = this->getPosition().getDistance(tarPos);

	

	/*Vec2 monsterPos = this->getPosition();
	float fAngle = Utils::getPosAngle(tarPos, monsterPos);
	int nDirection = this->getDirection(fAngle);
	if (fAngle > 90 && fAngle < 270)
	{
		_pSprite->setFlippedX(true);
	}
	else
	{
		_pSprite->setFlippedX(false);
	}

	if (_nLastDirection != nDirection)
	{
		_nLastDirection = nDirection;
		_pSprite->stopAllActions();
		Animation* standAnimation = Animation::create();
		for (int i = 0; i < 6; ++i)
		{
			string fileName = StringUtils::format("m_r0%d_0008_000%d.png", _nLastDirection, i);
			standAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(fileName));
		}
		standAnimation->setDelayPerUnit(0.1f);
		_pSprite->runAction(RepeatForever::create(Animate::create(standAnimation)));
	}*/

	auto pMoveAction = Sequence::create(MoveTo::create(fDis / _fSpeed, tarPos), CallFunc::create(CC_CALLBACK_0(Monster::autoMove, this)), NULL);
	pMoveAction->setTag(100);
	////根据路径列表移动人物
	this->runAction(pMoveAction);
	

}

Vec2 Monster::getRandomTarget(Vec2 target)
{
	_nTarPosIndex++;

	if (_nTarPosIndex > 8)
	{
		_nTarPosIndex = 1;
	}

	int nDirection = _nTarPosIndex;

	if (nDirection == 1)
	{
		target.x--;
		target.y--;
	}
	else if (nDirection == 2)
	{
		target.y--;
	}
	else if (nDirection == 3)
	{
		target.x++;
		target.y--;
	}
	else if (nDirection == 4)
	{
		target.x--;
	}
	else if (nDirection == 5)
	{
		target.x++;
	}
	else if (nDirection == 6)
	{
		target.x--;
		target.y++;
	}
	else if (nDirection == 7)
	{
		target.y++;
	}
	else if (nDirection == 8)
	{
		target.x++;
		target.y++;
	}

	return  target;
}

Vec2  Monster::getMinTile(Vec2 pos)
{
	float fMinDis = 1000;
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

		PathSprite* pPath = _SearchInfo->getObjFromInspectArray(target.x, target.y);
		if (pPath != nullptr)
		{
			float fDis = this->getPosition().getDistance(pPath->_pos);
			if (fDis < fMinDis)
			{
				fMinDis = fDis;
				minPos = target;
			}
		}
	}

	return minPos;
}