#pragma once

#include "cocos2d.h"
#include "IListener.h"

USING_NS_CC;

namespace GAMEDATA
{
	//全局数据
	const char* const SOUND = "sound";
	const char* const FINGER_PRINT_FLAG = "finger_print_flag";//apk签名是否有效
	const char* const FINGER_PRINT = "finger_print";//apk签名是否有效

	//远端数据
	const char* const USER_ID = "user_id";

	//用户当前选择的关卡和飞机
	const char* const PLAYER = "player";
	const char* const MAX_KILL_ENEMY_RATIO = "max_kill_enemy_ratio";  // 杀敌数除以总的敌机数
	const char* const MAX_GET_REWARD_RATIO = "max_get_reward_ratio";  // 获取到的奖励品数/总的奖励品数
	const char* const CONTINUOUS_KILL_ENEMY_CNT = "CONTINUOUS_KILL_ENEMY_CNT"; //连续杀敌数

	//游戏模式：1 关卡；2 无尽；3 急速
	const char* const MODE = "mode";

	const char* const LEVEL_STAR_1 = "level_star_1";  //难度1关卡评级： 0/1/2/3
	const char* const LEVEL_STAR_2 = "level_star_2";  //难度2关卡评级： 0/1/2/3
	const char* const LEVEL_STAR_3 = "level_star_3";  //难度3关卡评级： 0/1/2/3

	const char* const LEVEL = "level";               // 当前正在玩第几关
	const char* const LEVEL_MAX = "level_max";       // 最高玩到第几关
	const char* const LEVEL_FINISH = "level_finish"; // 已通过第几关

	const char* const RAPID_MODE_MAX_DISTANCE = "rapid_mode_max_distance";//急速模式最大飞行距离

	const char* const DIFFICULTY_LEVEL = "difficulty_level";     //闯关模式难度：1、2、3  容易->难
	const char* const DIFFICULTY_ENDLESS = "difficulty_endless"; //无尽模式级别：101开始
	const char* const DIFFICULTY_RAPID = "difficulty_rapid";     //急速模式难度：201开始

	const char* const ENDLESS_ENEMY_HP_RATIO = "endless_enemy_hp_ratio";   //无尽模式敌机hp加成

	//当前关卡数据，无需持久化
	const char* const LEVEL_ENEMY  = "level_enemy"; //杀敌数
	const char* const LEVEL_ENEMY_LEFT = "level_enemy_left"; //剩下的敌机数 = 敌机总数 - 击落的 - 逃跑的
	const char* const LEVEL_SCORE  = "level_score"; //得分
	const char* const LEVEL_MONEY  = "level_money"; //宝石/钱
	const char* const LEVEL_LIFE   = "level_life";  //玩家生命
	const char* const LEVEL_REWARD = "level_reward";//获得的奖励品数量
	const char* const LEVEL_KILLER = "level_killer";//释放必杀的次数

	const char* const LEVEL_ENEMY_TOTAL = "level_enemy_total";  //关卡总的敌机数
	const char* const LEVEL_SCORE_TOTAL = "level_score_total";  //关卡总的分数
	const char* const LEVEL_MONEY_TOTAL  = "level_money_total";  //关卡总的宝石数
	const char* const LEVEL_REWARD_TOTAL = "level_reward_total"; //关卡总的奖励品数量

	//无尽模式
	const char* const ENDLESS_BOSS       = "endless_boss";      //本次敌机波数
	const char* const ENDLESS_BOSS_LAST  = "endless_boss_last"; //上次敌机波数

	//急速模式
	const char* const RAPID_DISTANCE      = "rapid_distance";     //本次距离
	const char* const RAPID_DISTANCE_BEST = "rapid_distance_best";//历史最好距离
	const char* const RAPID_RANK          = "rapid_rank";	      //本次排名
	const char* const RAPID_RANK_BEST     = "rapid_rank_best";	  //历史最好排名

	//关卡累计数据
	const char* const ENEMY = "enemy"; //累计消灭的敌机总数
	const char* const SCORE = "score"; //得分
	const char* const MONEY = "money"; //宝石/钱
	const char* const LIFE  = "life";  //玩家生命

	//玩家行为。0表示没有此行为，1表示执行过此行为
	const char* const DIE = "die";                        // 死亡
	const char* const REVIVE = "revive";                  // 复活
	const char* const USE_KILLER = "use_killer";          // 使用必杀
	const char* const USE_SHIELD = "use_shield";          // 使用护盾
	const char* const USE_MAX_BULLLET = "use_max_bullet"; // 暴走

	const char* const NEW_GIFT = "new_gift";          // 购买过新手礼包
	const char* const MID_GIFT = "mid_gift";          // 购买过超值礼包
	const char* const BIG_GIFT = "big_gift";          // 购买过豪华礼包
	const char* const HUG_GIFT = "hug_gift";          // 购买过超土豪礼包
	const char* const NEW_GIFT_RECOMMEND = "new_gift_recommend"; //推荐过新手礼包
	const char* const MID_GIFT_RECOMMEND = "mid_gift_recommend"; //推荐过超值礼包
	const char* const BIG_GIFT_RECOMMEND = "big_gift_recommend"; //推荐过豪华礼包
	const char* const HUG_GIFT_RECOMMEND = "hug_gift_recommend"; //推荐过超土豪礼包

	const char* const REINFORCE_FIRST     = "reinforce_first";     // 第一次使用强化
	const char* const REINFORCE_ALL       = "reinforce_all";       // TODO 所有强化满级

	//强化（全局）级别
	const char* const REINFORCE_GLOBAL_DIAMOND = "reinforce_diamond";   // 宝石转化
	const char* const REINFORCE_GLOBAL_RAMPAGE = "reinforce_rampage";   // 战机暴走（加农神炮）
	const char* const REINFORCE_GLOBAL_KILLER  = "reinforce_killer";    // 必杀僚机
	const char* const REINFORCE_GLOBAL_SHIELD  = "reinforce_shield";    // 量子护盾
	const char* const REINFORCE_GLOBAL_REPAIR  = "reinforce_repair";    // 装甲修复

	const char* const REINFORCE_VALUE_DIAMOND_RATIO = "reinforce_value_diamond_ratio";  // 宝石转化提升比例 float
	const char* const REINFORCE_VALUE_RAMPAGE_DURA = "reinforce_value_rampage_dura";    // 战机暴走时间增加 float
	const char* const REINFORCE_VALUE_KILLER_COLD = "reinforce_value_killer_cold";    // 超级必杀冷却时间减少 float
	const char* const REINFORCE_VALUE_SHIELD_INIT   = "reinforce_value_shield_init";    // 初始护盾个数增加 float
	const char* const REINFORCE_VALUE_REPAIR_RATIO  = "reinforce_value_repair_ratio";   // 装甲修复比例 float

	//强化（装备）级别。结构：id1,own1,equip1,grade1;
	const char* const REINFORCE_EQUIP = "reinforce_equip";  // 战机/护甲/导弹/僚机

	//结构：equip1,equip2,equip3,aricraft1;
	const char* const PLAYER_EQUIP = "player_equip";        //不同的战机所装备的武器

	const char* const EQUIP_AIRCRAFT = "equip_aircraft";    // 当前装备的战机id
	const char* const EQUIP_ARMATURE = "equip_armature";    // 当前装备的护甲id
	const char* const EQUIP_MISSILE  = "equip_missile";     // 当前装备的导弹id
	const char* const EQUIP_WINGMAN  = "equip_wingman";     // 当前装备的僚机id

	//玩家背包
	const char* const KILLER  = "killer";  //超级必杀数量
	const char* const SHIELD  = "shield";  //子弹晶化数量
	const char* const SKILL_CD_TIME = "skill_cd_time"; //必杀的冷却时间

	//成就
	const char* const ACHIEVEMENT = "achievement";//成就是否达成、是否领取奖励明细

	const char* const USE_AIRCRAFT_4 = "use_aircraft_4";//成就是否达成
	const char* const USE_AIRCRAFT_5 = "use_aircraft_5";//成就是否达成
	const char* const USE_AIRCRAFT_6 = "use_aircraft_6";//成就是否达成

	//每日奖励
	const char* const DAY_REWARD_LOGIN_DAYS = "day_reward_login_days"; //连续登陆的天数(>=0)
	const char* const DAY_REWARD_LAST_LOGIN = "day_reward_last_login"; //上一次登陆的日期(1970年到当天0点0分0秒的秒数)

	//VIP
	const char* const VIP_START_TIME = "vip_start_time"; //vip起始时间（精确到秒）
	const char* const VIP_DAYS = "vip_days";             //持续天数
	const char* const VIP_FLAG = "vip_flag";             //vip标志。用于在游戏中快速判断是否是vip

	const char* const VIP_EXPIRE_REMIND = "vip_expire_remind";//vip即将到期提醒，每次启动时都提醒，点击后关闭提醒
	const char* const VIP_ORDER_REMIND = "vip_order_remind";  //vip开通提醒，仅提醒一次，点击后不再提醒

	const char* const VIP_FREE_REFLIE = "vip_free_relife";    //vip每一关的免费复活次数
	const char* const VIP_RELIFE_ATTR_ADJUST = "vip_relife_attr_adjust"; //vip复活后，战机属性增强

	//新手引导
	const char* const GUIDE = "guide";

	//游戏内临时变量
	const char* const TIME = "time"; //从远程获取的当前时间

	//购买了复活
	const char* const BUY_RELIFE = "buy_relife";
};

enum EGameDataType
{
	DataType_Int = 0,
	DataType_Float = 1,
	DataType_String = 2,
};

struct TGameData
{
	std::string key;
	EGameDataType type;
	int iDefaultValue;
	float fDefaultValue;
	std::string strDefaultValue;

	TGameData(const std::string& k, int v) :key(k), type(EGameDataType::DataType_Int), iDefaultValue(v){}
	TGameData(const std::string& k, float v) :key(k), type(EGameDataType::DataType_Float), fDefaultValue(v){}
	TGameData(const std::string& k, std::string v) :key(k), type(EGameDataType::DataType_String), strDefaultValue(v){}
};

typedef std::vector<IListener*> ListenerArray;
typedef std::vector<IListener*>* ListenerArrayPtr;
typedef std::map<std::string, ListenerArrayPtr>  ListenerMap;


class GameData
{
public:
	static GameData* getInstance()
	{
		if (m_pInstance == NULL)
		{
			m_pInstance = new GameData();
		}

		return m_pInstance;
	}

	void reset();
	bool loadData();
	bool saveData();

	int getValueToInt(const char* const key, int defaultVallue = 0);
	const std::string& getValue(const char* const key, const std::string& defaultVallue = "");
	float getValueToFloat(const char* const key, float defaultVallue = 0);

	int incValue(const char* const key, int value);
	float incValue(const char* const key, float value);

	void setValue(const char* const key, int value);
	void setValue(const char* const key, float value);
	void setValue(const char* const key, const std::string& value);

	//标识成就是否达成，用户是否已领取奖励
	void setAchieveData(int id, int isAchieve, int isReceive);

	//过关评级
	void setLevelStar(int difficulty, int level, int star);
	int getLevelStar(int difficulty, int level);

	void setLevelMax(int level);
	void setLevelFinish(int level);

	int getLevelMax(int difficulty = -1);
	int getLevelFinish(int difficulty = -1);

	//新手引导
	void addGuide(int type);
	bool checkGuide(int type);

	//vip: 获取vip剩余的时间（秒）
	int getVipTime();
	int addVipTime(int days);

	//vip免费复活
	void incVipFreeRelifeCnt(int level);
	int  getVipFreeRelifeCnt(int level);

	//设置监听者
	void regListener(IListener* listener, const std::string& type);
	void unRegListener(IListener* listener, const std::string& type);
	void notifyListener(const std::string& type, int value);

protected:
	GameData();
	virtual ~GameData();

protected:
	bool saveJson(std::string& buf);
	bool loadJson(const char* json);

	bool saveAchievement();
	bool loadAchievement();

	bool saveReinforceEquip();
	bool loadReinforceEquip();

	bool savePlayerEquip();
	bool loadPlayerEquip();

	bool saveLevelStar();
	bool loadLevelStar();

	bool saveLevelMax();
	bool loadLevelMax();

	bool saveLevelFinish();
	bool loadLevelFinish();

	bool saveGuide();
	bool loadGuide();

	bool saveFile();
	bool loadFile();

	bool saveVipFreeRelifeCnt();
	bool loadVipFreeRelifeCnt();

	//
	int     getIntegerForKey(const char* pKey, int defaultValue = 0);
	float    getFloatForKey(const char* pKey, float defaultValue = 0.f);
	std::string getStringForKey(const char* pKey, const std::string & defaultValue = "");

	void    setIntegerForKey(const char* pKey, int value);
	void    setFloatForKey(const char* pKey, float value);
	void    setStringForKey(const char* pKey, const std::string & value);

private:
	static GameData* m_pInstance;

	FILE* m_pFile;

	std::map<std::string, int> m_iMap;
	std::map<std::string, float> m_fMap;
	std::map<std::string, std::string> m_strMap;

	std::vector<TGameData> m_gameDataDefine;

	std::map<int, std::vector<int>> m_levelStar;
	std::map<int, int> m_levelMax;
	std::map<int, int> m_levelFinish;

	std::set<int> m_guideList;

	std::map<int, int> m_vipFreeRelife;

	//监听器
	ListenerMap m_listenerMap;
};