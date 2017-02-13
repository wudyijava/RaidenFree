#pragma once

#include "cocos2d.h"

USING_NS_CC;

class StartLayer : public Layer
{
public:
	static Scene* scene();
	CREATE_FUNC(StartLayer);

	virtual bool init();

	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent) override;
	virtual	void onTouchEnded(Touch *pTouch, Event *pEvent) override;

};
