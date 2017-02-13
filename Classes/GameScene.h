#pragma once

#include "cocos2d.h"
#include "GameLayer.h"

USING_NS_CC;

class GameScene :
	public Scene
{
public:
	
	GameScene(void);

	virtual ~GameScene(void);

	CREATE_FUNC(GameScene);

	virtual bool init();

	virtual void onEnter() override;
	virtual void onExit() override;

	static bool isSceneRunning();

private:
	GameLayer* m_gameLayer;

	static int m_bIsRunning;
};
