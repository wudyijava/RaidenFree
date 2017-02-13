#include "FakeLoadingLayer.h"
#include "Common/Macro.h"
#include "GameScene.h"
#include "Sound.h"
#include "Preload.h"
#include "GameData.h"
#include "GlobalData.h"
#include "Level.h"
#include "PlaneLayer.h"

Scene* FakeLoadingLayer::scene()
{
	Scene *scene = Scene::create();
	FakeLoadingLayer *layer = FakeLoadingLayer::create();
	scene->addChild(layer, 1);

	return scene;
}

FakeLoadingLayer::FakeLoadingLayer()
    : m_pTxtInfo(nullptr)
{

}

bool FakeLoadingLayer::init()
{
	Sound::stopAll();

	int level = GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL);
	auto pLevelData = GlobalData::getInstance()->getLevelData(level);
	CCASSERT(pLevelData, "FakeLoadingLayer::init : pLevelData is NULL");
	for (auto it : *pLevelData->enemy)
	{
		if (it->event == PlaneLayerConstant::EventMusic)
		{
			if (it->repeat < 0)
			{
				Sound::playSound(it->object_id, true);
			}
			else
			{
				Sound::playSound(it->object_id, false);
			}

			Sound::setVolume(0.5f);
			break;
		}
	}

	auto pRootNode = Preload::getInstance()->getUI("Loading.csb");
	addChild(pRootNode);

	FIND_UI_CONTROL_RE(ui::Text*, "Error_Info_Text", m_pTxtInfo, pRootNode);
	m_pTxtInfo->setString("");

	auto pAction = CSLoader::createTimeline("Loading.csb");
	pRootNode->runAction(pAction);
	pAction->gotoFrameAndPlay(0, false);
	pAction->setLastFrameCallFunc([this, pRootNode](){
		
		auto delay = Utils::delayToDo(0.5f, [](){
			Sound::setVolume(1.0f);
			Director::getInstance()->replaceScene(TransitionCrossFade::create(.2f, GameScene::create()));
		});

		this->runAction(delay);
	});

	//tips
	const std::vector<std::string>& tips = GlobalData::getInstance()->getTips();
	if (tips.size() > 0)
	{
		int idx = Utils::getRandom() * tips.size();
		if (idx < 0)
		{
			idx = 0;
		}
		if (idx >= (int)tips.size())
		{
			idx = tips.size() - 1;
		}
		m_pTxtInfo->setVisible(true);
		m_pTxtInfo->setString(tips.at(idx));
	}

	return true;
}
