#include "Monster.h"
#include "../Service/GameService.h"
#include "../GameScene.h"
#include "../Utils/Utils.h"
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
	
	_SearchInfo = new PathSearchInfo();
	_SearchInfo = GameService::getInstance()->getGameScene()->_pPathInfo;
	_pBackGround = GameService::getInstance()->getGameScene()->getBackGround();

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
	//������ʼ���յ�
	_SearchInfo->m_startX = startPos.x;
	_SearchInfo->m_startY = startPos.y;

	////���֮ǰ��·��
	clearPath();

	nodePosition = Utils::getPosInMap(nodePosition, _pBackGround);
	PathSprite* targetPath =_SearchInfo->getObjFromInspectArray((int)nodePosition.x, (int)nodePosition.y);
	if (targetPath == nullptr)
	{
		targetPath = new PathSprite();
		targetPath->m_x = nodePosition.x;
		targetPath->m_y = nodePosition.y;
		targetPath->_pos = _pBackGround->getLayer("background")->getPositionAt(Vec2(nodePosition.x, nodePosition.y));
		_SearchInfo->m_inspectArray[targetPath->m_x][targetPath->m_y] = targetPath;
	}
	/*Vec2 targetPos = this->getRandomTarget(nodePosition);
	targetPath = _SearchInfo->getObjFromInspectArray((int)targetPos.x, (int)targetPos.y);
	while (targetPath == nullptr)
	{
	_nTarPosIndex = 0;
	targetPos = this->getRandomTarget(targetPos);
	targetPath = _SearchInfo->getObjFromInspectArray((int)targetPos.x, (int)targetPos.y);
	}*/

	//��ȡ������, ����Ϊ�յ�
	_SearchInfo->m_endX = targetPath->m_x;
	_SearchInfo->m_endY = targetPath->m_y;
	//����·��
	calculatePath();

	/*for (auto& pPath : _SearchInfo->m_openList)
	{
		auto sp = _pBackGround->getLayer("background")->tileAt(Vec2(pPath->m_x, pPath->m_y));
		sp->setColor(Color3B::YELLOW);
	}

	for (auto& pPath : _SearchInfo->m_pathList)
	{
		auto sp = _pBackGround->getLayer("background")->tileAt(Vec2(pPath->m_x, pPath->m_y));
		sp->setColor(Color3B::GREEN);
	}*/

	//�ƶ�����
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
			
			_nTotalHp--;
			if (_nTotalHp <= 0)
			{
				this->unscheduleUpdate();
				this->die();
			}			
			break;
		}
	}
}


void Monster::calculatePath()
{
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

	//�õ���ʼ��Ľڵ�
	PathSprite* _startNode = _SearchInfo->m_inspectArray[_SearchInfo->m_startX][_SearchInfo->m_startY];
	//�õ�������Ľڵ�
	PathSprite* _endNode =  _SearchInfo->m_inspectArray[ _SearchInfo->m_endX][ _SearchInfo->m_endY];
	if (_startNode == nullptr)
	{
		_startNode = new PathSprite();
		_startNode->m_x = _SearchInfo->m_startX;
		_startNode->m_y = _SearchInfo->m_startY;
		_startNode->_pos = _pBackGround->getLayer("background")->getPositionAt(Vec2(_SearchInfo->m_startX, _SearchInfo->m_startY));
		_startNode->_bIsTemp = true;
	}

	//��Ϊ�ǿ�ʼ�� �ѵ���ʼ��ľ�����Ϊ0, FֵҲΪ0
	_startNode->m_costToSource = 0;
	_startNode->m_FValue = 0;

	//���Ѿ������ĵ�Ӽ���б���ɾ��
	 _SearchInfo->m_inspectArray[ _SearchInfo->m_startX][ _SearchInfo->m_startY] = nullptr;
	//�Ѹõ�����Ѿ���������б���
	 _SearchInfo->m_haveInspectList.pushBack(_startNode);
	//Ȼ����뿪���б�
	 _SearchInfo->m_openList.pushBack(_startNode);

	PathSprite* _node = nullptr;
	while (true)
	{
		//�õ�����ʼ������ĵ�(����ǵ�һ��ִ��, �õ��������)
		_node =  _SearchInfo->getMinPathFormOpenList();
		if (!_node)
		{
			//�Ҳ���·��
			break;
		}
		//�Ѽ�����ĵ�ӿ����б���ɾ��
		 _SearchInfo->removeObjFromOpenList(_node);
		int _x = _node->m_x;
		int _y = _node->m_y;

		if (_x ==  _SearchInfo->m_endX && _y ==  _SearchInfo->m_endY)
		{
			break;
		}

		//���8����������ڽڵ��Ƿ���Է��뿪���б���
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
		//��·������뵽·���б���
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
	/*for (auto& pPath : _SearchInfo->m_openList)
	{
		auto sp = _pBackGround->getLayer("background")->tileAt(Vec2(pPath->m_x, pPath->m_y));
		sp->setColor(Color3B::WHITE);
	}

	for (auto& pPath : _SearchInfo->m_pathList)
	{
		auto sp = _pBackGround->getLayer("background")->tileAt(Vec2(pPath->m_x, pPath->m_y));
		sp->setColor(Color3B::WHITE);
	}*/

	
	//���Ƴ����ϰ���ĵ�ͼ�������б���
	 _SearchInfo->m_openList.clear();
	 _SearchInfo->m_pathList.clear();
	 _SearchInfo->m_haveInspectList.clear();

	m_playerMoveStep = 0;
}

void Monster::runAway()
{
//	_emStatus = Monster_Run;
	Vec2 heroPos = GameService::getInstance()->getGameScene()->getHero()->getPosition();
	heroPos = _pBackGround->convertToNodeSpace(heroPos);
	float fR = 70;
	int nDriection = 1;
	if (Utils::randomPercent(50))
	{
		nDriection = -1;
	}
	float fRandomX = Utils::random(0, fR*2);
	float fX = heroPos.x - (fR - fRandomX);
	float fY = nDriection*sqrtf(fR*fR - (fX - heroPos.x)*(fX - heroPos.x)) + heroPos.y;
	Vec2 targetPos = Vec2(fX, fY);
	

	for (auto& checkRect:GameService::getInstance()->getGameScene()->getCollideRects())
	{
		if (checkRect.containsPoint(targetPos))
		{
			targetPos.y = -nDriection*sqrtf(fR*fR - (fX - heroPos.x)*(fX - heroPos.x)) + heroPos.y;
			break;
		}
	}

	for (auto& checkRect : GameService::getInstance()->getGameScene()->getCollideRects())
	{
		if (checkRect.containsPoint(targetPos))
		{
			targetPos.x = heroPos.x - (fRandomX - fR);
			break;
		}
	}

	float fSideWidth = 50;
	if (targetPos.x < fSideWidth || targetPos.x > Constants::DESIGN_WIDTH - fSideWidth)
	{
		targetPos.x = heroPos.x - (fRandomX - fR);
	}

	if (targetPos.y < fSideWidth || targetPos.y > Constants::DESIGN_HEIGHT - fSideWidth)
	{
		targetPos.y = -nDriection*sqrtf(fR*fR - (fX - heroPos.x)*(fX - heroPos.x)) + heroPos.y;
	}

	

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

	//if (_nLastDirection != nDirection)
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

	float fSideWidth = 50;
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
	}

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
	////����·���б��ƶ�����
	this->runAction(pMoveAction);
	

}

Vec2 Monster::getRandomTarget(Vec2 target)
{
	if (_nTarPosIndex == 0)
	{
		_nTarPosIndex = Utils::random(1, 8);
	}

	Vec2 normalPos = target;
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