#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

class PlaneLayer;
class HubLayer;
class DebugLayer;

class GameLayer : public Layer
{
public:
	friend class PlaneLayer;
	friend class AppDelegate;

	GameLayer(void);

	virtual ~GameLayer(void);

	CREATE_FUNC(GameLayer);

	virtual bool init();

private:
	static void setPlaneLayer(Layer* layer);
	static void setGameLayer(Layer* layer);
	static void setHubLayer(Layer* layer);

	static Layer* getPlaneLayer();
	static Layer* getGameLayer();
	static Layer* getHubLayer();

private:
	static Layer*  m_gameLayer;
	static Layer*  m_planeLayer;
	static Layer*  m_hubLayer;
};
