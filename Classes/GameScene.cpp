#include "GameScene.h"

int GameScene::m_bIsRunning = 0;

GameScene::GameScene(void)
{
	m_gameLayer = NULL;
}

GameScene::~GameScene(void)
{
}

void GameScene::onEnter()
{
	Node::onEnter();

	++m_bIsRunning;
}

void GameScene::onExit()
{
	Node::onExit();

	--m_bIsRunning;
}

bool GameScene::init()
{
	bool bRet=false;
	do 
	{
		CC_BREAK_IF(!Scene::init());

		//暂不启用物理引擎
		//initWithPhysics();
		//getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);//debug模式
		//Vect gravity = Vect(0.0f, 0.0f);//0重力
		//getPhysicsWorld()->setGravity(gravity);

		m_gameLayer = GameLayer::create();
		CC_BREAK_IF(!m_gameLayer);
		this->addChild(m_gameLayer);

		bRet=true;
	} while (0);

	return bRet;
}

bool GameScene::isSceneRunning()
{
	return m_bIsRunning > 0;
}