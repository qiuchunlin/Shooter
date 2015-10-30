#ifndef __CLIENT_LAYER_H__
#define __CLIENT_LAYER_H__

#include "GameBase.h"
#include "socket/SocketClient.h"

class ClientLayer : public GameBase
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(ClientLayer);
	void onEnter();
	void onExit();

	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);

	void onRecv(const char* data, int count);
	void onDisconnect();

protected:
	void initNetwork();
	std::vector<Sprite*> _enemies;
	SocketClient* _client;
};

#endif 
