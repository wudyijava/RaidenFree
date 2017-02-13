#include "ModeRapid.h"
#include "Level.h"
#include "Common/Macro.h"
#include "MenuSettle.h"
#include "GameData.h"
#include "BackgroundLayer.h"
#include "Aircraft.h"
#include "Track.h"
#include "Difficulty.h"
#include "AircraftPool.h"
#include "Enemy.h"
#include "HubLayer.h"
#include "Player.h"
//#include "GeneralDialog.h"
#include "MenuMainUI.h"
#include "Analytics.h"
#include "Guide.h"
#include "MenuSettleRapid.h"

namespace ModeRapidConstant
{
	const int DIFICULTY_BASE = 200;

	const float GRADE_INTERVAL = 60.f;
	const int MAX_INTERVAL_GRADE = 2;
	const float MAX_INTERVAL = 1.2f;
	const float PER_INTERVAL = 0.1f;
	const float RANDOM_INTERVAL = 0.2f;

	//�����꣺20-520�����������1��9��ÿ�ζ���+-1�������������ÿ��15s������+1��֪��9Ϊֹ
	const float GRADE_DISTANCE = 30.f;//ÿ30�������Ѷ�
	const float MAX_DISTANCE = 520.f;
	const float MIN_DISTANCE = 20.f;
	const int MIN_NUMBER = 3;
	const int MAX_NUMBER = 1;
	const int PER_NUMBER = -1;
	const int RANDOM_NUMBER = 1;
	const int MAX_DISTANCE_GRADE = 2;//abs(MIN_NUMBER - MAX_NUMBER)

	const float KM_PER_PIXEL = 0.01f;//0.01km/����
}

ModeRapid::ModeRapid()
: m_rapidLevel(0)
, m_tmpSeq(0)
, m_startFlag(false)
, m_fMinMovement(0)
, m_bMinMovement(false)
, m_fMovementRatio(0)
, m_fMovementSum(0)
, m_fTime(0)
, m_fInterval(-1)
{

}

bool ModeRapid::init()
{
	bool ret = PlaneLayer::init();

	m_backgroundLayer->setOnMoveCallback([this](float dt){

		this->updateMove(dt);
	});

	return ret;
}

void ModeRapid::setHubLayer(HubLayer* layer)
{
	PlaneLayer::setHubLayer(layer);

	m_pHubLayer->displayKm(true);
	m_pHubLayer->updateKm(0);
}

void ModeRapid::updateMove(float dt)
{
	PlayerPlane* pPlayer = (PlayerPlane*)this->getPlayer();
	if (pPlayer != NULL && pPlayer->isAlive() && !pPlayer->isNoDie())
	{
		m_fMovementSum += dt;
		m_pHubLayer->updateKm((int)(m_fMovementSum * ModeRapidConstant::KM_PER_PIXEL));
	}


	if (!m_bMinMovement)
	{
		m_fMinMovement = dt;
		m_bMinMovement = true;
	}

	m_fMovementRatio = dt / m_fMinMovement;

	float y = 0;
	for (Aircraft* p : m_staticObjectArray)
	{
		y = p->getPositionY() + dt;
		p->setPositionY(y);
	}

	while (m_staticObjectArray.begin() != m_staticObjectArray.end())
	{
		auto it = m_staticObjectArray.begin();

		if ((*it)->getPositionY() + (*it)->getContentSize().height + 100 < 0)
		{
			//(*it)->recycle();
			AircraftPool::getInstance()->recycle(*it);
			this->destoryPlane((*it));

			m_staticObjectArray.pop_front();
		}
		else
		{
			break;
		}
	}
}

void ModeRapid::procEventSpwanList(float dt)
{
	m_fTime += dt;

	//if (!m_startFlag)
	//{
	//	m_startFlag = true;
	//	m_tmpSeq = m_eventSpwanList.size();
	//	m_rapidLevel++;

	//	GameData::getInstance()->setValue(GAMEDATA::DIFFICULTY_ENDLESS, ModeRapidConstant::DIFICULTY_BASE + m_rapidLevel);
	//}

	while (m_eventSpwanList.size() > 0)
	{
		m_startFlag = true;

		std::list<TLevelEventData>::iterator it = m_eventSpwanList.begin();
		//�ѳ�ʱ
		if (it->time / m_fMovementRatio <= m_time)
		{
			//����Ҫ��ͣ �� ��Ҫ��ͣ����û�л��ŵĵл�
			if ((it->pause != 0 && m_enemyArray.size() <= 0 && m_eventArray.size() <= 0)
				|| it->pause == 0)
			{
				m_time = it->time / m_fMovementRatio;

				//TODO  �л��ȼ�����������Ⱥ��ļ��ʱ�������̡������߸�����仯������л����ϰ�����仯
				spwanOneEvent(it);
				
				//m_tmpSeq--;
				//if (m_tmpSeq <= 0)
				//{
				//	m_startFlag = false;
				//}

				//if (strcmp(it->event.c_str(), PlaneLayerConstant::EventDialog)
				//	&& strcmp(it->event.c_str(), PlaneLayerConstant::EventReadygo)
				//	&& strcmp(it->event.c_str(), PlaneLayerConstant::EventStartShoot)
				//	&& strcmp(it->event.c_str(), PlaneLayerConstant::EventPlayerUp))
				//{
				//	m_eventSpwanList.push_back(*it);
				//}

				m_eventSpwanList.pop_front();
			}
			else if (it->pause != 0 && (m_enemyArray.size() > 0 || m_eventArray.size() > 0))
			{
				break;
			}
			else
			{
				//���ﰴ��˵�ǲ�����ֵ�
				break;
			}
		}
		//�����ǰ�ķɻ��Ѿ����꣬����һ���ɻ���û��ʱ�������ú���ķɻ�����ǰ
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

	if (m_startFlag && m_eventSpwanList.size() > 0)
	{
		return;
	}
	//ʱ��������1.2��0.6��ÿ�ζ���+-0.1�����������ÿ��20s��y��0.1��ֱ��0.6Ϊֹ

	//ʱ�����ȼ�
	int iGrade = m_time / ModeRapidConstant::GRADE_INTERVAL;//0-5
	if (iGrade > ModeRapidConstant::MAX_INTERVAL_GRADE)
	{
		iGrade = ModeRapidConstant::MAX_INTERVAL_GRADE;
	}

	//�ж��Ƿ�ʱ
	if (m_fInterval < 0)
	{
		m_fInterval = ModeRapidConstant::MAX_INTERVAL - iGrade * ModeRapidConstant::PER_INTERVAL
			+ (Utils::getRandom() * ModeRapidConstant::RANDOM_INTERVAL - ModeRapidConstant::RANDOM_INTERVAL / 2);
	}

	if (m_fTime * m_fMovementRatio >= m_fInterval)
	{
		this->spwanEnemy();

		m_fInterval = ModeRapidConstant::MAX_INTERVAL - iGrade * ModeRapidConstant::PER_INTERVAL
			+ (Utils::getRandom() * ModeRapidConstant::RANDOM_INTERVAL - ModeRapidConstant::RANDOM_INTERVAL / 2);

		m_fTime = 0;
	}
}

void ModeRapid::spwanEnemy()
{
	//�����꣺20-520�����������1��9��ÿ�ζ���+-1�������������ÿ��12s������+1��֪��9Ϊֹ
	int iGrade = (int)m_time / ModeRapidConstant::GRADE_DISTANCE;
	if (iGrade > ModeRapidConstant::MAX_DISTANCE_GRADE)
	{
		iGrade = ModeRapidConstant::MAX_DISTANCE_GRADE;
	}

	int cnt = 3//MIN(ModeRapidConstant::MIN_NUMBER, ModeRapidConstant::MAX_NUMBER) //���� 1
		//+ ModeRapidConstant::MAX_DISTANCE_GRADE - iGrade * ModeRapidConstant::RANDOM_NUMBER //�ȼ�  0,1,2
		-1 + 3 * Utils::getRandom(); //���  -1,0,+1
	if (cnt > MAX(ModeRapidConstant::MAX_NUMBER, ModeRapidConstant::MIN_NUMBER))
	{
		cnt = MAX(ModeRapidConstant::MAX_NUMBER, ModeRapidConstant::MIN_NUMBER);
	}

	for (int i = 0; i < cnt; i++)
	{
		float x = ModeRapidConstant::MIN_DISTANCE + //����
			Utils::getRandom() * (ModeRapidConstant::MAX_DISTANCE - ModeRapidConstant::MIN_DISTANCE);//���

		float y = CONSTANT::DESIGN_RES_HEIGHT + 100.f;

		TTrackData track;
		track.startPosX = x;
		track.startPosY = y + 200 * Utils::getRandom() - 100;
		track.loop = 0;

		int aircraftArray[] = { 2011001 //�ܴ�����ᱬ��ʯ����ʯ������1/9
			, 2012001, 2013001, 2014001, 2015001
			, 2012001, 2013001, 2014001, 2015001
		};
		int size = sizeof(aircraftArray) / sizeof(aircraftArray[0]);
		int idx = (int)(size * Utils::getRandom());
		if (idx < 0)
		{
			idx = 0;
		}
		else if (idx > size - 1)
		{
			idx = size - 1;
		}

		int aircraftLevelId = aircraftArray[idx];
		EnemyPlane* pEnemy = nullptr;

		const TAircraftLevelData* pEnemyLevelData = nullptr;
		const TAircraftData* pAircraftData = nullptr;
		pEnemyLevelData = GlobalData::getInstance()->getAircraftLevelData(aircraftLevelId);

		//�Ѷ�����
		int gradeIncrease2 = GlobalData::getInstance()->getDifficulty()->enemy_grade;
		if (gradeIncrease2 > 0)
		{
			int newEnemyGrade = pEnemyLevelData->grade + gradeIncrease2;
			pEnemyLevelData = GlobalData::getInstance()->getAircraftLevelData(pEnemyLevelData->aircraftId, newEnemyGrade);
		}

		pAircraftData = GlobalData::getInstance()->getEnemyData(pEnemyLevelData->aircraftId);
		if (!pAircraftData)
		{
			DEBUG_LOG("Can't find aircraft data[%d]", aircraftLevelId);
			return;
		}
		pEnemy = AircraftPool::getInstance()->get<EnemyPlane>(this, pAircraftData, pEnemyLevelData);
		pEnemy->fly(&track); //����
		pEnemy->attack();    //���

		m_enemyArray.pushBack(pEnemy);

		pEnemy->setOtherSidePlane(&m_playerArray);

		pEnemy->setMoney(pEnemyLevelData->money);
		pEnemy->setScore(pEnemyLevelData->score);
		pEnemy->setDirectionFlag(0);
		//TODO
		//pEnemy->setRewardGroupArray(curEnemy->rewardArray);

		GameData::getInstance()->incValue(GAMEDATA::LEVEL_SCORE_TOTAL, pEnemyLevelData->score);

		m_staticObjectArray.push_back(pEnemy);

	}
}

void ModeRapid::initGameData()
{
	GuideMgr::getInstance()->destory(GuideType::ModeRapid);

	GameData::getInstance()->setValue(GAMEDATA::RAPID_DISTANCE, 0);
	GameData::getInstance()->setValue(GAMEDATA::RAPID_RANK, 0);
	
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
}

void ModeRapid::doAllPlayerKilled()
{
	//�����о���
	int distance = abs((int)(ModeRapidConstant::KM_PER_PIXEL * m_fMovementSum));
	int maxDistance = GameData::getInstance()->getValueToInt(GAMEDATA::RAPID_MODE_MAX_DISTANCE);
	if (distance > maxDistance)
	{
		GameData::getInstance()->setValue(GAMEDATA::RAPID_MODE_MAX_DISTANCE, distance);
	}
	GameData::getInstance()->setValue(GAMEDATA::RAPID_DISTANCE, distance);

	char buf[20 + 1] = { 0 };
	snprintf(buf, 20, "%d", distance);
	Analytics::getInstance()->event(AnalyticsEvent::RapidDistnace, buf);

	//���ڱ��η��о��롢��������ʷ��óɼ����Ƿ����汾��
	auto pLayer = MenuSettleRapid::create();
	pLayer->setCurDistance(distance);
	pLayer->setBestDistance(maxDistance);
	pLayer->getRemoteRank();
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

void ModeRapid::gotoSettleScene(bool win)
{
	Analytics::getInstance()->failLevel(m_pLevelData->name.c_str());

	Director::getInstance()->replaceScene(TransitionCrossFade::create(.2f, MenuMainUI::scene()));
}

void ModeRapid::cancelGiftCallback()
{
	//gotoSettleScene(true);
}

