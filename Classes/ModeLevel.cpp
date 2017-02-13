#include "ModeLevel.h"
#include "Level.h"
#include "MenuSettle.h"
#include "GameLayer.h"
#include "GeneralDialog.h"
#include "GameData.h"
#include "HubLayer.h"
#include "Goods.h"
#include "Relife.h"
#include "BackgroundLayer.h"
#include "Analytics.h"
#include "Sound.h"

namespace ModeLevelConstant
{
	const int SoundGood = 120;
	const int SoundNice = 121;
	const int SoundGreat = 122;
	const int SoundExcellent = 123;
}


bool ModeLevel::init()
{
	bool ret = PlaneLayer::init();

	m_iSoundFlag = 0;
	m_iOldKillCnt = 0;

	return ret;
}

void ModeLevel::setHubLayer(HubLayer* layer)
{
	PlaneLayer::setHubLayer(layer);

	m_pHubLayer->displayCombo(false);
}

void ModeLevel::procEventSpwanList(float dt)
{
	while (m_eventSpwanList.size() > 0)
	{
		std::list<TLevelEventData>::iterator it = m_eventSpwanList.begin();
		//已超时
		if (it->time <= m_time)
		{
			//不需要暂停 或 需要暂停并且没有活着的敌机
			if ((it->pause != 0 && m_enemyArray.size() <= 0 && m_eventArray.size() <= 0)
				|| it->pause == 0)
			{
				m_time = it->time;

				spwanOneEvent(it);

				m_eventSpwanList.pop_front();
			}
			else if (it->pause != 0 && (m_enemyArray.size() > 0 || m_eventArray.size() > 0))
			{
				break;
			}
			else
			{
				//这里按理说是不会出现的
				break;
			}
		}
		//如果当前的飞机已经打完，但下一波飞机还没超时，则需让后面的飞机都提前
		else if (it->time > m_time + 1 && m_enemyArray.size() <= 0 && m_eventArray.size() <= 0 /*&& !m_bFirstEnemy*/)
		{
			m_time += it->time - m_time - 1;
			break;
		}
		else
		{
			break;
		}
	}
}

void ModeLevel::initGameData()
{
	int life = GameData::getInstance()->getValueToInt(GAMEDATA::LIFE);
	GameData::getInstance()->setValue(GAMEDATA::LEVEL_LIFE, life);
	GameData::getInstance()->setValue(GAMEDATA::LEVEL_ENEMY, 0);
	GameData::getInstance()->setValue(GAMEDATA::LEVEL_SCORE, 0);
	GameData::getInstance()->setValue(GAMEDATA::LEVEL_MONEY, 0);
	GameData::getInstance()->setValue(GAMEDATA::LEVEL_REWARD, 0);
	GameData::getInstance()->setValue(GAMEDATA::LEVEL_KILLER, 0);

	GameData::getInstance()->setValue(GAMEDATA::LEVEL_SCORE_TOTAL, 0);
	GameData::getInstance()->setValue(GAMEDATA::LEVEL_MONEY_TOTAL, 0);
	GameData::getInstance()->setValue(GAMEDATA::LEVEL_REWARD_TOTAL, 0);

	GameData::getInstance()->setValue(GAMEDATA::CONTINUOUS_KILL_ENEMY_CNT, 0);
}

void ModeLevel::doAllPlayerKilled()
{
	//int enemyTotal = GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL_ENEMY_TOTAL);
	//int enemyLeft = GameData::getInstance()->getValueToInt(GAMEDATA::LEVEL_ENEMY_LEFT);
	//int ratio = 0;
	//if (enemyTotal > 0)
	//{
	//	ratio = (int)enemyLeft * 100.0f / enemyTotal; //还差xx%即可通过本关
	//}

	////auto layer = GiftLayer::create(GoodsType::LIFE_PKG);
	//std::vector<const char*> goodsArray;
	//goodsArray.push_back(GoodsType::LIFE_1);
	//goodsArray.push_back(GoodsType::LIFE_PKG);
	//auto pLayer = GiftDuoLayer::create(goodsArray);
	auto pLayer = RelifeLayer::create();

	pLayer->setPlaneLayer(this);
	pLayer->setCancelCallback([this](){
		m_backgroundLayer->constant(false);
		cancelGiftCallback();
	});
	pLayer->setOnEnterCallback([this](){
		++m_bPause;
	});
	pLayer->setOnExitCallback([this](){
		if (m_bPause > 0)
		{
			--m_bPause;
		}
	});
	this->getParent()->addChild(pLayer);
}

void ModeLevel::gotoSettleScene(bool win)
{
	Analytics::getInstance()->failLevel(m_pLevelData->name.c_str());

	MenuSettle::setBattleResult(win);
	Director::getInstance()->replaceScene(TransitionCrossFade::create(.2f, MenuSettle::scene()));
}

void ModeLevel::cancelGiftCallback()
{
	auto pLayer = GeneralDialogCreator::restartGame();
	pLayer->setPlaneLayer(this);
	pLayer->setOnEnterCallback([this](){
		++m_bPause;
	});
	pLayer->setOnExitCallback([this](){
		if (m_bPause > 0)
		{
			--m_bPause;
		}
	});
	getParent()->addChild(pLayer);

}

void ModeLevel::update(float dt)
{
	PlaneLayer::update(dt);

	static int data[][4] = { 
		//杀敌数、宝石数、分数、音效
		{ 10, 10, 100, ModeLevelConstant::SoundGood },  
		{ 30, 40, 400, ModeLevelConstant::SoundNice },
		{ 60, 80, 800, ModeLevelConstant::SoundGreat },
		{ 100, 130, 1300, ModeLevelConstant::SoundExcellent }
	};

	int cnt = GameData::getInstance()->getValueToInt(GAMEDATA::CONTINUOUS_KILL_ENEMY_CNT);
	if (cnt >= 5 && cnt != m_iOldKillCnt)
	{
		m_pHubLayer->displayCombo(true);
		m_pHubLayer->updateCombo(cnt);

		for (int i = 0; i < 3; i++)
		{
			if (cnt == data[i][0] && m_iSoundFlag == i)
			{
				++m_iSoundFlag;
				GameData::getInstance()->incValue(GAMEDATA::LEVEL_MONEY, data[i][1]);
				GameData::getInstance()->incValue(GAMEDATA::LEVEL_SCORE, data[i][2]);

				Sound::playSound(data[i][3]);
				m_pHubLayer->updateMoneyPresent(data[i][1]);
				break;
			}
		}
	}
	else if (cnt == 0 && cnt != m_iOldKillCnt)
	{
		m_iSoundFlag = 0;
		m_pHubLayer->displayCombo(false);
	}

	m_iOldKillCnt = cnt;
}