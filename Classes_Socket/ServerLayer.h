#ifndef __SERVER_LAYER_H__
#define __SERVER_LAYER_H__

#include "GameBase.h"
#include "socket/SocketServer.h"

class ServerLayer : public GameBase
{
public:
    static cocos2d::Scene* createScene();
	virtual bool init();
    CREATE_FUNC(ServerLayer);
	void onEnter();
	void onExit();

	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);

	void update(float dt);

	void run(float angle);
	void Stand();

protected:
	void initNetwork();
	void sendData(DataType type);
	void runAndStand(const Vec2& pos);

	void handleTouchChange(Touch* touch);

private:
	SocketServer* _server;
	Node* _pJoystick;
	DataType _CurDataType = STAND;
};

#endif 
