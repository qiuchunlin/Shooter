#include "GameScene.h"
#include "Joystick.h"
#include "Common/Constants.h"
#include "Service/GameService.h"
#include "Utils/Utils.h"
#include "Sprite/Boss.h"

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();

	scene->addChild(layer);

	return scene;
}

bool GameScene::init()
{
	if ( !Layer::init() )
	{
		return false;
	}
	GameService::getInstance()->setGameScene(this);
	this->initUI();
	this->initTouch();
	this->initHero();
	this->scheduleUpdate();
	return true;
}

void GameScene::initUI()
{
	
	_pLeftJoystick = Joystick::create(true);
	_pLeftJoystick->setPosition(Vec2(200, 200));
	this->addChild(_pLeftJoystick, Constants::ZORDER_UI);

	_pRightJoystick = Joystick::create(false);
	_pRightJoystick->setPosition(Vec2(900, 200));
	this->addChild(_pRightJoystick, Constants::ZORDER_UI);

	_pBackGround = TMXTiledMap::create("images/maps/scene001.tmx");
	_pBackGround->setPosition(Vec2::ZERO);
	this->addChild(_pBackGround);

	for (auto& pGroup : _pBackGround->getObjectGroups())
	{
		ValueMap& dict = pGroup->getProperties();
		string sFileName = dict["filename"].asString();
		if (sFileName != "")
		{
			sFileName = "images/maps/" + sFileName;
			Vec2 spritePos = Vec2::ZERO;
			for (auto& pItem : pGroup->getObjects())
			{
				Sprite* pSprite = Sprite::create(sFileName);
				ValueMap& itemDict = pItem.asValueMap();
				spritePos.x = itemDict["x"].asFloat();
				spritePos.y = itemDict["y"].asFloat() + pSprite->getContentSize().height;
				_pBackGround->addChild(pSprite);
				pSprite->setAnchorPoint(Vec2(0, 0));
				pSprite->setPosition(spritePos);
			}
		}
	}

	auto  pCollideGroup = _pBackGround->getObjectGroup("collide_people");
	for (auto& pItem : pCollideGroup->getObjects())
	{
		ValueMap& itemDict = pItem.asValueMap();
		float fX = itemDict["x"].asFloat();
		float fY = itemDict["y"].asFloat();
		float fHeight = itemDict["height"].asFloat();
		float fWidth = itemDict["width"].asFloat();
		Rect collodeRect = Rect(fX, fY, fWidth, fHeight);
		collideRects.push_back(collodeRect);
	}
	
	//��ֹ�ڱ�
	/*Director::getInstance()->setProjection(Director::Projection::_2D);
	for (auto& pNode : _pBackGround->getChildren())
	{
		((SpriteBatchNode*)pNode)->getTexture()->setAntiAliasTexParameters();
	}*/

	_pPathInfo = new PathSearchInfo();

	TMXLayer* _road = _pBackGround->getLayer("background");//����·���ĵ�ͼ
	Size _mapSize = _pBackGround->getMapSize();
	for (int j = 0; j < _mapSize.height; j++) {
		for (int i = 0; i < _mapSize.width; i++) {
			Sprite* _sp = _road->getTileAt(Point(i, j));
			for (auto& rect : collideRects)
			{
				if (_sp->getBoundingBox().intersectsRect(rect))
				{
					_pPathInfo->m_inspectArray[i][j] = nullptr;
					_sp = nullptr;
					break;
				}
			}

			if (_sp) {
				PathSprite* _pathSprite = new PathSprite();
				_pathSprite->m_x = i;
				_pathSprite->m_y = j;
				_pathSprite->_pos = _pBackGround->getLayer("background")->getPositionAt(Vec2(i,j));
				_pPathInfo->m_inspectArray[i][j] = _pathSprite;//�ѵ�ͼ�����еĵ�һһ��Ӧ�������б���

			}
		}
	}

	TMXObjectGroup *objects = _pBackGround->getObjectGroup("Objects");
	for (auto& object : objects->getObjects())
	{
		ValueMap& dict = object.asValueMap();
		Vec2 monsterPos;
		if (dict["MonsterType"].asInt() == 1) {
			monsterPos.x = dict["x"].asFloat();
			monsterPos.y = dict["y"].asFloat();
			Monster* pMonster = Monster::create();
			_vMonsters.pushBack(pMonster);
			_pBackGround->addChild(pMonster, Constants::ZORDER_MONSTER);
			pMonster->setPosition(monsterPos);
		}
	}

	Boss* pBoss = Boss::create();
	pBoss->setPosition(Vec2(300, 300));
	_pBackGround->addChild(pBoss, Constants::ZORDER_MONSTER);

}

void GameScene::initTouch()
{
	auto _touchListener = EventListenerTouchOneByOne::create();
	_touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	_touchListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	_touchListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_touchListener, this);
}

void GameScene::initHero()
{
	_pHero = Hero::create();
	_pHero->setPosition(Vec2(Constants::DESIGN_WIDTH / 2, Constants::DESIGN_HEIGHT / 2));

	TMXObjectGroup *objects = _pBackGround->getObjectGroup("Objects");	
	auto spawnPoint = objects->getObject("HeroPos");

	float x = spawnPoint["x"].asFloat();
	float y = spawnPoint["y"].asFloat();

	Point startPos = Utils::getPosInMap(Vec2(x,y), _pBackGround);

	Vec2 bgPos = Vec2::ZERO;
	if (y > Constants::DESIGN_HEIGHT/2)
	{
		bgPos.y = Constants::DESIGN_HEIGHT - y - Constants::DESIGN_HEIGHT/2;
		if (bgPos.y < -_pBackGround->getContentSize().height + Constants::DESIGN_HEIGHT)
		{
			bgPos.y = -_pBackGround->getContentSize().height + Constants::DESIGN_HEIGHT;
		}
	}

	if (x > Constants::DESIGN_WIDTH/2)
	{
		bgPos.x = Constants::DESIGN_WIDTH - x - Constants::DESIGN_WIDTH / 2;
		if (bgPos.x < -_pBackGround->getContentSize().width + Constants::DESIGN_WIDTH)
		{
			bgPos.x = -_pBackGround->getContentSize().width + Constants::DESIGN_WIDTH;
		}
	}
	_pBackGround->setPosition(bgPos);

	Vec2 heroPos = Vec2(x, y);
	heroPos = _pBackGround->convertToWorldSpace(heroPos);

	_pHero->setPosition(heroPos);

	this->addChild(_pHero, Constants::ZORDER_HERO);
	_pHero->stand();
}

void  GameScene::addBubble(Bubble* pBubble)
{
	_vBubbles.pushBack(pBubble);
}

void GameScene::removeBubble(Bubble* pBubble)
{
	_vBubbles.eraseObject(pBubble);
}

void GameScene::killMonster(Monster* pMonster)
{
	_vMonsters.eraseObject(pMonster);
}


void GameScene::update(float delta)
{
	Vec2 heroPos = _pHero->getPosition();
	heroPos = _pBackGround->convertToNodeSpace(heroPos);
	float fCheckSide = 300;
	for (auto& pMonster:_vMonsters)
	{
		Rect checkBox = Rect(pMonster->getPosition().x - fCheckSide, pMonster->getPosition().y - fCheckSide, 2 * fCheckSide, 2 * fCheckSide);
		if (!pMonster->getIsRun() && checkBox.containsPoint(heroPos))
		{
			pMonster->run();
		}
	}
}

void GameScene::updateMonster()
{
	for (auto& pMonster : _vMonsters)
	{
		pMonster->run();
	}
}

bool GameScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	return true;
}

void GameScene::onTouchMoved(Touch *touch, Event *unused_event)
{

}

void GameScene::onTouchEnded(Touch *touch, Event *unused_event)
{

}
