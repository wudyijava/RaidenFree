#include "StartLayer.h"
#include "cocostudio/CocoStudio.h"
#include "MenuMainUI.h"
#include "GlobalData.h"
#include "Common/Macro.h"
#include "MenuDayGift.h"
#include "GameScene.h"
#include "Sound.h"
#include "Preload.h"

Scene* StartLayer::scene()
{
	Scene *scene = Scene::create();
	StartLayer *layer = StartLayer::create();
	scene->addChild(layer, 1);

	return scene;
}

bool StartLayer::init()
{
	auto pRootNode = Preload::getInstance()->getUI("Start.csb");
	addChild(pRootNode);

	auto pAction = CSLoader::createTimeline("Start.csb");
	pRootNode->runAction(pAction);
	pAction->gotoFrameAndPlay(0, 50, false);
	pAction->setLastFrameCallFunc([pRootNode](){
		auto pAction = CSLoader::createTimeline("Start.csb");
		pRootNode->runAction(pAction);
		pAction->gotoFrameAndPlay(51, 150, true);

	});

	//触摸退出
	auto dispatcher = Director::getInstance()->getEventDispatcher();

	//创建一个排队的触控事件监听器 ( 同时仅仅处理一个触控事件 )
	auto listener = EventListenerTouchOneByOne::create();

	// 当 "swallow touches" 设置为 true, 然后，在 onTouchBegan 方法发返回 'true' 将会吃掉触控事件, 防止其他监听器使用这个事件.
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(StartLayer::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(StartLayer::onTouchEnded, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	Sound::playSound(SOUND::ID_MENU_GB, true);

	return true;
}

bool StartLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	return true;
}

void StartLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	if (GlobalData::getInstance()->isIgnoreMenu())//测试时可选择是否跳过菜单直接进入游戏
	{
		DEBUG_LOG("Go to scene GameScene");
		Director::getInstance()->replaceScene(TransitionCrossFade::create(.2f, GameScene::create()));
	}
	else
	{
		//暂时关闭每日签到奖励
		if (MenuDayGift::isDisplayDailyGift()
			&& Utils::str2int(GlobalData::getInstance()->getParameter(GLOBALDATA::DAILY_GIFT)))
		//if (false)
		{
			DEBUG_LOG("Go to scene MenuDayGift");
			//每日登录奖励
			Director::getInstance()->replaceScene(TransitionCrossFade::create(.2f, MenuDayGift::scene()));
		}
		else
		{
			DEBUG_LOG("Go to scene MenuPlaneLayer");
			//主菜单
			Director::getInstance()->replaceScene(TransitionCrossFade::create(.2f, MenuMainUI::scene()));
		}

	}
}
