#include "ClientLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocostudio::timeline;

Scene* ClientLayer::createScene()
{
	auto scene = Scene::create();
	auto layer = ClientLayer::create();

	scene->addChild(layer);

	return scene;
}

bool ClientLayer::init()
{
	if ( !GameBase::init() )
	{
		return false;
	}

	log("client");
	return true;
}

void ClientLayer::initNetwork()
{
	_client = SocketClient::construct();
	_client->onRecv = CC_CALLBACK_2(ClientLayer::onRecv, this);
	_client->onDisconnect = CC_CALLBACK_0(ClientLayer::onDisconnect, this);
	_lblInfo->setString("This is Client");
	if (!_client->connectServer("127.0.0.1", 8000))
	{
		_lblInfo->setString("Client connect error");
	}
}

void ClientLayer::onEnter()
{
	GameBase::onEnter();
}

void ClientLayer::onExit()
{
	_client->destroy();
	_client = nullptr;
	GameBase::onExit();
}

bool ClientLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	
	return true;
}

void ClientLayer::onTouchMoved(Touch *touch, Event *unused_event)
{

}

void ClientLayer::onTouchEnded(Touch *touch, Event *unused_event)
{

}

void ClientLayer::onRecv(const char* data, int count)
{
	GameData* gameData = (GameData*)data;
	if (gameData->dataSize == sizeof(GameData))
	{
		switch (gameData->dataType)
		{
		case RUN:
			_enemy->stopAllActions();
			_enemy->runAction(RepeatForever::create(_runAnim));
			break;
		case STAND:
			_enemy->stopAllActions();
			_enemy->runAction(RepeatForever::create(_standAnim));
			break;
		case POSITION:
			if (gameData->position.x < _enemy->getPositionX())
				_enemy->setFlippedX(true);
			else if (fabs(gameData->position.x  - _enemy->getPositionX()) > MATH_EPSILON)
				_enemy->setFlippedX(false);
			_enemy->setPosition(gameData->position);
			break;

		default:
			break;
		}
	}
	
}

void ClientLayer::onDisconnect()
{
	_lblInfo->setString("Client disconnect");
}