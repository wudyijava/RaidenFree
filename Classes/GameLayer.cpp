#include "GlobalData.h"
#include "GameData.h"
#include "GameLayer.h"
#include "HubLayer.h"
#include "DebugLayer.h"
#include "Enemy.h"
#include "Player.h"
#include "Bullet.h"
#include "Level.h"
#include "Sound.h"
#include "ModeBase.h"
#include "PlaneLayer.h"
#include "Bmob/BmobUtils.h"
#include "Common/Macro.h"

Layer* GameLayer::m_gameLayer = nullptr;
Layer* GameLayer::m_planeLayer = nullptr;
Layer* GameLayer::m_hubLayer = nullptr;

GameLayer::GameLayer(void)
{
	m_planeLayer = nullptr;
	m_hubLayer = nullptr;
}

GameLayer::~GameLayer(void)
{
	m_planeLayer = nullptr;
	m_hubLayer = nullptr;
}

bool GameLayer::init()
{
	bool bRet=false;
	do 
	{
		CC_BREAK_IF(!Layer::init());

		Sound::turnOnBulletSound();

		//加入飞机层
		int mode = GameData::getInstance()->getValueToInt(GAMEDATA::MODE);
		auto planeLayer = ModeBase::createLayer(mode);
		this->addChild(planeLayer);

		//加入Hub层
		auto hubLayer = HubLayer::create(planeLayer);
		this->addChild(hubLayer);

		planeLayer->regHpListener(hubLayer);
		planeLayer->regBossHpListener(hubLayer);

		GameLayer::setGameLayer(this);
		GameLayer::setPlaneLayer(planeLayer);
		GameLayer::setHubLayer(hubLayer);

		bRet=true;
	} while (0);
	return bRet;
}

Layer* GameLayer::getGameLayer()
{
	if (!m_gameLayer)
	{
		DEBUG_LOG("FUCK");
	}
	return m_gameLayer;
}

Layer* GameLayer::getPlaneLayer()
{
	if (!m_planeLayer)
	{
		DEBUG_LOG("FUCK");
	}
	return m_planeLayer;
}

Layer*   GameLayer::getHubLayer()
{
	if (!m_hubLayer)
	{
		DEBUG_LOG("FUCK");
	}
	return m_hubLayer;
}

void GameLayer::setPlaneLayer(Layer* layer)
{
	m_planeLayer = layer;
}

void GameLayer::setHubLayer(Layer* layer)
{
	m_hubLayer = layer;
}

void GameLayer::setGameLayer(Layer* layer)
{
	m_gameLayer = layer;
}