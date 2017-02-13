#include "GameData.h"
#include "GlobalData.h"
#include "Achievement.h"
#include "Equipment.h"
#include "ModeBase.h"
#include "Common/Macro.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

GameData* GameData::m_pInstance = NULL;

const char* const CFG_FILE_NAME_JSON = "thorqq.json";

GameData::GameData()
{
	reset();
}

GameData::~GameData()
{
}

void GameData::reset()
{
	m_pFile = NULL;
	m_iMap.clear();
	m_fMap.clear();
	m_strMap.clear();

	ListenerMap::iterator it = m_listenerMap.begin();
	for (; it != m_listenerMap.end(); it++)
	{
		delete it->second;
	}
	m_listenerMap.clear();
}

bool GameData::loadData()
{
	//DEBUG_LOG("Load data from %s", UserDefault::getInstance()->getXMLFilePath().c_str());

	m_gameDataDefine.push_back(TGameData(GAMEDATA::SOUND, GlobalData::getInstance()->isSound()));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::FINGER_PRINT_FLAG, 1));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::FINGER_PRINT, ""));

	m_gameDataDefine.push_back(TGameData(GAMEDATA::USER_ID, ""));

	m_gameDataDefine.push_back(TGameData(GAMEDATA::MODE, ModeBase::ModeLevel));

	m_gameDataDefine.push_back(TGameData(GAMEDATA::PLAYER, GlobalData::getInstance()->getPlayerId()));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::LEVEL, 1));
	//m_gameDataDefine.push_back(TGameData(GAMEDATA::MAX_LEVEL, 1));
	//m_gameDataDefine.push_back(TGameData(GAMEDATA::FINISH_LEVEL, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::LEVEL_MAX, ""));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::LEVEL_FINISH, ""));

	m_gameDataDefine.push_back(TGameData(GAMEDATA::MAX_KILL_ENEMY_RATIO, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::MAX_GET_REWARD_RATIO, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::CONTINUOUS_KILL_ENEMY_CNT, 0));

	m_gameDataDefine.push_back(TGameData(GAMEDATA::RAPID_MODE_MAX_DISTANCE, 0));

	m_gameDataDefine.push_back(TGameData(GAMEDATA::DIFFICULTY_LEVEL, 1));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::DIFFICULTY_ENDLESS, 101));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::DIFFICULTY_RAPID, 201));

	m_gameDataDefine.push_back(TGameData(GAMEDATA::ENDLESS_ENEMY_HP_RATIO, 0));

	m_gameDataDefine.push_back(TGameData(GAMEDATA::LEVEL_STAR_1, ""));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::LEVEL_STAR_2, ""));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::LEVEL_STAR_3, ""));

	m_gameDataDefine.push_back(TGameData(GAMEDATA::ENDLESS_BOSS, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::ENDLESS_BOSS_LAST, 0));

	m_gameDataDefine.push_back(TGameData(GAMEDATA::RAPID_DISTANCE, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::RAPID_DISTANCE_BEST, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::RAPID_RANK, 0.0f));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::RAPID_RANK_BEST, 0.0f));

	m_gameDataDefine.push_back(TGameData(GAMEDATA::SCORE, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::MONEY, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::ENEMY, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::LEVEL_ENEMY_LEFT, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::LIFE, GlobalData::getInstance()->getLife()));

	m_gameDataDefine.push_back(TGameData(GAMEDATA::DIE, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::REVIVE, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::USE_KILLER, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::USE_SHIELD, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::USE_MAX_BULLLET, 0));

	m_gameDataDefine.push_back(TGameData(GAMEDATA::NEW_GIFT, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::MID_GIFT, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::BIG_GIFT, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::HUG_GIFT, 0));

	m_gameDataDefine.push_back(TGameData(GAMEDATA::NEW_GIFT_RECOMMEND, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::MID_GIFT_RECOMMEND, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::BIG_GIFT_RECOMMEND, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::HUG_GIFT_RECOMMEND, 0));

	m_gameDataDefine.push_back(TGameData(GAMEDATA::REINFORCE_FIRST, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::REINFORCE_ALL, 0));

	m_gameDataDefine.push_back(TGameData(GAMEDATA::REINFORCE_GLOBAL_DIAMOND, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::REINFORCE_GLOBAL_RAMPAGE, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::REINFORCE_GLOBAL_KILLER, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::REINFORCE_GLOBAL_SHIELD, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::REINFORCE_GLOBAL_REPAIR, 0));

	m_gameDataDefine.push_back(TGameData(GAMEDATA::REINFORCE_VALUE_DIAMOND_RATIO, 0.0f));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::REINFORCE_VALUE_RAMPAGE_DURA, 0.0f));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::REINFORCE_VALUE_KILLER_COLD, 0.0f));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::REINFORCE_VALUE_SHIELD_INIT, 0.0f));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::REINFORCE_VALUE_REPAIR_RATIO, 0.0f));

	m_gameDataDefine.push_back(TGameData(GAMEDATA::REINFORCE_EQUIP, ""));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::PLAYER_EQUIP, ""));

	m_gameDataDefine.push_back(TGameData(GAMEDATA::EQUIP_AIRCRAFT, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::EQUIP_ARMATURE, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::EQUIP_MISSILE, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::EQUIP_WINGMAN, 0));

	m_gameDataDefine.push_back(TGameData(GAMEDATA::KILLER, GlobalData::getInstance()->getKiller()));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::SHIELD, GlobalData::getInstance()->getShield()));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::SKILL_CD_TIME, GlobalData::getInstance()->getCdTime()));

	m_gameDataDefine.push_back(TGameData(GAMEDATA::ACHIEVEMENT, ""));

	m_gameDataDefine.push_back(TGameData(GAMEDATA::USE_AIRCRAFT_4, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::USE_AIRCRAFT_5, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::USE_AIRCRAFT_6, 0));

	m_gameDataDefine.push_back(TGameData(GAMEDATA::DAY_REWARD_LOGIN_DAYS, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::DAY_REWARD_LAST_LOGIN, 0));

	m_gameDataDefine.push_back(TGameData(GAMEDATA::VIP_START_TIME, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::VIP_DAYS, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::VIP_FLAG, 0));

	m_gameDataDefine.push_back(TGameData(GAMEDATA::VIP_EXPIRE_REMIND, 0));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::VIP_ORDER_REMIND, 0));

	m_gameDataDefine.push_back(TGameData(GAMEDATA::VIP_FREE_REFLIE, ""));
	m_gameDataDefine.push_back(TGameData(GAMEDATA::VIP_RELIFE_ATTR_ADJUST, 1.0f));	

	m_gameDataDefine.push_back(TGameData(GAMEDATA::GUIDE, ""));

	m_gameDataDefine.push_back(TGameData(GAMEDATA::TIME, -1));

	m_gameDataDefine.push_back(TGameData(GAMEDATA::BUY_RELIFE, -1));

	for (unsigned i = 0; i < m_gameDataDefine.size(); i++)
	{
		TGameData& data = m_gameDataDefine.at(i);
		if (EGameDataType::DataType_Int == data.type)
		{
			m_iMap.insert(std::map<std::string, int>::value_type(data.key, data.iDefaultValue));
		}
		else if (EGameDataType::DataType_Float == data.type)
		{
			m_fMap.insert(std::map<std::string, float>::value_type(data.key, data.fDefaultValue));
		}
		else if (EGameDataType::DataType_String == data.type)
		{
			m_strMap.insert(std::map<std::string, std::string>::value_type(data.key, data.strDefaultValue));
		}
	}

	if (!this->loadFile())
	{
		DEBUG_LOG("Load data from file failed");
		return false;
	}

	//强制修改声音设置
	setValue(GAMEDATA::SOUND, GlobalData::getInstance()->isSound());

	//强制修改关卡数据
	int testLevel = GlobalData::getInstance()->getTestLevelId();
	if (testLevel > 0)
	{
		this->setValue(GAMEDATA::LEVEL, testLevel);
	}

	//强制修改新手引导标识
	if (GlobalData::getInstance()->isTutorialTest())
	{
		setValue(GAMEDATA::GUIDE, "");
		m_guideList.clear();

		//setValue(GAMEDATA::TUTORIAL_SHIELD, 0);
		//setValue(GAMEDATA::TUTORIAL_KILLER, 0);
		//setValue(GAMEDATA::TUTORIAL_PLANE, 0);
		//setValue(GAMEDATA::TUTORIAL_REWARD, 0);
		//setValue(GAMEDATA::TUTORIAL_REINFORCE, 0);
		//setValue(GAMEDATA::TUTORIAL_ACHIEVEMENT, 0);
	}

	//注册监听器
	m_listenerMap.insert(ListenerMap::value_type(GAMEDATA::LEVEL_MONEY, new ListenerArray));
	m_listenerMap.insert(ListenerMap::value_type(GAMEDATA::LEVEL_SCORE, new ListenerArray));
	m_listenerMap.insert(ListenerMap::value_type(GAMEDATA::LEVEL_ENEMY, new ListenerArray));
	m_listenerMap.insert(ListenerMap::value_type(GAMEDATA::LEVEL_LIFE, new ListenerArray));
	m_listenerMap.insert(ListenerMap::value_type(GAMEDATA::LEVEL_REWARD, new ListenerArray));
	m_listenerMap.insert(ListenerMap::value_type(GAMEDATA::LEVEL_KILLER, new ListenerArray));

	m_listenerMap.insert(ListenerMap::value_type(GAMEDATA::MONEY, new ListenerArray));
	m_listenerMap.insert(ListenerMap::value_type(GAMEDATA::SCORE, new ListenerArray));
	m_listenerMap.insert(ListenerMap::value_type(GAMEDATA::LIFE, new ListenerArray));

	m_listenerMap.insert(ListenerMap::value_type(GAMEDATA::KILLER, new ListenerArray));
	m_listenerMap.insert(ListenerMap::value_type(GAMEDATA::SHIELD, new ListenerArray));

	//载入成就数据/装备强化数据/战机装备数据
	if (!loadAchievement()
		|| !loadReinforceEquip()
		|| !loadPlayerEquip()
		|| !loadLevelStar()
		|| !loadLevelMax()
		|| !loadLevelFinish()
		|| !loadGuide()
		|| !loadVipFreeRelifeCnt()
		)
	{
		return false;
	}

	//强制初始化
	GameData::getInstance()->setValue(GAMEDATA::ENDLESS_BOSS, 0);
	GameData::getInstance()->setValue(GAMEDATA::ENDLESS_BOSS_LAST, 0);

	GameData::getInstance()->setValue(GAMEDATA::RAPID_DISTANCE, 0);
	GameData::getInstance()->setValue(GAMEDATA::RAPID_RANK, 0.0f);

	GameData::getInstance()->setValue(GAMEDATA::LEVEL_ENEMY_LEFT, 0);
	GameData::getInstance()->setValue(GAMEDATA::MODE, ModeBase::ModeLevel);
	GameData::getInstance()->setValue(GAMEDATA::TIME, -1);
	GameData::getInstance()->setValue(GAMEDATA::CONTINUOUS_KILL_ENEMY_CNT, 0);
	GameData::getInstance()->setValue(GAMEDATA::VIP_FLAG, 0);
	GameData::getInstance()->setValue(GAMEDATA::VIP_EXPIRE_REMIND, 0);
	GameData::getInstance()->setValue(GAMEDATA::VIP_RELIFE_ATTR_ADJUST, 1.0f);

	GameData::getInstance()->setValue(GAMEDATA::BUY_RELIFE, 0);

	return true;
}

bool GameData::saveData()
{
	saveAchievement();
	saveReinforceEquip();
	savePlayerEquip();
	saveLevelStar();
	saveLevelMax();
	saveLevelFinish();
	saveGuide();
	saveVipFreeRelifeCnt();

	this->saveFile();
	return true;
}

bool GameData::saveFile()
{
	//object to json
	std::string buf;
	this->saveJson(buf);

	buf = Utils::formatJson(buf);

	std::string destPath = CCFileUtils::getInstance()->getWritablePath();

	destPath += CFG_FILE_NAME_JSON;

	FILE *pFp = fopen(destPath.c_str(), "wb");
	fwrite(buf.c_str(), sizeof(char), buf.length(), pFp);
	fclose(pFp);

	return true;
}

bool GameData::loadFile()
{
	std::string destPath = CCFileUtils::getInstance()->getWritablePath();

	destPath += CFG_FILE_NAME_JSON;

	std::string content = FileUtils::getInstance()->getStringFromFile(destPath);
	if (content.length() <= 0)
	{
		return true;
	}

	if (!loadJson(content.c_str()))
	{
		DEBUG_LOG("loadJson ERROR");
		return false;
	}
	
	return true;
}

void GameData::regListener(IListener* listener, const std::string& type)
{
	m_listenerMap.at(type)->push_back(listener);
}

void GameData::unRegListener(IListener* listener, const std::string& type)
{
	auto it0 = m_listenerMap.find(type);
	if (it0 == m_listenerMap.end())
	{
		return;
	}

	ListenerArrayPtr pArray = it0->second;
	ListenerArray::iterator it = pArray->begin();

	for (; it != pArray->end(); it++)
	{
		if (*it == listener)
		{
			pArray->erase(it);
			break;
		}
	}
}

void GameData::notifyListener(const std::string& type, int value)
{
	auto it0 = m_listenerMap.find(type);
	if (it0 == m_listenerMap.end())
	{
		return;
	}

	ListenerArrayPtr pArray = it0->second;
	ListenerArray::iterator it = pArray->begin();

	for (; it != pArray->end(); it++)
	{
		(*it)->updateValue(type, value);
	}
}

int GameData::getValueToInt(const char* const key, int defaultVallue)
{
	std::map<std::string, int>::iterator it = m_iMap.find(key);

	if (it != m_iMap.end())
	{
		return it->second;
	}
	else
	{
		return defaultVallue;
	}
}

const std::string& GameData::getValue(const char* const key, const std::string& defaultVallue)
{
	std::map<std::string, std::string>::iterator it = m_strMap.find(key);

	if (it != m_strMap.end())
	{
		return it->second;
	}
	else
	{
		return defaultVallue;
	}
}

float GameData::getValueToFloat(const char* const key, float defaultVallue)
{
	std::map<std::string, float>::iterator it = m_fMap.find(key);

	if (it != m_fMap.end())
	{
		return it->second;
	}
	else
	{
		return defaultVallue;
	}
}

int GameData::incValue(const char* const key, int value)
{
	std::map<std::string, int>::iterator it = m_iMap.find(key);
	int result = 0;
	if (it != m_iMap.end())
	{
		it->second += value;
		result = it->second;
	}
	else
	{
		m_iMap.insert(std::map<std::string, int>::value_type(key, value));
		result = value;
	}

	this->notifyListener(key, result);

	return result;
}

float GameData::incValue(const char* const key, float value)
{
	std::map<std::string, float>::iterator it = m_fMap.find(key);
	float result = 0;
	if (it != m_fMap.end())
	{
		it->second += value;
		result = it->second;
	}
	else
	{
		m_fMap.insert(std::map<std::string, float>::value_type(key, value));
		result = value;
	}

	return result;
}

void GameData::setValue(const char* const key, int value)
{
	std::map<std::string, int>::iterator it = m_iMap.find(key);
	if (it != m_iMap.end())
	{
		it->second = value;
	}
	else
	{
		m_iMap.insert(std::map<std::string, int>::value_type(key, value));
	}

	this->notifyListener(key, value);
}

void GameData::setValue(const char* const key, float value)
{
	std::map<std::string, float>::iterator it = m_fMap.find(key);
	if (it != m_fMap.end())
	{
		it->second = value;
	}
	else
	{
		m_fMap.insert(std::map<std::string, float>::value_type(key, value));
	}
}

void GameData::setValue(const char* const key, const std::string& value)
{
	std::map<std::string, std::string>::iterator it = m_strMap.find(key);
	if (it != m_strMap.end())
	{
		it->second = value;
	}
	else
	{
		m_strMap.insert(std::map<std::string, std::string>::value_type(key, value));
	}
}

void GameData::setAchieveData(int id, int isAchieve, int isReceive)
{
	GlobalData::getInstance()->getAllAchievementData();
	const std::map<int, TAchievementData*>& data = GlobalData::getInstance()->getAllAchievementData();

	auto it = data.find(id);
	if (it != data.end())
	{
		it->second->isAchieve = isAchieve;
		it->second->isReceive = isReceive;
	}
}

bool GameData::saveAchievement()
{
	GlobalData::getInstance()->getAllAchievementData();
	const std::map<int, TAchievementData*>& data = GlobalData::getInstance()->getAllAchievementData();

	std::string value;
	char s1[10] = { 0 };
	char s2[10] = { 0 };
	char s3[10] = { 0 };
	for (auto it : data)
	{
		snprintf(s1, 9, "%d", it.second->id);
		snprintf(s2, 9, "%d", it.second->isAchieve);
		snprintf(s3, 9, "%d", it.second->isReceive);
		value.append(s1).append("|").append(s2).append("|").append(s3).append(";");
	}

	this->setValue(GAMEDATA::ACHIEVEMENT, value);

	return true;
}

bool GameData::loadAchievement()
{
	GlobalData::getInstance()->getAllAchievementData();
	const std::map<int, TAchievementData*>& data = GlobalData::getInstance()->getAllAchievementData();

	std::vector<std::string> result;

	Utils::splite(this->getValue(GAMEDATA::ACHIEVEMENT), ";", result);

	for (auto str : result)
	{
		std::vector<std::string> achieve;
		int cnt = Utils::splite(str, "|", achieve);
		if (cnt != 3)
		{
			continue;
		}

		int id = Utils::str2int(achieve.at(0));
		int isAchieve = Utils::str2int(achieve.at(1));
		int isReceive = Utils::str2int(achieve.at(2));

		this->setAchieveData(id, isAchieve, isReceive);
	}

	return true;
}

bool GameData::saveReinforceEquip()
{
	char buf[101] = { 0 };
	std::string result;
	const std::map<int, std::vector<TEquipmentData*>*>& map = GlobalData::getInstance()->getAllEquipment();
	for (auto array : map)
	{
		for (auto data : *array.second)
		{
			snprintf(buf, 100, "%d|%d|%d|%d|%d;", data->id, data->own, data->equip, data->grade, data->pick);
			result.append(buf);
		}
	}

	this->setValue(GAMEDATA::REINFORCE_EQUIP, result);

	return true;
}

bool GameData::loadReinforceEquip()
{
	const std::map<int, TEquipmentData*>& map = GlobalData::getInstance()->getAllEquipmentId();

	std::vector<std::string> result;

	Utils::splite(this->getValue(GAMEDATA::REINFORCE_EQUIP), ";", result);

	for (auto str : result)
	{
		std::vector<std::string> equipment;
		int cnt = Utils::splite(str, "|", equipment);
		if (cnt < 2)
		{
			continue;
		}

		int id = Utils::str2int(equipment.at(0));
		--cnt;

		if (cnt > 0)
		{
			map.at(id)->own = Utils::str2int(equipment.at(1));
			--cnt;
		}

		if (cnt > 0)
		{
			map.at(id)->equip = Utils::str2int(equipment.at(2));//除战机外，其他装备保存在PlayerEquip中
			--cnt;
		}

		if (cnt > 0)
		{
			map.at(id)->grade = Utils::str2int(equipment.at(3));
			--cnt;
		}

		if (cnt > 0)
		{
			map.at(id)->pick = Utils::str2int(equipment.at(4));
			--cnt;
		}

	}

	if (result.size() == 0)
	{
		// 默认开始飞机1
		map.at(EquipmentConstant::DefaultPlayerId)->own = 1;
		map.at(EquipmentConstant::DefaultPlayerId)->equip = 1;
		map.at(EquipmentConstant::DefaultPlayerId)->grade = 0;
		map.at(EquipmentConstant::DefaultPlayerId)->pick = 0;
	}

	return true;
}

bool GameData::savePlayerEquip()
{
	char buf[101] = { 0 };
	int len = 0;
	std::string result;
	const std::map<int, std::vector<const TEquipmentData*>*>& map = PlayerEquip::getInstance()->getAllPlayerEquip();
	for (auto m : map)
	{
		for (auto data : *m.second)
		{
			len += snprintf(buf + len, 100, "%d|", data->id);
		}
		len += snprintf(buf + len, 100, "%d;", m.first);
	}

	this->setValue(GAMEDATA::PLAYER_EQUIP, buf);

	return true;
}

bool GameData::loadPlayerEquip()
{
	std::vector<std::string> result;

	Utils::splite(this->getValue(GAMEDATA::PLAYER_EQUIP), ";", result);

	if (result.size() == 0)
	{
		result.push_back("1|1");
	}

	for (auto str : result)
	{
		std::vector<std::string> equipment;
		int cnt = Utils::splite(str, "|", equipment);
		//if (cnt < 1)
		//{
		//	continue;
		//}

		int playerId = Utils::str2int(equipment.back());
		equipment.pop_back();

		if (cnt > 1)
		{
			for (auto e : equipment)
			{
				int equipId = Utils::str2int(e);
				const TEquipmentData* pEquip = GlobalData::getInstance()->getEquipment(equipId);
				PlayerEquip::getInstance()->addEquip(playerId, pEquip);
			}
		}
		else
		{
			const TEquipmentData* pEquip = GlobalData::getInstance()->getEquipment(playerId);
			PlayerEquip::getInstance()->addEquip(playerId, pEquip);
		}

	}

	auto xx = PlayerEquip::getInstance()->getEquip(1, EquipTypeAircraft);
	if (!xx)
	{
		PlayerEquip::getInstance()->addEquip(1, GlobalData::getInstance()->getEquipment(1));
	}

	return true;
}

bool GameData::saveLevelStar()
{
	{
		char buf[101] = { 0 };
		std::string result;
		const std::vector<int>& array = m_levelStar.at(1);
		for (auto it : array)
		{
			snprintf(buf, 100, "%d;", it);
			result.append(buf);
		}
		GameData::getInstance()->setValue(GAMEDATA::LEVEL_STAR_1, result.c_str());
	}

	{
		char buf[101] = { 0 };
		std::string result;
		const std::vector<int>& array = m_levelStar.at(2);
		for (auto it : array)
		{
			snprintf(buf, 100, "%d;", it);
			result.append(buf);
		}
		GameData::getInstance()->setValue(GAMEDATA::LEVEL_STAR_2, result.c_str());
	}

	{
		char buf[101] = { 0 };
		std::string result;
		const std::vector<int>& array = m_levelStar.at(3);
		for (auto it : array)
		{
			snprintf(buf, 100, "%d;", it);
			result.append(buf);
		}
		GameData::getInstance()->setValue(GAMEDATA::LEVEL_STAR_3, result.c_str());
	}

	return true;
}

bool GameData::loadLevelStar()
{
	if (m_levelStar.size() != 3)
	{
		m_levelStar.clear();
		std::vector<int> a;
		std::vector<int> b;
		std::vector<int> c;
		m_levelStar.insert(std::map<int, std::vector<int>>::value_type(1, a));
		m_levelStar.insert(std::map<int, std::vector<int>>::value_type(2, b));
		m_levelStar.insert(std::map<int, std::vector<int>>::value_type(3, c));
	}

	{
		std::vector<std::string> result;
		std::vector<int>& levelStar = m_levelStar.at(1);
		Utils::splite(this->getValue(GAMEDATA::LEVEL_STAR_1), ";", result);
		for (auto it : result)
		{
			levelStar.push_back(Utils::str2int(it));
		}
		if (levelStar.size() != CONSTANT::TOTAL_MISSION)
		{
			levelStar.clear();
			for (int i = 0; i < CONSTANT::TOTAL_MISSION; i++)
			{
				levelStar.push_back(0);
			}
		}
	}

	{
		std::vector<std::string> result;
		std::vector<int>& levelStar = m_levelStar.at(2);
		Utils::splite(this->getValue(GAMEDATA::LEVEL_STAR_2), ";", result);
		for (auto it : result)
		{
			levelStar.push_back(Utils::str2int(it));
		}
		if (levelStar.size() != CONSTANT::TOTAL_MISSION)
		{
			levelStar.clear();
			for (int i = 0; i < CONSTANT::TOTAL_MISSION; i++)
			{
				levelStar.push_back(0);
			}
		}
	}

	{
		std::vector<std::string> result;
		std::vector<int>& levelStar = m_levelStar.at(3);
		Utils::splite(this->getValue(GAMEDATA::LEVEL_STAR_3), ";", result);
		for (auto it : result)
		{
			levelStar.push_back(Utils::str2int(it));
		}
		if (levelStar.size() != CONSTANT::TOTAL_MISSION)
		{
			levelStar.clear();
			for (int i = 0; i < CONSTANT::TOTAL_MISSION; i++)
			{
				levelStar.push_back(0);
			}
		}
	}

	return true;
}

void GameData::setLevelStar(int difficulty, int level, int star)
{
	auto it = m_levelStar.find(difficulty);
	if (it != m_levelStar.end())
	{
		if ((int)it->second.size() >= level && level >= 0)
		{
			it->second[level - 1] = star;
			return;
		}
	}

	DEBUG_LOG("GameData::setLevelStar error: difficulty = %d, level = %d, star = %d", difficulty, level, star);
}

int GameData::getLevelStar(int difficulty, int level)
{
	auto it = m_levelStar.find(difficulty);
	if (it != m_levelStar.end())
	{
		if ((int)it->second.size() >= level && level >= 0)
		{
			return it->second[level - 1];
		}
	}

	DEBUG_LOG("GameData::getLevelStar error: difficulty = %d, level = %d", difficulty, level);
	return 0;
}

bool GameData::saveLevelMax()
{
	char buf[11] = { 0 };
	std::string result;
	for (unsigned i = 0; i < m_levelMax.size(); i++)
	{
		auto it = m_levelMax.find(i + 1);
		if (it != m_levelMax.end())
		{
			snprintf(buf, 10, "%d;", it->second);
		}
		else
		{
			snprintf(buf, 10, "%d;", 1);
		}
		result.append(buf);
	}
	GameData::getInstance()->setValue(GAMEDATA::LEVEL_MAX, result.c_str());

	return true;
}

bool GameData::loadLevelMax()
{
	m_levelMax.clear();

	std::vector<std::string> result;
	Utils::splite(this->getValue(GAMEDATA::LEVEL_MAX), ";", result);
	unsigned i = 0;
	for (; i < MIN(3, result.size()); i++)
	{
		m_levelMax.insert(std::map<int, int>::value_type(i+1, Utils::str2int(result[i])));
	}

	if (m_levelMax.size() < 3)
	{
		for (; i < 3; i++)
		{
			m_levelMax.insert(std::map<int, int>::value_type(i + 1, 1));
		}
	}

	return true;
}

bool GameData::saveLevelFinish()
{
	char buf[11] = { 0 };
	std::string result;
	for (unsigned i = 0; i < m_levelFinish.size(); i++)
	{
		auto it = m_levelFinish.find(i + 1);
		if (it != m_levelFinish.end())
		{
			snprintf(buf, 10, "%d;", it->second);
		}
		else
		{
			snprintf(buf, 10, "%d;", 0);
		}
		result.append(buf);
	}
	GameData::getInstance()->setValue(GAMEDATA::LEVEL_FINISH, result.c_str());

	return true;
}

bool GameData::loadLevelFinish()
{
	m_levelFinish.clear();

	std::vector<std::string> result;
	Utils::splite(this->getValue(GAMEDATA::LEVEL_FINISH), ";", result);
	unsigned i = 0;
	for (; i < MIN(3, result.size()); i++)
	{
		m_levelFinish.insert(std::map<int, int>::value_type(i + 1, Utils::str2int(result[i])));
	}

	if (m_levelFinish.size() < 3)
	{
		for (; i < 3; i++)
		{
			m_levelFinish.insert(std::map<int, int>::value_type(i + 1, 0));
		}
	}

	return true;
}


void GameData::setLevelMax(int level)
{
	int difficulty = GameData::getInstance()->getValueToInt(GAMEDATA::DIFFICULTY_LEVEL);
	auto it = m_levelMax.find(difficulty);
	if (it != m_levelMax.end())
	{
		it->second = level;
	}
	else
	{
		m_levelMax.insert(std::map<int, int>::value_type(difficulty, level));
	}

}

void GameData::setLevelFinish(int level)
{
	int difficulty = GameData::getInstance()->getValueToInt(GAMEDATA::DIFFICULTY_LEVEL);
	auto it = m_levelFinish.find(difficulty);
	if (it != m_levelFinish.end())
	{
		it->second = level;
	}
	else
	{
		m_levelFinish.insert(std::map<int, int>::value_type(difficulty, level));
	}
}

int GameData::getLevelMax(int difficulty)
{
	if (difficulty < 0)
	{
		difficulty = GameData::getInstance()->getValueToInt(GAMEDATA::DIFFICULTY_LEVEL);
	}
	auto it = m_levelMax.find(difficulty);
	if (it != m_levelMax.end())
	{
		return it->second;
	}
	else
	{
		m_levelMax.insert(std::map<int, int>::value_type(difficulty, 1));
		return 1;
	}
}

int GameData::getLevelFinish(int difficulty)
{
	if (difficulty < 0)
	{
		difficulty = GameData::getInstance()->getValueToInt(GAMEDATA::DIFFICULTY_LEVEL);
	}
	auto it = m_levelFinish.find(difficulty);
	if (it != m_levelFinish.end())
	{
		return it->second;
	}
	else
	{
		m_levelFinish.insert(std::map<int, int>::value_type(difficulty, 1));
		return 1;
	}
}

void GameData::addGuide(int type)
{
	m_guideList.insert(type);
}

bool GameData::checkGuide(int type)
{
	auto it = m_guideList.find(type);
	if (it != m_guideList.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool GameData::saveGuide()
{
	char buf[11] = { 0 };
	std::string result;
	for (auto item : m_guideList)
	{
		snprintf(buf, 10, "%d;", item);
		result.append(buf);
	}
	GameData::getInstance()->setValue(GAMEDATA::GUIDE, result.c_str());

	return true;
}

bool GameData::loadGuide()
{
	m_guideList.clear();

	std::vector<std::string> result;
	Utils::splite(this->getValue(GAMEDATA::GUIDE), ";", result);
	for (unsigned i = 0; i < result.size(); i++)
	{
		m_guideList.insert(Utils::str2int(result[i]));
	}

	return true;
}

int GameData::getVipTime()
{
	const int REFERENCE_TIME = 1460215664;
	const int SECOND_IN_DAY = 24 * 60 * 60;

	int vipStartTime = GameData::getInstance()->getValueToInt(GAMEDATA::VIP_START_TIME);
	int vipDays = GameData::getInstance()->getValueToInt(GAMEDATA::VIP_DAYS);
	int now = GameData::getInstance()->getValueToInt(GAMEDATA::TIME);
	int nowLocal = time(NULL);

	if (now < 0)//未联网
	{
		if (vipStartTime + vipDays * SECOND_IN_DAY - nowLocal > 0) //是vip
		{
			return -1;
		}
		else //非vip
		{
			return -2;
		}
	}

	//已联网，但时间错误，认为是非vip
	if (vipDays <= 0 /*|| vipStartTime <= REFERENCE_TIME || now <= REFERENCE_TIME*/)
	{
		return 0;
	}

	int resultTime = vipStartTime + vipDays * SECOND_IN_DAY - now;
	if (resultTime > 0)
	{
		GameData::getInstance()->setValue(GAMEDATA::VIP_FLAG, 1);
		return resultTime;
	}
	else
	{
		GameData::getInstance()->setValue(GAMEDATA::VIP_FLAG, 0);
		return 0;
	}
}

int GameData::addVipTime(int days)
{
	//TODO 必须提前更新下时间

	int time = getVipTime();//vip剩余时间
	int now = GameData::getInstance()->getValueToInt(GAMEDATA::TIME);//当前时间

	//vip已过期
	if (time <= 0)
	{
		GameData::getInstance()->setValue(GAMEDATA::VIP_START_TIME, now);
		GameData::getInstance()->setValue(GAMEDATA::VIP_DAYS, days);
	}
	else //vip未过期
	{
		GameData::getInstance()->incValue(GAMEDATA::VIP_DAYS, days);
	}
	
	return getVipTime();
}

void GameData::incVipFreeRelifeCnt(int level)
{
	auto it = m_vipFreeRelife.find(level);
	if (it != m_vipFreeRelife.end())
	{
		it->second++;
	}
	else
	{
		m_vipFreeRelife.insert(std::map<int, int>::value_type(level, 1));
	}
}

int  GameData::getVipFreeRelifeCnt(int level)
{
	auto it = m_vipFreeRelife.find(level);
	if (it != m_vipFreeRelife.end())
	{
		return it->second;
	}
	else
	{
		return 0;
	}
}

bool GameData::saveVipFreeRelifeCnt()
{
	char buf[21] = { 0 };
	std::string result;
	for (auto it : m_vipFreeRelife)
	{
		snprintf(buf, 20, "%d|%d;", it.first, it.second);
		result.append(buf);
	}
	GameData::getInstance()->setValue(GAMEDATA::VIP_FREE_REFLIE, result.c_str());

	return true;
}

bool GameData::loadVipFreeRelifeCnt()
{
	m_vipFreeRelife.clear();
	std::vector<std::string> result;

	Utils::splite(this->getValue(GAMEDATA::VIP_FREE_REFLIE), ";", result);

	for (auto str : result)
	{
		std::vector<std::string> xx;
		int yy = Utils::splite(str, "|", xx);
		if (yy != 2)
		{
			continue;
		}

		int level = Utils::str2int(xx.at(0));
		int cnt = Utils::str2int(xx.at(1));

		m_vipFreeRelife.insert(std::map<int, int>::value_type(level, cnt));
	}

	return true;
}


int  GameData::getIntegerForKey(const char* pKey, int defaultValue)
{
	return UserDefault::getInstance()->getIntegerForKey(pKey, 0);
}

float GameData::getFloatForKey(const char* pKey, float defaultValue)
{
	return UserDefault::getInstance()->getFloatForKey(pKey, defaultValue);
}

std::string GameData::getStringForKey(const char* pKey, const std::string & defaultValue)
{
	return UserDefault::getInstance()->getStringForKey(pKey, defaultValue);
}

void GameData::setIntegerForKey(const char* pKey, int value)
{
	UserDefault::getInstance()->setIntegerForKey(pKey, value);
}

void GameData::setFloatForKey(const char* pKey, float value)
{
	UserDefault::getInstance()->setFloatForKey(pKey, value);
}

void GameData::setStringForKey(const char* pKey, const std::string & value)
{
	UserDefault::getInstance()->setStringForKey(pKey, value);
}

bool GameData::saveJson(std::string& buf)
{
	rapidjson::Document d;
	d.SetObject();
	for (auto it = m_iMap.begin(); it != m_iMap.end(); it++)
	{
		rapidjson::Value k;
		k.SetString(it->first.c_str(), it->first.length(), d.GetAllocator());
		rapidjson::Value v(it->second);
		d.AddMember(k, v, d.GetAllocator());
	}
	for (auto it = m_fMap.begin(); it != m_fMap.end(); it++)
	{
		rapidjson::Value k;
		k.SetString(it->first.c_str(), it->first.length(), d.GetAllocator());
		rapidjson::Value v(it->second);
		d.AddMember(k, v, d.GetAllocator());
	}
	for (auto it = m_strMap.begin(); it != m_strMap.end(); it++)
	{
		rapidjson::Value k;
		k.SetString(it->first.c_str(), it->first.length(), d.GetAllocator());
		rapidjson::Value v;
		v.SetString(it->second.c_str(), it->second.length(), d.GetAllocator());
		d.AddMember(k, v, d.GetAllocator());
	}
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	d.Accept(writer);
	buf = buffer.GetString();

	return true;
}

bool GameData::loadJson(const char* json)
{
	rapidjson::Document document;
	document.Parse(json);
	for (auto it = m_iMap.begin(); it != m_iMap.end(); it++)
	{
		if (document.HasMember(it->first.c_str()) && document[it->first.c_str()].IsInt())
		{
			it->second = document[it->first.c_str()].GetInt();
		}
		else
		{
			it->second = 0;
		}
	}
	for (auto it = m_fMap.begin(); it != m_fMap.end(); it++)
	{
		if (document.HasMember(it->first.c_str()) && document[it->first.c_str()].IsDouble())
		{
			it->second = document[it->first.c_str()].GetDouble();
		}
		else
		{
			it->second = 0;
		}
	}
	for (auto it = m_strMap.begin(); it != m_strMap.end(); it++)
	{
		if (document.HasMember(it->first.c_str()) && document[it->first.c_str()].IsString())
		{
			it->second = document[it->first.c_str()].GetString();
		}
		else
		{
			it->second = "";
		}
	}

	return true;
}
