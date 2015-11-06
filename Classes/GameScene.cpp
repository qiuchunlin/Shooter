#include "GameScene.h"
#include "Joystick.h"
#include "Common/Constants.h"
#include "Service/GameService.h"
#include "Utils/Utils.h"
#include "Sprite/Boss.h"
#include "Sprite/HeroSkill.h"
#include "Utils/LayoutUtil.h"

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

	_pHero = nullptr;

	this->initUI();
	this->initHero();
	this->initMonster();
	this->scheduleUpdate();

	auto _touchListener = EventListenerTouchOneByOne::create();
	_touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	_touchListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	_touchListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_touchListener, this);

	return true;
}

void GameScene::initUI()
{
	_pLeftJoystick = Joystick::create(true);
	_pLeftJoystick->setPosition(Vec2(200, 150));
	this->addChild(_pLeftJoystick, Constants::ZORDER_UI + 1);

	_pRightJoystick = Joystick::create(false);
	_pRightJoystick->setPosition(Vec2(900, 150));
	this->addChild(_pRightJoystick, Constants::ZORDER_UI + 1);

	_pBackGround = TMXTiledMap::create("images/map/map.tmx");
	_pBackGround->setPosition(Vec2::ZERO);
	this->addChild(_pBackGround);

	_pBackGroundUp = Node::create();
	_pBackGroundUp->setContentSize(_pBackGround->getContentSize());
	_pBackGroundUp->setPosition(Vec2::ZERO);
	this->addChild(_pBackGroundUp,Constants::ZORDER_UI);

	for (auto& pObject : _pBackGround->getObjectGroup("obj_bg")->getObjects())
	{
		ValueMap& itemDict = pObject.asValueMap();
		string sFileName = itemDict["fileName"].asString();
		if (sFileName != "")
		{
			sFileName = "images/map/" + sFileName;
			Sprite* pSprite = Sprite::create(sFileName);
			Vec2 spritePos;
			spritePos.x = itemDict["x"].asFloat();
			spritePos.y = itemDict["y"].asFloat() + pSprite->getContentSize().height;
			
			pSprite->setAnchorPoint(Vec2(0, 0));
			pSprite->setPosition(spritePos);

			if (itemDict["zorder"].asInt() == 1)
			{
				_pBackGround->addChild(pSprite);
			}
			else
			{
				_pBackGroundUp->addChild(pSprite);
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

	_pAStarInfo = new AStarInfo();
	TMXLayer* _road = _pBackGround->getLayer("background");//行走路径的地图
	Size _mapSize = _pBackGround->getMapSize();
	for (int j = 0; j < _mapSize.height; j++) {
		for (int i = 0; i < _mapSize.width; i++) {
			Vec2 pos = _road->getPositionAt(Vec2(i, j));
			Rect tileRect = Rect(pos.x, pos.y, _pBackGround->getTileSize().width, _pBackGround->getTileSize().height);

			PathItem* _pathSprite = new PathItem();
			_pathSprite->_nX = i;
			_pathSprite->_nY = j;
			_pathSprite->_posInMap = Vec2(tileRect.getMidX(), tileRect.getMidY());
			_pAStarInfo->_aSearchArr[i][j] = _pathSprite;//把地图中所有的点一一对应放入检测列表中

			for (auto& rect : GameService::getInstance()->getGameScene()->getCollideRects())
			{
				if (Utils::IsContainsRect(rect, tileRect))
				{
					_pAStarInfo->_aSearchArr[i][j] = nullptr;
					_pathSprite->release();
					break;
				}
			}
		}
	}
	
	//防止黑边
	/*Director::getInstance()->setProjection(Director::Projection::_2D);
	for (auto& pNode : _pBackGround->getChildren())
	{
		((SpriteBatchNode*)pNode)->getTexture()->setAntiAliasTexParameters();
	}*/

    //HeroSkillUi
    ui::Button* pBtn = ui::Button::create("images/skills/skill_1.png","images/skills/skill_1.png");
    pBtn->setTag(0);
    this->addChild(pBtn,Constants::ZORDER_POP);
    pBtn->setPosition(Vec2(1060,400));
    pBtn->addClickEventListener(CC_CALLBACK_1(GameScene::heroSkillCallBack,this));
    
    pBtn = ui::Button::create("images/skills/skill_2.png","images/skills/skill_2.png");
    pBtn->setTag(1);
    this->addChild(pBtn,Constants::ZORDER_POP);
    pBtn->setPosition(Vec2(1060,290));
    pBtn->addClickEventListener(CC_CALLBACK_1(GameScene::heroSkillCallBack,this));
    
    pBtn = ui::Button::create("images/skills/skill_3.png","images/skills/skill_3.png");
    pBtn->setTag(2);
    this->addChild(pBtn,Constants::ZORDER_POP);
    pBtn->setPosition(Vec2(1060,180));
    pBtn->addClickEventListener(CC_CALLBACK_1(GameScene::heroSkillCallBack,this));

	for (int i = 0; i < 3;i++)
	{
		HeroSkill* pHeroSkill = HeroSkill::create(i);
		_vHeroSkills.pushBack(pHeroSkill);
		this->addChild(pHeroSkill, Constants::ZORDER_MONSTER);
		pHeroSkill->setPosition(Vec2(Constants::DESIGN_WIDTH / 2, Constants::DESIGN_HEIGHT / 2));
	}
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
	_pBackGroundUp->setPosition(bgPos);

	Vec2 heroPos = Vec2(x, y);
	heroPos = _pBackGround->convertToWorldSpace(heroPos);

	_pHero->setPosition(heroPos);

	this->addChild(_pHero, Constants::ZORDER_HERO);
	_pHero->stand();

	
}

void GameScene::initMonster()
{ 
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

	_pBoss = Boss::create();
	_pBoss->setPosition(Vec2(900, 300));
	_pBackGround->addChild(_pBoss, Constants::ZORDER_MONSTER);
	_vMonsters.pushBack(_pBoss);
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
		if (pMonster->getMonsterType() != Type_Boss)
		{
			Rect checkBox = Rect(pMonster->getPosition().x - fCheckSide, pMonster->getPosition().y - fCheckSide, 2 * fCheckSide, 2 * fCheckSide);
			if (pMonster->getStatus() == Monster_Stand && checkBox.containsPoint(heroPos))
			{
				pMonster->run();
			}
		}
	}

	while (_vMonsters.size() == 0)
	{
		this->initMonster();
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

void  GameScene::heroSkillCallBack(cocos2d::Ref* pRef)
{
    Node* pNoed = (Node*)pRef;
    int nTag = pNoed->getTag();
    if(nTag == 0){
		_vHeroSkills.at(0)->skillEffect();
    }else if(nTag == 1){
       _vHeroSkills.at(1)->skillEffect();
    }else if(nTag == 2){
	   _vHeroSkills.at(2)->skillEffect();
    }

}

