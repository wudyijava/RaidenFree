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

	//�����˳�
	auto dispatcher = Director::getInstance()->getEventDispatcher();

	//����һ���ŶӵĴ����¼������� ( ͬʱ��������һ�������¼� )
	auto listener = EventListenerTouchOneByOne::create();

	// �� "swallow touches" ����Ϊ true, Ȼ���� onTouchBegan ���������� 'true' ����Ե������¼�, ��ֹ����������ʹ������¼�.
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
	if (GlobalData::getInstance()->isIgnoreMenu())//����ʱ��ѡ���Ƿ������˵�ֱ�ӽ�����Ϸ
	{
		DEBUG_LOG("Go to scene GameScene");
		Director::getInstance()->replaceScene(TransitionCrossFade::create(.2f, GameScene::create()));
	}
	else
	{
		//��ʱ�ر�ÿ��ǩ������
		if (MenuDayGift::isDisplayDailyGift()
			&& Utils::str2int(GlobalData::getInstance()->getParameter(GLOBALDATA::DAILY_GIFT)))
		//if (false)
		{
			DEBUG_LOG("Go to scene MenuDayGift");
			//ÿ�յ�¼����
			Director::getInstance()->replaceScene(TransitionCrossFade::create(.2f, MenuDayGift::scene()));
		}
		else
		{
			DEBUG_LOG("Go to scene MenuPlaneLayer");
			//���˵�
			Director::getInstance()->replaceScene(TransitionCrossFade::create(.2f, MenuMainUI::scene()));
		}

	}
}
