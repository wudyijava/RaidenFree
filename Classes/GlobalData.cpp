#include "GlobalData.h"
#include "GameData.h"
#include "Aircraft.h"
#include "Boss.h"
#include "Track.h"
#include "Reward.h"
#include "Level.h"
#include "Bullet.h"
#include "Armature.h"
#include "Sound.h"
#include "Achievement.h"
#include "Reinforce.h"
#include "Goods.h"
#include "Com.h"
#include "UniBilling.h"
#include "Carrier.h"
#include "Dialog.h"
#include "DayGift.h"
#include "Difficulty.h"
#include "Equipment.h"
#include "ModeBase.h"
#include "Preload.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
#include <jni.h>  
#include "platform/android/jni/JniHelper.h"  
#include <android/log.h>  
#endif

GlobalData* GlobalData::m_pInstance = NULL;

GlobalData::GlobalData()
{
}

GlobalData::~GlobalData()
{
	if (m_pInstance != NULL)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

void GlobalData::destory()
{
	for (unsigned i = 0; i < m_ArmatureArray.size(); i++)
	{
		delete m_ArmatureArray.at(i);
	}
	m_ArmatureArray.clear();

	for (unsigned i = 0; i < m_PlayerArray.size(); i++)
	{
		delete m_PlayerArray.at(i);
	}
	m_PlayerArray.clear();

	for (unsigned i = 0; i < m_EnemyArray.size(); i++)
	{
		delete m_EnemyArray.at(i);
	}
	m_EnemyArray.clear();

	for (unsigned i = 0; i < m_BossArray.size(); i++)
	{
		delete m_BossArray.at(i);
	}
	m_BossArray.clear();

	for (unsigned i = 0; i < m_WingmanArray.size(); i++)
	{
		delete m_WingmanArray.at(i);
	}
	m_WingmanArray.clear();

	for (unsigned i = 0; i < m_BulletArray.size(); i++)
	{
		delete m_BulletArray.at(i);
	}
	m_BulletArray.clear();

	for (unsigned i = 0; i < m_TrackArray.size(); i++)
	{
		delete m_TrackArray.at(i);
	}
	m_TrackArray.clear();

	for (unsigned i = 0; i < m_RewardArray.size(); i++)
	{
		delete m_RewardArray.at(i);
	}
	m_RewardArray.clear();

	for (unsigned i = 0; i < m_LevelArray.size(); i++)
	{
		delete m_LevelArray.at(i);
	}
	m_LevelArray.clear();

	for (unsigned i = 0; i < m_MusicArray.size(); i++)
	{
		delete m_MusicArray.at(i);
	}
	m_MusicArray.clear();

	for (unsigned i = 0; i < m_AchievementMap.size(); i++)
	{
		delete m_AchievementMap.at(i);
	}
	m_AchievementMap.clear();

	for (unsigned i = 0; i < m_AnimationMap.size(); i++)
	{
		delete m_AnimationMap.at(i);
	}
	m_AnimationMap.clear();

	for (unsigned i = 0; i < m_HpBarMap.size(); i++)
	{
		delete m_HpBarMap.at(i);
	}
	m_HpBarMap.clear();

	for (unsigned i = 0; i < m_ReinforceArray.size(); i++)
	{
		delete m_ReinforceArray.at(i);
	}
	m_ReinforceArray.clear();

	{
		auto it = m_GoodsMap.begin();
		for (; it != m_GoodsMap.end(); it++)
		{
			delete it->second;
		}
		m_GoodsMap.clear();
	}
	{
		auto it = m_dialogMap.begin();
		for (; it != m_dialogMap.end(); it++)
		{
			delete it->second;
		}
		m_dialogMap.clear();
	}
	{
		auto it = m_difficultyMap.begin();
		for (; it != m_difficultyMap.end(); it++)
		{
			delete it->second;
		}
		m_difficultyMap.clear();
	}
	{
		auto it = m_equipmentMap.begin();
		for (; it != m_equipmentMap.end(); it++)
		{
			for (unsigned i = 0; i < it->second->size(); i++)
			{
				delete it->second->at(i);
			}
			it->second->clear();
			delete it->second;
		}
		m_equipmentMap.clear();
	}

}

bool GlobalData::initialize(GlobalDataLoadListener* pLisiener)
{
	float total = 25.0f / 100;
	int step = 0;

	if (!DbHelper::getInstance()->open())
	{
		CCLOG("Open database failed");
		return false;
	}
	pLisiener->onGlobalDataProgress(++step / total);

	bool success = false;
	do
	{
		pLisiener->onGlobalDataProgress(++step / total); DEBUG_LOG("GlobalData::initGlobal");		if (!initGlobal()) break;
		pLisiener->onGlobalDataProgress(++step / total); DEBUG_LOG("GlobalData::initMusic");		if (!initMusic()) break;
		pLisiener->onGlobalDataProgress(++step / total); DEBUG_LOG("GlobalData::initAnimation");	if (!initAnimation()) break;
		pLisiener->onGlobalDataProgress(++step / total); DEBUG_LOG("GlobalData::initArmature");		if (!initArmature()) break;
		pLisiener->onGlobalDataProgress(++step / total); DEBUG_LOG("GlobalData::initBulletGrade");	if (!initBulletGrade()) break;
		pLisiener->onGlobalDataProgress(++step / total); DEBUG_LOG("GlobalData::initBullet");		if (!initBullet()) break;
		pLisiener->onGlobalDataProgress(++step / total); DEBUG_LOG("GlobalData::initHpBar");		if (!initHpBar()) break;
		pLisiener->onGlobalDataProgress(++step / total); DEBUG_LOG("GlobalData::initWingman");		if (!initWingman()) break;  //必须要在初始化飞机的前面
		pLisiener->onGlobalDataProgress(++step / total); DEBUG_LOG("GlobalData::initPlayer");		if (!initPlayer()) break;
		pLisiener->onGlobalDataProgress(++step / total); DEBUG_LOG("GlobalData::initEnemy");		if (!initEnemy()) break;
		pLisiener->onGlobalDataProgress(++step / total); DEBUG_LOG("GlobalData::initAircraftLevel");if (!initAircraftLevel()) break;
		pLisiener->onGlobalDataProgress(++step / total); DEBUG_LOG("GlobalData::initBoss");			if (!initBoss()) break;
		pLisiener->onGlobalDataProgress(++step / total); DEBUG_LOG("GlobalData::initReward");		if (!initReward()) break;
		pLisiener->onGlobalDataProgress(++step / total); DEBUG_LOG("GlobalData::initTrack");		if (!initTrack()) break;
		pLisiener->onGlobalDataProgress(++step / total); DEBUG_LOG("GlobalData::initLevel");		if (!initLevel()) break;
		pLisiener->onGlobalDataProgress(++step / total); DEBUG_LOG("GlobalData::initAchievement");	if (!initAchievement()) break;
		pLisiener->onGlobalDataProgress(++step / total); DEBUG_LOG("GlobalData::initReinforce");	if (!initReinforce()) break;
		pLisiener->onGlobalDataProgress(++step / total); DEBUG_LOG("GlobalData::initGoods");		if (!initGoods()) break;
		pLisiener->onGlobalDataProgress(++step / total); DEBUG_LOG("GlobalData::initDialog");		if (!initDialog()) break;
		pLisiener->onGlobalDataProgress(++step / total); DEBUG_LOG("GlobalData::initDayGift");		if (!initDayGift()) break;
		pLisiener->onGlobalDataProgress(++step / total); DEBUG_LOG("GlobalData::initDifficulty");	if (!initDifficulty()) break;
		pLisiener->onGlobalDataProgress(++step / total); DEBUG_LOG("GlobalData::initEquipment");	if (!initEquipment()) break;
		
		success = true;
	} while (0);

	DbHelper::getInstance()->close();
	CCASSERT(success, "GlobalData::initialize error!!!");
	pLisiener->onGlobalDataProgress(++step / total);

	return success;
}

int GlobalData::getVersionCode()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台  
	JniMethodInfo minfo;//定义Jni函数信息结构体  
	//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数  
	bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/cpp/AppActivity", "getVersionCode", "()I");

	if (!isHave) {
		DEBUG_LOG("jni:getVersionCode not exists");
		return -1;
	}
	else{
		//调用此函数  
		jint _int = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID);
		return _int;
	}
#else
	return -1;
#endif
}

std::string GlobalData::getVersionName()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台  
	JniMethodInfo minfo;//定义Jni函数信息结构体  
	//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数  
	bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/cpp/AppActivity", "getVersionName", "()Ljava/lang/String;");

	if (!isHave) {
		DEBUG_LOG("jni:getVersionName not exists");
		return "0.0.0";
	}
	else{
		//调用此函数  
		jstring jstr = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
		std::string ret = JniHelper::jstring2string(jstr);
		return ret;
	}
#else
	return "0.0.0";
#endif
}

const TMusicData* GlobalData::getMusic(int id)
{
	for (unsigned i = 0; i < m_MusicArray.size(); i++)
	{
		if (m_MusicArray.at(i)->id == id)
		{
			return m_MusicArray.at(i);
		}
	}

	return NULL;
}

const TAircraftData* GlobalData::getPlayerData(int id)
{
	for (unsigned i = 0; i < m_PlayerArray.size(); i++)
	{
		if (m_PlayerArray.at(i)->id == id)
		{
			return m_PlayerArray.at(i);
		}
	}

	return NULL;
}

const TAircraftData* GlobalData::getEnemyData(int id)
{
	for (unsigned i = 0; i < m_EnemyArray.size(); i++)
	{
		if (m_EnemyArray.at(i)->id == id)
		{
			return m_EnemyArray.at(i);
		}
	}

	return NULL;
}

const TBossData* GlobalData::getBossData(int id)
{
	for (unsigned i = 0; i < m_BossArray.size(); i++)
	{
		if (m_BossArray.at(i)->aircraftId == id)
		{
			return m_BossArray.at(i);
		}
	}

	return NULL;
}

const TAircraftData* GlobalData::getWingmanData(int id)
{
	for (unsigned i = 0; i < m_WingmanArray.size(); i++)
	{
		if (m_WingmanArray.at(i)->id == id)
		{
			return m_WingmanArray.at(i);
		}
	}

	return NULL;
}

const TAircraftLevelData* GlobalData::getAircraftLevelData(int aircraftId, int grade)
{
	auto it = m_aircraftLevelIdGradeMap.find(aircraftId);
	if (it != m_aircraftLevelIdGradeMap.end())
	{
		auto it2 = it->second->find(grade);
		if (it2 != it->second->end())
		{
			return it2->second;
		}
	}

	return nullptr;
}

const TBulletData* GlobalData::getBulletData(int id)
{
	for (unsigned i = 0; i < m_BulletArray.size(); i++)
	{
		if (m_BulletArray.at(i)->id == id)
		{
			return m_BulletArray.at(i);
		}
	}

	return NULL;
}

const std::vector<std::vector<int>*>* GlobalData::getBulletGradeData(int id)
{
	auto it = m_BulletGradeMap.find(id);
	if (it == m_BulletGradeMap.end())
	{
		return NULL;
	}
	else
	{
		return it->second;
	}
}

const TArmatureData* GlobalData::getArmatureData(int id)
{
	for (unsigned i = 0; i < m_ArmatureArray.size(); i++)
	{
		if (m_ArmatureArray.at(i)->id == id)
		{
			return m_ArmatureArray.at(i);
		}
	}

	return NULL;
}

const TArmatureData* GlobalData::getArmatureData(const std::string& name)
{
	for (unsigned i = 0; i < m_ArmatureArray.size(); i++)
	{
		if (m_ArmatureArray.at(i)->name == name)
		{
			return m_ArmatureArray.at(i);
		}
	}

	return NULL;
}

const TTrackData* GlobalData::getTrackData(int id)
{
	for (unsigned i = 0; i < m_TrackArray.size(); i++)
	{
		if (m_TrackArray.at(i)->id == id)
		{
			return m_TrackArray.at(i);
		}
	}

	return NULL;
}

const TRewardData* GlobalData::getRewardData(int id)
{
	for (unsigned i = 0; i < m_RewardArray.size(); i++)
	{
		if (m_RewardArray.at(i)->id == id)
		{
			return m_RewardArray.at(i);
		}
	}

	return NULL;
}

const TRewardData* GlobalData::getRewardData(const char* const rewardType)
{
	for (unsigned i = 0; i < m_RewardArray.size(); i++)
	{
		if (!strcmp(m_RewardArray.at(i)->type.c_str(), rewardType))
		{
			return m_RewardArray.at(i);
		}
	}

	return NULL;
}

const TLevelData* GlobalData::getLevelData(int id)
{
	for (unsigned i = 0; i < m_LevelArray.size(); i++)
	{
		if (m_LevelArray.at(i)->id == id)
		{
			return m_LevelArray.at(i);
		}
	}

	return NULL;
}

const std::vector<TLevelBackground*>* GlobalData::getRandomBackground()
{
	int cnt = m_levelBgMap.size();
	int idx = (int)(Utils::getRandom()* cnt);

	//需剔除901急速模式的背景
	for (auto p : m_levelBgMap)
	{
		if (idx-- < 0)
		{
			if (p.first == 901)
			{
				return m_levelBgMap.begin()->second;
			}
			else
			{
				return p.second;
			}
		}
	}

	for (auto p : m_levelBgMap)
	{
		return p.second;
	}

	return nullptr;
}

int GlobalData::getKillerId()
{
	for (unsigned i = 0; i < m_RewardArray.size(); i++)
	{
		if (!strcmp(m_RewardArray.at(i)->type.c_str(), REWARD_TYPE_KILLER))
		{
			return (int)m_RewardArray.at(i)->paramMap.at(REWARD_ATTR_WINGMAN);
		}
	}

	return -1;
}

int GlobalData::getSuperKillerId()
{
	for (unsigned i = 0; i < m_RewardArray.size(); i++)
	{
		if (!strcmp(m_RewardArray.at(i)->type.c_str(), REWARD_TYPE_KILLER))
		{
			return (int)m_RewardArray.at(i)->paramMap.at(REWARD_ATTR_WINGMAN);
		}
	}

	return -1;
}

const TDifficultyData* GlobalData::getDifficulty()
{
	//当前的游戏模式
	auto mode = GameData::getInstance()->getValueToInt(GAMEDATA::MODE);
	//当前模式的游戏难度
	int difficulty = 1;
	switch (mode)
	{
	case ModeBase::ModeLevel:
		difficulty = GameData::getInstance()->getValueToInt(GAMEDATA::DIFFICULTY_LEVEL);
		break;
	case ModeBase::ModeEndless:
		difficulty = GameData::getInstance()->getValueToInt(GAMEDATA::DIFFICULTY_ENDLESS);
		break;
	case ModeBase::ModeRapid:
		difficulty = GameData::getInstance()->getValueToInt(GAMEDATA::DIFFICULTY_RAPID);
		break;
	}

	auto result = getDifficulty(difficulty);
	if (result)
	{
		return result;
	}
	else //找不到对应难度，则取当前模式下的最高难度
	{
		int max = -1;
		for (auto d : m_difficultyMap)
		{
			if (d.second->type == mode && d.second->id > max)
			{
				max = d.second->id;
			}
		}

		switch (mode)
		{
		case ModeBase::ModeLevel:
			GameData::getInstance()->setValue(GAMEDATA::DIFFICULTY_LEVEL, max);
			break;
		case ModeBase::ModeEndless:
			GameData::getInstance()->setValue(GAMEDATA::DIFFICULTY_ENDLESS, max);
			break;
		case ModeBase::ModeRapid:
			GameData::getInstance()->setValue(GAMEDATA::DIFFICULTY_RAPID, max);
			break;
		}

		return getDifficulty(max);
	}
}

const std::vector<TReinforceGradeData*>& GlobalData::getReinforce(int reinforceId)
{
	for (auto p : m_ReinforceArray)
	{
		if (p->id == reinforceId)
		{
			return p->gradeArray;
		}
	}

	return *(std::vector<TReinforceGradeData*>*)nullptr;
}

const std::string& GlobalData::getValue(const DbKVPtr pKv, const std::string& key)
{
	static std::string empty = "";
	DbKVIter it = pKv->find(key);

	if (it != pKv->end())
	{
		return it->second;
	}
	else
	{
		return empty;
	}
}

const int GlobalData::getValueToInt(const DbKVPtr pKv, const std::string& key)
{
	return Utils::str2int(getValue(pKv, key));
}

const float GlobalData::getValueToFloat(const DbKVPtr pKv, const std::string& key)
{
	return Utils::str2float(getValue(pKv, key));
}

int GlobalData::getParameterValue(std::map<std::string, std::string>& map, const char* const pKey, int defaultValue)
{
	auto it = map.find(pKey);
	if (it != map.end())
	{
		return Utils::str2int(it->second);
	}
	else
	{
		return defaultValue;
	}
}

const std::string& GlobalData::getParameter(const std::string& key, const std::string& defaultValue)
{
	auto it = m_ParameterMap.find(key);
	if (it != m_ParameterMap.end())
	{
		return it->second;
	}
	else
	{
		return defaultValue;
	}
}

int GlobalData::getParameterToInt(const std::string& key, const std::string& defaultValue)
{
	return Utils::str2int(getParameter(key, defaultValue));
}

float GlobalData::getParameterToFloat(const std::string& key, const std::string& defaultValue)
{
	return Utils::str2float(getParameter(key, defaultValue));
}

bool GlobalData::initAnimation()
{
	//animation 表
	DbRecordSetPtr pRecordSet = new DbRecordSet();
	DbHelper::getInstance()->getRecord(DBTABLE::ANIMATION::TABLE, pRecordSet);
	for (unsigned i = 0; i < pRecordSet->size(); i++)
	{
		DbKVPtr pRecord = pRecordSet->at(i);
		TAnimationData* pData = new TAnimationData;

		pData->id = getValueToInt(pRecord, DBTABLE::ANIMATION::COL_ID);
		pData->dura = getValueToFloat(pRecord, DBTABLE::ANIMATION::COL_DURATION);

		m_AnimationMap.insert(std::map<int, TAnimationData*>::value_type(pData->id, pData));
	}

	//animation_file 表
	DbKV order;
	order.insert(DbKV::value_type(DBTABLE::ANIMATION_FILE::COL_ANIMATION_ID, "asc"));
	order.insert(DbKV::value_type(DBTABLE::ANIMATION_FILE::COL_SEQ, "asc"));
	DbRecordSetPtr pRecordSet2 = new DbRecordSet();
	DbHelper::getInstance()->getRecord(DBTABLE::ANIMATION_FILE::TABLE, pRecordSet2, NULL, &order);
	for (unsigned j = 0; j < pRecordSet2->size(); j++)
	{
		DbKVPtr pRecord = pRecordSet2->at(j);

		int id = getValueToInt(pRecord, DBTABLE::ANIMATION_FILE::COL_ANIMATION_ID);
		const std::string& filename = getValue(pRecord, DBTABLE::ANIMATION_FILE::COL_FILENAME);//临时用这个值代替

		auto it = m_AnimationMap.find(id);
		if (it != m_AnimationMap.end())
		{
			it->second->filenames.push_back(filename);
		}
	}

	return true;
}

int GlobalData::getAnimation(int id, float& duration, std::vector<std::string>& filenames)
{
	auto it = m_AnimationMap.find(id);
	if (it != m_AnimationMap.end())
	{
		duration = it->second->dura;
		unsigned cnt = it->second->filenames.size();
		for (unsigned i = 0; i < cnt; i++)
		{
			filenames.push_back(it->second->filenames.at(i));
		}
		return cnt;
	}

	return 0;
}

bool GlobalData::initGlobal()
{
	m_ParameterMap.clear();

	DbRecordSetPtr pRecordSet = new DbRecordSet();
	int cnt = DbHelper::getInstance()->getRecord(DBTABLE::PARAMETER::TABLE, pRecordSet);
	for (int i = 0; i < cnt; i++)
	{
		DbKVPtr pRecord = pRecordSet->at(i);
		const std::string& key = getValue(pRecord, DBTABLE::PARAMETER::COL_KEY);
		const std::string& value = getValue(pRecord, DBTABLE::PARAMETER::COL_VALUE);

		m_ParameterMap.insert(std::map<std::string, std::string>::value_type(key, value));

		//tips
		if (!strncmp(key.c_str(), "tips_", 5))
		{
			m_tipsArray.push_back(value);
		}
	}
		
	//测试模式仅在电脑上有用
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	m_GlobalData.debug = getParameterValue(m_ParameterMap, GLOBALDATA::DEBUG, 0);
#else
	m_GlobalData.debug = 0;
#endif

	if (m_GlobalData.debug)
	{
		//m_GlobalData.log           = getParameterValue(m_ParameterMap, GLOBALDATA::LOG, 0);
		m_GlobalData.display_body  = getParameterValue(m_ParameterMap, GLOBALDATA::DISPLAY_BODY, 0);
		m_GlobalData.ignore_menu   = getParameterValue(m_ParameterMap, GLOBALDATA::IGNORE_MENU, 0);
		m_GlobalData.only_boss     = getParameterValue(m_ParameterMap, GLOBALDATA::ONLY_BOSS, 0);
		m_GlobalData.sound         = getParameterValue(m_ParameterMap, GLOBALDATA::SOUND, 1);
		m_GlobalData.level_id      = getParameterValue(m_ParameterMap, GLOBALDATA::LEVEL_ID, 1);
		m_GlobalData.enemy_seq     = getParameterValue(m_ParameterMap, GLOBALDATA::ENEMY_SEQ, 1);
		m_GlobalData.tutorial_test = getParameterValue(m_ParameterMap, GLOBALDATA::TUTORIAL_TEST, 0);
		m_GlobalData.day_gift_test = getParameterValue(m_ParameterMap, GLOBALDATA::DAY_GIFT_TEST, 0);
	}
	else
	{
		//m_GlobalData.log          = 0;
		m_GlobalData.display_body = 0;
		m_GlobalData.ignore_menu  = 0;
		m_GlobalData.only_boss    = 0;
		m_GlobalData.sound        = 1;
		m_GlobalData.level_id     = -1;
		m_GlobalData.enemy_seq    = -1;
		m_GlobalData.tutorial_test = 0;
		m_GlobalData.day_gift_test = 0;
	}

	m_GlobalData.log = getParameterValue(m_ParameterMap, GLOBALDATA::LOG, 0);
	m_GlobalData.life = getParameterValue(m_ParameterMap, GLOBALDATA::LIFE, 0);
	m_GlobalData.killer = getParameterValue(m_ParameterMap, GLOBALDATA::KILLER, 0);
	m_GlobalData.shield   = getParameterValue(m_ParameterMap, GLOBALDATA::SHIELD, 0);
	m_GlobalData.player    = getParameterValue(m_ParameterMap, GLOBALDATA::PLAYER, 0);
	m_GlobalData.cd_time   = getParameterValue(m_ParameterMap, GLOBALDATA::CD_TIME, 0);

	return true;
}

bool GlobalData::initHpBar()
{
	DbRecordSetPtr pRecordSet2 = new DbRecordSet();
	int cnt = DbHelper::getInstance()->getRecord(DBTABLE::HPBAR::TABLE, pRecordSet2);
	for (int i = 0; i < cnt; i++)
	{
		DbKVPtr pRecord2 = pRecordSet2->at(i);
		int id = getValueToInt(pRecord2, DBTABLE::HPBAR::COL_ID);
		THpBarData* pData = new THpBarData;
		pData->hpStyle = getValue(pRecord2, DBTABLE::HPBAR::COL_FOREGROUND);
		pData->hpBgStyle = getValue(pRecord2, DBTABLE::HPBAR::COL_BACKGROUND);

		m_HpBarMap.insert(std::map<int, THpBarData*>::value_type(id, pData));
	}
	CC_SAFE_DELETE(pRecordSet2);

	return true;
}


bool GlobalData::initAircraftLevel()
{
	DbRecordSetPtr pRecordSet = new DbRecordSet();
	int cnt = DbHelper::getInstance()->getRecord(DBTABLE::AIRCRAFT_LEVEL::TABLE, pRecordSet);
	if (cnt <= 0)
	{
		CCLOG("GlobalData::initAircraftLevel : no record");
		return false;
	}

	for (unsigned i = 0; i < pRecordSet->size(); i++)
	{
		TAircraftLevelData* pData = new TAircraftLevelData;
		DbKVPtr pRecord = pRecordSet->at(i);

		pData->id = getValueToInt(pRecord, DBTABLE::AIRCRAFT_LEVEL::COL_ID);
		pData->aircraftId = getValueToInt(pRecord, DBTABLE::AIRCRAFT_LEVEL::COL_AIRCRAFT_ID);
		pData->grade = getValueToInt(pRecord, DBTABLE::AIRCRAFT_LEVEL::COL_GRADE);
		pData->hp = getValueToInt(pRecord, DBTABLE::AIRCRAFT_LEVEL::COL_HP);
		pData->money = getValueToInt(pRecord, DBTABLE::AIRCRAFT_LEVEL::COL_MONEY);
		pData->score = getValueToInt(pRecord, DBTABLE::AIRCRAFT_LEVEL::COL_SCORE);

		//子弹组
		{
			int bulletGradeId = getValueToInt(pRecord, DBTABLE::AIRCRAFT_LEVEL::COL_BULLET_GRADE_ID);
			pData->bulletIdArray.clear();
			auto array = getBulletGradeData(bulletGradeId);
			if (array != NULL)
			{
				for (auto a : *array)
				{
					auto xx = new std::vector<int>;
					for (auto i : *a)
					{
						xx->push_back(i);
					}
					pData->bulletIdArray.push_back(xx);
				}
			}

			if (pData->bulletIdArray.size() == 0)
			{
				int bulletId = getValueToInt(pRecord, DBTABLE::AIRCRAFT_LEVEL::COL_BULLET_ID);
				if (bulletId > 0)
				{
					std::vector<int>* pBulletArrray = new std::vector<int>();
					pBulletArrray->push_back(bulletId);
					pData->bulletIdArray.push_back(pBulletArrray);
				}
			}
		}

		m_aircraftLevelMap.insert(std::map<int, TAircraftLevelData*>::value_type(pData->id, pData));

		//std::map<int, std::map<int, TAircraftLevelData*>*> m_aircraftLevelIdGradeMap;
		//第一个int是aircraftId，第二个int是grade
		auto it = m_aircraftLevelIdGradeMap.find(pData->aircraftId);
		if (it == m_aircraftLevelIdGradeMap.end())
		{
			auto map = new std::map<int, TAircraftLevelData*>;
			map->insert(std::map<int, TAircraftLevelData*>::value_type(pData->grade, pData));
			m_aircraftLevelIdGradeMap.insert(std::map<int, std::map<int, TAircraftLevelData*>*>::value_type(pData->aircraftId, map));
		}
		else
		{
			it->second->insert(std::map<int, TAircraftLevelData*>::value_type(pData->grade, pData));
		}
	}

	DbHelper::freeResultSet(pRecordSet);

	return true;
}

bool GlobalData::initAircraft(const char* pType, std::vector<TAircraftData*>* pAircraftArray)
{
	DbRecordSetPtr pRecordSet = new DbRecordSet();
	int cnt = DbHelper::getInstance()->getRecordByKey(DBTABLE::AIRCRAFT::TABLE, pRecordSet, 
		DBTABLE::AIRCRAFT::COL_TYPE, pType);
	if (cnt <= 0)
	{
		CCLOG("GlobalData::initAircraft : no record");
		return false;
	}

	//属性
	DbRecordSetPtr pRecordSet2 = new DbRecordSet();
	DbHelper::getInstance()->getRecord(DBTABLE::AIRCRAFT_ATTR::TABLE, pRecordSet2);

	for (unsigned i = 0; i < pRecordSet->size(); i++)
	{
		TAircraftData* pData = new TAircraftData;
		DbKVPtr pRecord = pRecordSet->at(i);

		pData->id = getValueToInt(pRecord, DBTABLE::AIRCRAFT::COL_ID);
		pData->name = getValue(pRecord, DBTABLE::AIRCRAFT::COL_NAME);
		pData->type = getValue(pRecord, DBTABLE::AIRCRAFT::COL_TYPE);
		pData->armatureName = getValue(pRecord, DBTABLE::AIRCRAFT::COL_ARMATURE_NAME);
		pData->hp = getValueToInt(pRecord, DBTABLE::AIRCRAFT::COL_HP);
		pData->maxHp = getValueToInt(pRecord, DBTABLE::AIRCRAFT::COL_HP);
		pData->attack = getValueToInt(pRecord, DBTABLE::AIRCRAFT::COL_ATTACK);
		pData->defence = getValueToInt(pRecord, DBTABLE::AIRCRAFT::COL_DEFENCE);
		pData->hpPosX = getValueToFloat(pRecord, DBTABLE::AIRCRAFT::COL_HPBAR_OFFSET_X);
		pData->hpPosY = getValueToFloat(pRecord, DBTABLE::AIRCRAFT::COL_HPBAR_OFFSET_Y);
		pData->bodyCenterX = getValueToFloat(pRecord, DBTABLE::AIRCRAFT::COL_BODY_CENTER_X);
		pData->bodyCenterY = getValueToFloat(pRecord, DBTABLE::AIRCRAFT::COL_BODY_CENTER_Y);
		pData->bodySizeW = getValueToFloat(pRecord, DBTABLE::AIRCRAFT::COL_BODY_SIZE_W);
		pData->bodySizeH = getValueToFloat(pRecord, DBTABLE::AIRCRAFT::COL_BODY_SIZE_H);
		pData->scale = getValueToFloat(pRecord, DBTABLE::AIRCRAFT::COL_SCALE);
		pData->fireOnTop = getValueToInt(pRecord, DBTABLE::AIRCRAFT::COL_FIRE_ON_TOP);
		pData->fireOffsetX = getValueToFloat(pRecord, DBTABLE::AIRCRAFT::COL_FIRE_OFFSET_X);
		pData->fireOffsetY = getValueToFloat(pRecord, DBTABLE::AIRCRAFT::COL_FIRE_OFFSET_Y);
		pData->startPosX = getValueToInt(pRecord, DBTABLE::AIRCRAFT::COL_START_POS_X);
		pData->startPosY = getValueToInt(pRecord, DBTABLE::AIRCRAFT::COL_START_POS_Y);

		getAnimation(getValueToInt(pRecord, DBTABLE::AIRCRAFT::COL_STYLE_ANI_ID), pData->aniDura, pData->styleArray);
		getAnimation(getValueToInt(pRecord, DBTABLE::AIRCRAFT::COL_BLAST_ANI_ID), pData->blastAniDura, pData->blastStyleArray);
		getAnimation(getValueToInt(pRecord, DBTABLE::AIRCRAFT::COL_FIRE_ANI_ID), pData->fireAniDura, pData->fireFameNameArray);

		pData->blastMusicId = getValueToInt(pRecord, DBTABLE::AIRCRAFT::COL_BLAST_MUSIC_ID);

		//血槽
		{
			int hpbar = getValueToInt(pRecord, DBTABLE::AIRCRAFT::COL_HPBAR_ID);
			auto it = m_HpBarMap.find(hpbar);
			if (it != m_HpBarMap.end())
			{
				pData->hpStyle = it->second->hpStyle;
				pData->hpBgStyle = it->second->hpBgStyle;
			}
		}

		//子弹组
		{
			int bulletGradeId = getValueToInt(pRecord, DBTABLE::AIRCRAFT::COL_BULLET_GRADE_ID);
			pData->bulletIdArray.clear();
			auto array = getBulletGradeData(bulletGradeId);
			if (array != NULL)
			{
				for (auto a : *array)
				{
					auto xx = new std::vector<int>;
					for (auto i : *a)
					{
						if (i != 0)
						{
							xx->push_back(i);
						}
					}
					pData->bulletIdArray.push_back(xx);
				}
			}

			if (pData->bulletIdArray.size() == 0)
			{
				std::vector<int>* pBulletArrray = new std::vector<int>();

				int bulletId = getValueToInt(pRecord, DBTABLE::AIRCRAFT::COL_BULLET_ID);
				if (bulletId != 0)
				{
					pBulletArrray->push_back(bulletId);
					pData->bulletIdArray.push_back(pBulletArrray);
				}
			}
		}
		// 僚机
		//{
		//	int wingman = getValueToInt(pRecord, DBTABLE::AIRCRAFT::COL_WINGMAN_ID);
		//	auto it = m_AircraftWingmanMap.find(wingman);
		//	if (it != m_AircraftWingmanMap.end())
		//	{
		//		pData->wingmanArray.clear();
		//		for (unsigned j = 0; j < it->second->size(); j++)
		//		{
		//			pData->wingmanArray.push_back(it->second->at(j));
		//		}
		//	}
		//}
		//属性
		{
			for (unsigned j = 0; j < pRecordSet2->size(); j++)
			{
				DbKVPtr pRecord2 = pRecordSet2->at(j);
				if (getValueToInt(pRecord2, DBTABLE::AIRCRAFT_ATTR::COL_AIRCRAFT_ID) == pData->id)
				{
					pData->paramMap.insert(std::map<std::string, std::string>::value_type(
						getValue(pRecord2, DBTABLE::AIRCRAFT_ATTR::COL_KEY),
						getValue(pRecord2, DBTABLE::AIRCRAFT_ATTR::COL_VALUE)));
				}
			}
		}

		pAircraftArray->push_back(pData);
	}

	DbHelper::freeResultSet(pRecordSet);
	DbHelper::freeResultSet(pRecordSet2);

	return true;
}

bool GlobalData::initPlayer()
{
	return initAircraft(AircraftType::PLAYER, &m_PlayerArray);
}

bool GlobalData::initEnemy()
{
	bool result = false;
	result = initAircraft(AircraftType::ENEMY1, &m_EnemyArray) || result;
	result = initAircraft(AircraftType::ENEMY2, &m_EnemyArray) || result;
	result = initAircraft(AircraftType::BOSS, &m_EnemyArray) || result;
	return result;
}

bool GlobalData::initWingman()
{
	return initAircraft(AircraftType::WINGMAN, &m_WingmanArray);
}

bool GlobalData::initBoss()
{
	TBossData* pData = new TBossData;
	m_BossArray.push_back(pData);

	//HP门限动作
	DbRecordSetPtr pRecordSet2 = new DbRecordSet();

	DbKV orderParam;
	orderParam.insert(std::map < std::string, std::string >::value_type(
		DBTABLE::BOSS_HP_ACTION::COL_AIRCRAFT_ID, "ASC"));
	orderParam.insert(std::map < std::string, std::string >::value_type(
		DBTABLE::BOSS_HP_ACTION::COL_HP_THRESHOLD, "DESC"));
	orderParam.insert(std::map < std::string, std::string >::value_type(
		DBTABLE::BOSS_HP_ACTION::COL_SEQ, "ASC"));

	int cnt = DbHelper::getInstance()->getRecord(DBTABLE::BOSS_HP_ACTION::TABLE, 
		pRecordSet2, nullptr, &orderParam);
	for (unsigned j = 0; j < pRecordSet2->size(); j++)
	{
		DbKVPtr pRecord2 = pRecordSet2->at(j);

		TBossHpActionData hpAction;
		int aircraftId = getValueToInt(pRecord2, DBTABLE::BOSS_HP_ACTION::COL_AIRCRAFT_ID);
		hpAction.seq = getValueToInt(pRecord2, DBTABLE::BOSS_HP_ACTION::COL_SEQ);
		hpAction.hpThreshold = getValueToInt(pRecord2, DBTABLE::BOSS_HP_ACTION::COL_HP_THRESHOLD);
		hpAction.delay = getValueToFloat(pRecord2, DBTABLE::BOSS_HP_ACTION::COL_DELAY);
		hpAction.repeat = getValueToInt(pRecord2, DBTABLE::BOSS_HP_ACTION::COL_REPEAT);
		int actionGroupId = getValueToInt(pRecord2, DBTABLE::BOSS_HP_ACTION::COL_ACTION_GROUP_ID);

		//动作组
		{
			DbRecordSetPtr pRecordSet3 = new DbRecordSet();
			int cnt = DbHelper::getInstance()->getRecordByKey(DBTABLE::BOSS_ACTION_GROUP::TABLE,
				pRecordSet3, DBTABLE::BOSS_ACTION_GROUP::COL_GROUP_ID, actionGroupId,
				DBTABLE::BOSS_ACTION_GROUP::COL_SEQ, EDbSort::ASC);
			for (unsigned k = 0; k < pRecordSet3->size(); k++)
			{
				DbKVPtr pRecord3 = pRecordSet3->at(k);
				TBossActionGroupData actionGroupData;
				actionGroupData.seq = getValueToInt(pRecord3, DBTABLE::BOSS_ACTION_GROUP::COL_SEQ);
				actionGroupData.actionName = getValue(pRecord3, DBTABLE::BOSS_ACTION_GROUP::COL_ARMATURE_ACTION);
				actionGroupData.noDie = getValueToInt(pRecord3, DBTABLE::BOSS_ACTION_GROUP::COL_NO_DIE);
				actionGroupData.repeat = getValueToInt(pRecord3, DBTABLE::BOSS_ACTION_GROUP::COL_REPEAT);
				actionGroupData.delay = getValueToFloat(pRecord3, DBTABLE::BOSS_ACTION_GROUP::COL_DELAY);
				actionGroupData.bulletGradeId = getValueToInt(pRecord3, DBTABLE::BOSS_ACTION_GROUP::COL_BULLET_GRADE_ID);
				actionGroupData.bulletFlag = (EBulletFlag)getValueToInt(pRecord3, DBTABLE::BOSS_ACTION_GROUP::COL_BULLET_FLAG);

				hpAction.actionArray.push_back(actionGroupData);
			}
		}

		if (aircraftId != m_BossArray.back()->aircraftId
			&& -1 != m_BossArray.back()->aircraftId)
		{
			pData = new TBossData;
			pData->aircraftId = aircraftId;
			m_BossArray.push_back(pData);
		}

		m_BossArray.back()->aircraftId = aircraftId;
		m_BossArray.back()->hpActionArray.push_back(hpAction);
	}
	DbHelper::freeResultSet(pRecordSet2);


	return true;
}

bool GlobalData::initBulletGrade()
{
	//select * from bullet_grade order by id asc ,grade asc,bullet_id asc
	DbRecordSetPtr pRecordSet = new DbRecordSet;
	std::string col = std::string(" distinct ") + std::string(DBTABLE::BULLET_GRADE::COL_ID);
	char sz[200] = { 0 };
	snprintf(sz, 199, "select * from %s order by %s asc, %s asc, %s asc",
		DBTABLE::BULLET_GRADE::TABLE,
		DBTABLE::BULLET_GRADE::COL_ID,
		DBTABLE::BULLET_GRADE::COL_GRADE,
		DBTABLE::BULLET_GRADE::COL_BULLET_ID);
	int cnt = DbHelper::getInstance()->getRecordBySql(sz, pRecordSet);

	//DbKV order;
	//order.insert(std::map < std::string, std::string >::value_type(DBTABLE::BULLET_GRADE::COL_ID, "asc"));
	//order.insert(std::map < std::string, std::string >::value_type(DBTABLE::BULLET_GRADE::COL_GRADE, "asc"));
	//order.insert(std::map < std::string, std::string >::value_type(DBTABLE::BULLET_GRADE::COL_BULLET_ID, "asc"));
	//DbRecordSetPtr pRecordSet = new DbRecordSet;
	//int cnt = DbHelper::getInstance()->getRecord(DBTABLE::BULLET_GRADE::TABLE, pRecordSet, NULL, &order);

	int preGrade = -1;
	for (unsigned i = 0; i < pRecordSet->size(); i++)
	{
		DbKVPtr pRecord = pRecordSet->at(i);
		int id = getValueToInt(pRecord, DBTABLE::BULLET_GRADE::COL_ID);
		int grade = getValueToInt(pRecord, DBTABLE::BULLET_GRADE::COL_GRADE);
		int bulletId = getValueToInt(pRecord, DBTABLE::BULLET_GRADE::COL_BULLET_ID);

		auto it = m_BulletGradeMap.find(id);
		if (it != m_BulletGradeMap.end()) //找到id
		{
			if (preGrade == -1) //第一个grade
			{
				std::vector<int>* pBulletArrray = new std::vector<int>();
				pBulletArrray->push_back(bulletId);
				it->second->push_back(pBulletArrray);
			}
			else if (preGrade != -1 && preGrade == grade)  //找到了grade，插入到最后一个grade中去
			{
				int idx = it->second->size() - 1;
				it->second->at(idx)->push_back(bulletId);
			}
			else if (preGrade != -1 && preGrade != grade)  //是一个新的grade
			{
				std::vector<int>* pBulletArrray = new std::vector<int>();
				pBulletArrray->push_back(bulletId);
				it->second->push_back(pBulletArrray);
			}
		}
		else //没找到id
		{
			std::vector<int>* pBulletArrray = new std::vector<int>();
			pBulletArrray->push_back(bulletId);

			auto pBulletIdArray = new std::vector<std::vector<int>*>;
			pBulletIdArray->push_back(pBulletArrray);

			m_BulletGradeMap.insert(std::map<int, std::vector<std::vector<int>*>*>::value_type(id, pBulletIdArray));
		}

		preGrade = grade;
	}

	DbHelper::freeResultSet(pRecordSet);

	return true;
}

bool GlobalData::initBullet()
{
	//bullet
	DbRecordSetPtr pRecordSet = new DbRecordSet();
	DbKV order;
	order.insert(DbKV::value_type(DBTABLE::BULLET::COL_ID, "ASC"));
	int cnt = DbHelper::getInstance()->getRecord(DBTABLE::BULLET::TABLE, pRecordSet, NULL, &order);
	if (cnt <= 0)
	{
		DbHelper::freeResultSet(pRecordSet);
		CCLOG("GlobalData::initBullet : no record");
		return false;
	}

	std::map<int, TBulletData*> bulletMap;
	for (unsigned i = 0; i < pRecordSet->size(); i++)
	{
		TBulletData* pData = new TBulletData;
		DbKVPtr pRecord = pRecordSet->at(i);

		pData->id = getValueToInt(pRecord, DBTABLE::BULLET::COL_ID);
		pData->name = getValue(pRecord, DBTABLE::BULLET::COL_NAME);
		pData->type = getValue(pRecord, DBTABLE::BULLET::COL_TYPE);
		pData->armatureName = getValue(pRecord, DBTABLE::BULLET::COL_ARMATURE_NAME);
		pData->topOfAircraft = getValueToInt(pRecord, DBTABLE::BULLET::COL_TOP_OF_AIRCRAFT);
		pData->musicId = getValueToInt(pRecord, DBTABLE::BULLET::COL_MUSIC_ID);
		
		pData->attack = getValueToInt(pRecord, DBTABLE::BULLET::COL_ATTACK);
		pData->speed = getValueToFloat(pRecord, DBTABLE::BULLET::COL_SPEED);

		pData->count = getValueToInt(pRecord, DBTABLE::BULLET::COL_COUNT);
		pData->count = (pData->count > 0 ? pData->count : 1);
		pData->max_count = getValueToInt(pRecord, DBTABLE::BULLET::COL_MAX_COUNT);
		pData->timeLimit = getValueToFloat(pRecord, DBTABLE::BULLET::COL_TIME_LIMIT);
		
		pData->angle_of_center = getValueToFloat(pRecord, DBTABLE::BULLET::COL_ANGLE_OF_CENTER);
		pData->angle_interval = getValueToFloat(pRecord, DBTABLE::BULLET::COL_ANGLE_INTERVAL);
		pData->x_interval = getValueToFloat(pRecord, DBTABLE::BULLET::COL_X_INTERVAL);
		
		pData->delay = getValueToFloat(pRecord, DBTABLE::BULLET::COL_DELAY);
		pData->interval = getValueToFloat(pRecord, DBTABLE::BULLET::COL_INTERVAL);
		pData->interval_2 = getValueToFloat(pRecord, DBTABLE::BULLET::COL_INTERVAL_2);
		pData->interval_2_cnt = getValueToInt(pRecord, DBTABLE::BULLET::COL_INTERVAL_2_CNT);
	
		pData->rotate_angle = getValueToFloat(pRecord, DBTABLE::BULLET::COL_ROTATE_ANGLE);
		pData->rotate_max_angle = getValueToFloat(pRecord, DBTABLE::BULLET::COL_ROTATE_MAX_ANGLE);
		pData->rotate_flag = getValueToInt(pRecord, DBTABLE::BULLET::COL_ROTATE_FLAG);

		pData->bodyCenterX = getValueToFloat(pRecord, DBTABLE::BULLET::COL_BODY_CENTER_X);
		pData->bodyCenterY = getValueToFloat(pRecord, DBTABLE::BULLET::COL_BODY_CENTER_Y);
		pData->bodySizeW = getValueToFloat(pRecord, DBTABLE::BULLET::COL_BODY_SIZE_W);
		pData->bodySizeH = getValueToFloat(pRecord, DBTABLE::BULLET::COL_BODY_SIZE_H);
		pData->origin_offset_x = getValueToFloat(pRecord, DBTABLE::BULLET::COL_ORIGIN_OFFSET_X);
		pData->origin_offset_y = getValueToFloat(pRecord, DBTABLE::BULLET::COL_ORIGIN_OFFSET_Y);
		pData->fireOnTop = getValueToInt(pRecord, DBTABLE::BULLET::COL_FIRE_ON_TOP);
		pData->fireOffsetX = getValueToFloat(pRecord, DBTABLE::BULLET::COL_FIRE_OFFSET_X);
		pData->fireOffsetY = getValueToFloat(pRecord, DBTABLE::BULLET::COL_FIRE_OFFSET_Y);

		getAnimation(getValueToInt(pRecord, DBTABLE::BULLET::COL_STYLE_ANI_ID), pData->aniDura, pData->styleArray);
		getAnimation(getValueToInt(pRecord, DBTABLE::BULLET::COL_BLAST_ANI_ID), pData->blastAniDura, pData->blastStyleArray);
		getAnimation(getValueToInt(pRecord, DBTABLE::BULLET::COL_FIRE_ANI_ID), pData->fireAniDura, pData->fireFameNameArray);

		m_BulletArray.push_back(pData);
		bulletMap.insert(std::map<int, TBulletData*>::value_type(pData->id, pData));
	}
	DbHelper::freeResultSet(pRecordSet);

	//bullet_attr
	DbRecordSetPtr pRecordSet2 = new DbRecordSet();
	int cnt2 = DbHelper::getInstance()->getRecord(DBTABLE::BULLET_ATTR::TABLE, pRecordSet2);
	for (int j = 0; j < cnt2; j++)
	{
		DbKVPtr pRecord2 = pRecordSet2->at(j);

		int bulletId = getValueToInt(pRecord2, DBTABLE::BULLET_ATTR::COL_BULLET_ID);
		auto it = bulletMap.find(bulletId);
		if (it != bulletMap.end())
		{
			it->second->paramMap.insert(std::map<std::string, float>::value_type(
				getValue(pRecord2, DBTABLE::BULLET_ATTR::COL_KEY),
				getValueToFloat(pRecord2, DBTABLE::BULLET_ATTR::COL_VALUE)));
		}
	}
	DbHelper::freeResultSet(pRecordSet2);

	//bullet_custom
	DbRecordSetPtr pRecordSet3 = new DbRecordSet();
	DbKV order2;
	order2.insert(DbKV::value_type(DBTABLE::BULLET_CUSTOM::COL_TIME, "asc"));
	int cnt3 = DbHelper::getInstance()->getRecord(DBTABLE::BULLET_CUSTOM::TABLE, pRecordSet3, NULL, &order2);
	for (int j = 0; j < cnt3; j++)
	{
		DbKVPtr pRecord2 = pRecordSet3->at(j);

		int bulletId = getValueToInt(pRecord2, DBTABLE::BULLET_CUSTOM::COL_BULLET_ID);
		auto it = bulletMap.find(bulletId);
		if (it != bulletMap.end())
		{
			TBulletCustomData* pCustomData = new TBulletCustomData;
			pCustomData->time = getValueToFloat(pRecord2, DBTABLE::BULLET_CUSTOM::COL_TIME);
			pCustomData->x = getValueToFloat(pRecord2, DBTABLE::BULLET_CUSTOM::COL_X);
			pCustomData->y = getValueToFloat(pRecord2, DBTABLE::BULLET_CUSTOM::COL_Y);
			pCustomData->speed = getValueToFloat(pRecord2, DBTABLE::BULLET_CUSTOM::COL_SPEED);
			pCustomData->angle = getValueToFloat(pRecord2, DBTABLE::BULLET_CUSTOM::COL_ANGLE);

			it->second->customArray.push_back(pCustomData);
		}

	}
	DbHelper::freeResultSet(pRecordSet3);

	return true;
}

bool GlobalData::initLevel()
{
	//关卡背景
	{
		DbKV order;
		order.insert(DbKV::value_type(DBTABLE::LEVEL_BACKGROUND::COL_SUB_GROUP_ID, "asc"));
		order.insert(DbKV::value_type(DBTABLE::LEVEL_BACKGROUND::COL_ZORDER, "asc"));

		DbRecordSetPtr pRecordSet2 = new DbRecordSet();
		int cnt = DbHelper::getInstance()->getRecord(DBTABLE::LEVEL_BACKGROUND::TABLE, pRecordSet2, nullptr, &order);
		for (unsigned j = 0; j < pRecordSet2->size(); j++)
		{
			DbKVPtr pRecord2 = pRecordSet2->at(j);

			TLevelBackground* pDataBg = new TLevelBackground;
			pDataBg->group_id = getValueToInt(pRecord2, DBTABLE::LEVEL_BACKGROUND::COL_GROUP_ID);
			pDataBg->sub_group_id = getValueToInt(pRecord2, DBTABLE::LEVEL_BACKGROUND::COL_SUB_GROUP_ID);
			pDataBg->zorder = getValueToInt(pRecord2, DBTABLE::LEVEL_BACKGROUND::COL_ZORDER);
			pDataBg->filename = getValue(pRecord2, DBTABLE::LEVEL_BACKGROUND::COL_FILENAME);
			pDataBg->speed_start = getValueToFloat(pRecord2, DBTABLE::LEVEL_BACKGROUND::COL_SPEED_START);
			pDataBg->speed_end = getValueToFloat(pRecord2, DBTABLE::LEVEL_BACKGROUND::COL_SPEED_END);
			pDataBg->duration = getValueToFloat(pRecord2, DBTABLE::LEVEL_BACKGROUND::COL_DURATION);
			pDataBg->distance = getValueToFloat(pRecord2, DBTABLE::LEVEL_BACKGROUND::COL_DISTANCE);
			pDataBg->count = getValueToInt(pRecord2, DBTABLE::LEVEL_BACKGROUND::COL_COUNT);
			pDataBg->flip_y = getValueToInt(pRecord2, DBTABLE::LEVEL_BACKGROUND::COL_FLIP_Y);

			auto it = m_levelBgMap.find(pDataBg->group_id);
			if (it != m_levelBgMap.end())
			{
				it->second->push_back(pDataBg);
			}
			else
			{
				auto array = new std::vector<TLevelBackground*>;
				m_levelBgMap.insert(std::map<int, std::vector<TLevelBackground*>*>::value_type(pDataBg->group_id, array));
				m_levelBgMap.at(pDataBg->group_id)->push_back(pDataBg);
			}
		}
		DbHelper::freeResultSet(pRecordSet2);
	}

	//关卡事件
	std::map<int, std::vector<TLevelEventData*>*> levelEventMap;
	{
		DbKV order;
		order.insert(DbKV::value_type(DBTABLE::LEVEL_EVENT::COL_GROUP_ID, "asc"));
		order.insert(DbKV::value_type(DBTABLE::LEVEL_EVENT::COL_SEQ, "asc"));

		DbRecordSetPtr pRecordSet2 = new DbRecordSet();
		int cnt = DbHelper::getInstance()->getRecord(DBTABLE::LEVEL_EVENT::TABLE, pRecordSet2, nullptr, &order);
		for (unsigned j = 0; j < pRecordSet2->size(); j++)
		{
			DbKVPtr pRecord2 = pRecordSet2->at(j);

			TLevelEventData* enemy = new TLevelEventData;
			enemy->group_id = getValueToInt(pRecord2, DBTABLE::LEVEL_EVENT::COL_GROUP_ID);
			enemy->seq = getValueToInt(pRecord2, DBTABLE::LEVEL_EVENT::COL_SEQ);
			enemy->event = getValue(pRecord2, DBTABLE::LEVEL_EVENT::COL_EVENT);
			enemy->time = getValueToFloat(pRecord2, DBTABLE::LEVEL_EVENT::COL_TIME);
			enemy->pause = getValueToInt(pRecord2, DBTABLE::LEVEL_EVENT::COL_PAUSE);
			enemy->object_id = getValueToFloat(pRecord2, DBTABLE::LEVEL_EVENT::COL_OBJECT_ID);
			enemy->bullet = getValueToInt(pRecord2, DBTABLE::LEVEL_EVENT::COL_BULLET_ID);
			enemy->track = getValueToInt(pRecord2, DBTABLE::LEVEL_EVENT::COL_TRACK_ID);
			enemy->direction_flag = getValueToInt(pRecord2, DBTABLE::LEVEL_EVENT::COL_DIRECTION_FLAG);
			enemy->repeat = getValueToInt(pRecord2, DBTABLE::LEVEL_EVENT::COL_REPEAT);
			enemy->interval = getValueToFloat(pRecord2, DBTABLE::LEVEL_EVENT::COL_INTERVAL);
			int rewardGroupId = getValueToInt(pRecord2, DBTABLE::LEVEL_EVENT::COL_REWARD_GROUP);

			//奖励品组
			auto it = m_RewardGroupMap.find(rewardGroupId);
			if (it != m_RewardGroupMap.end())
			{
				enemy->rewardArray.clear();
				for (unsigned x = 0; x < it->second->size(); x++)
				{
					enemy->rewardArray.push_back(it->second->at(x));
				}
			}

			auto it2 = levelEventMap.find(enemy->group_id);
			if (it2 != levelEventMap.end())
			{
				it2->second->push_back(enemy);
			}
			else
			{
				auto array = new std::vector<TLevelEventData*>;
				levelEventMap.insert(std::map<int, std::vector<TLevelEventData*>*>::value_type(enemy->group_id, array));
				levelEventMap.at(enemy->group_id)->push_back(enemy);
			}
		}
		DbHelper::freeResultSet(pRecordSet2);
	}

	DbRecordSetPtr pRecordSet = new DbRecordSet();
	int cnt = DbHelper::getInstance()->getRecord(DBTABLE::LEVEL::TABLE, pRecordSet);
	if (cnt <= 0)
	{
		DbHelper::freeResultSet(pRecordSet);
		CCLOG("GlobalData::initLevel : no record");
		return false;
	}

	for (unsigned i = 0; i < pRecordSet->size(); i++)
	{
		TLevelData* pData = new TLevelData;
		DbKVPtr pRecord = pRecordSet->at(i);

		pData->id = getValueToInt(pRecord, DBTABLE::LEVEL::COL_ID);
		pData->name = getValue(pRecord, DBTABLE::LEVEL::COL_NAME);
		pData->musicId = getValueToInt(pRecord, DBTABLE::LEVEL::COL_MUSIC_ID);
		pData->bossType = (BossType)getValueToInt(pRecord, DBTABLE::LEVEL::COL_BOSS_TYPE);
		pData->piece = getValueToInt(pRecord, DBTABLE::LEVEL::COL_PIECE);

		int bg_group_id = getValueToInt(pRecord, DBTABLE::LEVEL::COL_BG_GROUP_ID);
		int reward_group_id = getValueToInt(pRecord, DBTABLE::LEVEL::COL_REWARD_GROUP_ID);
		int enemy_group_id = getValueToInt(pRecord, DBTABLE::LEVEL::COL_ENEMY_GROUP_ID);

		//关卡背景
		pData->background = m_levelBgMap.find(bg_group_id)->second;

		//关卡事件
		pData->enemy = levelEventMap.find(enemy_group_id)->second;

		//奖励品出现概率。暂时去除此功能
		{
			//DbRecordSetPtr pRecordSet2 = new DbRecordSet();
			//int cnt = DbHelper::getInstance()->getRecordByKey(DBTABLE::LEVEL_REWARD::TABLE, pRecordSet2,
			//	DBTABLE::LEVEL_REWARD::COL_GROUP_ID, reward_group_id);
			//for (unsigned j = 0; j < pRecordSet2->size(); j++)
			//{
			//	DbKVPtr pRecord2 = pRecordSet2->at(j);

			//	TLevelRewardData reward;
			//	reward.id = getValueToInt(pRecord2, DBTABLE::LEVEL_REWARD::COL_REWARD_ID);
			//	reward.probability = getValueToFloat(pRecord2, DBTABLE::LEVEL_REWARD::COL_PROBABILITY);
			//	pData->reward.push_back(reward);
			//}
			//DbHelper::freeResultSet(pRecordSet2);
		}

		m_LevelArray.push_back(pData);
	}

	DbHelper::freeResultSet(pRecordSet);

	return true;
}

bool GlobalData::initTrack()
{
	DbRecordSetPtr pRecordSet = new DbRecordSet();
	int cnt = DbHelper::getInstance()->getRecord(DBTABLE::TRACK::TABLE, pRecordSet);
	if (cnt <= 0)
	{
		DbHelper::freeResultSet(pRecordSet);
		CCLOG("GlobalData::initTrack : no record");
		return false;
	}

	for (unsigned i = 0; i < pRecordSet->size(); i++)
	{
		TTrackData* pData = new TTrackData;
		DbKVPtr pRecord = pRecordSet->at(i);

		pData->id = getValueToInt(pRecord, DBTABLE::TRACK::COL_ID);
		pData->name = getValue(pRecord, DBTABLE::TRACK::COL_NAME);
		pData->loop = getValueToInt(pRecord, DBTABLE::TRACK::COL_LOOP);
		pData->startPosX = getValueToFloat(pRecord, DBTABLE::TRACK::COL_START_POS_X);
		pData->startPosY = getValueToFloat(pRecord, DBTABLE::TRACK::COL_START_POS_Y);

		m_TrackArray.push_back(pData);
	}

	DbKV order;
	order.insert(DbKV::value_type(DBTABLE::TRACK_SEGMENT::COL_TRACK_ID, "asc"));
	order.insert(DbKV::value_type(DBTABLE::TRACK_SEGMENT::COL_SEQ, "asc"));
	DbRecordSetPtr pRecordSet2 = new DbRecordSet();
	int cnt2 = DbHelper::getInstance()->getRecord(DBTABLE::TRACK_SEGMENT::TABLE, pRecordSet2, NULL, &order);
	for (int j = 0; j < cnt2; j++)
	{
		DbKVPtr pRecord2 = pRecordSet2->at(j);

		TPos pos;
		int trackId = getValueToInt(pRecord2, DBTABLE::TRACK_SEGMENT::COL_TRACK_ID);
		pos.speed = getValueToFloat(pRecord2, DBTABLE::TRACK_SEGMENT::COL_SPEED);
		pos.x = getValueToInt(pRecord2, DBTABLE::TRACK_SEGMENT::COL_X);
		pos.y = getValueToFloat(pRecord2, DBTABLE::TRACK_SEGMENT::COL_Y);
		pos.path = getValue(pRecord2, DBTABLE::TRACK_SEGMENT::COL_TYPE);
		pos.param_1 = getValueToFloat(pRecord2, DBTABLE::TRACK_SEGMENT::COL_CTRL_X_1);
		pos.param_2 = getValueToFloat(pRecord2, DBTABLE::TRACK_SEGMENT::COL_CTRL_Y_1);
		pos.param_3 = getValueToFloat(pRecord2, DBTABLE::TRACK_SEGMENT::COL_CTRL_X_2);
		pos.param_4 = getValueToFloat(pRecord2, DBTABLE::TRACK_SEGMENT::COL_CTRL_Y_2);

		//TODO 这一段比较费时，最好要把m_TrackArray改成map
		for (unsigned x = 0; x < m_TrackArray.size(); x++)
		{
			if (m_TrackArray.at(x)->id == trackId)
			{
				m_TrackArray.at(x)->pos.push_back(pos);
			}
		}
	}
	DbHelper::freeResultSet(pRecordSet2);
	DbHelper::freeResultSet(pRecordSet);

	return true;
}

bool GlobalData::initReward()
{
	//reward
	DbRecordSetPtr pRecordSet = new DbRecordSet();
	int cnt = DbHelper::getInstance()->getRecord(DBTABLE::REWARD::TABLE, pRecordSet);
	if (cnt <= 0)
	{
		CCLOG("GlobalData::initReward : no record");
		return false;
	}

	for (unsigned i = 0; i < pRecordSet->size(); i++)
	{
		TRewardData* pData = new TRewardData;
		DbKVPtr pRecord = pRecordSet->at(i);

		pData->id = getValueToInt(pRecord, DBTABLE::REWARD::COL_ID);
		pData->name = getValue(pRecord, DBTABLE::REWARD::COL_NAME);
		pData->type = getValue(pRecord, DBTABLE::REWARD::COL_TYPE);
		pData->armatureName = getValue(pRecord, DBTABLE::REWARD::COL_ARMATURE_NAME);

		getAnimation(getValueToInt(pRecord, DBTABLE::REWARD::COL_STYLE_ANI_ID), pData->aniDura, pData->styleArray);
		pData->musicId= getValueToInt(pRecord, DBTABLE::REWARD::COL_MUSIC_ID);
		pData->prompt = getValue(pRecord, DBTABLE::REWARD::COL_PROMPT);

		pData->speed = getValueToFloat(pRecord, DBTABLE::REWARD::COL_SPEED);
		pData->maxSpeed = getValueToFloat(pRecord, DBTABLE::REWARD::COL_MAX_SPEED);
		pData->radius = getValueToFloat(pRecord, DBTABLE::REWARD::COL_RADIUS);
		pData->lifeTime = getValueToFloat(pRecord, DBTABLE::REWARD::COL_LIFE_TIME);

		pData->bodyCenterX = getValueToFloat(pRecord, DBTABLE::REWARD::COL_BODY_CENTER_X);
		pData->bodyCenterY = getValueToFloat(pRecord, DBTABLE::REWARD::COL_BODY_CENTER_Y);
		pData->bodySizeW = getValueToFloat(pRecord, DBTABLE::REWARD::COL_BODY_SIZE_W);
		pData->bodySizeH = getValueToFloat(pRecord, DBTABLE::REWARD::COL_BODY_SIZE_H);

		//属性
		{
			DbRecordSetPtr pRecordSet2 = new DbRecordSet();
			int cnt = DbHelper::getInstance()->getRecordByKey(DBTABLE::REWARD_ATTR::TABLE, pRecordSet2,
				DBTABLE::REWARD_ATTR::COL_REWARD_ID, pData->id);
			for (unsigned j = 0; j < pRecordSet2->size(); j++)
			{
				DbKVPtr pRecord2 = pRecordSet2->at(j);
				pData->paramMap.insert(std::map<std::string, float>::value_type(
					getValue(pRecord2, DBTABLE::REWARD_ATTR::COL_KEY),
					getValueToFloat(pRecord2, DBTABLE::REWARD_ATTR::COL_VALUE)));
			}
			DbHelper::freeResultSet(pRecordSet2);
		}

		m_RewardArray.push_back(pData);
	}

	DbHelper::freeResultSet(pRecordSet);

	//reward_group
	DbRecordSetPtr pRecordSet3 = new DbRecordSet();
	int cnt2 = DbHelper::getInstance()->getRecord(DBTABLE::REWARD_GROUP::TABLE,pRecordSet3);
	for (int k = 0; k < cnt2; k++)
	{
		DbKVPtr pRecord3 = pRecordSet3->at(k);
		TRewardGroupData* pRewardGroupData = new TRewardGroupData;
		int id = getValueToInt(pRecord3, DBTABLE::REWARD_GROUP::COL_GROUP_ID);
		pRewardGroupData->rewardId = getValueToInt(pRecord3, DBTABLE::REWARD_GROUP::COL_REWARD_ID);
		pRewardGroupData->cnt = getValueToInt(pRecord3, DBTABLE::REWARD_GROUP::COL_CNT);
		pRewardGroupData->probability = getValueToFloat(pRecord3, DBTABLE::REWARD_GROUP::COL_PROBABILITY);

		auto it = m_RewardGroupMap.find(id);
		if (it != m_RewardGroupMap.end())
		{
			it->second->push_back(pRewardGroupData);
		}
		else
		{
			std::vector<TRewardGroupData*>* pArray = new std::vector<TRewardGroupData*>();
			pArray->push_back(pRewardGroupData);
			m_RewardGroupMap.insert(std::map<int, std::vector<TRewardGroupData*>*>::value_type(id, pArray));
		}
	}
	DbHelper::freeResultSet(pRecordSet3);

	return true;
}

bool GlobalData::initArmature()
{
	DbRecordSetPtr pRecordSet = new DbRecordSet;
	int cnt = DbHelper::getInstance()->getRecord(DBTABLE::ARMATURE::TABLE, pRecordSet);
	if (cnt <= 0)
	{
		CCLOG("GlobalData::initArmature : no record");
		return false;
	}

	for (unsigned i = 0; i < pRecordSet->size(); i++)
	{
		TArmatureData* pData = new TArmatureData;
		DbKVPtr pRecord = pRecordSet->at(i);

		pData->id = getValueToInt(pRecord, DBTABLE::ARMATURE::COL_ID);
		pData->name = getValue(pRecord, DBTABLE::ARMATURE::COL_NAME);

		pData->defaultAction = getValue(pRecord, DBTABLE::ARMATURE::COL_DEFAULT_ACTION);

		m_ArmatureArray.push_back(pData);
	}

	DbHelper::freeResultSet(pRecordSet);

	return true;
}

bool GlobalData::initMusic()
{
	DbRecordSetPtr pRecordSet = new DbRecordSet();
	int cnt = DbHelper::getInstance()->getRecord(DBTABLE::MUSIC::TABLE, pRecordSet);
	if (cnt <= 0)
	{
		CCLOG("GlobalData::initMusic : no record");
		return false;
	}

	for (unsigned i = 0; i < pRecordSet->size(); i++)
	{
		TMusicData* pData = new TMusicData;
		DbKVPtr pRecord = pRecordSet->at(i);

		pData->id = getValueToInt(pRecord, DBTABLE::MUSIC::COL_ID);
		pData->filename = getValue(pRecord, DBTABLE::MUSIC::COL_FILENAME);
		pData->isEffect = getValueToInt(pRecord, DBTABLE::MUSIC::COL_IS_EFFECT);

		m_MusicArray.push_back(pData);
	}

	DbHelper::freeResultSet(pRecordSet);

	return true;
}

bool GlobalData::initAchievement()
{
	DbKV order;
	order.insert(DbKV::value_type(DBTABLE::ACHIEVEMENT::COL_ID, "asc"));

	DbRecordSetPtr pRecordSet = new DbRecordSet();
	int cnt = DbHelper::getInstance()->getRecord(DBTABLE::ACHIEVEMENT::TABLE, pRecordSet, NULL, &order);
	if (cnt <= 0)
	{
		DbHelper::freeResultSet(pRecordSet);
		CCLOG("GlobalData::initAchievement : no record");
		return false;
	}

	for (unsigned i = 0; i < pRecordSet->size(); i++)
	{
		TAchievementData* pData = new TAchievementData;
		DbKVPtr pRecord = pRecordSet->at(i);

		pData->id = getValueToInt(pRecord, DBTABLE::ACHIEVEMENT::COL_ID);
		pData->name = getValue(pRecord, DBTABLE::ACHIEVEMENT::COL_NAME);
		pData->remark = getValue(pRecord, DBTABLE::ACHIEVEMENT::COL_REMARK);
		pData->key = getValue(pRecord, DBTABLE::ACHIEVEMENT::COL_KEY);
		pData->value = getValueToInt(pRecord, DBTABLE::ACHIEVEMENT::COL_VALUE);
		pData->rewardName = getValue(pRecord, DBTABLE::ACHIEVEMENT::COL_REWARD_NAME);
		pData->rewardKey = getValue(pRecord, DBTABLE::ACHIEVEMENT::COL_REWARD_KEY);
		pData->rewardValue = getValueToInt(pRecord, DBTABLE::ACHIEVEMENT::COL_REWARD_VALUE);
		pData->isAchieve = 0;
		pData->isReceive = 0;

		m_AchievementMap.insert(std::map<int, TAchievementData*>::value_type(pData->id, pData));
	}

	DbHelper::freeResultSet(pRecordSet);

	return true;
}

bool GlobalData::initReinforce()
{
	{
		//主表
		DbKV order;
		order.insert(DbKV::value_type(DBTABLE::REINFORCE::COL_ID, "asc"));

		DbRecordSetPtr pRecordSet = new DbRecordSet();
		int cnt = DbHelper::getInstance()->getRecord(DBTABLE::REINFORCE::TABLE, pRecordSet, NULL, &order);
		if (cnt <= 0)
		{
			DbHelper::freeResultSet(pRecordSet);
			CCLOG("GlobalData::initReinforce : no record");
			return false;
		}

		for (unsigned i = 0; i < pRecordSet->size(); i++)
		{
			TReinforceData* pData = new TReinforceData;
			DbKVPtr pRecord = pRecordSet->at(i);

			pData->id = getValueToInt(pRecord, DBTABLE::REINFORCE::COL_ID);
			pData->name = getValue(pRecord, DBTABLE::REINFORCE::COL_NAME);
			pData->key = getValue(pRecord, DBTABLE::REINFORCE::COL_KEY);

			m_ReinforceArray.push_back(pData);
		}

		DbHelper::freeResultSet(pRecordSet);
	}

	//从表
	DbKV order2;
	order2.insert(DbKV::value_type(DBTABLE::REINFORCE_GRADE::COL_REINFORCE_ID, "asc"));
	order2.insert(DbKV::value_type(DBTABLE::REINFORCE_GRADE::COL_GRADE, "asc"));

	DbRecordSetPtr pRecordSet2 = new DbRecordSet();
	int cnt2 = DbHelper::getInstance()->getRecord(DBTABLE::REINFORCE_GRADE::TABLE, pRecordSet2, NULL, &order2);
	if (cnt2 <= 0)
	{
		DbHelper::freeResultSet(pRecordSet2);
		CCLOG("GlobalData::initReinforce2 : no record");
		return false;
	}

	for (unsigned i = 0; i < pRecordSet2->size(); i++)
	{
		TReinforceGradeData* pData = new TReinforceGradeData;
		DbKVPtr pRecord = pRecordSet2->at(i);

		int reinforceId = getValueToInt(pRecord, DBTABLE::REINFORCE_GRADE::COL_REINFORCE_ID);
		pData->grade = getValueToInt(pRecord, DBTABLE::REINFORCE_GRADE::COL_GRADE);
		pData->money = getValueToInt(pRecord, DBTABLE::REINFORCE_GRADE::COL_MONEY);
		pData->key = getValue(pRecord, DBTABLE::REINFORCE_GRADE::COL_KEY);
		pData->value = getValueToFloat(pRecord, DBTABLE::REINFORCE_GRADE::COL_VALUE);
		pData->remark = getValue(pRecord, DBTABLE::REINFORCE_GRADE::COL_REMARK);

		for (unsigned xx = 0; xx < m_ReinforceArray.size(); xx++)
		{
			if (m_ReinforceArray.at(xx)->id == reinforceId)
			{
				m_ReinforceArray.at(xx)->gradeArray.push_back(pData);
			}
		}
	}

	DbHelper::freeResultSet(pRecordSet2);

	return true;
}

bool GlobalData::initGoods()
{
	BillingType billingType = Carrier::getBillingType();
	switch (billingType)
	{
	case BillingTypeChinaMobile:
	case BillingTypeChinaMobileCh:
		billingType = BillingTypeChinaMobile;
		break;
	case BillingTypeChinaTelcom:
	case BillingTypeChinaTelcomCh:
		billingType = BillingTypeChinaTelcom;
		break;
	case BillingTypeChinaUnicom:
	case BillingTypeChinaUnicomCh:
		billingType = BillingTypeChinaUnicom;
		break;
	//case BillingTypeXiaoMi:
	//	billingType = BillingTypeXiaoMi;
	//	break;
	case BillingTypeYijie:
		billingType = BillingTypeYijie;
		break;
	case BillingTypeThreeNet:
		billingType = (BillingType)Carrier::getCarrierType();
		if (BillingTypeChinaMobile != billingType
			&& BillingTypeChinaTelcom != billingType
			&& BillingTypeChinaUnicom != billingType
			&& BillingTypeSimulate != billingType)
		{
			billingType = BillingTypeChinaMobile;
		}
		break;
	case BillingTypeSimulate:
		billingType = BillingTypeSimulate;
		break;
	default:
		break;
	}

	//查询goods_item表
	std::map<int, std::map<std::string, int>*> goodsItemMap;
	{
		DbKV whereParam;
		char szBillType[20] = { 0 };
		snprintf(szBillType, 19, "%d", billingType);
		whereParam.insert(std::map < std::string, std::string >::value_type(
			DBTABLE::GOODS_ITEM::COL_CARRIER, szBillType));

		DbRecordSetPtr pRecordSet2 = new DbRecordSet();

		int cnt = DbHelper::getInstance()->getRecord(DBTABLE::GOODS_ITEM::TABLE, pRecordSet2, &whereParam);
		if (cnt <= 0)
		{
			DbHelper::freeResultSet(pRecordSet2);
			DEBUG_LOG("GlobalData::initGoods : no record. BillingType = %d", billingType);
			return false;
		}

		for (unsigned i = 0; i < pRecordSet2->size(); i++)
		{
			TGoodsItemData* pData = new TGoodsItemData;
			DbKVPtr pRecord = pRecordSet2->at(i);

			pData->goodsId = getValueToInt(pRecord, DBTABLE::GOODS_ITEM::COL_GOODS_ID);
			pData->key = getValue(pRecord, DBTABLE::GOODS_ITEM::COL_KEY);
			pData->value = getValueToInt(pRecord, DBTABLE::GOODS_ITEM::COL_VALUE);

			auto it = goodsItemMap.find(pData->goodsId);
			if (it == goodsItemMap.end())
			{
				auto p = new std::map<std::string, int>();
				p->insert(std::map<std::string, int>::value_type(pData->key, pData->value));
				goodsItemMap.insert(std::map<int, std::map<std::string, int>*>::value_type(pData->goodsId, p));
			}
			else
			{
				it->second->insert(std::map<std::string , int>::value_type(pData->key, pData->value));
			}
		}
		DbHelper::freeResultSet(pRecordSet2);
	}


	//查询goods表
	DbKV whereParam;
	char szBillType[20] = { 0 };
	snprintf(szBillType, 19, "%d", billingType);
	whereParam.insert(std::map < std::string, std::string >::value_type(
		DBTABLE::GOODS::COL_CARRIER, szBillType));

	DbRecordSetPtr pRecordSet = new DbRecordSet();

	int cnt = DbHelper::getInstance()->getRecord(DBTABLE::GOODS::TABLE, pRecordSet, &whereParam);
	if (cnt <= 0)
	{
		DbHelper::freeResultSet(pRecordSet);
		DEBUG_LOG("GlobalData::initGoods : no record. BillingType = %d", billingType);
		return false;
	}

	for (unsigned i = 0; i < pRecordSet->size(); i++)
	{
		TGoodsData* pData = new TGoodsData;
		DbKVPtr pRecord = pRecordSet->at(i);

		pData->id = getValueToInt(pRecord, DBTABLE::GOODS::COL_ID);
		pData->goods = getValue(pRecord, DBTABLE::GOODS::COL_GOODS);
		pData->orignPrice = getValueToInt(pRecord, DBTABLE::GOODS::COL_ORIGN_PRICE);
		pData->price = getValueToInt(pRecord, DBTABLE::GOODS::COL_PRICE);
		pData->currency = getValue(pRecord, DBTABLE::GOODS::COL_CURRENCY);
		pData->name = getValue(pRecord, DBTABLE::GOODS::COL_NAME);
		pData->billIndex = getValue(pRecord, DBTABLE::GOODS::COL_BILL_INDEX);
		pData->desc = getValue(pRecord, DBTABLE::GOODS::COL_DESC);

		auto it = goodsItemMap.find(pData->id);
		if (it != goodsItemMap.end())
		{
			pData->itemMap = it->second;
		}
		else
		{
			pData->itemMap = nullptr;
		}

		m_GoodsMap.insert(std::map<std::string, TGoodsData*>::value_type(pData->goods, pData));
	}

	DbHelper::freeResultSet(pRecordSet);

	return true;
}

bool GlobalData::initDialog()
{
	DbRecordSetPtr pRecordSet = new DbRecordSet();
	int cnt = DbHelper::getInstance()->getRecord(DBTABLE::DIALOG::TABLE, pRecordSet);
	if (cnt <= 0)
	{
		CCLOG("GlobalData::initDialog : no record");
		return false;
	}

	for (unsigned i = 0; i < pRecordSet->size(); i++)
	{
		TDialogData* pData = new TDialogData;
		DbKVPtr pRecord = pRecordSet->at(i);

		pData->id = getValueToInt(pRecord, DBTABLE::DIALOG::COL_ID);
		pData->type = getValueToInt(pRecord, DBTABLE::DIALOG::COL_TYPE);
		pData->name = getValue(pRecord, DBTABLE::DIALOG::COL_NAME);
		pData->pic = getValue(pRecord, DBTABLE::DIALOG::COL_PIC);
		pData->content = getValue(pRecord, DBTABLE::DIALOG::COL_CONTENT);

		m_dialogMap.insert(std::map<int, TDialogData*>::value_type(pData->id, pData));
	}

	DbHelper::freeResultSet(pRecordSet);

	return true;
}

bool GlobalData::initDayGift()
{
	DbKV order;
	order.insert(DbKV::value_type(DBTABLE::DAY_GIFT::COL_DAY, "asc"));
	order.insert(DbKV::value_type(DBTABLE::DAY_GIFT::COL_SEQ, "asc"));

	DbRecordSetPtr pRecordSet = new DbRecordSet();
	int cnt = DbHelper::getInstance()->getRecord(DBTABLE::DAY_GIFT::TABLE, pRecordSet, NULL, &order);
	if (cnt <= 0)
	{
		CCLOG("GlobalData::initDayGift : no record");
		return false;
	}

	for (unsigned i = 0; i < pRecordSet->size(); i++)
	{
		TDayGiftData* pData = new TDayGiftData;
		DbKVPtr pRecord = pRecordSet->at(i);

		pData->day = getValueToInt(pRecord, DBTABLE::DAY_GIFT::COL_DAY);
		pData->seq = getValueToInt(pRecord, DBTABLE::DAY_GIFT::COL_SEQ);
		pData->gift = getValue(pRecord, DBTABLE::DAY_GIFT::COL_GIFT);
		pData->giftName = getValue(pRecord, DBTABLE::DAY_GIFT::COL_GIFT_NAME);
		pData->count = getValueToInt(pRecord, DBTABLE::DAY_GIFT::COL_COUNT);

		m_dayGiftArray.push_back(pData);
	}

	DbHelper::freeResultSet(pRecordSet);

	return true;
}

bool GlobalData::initDifficulty()
{
	DbKV order;
	order.insert(DbKV::value_type(DBTABLE::DIFFICULTY::COL_ID, "asc"));

	DbRecordSetPtr pRecordSet = new DbRecordSet();
	int cnt = DbHelper::getInstance()->getRecord(DBTABLE::DIFFICULTY::TABLE, pRecordSet, NULL, &order);
	if (cnt <= 0)
	{
		CCLOG("GlobalData::initDifficulty : no record");
		return false;
	}

	for (unsigned i = 0; i < pRecordSet->size(); i++)
	{
		TDifficultyData* pData = new TDifficultyData;
		DbKVPtr pRecord = pRecordSet->at(i);

		pData->id = getValueToInt(pRecord, DBTABLE::DIFFICULTY::COL_ID);
		pData->type = getValueToInt(pRecord, DBTABLE::DIFFICULTY::COL_TYPE);
		pData->name = getValue(pRecord, DBTABLE::DIFFICULTY::COL_NAME);
		pData->money = getValueToFloat(pRecord, DBTABLE::DIFFICULTY::COL_MONEY);
		pData->bullet_speed = getValueToFloat(pRecord, DBTABLE::DIFFICULTY::COL_BULLET_SPEED);
		pData->attack = getValueToFloat(pRecord, DBTABLE::DIFFICULTY::COL_ATTACK);
		pData->hp = getValueToFloat(pRecord, DBTABLE::DIFFICULTY::COL_HP);
		pData->reward_prob = getValueToFloat(pRecord, DBTABLE::DIFFICULTY::COL_REWARD_PROB);
		pData->enemy_grade = getValueToInt(pRecord, DBTABLE::DIFFICULTY::COL_ENEMY_GRADE);

		m_difficultyMap.insert(std::map<int, TDifficultyData*>::value_type(pData->id, pData));
	}

	DbHelper::freeResultSet(pRecordSet);

	return true;
}

bool GlobalData::initEquipment()
{
	DbKV order;
	order.insert(DbKV::value_type(DBTABLE::EQUIPMENT::COL_ID, "asc"));

	DbRecordSetPtr pRecordSet = new DbRecordSet();
	int cnt = DbHelper::getInstance()->getRecord(DBTABLE::EQUIPMENT::TABLE, pRecordSet, NULL, &order);
	if (cnt <= 0)
	{
		CCLOG("GlobalData::initEquipment : no record");
		return false;
	}

	for (unsigned i = 0; i < pRecordSet->size(); i++)
	{
		TEquipmentData* pData = new TEquipmentData;
		DbKVPtr pRecord = pRecordSet->at(i);

		pData->id = getValueToInt(pRecord, DBTABLE::EQUIPMENT::COL_ID);
		pData->name = getValue(pRecord, DBTABLE::EQUIPMENT::COL_NAME);
		pData->type = (EquipType)getValueToInt(pRecord, DBTABLE::EQUIPMENT::COL_TYPE);
		pData->reinforce_id = getValueToInt(pRecord, DBTABLE::EQUIPMENT::COL_REINFORCE_ID);
		pData->value = getValueToInt(pRecord, DBTABLE::EQUIPMENT::COL_VALUE);
		pData->price = getValueToInt(pRecord, DBTABLE::EQUIPMENT::COL_PRICE);
		pData->currency = getValue(pRecord, DBTABLE::EQUIPMENT::COL_CURRENCY);
		pData->piece = getValueToInt(pRecord, DBTABLE::EQUIPMENT::COL_PIECE);
		pData->probabilty = getValueToFloat(pRecord, DBTABLE::EQUIPMENT::COL_PROBABILITY);
		pData->remark = getValue(pRecord, DBTABLE::EQUIPMENT::COL_REMARK);
		pData->pic = getValue(pRecord, DBTABLE::EQUIPMENT::COL_PIC);
		pData->own = 0;
		pData->equip = 0;
		pData->grade = 0;

		auto it = m_equipmentMap.find(pData->type);
		if (it == m_equipmentMap.end())
		{
			auto array = new std::vector<TEquipmentData*>;
			array->push_back(pData);
			m_equipmentMap.insert(std::map<int, std::vector<TEquipmentData*>*>::value_type(pData->type, array));
		}
		else
		{
			it->second->push_back(pData);
		}

		m_equipmentIdMap.insert(std::map<int, TEquipmentData*>::value_type(pData->id, pData));
	}

	DbHelper::freeResultSet(pRecordSet);

	return true;
}
