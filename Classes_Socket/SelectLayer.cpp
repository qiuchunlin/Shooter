#include "SelectLayer.h"
#include "ServerLayer.h"
#include "ClientLayer.h"

Scene* SelectLayer::createScene()
{
	auto scene = Scene::createWithPhysics();	
	auto layer = SelectLayer::create();

	scene->addChild(layer);
	return scene;
}

bool SelectLayer::init()
{
	if ( !Layer::init() )
	{
		return false;
	}

	this->initMenu();

	return true;
}

void SelectLayer::initMenu()
{
	auto winSize = Director::getInstance()->getVisibleSize();

	auto lblServer = Label::createWithSystemFont("Server", "Arial", 24.f);
	auto menuServer = MenuItemLabel::create(lblServer, CC_CALLBACK_1(SelectLayer::menuServerClick, this));
	menuServer->setPosition(Vec2(winSize.width * 0.5f, winSize.height * 0.6f));

	auto lblClient = Label::createWithSystemFont("Client", "Arial", 24.f);
	auto menuClient = MenuItemLabel::create(lblClient, CC_CALLBACK_1(SelectLayer::menuClientClick, this));
	menuClient->setPosition(Vec2(winSize.width * 0.5f, winSize.height * 0.5f));

	auto menu = Menu::create(menuServer, menuClient, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

}

void SelectLayer::menuClientClick(Ref* sender)
{
	auto scene = ClientLayer::createScene();
	Director::getInstance()->replaceScene(scene);
}

void SelectLayer::menuServerClick(Ref* sender)
{
	auto scene = ServerLayer::createScene();
	Director::getInstance()->replaceScene(scene);
}