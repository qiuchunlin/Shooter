#ifndef __SELECT_LAYER_H__
#define __SELECT_LAYER_H__

#include "cocos2d.h"
USING_NS_CC;

class SelectLayer : public Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(SelectLayer);
	void menuServerClick(Ref* sender); 
	void menuClientClick(Ref* sender);

private:
	void initMenu();

};

#endif 
