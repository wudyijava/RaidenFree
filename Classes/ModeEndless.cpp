#include "ModeEndless.h"
#include "Level.h"
#include "Common/Macro.h"
#include "MenuSettle.h"
#include "GameData.h"
#include "GlobalData.h"
#include "Aircraft.h"
#include "AircraftPool.h"
#include "GiftLayer.h"
#include "BackgroundLayer.h"
#include "Analytics.h"
#include "Guide.h"

namespace ModeEndlessConstant
{
	const int DIFICULTY_BASE = 100;
	const char* const ATTACK_BASE = "attack_base";
}

ModeEndless::ModeEndless()
: m_endlessLevel(0)
, m_tmpSeq(0)
, m_startFlag(false)
, m_fEnemyHpAdd(0)
{
	m_enemyGroupArray.push_back(EnemyGroup(11011001, 12011001, 13011001, 1121001));
	m_enemyGroupArray.push_back(EnemyGroup(11021001, 12021001, 13021001, 1131001));
	m_enemyGroupArray.push_back(EnemyGroup(11031001, 12031001, 13031001, 1141001));
	m_enemyGroupArray.push_back(EnemyGroup(11041001, 12041001, 13041001, 1151001));
	m_enemyGroupArray.push_back(EnemyGroup(11051001, 12051001, 13051001, 1161001));
	m_enemyGroupArray.push_back(EnemyGroup(11061001, 12061001, 13061001, 1111001));

	//根据战机综合战斗力给敌机加hp
	int attackBase = GlobalData::getInstance()->getParameterToInt(ModeEndlessConstant::ATTACK_BASE);
	auto player = GlobalData::getInstance()->getPlayerData(GameData::getInstance()->getValueToInt(GAMEDATA::PLAYER));
	int attack = player->getMaxAttack();
	m_fEnemyHpAdd= attack - attackBase;
}

void ModeEndless::procEventSpwanList(float dt)
{
	if (!m_startFlag)
	{
		m_startFlag = true;
		m_tmpSeq = m_eventSpwanList.size();
		m_endlessLevel++;
		GameData::getInstance()->setValue(GAMEDATA::DIFFICULTY_ENDLESS, ModeEndlessConstant::DIFICULTY_BASE + m_endlessLevel);
		DEBUG_LOG("Endless mode level: %d", m_endlessLevel);
	}

	int idx = (m_endlessLevel - 1) % m_enemyGroupArray.size();

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

				//敌机等级需升级、机群间的间隔时间需缩短、爆道具概率需变化、随机敌机和障碍物需变化
				int objectId = it->object_id;
				if (!strcmp(it->event.c_str(), PlaneLayerConstant::EventBossDown)
					|| !strcmp(it->event.c_str(), PlaneLayerConstant::EventBossFlyShoot)
					|| !strcmp(it->event.c_str(), PlaneLayerConstant::EventEnemy)
					|| !strcmp(it->event.c_str(), PlaneLayerConstant::EventEnemyLevel))
				{
					switch (it->object_id)
					{
					case 1:
						GameData::getInstance()->setValue(GAMEDATA::ENDLESS_ENEMY_HP_RATIO, m_fEnemyHpAdd * 0.3f);
						it->object_id = m_enemyGroupArray.at(idx).enemy1;
						break;
					case 2:
						GameData::getInstance()->setValue(GAMEDATA::ENDLESS_ENEMY_HP_RATIO, m_fEnemyHpAdd * 0.3f);
						it->object_id = m_enemyGroupArray.at(idx).enemy2;
						break;
					case 3:
						GameData::getInstance()->setValue(GAMEDATA::ENDLESS_ENEMY_HP_RATIO, m_fEnemyHpAdd * 0.3f);
						it->object_id = m_enemyGroupArray.at(idx).enemy3;
						break;
					case 4:
						GameData::getInstance()->setValue(GAMEDATA::ENDLESS_ENEMY_HP_RATIO, m_fEnemyHpAdd * 5.0f + m_endlessLevel * 5000);
						it->object_id = m_enemyGroupArray.at(idx).boss;
						break;
					default:
						break;
					}
				}

				//在出现第一架飞机的时候先release一下
				if (m_tmpSeq == m_eventSpwanList.size())
				{
					AircraftPool::getInstance()->release();
				}

				spwanOneEvent(it);

				it->object_id = objectId;
				
				m_tmpSeq--;
				if (m_tmpSeq <= 0)
				{
					m_startFlag = false;
				}

				if (strcmp(it->event.c_str(), PlaneLayerConstant::EventDialog)
					&& strcmp(it->event.c_str(), PlaneLayerConstant::EventReadygo)
					&& strcmp(it->event.c_str(), PlaneLayerConstant::EventPlayerUp))
				{
					m_eventSpwanList.push_back(*it);
				}

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

void ModeEndless::initGameData()
{
	GuideMgr::getInstance()->destory(GuideType::ModeEndless);

	//int life = GameData::getInstance()->getValueToInt(GAMEDATA::LIFE);
	GameData::getInstance()->setValue(GAMEDATA::LEVEL_LIFE, 1);
	GameData::getInstance()->setValue(GAMEDATA::LEVEL_ENEMY, 0);
	GameData::getInstance()->setValue(GAMEDATA::LEVEL_SCORE, 0);
	GameData::getInstance()->setValue(GAMEDATA::LEVEL_MONEY, 0);
	GameData::getInstance()->setValue(GAMEDATA::LEVEL_REWARD, 0);
	GameData::getInstance()->setValue(GAMEDATA::LEVEL_KILLER, 0);

	GameData::getInstance()->setValue(GAMEDATA::LEVEL_SCORE_TOTAL, 0);
	GameData::getInstance()->setValue(GAMEDATA::LEVEL_MONEY_TOTAL, 0);
	GameData::getInstance()->setValue(GAMEDATA::LEVEL_REWARD_TOTAL, 0);

	GameData::getInstance()->setValue(GAMEDATA::ENDLESS_BOSS, 0);
}

void ModeEndless::doAllPlayerKilled()
{
	auto pLayer = GiftLayer::create(GoodsType::LIFE_1);
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

void ModeEndless::gotoSettleScene(bool win)
{
	Analytics::getInstance()->failLevel(m_pLevelData->name.c_str());

	int enemy = GameData::getInstance()->getValueToInt(GAMEDATA::ENEMY);

	MenuSettle::setBattleResult(true);
	Director::getInstance()->replaceScene(TransitionCrossFade::create(.2f, MenuSettle::scene()));
}

void ModeEndless::cancelGiftCallback()
{
	gotoSettleScene(true);
}
