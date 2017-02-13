#pragma once

#include "cocos2d.h"
#include "Common/DbHelper.h"

USING_NS_CC;

struct TAircraftData;
struct TAircraftLevelData;
struct TBossData;
struct TBulletData;
struct TTrackData;
struct TRewardData;
struct TLevelData;
struct TArmatureData;
struct TMusicData;
struct TAchievementData;
struct THpBarData;
struct TRewardGroupData;
struct TReinforceData;
struct TReinforceGradeData;
struct TGoodsData;
struct TDialogData;
struct TDayGiftData;
struct TDifficultyData;
struct TEquipmentData;
struct TLevelBackground;

class GlobalDataLoadListener
{
public:
	virtual void onGlobalDataProgress(int percent) = 0;
};

struct TAnimationData
{
	int id;
	float dura;
	std::vector<std::string> filenames;
};

//相关的常量
namespace GLOBALDATA
{
	const char* const DEBUG = "debug";
	const char* const LOG = "log";
	const char* const DISPLAY_BODY = "display_body";
	const char* const IGNORE_MENU = "ignore_menu";
	const char* const ONLY_BOSS = "only_boss";
	const char* const SOUND = "sound";
	const char* const LEVEL_ID = "level_id";
	const char* const ENEMY_SEQ = "enemy_seq";
	const char* const TUTORIAL_TEST = "tutorial_test";
	const char* const DAY_GIFT_TEST = "day_gift_test";
	const char* const PRICE_FLAG = "price_flag";

	const char* const LIFE = "life";
	const char* const KILLER = "killer";
	const char* const SHIELD = "shield";
	const char* const REPAIR = "repair";
	const char* const PLAYER = "player";
	const char* const CD_TIME = "cd_time";

	const char* const ENABLE_VIP = "enable_vip";//启用vip
	const char* const DAILY_GIFT = "daily_gift";//启用每日登陆奖励

	const char* const STRING_RECEIVED_REWARD = "received_reward"; //领取成就奖励时的提示
	const char* const STRING_DAY_GIFT_PROMPT = "day_gift_prompt";

	const char* const STRING_UPGRADE_WEAPON_MAX_LEVEL = "upgrade_weapon_max_level";
	const char* const STRING_UPGRADE_WEAPON_NO_MONEY = "upgrade_weapon_no_money";
	const char* const STRING_UNLOCK_PLANE_LEVEL_1 = "unlock_plane_level_1";
	const char* const STRING_UNLOCK_PLANE_LEVEL_2 = "unlock_plane_level_2";
	const char* const STRING_UNLOCK_PLANE_LEVEL_3 = "unlock_plane_level_3";

	const char* const STRING_TUTORIAL_PLANE = "tutorial_plane";
	const char* const STRING_TUTORIAL_SHIELD = "tutorial_shield";
	const char* const STRING_TUTORIAL_KILLER = "tutorial_killer";
	const char* const STRING_TUTORIAL_REINFORCE = "tutorial_reinforce";
	const char* const STRING_TUTORIAL_ACHIEVEMENT_1 = "tutorial_achievement_1";
	const char* const STRING_TUTORIAL_ACHIEVEMENT_2 = "tutorial_achievement_2";
	const char* const STRING_TUTORIAL_WEAPON = "tutorial_get_weapon";
}

struct TGlobalData
{
	int debug;
	int log;
	int ignore_menu;
	int display_body;
	int only_boss;
	int sound;
	int level_id;
	int enemy_seq;
	int tutorial_test;
	int day_gift_test;

	int life;
	int killer;
	int shield;
	int repair;
	int player;
	int cd_time;

	void reset()
	{
		debug = 0;
		log = 0;
		ignore_menu = 0;
		display_body = 0;
		only_boss = 0;
		sound = 0;
		level_id = 0;
		enemy_seq = 0;
		tutorial_test = 0;
		day_gift_test = 0;

		life = 0;
		killer = 0;
		shield = 0;
		repair = 0;
		player = 0;
		cd_time = 0;
	}

	TGlobalData()
	{
		reset();
	}

	TGlobalData(const TGlobalData& copy)
	{
		clone(copy);
	}

	inline void clone(const TGlobalData& data)
	{
		debug = data.debug;
		log = data.log;
		ignore_menu = data.ignore_menu;
		display_body = data.display_body;
		only_boss = data.only_boss;
		sound = data.sound;
		level_id = data.level_id;
		enemy_seq = data.enemy_seq;
		tutorial_test = data.tutorial_test;
		day_gift_test = data.day_gift_test;

		life = data.life;
		killer = data.killer;
		shield = data.shield;
		repair = data.repair;
		player = data.player;
		cd_time = data.cd_time;
	}
};

class GlobalData
{
public:
	static GlobalData* getInstance()
	{
		if (m_pInstance == NULL)
		{
			m_pInstance = new GlobalData();
		}

		return m_pInstance;
	}

	void destory();

	bool initialize(GlobalDataLoadListener* pLisiener = nullptr); //初始化基础配置参数

	inline bool isDebug()     { return m_GlobalData.debug == 1 ? true : false; }
	inline bool isLog()       { return m_GlobalData.log == 1 ? true : false; }
	inline bool isDisplayBody() { return m_GlobalData.display_body == 1 ? true : false; }
	inline bool isIgnoreMenu(){ return m_GlobalData.ignore_menu == 1 ? true : false; }
	inline bool isOnlyBoss()  { return m_GlobalData.only_boss == 1 ? true : false; }
	inline int isSound()  { return m_GlobalData.sound; }
	inline int getTestLevelId()	  { return m_GlobalData.level_id; }
	inline int getTestEnemySeq()  { return m_GlobalData.enemy_seq; }
	inline int isTutorialTest()	  { return m_GlobalData.tutorial_test == 1 ? true : false; }
	inline int dayGiftTestDay()	  { return m_GlobalData.day_gift_test; }

	inline int getLife() { return m_GlobalData.life; }
	inline int getKiller() { return m_GlobalData.killer; }
	inline int getShield() { return m_GlobalData.shield; }
	inline int getRepair() { return m_GlobalData.repair; }
	inline int getPlayerId() { return m_GlobalData.player; }
	inline int getCdTime() { return m_GlobalData.cd_time; }

	int getVersionCode();
	std::string getVersionName();

	int getKillerId();
	int getSuperKillerId();

	const std::vector<TAircraftData*>& getAllPlayerData() const { return m_PlayerArray; }
	const TArmatureData* getArmatureData(int id);
	const TArmatureData* getArmatureData(const std::string& name);
	const std::vector<TArmatureData*>& getAllArmatureData() const { return m_ArmatureArray; }
	const TAircraftData* getPlayerData(int id);
	const TAircraftData* getEnemyData(int id);
	const TAircraftLevelData* getAircraftLevelData(int id){ return m_aircraftLevelMap.find(id) == m_aircraftLevelMap.end() ? nullptr : m_aircraftLevelMap.at(id); }
	const TAircraftLevelData* getAircraftLevelData(int aircraftId, int grade);
	const TBossData* getBossData(int id);
	const TAircraftData* getWingmanData(int id);
	const TBulletData* getBulletData(int id);
	const std::vector<std::vector<int>*>* getBulletGradeData(int id);
	const TTrackData* getTrackData(int id);
	const TRewardData* getRewardData(int id);
	const TRewardData* getRewardData(const char* const rewardType);
	const TLevelData* getLevelData(int id);
	const TMusicData* getMusic(int id);
	const TGoodsData* getGoods(const std::string& goods) { return m_GoodsMap.at(goods); }
	const TDialogData* getDialog(int id){ return m_dialogMap.find(id) == m_dialogMap.end() ? nullptr : m_dialogMap.at(id); }
	const TDifficultyData* getDifficulty(int id){ return m_difficultyMap.find(id) == m_difficultyMap.end() ? nullptr : m_difficultyMap.at(id); }
	const TDifficultyData* getDifficulty();
	const std::map<int, std::vector<TEquipmentData*>*>& getAllEquipment(){ return m_equipmentMap; }
	const std::map<int, TEquipmentData*>& getAllEquipmentId(){ return m_equipmentIdMap; }
	const std::vector<TEquipmentData*>& getEquipmentArray(int type){ return *m_equipmentMap.at(type); }
	TEquipmentData* getEquipment(int id){ return m_equipmentIdMap.at(id); }
	const std::vector<TDayGiftData*>& getDayGiftArray(){ return m_dayGiftArray; }
	const std::vector<TReinforceData*>& getAllReinforce() { return m_ReinforceArray; }
	const std::vector<TReinforceGradeData*>& getReinforce(int reinforceId);
	const std::vector<TMusicData*>& getAllMusicData() const { return m_MusicArray; }
	const std::map<int, TAchievementData*>& getAllAchievementData() const { return m_AchievementMap; }
	const std::string& getParameter(const std::string& key, const std::string& defaultValue = "");
	int getParameterToInt(const std::string& key, const std::string& defaultValue = "");
	float getParameterToFloat(const std::string& key, const std::string& defaultValue = "");
	const std::vector<TLevelBackground*>* getRandomBackground();
	const std::vector<std::string>& getTips(){ return m_tipsArray; }
	
protected:
	GlobalData();

	virtual ~GlobalData();

	const std::string& getValue(const DbKVPtr pKv, const std::string& key);
	const int getValueToInt(const DbKVPtr pKv, const std::string& key);
	const float getValueToFloat(const DbKVPtr pKv, const std::string& key);

	int getParameterValue(std::map<std::string, std::string>& map, const char* const pKey, int defaultValue);

	bool initGlobal();

	bool initArmature();//初始化骨骼动画
	bool initPlayer();  // 初始化玩家飞机
	bool initEnemy();   // 初始化敌机
	bool initAircraftLevel();//初始化飞机级别
	bool initWingman(); // 初始化僚机
	bool initBoss();    // 初始化BOSS
	bool initBullet();  // 初始化子弹
	bool initBulletGrade();
	bool initTrack();   // 初始化轨迹
	bool initReward();  // 初始化奖励品
	bool initLevel();   // 初始化关卡
	bool initMusic();   // 初始化音乐
	bool initAchievement();   // 初始化成就奖励数据
	bool initHpBar();   //初始化血槽数据
	bool initReinforce();   //初始化血槽数据
	bool initGoods();   //初始化物品道具
	bool initDialog();  //初始化对话
	bool initDayGift(); //初始化每日签到奖励
	bool initDifficulty(); //初始化难度等级
	bool initEquipment(); //初始化装备

	//初始化飞机数据
	bool initAircraft(const char* pType, std::vector<TAircraftData*>* pAircraft);
	//动画相关
	int getAnimation(int id, float& duration, std::vector<std::string>& filenames);
	bool initAnimation();

private:
	static GlobalData* m_pInstance;

	TGlobalData                 m_GlobalData;
	std::vector<TArmatureData*> m_ArmatureArray;
	std::vector<TAircraftData*> m_PlayerArray;
	std::vector<TAircraftData*> m_EnemyArray;
	std::vector<TAircraftData*> m_WingmanArray;
	std::vector<TBossData*>     m_BossArray;
	std::vector<TBulletData*>   m_BulletArray;
	std::vector<TTrackData*>    m_TrackArray;
	std::vector<TRewardData*>   m_RewardArray;
	std::vector<TLevelData*>    m_LevelArray;
	std::vector<TMusicData*>    m_MusicArray;
	std::vector<TReinforceData*> m_ReinforceArray;
	std::vector<TDayGiftData*>  m_dayGiftArray;
	std::vector<std::string>    m_tipsArray;

	std::map<int, TAircraftLevelData*> m_aircraftLevelMap;
	std::map<int, std::map<int, TAircraftLevelData*>*> m_aircraftLevelIdGradeMap;
	std::map<std::string, std::string> m_ParameterMap;
	std::map<int, TAchievementData*> m_AchievementMap;
	std::map<int, std::vector<std::vector<int>*>*> m_BulletGradeMap;
	std::map<int, TAnimationData*> m_AnimationMap;
	std::map<int, THpBarData*> m_HpBarMap;
	std::map<int, std::vector<TRewardGroupData*>*> m_RewardGroupMap;
	std::map<std::string, TGoodsData*> m_GoodsMap;
	std::map<int, TDialogData*> m_dialogMap;
	std::map<int, TDifficultyData*> m_difficultyMap;
	std::map<int, std::vector<TEquipmentData*>*> m_equipmentMap;
	std::map<int, TEquipmentData*> m_equipmentIdMap;
	std::map<int, std::vector<TLevelBackground*>*> m_levelBgMap;
};
